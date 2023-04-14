#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 8192
#define ERROR_MESSAGE "usage: grep [-eivclnhsfo] [-e pattern] [-f file] [template] [file ...]\n"

static char search_string[BUFFSIZE] = {0};

struct flags {
    bool e;  // pattern
    bool i;  // ignore uppercase vs lowercase
    bool v;  // invert match
    bool c;  // output count of matching lines only
    bool l;  // output matching files only
    bool n;  // precede each matching line with a line number
    bool h;  // output matching lines without preceding them by file names
    bool s;  // suppress error messages about nonexistent or unreadable files
    bool f;  // take regexes from a file
    bool o;  // output the matched parts of a matching line
} flags;

struct parameters {
    int number_of_files;
    int number_of_lines;
    int empty_line;
    int number_of_e;
} parameters;

void s21_grep(int argc, char **argv);

void get_flags(int argc, char **argv);

void flag_e();

void flag_f();

void open_file(int argc, char **argv);

void processing(char **argv, FILE *steam);

#endif  // SRC_GREP_S21_GREP_H_
