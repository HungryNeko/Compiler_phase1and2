gcc lexer.c -c -o lexer.o
gcc symbol_table.c -c -o symbol_table.o
gcc parser.c -c -o parser.o
gcc parse_tree.c -c -o parse_tree.o
gcc func.c parse_tree.o lexer.o symbol_table.o parser.o -o func.exe
func.exe sample.txt
