#include <stdlib.h>
#include <stdbool.h>

typedef struct VectorInt
{
    int  *data;
    size_t  size;
    size_t capacity;

}VectorInt;

typedef enum {
    ERR_NONE = 0,
    ERR_NOT_FOUND,
    ERR_OUT_OF_BOUNDS,
    ERR_EMPTY,
    ERR_NULL_POINTER
} VectorInt_ErrorCode;

typedef struct {
    int *ptr;
    size_t len;
} VectorInt_Slice;


typedef struct {
    bool success;
    union {
        int value;
        VectorInt_ErrorCode error;
    } data;
} VectorInt_IntResult;


typedef struct {
    bool success;
    union {
        VectorInt_Slice slice;
        VectorInt_ErrorCode error;
    } data;
} VectorInt_SliceResult;


typedef struct {
    bool success;
    VectorInt_ErrorCode error;
} VectorInt_InitStatus;


#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))


bool init_VectorInt(VectorInt *vector);
bool allocate_VectorInt(VectorInt *vector, size_t length);
bool init_VectorInt_with_values(VectorInt *vector, int *values, size_t array_size);