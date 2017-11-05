cc = cc
c_opt = -Wall -Wpedantic -Werror
l_opt = 

prog = x
obj = main.o

main_h = 

all: $(prog)

$(prog): $(obj)
	$(cc) $(l_opt) -o$(prog) $(obj)

main.o: main.c
	$(cc) -c $(c_opt) main.c

run: all
	./$(prog)

clean:
	rm -rf $(obj)

distclean: clear
	rm -rf $(prog)
