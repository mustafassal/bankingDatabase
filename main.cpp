#include "bankingManager.h"
#include "account.h"
#include "customer.h"
#include "transactions.h"
#include <iostream>
#include <iomanip>
#include <unordered_map>


int main()
{
	const char* hostname = "localhost";
	const char* username = "root";
	const char* password = "Cheesenugget_69076";
	const char* database = "banking";
	int port = 3306;

	if (connectToServer(hostname, username, password, database, port))
	{
	
	}
	else
		exit(5);
}