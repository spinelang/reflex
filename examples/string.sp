(begin
  (define == (lambda (a b) (and (<= a b) (<= b a))))
  (define strlen (lambda (s) (if (null? s) 0 (+ 1 (strlen (cdr s))))))
  (define streq (lambda (a b)
    (if (and (null? a) (null? b))
      1
      (if (or (null? a) (null? b))
        0
        (if(== (car a) (car b))
          (streq (cdr a) (cdr b))
            0)))))
  (+ (streq "abcd" "abcd") (streq "abcc" "abcd") (streq "aaaaa" "aaaaa"))))