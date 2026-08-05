; double-investment.scm
;
; You invest $1000 at 7% annual interest, compounded yearly.
; How many years until the money has at least doubled?
;
; run: ./scheme examples/double-investment.scm

(define principal 1000)
(define rate 7)
(define target (* principal 2))

; grow a balance by rate percent, e.g. 1000 -> 1070
(define grow
  (lambda (balance)
    (+ balance (/ (* balance rate) 100))))

; compound year by year, counting until the target is reached
(define yearsuntil
  (lambda (balance count)
    (if (>= balance target)
        count
        (yearsuntil (grow balance) (+ count 1)))))

; answer
(yearsuntil principal 0)