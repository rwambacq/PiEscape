#define NUM_WORDS 64
#define FILESIZE (NUM_WORDS * sizeof(uint16_t))
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include "rgb_triple.h"

void delay(int t);

void display_ledgrid(SPGM_RGBTRIPLE ledgrid[8][8], const char* framebuffer) {
	int i;
	int fbfd;
	uint16_t *map;
	uint16_t *p;
	struct fb_fix_screeninfo fix_info;
	fbfd = open(framebuffer, O_RDWR);
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &fix_info) == -1) {
		perror("Error (call to 'ioctl')");
		close(fbfd);
		exit(EXIT_FAILURE);
	}
	if (strcmp(fix_info.id, "RPi-Sense FB") != 0) {
		printf("%s\n", "not found");
		close(fbfd);
		exit(EXIT_FAILURE);
	}


	map =
		mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);

	if (ledgrid == NULL) {
		close(fbfd);
	}
	else {}
	p = map;
	memset(map, 0, FILESIZE);

	for (i = 0; i < 8; i++) {

		for (int x = 0; x < 8; x++) {
			SPGM_RGBTRIPLE kleur = ledgrid[i][x];
			int Uint16_value = (((31 * (kleur.rgbRed + 4)) / 255) << 11) |
				(((63 * (kleur.rgbGreen + 2)) / 255) << 5) |
				((31 * (kleur.rgbBlue + 4)) / 255);
			*(p + (i * 8) + x) = Uint16_value;
		}
	}
}
