#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "object.h"

static object *next_token(FILE *in)
{
    char buffer[1024];
    int ch, index = 0;

    ch = getc(in);
    while (isspace(ch))
        ch = getc(in);

    if (ch == '\n')
        ch = getc(in);

    if (ch == EOF) {
        exit(0);
    }

    if (ch == ')')
        return atom(")");
    if (ch == '(')
        return atom("(");

    while (!isspace(ch) && ch != ')') {
        buffer[index++] = ch;
        ch = getc(in);
    }

    buffer[index++] = '\0';
    if (ch == ')')
        ungetc(ch, in);

    return atom(buffer);
}

static object *read_tail(FILE *in)
{
    object *token = next_token(in);

    if (strcmp(object_name(token), ")") == 0){
        return NULL;
    } else if (strcmp(object_name(token), "(") == 0) {
        object *first = read_tail(in);
        object *second = read_tail(in);

        return cons(first, second);
    } else {
        object *first = token;
        object *second = read_tail(in);

        return cons(first, second);
    }
}

object *read(FILE *in)
{
    object *token = next_token(in);

    if (strcmp(object_name(token), "(") == 0)
        return read_tail(in);

    return token;
}
