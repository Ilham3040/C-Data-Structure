#include <assert.h>


// only for compile time

#define ASSERT_EQUAL(base, target)   (assert((base) == (target)))
#define ASSERT_SMALLER(base, target) (assert((base) < (target)))
#define ASSERT_BIGGER(base, target)  (assert((base) > (target)))
#define ASSERT_TRUE(base)            (assert((base)))
#define ASSERT_FALSE(base)           (assert(!(base)))