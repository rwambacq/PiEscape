#include <stdio.h>
unsigned char header[14];
unsigned char dipheader[40];
unsigned char colorzelf[36];

void display_ledgrid(SPGM_RGBTRIPLE ledgrid[8][8], const char* framebuffer) {
	maakFake(ledgrid, 8, 8);
}