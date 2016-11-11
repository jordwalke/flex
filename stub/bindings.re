open LayoutSupport;

open LayoutTypes;

/* Force allocating a new node */
let cssNodeNew ptr => {
  let r = {
    ...theNullNode,
    selfRef: ptr,
    children: [||],
    layout: createLayout (),
    style: createStyle (),
    context: ()
  };
  r
};

let cssNodeSetSelfRef node ptr => node.selfRef = ptr;

let cssNodeGetSelfRef node => node.selfRef;

let cssNodeInsertChild node child index => {
  if (Array.length node.children == node.childrenCount) {
    let fill = Array.make ((Array.length node.children + 1) * 2) theNullNode;
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
  node.childrenCount = node.childrenCount - 1;
  Array.fold_left
    (
      fun i x =>
        if (x.selfRef != child) {
          node.children.(i) = x;
          i + 1
        } else {
          i
        }
    )
    0
    oldChildren
};

let cssNodeStyleSetDirection node direction => node.style = {...node.style, direction};

let cssNodeStyleGetDirection node => node.style.direction;

let cssNodeIsDirty node => node.isDirty node.context;

let cssNodeChildCount node => node.childrenCount;

let cssNodeGetChild node i => node.children.(i).selfRef;

let cssNodeCalculateLayout = Layout.layoutNode;

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
  "CSSNodeStyleSetJustifyContent"
  (fun node justifyContent => node.style = {...node.style, justifyContent});

Callback.register "CSSNodeStyleGetJustifyContent" (fun node => node.style.justifyContent);

Callback.register
  "CSSNodeStyleSetAlignItems" (fun node alignItems => node.style = {...node.style, alignItems});

Callback.register "CSSNodeStyleGetAlignItems" (fun node => node.style.alignItems);

/* Padding */
let cssNodeStyleSetPadding node edge v =>
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
    node.style = {...node.style, paddingTop: v, paddingBottom: v, paddingLeft: v, paddingRight: v}
  };

Callback.register "CSSNodeStyleSetPadding" cssNodeStyleSetPadding;

/* Layout */
Callback.register "CSSNodeLayoutGetWidth" (fun node => node.layout.width);

Callback.register "CSSNodeLayoutGetHeight" (fun node => node.layout.height);

Callback.register "CSSNodeLayoutGetTop" (fun node => node.layout.top);

Callback.register "CSSNodeLayoutGetBottom" (fun node => node.layout.bottom);

Callback.register "CSSNodeLayoutGetLeft" (fun node => node.layout.left);

Callback.register "CSSNodeLayoutGetRight" (fun node => node.layout.right);

/* Misc */
Callback.register "minInt" (fun () => min_int);

Callback.register "CSSNodeNew" cssNodeNew;

Callback.register "CSSNodeGetSelfRef" cssNodeGetSelfRef;

Callback.register "CSSNodeInsertChild" cssNodeInsertChild;

Callback.register "CSSNodeRemoveChild" cssNodeRemoveChild;

Callback.register "CSSNodeChildCount" cssNodeChildCount;

Callback.register "CSSNodeGetChild" cssNodeGetChild;

Callback.register "CSSNodeStyleGetDirection" cssNodeStyleGetDirection;

Callback.register "CSSNodeStyleSetDirection" cssNodeStyleSetDirection;

Callback.register "CSSNodeIsDirty" cssNodeIsDirty;

Callback.register "CSSNodeCalculateLayout" cssNodeCalculateLayout;
