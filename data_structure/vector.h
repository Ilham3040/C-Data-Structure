#include <stdlib.h>
#include <stdbool.h>

typedef struct Vector
{
    int  *data;
    size_t  size;
    size_t capacity;

}Vector;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))


bool init_Vector(Vector *vector);
bool allocate_Vector(Vector *vector, size_t length);
bool init_Vector_with_values(Vector *vector, int *values, size_t array_size);