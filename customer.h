#ifndef _CUSTOMER_H_
#define _CUSTOMER_H_

#include <string>
using namespace std;

class Customer
{
public:
	Customer(int customerID, string firstName, string LastName, string phoneNumber, string emailAddress, string dateOfBirth);

	const string getFirstName();
	const string getLastName();
	const string getPhoneNumber();
	const string getEmailAddress();
	const string getDateOfBirth();
	const int getCustomerID();

	void setFirstName(string& newFirstName);
	void setLastName(string& newLastName);
	void setPhoneNumber(string& newPhoneNumber);
	void setEmailAddress(string& newEmailAddress);
	void setDateOfBirth(string& newDateOfBirth);

private:
	int customerID;
	string firstName;
	string lastName;
	string phoneNumber;
	string emailAddress;
	string dateOfBirth; // Potentially change to ADT "Date"
};

#endif //_CUSTOMER_H_

