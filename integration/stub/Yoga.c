#include <caml/mlvalues.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/fail.h>
#include <caml/callback.h>
#include <caml/printexc.h>
#include <caml/threads.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
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


#define CAMLdrop caml_local_roots = caml__frame

#define REreturn0 do{ \
        CAMLdrop;     \
        reunlock();   \
        return;       \
    }while (0)

#define REreturnT(type, result) do{ \
        type caml__temp_result = (result);      \
        CAMLdrop;                               \
        reunlock();                             \
        return caml__temp_result;               \
    }while(0)

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

value re_callback (value closure, value arg1, const char * tag) {
    CAMLparam2 (closure, arg1);
    CAMLlocal1 (res);
    __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag);
    res = caml_callback(closure, arg1);
    __android_log_write(ANDROID_LOG_ERROR, "REASONEND", tag);
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    CAMLreturn (res);
}

value re_callback2 (value closure, value arg1, value arg2, const char * tag) {
    CAMLparam3 (closure, arg1, arg2);
    CAMLlocal1 (res);
    __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag);
    res = caml_callback2(closure, arg1, arg2);
    __android_log_write(ANDROID_LOG_ERROR, "REASONSEND", tag);
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    CAMLreturn (res);
}

value re_callback3 (value closure, value arg1, value arg2,
                    value arg3, const char * tag) {
    CAMLparam4 (closure, arg1, arg2, arg3);
    CAMLlocal1 (res);
    __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag);
    res = caml_callback3(closure, arg1, arg2, arg3);
    __android_log_write(ANDROID_LOG_ERROR, "REASONEND", tag);
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    CAMLreturn (res);
}

value re_callback4 (value closure, value arg1, value arg2,
                    value arg3, value arg4, const char * tag) {
    CAMLparam5 (closure, arg1, arg2, arg3, arg4);
    CAMLlocal1 (res);
    value arg[4];
    arg[0] = arg1;
    arg[1] = arg2;
    arg[2] = arg3;
    arg[3] = arg4;
    __android_log_write(ANDROID_LOG_ERROR, "REASONSTART", tag);
    res = caml_callbackN_exn(closure, 4, arg);
    __android_log_write(ANDROID_LOG_ERROR, "REASONEND", tag);
    if (Is_exception_result(res)) {
        char *exn = caml_format_exception(Extract_exception(res));
        char explain [50];
        sprintf(explain, "%s: %s", tag, exn);
        __android_log_write(ANDROID_LOG_ERROR, "REASONEXN", explain);
        caml_raise(Extract_exception(res));
    }
    CAMLreturn (res);
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

volatile int lock = 0;

value Min_int;
__attribute__ ((__constructor__))
void init(void) {
    char *caml_argv[1] = { NULL };
    caml_startup(caml_argv);
    camlMethodWithName(initThread, "initThread");
    re_callback(*initThread, Val_unit, "initThread");

    camlMethodWithName(minInt, "minInt");
    Min_int = re_callback(*minInt, Val_unit, __func__);
    caml_release_runtime_system();
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

value thread_initialize(value unit);

static void relock(const char * tag) {
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "registering");
    caml_c_thread_register();
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "done registering");
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "locking");
    __android_log_write(ANDROID_LOG_ERROR, "REASON", tag);
    while(__sync_lock_test_and_set(&lock, 1)) {}
    /* caml_acquire_runtime_system(); */
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "got locking");
}

static void reunlock() {
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "unlocking");
    // read/write barrier
    __sync_synchronize();
    lock = 0;
    /* caml_release_runtime_system(); */
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "got unlocking");
}

YGNodeRef YGNodeNew(void) {
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "registering");
    caml_c_thread_register();
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "registered");
    relock(__func__);
    CAMLparam0();
    CAMLlocal2(v, selfRef);
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "getting lock");
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "got lock");
    value *valp;
    valp = (value *) malloc(sizeof *valp);
    selfRef = caml_copy_nativeint((intnat)valp);
    camlMethod(closure);
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "entering");
    v = re_callback(*closure, selfRef, __func__);
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "leaving");

    *valp = v;
    gNodeInstanceCount++;
    // Register the value with global heap
    caml_register_global_root(valp);
    __android_log_write(ANDROID_LOG_ERROR, "REASON", "done");
    REreturnT(value *, valp);
}

void YGNodeReset(const YGNodeRef node){
    // - Create a new, dummy node and assign it to the pointer
    // - Old value should automatically be GCed
    // - No need to call caml_register_global_root again as the pointer remain the same
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethodWithName(closure, "YGNodeNew");
    v = caml_copy_nativeint((intnat)node);
    *node = re_callback(*closure, v, __func__);
    REreturn0;
}

void YGNodeSetMeasureFunc(const YGNodeRef node, YGMeasureFunc measureFunc) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = caml_copy_nativeint((intnat)measureFunc);
    re_callback2(*closure, *node, v, __func__);
    REreturn0;
}

YGMeasureFunc YGNodeGetMeasureFunc(const YGNodeRef node) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, *node, __func__);
    YGMeasureFunc mf = (YGMeasureFunc)Nativeint_val(v);
    REreturnT(YGMeasureFunc, mf);
}

void YGNodeSetHasNewLayout(const YGNodeRef node, bool hasNewLayout) {
    relock(__func__);
    camlMethod(closure);
    re_callback(*closure, Val_int(hasNewLayout), __func__);
    reunlock();
}

void YGNodeSetContext(const YGNodeRef node, void *context) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = caml_copy_nativeint((intnat)context);
    re_callback2(*closure, *node, v, __func__);
    REreturn0;
}

void *YGNodeGetContext(const YGNodeRef node) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, *node, __func__);
    void * ret = (void *)Nativeint_val(v);
    REreturnT(void *, ret);
}

bool YGNodeGetHasNewLayout(const YGNodeRef node) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, *node, __func__);
    REreturnT(bool, Int_val(v));
}

static YGNodeRef YGNodeGetSelfRef(value node) {
    relock(__func__);
    CAMLparam1(node);
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, node, __func__);
    YGNodeRef ret = (YGNodeRef)Nativeint_val(v);
    REreturnT(YGNodeRef, ret);
}

int32_t YGNodeGetInstanceCount(void) {
    return gNodeInstanceCount;
}

void YGNodeInit(const YGNodeRef node) {
    // all objects from ocaml are already inited. This is an noop
    return;
}

void YGNodeFree(const YGNodeRef node) {
    relock(__func__);
    gNodeInstanceCount--;
    // deregister the value with global heap
    caml_remove_global_root(node);
    reunlock();
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
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback3(*closure, *node, *child, Val_int(index), __func__);
    REreturn0;
}

void YGNodeRemoveChild(const YGNodeRef node,
                       const YGNodeRef child) {
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback2(*closure, *node, *child, __func__);
    REreturn0;
}

uint32_t YGNodeChildCount(const YGNodeRef node) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, *node, __func__);
    REreturnT(uint32_t, (uint32_t)Int_val(v));
}

void YGNodeCalculateLayout(const YGNodeRef node,
                           const float availableWidth,
                           const float availableHeight,
                           const YGDirection parentDirection) {
    /* camlMethodWithName(initThread, "initThread"); */
    /* re_callback(*initThread, Val_unit, __func__); */
    relock(__func__);
    CAMLparam0();
    CAMLlocal2(width, height);
    width = floatToCamlVal(availableWidth);
    height = floatToCamlVal(availableHeight);
    camlMethod(closure);
    re_callback4(*closure, *node, width, height,
                 YGDirectionToCamlVal(parentDirection), __func__);
    REreturn0;
}

YGNodeRef YGNodeGetChild(const YGNodeRef node,
                         const uint32_t index) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback2(*closure,
                     *node, Val_int(index), __func__);
    YGNodeRef ret = (YGNodeRef)Nativeint_val(v);
    REreturnT(YGNodeRef, ret);
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
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback(*closure, *node, __func__);
    REreturn0;
}

void YGNodeCopyStyle(const YGNodeRef destNode,
                     const YGNodeRef srcNode) {
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback2(*closure, *destNode, *srcNode, __func__);
    REreturn0;
}

bool YGNodeIsDirty(const YGNodeRef node) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, *node, __func__);
    REreturnT(bool, Bool_val(v));
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
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
    REreturn0;
}

void YGNodeStyleSetPosition(const YGNodeRef node, YGEdge edge, float v) {
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
    REreturn0;
}

void YGNodeStyleSetMargin(const YGNodeRef node, YGEdge edge, float v) {
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
    REreturn0;
}

void YGNodeStyleSetBorder(const YGNodeRef node, YGEdge edge, float v) {
    relock(__func__);
    CAMLparam0();
    camlMethod(closure);
    re_callback3(*closure, *node, YGEdgeToCamlVal(edge), floatToCamlVal(v), __func__);
    REreturn0;
}


float YGNodeStyleGetPadding(const YGNodeRef node, YGEdge edge) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__);
    float ret = CamlValTofloat(v);
    REreturnT(float, ret);
}

float YGNodeStyleGetMargin(const YGNodeRef node, YGEdge edge) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__);
    float ret = CamlValTofloat(v);
    REreturnT(float, ret);
}

float YGNodeStyleGetPosition(const YGNodeRef node, YGEdge edge) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__);
    float ret = CamlValTofloat(v);
    REreturnT(float, ret);
}

float YGNodeStyleGetBorder(const YGNodeRef node, YGEdge edge) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback2(*closure, *node, YGEdgeToCamlVal(edge), __func__);
    float ret = CamlValTofloat(v);
    REreturnT(float, ret);
}

/* Style */
#define defineNodeStyle(type, name)                                     \
    void YGNodeStyleSet##name(const YGNodeRef node, const type v) {     \
        relock(__func__);                                               \
        camlMethod(closure);                                            \
        re_callback2(*closure, *node, type##ToCamlVal(v), __func__);    \
        reunlock(__func__);                                             \
    }                                                                   \
    type YGNodeStyleGet##name(const YGNodeRef node) {                   \
        relock(__func__);                                               \
        camlMethod(closure);                                            \
        value v = re_callback(*closure, *node, __func__);               \
        reunlock(__func__);                                             \
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
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, *node, __func__);
    float ret = CamlValTofloat(v);
    REreturnT(float, ret);
}

float YGNodeLayoutGetHeight(const YGNodeRef node) {
    relock(__func__);
    CAMLparam0();
    CAMLlocal1(v);
    camlMethod(closure);
    v = re_callback(*closure, *node, __func__);
    float ret = CamlValTofloat(v);
    REreturnT(float, ret);
}

float YGNodeLayoutGetTop(const YGNodeRef node) {
    relock(__func__);
    camlMethod(closure);
    value v = re_callback(*closure, *node, __func__);
    reunlock();
    return CamlValTofloat(v);
}

float YGNodeLayoutGetBottom(const YGNodeRef node) {
    relock(__func__);
    camlMethod(closure);
    value v = re_callback(*closure, *node, __func__);
    reunlock();
    return CamlValTofloat(v);
}

float YGNodeLayoutGetLeft(const YGNodeRef node) {
    relock(__func__);
    camlMethod(closure);
    value v = re_callback(*closure, *node, __func__);
    reunlock();
    return CamlValTofloat(v);
}

float YGNodeLayoutGetRight(const YGNodeRef node) {
    relock(__func__);
    camlMethod(closure);
    value v = re_callback(*closure, *node, __func__);
    reunlock();
    return CamlValTofloat(v);
}

YGDirection YGNodeLayoutGetDirection(const YGNodeRef node) {
    relock(__func__);
    camlMethod(closure);
    value v = re_callback(*closure, *node, __func__);
    reunlock();
    return CamlValToYGDirection(v);

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
    reunlock();

    YGSize s = f(ref,
                 CamlValTofloat(w),
                 CamlValToYGMeasureMode(wm),
                 CamlValTofloat(h),
                 CamlValToYGMeasureMode(hm));
    relock(__func__);

    camlMethodWithName(getMeasurement, "GetMeasurement");

    width = floatToCamlVal(s.width);
    height = floatToCamlVal(s.height);
    CAMLreturn(re_callback2(*getMeasurement, width, height, __func__));
}


CAMLprim value cssMeasureFFI_bytecode(value * argv, int argn) {
    return cssMeasureFFI(argv[0], argv[1], argv[2], argv[3], argv[4]);
}
