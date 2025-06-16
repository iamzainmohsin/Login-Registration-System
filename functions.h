#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//SettingCustomReturnTypes
enum LoginStatus {
    LOGIN_SUCCESS,
    LOGIN_FILE_NOT_FOUND,
    LOGIN_FAILED,
    LOGIN_USER_NOT_FOUND,
    LOGIN_INCORRECT_PASSWORD
};


//HashAlgorithm
string hashPassword(const string &input){

    stringstream ss;
    string symbols = "!@#$%^&*";

    for(int i = 0; i < input.length(); i++){
        char ch = input[i];
        int shift = static_cast <int>(ch) + i*i + 7;
        ss << hex << shift;


        if(i%2 == 0){
            char symbol = symbols[(ch + i) % symbols.length()];
            ss << symbol;
        }

    }

    return ss.str();
}


//Writing To File:
void saveToFile(const string &username, const string &email, const string &pswd){
    //CheckExistence:
    ifstream checkFile("users.csv");
    bool fileExists = checkFile.good();
    checkFile.close();

    //AddsHEader:
    if(!fileExists){
        ofstream createFile("users.csv");
        createFile << "User Name,Email,Password" << endl;
        createFile.close();
    }

    //AddsData:
    ofstream user_file("users.csv", ios::app);

    if(!user_file){
        cout << "Error Opening Your File";
    }
    else{
        user_file << username << "," << email << "," << pswd << endl;
        user_file.close();
    }

}


//Registration:
void registerUser(){
    string username, email, pswd;

    cout << "Enter your User Name: ";
    getline(cin, username);

    cout << "Enter your Email: ";
    getline(cin, email);

    cout << "Enter your Password: ";
    getline(cin, pswd);

    //ChecksAlreadyExisitingUserName:
    ifstream userFile("users.csv");
    string line;
    bool duplicateFound = false;
    while(getline(userFile, line)){
        stringstream ss(line);
        string exisitngUserName, exisitngEmail;

        getline(ss, exisitngUserName, ',');
        getline(ss, exisitngEmail, ',');

        if(exisitngUserName == username || exisitngEmail == email){
            cout << "Username or email already exists" << endl;
            duplicateFound = true;
            break;
        }
    }

    //SavesToFile:
    if(!duplicateFound){
        saveToFile(username, email, hashPassword(pswd));
        cout << "Account sucessfully created." << endl;
    }
}


//Login:
LoginStatus loginUser(){
    
    string username, pswd;
    cout << "Enter your username: ";
    getline(cin, username);

    cout << "Enter your password: ";
    getline(cin, pswd);
    string hashedInputPassword = hashPassword(pswd);

    //ChecksForFileExistence:
    ifstream checkFile("users.csv");
    if(!checkFile.good()){
        return LOGIN_FILE_NOT_FOUND;
    }

    //MatchingCredentials:
    ifstream user_file("users.csv");
    string line;
    bool user_found = false;

    while(getline(user_file, line)){
        stringstream ss(line);
        string matchUser, matchEmail, matchPassword;

        getline(ss, matchUser, ',');
        getline(ss, matchEmail, ',');
        getline(ss, matchPassword, ',');

        if(matchUser == username){
            user_found = true;
            if(matchPassword == hashedInputPassword){
                return LOGIN_SUCCESS;
            }
            else{

                return LOGIN_INCORRECT_PASSWORD;
            
            }
        }
    }

    if(!user_found){
        return LOGIN_USER_NOT_FOUND;
    }



    return LOGIN_INCORRECT_PASSWORD;

}


//MainMenu
void showMenu() {
    cout << "\n************* Welcome *************\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "***********************************\n";
    cout << "> ";
}

#endif