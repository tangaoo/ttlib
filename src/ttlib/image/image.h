#if 0

#ifndef _IMAGE_H_
#define _IMAGE_H_

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef long LONG;

#pragma pack(1)
//位图文件头
typedef struct tag_bitmap_file_header
{
	WORD   type;            //位图文件类型，“BM”
	DWORD  size;            //位图文件总大小， 单位字节
	WORD   reserved1;       //保留 0
	WORD   reserved2;       //保留 0
	DWORD  offset;          //头到位图数据的偏移量，单位字节
}t_bitmap_file_header, t_bitmap_file_header_p;


//位图信息头
typedef struct tag_bitmap_info_header
{
	DWORD size;             //位图信息头所占字节数
	LONG  width;            //位图宽，单位像素
	LONG  height;           //位图高，单位像素
	WORD  planes;           //设备级别，必须为1
	WORD  bit_count;        //每个像素的位数，1， 4， 8， 24
	DWORD compression;      //压缩类型，0不压缩
	DWORD img_size;         //位图大小
	LONG  xpels_per_meter;  //水平分辨率，每米像素数
	LONG  ypels_per_meter;  //垂直分辨率，每米像素数
	DWORD colour_used;      //位图实际使用颜色表中的颜色数
	DWORD colour_important; //位图显示过程中重要的颜色数
}t_bitmap_info_header, t_bitmap_info_header_p;

//颜色表
typedef struct tag_rgb_quad 
{
	BYTE  blue;             //颜色的蓝色分量
	BYTE  green;            //颜色的绿色分量
	BYTE  red;              //颜色的红色分量
	BYTE  reserved;         //保留
}t_rgb_quad;

//灰度图结构体
typedef struct tag_grey
{
	int row;                //矩阵行数
	int col;                //矩阵列数
	unsigned char *matrix_pp; //灰度图像矩阵
}t_grey;

#pragma pack () 

extern unsigned char grg_to_bmp(void);

#endif // !_IMAGE_H_


#endif