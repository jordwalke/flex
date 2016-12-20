module Node = {
  type context = {mutable measureFuncPtr: nativeint, mutable contextPtr: nativeint};
  let nullContext = {measureFuncPtr: Nativeint.zero, contextPtr: Nativeint.zero};
};

module Encoding = FixedEncoding;

module LayoutTypes = LayoutTypes.Create Node Encoding;

module Layout = Layout.Create Node Encoding;

open Layout.LayoutSupport;

open LayoutTypes;

open Encoding;

external cssMeasureFFI : nativeint => int => measureMode => int => measureMode => dimensions = "cssMeasureFFI_bytecode"
                                                                    "cssMeasureFFI";

/* Force allocating a new node */
let cssNodeNew ptr => {
  ...theNullNode,
  selfRef: ptr,
  children: [||],
  layout: createLayout (),
  /* Since context is mutated, every node must have its own new copy */
  context: {measureFuncPtr: Nativeint.zero, contextPtr: Nativeint.zero}
};

let cssNodeSetSelfRef node ptr => node.selfRef = ptr;

let cssNodeGetSelfRef node => node.selfRef;


/**
 * TODO: Handle the case where `child` is already a `child` of `node` (`Yoga`
 * doesn't appear to handle this case though).
 */
let cssNodeInsertChild node child index => {
  assert (child.parent === theNullNode);
  assert (node.measure === None);
  let capacity = Array.length node.children;
  if (capacity == node.childrenCount) {
    /* TODO:Simply use Array.fill (no need to allocate a separate `fill` array
     * */
    let fill = Array.make (capacity + 1) theNullNode;
    Array.blit node.children 0 fill 0 capacity;
    node.children = fill
  };
  for i in node.childrenCount downto (index + 1) {
    node.children.(i) = node.children.(i - 1)
  };
  node.childrenCount = node.childrenCount + 1;
  node.children.(index) = child;
  child.parent = node;
  Layout.LayoutSupport.markDirtyInternal node
};

let nodeListRemove nodeWithList index => {
  let list = nodeWithList.children;
  let removed = list.(index);
  list.(index) = theNullNode;
  for i in index to (nodeWithList.childrenCount - 2) {
    list.(i) = list.(i + 1);
    list.(i + 1) = theNullNode
  };
  nodeWithList.childrenCount = nodeWithList.childrenCount - 1;
  removed
};

let rec nodeListDeleteImpl nodeWithList node from =>
  if (from < nodeWithList.childrenCount) {
    if (nodeWithList.children.(from) === node) {
      nodeListRemove nodeWithList from
    } else {
      nodeListDeleteImpl nodeWithList node (from + 1)
    }
  } else {
    theNullNode
  };

let nodeListDelete nodeWithList node => nodeListDeleteImpl nodeWithList node 0;

let cssNodeRemoveChild nodeWithList child =>
  if (nodeListDelete nodeWithList child !== theNullNode) {
    child.parent = theNullNode;
    Layout.LayoutSupport.markDirtyInternal nodeWithList
  };

let cssNodeIsDirty node => node.isDirty;

let cssNodeChildCount node => node.childrenCount;

let cssNodeGetChild node i => {
  let capacity = node.childrenCount;
  if (i >= capacity) {
    Nativeint.zero
  } else {
    node.children.(i).selfRef
  }
};

let cssNodeCalculateLayout node aw ah pd => Layout.layoutNode node aw ah pd;

/* New Layout */
Callback.register "YGNodeSetHasNewLayout" (fun node hasNewLayout => node.hasNewLayout = hasNewLayout);

Callback.register "YGNodeGetHasNewLayout" (fun node => node.hasNewLayout);

/* Style */
Callback.register
  "YGNodeStyleSetWidth"
  (
    fun node width =>
      if (node.style.width != width) {
        markDirtyInternal node;
        node.style = {...node.style, width}
      }
  );

Callback.register "YGNodeStyleGetWidth" (fun node => node.style.width);

Callback.register "YGNodeStyleSetHeight" (fun node height => node.style = {...node.style, height});

Callback.register "YGNodeStyleGetHeight" (fun node => node.style.height);

Callback.register "YGNodeStyleSetFlexGrow" (fun node flexGrow => node.style = {...node.style, flexGrow});

Callback.register "YGNodeStyleGetFlexGrow" cssGetFlexGrow;

Callback.register
  "YGNodeStyleSetFlexShrink" (fun node flexShrink => node.style = {...node.style, flexShrink});

Callback.register "YGNodeStyleGetFlexShrink" cssGetFlexShrink;

Callback.register "YGNodeStyleSetFlexWrap" (fun node flexWrap => node.style = {...node.style, flexWrap});

Callback.register "YGNodeStyleGetFlexWrap" (fun node => node.style.flexWrap);

Callback.register
  "YGNodeStyleSetJustifyContent" (fun node justifyContent => node.style = {...node.style, justifyContent});

Callback.register "YGNodeStyleGetJustifyContent" (fun node => node.style.justifyContent);

Callback.register
  "YGNodeStyleSetAlignItems" (fun node alignItems => node.style = {...node.style, alignItems});

Callback.register "YGNodeStyleGetAlignItems" (fun node => node.style.alignItems);

Callback.register
  "YGNodeStyleSetAlignContent" (fun node alignContent => node.style = {...node.style, alignContent});

Callback.register "YGNodeStyleGetAlignContent" (fun node => node.style.alignContent);

Callback.register "YGNodeStyleSetAlignSelf" (fun node alignSelf => node.style = {...node.style, alignSelf});

Callback.register "YGNodeStyleGetAlignSelf" (fun node => node.style.alignSelf);

Callback.register "YGNodeStyleSetMaxWidth" (fun node maxWidth => node.style = {...node.style, maxWidth});

Callback.register "YGNodeStyleGetMaxWidth" (fun node => node.style.maxWidth);

Callback.register "YGNodeStyleSetMaxHeight" (fun node maxHeight => node.style = {...node.style, maxHeight});

Callback.register "YGNodeStyleGetMaxHeight" (fun node => node.style.maxHeight);

Callback.register "YGNodeStyleSetMinWidth" (fun node minWidth => node.style = {...node.style, minWidth});

Callback.register "YGNodeStyleGetMinWidth" (fun node => node.style.minWidth);

Callback.register "YGNodeStyleSetMinHeight" (fun node minHeight => node.style = {...node.style, minHeight});

Callback.register "YGNodeStyleGetMinHeight" (fun node => node.style.minHeight);

Callback.register "YGNodeStyleGetDirection" (fun node => node.style.direction);

Callback.register "YGNodeStyleSetDirection" (fun node direction => node.style = {...node.style, direction});

Callback.register "YGNodeStyleGetFlexDirection" (fun node => node.style.flexDirection);

Callback.register
  "YGNodeStyleSetPositionType" (fun node positionType => node.style = {...node.style, positionType});

Callback.register "YGNodeStyleGetPositionType" (fun node => node.style.positionType);

Callback.register
  "YGNodeStyleSetFlexDirection" (fun node flexDirection => node.style = {...node.style, flexDirection});

Callback.register "YGNodeStyleGetOverflow" (fun node => node.style.overflow);

Callback.register "YGNodeStyleSetFlexBasis" (fun node flexBasis => node.style = {...node.style, flexBasis});

Callback.register "YGNodeStyleGetFlexBasis" cssGetFlexBasis;

let setIfUndefined oldValue newValue =>
  if (isUndefined oldValue) {
    newValue
  } else {
    oldValue
  };

let setIfZero oldValue newValue =>
  if (0 == oldValue) {
    newValue
  } else {
    oldValue
  };

Callback.register "YGNodeStyleSetFlex" (fun node flex => node.style = {...node.style, flex});

Callback.register "YGNodeStyleSetOverflow" (fun node overflow => node.style = {...node.style, overflow});

Callback.register
  "YGNodeStyleSetPadding"
  (
    fun node edge v =>
      switch edge {
      | Left => node.style = {...node.style, paddingLeft: v}
      | Top => node.style = {...node.style, paddingTop: v}
      | Right => node.style = {...node.style, paddingRight: v}
      | Bottom => node.style = {...node.style, paddingBottom: v}
      | Start => node.style = {...node.style, paddingStart: v}
      | End => node.style = {...node.style, paddingEnd: v}
      | Horizontal => node.style = {...node.style, paddingHorizontal: v}
      | Vertical => node.style = {...node.style, paddingVertical: v}
      | All => node.style = {...node.style, padding: v}
      }
  );

Callback.register
  "YGNodeStyleSetMargin"
  (
    fun node edge v =>
      switch edge {
      | Left => node.style = {...node.style, marginLeft: v}
      | Top => node.style = {...node.style, marginTop: v}
      | Right => node.style = {...node.style, marginRight: v}
      | Bottom => node.style = {...node.style, marginBottom: v}
      | Start => node.style = {...node.style, marginStart: v}
      | End => node.style = {...node.style, marginEnd: v}
      | Horizontal => node.style = {...node.style, marginHorizontal: v}
      | Vertical => node.style = {...node.style, marginVertical: v}
      | All => node.style = {...node.style, margin: v}
      }
  );

Callback.register
  "YGNodeStyleSetBorder"
  (
    fun node edge v =>
      switch edge {
      | Left => node.style = {...node.style, borderLeft: v}
      | Top => node.style = {...node.style, borderTop: v}
      | Right => node.style = {...node.style, borderRight: v}
      | Bottom => node.style = {...node.style, borderBottom: v}
      | Start => node.style = {...node.style, borderStart: v}
      | End => node.style = {...node.style, borderEnd: v}
      | Horizontal => node.style = {...node.style, borderHorizontal: v}
      | Vertical => node.style = {...node.style, borderVertical: v}
      | All => node.style = {...node.style, border: v}
      }
  );

Callback.register
  "YGNodeStyleSetPosition"
  (
    fun node edge v =>
      switch edge {
      | Left => node.style = {...node.style, left: v}
      | Top => node.style = {...node.style, top: v}
      | Right => node.style = {...node.style, right: v}
      | Bottom => node.style = {...node.style, bottom: v}
      | Start => node.style = {...node.style, start: v}
      | End => node.style = {...node.style, endd: v}
      | Horizontal => node.style = {...node.style, horizontal: v}
      | Vertical => node.style = {...node.style, vertical: v}
      | All => node.style = {...node.style, position: v}
      }
  );

Callback.register
  "YGNodeStyleGetPosition" (fun node edge => computedEdgeValuePosition node.style edge cssUndefined);

Callback.register "YGNodeStyleGetMargin" (fun node edge => computedEdgeValueMargin node.style edge 0);

Callback.register "YGNodeStyleGetBorder" (fun node edge => computedEdgeValueBorder node.style edge 0);

Callback.register "YGNodeStyleGetPadding" (fun node edge => computedEdgeValuePadding node.style edge 0);

/* Layout */
Callback.register "YGNodeLayoutGetWidth" (fun node => node.layout.width);

Callback.register "YGNodeLayoutGetHeight" (fun node => node.layout.height);

Callback.register "YGNodeLayoutGetTop" (fun node => node.layout.top);

Callback.register "YGNodeLayoutGetBottom" (fun node => node.layout.bottom);

Callback.register "YGNodeLayoutGetLeft" (fun node => node.layout.left);

Callback.register "YGNodeLayoutGetRight" (fun node => node.layout.right);

Callback.register "YGNodeLayoutGetDirection" (fun node => node.layout.direction);

/* Misc */
Callback.register "minInt" (fun () => min_int);

Callback.register "YGNodeCopyStyle" (fun destNode srcNode => destNode.style = srcNode.style);

Callback.register "YGNodeNew" cssNodeNew;

Callback.register "YGNodeGetSelfRef" cssNodeGetSelfRef;

Callback.register "YGNodeInsertChild" cssNodeInsertChild;

Callback.register "YGNodeRemoveChild" cssNodeRemoveChild;

Callback.register "YGNodeChildCount" cssNodeChildCount;

Callback.register "YGNodeGetChild" cssNodeGetChild;

Callback.register "YGNodeIsDirty" cssNodeIsDirty;

Callback.register "YGNodeCalculateLayout" cssNodeCalculateLayout;

Callback.register "YGNodeSetContext" (fun node context => node.context.contextPtr = context);

Callback.register "YGNodeGetContext" (fun node => node.context.contextPtr);

Callback.register
  "YGNodeSetMeasureFunc"
  (
    fun node ptr => {
      node.context.measureFuncPtr = ptr;
      if (node.context.measureFuncPtr != Nativeint.zero) {
        node.measure = Some (fun node w wm h hm => cssMeasureFFI node.selfRef w wm h hm)
      } else {
        node.measure = None
      }
    }
  );

Callback.register "YGNodeGetMeasureFunc" (fun node => node.context.measureFuncPtr);

Callback.register "GetMeasurement" (fun width height => {width, height});
