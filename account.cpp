#include "account.h"

using namespace std;

Account::Account(int accountNumber, string accountType, double startingDeposit, Customer* accountHolder) :
	accountNumber(accountNumber), accountType(accountType), balance(startingDeposit), accountHolder(accountHolder) {}

const int Account::getAccountNumber() { return accountNumber; }
const string Account::getAccountType() { return accountType; }
const double Account::getBalance() { return balance; }
Customer* Account::getAccountHolder() { return accountHolder; }

void Account::deposit(double& amount) 
{	
	balance += amount; 
	cout << "SUCCESSFULLY DEPOSITED $" << setprecision(2) << fixed << amount << endl;
	cout << "NEW ACCOUNT BALANCE: $" << setprecision(2) << fixed << balance << endl;
}

void Account::withdraw(double& amount) 
{ 
	balance -= amount; 
	cout << "SUCCESSFULLY WITHDREW $" << setprecision(2) << fixed << amount << endl;
	cout << "NEW ACCOUNT BALANCE: $" << setprecision(2) << fixed << balance << endl;
}
