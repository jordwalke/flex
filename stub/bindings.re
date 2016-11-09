open LayoutSupport;

open LayoutTypes;

/* Force allocating a new node */
let cssNodeNew ptr => {
  ...theNullNode,
  selfRef: ptr,
  children: [||],
  layout: createLayout (),
  style: createStyle (),
  context: ()
};

let cssNodeSetSelfRef node ptr => node.selfRef = ptr;

let cssNodeGetSelfRef node => node.selfRef;

let cssNodeInsertChild node child i => {
  if (Array.length node.children <= i) {
    let fill = Array.make (i - Array.length node.children + 1) theNullNode;
    node.children = Array.append node.children fill
  };
  node.children.(i) = child
};

let cssNodeGetChild node i => node.children.(i);

let cssNodeStyleSetDirection node direction => node.style.direction = direction;

let cssNodeStyleGetDirection node => node.style.direction;

let cssNodeIsDirty node => node.isDirty node.context;

let cssNodeGetChild node i => node.children.(i).selfRef;

let _ = Callback.register "CSSNodeNew" cssNodeNew;

let _ = Callback.register "CSSNodeGetSelfRef" cssNodeGetSelfRef;

let _ = Callback.register "CSSNodeInsertChild" cssNodeInsertChild;

let _ = Callback.register "CSSNodeGetChild" cssNodeGetChild;

let _ = Callback.register "CSSNodeStyleGetDirection" cssNodeStyleGetDirection;

let _ = Callback.register "CSSNodeStyleSetDirection" cssNodeStyleSetDirection;

let _ = Callback.register "CSSNodeIsDirty" cssNodeIsDirty;
