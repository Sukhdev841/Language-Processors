lex encrypt.l
g++ lex.yy.c -o encrypt
./encrypt < input.txt
