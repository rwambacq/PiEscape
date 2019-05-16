#ifdef RPI
#ifndef HTS221
#define HTS221

#include <stdint.h>
#include "i2c.h"
#include <unistd.h>

int hts221_init(int frequentie);

int hts221_read_humidity();

int hts221_read_temperature();

#endif /* HTS221 */
#endif

