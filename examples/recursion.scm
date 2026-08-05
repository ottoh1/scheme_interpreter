; recursion.scm
;
; run: ./scheme examples/recursion.scm

; factorial
(define fact
  (lambda (n)
    (if (= n 0)
        1
        (* n (fact (- n 1))))))

(fact 0)
(fact 5)
(fact 10)

; fibonacci — two recursive calls per level
(define fib
  (lambda (n)
    (if (< n 2)
        n
        (+ (fib (- n 1)) (fib (- n 2))))))

(fib 1)
(fib 10)
(fib 15)

; sum of 1..n
(define sumto
  (lambda (n)
    (if (= n 0)
        0
        (+ n (sumto (- n 1))))))

(sumto 10)
(sumto 100)

; exponentiation by repeated multiplication
(define power
  (lambda (base exponent)
    (if (= exponent 0)
        1
        (* base (power base (- exponent 1))))))

(power 2 10)
(power 3 4)

; an accumulator-style recursion: two values threaded through each call
(define facthelper
  (lambda (n acc)
    (if (= n 0)
        acc
        (facthelper (- n 1) (* n acc)))))

(facthelper 6 1)

; mutual recursion — two functions calling each other
(define iseven
  (lambda (n)
    (if (= n 0)
        #t
        (isodd (- n 1)))))

(define isodd
  (lambda (n)
    (if (= n 0)
        #f
        (iseven (- n 1)))))

(iseven 10)
(isodd 10)
(iseven 7)