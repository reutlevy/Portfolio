#lang racket

(provide (all-defined-out))

(define integers-from
  (lambda (n)
    (cons-lzl n (lambda () (integers-from (+ n 1))))))

(define sqrt (lambda (x init epsilon) (sqrt-iter x init epsilon)))

(define sqrt-iter
  (lambda (x guess epsilon)
    (if (good-enough? guess x epsilon)
        guess
        (sqrt-iter x (improve guess x) epsilon))))

(define abs (lambda (x) (if (< x 0) (- x) x)))
(define square (lambda (x) (* x x)))

(define good-enough?
  (lambda (guess x epsilon)
    (< (abs (- (square guess) x)) epsilon)))

(define average
  (lambda (x y) (/ (+ x y) 2)))

(define improve
  (lambda (guess x)
    (average guess (/ x guess))))

(define cons-lzl cons)
(define empty-lzl? empty?)
(define empty-lzl '())
(define head car)
(define tail
  (lambda (lzl)
    ((cdr lzl))))

(define take
  (lambda (lz-lst n)
    (if (or (= n 0) (empty-lzl? lz-lst))
        empty-lzl
        (cons (head lz-lst)
              (take (tail lz-lst) (- n 1))))))



;;; Q1


;;Signature: sqrt-lzl(x, init)
;;Purpose: Generate a lazy list of approximations (pairs of <guess, accuracy>) of the square root of the given number x, according to Newton method, starting from init guess.
;;Type: [Number * Number -> LzlList<Pair<Number,Number>>
;;Pre-condition: init =/= 0
;;Tests: (take (sqrt-lzl 2 1) 3) →  '((1 . 1) (3/2 . 1/4) (17/12 . 1/144)) 

(define sqrt-lzl
  (lambda (x init) ; init=guess
        (cons-lzl (cons init (abs (- (square init) x)))
                  (lambda () (sqrt-lzl x (improve init x))))))

;;Signature: find-first(lzlst, p)
;;Purpose: Return the first item in the given lazy list which satisfies the given predicate. If no such item exists return 'fail.
;;Type: [[LzlList<T> * T->Boolean] -> T | {'fail} ]
;;Pre-condition: /
;;Tests: (find-first (integers-from 1) (lambda (x) (> x 10))) --> 11; (find-first (cons-lzl 1 (lambda() (cons-lzl 2 (lambda () '())))) (lambda (x) (> x 10))) --> 'fail


(define find-first
  (lambda (lzlst p)
    (cond ((empty-lzl? lzlst) 'fail)
          ((p (head lzlst)) 
           (head lzlst))
          (else (find-first (tail lzlst) p)))))

;;Signature: sqrt2(x,init,epsilon)
;;Purpose: return approximation of the square root of the given number x, according to Newton method, starting from init guess with epsilon threshold.  The procedure uses sqrt-lzl and find-first procedures.
;;Type: [Number * Number * Number -> Number]
;;Pre-condition: init =/= 0
;;Tests: (sqrt2 2 1 0.0001) → 1 169/408
(define sqrt2 (lambda (x init epsilon) (sqrt-iter1 x init epsilon)))

(define sqrt-iter1
  (lambda (x guess epsilon)
    (car(find-first (sqrt-lzl x guess) (lambda (x) (> epsilon (cdr x)))))))


;;;; Q2

;;Signature: get-value(assoc-list, key)
;;Purpose: Find the value of 'key'. If 'key' is not found return �fail.
;;Type: [List<Pair(Symbol,T)>*Symbol -> T | 'fail)
;;Tests: (get-value '((a . 3) (b . 4)) 'b) --> 4,(get-value '((a . 3) (b . 4)) 'c) --> 'fail

(define get-value
  (lambda (assoc-list key)
    (if (empty? assoc-list)
        'fail
        (if (equal? (caar assoc-list) key)
            (cdar assoc-list)
            (get-value (cdr assoc-list) key)))))



;;Signature: get-value$(assoc-list, key, success, fail)
;;Purpose: Find the value of 'key'. If 'key' is found, then apply the continuation 'success' on its value val. Otherwise, apply the continuation 'fail'.
;;Type: [List<Pair<Symbol,T>>*Symbol*[T>->T1] * [Empty->T2]] -> T1 | T2)
;;Tests: > (get-value$ '((a . 3) (b . 4)) 'b (lambda(x) (* x x )) (lambda()#f)) --> 16, (get-value$ '((a . 3) (b . 4)) 'c (lambda(x) (* x x)) (lambda()#f)) --> #f


(define get-value$
  (lambda (assoc-list key success fail)
    (if (empty? assoc-list)
        (fail)
        (if (equal? (caar assoc-list) key)
            (success (cdar assoc-list))
            (get-value$ (cdr assoc-list) key success fail)))))
      

;;Signature: collect-all-values(list-assoc-lists, key)
;;Purpose: Returns a list of all values of the first occurrence of 'key' in each of the given association lists. If no such value, returns the empty list.
;;Type: [List<List<Pair<Symbol,T>>>*Symbol -> List<T>]
;;Tests: 
;;(define l1 '((a . 1) (e . 2)))
;;(define l2 '((e . 5) (f . 6)))
;;(collect-all-values (list l1 l2) 'e) --> '(2 5)
;;(collect-all-values (list l1 l2) 'k)--> '()

(define get-value1
  (lambda (assoc-list key)
    (if (empty? assoc-list)
        assoc-list
        (if (equal? (caar assoc-list) key)
            (cdar assoc-list)
            (get-value (cdr assoc-list) key)))))

(define is-value
  (lambda (assoc-list key)
    (if (empty? assoc-list)
        #t
        (if (equal? (caar assoc-list) key)
            #f
            (is-value (cdr assoc-list) key)))))

(define collect-all-values-1
  (lambda (assoc-list key)
    (if (empty? assoc-list)
        '()
        (if (is-value (car assoc-list) key)
            (collect-all-values-1 (cdr assoc-list) key)
            (cons (get-value (car assoc-list) key)
                  (collect-all-values-1 (cdr assoc-list) key))))))

(define collect-all-values-2
  (lambda (lists key)
    (if (empty? lists)
        '()
        (get-value$ (car lists) key
                    (lambda (x) (cons x (collect-all-values-2 (cdr lists) key)))
                      (lambda() '())))))

