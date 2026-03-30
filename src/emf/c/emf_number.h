/* ---------------------------------------------------------------------------
 * Generic EMF number implementation for C.
 *
 * Use the macro DEFINE_EMF_NUMBER to create a concrete type based on a
 * primitive or user‑defined type. The macro expands a struct and a set of
 *  functions that mimic the behavior of the original C++ class.
 *
 * Example:
 *
 *   // Create an EMF number type that works with uint16_t
 *   DEFINE_EMF_NUMBER(uint16_t, emf_u16);
 *
 *   // Use it
 *   emf_u16_t a;
 *   emf_u16_init(&a, 10);
 *   emf_u16_add(&a, 5);          // a.value becomes 15
 *   uint16_t raw = emf_u16_get(&a); // raw == 15
 *
 * --------------------------------------------------------------------------- */

#ifndef EMF_NUMBER_H
#define EMF_NUMBER_H

/* Macro that generates a complete set of functions for a given underlying type.
 *
 *   TYPE   – the underlying numeric type (e.g., uint16_t, int32_t, double)
 *   PREFIX – a unique identifier that will prefix all generated symbols
 *
 * The generated symbols are:
 *
 *   PREFIX##_t                 – struct holding the value
 *   PREFIX##_init(&obj, v)     – constructor (default value 0 if v omitted)
 *   PREFIX##_assign(&obj, v)   – assign a raw value
 *   PREFIX##_add(&obj, v)      – add a raw value
 *   PREFIX##_sub(&obj, v)      – subtract a raw value
 *
 *   PREFIX##_add_num(&obj, *other) – add another emf number
 *   PREFIX##_sub_num(&obj, *other) – subtract another emf number
 *   PREFIX##_mul_num(&obj, *other) – multiply by another emf number
 *   PREFIX##_div_num(&obj, *other) – divide by another emf number
 *
 *   PREFIX##_inc(&obj, *other) – compound +=
 *   PREFIX##_dec(&obj, *other) – compound -=
 *
 *   PREFIX##_get(&obj)         – returns the stored raw value (unsigned)
 *   PREFIX##_to_raw(&obj)      – implicit conversion to raw type
 *
 *   PREFIX##_eq(&obj, *other)  – equality comparison (EMF numbers)
 *   PREFIX##_neq(&obj, *other) – inequality comparison (EMF numbers)
 *
 *   PREFIX##_eq_raw(&obj, v)   – compare with a raw value
 *   PREFIX##_neq_raw(&obj, v)  – compare with a raw value (negated)
 * --------------------------------------------------------------------------- */

#define DEFINE_EMF_NUMBER(TYPE, PREFIX)                                          \
                                                                                  \
typedef struct {                                                                  \
    TYPE value; /* stored numeric value */                                        \
} PREFIX##_t;                                                                     \
                                                                                  \
/* Constructor – initialise with a value (default 0) */                          \
static  void PREFIX##_init(PREFIX##_t *obj, TYPE v) {                       \
    if (obj) obj->value = v;                                                      \
}                                                                                 \
static  void PREFIX##_init_default(PREFIX##_t *obj) {                       \
    PREFIX##_init(obj, (TYPE)0);                                                  \
}                                                                                 \
                                                                                  \
/* Assign a raw value */                                                          \
static  void PREFIX##_assign(PREFIX##_t *obj, TYPE v) {                     \
    if (obj) obj->value = v;                                                      \
}                                                                                 \
                                                                                  \
/* Add / subtract a raw value */                                                  \
static  void PREFIX##_add(PREFIX##_t *obj, TYPE v) {                        \
    if (obj) obj->value += v;                                                     \
}                                                                                 \
static  void PREFIX##_sub(PREFIX##_t *obj, TYPE v) {                        \
    if (obj) obj->value -= v;                                                     \
}                                                                                 \
                                                                                  \
/* Arithmetic with another emf number */                                          \
static  PREFIX##_t PREFIX##_add_num(const PREFIX##_t *a,                     \
                                          const PREFIX##_t *b) {                 \
    PREFIX##_t result = { (uint16_t)a->value + (uint16_t)b->value };                                  \
    return result;                                                                \
}                                                                                 \
static  PREFIX##_t PREFIX##_sub_num(const PREFIX##_t *a,                     \
                                          const PREFIX##_t *b) {                 \
    PREFIX##_t result = { (uint16_t)a->value - (uint16_t)b->value };                                  \
    return result;                                                                \
}                                                                                 \
static  PREFIX##_t PREFIX##_mul_num(const PREFIX##_t *a,                     \
                                          const PREFIX##_t *b) {                 \
    PREFIX##_t result = { (uint16_t)a->value * (uint16_t)b->value };                                  \
    return result;                                                                \
}                                                                                 \
static  PREFIX##_t PREFIX##_div_num(const PREFIX##_t *a,                     \
                                          const PREFIX##_t *b) {                 \
    PREFIX##_t result = { (uint16_t)a->value / (uint16_t)b->value };                                  \
    return result;                                                                \
}                                                                                 \
                                                                                  \
/* Compound assignment operators */                                               \
static  void PREFIX##_inc(PREFIX##_t *obj, const PREFIX##_t *other) {       \
    if (obj) obj->value += other->value;                                          \
}                                                                                 \
static  void PREFIX##_dec(PREFIX##_t *obj, const PREFIX##_t *other) {       \
    if (obj) obj->value -= other->value;                                          \
}                                                                                 \
                                                                                  \
/* Get the stored raw (unsigned) value */                                         \
static  TYPE PREFIX##_get(const PREFIX##_t *obj) {                          \
    return obj ? obj->value : (TYPE)0;                                            \
}                                                                                 \
                                                                                  \
/* Get the stored raw (unsigned) value */                                         \
static  TYPE PREFIX##_getUnsigned(const PREFIX##_t *obj) {                          \
    return obj ? obj->value : (TYPE)0;                                            \
}                                                                                 \
                                                                                  \
/* Implicit conversion – identical to get() */                                    \
static  TYPE PREFIX##_to_raw(const PREFIX##_t *obj) {                       \
    return PREFIX##_get(obj);                                                     \
}                                                                                 \
                                                                                  \
/* Equality / inequality comparisons (EMF numbers) */                           \
static  bool PREFIX##_eq(const PREFIX##_t *a, const PREFIX##_t *b) {        \
    return PREFIX##_get(a) == PREFIX##_get(b);                                    \
}                                                                                 \
static  bool PREFIX##_neq(const PREFIX##_t *a, const PREFIX##_t *b) {       \
    return PREFIX##_get(a) != PREFIX##_get(b);                                    \
}                                                                                 \
static  bool PREFIX##_notEquals(const PREFIX##_t *a, const PREFIX##_t *b) {       \
    return PREFIX##_get(a) != PREFIX##_get(b);                                    \
}                                                                                 \
                                                                                  \
/* Equality / inequality comparisons with a raw value */                         \
static  bool PREFIX##_eq_raw(const PREFIX##_t *obj, TYPE v) {               \
    return PREFIX##_get(obj) == v;                                                \
}                                                                                 \
static  bool PREFIX##_neq_raw(const PREFIX##_t *obj, TYPE v) {              \
    return PREFIX##_get(obj) != v;                                                \
}

/* ---------------------------------------------------------------------------
 * Example instantiation for a 16‑bit unsigned type.
 * --------------------------------------------------------------------------- */
// DEFINE_EMF_NUMBER(uint16_t, emf_u16)
DEFINE_EMF_NUMBER(uint16_t, emf_u16)
DEFINE_EMF_NUMBER(uint8_t, emf_u8)

#endif /* EMF_NUMBER_H */
