// Copyright 2022
// Client built for individual authenticated users to view and create files.
// Users can navigate and view their own directories and files on the server,
// as well as create them remotely.
// Created files can be analyzed by the word counts.
// Author: Caleb Mostyn

#include <iostream>  // For output
#include <cstring>  // C-style strings are heavily utilized for ease
#include "PracticalSocket.h"  // For Socket
#include "ClientFunctions.h"

using std::cout;
using std::cerr;
using std::endl;

// Login menu - preauthentication
const char LOGIN_MENU[] = "REMOTE SERVER\n"
                    "=============\n"
                    "1. Login with Existing Credentials\n"
                    "2. Create a New Account\n"
                    "3. Exit Server\n";

// Function menu - postauthentication
const char FUNCTION_MENU[] = "REMOTE SERVER\n"
                    "=============\n"
                    "1. Create a New Directory\n"
                    "2. List Files in Directory\n"
                    "3. Write a New File\n"
                    "4. Display a File\n"
                    "5. Analyze a File\n"
                    "6. Exit Server\n";


int main(int argc, char *argv[]) {
    if (argc != 3) {     // Test for correct number of arguments
        cerr << "Usage: " << argv[0]
             << " <Server> <Port>" << endl;
        exit(1);
    }

    // First arg: server address
    string serverAddress = argv[1];
    // Second arg: local port
    unsigned short serverPort = atoi(argv[2]);  // NOLINT

    try {
        // Establish connection with the server
        TCPSocket sock(serverAddress, serverPort);

        // Initializing variables for menu interaction
        bool exit = false;
        bool auth = false;
        char usr[32];
        char psw[32];
        string selection;
        int menuSelection;

        // Runs loop until user exits
        while (!exit) {
            // Checks for authentication
            if (!auth) {
                // Print login menu and accept selection
                cout << LOGIN_MENU << endl;
                cin >> selection;

                // Ensure that menu input is an integer
                try {
                    menuSelection = atoi(selection.c_str());
                }catch(std::exception &e) {
                    menuSelection = 0;
                }

                // User sends login, whether new or old
                cout << endl;
                if (menuSelection == 1 || menuSelection == 2) {
                    // Take username and password
                    cout << "Enter Username: ";
                    cin >> usr;
                    cout << "Enter Password: ";
                    cin >> psw;

                    // Server accepts selection and login
                    sock.send(&menuSelection, sizeof(menuSelection));
                    sock.send(&usr, sizeof(usr));
                    sock.send(&psw, sizeof(psw));

                    // Server returns boolean (authenticated ? true : false)
                    sock.recv(&auth, sizeof(auth));

                    loginMessage(auth, menuSelection, usr);

                } else if (menuSelection == 3) {
                    // Exiting the server
                    sock.send(&menuSelection, sizeof(menuSelection));
                    exit = true;
                }
            } else {
                // Print function menu and accept selection
                cout << FUNCTION_MENU << endl;
                cin >> selection;

                // Ensure that menu input is an integer
                try {
                    menuSelection = atoi(selection.c_str());
                }catch(std::exception &e) {
                    menuSelection = 0;
                }

                cout << endl;
                // Performing user specified function
                if (menuSelection != 6 && menuSelection != 0) {
                    // Sending menu selection
                    sock.send(&menuSelection, sizeof(menuSelection));

                    if (menuSelection == 1) {
                        mkdir(&sock);
                    } else if (menuSelection == 2) {
                        list(&sock);
                    } else if (menuSelection == 3) {
                        writeFile(&sock);
                    } else if (menuSelection == 4 || menuSelection == 5) {
                        // showFile() is used for both display and analyze,
                        // distinction is made on server side
                        showFile(&sock);
                    }
                } else if (menuSelection == 6) {
                    // Exiting the server
                    sock.send(&menuSelection, sizeof(menuSelection));
                    exit = true;
                }
            }
        }
    } catch(std::exception &e) {
        cerr << e.what() << endl;
        exit(1);
    }
    return(0);
}

