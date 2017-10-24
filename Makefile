all: unittest

unittest : unittest.cc tlv.cc
	g++ -g -Wall -o $@ $^

clean:
	rm unittest
