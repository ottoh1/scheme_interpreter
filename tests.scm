; arithmetic
(+ 1 2)
(- 5 3)
(* 2 3)
(/ 10 2)
(+ 1 2 3 4)
(* 2 3 4)
(+ (* 2 3) (- 4 1))
(/ (* 2 6) (+ 1 2))
(/ 1 2)
(/ 3 4)
(+ 1.5 2.5)
(- 5.5 2.5)
(* 2.5 2)
(/ 5.0 2)
(+ 1.1 2.2 3.3)

; comparisons
(= 1 1)
(= 1 2)
(= 3 3 3)
(= 1 1 2)
(= 1.5 1.5)
(< 1 2)
(< 2 1)
(< 1 2 3)
(< 1 3 2)
(< 1.5 2.5)
(> 2 1)
(> 1 2)
(> 2.5 1.5)
(<= 1 1)
(<= 1 2)
(<= 2 1)
(<= 1.5 1.5)
(>= 1 1)
(>= 2 1)
(>= 1 2)
(>= 2.5 2.5)

; boolean
(and #t #t)
(and #t #f)
(and #f #t)
(and #f #f)
(or #t #t)
(or #t #f)
(or #f #t)
(or #f #f)
(not #t)
(not #f)

; if
(if #t 1 2)
(if #f 1 2)
(if #t 99 0)

; nested
(if (= 1 1) 10 20)
(if (< 3 1) 10 20)
(if (and #t #t) 1 0)
(if (or #f #t) 1 0)
(if (= 1 1) (+ 1 2) 0)
(and (= 1 1) (< 1 2))
(or (= 1 2) (= 2 2))
(if (> 5 3) (* 2 3) (+ 1 1))
(if #t (+ 1 2) 0)
(if #f 0 (+ 1 2))
(if (= 1 1) (* 2 3) 0)
(if #t (if #t (+ 1 1) 0) 0)
(if (< 1 2) (+ (* 2 2) 1) 9)
(and (= 1 1) (= 2 2))

; define
(define x 5)
(define y 10)
(+ x y)
(define x 20)
(* x y)
(define a 1)
(define b 2)
(define c 3)
(+ a b c)
(- x a)
(if (= x 20) x y)
(and (= x 20) (= y 10))
(+ x x)
(* x x)
(< x y)
(> x y)
(define pi 3.14)
pi
(* pi 2)

; self-eval and variable lookup
5
3.14
#t
#f
(define z 7)
z

(define sq (lambda (x) (* x x)))
(sq 2)
(sq 5)
(sq 10)

(define add-both (lambda (x y) (+ x y)))
(add-both 3 4)
(add-both 10 20)

(define avg-three (lambda (a b c) (/ (+ a b c) 3)))
(avg-three 1 2 3)
(avg-three 10 20 30)

(define n 10)
(define add-n (lambda (x) (+ x n)))
(add-n 5)
(add-n 0)

(define w 100)
(define shadow-test (lambda (w) (* w 2)))
(shadow-test 3)
w

(define dbl (lambda (x) (* x 2)))
(define quad (lambda (x) (dbl (dbl x))))
(quad 5)
(quad 1)

(sq (+ 1 2))
(dbl (sq 3))

(define abs-val (lambda (x) (if (< x 0) (- 0 x) x)))
(abs-val 5)
(abs-val (- 0 7))

(define fact (lambda (m) (if (= m 0) 1 (* m (fact (- m 1))))))
(fact 0)
(fact 1)
(fact 5)

(define fib (lambda (k) (if (< k 2) k (+ (fib (- k 1)) (fib (- k 2))))))
(fib 0)
(fib 1)
(fib 6)
(fib 10)