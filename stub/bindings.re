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
  style: createStyle (),
  isDirty: true,
  /* Since context is mutated, every node must have its own new copy */
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
          i < Array.length node.children ?
            node.children.(i) = x : node.children = Array.append node.children [|x|];
          i + 1
        } else {
          i
        }
    )
    0
    oldChildren
};

let cssNodeIsDirty node => true /* node.isDirty node.context */;

let cssNodeChildCount node => Array.length node.children;

let cssNodeGetChild node i =>
  if (i >= Array.length node.children) {
    Nativeint.zero
  } else {
    node.children.(i).selfRef
  };

let cssNodeCalculateLayout node aw ah pd => Layout.layoutNode node aw ah pd;

/* New Layout */
Callback.register "CSSNodeSetHasNewLayout" (fun node hasNewLayout => node.hasNewLayout = hasNewLayout);

Callback.register "CSSNodeGetHasNewLayout" (fun node => node.hasNewLayout);

/* Style */
Callback.register "CSSNodeStyleSetWidth" (fun node width => node.style = {...node.style, width});

Callback.register "CSSNodeStyleGetWidth" (fun node => node.style.width);

Callback.register "CSSNodeStyleSetHeight" (fun node height => node.style = {...node.style, height});

Callback.register "CSSNodeStyleGetHeight" (fun node => node.style.height);

Callback.register "CSSNodeStyleSetFlexGrow" (fun node flexGrow => node.style = {...node.style, flexGrow});

Callback.register "CSSNodeStyleGetFlexGrow" cssGetFlexGrow;

Callback.register
  "CSSNodeStyleSetFlexShrink" (fun node flexShrink => node.style = {...node.style, flexShrink});

Callback.register "CSSNodeStyleGetFlexShrink" cssGetFlexShrink;

Callback.register "CSSNodeStyleSetFlexWrap" (fun node flexWrap => node.style = {...node.style, flexWrap});

Callback.register "CSSNodeStyleGetFlexWrap" (fun node => node.style.flexWrap);

Callback.register
  "CSSNodeStyleSetJustifyContent" (fun node justifyContent => node.style = {...node.style, justifyContent});

Callback.register "CSSNodeStyleGetJustifyContent" (fun node => node.style.justifyContent);

Callback.register
  "CSSNodeStyleSetAlignItems" (fun node alignItems => node.style = {...node.style, alignItems});

Callback.register "CSSNodeStyleGetAlignItems" (fun node => node.style.alignItems);

Callback.register
  "CSSNodeStyleSetAlignContent" (fun node alignContent => node.style = {...node.style, alignContent});

Callback.register "CSSNodeStyleGetAlignContent" (fun node => node.style.alignContent);

Callback.register "CSSNodeStyleSetAlignSelf" (fun node alignSelf => node.style = {...node.style, alignSelf});

Callback.register "CSSNodeStyleGetAlignSelf" (fun node => node.style.alignSelf);

Callback.register "CSSNodeStyleSetMaxWidth" (fun node maxWidth => node.style = {...node.style, maxWidth});

Callback.register "CSSNodeStyleGetMaxWidth" (fun node => node.style.maxWidth);

Callback.register "CSSNodeStyleSetMaxHeight" (fun node maxHeight => node.style = {...node.style, maxHeight});

Callback.register "CSSNodeStyleGetMaxHeight" (fun node => node.style.maxHeight);

Callback.register "CSSNodeStyleSetMinWidth" (fun node minWidth => node.style = {...node.style, minWidth});

Callback.register "CSSNodeStyleGetMinWidth" (fun node => node.style.minWidth);

Callback.register "CSSNodeStyleSetMinHeight" (fun node minHeight => node.style = {...node.style, minHeight});

Callback.register "CSSNodeStyleGetMinHeight" (fun node => node.style.minHeight);

Callback.register "CSSNodeStyleGetDirection" (fun node => node.style.direction);

Callback.register "CSSNodeStyleSetDirection" (fun node direction => node.style = {...node.style, direction});

Callback.register "CSSNodeStyleGetFlexDirection" (fun node => node.style.flexDirection);

Callback.register
  "CSSNodeStyleSetPositionType" (fun node positionType => node.style = {...node.style, positionType});

Callback.register "CSSNodeStyleGetPositionType" (fun node => node.style.positionType);

Callback.register
  "CSSNodeStyleSetFlexDirection" (fun node flexDirection => node.style = {...node.style, flexDirection});

Callback.register "CSSNodeStyleGetOverflow" (fun node => node.style.overflow);

Callback.register "CSSNodeStyleSetFlexBasis" (fun node flexBasis => node.style = {...node.style, flexBasis});

Callback.register "CSSNodeStyleGetFlexBasis" cssGetFlexBasis;

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

Callback.register "CSSNodeStyleSetFlex" (fun node flex => {...node.style, flex});

Callback.register "CSSNodeStyleSetOverflow" (fun node overflow => node.style = {...node.style, overflow});

Callback.register
  "CSSNodeStyleSetPadding"
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
  "CSSNodeStyleSetMargin"
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
  "CSSNodeStyleSetBorder"
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
  "CSSNodeStyleSetPosition"
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
  "CSSNodeStyleGetPosition" (fun node edge => computedEdgeValuePosition node.style edge cssUndefined);

Callback.register "CSSNodeStyleGetMargin" (fun node edge => computedEdgeValueMargin node.style edge 0);

Callback.register "CSSNodeStyleGetBorder" (fun node edge => computedEdgeValueBorder node.style edge 0);

Callback.register "CSSNodeStyleGetPadding" (fun node edge => computedEdgeValuePadding node.style edge 0);

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
        node.measure = Some (fun node w wm h hm => cssMeasureFFI node.selfRef w wm h hm)
      } else {
        node.measure = None
      }
    }
  );

Callback.register "CSSNodeGetMeasureFunc" (fun node => node.context.measureFuncPtr);

Callback.register "GetMeasurement" (fun width height => {width, height});
