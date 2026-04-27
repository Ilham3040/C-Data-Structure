#include <string.h>
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
    ERR_EMPTY_VECTOR,
    ERR_NULL_POINTER,
    ERR_INVALID_LENGTH,
    ERR_HEAP_ALLOCATION_FAILED,
    ERR_RESIZE_FAILED,
    ERR_INVALID_SLICE_RANGE
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
} VectorInt_ReturnStatus;



#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))


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


static inline VectorInt_ReturnStatus _reallocate_VectorInt(VectorInt *vector, size_t new_capacity) {
    if (vector == NULL) { 
         return (VectorInt_ReturnStatus){
            .success = false, 
            .error = ERR_NULL_POINTER
        };
    };

    if (new_capacity == 0) {
        return (VectorInt_ReturnStatus){
            .success = false, 
            .error = ERR_INVALID_LENGTH
        };
    }

    if (new_capacity < vector->size) {
        return (VectorInt_ReturnStatus){
            .success = false, 
            .error = ERR_RESIZE_FAILED
        };
    }

    int *temp = realloc(vector->data,sizeof(int)*new_capacity);

    if (temp == NULL)
    {
        return (VectorInt_ReturnStatus){
            .success = false, 
            .error = ERR_HEAP_ALLOCATION_FAILED
        };
    }

    vector->capacity = new_capacity;
    vector->data = temp;

    return (VectorInt_ReturnStatus){
        .success = true, 
        .error = ERR_NONE
    };
}


VectorInt_ReturnStatus insert_VectorInt(VectorInt *vector, int value) {

    if (vector->size >= vector->capacity) {
        VectorInt_ReturnStatus result = _reallocate_VectorInt(vector,vector->capacity*2);

        if (!result.success)
        {
            return result;
        } 
        
    }
    vector->data[vector->size] = value;
    vector->size++;
    return (VectorInt_ReturnStatus){
        .success = true, 
        .error = ERR_NONE
    };

}

VectorInt_ReturnStatus mutiple_insert_VectorInt(VectorInt *vector, size_t array_size, int *values) {

    if (vector->size+array_size >= vector->capacity) {
        size_t needed = MAX(vector->capacity*2,(array_size+vector->size)*2);
        VectorInt_ReturnStatus result = _reallocate_VectorInt(vector,needed);

        if (!result.success)
        {
            return result;
        } 
        
    }

    memcpy(vector->data+vector->size,values,array_size * sizeof(int));
    vector->size += array_size;
    return (VectorInt_ReturnStatus){
        .success = true, 
        .error = ERR_NONE
    };

}


VectorInt_ReturnStatus resize_VectorInt(VectorInt *vector, size_t size) {

    return _reallocate_VectorInt(vector,size);
}


VectorInt_ValueResult get_index(VectorInt *vector, size_t index) {
    if (vector == NULL) { 
         return (VectorInt_ValueResult){
            .success = false, 
            .data.error = ERR_NULL_POINTER
        };
    };

    if (vector->size == 0) { 
         return (VectorInt_ValueResult){
            .success = false, 
            .data.error = ERR_EMPTY_VECTOR
        };
    };

    if (index >= vector->size) {
        return (VectorInt_ValueResult){
            .success = false, 
            .data.error = ERR_OUT_OF_BOUNDS
        };
    }

    return (VectorInt_ValueResult){
        .success = true, 
        .data.value = vector->data[index]
    };
}

VectorInt_SliceResult slice_index_VectorInt(VectorInt *vector, size_t start, size_t end) {
    if (vector == NULL) { 
         return (VectorInt_SliceResult){
            .success = false, 
            .data.error = ERR_NULL_POINTER
        };
    };

    if (vector->size == 0) { 
         return (VectorInt_SliceResult){
            .success = false, 
            .data.error = ERR_EMPTY_VECTOR
        };
    };

    if (start >= vector->size) {
        return (VectorInt_SliceResult){
            .success = false, 
            .data.error = ERR_OUT_OF_BOUNDS
        };
    }

    if (end >= vector->size) {
        return (VectorInt_SliceResult){
            .success = false, 
            .data.error = ERR_OUT_OF_BOUNDS
        };
    }

    if (start > end) {
        return (VectorInt_SliceResult){
            .success = false, 
            .data.error = ERR_INVALID_SLICE_RANGE
        };
    }

    size_t len = (end - start + 1);

    VectorInt_Slice slice = {
        .ptr = vector->data+start,
        .len = len
    };
    
    return (VectorInt_SliceResult){
        .success = true, 
        .data.slice = slice
    };
}

VectorInt_ReturnStatus free_VectorInt(VectorInt *vector) {
    if (vector == NULL) { 
         return (VectorInt_ReturnStatus){
            .success = false, 
            .error = ERR_NULL_POINTER
        };
    };

    free(vector->data);     
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    
    return (VectorInt_ReturnStatus){
        .success = true, 
        .error = ERR_NONE
    };
}



const char* printerror_VectorInt(VectorInt_ErrorCode error) {
    switch (error) {
        case ERR_NONE:                    return "ERR_NONE";
        case ERR_NOT_FOUND:               return "ERR_NOT_FOUND";
        case ERR_OUT_OF_BOUNDS:           return "ERR_OUT_OF_BOUNDS";
        case ERR_EMPTY_VECTOR:            return "ERR_EMPTY_VECTOR";
        case ERR_NULL_POINTER:            return "ERR_NULL_POINTER";
        case ERR_INVALID_LENGTH:          return "ERR_INVALID_LENGTH";
        case ERR_HEAP_ALLOCATION_FAILED:  return "ERR_HEAP_ALLOCATION_FAILED";
        case ERR_RESIZE_FAILED:           return "ERR_RESIZE_FAILED";
        case ERR_INVALID_SLICE_RANGE:     return "ERR_INVALID_SLICE_RANGE";
        default:                          return "UNKNOWN_ERROR";
    }
}

void _internal_quicksort_VectorInt(VectorInt *vector,size_t start, size_t end) {

    if (start >= end) return;

    int pivot = vector->data[start];
    size_t i  = start;
    size_t j = end;

    while (true)
    {
        while(vector->data[i] < pivot) i++;
        while(vector->data[j] > pivot) j--;

        if (i >= j)
        {
            pivot = vector->data[j];
            
            break;
            
        }

        int temp = vector->data[i];
        vector->data[i] = vector->data[j];
        vector->data[j] = temp;

        i++;
        if (j>0)j--;
        
    }

    _internal_quicksort_VectorInt(vector, start, j);
    _internal_quicksort_VectorInt(vector, j + 1, end);
    
}
    

VectorInt_ReturnStatus quicksort_VectorInt(VectorInt *vector) {
    if (vector == NULL) { 
        return (VectorInt_ReturnStatus){
            .success = false, 
            .error = ERR_NULL_POINTER
        };
    };


    _internal_quicksort_VectorInt(vector,0,vector->size-1);



    return (VectorInt_ReturnStatus){
        .success = true, 
        .error = ERR_NONE
    };
}

