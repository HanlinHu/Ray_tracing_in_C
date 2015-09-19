#include <stdio.h>

#include "ppm.h"


void write_ppm_file(FILE *output, unsigned char *pixels, int width, int height)
{
	fprintf(output, "P6\n%d %d\n%d\n", width, height, 255);
	fwrite(pixels, 1, height * width * 3, output);
}
