CXXFLAGS = -g -std=c++20 -pthread


poolTester: poolTester.o tPool.o
	g++ $(CXXFLAGS) poolTester.o tPool.o -o poolTester

poolTester.o: poolTester.cc tPool.h tPool.cc
	g++ $(CXXFLAGS) poolTester.cc -c

tPool.o: tPool.cc tPool.h
	g++ $(CXXFLAGS) tPool.cc -c

clean:
	rm -f *.o
	rm -f poolTester