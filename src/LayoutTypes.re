type printOptions = {printLayout: bool, printStyle: bool, printChildren: bool};

type direction =
  | CSS_DIRECTION_NEGATIVE_ONE_WHATEVER_THAT_MEANS /* 'inherit' */
  | CssDirectionInherit /* 'inherit' */
  | CssDirectionLtr /* 'ltr'     */
  | CssDirectionRtl /* 'rtl'     */;

type flexDirection =
  | CssFlexDirectionRow /* 'row' */
  | CssFlexDirectionRowReverse
  /* 'row-reverse' */
  | CssFlexDirectionColumn /* 'column' */
  | CssFlexDirectionColumnReverse;

/* 'column-reverse' */
type justify =
  | CssJustifyFlexStart /* 'flex-start' */
  | CssJustifyCenter /* 'center' */
  | CssJustifyFlexEnd /* 'flex-end' */
  | CssJustifySpaceBetween /* 'space-between' */
  | CssJustifySpaceAround /* space-around' */;

type align =
  | CssAlignAuto
  | CssAlignFlexStart /* 'flex-start' */
  | CssAlignCenter /* 'center' */
  | CssAlignFlexEnd /* 'flex-end' */
  | CssAlignStretch /* 'stretch' */;

type positionType =
  | CssPositionRelative /* 'relative' */
  | CssPositionAbsolute /* 'absolute' */;

type measureMode =
  /* there are some places in the reference implementation that set
   * the measure mode to negative one.*/
  /* https://github.com/facebook/css-layout/pull/185#r64995699 */
  | CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS
  | CssMeasureModeUndefined /* 'undefined' */
  | CssMeasureModeExactly /* 'exactly' */
  | CssMeasureModeAtMost /* 'at-most' */;

let css_max_cached_result_count = 6;

type unitOfM = LayoutValue.scalar;


/**
 * Intentionally, nothing is mutable inside each
 */
type cachedMeasurement = {
  mutable availableWidth: unitOfM,
  mutable availableHeight: unitOfM,
  mutable widthMeasureMode: measureMode,
  mutable heightMeasureMode: measureMode,
  mutable computedWidth: unitOfM,
  mutable computedHeight: unitOfM
};

type overflow =
  | Visible
  | Scroll
  | Hidden;

type wrapType =
  | CssNoWrap
  | CssWrap;

type dimensions = {width: unitOfM, height: unitOfM};

type specificDirection =
  | Left
  | Right
  | Top
  | Bottom;

type coordinates = {left: unitOfM, top: unitOfM};

type cssStyle = {
  direction: direction,
  flexDirection: flexDirection,
  justifyContent: justify,
  alignContent: align,
  alignItems: align,
  alignSelf: align,
  positionType: positionType,
  flexWrap: wrapType,
  overflow: overflow,
  flexGrow: unitOfM,
  flexShrink: unitOfM,
  flexBasis: unitOfM,
  marginLeft: unitOfM,
  marginTop: unitOfM,
  marginRight: unitOfM,
  marginBottom: unitOfM,
  marginStart: unitOfM,
  marginEnd: unitOfM,
  width: unitOfM,
  height: unitOfM,
  minWidth: unitOfM,
  minHeight: unitOfM,
  maxWidth: unitOfM,
  maxHeight: unitOfM,
  left: unitOfM,
  top: unitOfM,
  right: unitOfM,
  bottom: unitOfM,
  /**
   * Start position.
   */
  start: unitOfM,
  /**
   * End position.
   */
  endd: unitOfM,
  /**
   * You should skip all the rules that contain negative values for the
   * following attributes. For example:
   *   {padding: 10, paddingLeft: -5}
   * should output:
   *   {left: 10 ...}
   * the following two are incorrect:
   *   {left: -5 ...}
   *   {left: 0 ...}
   */
  paddingLeft: unitOfM,
  paddingTop: unitOfM,
  paddingRight: unitOfM,
  paddingBottom: unitOfM,
  paddingStart: unitOfM,
  paddingEnd: unitOfM,
  borderLeft: unitOfM,
  borderTop: unitOfM,
  borderRight: unitOfM,
  borderBottom: unitOfM,
  borderStart: unitOfM,
  borderEnd: unitOfM
};


/**
 * Analog to "computed style" - the position takes into account all of the CSS
 * styles and inheritance.
 */
type cssLayout = {
  mutable left: unitOfM,
  mutable top: unitOfM,
  mutable right: unitOfM,
  mutable bottom: unitOfM,
  mutable width: unitOfM,
  mutable height: unitOfM,
  mutable direction: direction,
  /* Instead of recomputing the entire layout every single time, we
   * cache some information to break early when nothing changed */
  mutable hasNewLayout: bool,
  mutable generationCount: int,
  mutable lastParentDirection: direction,
  mutable computedFlexBasis: unitOfM,
  mutable nextCachedMeasurementsIndex: int,
  /**
   * Hardcoded to 6 previous measurements.
   */
  mutable cachedMeasurement1: cachedMeasurement,
  mutable cachedMeasurement2: cachedMeasurement,
  mutable cachedMeasurement3: cachedMeasurement,
  mutable cachedMeasurement4: cachedMeasurement,
  mutable cachedMeasurement5: cachedMeasurement,
  mutable cachedMeasurement6: cachedMeasurement,
  mutable measuredWidth: unitOfM,
  mutable measuredHeight: unitOfM,
  mutable cachedLayout: cachedMeasurement
};

type node 'context = {
  style: cssStyle,
  layout: cssLayout,
  mutable lineIndex: int,
  mutable nextChild: node 'context,
  measure: 'context => unitOfM => measureMode => unitOfM => measureMode => dimensions,
  print: option ('context => unit),
  mutable children: array (node 'context),
  isDirty: 'context => bool,
  context: 'context
};
/* static css_position_t leading[4] = { */
/*   /* CssFlexDirectionColumn = */ CSS_TOP, */
/*   /* CssFlexDirectionColumnReverse = */ CSS_BOTTOM, */
/*   /* CssFlexDirectionRow = */ CSS_LEFT, */
/*   /* CssFlexDirectionRowReverse = */ CSS_RIGHT */
/* }; */
/* static css_position_t trailing[4] = { */
/*   /* CssFlexDirectionColumn = */ CSS_BOTTOM, */
/*   /* CssFlexDirectionColumnReverse = */ CSS_TOP, */
/*   /* CssFlexDirectionRow = */ CSS_RIGHT, */
/*   /* CssFlexDirectionRowReverse = */ CSS_LEFT */
/* }; */
/* static css_position_t pos[4] = { */
/*   /* CssFlexDirectionColumn = */ CSS_TOP, */
/*   /* CssFlexDirectionColumnReverse = */ CSS_BOTTOM, */
/*   /* CssFlexDirectionRow = */ CSS_LEFT, */
/*   /* CssFlexDirectionRowReverse = */ CSS_RIGHT */
/* }; */
/* static css_dimension_t dim[4] = { */
/*   /* CssFlexDirectionColumn = */ CSS_HEIGHT, */
/*   /* CssFlexDirectionColumnReverse = */ CSS_HEIGHT, */
/*   /* CssFlexDirectionRow = */ CSS_WIDTH, */
/*   /* CssFlexDirectionRowReverse = */ CSS_WIDTH */
/* }; */
