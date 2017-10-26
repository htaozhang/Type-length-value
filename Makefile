TLV_DIR = .
TLV_HEADERS = $(TLV_DIR)/*.h
CXXFLAGS += -g -Wall -Wextra

all: tlv_unittest

clean: 
	rm -rf tlv_unittest *.o *.a

tlv.o: $(TLV_DIR)/tlv.cc $(TLV_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(TLV_DIR)/tlv.cc

tlv.a: tlv.o
	$(AR) $(ARFLAGS) $@ $^

tlv_unittest.o: $(TLV_DIR)/tlv_unittest.cc $(TLV_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(TLV_DIR)/tlv_unittest.cc

tlv_unittest: tlv_unittest.o tlv.a
	$(CXX) $(CXXFLAGS) $^ -o $@

