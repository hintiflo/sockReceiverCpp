
main.o:
	g++ sockReceiver.cpp cppSocket.cpp -o sockReceiver
	./sockReceiver localhost 1234

only:
	g++ sockReceiver.cpp cppSocket.cpp -o sockReceiver

clean:
	sockReceiver


