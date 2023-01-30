// Copyright 2022
// Client functions built for individual authenticated users to view
// and create files. Users can navigate and view their own directories
// and files on the server, as well as create them remotely.
// Created files can be analyzed by the word counts.
// Author: Caleb Mostyn

#ifndef _HOME_CALEB_HEADERS_CLIENTFUNCTIONS_H_
#define _HOME_CALEB_HEADERS_CLIENTFUNCTIONS_H_

#include "PracticalSocket.h"

/**
 * @brief Prints a simple message for client based on whether or not
 * they created a new login or used an existing login, as well as whether
 * or not it was successful.
 * 
 * @param auth true or false, success of authentication
 * @param menuSelection 1 or 2, representing existing vs. new login
 * @param usr client username
 */
void loginMessage(bool auth, int menuSelection, char *usr);

/**
 * @brief Prompts client for a directory name to send
 * to the server to create a new directory.
 * 
 * @param sock server socket
 */
void mkdir(TCPSocket *sock);

/**
 * @brief Prompts client for a directory name to send
 * to the server to list the contents of. Contents of the
 * directory are then received and displayed.
 * 
 * @param sock server socket
 */
void list(TCPSocket *sock);

/**
 * @brief Prompts client for a directory name and a
 * file name to send to the server for file creation.
 * Client is then prompted for the content of the file,
 * word by word with a while loop.
 * 
 * @param sock server socket
 */
void writeFile(TCPSocket *sock);

/**
 * @brief Prompts client for a directory name and a
 * file name to send to the server as to be displayed.
 * Receives back the contents of the file
 * 
 * @param sock server socket
 */
void showFile(TCPSocket *sock);

#endif  // _HOME_CALEB_HEADERS_CLIENTFUNCTIONS_H_
