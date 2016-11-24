type scalar = float;

let cssUndefined = nan;

let isUndefined (num: float) => num != num;

let zero = 0.0;

let divideScalarByInt (f: float) (i: int) => f /. float_of_int i;

let (-.) = (-.);

let (+.) = (+.);

let (/.) = (/.);

let ( *. ) = ( *. );

let (~-.) = (~-.);

let scalarToString = string_of_float;

let scalarToFloat (f: float) => f;
