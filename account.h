#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include "customer.h"
#include <iostream>
#include <iomanip>

class Account
{
public:
	Account(int accountNumber, string accountType, double startingDeposit, Customer* accountHolder);

	const int getAccountNumber();
	const string getAccountType();
	const double getBalance();
	Customer* getAccountHolder();

	void deposit(double& amount);
	void withdraw(double& amount);


private:
	int accountNumber;
	string accountType;
	double balance;
	Customer* accountHolder;
};
#endif // !_ACCOUNT_H_

