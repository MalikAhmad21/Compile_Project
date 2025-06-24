tiny_compiler: tiny.l tiny.y ast.c symbol.c semantic.c codegen.c main.c
	flex tiny.l
	bison -d tiny.y
	gcc -o tiny_compiler lex.yy.c tiny.tab.c ast.c symbol.c semantic.c codegen.c main.c -lfl

run: tiny_compiler
	./tiny_compiler < examples/test.tp
	dot -Tpng ast.dot -o ast.png
	dot -Tpng symtable.dot -o symtable.png
	dot -Tpng ir.dot -o ir.png
	nasm -f elf64 output.asm -o output.o
	ld output.o -o output
	@echo "Compilation complete. Run with: ./output"

clean:
	rm -f lex.yy.c tiny.tab.c tiny.tab.h tiny_compiler \
	      ast.dot symtable.dot ir.dot \
	      ast.png symtable.png ir.png \
	      output.asm output.o output
