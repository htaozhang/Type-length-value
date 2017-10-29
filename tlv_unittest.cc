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
    TYPE_STRING,
    TYPE_TLV,
    TYPE_TLVMAP
};

TEST(Tlv, Constructor) {
    int x = 10;
    Tlv *ptlv = new Tlv(TYPE_INT32, sizeof(int), &x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_INT32, ptlv->Type());
    EXPECT_EQ(sizeof(int), ptlv->Length());
    EXPECT_EQ(x, *((int*)(ptlv->Value())));
}

TEST(Tlv, GenerateBool) {
    bool x = true;
    Tlv* ptlv = Tlv::Generate(TYPE_BOOL, sizeof(bool), x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_BOOL, ptlv->Type());
    EXPECT_EQ(sizeof(bool), ptlv->Length());
    EXPECT_EQ(x, *((bool*)(ptlv->Value())));
    EXPECT_EQ(0, memcmp(&x, ptlv->Value(), sizeof(bool)));
}

TEST(Tlv, GenerateInt32) {
    int x = 100;
    Tlv* ptlv = Tlv::Generate(TYPE_INT32, sizeof(int), x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_INT32, ptlv->Type());
    EXPECT_EQ(sizeof(int), ptlv->Length());
    EXPECT_EQ(x, *((int*)(ptlv->Value())));
    EXPECT_EQ(0, memcmp(&x, ptlv->Value(), sizeof(int)));
}

TEST(Tlv, GenerateFloat) {
    float x = 3.14f;
    Tlv* ptlv = Tlv::Generate(TYPE_FLOAT, sizeof(float), x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_FLOAT, ptlv->Type());
    EXPECT_EQ(sizeof(float), ptlv->Length());
    EXPECT_EQ(x, *((float*)(ptlv->Value())));
    EXPECT_EQ(0, memcmp(&x, ptlv->Value(), sizeof(float)));
}

TEST(Tlv, GenerateChars) {
    const char* x = "hi, tlv~i'm char*";
    Tlv* ptlv = Tlv::Generate(TYPE_CHARS, strlen(x) + 1, x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_CHARS, ptlv->Type());
    EXPECT_EQ(strlen(x) + 1, ptlv->Length());
    EXPECT_STREQ(x, (char*)(ptlv->Value()));
    EXPECT_EQ(0, memcmp(x, ptlv->Value(), strlen(x) + 1));
}

TEST(Tlv, GenerateString) {
    std::string x = "hi, tlv~i'm std::string";
    Tlv* ptlv = Tlv::Generate(TYPE_STRING, 0, x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_STRING, ptlv->Type());
    EXPECT_EQ(x.size() + 1, ptlv->Length());
    EXPECT_STREQ(x.c_str(), (char*)(ptlv->Value()));
    EXPECT_EQ(0, memcmp(x.c_str(), ptlv->Value(), x.size() + 1));
}

TEST(Tlv, GenerateTlv) {
    std::string x = "hi, tlv~i'm tlv";
    Tlv* ptlv = Tlv::Generate(TYPE_TLV, 0, *(Tlv::Generate(TYPE_STRING, 0, x)));
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_TLV, ptlv->Type());
    EXPECT_EQ(x.size() + 1, ptlv->Length());
    EXPECT_STREQ(x.c_str(), (char*)(ptlv->Value()));
    EXPECT_EQ(0, memcmp(x.c_str(), ptlv->Value(), x.size() + 1));
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

    bool setbool = true;
    bool setint32 = true;
    bool setfloat = true;
    x.b = true;

    std::size_t len = 0;
    unsigned char* buff = new unsigned char[1024];

    if (setbool) {
        EXPECT_TRUE(tlvmap.Set(TYPE_BOOL, x.b));
        int type = TYPE_BOOL, length = sizeof(x.b);
        memcpy(buff + len, &type, sizeof(type));
        memcpy(buff + len + sizeof(type), &length, sizeof(length));
        memcpy(buff + len + sizeof(type) + sizeof(length), &(x.b), sizeof(x.b));
        len += 8 + sizeof(x.b);
    }

    if (setint32) {
        EXPECT_TRUE(tlvmap.Set(TYPE_INT32, x.i32));
        int type = TYPE_INT32, length = sizeof(x.i32);
        memcpy(buff + len, &type, sizeof(type));
        memcpy(buff + len + sizeof(type), &length, sizeof(length));
        memcpy(buff + len + sizeof(type) + sizeof(length), &(x.i32), sizeof(x.i32));
        len += 8 + sizeof(x.i32);
    }

    if (setfloat) {
        EXPECT_TRUE(tlvmap.Set(TYPE_FLOAT, x.f));
        int type = TYPE_FLOAT, length = sizeof(x.f);
        memcpy(buff + len, &type, sizeof(type));
        memcpy(buff + len + sizeof(type), &length, sizeof(length));
        memcpy(buff + len + sizeof(type) + sizeof(length), &(x.f), sizeof(x.f));
        len += 8 + sizeof(x.f);
    }

    EXPECT_TRUE(tlvmap.Encode());
    EXPECT_EQ(len, tlvmap.Length());
    EXPECT_EQ(0, memcmp(tlvmap.Buffer(), buff, len));
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
    //EXPECT_TRUE(tlvmap == that);
    /*
    EXPECT_TRUE(that.Get(TYPE_BOOL, y.b));
    EXPECT_TRUE(that.Get(TYPE_INT32, y.i32));
    EXPECT_TRUE(that.Get(TYPE_FLOAT, y.f));
    EXPECT_EQ(x.b, y.b);
    EXPECT_EQ(x.i32, y.i32);
    EXPECT_EQ(x.f, y.f);
    */
    //TlvMap that2(tlvmap.Buffer(), tlvmap.Length());
    //EXPECT_TRUE(that2.Decode());
    //EXPECT_TRUE(tlvmap == that2);
}










