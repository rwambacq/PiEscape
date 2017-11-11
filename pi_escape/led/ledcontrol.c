#include "ledcontrol.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include "../../util/rgb_triple.h"
#if defined(RPI)
#include "../../util/lampenunix.h"
#else
#include "../../util/lampenwindows.h"
#endif


int showColor(int kleurtje)
{
	SPGM_RGBTRIPLE bl = { 0,0,0 };
	SPGM_RGBTRIPLE g = { 0,250,0 };
	SPGM_RGBTRIPLE r = { 250,0,0 };
	SPGM_RGBTRIPLE b = { 0,0,250 };

	if (kleurtje == 2) {
		SPGM_RGBTRIPLE gridgroen[] =
		{ bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,g,g,g,bl,bl,bl,bl,
			g,bl,bl,g,g,g,g,g,
			g,bl,bl,g,bl,g,bl,g,
			bl,g,g,bl,bl,g,bl,g,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl, };

		display_ledgrid(gridgroen, "/dev/fb1");

	}
	else if (kleurtje == 5) {
		display_ledgrid(NULL, "/dev/fb1");
	}
	else if (kleurtje == 1) {

		SPGM_RGBTRIPLE gridblauw[] =
		{ bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,b,b,b,bl,bl,bl,bl,
			b,bl,bl,b,b,b,b,b,
			b,bl,bl,b,bl,b,bl,b,
			bl,b,b,bl,bl,b,bl,b,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl, };
		display_ledgrid(gridblauw, "/dev/fb1");
	}
	else if (kleurtje == 3) {
		SPGM_RGBTRIPLE gridrood[] =
		{ bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,r,r,r,bl,bl,bl,bl,
			r,bl,bl,r,r,r,r,r,
			r,bl,bl,r,bl,r,bl,r,
			bl,r,r,bl,bl,r,bl,r,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl, };
		display_ledgrid(gridrood, "/dev/fb1");
	}
	else if (kleurtje == 0) {

		char pixels[64][3] = {
			{ 255, 0, 0 },{ 255, 0, 0 },{ 255, 87, 0 },{ 255, 196, 0 },{ 205, 255, 0 },{ 95, 255, 0 },{ 0, 255, 13 },{ 0, 255, 122 },
			{ 255, 0, 0 },{ 255, 96, 0 },{ 255, 205, 0 },{ 196, 255, 0 },{ 87, 255, 0 },{ 0, 255, 22 },{ 0, 255, 131 },{ 0, 255, 240 },
			{ 255, 105, 0 },{ 255, 214, 0 },{ 187, 255, 0 },{ 78, 255, 0 },{ 0, 255, 30 },{ 0, 255, 140 },{ 0, 255, 248 },{ 0, 152, 255 },
			{ 255, 223, 0 },{ 178, 255, 0 },{ 70, 255, 0 },{ 0, 255, 40 },{ 0, 255, 148 },{ 0, 253, 255 },{ 0, 144, 255 },{ 0, 34, 255 },
			{ 170, 255, 0 },{ 61, 255, 0 },{ 0, 255, 48 },{ 0, 255, 157 },{ 0, 243, 255 },{ 0, 134, 255 },{ 0, 26, 255 },{ 83, 0, 255 },
			{ 52, 255, 0 },{ 0, 255, 57 },{ 0, 255, 166 },{ 0, 235, 255 },{ 0, 126, 255 },{ 0, 17, 255 },{ 92, 0, 255 },{ 201, 0, 255 },
			{ 0, 255, 66 },{ 0, 255, 174 },{ 0, 226, 255 },{ 0, 117, 255 },{ 0, 8, 255 },{ 100, 0, 255 },{ 210, 0, 255 },{ 255, 0, 192 },
			{ 0, 255, 183 },{ 0, 217, 255 },{ 0, 109, 255 },{ 0, 0, 255 },{ 110, 0, 255 },{ 218, 0, 255 },{ 255, 0, 183 },{ 255, 0, 74 }
		};

		SPGM_RGBTRIPLE gridRainbow[64];
		for (int i = 0; i < 64; i++) {
			SPGM_RGBTRIPLE regenboog = { pixels[i][0],pixels[i][1],pixels[i][2] };
			gridRainbow[i] = regenboog;
		}
		display_ledgrid(gridRainbow, "/dev/fb1");
	}
	else {

		SPGM_RGBTRIPLE gridzwart[] = {
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl,
			bl,bl,bl,bl,bl,bl,bl,bl };
		display_ledgrid(gridzwart, "/dev/fb1");

	}

}