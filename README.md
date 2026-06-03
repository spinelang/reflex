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
  (define testlist (cons 1 (cons 2 (cons 3 (cons 4 (cons 5 nil))))))
  (define sum_list
    (lambda (l)
      (if (null? l)
        0
        (+ (car l) (sum_list (cdr l))))))
  (sum_list testlist))
```

Examples can be run via `just run example/example.sp`
