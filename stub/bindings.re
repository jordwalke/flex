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

let cssNodeStyleSetDirection node direction => node.style.direction = direction;

let cssNodeStyleGetDirection node => node.style.direction;

let cssNodeIsDirty node => node.isDirty node.context;

let cssNodeChildCount node => node.childrenCount;

let cssNodeGetChild node i => node.children.(i).selfRef;

let _ = Callback.register "CSSNodeNew" cssNodeNew;

let _ = Callback.register "CSSNodeGetSelfRef" cssNodeGetSelfRef;

let _ = Callback.register "CSSNodeInsertChild" cssNodeInsertChild;

let _ = Callback.register "CSSNodeRemoveChild" cssNodeRemoveChild;

let _ = Callback.register "CSSNodeChildCount" cssNodeChildCount;

let _ = Callback.register "CSSNodeGetChild" cssNodeGetChild;

let _ = Callback.register "CSSNodeStyleGetDirection" cssNodeStyleGetDirection;

let _ = Callback.register "CSSNodeStyleSetDirection" cssNodeStyleSetDirection;

let _ = Callback.register "CSSNodeIsDirty" cssNodeIsDirty;
