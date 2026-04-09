#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "vector.h"
#include "print.h"
#include "expect.h"

bool test_init() {
    Vector my_arr;
    EXPECT_EQUAL(init_Vector(&my_arr),true);

    EXPECT_EQUAL(my_arr.size,16);
    EXPECT_EQUAL(my_arr.capacity,16);
    return true;
}

bool test_allocation() {
    Vector my_arr;
    EXPECT_EQUAL(allocate_Vector(&my_arr,30),true);

    EXPECT_EQUAL(my_arr.size,0);
    EXPECT_EQUAL(my_arr.capacity,30);
    return true;
}

bool test_init_with_values() {
    Vector my_arr;
    int staticarr[] = {1,2,3,4,5,6};
    EXPECT_EQUAL(ARRAY_LEN(staticarr),6);

    EXPECT_EQUAL(init_Vector_with_values(&my_arr,staticarr,ARRAY_LEN(staticarr)),true);

    EXPECT_EQUAL(my_arr.size,6);
    EXPECT_EQUAL(my_arr.capacity,12);

    for (size_t i = 0; i < my_arr.size; i++)
    {
        EXPECT_EQUAL(my_arr.data[i],staticarr[i]);
    }
    return true;
}

void run_vector_tests() {
    if(test_init()) PRINTSTRING("Init test sucess"); 
    if(test_allocation()) PRINTSTRING("Allocation test sucess");;
    if(test_init_with_values()) PRINTSTRING("Init with values test sucess");;
}