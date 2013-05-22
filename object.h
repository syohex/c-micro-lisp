#ifndef _OBJECT_H_
#define _OBJECT_H_

enum object_type {
    CONS,
    ATOM,
    FUNC,
    LAMBDA
};

#define IS_ATOM(obj)       ((obj)->type == ATOM)
#define IS_CONS(obj)       ((obj)->type == CONS)
#define IS_FUNCTION(obj)   ((obj)->type == FUNC)
#define IS_LAMBDA(obj)     ((obj)->type == LAMBDA)

/*
 * Object Type Definitions
 */
typedef struct {
    enum object_type type;
} object;

typedef struct {
    enum object_type type;
    char *name;
} atom_object;

typedef struct {
    enum object_type type;
    object *car;
    object *cons;
    object *cdr;
} cons_object;

typedef struct {
    enum object_type type;
    object *(*fn)(object *, object*);
} function_object;

typedef struct {
    enum object_type type;
    object *args;
    object *sexp;
} lambda_object;

#define car(obj)        (((cons_object*)(obj))->car)
#define cdr(obj)        (((cons_object*)(obj))->cdr)

extern char *object_name(object *obj);
extern object *cons(object *first, object *rest);
extern void append(object *list, object *obj);

extern object *atom(char *atom_name);
extern object *function(object* (*fn)(object*, object*));
extern object *lambda(object *args, object *sexp);

extern object *tee;
extern object *nil;

#endif /* _OBJECT_H_ */
