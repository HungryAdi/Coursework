;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 09-calc-depth) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;Test cases;
;(check-expect (calc-depth (list 1 2 3)) 1)
;(check-expect (calc-depth (list 1 (list 2) 3))  2)
;(check-expect (calc-depth (list 1 (list 2 (list 3) 4) 5))  3)
(check-expect (calc-depth (list 1 (list 2) 3 (list 4 (list 5)))) (list 1 2 1 3))

(define (calc-depth list)
  (get-list-of-depths list))
   
   
(define (get-list-of-depths list)
  (calc-depth-2 list '()))

(check-expect (calc-depth-3 1 (list 4 (list 1))) 2)
(define (calc-depth-2 list appendto)
  (cond
    [(empty? list) appendto]
    [(not (empty? (first list))) (calc-depth-2 (rest list ) (my-append appendto (calc-depth-3 1 (first list))))]
    [else appendto]))
  
    
(check-expect (calc-depth-3 1 (list 4 (list 1))) 2)
(check-expect (calc-depth-3 1 (list (list 2))) 2)
(check-expect (calc-depth-3 1 (list (list 4 (list 5)))) 3)

(define (calc-depth-3 depth list)
  (cond
    [(empty? list) depth]
    [(not (list? list)) depth]
    [(not (list? (first list))) (calc-depth-3 (+ depth 0) (rest list))]
    [(list? (first list)) (calc-depth-3  (calc-depth-3 (+ depth 1) (first list)) (rest list))]
    [else (calc-depth-3 (+ depth 0) (first list))]))


;; Append an item to a list     
(define (my-append list1 item)
  (cond
    [(empty? list1) (list item)]
    [(empty? item) list1]
    [else (cons (first list1) (my-append (rest list1) item))]))

;; find the max of a list
;; following 2 functions
(define (find-max list)
  (find-max2 (first list) list))
    
(define (find-max2 max list)
  (cond
    [(empty? (rest list)) (greater max (first list))]
    [else (find-max2 (greater max (first list)) (rest list))]))
    
;; item1 is > item2 ?    
(define (greater item1 item2)
  (cond
    [(> item1 item2) item1]
    [else item2]))
