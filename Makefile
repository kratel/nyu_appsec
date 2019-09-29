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
	gcc -Wall -c spell.c

test.o: test_main.c
	gcc -Wall -c test_main.c

main.o: main.c
	gcc -Wall -c main.c

UNAME_S := $(shell uname -s)
test: dictionary.o spell.o test.o
ifneq (,$(wildcard *.gcda))
	rm *.gcda
endif
ifneq (,$(wildcard *.gcno))
	rm *.gcno
endif
ifeq ($(UNAME_S),Linux)
	gcc -Wall -c $(CFLAGS) spell.c
	gcc -Wall -o run_test_main test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit $(LFLAGS)
endif
ifeq ($(UNAME_S),Darwin)
	gcc -Wall -c $(CFLAGS) spell.c
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

alt_prog: spell.c dictionary.c main.c
	gcc -o spell_check -O0 -g dictionary.c spell.c main.c

.PHONY: clean cleancov cleanall spell.o

clean:
ifneq (,$(wildcard dictionary.o))
	rm dictionary.o
endif
ifneq (,$(wildcard spell.o))
	rm spell.o
endif
ifneq (,$(wildcard main.o))
	rm main.o
endif
ifneq (,$(wildcard test_main.o))
	rm test_main.o
endif
ifneq (,$(wildcard *.h.gch))
	rm *.h.gch
endif

cleancov:
ifneq (,$(wildcard *.gcda))
	rm *.gcda
endif
ifneq (,$(wildcard *.gcno))
	rm *.gcno
endif
ifneq (,$(wildcard coverage*.html))
	rm coverage*.html
endif

cleanall: cleancov clean
ifneq (,$(wildcard spell_check))
	rm spell_check
endif
ifneq (,$(wildcard run_test_main))
	rm run_test_main
endif
	
