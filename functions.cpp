#include "functions.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

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


    cout << "\n--- Register New Account ---\n";


    cout << "Enter your User Name: ";
    getline(cin, username);

    //emailCheck:
    while(true){
        cout << "Enter your Email: ";
        getline(cin, email);
        if (email.find('@') != string::npos && email.find('.') != string::npos) {
            break;  
        } 
        // else if (email.find(',') != string::npos){
        //     cout << "You cannot use a comma in here." << endl;
        // }
        else {
            cout << "Invalid email format. Try again." << endl;
        }
    }
    

    //passwordCheck:
    while(true){
        cout << "Enter your Password: ";
        getline(cin, pswd);
        if (pswd.length() < 8) {
        cout << "Password needs to be more than 8 characters." << endl;
        }
        else{
            break;
        }
    }
    


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

    cout << "\n--- Login to Your Account ---\n";


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
LoginStatus resetPassword(){
    
    string username, pswd, newHashedPassword;

    cout << "\n--- Reset your Password ---\n";


    cout << "Enter your username: ";
    getline(cin, username);


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
            break;
        }
    }
    user_file.close();

    if(!user_found){
        return LOGIN_USER_NOT_FOUND;
    }

    //NewPassword
    while(true){
        cout << "Enter your new password: ";
        getline(cin, pswd);
        
        if (pswd.length() < 8) {
        cout << "Password needs to be more than 8 characters." << endl;
        }
        else{
            newHashedPassword = hashPassword(pswd);
            break;
        }
    }

    //SettingTemporaryFile&Updating:
    ifstream user_file_read("users.csv");
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


    remove("users.csv");
    rename("temp.csv", "users.csv");

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
