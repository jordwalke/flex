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

external logcat : string => unit = "logcat";

module Create (Node: Spec.Node) (Encoding: Spec.Encoding) => {

  /**
   * It's okay to shadow existing modules, even though you can't export two
   * modules with the same name.
   *
   * But it is incorrect that we even generate this module here, as it is a
   * second *instance* of the LayoutSupport module for a given encoding/node.
   * There should be at most one, and that should be shared by reference.
   */
  module LayoutSupport_TODO_REMOVE_ME = LayoutSupport.Create Node Encoding;
  open LayoutSupport_TODO_REMOVE_ME.LayoutTypes;
  open LayoutSupport_TODO_REMOVE_ME;
  /* open Encoding; */
  open HardCodedEncoding;
  let shouldFilter = true;
  let indent n => String.make n ' ' |> logcat;
  let print_number_0 (indentNum, str, number) =>
    if (not shouldFilter || not (number == zero)) {
      indent indentNum;
      Printf.sprintf "%s: %s,\n" str (scalarToString number) |> logcat
    };
  let print_number_nan (indentNum, str, number) =>
    if (not shouldFilter || not (isUndefined number)) {
      indent indentNum;
      Printf.sprintf "%s: %s,\n" str (scalarToString number) |> logcat
    };
  let layoutStr layout =>
    "{left:" ^
    scalarToString layout.left ^
    ", top:" ^
    scalarToString layout.top ^
    ", width:" ^ scalarToString layout.width ^ ", height:" ^ scalarToString layout.height ^ "}";
  let rec printCssNodeRec (node, options, level) => {
    indent level;
    Printf.sprintf "{\n" |> logcat;
    switch node.print {
    | None => ()
    | Some printer => printer node.context
    };
    if options.printLayout {
      indent (level + 1);
      Printf.sprintf "layout: {\n" |> logcat;
      print_number_nan (level + 2, "width", node.layout.width);
      print_number_nan (level + 2, "height", node.layout.height);
      print_number_nan (level + 2, "measuredWidth", node.layout.measuredWidth);
      print_number_nan (level + 2, "measuredHeight", node.layout.measuredHeight);
      print_number_0 (level + 2, "top", node.layout.top);
      print_number_0 (level + 2, "left", node.layout.left);
      /* indent (level + 2); */
      /* Printf.sprintf "shouldUpdate: %b,\n" node.layout.shouldUpdate; */
      /* print_number_0 (level + 2, "generationCount", float_of_int node.layout.generationCount); */
      indent (level + 1);
      Printf.sprintf "},\n" |> logcat
    };
    if options.printStyle {
      indent (level + 1);
      Printf.sprintf "style: {\n" |> logcat;
      indent (level + 2);
      switch node.style.justifyContent {
      | JustifyFlexStart => Printf.sprintf "justify: 'start',\n" |> logcat
      | JustifyCenter => Printf.sprintf "justify: 'center',\n" |> logcat
      | JustifyFlexEnd => Printf.sprintf "justify: 'flexend',\n" |> logcat
      | JustifySpaceBetween => Printf.sprintf "justify: 'speceBetween',\n" |> logcat
      | JustifySpaceAround => Printf.sprintf "justify: 'speceAround',\n" |> logcat
      };
      if (node.style.flexDirection == Column) {
        indent (level + 2);
        Printf.sprintf "flexDirection: 'column',\n" |> logcat
      } else if (
        node.style.flexDirection == ColumnReverse
      ) {
        indent (level + 2);
        Printf.sprintf "flexDirection: 'column-reverse',\n" |> logcat
      } else if (
        node.style.flexDirection == Row
      ) {
        indent (level + 2);
        Printf.sprintf "flexDirection: 'row',\n" |> logcat
      } else if (
        node.style.flexDirection == RowReverse
      ) {
        indent (level + 2);
        Printf.sprintf "flexDirection: 'row-reverse',\n" |> logcat
      };
      if (node.style.justifyContent == JustifyCenter) {
        indent (level + 2);
        Printf.sprintf "justifyContent: 'center',\n" |> logcat
      } else if (
        node.style.justifyContent == JustifyFlexEnd
      ) {
        indent (level + 2);
        Printf.sprintf "justifyContent: 'flex-end',\n" |> logcat
      } else if (
        node.style.justifyContent == JustifySpaceAround
      ) {
        indent (level + 2);
        Printf.sprintf "justifyContent: 'space-around',\n" |> logcat
      } else if (
        node.style.justifyContent == JustifySpaceBetween
      ) {
        indent (level + 2);
        Printf.sprintf "justifyContent: 'space-between',\n" |> logcat
      };
      if (node.style.alignItems == AlignCenter) {
        indent (level + 2);
        Printf.sprintf "alignItems: 'center',\n" |> logcat
      } else if (
        node.style.alignItems == AlignFlexEnd
      ) {
        indent (level + 2);
        Printf.sprintf "alignItems: 'flex-end',\n" |> logcat
      } else if (
        node.style.alignItems == AlignStretch
      ) {
        indent (level + 2);
        Printf.sprintf "alignItems: 'stretch',\n" |> logcat
      };
      if (node.style.alignContent == AlignCenter) {
        indent (level + 2);
        Printf.sprintf "alignContent: 'center',\n" |> logcat
      } else if (
        node.style.alignContent == AlignFlexEnd
      ) {
        indent (level + 2);
        Printf.sprintf "alignContent: 'flex-end',\n" |> logcat
      } else if (
        node.style.alignContent == AlignStretch
      ) {
        indent (level + 2);
        Printf.sprintf "alignContent: 'stretch',\n" |> logcat
      };
      if (node.style.flexWrap == CssNoWrap) {
        indent (level + 2);
        Printf.sprintf "flexWrap: 'nowrap',\n" |> logcat
      } else {
        indent (level + 2);
        Printf.sprintf "flexWrap: 'wrap',\n" |> logcat
      };
      if (node.style.alignSelf == AlignFlexStart) {
        indent (level + 2);
        Printf.sprintf "alignSelf: 'flex-start',\n" |> logcat
      } else if (
        node.style.alignSelf == AlignCenter
      ) {
        indent (level + 2);
        Printf.sprintf "alignSelf: 'center',\n" |> logcat
      } else if (
        node.style.alignSelf == AlignFlexEnd
      ) {
        indent (level + 2);
        Printf.sprintf "alignSelf: 'flex-end',\n" |> logcat
      } else if (
        node.style.alignSelf == AlignStretch
      ) {
        indent (level + 2);
        Printf.sprintf "alignSelf: 'stretch',\n" |> logcat
      };
      print_number_nan (level + 2, "flex", node.style.flex);
      print_number_nan (level + 2, "flexGrow", node.style.flexGrow);
      print_number_nan (level + 2, "flexBasis", node.style.flexBasis);
      print_number_nan (level + 2, "flexShrink", node.style.flexShrink);
      if (node.style.overflow == Hidden) {
        indent (level + 2);
        Printf.sprintf "overflow: 'hidden',\n" |> logcat
      } else if (
        node.style.overflow == Visible
      ) {
        indent (level + 2);
        Printf.sprintf "overflow: 'visible',\n" |> logcat
      } else {
        indent (level + 2);
        Printf.sprintf "overflow: 'scroll',\n" |> logcat
      };
      print_number_0 (level + 2, "marginLeft", node.style.marginLeft);
      print_number_0 (level + 2, "marginRight", node.style.marginRight);
      print_number_0 (level + 2, "marginTop", node.style.marginTop);
      print_number_0 (level + 2, "marginBottom", node.style.marginBottom);
      print_number_0 (level + 2, "paddingLeft", node.style.paddingLeft);
      print_number_0 (level + 2, "paddingRight", node.style.paddingRight);
      print_number_0 (level + 2, "paddingTop", node.style.paddingTop);
      print_number_0 (level + 2, "paddingBottom", node.style.paddingBottom);
      print_number_0 (level + 2, "borderLeftWidth", node.style.borderLeft);
      print_number_0 (level + 2, "borderRightWidth", node.style.borderRight);
      print_number_0 (level + 2, "borderTopWidth", node.style.borderTop);
      print_number_0 (level + 2, "borderBottomWidth", node.style.borderBottom);
      print_number_nan (level + 2, "borderStartWidth", node.style.borderStart);
      print_number_nan (level + 2, "borderEndWidth", node.style.borderEnd);
      print_number_nan (level + 2, "paddingStart", node.style.paddingStart);
      print_number_nan (level + 2, "paddingEnd", node.style.paddingEnd);
      print_number_nan (level + 2, "marginStart", node.style.marginStart);
      print_number_nan (level + 2, "marginEnd", node.style.marginEnd);
      print_number_nan (level + 2, "width", node.style.width);
      print_number_nan (level + 2, "height", node.style.height);
      print_number_nan (level + 2, "maxWidth", node.style.maxWidth);
      print_number_nan (level + 2, "maxHeight", node.style.maxHeight);
      print_number_nan (level + 2, "minWidth", node.style.minWidth);
      print_number_nan (level + 2, "minHeight", node.style.minHeight);
      if (node.style.positionType == Absolute) {
        indent (level + 2);
        Printf.sprintf "position: 'absolute', " |> logcat
      };
      print_number_nan (level + 2, "left", node.style.left);
      print_number_nan (level + 2, "right", node.style.right);
      print_number_nan (level + 2, "top", node.style.top);
      print_number_nan (level + 2, "bottom", node.style.bottom);
      indent (level + 1);
      Printf.sprintf "},\n" |> logcat
    };
    if (options.printChildren && Array.length node.children > 0) {
      indent (level + 1);
      Printf.sprintf "children: [\n" |> logcat;
      for i in 0 to (Array.length node.children - 1) {
        printCssNodeRec (node.children.(i), options, level + 2)
      };
      indent (level + 1);
      Printf.sprintf "]},\n" |> logcat
    } else {
      indent level;
      Printf.sprintf "},%!\n" |> logcat
    }
  };
  let printCssNode (node, options) => printCssNodeRec (node, options, 0);
};

/* Force allocating a new node */
let cssNodeNew ptr => {
  ...theNullNode,
  selfRef: ptr,
  children: [||],
  layout: createLayout (),
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

let cssNodeCalculateLayout node aw ah pd => {
  module LayoutPrint = Create Node Encoding;
  logcat "before:";
  logcat (Printf.sprintf "aw: %d" aw);
  LayoutPrint.printCssNode (node, {printLayout: true, printChildren: true, printStyle: true});
  Layout.layoutNode node aw ah pd;
  logcat "after:";
  LayoutPrint.printCssNode (node, {printLayout: true, printChildren: true, printStyle: true})
};

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
