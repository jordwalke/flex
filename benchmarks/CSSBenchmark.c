/**
 * Copyright (c) 2014-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#include "CSSBenchmark.h"

#include <CSSLayout/CSSLayout.h>
#include <time.h>

// Measure functions can be quite slow, for example when measuring text.
// Simulate this by sleeping for 1 millisecond.
static CSSSize _measure(void *context,
                        float width,
                        CSSMeasureMode widthMode,
                        float height,
                        CSSMeasureMode heightMode) {
  const struct timespec sleeptime = {0, 1000000};
  nanosleep(&sleeptime, NULL);
  return (CSSSize){
      .width = widthMode == CSSMeasureModeUndefined ? 10 : width,
      .height = heightMode == CSSMeasureModeUndefined ? 10 : width,
  };
}


#define TEST(a, b)

CSS_BENCHMARKS({

  CSS_BENCHMARK("JORDANS BENCHMARK", {
    TEST(CSSLayoutTest, align_items_stretch) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);

      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_items_center) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetAlignItems(root, CSSAlignCenter);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);

      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_items_flex_start) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetAlignItems(root, CSSAlignFlexStart);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_items_flex_end) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetAlignItems(root, CSSAlignFlexEnd);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_self_center) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child0, CSSAlignCenter);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_self_flex_end) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child0, CSSAlignFlexEnd);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_self_flex_start) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child0, CSSAlignFlexStart);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_self_flex_end_override_flex_start) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetAlignItems(root, CSSAlignFlexStart);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child0, CSSAlignFlexEnd);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, border_no_size) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetBorder(root, CSSEdgeLeft, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeTop, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeRight, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeBottom, 10);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, border_container_match_child) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetBorder(root, CSSEdgeLeft, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeTop, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeRight, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeBottom, 10);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);

      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, border_stretch_child) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetBorder(root, CSSEdgeLeft, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeTop, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeRight, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeBottom, 10);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);

      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, border_center_child) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetJustifyContent(root, CSSJustifyCenter);
      CSSNodeStyleSetAlignItems(root, CSSAlignCenter);
      CSSNodeStyleSetBorder(root, CSSEdgeStart, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeEnd, 20);
      CSSNodeStyleSetBorder(root, CSSEdgeBottom, 20);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);

      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, max_width) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetMaxWidth(root_child0, 50);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, max_height) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetMaxHeight(root_child0, 50);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, padding_no_size) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetPadding(root, CSSEdgeLeft, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeTop, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeRight, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeBottom, 10);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, padding_container_match_child) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetPadding(root, CSSEdgeLeft, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeTop, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeRight, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeBottom, 10);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, padding_stretch_child) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetPadding(root, CSSEdgeLeft, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeTop, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeRight, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeBottom, 10);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, padding_center_child) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetJustifyContent(root, CSSJustifyCenter);
      CSSNodeStyleSetAlignItems(root, CSSAlignCenter);
      CSSNodeStyleSetPadding(root, CSSEdgeBottom, 20);
      CSSNodeStyleSetPadding(root, CSSEdgeTop, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeStart, 10);
      CSSNodeStyleSetPadding(root, CSSEdgeEnd, 20);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child0, CSSAlignCenter);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, absolute_layout_width_height_start_top) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetPositionType(root_child0, CSSPositionTypeAbsolute);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeStart, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeTop, 10);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, absolute_layout_width_height_end_bottom) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetPositionType(root_child0, CSSPositionTypeAbsolute);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeEnd, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeBottom, 10);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, absolute_layout_start_top_end_bottom) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetPositionType(root_child0, CSSPositionTypeAbsolute);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeStart, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeTop, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeEnd, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeBottom, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, absolute_layout_width_height_start_top_end_bottom) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetPositionType(root_child0, CSSPositionTypeAbsolute);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeStart, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeTop, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeEnd, 10);
      CSSNodeStyleSetPosition(root_child0, CSSEdgeBottom, 10);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, do_not_clamp_height_of_absolute_node_to_height_of_its_overflow_hidden_parent) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetOverflow(root, CSSOverflowHidden);
      CSSNodeStyleSetWidth(root, 50);
      CSSNodeStyleSetHeight(root, 50);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetPositionType(root_child0, CSSPositionTypeAbsolute);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child0_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0_child0, 100);
      CSSNodeStyleSetHeight(root_child0_child0, 100);
      CSSNodeInsertChild(root_child0, root_child0_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);

      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, flex_direction_column_no_height) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, flex_direction_row_no_width) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);
      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, flex_direction_column) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, flex_direction_row) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, flex_direction_column_reverse) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionColumnReverse);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, flex_direction_row_reverse) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRowReverse);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, wrap_column) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetHeight(root, 100);
      CSSNodeStyleSetWidth(root, 60);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 30);
      CSSNodeStyleSetHeight(root_child0, 30);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 30);
      CSSNodeStyleSetHeight(root_child1, 30);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 30);
      CSSNodeStyleSetHeight(root_child2, 30);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child3, 30);
      CSSNodeStyleSetHeight(root_child3, 30);
      CSSNodeInsertChild(root, root_child3, 3);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, wrap_row) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetWidth(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 30);
      CSSNodeStyleSetHeight(root_child0, 30);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 30);
      CSSNodeStyleSetHeight(root_child1, 30);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 30);
      CSSNodeStyleSetHeight(root_child2, 30);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child3, 30);
      CSSNodeStyleSetHeight(root_child3, 30);
      CSSNodeInsertChild(root, root_child3, 3);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, wrap_row_align_items_flex_end) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetAlignItems(root, CSSAlignFlexEnd);
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetWidth(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child0, CSSAlignFlexEnd);
      CSSNodeStyleSetWidth(root_child0, 30);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child1, CSSAlignFlexEnd);
      CSSNodeStyleSetWidth(root_child1, 30);
      CSSNodeStyleSetHeight(root_child1, 20);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child2, CSSAlignFlexEnd);
      CSSNodeStyleSetWidth(root_child2, 30);
      CSSNodeStyleSetHeight(root_child2, 30);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetAlignSelf(root_child3, CSSAlignFlexEnd);
      CSSNodeStyleSetWidth(root_child3, 30);
      CSSNodeStyleSetHeight(root_child3, 30);
      CSSNodeInsertChild(root, root_child3, 3);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, wrap_row_align_items_center) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetAlignItems(root, CSSAlignCenter);
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetWidth(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 30);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeStyleSetAlignSelf(root_child0, CSSAlignCenter);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 30);
      CSSNodeStyleSetHeight(root_child1, 20);
      CSSNodeStyleSetAlignSelf(root_child1, CSSAlignCenter);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 30);
      CSSNodeStyleSetHeight(root_child2, 30);
      CSSNodeStyleSetAlignSelf(root_child2, CSSAlignCenter);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child3, 30);
      CSSNodeStyleSetHeight(root_child3, 30);
      CSSNodeStyleSetAlignSelf(root_child3, CSSAlignCenter);
      CSSNodeInsertChild(root, root_child3, 3);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, margin_start) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetMargin(root_child0, CSSEdgeStart, 10);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, margin_end) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetJustifyContent(root, CSSJustifyFlexEnd);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetMargin(root_child0, CSSEdgeEnd, 10);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, margin_left) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeStyleSetMargin(root_child0, CSSEdgeLeft, 10);

      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }


    TEST(CSSLayoutTest, margin_top) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetMargin(root_child0, CSSEdgeTop, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, margin_right) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetJustifyContent(root, CSSJustifyFlexEnd);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetMargin(root_child0, CSSEdgeRight, 10);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, margin_bottom) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetJustifyContent(root, CSSJustifyFlexEnd);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetMargin(root_child0, CSSEdgeBottom, 10);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }


    TEST(CSSLayoutTest, align_content_flex_start) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 50);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 50);
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 50);
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child3, 50);
      CSSNodeStyleSetHeight(root_child3, 10);
      CSSNodeInsertChild(root, root_child3, 3);

      const CSSNodeRef root_child4 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child4, 50);
      CSSNodeStyleSetHeight(root_child4, 10);
      CSSNodeInsertChild(root, root_child4, 4);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_content_flex_end) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetAlignContent(root, CSSAlignFlexEnd);
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 50);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 50);
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 50);
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child3, 50);
      CSSNodeStyleSetHeight(root_child3, 10);
      CSSNodeInsertChild(root, root_child3, 3);

      const CSSNodeRef root_child4 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child4, 50);
      CSSNodeStyleSetHeight(root_child4, 10);
      CSSNodeInsertChild(root, root_child4, 4);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_content_center) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetAlignContent(root, CSSAlignCenter);
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 50);
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 50);
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 50);
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child3, 50);
      CSSNodeStyleSetHeight(root_child3, 10);
      CSSNodeInsertChild(root, root_child3, 3);

      const CSSNodeRef root_child4 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child4, 50);
      CSSNodeStyleSetHeight(root_child4, 10);
      CSSNodeInsertChild(root, root_child4, 4);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, align_content_stretch) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetAlignContent(root, CSSAlignStretch);
      CSSNodeStyleSetFlexWrap(root, CSSWrapTypeWrap);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 50);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 50);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 50);
      CSSNodeInsertChild(root, root_child2, 2);

      const CSSNodeRef root_child3 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child3, 50);
      CSSNodeInsertChild(root, root_child3, 3);

      const CSSNodeRef root_child4 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child4, 50);
      CSSNodeInsertChild(root, root_child4, 4);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_row_flex_start) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_row_flex_end) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetJustifyContent(root, CSSJustifyFlexEnd);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_row_center) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetJustifyContent(root, CSSJustifyCenter);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_row_space_between) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetJustifyContent(root, CSSJustifySpaceBetween);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_row_space_around) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetJustifyContent(root, CSSJustifySpaceAround);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetWidth(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_column_flex_start) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_column_flex_end) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetJustifyContent(root, CSSJustifyFlexEnd);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_column_center) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetJustifyContent(root, CSSJustifyCenter);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_column_space_between) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetJustifyContent(root, CSSJustifySpaceBetween);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, justify_content_column_space_around) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetJustifyContent(root, CSSJustifySpaceAround);
      CSSNodeStyleSetWidth(root, 102);
      CSSNodeStyleSetHeight(root, 102);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child1, 10);
      CSSNodeInsertChild(root, root_child1, 1);

      const CSSNodeRef root_child2 = CSSNodeNew();
      CSSNodeStyleSetHeight(root_child2, 10);
      CSSNodeInsertChild(root, root_child2, 2);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }



    TEST(CSSLayoutTest, border_flex_child) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetBorder(root, CSSEdgeLeft, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeTop, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeRight, 10);
      CSSNodeStyleSetBorder(root, CSSEdgeBottom, 10);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetFlexGrow(root_child0, 1);
      CSSNodeStyleSetWidth(root_child0, 10);
      CSSNodeInsertChild(root, root_child0, 0);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, min_height) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetFlexGrow(root_child0, 1);
      CSSNodeStyleSetMinHeight(root_child0, 60);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetFlexGrow(root_child1, 1);
      CSSNodeInsertChild(root, root_child1, 1);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }

    TEST(CSSLayoutTest, min_width) {
      const CSSNodeRef root = CSSNodeNew();
      CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
      CSSNodeStyleSetWidth(root, 100);
      CSSNodeStyleSetHeight(root, 100);

      const CSSNodeRef root_child0 = CSSNodeNew();
      CSSNodeStyleSetFlexGrow(root_child0, 1);
      CSSNodeStyleSetMinWidth(root_child0, 60);
      CSSNodeInsertChild(root, root_child0, 0);

      const CSSNodeRef root_child1 = CSSNodeNew();
      CSSNodeStyleSetFlexGrow(root_child1, 1);
      CSSNodeInsertChild(root, root_child1, 1);

      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



      CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



      CSSNodeFreeRecursive(root);
    }


      TEST(CSSLayoutTest, padding_flex_child) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetPadding(root, CSSEdgeLeft, 10);
        CSSNodeStyleSetPadding(root, CSSEdgeTop, 10);
        CSSNodeStyleSetPadding(root, CSSEdgeRight, 10);
        CSSNodeStyleSetPadding(root, CSSEdgeBottom, 10);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeStyleSetWidth(root_child0, 10);
        CSSNodeInsertChild(root, root_child0, 0);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }


      TEST(CSSLayoutTest, margin_and_flex_row) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeStart, 10);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeEnd, 10);
        CSSNodeInsertChild(root, root_child0, 0);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }


      TEST(CSSLayoutTest, margin_and_flex_column) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeTop, 10);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeBottom, 10);
        CSSNodeInsertChild(root, root_child0, 0);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }


      TEST(CSSLayoutTest, margin_and_stretch_row) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeTop, 10);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeBottom, 10);
        CSSNodeInsertChild(root, root_child0, 0);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }


      TEST(CSSLayoutTest, margin_and_stretch_column) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeStart, 10);
        CSSNodeStyleSetMargin(root_child0, CSSEdgeEnd, 10);
        CSSNodeInsertChild(root, root_child0, 0);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }


      TEST(CSSLayoutTest, margin_with_sibling_row) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetMargin(root_child0, CSSEdgeEnd, 10);
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeInsertChild(root, root_child0, 0);

        const CSSNodeRef root_child1 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child1, 1);
        CSSNodeInsertChild(root, root_child1, 1);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }

      TEST(CSSLayoutTest, margin_with_sibling_column) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetMargin(root_child0, CSSEdgeBottom, 10);
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeInsertChild(root, root_child0, 0);

        const CSSNodeRef root_child1 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child1, 1);
        CSSNodeInsertChild(root, root_child1, 1);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }

      TEST(CSSLayoutTest, flex_basis_flex_grow_column) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeStyleSetFlexBasis(root_child0, 50);
        CSSNodeInsertChild(root, root_child0, 0);

        const CSSNodeRef root_child1 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child1, 1);
        CSSNodeInsertChild(root, root_child1, 1);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }

      TEST(CSSLayoutTest, flex_basis_flex_grow_row) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child0, 1);
        CSSNodeStyleSetFlexBasis(root_child0, 50);
        CSSNodeInsertChild(root, root_child0, 0);

        const CSSNodeRef root_child1 = CSSNodeNew();
        CSSNodeStyleSetFlexGrow(root_child1, 1);
        CSSNodeInsertChild(root, root_child1, 1);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }


      TEST(CSSLayoutTest, flex_basis_flex_shrink_column) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexShrink(root_child0, 1);
        CSSNodeStyleSetFlexBasis(root_child0, 100);
        CSSNodeInsertChild(root, root_child0, 0);

        const CSSNodeRef root_child1 = CSSNodeNew();
        CSSNodeStyleSetFlexBasis(root_child1, 50);
        CSSNodeInsertChild(root, root_child1, 1);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }

      TEST(CSSLayoutTest, flex_basis_flex_shrink_row) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetFlexDirection(root, CSSFlexDirectionRow);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetHeight(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetFlexShrink(root_child0, 1);
        CSSNodeStyleSetFlexBasis(root_child0, 100);
        CSSNodeInsertChild(root, root_child0, 0);

        const CSSNodeRef root_child1 = CSSNodeNew();
        CSSNodeStyleSetFlexBasis(root_child1, 50);
        CSSNodeInsertChild(root, root_child1, 1);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);


        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);


        CSSNodeFreeRecursive(root);
      }


      TEST(CSSLayoutTest, jwalke_border_width_only_start) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetJustifyContent(root, CSSJustifyCenter);
        CSSNodeStyleSetAlignContent(root, CSSAlignCenter);

        CSSNodeStyleSetBorder(root, CSSEdgeTop, 10);
        CSSNodeStyleSetBorder(root, CSSEdgeBottom, 20);
        CSSNodeStyleSetBorder(root, CSSEdgeStart, 10);
        CSSNodeStyleSetHeight(root, 100);
        CSSNodeStyleSetWidth(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetHeight(root, 100);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetAlignSelf(root_child0, CSSAlignCenter);
        CSSNodeInsertChild(root, root_child0, 0);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }

      TEST(CSSLayoutTest, jwalke_border_width_only_end) {
        const CSSNodeRef root = CSSNodeNew();
        CSSNodeStyleSetJustifyContent(root, CSSJustifyCenter);
        CSSNodeStyleSetAlignContent(root, CSSAlignCenter);

        CSSNodeStyleSetBorder(root, CSSEdgeTop, 10);
        CSSNodeStyleSetBorder(root, CSSEdgeBottom, 20);
        CSSNodeStyleSetBorder(root, CSSEdgeEnd, 10);
        CSSNodeStyleSetHeight(root, 100);
        CSSNodeStyleSetWidth(root, 100);

        const CSSNodeRef root_child0 = CSSNodeNew();
        CSSNodeStyleSetHeight(root, 100);
        CSSNodeStyleSetWidth(root, 100);
        CSSNodeStyleSetAlignSelf(root_child0, CSSAlignCenter);
        CSSNodeInsertChild(root, root_child0, 0);

        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionLTR);



        CSSNodeCalculateLayout(root, CSSUndefined, CSSUndefined, CSSDirectionRTL);



        CSSNodeFreeRecursive(root);
      }
  })

});
