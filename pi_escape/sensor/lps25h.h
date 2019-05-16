#ifdef RPI
#ifndef LPS25H
#define LPS25H

#include <stdint.h>
#include "i2c.h"
#include <unistd.h>

int lps25h_init(int frequentie);

int lps25h_read_pressure();

int lps25h_read_temperature();

#endif /* LPS25H */
#endif