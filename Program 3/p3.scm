;this is the prototype for p3.scm (cs320 Program 3, Spring 2017)
;EDIT the next few comments to add header info (name, instructor, purpose, etc.)
;CS320 - Project 3
;Student: Tom Nguyen
;Professor: Dr. John Carroll
;username: CSSC0155@edoras.sdsu.edu
;Aprile 14, 2017
;File: p3.scm
;
; A good website to use for reference: https://docs.racket-lang.org/reference/pairs.html#%28def._%28%28quote._~23~25kernel%29._pair~3f%29%29
;apart from adding the above header comments, leave this file section UNCHANGED
;To run this file, you would type (from the '%' prompt):
; scheme --load p3.scm
;

(load "~cs320/Scheme/simply.scm")
;Here I define a few handy lists that we can manipulate:
(DEFINE list1 (LIST 'a 'b 'c 'd 'e 'f 'g))
(DEFINE list2 (LIST 's 't 'u 'v 'w 'x 'y 'z))
(DEFINE list3 '((a b) c (d e f) g (h i)) )
(DEFINE list4 (LIST 'n 'o 'p 'q 'q 'p 'o 'n))
(DEFINE list5 '((a b) c (d (e f)) g (h i)) )
(DEFINE list6 '((h i) (j k) l (m n o)))
;here is a typical function definition (from Page 681 of Sebesta):
(DEFINE (adder lis)
  (COND
    ((NULL? lis) 0)
    (ELSE (+ (CAR lis) (adder (CDR lis))))
))
;the above five lines are the sort of definition you would need to add to
;this file if you were asked to:
;"Create a function that accepts a single list as a parameter and returns
;the sum of the [numerical] atoms in the list.  To do this, uncomment the
;next DEFINE and modify it to conform to the specs."
; (DEFINE (adder ...complete this definition
; 
; 'adder' has already been defined for you (see above), but you must create
; the following three definitions [see ~cs320/program3 for full details;
; make four definitions if you want all the extra credit]
; So, uncomment the next four DEFINEs, and modify them to conform to the
; specs given in the ~cs320/program3 writeup.

; (DEFINE (thirds  ...complete this definition, as described in ~cs320/program3
; This will take the first element in the list and do a recursive called drop, dropping the
; first three elements from the list. It will repeat this until it reaches null.
; unexpected behavior: if it is in capitial letters it will print out as lower case.
; unexpected behavior: if used (third (a)) or (third a) will cause an error 
; bypassing the usage message.
; Got the code to handle mismatch arguments online at stack overflow by user Barmar
; The three here is a constant but it does not have to be. I could have made a recursive that
; drops two at a time and when it reaches null return null but i wanted to play with the idea
; of using n in a recursive.
(DEFINE (thirds . args)
	(if (= (length args) 1)
		(let ((a_lis1 (car args)))
	(COND
		((NOT (LIST? a_lis1)) (DISPLAY "USAGE: (thirds [list])\nTry again."))
		((NULL? a_lis1) '())
		(ELSE (CONS (CAR a_lis1) (thirds (drop a_lis1 3))))
	))
	(display "USAGE: (thirds [list])\nTry again."))
)
; drop recursive will take in a (n)umber that represents the amount of number to be dropped
; from the front of the list and return it.
(DEFINE (drop b_lis1 n)
	(COND
		((NULL? b_lis1) '())
		((ZERO? N) CDDDR b_lis1)
		(ELSE (drop (CDR b_lis1) (- n 1)))
		)
	)
; (DEFINE (swap ...complete this definition, as described in ~cs320/program3
; Take two elements at a time in a list and switches it. For example (a b) will be (b a)
; switching position1 to position2.
; drop the first letter and return the next element only.
; if the list is empty return nothing.
; if the list has only one element return the list.
; example used: (CONS 'apple (CONS 'orange (CONS 'grape '())))
; unexpected behavior: if capitial letter(s) are used it will print lower case letter(s).
; unexpected behavior: if used (swap (a)) or (swap a) will cause an error 
; bypassing the usage message.
(DEFINE (SWAP . args)
	(if (= (length args) 1)
		(let ((s_lis (car args)))
	(COND
		((NOT (LIST? s_lis)) (DISPLAY "USAGE: (swap [list])\nTry again."))
		((NULL? s_lis) '())
		((NULL? (CDR s_lis)) s_lis)
		(ELSE (CONS (CADR s_lis) (CONS (CAR s_lis) (SWAP (CDDR s_lis)) )))
	))
	(display "USAGE: (swap [list])\nTry again."))
)
; (DEFINE (evenatom ...complete this definition, as described in ~cs320/program3
; counts the number of atoms in the list and return true if it is even otherwise false.
; unexpected behavior: if capitial letter(s) are used it will print lower case letter(s).
; unexpected behavior: if used (evenatom (a)) or (evenatom a) will cause an error 
; bypassing the usage message.
(DEFINE (EVENATOM . args)
	(if (= (length args) 1)
		(let ((e_lis (car args)))
	(COND
		((NOT (LIST? e_lis)) (DISPLAY "USAGE: (evenatom [list])\nTry again."))
		((NULL? e_lis) #t)
		(ELSE (EVEN? (COUNT e_lis)))
		
	))
	(display "USAGE: (evenatom [list])\nTry again."))
)
; Will count how many atoms are in the list
; If the list is empty return 0
; if the first element is removed from the list is still a list, recursive until only atoms
; count how many atoms and return. At the end add it all up and return final result.
(DEFINE (COUNT c_lis)
	(COND
		((NULL? c_lis) 0)
		((LIST? (CAR c_lis)) (+ (COUNT (CAR c_lis)) (COUNT (CDR c_lis))))
		(ELSE (+ 1 (COUNT(CDR c_lis))))
	)
)
; (DEFINE (rswap... complete this definition, as described in ~cs320/program3)
; unexpected behavior did not put USAGE for this one. does not check the arugments.
; unexpected behavior: if capitial letter(s) are used it will print lower case letter(s).
; unexpected behavior: if used (rswap (a)) or (rswap a) will cause an error 
(DEFINE (RSWAP r_lis)
	(COND
		((NOT(LIST? r_lis)) (DISPLAY "USAGE: (rswap [list])\nTry again."))
		((NULL? r_lis) '())
		((null? (CDR r_lis)) (cdr (rswap (list (car r_lis) (cdr r_lis)))))
		((not (list? (first r_lis)) ) (rcheck2 r_lis))
		((not (list? (second r_lis))) (rcheck3 r_lis))
		(ELSE (CONS (swap(CADR r_lis)) (CONS (swap(CAR r_lis)) (rswap(CDDR r_lis)) )))
	)
)
; the first element was checked and this checks the second element and sees if it is an atom
; if it is not it does a recursive action until it gets to the last element.
(define (rcheck2 rc_lis2)
	(cond
		((not(list? (second rc_lis2))) (cons (second rc_lis2) (cons (first rc_lis2) (rswap(cddr rc_lis2)) )))
		(else (cons (rswap (cadr rc_lis2)) (cons (first rc_lis2) (rswap(cddr rc_lis2)) )))
	)
)
; the second element was checked and this checks the first element and sees if it is an atom
; if it is not it does a recursive action until it gets to the last element.
(define (rcheck3 rc_lis3)
	(cond
		((not(list? (first rc_lis3))) (cons (second rc_lis3) (cons (first rc_lis3) (rswap(cddr rc_lis3)) )))
		(else (cons (second rc_lis3)(cons (rswap(car rc_lis3)) (rswap(cddr rc_lis3)) )))
	)
)

