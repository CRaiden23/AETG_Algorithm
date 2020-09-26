a.out : main.o
	g++ main.o -Ofast

topo.o : main.cpp
	g++ -c main.cpp

clean :
	rm *.o a.out results

run :
	./a.out

