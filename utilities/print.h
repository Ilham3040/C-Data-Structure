#include <stdio.h>

#define PRINTUNSIGNED(x) (printf("%u \n",x))
#define PRINTBOOLEAN(x) ((x) == 1 ? printf("True \n") : printf("False \n"));
#define PRINTINTEGER(x) (printf("%i \n",x))
#define PRINTSPACE() (printf("\n"))
#define PRINTSIZE(x) (printf("%zu \n",x))
#define PRINTSTRING(x) (printf("%s \n",x))