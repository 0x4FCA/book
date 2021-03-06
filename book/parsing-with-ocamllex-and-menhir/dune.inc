(alias
 (name runtest)
 (deps
  (:x README.md)
  (package core)
  (package mdx)
  (package ppx_jane)
  (:y5 examples/parsing-test/test.ml)
  (:y4 examples/parsing/json.ml)
  (:y3 examples/parsing/manual_token_type.ml)
  (:y2 examples/parsing/parsed_example.ml)
  (:y1 examples/parsing/prog.mli)
  (:y0 examples/parsing/tokens.ml)
  (source_tree examples/parsing)
  (source_tree examples/parsing-test)
  prelude.ml)
 (locks ../../global-lock)
 (action
  (progn
   (run ocaml-mdx test --prelude=prelude.ml --direction=to-md %{x})
   (diff? %{x} %{x}.corrected)
   (diff? %{y5} %{y5}.corrected)
   (diff? %{y4} %{y4}.corrected)
   (diff? %{y3} %{y3}.corrected)
   (diff? %{y2} %{y2}.corrected)
   (diff? %{y1} %{y1}.corrected)
   (diff? %{y0} %{y0}.corrected))))
