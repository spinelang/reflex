(begin
  (define testlist '(1 2 3 4 5))
  (define sum_list
    (lambda (l)
      (if (null? l)
        0
        (+ (car l) (sum_list (cdr l))))))
  (sum_list testlist))
