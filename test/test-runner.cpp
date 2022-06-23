#include "gtest/gtest.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


// Uncomment to sanity check the leak detector
// #define CHECK_FOR_MEMORY_LEAKS
#ifdef CHECK_FOR_MEMORY_LEAKS
TEST(Checkout, SanityCheckLeakDetector)
{
    int* ptr = new int { 10 };
    ptr = nullptr;
}
#endif