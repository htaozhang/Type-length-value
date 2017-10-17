#include <stdio.h>
#include <iostream>

#include "tlv.h"
using namespace std;

#ifdef WIN32
    #pragma warning(disable: 4005)
    #pragma warning(disable: 4244)
    #include <time.h>
    #include <Windows.h>
    #ifndef INT64_MIN
    	#define INT64_MIN  LLONG_MIN
    #endif
    #ifndef INT64_MAX
        #define INT64_MAX  LLONG_MAX
    #endif
    #ifndef UINT64_MAX
        #define UINT64_MAX ULLONG_MAX
    #endif
#elif __APPLE__
    #include <sys/time.h>
    typedef suseconds_t __suseconds_t;
#else
    #include <sys/time.h>
    #ifndef INT64_MIN
        #define INT64_MIN   LONG_LONG_MIN
    #endif
    #ifndef INT64_MAX
        #define INT64_MAX   LONG_LONG_MAX
    #endif
    #ifndef UINT64_MAX
        #define UINT64_MAX  ULONG_LONG_MAX
    #endif
#endif


enum Type {
    TYPE_BOOL = 1,
    TYPE_INT8, TYPE_UINT8, TYPE_INT16, TYPE_UINT16, 
    TYPE_INT32, TYPE_UINT32, TYPE_INT64, TYPE_UINT64,
    TYPE_FLOAT, TYPE_DOUBLE, TYPE_CHARS, TYPE_STRING, TYPE_OBJECT 
};

int main() {
    tlv::TlvMap encoder;
    encoder.Set(TYPE_BOOL, true);
    encoder.Set(TYPE_INT8, INT8_MIN);
    encoder.Set(TYPE_UINT8, UINT8_MAX);
    encoder.Set(TYPE_INT16, INT16_MIN);
    encoder.Set(TYPE_UINT16, UINT16_MAX);
    encoder.Set(TYPE_INT32, (int32_t)INT32_MIN);
    encoder.Set(TYPE_UINT32, (uint32_t)UINT32_MAX);
    encoder.Set(TYPE_INT64, (int64_t)INT64_MIN);
    encoder.Set(TYPE_UINT64, (uint64_t)UINT64_MAX);
    encoder.Set(TYPE_FLOAT, (float)3.14);
    encoder.Set(TYPE_DOUBLE, (double)3.14);
    encoder.Set(TYPE_CHARS, "hi, const char*");
    encoder.Set(TYPE_STRING, std::string("hi, std::string"));
    if (encoder.Serialization()) {
        cout << "decoder Encode ok, " << encoder.Length() << " bytes" << endl;
    }

    tlv::TlvMap maps;
    maps.Set(TYPE_OBJECT, encoder);
    if (maps.Serialization()) {
        cout << "maps serialization ok, " << maps.Length() << " bytes" << endl;
    }

    tlv::TlvMap maps2;
    maps2.Deserialization(maps.Buffer(), maps.Length());

    tlv::TlvMap decoder;
    maps2.Get(TYPE_OBJECT, decoder);
    

#define TEST(type, x) \
    { \
        type value; \
	decoder.Get(x, value); \
	cout << "Get "#type" ok, " << value << endl; \
    }

    TEST(bool, TYPE_BOOL)
    TEST(int8_t, TYPE_INT8)
    TEST(uint8_t, TYPE_UINT8)
    TEST(int16_t, TYPE_INT16)
    TEST(uint16_t, TYPE_UINT16)
    TEST(int32_t, TYPE_INT32)
    TEST(uint32_t, TYPE_UINT32)
    TEST(int64_t, TYPE_INT64)
    TEST(uint64_t, TYPE_UINT64)
    TEST(float, TYPE_FLOAT)
    TEST(double, TYPE_DOUBLE)
    TEST(std::string, TYPE_STRING);

    {
        char value[64];
	int length = 64;
	decoder.Get(TYPE_CHARS, value, length);
	cout << "Get char* ok, " << value << endl;
    }

    return 0;
}




