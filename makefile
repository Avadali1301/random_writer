randwriter: randwriter.o hashMap.o markov.o
	g++ -o randwriter hashMap.o markov.o randwriter.o

hashMap.o: hashMap.cc hashMap.h
	g++ -c hashMap.cc

markov.o: markov.cc markov.h
	g++ -c markov.cc

randwriter.o: randwriter.cc hashMap.h markov.h
	g++ -c randwriter.cc

clean:
	rm -rf hashMap.o markov.o randwriter.o randwriter
