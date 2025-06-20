#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
using namespace std;

const string FILE_NAME = "users.csv";

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

//WritingToFile:
void saveToFile(const string &username, const string &email, const string &pswd){
    //CheckExistence:
    ifstream checkFile(FILE_NAME);
    bool fileExists = checkFile.good();
    checkFile.close();

    //AddsHEader:
    if(!fileExists){
        ofstream createFile(FILE_NAME);
        createFile << "User Name,Email,Password" << endl;
        createFile.close();
    }

    //AddsData:
    ofstream user_file(FILE_NAME, ios::app);

    if(!user_file){
        cout << "\nError Opening Your File";
    }
    else{
        user_file << username << "," << email << "," << pswd << endl;
        user_file.close();
    }

}

//Registration:
void registerUser(){
    string username, email, pswd;

    cout << "\n--- Register New Account ---\n";


    //usernameCheck:
    while (true) {
        cout << "Enter your User Name: ";
        getline(cin, username);
        if (username.find(',') != string::npos) {
            cout << "Commas are not allowed in the username. Please try again." << endl;
        } else {
            break;
        }
    }


    //emailCheck:
    while(true){
        cout << "Enter your Email: ";
        getline(cin, email);
        if (email.find(',') != string::npos){
            cout << "Commas are not allowed in the email. Please try again." << endl;
        }
        else if (email.find('@') != string::npos && email.find('.') != string::npos) {
            break;  
        } 
        else {
            cout << "Invalid email format. Try again." << endl;
        }
    }
    

    //passwordCheck:
    while(true){
        cout << "Enter your Password: ";
        getline(cin, pswd);
        if (pswd.find(',') != string::npos) {
            cout << "Commas are not allowed in the password. Please try again." << endl;
        }
        else if (pswd.length() < 8) {
        cout << "Password needs to be more than 8 characters." << endl;
        }
        else{
            break;
        }
    }
    


    //ChecksAlreadyExisitingUserName:
    ifstream userFile(FILE_NAME);
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
        cout << "\nAccount sucessfully created." << endl;
        cout << "Thanks for creating an account '" << username << "' !" << endl;
    }
}

//Login:
LoginStatus loginUser(string &username){
    
    string pswd;

    cout << "\n--- Login to Your Account ---\n";


    cout << "Enter your username: ";
    getline(cin, username);


    cout << "Enter your password: ";
    getline(cin, pswd);
    string hashedInputPassword = hashPassword(pswd);


    //ChecksForFileExistence:
    ifstream checkFile(FILE_NAME);
    bool fileExists = checkFile.good();
    checkFile.close();


    //MatchingCredentials:
    ifstream user_file(FILE_NAME);
    string line;
    bool user_found = false;
    bool passwordMatched = false;

    while(getline(user_file, line)){
        stringstream ss(line);
        string matchUser, matchEmail, matchPassword;

        getline(ss, matchUser, ',');
        getline(ss, matchEmail, ',');
        getline(ss, matchPassword, ',');

        if(matchUser == username){
            user_found = true;
            if(matchPassword == hashedInputPassword){
                passwordMatched = true;
            }
            break;
        }
    }

    if(!user_found){
        return LOGIN_USER_NOT_FOUND;
    }
    else if (passwordMatched) {
        return LOGIN_SUCCESS;
    } 
    else {
        return LOGIN_INCORRECT_PASSWORD;
    }

}

//ResetPassword:
LoginStatus resetPassword(const string &username){
    
    string pswd, newHashedPassword;

    cout << "\n--- Reset your Password ---\n";

    //ChecksForFileExistence:
    ifstream checkFile(FILE_NAME);
    bool fileExists = checkFile.good();
    checkFile.close();



    //MatchingCredentials:
    ifstream user_file(FILE_NAME);
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
            break;
        }
    }
    user_file.close();

    if(!user_found){
        return LOGIN_USER_NOT_FOUND;
    }

    //NewPassword
    while(true){
        cout << "Enter your Password: ";
        getline(cin, pswd);
        if (pswd.find(',') != string::npos) {
            cout << "Commas are not allowed in the password. Please try again." << endl;
        }
        else if (pswd.length() < 8) {
        cout << "Password needs to be more than 8 characters." << endl;
        }
        else{
            newHashedPassword = hashPassword(pswd);
            break;
        }
    }

    //SettingTemporaryFile&Updating:
    ifstream user_file_read(FILE_NAME);
    ofstream temp_file("temp.csv");

    while(getline(user_file_read, line)){
        stringstream ss(line);
        string matchUser, matchEmail, matchPassword;

        getline(ss, matchUser, ',');
        getline(ss, matchEmail, ',');
        getline(ss, matchPassword, ',');

        if(matchUser == username){
            temp_file << matchUser << "," << matchEmail << "," << newHashedPassword << endl;
        } else {
            temp_file << line << endl;
        }
    }

    user_file_read.close();
    temp_file.close();


    remove(FILE_NAME.c_str());
    rename("temp.csv", FILE_NAME.c_str());

    return LOGIN_SUCCESS;

}

//MainMenu
void showMenu() {
    cout << "----------------------------------\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "----------------------------------\n";
    cout << "> ";

    //PreventsInvalidInput
    while (cin.fail()) {
    cin.clear();
    cin.ignore(10000, '\n');
}
}
