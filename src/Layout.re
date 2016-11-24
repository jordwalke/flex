/**
 * Copyright (c) 2014, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */
/*
 * From css-layout comments:
 * The spec describes four different layout modes: "fill available", "max
 * content", "min content", and "fit content". Of these, we don't use
 * "min content" because we don't support default minimum main sizes (see
 * above for details). Each of our measure modes maps to a layout mode
 * from the spec (https://www.w3.org/TR/css3-sizing/#terms):
 *
 *   -.CssMeasureModeUndefined: `max-content`
 *   -.CssMeasureModeExactly: `fill-available`
 *   -.CssMeasureModeAtMost: `fit-content`
 *      If infinite space available in that axis, then `max-content.`
 *      Else, `min(max-content size, max(min-content size, fill-available size))`
 *      (Although, we don't support min-content)
 */
let module Create (Node: Spec.Node) (Encoding: Spec.Encoding) => {
  let module LayoutTypes = LayoutTypes.Create Node Encoding;
  let module LayoutSupport = LayoutSupport.Create Node Encoding;
  let module LayoutPrint = LayoutPrint.Create Node Encoding;
  open LayoutTypes;
  open LayoutSupport;
  /* open Encoding; */
  open HardCodedEncoding;
  let gCurrentGenerationCount = ref 0;
  let gDepth = ref 0;
  let gPrintTree = {contents: false};
  let gPrintChanges = {contents: false};
  let gPrintSkips = {contents: false};
  let measureString = "measure";
  let stretchString = "stretch";
  let absMeasureString = "abs-measure";
  let absLayoutString = "abs-layout";
  let initialString = "initial";
  let flexString = "flex";
  let spacer = "                                                            ";
  let getSpacer level => {
    let spacerLen = String.length spacer;
    let lvl = level > spacerLen ? level : spacerLen;
    String.sub spacer lvl (String.length spacer)
  };
  let getModeName (mode, isLayoutInsteadOfMeasure) =>
    switch mode {
    | CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS =>
      isLayoutInsteadOfMeasure ?
        "CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS" :
        "CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS"
    | CssMeasureModeUndefined => isLayoutInsteadOfMeasure ? "LAY_UNDEFINED" : "UNDEFINED"
    | CssMeasureModeExactly => isLayoutInsteadOfMeasure ? "LAY_EXACTLY" : "EXACTLY"
    | CssMeasureModeAtMost => isLayoutInsteadOfMeasure ? "LAY_AT_MOST" : "AT_MOST"
    };
  let canUseCachedMeasurement
      (
        availableWidth,
        availableHeight,
        marginRow,
        marginColumn,
        widthMeasureMode,
        heightMeasureMode,
        cachedLayout
      ) =>
    if (
      cachedLayout.availableWidth == availableWidth &&
      cachedLayout.availableHeight == availableHeight &&
      cachedLayout.widthMeasureMode == widthMeasureMode &&
      cachedLayout.heightMeasureMode == heightMeasureMode
    ) {
      true
    } else if
      /* Is it an exact match?*/
      /* If the width is an exact match, try a fuzzy match on the height.*/
      (
        cachedLayout.widthMeasureMode == widthMeasureMode &&
        cachedLayout.availableWidth == availableWidth &&
        heightMeasureMode === CssMeasureModeExactly &&
        availableHeight -. marginColumn == cachedLayout.computedHeight
      ) {
      true
    } else if
      /* If the height is an exact match, try a fuzzy match on the width.*/
      (
        cachedLayout.heightMeasureMode == heightMeasureMode &&
        cachedLayout.availableHeight == availableHeight &&
        widthMeasureMode === CssMeasureModeExactly &&
        availableWidth -. marginRow == cachedLayout.computedWidth
      ) {
      true
    } else {
      false
    };
  let cachedMeasurementAt layout i =>
    switch i {
    | 0 => layout.cachedMeasurement1
    | 1 => layout.cachedMeasurement2
    | 2 => layout.cachedMeasurement3
    | 3 => layout.cachedMeasurement4
    | 4 => layout.cachedMeasurement5
    | 5 => layout.cachedMeasurement6
    | _ => raise (Invalid_argument ("No cached measurement at " ^ string_of_int i))
    };

  /**
   * This is a wrapper around the layoutNodeImpl function. It determines
   * whether the layout request is redundant and can be skipped.
   *
   * Parameters:
   *  Input parameters are the same as layoutNodeImpl (see above)
   *  Return parameter is true if layout was performed, false if skipped
   */
  let rec layoutNodeInternal
          node
          availableWidth
          availableHeight
          parentDirection
          widthMeasureMode
          heightMeasureMode
          performLayout
          reason => {
    let layout = node.layout;
    gDepth.contents = gDepth.contents + 1;
    let needToVisitNode =
      node.isDirty node.context && layout.generationCount != gCurrentGenerationCount.contents ||
      layout.lastParentDirection != parentDirection;
    if needToVisitNode {
      /* Invalidate the cached results.*/
      layout.nextCachedMeasurementsIndex = 0;
      layout.cachedLayout.widthMeasureMode = CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS;
      layout.cachedLayout.heightMeasureMode = CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS
    };
    let cachedResults = ref None;
    /* Determine whether the results are already cached. We maintain a separate*/
    /* cache for layouts and measurements. A layout operation modifies the positions*/
    /* and dimensions for nodes in the subtree. The algorithm assumes that each node*/
    /* gets layed out a maximum of one time per tree layout, but multiple measurements*/
    /* may be required to resolve all of the flex dimensions.*/
    /* We handle nodes with measure functions specially here because they are the most
     * expensive to measure, so it's worth avoiding redundant measurements if at all possible.*/
    if (node.measure !== dummyMeasure && node.childrenCount === 0) {
      let marginAxisRow = getMarginAxis node CssFlexDirectionRow;
      let marginAxisColumn = getMarginAxis node CssFlexDirectionColumn;
      /* First, try to use the layout cache.*/
      if (
        canUseCachedMeasurement (
          availableWidth,
          availableHeight,
          marginAxisRow,
          marginAxisColumn,
          widthMeasureMode,
          heightMeasureMode,
          layout.cachedLayout
        )
      ) {
        cachedResults.contents = Some layout.cachedLayout
      } else {
        /* Try to use the measurement cache.*/
        let foundCached = {contents: false};
        for i in 0 to (layout.nextCachedMeasurementsIndex - 1) {
          /* This is basically the "break" */
          if (not foundCached.contents) {
            let cachedMeasurementAtIndex = cachedMeasurementAt layout i;
            if (
              canUseCachedMeasurement (
                availableWidth,
                availableHeight,
                marginAxisRow,
                marginAxisColumn,
                widthMeasureMode,
                heightMeasureMode,
                cachedMeasurementAtIndex
              )
            ) {
              cachedResults.contents = Some cachedMeasurementAtIndex;
              foundCached.contents = true
            }
          }
        }
      }
    } else if performLayout {
      if (
        layout.cachedLayout.availableWidth == availableWidth &&
        layout.cachedLayout.availableHeight == availableHeight &&
        layout.cachedLayout.widthMeasureMode == widthMeasureMode &&
        layout.cachedLayout.heightMeasureMode == heightMeasureMode
      ) {
        cachedResults.contents = Some layout.cachedLayout
      }
    } else {
      let foundCached = {contents: false};
      for i in 0 to (layout.nextCachedMeasurementsIndex - 1) {
        /* This is basically the "break" */
        if (not foundCached.contents) {
          let cachedMeasurementAtIndex = cachedMeasurementAt layout i;
          if (
            cachedMeasurementAtIndex.availableWidth == availableWidth &&
            cachedMeasurementAtIndex.availableHeight == availableHeight &&
            cachedMeasurementAtIndex.widthMeasureMode == widthMeasureMode &&
            cachedMeasurementAtIndex.heightMeasureMode == heightMeasureMode
          ) {
            cachedResults.contents = Some cachedMeasurementAtIndex;
            foundCached.contents = true
          }
        }
      }
    };
    if (not needToVisitNode && cachedResults.contents != None) {
      let cachedResults_ =
        switch cachedResults.contents {
        | None => raise (Invalid_argument "Not possible")
        | Some cr => cr
        };
      layout.measuredWidth = cachedResults_.computedWidth;
      layout.measuredHeight = cachedResults_.computedHeight;
      if (gPrintChanges.contents && gPrintSkips.contents) {
        Printf.printf "%s%d.{[skipped] " (getSpacer gDepth.contents) gDepth.contents;
        switch node.print {
        | None => ()
        | Some printer => printer node.context
        };
        Printf.printf
          "wm: %s, hm: %s, aw: %s ah: %s => d: (%s, %s) %s\n"
          (getModeName (widthMeasureMode, performLayout))
          (getModeName (heightMeasureMode, performLayout))
          (scalarToString availableWidth)
          (scalarToString availableHeight)
          (scalarToString cachedResults_.computedWidth)
          (scalarToString cachedResults_.computedHeight)
          reason
      }
    } else {
      if gPrintChanges.contents {
        Printf.printf "%s%d.{%s" (getSpacer gDepth.contents) gDepth.contents (needToVisitNode ? "*" : "");
        switch node.print {
        | None => ()
        | Some printer => printer node.context
        };
        Printf.printf
          "wm: %s, hm: %s, aw: %s ah: %s %s\n"
          (getModeName (widthMeasureMode, performLayout))
          (getModeName (heightMeasureMode, performLayout))
          (scalarToString availableWidth)
          (scalarToString availableHeight)
          reason
      };
      layoutNodeImpl (
        node,
        availableWidth,
        availableHeight,
        parentDirection,
        widthMeasureMode,
        heightMeasureMode,
        performLayout
      );
      if gPrintChanges.contents {
        Printf.printf "%s%d.}%s" (getSpacer gDepth.contents) gDepth.contents (needToVisitNode ? "*" : "");
        switch node.print {
        | None => ()
        | Some printer => printer node.context
        };
        Printf.printf
          "wm: %s, hm: %s, d: (%s, %s) %s\n"
          (getModeName (widthMeasureMode, performLayout))
          (getModeName (heightMeasureMode, performLayout))
          (scalarToString layout.measuredWidth)
          (scalarToString layout.measuredHeight)
          reason
      };
      layout.lastParentDirection = parentDirection;
      if (cachedResults.contents === None) {
        if (layout.nextCachedMeasurementsIndex == css_max_cached_result_count) {
          if gPrintChanges.contents {
            Printf.printf "Out of cache entries!\n"
          };
          layout.nextCachedMeasurementsIndex = 0
        };
        let newCacheEntry =
          performLayout ?
            /* Use the single layout cache entry.*/
            layout.cachedLayout :
            {
              /* Allocate a new measurement cache entry.*/
              let newCacheEntry_ = cachedMeasurementAt layout layout.nextCachedMeasurementsIndex;
              layout.nextCachedMeasurementsIndex = layout.nextCachedMeasurementsIndex + 1;
              newCacheEntry_
            };
        newCacheEntry.availableWidth = availableWidth;
        newCacheEntry.availableHeight = availableHeight;
        newCacheEntry.widthMeasureMode = widthMeasureMode;
        newCacheEntry.heightMeasureMode = heightMeasureMode;
        newCacheEntry.computedWidth = layout.measuredWidth;
        newCacheEntry.computedHeight = layout.measuredHeight
      }
    };
    if performLayout {
      node.layout.width = node.layout.measuredWidth;
      node.layout.height = node.layout.measuredHeight;
      layout.hasNewLayout = true
    };
    gDepth.contents = gDepth.contents - 1;
    layout.generationCount = gCurrentGenerationCount.contents;
    needToVisitNode || cachedResults.contents === None
  }
  and computeChildFlexBasis node child width widthMode height heightMode direction => {
    let mainAxis = resolveAxis node.style.flexDirection direction;
    let isMainAxisRow = isRowDirection mainAxis;
    let childWidth = {contents: zero};
    let childHeight = {contents: zero};
    let childWidthMeasureMode = {contents: CssMeasureModeUndefined};
    let childHeightMeasureMode = {contents: CssMeasureModeUndefined};
    if (isMainAxisRow && isStyleDimDefined child CssFlexDirectionRow) {
      child.layout.computedFlexBasis =
        fmaxf child.style.width (getPaddingAndBorderAxis child CssFlexDirectionRow)
    } else if (
      not isMainAxisRow && isStyleDimDefined child CssFlexDirectionColumn
    ) {
      child.layout.computedFlexBasis =
        fmaxf child.style.height (getPaddingAndBorderAxis child CssFlexDirectionColumn)
    } else if (
      not (isUndefined child.style.flexBasis)
    ) {
      if (isUndefined child.layout.computedFlexBasis) {
        child.layout.computedFlexBasis = fmaxf child.style.flexBasis (getPaddingAndBorderAxis child mainAxis)
      }
    } else {
      childWidth.contents = cssUndefined;
      childHeight.contents = cssUndefined;
      childWidthMeasureMode.contents = CssMeasureModeUndefined;
      childHeightMeasureMode.contents = CssMeasureModeUndefined;
      if (isStyleDimDefined child CssFlexDirectionRow) {
        childWidth.contents = child.style.width +. getMarginAxis child CssFlexDirectionRow;
        childWidthMeasureMode.contents = CssMeasureModeExactly
      };

      /**
       * Why can't this just be inlined to .height !== cssUndefined.
       */
      if (isStyleDimDefined child CssFlexDirectionColumn) {
        childHeight.contents = child.style.height +. getMarginAxis child CssFlexDirectionColumn;
        childHeightMeasureMode.contents = CssMeasureModeExactly
      };
      if (not isMainAxisRow && node.style.overflow === Scroll || node.style.overflow !== Scroll) {
        if (isUndefined childWidth.contents && not (isUndefined width)) {
          childWidth.contents = width;
          childWidthMeasureMode.contents = CssMeasureModeAtMost
        }
      };
      if (isMainAxisRow && node.style.overflow === Scroll || node.style.overflow !== Scroll) {
        if (isUndefined childHeight.contents && not (isUndefined height)) {
          childHeight.contents = height;
          childHeightMeasureMode.contents = CssMeasureModeAtMost
        }
      };
      /*
       * If child has no defined size in the cross axis and is set to
       * stretch, set the cross axis to be measured exactly with the
       * available inner width.
       */
      if (
        not isMainAxisRow &&
        not (isUndefined width) &&
        not (isStyleDimDefined child CssFlexDirectionRow) &&
        widthMode === CssMeasureModeExactly && getAlignItem node child === CssAlignStretch
      ) {
        childWidth.contents = width;
        childWidthMeasureMode.contents = CssMeasureModeExactly
      };
      if (
        isMainAxisRow &&
        not (isUndefined height) &&
        not (isStyleDimDefined child CssFlexDirectionColumn) &&
        heightMode === CssMeasureModeExactly && getAlignItem node child === CssAlignStretch
      ) {
        childHeight.contents = height;
        childHeightMeasureMode.contents = CssMeasureModeExactly
      };
      let _ =
        layoutNodeInternal
          child
          childWidth.contents
          childHeight.contents
          direction
          childWidthMeasureMode.contents
          childHeightMeasureMode.contents
          false
          measureString;
      child.layout.computedFlexBasis =
        fmaxf
          (isMainAxisRow ? child.layout.measuredWidth : child.layout.measuredHeight)
          (getPaddingAndBorderAxis child mainAxis)
    }
  }
  /**
   * By default, mathematical operations are floating point.
   */
  and layoutNodeImpl
      (
        node,
        availableWidth,
        availableHeight,
        parentDirection,
        widthMeasureMode,
        heightMeasureMode,
        performLayout
      ) => {

    /** START_GENERATED **/
    /* re_assert */
    /*   (isUndefined availableWidth ? widthMeasureMode === CssMeasureModeUndefined : true) */
    /*   "availableWidth is indefinite so widthMeasureMode must be CssMeasureModeUndefined"; */
    /* re_assert */
    /*   (isUndefined availableHeight ? heightMeasureMode === CssMeasureModeUndefined : true) */
    /*   "availableHeight is indefinite so heightMeasureMode must be CssMeasureModeUndefined"; */
    let paddingAndBorderAxisRow = getPaddingAndBorderAxis node CssFlexDirectionRow;
    let paddingAndBorderAxisColumn = getPaddingAndBorderAxis node CssFlexDirectionColumn;
    let marginAxisRow = getMarginAxis node CssFlexDirectionRow;
    let marginAxisColumn = getMarginAxis node CssFlexDirectionColumn;
    let direction = resolveDirection node parentDirection;
    node.layout.direction = direction;
    /* For content (text) nodes, determine the dimensions based on the text
       contents. */
    if (node.measure !== dummyMeasure && node.childrenCount === 0) {
      let innerWidth = availableWidth -. marginAxisRow -. paddingAndBorderAxisRow;
      let innerHeight = availableHeight -. marginAxisColumn -. paddingAndBorderAxisColumn;
      if (widthMeasureMode === CssMeasureModeExactly && heightMeasureMode === CssMeasureModeExactly) {
        node.layout.measuredWidth = boundAxis node CssFlexDirectionRow (availableWidth -. marginAxisRow);
        node.layout.measuredHeight =
          boundAxis node CssFlexDirectionColumn (availableHeight -. marginAxisColumn)
      } else if (
        not (isUndefined innerWidth) && innerWidth <= zero ||
        not (isUndefined innerHeight) && innerHeight <= zero
      ) {
        node.layout.measuredWidth = boundAxis node CssFlexDirectionRow zero;
        node.layout.measuredHeight = boundAxis node CssFlexDirectionColumn zero
      } else {
        let measureDim = node.measure node innerWidth widthMeasureMode innerHeight heightMeasureMode;
        node.layout.measuredWidth =
          boundAxis
            node
            CssFlexDirectionRow
            (
              widthMeasureMode === CssMeasureModeUndefined || widthMeasureMode === CssMeasureModeAtMost ?
                measureDim.width +. paddingAndBorderAxisRow : availableWidth -. marginAxisRow
            );
        node.layout.measuredHeight =
          boundAxis
            node
            CssFlexDirectionColumn
            (
              heightMeasureMode === CssMeasureModeUndefined || heightMeasureMode === CssMeasureModeAtMost ?
                measureDim.height +. paddingAndBorderAxisColumn : availableHeight -. marginAxisColumn
            )
      }
    } else {
      let childCount = Array.length node.children;
      if (childCount === 0) {
        node.layout.measuredWidth =
          boundAxis
            node
            CssFlexDirectionRow
            (
              widthMeasureMode === CssMeasureModeUndefined || widthMeasureMode === CssMeasureModeAtMost ?
                paddingAndBorderAxisRow : availableWidth -. marginAxisRow
            );
        node.layout.measuredHeight =
          boundAxis
            node
            CssFlexDirectionColumn
            (
              heightMeasureMode === CssMeasureModeUndefined || heightMeasureMode === CssMeasureModeAtMost ?
                paddingAndBorderAxisColumn : availableHeight -. marginAxisColumn
            )
      } else {
        let shouldContinue = {contents: true};
        if (not performLayout) {
          if (
            (
              (
                widthMeasureMode === CssMeasureModeAtMost &&
                not (isUndefined availableWidth) && availableWidth <= zero
              ) &&
              heightMeasureMode === CssMeasureModeAtMost
            ) &&
            not (isUndefined availableHeight) && availableHeight <= zero
          ) {
            node.layout.measuredWidth = boundAxis node CssFlexDirectionRow zero;
            node.layout.measuredHeight = boundAxis node CssFlexDirectionColumn zero;
            shouldContinue.contents = false
          } else if (
            widthMeasureMode === CssMeasureModeAtMost &&
            not (isUndefined availableWidth) && availableWidth <= zero
          ) {
            node.layout.measuredWidth = boundAxis node CssFlexDirectionRow zero;
            node.layout.measuredHeight =
              boundAxis
                node
                CssFlexDirectionColumn
                (isUndefined availableHeight ? zero : availableHeight -. marginAxisColumn);
            shouldContinue.contents = false
          } else if (
            heightMeasureMode === CssMeasureModeAtMost &&
            not (isUndefined availableHeight) && availableHeight <= zero
          ) {
            node.layout.measuredWidth =
              boundAxis
                node
                CssFlexDirectionRow
                (isUndefined availableWidth ? zero : availableWidth -. marginAxisRow);
            node.layout.measuredHeight = boundAxis node CssFlexDirectionColumn zero;
            shouldContinue.contents = false
          } else if (
            widthMeasureMode === CssMeasureModeExactly && heightMeasureMode === CssMeasureModeExactly
          ) {
            node.layout.measuredWidth = boundAxis node CssFlexDirectionRow (availableWidth -. marginAxisRow);
            node.layout.measuredHeight =
              boundAxis node CssFlexDirectionColumn (availableHeight -. marginAxisColumn);
            shouldContinue.contents = false
          }
        };
        if shouldContinue.contents {
          let mainAxis = resolveAxis node.style.flexDirection direction;
          let crossAxis = getCrossFlexDirection mainAxis direction;
          let isMainAxisRow = isRowDirection mainAxis;
          let justifyContent = node.style.justifyContent;
          let isNodeFlexWrap = node.style.flexWrap === CssWrap;
          let firstAbsoluteChild = {contents: theNullNode};
          let currentAbsoluteChildRef = {contents: theNullNode};
          let leadingPaddingAndBorderMain = getLeadingPaddingAndBorder node mainAxis;
          let trailingPaddingAndBorderMain = getTrailingPaddingAndBorder node mainAxis;
          let leadingPaddingAndBorderCross = getLeadingPaddingAndBorder node crossAxis;
          let paddingAndBorderAxisMain = getPaddingAndBorderAxis node mainAxis;
          let paddingAndBorderAxisCross = getPaddingAndBorderAxis node crossAxis;
          let measureModeMainDim = isMainAxisRow ? widthMeasureMode : heightMeasureMode;
          let measureModeCrossDim = isMainAxisRow ? heightMeasureMode : widthMeasureMode;
          let availableInnerWidth = availableWidth -. marginAxisRow -. paddingAndBorderAxisRow;
          let availableInnerHeight = availableHeight -. marginAxisColumn -. paddingAndBorderAxisColumn;
          let availableInnerMainDim = isMainAxisRow ? availableInnerWidth : availableInnerHeight;
          let availableInnerCrossDim = isMainAxisRow ? availableInnerHeight : availableInnerWidth;
          let child = {contents: theNullNode};
          /* STEP 3: DETERMINE FLEX BASIS FOR EACH ITEM */

          /**
           * This loop computes `.computedFlexBasis` for each child, and
           * constructs a chain of absolute children using their `.nextChild`
           * property.
           *
           * For every non-absolute child, we store a `.computedFlexBasis`.
           *
           * Later, after this for loop, we construct a chain of relative
           * children.
           *
           * We should find a way to avoid having to construct these chains or
           * store a computed flex basis at all. Perhaps we can show that for each
           * node it's only used once, and therefore can be computed when needed,
           * resulting in just as many computations, but zero mutations.
           *
           * Insight: Most children in practice are either all absolute or all
           * relative. Maybe there's a way to optimize for that. If so, we could
           * then just do straight loops over the children, and not mess with
           * `nextChild` pointers.
           */
          for i in 0 to (childCount - 1) {
            let child = node.children.(i);
            if performLayout {
              /* This is strange. We set the layout values to some intermediate
               * ones based purely on the style (essentially marginLeft + left).
               * This doesn't take into account the flex algorithm at all at thi
               * point, only the style.
               *
               * I think this intermediate position is either supposed to be
               * relative to the parent (if it's position:absolute) or relative to
               * its default layout (position:relative).
               */
              let childDirection = resolveDirection child direction;
              setPosition child childDirection
            };
            if (child.style.positionType === CssPositionAbsolute) {
              if (firstAbsoluteChild.contents === theNullNode) {
                firstAbsoluteChild.contents = child
              };
              let previousAbsoluteChild = currentAbsoluteChildRef.contents;
              /* If there was a prev absolute, set its `nextChild` to `child`. */
              if (previousAbsoluteChild !== theNullNode) {
                previousAbsoluteChild.nextChild = child
              };
              currentAbsoluteChildRef.contents = child
            } else {
              /**
               * We compute the flex basis for each child, then just read it out
               * immediately.  That might be okay, but it's not clear if we read
               * that value out more than once for any given child. If not, we
               * should only compute it on the fly and not pay the cost of
               * mutation. This mutation happens *all* the time.
               */
              computeChildFlexBasis
                node
                child
                availableInnerWidth
                widthMeasureMode
                availableInnerHeight
                heightMeasureMode
                direction
            }
          };
          if (currentAbsoluteChildRef.contents !== theNullNode) {
            /* Set `child.nextChild` to null in case it's the last absolute */
            currentAbsoluteChildRef.contents.nextChild = theNullNode
          };
          /* STEP 4: COLLECT FLEX ITEMS INTO FLEX LINES */
          let startOfLineIndex = {contents: 0};
          let endOfLineIndex = {contents: 0};
          let lineCount = {contents: 0};
          let totalLineCrossDim = {contents: zero};
          let maxLineMainDim = {contents: zero};
          while (endOfLineIndex.contents < childCount) {
            let itemsOnLine = {contents: 0};
            let sizeConsumedOnCurrentLine = {contents: zero};
            let totalFlexGrowFactors = {contents: zero};
            let totalFlexShrinkScaledFactors = {contents: zero};
            let curIndex = {contents: startOfLineIndex.contents};
            let firstRelativeChild = {contents: theNullNode};
            let currentRelativeChild = {contents: theNullNode};
            let shouldContinue = {contents: true};
            while (curIndex.contents < childCount && shouldContinue.contents) {
              child.contents = node.children.(curIndex.contents);
              child.contents.lineIndex = lineCount.contents;
              if (child.contents.style.positionType !== CssPositionAbsolute) {
                let outerFlexBasis =
                  child.contents.layout.computedFlexBasis +. getMarginAxis child.contents mainAxis;
                if (
                  (
                    sizeConsumedOnCurrentLine.contents +. outerFlexBasis > availableInnerMainDim && isNodeFlexWrap
                  ) &&
                  itemsOnLine.contents > 0
                ) {
                  shouldContinue.contents = false
                } else {
                  sizeConsumedOnCurrentLine.contents = sizeConsumedOnCurrentLine.contents +. outerFlexBasis;
                  itemsOnLine.contents = itemsOnLine.contents + 1;
                  if (isFlex child.contents) {
                    totalFlexGrowFactors.contents =
                      totalFlexGrowFactors.contents +. child.contents.style.flexGrow;
                    totalFlexShrinkScaledFactors.contents =
                      totalFlexShrinkScaledFactors.contents +.
                      -. child.contents.style.flexShrink *. child.contents.layout.computedFlexBasis
                  };
                  if (firstRelativeChild.contents === theNullNode) {
                    firstRelativeChild.contents = child.contents
                  };
                  if (currentRelativeChild.contents !== theNullNode) {
                    currentRelativeChild.contents.nextChild = child.contents
                  };
                  currentRelativeChild.contents = child.contents;
                  child.contents.nextChild = theNullNode;
                  curIndex.contents = curIndex.contents + 1;
                  endOfLineIndex.contents = endOfLineIndex.contents + 1
                }
              } else {
                curIndex.contents = curIndex.contents + 1;
                endOfLineIndex.contents = endOfLineIndex.contents + 1
              }
            };
            let canSkipFlex = not performLayout && measureModeCrossDim === CssMeasureModeExactly;
            let leadingMainDim = {contents: zero};
            let betweenMainDim = {contents: zero};
            let remainingFreeSpace = {contents: zero};
            if (not (isUndefined availableInnerMainDim)) {
              remainingFreeSpace.contents = availableInnerMainDim -. sizeConsumedOnCurrentLine.contents
            } else if (
              sizeConsumedOnCurrentLine.contents < zero
            ) {
              remainingFreeSpace.contents = -. sizeConsumedOnCurrentLine.contents
            };
            let originalRemainingFreeSpace = remainingFreeSpace.contents;
            let deltaFreeSpace = {contents: zero};
            if (not canSkipFlex) {
              let childFlexBasis = {contents: zero};
              let flexShrinkScaledFactor = {contents: zero};
              let flexGrowFactor = {contents: zero};
              let baseMainSize = {contents: zero};
              let boundMainSize = {contents: zero};
              let deltaFlexShrinkScaledFactors = {contents: zero};
              let deltaFlexGrowFactors = {contents: zero};
              currentRelativeChild.contents = firstRelativeChild.contents;
              while (currentRelativeChild.contents !== theNullNode) {
                childFlexBasis.contents = currentRelativeChild.contents.layout.computedFlexBasis;
                if (remainingFreeSpace.contents < zero) {
                  flexShrinkScaledFactor.contents =
                    -. currentRelativeChild.contents.style.flexShrink *. childFlexBasis.contents;
                  if (flexShrinkScaledFactor.contents != zero) {
                    baseMainSize.contents =
                      childFlexBasis.contents +.
                      /*
                       * Important to first scale, then divide - to support fixed
                       * point encoding.
                       */
                      flexShrinkScaledFactor.contents *. remainingFreeSpace.contents /.
                      totalFlexShrinkScaledFactors.contents;
                    boundMainSize.contents =
                      boundAxis currentRelativeChild.contents mainAxis baseMainSize.contents;
                    if (baseMainSize.contents != boundMainSize.contents) {
                      deltaFreeSpace.contents =
                        deltaFreeSpace.contents -. (boundMainSize.contents -. childFlexBasis.contents);
                      deltaFlexShrinkScaledFactors.contents =
                        deltaFlexShrinkScaledFactors.contents -. flexShrinkScaledFactor.contents
                    }
                  }
                } else if (
                  remainingFreeSpace.contents > zero
                ) {
                  flexGrowFactor.contents = currentRelativeChild.contents.style.flexGrow;
                  if (flexGrowFactor.contents != zero) {
                    baseMainSize.contents =
                      childFlexBasis.contents +.
                      /*
                       * Important to first scale, then divide - to support fixed
                       * point encoding.
                       */
                      flexGrowFactor.contents *. remainingFreeSpace.contents /.
                      totalFlexGrowFactors.contents;
                    boundMainSize.contents =
                      boundAxis currentRelativeChild.contents mainAxis baseMainSize.contents;
                    if (baseMainSize.contents != boundMainSize.contents) {
                      deltaFreeSpace.contents =
                        deltaFreeSpace.contents -. (boundMainSize.contents -. childFlexBasis.contents);
                      deltaFlexGrowFactors.contents =
                        deltaFlexGrowFactors.contents -. flexGrowFactor.contents
                    }
                  }
                };
                currentRelativeChild.contents = currentRelativeChild.contents.nextChild
              };
              totalFlexShrinkScaledFactors.contents =
                totalFlexShrinkScaledFactors.contents +. deltaFlexShrinkScaledFactors.contents;
              totalFlexGrowFactors.contents = totalFlexGrowFactors.contents +. deltaFlexGrowFactors.contents;
              remainingFreeSpace.contents = remainingFreeSpace.contents +. deltaFreeSpace.contents;
              deltaFreeSpace.contents = zero;
              currentRelativeChild.contents = firstRelativeChild.contents;
              while (currentRelativeChild.contents !== theNullNode) {
                childFlexBasis.contents = currentRelativeChild.contents.layout.computedFlexBasis;
                let updatedMainSize = {contents: childFlexBasis.contents};
                if (remainingFreeSpace.contents < zero) {
                  flexShrinkScaledFactor.contents =
                    -. currentRelativeChild.contents.style.flexShrink *. childFlexBasis.contents;
                  if (flexShrinkScaledFactor.contents != zero) {
                    updatedMainSize.contents =
                      boundAxis
                        currentRelativeChild.contents
                        mainAxis
                        (
                          childFlexBasis.contents +.
                          /*
                           * Important to first scale, then divide - to support
                           * fixed point encoding.
                           */
                          flexShrinkScaledFactor.contents *. remainingFreeSpace.contents /.
                          totalFlexShrinkScaledFactors.contents
                        )
                  }
                } else if (
                  remainingFreeSpace.contents > zero
                ) {
                  flexGrowFactor.contents = currentRelativeChild.contents.style.flexGrow;
                  if (flexGrowFactor.contents != zero) {
                    updatedMainSize.contents =
                      boundAxis
                        currentRelativeChild.contents
                        mainAxis
                        (
                          childFlexBasis.contents +.
                          /*
                           * Important to first scale, then divide - to support
                           * fixed point encoding.
                           */
                          flexGrowFactor.contents *. remainingFreeSpace.contents /.
                          totalFlexGrowFactors.contents
                        )
                  }
                };
                deltaFreeSpace.contents =
                  deltaFreeSpace.contents -. (updatedMainSize.contents -. childFlexBasis.contents);
                let childWidth = {contents: zero};
                let childHeight = {contents: zero};
                let childWidthMeasureMode = {contents: CssMeasureModeUndefined};
                let childHeightMeasureMode = {contents: CssMeasureModeUndefined};
                if isMainAxisRow {
                  childWidth.contents =
                    updatedMainSize.contents +.
                    getMarginAxis currentRelativeChild.contents CssFlexDirectionRow;
                  childWidthMeasureMode.contents = CssMeasureModeExactly;
                  if (
                    not (isUndefined availableInnerCrossDim) &&
                    not (isStyleDimDefined currentRelativeChild.contents CssFlexDirectionColumn) &&
                    heightMeasureMode === CssMeasureModeExactly &&
                    getAlignItem node currentRelativeChild.contents === CssAlignStretch
                  ) {
                    childHeight.contents = availableInnerCrossDim;
                    childHeightMeasureMode.contents = CssMeasureModeExactly
                  } else if (
                    not (isStyleDimDefined currentRelativeChild.contents CssFlexDirectionColumn)
                  ) {
                    childHeight.contents = availableInnerCrossDim;
                    childHeightMeasureMode.contents =
                      isUndefined childHeight.contents ? CssMeasureModeUndefined : CssMeasureModeAtMost
                  } else {
                    childHeight.contents =
                      currentRelativeChild.contents.style.height +.
                      getMarginAxis currentRelativeChild.contents CssFlexDirectionColumn;
                    childHeightMeasureMode.contents = CssMeasureModeExactly
                  }
                } else {
                  childHeight.contents =
                    updatedMainSize.contents +.
                    getMarginAxis currentRelativeChild.contents CssFlexDirectionColumn;
                  childHeightMeasureMode.contents = CssMeasureModeExactly;
                  if (
                    not (isUndefined availableInnerCrossDim) &&
                    not (isStyleDimDefined currentRelativeChild.contents CssFlexDirectionRow) &&
                    widthMeasureMode === CssMeasureModeExactly &&
                    getAlignItem node currentRelativeChild.contents === CssAlignStretch
                  ) {
                    childWidth.contents = availableInnerCrossDim;
                    childWidthMeasureMode.contents = CssMeasureModeExactly
                  } else if (
                    not (isStyleDimDefined currentRelativeChild.contents CssFlexDirectionRow)
                  ) {
                    childWidth.contents = availableInnerCrossDim;
                    childWidthMeasureMode.contents =
                      isUndefined childWidth.contents ? CssMeasureModeUndefined : CssMeasureModeAtMost
                  } else {
                    childWidth.contents =
                      currentRelativeChild.contents.style.width +.
                      getMarginAxis currentRelativeChild.contents CssFlexDirectionRow;
                    childWidthMeasureMode.contents = CssMeasureModeExactly
                  }
                };
                let requiresStretchLayout =
                  not (isStyleDimDefined currentRelativeChild.contents crossAxis) &&
                  getAlignItem node currentRelativeChild.contents === CssAlignStretch;
                let _ =
                  layoutNodeInternal
                    currentRelativeChild.contents
                    childWidth.contents
                    childHeight.contents
                    direction
                    childWidthMeasureMode.contents
                    childHeightMeasureMode.contents
                    (performLayout && not requiresStretchLayout)
                    flexString;
                currentRelativeChild.contents = currentRelativeChild.contents.nextChild
              }
            };
            remainingFreeSpace.contents = originalRemainingFreeSpace +. deltaFreeSpace.contents;
            /* If we are using "at most" rules in the main axis. Calculate the remaining space when
               constraint by the min size defined for the main axis. */
            if (measureModeMainDim === CssMeasureModeAtMost) {
              let minDim = styleMinDimensionForAxis node mainAxis;
              if (not (isUndefined minDim) && minDim >= 0) {
                remainingFreeSpace.contents =
                  fmaxf 0 (minDim - (availableInnerMainDim -. remainingFreeSpace.contents))
              } else {
                remainingFreeSpace.contents = zero
              }
            };
            switch justifyContent {
            | CssJustifyCenter => leadingMainDim.contents = divideScalarByInt remainingFreeSpace.contents 2
            | CssJustifyFlexEnd => leadingMainDim.contents = remainingFreeSpace.contents
            | CssJustifySpaceBetween =>
              if (itemsOnLine.contents > 1) {
                betweenMainDim.contents =
                  divideScalarByInt (fmaxf remainingFreeSpace.contents zero) (itemsOnLine.contents - 1)
              } else {
                betweenMainDim.contents = zero
              }
            | CssJustifySpaceAround =>
              betweenMainDim.contents = divideScalarByInt remainingFreeSpace.contents itemsOnLine.contents;
              leadingMainDim.contents = divideScalarByInt betweenMainDim.contents 2
            | CssJustifyFlexStart => ()
            };
            let mainDim = {contents: leadingPaddingAndBorderMain +. leadingMainDim.contents};
            let crossDim = {contents: zero};
            for i in startOfLineIndex.contents to (endOfLineIndex.contents - 1) {
              child.contents = node.children.(i);
              if (
                child.contents.style.positionType === CssPositionAbsolute &&
                isLeadingPosDefinedWithFallback child.contents mainAxis
              ) {
                if performLayout {
                  setLayoutLeadingPositionForAxis
                    child.contents
                    mainAxis
                    (
                      getLeadingPositionWithFallback child.contents mainAxis +. getLeadingBorder node mainAxis +.
                      getLeadingMargin child.contents mainAxis
                    )
                }
              } else {
                if performLayout {
                  setLayoutLeadingPositionForAxis
                    child.contents
                    mainAxis
                    (layoutPosPositionForAxis child.contents mainAxis +. mainDim.contents)
                };
                if (child.contents.style.positionType === CssPositionRelative) {
                  if canSkipFlex {
                    mainDim.contents =
                      mainDim.contents +. betweenMainDim.contents +. getMarginAxis child.contents mainAxis +.
                      child.contents.layout.computedFlexBasis;
                    crossDim.contents = availableInnerCrossDim
                  } else {
                    mainDim.contents =
                      mainDim.contents +. betweenMainDim.contents +.
                      getDimWithMargin child.contents mainAxis;
                    crossDim.contents = fmaxf crossDim.contents (getDimWithMargin child.contents crossAxis)
                  }
                }
              }
            };
            mainDim.contents = mainDim.contents +. trailingPaddingAndBorderMain;
            let containerCrossAxis = {contents: availableInnerCrossDim};
            if (
              measureModeCrossDim === CssMeasureModeUndefined || measureModeCrossDim === CssMeasureModeAtMost
            ) {
              containerCrossAxis.contents =
                boundAxis node crossAxis (crossDim.contents +. paddingAndBorderAxisCross) -. paddingAndBorderAxisCross;
              if (measureModeCrossDim === CssMeasureModeAtMost) {
                containerCrossAxis.contents = fminf containerCrossAxis.contents availableInnerCrossDim
              }
            };
            if (not isNodeFlexWrap && measureModeCrossDim === CssMeasureModeExactly) {
              crossDim.contents = availableInnerCrossDim
            };
            crossDim.contents =
              boundAxis node crossAxis (crossDim.contents +. paddingAndBorderAxisCross) -. paddingAndBorderAxisCross;
            /*
             * STEP 7: CROSS-AXIS ALIGNMENT We can skip child alignment if we're
             * just measuring the container.
             */
            if performLayout {
              for i in startOfLineIndex.contents to (endOfLineIndex.contents - 1) {
                child.contents = node.children.(i);
                if (child.contents.style.positionType === CssPositionAbsolute) {
                  if (isLeadingPosDefinedWithFallback child.contents crossAxis) {
                    setLayoutLeadingPositionForAxis
                      child.contents
                      crossAxis
                      (
                        getLeadingPositionWithFallback child.contents crossAxis +.
                        getLeadingBorder node crossAxis +.
                        getLeadingMargin child.contents crossAxis
                      )
                  } else {
                    setLayoutLeadingPositionForAxis
                      child.contents
                      crossAxis
                      (leadingPaddingAndBorderCross +. getLeadingMargin child.contents crossAxis)
                  }
                } else {
                  let leadingCrossDim = {contents: leadingPaddingAndBorderCross};
                  let alignItem = getAlignItem node child.contents;
                  if (alignItem === CssAlignStretch) {
                    let childWidth = {contents: zero};
                    let childHeight = {contents: zero};
                    let childWidthMeasureMode = {contents: CssMeasureModeUndefined};
                    let childHeightMeasureMode = {contents: CssMeasureModeUndefined};
                    childWidth.contents =
                      child.contents.layout.measuredWidth +.
                      getMarginAxis child.contents CssFlexDirectionRow;
                    childHeight.contents =
                      child.contents.layout.measuredHeight +.
                      getMarginAxis child.contents CssFlexDirectionColumn;
                    let isCrossSizeDefinite = {contents: false};
                    if isMainAxisRow {
                      isCrossSizeDefinite.contents = isStyleDimDefined child.contents CssFlexDirectionColumn;
                      childHeight.contents = crossDim.contents
                    } else {
                      isCrossSizeDefinite.contents = isStyleDimDefined child.contents CssFlexDirectionRow;
                      childWidth.contents = crossDim.contents
                    };
                    if (not isCrossSizeDefinite.contents) {
                      childWidthMeasureMode.contents =
                        isUndefined childWidth.contents ? CssMeasureModeUndefined : CssMeasureModeExactly;
                      childHeightMeasureMode.contents =
                        isUndefined childHeight.contents ? CssMeasureModeUndefined : CssMeasureModeExactly;
                      let _ =
                        layoutNodeInternal
                          child.contents
                          childWidth.contents
                          childHeight.contents
                          direction
                          childWidthMeasureMode.contents
                          childHeightMeasureMode.contents
                          true
                          stretchString;
                      ()
                    }
                  } else if (
                    alignItem !== CssAlignFlexStart
                  ) {
                    let remainingCrossDim =
                      containerCrossAxis.contents -. getDimWithMargin child.contents crossAxis;
                    if (alignItem === CssAlignCenter) {
                      leadingCrossDim.contents =
                        leadingCrossDim.contents +. divideScalarByInt remainingCrossDim 2
                    } else {
                      leadingCrossDim.contents = leadingCrossDim.contents +. remainingCrossDim
                    }
                  };
                  setLayoutLeadingPositionForAxis
                    child.contents
                    crossAxis
                    (
                      layoutPosPositionForAxis child.contents crossAxis +. totalLineCrossDim.contents +.
                      leadingCrossDim.contents
                    )
                }
              }
            };
            totalLineCrossDim.contents = totalLineCrossDim.contents +. crossDim.contents;
            maxLineMainDim.contents = fmaxf maxLineMainDim.contents mainDim.contents;
            lineCount.contents = lineCount.contents + 1;
            startOfLineIndex.contents = endOfLineIndex.contents
          };
          if (lineCount.contents > 1 && performLayout && not (isUndefined availableInnerCrossDim)) {
            let remainingAlignContentDim = availableInnerCrossDim -. totalLineCrossDim.contents;
            let crossDimLead = {contents: zero};
            let currentLead = {contents: leadingPaddingAndBorderCross};
            let alignContent = node.style.alignContent;
            if (alignContent === CssAlignFlexEnd) {
              currentLead.contents = currentLead.contents +. remainingAlignContentDim
            } else if (
              alignContent === CssAlignCenter
            ) {
              currentLead.contents = currentLead.contents +. divideScalarByInt remainingAlignContentDim 2
            } else if (
              alignContent === CssAlignStretch
            ) {
              if (availableInnerCrossDim > totalLineCrossDim.contents) {
                crossDimLead.contents = divideScalarByInt remainingAlignContentDim lineCount.contents
              }
            };
            let endIndex = {contents: 0};
            for i in 0 to (lineCount.contents - 1) {
              let startIndex = endIndex.contents;
              let j = {contents: startIndex};
              let lineHeight = {contents: zero};
              let shouldContinue = {contents: false};
              while (j.contents < childCount && shouldContinue.contents) {
                child.contents = node.children.(j.contents);
                if (child.contents.style.positionType === CssPositionRelative) {
                  if (child.contents.lineIndex !== i) {
                    shouldContinue.contents = false
                  } else if (
                    isLayoutDimDefined child.contents crossAxis
                  ) {
                    lineHeight.contents =
                      fmaxf
                        lineHeight.contents
                        (
                          layoutMeasuredDimensionForAxis child.contents crossAxis +.
                          getMarginAxis child.contents crossAxis
                        )
                  }
                };
                j.contents = j.contents + 1
              };
              endIndex.contents = j.contents;
              lineHeight.contents = lineHeight.contents +. crossDimLead.contents;
              if performLayout {
                for j in startIndex to (endIndex.contents - 1) {
                  child.contents = node.children.(j);
                  if (child.contents.style.positionType === CssPositionRelative) {
                    switch (getAlignItem node child.contents) {
                    | CssAlignFlexStart =>
                      setLayoutLeadingPositionForAxis
                        child.contents
                        crossAxis
                        (currentLead.contents +. getLeadingMargin child.contents crossAxis)
                    | CssAlignFlexEnd =>
                      setLayoutLeadingPositionForAxis
                        child.contents
                        crossAxis
                        (
                          currentLead.contents +. lineHeight.contents -.
                          getTrailingMargin child.contents crossAxis -.
                          layoutMeasuredDimensionForAxis child.contents crossAxis
                        )
                    | CssAlignCenter =>
                      let childHeight = layoutMeasuredDimensionForAxis child.contents crossAxis;
                      setLayoutLeadingPositionForAxis
                        child.contents
                        crossAxis
                        (currentLead.contents +. divideScalarByInt (lineHeight.contents -. childHeight) 2)
                    | CssAlignStretch =>
                      setLayoutLeadingPositionForAxis
                        child.contents
                        crossAxis
                        (currentLead.contents +. getLeadingMargin child.contents crossAxis)
                    | CssAlignAuto => raise (Invalid_argument "getAlignItem should never return auto")
                    }
                  }
                }
              };
              currentLead.contents = currentLead.contents +. lineHeight.contents
            }
          };
          /* STEP 9: COMPUTING FINAL DIMENSIONS */
          node.layout.measuredWidth = boundAxis node CssFlexDirectionRow (availableWidth -. marginAxisRow);
          node.layout.measuredHeight =
            boundAxis node CssFlexDirectionColumn (availableHeight -. marginAxisColumn);
          /* If the user didn't specify a width or height for the node, set the
           * dimensions based on the children. */
          if (measureModeMainDim === CssMeasureModeUndefined) {
            setLayoutMeasuredDimensionForAxis node mainAxis (boundAxis node mainAxis maxLineMainDim.contents)
          } else if (
            measureModeMainDim === CssMeasureModeAtMost
          ) {
            setLayoutMeasuredDimensionForAxis
              node
              mainAxis
              (
                fmaxf
                  (
                    fminf
                      (availableInnerMainDim +. paddingAndBorderAxisMain)
                      (boundAxisWithinMinAndMax node mainAxis maxLineMainDim.contents)
                  )
                  paddingAndBorderAxisMain
              )
          };
          if (measureModeCrossDim === CssMeasureModeUndefined) {
            setLayoutMeasuredDimensionForAxis
              node
              crossAxis
              (boundAxis node crossAxis (totalLineCrossDim.contents +. paddingAndBorderAxisCross))
          } else if (
            measureModeCrossDim === CssMeasureModeAtMost
          ) {
            setLayoutMeasuredDimensionForAxis
              node
              crossAxis
              (
                fmaxf
                  (
                    fminf
                      (availableInnerCrossDim +. paddingAndBorderAxisCross)
                      (
                        boundAxisWithinMinAndMax
                          node crossAxis (totalLineCrossDim.contents +. paddingAndBorderAxisCross)
                      )
                  )
                  paddingAndBorderAxisCross
              )
          };
          let currentAbsoluteChildRef = {contents: firstAbsoluteChild.contents};
          while (currentAbsoluteChildRef.contents !== theNullNode) {
            let currentAbsoluteChild = currentAbsoluteChildRef.contents;
            if performLayout {
              let childWidth = {contents: cssUndefined};
              let childHeight = {contents: cssUndefined};
              let childWidthMeasureMode = {contents: CssMeasureModeUndefined};
              let childHeightMeasureMode = {contents: CssMeasureModeUndefined};
              if (isStyleDimDefined currentAbsoluteChild CssFlexDirectionRow) {
                childWidth.contents =
                  currentAbsoluteChild.style.width +. getMarginAxis currentAbsoluteChild CssFlexDirectionRow
              } else if (
                isLeadingPosDefinedWithFallback currentAbsoluteChild CssFlexDirectionRow &&
                isTrailingPosDefinedWithFallback currentAbsoluteChild CssFlexDirectionRow
              ) {
                childWidth.contents =
                  node.layout.measuredWidth -. (
                    getLeadingBorder node CssFlexDirectionRow +. getTrailingBorder node CssFlexDirectionRow
                  ) -. (
                    getLeadingPositionWithFallback currentAbsoluteChild CssFlexDirectionRow +.
                    getTrailingPositionWithFallback currentAbsoluteChild CssFlexDirectionRow
                  );
                childWidth.contents = boundAxis currentAbsoluteChild CssFlexDirectionRow childWidth.contents
              };
              if (isStyleDimDefined currentAbsoluteChild CssFlexDirectionColumn) {
                childHeight.contents =
                  currentAbsoluteChild.style.height +.
                  getMarginAxis currentAbsoluteChild CssFlexDirectionColumn
              } else if (
                /* If the child doesn't have a specified height, compute the height based on the top/bottom offsets if they're defined. */
                isLeadingPosDefinedWithFallback currentAbsoluteChild CssFlexDirectionColumn &&
                isTrailingPosDefinedWithFallback currentAbsoluteChild CssFlexDirectionColumn
              ) {
                childHeight.contents =
                  node.layout.measuredHeight -. (
                    getLeadingBorder node CssFlexDirectionColumn +.
                    getTrailingBorder node CssFlexDirectionColumn
                  ) -. (
                    getLeadingPositionWithFallback currentAbsoluteChild CssFlexDirectionColumn +.
                    getTrailingPositionWithFallback currentAbsoluteChild CssFlexDirectionColumn
                  );
                childHeight.contents =
                  boundAxis currentAbsoluteChild CssFlexDirectionColumn childHeight.contents
              };
              if (isUndefined childWidth.contents || isUndefined childHeight.contents) {
                childWidthMeasureMode.contents =
                  isUndefined childWidth.contents ? CssMeasureModeUndefined : CssMeasureModeExactly;
                childHeightMeasureMode.contents =
                  isUndefined childHeight.contents ? CssMeasureModeUndefined : CssMeasureModeExactly;
                /*
                 * According to the spec, if the main size is not definite and the
                 * child's inline axis is parallel to the main axis (i.e. it's
                 * horizontal), the child should be sized using "UNDEFINED" in
                 * the main size. Otherwise use "AT_MOST" in the cross axis.
                 */
                if (
                  (not isMainAxisRow && isUndefined childWidth.contents) &&
                  not (isUndefined availableInnerWidth)
                ) {
                  childWidth.contents = availableInnerWidth;
                  childWidthMeasureMode.contents = CssMeasureModeAtMost
                };
                /*
                 * If child has no defined size in the cross axis and is set to stretch, set the cross
                 * axis to be measured exactly with the available inner width
                 */
                let _ =
                  layoutNodeInternal
                    currentAbsoluteChild
                    childWidth.contents
                    childHeight.contents
                    direction
                    childWidthMeasureMode.contents
                    childHeightMeasureMode.contents
                    false
                    absMeasureString;
                childWidth.contents =
                  currentAbsoluteChild.layout.measuredWidth +.
                  getMarginAxis currentAbsoluteChild CssFlexDirectionRow;
                childHeight.contents =
                  currentAbsoluteChild.layout.measuredHeight +.
                  getMarginAxis currentAbsoluteChild CssFlexDirectionColumn
              };
              let _ =
                layoutNodeInternal
                  currentAbsoluteChild
                  childWidth.contents
                  childHeight.contents
                  direction
                  CssMeasureModeExactly
                  CssMeasureModeExactly
                  true
                  absLayoutString;
              if (
                isTrailingPosDefinedWithFallback currentAbsoluteChild mainAxis &&
                not (isLeadingPosDefinedWithFallback currentAbsoluteChild mainAxis)
              ) {
                setLayoutLeadingPositionForAxis
                  currentAbsoluteChild
                  mainAxis
                  (
                    layoutMeasuredDimensionForAxis node mainAxis -.
                    layoutMeasuredDimensionForAxis currentAbsoluteChild mainAxis -.
                    getTrailingPositionWithFallback currentAbsoluteChild mainAxis
                  )
              };
              if (
                isTrailingPosDefinedWithFallback currentAbsoluteChild crossAxis &&
                not (isLeadingPosDefinedWithFallback currentAbsoluteChild crossAxis)
              ) {
                setLayoutLeadingPositionForAxis
                  currentAbsoluteChild
                  crossAxis
                  (
                    layoutMeasuredDimensionForAxis node crossAxis -.
                    layoutMeasuredDimensionForAxis currentAbsoluteChild crossAxis -.
                    getTrailingPositionWithFallback currentAbsoluteChild crossAxis
                  )
              }
            };
            currentAbsoluteChildRef.contents = currentAbsoluteChild.nextChild
          };
          /* STEP 11: SETTING TRAILING POSITIONS FOR CHILDREN */
          if performLayout {
            let needsMainTrailingPos =
              mainAxis == CssFlexDirectionRowReverse || mainAxis == CssFlexDirectionColumnReverse;
            let needsCrossTrailingPos =
              crossAxis == CssFlexDirectionRowReverse || crossAxis == CssFlexDirectionColumnReverse;
            /* Set trailing position if necessary. */
            if (needsMainTrailingPos || needsCrossTrailingPos) {
              for i in 0 to (childCount - 1) {
                let child = node.children.(i);
                if needsMainTrailingPos {
                  setTrailingPosition node child mainAxis
                };
                if needsCrossTrailingPos {
                  setTrailingPosition node child crossAxis
                }
              }
            }
          }
        }
      }
    }
    /** END_GENERATED **/
  };
  let layoutNode node availableWidth availableHeight parentDirection => {
    /* Increment the generation count. This will force the recursive routine to visit*/
    /* all dirty nodes at least once. Subsequent visits will be skipped if the input*/
    /* parameters don't change.*/
    gCurrentGenerationCount.contents = gCurrentGenerationCount.contents + 1;
    /* If the caller didn't specify a height/width, use the dimensions*/
    /* specified in the style.*/
    let (width, widthMeasureMode) =
      if (not (isUndefined availableWidth)) {
        (availableWidth, CssMeasureModeExactly)
      } else if (
        isStyleDimDefined node CssFlexDirectionRow
      ) {
        (node.style.width +. getMarginAxis node CssFlexDirectionRow, CssMeasureModeExactly)
      } else if (
        node.style.maxWidth >= zero
      ) {
        (node.style.maxWidth, CssMeasureModeAtMost)
      } else {
        (availableWidth, CssMeasureModeUndefined)
      };
    let (height, heightMeasureMode) =
      if (not (isUndefined availableHeight)) {
        (availableHeight, CssMeasureModeExactly)
      } else if (
        isStyleDimDefined node CssFlexDirectionColumn
      ) {
        (node.style.height +. getMarginAxis node CssFlexDirectionColumn, CssMeasureModeExactly)
      } else if (
        node.style.maxHeight >= zero
      ) {
        (node.style.maxHeight, CssMeasureModeAtMost)
      } else {
        (availableHeight, CssMeasureModeUndefined)
      };
    if (
      layoutNodeInternal
        node width height parentDirection widthMeasureMode heightMeasureMode true initialString
    ) {
      setPosition node node.layout.direction;
      if gPrintTree.contents {
        LayoutPrint.printCssNode (node, {printLayout: true, printChildren: true, printStyle: true})
      }
    }
  };
};
