#ifndef _BANKINGMANAGER_H_
#define _BANKINGMANAGER_H_

#include "account.h"
#include "customer.h"
#include "transactions.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <mysql.h>

const double MINIMUM_STARTING_DEPOSIT = 500.00;
static int NEXT_AVAILABLE_ACCOUNT_NUM = 100000;
static int NEXT_AVAILABLE_CUSTOMER_ID = 502;

static MYSQL* connector;

class BankingManager
{
public:
	BankingManager();
	void addCustomer(const int& customerID, const string& firstName, const string& lastName, const string& phoneNumber, const string& emailAddress, string dateOfBirth);
	void addAccount(const int& accountNumber, const string& accountType, const double& startingDeposit, Customer* accountHolder);
	void addTransaction(const int& transactionID, const string& description, string date, Account* account);

	void loadCustomer(const int& customerID, const string& firstName, const string& lastName, const string& phoneNumber, const string& emailAddress, string dateOfBirth);
	void loadAccount(const int& accountNumber, const string& accountType, const double& startingDeposit, Customer* accountHolder);

	void openAccountForExistingCustomer();
	void openAccountForNewCustomer();

	void makeDeposit();
	void makeWithdrawl();
	void viewAccountBalance();
	void changeCustomerInformation();
	void viewAccountTransactions();
	void closeAccount();

	void changeFirstName(const int& customerID);
	void changeLastName(const int& customerID);
	void changePhoneNumber(const int& customerID);
	void changeEmailAddress(const int& customerID);
	void changeDateOfBirth(const int& customerID);

	Customer* getAccountHolder(const int& accountNumber);
	Customer* getCustomer(const int& customerID);
	Account* getAccount(const int& accountNumber);
	const int getValidAccountNumber();
	const void displayInformation(const int& customerID, const int& accountNumber);

private:
	unordered_map<int, Customer*> customers;
	unordered_map<int, Account*> accounts;
	unordered_map<int, Transactions*> transactions;
};

/////////// NON-MEMBER FUNTIONS ////////////

void loadDatabase();
void start(BankingManager& manager);
bool connectToServer(const char* hostname, const char* username, const char* password, const char* database, const int& port);

const double getDesiredStartingDeposit();
const double getDesiredDepositAmount();
const double getDesiredWithdrawlAmount();
const string getDesiredAccountType();
vector<string> getNewCustomerInformation();

bool holdsAnyAccounts();
void promptMainMenuReturnOption(BankingManager& bdb);
int getUserInput(const char& key);
bool isValidEmail(const string& emailAddress);
bool isValidPhoneNumber(const string& phoneNumber);
bool isValidDOB(const string dateOfBirth);


#endif // !_BANKINGMANAGER_H_
