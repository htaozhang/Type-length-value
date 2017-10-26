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

//TEST(TlvMap, Constructor) {
//    const char* x = "hi, tlv~i'm char*";
//    TlvMap tlvmap(x);
//}

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

TEST(TlvMap, Encode) {
    TlvMap tlvmap;
    EXPECT_TRUE(tlvmap.Set(TYPE_BOOL, true));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT8, INT8_MIN));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT8, UINT8_MAX));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT16, INT16_MIN));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT16, UINT16_MAX));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT32, INT_MIN));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT32, UINT_MAX));
    EXPECT_TRUE(tlvmap.Set(TYPE_INT64, 1LL));
    EXPECT_TRUE(tlvmap.Set(TYPE_UINT64, 1ULL));
    EXPECT_TRUE(tlvmap.Set(TYPE_FLOAT, 3.14F));
    EXPECT_TRUE(tlvmap.Set(TYPE_DOUBLE, 3.14L));
}
















