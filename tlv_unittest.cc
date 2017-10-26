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
}

TEST(Tlv, GenerateInt32) {
    int x = 100;
    Tlv* ptlv = Tlv::Generate(TYPE_INT32, sizeof(int), x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_INT32, ptlv->Type());
    EXPECT_EQ(sizeof(int), ptlv->Length());
    EXPECT_EQ(x, *((int*)(ptlv->Value())));
}

TEST(Tlv, GenerateFloat) {
    float x = 3.14f;
    Tlv* ptlv = Tlv::Generate(TYPE_FLOAT, sizeof(float), x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_FLOAT, ptlv->Type());
    EXPECT_EQ(sizeof(float), ptlv->Length());
    EXPECT_EQ(x, *((float*)(ptlv->Value())));
}

TEST(Tlv, GenerateChars) {
    const char* x = "hi, tlv~i'm char*";
    Tlv* ptlv = Tlv::Generate(TYPE_CHARS, strlen(x) + 1, x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_CHARS, ptlv->Type());
    EXPECT_EQ(strlen(x) + 1, ptlv->Length());
    EXPECT_STREQ(x, (char*)(ptlv->Value()));
}

TEST(Tlv, GenerateString) {
    std::string x = "hi, tlv~i'm std::string";
    Tlv* ptlv = Tlv::Generate(TYPE_STRING, 0, x);
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_STRING, ptlv->Type());
    EXPECT_EQ(x.size() + 1, ptlv->Length());
    EXPECT_STREQ(x.c_str(), (char*)(ptlv->Value()));
}

TEST(Tlv, GenerateTlv) {
    std::string x = "hi, tlv~i'm tlv";
    Tlv* ptlv = Tlv::Generate(TYPE_TLV, 0, *(Tlv::Generate(TYPE_STRING, 0, x)));
    EXPECT_TRUE(ptlv != NULL);
    EXPECT_EQ(TYPE_TLV, ptlv->Type());
    EXPECT_EQ(x.size() + 1, ptlv->Length());
    EXPECT_STREQ(x.c_str(), (char*)(ptlv->Value()));
}

//TEST(TlvMap, Constructor) {
//    const char* x = "hi, tlv~i'm char*";
//    TlvMap tlvmap(x);
//}

//TEST(TlvMap, SetInt32) {
//    TlvMap tlvmap;
//    int x = 1000, y = TYPE_INT32;
//    tlvmap.Set(TYPE_INT32, x);
//    EXPECT_EQ(sizeof(int) * 2 + sizeof(int), tlvmap.Length());
    //printf("------------------------%d\n", memcmp((void*)&y, (void*)(tlvmap.Buffer()), 4));
    //EXPECT_EQ(0, memcmp((void*)&y, (void*)(tlvmap.Buffer()), 4));
//}



