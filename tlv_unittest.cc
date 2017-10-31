#include <stdio.h>
#include <iostream>

#include "tlv.h"
#include "gtest/gtest.h"

using namespace std;
using namespace tlv;

enum {
    TYPE_BOOL = 1,
    TYPE_INT8,
    TYPE_UINT8,
    TYPE_INT16,
    TYPE_UINT16,
    TYPE_INT32,
    TYPE_UINT32,
    TYPE_INT64,
    TYPE_UINT64,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_CHARS,
    TYPE_CONSTCHARS,
    TYPE_STRING,
    TYPE_TLV,
    TYPE_TLVMAP
};

TEST(Tlv, Constructor) {
#define __tlv_impl_code(t, l, o, v, pv) \
    do { \
        Tlv *ptlv = new Tlv((t), l, (pv)); \
        EXPECT_TRUE(ptlv != NULL); \
        EXPECT_EQ((t), ptlv->Type()); \
        EXPECT_EQ(l, ptlv->Length()); \
        EXPECT_EQ((v), *((o)(ptlv->Value()))); \
    } while (0);

    struct X {
        bool b;
        int32_t i32;
        float f;
        char cstring[18];
        std::string str;
    } x = { 1, 128, 3.14f, "hi, tlv~i'm char*", "hi, tlv~i'm std::string"};

    __tlv_impl_code(TYPE_BOOL, sizeof(bool), bool*, x.b, &(x.b));
    __tlv_impl_code(TYPE_BOOL, sizeof(int32_t), int32_t*, x.i32, &(x.i32));
    __tlv_impl_code(TYPE_BOOL, sizeof(float), float*, x.f, &(x.f));
}

TEST(Tlv, OperatorEQ) {
    int v1 = 10, v2 = 10;
    Tlv x(TYPE_INT32, sizeof(v1), &v1);
    Tlv y(TYPE_INT32, sizeof(v2), &v2);
    EXPECT_TRUE(x == y);
}

TEST(TlvMap, SetBool) {
    TlvMap tlvmap;
    bool x = true, y = false;
    EXPECT_TRUE(tlvmap.Set(TYPE_BOOL, x));
    EXPECT_EQ(sizeof(int) * 2 + sizeof(bool), tlvmap.Length());
    EXPECT_TRUE(tlvmap.Get(TYPE_BOOL, y));
    EXPECT_EQ(x, y);
}

TEST(TlvMap, SetInt32) {
    TlvMap tlvmap;
    int x = 1000, y = 0;
    EXPECT_TRUE(tlvmap.Set(TYPE_INT32, x));
    EXPECT_EQ(sizeof(int) * 2 + sizeof(int), tlvmap.Length());
    EXPECT_TRUE(tlvmap.Get(TYPE_INT32, y));
    EXPECT_EQ(x, y);
}

TEST(TlvMap, SetFloat) {
    TlvMap tlvmap;
    float x = 3.14f, y = 0;
    EXPECT_TRUE(tlvmap.Set(TYPE_FLOAT, x));
    EXPECT_EQ(sizeof(int) * 2 + sizeof(float), tlvmap.Length());
    EXPECT_TRUE(tlvmap.Get(TYPE_FLOAT, y));
    EXPECT_EQ(x, y);
}

TEST(TlvMap, SetChars) {
    TlvMap tlvmap;
    const char* x = "hi, tlv~i'm char*";
    int len = strlen(x) + 1;
    char* y = new char[len];
    EXPECT_TRUE(tlvmap.Set(TYPE_CHARS, x));
    EXPECT_EQ(sizeof(int) * 2 + strlen(x) + 1, tlvmap.Length());
    EXPECT_TRUE(tlvmap.Get(TYPE_CHARS, y, len));
    EXPECT_STREQ(x, y);
}

TEST(TlvMap, SetString) {
    TlvMap tlvmap;
    std::string x = "hi, tlv~i'm char*";
    std::string y;
    EXPECT_TRUE(tlvmap.Set(TYPE_STRING, x));
    EXPECT_EQ(sizeof(int) * 2 + x.size() + 1, tlvmap.Length());
    EXPECT_TRUE(tlvmap.Get(TYPE_STRING, y));
    EXPECT_STREQ(x.c_str(), y.c_str());
}

TEST(TlvMap, SetAllArithmeticType) {
    TlvMap tlvmap;
    union X {
        bool b;
        int8_t i8;
        uint8_t ui8;
        int16_t i16;
        uint16_t ui16;
        int32_t i32;
        uint32_t ui32;
        int64_t i64;
        uint64_t ui64;
        float f;
        double lf;
    } x, y;

    std::size_t len =
        8 + sizeof(bool) +
        8 + sizeof(int8_t) +
        8 + sizeof(uint8_t) +
        8 + sizeof(int16_t) +
        8 + sizeof(uint16_t) +
        8 + sizeof(int32_t) +
        8 + sizeof(uint32_t) +
        8 + sizeof(int64_t) +
        8 + sizeof(uint64_t) +
        8 + sizeof(float) +
        8 + sizeof(double);

    x.b = true;

    EXPECT_TRUE(tlvmap.Set(TYPE_BOOL, x.b));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT8, x.i8));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT8, x.ui8));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT16, x.i16));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT16, x.ui16));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT32, x.i32));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT32, x.ui32));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT64, x.i64));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT64, x.ui64));
    EXPECT_TRUE(tlvmap.Set(TYPE_FLOAT, x.f));
    EXPECT_TRUE(tlvmap.Set(TYPE_DOUBLE, x.lf));

    EXPECT_EQ(len, tlvmap.Length());

    EXPECT_TRUE(tlvmap.Get(TYPE_BOOL, y.b));
    EXPECT_EQ(x.b, y.b);

    EXPECT_TRUE(tlvmap.Get(TYPE_INT8, y.i8));
    EXPECT_EQ(x.i8, y.i8);

    EXPECT_TRUE(tlvmap.Get(TYPE_UINT8, y.ui8));
    EXPECT_EQ(x.ui8, y.ui8);

    EXPECT_TRUE(tlvmap.Get(TYPE_INT16, y.i16));
    EXPECT_EQ(x.i16, y.i16);

    EXPECT_TRUE(tlvmap.Get(TYPE_UINT16, y.ui16));
    EXPECT_EQ(x.ui16, y.ui16);

    EXPECT_TRUE(tlvmap.Get(TYPE_INT32, y.i32));
    EXPECT_EQ(x.i32, y.i32);

    EXPECT_TRUE(tlvmap.Get(TYPE_UINT32, y.ui32));
    EXPECT_EQ(x.ui32, y.ui32);

    EXPECT_TRUE(tlvmap.Get(TYPE_INT64, y.i64));
    EXPECT_EQ(x.i64, y.i64);

    EXPECT_TRUE(tlvmap.Get(TYPE_UINT64, y.ui64));
    EXPECT_EQ(x.ui64, y.ui64);

    EXPECT_TRUE(tlvmap.Get(TYPE_FLOAT, y.f));
    EXPECT_EQ(x.f, y.f);

    EXPECT_TRUE(tlvmap.Get(TYPE_DOUBLE, y.lf));
    EXPECT_EQ(x.lf, y.lf);

}

TEST(TlvMap, OperatorEQ) {
    TlvMap x, y;
    EXPECT_TRUE(x == y);
}

TEST(TlvMap, OperatorNE) {
    TlvMap x, y;
    x.Set(TYPE_INT32, 1);
    EXPECT_TRUE(x != y);
}

TEST(TlvMap, Encode) {
    TlvMap tlvmap;
    union X {
        bool b;
        int32_t i32;
        float f;
    } x;

    x.b = true;

    std::size_t len = 0;
    unsigned char* buff = new unsigned char[1024];

#define __impl_code(t, l, v, pv) \
    do { \
        EXPECT_TRUE(tlvmap.Set(t, (v))); \
        int type = (t), length = (l); \
        memcpy(buff + len, &type, sizeof(type)); \
        memcpy(buff + len + sizeof(type), &length, sizeof(length)); \
        memcpy(buff + len + sizeof(type) + sizeof(length), (pv), length); \
        len += 8 + length; \
    } while (0); 

    __impl_code(TYPE_BOOL, sizeof(x.b), x.b, &(x.b));
    __impl_code(TYPE_INT32, sizeof(x.i32), x.i32, &(x.i32));
    __impl_code(TYPE_FLOAT, sizeof(x.f), x.f, &(x.f));

    char cstring[] = "hello, cstring";
    __impl_code(TYPE_CHARS, sizeof(cstring), cstring, cstring);

    const char* ccstring = "hello, const cstring";
    __impl_code(TYPE_CONSTCHARS, strlen(ccstring) + 1, ccstring, ccstring);

    std::string str = "hi, std::string";
    __impl_code(TYPE_STRING, str.size() + 1, str, str.c_str());

    EXPECT_TRUE(tlvmap.Encode());
    EXPECT_EQ(len, tlvmap.Length());
    EXPECT_EQ(0, std::memcmp(tlvmap.Buffer(), buff, len));
}

TEST(TlvMap, Decode) {
    TlvMap tlvmap;
    struct X {
        bool b;
        int32_t i32;
        float f;
    } y, x = {1, 128, 3.14f};

    EXPECT_TRUE(tlvmap.Set(TYPE_BOOL, x.b));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT32, x.i32));
    EXPECT_TRUE(tlvmap.Set(TYPE_FLOAT, x.f));
    EXPECT_TRUE(tlvmap.Encode());
    EXPECT_EQ(33u, tlvmap.Length());

    TlvMap that;
    EXPECT_TRUE(that.Decode(tlvmap.Buffer(), tlvmap.Length()));
    EXPECT_EQ(33u, tlvmap.Length());
    EXPECT_TRUE(tlvmap == that);
    EXPECT_TRUE(that.Get(TYPE_BOOL, y.b));
    EXPECT_TRUE(that.Get(TYPE_INT32, y.i32));
    EXPECT_TRUE(that.Get(TYPE_FLOAT, y.f));
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.i32, y.i32);
    EXPECT_EQ(x.f, y.f);

    TlvMap that2(tlvmap.Buffer(), tlvmap.Length());
    EXPECT_TRUE(that2.Decode());
    EXPECT_EQ(33u, that2.Length());
    EXPECT_TRUE(tlvmap == that2);
}





