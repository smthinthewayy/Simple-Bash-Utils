#include "s21_cat.h"

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
    (argc >= 2) ? s21_cat(argc, argv) : printf("Please enter a filename\n");
    return 0;
}

/**
 * @brief Implementation of the cat utility
 *
 * Cat is one of the most frequently used commands on Unix-like operating systems. It has three related
 * functions with regard to text files: displaying them, combining copies of them and creating new ones.
 * This function handles all flags passed to the command line parameters
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 */
void s21_cat(int argc, char **argv) {
    flags.b = flags.e = flags.E = flags.v = flags.n = flags.s = flags.t = flags.T = false;

    int i = 1;
    bool error = false;

    for (; i < argc; ++i) {
        if (!strspn(argv[i], "-")) break;
        error = get_flags(argv[i]);
    }

    if (flags.b) flags.n = false;

    if (!error) {
        for (; i < argc; ++i) {
            FILE *filename = fopen(argv[i], "r");

            if (filename) {
                char buffer[1024] = {'\0'};
                int first_line = 1, number_of_empty_lines = 1, number_of_all_lines = 1;
                char previous_character = '\n', next_character = ' ';

                while ((*buffer = (char)fgetc(filename)) != EOF) {
                    if (flags.b) flag_b(previous_character, &number_of_empty_lines, *buffer);
                    if (flags.E) flag_E(*buffer);
                    if (flags.v) flag_v(buffer);
                    if (flags.n) first_line = flag_n(first_line, &number_of_all_lines, *buffer);
                    if (flags.s && flag_s(previous_character, next_character, *buffer)) continue;
                    if (flags.T && flag_T(*buffer)) continue;
                    (*buffer == '\0') ? fputc(*buffer, stdout) : fputs(buffer, stdout);
                    next_character = previous_character;
                    previous_character = *buffer;
                    memset(buffer, '\0', 1024);
                }

                fclose(filename);
            } else {
                printf("No such file or directory\n");
            }
        }
    } else {
        printf("Invalid flag '%s'\n", argv[i - 1]);
    }
}

/**
 * @brief Reading flags
 *
 * This function assigns true to the flag if it is passed to the command line parameters
 *
 * @param argv List of arguments
 *
 * @return Error status
 */
int get_flags(char *argv) {
    bool error = false;

    if ((strlen(argv) == 1) || (strlen(argv) != strspn(argv, "-beEvnstT"))) error = true;

    if (!strcmp(argv, "--number")) {
        error = false;
        flags.n = true;
    } else if (!strcmp(argv, "--number-nonblank")) {
        error = false;
        flags.b = true;
    } else if (!strcmp(argv, "--squeeze-blank")) {
        error = false;
        flags.s = true;
    } else if (strchr(argv, 'b')) {
        flags.b = true;
    } else if ((strchr(argv, 'e'))) {
        flags.v = flags.E = true;
    } else if (strchr(argv, 'E')) {
        flags.E = true;
    } else if (strchr(argv, 'v')) {
        flags.v = true;
    } else if (strchr(argv, 'n')) {
        flags.n = true;
    } else if (strchr(argv, 's')) {
        flags.s = true;
    } else if (strchr(argv, 't')) {
        flags.v = flags.T = true;
    } else if (strchr(argv, 'T')) {
        flags.T = true;
    }

    return error;
}

/**
 * @brief Handling the flag b
 *
 * Numbers only non-empty lines
 *
 * @param previous_character Previous character
 * @param number_of_empty_line Number of empty line
 * @param c Current character
 */
void flag_b(char previous_character, int *number_of_empty_line, char c) {
    if (c != '\n' && previous_character == '\n') printf("%6d\t", (*number_of_empty_line)++);
}

/**
 * @brief Handling the flag E
 *
 * Display end-of-line characters as '$'
 *
 * @param c Current character
 */
void flag_E(char c) {
    if (c == '\n') printf("$");
}

/**
 * @brief Handling the flag v
 *
 * Display nonprinting characters
 *
 * @param buffer Output string
 */
void flag_v(char *buffer) {
    unsigned char c = (unsigned char)*buffer;
    if ((c <= 8) || (11 <= c && c <= 31) || (127 <= c && c <= 159)) strcpy(buffer, nonprinting_characters[c]);
}

/**
 * @brief Handling the flag n
 *
 * Number all output lines
 *
 * @param first_line
 * @param number_of_all_lines Number of all lines
 * @param c Current character
 *
 * @return 1 if the current character is '\n', otherwise 0
 */
int flag_n(int first_line, int *number_of_all_lines, char c) {
    if (first_line) printf("%6d\t", (*number_of_all_lines)++);
    return c == '\n';
}

/**
 * @brief Handling the flag s
 *
 * Squeeze multiple adjacent blank lines
 *
 * @param previous_character Previous character
 * @param next_character Next character
 * @param c Current character
 *
 * @return 1 if the previous, current and next characters are equal to '\n', otherwise 0
 */
int flag_s(char previous_character, char next_character, char c) {
    return c == '\n' && previous_character == '\n' && next_character == '\n';
}

/**
 * @brief Handling the flag T
 *
 * Display tabs as ^I
 *
 * @param c Current character
 * @return 1 if the current character is '\n', otherwise 0
 */
int flag_T(char c) { return (c == '\t') ? printf("^I") : 0; }
