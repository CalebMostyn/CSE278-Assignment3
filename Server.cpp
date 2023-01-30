// Copyright 2022
// Server built for individual authenticated users to view and create files.
// Users can navigate and view their own directories and files, as well as
// create them remotely. Created files can be analyzed by the word counts.
// Author: Caleb Mostyn

#include <iostream>  // For output
#include <cstring>  // C-style strings are heavily utilized for ease
#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include "ServerFunctions.h"  // For server functionality

using std::cout;
using std::endl;

void HandleTCPClient(TCPSocket *sock);  // TCP client handling function

int main(int argc, char *argv[]) {
    if (argc != 2) {                     // Test for correct number of arguments
        cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
        exit(1);
    }

    // First arg: local port
    unsigned short serverPort = atoi(argv[1]);  // NOLINT

    try {
        TCPServerSocket serverSock(serverPort);  // Server Socket object

        for (;;) {   // Run forever
            // Wait for client to connect
            HandleTCPClient(serverSock.accept());
        }
    } catch (const std::exception& e) {
        cerr << e.what() << endl;
        exit(1);
    }
    return(0);
}

// TCP client handling function
void HandleTCPClient(TCPSocket *sock) {
    cout << "Handling client ";
    try {
        cout << sock->getForeignAddress() << ":";
    } catch (const std::exception& e) {
        cerr << "Unable to get foreign address" << endl;
        cerr << e.what() << endl;
    }
    try {
        cout << sock->getForeignPort();
    } catch (const std::exception& e) {
        cerr << "Unable to get foreign port" << endl;
        cerr << e.what() << endl;
    }
    cout << endl;

    // initializing variables for menu interaction
    bool exit = false;
    bool auth = false;
    char usr[32];
    char psw[32];
    char dirName[32];
    char fileName[32];
    string analysisFileName;

    // Runs until user chooses exit menu option
    while (!exit) {
        // Taking username and password until logged in
        if (!auth) {
            int menuSelection = 3;
            sock->recv(&menuSelection, sizeof(menuSelection));

            // User either logs in with an existing account,
            // creates a new login, or exits
            switch (menuSelection) {
                case 1:
                    // *Login to existing account*
                    // Receive username and password
                    sock->recv(&usr, sizeof(usr));
                    sock->recv(&psw, sizeof(psw));

                    // Checks login against database
                    auth = existingLogin(usr, psw);

                    // Returns to user success of authorization
                    sock->send(&auth, sizeof(auth));
                    break;
                case 2:
                    // *Create a new login*
                    // Receive username and password
                    sock->recv(&usr, sizeof(usr));
                    sock->recv(&psw, sizeof(psw));

                    // Checks if username is existing,
                    // if not, creates new login
                    auth = newLogin(usr, psw);

                    // Returns to user success of authorization
                    sock->send(&auth, sizeof(auth));
                    break;
                case 3:
                    // Client exits the server
                    exit = true;
                    break;
            }
        } else {
            int menuSelection = 5;
            sock->recv(&menuSelection, sizeof(menuSelection));
            switch (menuSelection) {
                case 1:
                    // *Create a new directory*
                    // receive directory name
                    sock->recv(&dirName, sizeof(dirName));

                    // Create unique directory with given
                    // directory name appended to username
                    mkdir(dirName, usr);
                    break;
                case 2:
                    // *List files in a directory*
                    // receive directory name
                    sock->recv(&dirName, sizeof(dirName));

                    // Sends list of files in directory to client
                    list(dirName, usr, sock);
                    break;
                case 3:
                    // *write a file remotely*
                    // receive directory name
                    sock->recv(&dirName, sizeof(dirName));
                    // receive output file name
                    sock->recv(&fileName, sizeof(fileName));

                    // Takes user input to write to a file
                    // with a specified name and directory
                    writeFile(dirName, fileName, usr, sock);
                    break;
                case 4:
                    // *"display" a file*
                    // receive a directory name
                    sock->recv(&dirName, sizeof(dirName));
                    // receive an input file name
                    sock->recv(&fileName, sizeof(fileName));

                    // Reads file and sends full contents to client
                    showFile(dirName, fileName, usr, sock);
                    break;
                case 5:
                    // *analyze a file and display analysis*
                    // receive a directory name
                    sock->recv(&dirName, sizeof(dirName));
                    // receive an input file name
                    sock->recv(&fileName, sizeof(fileName));

                    // Creates an analysis file based on specified file
                    analyzeFile(dirName, fileName, usr);

                    // appending file name to anaylsis
                    analysisFileName = "analysis";
                    analysisFileName += fileName;

                    // Sends analysis file to client
                    showFile(dirName, analysisFileName.c_str(), usr, sock);
                    break;
                case 6:
                    // Client exits the server
                    exit = true;
                    break;
            }
        }
    }
    delete sock;
}

