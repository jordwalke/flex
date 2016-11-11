#include <CSSLayout.h>
#include <stdio.h>
#include <assert.h>

void globalTest() {
    CSSNodeRef r = CSSNodeNew();
    CSSNodeRef r2 = CSSNodeNew();
    CSSNodeInsertChild(r, r2, 1);

    assert(CSSNodeGetChild(r, 1) == r2);
    CSSNodeStyleSetDirection(r, CSSDirectionLTR);
    assert(CSSNodeStyleGetDirection(r) == CSSDirectionLTR);

    // reset and check again
    CSSNodeReset(r);
    assert(CSSNodeStyleGetDirection(r) != CSSDirectionLTR);
    assert(CSSNodeGetInstanceCount() == 2);
    CSSNodeIsDirty(r);
    CSSNodeFree(r);
    CSSNodeFree(r2);

    assert(CSSNodeGetInstanceCount() == 0);
}
void align_items_stretch_test() {
    const CSSNodeRef root = CSSNodeNew();
    CSSNodeStyleSetWidth(root, 100);
    CSSNodeStyleSetHeight(root, 100);

    const CSSNodeRef root_child0 = CSSNodeNew();
    CSSNodeInsertChild(root, root_child0, 0);

    CSSNodeStyleSetHeight(root_child0, 10);

    assert(CSSNodeLayoutGetHeight(root_child0) == 10);

    assert(CSSNodeLayoutGetWidth(root_child0) == 100);

    CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);

    CSSNodeFreeRecursive(root);
}

void testInsertGet() {
    CSSNodeRef r = CSSNodeNew();
    CSSNodeRef r2 = CSSNodeNew();
    CSSNodeInsertChild(r, r2, 1);
    assert(CSSNodeGetChild(r, 1) == r2);
    CSSNodeFree(r);
    CSSNodeFree(r2);
}

void testBigInsert() {
    CSSNodeRef r = CSSNodeNew();
    CSSNodeRef r2 = CSSNodeNew();
    assert(CSSNodeChildCount(r) == 0);
    CSSNodeInsertChild(r, r2, 1);
    assert(CSSNodeChildCount(r) == 1);
    CSSNodeRemoveChild(r, r2);
    assert(CSSNodeChildCount(r) == 0);
    CSSNodeFree(r);
    CSSNodeFree(r2);
}

void testChildInsertRemove() {
    CSSNodeRef r = CSSNodeNew();
    CSSNodeRef r2 = CSSNodeNew();
    for (int i = 0; i < 16; i ++) {
        assert(CSSNodeChildCount(r) == i);
        CSSNodeInsertChild(r, r2, i);
    }
    CSSNodeFree(r);
    CSSNodeFree(r2);
}

int main() {
    printf("****************************\n");
    printf("Starting poorman's unit test\n");

    align_items_stretch_test();

    testInsertGet();
    globalTest();
    testChildInsertRemove ();
    testBigInsert ();

    printf("all good\n");
    printf("****************************\n");
}
