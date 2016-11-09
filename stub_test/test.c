#include <relayout.h>
#include <stdio.h>
#include <assert.h>

int main() {
    CSSNodeRef r = CSSNodeNew();
    CSSNodeRef r2 = CSSNodeNew();
    CSSNodeInsertChild(r, r2, 0);
    CSSNodeStyleSetDirection(r, CSSDirectionLTR);
    assert(CSSNodeStyleGetDirection(r) == CSSDirectionLTR);

    // reset and check again
    CSSNodeReset(r);
    assert(CSSNodeStyleGetDirection(r) != CSSDirectionLTR);

    CSSNodeIsDirty(r);
    CSSNodeFree(r);
    CSSNodeFree(r2);
    printf("all good\n");
}
