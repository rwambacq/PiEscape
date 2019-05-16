#ifdef __cplusplus
extern "C" {
#endif
#ifndef LED_CONTROL_INCLUDED
#define LED_CONTROL_INCLUDED
#include "../../util/rgb_triple.h"
int showColor(int);
int showColor2(SPGM_RGBTRIPLE ledgrid[8][8]);
#endif
#ifdef __cplusplus
}
#endif