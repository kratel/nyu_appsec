default: prog
CFLAGS:=-fprofile-arcs -ftest-coverage
LFLAGS:=-lgcov --coverage
MAC_LFLAGS:=--coverage
get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get update
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h

spell.o: spell.c
	gcc -Wall -c $(CFLAGS) spell.c

test.o: test_main.c
	gcc -Wall -c test_main.c

main.o: main.c
	gcc -Wall -c main.c

UNAME_S := $(shell uname -s)
test: dictionary.o spell.o test_main.o
ifeq ($(UNAME_S),Linux)
	gcc -Wall -o run_test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit $(LFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
	gcc -Wall -o run_test_main test_main.o spell.o dictionary.o -lcheck -lm -lpthread $(MAC_LFLAGS)
endif
	./run_test_main

report: spell.gcda spell.gcno
ifeq ($(UNAME_S),Linux)
	@echo "Detected Linux, Travis will trigger coverage bash script."
endif
ifeq ($(UNAME_S),Darwin)
	gcovr -r . --html --html-details -o coverage_report.html
endif

prog: dictionary.o spell.o main.o
	gcc -Wall -o spell_check dictionary.o spell.o main.o

.PHONY: clean cleancov cleanall

clean:
	rm dictionary.o spell.o main.o test_main.o check_spell.o

cleancov:
	rm *.gcda *.gcno coverage*.html

cleanall:clean
	rm spell_check
	
