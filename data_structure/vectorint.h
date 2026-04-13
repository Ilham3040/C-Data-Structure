#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int  *data;
    size_t  size;
    size_t capacity;

}VectorInt;

typedef struct {
    int *ptr;
    size_t len;
} VectorInt_Slice;




typedef enum {
    ERR_NONE = 0,
    ERR_NOT_FOUND,
    ERR_OUT_OF_BOUNDS,
    ERR_EMPTY,
    ERR_NULL_POINTER
} VectorInt_ErrorCode;


typedef struct {
    bool success;
    union {
        VectorInt vector;
        VectorInt_ErrorCode error;
    } data;
} VectorInt_InitResult;

typedef struct {
    bool success;
    union {
        int value;
        VectorInt_ErrorCode error;
    } data;
} VectorInt_ValueResult;


typedef struct {
    bool success;
    union {
        VectorInt_Slice slice;
        VectorInt_ErrorCode error;
    } data;
} VectorInt_SliceResult;



#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ARRAY_LEN(x) (sizeof(x) / sizeof(x[0]))


VectorInt_InitResult init_VectorInt();
VectorInt_InitResult allocate_VectorInt(size_t length);
VectorInt_InitResult init_VectorInt_with_values(int *values, size_t array_size);
const char* printerror_VectorInt(VectorInt_ErrorCode error);