#include "unique_ptr_test.hh"

TEST_F(UniquePtrTest, ConstructorTest) {
    EXPECT_EQ(*unique_ptr.raw_ptr, 5);
}