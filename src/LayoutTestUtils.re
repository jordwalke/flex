open LayoutSupport;

let round num => int_of_float (floor (num +. 0.5));

open LayoutValue;

let exceptions = {contents: []};

let failures = {contents: []};

let testCount = {contents: 0};

let assertionCount = {contents: 0};


/**
 * Pass an argument ---testNameFilter to only run tests matching
 * ---testNameFilter You can pass multiple ---arg filters. The tripple ---
 * makes sure the args are separated from core_bench args (which begin with a
 * single -) and the runMode flag `--bench` which begins with two dashes, and
 * is used to toggle between benchmark mode and test mode.
 */
let isFilterArg arg => String.length arg > 3 && String.sub arg 0 3 == "---";

let getSearchStr arg => String.sub arg 3 (String.length arg - 3);

let testNameFilter =
  Array.length Sys.argv > 1 ?
    List.find_all isFilterArg (Array.to_list Sys.argv) |> List.map getSearchStr : [];

type runMode =
  | Bench
  | Test;

let hasAnArgMatching tester =>
  Array.length Sys.argv > 1 && List.length (List.find_all tester (Array.to_list Sys.argv)) > 0;


/**
 * Specifying `-quota` turns the application into a benchmarking suite instead
 * of a test. Supplying `-v` will run the `-bench` in verbose mode, and will
 * also cause the tests to be broken down by individual test case.
 */
let runMode = hasAnArgMatching (fun arg => String.compare arg "-quota" === 0) ? Bench : Test;

let shouldBenchmarkAllAsOne = not (hasAnArgMatching (fun arg => String.compare arg "-v" === 0));

let currentTestName = {contents: ""};

let doTest desc test => {
  currentTestName.contents = desc;
  testCount.contents = testCount.contents + 1;
  try (test ()) {
  | e => exceptions.contents = [(desc, e), ...exceptions.contents]
  }
};

let rec shouldRun withTestFilter::filter=testNameFilter name =>
  switch filter {
  /* No filter at all means run everything.  */
  | [] => true
  /* If there's only one item, it had better match. */
  | [hd] => String.compare name hd === 0
  | [hd, hdHd, ...tl] => String.compare name hd === 0 || shouldRun withTestFilter::[hdHd, ...tl] name
  };

let rec it withTestFilter::filter=testNameFilter desc test =>
  if (shouldRun desc) {
    doTest desc test
  };

let displayOutcomes () => {
  let forEachException (desc: string, exc) => {
    print_string ("[Exception] " ^ desc);
    print_newline ()
  };
  let forEachFailure (desc: string, msg) => {
    print_string ("[TestFailure] " ^ desc ^ " - " ^ msg);
    print_newline ()
  };
  exceptions.contents |> List.iter forEachException;
  failures.contents |> List.iter forEachFailure;
  let exceptionsLength = List.length exceptions.contents;
  let failuresLength = List.length failures.contents;
  if (exceptionsLength === 0 && failuresLength === 0) {
    print_string ("[SUCCESS] " ^ string_of_int testCount.contents ^ " tests passed");
    print_newline ()
  } else {
    print_newline ();
    print_string "ð\159\146\128  ð\159\146\128  ð\159\146\128  ð\159\146\128";
    print_newline ();
    print_string (
      "[NON-SUCCESS] (" ^
      string_of_int exceptionsLength ^
      "/" ^
      string_of_int testCount.contents ^
      ") tests threw exceptions and (" ^
      string_of_int failuresLength ^ "/" ^ string_of_int assertionCount.contents ^ ") assertions failed."
    );
    print_newline ();
    print_string "ð\159\146\128  ð\159\146\128  ð\159\146\128  ð\159\146\128";
    print_newline ()
  };
  print_newline ();
  if (List.length exceptions.contents > 0) {
    let (tlDesc, tlExc) = List.hd (List.rev exceptions.contents);
    raise_notrace tlExc
  };
  testCount.contents = 0;
  assertionCount.contents = 0;
  exceptions.contents = [];
  failures.contents = []
};

let assertEq testNum layoutAttr a b => {
  assertionCount.contents = assertionCount.contents + 1;
  if (a != b) {
    failures.contents = [
      (
        currentTestName.contents,
        "Test " ^
        string_of_int testNum ^
        ": " ^
        layoutAttr ^ " not equal(expected=" ^ string_of_float a ^ ", observed=" ^ string_of_float b ^ ")"
      ),
      ...failures.contents
    ]
  }
};

let rec hasMismatchedLayout observedAndExpected =>
  switch observedAndExpected {
  | [] => false
  | [(expected, observed), ...tl] =>
    expected.LayoutTypes.top != observed.LayoutTypes.top ||
    expected.left != observed.left ||
    expected.width != observed.width || expected.height != observed.height || hasMismatchedLayout tl
  };

let rec best cmp extract hd tl =>
  switch tl {
  | [] => extract hd
  | [tlHd, ...tlTl] =>
    let hdVal = extract hd;
    let minFromTail = best cmp extract tlHd tlTl;
    cmp minFromTail hdVal ? minFromTail : hdVal
  };

let renderBox matrix minLeft minTop layout ch => {
  let leftIndex = int_of_float (scalarToFloat (layout.LayoutTypes.left -. minLeft));
  let topIndex = int_of_float (scalarToFloat (layout.top -. minTop));
  let rightIndex = round (scalarToFloat (layout.left +. layout.width -. minLeft));
  let bottomIndex = round (scalarToFloat (layout.top +. layout.height -. minTop));
  for y in topIndex to bottomIndex {
    for x in leftIndex to rightIndex {
      if (x === leftIndex || x === rightIndex || y === topIndex || y === bottomIndex) {
        matrix.(x).(y) = ch
      }
    }
  }
};

let renderDiagram (containerLayout: LayoutTypes.cssLayout) childLayouts containerChar childChar => {
  let minLeft = best (<) (fun layout => layout.LayoutTypes.left) containerLayout childLayouts;
  let minTop = best (<) (fun layout => layout.LayoutTypes.top) containerLayout childLayouts;
  let maxRight =
    best
      (>) (fun layout => layout.LayoutTypes.left +. layout.LayoutTypes.width) containerLayout childLayouts;
  let maxBottom =
    best
      (>) (fun layout => layout.LayoutTypes.top +. layout.LayoutTypes.height) containerLayout childLayouts;

  /**
   * Add + 1 so that we can render zero width as one wide.
   */
  let numCols = round (scalarToFloat (maxRight -. minLeft)) + 1;
  let numRows = round (scalarToFloat (maxBottom -. minTop)) + 1;
  let matrix = Array.make_matrix numCols numRows '.';
  renderBox matrix minLeft minTop containerLayout containerChar;
  List.iter (fun childLayout => renderBox matrix minLeft minTop childLayout childChar) childLayouts;
  let ret = {contents: ""};
  for y in 0 to (numRows - 1) {
    for x in 0 to (numCols - 1) {
      let ch = matrix.(x).(y);
      ret.contents = ret.contents ^ String.make 1 ch
    };
    ret.contents = ret.contents ^ "\n"
  };
  ret.contents
};

let mismatchText (expectedContainerLayout, observedContainerLayout) childExpectedAndObserved => {
  let childStr (expected, observed) =>
    "  Child[expected(e)]:" ^
    LayoutPrint.layoutStr expected ^ "\n  Child[observed(o)]:" ^ LayoutPrint.layoutStr observed;
  let containerExpected = "Container[expected(E)]:" ^ LayoutPrint.layoutStr expectedContainerLayout ^ "\n";
  let containerObserved = "Container[observed(O)]:" ^ LayoutPrint.layoutStr observedContainerLayout ^ "\n";
  containerExpected ^ containerObserved ^ String.concat "\n" (List.map childStr childExpectedAndObserved)
};

let assertLayouts testNum (expectedContainerLayout, observedContainerLayout) childExpectedAndObserved => {
  assertionCount.contents = assertionCount.contents + 1;
  if (hasMismatchedLayout [(expectedContainerLayout, observedContainerLayout), ...childExpectedAndObserved]) {
    let text = mismatchText (expectedContainerLayout, observedContainerLayout) childExpectedAndObserved;
    let expectedDiagram =
      renderDiagram expectedContainerLayout (List.map fst childExpectedAndObserved) 'E' 'e';
    let observedDiagram =
      renderDiagram observedContainerLayout (List.map snd childExpectedAndObserved) 'O' 'o';
    let title = "Test " ^ string_of_int testNum ^ ":\n";
    let expected = "\nEXPECTED\n========\n" ^ expectedDiagram;
    let observed = "\nOBSERVED\n========\n" ^ observedDiagram;
    failures.contents = [
      (currentTestName.contents, title ^ text ^ expected ^ observed),
      ...failures.contents
    ]
  }
};
