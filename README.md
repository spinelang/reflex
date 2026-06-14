# Reflex
Interpreter for the spine programming language.

## Examples
### nth Fibonacci number
```
(begin
    (define fib (lambda (n)
      (if (<= n 2)
        1
        (+
          (fib (+ n -1))
          (fib (+ n -2))))))
    (fib 29))
```
### Getting a sum of a list of arbitrary length
```
(begin
  (define testlist '(1 2 3 4 5))
  (define sum
    (lambda (l)
      (if (null? l)
        0
        (+ (car l) (sum (cdr l))))))
  (sum testlist))
```
### String equality
```
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
```

Examples can be run via `just run example/example.sp`
Take a look at the other ones!
