flex ./word.l
yacc -d ./grammar.y
g++ y.tab.c lex.yy.c -o aaa -w
./aaa ./test.sy
gcc assemble.s -o assemble
./assemble

