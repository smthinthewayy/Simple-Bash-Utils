#include "s21_grep.h"

/**
 * @brief Entry point
 *
 * Execution of the program starts here
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 *
 * @return Program exit status
 */
int main(int argc, char **argv) {
    (argc >= 2) ? s21_grep(argc, argv) : printf(ERROR_MESSAGE);
    return 0;
}

/**
 * @brief Implementation of the grep utility
 *
 * Grep is a command-line utility for searching plain-text data sets for lines that match a regular expression
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 */
void s21_grep(int argc, char **argv) {
    flags.e = flags.i = flags.v = flags.c = flags.l = flags.n = flags.h = flags.s = flags.f = flags.o = false;
    get_flags(argc, argv);
    open_file(argc, argv);
}

/**
 * @brief Reading flags
 *
 * This function assigns true to the flag if it is passed to the command-line arguments. The getopt_long()
 * function parses command-line arguments. An argv element that begins with "-" or "--" is considered an
 * option. The characters of this element are option characters. Each repeated call to getopt_long()
 * returns the characters of the next option. If there are no more option characters, getopt_long() returns -1
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 */
void get_flags(int argc, char **argv) {
    while (true) {
        int flag = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL);

        if (flag == -1) {
            break;
        } else if (flag == 'e') {
            flags.e = true;
            parameters.number_of_e++;
            flag_e();
        } else if (flag == 'i') {
            flags.i = true;
        } else if (flag == 'v') {
            flags.v = true;
        } else if (flag == 'c') {
            flags.c = true;
        } else if (flag == 'l') {
            flags.l = true;
        } else if (flag == 'n') {
            flags.n = true;
        } else if (flag == 'h') {
            flags.h = true;
        } else if (flag == 's') {
            flags.s = true;
        } else if (flag == 'f') {
            flags.f = true;
            flag_f();
        } else if (flag == 'o') {
            flags.o = true;
        } else {
            exit(1);
        }
    }

    if (parameters.empty_line) flags.o = false;

    // forming a regular expression
    if (!(flags.e || flags.f)) strcat(search_string, argv[optind++]);
}

/**
 * @brief Handling the flag e
 *
 * Pattern
 */
void flag_e() {
    // forming a regular expression
    if (parameters.number_of_e > 1) strcat(search_string, "|");
    strcat(search_string, optarg);
}

/**
 * @brief Handling the flag f
 *
 * Take regexes from a file
 */
void flag_f() {
    FILE *file = NULL;
    char line[BUFFSIZE] = {0};

    if ((file = fopen(optarg, "r"))) {
        fseek(file, 0, SEEK_SET);
        while (fgets(line, BUFFSIZE, file) != NULL) {
            if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = 0;
            if (parameters.number_of_e > 0) strcat(search_string, "|");
            if (*line == '\0') {
                parameters.empty_line = 1;
                strcat(search_string, ".");
            } else {
                strcat(search_string, line);
            }
            parameters.number_of_e++;
        }
        fclose(file);
    } else {
        printf("No such file\n");
        exit(1);
    }
}

/**
 * @brief Opening a file
 *
 * If the file is successfully opened, we process it, otherwise we get an error
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 */
void open_file(int argc, char **argv) {
    parameters.number_of_files = argc - optind;
    while (optind < argc) {
        FILE *file;
        if ((file = fopen(argv[optind], "r")) != NULL) {
            processing(argv, file);
            fclose(file);
        } else if (!flags.s) {
            printf("No such file or directory\n");
        }
        optind++;
    }
}

/**
 * @brief File processing
 *
 * Process regular expressions and output the necessary information from the file
 *
 * @param argv List of arguments
 * @param file File for processing
 */
void processing(char **argv, FILE *file) {
    regex_t regex;
    regmatch_t regmatch[1] = {0};
    int success, regflag = REG_EXTENDED;
    char temp[BUFFSIZE] = {0};
    size_t line_number = 1, nmatch = 1;
    parameters.number_of_lines = 0;

    if (flags.i) regflag |= REG_ICASE;

    regcomp(&regex, search_string, regflag);

    while (!feof(file)) {
        if (fgets(temp, BUFFSIZE, file)) {
            int new_line_o_counter = 1;
            success = regexec(&regex, temp, nmatch, regmatch, 0);
            // Reverse the success value if the '-v' flag
            if (flags.v) success = success ? 0 : 1;

            if (success != REG_NOMATCH) {
                if (!flags.c && !flags.l) {
                    if (!flags.h && parameters.number_of_files > 1) printf("%s:", argv[optind]);
                    // Handling the flag '-n'
                    if (flags.n) printf("%lu:", line_number);
                    if (flags.o && !flags.v) {
                        new_line_o_counter = false;
                        char *ptr = temp;
                        while (!success) {
                            if (regmatch[0].rm_eo == regmatch[0].rm_so) break;
                            printf("%.*s\n", (int)(regmatch[0].rm_eo - regmatch[0].rm_so),
                                   ptr + regmatch[0].rm_so);
                            ptr += regmatch[0].rm_eo;
                            success = regexec(&regex, ptr, nmatch, regmatch, REG_NOTBOL);
                        }
                    }

                    // Output the result of the all flags except '-o', '-c', '-l'
                    if (!flags.o) printf("%s", temp);

                    if (temp[strlen(temp) - 1] != '\n' && new_line_o_counter) printf("\n");
                }
                parameters.number_of_lines++;
            }
            line_number++;
        }
    }

    // Handling the flag '-c'
    if (flags.c) {
        if (!flags.h && parameters.number_of_files > 1) printf("%s:", argv[optind]);
        printf((flags.l && parameters.number_of_lines) ? "1\n" : "%d\n", parameters.number_of_lines);
    }

    // Handling the flag '-l'
    if (flags.l && parameters.number_of_lines) printf("%s\n", argv[optind]);

    regfree(&regex);
}
