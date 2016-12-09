module Create (Node: Spec.Node) (Encoding: Spec.Encoding) => {
  type printOptions = {printLayout: bool, printStyle: bool, printChildren: bool};
  type edge =
    | Start
    | End
    | Left
    | Right
    | Vertical
    | Horizontal
    | Top
    | Bottom
    | All;
  type direction =
    | CSS_DIRECTION_NEGATIVE_ONE_WHATEVER_THAT_MEANS /* inherit */
    | Inherit /* 'inherit' */
    | Ltr /* 'ltr'     */
    | Rtl /* 'rtl'     */;
  type flexDirection =
    /* 'row-reverse' */
    | Column /* 'column' */
    | ColumnReverse
    | Row /* 'row' */
    | RowReverse;
  /* 'column-reverse' */
  type justify =
    | JustifyFlexStart /* 'flex-start' */
    | JustifyCenter /* 'center' */
    | JustifyFlexEnd /* 'flex-end' */
    | JustifySpaceBetween /* 'space-between' */
    | JustifySpaceAround /* space-around' */;
  type align =
    | AlignAuto
    | AlignFlexStart /* 'flex-start' */
    | AlignCenter /* 'center' */
    | AlignFlexEnd /* 'flex-end' */
    | AlignStretch /* 'stretch' */;
  type positionType =
    | Relative /* 'relative' */
    | Absolute /* 'absolute' */;
  type measureMode =
    /* there are some places in the reference implementation that set
     * the measure mode to negative one.*/
    /* https://github.com/facebook/css-layout/pull/185#r64995699 */
    | Undefined /* 'undefined' */
    | Exactly /* 'exactly' */
    | AtMost /* 'at-most' */
    | CSS_MEASURE_MODE_NEGATIVE_ONE_WHATEVER_THAT_MEANS;
  let css_max_cached_result_count = 6;

  /**
   * We really *want* to be using the definitions from `Encoding`, but we want
   * to guarantee that all of the floating point operations are inlined.
   * Therefore, we directly reference the `HardCodedEncoding`. functors,
   */
  type unitOfM = HardCodedEncoding.scalar;

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
    | Hidden
    | Scroll;
  type wrapType =
    | CssNoWrap
    | CssWrap;
  type dimensions = {width: unitOfM, height: unitOfM};
  type coordinates = {left: unitOfM, top: unitOfM};
  /* TODO: Benchmark the immutable version versus mutable version */
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
    flex: unitOfM,
    flexGrow: unitOfM,
    flexShrink: unitOfM,
    flexBasis: unitOfM,
    marginLeft: unitOfM,
    marginTop: unitOfM,
    marginRight: unitOfM,
    marginBottom: unitOfM,
    marginStart: unitOfM,
    marginEnd: unitOfM,
    marginHorizontal: unitOfM,
    marginVertical: unitOfM,
    margin: unitOfM,
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
    horizontal: unitOfM,
    vertical: unitOfM,
    position: unitOfM,
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
    paddingHorizontal: unitOfM,
    paddingVertical: unitOfM,
    padding: unitOfM,
    borderLeft: unitOfM,
    borderTop: unitOfM,
    borderRight: unitOfM,
    borderBottom: unitOfM,
    borderStart: unitOfM,
    borderEnd: unitOfM,
    borderHorizontal: unitOfM,
    borderVertical: unitOfM,
    border: unitOfM
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
    mutable generationCount: int,
    mutable computedFlexBasisGeneration: int,
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
  type node = {
    mutable selfRef: nativeint, /* a C pointer pointing to itself, needed for introp with C's memory management */
    mutable style: cssStyle,
    layout: cssLayout,
    mutable lineIndex: int,
    mutable nextChild: node,
    mutable hasNewLayout: bool,
    mutable measure: option (node => unitOfM => measureMode => unitOfM => measureMode => dimensions),
    print: option (Node.context => unit),
    mutable children: array node,
    mutable childrenCount: int,
    mutable isDirty: bool,
    context: Node.context
  };
};
