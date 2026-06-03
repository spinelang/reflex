(begin
  (define map
  (lambda (f l)
    (if (null? l)
      nil
      (cons
        (f (car l))
        (map f (cdr l))))))
  (define sum
    (lambda (l)
      (if (null? l)
        0
        (+ (car l) (sum (cdr l))))))
  (define testlist (map (lambda (x) (* x 2)) '(1 2 3 4 5)))
  (sum testlist))
