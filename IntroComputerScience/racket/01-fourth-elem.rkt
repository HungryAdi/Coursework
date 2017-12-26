;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 01-fourth-elem) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; Test cases

;;fourth-element
(check-expect (fourth-element (list 'a 'b 'c 'd 'e)) 'd)
(check-expect (fourth-element (list 'x (list 'y 'z) 'w 'h 'j)) 'h)
(check-expect (fourth-element (list (list 'a 'b) (list 'c 'd) (list 'e 'f) (list 'g 'h) (list 'i 'j))) (list 'g 'h))
(check-expect (fourth-element (list 'a 'b 'c)) empty)
(check-expect (fourth-element (list 'a)) empty)

(define (fourth-element list)
     (cond
       [(empty? (first list)) '()]
       [(empty? (rest list)) '()]
       [(empty? (rest (rest list))) '()]
       [(empty? (rest (rest (rest list)))) '()]
       [else (first (rest (rest (rest list))))]))                
                    