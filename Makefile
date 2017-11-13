cc = cc
c_opt = -std=gnu99 -Wall -Wpedantic -Werror -DDEBUG
l_opt = -lpthread

prog = x
obj = main.o

main_h = 

exmpl_args = 20 9 8 30 365

all: $(prog)

$(prog): $(obj)
	$(cc) $(l_opt) -o$(prog) $(obj)

main.o: main.c
	$(cc) -c $(c_opt) $(opt_test) main.c

run: all
	./$(prog) $(exmpl_args)

clean:
	rm -rf $(obj)

distclean: clean
	rm -rf $(prog)

rebuild: distclean all
