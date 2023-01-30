// Copyright 2022
// Client functions built for individual authenticated users to view
// and create files. Users can navigate and view their own directories
// and files on the server, as well as create them remotely.
// Created files can be analyzed by the word counts.
// Author: Caleb Mostyn

#include <iostream>  // For output
#include <cstring>  // C-style strings are heavily utilized for ease
#include "PracticalSocket.h"  // For Socket
#include "ClientFunctions.h"

using std::cout;
using std::cerr;
using std::endl;

// Prints resulting authentication message for client
void loginMessage(bool auth, int menuSelection, char *usr) {
    cout << endl;
    if (auth) {
        switch (menuSelection) {
            case 1:
                cout << "Logged in Successfully!" << endl;
                break;
            case 2:
                cout << "Created New Account with Username " << usr << endl;
                break;
        }
    } else {
        switch (menuSelection) {
            case 1:
                cout << "Incorrect Username or Password" << endl;
                break;
            case 2:
                cout << "Unsuccessful - Existing Username " << usr << endl;
                break;
        }
    }
    cout << endl;
}

// Sends directory name to be made
void mkdir(TCPSocket *sock) {
    char dirName[32];

    // Prompt client for directory name
    cout << "Enter a Directory Name: ";
    cin >> dirName;
    // Send directory name to server
    sock->send(&dirName, sizeof(dirName));
    cout << endl;
}

// Sends directory name to be listed and receives
// ls output in return
void list(TCPSocket *sock) {
    char dirName[32];
    int len;

    // Prompt client for directory name
    cout << "Enter a Directory Name: ";
    cin >> dirName;
    // Send directory name to server
    sock->send(&dirName, sizeof(dirName));

    // Receive length of file data
    sock->recv(&len, sizeof(len));
    // Receive file data
    char result[len];  // NOLINT
    sock->recv(&result, len);
    result[len - 1] = '\0';

    cout << endl << result << endl << endl;
}

// Sends directory and file name to write to,
// as well as contents of file
void writeFile(TCPSocket *sock) {
    char dirName[32];
    char fileName[32];

    // Prompt client for directory name
    cout << "Enter a Directory Name: ";
    cin >> dirName;
    // Prompt client for file name
    cout << "Enter a File Name: ";
    cin >> fileName;
    // Send directory and file name to server
    sock->send(&dirName, sizeof(dirName));
    sock->send(&fileName, sizeof(fileName));

    // Prompt client for file content
    cout << "Write to File (""newline"" for a new line,";
    cout << " endfile"" to end): " << endl;
    char content[32];
    // Reads in from client and sends to server
    while (strcmp(content, "endfile") != 0) {
        cin >> content;
        sock->send(&content, sizeof(content));
    }
    cout << endl;
}

// Sends directory and file name to display,
// receiving the contents of the file
void showFile(TCPSocket *sock) {
    char dirName[32];
    char fileName[32];
    int len;

    // Prompt client for directory name
    cout << "Enter a Directory Name: ";
    cin >> dirName;
    // Prompt client for file name
    cout << "Enter a File Name: ";
    cin >> fileName;
    // Send directory and file name to server
    sock->send(&dirName, sizeof(dirName));
    sock->send(&fileName, sizeof(fileName));

    // Receive length of file data
    sock->recv(&len, sizeof(len));
    // Receive file data
    char result[len];  // NOLINT
    sock->recv(&result, len);
    result[len - 1] = '\0';

    cout << endl << result << endl << endl;
}
