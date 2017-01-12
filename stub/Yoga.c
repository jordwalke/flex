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
#include "ringbuf.h"


enum command_type {
 CommandYGNodeFree,
 CommandYGNodeFreeRecursive,
 CommandYGNodeReset,
 CommandYGNodeInsertChild,
 CommandYGNodeRemoveChild,
 CommandYGNodeCalculateLayout,
 CommandYGNodeMarkDirty,
 CommandYGNodeCopyStyle,
 CommandYGNodeSetContext,
 CommandYGNodeSetMeasureFunc,
 CommandYGNodeSetPrintFunc,
 CommandYGNodeSetHasNewLayout,
 CommandYGNodeStyleSetDirection,
 CommandYGNodeStyleSetFlexDirection,
 CommandYGNodeStyleSetJustifyContent,
 CommandYGNodeStyleSetAlignContent,
 CommandYGNodeStyleSetAlignItems,
 CommandYGNodeStyleSetAlignSelf,
 CommandYGNodeStyleSetPositionType,
 CommandYGNodeStyleSetFlexWrap,
 CommandYGNodeStyleSetOverflow,
 CommandYGNodeStyleSetFlex,
 CommandYGNodeStyleSetFlexGrow,
 CommandYGNodeStyleSetFlexShrink,
 CommandYGNodeStyleSetFlexBasis,
 CommandYGNodeStyleSetPosition,
 CommandYGNodeStyleSetMargin,
 CommandYGNodeStyleSetPadding,
 CommandYGNodeStyleSetBorder,
 CommandYGNodeStyleSetWidth,
 CommandYGNodeStyleSetHeight,
 CommandYGNodeStyleSetMinWidth,
 CommandYGNodeStyleSetMinHeight,
 CommandYGNodeStyleSetMaxWidth,
 CommandYGNodeStyleSetMaxHeight,
 CommandYGNodeStyleSetAspectRatio
} command_type_t;

struct command_t
{
    enum command_type cmd;
    void *arg1;
    void *arg2;
};

struct ringbuf_t
{
    struct command_t buf[BUF_SIZE_MINUS_ONE];
    struct command_t *head;
    struct command_t *tail;
    size_t size;
};



/**
 * The following license applies to section of this file that defines the
 * ringbuf data structure (not the layout algorithm or Yoga.c)
 */
/*
 * ringbuf.c - C ring buffer (FIFO) implementation.
 *
 * Written in 2011 by Drew Hess <dhess-src@bothan.net>.
 *
 * To the extent possible under law, the author(s) have dedicated all
 * copyright and related and neighboring rights to this software to
 * the public domain worldwide. This software is distributed without
 * any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication
 * along with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <assert.h>

/* [> One byte is used for detecting the full condition. <] */
static const size_t BUF_SIZE_MINUS_ONE = 999;

/*
 * The code is written for clarity, not cleverness or performance, and
 * contains many assert()s to enforce invariant assumptions and catch
 * bugs. Feel free to optimize the code and to remove asserts for use
 * in your own projects, once you're comfortable that it functions as
 * intended.
 */

/* ringbuf_t */
/* ringbuf_new(size_t capacity) */
/* { */
/*     ringbuf_t rb = malloc(sizeof(struct ringbuf_t)); */
/*     if (rb) { */

/*         [> One byte is used for detecting the full condition. <] */
/*         rb->size = capacity + 1; */
/*         rb->buf = malloc(rb->size); */
/*         if (rb->buf) */
/*             ringbuf_reset(rb); */
/*         else { */
/*             free(rb); */
/*             return 0; */
/*         } */
/*     } */
/*     return rb; */
/* } */

size_t
ringbuf_buffer_size(const struct ringbuf_t *rb)
{
    return rb->size;
}

void
ringbuf_reset(ringbuf_t rb)
{
    rb->head = rb->tail = rb->buf;
}

/* void */
/* ringbuf_free(ringbuf_t *rb) */
/* { */
/*     assert(rb && *rb); */
/*     free((*rb)->buf); */
/*     free(*rb); */
/*     *rb = 0; */
/* } */

size_t
ringbuf_capacity(const struct ringbuf_t *rb)
{
    return ringbuf_buffer_size(rb) - 1;
}

/*
 * Return a pointer to one-past-the-end of the ring buffer's
 * contiguous buffer. You shouldn't normally need to use this function
 * unless you're writing a new ringbuf_* function.
 */
static const struct command_t *
ringbuf_end(const struct ringbuf_t *rb)
{
    return rb->buf + ringbuf_buffer_size(rb);
}

size_t
ringbuf_bytes_free(const struct ringbuf_t *rb)
{
    if (rb->head >= rb->tail)
        return ringbuf_capacity(rb) - (rb->head - rb->tail);
    else
        return rb->tail - rb->head - 1;
}

size_t
ringbuf_bytes_used(const struct ringbuf_t *rb)
{
    return ringbuf_capacity(rb) - ringbuf_bytes_free(rb);
}

int
ringbuf_is_full(const struct ringbuf_t *rb)
{
    return ringbuf_bytes_free(rb) == 0;
}

int
ringbuf_is_empty(const struct ringbuf_t *rb)
{
    return ringbuf_bytes_free(rb) == ringbuf_capacity(rb);
}

const void *
ringbuf_tail(const struct ringbuf_t *rb)
{
    return rb->tail;
}

const void *
ringbuf_head(const struct ringbuf_t *rb)
{
    return rb->head;
}

/*
 * Given a ring buffer rb and a pointer to a location within its
 * contiguous buffer, return the a pointer to the next logical
 * location in the ring buffer.
 */
static struct command_t *
ringbuf_nextp(ringbuf_t rb, const struct command_t *p)
{
    /*
     * The assert guarantees the expression (++p - rb->buf) is
     * non-negative; therefore, the modulus operation is safe and
     * portable.
     */
    assert((p >= rb->buf) && (p < ringbuf_end(rb)));
    return rb->buf + ((++p - rb->buf) % ringbuf_buffer_size(rb));
}

size_t
ringbuf_findchr(const struct ringbuf_t *rb, int c, size_t offset)
{
    const struct command_t *bufend = ringbuf_end(rb);
    size_t bytes_used = ringbuf_bytes_used(rb);
    if (offset >= bytes_used)
        return bytes_used;

    const struct command_t *start = rb->buf +
        (((rb->tail - rb->buf) + offset) % ringbuf_buffer_size(rb));
    assert(bufend > start);
    size_t n = MIN(bufend - start, bytes_used - offset);
    const struct command_t *found = memchr(start, c, n);
    if (found)
        return offset + (found - start);
    else
        return ringbuf_findchr(rb, c, offset + n);
}

size_t
ringbuf_memset(ringbuf_t dst, int c, size_t len)
{
    const struct command_t *bufend = ringbuf_end(dst);
    size_t nwritten = 0;
    size_t count = MIN(len, ringbuf_buffer_size(dst));
    int overflow = count > ringbuf_bytes_free(dst);

    while (nwritten != count) {

        /* don't copy beyond the end of the buffer */
        assert(bufend > dst->head);
        size_t n = MIN(bufend - dst->head, count - nwritten);
        memset(dst->head, c, n);
        dst->head += n;
        nwritten += n;

        /* wrap? */
        if (dst->head == bufend)
            dst->head = dst->buf;
    }

    if (overflow) {
        dst->tail = ringbuf_nextp(dst, dst->head);
        assert(ringbuf_is_full(dst));
    }

    return nwritten;
}

void *
ringbuf_memcpy_into(ringbuf_t dst, const void *src, size_t count)
{
    const struct command_t *u8src = src;
    const struct command_t *bufend = ringbuf_end(dst);
    int overflow = count > ringbuf_bytes_free(dst);
    size_t nread = 0;

    while (nread != count) {
        /* don't copy beyond the end of the buffer */
        assert(bufend > dst->head);
        size_t n = MIN(bufend - dst->head, count - nread);
        memcpy(dst->head, u8src + nread, n);
        dst->head += n;
        nread += n;

        /* wrap? */
        if (dst->head == bufend)
            dst->head = dst->buf;
    }

    if (overflow) {
        dst->tail = ringbuf_nextp(dst, dst->head);
        assert(ringbuf_is_full(dst));
    }

    return dst->head;
}

ssize_t
ringbuf_read(int fd, ringbuf_t rb, size_t count)
{
    const struct command_t *bufend = ringbuf_end(rb);
    size_t nfree = ringbuf_bytes_free(rb);

    /* don't write beyond the end of the buffer */
    assert(bufend > rb->head);
    count = MIN(bufend - rb->head, count);
    ssize_t n = read(fd, rb->head, count);
    if (n > 0) {
        assert(rb->head + n <= bufend);
        rb->head += n;

        /* wrap? */
        if (rb->head == bufend)
            rb->head = rb->buf;

        /* fix up the tail pointer if an overflow occurred */
        if (n > nfree) {
            rb->tail = ringbuf_nextp(rb, rb->head);
            assert(ringbuf_is_full(rb));
        }
    }

    return n;
}

void *
ringbuf_memcpy_from(void *dst, ringbuf_t src, size_t count)
{
    size_t bytes_used = ringbuf_bytes_used(src);
    if (count > bytes_used)
        return 0;

    struct command_t *u8dst = dst;
    const struct command_t *bufend = ringbuf_end(src);
    size_t nwritten = 0;
    while (nwritten != count) {
        assert(bufend > src->tail);
        size_t n = MIN(bufend - src->tail, count - nwritten);
        memcpy(u8dst + nwritten, src->tail, n);
        src->tail += n;
        nwritten += n;

        /* wrap ? */
        if (src->tail == bufend)
            src->tail = src->buf;
    }

    assert(count + ringbuf_bytes_used(src) == bytes_used);
    return src->tail;
}

ssize_t
ringbuf_write(int fd, ringbuf_t rb, size_t count)
{
    size_t bytes_used = ringbuf_bytes_used(rb);
    if (count > bytes_used)
        return 0;

    const struct command_t *bufend = ringbuf_end(rb);
    assert(bufend > rb->head);
    count = MIN(bufend - rb->tail, count);
    ssize_t n = write(fd, rb->tail, count);
    if (n > 0) {
        assert(rb->tail + n <= bufend);
        rb->tail += n;

        /* wrap? */
        if (rb->tail == bufend)
            rb->tail = rb->buf;

        assert(n + ringbuf_bytes_used(rb) == bytes_used);
    }

    return n;
}

void *
ringbuf_copy(ringbuf_t dst, ringbuf_t src, size_t count)
{
    size_t src_bytes_used = ringbuf_bytes_used(src);
    if (count > src_bytes_used)
        return 0;
    int overflow = count > ringbuf_bytes_free(dst);

    const struct command_t *src_bufend = ringbuf_end(src);
    const struct command_t *dst_bufend = ringbuf_end(dst);
    size_t ncopied = 0;
    while (ncopied != count) {
        assert(src_bufend > src->tail);
        size_t nsrc = MIN(src_bufend - src->tail, count - ncopied);
        assert(dst_bufend > dst->head);
        size_t n = MIN(dst_bufend - dst->head, nsrc);
        memcpy(dst->head, src->tail, n);
        src->tail += n;
        dst->head += n;
        ncopied += n;

        /* wrap ? */
        if (src->tail == src_bufend)
            src->tail = src->buf;
        if (dst->head == dst_bufend)
            dst->head = dst->buf;
    }

    assert(count + ringbuf_bytes_used(src) == src_bytes_used);
    
    if (overflow) {
        dst->tail = ringbuf_nextp(dst, dst->head);
        assert(ringbuf_is_full(dst));
    }

    return dst->head;
}


/**
 * For thread local counters, we can set nodeCount to zero, then when
 * allocating IDs, if the nodeCount is zero, we can lazily set it to
 * observedThreadCount*90,000. observedThreadCount would be a global guarded by
 * a spin lock.
 */
__thread struct command_t buf[BUF_SIZE_MINUS_ONE];
__thread int nodeId;
// TODO: Lock around accessing observedThreadCount if threadlocal nodeId is zero.
int observedThreadCount;

__thread struct ringbuf_t queue = { 
  // Tried the following
  // .buf = &((struct command_t*){[0 ... 999] = &((struct command_t) {.cmd = CommandYGNodeFree, .arg1=NULL, .arg2 = NULL})}),
  .buf = {[0 ... BUF_SIZE_MINUS_ONE-1] = {.cmd = CommandYGNodeFree, .arg1=NULL, .arg2 = NULL}},
  .head = 0,
  .tail = 0,
  .size = BUF_SIZE_MINUS_ONE,
};






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

/**
 * Spin locking consumes about 10-11%.
 */
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
    if (nodeId == 0) {
      // TODO: Spin lock around the observedThreadCount.
      nodeId = 900000 * observedThreadCount;
      observedThreadCount++;
    }
    nodeId++;
    gNodeInstanceCount++;
    return nodeId;
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
