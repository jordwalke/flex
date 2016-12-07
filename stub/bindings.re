module Node = {
  type context = {mutable measureFuncPtr: nativeint, mutable contextPtr: nativeint};
  let nullContext = {measureFuncPtr: Nativeint.zero, contextPtr: Nativeint.zero};
};

module Encoding = FixedEncoding;

module LayoutTypes = LayoutTypes.Create Node Encoding;

module LayoutSupport = LayoutSupport.Create Node Encoding;

module Layout = Layout.Create Node Encoding;

open LayoutSupport;

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
  isDirty: fun c => true,
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

Callback.register
  "CSSNodeStyleSetFlex"
  (
    fun node flex => {
      ...node.style,
      flexBasis: setIfUndefined node.style.flexBasis flex,
      flexGrow: setIfUndefined node.style.flexGrow flex,
      flexShrink: setIfUndefined node.style.flexShrink flex
    }
  );

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
      | CSSEdgeBottom =>
        Printf.printf "setting bottom to %d %!\n" v;
        node.style = {...node.style, paddingBottom: v}
      | CSSEdgeStart =>
        node.style = {
          Printf.printf "setting start to %d %!\n" v;
          {...node.style, paddingStart: v}
        }
      | CSSEdgeEnd =>
        Printf.printf "setting end to %d %!\n" v;
        node.style = {...node.style, paddingEnd: v}
      | CSSEdgeHorizontal =>
        node.style = {
          ...node.style,
          paddingLeft: setIfZero node.style.paddingLeft v,
          paddingRight: setIfZero node.style.paddingRight v
        }
      | CSSEdgeVertical =>
        node.style = {
          ...node.style,
          paddingTop: setIfZero node.style.paddingTop v,
          paddingBottom: setIfZero node.style.paddingBottom v
        }
      | CSSEdgeAll =>
        node.style = {
          ...node.style,
          paddingTop: setIfZero node.style.paddingTop v,
          paddingBottom: setIfZero node.style.paddingBottom v,
          paddingLeft: setIfZero node.style.paddingLeft v,
          paddingRight: setIfZero node.style.paddingRight v
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
      | CSSEdgeHorizontal =>
        node.style = {
          ...node.style,
          marginLeft: setIfZero node.style.marginLeft v,
          marginRight: setIfZero node.style.marginRight v
        }
      | CSSEdgeVertical =>
        node.style = {
          ...node.style,
          marginTop: setIfZero node.style.marginTop v,
          marginBottom: setIfZero node.style.marginBottom v
        }
      | CSSEdgeAll =>
        node.style = {
          ...node.style,
          marginTop: setIfZero node.style.marginTop v,
          marginBottom: setIfZero node.style.marginBottom v,
          marginLeft: setIfZero node.style.marginLeft v,
          marginRight: setIfZero node.style.marginRight v
        }
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
      | CSSEdgeHorizontal =>
        node.style = {
          ...node.style,
          borderLeft: setIfZero node.style.borderLeft v,
          borderRight: setIfZero node.style.borderRight v
        }
      | CSSEdgeVertical =>
        node.style = {
          ...node.style,
          borderTop: setIfZero node.style.borderTop v,
          borderBottom: setIfZero node.style.borderBottom v
        }
      | CSSEdgeAll =>
        node.style = {
          ...node.style,
          borderTop: setIfZero node.style.borderTop v,
          borderBottom: setIfZero node.style.borderBottom v,
          borderLeft: setIfZero node.style.borderLeft v,
          borderRight: setIfZero node.style.borderRight v
        }
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
      | CSSEdgeHorizontal =>
        node.style = {
          ...node.style,
          left: setIfZero node.style.left v,
          right: setIfZero node.style.right v
        }
      | CSSEdgeVertical =>
        node.style = {
          ...node.style,
          top: setIfZero node.style.top v,
          bottom: setIfZero node.style.bottom v
        }
      | CSSEdgeAll =>
        node.style = {
          ...node.style,
          left: setIfZero node.style.left v,
          bottom: setIfZero node.style.bottom v,
          top: setIfZero node.style.top v,
          right: setIfZero node.style.right v
        }
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
        node.measure = Some (fun node w wm h hm => cssMeasureFFI node.selfRef w wm h hm)
      } else {
        node.measure = None
      }
    }
  );

Callback.register "CSSNodeGetMeasureFunc" (fun node => node.context.measureFuncPtr);

Callback.register "GetMeasurement" (fun width height => {width, height});
