cc = cc
c_opt = -std=gnu99 -Wall -Wpedantic -Werror -DDEBUG -DTERM_CODES
l_opt = -lpthread

prog = x
obj = main.o

main_h = 

all: $(prog)

$(prog): $(obj)
	$(cc) $(l_opt) -o$(prog) $(obj)

main.o: main.c
	$(cc) -c $(c_opt) $(opt_test) main.c

run: all
	./$(prog)

clean:
	rm -rf $(obj)

distclean: clean
	rm -rf $(prog)

rebuild: distclean all
