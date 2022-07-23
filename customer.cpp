#include "customer.h"

Customer::Customer(int customerID, string firstName, string lastName, string phoneNumber, string emailAddress, string dateOfBirth) :
	customerID(customerID), firstName(firstName), lastName(lastName), phoneNumber(phoneNumber), emailAddress(emailAddress), dateOfBirth(dateOfBirth) {}

const string Customer::getFirstName() { return firstName; }
const string Customer::getLastName() { return lastName; }
const string Customer::getPhoneNumber() { return phoneNumber; }
const string Customer::getEmailAddress() { return emailAddress; }
const string Customer::getDateOfBirth() { return dateOfBirth; }
const int Customer::getCustomerID() { return customerID; }

void Customer::setFirstName(string& newFirstName) { firstName = newFirstName; }
void Customer::setLastName(string& newLastName) { lastName = newLastName; }
void Customer::setPhoneNumber(string& newPhoneNumber) { phoneNumber = newPhoneNumber; }
void Customer::setEmailAddress(string& newEmailAddress) { emailAddress = newEmailAddress; }
void Customer::setDateOfBirth(string& newDateOfBirth) { dateOfBirth = newDateOfBirth; }