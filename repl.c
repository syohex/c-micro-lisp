#include <stdio.h>

#include "object.h"
#include "environment.h"
#include "eval.h"
#include "read.h"

static void print(object *sexp)
{
    if (sexp == NULL)
        return;

    if (IS_CONS(sexp)) {
        printf("(");

        print(car(sexp));
        sexp = cdr(sexp);
        while (sexp != NULL && IS_CONS(sexp)) {
            printf(" ");
            print(car(sexp));

            sexp = cdr(sexp);
        }

        printf(")");
    } else if (IS_ATOM(sexp)) {
        printf("%s", object_name(sexp));
    } else if (IS_LAMBDA(sexp)) {
        lambda_object *lambda = (lambda_object*)sexp;

        printf("#");

        print(lambda->args);
        print(lambda->sexp);
    } else {
        printf("Error.");
    }
}

void repl(FILE *input)
{
    object *env;

    env = init_env();

    do {
        printf("(simple-lisp)> ");
        print(eval(read(input), env));
        printf("\n");
    } while(1);
}
