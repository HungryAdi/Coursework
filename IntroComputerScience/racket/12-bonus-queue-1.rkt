;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname 12-bonus-queue) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;Test cases


; make-queue which takes no parameters and returns an empty queue (in this case, an empty list).
(define (make-queue)
  ('()))

;queue-enqueue, which takes a queue and a new element and returns a queue with the new element added to the back (in this case, added to the end of the list).
(define (queue-enqueue queue element)
  (my-append queue element))
  
  
;queue-dequeue, which takes a queue and returns a queue with the front element removed (in this case, the first list element is removed).
(define (queue-dequeue queue)
  (rest queue))


;queue-front, which takes a queue and returns the element at the front (in this case, the first list element).
(define (queue-front queue)
  (first queue))

;queue-empty?, which takes a queue and returns true if it's empty and false if not (in this case, we can check if the list is empty).
(define (queue-empty? queue)
  (empty? queue))

;; From previous problems     
(define (my-append list1 item)
  (cond
    [(empty? list1) (list item)]
    [(empty? item) list1]
    [else (cons (first list1) (my-append (rest list1) item))]))