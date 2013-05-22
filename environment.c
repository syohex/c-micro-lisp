#include <stdio.h>
#include <string.h>

#include "object.h"
#include "eval.h"

object *tee;
object *nil;

static object *fn_car(object *args, object *env)
{
    object *first_arg = car(args);
    return car(first_arg);
}

static object *fn_cdr(object *args, object *env)
{
    object *first_arg = car(args);
    return cdr(first_arg);
}

static object *fn_quote(object *args, object *env)
{
    return car(args);
}

static object *fn_cons(object *args, object *env)
{
    object *list = cons(car(args), NULL);
    args = car(cdr(args));

    // @@@

    while (args != NULL && IS_CONS(args)) {
        append(list, car(args));
        args = cdr(args);
    }

    return list;
}

static object *fn_equal(object *args, object *env)
{
    object *first_arg = car(args);
    object *second_arg = car(cdr(args));

    if (strcmp(object_name(first_arg), object_name(second_arg)) == 0)
        return tee;

    return nil;
}

static object *fn_atom(object *args, object *env)
{
    object *first_arg = car(args);

    if (IS_ATOM(first_arg))
        return tee;
    else
        return nil;
}

static object *fn_cond(object *args, object *env)
{
    while (args != NULL && IS_CONS(args)) {
        object *cond_and_body = car(args);
        object *condition_result = nil;
        object *condition = car(cond_and_body);
        object *body = car(cdr(cond_and_body));

        if (condition != nil)
            condition_result = eval(condition, env);

        if (condition_result != nil)
            return eval(body, env);

        args = cdr(args);
    }
    return NULL;
}

static object *create_parameter_pairs(object *parameters, object *arguments)
{
    object *pairs;
    object *pair;
    object *param, *arg;

    param = car(parameters);
    arg   = car(arguments);
    pair = cons(param, cons(arg, NULL));

    pairs = cons(pair, NULL);

    parameters = cdr(parameters);
    arguments = cdr(arguments);

    while (parameters != NULL && IS_CONS(parameters)) {
        param = car(parameters);
        arg   = car(arguments);

        pair = cons(param, cons(arg, NULL));
        append(pairs, pair);

        parameters = cdr(parameters);
        arguments  = cdr(arguments);
    }

    return pairs;
}

static object *replace_atom(object *sexp, object *parameter_pairs)
{
    if (IS_CONS(sexp)) {
        object *list;

        list = cons(replace_atom(car(sexp), parameter_pairs), NULL);
        sexp = cdr(sexp);

        while (sexp != NULL && IS_CONS(sexp)) {
            append(list, replace_atom(car(sexp), parameter_pairs));
            sexp = cdr(sexp);
        }

        return list;

    } else {
        object *tmp = parameter_pairs;

        while (tmp != NULL && IS_CONS(tmp)) {
            object *pair = car(tmp);
            object *parameter = car(pair);
            object *argument = car(cdr(pair));

            if (strcmp(object_name(parameter), object_name(sexp)) == 0)
                return argument;

            tmp = cdr(tmp);
        }

        return sexp;
    }
}

object *fn_lambda(object *args, object *env)
{
    lambda_object *lambda;
    object *arguments, *sexp;
    object *parameter_pairs;

    lambda    = (lambda_object*)car(args);
    arguments = cdr(args);

    parameter_pairs = create_parameter_pairs(lambda->args, arguments);
    sexp = replace_atom(lambda->sexp, parameter_pairs);

    return eval(sexp, env);
}

static object *fn_label(object *args, object *env)
{
    object *first_arg = car(args);
    object *symbol = atom(object_name(first_arg));
    object *value  = car(cdr(args));

    append(env, cons(symbol, cons(value, NULL)));
    return tee;
}
#include <stdlib.h>
object* init_env(void)
{
    object *env;

#define FUNCTION_SYMBOL(name, func_ptr) \
    (cons(atom((name)), cons(function((func_ptr)), NULL)))

    env = cons(FUNCTION_SYMBOL("QUOTE", &fn_quote), NULL);

    append( env, FUNCTION_SYMBOL("CAR", &fn_car));
    append( env, FUNCTION_SYMBOL("CDR", &fn_cdr));
    append( env, FUNCTION_SYMBOL("CONS", &fn_cons));
    append( env, FUNCTION_SYMBOL("EQUAL", &fn_equal));
    append( env, FUNCTION_SYMBOL("ATOM", &fn_atom));
    append( env, FUNCTION_SYMBOL("COND", &fn_cond));
    append( env, FUNCTION_SYMBOL("LAMBDA", &fn_lambda));
    append( env, FUNCTION_SYMBOL("LABEL", &fn_label));

#undef FUNCTION_SYMBOL

    tee = atom("#T");
    nil = cons(NULL,NULL);

    return env;
}
