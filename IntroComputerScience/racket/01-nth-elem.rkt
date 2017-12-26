;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 01-nth-elem) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;; Test cases

;;fourth-element
(check-expect (fourth-element (list 'a)) empty)
(check-expect (fourth-element (list 'a 'b 'c 'd 'e)) 'd)
(check-expect (fourth-element (list 'x (list 'y 'z) 'w 'h 'j)) 'h)
(check-expect (fourth-element (list (list 'a 'b) (list 'c 'd) (list 'e 'f) (list 'g 'h) (list 'i 'j))) (list 'g 'h))
(check-expect (fourth-element (list 'a 'b 'c)) empty)


(define (fourth-element list)
  (nth-element 4 list))


; generic nth-element
(check-expect (nth-element 4 (list 'a 'b 'c 'd 'e)) 'd)

(define (nth-element n list)
     (cond
       [(empty? list) '()]
       [(equal? n 0) '()]
       [(equal? n 1) (first list)]
       [else (nth-element (- n 1) (rest list))]))