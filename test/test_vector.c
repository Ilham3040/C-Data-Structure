#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "vectorint.h"
#include "expect.h"

bool test_init() {

    VectorInt_InitResult result = init_VectorInt();

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;


    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,16);
    return true;
}

bool test_allocation() {

    VectorInt_InitResult result = allocate_VectorInt(30);

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;


    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,30);
    return true;
}

bool test_init_with_values() {

    int staticarr[] = {1,2,3,4,5,6};

    VectorInt_InitResult result = init_VectorInt_with_values(staticarr,ARRAY_LEN(staticarr));

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;


    EXPECT_EQUAL(my_arr.size,ARRAY_LEN(staticarr));
    EXPECT_EQUAL(my_arr.capacity, ARRAY_LEN(staticarr)*2);

    for (size_t i = 0; i < my_arr.size; i++)
    {
        EXPECT_EQUAL(my_arr.data[i],staticarr[i]);
    }

    return true;
}

bool test_insert() {
    VectorInt_InitResult result = allocate_VectorInt(10);

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;

    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,10);

    VectorInt_ReturnStatus insertResult = insert_VectorInt(&my_arr,13);

    EXPECT_EQUAL(insertResult.success,true);

    EXPECT_EQUAL(my_arr.size,1);
    EXPECT_EQUAL(my_arr.capacity,10);

    EXPECT_EQUAL(my_arr.data[0],13);

    return true;
}

bool test_multiple_insert() {
    VectorInt_InitResult result = allocate_VectorInt(4);

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);

    VectorInt my_arr = result.data.vector;

    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,4);

    int staticarr[] = {1,2,3,4,5,6};

    VectorInt_ReturnStatus insertResult = mutiple_insert_VectorInt(&my_arr,ARRAY_LEN(staticarr),staticarr);

    EXPECT_EQUAL(insertResult.success,true);

    EXPECT_EQUAL(my_arr.size,6);
    EXPECT_EQUAL(my_arr.capacity,12);

    for (size_t i = 0; i < my_arr.size; i++)
    {
        EXPECT_EQUAL(my_arr.data[i],staticarr[i]);
    }

    return true;
}

bool test_resizing() {
    VectorInt_InitResult result = allocate_VectorInt(4);

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);

    VectorInt my_arr = result.data.vector;

    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,4);


    VectorInt_ReturnStatus insertResult = resize_VectorInt(&my_arr,49);
    EXPECT_EQUAL(insertResult.success,true);

    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,49);
    return true;
}

bool test_get_index() {
    VectorInt_InitResult result = allocate_VectorInt(4);

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;
    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,4);

    VectorInt_ReturnStatus insertResult = insert_VectorInt(&my_arr,13);

    EXPECT_EQUAL(insertResult.success,true);

    EXPECT_EQUAL(my_arr.size,1);
    EXPECT_EQUAL(my_arr.capacity,4);

    VectorInt_ValueResult getindexResult = get_index(&my_arr,0);

    EXPECT_EQUAL(getindexResult.success,true);
    EXPECT_EQUAL(getindexResult.data.value,13);

    return true;
}

bool test_get_slice() {
    int staticarr[] = {1,2,3,4,5,6};

    VectorInt_InitResult result = init_VectorInt_with_values(staticarr,ARRAY_LEN(staticarr));

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;

    VectorInt_SliceResult sliceResult = slice_index_VectorInt(&my_arr,0,3);
    EXPECT_EQUAL(sliceResult.success,true);

    VectorInt_Slice slice = sliceResult.data.slice;


    EXPECT_EQUAL(my_arr.size,ARRAY_LEN(staticarr));
    EXPECT_EQUAL(my_arr.capacity, ARRAY_LEN(staticarr)*2);

    EXPECT_EQUAL(slice.len,4);
    EXPECT_EQUAL(slice.ptr, my_arr.data+0);

    for (size_t i = slice.len; i < my_arr.size; i++)
    {
        EXPECT_EQUAL(my_arr.data[i+0],slice.ptr[i]);
    }
    
    return true;
}

bool test_free() {
    VectorInt_InitResult result = init_VectorInt();

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;


    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,16);

    VectorInt_ReturnStatus freeResult = free_VectorInt(&my_arr);

    EXPECT_EQUAL(freeResult.success,true);

    EXPECT_EQUAL(my_arr.data, NULL);
    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,0);


    return true;
}

bool test_quick_sort() {
    int staticarr[] = {3,5,6,2,1,4};
    int sortedarr[] = {1,2,3,4,5,6};

    VectorInt_InitResult result = init_VectorInt_with_values(staticarr,ARRAY_LEN(staticarr));

    if (!result.success)
    {
        printf("%s",printerror_VectorInt(result.data.error));
        return false;
    }

    EXPECT_EQUAL(result.success,true);
    VectorInt my_arr = result.data.vector;

    EXPECT_EQUAL(my_arr.size,ARRAY_LEN(staticarr));
    EXPECT_EQUAL(my_arr.capacity, ARRAY_LEN(staticarr)*2);

    VectorInt_ReturnStatus sortResult = quicksort_VectorInt(&my_arr);
    EXPECT_EQUAL(sortResult.success,true);

    for (size_t i = 0; i < my_arr.size; i++)
    {
        EXPECT_EQUAL(my_arr.data[i],sortedarr[i]);
        printf("%i \n",my_arr.data[i]);
    }

    
    return true;
}



void run_vector_tests() {
    if(test_init()) printf("Init test success\n"); 
    if(test_allocation()) printf("Allocation test success\n");
    if(test_init_with_values()) printf("Init with values test success\n");
    if(test_insert()) printf("Insert test success\n");
    if(test_multiple_insert()) printf("Multiple insert test success\n");
    if(test_resizing()) printf("Resizing test success\n");
    if(test_get_index()) printf("Get index test successful\n");
    if(test_get_slice()) printf("Get slice test success\n");
    if(test_free()) printf("Free test success\n");
    if(test_quick_sort()) printf("Quick Sort test success\n");
}