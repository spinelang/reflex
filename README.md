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

Examples can be run via `just run example/example.sp`
Take a look at the other ones!
