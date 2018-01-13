
# ReLayout: FlexBox For Reason

[![Build Status](https://travis-ci.org/jordwalke/ReLayout.svg?branch=master)](https://travis-ci.org/jordwalke/ReLayout)


This is a **native** [`Reason`](https://github.com/facebook/reason)
implementation of CSS Flexbox layout. It is a port of the
[`Yoga`](https://github.com/facebook/Yoga) project.

It can also be compiled to JavaScript.

## Building and Installing

#### Install [`esy`](https://github.com/esy/esy)

Esy is "npm for native Reason". You install dependencies and make sure they're
prepared via the `esy install`, and `esy build` commands.


```sh
npm install -g esy
```


#### Clone, and Build
```
git clone git+ssh://git@github.com/jordwalke/ReLayout.git
cd ReLayout
esy install
esy build
```

> Installing may take several minutes if this is your first time doing native
> Reason development. Next time you create a project it will be fast.

## Rebuilding
Edit source and then just run:
```sh
esy build
```

## Running The Tests:

There are known failures where we are not compliant with browser's current
implementations - for some of those our implementation may be more compliant
with the W3C spec.


```sh
./test              # Run the test in native.
./test byte         # Run the tests in compiled bytecode.
./test jsc          # Run tests in JavaScriptCore.
./test jscWithJit   # Run tests in JavaScriptCore with JIT.
./test v8           # Run tests in v8.
```

The tests only run the layout tests (not the measurement test) so that we can
run the tests as a way to approximate startup time. (Otherwise JS engines would
be dually punished since they'd have to start up twice, whereas native targets
don't really have a "startup" process).

## Benchmarking:
While benchmarking, close any programs that are running so they do not
interfere with benchmarks (especially Chrome, or music/media apps).

After running `esy build`:

```sh
./bench              # Run the test in native.
./bench byte         # Run the tests in compiled bytecode.
./bench jsc          # Run tests in JavaScriptCore.
./bench jscWithJit   # Run tests in JavaScriptCore with JIT.
./bench v8           # Run tests in v8.
```

It will print the `mean` and `median` running time of the layout test runs.
The `median` is not reliably on any of the `JS` targets due to lack of
ubiquitous high precision timers in JS environments, so the `mean` is the only
universally comparable metric - still, the `median` is useful when comparing
against the `C` implementation of `Yoga`, and

## More Debuggable JavaScript

If you need to debug an issue in the JS behavior, generate more readable
JavaScript by explicitly asking for `--dev` versions of a JS file.

```sh
esy build jbuilder build --dev ./src/layout-test-fixed-encoding/LayoutTestFixedEncoding.bc.js
node ./src/layout-test-fixed-encoding/LayoutTestFixedEncoding.bc.js
```

After doing so, and before you run any benchmarks on the native/byte versions,
completely rm `_build` to make sure the `--dev` flag didn't cause deoptimized
native binaries.


## Crosscompiling to armv7 and x86
This was as simple as 3 steps, but it needs to be brought up to date and
retested. It generates a pure C API that any library can consume.

1. Install [docker](https://docs.docker.com/engine/installation)
2. Run `build_android.sh`
3. Artifacts will be generated in `_dist` directory

## Building and running cstub tests 
```sh
cd stub_test
make gtest
cd ../
npm run stubtest
```


## Some sample benchmark results (for those curious):

Your mileage may vary, but here's an example of the different performance you
can observe when compiling `Reason` layout to either native assembly, byte code,
and various JS engines. Comparison against a pure-`C` implementation is also
included.

This test uses the FixedPoint encoding of layout data. `native` currently
performs better on FixedPoint encoding (Float encoding will be improved
shortly - so that neither is significantly advantaged).

| Method       | Average time per test execution | Requires JIT |
| -------------|---------------------------------|---------------
| `c`          | `0.33ms`                        | No
| `native`     | `0.26ms`                        | No
| `byte`       | `5.0ms`                         | No
| `jsc`        | `18.6ms`                        | No
| `jscWithJit` | `4.05ms`                        | Yes
| `v8`         | `3.28ms`                        | Yes


> Update: Retesting on the newer compiler shows a greater difference between
> ocamlopt and ocamlc. I will investigate further once I have the C version
> rebuilt on the same machine.

## Comparison to `C` Implementation.

The native `C` implementation in the table above uses the official `Yoga`
implementation, but with a benchmark test suite that matches the `ReLayout`
benchmark tests. You can run the `C` benchmark yourself by cloning the official
`Yoga` project, checking out the revision at the time ReLayout was ported from
Yoga, and replacing the official benchmark with
[this](./benchmarks/CSSBenchmark.c). It's important to check out the revision
at the time this was ported to Reason because only then are the algorithms
identical (this cross-language benchmark attempts as much as possible to only
change one thing at a time - the language of implementation).


Here are several datapoints gathered from running the `ReLayout` test suite,
and comparing it with those same tests executed in the `C` implementation.  We
include both the fixed point and the floating point encodings.

The Reason implementation of layout hasn't been optimized at all. Cleaning up
the code to be less imperative will likely make it even faster. Upgrading to
F-lambda will also likely make both fixed point, and especially floating point,
faster.


```
    ┌────────────────────────────┬────────────────────────────┬────────────────────────────┐
    │    Reason Layout Float     |      C Implementation      |   ReasonLayout FixedPoint  |
    ├────────────────────────────┼────────────────────────────┼────────────────────────────┤
    │                            |                            |                            |
    │   ┌──────────┬─────────┐   |   ┌──────────┬─────────┐   |   ┌──────────┬─────────┐   |
    │   │ AverageMs│ MedianMs│   |   │ AverageMs│ MedianMs│   |   │ AverageMs│ MedianMs│   |
    │   ├──────────┼─────────┤   |   ├──────────┼─────────┤   |   ├──────────┼─────────┤   |
    │   │ 0.32469  │ 0.296   │   |   │ 0.39579  │ 0.350   │   |   │ 0.27288  │ 0.245   │   |
    │   │ 0.36458  │ 0.328   │   |   │ 0.33136  │ 0.299   │   |   │ 0.25957  │ 0.238   │   |
    │   │ 0.36345  │ 0.329   │   |   │ 0.33733  │ 0.325   │   |   │ 0.26478  │ 0.241   │   |
    │   │ 0.31851  │ 0.265   │   |   │ 0.31649  │ 0.289   │   |   │ 0.25594  │ 0.234   │   |
    │   │ 0.3742   │ 0.331   │   |   │ 0.29206  │ 0.281   │   |   │ 0.27518  │ 0.241   │   |
    │   │ 0.33161  │ 0.307   │   |   │ 0.34415  │ 0.319   │   |   │ 0.23987  │ 0.216   │   |
    │   │ 0.34044  │ 0.303   │   |   │ 0.32270  │ 0.282   │   |   │ 0.25471  │ 0.233   │   |
    │   │ 0.38866  │ 0.341   │   |   │ 0.35345  │ 0.332   │   |   │ 0.32457  │ 0.297   │   |  
    │   └──────────┴─────────┘   |   └──────────┴─────────┘   |   └──────────┴─────────┘   |
    │                            |                            |                            | 
    └────────────────────────────┴────────────────────────────┴────────────────────────────┘
```
    

## Interpreting the Data

This benchmark is very useful because it tests the core compiler and runtimes
of various language ecosystems - knowing that we are compiling a *single* exact
algorithm to different runtimes. In general the JS that is compiled here (by
`js_of_ocaml`), is somewhat idiomatic JS, and generally performs slightly
better than what you would write by hand in JS. In this case, because of how
the original code is written in `Reason` (which was ported from `C` and is not
very functional), many of `js_of_ocaml`'s optimization opportunities are lost,
so it's likely that this JS output is slightly slower than what you'd write by
hand (I'd estimate 20% max). We can easily fix that in the `Reason` source, and
not only will the `JS` output's performance benefit, but likely the
`native`/`byte` targets as well.

Still, regardless of what we do to improve the JS output, it's likely not going
to recover the order(s) of magnitude.

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




## Theories

In past benchmark experiments I've performed with `jsc`/`v8`/`ocamlopt`, I've
found that `jsc` with a JIT can be competitive with `ocamlopt`, under the right
circumstances, but those other experiments were very allocation heavy. The
layout algorithm is very computationally heavy, and not allocation heavy.  It
seems that `ocamlopt` does well in a wide variety of cases (elegant allocation
heavy function style, or dirty imperative systems work/computation).

## Startup Time

Different runtimes have different startup time characteristics. The following
benchmarks give a *very rough* idea of how long each of the respective language
runtimes take to startup. This isn't measuring how fast the *languages*
initialize their environments, but rather, the languages *along with* their
containing ecosystems (such as Node setup, and the JSC harness). `v8`'s startup
time is at an inherent disadvantage because it includes to initialize the VM
along with many built in libraries. Still, this is a good idea of what you
could expect your relative startup time overhead to be for a very small (1000
line) app in these respective environments.

One thing not accounted for here, is how the startup time *grows* (or doesn't)
with the amount of code added. JS engines must parse their code and generate
some intermediate representation at startup time, so that will cause large apps
to slow down during the startup phase. This is much less of an issue for
natively compiled code, where all compilation has been done ahead of time.


| Method       | Startup duration + running tests once | Requires JIT
| -------------|---------------------------------------|-------------
| `native`     | `7 ms`                                | No
| `byte`       | `18 ms`                               | No
| `jsc`        | `98 ms`                               | No
| `jscWithJit` | `150 ms`                              | Yes
| `v8`         | `220 ms`                              | Yes

Measurements use the `time` command line program (`real`).

To test startup time, change the number numIterations to `1` in
`LayoutTestFixedEncoding.re`, rerun `esy build`, then do `./bench` or `./bench
byte` etc.


## Multiple layout representations:

There are two implementations of layout data encodings, one uses fixed point
with explicit rounding, and the other uses floating point. The default is
currently fixed point. You can toggle between the two by doing the following:


- To test the floating point representation, open `./src/LayoutValue.re` and
  uncomment the upper half, and comment out the lower half.
- Open `package.json` and replace `src/LayoutTestFixedEncoding.re` with
  `src/LayoutTestFloatEncoding.re`.
- Run `npm run build`, `npm run test`, `npm run bench`, etc.


The performance in `ocamlopt` would decrease by about `20%` when switching to
floating point representation. That can be fixed in the floating point
representation without having to use fixed point representation, but I just
haven't gotten to that yet.

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

See the [`Yoga`](https://github.com/facebook/yoga)'s README/docs
for more information about the limitations and special defaults of `ReLayout`.
`Relayout` is a direct port of that project from `C` to `Reason` on `ocamlopt`.

#### Profiling on Mac:

This command will build the artifacts with the proper profiling/debug symbol
flags, and will change `YGAlignContentTest` to wrap one test case in a large
for loop.  It should be suitable for either mac or linux.

```
npm run stubtestbindingsperf
```

This command is useful for generating profiling traces on mac:

Warning: You currently must clean the build before switching between `npm run
stubtest` and `npm run stubtestbindingsperf`.


```sh
sudo instruments -v -t 'Time Profiler'  -D ~/Desktop/yourTrace.trace _build/test/test
# To open in Instruments, need to restore perms.
sudo chown -R you:staff ~/Desktop/yourTrace.trace
# Now open Instruments.app and open the trace.
```

Then open Instruments.app (GUI), and open file test.trace.
- Enable "High Frequency" and disable "Hide System Libraries" in the GUI to see
  all the content.
- Click on the gear, then click "Invert Call Tree" and "Top Functions".
- Right click on the table view header and enable `Self # Samples`  and `#
  Samples`. It's important to open these to get the full picture in the
  abscense of perfect profiling symbols.
- Click on "Self #Samples" to sort so that the highest numbers are first.


The result is that you have the bottlenecks at the top. When you drill down in
the tree, it tells you who's calling that bottleneck.  Many of the symbols are
still obfuscated (4.03 improves that).  Open up all the .s files that ocamlopt
dumped so that when you see obfuscated symbols in the Instruments GUI, you can
search for that symbol in all the .s files you have open. Double click on the
actual symbol in instruments and it will show you what the assembly looks like
and you can match it to the symbol you have open in your editor. There may be
multiple .L123 across all the files so you can cross reference Instruments
(double click) which shows you the assembly.  4.03 is said to greatly improve
the source locations when profiling so it tells you the function names instead
of assembly locations more often than not.
