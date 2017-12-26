;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 02-list-length) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))

(check-expect (list-length (list 'a 'b 'c)) 3)
(check-expect (list-length (list 'a (list 'b 'c) 'd 'e 'f)) 5)
(check-expect (list-length '()) 0)


(define (list-length list)
  (cond
    [(empty? list) 0]
    [(empty? (rest list)) 1]
    [else (+ (list-length (rest list)) 1)]))