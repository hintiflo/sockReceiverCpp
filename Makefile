
main.o:
	g++ sockReceiver.cpp cppSocket.cpp -o sockReceiver -lgtest
	./sockReceiver localhost 1234

only:
	g++ sockReceiver.cpp cppSocket.cpp -o sockReceiver -lgtest

clean:
	sockReceiver


