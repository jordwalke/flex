open LayoutSupport;

open LayoutTypes;

/* Force allocating a new node */
let cssNodeNew () => {
  ...theNullNode,
  children: [||],
  layout: createLayout (),
  style: createStyle (),
  context: ()
};

let cssNodeInsertChild node child i => {
  if (Array.length node.children <= i) {
    let fill = Array.make (i - Array.length node.children + 1) theNullNode;
    node.children = Array.append node.children fill
  };
  node.children.(i) = child
};

let cssNodeStyleSetDirection node direction => node.style.direction = direction;

let cssNodeStyleGetDirection node => node.style.direction;

let cssNodeIsDirty node => node.isDirty node.context;

let _ = Callback.register "CSSNodeNew" cssNodeNew;

let _ = Callback.register "CSSNodeInsertChild" cssNodeInsertChild;

let _ = Callback.register "CSSNodeStyleGetDirection" cssNodeStyleGetDirection;

let _ = Callback.register "CSSNodeStyleSetDirection" cssNodeStyleSetDirection;

let _ = Callback.register "CSSNodeIsDirty" cssNodeIsDirty;
