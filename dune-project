(lang dune 2.0)
(generate_opam_files true)

(package
 (name flex)
 (synopsis "Implementation CSS layout using pure Reason - powered by css-layout project")
 (authors "Jordan Walke <jordojw@gmail.com>")
 (maintainers "Jordan Walke <jordojw@gmail.com>")
 (homepage "https://github.com/jordwalke/flex")
 (bug_reports "https://github.com/jordwalke/flex/issues")
 (depends
  (ocaml (and (>= 4.06.1) (< 4.10.0)))
  (reason (and (>= 3.0.0) (< 4.0.0)))
 )
)
