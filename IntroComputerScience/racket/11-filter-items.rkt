;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 11-filter-items) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;Test cases
(check-expect (filter-items positive? (list 1 -3 2 -4 3 -5 4 -6))  (list 1 2 3 4))
(check-expect (filter-items odd? (list 1 2 3 4 5 6)) (list 1 3 5));
(check-expect (filter-items is-increasing? (list (list 1 4) (list 4 3 2) (list 5 6)))  (list (list 1 4) (list 5 6)))
(check-expect (filter-items (lambda (s)
                   (<= (string-length s) 4))
                  (list "Alex" "is" "happy" "today")) (list "Alex" "is"))


(define (filter-items FUNCTION list)
  (filter-items-2 FUNCTION list '()))
           
(define (filter-items-2 FUNCTION list result-list)
  (cond
    [(empty? list) result-list]
    [(FUNCTION (first list)) (filter-items-2 FUNCTION (rest list) (my-append result-list (first list)))]
    [else (filter-items-2 FUNCTION (rest list) result-list)]))
     
;; From previous problems     
(define (my-append list1 item)
  (cond
    [(empty? list1) (list item)]
    [(empty? item) list1]
    [else (cons (first list1) (my-append (rest list1) item))]))

(define (is-increasing? list)
  (cond
    [(empty? list) true]
    [(empty? (rest list)) true]
    [(> (first list) (first (rest list))) false]
    [else (is-increasing? (rest list))]))