(begin
  (define map
  (lambda (f l)
    (if (null? l)
      nil
      (cons
        (f (car l))
        (map f (cdr l))))))
  (define sum_list
    (lambda (l)
      (if (null? l)
        0
        (+ (car l) (sum_list (cdr l))))))
  (define testlist (map (lambda (x) (* x 2)) '(1 2 3 4 5)))
  (sum_list testlist))
