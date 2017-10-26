TLV_DIR = ..
GTEST_INC = $(TLV_DIR)/3rd/googletest/include
GTEST_LIB = $(TLV_DIR)/3rd/googletest/lib
TLV_HEADERS = $(TLV_DIR)/*.h
CXXFLAGS += -g -Wall -Wextra -std=c++11

all: tlv_unittest

clean: 
	rm -rf tlv_unittest *.o *.a

tlv.o: $(TLV_DIR)/tlv.cc $(TLV_HEADERS)
	$(CXX) $(CXXFLAGS) -c $(TLV_DIR)/tlv.cc

tlv.a: tlv.o
	$(AR) $(ARFLAGS) $@ $^

tlv_unittest.o: $(TLV_DIR)/tlv_unittest.cc $(TLV_HEADERS)
	$(CXX) $(CXXFLAGS) -I $(GTEST_INC) -c $(TLV_DIR)/tlv_unittest.cc

tlv_unittest: tlv_unittest.o tlv.a
	$(CXX) $(CXXFLAGS) -I $(GTEST_INC) -L $(GTEST_LIB) -lgtest -lgtest_main $^ -o $@

