#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "object.h"

char *object_name(object *obj)
{
    if (!IS_ATOM(obj)) {
        fprintf(stderr, "[%s] Object is not Atom(type=%d)\n", __func__, obj->type);
        exit(1);
    }

    return ((atom_object*)obj)->name;
}

object *atom(char *atom_name)
{
    atom_object *ptr;
    char *name;
    size_t name_len;

    name_len = strlen(atom_name) + 1; /* with '\0' */

    ptr = (atom_object*)malloc(sizeof(atom_object));
    if (ptr == NULL) {
        perror("malloc(obj):");
        exit(1);
    }

    ptr->type = ATOM;
    name = (char*)malloc(name_len);
    if (name == NULL) {
        perror("malloc(name):");
        exit(1);
    }

    strncpy(name, atom_name, name_len);
    ptr->name = name;
    return (object*)ptr;
}

object *cons(object *first, object *rest)
{
    cons_object *ptr;

    ptr = (cons_object*)malloc(sizeof(cons_object));
    if (ptr == NULL) {
        perror("malloc(ptr)");
        exit(1);
    }

    ptr->type = CONS;
    ptr->car  = first;
    ptr->cdr  = rest;

    return (object*)ptr;
}

object *function(object* (*fn)(object*, object*))
{
    function_object *ptr;

    ptr = (function_object*)malloc(sizeof(function_object));
    if (ptr == NULL) {
        perror("malloc(ptr)");
        exit(1);
    }

    ptr->type = FUNC;
    ptr->fn   = fn;

    return (object*)ptr;
}

object *lambda(object *args, object *sexp)
{
    lambda_object *ptr;

    ptr = (lambda_object*)malloc(sizeof(lambda_object));
    if (ptr == NULL) {
        perror("malloc(obj)");
        exit(1);
    }

    ptr->type = LAMBDA;
    ptr->args = args;
    ptr->sexp = sexp;

    return (object*)ptr;
}

void append(object *list, object *obj)
{
    object *ptr;

    for (ptr = list; cdr(ptr) != NULL; ptr = cdr(ptr))
        ;

    cdr(ptr) = cons(obj, NULL);
}
