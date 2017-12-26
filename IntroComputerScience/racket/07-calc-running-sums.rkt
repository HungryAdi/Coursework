;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 07-calc-running-sums) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
(check-expect (calc-running-sums (list 1)) (list 1))
(check-expect (calc-running-sums (list 2 2 2 2 2)) (list 2 4 6 8 10))
(check-expect (calc-running-sums (list 2 5 8)) (list 2 7 15))


(define (calc-running-sums list)
  (calc-sum2 '() 0 list))
    
(define (calc-sum2 new-list ssf list)
  (cond
    [(empty? list) new-list]
    [else (calc-sum2 (my-append new-list (+ ssf (first list))) (+ ssf (first list)) (rest list))]))

(define (my-append list1 item)
  (cond
    [(empty? list1) (list item)]
    [(empty? item) list1]
    [else (cons (first list1) (my-append (rest list1) item))]))  