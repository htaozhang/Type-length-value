all: tlv_unittest

unittest : tlv_unittest.cc tlv.cc
	g++ -g -Wall -o $@ $^

clean:
	rm tlv_unittest
