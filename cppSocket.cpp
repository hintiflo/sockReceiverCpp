/*
 * cppSocket.cpp
 *
 *  Created on: 12.01.2025
 *      Author: Florian Hinterleitner
 */


#include <cmath>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cppSocket.h"

using namespace std;
#define INET_RX_BLOCKSIZE 4096

bool cppSocket::create()	// 1.)
{	listenerFID = socket(AF_INET, SOCK_STREAM, 0);
	// bool options;
	int flag =1;
	if(	0 > setsockopt(listenerFID, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) ||
		0 > setsockopt(listenerFID, SOL_SOCKET, SO_REUSEPORT, &flag, sizeof(int))	)
		;
		// options = false;
	// else 
		// options = true;

	if(0 < listenerFID)
		return true;
	else 
		return false;
}

bool cppSocket::binding()	// 2.)
{
	sockaddr_in listenerAddress;
	listenerAddress.sin_family = AF_INET;
	listenerAddress.sin_port = htons(listenerPort);

	localIP = "127.0.0.1";
	// sock.localIP = "192.168.1.1";
	inet_pton(AF_INET, localIP.c_str(), &listenerAddress.sin_addr);

	sockIP = inet_ntop(AF_INET, &listenerAddress.sin_addr, addrBuf, INET_ADDRSTRLEN);
	sockPort = ntohs(listenerAddress.sin_port);
	
	if(	0 > bind(listenerFID, (sockaddr*)&listenerAddress, sizeof(listenerAddress))	)
	{	return false;
	}
	return true;
}

bool cppSocket::listening()	// 3.)
{

	if(	0 > listen(listenerFID, SOMAXCONN)	)
		return false;
	return true;
}

bool cppSocket::accepting()
{
	socklen_t clientAddrLen = sizeof(clientAddr);
	if( 0 >  (clientFID = accept(listenerFID, (sockaddr * ) &clientAddr, &clientAddrLen))	)
			return true;

	closeListener();
	clientPort = ntohs( clientAddr.sin_port);
	clientIP = inet_ntop(AF_INET, &(clientAddr.sin_addr), addrBuf, INET_ADDRSTRLEN);

	return true;
}

bool cppSocket::receiving(string * rxMsg)
{
	char msgBuffer[INET_RX_BLOCKSIZE];
	int rxCount = recv(clientFID, &msgBuffer, INET_RX_BLOCKSIZE, 0);		
	if( 0 < rxCount )
		{	*rxMsg = msgBuffer;
			*rxMsg = (*rxMsg).erase((*rxMsg).find("\n") ,(*rxMsg).find("\n"));
			// clean the RX buffer
			memset(msgBuffer, 0, sizeof(msgBuffer));					
			return true;
		}
	else 
		return false;
}

bool cppSocket::sending(string txMsg)
{	char * msgBuff = (char *)(( txMsg.append("\n")).data());
	send(clientFID, msgBuff, strlen(msgBuff), 0);
	return true;
}

void 	cppSocket::closeListener()		{	close(listenerFID);			}
void 	cppSocket::closeClient()		{	close(clientFID);			}	
string 	cppSocket::getSockIP()			{	return sockIP;				}
int  	cppSocket::getSockPort()		{	return sockPort;			}
string 	cppSocket::getClientIP()		{	return clientIP;			}
int  	cppSocket::getClientPort()		{	return clientPort;			}

