;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 13-find-max) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; Test cases

(check-expect (find-max (list -1 -2 -3 -5 0)) 0)


(define (find-max list)
  (find-max2 (first list) list))
    
(define (find-max2 max list)
  (cond
    [(empty? (rest list)) (greater max (first list))]
    [else (find-max2 (greater max (first list)) (rest list))]))
    
    
(define (greater item1 item2)
  (cond
    [(> item1 item2) item1]
    [else item2]))