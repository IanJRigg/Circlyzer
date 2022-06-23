#include "gtest/gtest.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


/**********************************************************************************************//**
 * The following is an example of a circular dependency that causes a memory leak when implemented
 * with std::shared_ptr's. When the two standalone pointers leave the test scope, they are
 * destructed as expected. This destruction reduces the reference count on each pointer by 1. 
 * Critically, each reference count was initially 2, due to the circular pointing. Because of this,
 * the two underlying structs both remain in memory with their pointers each referencing each other
 *************************************************************************************************/
//#define GIVE_ME_MEMORY_LEAKS // Uncomment to sanity check the leak detector
#ifdef GIVE_ME_MEMORY_LEAKS
#include <memory>

struct B;
struct A
{
    std::shared_ptr<B> ptr;
};

struct B
{
    std::shared_ptr<A> ptr;
};

TEST(Checkout, SanityCheckLeakDetector)
{
    auto a = std::make_shared<A>();
    auto b = std::make_shared<B>();

    a->ptr = b;
    b->ptr = a;
}

#endif
