#include <stm32f031x6.h>
#include "font5x7.h"
#include "display.h"

void test(const uint16_t *image)
{
    putImage(20, 40, 16, 16, image, 0, 0);
}