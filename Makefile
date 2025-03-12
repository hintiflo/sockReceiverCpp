

main.o:
	g++ -Wall -c sockReceiver.cpp -o sockReceiver.o
	g++ sockReceiver.o -o sockReceiver
	./sockReceiver localhost 1234


