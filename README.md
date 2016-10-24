
# ReLayout: Reason CSS Flexbox implementation.

[![Build Status](https://travis-ci.org/jordwalke/ReLayout.svg?branch=master)](https://travis-ci.org/jordwalke/ReLayout)


This is a [`Reason`](https://github.com/facebook/reason) implementation of CSS Flexbox layout.
It is a port of the [`css-layout`](https://github.com/facebook/css-layout)
project.


## Building and Installing
```sh
git clone git+ssh://git@github.com/jordwalke/ReLayout.git
cd ReLayout
npm install
```

> Note that installing may take a while (expect 20 minutes).

## Rebuilding
```sh
npm run build
```

## Running Tests
```sh
npm run test              # Run the test in native.
npm run test:byte         # Run the tests in compiled bytecode.
npm run test:jsc          # Run tests in JavaScriptCore.
npm run test:jscWithJit   # Run tests in JavaScriptCore with JIT.
npm run test:v8           # Run tests in v8.
```

## Running Benchmarks
While benchmarking, close any programs that are running so they do not
interfere with benchmarks (especially Chrome, or music/media apps).

```sh
npm run bench              # Run the benchmarks in native.
npm run bench:byte         # Run the benchmarks in compiled bytecode.
npm run bench:jsc          # Run benchmarks in JavaScriptCore.
npm run bench:jscWithJit   # Run benchmarks in JavaScriptCore with JIT.
npm run bench:v8           # Run benchmarks in v8.
```

## Some sample benchmark results (for those curious):

Your mileage may vary, but here's an example of the different performance you
can observe when compiling `Reason` layout to either native assembly, byte code,
and various JS engines.

| Method       | Average time per test execution |
| -------------|---------------------------------|
| `native`     | `0.20ms`                        |
| `byte`       | `5.0ms`                         |
| `jsc`        | `18.6ms`                        |
| `jscWithJit` | `3.05ms`                        |
| `v8`         | `1.28ms`                        |

One interesting fact is that `ReLayout` is compiles the `byte` version using
`ocamlc`, which produces a Virtual Machine byte code, but this VM is
interesting in that it does not use any runtime JIT to achieve decent
performance. If a JIT is out of the question for you (or you just don't want to
wait for JIT warmup at startup time), then `byte` is a good option for you. If
raw performance and startup time are important to you, then compiling to native
is the best option.

All of the JS benchmarks are compiled with `js_of_ocaml`, and `--opt 3`. You
can adjust the `jsoo` flags in `package.json`. `v8`'s performance is especially
impacted by `--opt 3` vs. `--opt 1`. Note that `v8` *only* has the option to
enable the JIT (for now).
There are two implementations of layout data encodings, one uses fixed point
with explicit rounding, and the other uses floating point. The default is
currently fixed point. You can toggle between the two by doing the following:

## Multiple layout representations:

- To test the floating point representation, open `./src/LayoutValue.re` and
  uncomment the upper half, and comment out the lower half.
- Open `package.json` and replace `src/LayoutTestFixedEncoding.re` with
  `src/LayoutTestFloatEncoding.re`.
- Run `npm run build`, `npm run test`, `npm run bench`, etc.

## More accurate benchmarks for native compilation

`ReLayout` depends on `Core_bench`, which allows much better isolation of
benchmarks, and ensures that various batch sizes are tested to attempt to
eliminate misleading measurements caused by convenient (or inconvenient) batch
sizes.

Since it uses native hooks, `Core_bench` won't work in `byte` or any of the JS modes,
to enable `Core_bench`:

- Inside of `src/LayoutTestFixedEncoding.re` and `src/LayoutTestFloatEncoding.re`:
- Comment out `include FakeCore;` below.
- Uncomment `open Core_bench.Std;`
- Delete the two targets in `package.json` (`byteTarget`, `jsTarget`)
- Run `npm run build`, then `npm run bench`

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
