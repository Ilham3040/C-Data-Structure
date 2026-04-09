#include <stdbool.h>

#define EXPECT(statement, msg) do { \
    if (!(statement)) { \
        printf("\033[1;31mFAILED:\033[0m %s (File: %s, Line: %d)\n", msg, __FILE__, __LINE__); \
        return false; \
    } \
} while (0)

#define EXPECT_EQUAL(base, target) \
    EXPECT((base) == (target), "Value " #base " is not equal to " #target)

#define EXPECT_SMALLER(base, target, msg)  \
    EXPECT((base) < (target), "Value " #base " is not smaller than " #target)

#define EXPECT_BIGGER(base, target, msg)  \
    EXPECT((base) > (target), "Value " #base " is not bigger than " #target)

#define EXPECT_TRUE(base) \
    EXPECT((base), "Expression (" #base ") is not true")

#define EXPECT_FALSE(base) \
    EXPECT((!base), "Expression (" #base ") is not false")
