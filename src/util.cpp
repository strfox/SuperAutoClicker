#include "util.h"

#include <math.h>

unsigned int digitsInNumber(unsigned int n)
{
    if (n == 0) {
        return 1;
    }
    return static_cast<unsigned int>( floor( 1 + log10(n) ) );
}
