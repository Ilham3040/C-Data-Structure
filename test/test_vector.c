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
    return true;

    for (size_t i = 0; i < my_arr.size; i++)
    {
        EXPECT_EQUAL(my_arr.data[i],staticarr[i]);
    }
    return true;
}

void run_vector_tests() {
    if(test_init()) printf("Init test sucess\n"); 
    if(test_allocation()) printf("Allocation test sucess\n");
    if(test_init_with_values()) printf("Init with values test sucess\n");
}