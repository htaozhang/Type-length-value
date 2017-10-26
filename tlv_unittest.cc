#include <stdio.h>
#include <iostream>

#include "tlv.h"
#include "gtest/gtest.h"

using namespace std;
using namespace tlv;


TEST(Tlv, Constructor) {
    int value = 10;
    Tlv *ptlv = new Tlv(1, sizeof(int), &value);
    EXPECT_EQ(1, ptlv->Type());
    EXPECT_EQ(4, ptlv->Length());
    EXPECT_EQ(10, *((int*)ptlv->Value()));
}





