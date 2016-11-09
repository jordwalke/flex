#include <relayout.h>
#include <stdio.h>
#include <assert.h>

int main() {
    printf("****************************\n");
    printf("Starting poorman's unit test\n");

    CSSNodeRef r = CSSNodeNew();
    CSSNodeRef r2 = CSSNodeNew();
    CSSNodeInsertChild(r, r2, 0);
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
    printf("all good\n");
    printf("****************************\n");
}
