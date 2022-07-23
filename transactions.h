#ifndef _TRANSACTIONS_H_
#define _TRANSACTIONS_H_

#include "account.h"
#include <string>

class Transactions
{
public:
	Transactions(int transactionID, string description, string date, Account* account);

	const int getTransactionID();
	const string getDescription();
	const string getDate();
	const int getAccountNumber();


	const void viewTransaction();

private:
	int transactionID;
	string description;
	string date;
	Account* account;
};

#endif // !_TRANSACTIONS_H_
