;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 04-my-append) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; tests
(check-expect (my-append '() (list 'a 'b)) (list 'a 'b))
(check-expect  (my-append (list 'a 'b) '()) (list 'a 'b))
(check-expect (my-append (list 'a 'b 'c) (list 'd 'e)) (list 'a 'b 'c 'd 'e))

;;
(define (my-append list1 list2)
  (cond
    [(empty? list1) list2]
    [(empty? list2) list1]
    [else (cons (first list1) (my-append (rest list1) list2))]))