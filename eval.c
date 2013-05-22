#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "object.h"
#include "environment.h"

static object *eval_function(object *sexp, object *env)
{
    object *symbol = car(sexp);
    object *args   = cdr(sexp);

    if (IS_LAMBDA(symbol))
        return fn_lambda(sexp, env);
    else if (IS_FUNCTION(symbol)) {
        function_object *func = (function_object*)symbol;
        return (func->fn)(args, env);
    } else
        return sexp;
}

static object *lookup(char *symbol_name, object *env)
{
    object *tmp = env;

    while (tmp != NULL && IS_CONS(tmp)) {
        object *pair, *name, *value;
        pair = car(tmp);
        name = car(pair);
        value = car(cdr(pair));

        if (strcmp(object_name(name), symbol_name) == 0)
            return value;

        tmp = cdr(tmp);
    }

    return NULL;
}

object *eval(object *sexp, object *env)
{
    if (sexp == NULL)
        return nil;

    if (IS_CONS(sexp)) {
        object *first_param = car(sexp);

        if (IS_ATOM(first_param)
            && strcmp(object_name(first_param), "LAMBDA") == 0) {
            object *rest_args = cdr(sexp);
            object *lambda_args = car(rest_args);
            object *lambda_sexp = car( cdr(rest_args) );

            return lambda(lambda_args, lambda_sexp);
        } else {
            object *func_symbol = car(sexp);
            object *accum = cons(eval(func_symbol, env), NULL);

            sexp = cdr(sexp);
            while (sexp != NULL && IS_CONS(sexp)) {
                append(accum, eval(car(sexp), env));
                sexp = cdr(sexp);
            }

            return eval_function(accum, env);
        }
    } else {
        object *value = lookup(object_name(sexp), env);

        if (value == NULL)
            return sexp;
        else
            return value;
    }

    fprintf(stderr, "Never Reach Here");
    exit(1);
}
