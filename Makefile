
main.o:	compile test

run:
	./sockReceiver localhost 4321

test:	# only triggers test cases
	./sockReceiver
	

compile:
	g++ sockReceiver.cpp cppSocket.cpp -o sockReceiver -lgtest -lgtest_main

clean:
	rm sockReceiver
