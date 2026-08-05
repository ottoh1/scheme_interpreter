; basics.scm
;
; run: ./scheme examples/basics.scm

; arithmetic takes any number of arguments
(+ 1 2 3 4 5)
(* 2 3 4)
(- 100 30 20)
(/ 100 4)

; expressions nest arbitrarily
(+ (* 2 3) (- 10 4))
(* (+ 1 2) (+ 3 4))

; division produces floats when it needs to
(/ 7 2)
(/ 1 3)

; comparisons also take multiple arguments
(= 5 5 5)
(< 1 2 3 4)
(> 10 5)
(<= 3 3)
(>= 2 7)

; booleans
(and #t #t)
(or #f #t)
(not #f)

; conditionals choose one branch
(if (> 10 5) 100 200)
(if (= 1 2) 100 200)

; conditions can be any expression
(if (< (* 2 3) (+ 3 4)) 1 0)