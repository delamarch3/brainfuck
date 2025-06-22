#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 30000

void run(char *program, char *data) {
    char c;
    int stack = 0;

    // clang-format off
    while ((c = *program++)) {
outer:
        switch (c) {
        case '>': data++; break;
        case '<': data--; break;
        case '+': (*data)++; break;
        case '-': (*data)--; break;
        case '.': putchar(*data); break;
        case ',': *data = (unsigned char)getchar(); break;
        case '[': if (*data == 0) { while ((c = *program++)) { switch (c) { case ']': if (stack == 0) { goto outer; } else { stack--; }; break; case '[': stack++; break; } } } break;
        case ']': if (*data != 0) { program--; while ((c = *--program)) { switch (c) { case '[': if (stack == 0) { goto outer; } else { stack--; }; break; case ']': stack++; break; } } } break;
        case '\n': case '\t': case ' ': continue;
        default: break;
        }
    }
    // clang-format on
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <input file>", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "could not open input file: %s\n", strerror(errno));
        return 1;
    }

    char program[BUFLEN] = {0};
    fread(program, BUFLEN, 1, input);
    if (ferror(input)) {
        fprintf(stderr, "there was an error reading the input file\n");
        return 1;
    }

    char data[BUFLEN] = {0};
    run(program, data);

    return 0;
}
