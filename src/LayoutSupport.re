external reraise : exn => _ = "%reraise";


/**
 * Copyright (c) 2014, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 * Converted to Reason:
 * Replace:
 *
 *   function() {
 *
 * With:
 *   fun () => {
 *
 * :%s/\<var\>/let/g
 *
 * For enums:
 * Either:
 *
 *    :%s/\<\([A-Z_]\+\)\>/\L&/g
 *
 * Or Better:
 *   Just define the types as such below, and then regex away the constants
 *   value declarations.
 *
 * %s/\_s*let \<\([A-Z_]\+\)\> = .*;//g
 *
 * Since we've defined our own css undefined, use it.
 * :%s/\<CSS_UNDEFINED\>/CSS_UNDEFINED/g
 */

/**
 * So we can replace:
 * `!blah(x,y)` with `not <| blah(x,y)` with the same precedence.
 */
open LayoutTypes;

open LayoutValue;

let positive_flex_is_auto = false;

let gCurrentGenerationCount = 0;

let failOnDummyMeasure = true;


/**
 * In the integer encoding of all
 */
let dummyMeasure context width widthMeasureMode height heightMeasureMode =>
  if failOnDummyMeasure {
    raise (
      Invalid_argument (
        "A node does not have its measure function " ^ " implemented, yet requires measurement"
      )
    )
  } else {
    {width: zero, height: zero}
  };

let dummyIsDirty context => false;

/*
   node->style.align_items = CssAlignStretch;
   node->style.align_content = CssAlignFlexStart;

   node->style.direction = CssDirectionInherit;
   node->style.flex_direction = CssFlexDirectionColumn;

   node->style.overflow = Visible;


   node->layout.dimensions[CSS_WIDTH] = CSS_UNDEFINED;
   node->layout.dimensions[CSS_HEIGHT] = CSS_UNDEFINED;

   // Such that the comparison is always going to be false
   node->layout.last_parent_direction = (css_direction_t)-1;
   node->layout.should_update = true;
   node->layout.next_cached_measurements_index = 0;

   node->layout.measured_dimensions[CSS_WIDTH] = CSS_UNDEFINED;
   node->layout.measured_dimensions[CSS_HEIGHT] = CSS_UNDEFINED;
   node->layout.cached_layout.width_measure_mode = (css_measure_mode_t)-1;
   node->layout.cached_layout.height_measure_mode = (css_measure_mode_t)-1;

 */
let dummyCachedMeasurement = {
  availableWidth: zero,
  availableHeight: zero,
  widthMeasureMode: CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS,
  heightMeasureMode: CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS,
  computedWidth: zero,
  computedHeight: zero
};

let defaultStyle = {
  direction: CssDirectionInherit,
  flexDirection: CssFlexDirectionColumn,
  justifyContent: CssJustifyFlexStart,
  alignContent: CssAlignFlexStart,
  alignItems: CssAlignStretch,
  alignSelf: CssAlignAuto,
  positionType: CssPositionRelative,
  flexWrap: CssNoWrap,
  overflow: Visible,
  /**
   * Properties that start out as zero.
   */
  flexGrow: zero,
  flexShrink: zero,
  flexBasis: cssUndefined,
  marginLeft: zero,
  marginTop: zero,
  marginRight: zero,
  marginBottom: zero,
  paddingLeft: zero,
  paddingTop: zero,
  paddingRight: zero,
  paddingBottom: zero,
  borderLeft: zero,
  borderTop: zero,
  borderRight: zero,
  borderBottom: zero,
  /**
   * Properties that start out as undefined.
   */
  width: cssUndefined,
  height: cssUndefined,
  minWidth: cssUndefined,
  minHeight: cssUndefined,
  maxWidth: cssUndefined,
  maxHeight: cssUndefined,
  left: cssUndefined,
  top: cssUndefined,
  right: cssUndefined,
  bottom: cssUndefined,
  start: cssUndefined,
  endd: cssUndefined,
  marginStart: cssUndefined,
  marginEnd: cssUndefined,
  paddingStart: cssUndefined,
  paddingEnd: cssUndefined,
  borderStart: cssUndefined,
  borderEnd: cssUndefined
};

let rec theNullNode = {
  children: [||],
  style: defaultStyle,
  layout: {
    direction: CssDirectionInherit,
    /* Instead of recomputing the entire layout every single time, we
     * cache some information to break early when nothing changed */
    hasNewLayout: true,
    generationCount: 0,
    lastParentDirection: CSS_DIRECTION_NEGATIVE_ONE_WHATEVER_THAT_MEANS,
    nextCachedMeasurementsIndex: 0,
    /**
     * Hardcoded to 6 previous measurements.
     */
    cachedMeasurement1: dummyCachedMeasurement,
    cachedMeasurement2: dummyCachedMeasurement,
    cachedMeasurement3: dummyCachedMeasurement,
    cachedMeasurement4: dummyCachedMeasurement,
    cachedMeasurement5: dummyCachedMeasurement,
    cachedMeasurement6: dummyCachedMeasurement,
    cachedLayout: dummyCachedMeasurement,
    /**
     * Start out as zero.
     */
    computedFlexBasis: cssUndefined,
    left: zero,
    top: zero,
    right: zero,
    bottom: zero,
    /**
     * Start out as undefined.
     */
    width: cssUndefined,
    height: cssUndefined,
    measuredWidth: cssUndefined,
    measuredHeight: cssUndefined
  },
  lineIndex: 0,
  /**
   * As a clever trick, to encode "NULL" node, we can create a recursive
   * binding and point nextChild to itself, and interpreting that as NULL.
   */
  nextChild: theNullNode,
  measure: dummyMeasure,
  print: None,
  isDirty: dummyIsDirty,
  context: ()
};


/**
 * It is critical that this actually be a different reference
 * than theNullNode.
 */
let createNode context => {
  ...theNullNode,
  children: [||],
  /**
   * We can keep the original style because it's immutable, but layout is not.
   */
  layout: {...theNullNode.layout, direction: CssDirectionInherit},
  context
};

let createNode withChildren::children andStyle::style=defaultStyle context => {
  ...theNullNode,
  children,
  style,
  /**
   * We can keep the original style because it's immutable, but layout is not.
   */
  layout: {...theNullNode.layout, direction: CssDirectionInherit},
  context
};


/**
 * Layout getters.
 */
let layoutMeasuredDimensionForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.layout.measuredHeight
  | CssFlexDirectionColumnReverse => node.layout.measuredHeight
  | CssFlexDirectionRow => node.layout.measuredWidth
  | CssFlexDirectionRowReverse => node.layout.measuredWidth
  };


/**
 * Pos[] based layout position for axis.
 */
let layoutPosPositionForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.layout.top
  | CssFlexDirectionColumnReverse => node.layout.bottom
  | CssFlexDirectionRow => node.layout.left
  | CssFlexDirectionRowReverse => node.layout.right
  };


/**
 * Leading[] based layout position for axis.
 */
let layoutLeadingPositionForAxis = layoutPosPositionForAxis;


/**
 * Trailing[] based layout position for axis.
 */
let layoutTrailingPositionForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.layout.bottom
  | CssFlexDirectionColumnReverse => node.layout.top
  | CssFlexDirectionRow => node.layout.right
  | CssFlexDirectionRowReverse => node.layout.left
  };


/**
 * Dimension style getter.
 */
let styleDimensionForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.style.height
  | CssFlexDirectionColumnReverse => node.style.height
  | CssFlexDirectionRow => node.style.width
  | CssFlexDirectionRowReverse => node.style.width
  };


/**
 * Leading style getters.
 */
let styleLeadingPositionForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.style.top
  | CssFlexDirectionColumnReverse => node.style.bottom
  | CssFlexDirectionRow => node.style.left
  | CssFlexDirectionRowReverse => node.style.right
  };

let styleLeadingPositionForAxisOrZero node axis => {
  let leadingPos = styleLeadingPositionForAxis node axis;
  not (isUndefined leadingPos) ? leadingPos : zero
};

let styleLeadingPaddingForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.style.paddingTop
  | CssFlexDirectionColumnReverse => node.style.paddingBottom
  | CssFlexDirectionRow => node.style.paddingLeft
  | CssFlexDirectionRowReverse => node.style.paddingRight
  };

let styleLeadingMarginForAxis node leadingAxis =>
  switch leadingAxis {
  | CssFlexDirectionColumn => node.style.marginTop
  | CssFlexDirectionColumnReverse => node.style.marginBottom
  | CssFlexDirectionRow => node.style.marginLeft
  | CssFlexDirectionRowReverse => node.style.marginRight
  };

let styleLeadingBorderForAxis node leadingAxis =>
  switch leadingAxis {
  | CssFlexDirectionColumn => node.style.borderTop
  | CssFlexDirectionColumnReverse => node.style.borderBottom
  | CssFlexDirectionRow => node.style.borderLeft
  | CssFlexDirectionRowReverse => node.style.borderRight
  };


/**
 * Trailing style getters.
 */
let styleTrailingPositionForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.style.bottom
  | CssFlexDirectionColumnReverse => node.style.top
  | CssFlexDirectionRow => node.style.right
  | CssFlexDirectionRowReverse => node.style.left
  };

let styleTrailingPositionForAxisOrZero node axis => {
  let leadingPos = styleTrailingPositionForAxis node axis;
  not (isUndefined leadingPos) ? leadingPos : zero
};

let styleTrailingPaddingForAxis node axis =>
  switch axis {
  | CssFlexDirectionColumn => node.style.paddingBottom
  | CssFlexDirectionColumnReverse => node.style.paddingTop
  | CssFlexDirectionRow => node.style.paddingRight
  | CssFlexDirectionRowReverse => node.style.paddingLeft
  };

let styleTrailingMarginForAxis node trailingAxis =>
  switch trailingAxis {
  | CssFlexDirectionColumn => node.style.marginBottom
  | CssFlexDirectionColumnReverse => node.style.marginTop
  | CssFlexDirectionRow => node.style.marginRight
  | CssFlexDirectionRowReverse => node.style.marginLeft
  };

let styleTrailingBorderForAxis node trailingAxis =>
  switch trailingAxis {
  | CssFlexDirectionColumn => node.style.borderBottom
  | CssFlexDirectionColumnReverse => node.style.borderTop
  | CssFlexDirectionRow => node.style.borderRight
  | CssFlexDirectionRowReverse => node.style.borderLeft
  };


/**
 * Dim[] based layout dimension setter.
 */
let setDimLayoutDimensionForAxis node axis value =>
  switch axis {
  | CssFlexDirectionColumn => node.layout.height = value
  | CssFlexDirectionColumnReverse => node.layout.height = value
  | CssFlexDirectionRow => node.layout.width = value
  | CssFlexDirectionRowReverse => node.layout.width = value
  };

let setLayoutMeasuredDimensionForAxis node axis value =>
  switch axis {
  | CssFlexDirectionColumn => node.layout.measuredHeight = value
  | CssFlexDirectionColumnReverse => node.layout.measuredHeight = value
  | CssFlexDirectionRow => node.layout.measuredWidth = value
  | CssFlexDirectionRowReverse => node.layout.measuredWidth = value
  };


/**
 * Leading layout setter.
 */
let setLayoutLeadingPositionForAxis node axis value =>
  switch axis {
  | CssFlexDirectionColumn => node.layout.top = value
  | CssFlexDirectionColumnReverse => node.layout.bottom = value
  | CssFlexDirectionRow => node.layout.left = value
  | CssFlexDirectionRowReverse => node.layout.right = value
  };


/**
 * Trailing layout setter.
 */
let setLayoutTrailingPositionForAxis node axis value =>
  switch axis {
  | CssFlexDirectionColumn => node.layout.bottom = value
  | CssFlexDirectionColumnReverse => node.layout.top = value
  | CssFlexDirectionRow => node.layout.right = value
  | CssFlexDirectionRowReverse => node.layout.left = value
  };

let resolveDirection node parentDirection => {
  let direction = node.style.direction;
  if (direction === CssDirectionInherit) {
    parentDirection === CssDirectionInherit ? CssDirectionLtr : parentDirection
  } else {
    direction
  }
};

let resolveAxis flex_direction direction =>
  if (direction === CssDirectionRtl) {
    if (flex_direction === CssFlexDirectionRow) {
      CssFlexDirectionRowReverse
    } else if (
      flex_direction === CssFlexDirectionRowReverse
    ) {
      CssFlexDirectionRow
    } else {
      flex_direction
    }
  } else {
    flex_direction
  };

let isRowDirection flexDirection =>
  flexDirection === CssFlexDirectionRow || flexDirection === CssFlexDirectionRowReverse;

let isColumnDirection flexDirection =>
  flexDirection === CssFlexDirectionColumn || flexDirection === CssFlexDirectionColumnReverse;

let getCrossFlexDirection flex_direction direction =>
  isColumnDirection flex_direction ? resolveAxis CssFlexDirectionRow direction : CssFlexDirectionColumn;

let isFlex node =>
  node.style.positionType === CssPositionRelative && (
    node.style.flexGrow != zero || node.style.flexShrink != zero
  );

let getLeadingMargin node axis =>
  if (isRowDirection axis && not (isUndefined node.style.marginStart)) {
    node.style.marginStart
  } else {
    styleLeadingMarginForAxis node axis
  };

let getTrailingMargin node axis =>
  if (isRowDirection axis && not (isUndefined node.style.marginEnd)) {
    node.style.marginEnd
  } else {
    styleTrailingMarginForAxis node axis
  };

let getLeadingPadding node axis =>
  if (isRowDirection axis && not (isUndefined node.style.paddingStart) && node.style.paddingStart >= zero) {
    node.style.paddingStart
  } else {
    let leadingPadding = styleLeadingPaddingForAxis node axis;
    if (leadingPadding >= zero) {
      leadingPadding
    } else {
      zero
    }
  };

let getTrailingPadding node axis =>
  if (isRowDirection axis && not (isUndefined node.style.paddingEnd) && node.style.paddingEnd >= zero) {
    node.style.paddingEnd
  } else {
    let trailingPadding = styleTrailingPaddingForAxis node axis;
    trailingPadding >= zero ? trailingPadding : zero
  };

let getLeadingBorder node axis =>
  if (isRowDirection axis && not (isUndefined node.style.borderStart) && node.style.borderStart >= zero) {
    node.style.borderStart
  } else {
    let leadingBorder = styleLeadingBorderForAxis node axis;
    leadingBorder >= zero ? leadingBorder : zero
  };

let getTrailingBorder node axis =>
  if (isRowDirection axis && not (isUndefined node.style.borderEnd) && node.style.borderEnd >= zero) {
    node.style.borderEnd
  } else {
    let trailingBorder = styleTrailingBorderForAxis node axis;
    trailingBorder >= zero ? trailingBorder : zero
  };

let getLeadingPaddingAndBorder node axis => getLeadingPadding node axis +. getLeadingBorder node axis;

let getTrailingPaddingAndBorder node axis => getTrailingPadding node axis +. getTrailingBorder node axis;

let getMarginAxis node axis => getLeadingMargin node axis +. getTrailingMargin node axis;

let getPaddingAndBorderAxis node axis =>
  getLeadingPaddingAndBorder node axis +. getTrailingPaddingAndBorder node axis;

let getAlignItem node child =>
  child.style.alignSelf !== CssAlignAuto ? child.style.alignSelf : node.style.alignItems;

let getDimWithMargin node axis =>
  layoutMeasuredDimensionForAxis node axis +. getLeadingMargin node axis +. getTrailingMargin node axis;

let isStyleDimDefined node axis => {
  let value = styleDimensionForAxis node axis;
  not (isUndefined value) && value >= zero
};

let isLayoutDimDefined node axis => {
  let value = layoutMeasuredDimensionForAxis node axis;
  not (isUndefined value) && value >= zero
};

let isLeadingPosDefinedWithFallback node axis =>
  isRowDirection axis && not (isUndefined node.style.start) ||
  not (isUndefined (styleLeadingPositionForAxis node axis));

let isTrailingPosDefinedWithFallback node axis =>
  isRowDirection axis && not (isUndefined node.style.endd) ||
  not (isUndefined (styleTrailingPositionForAxis node axis));


/**
 * The C implementation calls this `getLeadingPosition`.
 */
let getLeadingPositionWithFallback node axis =>
  if (isRowDirection axis && not (isUndefined node.style.start)) {
    node.style.start
  } else {
    styleLeadingPositionForAxisOrZero node axis
  };


/**
 * The C implementation calls this `getTrailingPosition`.
 */
let getTrailingPositionWithFallback node axis =>
  if (isRowDirection axis && not (isUndefined node.style.endd)) {
    node.style.endd
  } else {
    styleTrailingPositionForAxisOrZero node axis
  };

let normalizePosition position => not (isUndefined position) ? position : zero;

let boundAxisWithinMinAndMax node axis value => {
  let (min, max) =
    if (isColumnDirection axis) {
      (node.style.minHeight, node.style.maxHeight)
    } else if (isRowDirection axis) {
      (node.style.minWidth, node.style.maxWidth)
    } else {
      (cssUndefined, cssUndefined)
    };
  let boundValue = value;
  let nextBoundValue =
    if (not (isUndefined max) && max >= zero && boundValue > max) {
      max
    } else {
      boundValue
    };
  let nextNextBoundValue =
    if (not (isUndefined min) && min >= zero && nextBoundValue < min) {
      min
    } else {
      nextBoundValue
    };
  nextNextBoundValue
};

let fminf a b => isUndefined b || a < b ? a : b;

let fmaxf a b => isUndefined b || a > b ? a : b;

/* Like boundAxisWithinMinAndMax but also ensures that the value doesn't go below the
 * padding and border amount. */
let boundAxis node axis value =>
  fmaxf (boundAxisWithinMinAndMax node axis value) (getPaddingAndBorderAxis node axis);

/* /* When the user specifically sets a value for width or height */ */
/* let setDimensionFromStyle (node, axis) => */
/*   /* The parent already computed us a width or height. We just skip it */ */
/*   if (isLayoutDimDefined (node, axis)) { */
/*     () */
/*   } else if */
/*     /* We only run if there's a width or height defined */ */
/*     (not (isStyleDimDefined (node, axis))) { */
/*     () */
/*   } else { */
/*     /* The dimensions can never be smaller than the padding and border */ */
/*     let dimValue = */
/*       fmaxf (boundAxis (node, axis, node.style [dim [axis]])) (getPaddingAndBorderAxis (node, axis)); */
/*     setDimLayoutDimensionForAxis node dimValue */
/*   }; */

/**
 * Sets trailing position for a child node for a given axis.
 */
let setTrailingPosition node child axis => {
  let measuredChildDimensionForAxis = layoutMeasuredDimensionForAxis child axis;
  let childLayoutPosValueForAxis = layoutPosPositionForAxis child axis;
  let nextValue =
    layoutMeasuredDimensionForAxis node axis -. measuredChildDimensionForAxis -. childLayoutPosValueForAxis;
  setLayoutTrailingPositionForAxis child axis nextValue
};

/* If both left and right are defined, then use left. Otherwise return */
/* +left or -right depending on which is defined. */
let getRelativePosition node axis =>
  if (isLeadingPosDefinedWithFallback node axis) {
    getLeadingPositionWithFallback node axis
  } else {
    -. getTrailingPositionWithFallback node axis
  };


/**
 * TODO: A more functional version of this.
 */
let setPosition node direction => {
  let mainAxis = resolveAxis node.style.flexDirection direction;
  let crossAxis = getCrossFlexDirection mainAxis direction;
  setLayoutLeadingPositionForAxis
    node mainAxis (getLeadingMargin node mainAxis +. getRelativePosition node mainAxis);
  setLayoutTrailingPositionForAxis
    node mainAxis (getTrailingMargin node mainAxis +. getRelativePosition node mainAxis);
  setLayoutLeadingPositionForAxis
    node crossAxis (getLeadingMargin node crossAxis +. getRelativePosition node crossAxis);
  setLayoutTrailingPositionForAxis
    node crossAxis (getTrailingMargin node crossAxis +. getRelativePosition node crossAxis)
};

let cssNodeStyleGetFlex node =>
  if (node.style.flexGrow > zero) {
    node.style.flexGrow
  } else if (node.style.flexShrink > zero) {
    -. node.style.flexShrink
  } else {
    zero
  };
