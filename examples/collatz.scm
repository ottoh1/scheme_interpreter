; collatz.scm
;
; If n is even, halve it; if odd, do 3n + 1.
; Repeat until you reach 1.
;
; run: ./scheme examples/collatz.scm

; is n even? subtract two at a time until we land on 0 or 1
(define iseven
  (lambda (n)
    (if (= n 0)
        #t
        (if (= n 1)
            #f
            (iseven (- n 2))))))

; integer halving: count how many 2s fit into n
(define halve
  (lambda (n acc)
    (if (= n 0)
        acc
        (halve (- n 2) (+ acc 1)))))

; one Collatz step
(define step
  (lambda (n)
    (if (iseven n)
        (halve n 0)
        (+ (* 3 n) 1))))

; count steps until we reach 1
(define collatzlength
  (lambda (n count)
    (if (= n 1)
        count
        (collatzlength (step n) (+ count 1)))))

; a few sanity checks on the pieces
(iseven 10)
(iseven 7)
(halve 10 0)
(step 10)
(step 7)

; THE ANSWERS: how many steps each starting number takes
(collatzlength 6 0)
(collatzlength 7 0)
(collatzlength 27 0)