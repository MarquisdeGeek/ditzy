/* ---------------------------------------------------------------------------
 *  C version of the C++ class `emfRegisterPair`.
 * ---------------------------------------------------------------------------
 *  The class encapsulated two `emfNumber<uint8_t>` objects (high byte and low
 *  byte) and provided utilities for handling a combined 16‑bit register.
 *
 *  In C we model the same behaviour with a `struct` that holds pointers to the
 *  high‑ and low‑byte objects.  All operations are expressed as functions that
 *  accept a pointer to the `emfRegisterPair` instance.
 *
 *  The helper type `emf_u8_t` is a minimal stand‑in that stores a
 *  single unsigned byte and offers `getUnsigned` / `set` accessors.  In a real
 *  project this would be replaced by the actual definition of `emfNumber`.
 * --------------------------------------------------------------------------- */


/* ---------------------------------------------------------------------------
 *  Definition of emfRegisterPair
 * --------------------------------------------------------------------------- */
typedef struct {
    emf_u8_t *high;   /* pointer to the high‑byte register */
    emf_u8_t *low;    /* pointer to the low‑byte register  */
} emfRegisterPair;

/* Constructor‑like initializer */
static  void emfRegisterPair_init(emfRegisterPair *pair,
                                        emf_u8_t *high,
                                        emf_u8_t *low)
{
    pair->high = high;
    pair->low  = low;
}

/* Access individual bytes */
static  emf_u8_t *emfRegisterPair_hi(emfRegisterPair *pair)
{
    return pair->high;
}

static  emf_u8_t *emfRegisterPair_lo(emfRegisterPair *pair)
{
    return pair->low;
}

/* ---------------------------------------------------------------------------
 *  Combined 16‑bit access
 * --------------------------------------------------------------------------- */
static  uint16_t emfRegisterPair_get(const emfRegisterPair *pair)
{
    uint16_t highPart = ((uint16_t)emf_u8_get(pair->high)) << (uint16_t)8;
    uint16_t lowPart  = (uint16_t)emf_u8_get(pair->low);
    return highPart | lowPart;
}

/* Alias – the original class offered getUnsigned() */
static  uint16_t emfRegisterPair_getUnsigned(const emfRegisterPair *pair)
{
    return emfRegisterPair_get(pair);
}

/* Set the combined 16‑bit value (splits into two bytes) */
static  void emfRegisterPair_set(emfRegisterPair *pair, uint16_t value)
{
    uint8_t highByte = (uint8_t)((value >> 8) & 0xFF);
    uint8_t lowByte  = (uint8_t)(value & 0xFF);
    emf_u8_assign(pair->high, highByte);
    emf_u8_assign(pair->low,  lowByte);
}

/* Alias – original class offered assign() */
static  void emfRegisterPair_assign(emfRegisterPair *pair, uint16_t value)
{
    emfRegisterPair_set(pair, value);
}

/* ---------------------------------------------------------------------------
 *  Assignment‑like operations
 * --------------------------------------------------------------------------- */
static  void emfRegisterPair_copyFromPair(emfRegisterPair *dest,
                                                const emfRegisterPair *src)
{
    emfRegisterPair_set(dest, emfRegisterPair_get(src));
}

/* Assign from raw 16‑bit value */
static  void emfRegisterPair_copyFromUint16(emfRegisterPair *dest,
                                                  uint16_t value)
{
    emfRegisterPair_set(dest, value);
}

/* ---------------------------------------------------------------------------
 *  Arithmetic helpers (return a new temporary pair)
 * --------------------------------------------------------------------------- */
static  emfRegisterPair emfRegisterPair_add(const emfRegisterPair *a,
                                                  const emfRegisterPair *b)
{
    emfRegisterPair result = *a;            /* copy pointers only */
    emfRegisterPair_set(&result,
                        emfRegisterPair_get(a) + emfRegisterPair_get(b));
    return result;
}

static  emfRegisterPair emfRegisterPair_sub(const emfRegisterPair *a,
                                                  const emfRegisterPair *b)
{
    emfRegisterPair result = *a;
    emfRegisterPair_set(&result,
                        emfRegisterPair_get(a) - emfRegisterPair_get(b));
    return result;
}

/* In‑place addition / subtraction */
static  void emfRegisterPair_addAssign(emfRegisterPair *target,
                                             const emfRegisterPair *other)
{
    emfRegisterPair_set(target,
                        emfRegisterPair_get(target) + emfRegisterPair_get(other));
}

static  void emfRegisterPair_subAssign(emfRegisterPair *target,
                                             const emfRegisterPair *other)
{
    emfRegisterPair_set(target,
                        emfRegisterPair_get(target) - emfRegisterPair_get(other));
}

/* ---------------------------------------------------------------------------
 *  Equality helpers
 * --------------------------------------------------------------------------- */
static  bool emfRegisterPair_equalPair(const emfRegisterPair *a,
                                             const emfRegisterPair *b)
{
    return emfRegisterPair_get(a) == emfRegisterPair_get(b);
}

static  bool emfRegisterPair_notEqualPair(const emfRegisterPair *a,
                                                const emfRegisterPair *b)
{
    return emfRegisterPair_get(a) != emfRegisterPair_get(b);
}

static  bool emfRegisterPair_equals(const emfRegisterPair *a,
                                             uint16_t value)
{
    return emfRegisterPair_get(a) == value;
}

static  bool emfRegisterPair_notEquals(const emfRegisterPair *a,
                                                uint16_t value)
{
    return emfRegisterPair_get(a) != value;
}


static  bool emfRegisterPair_equalUint16(const emfRegisterPair *pair,
                                               uint16_t value)
{
    return emfRegisterPair_get(pair) == value;
}

static  bool emfRegisterPair_notEqualUint16(const emfRegisterPair *pair,
                                                  uint16_t value)
{
    return emfRegisterPair_get(pair) != value;
}

/* ---------------------------------------------------------------------------
 *  Explicit “equals” / “notEquals” methods from the original class
 * --------------------------------------------------------------------------- */
static  bool emfRegisterPair_equalsUint16(const emfRegisterPair *pair,
                                                const uint16_t *value)
{
    return emfRegisterPair_getUnsigned(pair) == *value;
}

static  bool emfRegisterPair_notEqualsUint16(const emfRegisterPair *pair,
                                                   const uint16_t *value)
{
    return emfRegisterPair_getUnsigned(pair) != *value;
}

static  bool emfRegisterPair_equalsPair(const emfRegisterPair *pair,
                                              const emfRegisterPair *other)
{
    return emfRegisterPair_getUnsigned(pair) ==
           emfRegisterPair_getUnsigned(other);
}

static  bool emfRegisterPair_notEqualsPair(const emfRegisterPair *pair,
                                                 const emfRegisterPair *other)
{
    return emfRegisterPair_getUnsigned(pair) !=
           emfRegisterPair_getUnsigned(other);
}

/* ---------------------------------------------------------------------------
 *  Implicit conversion to uint16_t – in C we provide an explicit helper.
 * --------------------------------------------------------------------------- */
static  uint16_t emfRegisterPair_toUint16(const emfRegisterPair *pair)
{
    return emfRegisterPair_get(pair);
}
