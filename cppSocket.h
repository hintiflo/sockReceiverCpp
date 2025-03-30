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

#define PORT_LOWER_LIM 0
#define PORT_UPPER_LIM 65536

using namespace std;

class cppSocket
{
	public:
		
		cppSocket(){}

		bool	create();
		bool	binding();
		bool	listening();
		bool	accepting();
		bool	receiving(string * rxMsg);
		bool	sending(string txMsg);
		void	closeListener();
		void	closeClient()	;
		string	getSockIP()	;
		int		getSockPort()	;
		string	getClientIP();
		int		getClientPort();
		string	getLocalIP()		{	return localIP;			};
		int		getListenerPort()	{	return listenerPort;	};

		bool	setLocalIP(string IP)		
		{	localIP = IP;
			return true;
		};
		bool	setListenerPort(int port)	
		{	if(	port > PORT_LOWER_LIM &&
				port < PORT_UPPER_LIM
			  )
			{	listenerPort = port;
				return true;
			}else
				return false;
		};

	private:
		string	localIP = "-1.-1.-1.-1";
		int		listenerPort = -1;
		int		clientFID = -1;
		string	rxMsg = "";
		int		listenerFID = -1;
		string	sockIP = "";
		int		sockPort = -1;
		char	addrBuf[INET_ADDRSTRLEN];
		int		clientPort = -1;
		string	clientIP = "";
		sockaddr_in clientAddr;
	
};

