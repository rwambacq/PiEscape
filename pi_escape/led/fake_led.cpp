#include "ledcontrol.h"
#include "fake_led.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string>
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include <bitset>
#include <iterator>
#include "../../util/sleep.h"
#include "../../pi_escape/led/ledcontrol.h"

using namespace std;

int aantal_bmp = 0;
int getalx = 0;
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

int toonLed(int geval) {

	string c;
	SPGM_RGBTRIPLE g = { 250,0,0 };
	SPGM_RGBTRIPLE bl = { 0,0,0 };
	SPGM_RGBTRIPLE zl = { 0,0,250 };
	//getal = 0;

	SPGM_RGBTRIPLE gridzwart[8][8];

	if (geval == 0) {
		c = "Start Tutorial";
		SPGM_RGBTRIPLE grid[8][8] = {
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,zl,zl,zl,zl,bl,bl } };
		for (int i = 0; i < 8; i++) {
			for (int x = 0; x < 8; x++) {
				gridzwart[i][x] = grid[i][x];
			}
		}

	}
	else if (geval == 1) {
		c = "Start Game";
		SPGM_RGBTRIPLE grid[8][8] = {
			{ bl,bl,bl,zl,zl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,zl,zl,bl,bl,bl } };
		for (int i = 0; i < 8; i++) {
			for (int x = 0; x < 8; x++) {
				gridzwart[i][x] = grid[i][x];
			}
		}
	}
	else {
		c = "Exit";
		SPGM_RGBTRIPLE grid[8][8] = {
			{ bl,bl,zl,zl,zl,zl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl },
			{ bl,bl,bl,bl,bl,bl,bl,bl } };
		for (int i = 0; i < 8; i++) {
			for (int x = 0; x < 8; x++) {
				gridzwart[i][x] = grid[i][x];
			}
		}
	}


	unsigned int** arrays = new unsigned int*[79];

	//lezen font

	std::ifstream is("pi_escape/led/TinyFont", std::ifstream::binary);
	if (is) {
		int x = 0;
		unsigned char * buffer = new unsigned char[3];
		while (x < 237) {
			is.read((char*)buffer, 3);
			unsigned int *toevoeg = new unsigned int[2];
			toevoeg[0] = (unsigned int)buffer[1];
			toevoeg[1] = (unsigned int)buffer[2];
			arrays[(unsigned int)buffer[0] - 48] = toevoeg;
			x += 3;
		}
		delete[] buffer;
		is.close();
	}


	//opvullen voor letter

	for (std::string::iterator it = c.begin(); it != c.end(); ++it) {
		
		if (getalx == 1) {
			return 0;
		}else{
		
		SPGM_RGBTRIPLE gridgroen[8][4];

		if (*it == ' ') {

			SPGM_RGBTRIPLE gridgoen[8][4] = {
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl },
				{ bl,bl,bl,bl } };

			for (int i = 0; i < 8; i++) {
				for (int x = 0; x < 4; x++) {
					gridgroen[i][x] = gridgoen[i][x];
				}
			}
		}
		else {
			unsigned int xx = arrays[((int)*it) - 48][1];
			unsigned int cc = arrays[((int)*it) - 48][0];

			std::string binary = std::bitset<8>(xx).to_string(); //to binary
			std::string binary2 = std::bitset<8>(cc).to_string(); //to binary
			string totaal = binary.substr(1, 8) + binary2;



			for (int y = 0; y < totaal.length(); y += 3) {
			}
			int i = 0;

			for (int x = 0; x < 4; x++) {
				gridgroen[i][x] = bl;
			}
			i++;

			if (binary.substr(0, 1) == "1") {
				for (int x = 0; x < 4; x++) {
					gridgroen[i][x] = bl;
				}
				i++;
			}

			for (int y = 0; y < totaal.length(); y += 3) {
				int xy = i;
				int x = 0;
				string str = totaal.substr(y, 3);
				for (std::string::iterator it = str.begin(); it != str.end(); ++it) {

					if (*it == '1') {
						gridgroen[i][x] = g;
					}
					else {
						gridgroen[i][x] = bl;
					}
					x += 1;
				}

				int yx = i;
				for (x; x < 4; x++) {
					gridgroen[i][x] = bl;
				}
				i++;
			}

			int iy = i;
			for (int y = 0; y < 4; y++) {
				gridgroen[i][y] = bl;
			}
			i++;

			if (binary.substr(0, 1) == "0") {
				for (int y = 0; y < 4; y++) {
					gridgroen[i][y] = bl;
				}
			}

		}
		showColor2(gridzwart);
		//aantal keer

		//while komt hierrond

		for (int x = 0; x < 4; x++) {
			//shiften
			for (int y = 1; y < 7; y++) {
				for (int i = 0; i < 8 - 1; i++)
				{
					gridzwart[y][i] = gridzwart[y][i + 1];
				}
				gridzwart[y][7] = gridgroen[y][x];
			}

			showColor2(gridzwart);
			sleep_ms(200);

		}
		}
	}


	if (getalx == 1) {
		return 0;
	}

	//opkuis van led
	for (int x = 0; x < 8; x++) {
		//shiften
		for (int y = 1; y < 7; y++) {
			for (int i = 0; i < 8 - 1; i++)
			{
				gridzwart[y][i] = gridzwart[y][i + 1];
			}
			gridzwart[y][7] = bl;
		}
		showColor2(gridzwart);
		sleep_ms(200);
	}
	return 0;
}

void changegetal(int getal) {
	
	std::thread first(aanroeper, getal);
	first.detach();
	
}


void aanroeper(int geval) {
	getalx = 1;
	sleep_ms(1000);
	getalx = 0;
	 while (getalx==0){
		 toonLed(geval);
	}
}