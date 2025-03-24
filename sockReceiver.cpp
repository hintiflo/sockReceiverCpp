/*
 * sockReceiver.cpp
 *
 *  Created on: 12.01.2025
 *      Author: Florian Hinterleitner
 */

#include "gtest/gtest.h" 
#include <iostream>
#include <exception>
#include <signal.h>
#include <string.h>
#include "cppSocket.h"

using namespace std;

class KeyExcept : public std::exception
{	public:
		KeyExcept(int signal) : Signal(signal){}
	int Signal;
};

void signalException(int signal)	{		throw KeyExcept(signal);	}

#define INFO "[INFO] "
#define ERRO "[ERRO] "
#define WARN "[WARN] "
#define DATA "[DATA] "
#define TODO "[TODO] "
#define TAST "[TEST] "
#define EERO_POST endl << "    exiting program with error -1, goodbye!" << endl

string clientGoneMsg = "client gone from port ";
string clientRxMsg = "client ";
string clientPortMsg = "client address: ";
string keyExceptMsg = "Ctrl+C irq";
string serverStopMsg = "server out";

int main(int argc, char ** argv)
{
	cppSocket sock;
	testing::InitGoogleTest(&argc, argv);
	cout << RUN_ALL_TESTS();

	struct sigaction signalHandler;
	signalHandler.sa_handler = signalException;
	sigemptyset(&signalHandler.sa_mask);
	signalHandler.sa_flags = 0;
	sigaction(SIGINT, &signalHandler, NULL);


	cout << "***************************************************" << endl;
	cout << "    tcpReceiver, called with " << argc-1 << " args " << endl;
	cout << "***************************************************" << endl;

	// 0. Sanitize input values or set std values
	if (argc < 2)
	{	sock.setLocalIP("127.0.0.1");
		sock.setListenerPort(10000);
		cout << INFO << "no params given, IP: " << sock.getLocalIP() << ", port: " << sock.getListenerPort() << endl;
	} 

	if (argc == 2)
	{	sock.setLocalIP("127.0.0.1");
		auto argv1 = argv[1];
		sock.setListenerPort( atoi( argv1 ));
		cout << INFO << "port param given, IP: " << sock.getLocalIP() << ", sock.listenerPort: " << sock.getListenerPort() << endl;
	}

	if (argc == 3)
	{	auto argv1 = argv[1];
		auto argv2 = argv[2];

		sock.setLocalIP(argv1);
		if("localhost" == argv1)
			sock.setLocalIP("127.0.0.1");

		sock.setListenerPort( atoi( argv2 ));
		cout << INFO << "port param given, IP: " << sock.getLocalIP() << ", port: " << sock.getListenerPort() << endl;
	}

	try
	{	while(true)	// outer waiting loop
		{
			// 1.) create a listener socket
			if( !sock.create() )
			{	cout << ERRO << "socket could not be created" << endl << "    exiting program with error -1, goodbye!" << endl;
				return -1;
			}

			// 2.) bind socket to adress/port
			if( !sock.binding() )
			{	cout << ERRO << "couldnt bind listener to " << sock.getSockIP() << "|" << sock.getSockPort() << endl  << EERO_POST;
				return -1;
			}	

			// 3.) waiting for inbound connection
			if( !sock.listening() )
			{	cout << ERRO << "listener can't be established" << endl <<  EERO_POST;;
				return -1;
			}
			cout << INFO << "listener listening on " << sock.getSockIP() << "|" << sock.getSockPort() << endl;

			if( !sock.accepting() )
			{	cout << ERRO << "incoming connection could not be accpted" << EERO_POST;
				return -1;
			}
			cout << INFO << clientPortMsg << sock.getClientIP() << "|" << sock.getClientPort() << endl;

			while(true)	// inner connected loop
			{	// 5.) receive incoming messages
				string rxMsg = "";
				if( sock.receiving( &rxMsg ) )
				{	cout << INFO << clientRxMsg << sock.getClientPort() << ": " << rxMsg << endl;
					// 6.) echo message back to the client
					sock.sending(rxMsg);
				}else
				{	cout << INFO << clientGoneMsg << sock.getClientPort() << endl;
					break;
				}
			}	// inner connected loop
			// 7.) close the connection 
			sock.closeClient();
		}	// outer waiting loop
	}	// try block

	catch(KeyExcept& except)
	{	sock.closeListener();
		cout << WARN << keyExceptMsg << except.Signal << endl;
	}

	cout << INFO << serverStopMsg << endl;
	return 0;
}


TEST(cppSocket, initListener )
{	cppSocket sock;
	ASSERT_TRUE(sock.setListenerPort(54321));
	ASSERT_TRUE(sock.setLocalIP("192.168.1.1"));
	ASSERT_TRUE(sock.create());
	ASSERT_TRUE(sock.binding());
	// cout << TAST << "LocalIP " << sock.getLocalIP() << ", port: " << sock.getListenerPort() << endl;
	ASSERT_TRUE(sock.listening());
}

TEST(cppSocket, wrongPort)
{
	// TODO: init sock with invalid port number
	ASSERT_TRUE(false);
}

TEST(cppSocket, wrongIP)
{
	// TODO: init sock with invalid IP format
	ASSERT_TRUE(false);
}

