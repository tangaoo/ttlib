
#ifndef _IMAGE_H_
#define _IMAGE_H_

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef long LONG;

#pragma pack(1)
//λͼ�ļ�ͷ
typedef struct tag_bitmap_file_header
{
	WORD   type;            //λͼ�ļ����ͣ���BM��
	DWORD  size;            //λͼ�ļ��ܴ�С�� ��λ�ֽ�
	WORD   reserved1;       //���� 0
	WORD   reserved2;       //���� 0
	DWORD  offset;          //ͷ��λͼ���ݵ�ƫ��������λ�ֽ�
}t_bitmap_file_header, t_bitmap_file_header_p;


//λͼ��Ϣͷ
typedef struct tag_bitmap_info_header
{
	DWORD size;             //λͼ��Ϣͷ��ռ�ֽ���
	LONG  width;            //λͼ����λ����
	LONG  height;           //λͼ�ߣ���λ����
	WORD  planes;           //�豸���𣬱���Ϊ1
	WORD  bit_count;        //ÿ�����ص�λ����1�� 4�� 8�� 24
	DWORD compression;      //ѹ�����ͣ�0��ѹ��
	DWORD img_size;         //λͼ��С
	LONG  xpels_per_meter;  //ˮƽ�ֱ��ʣ�ÿ��������
	LONG  ypels_per_meter;  //��ֱ�ֱ��ʣ�ÿ��������
	DWORD colour_used;      //λͼʵ��ʹ����ɫ���е���ɫ��
	DWORD colour_important; //λͼ��ʾ��������Ҫ����ɫ��
}t_bitmap_info_header, t_bitmap_info_header_p;

//��ɫ��
typedef struct tag_rgb_quad 
{
	BYTE  blue;             //��ɫ����ɫ����
	BYTE  green;            //��ɫ����ɫ����
	BYTE  red;              //��ɫ�ĺ�ɫ����
	BYTE  reserved;         //����
}t_rgb_quad;

//�Ҷ�ͼ�ṹ��
typedef struct tag_grey
{
	int row;                //��������
	int col;                //��������
	unsigned char *matrix_pp; //�Ҷ�ͼ�����
}t_grey;

#pragma pack () 

extern unsigned char grg_to_bmp(void);

#endif // !_IMAGE_H_
