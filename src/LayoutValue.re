/* type scalar = float; */
/* let cssUndefined = nan; */
/* let isUndefined (num : float) => num != num; */
/* let zero = 0.0; */
/* let divideScalarByInt (f : float) (i : int) => f /. float_of_int i; */
/* let (-.) = (-.); */
/* let (+.) = (+.); */
/* let (/.) = (/.); */
/* let ( *. ) = ( *. ); */
/* let (~-.) = (~-.); */
/* let scalarToString = string_of_float; */
/* let scalarToFloat (f: float) => f; */

type scalar = int;
let cssUndefined = min_int;
let isUndefined num => num === cssUndefined;
let zero = 0;
let divideScalarByInt = (/);
let unitsPerPixel = 100.0;
let scalarToFloat (f: int) => float_of_int f /. unitsPerPixel;
let (-.) = (-);
let (+.) = (+);
let (/.) = (/);
let ( *. ) = ( * );
let (~-.) = (~-);
let scalarToString = string_of_int;
