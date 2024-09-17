#include <iostream>
#include <gtest/gtest.h>
// #include "vector_util.h"

#define DEBUG_FLAG 1

class KVTest : public ::testing::Test
{
public:
    void SetUP()
    {
        std::cout << "KVTest::SetUP" << std::endl;
    }
    void TearDown()
    {
        std::cout << "KVTest::TearDown" << std::endl;
    }

    // static void SetUpTestCase()
    // {
    //     std::cout << "KVTest::SetUpTestCase" << std::endl;
    // }
    // static void TearDownTestCase()
    // {
    //     std::cout << "KVTest::TearDownTestCase" << std::endl;
    // }
};