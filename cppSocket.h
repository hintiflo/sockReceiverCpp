/*
 * cppSocket.h
 *
 *  Created on: 12.01.2025
 *      Author: Florian Hinterleitner
 */

#pragma once

#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

using namespace std;

class cppSocket
{
	public:
		string localIP = "-1.-1.-1.-1";
		int listenerPort = -1;
		
		cppSocket(){}
		
		bool create();
		bool binding();
		bool listening();
		bool accepting();
		bool receiving(string * rxMsg);
		bool sending(string txMsg);
		void closeListener();
		void closeClient()	;
		string getSockIP()	;
		int  getSockPort()	;
		string getClientIP();
		int  getClientPort();
	
	private:
		int clientFID = -1;
		string rxMsg = "";
		int listenerFID = -1;
		string sockIP = "";
		int sockPort = -1;
		char addrBuf[INET_ADDRSTRLEN];
		sockaddr_in clientAddr;
		int clientPort = -1;
		string clientIP = "";
	
	
};

