
#if 0

#define _CRT_SECURE_NO_WARNINGS

#include "image.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


unsigned char *raw_to_grey8(t_grey * grey_p, int *length_p)
{  
	int i, j, idx;
	unsigned char *grey_buffer_p, *p;
	t_bitmap_file_header file_header;
	t_bitmap_info_header info_header;
	t_rgb_quad quad[256];

	file_header.type = 0x4D42;           //bmp类型
	file_header.size = sizeof(t_bitmap_file_header) + sizeof(t_bitmap_info_header) + 256 * sizeof(t_rgb_quad) + grey_p->row * grey_p->col;
	printf("%d, %d, %d, %d\n", sizeof(t_bitmap_file_header), sizeof(t_bitmap_info_header), 256 * sizeof(t_rgb_quad), grey_p->row * grey_p->col);
	file_header.reserved1 = 0;
	file_header.reserved2 = 0;
	file_header.offset = sizeof(t_bitmap_file_header) + sizeof(t_bitmap_info_header) + 256 * sizeof(t_rgb_quad);

	info_header.size  = sizeof(t_bitmap_info_header);
	info_header.width = grey_p->row;
	info_header.height = grey_p->col;
	info_header.planes = 1;
	info_header.bit_count = 8;
	info_header.compression = 0;
	info_header.img_size = 0;
	info_header.xpels_per_meter = 3780;
	info_header.ypels_per_meter = 3780;
	info_header.colour_used = 256;
	info_header.colour_important = 0;

	for (i=0; i<256; i++)
	{
		quad[i].blue = i;
		quad[i].green = i;
		quad[i].red = i;
		quad[i].reserved = 0;
	}

	grey_buffer_p = malloc(file_header.size);
	if (grey_buffer_p != NULL)
	{
		printf("malloc success, size, %d\n", file_header.size);
	}
	
	idx = 0;
	memcpy(grey_buffer_p + idx, &file_header, sizeof(t_bitmap_file_header));
	idx = sizeof(t_bitmap_file_header);
	memcpy(grey_buffer_p + idx, &info_header, sizeof(t_bitmap_info_header));
	idx += sizeof(t_bitmap_info_header);
	memcpy(grey_buffer_p + idx, &quad, 256 * sizeof(t_rgb_quad));
	idx += 256 * sizeof(t_rgb_quad);

	p = grey_buffer_p + idx;

	for (i = grey_p->col-1; i >= 0; i--)
	{
		for (j = 0; j < grey_p->row; j++)
		{
			*(p++) = *(grey_p->matrix_pp + i*grey_p->row + j);
		}
	}
	*length_p = file_header.size;

	return grey_buffer_p;
}

unsigned char grg_to_bmp(void)
{
	FILE *fdst=NULL, *fsrc=NULL;
	int length, res;
	t_grey grey;
	unsigned char *bmp_buffer, row_str[2], col_str[2];

	memset(&grey, 0, sizeof(t_grey));
	fsrc = fopen("C:\\Users\\tangaoo\\Desktop\\180920160209_1_2.dat", "rb");
	fread(row_str, 2, 1, fsrc);
	fread(col_str, 2, 1, fsrc);
	memcpy(&grey.row, row_str, 2); //todo need opt
	memcpy(&grey.col, col_str, 2);
	printf("row, %d, col, %d", grey.row, grey.col);
	grey.row = 792;
	grey.col = 320;
	grey.matrix_pp = malloc(grey.row * grey.col);
	memset(grey.matrix_pp, 0, grey.row * grey.col);

	res = fread(grey.matrix_pp, 1, grey.row*grey.col, fsrc);
	printf("res, %d, %d\n", res, grey.row * grey.col);

	bmp_buffer = raw_to_grey8(&grey, &length);
	fdst = fopen("C:\\Users\\tangaoo\\Desktop\\180920160209_1_2.bmp", "wb");

	fwrite(bmp_buffer, 1, length, fdst);

	free(bmp_buffer);
	free(grey.matrix_pp);
	fclose(fsrc);
	fclose(fdst);



	return 0;
}
#endif