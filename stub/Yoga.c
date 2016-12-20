#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>

#include <assert.h>
#include <stdio.h>
#include "Yoga.h"

/* #include <android/log.h> */
#define __android_log_write(x, y, z)

#define assert__(x) for ( ; !(x) ; assert(x) )

// Look for a ocaml method with the same name as function name, and memorize it
#define camlMethod(x) \
    camlMethodWithName(x, __func__)

#define camlMethodWithName(x, name) \
    static value * x = NULL; \
    if (x == NULL) x = caml_named_value(name); \
    assert__(x) { \
        printf("FATAL: function %s not implemented in OCaml, check bindings.re\n", name);  \
    };

char* itoa(uintnat val, int base){

    static char buf[65] = {0};

    int i = 64;

    for(; i ; --i, val /= base)

        buf[i] = "0123456789abcdef"[val % base];

    return &buf[i+1];

}

/* static void printBinary(uintnat i) { */
/*     char *buffer = itoa (i,2); */
/*     printf ("binary: %s\n",buffer); */
/* } */

static bool inCall = false;

value re_callback (value closure, value arg1, const char * tag) {
    value arg[1];
    arg[0] = arg1;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag); */
    inCall = true;
    value res = caml_callbackN_exn(closure, 1, arg);
    inCall = false;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONEND", tag); */
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    return res;
}

value re_callback2 (value closure, value arg1, value arg2, const char * tag) {
    value arg[2];
    arg[0] = arg1;
    arg[1] = arg2;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag); */
    inCall = true;
    value res = caml_callbackN_exn(closure, 2, arg);
    inCall = false;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONSEND", tag); */
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    return res;
}

value re_callback3 (value closure, value arg1, value arg2,
                    value arg3, const char * tag) {
    value arg[3];
    arg[0] = arg1;
    arg[1] = arg2;
    arg[2] = arg3;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag); */
    inCall = true;
    value res = caml_callbackN_exn(closure, 3, arg);
    inCall = false;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONEND", tag); */
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    return res;
}

value re_callback4 (value closure, value arg1, value arg2,
                    value arg3, value arg4, const char * tag) {
    value arg[4];
    arg[0] = arg1;
    arg[1] = arg2;
    arg[2] = arg3;
    arg[3] = arg4;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag); */
    inCall = true;
    value res = caml_callbackN_exn(closure, 4, arg);
    inCall = false;
    /* __android_log_write(ANDROID_LOG_ERROR, "REASONEND", tag); */
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    return res;
}

static int32_t gNodeInstanceCount = 0;

inline bool YGValueIsUndefined(const float v) {
    return isnan(v);
}

#define bridgeEnumToCamlVal(type, offset)       \
    inline value type##ToCamlVal(const type v) {\
        return Val_int((int)v + offset);        \
    }                                           \
    inline type CamlValTo##type(value v) {      \
        return Int_val(v) - offset;             \
    }

bridgeEnumToCamlVal(YGAlign, 0)
bridgeEnumToCamlVal(YGJustify, 0)
bridgeEnumToCamlVal(YGDirection, 1)
bridgeEnumToCamlVal(YGEdge, 0)
bridgeEnumToCamlVal(YGFlexDirection, 0)
bridgeEnumToCamlVal(YGMeasureMode, 0)
bridgeEnumToCamlVal(YGWrap, 0)
bridgeEnumToCamlVal(YGPositionType, 0)
bridgeEnumToCamlVal(YGOverflow, 0)


value Min_int;
__attribute__ ((__constructor__))
void initMinInt(void) {
    camlMethodWithName(minInt, "minInt");
    Min_int = re_callback(*minInt, Val_unit, __func__);
}

inline value floatToCamlVal(const float v) {
    if (YGValueIsUndefined(v)) {
        return Min_int;
    }
    return Val_int(v * 100);
}

static float CamlValTofloat(value v) {
    if (v == Min_int) {
        return YGUndefined;
    }
    return Int_val(v) / 100;
}

#define scale_factor 100;

YGNodeRef YGNodeNew(void) {
    CAMLparam0();
    CAMLlocal2(v, selfRef);
    value *valp;
    valp = (value *) malloc(sizeof *valp);
    selfRef = caml_copy_nativeint((intnat)valp);
    camlMethod(closure);
    v = re_callback(*closure, selfRef, __func__);
    *valp = v;
    gNodeInstanceCount++;
    // Register the value with global heap
    caml_register_global_root(valp);
    CAMLreturnT(value *, valp);
}

void YGNodeReset(const YGNodeRef node){
    // - Create a new, dummy node and assign it to the pointer
    // - Old value should automatically be GCed
    // - No need to call caml_register_global_root again as the pointer remain the same
    camlMethodWithName(closure, "YGNodeNew");
    *node = re_callback(*closure, caml_copy_nativeint((intnat)node), __func__);
}

void YGNodeSetMeasureFunc(const YGNodeRef node, YGMeasureFunc measureFunc) {
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = caml_copy_nativeint((intnat)measureFunc);
    re_callback2(*closure, *node, v, __func__);
    CAMLreturn0;
}

YGMeasureFunc YGNodeGetMeasureFunc(const YGNodeRef node) {
    camlMethod(closure);
    return (YGMeasureFunc)Nativeint_val(re_callback(*closure, *node, __func__));
}

void YGNodeSetHasNewLayout(const YGNodeRef node, bool hasNewLayout) {
    camlMethod(closure);
    re_callback(*closure, Val_int(hasNewLayout), __func__);
}

void YGNodeSetContext(const YGNodeRef node, void *context) {
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = caml_copy_nativeint((intnat)context);
    re_callback2(*closure, *node, v, __func__);
    CAMLreturn0;
}

void *YGNodeGetContext(const YGNodeRef node) {
    if (node == NULL) {
        return NULL;
    }
    camlMethod(closure);
    return (void *)Nativeint_val(re_callback(*closure, *node, __func__));
}

bool YGNodeGetHasNewLayout(const YGNodeRef node) {
    return true;
    /* camlMethod(closure); */
    /* return Int_val(re_callback(*closure, *node, __func__)); */
}

static YGNodeRef YGNodeGetSelfRef(value node) {
    camlMethod(closure);
    return (YGNodeRef)Nativeint_val(re_callback(*closure, node, __func__));
}

int32_t YGNodeGetInstanceCount(void) {
    return gNodeInstanceCount;
}

void YGNodeInit(const YGNodeRef node) {
    // all objects from ocaml are already inited. This is an noop
    return;
}

void YGNodeFree(const YGNodeRef node) {
    gNodeInstanceCount--;
    // deregister the value with global heap
    caml_remove_global_root(node);
    free(node);
}

// On the contract, the ownership of a node always belong to the creator.
// This function, however assumes the ownership of a node belong to the tree.
// We have this function mostly for convenience purpose in unit tests.
void YGNodeFreeRecursive(const YGNodeRef root) {
    // deregister the value with global heap, children of this object still need to be freed
    // by its owner.
    while (YGNodeChildCount(root) > 0) {
        const YGNodeRef child = YGNodeGetChild(root, 0);
        YGNodeFreeRecursive(child);
        YGNodeRemoveChild(root, child);
    }
    YGNodeFree(root);
}

void YGNodeInsertChild(const YGNodeRef node,
                       const YGNodeRef child,
                       const uint32_t index) {
    // We have no local ocaml allocation here, so no need for CAMLparam/CAMLreturn/etc
    camlMethod(closure);
    re_callback3(*closure, *node, *child, Val_int(index), __func__);
    return;
}

void YGNodeRemoveChild(const YGNodeRef node,
                       const YGNodeRef child) {
    // We have no local ocaml allocation here, so no need for CAMLparam/CAMLreturn/etc
    CAMLparam0();
    CAMLlocal1(v);
    v = caml_copy_nativeint((intnat)child);
    camlMethod(closure);
    re_callback2(*closure, *node, v, __func__);
    CAMLreturn0;
}

uint32_t YGNodeChildCount(const YGNodeRef node) {
    camlMethod(closure);
    value v = re_callback(*closure, *node, __func__);
    return (uint32_t)Int_val(v);
}

void YGNodeCalculateLayout(const YGNodeRef node,
                            const float availableWidth,
                            const float availableHeight,
                            const YGDirection parentDirection) {
    CAMLparam0();
    CAMLlocal1(v);
    v = floatToCamlVal(availableWidth);
    camlMethod(closure);
    re_callback4(*closure, *node, floatToCamlVal(availableWidth),
                 floatToCamlVal(availableWidth),
                 YGDirectionToCamlVal(parentDirection), __func__);
    CAMLreturn0;
}

YGNodeRef YGNodeGetChild(const YGNodeRef node,
                           const uint32_t index) {
    // We have no local ocaml allocation here, so no need for CAMLparam/CAMLreturn/etc
    camlMethod(closure);
    return (YGNodeRef)Nativeint_val(re_callback2(*closure,
                                                 *node, Val_int(index), __func__));
}

void YGSetLogger(YGLogger logger) {
    // TODO: implement this
    return;
}

void YGLog(YGLogLevel level, const char *message, ...) {
    // TODO: implement this
    return;
}

void YGSetExperimentalFeatureEnabled(YGExperimentalFeature feature, bool enabled) {
    // TODO: implement this
    return;
}

bool YGIsExperimentalFeatureEnabled(YGExperimentalFeature feature) {
    // TODO: implement this
    return false;
}

void YGNodeMarkDirty(const YGNodeRef node) {
    // TODO: implement this
    return;
}

void YGNodeCopyStyle(const YGNodeRef destNode,
                     const YGNodeRef srcNode) {
    camlMethod(closure);
    re_callback2(*closure, *destNode, *srcNode, __func__);
    return;
}

bool YGNodeIsDirty(const YGNodeRef node) {
    return false;
    /* camlMethod(closure); */
    /* return Bool_val(re_callback(*closure, *node, __func__)); */
}

void YGNodePrint(const YGNodeRef node,
                  const YGPrintOptions options) {
    // TODO: implement this
    return;
}


void YGNodeSetPrintFunc(const YGNodeRef node, YGPrintFunc printfunc) {
    // TODO: implement this
    return;
}

YGPrintFunc YGNodeGetPrintFunc(const YGNodeRef node) {
    return 0;
}

bool YGNodeCanUseCachedMeasurement(const YGMeasureMode widthMode,
                                    const float width,
                                    const YGMeasureMode heightMode,
                                    const float height,
                                    const YGMeasureMode lastWidthMode,
                                    const float lastWidth,
                                    const YGMeasureMode lastHeightMode,
                                    const float lastHeight,
                                    const float lastComputedWidth,
                                    const float lastComputedHeight,
                                    const float marginRow,
                                    const float marginColumn) {
    // TODO: implement this
    return false;
}

/* Padding */
void YGNodeStyleSetPadding(const YGNodeRef node, YGEdge edge, float v) {
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
}

void YGNodeStyleSetPosition(const YGNodeRef node, YGEdge edge, float v) {
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
}

void YGNodeStyleSetMargin(const YGNodeRef node, YGEdge edge, float v) {
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
}

void YGNodeStyleSetBorder(const YGNodeRef node, YGEdge edge, float v) {
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
}


float YGNodeStyleGetPadding(const YGNodeRef node, YGEdge edge) {
    camlMethod(closure);
    return CamlValTofloat(re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__));
}

float YGNodeStyleGetMargin(const YGNodeRef node, YGEdge edge) {
    camlMethod(closure);
    return CamlValTofloat(re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__));
}

float YGNodeStyleGetPosition(const YGNodeRef node, YGEdge edge) {
    camlMethod(closure);
    return CamlValTofloat(re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__));
}

float YGNodeStyleGetBorder(const YGNodeRef node, YGEdge edge) {
    camlMethod(closure);
    return CamlValTofloat(re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__));
}

/* Style */
#define defineNodeStyle(type, name)                                     \
    void YGNodeStyleSet##name(const YGNodeRef node, const type v) {   \
        camlMethod(closure);                                            \
        re_callback2(*closure, *node, type##ToCamlVal(v), __func__);            \
        return;                                                         \
    }                                                                   \
    type YGNodeStyleGet##name(const YGNodeRef node) {                 \
        camlMethod(closure);                                            \
        value v = re_callback(*closure, *node, __func__);                       \
        return CamlValTo##type(v);                                      \
    }                                                                   \

/* Style */

defineNodeStyle(YGJustify, JustifyContent)

defineNodeStyle(YGAlign, AlignItems)

defineNodeStyle(YGAlign, AlignContent)

defineNodeStyle(YGAlign, AlignSelf)

defineNodeStyle(YGDirection, Direction)

defineNodeStyle(YGPositionType, PositionType)

defineNodeStyle(YGWrap, FlexWrap)

defineNodeStyle(YGFlexDirection, FlexDirection)

defineNodeStyle(YGOverflow, Overflow)

defineNodeStyle(float, Width);

defineNodeStyle(float, MaxWidth);

defineNodeStyle(float, MaxHeight);

defineNodeStyle(float, MinWidth);

defineNodeStyle(float, MinHeight);

defineNodeStyle(float, Height);

defineNodeStyle(float, FlexGrow);

defineNodeStyle(float, FlexShrink);

defineNodeStyle(float, FlexBasis);

defineNodeStyle(float, Flex);

void YGNodeStyleSetAspectRatio(const YGNodeRef node, float v) {
    // TODO: implement this
    return;
}

float YGNodeStyleGetAspectRatio(const YGNodeRef node) {
    // TODO: implement this
    return 1.f;
}


/* Layout */
float YGNodeLayoutGetWidth(const YGNodeRef node) {
    camlMethod(closure);
    return CamlValTofloat(re_callback(*closure, *node, __func__));
}

float YGNodeLayoutGetHeight(const YGNodeRef node) {
    camlMethod(closure);
    return CamlValTofloat(re_callback(*closure, *node, __func__));
}

float YGNodeLayoutGetTop(const YGNodeRef node) {
    camlMethod(closure);
    return CamlValTofloat(re_callback(*closure, *node, __func__));
}

float YGNodeLayoutGetBottom(const YGNodeRef node) {
    camlMethod(closure);
    return CamlValTofloat(re_callback(*closure, *node, __func__));
}

float YGNodeLayoutGetLeft(const YGNodeRef node) {
    camlMethod(closure);
    return CamlValTofloat(re_callback(*closure, *node, __func__));
}

float YGNodeLayoutGetRight(const YGNodeRef node) {
    camlMethod(closure);
    return CamlValTofloat(re_callback(*closure, *node, __func__));
}

YGDirection YGNodeLayoutGetDirection(const YGNodeRef node) {
    camlMethod(closure);
    return CamlValToYGDirection(re_callback(*closure, *node, __func__));
}

CAMLprim value logcat(value txt) {
  __android_log_write(ANDROID_LOG_ERROR, "REASON", String_val(txt));
  return Val_unit;
}

// This is a special case for OCaml functions that have more than 5 parameters, in such cases you have to provide 2 C functions

// This defines a stub api for ocaml to call back, it then passes control to the C function pointer
CAMLprim value cssMeasureFFI(value node, value w, value wm, value h, value hm) {
    CAMLparam5(node, w, wm, h, hm);
    CAMLlocal3(ptr, width, height);

    YGNodeRef ref = (YGNodeRef)Nativeint_val(node);

    camlMethodWithName(getMeasureFunc, "YGNodeGetMeasureFunc");

    ptr = re_callback(*getMeasureFunc, *ref, __func__);

    YGMeasureFunc f = (YGMeasureFunc)Nativeint_val(ptr);

    YGSize s = f(ref,
                  CamlValTofloat(w),
                  CamlValToYGMeasureMode(wm),
                  CamlValTofloat(h),
                  CamlValToYGMeasureMode(hm));

    camlMethodWithName(getMeasurement, "GetMeasurement");

    width = floatToCamlVal(s.width);
    height = floatToCamlVal(s.height);
    CAMLreturn(re_callback2(*getMeasurement, width, height, __func__));
}


CAMLprim value cssMeasureFFI_bytecode(value * argv, int argn) {
    return cssMeasureFFI(argv[0], argv[1], argv[2], argv[3], argv[4]);
}
