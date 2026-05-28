<sexp>    ::= <atom> | <list>
<list>    ::= "(" <sexp>* ")"
<atom>    ::= <number> | <symbol>
<symbol>  ::= [a-zA-Z_][a-zA-Z0-9_]*
<number>  ::= [+-]?[0-9]+
