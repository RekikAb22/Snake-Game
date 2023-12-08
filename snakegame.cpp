#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

#define TRUE 1
#define FALSE 0
#define HORIZONTAL 1
#define VERTICAL 2

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

#define SNK_BODY_COLOR GREEN
#define SNK_FACE_COLOR CYAN

#define BGCOLOR BLACK

#define MAXX 50
#define MAXY 40
#define SX 10
#define SY 10
#define WIDTH 10

#define UP_KEY 72 // Up arrow key
#define DOWN_KEY 80 // Down arrow key
#define RIGHT_KEY 77 // Right arrow key
#define LEFT_KEY 75 // Left arrow key
#define PAUSE_KEY 32 // Space Bar Key
#define EXIT_KEY 27 // Escape Key

using namespace std;

int snk_x[100], snk_y[100], length=8, l_k = RIGHT, xf, yf, eat=0, ch;
int chapter=1,level=1;
int isGamePause=FALSE;
long int score=0;
int timedelay=150;
int sXBlock[30]; // starting horizontal block position
int sYBlock[30];// starting vertical block position
int blockDir[30];//direction of block(horizontal or vertical)
int nBlock[30]; // Number of blocks vertically or horizontally
int nblock=0; // total number of blocks in one level

int starsX[200], starsY[200];
int random = rand();
// Function Prototype
void mainMenu();

void initStars()
{
 int sX, sY;
 for(int g=0;g<200;g++)
 {
  sX=rand()%getmaxx();
  sY=rand()%getmaxy();
  starsX[g]=sX;
  starsY[g]=sY;
 }
 
}



void initSnkPosition(int x, int y)
{
 for(int i=length-1;i>=0;i--)
 {
  snk_x[i]=x-i;
  snk_y[i]=y;
 }
 
}

int isSnkHitItself()
{


 for(int i=length-1;i>1;i--)
  if(snk_x[0]==snk_x[i]&&snk_y[0]==snk_y[i])
   return TRUE;
 return FALSE;
}

void drawSnake()
{
 for(int i=length-1;i>0;i--)
 {
  //Body
  setfillstyle(SOLID_FILL,SNK_BODY_COLOR);
  fillellipse((SX+snk_x[i]*WIDTH)+WIDTH/2,(SY+snk_y[i]*WIDTH)+WIDTH/2,WIDTH/2,WIDTH/2);
  

 }

 //Face
 setfillstyle(SOLID_FILL,SNK_FACE_COLOR);
 fillellipse((SX+snk_x[0]*WIDTH)+WIDTH/2,(SY+snk_y[0]*WIDTH)+WIDTH/2,WIDTH/2,WIDTH/2);


}

void moveSnake()
{
 for(int i=length-1;i>0;i--)
    {
  snk_x[i]=snk_x[i-1];
  snk_y[i]=snk_y[i-1];
    }
}

void hideLastPart()
{

 // Hide the last body part

 setcolor(BGCOLOR);
 setfillstyle(SOLID_FILL,BGCOLOR);
 fillellipse((SX+snk_x[length-1]*WIDTH)+WIDTH/2,(SY+snk_y[length-1]*WIDTH)+WIDTH/2,WIDTH/2,WIDTH/2);

}


int isCollide()
{
 if((xf==snk_x[0]) && (yf==snk_y[0]))
  return TRUE;
 return FALSE;
}

int isSnkHitBlock()
{
  for(int i=1;i<=nblock;i++)
  {
   for(int j=0;j<nBlock[i];j++)
   {
    if(blockDir[i]==HORIZONTAL)
    {
     if(snk_x[0]==sXBlock[i]+j && snk_y[0]==sYBlock[i])
      return TRUE;
    }
    else if(blockDir[i]==VERTICAL)
    {
     if(snk_x[0]==sXBlock[i] && snk_y[0]==sYBlock[i]+j)
      return TRUE;
    }
   }
  }


  return FALSE;
}

void checkFoodOnBlock()
{
  for(int i=1;i<=nblock;i++)
  {
   for(int j=0;j<nBlock[i];j++)
   {
    if(blockDir[i]==HORIZONTAL)
    {
     if(xf==sXBlock[i]+j && yf==sYBlock[i])
     {
            xf=20;
            yf=20;
     }
    }
    else if(blockDir[i]==VERTICAL)
    {
     if(xf==sXBlock[i] && yf==sYBlock[i]+j)
     {
      xf=20;
      yf=20;
     }
    }
   }
  }
}

void getFood(void)
{
    static int color;
    if(eat==0)
    {

  xf=rand()%(MAXX-4)+2;
  yf=rand()%(MAXY-4)+2;
  eat=1;
  color= rand()%(13)+2;
  checkFoodOnBlock();
    }


  setcolor(DARKGRAY);
  setfillstyle(SOLID_FILL,color);
  circle((SX+xf*WIDTH)+WIDTH/2,(SY+yf*WIDTH)+WIDTH/2,WIDTH/2);
  floodfill((SX+xf*WIDTH)+WIDTH/2,(SY+yf*WIDTH)+WIDTH/2,DARKGRAY);
  setcolor(BGCOLOR);

    if(isCollide())
    {
  char sData[20];
 // sound(2000);
  delay(30);
  //nosound();
  settextstyle(0,0,0);
  setcolor(0);
  outtextxy(550,170,itoa(score,sData,10));
  score=score+100;
  length++;
  eat=0;
  setcolor(3);
  outtextxy(550,170,itoa(score,sData,10));

    }
    setcolor(BGCOLOR);
}

void text3D(int x, int y, int depth, int bgcolor, int fgcolor, char *caption)
{
 int i;
 setcolor(bgcolor);
 for(i=0;i<depth;i++)
  outtextxy(x+i,y+i,caption);
 setcolor(fgcolor);
 outtextxy(x+i,y+i,caption);

}


void gameOver()
{

 char buffer[20];

  cleardevice();

  settextstyle(0,0,7);
  text3D(100,150,3,3,13,"Game Over");
  settextstyle(0,0,4);
  text3D(180,270,2,3,13,"Score : ");
  text3D(180+textwidth("Score : "),270,2,3,13,itoa(score,buffer,10));
  settextstyle(0,0,2);
  text3D(100,350,2,3,13,"Press any key to continue");
  settextstyle(0,0,0);
  for(int j=0;j<2000;j++)
   putpixel(rand()%(640),rand()%(480),0);

  getch();
  mainMenu();

}


void snakeTouchesWall()
{
 // If snake touches the wall

   if(snk_x[0]==0)
   {
    snk_x[0]=MAXX-3;
   }
   if(snk_x[0]==MAXX-2)
   {
    snk_x[0]=1;
   }
   if(snk_y[0]==0)
   {
    snk_y[0]=MAXY-3;
   }
   if(snk_y[0]==MAXY-2)
   {
    snk_y[0]=1;
   }
}

void keyEvents( int ch)
{
 // Key events

  if(ch==PAUSE_KEY)
  {
   setcolor(0);
   outtextxy(518,270,"PLAYING");
   setcolor(3);
   outtextxy(520,270,"PAUSED");
   while(1)
   {
    if(kbhit())
    {
     int key=getch();
     if(key==PAUSE_KEY)
     {
      setcolor(0);
      outtextxy(520,270,"PAUSED");
      setcolor(3);
      outtextxy(518,270,"PLAYING");
      break;
     }
     else if(key==EXIT_KEY)
      exit(0);
    }
   }
  }
  if(ch==EXIT_KEY)
   gameOver();
  else if(ch==UP_KEY && l_k!=DOWN)
   l_k=UP;
  else if(ch==DOWN_KEY && l_k!=UP)
   l_k=DOWN;
  else if(ch==RIGHT_KEY && l_k!=LEFT)
   l_k=RIGHT;
  else if(ch==LEFT_KEY && l_k!=RIGHT)
   l_k=LEFT;
}

void moveTheSnake()
{
 // move the snake

   if(l_k==RIGHT)
   {
    snk_x[0]++;
   }
   if(l_k==LEFT)
   {
    snk_x[0]--;
   }
   if(l_k==UP)
   {
    snk_y[0]--;
   }
   if(l_k==DOWN)
   {
    snk_y[0]++;
   }
}



void mainScreen()
{
 char sData[20];

 cleardevice();
 setcolor(12);
 rectangle(10,10,500,400);
 setcolor(3);
 rectangle(11,11,499,399);
 setcolor(12);
 rectangle(12,12,498,398);


 setcolor(13);
rectangle(510,110,600,200);
 setcolor(12);
 rectangle(510,210,600,300);
 settextstyle(0,0,0);
 setcolor(15);

 
 outtextxy(535,130,"SCORE");
 outtextxy(535,230,"STATUS");
 settextstyle(0,0,5);

 settextstyle(0,0,0);

 setcolor(3);
 outtextxy(550,170,itoa(score,sData,10));
 outtextxy(518,270,"PLAYING");
 
 initStars();

}


void snakeCaught()
{
 //nosound();
 for(int x=500;x>0;x--)
 { //sound(x);
  delay(1);
  setcolor(rand()%(13)+1);
  drawSnake();
 }
// nosound();
}

void drawBlock(int xb, int yb, int n, int dir)
{
 nblock++;
 sXBlock[nblock]=xb;
 sYBlock[nblock]=yb;
 nBlock[nblock]=n;
 blockDir[nblock]=dir;
 for(int i=1;i<=n;i++)
 {
  setfillstyle(SOLID_FILL,12);
  bar((SX+xb*WIDTH)+1,(SY+yb*WIDTH)+1,(SX+xb*WIDTH)+WIDTH-1,(SY+yb*WIDTH)+WIDTH-1);
  if(dir==HORIZONTAL)
   xb++;
  else if(dir==VERTICAL)
   yb++;
 }

}

void reset()
{
 nblock=0;
 length=8;
 score=0;
 l_k = RIGHT;
 for(int i=0;i<30;i++)
 {
  sXBlock[i]=0;
  sYBlock[i]=0;
  blockDir[i]=0;
  nBlock[i]=0;
 }
}



//***************************************************************
//   Introduction Function
//****************************************************************


/*-------------------------------------------------------------------------*/

void chapterThree()
{
 int isGameOver=FALSE;

 initSnkPosition(20,20);
 
  drawBlock(3,3,3,HORIZONTAL);
  drawBlock(3,4,2,VERTICAL);

  drawBlock(3,35,3,HORIZONTAL);
  drawBlock(3,33,2,VERTICAL);

  drawBlock(MAXX-7,3,3,HORIZONTAL);
  drawBlock(MAXX-5,4,2,VERTICAL);

  drawBlock(MAXX-7,35,3,HORIZONTAL);
  drawBlock(MAXX-5,33,2,VERTICAL);


 while(1)
 {

  setcolor(12);
  rectangle(10,10,500,400);
  setcolor(3);
  rectangle(11,11,499,399);
  setcolor(12);
  rectangle(12,12,498,398);
  setcolor(0);

   //drawStars();
   moveSnake();
   drawSnake();
   hideLastPart();
   getFood();

   delay(timedelay);

   snakeTouchesWall();
   moveTheSnake();

   if(isSnkHitItself()==TRUE || isSnkHitBlock())
   {
    isGameOver=TRUE;
    break;
   }

   while(kbhit())
    keyEvents(ch=getch());


 }
 if(isGameOver)
 {
  setcolor(0);
  outtextxy(518,270,"PLAYING");
  setcolor(3);
  outtextxy(520,270,"Game Over");
  snakeCaught();
  gameOver();
 }


}



void SelectChapter()
{
 reset();
 mainScreen();
 switch(chapter)
 {
 
  case 3:
  chapterThree();
  break;
  
  default:
  break;
 }


}


/*--------------------------------------------------------------*/


void chapterMenu()
{
 int _x=220,_y=50,_h=30;
 int cY=_y+2*_h;
 int choice=1;
 int key;
 cleardevice();
 do{

  settextstyle(0,0,4);
  text3D(_x-70,_y,2,3,13,"MENU");
  settextstyle(0,0,2);
  
  text3D(_x,_y+2*_h,1,3,13,"1. Play ");
text3D(_x,_y+3*_h,1,3,13,"2. Return to first page");

  for(int j=0;j<3000;j++)
   putpixel(random%(640),random%(480),random%(16));
   text3D(_x,_y+6*_h,2,3,10,"FIY");
  text3D(_x-50,_y+7*_h,2,3,10,"* Use Arrow keys to Play");
text3D(_x-50,_y+8*_h,2,3,10,"* Use space bar to Pause/Resume");

        setcolor(5);
        outtextxy(_x-50,cY,"->");

     do{
    key=getch();


     if(key==UP_KEY)
     {
    setcolor(0);
    outtextxy(_x-50,cY,"->");

    cY-=_h;
    choice--;

    if(choice<1)
    {
     choice=2;
     cY =_y+12*_h;
    }

    setcolor(5);
    outtextxy(_x-50,cY,"->");

     //sound(1000);
     delay(20);
     //nosound();


     }
     else if(key==DOWN_KEY)
     {
    setcolor(0);
    outtextxy(_x-50,cY,"->");
    cY+=_h;
    choice++;
    if(choice>2)
    {
     choice=1;
     cY=_y+2*_h;
    }

    setcolor(5);
    outtextxy(_x-50,cY,"->");

     //sound(1000);
     delay(20);
     //nosound();

    }

  }while(key!=13);

   switch(choice)
   {
   
    case 1:
    chapter=3;
    choice=0;
    break;
    
    case 2:
    mainMenu();
    break;

   }


 }while(choice!=0);
 cleardevice();
 SelectChapter();
}
void mainMenu()
{
 int _x=220,_y=100,_h=30;
 int cY=_y+2*_h;
 int choice=1;
 int key;
 cleardevice();
 do{

  settextstyle(0,0,4);
  text3D(_x-70,_y,2,3,13,"SNAKE GAME");
  settextstyle(0,0,2);
  text3D(_x,_y+2*_h,1,3,13,"1.  START GAME");
  text3D(_x,_y+3*_h,1,3,13,"2.  Exit");
  for(int j=0;j<3000;j++)
   putpixel(random%(640),random%(480),random%(16));


        setcolor(5);
        outtextxy(_x-50,cY,"->");

     do{
    key=getch();


     if(key==UP_KEY)
     {
    setcolor(0);
    outtextxy(_x-50,cY,"->");

    cY-=_h;
    choice--;

    if(choice<1)
    {
     choice=2;
     cY =_y+5*_h;
    }

    setcolor(5);
    outtextxy(_x-50,cY,"->");

     //sound(1000);
     delay(20);
     //nosound();


     }
     else if(key==DOWN_KEY)
     {
    setcolor(0);
    outtextxy(_x-50,cY,"->");
    cY+=_h;
    choice++;
    if(choice>2)
    {
     choice=1;
     cY=_y+2*_h;
    }

    setcolor(5);
    outtextxy(_x-50,cY,"->");

     //sound(1000);
     delay(20);
     //nosound();

    }

  }while(key!=13);

   switch(choice)
   {
    case 1:
    level=1;
    choice=0;
    break;
    case 2:
    closegraph();
    restorecrtmode();
    
    exit(0);
    break;

   }


 }while(choice!=0);
 chapterMenu();
}



int main()
{
 int gd=DETECT,gm;
 
 initgraph(&gd,&gm,(char*)"C:\\Dev-Cpp\\lib");
 mainMenu();
 
 return 0;
}


