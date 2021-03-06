/**
 * @autor: shurinov;
 * 
 **/
 
#include "keep_logo.h"
#include <stdlib.h>
#include "main.h"


typedef struct {
  int16_t x;
  int16_t y;
}point_t;

typedef struct {
  int nodesNumb;
  int16_t *arrX;
  int16_t *arrY;
} brLine_t;


uint8_t SIN_TAB_8BIT [] = {0, 2, 3, 5, 6, 8, 9, 11, 13, 14, 16, 17, 19, 20, 22, 24, 25, 27, 28, 30, 31, 33,
34, 36, 38, 39, 41, 42, 44, 45, 47, 48, 50, 51, 53, 55, 56, 58, 59, 61, 62, 64, 65,
67, 68, 70, 71, 73, 74, 76, 77, 79, 80, 82, 83, 85, 86, 88, 89, 91, 92, 94, 95, 96,
98, 99, 101, 102, 104, 105, 107, 108, 109, 111, 112, 114, 115, 116, 118, 119, 121, 122,
123, 125, 126, 127, 129, 130, 132, 133, 134, 136, 137, 138, 140, 141, 142, 143, 145, 146,
147, 149, 150, 151, 152, 154, 155, 156, 157, 159, 160, 161, 162, 164, 165, 166, 167, 168,
169, 171, 172, 173, 174, 175, 176, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 
190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 203, 204, 205, 206,
207, 208, 209, 210, 211, 212, 213, 213, 214, 215, 216, 217, 218, 218, 219, 220, 221, 222,
222, 223, 224, 225, 225, 226, 227, 228, 228, 229, 230, 230, 231, 232, 232, 233, 234, 234,
235, 235, 236, 237, 237, 238, 238, 239, 239, 240, 241, 241, 242, 242, 243, 243, 243, 244,
244, 245, 245, 246, 246, 247, 247, 247, 248, 248, 248, 249, 249, 249, 250, 250, 250, 251,
251, 251, 251, 252, 252, 252, 252, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254,
254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};

int16_t keepX[] = {-92, -92, -76, -76, -92, -92, -76, -76, -60, -60, -44, -44, -28, -28, -12, -12, -28,
    -28, -20, 36, 12, 12, 92, 92, 84, 16, 84, 92, 92, 12, 12, 36, -20, -28, -28, -12, -12, -92};
int16_t keepY[] = {-104, -64, -64, 64, 64, 104, 104, 88, 88, 104, 104, 88, 88, 104, 104, 64, 64, 16, 16,
    64, 64, 104, 104, 64, 64, 4.0, -64, -64, -104, -104, -64, -64, -8, -8, -64, -64, -104, -104};

int16_t PFK0_X [] = {96, 0, -95};//{98, 0, -98};
int16_t PFK0_Y [] = {14, 112, 14};

int16_t PFK1_X [] = { -28, 0, -42, 42, 0, 28};
int16_t PFK1_Y [] = { -56, -28, -112, -112, -28, -56};

int16_t T_X [] = {-63, -63, -7, -7, 7, 7, 63, 63, -63};
int16_t T_Y [] = {42, 28, 28, -42, -42, 28, 28, 42, 42};

brLine_t figureKEEP = {sizeof(keepX)/sizeof(keepX[0]), keepX, keepY};
point_t offset = {0, 0};
uint16_t angle = 0;
uint16_t angleSec = 0;
int16_t scaleFactor = 0;
void * workBuffX;
void * workBuffY;


int16_t getSinValue(uint16_t angle);
int16_t getCosValue(uint16_t angle);
void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void drawArc(point_t center, int16_t rad, uint16_t startAng, uint16_t finishAng, uint8_t speed);
void drawArcCurve(point_t center, int16_t rad, uint16_t startAng, uint16_t finishAng, uint8_t speed, uint16_t curveOffs);
void drawBrLine(brLine_t inst, point_t offset, int16_t scale);
void rotateFigure(brLine_t inst, int16_t rX, int16_t rY, int16_t * outX, int16_t * outY);


void initKeep(void)
{   
  workBuffX = malloc(sizeof(keepX));
  workBuffY = malloc(sizeof(keepX));
}


int dreifCnt = 0;

void lissajousDemo(void)
{  
  int16_t x = getSinValue(angle)*7;
  int16_t y = getSinValue(angleSec)*7;
  angle += 2;
  angleSec += 5;

  setPixel(x, y);

  dreifCnt++;
  if (dreifCnt > 0x01FF)
  {
    dreifCnt = 0;
    angleSec += 1;
  }
}


void drawKeep(void)
{  
  offset.x = 0;
  offset.y = 0;
  drawBrLine(figureKEEP, offset, 16);
}


void drawKeepResize(void)
{
  drawBrLine(figureKEEP, offset, scaleFactor/10);
  
  scaleFactor++;
  if (scaleFactor > 200)
    scaleFactor = 0;    
}


void drawKeepRot(void)
{
  angle+= 3;
  offset.x = 0;
  offset.y = 0;
  int16_t rotX = getSinValue(angle);
  int16_t rotY = getSinValue(angle+256);  
  brLine_t figRotKEEP = {figureKEEP.nodesNumb, workBuffX, workBuffY};
  rotateFigure(figureKEEP, rotX, rotY, workBuffX, workBuffY);
  drawBrLine(figRotKEEP, offset, 16);    
}
  

void drawPFK(void)
{
  brLine_t PFK0 = {sizeof(PFK0_X)/sizeof(PFK0_X[0]), PFK0_X, PFK0_Y};
  brLine_t PFK1 = {sizeof(PFK1_X)/sizeof(PFK1_X[0]), PFK1_X, PFK1_Y};
  point_t offset;
  
  offset.x = 0;
  offset.y = 0;
  drawBrLine(PFK0, offset, 16);

  offset.x = -1010;
  offset.y = -350;  
  drawArc(offset,0x300,(0x00FF + 0x007F),(0x02FF + 0x007F),4);

  offset.x = 0;
  offset.y = 0;
  drawBrLine(PFK1, offset, 16);

  offset.x = 1010;
  offset.y = -350;  
  drawArc(offset,0x300,(0x01FF + 0x007F),(0x03FF + 0x007F),4);

  // K letter
  offset.x = 0;
  offset.y = 350;  
  drawBrLine(figureKEEP, offset, 5);
}


void drawTBTC(void)
{
  brLine_t tFig;
  tFig.nodesNumb = sizeof(T_X)/sizeof(T_X[0]);
  tFig.arrX = T_X;
  tFig.arrY = T_Y;

  offset.x = 0;
  offset.y = 0;  
   
  angle+=10;//40;
  drawArcCurve(offset, 0x67F, 0, 0x400, 3, angle);  
     
  drawArc(offset, 0x47F, 0x000, 0x400, 10);

  offset.x = 0;
  offset.y = -200; 
  drawBrLine(tFig, offset, 12);    

  offset.x = 0;
  offset.y = 700; 

  drawArc(offset, 0x7F, 0x000, 0x400, 50);
}


void drawArc(point_t center, int16_t rad, uint16_t startAng, uint16_t finishAng, uint8_t speed)
{
  point_t point;
  if (startAng < finishAng)
  {
    while (startAng <= finishAng)
    {
      point.x = center.x + (getCosValue(startAng)*rad)/256 ;
      point.y = center.y + (getSinValue(startAng)*rad)/256 ;
      setPixel(point.x, point.y);

      startAng += speed;
    } 
  }
  else 
  {
    while (startAng >= finishAng)
    {
      point.x = center.x + (getCosValue(startAng)*rad)/256;
      point.y = center.y + (getSinValue(startAng)*rad)/256;
      setPixel(point.x, point.y);
      startAng -= speed;
    } 
  }
}


void drawArcCurve(point_t center, int16_t rad, uint16_t startAng, uint16_t finishAng, uint8_t speed, uint16_t curveOffs)
{
  point_t point;
  int16_t radius;
  if (startAng < finishAng)
  {
    while (startAng <= finishAng)
    {      
      radius = rad + getCosValue((startAng << 4) + curveOffs);
      point.x = center.x + (getCosValue(startAng)*radius)/256 ;
      point.y = center.y + (getSinValue(startAng)*radius)/256 ;
      setPixel(point.x, point.y);
      startAng += speed;
    } 
  }
  else 
  {
    while (startAng >= finishAng)
    {
      radius = rad + getCosValue((startAng << 4) + curveOffs);
      point.x = center.x + (getCosValue(startAng)*radius)/256 ;
      point.y = center.y + (getSinValue(startAng)*radius)/256 ;
      setPixel(point.x, point.y);
      startAng -= speed;
    } 
  }
}


void drawBrLine(brLine_t inst, point_t offset, int16_t scale)
{
  for (int i = 0; i < inst.nodesNumb-1; i++)
  {    
    int16_t x0 = offset.x + *(inst.arrX+i) * scale; //<< 4;
    int16_t y0 = offset.y + *(inst.arrY+i) * scale; //<< 4;
    int16_t x1 = offset.x + *(inst.arrX+i+1) * scale; //<< 4;
    int16_t y1 = offset.y + *(inst.arrY+i+1) * scale;  //<< 4;

    drawLine(x0, y0, x1, y1);
  }
}


void drawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  int16_t dx = x2 - x1;
  int16_t dy = y2 - y1;
  int8_t x_dir = 1;
  int8_t y_dir = 1;
  if (dx < 0)
  {
    x_dir = -1;
    dx = -dx;
  }
  if (dy < 0)
  {
    y_dir = -1;
    dy = -dy;
  }

  int8_t step = 20;
  int16_t pt_x = 0;
  int16_t pt_y = 0;  

  if (dx >= dy)
  {        
    for (int16_t i = 0; i <= dx; i += step)
    {      
      pt_x = x1 + x_dir*i;
      pt_y = y1 + y_dir*i*(dy+1)/(dx+1);
      setPixel(pt_x, pt_y);
    }
  }
  else 
  {
    for (int16_t i = 0; i <= dy; i += step)
    {      
      pt_x = x1 + x_dir*i*(dx+1)/(dy+1);
      pt_y = y1 + y_dir*i;
      setPixel(pt_x, pt_y);
    }
  }
}


int16_t getSinValue(uint16_t angle)
{
  uint16_t signBits = (angle >> 8) & 0x0003;
  if (signBits == 0)
    return (int16_t)((uint16_t)SIN_TAB_8BIT[angle&0x00FF]);
  if (signBits == 1)
    return (int16_t)((uint16_t)SIN_TAB_8BIT[255 - angle&0x00FF]);
  if (signBits == 2)
    return -(int16_t)((uint16_t)SIN_TAB_8BIT[angle & 0x00FF]);
  return -(int16_t)((uint16_t)SIN_TAB_8BIT[255 - angle&0x00FF]);  
}


int16_t getCosValue(uint16_t angle)
{
  return getSinValue(angle + 0x0100);
}


void rotateFigure(brLine_t inst, int16_t rX, int16_t rY, int16_t * outX, int16_t * outY)
{
  int32_t x = rX;
  int32_t y = rY;
  for (int i = 0; i < inst.nodesNumb; i++)
  {
    *(outX+i) = ((int32_t)*(inst.arrX+i) * x - (int32_t)*(inst.arrY+i) * y)/300;
    *(outY+i) = ((int32_t)*(inst.arrX+i) * y + (int32_t)*(inst.arrY+i) * x)/300;
  }
}







