
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

Instead of manually writing a test which ensures parity with web
implementations of flexbox you can run `gentest/gentest.sh` to generated a test
for you. After running `gentest/gentest.sh` a editor window should pop open
(make sure you have `$EDITOR` env variable exported). Here you can write html
which you want to verify in CSSLayout, such as the following.

Then put something like this in the editor that pops up:

```html
<div style="width: 100px; height: 100px; align-items: center;">
  <div style="width: 50px; height: 50px;"></div>
</div>
```

Once saving and exiting the editor window the script will open a browser
window. From here open the developer console and you should see two buttons,
that will copy tests cases to your clipboard. Copy the fixed point test and
paste it into `./src/LayoutTestFixedEncoding.re`, and copy/paste the floating
point test into `./src/LayoutTestFloatEncoding.re`. Run `npm run build` and
`npm run test`.

The ASCII output paints a pseudo-accurate picture of any broken layouts.



<br />
<br />
<br />
<br />
<br />
<br />

See the [`css-layout`](https://github.com/facebook/css-layout)'s README/docs
for more information about the limitations and special defaults of `ReLayout`.
`Relayout` is a direct port of that project from `C` to `Reason` on `ocamlopt`.
