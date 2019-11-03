# Assignment 1

[![Build Status](https://travis-ci.com/kratel/nyu_appsec.svg?token=9hqx4ysaqwyc5JJXpgtm&branch=master)](https://travis-ci.com/kratel/nyu_appsec)

[![codecov](https://codecov.io/gh/kratel/nyu_appsec/branch/master/graph/badge.svg?token=NKjVKkhy2N)](https://codecov.io/gh/kratel/nyu_appsec)

Project using C language and Travis for CI/CD.

Dictionary.h, dictionary.c, Makefile files taken from [here](https://github.com/kcg295/AppSecAssignment1)

## Goal

To learn tools used in software CI/CD cycle, by building a simple spell checker application.

## Testing

Testing was done using [Check](https://libcheck.github.io/check/).
Test cases can be run locally using the `Makefile` included.
To run tests enter:

```shell
make test
```

## Compiling and Usage

To compile an executable of this spell checker you can run the following command:

```shell
make
```

or:

```shell
make prog
```

Both are equivalent, and this will produce an executable called `spell_check`. This executable expects two command line arguments.

The first argument is an input text file that will have its words checked. The second argument should be a wordlist text file to act as a dictionary of correctly spelled words. The program outputs how many words were misspelled in the input file.

Sample usage using the files included in this repo:

```shell
./spell_check inputs/test1.txt wordlists/wordlist.txt
```

Output:
```text
There were 3 misspelled words in your input text.
Misspelled Words:
sogn
skyn
betta
```
