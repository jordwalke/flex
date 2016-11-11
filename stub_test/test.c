/* #include <CSSLayout.h> */
/* #include <stdio.h> */
/* #include <assert.h> */

/* void globalTest() { */
/*     CSSNodeRef r = CSSNodeNew(); */
/*     CSSNodeRef r2 = CSSNodeNew(); */
/*     CSSNodeInsertChild(r, r2, 1); */

/*     assert(CSSNodeGetChild(r, 1) == r2); */
/*     CSSNodeStyleSetDirection(r, CSSDirectionLTR); */
/*     assert(CSSNodeStyleGetDirection(r) == CSSDirectionLTR); */

/*     // reset and check again */
/*     CSSNodeReset(r); */
/*     assert(CSSNodeStyleGetDirection(r) != CSSDirectionLTR); */
/*     assert(CSSNodeGetInstanceCount() == 2); */
/*     CSSNodeIsDirty(r); */
/*     CSSNodeFree(r); */
/*     CSSNodeFree(r2); */

/*     assert(CSSNodeGetInstanceCount() == 0); */
/* } */
/* void align_items_stretch_test() { */
/*     const CSSNodeRef root = CSSNodeNew(); */
/*     CSSNodeStyleSetWidth(root, 100); */
/*     CSSNodeStyleSetHeight(root, 100); */

/*     const CSSNodeRef root_child0 = CSSNodeNew(); */
/*     CSSNodeInsertChild(root, root_child0, 0); */

/*     CSSNodeStyleSetHeight(root_child0, 10); */

/*     assert(CSSNodeLayoutGetHeight(root_child0) == 10); */

/*     assert(CSSNodeLayoutGetWidth(root_child0) == 100); */

/*     CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR); */

/*     CSSNodeFreeRecursive(root); */
/* } */

/* void testInsertGet() { */
/*     CSSNodeRef r = CSSNodeNew(); */
/*     CSSNodeRef r2 = CSSNodeNew(); */
/*     CSSNodeInsertChild(r, r2, 1); */
/*     assert(CSSNodeGetChild(r, 1) == r2); */
/*     CSSNodeFree(r); */
/*     CSSNodeFree(r2); */
/* } */

/* void testBigInsert() { */
/*     CSSNodeRef r = CSSNodeNew(); */
/*     CSSNodeRef r2 = CSSNodeNew(); */
/*     assert(CSSNodeChildCount(r) == 0); */
/*     CSSNodeInsertChild(r, r2, 1); */
/*     assert(CSSNodeChildCount(r) == 1); */
/*     CSSNodeRemoveChild(r, r2); */
/*     assert(CSSNodeChildCount(r) == 0); */
/*     CSSNodeFree(r); */
/*     CSSNodeFree(r2); */
/* } */

/* void testChildInsertRemove() { */
/*     CSSNodeRef r = CSSNodeNew(); */
/*     CSSNodeRef r2 = CSSNodeNew(); */
/*     for (int i = 0; i < 16; i ++) { */
/*         assert(CSSNodeChildCount(r) == i); */
/*         CSSNodeInsertChild(r, r2, i); */
/*     } */
/*     CSSNodeFree(r); */
/*     CSSNodeFree(r2); */
/* } */

/* int main() { */
/*     printf("****************************\n"); */
/*     printf("Starting poorman's unit test\n"); */

/*     align_items_stretch_test(); */

/*     testInsertGet(); */
/*     globalTest(); */
/*     testChildInsertRemove (); */
/*     testBigInsert (); */

/*     printf("all good\n"); */
/*     printf("****************************\n"); */
/* } */

#include "this/package/foo.h"
#include "gtest/gtest.h"

namespace {

    // The fixture for testing class Foo.
    class FooTest : public ::testing::Test {
         protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        FooTest() {
            // You can do set-up work for each test here.
        }

        virtual ~FooTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for Foo.
    };

    // Tests that the Foo::Bar() method does Abc.
    TEST_F(FooTest, MethodBarDoesAbc) {
        const string input_filepath = "this/package/testdata/myinputfile.dat";
        const string output_filepath = "this/package/testdata/myoutputfile.dat";
        Foo f;
        EXPECT_EQ(0, f.Bar(input_filepath, output_filepath));
    }

    // Tests that Foo does Xyz.
    TEST_F(FooTest, DoesXyz) {
        // Exercises the Xyz feature of Foo.
    }

}  // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
