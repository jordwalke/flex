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

void testInsertGet() {
    CSSNodeRef r = CSSNodeNew();
    CSSNodeRef r2 = CSSNodeNew();
    CSSNodeInsertChild(r, r2, 1);
    assert(CSSNodeGetChild(r, 1) == r2);
    CSSNodeFree(r);
    CSSNodeFree(r2);
}

int main() {
    printf("****************************\n");
    printf("Starting poorman's unit test\n");
    testInsertGet();
    globalTest();

    printf("all good\n");
    printf("****************************\n");
}
