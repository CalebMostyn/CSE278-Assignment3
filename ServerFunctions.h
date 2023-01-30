// Copyright 2022
// Server functions built for individual authenticated users to view
// and create files. Users can navigate and view their own directories
// and files, as well as create them remotely.
// Created files can be analyzed by the word counts.
// Author: Caleb Mostyn

#ifndef _HOME_CALEB_HEADERS_SERVERFUNCTIONS_H_
#define _HOME_CALEB_HEADERS_SERVERFUNCTIONS_H_

#include "PracticalSocket.h"

/**
 * @brief Attempts an authentication for a user with an
 * existing set of credentials. Reads from a server side set
 * of username and password files to check against provided login.
 * 
 * @param usrname client provided username 
 * @param psw client provided password
 * @return true if login successful (username/password exist and match)
 * @return false if login is unsuccessful (username/password do not exist or do not match)
 */
bool existingLogin(char *usr, char *psw);

/**
 * @brief Attempts an authentication for a user with an
 * new set of credentials. Reads from a server side username 
 * file to check if provided login exists. If new username, add login to files.
 * 
 * @param usr client provided username
 * @param psw client provided password
 * @return true if login successful (username did not already exist)
 * @return false if login unsuccessful (username already exists)
 */
bool newLogin(char *usr, char *psw);

/**
 * @brief Utilizes Linux mkdir to create a unique directory for the user,
 * appending their specified directory name to their username.
 * 
 * @param dirName client provided directory name
 * @param usr client username
 */
void mkdir(char *dirName, char *usr);

/**
 * @brief Utilizes Linux ls -l to list the files within a specified directory.
 * Output is put into a file and sent to the user.
 * 
 * @param dirName client provided directory name
 * @param usr client username
 * @param sock client socket
 */
void list(char *dirName, char *usr, TCPSocket *sock);

/**
 * @brief Takes a client specified existing directory name, as well as a file name
 * and creates a new file. Then client input is taken until the end of the file is
 * specified, with the input being written to the file.
 * 
 * @param dirName client provided directory name
 * @param fileName client provided file name
 * @param usr client username
 * @param sock client socket
 */
void writeFile(char *dirName, char *fileName, char *usr, TCPSocket *sock);

/**
 * @brief Finds a client specified file at a client specified directory, reads 
 * the file, and sends the content to the client.
 * 
 * @param dirName client provided directory name
 * @param fileName client provided file name
 * @param usr client username
 * @param sock client socket
 */
void showFile(char *dirName, const char *fileName, char *usr, TCPSocket *sock);

/**
 * @brief Analyzes a client specified file based on its content. An analysis
 * file is created, containing an itemized list of the words contained, their
 * number of appearances in the file, and the percentage of the file they make up.
 * 
 * @param dirName client provided directory name
 * @param fileName client provided file name
 * @param usr client username
 */
void analyzeFile(char *dirName, char *fileName, char *usr);

#endif  // _HOME_CALEB_HEADERS_SERVERFUNCTIONS_H_
