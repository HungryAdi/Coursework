;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 10-deep-reverse) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;Test cases
(check-expect  (deep-reverse (list 'a 'b 'c))  (list 'c 'b 'a))
(check-expect  (deep-reverse (list (list 'a 'b 'c) 'd (list (list 'e 'f) 'g))) (list (list 'g (list 'f 'e)) 'd (list 'c 'b 'a)))



(define (deep-reverse list)
  list)
