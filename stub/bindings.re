open LayoutSupport;

open LayoutTypes;

open LayoutValue;

external cssMeasureFFI : nativeint =>
                         nativeint =>
                         int =>
                         measureMode =>
                         int =>
                         measureMode =>
                         dimensions = "cssMeasureFFI_bytecode" "cssMeasureFFI";

type bindingContext = {mutable measureFuncPtr: nativeint, mutable contextPtr: nativeint};

let rec theNullNode = {
  selfRef: Nativeint.zero,
  children: [||],
  childrenCount: 0,
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
  context: {measureFuncPtr: Nativeint.zero, contextPtr: Nativeint.zero}
};

/* Force allocating a new node */
let cssNodeNew ptr => {
  ...theNullNode,
  selfRef: ptr,
  children: [||],
  layout: createLayout (),
  style: createStyle (),
  context: {measureFuncPtr: Nativeint.zero, contextPtr: Nativeint.zero}
};

let cssNodeSetSelfRef node ptr => node.selfRef = ptr;

let cssNodeGetSelfRef node => node.selfRef;

let cssNodeInsertChild node child index => {
  if (Array.length node.children == node.childrenCount) {
    let fill = Array.make (Array.length node.children + 1) theNullNode;
    Array.blit node.children 0 fill 0 (Array.length node.children);
    node.children = fill
  };
  for i in node.childrenCount downto (index + 1) {
    node.children.(i) = node.children.(i - 1)
  };
  node.childrenCount = node.childrenCount + 1;
  node.children.(index) = child
};

let cssNodeRemoveChild node child => {
  let oldChildren = node.children;
  node.children = Array.make (Array.length oldChildren - 1) theNullNode;
  Array.fold_left
    (
      fun i x =>
        if (x.selfRef != child) {
          if (i < Array.length node.children) {
            node.children.(i) = x
          } else {
            node.children = Array.append node.children [|x|]
          };
          i + 1
        } else {
          i
        }
    )
    0
    oldChildren
};

let cssNodeIsDirty node => node.isDirty node.context;

let cssNodeChildCount node => Array.length node.children;

let cssNodeGetChild node i =>
  if (i >= Array.length node.children) {
    Nativeint.zero
  } else {
    node.children.(i).selfRef
  };

let cssNodeCalculateLayout node aw ah pd => Layout.layoutNode node aw ah pd;

type cssEdge =
  | CSSEdgeLeft
  | CSSEdgeTop
  | CSSEdgeRight
  | CSSEdgeBottom
  | CSSEdgeStart
  | CSSEdgeEnd
  | CSSEdgeHorizontal
  | CSSEdgeVertical
  | CSSEdgeAll;

/* New Layout */
Callback.register
  "CSSNodeSetHasNewLayout" (fun node hasNewLayout => node.hasNewLayout = hasNewLayout);

Callback.register "CSSNodeGetHasNewLayout" (fun node => node.hasNewLayout);

/* Style */
Callback.register "CSSNodeStyleSetWidth" (fun node width => node.style = {...node.style, width});

Callback.register "CSSNodeStyleGetWidth" (fun node => node.style.width);

Callback.register
  "CSSNodeStyleSetHeight" (fun node height => node.style = {...node.style, height});

Callback.register "CSSNodeStyleGetHeight" (fun node => node.style.height);

Callback.register
  "CSSNodeStyleSetFlexGrow" (fun node flexGrow => node.style = {...node.style, flexGrow});

Callback.register "CSSNodeStyleGetFlexGrow" (fun node => node.style.flexGrow);

Callback.register
  "CSSNodeStyleSetFlexShrink" (fun node flexShrink => node.style = {...node.style, flexShrink});

Callback.register "CSSNodeStyleGetFlexShrink" (fun node => node.style.flexShrink);

Callback.register
  "CSSNodeStyleSetFlexWrap" (fun node flexWrap => node.style = {...node.style, flexWrap});

Callback.register "CSSNodeStyleGetFlexWrap" (fun node => node.style.flexWrap);

Callback.register
  "CSSNodeStyleSetJustifyContent"
  (fun node justifyContent => node.style = {...node.style, justifyContent});

Callback.register "CSSNodeStyleGetJustifyContent" (fun node => node.style.justifyContent);

Callback.register
  "CSSNodeStyleSetAlignItems" (fun node alignItems => node.style = {...node.style, alignItems});

Callback.register "CSSNodeStyleGetAlignItems" (fun node => node.style.alignItems);

Callback.register
  "CSSNodeStyleSetAlignContent"
  (fun node alignContent => node.style = {...node.style, alignContent});

Callback.register "CSSNodeStyleGetAlignContent" (fun node => node.style.alignContent);

Callback.register
  "CSSNodeStyleSetAlignSelf" (fun node alignSelf => node.style = {...node.style, alignSelf});

Callback.register "CSSNodeStyleGetAlignSelf" (fun node => node.style.alignSelf);

Callback.register
  "CSSNodeStyleSetMaxWidth" (fun node maxWidth => node.style = {...node.style, maxWidth});

Callback.register "CSSNodeStyleGetMaxWidth" (fun node => node.style.maxWidth);

Callback.register
  "CSSNodeStyleSetMaxHeight" (fun node maxHeight => node.style = {...node.style, maxHeight});

Callback.register "CSSNodeStyleGetMaxHeight" (fun node => node.style.maxHeight);

Callback.register
  "CSSNodeStyleSetMinWidth" (fun node minWidth => node.style = {...node.style, minWidth});

Callback.register "CSSNodeStyleGetMinWidth" (fun node => node.style.minWidth);

Callback.register
  "CSSNodeStyleSetMinHeight" (fun node minHeight => node.style = {...node.style, minHeight});

Callback.register "CSSNodeStyleGetMinHeight" (fun node => node.style.minHeight);

Callback.register "CSSNodeStyleGetDirection" (fun node => node.style.direction);

Callback.register
  "CSSNodeStyleSetDirection" (fun node direction => node.style = {...node.style, direction});

Callback.register "CSSNodeStyleGetFlexDirection" (fun node => node.style.flexDirection);

Callback.register
  "CSSNodeStyleSetPositionType"
  (fun node positionType => node.style = {...node.style, positionType});

Callback.register "CSSNodeStyleGetPositionType" (fun node => node.style.positionType);

Callback.register
  "CSSNodeStyleSetFlexDirection"
  (fun node flexDirection => node.style = {...node.style, flexDirection});

Callback.register "CSSNodeStyleGetOverflow" (fun node => node.style.overflow);

Callback.register
  "CSSNodeStyleSetFlexBasis" (fun node flexBasis => node.style = {...node.style, flexBasis});

Callback.register "CSSNodeStyleGetFlexBasis" (fun node => node.style.flexBasis);

Callback.register
  "CSSNodeStyleSetOverflow" (fun node overflow => node.style = {...node.style, overflow});

Callback.register
  "CSSNodeStyleSetPadding"
  (
    fun node edge v =>
      switch edge {
      | CSSEdgeLeft => node.style = {...node.style, paddingLeft: v}
      | CSSEdgeTop => node.style = {...node.style, paddingTop: v}
      | CSSEdgeRight => node.style = {...node.style, paddingRight: v}
      | CSSEdgeBottom => node.style = {...node.style, paddingBottom: v}
      | CSSEdgeStart => node.style = {...node.style, paddingStart: v}
      | CSSEdgeEnd => node.style = {...node.style, paddingEnd: v}
      | CSSEdgeHorizontal => node.style = {...node.style, paddingLeft: v, paddingRight: v}
      | CSSEdgeVertical => node.style = {...node.style, paddingTop: v, paddingBottom: v}
      | CSSEdgeAll =>
        node.style = {
          ...node.style,
          paddingTop: v,
          paddingBottom: v,
          paddingLeft: v,
          paddingRight: v
        }
      }
  );

Callback.register
  "CSSNodeStyleSetMargin"
  (
    fun node edge v =>
      switch edge {
      | CSSEdgeLeft => node.style = {...node.style, marginLeft: v}
      | CSSEdgeTop => node.style = {...node.style, marginTop: v}
      | CSSEdgeRight => node.style = {...node.style, marginRight: v}
      | CSSEdgeBottom => node.style = {...node.style, marginBottom: v}
      | CSSEdgeStart => node.style = {...node.style, marginStart: v}
      | CSSEdgeEnd => node.style = {...node.style, marginEnd: v}
      | CSSEdgeHorizontal => node.style = {...node.style, marginLeft: v, marginRight: v}
      | CSSEdgeVertical => node.style = {...node.style, marginTop: v, marginBottom: v}
      | CSSEdgeAll =>
        node.style = {...node.style, marginTop: v, marginBottom: v, marginLeft: v, marginRight: v}
      }
  );

Callback.register
  "CSSNodeStyleSetBorder"
  (
    fun node edge v =>
      switch edge {
      | CSSEdgeLeft => node.style = {...node.style, borderLeft: v}
      | CSSEdgeTop => node.style = {...node.style, borderTop: v}
      | CSSEdgeRight => node.style = {...node.style, borderRight: v}
      | CSSEdgeBottom => node.style = {...node.style, borderBottom: v}
      | CSSEdgeStart => node.style = {...node.style, borderStart: v}
      | CSSEdgeEnd => node.style = {...node.style, borderEnd: v}
      | CSSEdgeHorizontal => node.style = {...node.style, borderLeft: v, borderRight: v}
      | CSSEdgeVertical => node.style = {...node.style, borderTop: v, borderBottom: v}
      | CSSEdgeAll =>
        node.style = {...node.style, borderTop: v, borderBottom: v, borderLeft: v, borderRight: v}
      }
  );

Callback.register
  "CSSNodeStyleSetPosition"
  (
    fun node edge v =>
      switch edge {
      | CSSEdgeLeft => node.style = {...node.style, left: v}
      | CSSEdgeTop => node.style = {...node.style, top: v}
      | CSSEdgeRight => node.style = {...node.style, right: v}
      | CSSEdgeBottom => node.style = {...node.style, bottom: v}
      | CSSEdgeStart => node.style = {...node.style, start: v}
      | CSSEdgeEnd => node.style = {...node.style, endd: v}
      | CSSEdgeHorizontal => node.style = {...node.style, left: v, right: v}
      | CSSEdgeVertical => node.style = {...node.style, top: v, bottom: v}
      | CSSEdgeAll => node.style = {...node.style, top: v, bottom: v, left: v, right: v}
      }
  );

Callback.register
  "CSSNodeStyleGetPosition"
  (
    fun node edge =>
      switch edge {
      | CSSEdgeLeft => node.style.left
      | CSSEdgeTop => node.style.top
      | CSSEdgeRight => node.style.right
      | CSSEdgeBottom => node.style.bottom
      | CSSEdgeStart => node.style.start
      | CSSEdgeEnd => node.style.endd
      | CSSEdgeHorizontal => node.style.left
      | CSSEdgeVertical => node.style.top
      | CSSEdgeAll => node.style.left
      }
  );

Callback.register
  "CSSNodeStyleGetMargin"
  (
    fun node edge =>
      switch edge {
      | CSSEdgeLeft => node.style.marginLeft
      | CSSEdgeTop => node.style.marginTop
      | CSSEdgeRight => node.style.marginRight
      | CSSEdgeBottom => node.style.marginBottom
      | CSSEdgeStart => node.style.marginStart
      | CSSEdgeEnd => node.style.marginEnd
      | CSSEdgeHorizontal => node.style.marginLeft
      | CSSEdgeVertical => node.style.marginTop
      | CSSEdgeAll => node.style.marginLeft
      }
  );

Callback.register
  "CSSNodeStyleGetBorder"
  (
    fun node edge =>
      switch edge {
      | CSSEdgeLeft => node.style.borderLeft
      | CSSEdgeTop => node.style.borderTop
      | CSSEdgeRight => node.style.borderRight
      | CSSEdgeBottom => node.style.borderBottom
      | CSSEdgeStart => node.style.borderStart
      | CSSEdgeEnd => node.style.borderEnd
      | CSSEdgeHorizontal => node.style.borderLeft
      | CSSEdgeVertical => node.style.borderTop
      | CSSEdgeAll => node.style.borderLeft
      }
  );

Callback.register
  "CSSNodeStyleGetPadding"
  (
    fun node edge =>
      switch edge {
      | CSSEdgeLeft => node.style.paddingLeft
      | CSSEdgeTop => node.style.paddingTop
      | CSSEdgeRight => node.style.paddingRight
      | CSSEdgeBottom => node.style.paddingBottom
      | CSSEdgeStart => node.style.paddingStart
      | CSSEdgeEnd => node.style.paddingEnd
      | CSSEdgeHorizontal => node.style.paddingLeft
      | CSSEdgeVertical => node.style.paddingTop
      | CSSEdgeAll => node.style.paddingLeft
      }
  );

/* Layout */
Callback.register "CSSNodeLayoutGetWidth" (fun node => node.layout.width);

Callback.register "CSSNodeLayoutGetHeight" (fun node => node.layout.height);

Callback.register "CSSNodeLayoutGetTop" (fun node => node.layout.top);

Callback.register "CSSNodeLayoutGetBottom" (fun node => node.layout.bottom);

Callback.register "CSSNodeLayoutGetLeft" (fun node => node.layout.left);

Callback.register "CSSNodeLayoutGetRight" (fun node => node.layout.right);

Callback.register "CSSNodeLayoutGetDirection" (fun node => node.layout.direction);

/* Misc */
Callback.register "minInt" (fun () => min_int);

Callback.register "CSSNodeNew" cssNodeNew;

Callback.register "CSSNodeGetSelfRef" cssNodeGetSelfRef;

Callback.register "CSSNodeInsertChild" cssNodeInsertChild;

Callback.register "CSSNodeRemoveChild" cssNodeRemoveChild;

Callback.register "CSSNodeChildCount" cssNodeChildCount;

Callback.register "CSSNodeGetChild" cssNodeGetChild;

Callback.register "CSSNodeIsDirty" cssNodeIsDirty;

Callback.register "CSSNodeCalculateLayout" cssNodeCalculateLayout;

Callback.register "CSSNodeSetContext" (fun node context => node.context.contextPtr = context);

Callback.register "CSSNodeGetContext" (fun node => node.context.contextPtr);

Callback.register
  "CSSNodeSetMeasureFunc"
  (
    fun node ptr => {
      node.context.measureFuncPtr = ptr;
      if (node.context.measureFuncPtr != Nativeint.zero) {
        node.measure = (
          fun node w wm h hm => cssMeasureFFI node.context.measureFuncPtr node.selfRef w wm h hm
        )
      } else {
        node.measure = dummyMeasure
      }
    }
  );

Callback.register "CSSNodeGetMeasureFunc" (fun node ptr => node.context.measureFuncPtr);

Callback.register "GetMeasurement" (fun width height => {width, height});
