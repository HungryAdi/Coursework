;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 08-recursive-sum) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;Test cases
(check-expect (recursive-sum empty) 0)
(check-expect (recursive-sum (list 1 2 3)) 6)
(check-expect (recursive-sum (list 1 1 1 1)) 4)
(check-expect (recursive-sum (list 1 (list 2 (list 3)) 4)) 10)


(define (recursive-sum list)
  (recursive-sum-ssf 0 list))


(define (recursive-sum-ssf ssf list)
  (cond
    [(empty? list) ssf]
    [(list? (first list))  (recursive-sum-ssf (recursive-sum-ssf ssf (first list)) (rest list))]
    [else (recursive-sum-ssf (+ ssf (first list)) (rest list))]))
