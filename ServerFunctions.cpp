// Copyright 2022
// Server functions built for individual authenticated users to view
// and create files. Users can navigate and view their own directories
// and files, as well as create them remotely.
// Created files can be analyzed by the word counts.
// Author: Caleb Mostyn

#include <cstring>  // c-style strings are heavily utilized for ease
#include <fstream>  // for file input and output
#include <vector>  // for file analysis
#include <algorithm>  // for file analysis
#include <string>  // for string concatonation
#include "PracticalSocket.h"  // for Socket
#include "ServerFunctions.h"

using std::ifstream;
using std::ofstream;
using std::strcmp;
using std::vector;

// Login for existing user
bool existingLogin(char *usr, char *psw) {
    bool success = false;

        // Read in from the username and password files,
        // tracking whether or not the given username and password
        // are found and at what position in the file
        ifstream usrIn("usernames.txt");
        string username;
        int usrCount = 0;
        bool usrFound = false;
        ifstream pswIn("passwords.txt");
        string password;
        int pswCount = 0;
        int pswFound = false;

        // Checks all the usernames
        while (usrIn >> username) {
            usrCount++;
            if (strcmp(usr, username.c_str()) == 0) {
                usrFound = true;
                break;
            }
        }

        // Checks all the passwords
        while (pswIn >> password) {
            pswCount++;
            if (strcmp(psw, password.c_str()) == 0) {
                pswFound = true;
                break;
            }
        }

        // Only successful if both username and password are found in the file
        // at the same positions
        if (usrFound && pswFound && usrCount == pswCount) {
            success = true;
        }

    // Returns success of authentication
    return success;
}

// Login for new user
bool newLogin(char *usr, char *psw) {
    bool success = true;

        // Reads in from usernames file
        ifstream usrIn("usernames.txt");
        string username;

        // If username already exists, authentication fails
        while (usrIn >> username) {
            if (strcmp(usr, username.c_str()) == 0) {
                success = false;
                break;
            }
        }

    // If successful, append username and password to
    // their respective files
    if (success) {
        ofstream usrOut("usernames.txt", std::ios_base::app);
        ofstream pswOut("passwords.txt", std::ios_base::app);

        usrOut << usr << endl;
        pswOut << psw << endl;
    }

    // Returns success of authentication
    return success;
}

// Creates a new directory using Linux mkdir
void mkdir(char *dirName, char *usr) {
    // Creating linux command (unique to user,
    // only user can open their directories)

    // Format is "mkdir (username)(directoryName)"
    string command = "mkdir ";
    command += usr;
    command += dirName;

    // Makes directory
    system(command.c_str());
}

// Lists files in a specified directory
void list(char *dirName, char *usr, TCPSocket *sock) {
    // Creating linux command (unique to user,
    // only user can open their directories)

    // Format is "ls -l (username)(directoryName) > output.txt"
    string command = "ls -l ";
    command += usr;
    command += dirName;
    command += " > output.txt";

    // Creates list file
    system(command.c_str());

    // Read in the output file
    ifstream output("output.txt");
    output.seekg(0, output.end);
    int n = output.tellg();
    output.seekg(0, output.beg);
    char *data = new char[n];
    output.read(data, n);

    // Sends the size of the output file
    sock->send(&n, sizeof(n));
    // Sends the contents of the output file
    sock->send(data, n);
}

// Writing a file at specified directory
void writeFile(char *dirName, char *fileName, char *usr, TCPSocket *sock) {
    // Creating the full directory string
    // Format is "(username)(directoryName)/(fileName)"
    string fullDir;
    fullDir += usr;
    fullDir += dirName;
    fullDir += "/";
    fullDir += fileName;

    // Creates file
    ofstream output(fullDir);
    char out[32] = "";

    // Reads input from user, writing to output file,
    // until they specify the end of the file
    while (strcmp(out, "endfile") != 0) {
        sock->recv(&out, sizeof(out));
        if (strcmp(out, "endfile") == 0) {
            // Ends loop
            break;
        } else if  (strcmp(out, "newline") == 0) {
            // Prints new line if specified
            output << endl;
        } else {
            // Prints word with a space
            output << out << " ";
        }
    }
    // End file with a new line
    output << endl;
}

// Sends a file to the client
void showFile(char *dirName, const char *fileName, char *usr, TCPSocket *sock) {
    // Creating the full directory string
    // Format is "(username)(directoryName)/(fileName)"
    string fullDir;
    fullDir += usr;
    fullDir += dirName;
    fullDir += "/";
    fullDir += fileName;

    // Reading in specified file
    ifstream output(fullDir);
    output.seekg(0, output.end);
    int n = output.tellg();
    output.seekg(0, output.beg);
    char *data = new char[n];
    output.read(data, n);

    // Sends size of file
    sock->send(&n, sizeof(n));
    // Sends contents of file
    sock->send(data, n);
}

// Helper function for analysis, removes non-alphabetic characters
string removeChar(string word) {
    for (size_t i = 0; i < word.size(); i++) {
        if ((word[i] < 'A' || word[i] > 'Z')
        && (word[i] < 'a' || word[i] > 'z')) {
            word.erase(i, 1);
            i--;
        }
    }
    return word;
}

void analyzeFile(char *dirName, char *fileName, char *usr) {
    // Creating the full directory string
    // Format is "(username)(directoryName)/(fileName)"
    string fullDir;
    fullDir += usr;
    fullDir += dirName;
    fullDir += "/";
    fullDir += fileName;

    // Reading input file
    ifstream input(fullDir);

    // Initializing variables for analysis
    vector<string> words;
    vector<int> counts;
    string word;
    int fileWordCount = 0;

    // Read file one word at a time
    while (input >> word) {
        // Clean string
        word = removeChar(word);
        // Send to lowercase
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);

        // Searches words vector for word
        bool found = false;
        int index;
        for (size_t i = 0; i < words.size(); i++) {
            if (words[i] == word) {
                found = true;
                index = i;
            }
        }

        if (found) {
            // If already contained, increase count
            counts[index]++;
        } else {
            // Otherwise, add word to words with count of 1
            words.push_back(word);
            counts.push_back(1);
        }

        // Adds word if vector is empty
        if (words.size() == 0) {
            words.push_back(word);
            counts.push_back(1);
        }
        // Increase file word count by 1
        fileWordCount++;
    }

    // Creating the full analysis directory string
    // Format is "(username)(directoryName)/analysis(fileName)"
    string outDir;
    outDir += usr;
    outDir += dirName;
    outDir += "/";
    outDir += "analysis";
    outDir += fileName;

    // Writing to output file
    ofstream output(outDir);

    output << "Analysis of " << fileName << ":" << endl;
    output << "Total # of words: " << fileWordCount << endl;

    // Prints all words, with # of instances and their percentage
    // of the file
    for (size_t i = 0; i < words.size(); i++) {
        output << words[i] << ":" << endl;
        output << "\t # of instances: " << counts[i] << endl;
        output << "\t % of document: " << (static_cast<double>(counts[i])
            / fileWordCount) * 100 << "%" << endl;
    }
    // End file with a new line
    output << endl;
}

