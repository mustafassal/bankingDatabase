#include "transactions.h"

Transactions::Transactions(int transactionID, string description, string date, Account* account) :
	transactionID(transactionID), description(description), date(date), account(account) {}

const int Transactions::getTransactionID() { return transactionID; }
const string Transactions::getDescription() { return description; }
const string Transactions::getDate() { return date; }
const int Transactions::getAccountNumber() { return account->getAccountNumber(); }

const void Transactions::viewTransaction()
{
	cout << "TRANSACTION ID: " << transactionID << endl;
	cout << "DESCRIPTION: " << description << endl;
	cout << "DATE: " << date << '\n' << endl;
}