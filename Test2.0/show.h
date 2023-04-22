#include <iostream>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>
#include <Windows.h>

//���ڴ�С
const int HEIGHT = 750;
const int WIDTH = 1200;
//���������
POINT CameraPos;

//��Ϸ����
int player_speed = 10;//�����ٶ�
int bullet_speed = 30;//�ӵ��ٶ�
int bullet_number = 2;//�ӵ�����
int bullet_range = 400;//�ӵ�������Χ
using namespace std;






// ����PNGͼ��ȥ͸������
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) //xΪ����ͼƬ��X���꣬yΪY����
{

	// ������ʼ��
	DWORD* dst = GetImageBuffer();    // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ��
	DWORD* draw = GetImageBuffer();
	DWORD* src = GetImageBuffer(picture); //��ȡpicture���Դ�ָ��
	int picture_width = picture->getwidth(); //��ȡpicture�Ŀ��
	int picture_height = picture->getheight(); //��ȡpicture�ĸ߶�
	int graphWidth = getwidth();       //��ȡ��ͼ���Ŀ��
	int graphHeight = getheight();     //��ȡ��ͼ���ĸ߶�
	int dstX = 0;    //���Դ������صĽǱ�

	// ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //���Դ������صĽǱ�
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA��͸����
			int sr = ((src[srcX] & 0xff0000) >> 16); //��ȡRGB���R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //���Դ������صĽǱ�
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //��p=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //��p=sa/255 , FP=sb , BP=db
			}
		}
	}
}

//�����ӵ�
void drawbullet(int real_bullet_x, int real_bullet_y, int camera_x, int camera_y, IMAGE* bullet)
{
	int bullet_x = real_bullet_x - camera_x - 15;
	int bullet_y = real_bullet_y - camera_y - 15;
	if (bullet_x >= 0 && bullet_x <= WIDTH - 30 && bullet_y >= 0 && bullet_y <= HEIGHT - 30)//����30��ֱ�Ӵ����ӵ��Ĵ�С��
	{
		drawAlpha(bullet, bullet_x, bullet_y);
	}
}