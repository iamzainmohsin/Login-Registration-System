#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

//SettingCustomReturnTypes
enum LoginStatus {
    LOGIN_SUCCESS,
    LOGIN_FILE_NOT_FOUND,
    LOGIN_FAILED,
    LOGIN_USER_NOT_FOUND,
    LOGIN_INCORRECT_PASSWORD
};

std::string hashPassword(const std::string &input);
void saveToFile(const std::string &username, const std::string &email, const std::string &pswd);
void registerUser();
LoginStatus loginUser(std::string &username);
LoginStatus resetPassword(const std::string &username);
void showMenu();



#endif