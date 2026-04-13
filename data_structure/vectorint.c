#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


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
    ERR_NULL_POINTER,
    ERR_INVALID_LENGTH,
    ERR_HEAP_ALLOCATION_FAILED,
    ERR_RESIZE_FAILED,
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

typedef struct {
    bool success;
    VectorInt_ErrorCode error;
} VectorInt_InsertingResult;






#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define CHECKNULLPTR(ptr) \
    if ((ptr) == NULL) { \
        fprintf(stderr, "Error: %s is NULL at %s:%d\n", #ptr, __FILE__, __LINE__); \
        return false; \
    } 

#define CHECKLENGTH(x) \
    if ((x) == 0) { \
        fprintf(stderr, "Error: length cannot be zero or negative at %s:%d\n", __FILE__, __LINE__); \
    } 

#define CHECKRESIZE(resize, size) \
    if ((resize) < (size)) { \
        fprintf(stderr, "Error: cannot resize smaller than current vector size (size : %zu) at %s:%d\n",(size_t)(size) ,__FILE__, __LINE__); \
        return false; \
    }

#define CHECKINDEX(index, size) \
    if ((index) >= (size)) { \
        fprintf(stderr, "Error: Index %zu out of bounds (Size: %zu) at %s:%d\n", (size_t)(index),(size_t)(size), __FILE__, __LINE__); \
        return false; \
    }



static inline VectorInt_InitResult _internal_VectorInt_allocation(size_t len) {
    if (len == 0) {
        return (VectorInt_InitResult){
            .success = false, 
            .data.error = ERR_INVALID_LENGTH
        };
    }

    VectorInt new_vector = { .data = NULL, .size = 0, .capacity = len };

    new_vector.data = (int *)malloc(sizeof(int) * len);
    if (new_vector.data == NULL) {
        return (VectorInt_InitResult){
            .success = false, 
            .data.error = ERR_HEAP_ALLOCATION_FAILED
        };
    }

    return (VectorInt_InitResult){
        .success = true, 
        .data.vector = new_vector
    };
}







VectorInt_InitResult init_VectorInt() {
    return _internal_VectorInt_allocation(16);
    
}

VectorInt_InitResult allocate_VectorInt(size_t length) {
    return _internal_VectorInt_allocation(length);
}

VectorInt_InitResult init_VectorInt_with_values(int *values, size_t array_size) {
    VectorInt_InitResult result = _internal_VectorInt_allocation(array_size*2);


    if (result.success)
    {
        memcpy(result.data.vector.data,values,array_size*sizeof(int));
        result.data.vector.size = array_size;
        return result;
    }
    
    return result;

}


static inline VectorInt_InsertingResult _reallocate_VectorInt(VectorInt *vector, size_t new_capacity) {
    if (vector == NULL) { 
         return (VectorInt_InsertingResult){
            .success = false, 
            .error = ERR_NULL_POINTER
        };
    };

    if (new_capacity == 0) {
        return (VectorInt_InsertingResult){
            .success = false, 
            .error = ERR_INVALID_LENGTH
        };
    }

    if (new_capacity < vector->size) {
        return (VectorInt_InsertingResult){
            .success = false, 
            .error = ERR_RESIZE_FAILED
        };
    }

    int *temp = realloc(vector->data,sizeof(int)*new_capacity);

    if (temp == NULL)
    {
        return (VectorInt_InsertingResult){
            .success = false, 
            .error = ERR_HEAP_ALLOCATION_FAILED
        };
    }

    vector->capacity = new_capacity;
    vector->data = temp;

    return (VectorInt_InsertingResult){
        .success = true, 
        .error = ERR_NONE
    };
}


VectorInt_InsertingResult insert_VectorInt(VectorInt *vector, int value) {

    if (vector->size >= vector->capacity) {
        VectorInt_InsertingResult result = _reallocate_VectorInt(vector,vector->capacity*2);

        if (!result.success)
        {
            return result;
        } 
        
    }
    vector->data[vector->size] = value;
    vector->size++;
    return (VectorInt_InsertingResult){
        .success = true, 
        .error = ERR_NONE
    };

}

VectorInt_InsertingResult mutiple_insert_VectorInt(VectorInt *vector, size_t array_size, int *values) {

    if (vector->size+array_size >= vector->capacity) {
        size_t needed = MAX(vector->capacity*2,(array_size+vector->size)*2);
        VectorInt_InsertingResult result = _reallocate_VectorInt(vector,needed);

        if (!result.success)
        {
            return result;
        } 
        
    }

    memcpy(vector->data+vector->size,values,array_size * sizeof(int));
    vector->size += array_size;
    return (VectorInt_InsertingResult){
        .success = true, 
        .error = ERR_NONE
    };

}


VectorInt_InsertingResult resize_VectorInt(VectorInt *vector, size_t size) {

    return _reallocate_VectorInt(vector,size);
}


// bool get_index_VectorInt(VectorInt *vector, int *dest, size_t index) {
//     CHECKNULLPTR(vector);
//     CHECKNULLPTR(dest);
//     CHECKINDEX(index,vector->size);

//     *dest = vector->data[index];
//     return true;
// }

// bool slice_index_VectorInt(VectorInt *vector, VectorInt *dest, size_t start, size_t end) {
//     CHECKNULLPTR(vector);
//     CHECKNULLPTR(dest)
//     CHECKINDEX(start,vector->size);
//     CHECKINDEX(end,vector->size);

//     size_t len = (end - start + 1);

//     if (allocate_VectorInt(dest,len))
//     {
//         memcpy(dest->data,vector->data+start, len * sizeof(int));
//         return true;
//     }
    
//     return false;
// }

// bool free_VectorInt(VectorInt *vector) {
//     CHECKNULLPTR(vector);

//     free(vector->data);     
//     vector->data = NULL;
//     vector->size = 0;       
//     vector->capacity = 0;
    
//     return true;
// }

const char* printerror_VectorInt(VectorInt_ErrorCode error) {
    switch (error) {
        case ERR_NONE:                    return "ERR_NONE";
        case ERR_NOT_FOUND:               return "ERR_NOT_FOUND";
        case ERR_OUT_OF_BOUNDS:           return "ERR_OUT_OF_BOUNDS";
        case ERR_EMPTY:                   return "ERR_EMPTY";
        case ERR_NULL_POINTER:            return "ERR_NULL_POINTER";
        case ERR_INVALID_LENGTH:          return "ERR_INVALID_LENGTH";
        case ERR_HEAP_ALLOCATION_FAILED:  return "ERR_HEAP_ALLOCATION_FAILED";
        case ERR_RESIZE_FAILED:           return "ERR_RESIZE_FAILED";
        default:                          return "UNKNOWN_ERROR";
    }
}