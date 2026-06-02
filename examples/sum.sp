(begin
  (define testlist (cons 1 (cons 2 (cons 3 (cons 4 (cons 5 () ))))))
  (define sum_list
    (lambda (l)
      (if (null? l)
        0
        (+ (car l) (sum_list (cdr l))))))
  (sum_list testlist))
