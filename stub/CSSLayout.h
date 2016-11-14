/**
 * Copyright (c) 2014-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include <caml/mlvalues.h>

// Not defined in MSVC++
#ifndef NAN
static const unsigned long __nan[2] = {0xffffffff, 0x7fffffff};
#define NAN (*(const float *) __nan)
#endif

#define CSSUndefined NAN

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CSSDirection {
    // 0 is reserved for "non-exist"
    CSSDirectionInherit = 1,
    CSSDirectionLTR,
    CSSDirectionRTL,
} CSSDirection;

typedef enum CSSJustify {
    CSSJustifyFlexStart = 0,
    CSSJustifyCenter,
    CSSJustifyFlexEnd,
    CSSJustifySpaceBetween,
    CSSJustifySpaceAround,
} CSSJustify;

typedef enum CSSEdge {
    CSSEdgeLeft = 0,
    CSSEdgeTop,
    CSSEdgeRight,
    CSSEdgeBottom,
    CSSEdgeStart,
    CSSEdgeEnd,
    CSSEdgeHorizontal,
    CSSEdgeVertical,
    CSSEdgeAll,
    CSSEdgeCount,
} CSSEdge;

typedef enum CSSAlign {
    CSSAlignAuto,
    CSSAlignFlexStart,
    CSSAlignCenter,
    CSSAlignFlexEnd,
    CSSAlignStretch,
} CSSAlign;

typedef enum CSSFlexDirection {
    CSSFlexDirectionColumn,
    CSSFlexDirectionColumnReverse,
    CSSFlexDirectionRow,
    CSSFlexDirectionRowReverse,
} CSSFlexDirection;

typedef enum CSSMeasureMode {
    CSSMeasureModeUndefined,
    CSSMeasureModeExactly,
    CSSMeasureModeAtMost,
    CSSMeasureModeCount,
} CSSMeasureMode;

typedef enum CSSPositionType {
    CSSPositionTypeRelative,
    CSSPositionTypeAbsolute,
} CSSPositionType;

typedef enum CSSDimension {
    CSSDimensionWidth,
    CSSDimensionHeight,
} CSSDimension;

typedef enum CSSWrapType {
    CSSWrapTypeNoWrap,
    CSSWrapTypeWrap,
} CSSWrapType;

typedef struct CSSSize {
    float width;
    float height;
} CSSSize;

typedef enum CSSOverflow {
    CSSOverflowVisible,
    CSSOverflowHidden,
    CSSOverflowScroll,
} CSSOverflow;


typedef enum CSSLogLevel {
    CSSLogLevelError,
    CSSLogLevelWarn,
    CSSLogLevelInfo,
    CSSLogLevelDebug,
    CSSLogLevelVerbose,
} CSSLogLevel;

typedef enum CSSPrintOptions {
    CSSPrintOptionsLayout = 1,
    CSSPrintOptionsStyle = 2,
    CSSPrintOptionsChildren = 4,
} CSSPrintOptions;

typedef value *CSSNodeRef;

typedef CSSSize (*CSSMeasureFunc)(CSSNodeRef node,
                                  float width,
                                  CSSMeasureMode widthMode,
                                  float height,
                                  CSSMeasureMode heightMode);

typedef void (*CSSPrintFunc)(CSSNodeRef node);

typedef int (*CSSLogger)(CSSLogLevel level, const char *format, va_list args);


CSSNodeRef CSSNodeNew(void);

int32_t CSSNodeGetInstanceCount(void);

void CSSNodeInit(const CSSNodeRef node);

void CSSNodeReset(const CSSNodeRef node);

void CSSNodeFree(const CSSNodeRef node);

// On the contract, the ownership of a node always belong to the creator.
// This function, however assumes the ownership of a node belong to the tree.
// We have this function mostly for convenience purpose and are only encouraged to use in tests.
void CSSNodeFreeRecursive(const CSSNodeRef node);

void CSSNodeInsertChild(const CSSNodeRef node,
                        const CSSNodeRef child,
                        const uint32_t index);

void CSSNodeRemoveChild(const CSSNodeRef node, const CSSNodeRef child);

uint32_t CSSNodeChildCount(const CSSNodeRef node);

CSSNodeRef CSSNodeGetChild(const CSSNodeRef node, const uint32_t index);

void CSSNodeMarkDirty(const CSSNodeRef node);

bool CSSNodeIsDirty(const CSSNodeRef node);

bool CSSValueIsUndefined(const float value);

void CSSNodePrint(const CSSNodeRef node, const CSSPrintOptions options);

void CSSNodeSetHasNewLayout(const CSSNodeRef node, bool hasNewLayout);

bool CSSNodeGetHasNewLayout(const CSSNodeRef node);

void CSSNodeSetContext(const CSSNodeRef node, void *context);

void* CSSNodeGetContext(const CSSNodeRef node);

void CSSNodeCalculateLayout(const CSSNodeRef node,
                            const float availableWidth,
                            const float availableHeight,
                            const CSSDirection parentDirection);
/* style */

void CSSNodeStyleSetWidth(const CSSNodeRef node, float width);

void CSSNodeStyleSetMaxWidth(const CSSNodeRef node, float width);

void CSSNodeStyleSetMinWidth(const CSSNodeRef node, float width);

float CSSNodeStyleGetWidth(const CSSNodeRef node);

float CSSNodeStyleGetMaxWidth(const CSSNodeRef node);

float CSSNodeStyleGetMinWidth(const CSSNodeRef node);

void CSSNodeStyleSetHeight(const CSSNodeRef node, float height);

void CSSNodeStyleSetMaxHeight(const CSSNodeRef node, float height);

void CSSNodeStyleSetMinHeight(const CSSNodeRef node, float height);

float CSSNodeStyleGetHeight(const CSSNodeRef node);

float CSSNodeStyleGetMaxHeight(const CSSNodeRef node);

float CSSNodeStyleGetMinHeight(const CSSNodeRef node);

void CSSNodeStyleSetFlexGrow(const CSSNodeRef node, float grow);

float CSSNodeStyleGetFlexGrow(const CSSNodeRef node);

void CSSNodeStyleSetFlexBasis(const CSSNodeRef node, float grow);

float CSSNodeStyleGetFlexBasis(const CSSNodeRef node);

void CSSNodeStyleSetFlexShrink(const CSSNodeRef node, float shrink);

CSSOverflow CSSNodeStyleGetOverflow(const CSSNodeRef node);

void CSSNodeStyleSetOverflow(const CSSNodeRef node, CSSOverflow overflow);

float CSSNodeStyleGetFlexShrink(const CSSNodeRef node);

void CSSNodeStyleSetFlexDirection(const CSSNodeRef node, const CSSFlexDirection direction);

CSSFlexDirection CSSNodeStyleGetFlexDirection(const CSSNodeRef node);

void CSSNodeStyleSetFlexWrap(const CSSNodeRef node, const CSSWrapType flexType);

CSSWrapType CSSNodeStyleGetFlexWrap(const CSSNodeRef node);

void CSSNodeStyleSetDirection(const CSSNodeRef node, const CSSDirection direction);

CSSDirection CSSNodeStyleGetDirection(const CSSNodeRef node);

void CSSNodeStyleSetPositionType(const CSSNodeRef node, const CSSPositionType);

CSSPositionType CSSNodeStyleGetPositionType(const CSSNodeRef node);

void CSSNodeStyleSetJustifyContent(const CSSNodeRef node, const CSSJustify grow);

CSSJustify CSSNodeStyleGetJustifyContent(const CSSNodeRef node);

void CSSNodeStyleSetAlignItems(const CSSNodeRef node, const CSSAlign grow);

CSSAlign CSSNodeStyleGetAlignItems(const CSSNodeRef node);

void CSSNodeStyleSetAlignContent(const CSSNodeRef node, const CSSAlign);

CSSAlign CSSNodeStyleGetAlignContent(const CSSNodeRef node);

void CSSNodeStyleSetAlignSelf(const CSSNodeRef node, const CSSAlign);

CSSAlign CSSNodeStyleGetAlignSelf(const CSSNodeRef node);

/* layout */

float CSSNodeLayoutGetWidth(const CSSNodeRef node);

float CSSNodeLayoutGetHeight(const CSSNodeRef node);

float CSSNodeLayoutGetTop(const CSSNodeRef node);

float CSSNodeLayoutGetBottom(const CSSNodeRef node);

float CSSNodeLayoutGetRight(const CSSNodeRef node);

float CSSNodeLayoutGetLeft(const CSSNodeRef node);

CSSDirection CSSNodeLayoutGetDirection(const CSSNodeRef node);

void CSSNodeSetMeasureFunc(const CSSNodeRef node, CSSMeasureFunc measureFunc);

CSSMeasureFunc CSSNodeGetMeasureFunc(const CSSNodeRef node);

/* edge */

void CSSNodeStyleSetPadding(const CSSNodeRef node, CSSEdge edge, float v);

void CSSNodeStyleSetPosition(const CSSNodeRef node, CSSEdge edge, float v);

void CSSNodeStyleSetMargin(const CSSNodeRef node, CSSEdge edge, float v);

void CSSNodeStyleSetBorder(const CSSNodeRef node, CSSEdge edge, float v);

float CSSNodeStyleGetPadding(const CSSNodeRef node, CSSEdge edge);

float CSSNodeStyleGetPosition(const CSSNodeRef node, CSSEdge edge);

float CSSNodeStyleGetMargin(const CSSNodeRef node, CSSEdge edge);

float CSSNodeStyleGetBorder(const CSSNodeRef node, CSSEdge edge);


inline bool eq(const float a, const float b) {
    if (CSSValueIsUndefined(a)) {
        return CSSValueIsUndefined(b);
    }
    return fabs(a - b) < 0.0001;
}

#ifdef __cplusplus
}
#endif
