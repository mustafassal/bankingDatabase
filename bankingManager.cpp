#include "bankingManager.h"

BankingManager::BankingManager() {}

void BankingManager::addCustomer(const int& customerID, const string& firstName, const string& lastName, const string& phoneNumber, const string& emailAddress, string dateOfBirth)
{
	Customer* newCustomer = new Customer(customerID, firstName, lastName, phoneNumber, emailAddress, dateOfBirth);
	customers[customerID] = newCustomer;
	NEXT_AVAILABLE_CUSTOMER_ID++;
}
void BankingManager::addAccount(const int& accountNumber, const string& accountType, const double& startingDeposit, Customer* accountHolder)
{
	Account* newAccount = new Account(accountNumber, accountType, startingDeposit, accountHolder);
	accounts[accountNumber] = newAccount;
	NEXT_AVAILABLE_ACCOUNT_NUM++;
}
void BankingManager::addTransaction(const int& transactionID, const string& description, string date, Account* account)
{
	Transactions* newTransaction = new Transactions(transactionID, description, date, account);
	transactions[transactionID] = newTransaction;
}

void BankingManager::loadCustomer(const int& customerID, const string& firstName, const string& lastName, const string& phoneNumber, const string& emailAddress, string dateOfBirth)
{
	string custId = to_string(customerID);
	string str = "INSERT INTO customer(customer_id,first_name,last_name,phone_number,email_address,date_of_birth) VALUES ("
		+ custId + ",'" + firstName + "','" + lastName + "','" + phoneNumber + "','" + emailAddress + "','" + dateOfBirth + "');";

	const char* query = str.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
		addCustomer(customerID, firstName, lastName, phoneNumber, emailAddress, dateOfBirth);
	else
		cout << "FAILED TO ADD CUSTOMER" << endl;
}
void BankingManager::loadAccount(const int& accountNumber, const string& accountType, const double& startingDeposit, Customer* accountHolder)
{
	string accNum = to_string(accountNumber);
	string startDep = to_string(startingDeposit);
	string custID = to_string(accountHolder->getCustomerID());
	string str = "INSERT INTO account(account_number,account_type,balance,customer_id) VALUES ("
		+ accNum + ",'" + accountType + "'," + startDep + "," + custID + ");";

	const char* query = str.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
		addAccount(accountNumber, accountType, startingDeposit, accountHolder);
	else
		cout << "FAILED TO ADD ACCOUNT" << endl;
}

void BankingManager::openAccountForExistingCustomer()
{
	int accountNumber = getValidAccountNumber();
	Customer* customer = getAccountHolder(accountNumber);
	string accountType = getDesiredAccountType();
	double startingDeposit = getDesiredStartingDeposit();

	loadAccount(NEXT_AVAILABLE_ACCOUNT_NUM, accountType, startingDeposit, customer);

	for (int i = 0; i < accountType.length(); i++)
		accountType[i] = toupper(accountType[i]);

	cout << "SUCCESSFULLY CREATED NEW " << accountType << " ACCOUNT" << endl;
	promptMainMenuReturnOption(*this);
}
void BankingManager::openAccountForNewCustomer()
{
	vector<string> customerInformation = getNewCustomerInformation();
	loadCustomer(NEXT_AVAILABLE_CUSTOMER_ID, customerInformation[0], customerInformation[1], customerInformation[2], customerInformation[3], customerInformation[4]);

	Customer* currentCustomer = getCustomer(NEXT_AVAILABLE_CUSTOMER_ID - 1);
	string accountType = getDesiredAccountType();
	double startingDeposit = getDesiredStartingDeposit();

	loadAccount(NEXT_AVAILABLE_ACCOUNT_NUM, accountType, startingDeposit, currentCustomer);

	for (int i = 0; i < accountType.length(); i++)
		accountType[i] = toupper(accountType[i]);

	cout << "SUCCESSFULLY CREATED NEW " << accountType << " ACCOUNT" << endl;
	promptMainMenuReturnOption(*this);
}

void BankingManager::makeDeposit()
{
	int accountNumber = getValidAccountNumber();
	double amountToDeposit = getDesiredDepositAmount();
	
	string accNum = to_string(accountNumber);
	string newBal = to_string(getAccount(accountNumber)->getBalance() + amountToDeposit);

	string q = "UPDATE account SET balance = " + newBal + " WHERE account_number = " + accNum + ";";
	const char* query = q.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
		getAccount(accountNumber)->deposit(amountToDeposit);
	else
		cout << "FAILED TO MAKE DEPOSIT";

	promptMainMenuReturnOption(*this);
}
void BankingManager::makeWithdrawl()
{
	int accountNumber = getValidAccountNumber();
	double amountToWithdraw = getDesiredWithdrawlAmount();

	string accNum = to_string(accountNumber);
	string newBal = to_string(getAccount(accountNumber)->getBalance() - amountToWithdraw);

	string q = "UPDATE account SET balance = " + newBal + " WHERE account_number = " + accNum + ";";
	const char* query = q.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
		getAccount(accountNumber)->withdraw(amountToWithdraw);
	else
		cout << "FAILED TO MAKE WITHDRAWL";

	promptMainMenuReturnOption(*this);
}
void BankingManager::viewAccountBalance()
{
	int accountNumber = getValidAccountNumber();

	cout << "CURRENT ACCOUNT BALANCE: $" << setprecision(2) << fixed << getAccount(accountNumber)->getBalance() << endl;

	promptMainMenuReturnOption(*this);
}
void BankingManager::changeCustomerInformation()
{
	int accountNumber = getValidAccountNumber();
	int customerID = getAccountHolder(accountNumber)->getCustomerID();
	displayInformation(customerID, accountNumber);

	cout << "\nPLEASE INDICATE WHAT YOU WOULD LIKE TO CHANGE, OR PRESS 9 TO RETURN TO THE MAIN MENU:" << endl;
	int input = getUserInput('C');

	switch (input)
	{
	case 1:
		changeFirstName(customerID);
		break;
	case 2:
		changeLastName(customerID);
		break;
	case 3:
		changePhoneNumber(customerID);
		break;
	case 4:
		changeEmailAddress(customerID);
		break;
	case 5:
		changeDateOfBirth(customerID);
		break;
	case 9:
		start(*this);
		break;
	}

	promptMainMenuReturnOption(*this);
}
void BankingManager::viewAccountTransactions()
{
	int accountNumber = getValidAccountNumber();
	
	auto it = transactions.begin();
	cout << "ACCCOUNT NUMBER " << accountNumber << " TRANSACTIONS: " << endl;
	while (it != transactions.end())
	{
		if (it->second->getAccountNumber() == accountNumber)
			it->second->viewTransaction();
		it++;
	}
	promptMainMenuReturnOption(*this);
}
void BankingManager::closeAccount()
{
	int accountNumber = getValidAccountNumber();
	cout << "ARE YOU SURE YOU WANT TO PERMANENTLY CLOSE YOUR ACCOUNT? (y/n)" << endl;

	char confirmationInput;
	cin >> confirmationInput;
	if (tolower(confirmationInput) == 'y')
	{
		accounts.erase(accountNumber);
		cout << "SUCCESSFULLY CLOSED ACCOUNT NUMBER " << accountNumber << endl;
	}

	promptMainMenuReturnOption(*this);
}

void BankingManager::changeFirstName(const int& customerID)
{
	string newFirstName;
	cout << "PLEASE ENTER A NEW FIRST NAME:" << endl;
	cin >> newFirstName;

	string custID = to_string(customerID);
	string str = "UPDATE customer SET first_name = '" + newFirstName + "' WHERE customer_id = " + custID + ";";
	const char* query = str.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
	{
		getCustomer(customerID)->setFirstName(newFirstName);
		cout << "FIRST NAME SUCCESSFULLY CHANGED" << endl;
	}
	else
		cout << "FAILED TO CHANGE FIRST NAME" << endl;
}
void BankingManager::changeLastName(const int& customerID)
{
	string newLastName;
	cout << "PLEASE ENTER A NEW LAST NAME:" << endl;
	cin >> newLastName;

	string custID = to_string(customerID);
	string str = "UPDATE customer SET last_name = '" + newLastName + "' WHERE customer_id = " + custID + ";";
	const char* query = str.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
	{
		getCustomer(customerID)->setLastName(newLastName);
		cout << "LAST NAME SUCCESSFULLY CHANGED" << endl;
	}
	else
		cout << "FAILED TO CHANGE LAST NAME" << endl;
}
void BankingManager::changePhoneNumber(const int& customerID)
{
	string newPhoneNumber;
	cout << "PLEASE ENTER A NEW PHONE NUMBER:" << endl;
	cin >> newPhoneNumber;
	while (!isValidPhoneNumber(newPhoneNumber))
	{
		cout << "\nNOT A VALID PHONE NUMBER. PLEASE TRY AGAIN.\n" << endl;
		cin >> newPhoneNumber;
	}

	string custID = to_string(customerID);
	string str = "UPDATE customer SET phone_number = '" + newPhoneNumber + "' WHERE customer_id = " + custID + ";";
	const char* query = str.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
	{
		getCustomer(customerID)->setPhoneNumber(newPhoneNumber);
		cout << "PHONE NUMBER SUCCESSFULLY CHANGED" << endl;
	}
	else
		cout << "FAILED TO CHANGE PHONE NUMBER" << endl;
}
void BankingManager::changeEmailAddress(const int& customerID)
{
	string newEmailAddress;
	cout << "PLEASE ENTER A NEW EMAIL ADDRESS:" << endl;
	cin >> newEmailAddress;
	while (!isValidEmail(newEmailAddress))
	{
		cout << "\nNOT A VALID EMAIL ADDRESS. PLEASE TRY AGAIN.\n" << endl;
		cin >> newEmailAddress;
	}

	string custID = to_string(customerID);
	string str = "UPDATE customer SET email_address = '" + newEmailAddress + "' WHERE customer_id = " + custID + ";";
	const char* query = str.c_str();
	int queryState = mysql_query(connector, query);
	if (!queryState)
	{
		getCustomer(customerID)->setEmailAddress(newEmailAddress);
		cout << "EMAIL ADDRESS SUCCESSFULLY CHANGED" << endl;
	}
	else
		cout << "FAILED TO CHANGED EMAIL ADDRESS" << endl;

}
void BankingManager::changeDateOfBirth(const int& customerID)
{
	string newDateOfBirth;
	cout << "PLEASE ENTER YOUR DATE OF BIRTH: (YYYY-MM-DD)" << endl;
	cin >> newDateOfBirth;
	while (!isValidDOB(newDateOfBirth))
	{
		cout << "\nNOT A VALID DATE OF BIRTH. PLEASE TRY AGAIN.\n" << endl;
		cin >> newDateOfBirth;
	}

	string custID = to_string(customerID);
	string str = "UPDATE customer SET date_of_birth = '" + newDateOfBirth + "' WHERE customer_id = " + custID + ";";
	const char* query = str.c_str();
	int queryState = mysql_query(connector, query);

	if (!queryState)
	{
		getCustomer(customerID)->setDateOfBirth(newDateOfBirth);
		cout << "DATE OF BIRTH SUCCESSFULLY CHANGED" << endl;
	}
	else
		cout << "FAILED TO CHANGE DATE OF BIRTH" << endl;
}

Customer* BankingManager::getAccountHolder(const int& accountNumber)
{
	auto it = accounts.find(accountNumber);
	if (it != accounts.end())
		return it->second->getAccountHolder();

	return nullptr;
}
Customer* BankingManager::getCustomer(const int& customerID)
{
	auto it = customers.find(customerID);
	if (it != customers.end())
		return it->second;

	return nullptr;
}
Account* BankingManager::getAccount(const int& accountNumber)
{
	if (accountNumber > 0)
		return accounts[accountNumber];

	return nullptr;
}
const int BankingManager::getValidAccountNumber()
{
	int accountNumberInput;
	cout << "PLEASE ENTER ANY OF YOUR CURRENT ACCOUNT NUMBERS:" << endl;
	cin >> accountNumberInput;

	while (accounts.find(accountNumberInput) == accounts.end())
	{
		cout << "ACCOUNT NOT FOUND. PLEASE MAKE SURE YOU ENTERED YOUR ACCOUNT NUMBER IN CORRECTLY." << endl;
		cin >> accountNumberInput;
	}
	return accountNumberInput;
}
const void BankingManager::displayInformation(const int& customerID, const int& accountNumber)
{
	cout << "CURRENT ACCOUNT INFORMATION:\n" << endl;
	cout << "ACCOUNT HOLDER: " << getCustomer(customerID)->getFirstName() << " " << getCustomer(customerID)->getLastName() << endl;
	cout << "DATE OF BIRTH: " << getCustomer(customerID)->getDateOfBirth() << endl;
	cout << "PHONE NUMBER: " << getCustomer(customerID)->getPhoneNumber() << endl;
	cout << "EMAIL ADDRESS: " << getCustomer(customerID)->getEmailAddress() << endl;
	cout << "ACCOUNT NUMBER: " << accountNumber << endl;
	cout << "ACCOUNT TYPE: " << getAccount(accountNumber)->getAccountType() << endl;
	cout << "CURRENT BALANCE: $" << setprecision(2) << fixed << getAccount(accountNumber)->getBalance() << endl;
}

void loadDatabase() // fix query and loading
{
	BankingManager manager;
	MYSQL_ROW row;
	MYSQL_RES* result;

	const char* query = "SELECT * FROM banking.customer, banking.account, banking.transactions WHERE customer.customer_id = account.customer_id AND account.account_number = transactions.account_number;";
	int queryState = mysql_query(connector, query);

	if (!queryState)
	{
		result = mysql_store_result(connector);
		while (row = mysql_fetch_row(result))
		{
			int customerID = stoi(row[0]);
			string firstName = row[1];
			string lastName = row[2];
			string phoneNumber = row[3];
			string email = row[4];
			string DOB = row[5];

			int accountNumber = stoi(row[7]);
			string accountType = row[8];
			double balance = stod(row[9]);

			int transactionID = stoi(row[11]);
			string description = row[12];
			string date = row[13];

			manager.addCustomer(customerID, firstName, lastName, phoneNumber, email, DOB);
			manager.addAccount(accountNumber, accountType, balance, manager.getCustomer(customerID));
			manager.addTransaction(transactionID, description, date, manager.getAccount(accountNumber));
		}
		start(manager);
	}
	else
		cout << "Query failed: " << mysql_error(connector) << endl;
}
void start(BankingManager& manager)
{
	cout << "~~~~~WELCOME TO ASSAL ONLINE BANKING~~~~~\n" << endl;
	cout << "SELECT ONE OF THE FOLLOWING OPTION: " << endl;
INPUT: int input = getUserInput('S'); // key indicating to show the starting interface
	switch (input)
	{
	case 1:
		if (holdsAnyAccounts())
			manager.openAccountForExistingCustomer();
		else
			manager.openAccountForNewCustomer();
		break;
	case 2:
		manager.makeDeposit();
		break;
	case 3:
		manager.makeWithdrawl();
		break;
	case 4:
		manager.viewAccountBalance();
		break;
	case 5:
		manager.changeCustomerInformation();
		break;
	case 6:
		manager.viewAccountTransactions();
		break;
	case 7:
		manager.closeAccount();
		break;
	default:
		cout << "INVALID INPUT. TRY AGAIN" << endl;
		goto INPUT;
	}
}
bool connectToServer(const char* hostname, const char* username, const char* password, const char* database, const int& port)
{
	connector = mysql_init(0);
	connector = mysql_real_connect(connector, hostname, username, password, database, port, NULL, 0);

	if (connector)
	{
		cout << "SUCCESSFULLY CONNECTED TO SERVER" << endl;
		loadDatabase();
		return true;
	}
	else
	{
		cout << "FAILED TO CONNECT TO DATABASE" << endl;
		return false;
	}
	return false;
}

const double getDesiredStartingDeposit()
{
	cout << "PLEASE INDICATE A STARTING DEPOSIT AMOUNT: (MINIMUM STARTING DEPOIST: $" << setprecision(2) << fixed << MINIMUM_STARTING_DEPOSIT << "):" << endl;
	double startingDeposit;
	cin >> startingDeposit;
	while (startingDeposit < MINIMUM_STARTING_DEPOSIT)
	{
		cout << "MINIMUM STARTING DEPOSIT IS $" << setprecision(2) << fixed << MINIMUM_STARTING_DEPOSIT << ". " << "PLEASE TRY AGAIN." << endl;
		cin >> startingDeposit;
	}
	return startingDeposit;
}
const double getDesiredDepositAmount()
{
	cout << "PLEASE INDICATE HOW MUCH YOU WISH TO DEPOSIT:" << endl;
	double amount;
	cin >> amount;
	return amount;
}
const double getDesiredWithdrawlAmount()
{
	cout << "PLEASE INDICATE HOW MUCH YOU WISH TO WITHDRAW:" << endl;
	double amount;
	cin >> amount;
	return amount;
}
const string getDesiredAccountType()
{
	cout << "PLEASE SELECT AN ACCOUNT TYPE TO OPEN:" << endl;
	int typeSelection = getUserInput('O');
	string accountType;

	if (typeSelection == 1)
		accountType = "Checking";
	else if (typeSelection == 2)
		accountType = "Savings";
	else if (typeSelection == 3)
		accountType = "Investing";

	return accountType;
}
vector<string> getNewCustomerInformation()
{
	string firstName;
	cout << "PLEASE ENTER YOUR FIRST NAME:" << endl;
	cin >> firstName;

	string lastName;
	cout << "PLEASE ENTER YOUR LAST NAME:" << endl;
	cin >> lastName;

	string phoneNumber;
	cout << "PLEASE ENTER YOUR PHONE NUMBER: " << endl;
	cin >> phoneNumber;
	while (!isValidPhoneNumber(phoneNumber))
	{
		cout << "\nNOT A VALID PHONE NUMBER. PLEASE TRY AGAIN.\n" << endl;
		cin >> phoneNumber;
	}

	string emailAddress;
	cout << "PLEASE ENTER YOUR EMAIL ADDRESS: " << endl;
	cin >> emailAddress;
	while (!isValidEmail(emailAddress))
	{
		cout << "\nNOT A VALID EMAIL ADDRESS. PLEASE TRY AGAIN.\n" << endl;
		cin >> emailAddress;
	}

	string dateOfBirth;
	cout << "PLEASE ENTER YOUR DATE OF BIRTH: (YYYY-MM-DD)" << endl;
	cin >> dateOfBirth;
	while (!isValidDOB(dateOfBirth))
	{
		cout << "\nNOT A VALID DATE OF BIRTH. PLEASE TRY AGAIN.\n" << endl;
		cin >> dateOfBirth;
	}

	return { firstName, lastName, phoneNumber, emailAddress, dateOfBirth };
}

bool holdsAnyAccounts()
{
	cout << "DO YOU CURRENTLY HOLD ANY ACCOUNTS? (y/n)" << endl;
	char response;
	cin >> response;

	return response == 'y';
}
void promptMainMenuReturnOption(BankingManager& bdb)
{
	int returnToMainMenu;
	cout << "PRESS 9 IF YOU WISH TO RETURN TO THE MAIN MENU, OR PRESS ANY OTHER KEY TO LEAVE." << endl;
	cin.ignore(10, '\n');
	cin >> returnToMainMenu;

	if (returnToMainMenu == 9)
		start(bdb);
	else
		exit(0);
}
int getUserInput(const char& key) // Value of key tells the function which interface to show
{
	int userInput;
	if (key == 'S') // starting interface
	{
		cout << "1: OPEN ACCOUNT" << endl;
		cout << "2: MAKE A DEPOSIT" << endl;
		cout << "3: MAKE A WITHDRAWL" << endl;
		cout << "4: VIEW ACCOUNT BALANCE" << endl;
		cout << "5: CHANGE ACCOUNT INFORMATION" << endl;
		cout << "6: VIEW ACCOUNT TRANSACTIONS" << endl;
		cout << "7: CLOSE ACCOUNT" << endl;
		cin >> userInput;
		while (userInput > 7 && userInput != 9)
		{
			cout << "INVALID INPUT. PLEASE TRY AGAIN" << endl;
			cin >> userInput;
		}
		return userInput;
	}
	if (key == 'C') // changing account information interface
	{
		cout << "1: FIRST NAME" << endl;
		cout << "2: LAST NAME" << endl;
		cout << "3: PHONE NUMBER" << endl;
		cout << "4: EMAIL ADDRESS" << endl;
		cout << "5: DATE OF BIRTH" << endl;
		cin >> userInput;
		while (userInput > 5 && userInput != 9)
		{
			cout << "INVALID INPUT. PLEASE TRY AGAIN" << endl;
			cin >> userInput;
		}
		return userInput;
	}
	if (key == 'O') // opening an account interface
	{
		cout << "1: CHECKING ACCOUNT" << endl;
		cout << "2: SAVINGS ACCOUNT" << endl;
		cout << "3: INVESTING ACCOUNT" << endl;
		cin >> userInput;
		while (userInput > 3 && userInput != 9)
		{
			cout << "INVALID INPUT. PLEASE TRY AGAIN" << endl;
			cin >> userInput;
		}
		return userInput;
	}
}
bool isValidEmail(const string& emailAddress)
{
	int n = emailAddress.length();
	if (n < 6)
		return false;
	bool hasAtSymbol = false;
	bool hasValidDomain = false;
	for (int i = 0; i < n; i++)
	{
		if (i != 0 && emailAddress[i] == '@' && emailAddress[i + 1] != '.')
			hasAtSymbol = true;
	}
	if (isalpha(emailAddress[n - 1]) && isalpha(emailAddress[n - 2]) && isalpha(emailAddress[n - 3]) && emailAddress[n - 4] == '.')
		hasValidDomain = true;

	return hasAtSymbol && hasValidDomain;
}
bool isValidPhoneNumber(const string& phoneNumber)
{
	if (phoneNumber.length() != 10)
		return false;
	for (char n : phoneNumber)
		if (!isdigit(n))
			return false;
	return true;
}
bool isValidDOB(const string dateOfBirth)
{
	if (dateOfBirth.length() != 10)
		return false;
	if (dateOfBirth[7] != '-' || dateOfBirth[4] != '-')
		return false;

	int year = stoi(dateOfBirth.substr(0, 4));
	int month = stoi(dateOfBirth.substr(5, 2));
	int day = stoi(dateOfBirth.substr(8, 2));

	if (month > 12)
		return false;
	if (day > 31)
		return false;
	if (year < 1900)
		return false;

	if (month == 2 && day > 28)
		return false;
	if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
		return false;

	return true;
}
