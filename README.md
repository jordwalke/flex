
# ReLayout: Reason CSS Flexbox implementation.

[![Build Status](https://travis-ci.org/jordwalke/ReLayout.svg?branch=master)](https://travis-ci.org/jordwalke/ReLayout)


This is a [`Reason`](https://github.com/facebook/reason) implementation of CSS Flexbox layout.
It is a port of the [`css-layout`](https://github.com/facebook/css-layout)
project.


## Building and Installing
```sh
git clone git+ssh://git@github.com/jordwalke/css-layout.git
cd css-layout/src/re-layout
npm install
```

## Rebuilding
```sh
npm run build
```

## Running Tests
```sh
npm run test
```

## Running Benchmarks
```sh
npm run bench
```

## Testing and running the Floating Point Representation:

There are two implementations of layout data encodings, one uses fixed point
with explicit rounding, and the other uses floating point. The default is
currently fixed point. You can toggle between the two by doing the following:

- To test the floating point representation, open `./src/LayoutValue.re` and
  uncomment out the commented lines, and comment out the uncommented lines.
- Open `package.json` and replace `src/LayoutTestFixedEncoding.re` with
  `src/LayoutTestFloatEncoding.re`.
- Run `npm run build`, `npm run test`, `npm run bench`.

### Adding Tests
For any changes you make you should ensure that all the tests are passing. In case you make any fixes or additions to the library please also add at least one test to ensure we don't break anything in the future. Tests are located in `tests/CSSLayoutTest.cpp`. Run the tests by executing `buck test //:CSSLayout`.

Instead of manually writing a test which ensures parity with web
implementations of flexbox you can run `gentest/gentest.sh` to generated a test
for you. After running `gentest/gentest.sh` a editor window should pop open
(make sure you have `$EDITOR` env variable exported). Here you can write html
which you want to verify in CSSLayout, such as the following. It will then open
a browser window, and provide two buttons for you to click which will copy
`Reason` test cases to the clipboard.


```
cd src/re-layout/
./gentest/gentest.sh
```

Then put something like this in the editor that pops up:

```html
<div style="width: 100px; height: 100px; align-items: center;">
  <div style="width: 50px; height: 50px;"></div>
</div>
```

Once saving and exiting the editor window the script will open a browser
window. From here open the developer console and you should see that the web
page has output a test file. Copy this into a file and save it in the
`LayoutTest.re` file.  From within `re-layout`, `npm run postinstall` will
recompile the tests and `npm run start` will run them.

The ASCII output paints a pseudo-accurate picture of the broken layout.



## Fixing Things:

[Here](https://github.com/jordwalke/css-layout/blob/master/src/Layout.js) is
the JS version of the layout algorithm this was ported from. Something may have
broken in the port/translation and that file can be consulted.

It's also based on an older pull request before the css-layout library was
rewritten in C, and there might be a good handful of fixes we should manually
recreate here in the `Reason` port.

If there's any discrepancy [this pull
request](https://github.com/facebook/css-layout/pull/185) (and the subsequent
commits after it was accepted) should be considered the authority, not the
`Layout.js` file included in this fork.

At the time of writing, 35 of the 104 test cases are failing. This is because
this `Reason` port was created from an older JS based snapshot of `css-layout`,
but the test cases are based on the `master` branch which added a bunch of more
features. The failing test cases likely represent features that we need to add
here in order to play "catch up" with `master` `css-layout`.


## Generating the source files

The instructions above merely build the source files, some of which were
auto-generated.  Auto-generating involves running `grunt transpile` in
`../../`, but you must have the special branch of `Reason` and `rejs` build in
directories on your `~/Desktop` (yes, your desktop) for now. Then you will
have to manually fix a bunch of type errors. Hopefully we won't need to
regenerate the files ever again and can manually begin taking over the
`Layout.re` file by hand.



Below is the original README:


css-layout [![Build Status](https://travis-ci.org/facebook/css-layout.svg?branch=master)](https://travis-ci.org/facebook/css-layout)
==========

This project implements a subset of CSS including flexbox and the box model using pure JavaScript, then transpiled to C, Java and C#. The goal is to have a small standalone library to layout elements. It doesn't rely on the DOM at all.

The Java, C and JavaScript version of the code is available via [npm](https://www.npmjs.com/package/css-layout) or directly from the `dist` folder of this repo. The JavaScript version is also available via [cdnjs](https://cdnjs.com/libraries/css-layout).

In order to make sure that the code is correct, it is developed in JavaScript using TDD where each commit adds a unit test and the associated code to make it work. All the unit tests are tested against Chrome's implementation of CSS.

The JavaScript version has been implemented in a way that can be easily transpiled to C and Java via regexes. The layout function doesn't do any allocation nor uses any of the dynamic aspects of JavaScript. The tests are also transpiled to make sure that the implementations are correct everywhere.


Usage
-----

A single function `computeLayout` is exposed that
 - takes a tree of nodes: `{ style: { ... }, children: [ nodes ] }`
 - computes the layout and writes it back to the node tree.

For example,

```javascript
// create an initial tree of nodes
var nodeTree = {
    "style": {
      "padding": 50
    },
    "children": [
      {
        "style": {
          "padding": 10,
          "alignSelf": "stretch"
        }
      }
    ]
  };

// compute the layout
computeLayout(nodeTree);

// the layout information is written back to the node tree, with
// each node now having a layout property: 

// JSON.stringify(nodeTree, null, 2);
{
  "style": {
    "padding": 50
  },
  "children": [
    {
      "style": {
        "padding": 10,
        "alignSelf": "stretch"
      },
      "layout": {
        "width": 20,
        "height": 20,
        "top": 50,
        "left": 50,
        "right": 50,
        "bottom": 50,
        "direction": "ltr"
      },
      "children": [],
      "lineIndex": 0
    }
  ],
  "layout": {
    "width": 120,
    "height": 120,
    "top": 0,
    "left": 0,
    "right": 0,
    "bottom": 0,
    "direction": "ltr"
  }
}
```

Supported Attributes
--------------------

Name | Value
----:|------
width, height | positive number
minWidth, minHeight | positive number
maxWidth, maxHeight | positive number
left, right, top, bottom | number
margin, marginLeft, marginRight, marginTop, marginBottom | number
padding, paddingLeft, paddingRight, paddingTop, paddingBottom | positive number
borderWidth, borderLeftWidth, borderRightWidth, borderTopWidth, borderBottomWidth | positive number
flexDirection | 'column', 'row'
justifyContent | 'flex-start', 'center', 'flex-end', 'space-between', 'space-around'
alignItems, alignSelf | 'flex-start', 'center', 'flex-end', 'stretch'
flex | number
flexWrap | 'wrap', 'nowrap'
position | 'relative', 'absolute'
overflow | 'visible', 'hidden'

- Rather than allowing arbitrary combinations of `flex-grow`, `flex-shrink`, and `flex-basis` the implementation only supports a few common combinations expressed as a single number using the `flex` attribute:

  css-layout `flex` value | W3C `flex` short-hand equivalent
  ---|---
  n (where n > 0) | n 0 0
  0 | 0 0 auto
  -1 | 0 1 auto
  
- `inherit` value is not implemented because it's a way to disambiguate between multiple colliding rules. This should be done in a pre-processing step, not in the actual layout algorithm.



Default values
--------------
Since we are only using flexbox, we can use defaults that are much more sensible. This is the configuration to use in order to get the same behavior using the DOM and CSS. You can try those default settings with the [following JSFiddle](http://jsfiddle.net/vjeux/y11txxv9/).

```css
div, span {
  box-sizing: border-box;
  position: relative;

  display: flex;
  flex-direction: column;
  align-items: stretch;
  flex-shrink: 0;
  align-content: flex-start;

  border: 0 solid black;
  margin: 0;
  padding: 0;
  min-width: 0;
}
```

- `box-sizing: border-box` is the most convenient way to express the relation between `width` and `borderWidth`.
- Everything is `display: flex` by default. All the behaviors of `block` and `inline-block` can be expressed in term of `flex` but not the opposite.
- All the flex elements are oriented from top to bottom, left to right and do not shrink. This is how things are laid out using the default CSS settings and what you'd expect.
- Everything is `position: relative`. This makes `position: absolute` target the direct parent and not some parent which is either `relative` or `absolute`. If you want to position an element relative to something else, you should move it in the DOM instead of relying of CSS. It also makes `top, left, right, bottom` do something when not specifying `position: absolute`.

Native Usage Notes
------------------

The C equivalent of `computeLayout` is [`layoutNode`](dist/css-layout.h#L1378).

In order for layout to properly layout reflowable text, the `measure` function must be set on the `css_node` structure. The property can be found in [`css-layout.h`](dist/css-layout.h#L146). This function must take a void pointer to a `context` that will affect the size of the node and the `width` as computed by the layout engine, and must return a `css_dim_t` structure defining the actual needed size of the node. For the most part, the `context` field can be the text inside the node. No C implementation of this function is provided in provided - it depends on your use of css-layout. However an implementation of the function in JavaScript can be used for reference in the [test utilities](src/Layout-test-utils.js#L383).

Development
-----------

The core logic resides with `Layout.js`, which is transpiled into equivalent C and Java implementations.

The JavaScript build process is managed via Grunt. The build performs linting, runs the tests against Chrome, transpiles and packages the code (JavaScript and Java) into the `dist` folder. For JavaScript, the build output uses the Universal Module Format (UMD) so that it can be used via AMD / RequireJS, CommonJS or included directly into an HTML page.

While developing you can just run the lint / Chrome-based tests a follows:

```
grunt test-javascript
```
