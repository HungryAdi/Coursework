;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 05-is-increasing) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;;Test cases
(check-expect (is-increasing? empty) true)
(check-expect (is-increasing? (list 1)) true)
(check-expect (is-increasing? (list 1 2 3)) true)
(check-expect (is-increasing? (list 1 1 1 1 1)) true)
(check-expect (is-increasing? (list 1 2 3 2)) false)
(check-expect (is-increasing? (list 1 2 2 3 4 4 5)) true)


(define (is-increasing? list)
  (cond
    [(empty? list) true]
    [(empty? (rest list)) true]
    [(> (first list) (first (rest list))) false]
    [else (is-increasing? (rest list))]))
