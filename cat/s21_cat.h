#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct flags {
    bool b;  // numbers only non-empty lines
    bool e;  // but also display end-of-line characters as '$'
    bool E;  // same as '-e' but without '-v'
    bool v;  // show non printing characters
    bool n;  // number all output lines
    bool s;  // squeeze multiple adjacent blank lines
    bool t;  // but also display tabs as ^I
    bool T;  // same as '-t' but without '-v'
} flags;

static const char *nonprinting_characters[255] = {
        [0] = "^@",      [1] = "^A",     [2] = "^B",     [3] = "^C",     [4] = "^D",     [5] = "^E",
        [6] = "^F",      [7] = "^G",     [8] = "^H",     [9] = "^I",     [10] = "\n",    [11] = "^K",
        [12] = "^L",     [13] = "^M",    [14] = "^N",    [15] = "^O",    [16] = "^P",    [17] = "^Q",
        [18] = "^R",     [19] = "^S",    [20] = "^T",    [21] = "^U",    [22] = "^V",    [23] = "^W",
        [24] = "^X",     [25] = "^Y",    [26] = "^Z",    [27] = "^[",    [28] = "^\\",   [29] = "^]",
        [30] = "^^",     [31] = "^_",    [32] = " ",     [33] = "!",     [34] = "\"",    [35] = "#",
        [36] = "$",      [37] = "%",     [38] = "&",     [39] = "'",     [40] = "(",     [41] = ")",
        [42] = "*",      [43] = "+",     [44] = ",",     [45] = "-",     [46] = ".",     [47] = "/",
        [48] = "0",      [49] = "1",     [50] = "2",     [51] = "3",     [52] = "4",     [53] = "5",
        [54] = "6",      [55] = "7",     [56] = "8",     [57] = "9",     [58] = ":",     [59] = ";",
        [60] = "<",      [61] = "=",     [62] = ">",     [63] = "?",     [64] = "@",     [65] = "A",
        [66] = "B",      [67] = "C",     [68] = "D",     [69] = "E",     [70] = "F",     [71] = "G",
        [72] = "H",      [73] = "I",     [74] = "J",     [75] = "K",     [76] = "L",     [77] = "M",
        [78] = "N",      [79] = "O",     [80] = "P",     [81] = "Q",     [82] = "R",     [83] = "S",
        [84] = "T",      [85] = "U",     [86] = "V",     [87] = "W",     [88] = "X",     [89] = "Y",
        [90] = "Z",      [91] = "[",     [92] = "\\",    [93] = "]",     [94] = "^",     [95] = "_",
        [96] = "`",      [97] = "a",     [98] = "b",     [99] = "c",     [100] = "d",    [101] = "e",
        [102] = "f",     [103] = "g",    [104] = "h",    [105] = "i",    [106] = "j",    [107] = "k",
        [108] = "l",     [109] = "m",    [110] = "n",    [111] = "o",    [112] = "p",    [113] = "q",
        [114] = "r",     [115] = "s",    [116] = "t",    [117] = "u",    [118] = "v",    [119] = "w",
        [120] = "x",     [121] = "y",    [122] = "z",    [123] = "{",    [124] = "|",    [125] = "}",
        [126] = "~",     [127] = "^?",   [128] = "M-^@", [129] = "M-^A", [130] = "M-^B", [131] = "M-^C",
        [132] = "M-^D",  [133] = "M-^E", [134] = "M-^F", [135] = "M-^G", [136] = "M-^H", [137] = "M-^I",
        [138] = "M-^J",  [139] = "M-^K", [140] = "M-^L", [141] = "M-^M", [142] = "M-^N", [143] = "M-^O",
        [144] = "M-^P",  [145] = "M-^Q", [146] = "M-^R", [147] = "M-^S", [148] = "M-^T", [149] = "M-^U",
        [150] = "M-^V",  [151] = "M-^W", [152] = "M-^X", [153] = "M-^Y", [154] = "M-^Z", [155] = "M-^[",
        [156] = "M-^\\", [157] = "M-^]", [158] = "M-^^", [159] = "M-^_"};

void s21_cat(int argc, char *argv[]);

int get_flags(char *argv);

void flag_b(char previous_character, int *number_of_empty_line, char c);

void flag_E(char c);

void flag_v(char *buffer);

int flag_n(int first_line, int *number_of_all_lines, char c);

int flag_s(char previous_character, char next_character, char c);

int flag_T(char c);

#endif  // SRC_CAT_S21_CAT_H_
