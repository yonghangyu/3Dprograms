#include <stdio.h>
#include <stdlib.h>
#include<GLFW/glfw3.h>
#if defined(WIN32)
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  define GL_GLEXT_PROTOTYPES
#  include <GL/glut.h>
#endif

/* 画像解像度 2835 pixel/meter ≒ 72dpi */
#define XRESOLUTION 2835
#define YRESOLUTION 2835

/*
** ウィンドウの内容を BMP 形式で保存
*/
int saveimage(void)
{
  /* BMP/DIB ファイルのヘッダ定義 */
  struct biHeader {
    unsigned char bfType[2];
    unsigned char bfSize[4];
    unsigned char bfReserved1[2];
    unsigned char bfReserved2[2];
    unsigned char bfOffBits[4];
    unsigned char biSize[4];
    unsigned char biWidth[4];
    unsigned char biHeight[4];
    unsigned char biPlanes[2];
    unsigned char biBitCount[2];
    unsigned char biCompression[4];
    unsigned char biSizeImage[4];
    unsigned char biXPelsPerMeter[4];
    unsigned char biYPelsPerMeter[4];
    unsigned char biClrUsed[4];
    unsigned char biClrImportant[4];
  } header;
  
  GLint view[4];
  GLubyte *image;
  size_t size;

  /* 画面表示の完了を待つ */
  glFinish();

  /* 現在のビューポートのサイズを得る */
  glGetIntegerv(GL_VIEWPORT, view);
  
  /* ビューポートのサイズ分のメモリを確保する */
  size = view[2] * view[3] * 3;
  image = (GLubyte *)malloc(size);
  
  if (image) {
    static int count = 0;
    char filename[20];
    FILE *fp;
    
    snprintf(filename, sizeof(filename), "img%05d.bmp", count++);

    fp = fopen(filename, "wb");
    if (fp) {
      long temp;

      glReadPixels(view[0], view[1], view[2], view[3],
		  GL_BGR_EXT, GL_UNSIGNED_BYTE, image);
      
/*
** BMP は Little Endian なので
** 使用マシンの Endian に依存しないように
** 1 byte ずつ設定する
*/

      header.bfType[0] = 'B';
      header.bfType[1] = 'M';

      temp = size + 54;
      header.bfSize[0] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.bfSize[1] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.bfSize[2] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.bfSize[3] = (unsigned char)(temp & 0xff);

      header.bfReserved1[0] = 0;
      header.bfReserved1[1] = 0;
      header.bfReserved2[0] = 0;
      header.bfReserved2[1] = 0;

      header.bfOffBits[0] = 54;
      header.bfOffBits[1] = 0;
      header.bfOffBits[2] = 0;
      header.bfOffBits[3] = 0;

      header.biSize[0] = 40;
      header.biSize[1] = 0;
      header.biSize[2] = 0;
      header.biSize[3] = 0;

      temp = view[2];
      header.biWidth[0] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biWidth[1] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biWidth[2] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biWidth[3] = (unsigned char)(temp & 0xff);

      temp = view[3];
      header.biHeight[0] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biHeight[1] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biHeight[2] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biHeight[3] = (unsigned char)(temp & 0xff);

      header.biPlanes[0] = 1;
      header.biPlanes[1] = 0;

      header.biBitCount[0] = 24;
      header.biBitCount[1] = 0;

      header.biCompression[0] = 0;
      header.biCompression[1] = 0;
      header.biCompression[2] = 0;
      header.biCompression[3] = 0;

      temp = size;
      header.biSizeImage[0] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biSizeImage[1] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biSizeImage[2] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biSizeImage[3] = (unsigned char)(temp & 0xff);

      temp = XRESOLUTION;
      header.biXPelsPerMeter[0] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biXPelsPerMeter[1] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biXPelsPerMeter[2] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biXPelsPerMeter[3] = (unsigned char)(temp & 0xff);

      temp = YRESOLUTION;
      header.biYPelsPerMeter[0] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biYPelsPerMeter[1] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biYPelsPerMeter[2] = (unsigned char)(temp & 0xff);
      temp >>= 8;
      header.biYPelsPerMeter[3] = (unsigned char)(temp & 0xff);

      header.biClrUsed[0] = 0;
      header.biClrUsed[1] = 0;
      header.biClrUsed[2] = 0;
      header.biClrUsed[3] = 0;

      header.biClrImportant[0] = 0;
      header.biClrImportant[1] = 0;
      header.biClrImportant[2] = 0;
      header.biClrImportant[3] = 0;

      fwrite(&header, sizeof(header), 1, fp);
      fwrite(image, size, 1, fp);
      
      fclose(fp);
      
      return 1;
    }
  }
  
  return 0;
}
