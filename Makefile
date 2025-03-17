
main.o:
	# @echo $@  # print target name
	g++ -Wall -c sockReceiver.cpp -o sockReceiver.o
	g++ sockReceiver.o -o sockReceiver
	./sockReceiver localhost 1234

only:
	# @echo $@  # print target name
	g++ -Wall -c sockReceiver.cpp -o sockReceiver.o
	g++ sockReceiver.o -o sockReceiver



clean:
	rm *.o


