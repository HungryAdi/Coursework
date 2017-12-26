;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 06-remove-dups) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
(check-expect (remove-duplicates (list 1 2 3)) (list 1 2 3))
(check-expect (remove-duplicates (list 1 2 1 4)) (list 1 2 4))
(check-expect (remove-duplicates (list 3 3 3 3 3)) (list 3))


(define (remove-duplicates list)
  (add-to-list-if-not-present '() list))


(define (add-to-list-if-not-present new-list list)   
  (cond
    [(empty? list) new-list]
    [(exists-in-list new-list (first list)) (add-to-list-if-not-present new-list (rest list))]
    [else (add-to-list-if-not-present (my-append new-list (first list)) (rest list))]))
    

(define (exists-in-list list item)
  (cond
    [(empty? list) false]
    [(equal? (first list) item) true]
    [else (exists-in-list (rest list) item)]))


(define (my-append list1 item)
  (cond
    [(empty? list1) (list item)]
    [(empty? item) list1]
    [else (cons (first list1) (my-append (rest list1) item))]))  