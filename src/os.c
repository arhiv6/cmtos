#include "os.h"

void coverage_test()
{
    if (1)
    {
        return;
    }

    int a = 1;
    int b = 2;

    if (b>a)
    {
        a=b;
    }
}
