#include "functions.h"
#include <iostream>
using namespace std;
int main(){
    bool exit = false;
    int choice;
    cout << "------------ Welcome -------------\n";
    while (!exit)
    {
        cout << endl;
        showMenu();
        cin >> choice;
        cin.ignore();

        //RegisterationCase:
        if(choice == 1){
            registerUser();
        }

        //LoginCase:
        else if (choice == 2){
                const int maxAttempts = 3;
                int attempts = 0;
                bool isLoggedIn = false;

                while (attempts < maxAttempts && !isLoggedIn)
                {
                    string username;
                    LoginStatus result = loginUser(username);

                    if(result == LOGIN_SUCCESS){
                        cout << "\nLogin successful!" << endl;
                        cout << "Welcome '" << username << "' !" << endl;
                        exit = true;
                        break;
                    }

                    else if (result == LOGIN_FILE_NOT_FOUND || result == LOGIN_USER_NOT_FOUND) {
                        cout << "Account doesn't exist. Please Register First." << endl;
                        break;                    
                    }
                    
                    else if (result == LOGIN_INCORRECT_PASSWORD){
                        attempts++;
                        if(attempts < maxAttempts){
                            cout << "\nIncorrect username or password. Try again. Attempts left: " << maxAttempts - attempts << endl;
                            
                            char option;
                            cout << "Forgot Password? Would you like to reset your Password? [y/n]: ";
                            cin >> option;
                            cin.ignore();

                            if(tolower(option) == 'y'){
                                LoginStatus status = resetPassword(username);
                                if(status == LOGIN_SUCCESS){
                                    cout << "Password reset successful." << endl;
                                }
                                else if(status == LOGIN_USER_NOT_FOUND){
                                    cout << "Username not found. Try again or register." << endl;    
                                    }
                                break;
                            }
                        }
                        else{
                            cout << "\nLimit reached. Try again later." << endl;
                        }
                        exit = true;
                    }
                }
        }

        //Exit:
        else if (choice == 3){
            cout << "\nLeaving.....";
            exit = true;
        }

        else {
            cout << "Invalid option. Please enter 1, 2 or 3." << endl;
        }
    }




}