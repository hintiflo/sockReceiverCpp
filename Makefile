

main.o:	compile run

run:
	./sockReceiver localhost 4321

compile:
	g++ sockReceiver.cpp cppSocket.cpp -o sockReceiver -lgtest

clean:
	rm sockReceiver
