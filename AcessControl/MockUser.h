#ifndef USER_H
#define USER_H

int createUserProfile(char* name, char* numeroTel, char* birthdate, long int CPF, char* password);
int alterPassword(long int CPF, char* newPass);
int alterNumber(long int CPF, char* newNumber);

#endif
