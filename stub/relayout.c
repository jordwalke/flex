#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <assert.h>
#include <stdio.h>
#include "relayout.h"

#define assert__(x) for ( ; !(x) ; assert(x) )

// Look for a ocaml method with the same name as function name, and memorize it
#define camlMethod(x) \
    camlMethodWithName(x, __func__)

#define camlMethodWithName(x, name) \
    static value * x = NULL; \
    if (x == NULL) x = caml_named_value(name); \
    assert__(closure) { \
        printf("FATAL: function %s not implemented in OCaml, check bindings.re\n", name);  \
    };

static int32_t gNodeInstanceCount = 0;

CSSNodeRef CSSNodeNew(void) {
    CAMLparam0();
    CAMLlocal1(v);
    value *valp;
    camlMethod(closure);
    v = caml_callback(*closure, Val_unit);
    valp = (value *) malloc(sizeof *valp);
    *valp = v;
    gNodeInstanceCount++;
    // Register the value with global heap
    caml_register_global_root(valp);
    CAMLreturnT(value *, valp);
}

void CSSNodeReset(const CSSNodeRef node){
    // - Create a new, dummy node and assign it to the pointer
    // - Old value should automatically be GCed
    // - No need to call caml_register_global_root again as the pointer remain the same
    camlMethodWithName(closure, "CSSNodeNew");
    *node = caml_callback(*closure, Val_unit);
}

void CSSNodeInit(const CSSNodeRef node) {
    // all objects from ocaml are already inited. This is an noop
    return;
}

void CSSNodeFree(const CSSNodeRef node) {
    gNodeInstanceCount--;
    // deregister the value with global heap
    caml_remove_global_root(node);
    free(node);
}

// On the contract, the ownership of a node always belong to the creator.
// This function, however assumes the ownership of a node belong to the tree.
// We have this function mostly for convenience purpose in unit tests.
void CSSNodeFreeRecursive(const CSSNodeRef node) {
    // deregister the value with global heap, children of this object still need to be freed
    // by its owner.
    CSSNodeFree(node);
}

void CSSNodeInsertChild(const CSSNodeRef node,
                        const CSSNodeRef child,
                        const uint32_t index) {
    // We have no local ocaml allocation here, so no need for CAMLparam/CAMLreturn/etc
    camlMethod(closure);
    caml_callback3(*closure, *node, *child, Val_int(index));
    return;
}

void CSSNodeStyleSetDirection(const CSSNodeRef node, const CSSDirection direction) {
    // We have no local ocaml allocation here, so no need for CAMLparam/CAMLreturn/etc
    camlMethod(closure);
    caml_callback2(*closure, *node, Val_int(direction));
    return;
}

CSSDirection CSSNodeStyleGetDirection(const CSSNodeRef node) {
    camlMethod(closure);
    value v = caml_callback(*closure, *node);
    return Int_val(v);
}

void CSSNodeMarkDirty(const CSSNodeRef node) {
    assert__(false) {
        printf("Not implemented in OCaml\n");
    }
}

bool CSSNodeIsDirty(const CSSNodeRef node) {
    camlMethod(closure);
    return Bool_val(caml_callback(*closure, *node));
}
