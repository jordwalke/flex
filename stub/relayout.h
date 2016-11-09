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

typedef enum CSSDirection {
    // 0 is reserved for "non-exist"
    CSSDirectionInherit = 1,
    CSSDirectionLTR = 2,
    CSSDirectionRTL = 3,
} CSSDirection;

typedef value *CSSNodeRef;

CSSNodeRef CSSNodeNew(void);

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

void CSSNodeMarkDirty(const CSSNodeRef node);

bool CSSNodeIsDirty(const CSSNodeRef node);

void CSSNodeStyleSetDirection(const CSSNodeRef node, const CSSDirection direction);

CSSDirection CSSNodeStyleGetDirection(const CSSNodeRef node);
