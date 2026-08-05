; functions.scm
;
; run: ./scheme examples/functions.scm

; a lambda bound to a name with define
(define square (lambda (x) (* x x)))
(square 9)

; multiple parameters
(define addup (lambda (a b c) (+ a b c)))
(addup 10 20 30)

; functions can call other functions
(define cube (lambda (x) (* x (square x))))
(cube 4)

(define sumofsquares (lambda (a b) (+ (square a) (square b))))
(sumofsquares 3 4)

; conditionals inside a function body
(define absval (lambda (x) (if (< x 0) (- 0 x) x)))
(absval 17)
(absval (- 0 17))

(define maxof (lambda (a b) (if (> a b) a b)))
(define minof (lambda (a b) (if (< a b) a b)))
(maxof 14 8)
(minof 14 8)

; functions can read global variables
(define scale 3)
(define scaleby (lambda (x) (* x scale)))
(scaleby 7)

; a parameter shadows a global of the same name
(define shadowed 100)
(define usesparam (lambda (shadowed) (* shadowed 2)))
(usesparam 5)
shadowed

; arguments can themselves be expressions
(square (+ 2 3))
(sumofsquares (minof 3 9) (maxof 3 9))