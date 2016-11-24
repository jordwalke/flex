/**
 * Portion of the typical `Spec` that is ignored and instead hard coded to the
 * structure items below, so that we can ensure that inlining occurs. Once we
 * upgrade to `4.03+`, this hard coded module that the implementation relies
 * on can be removed, and we can rely on the module factories to produce code
 * as performant as monomorphic.
 */
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
/* Comment out the above, and uncomment out below to enable floating point */
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
