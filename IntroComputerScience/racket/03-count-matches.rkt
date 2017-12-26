;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 03-count-matches) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; tests
(check-expect (count-matches 'f (list 'a 'b 'c 'd 'e 'f 'g)) 1)
(check-expect (count-matches 'b (list 'a 'b 'a 'b 'a 'b 'a)) 3)
(check-expect (count-matches 'x (list 'a 'b 'c)) 0)
(check-expect (count-matches 'x (list 'x 'b 'c)) 1)


;;
(define (count-matches char list)
  (cond
    [(empty? list) 0]
    [(equal? char (first list)) (+ 1 (count-matches char (rest list)))]
    [else (+ 0 (count-matches char (rest list)))]))