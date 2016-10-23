open Ocamlbuild_plugin
let _ = flag ["ocaml"; "compile"; "native"] (A "-S" )
let _ = flag ["ocaml"; "compile"; "native"] (A "-inline" )
let _ = flag ["ocaml"; "compile"; "native"] (A "100" )
let _ = flag ["ocaml"; "compile"; "native"] (A "-g" )
let _ = flag ["ocaml"; "compile"; "native"] (A "-bin-annot" )
