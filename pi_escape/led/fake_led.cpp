#include "fake_led.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

int aantal_bmp = 0;
unsigned char header[14];
unsigned char dipheader[40];
unsigned char *colorzelf;


extern "C" int maakFake(SPGM_RGBTRIPLE ledgrid[8][8], int hoogte, int breedte);


int maakFake(SPGM_RGBTRIPLE ledgrid[8][8], int hoogte, int breedte) {


	FILE *bmpfile;

	//fill in header
	//bftype
	header[0] = 'B';
	header[1] = 'M';

	header[2] = (unsigned char)(54 + (hoogte*breedte * 3));

	//de rest , dus rest van bfsized, bfreserved worden op 0 ingesteld
	for (int i = 3; i < 10; i++) {
		header[i] = 0;
	}

	//bfOffbits
	header[10] = (unsigned char)(54);
	for (int i = 11; i < 13; i++) {
		header[i] = 0;
	}


	//fill in dipheader
	//bisize
	dipheader[0] = (unsigned char)40;
	for (int i = 1; i < 4; i++) {
		dipheader[i] = 0;
	}
	//biwidth
	dipheader[4] = breedte;
	for (int i = 5; i < 8; i++) {
		dipheader[i] = 0;
	}
	//biheight
	dipheader[8] = hoogte;
	for (int i = 9; i < 12; i++) {
		dipheader[i] = 0;
	}

	//biplanes
	dipheader[12] = 1;
	dipheader[13] = 0;
	//bibitcount
	dipheader[14] = (unsigned char)24;
	dipheader[15] = 0;
	//compression, sizeimage, xpelspetmeter ciclrused en ciclrimportant worden allemaal op 0 ingesteld
	for (int i = 16; i < 40; i++) {
		dipheader[i] = 0;

	}


	const int totaalkleur = (hoogte*breedte * 3);
	unsigned char colorzelf[8][24];

	//invullen kleuren

	for (int i = 0; i < 8; i++) {
		for (int x = 0; x < 8; x++) {
			colorzelf[i][x * 3] = (unsigned char)ledgrid[i][x].rgbBlue;
			colorzelf[i][(x * 3) + 1] = (unsigned char)ledgrid[i][x].rgbGreen;
			colorzelf[i][(x * 3) + 2] = (unsigned char)ledgrid[i][x].rgbRed;
		}
	}

	//maakt string opslagplaats;
	char zin[100];
	char *zin2 = ".bmp";
	char snum[5];

	string deeleen = "BMP/image" + std::to_string(aantal_bmp);
	string dit = deeleen + ".bmp";

	const char * c = dit.c_str();

	bmpfile = fopen(c, "w");



	//writen
	fwrite(header, 1, 14, bmpfile);
	fwrite(dipheader, 1, 40, bmpfile);
	for (int i = 7; i >= 0; i--) {
		fwrite(colorzelf[i], 1, 24, bmpfile);
	}

	fclose(bmpfile);
	aantal_bmp += 1;
}