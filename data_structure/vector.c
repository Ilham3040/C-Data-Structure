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

#define CHECKNULLPTR(ptr) \
    if ((ptr) == NULL) { \
        fprintf(stderr, "Error: %s is NULL at %s:%d\n", #ptr, __FILE__, __LINE__); \
        return false; \
    } 

#define CHECKLENGTH(x) \
    if ((x) == 0) { \
        fprintf(stderr, "Error: length cannot be zero or negative at %s:%d\n", __FILE__, __LINE__); \
        return false; \
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



static inline bool _internal_VectorInt_allocation(VectorInt *vector, size_t len) {
    CHECKLENGTH(len);

    VectorInt temp = { .data = NULL, .size = 0, .capacity = len };

    temp.data = (int *)malloc(sizeof(int) * len);
    if (temp.data == NULL) {
        return false;
    }

    *vector = temp;
    return true;
}

static inline bool _reallocate_VectorInt(VectorInt *vector, size_t new_capacity) {
    CHECKNULLPTR(vector);
    CHECKLENGTH(new_capacity);
    CHECKRESIZE(new_capacity,vector->size);

    int *temp = realloc(vector->data,sizeof(int)*new_capacity);
    if (temp == NULL)
    {
        return false;
    }

    vector->capacity = new_capacity;
    vector->data = temp;

    return true;
}





bool init_VectorInt(VectorInt *vector) {
    return _internal_VectorInt_allocation(vector,16);
    
}

bool allocate_VectorInt(VectorInt *vector, size_t length) {
    return _internal_VectorInt_allocation(vector,length);
}

bool init_VectorInt_with_values(VectorInt *vector, int *values, size_t array_size) {

    if (_internal_VectorInt_allocation(vector,array_size*2))
    {
        memcpy(vector->data,values,array_size*sizeof(int));
        vector->size = array_size;
        return true;
    }
    
    return false;

}



bool insert_VectorInt(VectorInt *vector, int value) {

    if (vector->size >= vector->capacity) {
        if (!_reallocate_VectorInt(vector,vector->capacity*2))
        {
            return false;
        }
        
    }

    vector->data[vector->size] = value;
    vector->size++;
    return true;

}

bool mutiple_insert_VectorInt(VectorInt *vector, size_t array_size, int *values) {

    if (vector->size+array_size >= vector->capacity) {
        size_t needed = MAX(vector->capacity*2,(array_size+vector->size)*2);
        if (!_reallocate_VectorInt(vector,needed)) {
            return false;
        }
        
    }

    memcpy(vector->data+vector->size,values,array_size * sizeof(int));
    vector->size += array_size;
    return true;

}


bool resize_VectorInt(VectorInt *vector, size_t size) {

    return _reallocate_VectorInt(vector,size);
}


bool get_index_VectorInt(VectorInt *vector, int *dest, size_t index) {
    CHECKNULLPTR(vector);
    CHECKNULLPTR(dest);
    CHECKINDEX(index,vector->size);

    *dest = vector->data[index];
    return true;
}

bool slice_index_VectorInt(VectorInt *vector, VectorInt *dest, size_t start, size_t end) {
    CHECKNULLPTR(vector);
    CHECKNULLPTR(dest)
    CHECKINDEX(start,vector->size);
    CHECKINDEX(end,vector->size);

    size_t len = (end - start + 1);

    if (allocate_VectorInt(dest,len))
    {
        memcpy(dest->data,vector->data+start, len * sizeof(int));
        return true;
    }
    
    return false;
}

bool free_VectorInt(VectorInt *vector) {
    CHECKNULLPTR(vector);

    free(vector->data);     
    vector->data = NULL;
    vector->size = 0;       
    vector->capacity = 0;
    
    return true;
}