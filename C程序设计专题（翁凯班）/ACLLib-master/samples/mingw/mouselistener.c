#include <stdio.h>
#include "acllib.h"

void mouseListener(int x,int y,int button,int event){
	//event的变化是不管是按下鼠标还是没有按下，只要移动了就是event=5
	//当按下鼠标，event=0
	//手指上抬，松开鼠标，event=2 
	static int ox=0;
	static int oy=0;
	static int flag=0;//如果不加static这样的话会画不出来  
	printf("x=%d,y=%d,button=%d,event=%d\n",x,y,button,event);
	if(event==0){
		ox=x;
		oy=y;//重新赋值坐标给ox、oy是为了在下一次在别的位置画图不会突然一条线连过去，重新开始在新的位置画图 
		flag=1;		
	}else if(event==2){//当鼠标抬起，不会执行画图代码 
		flag=0;
	}
	if(flag){//当按下鼠标时，flag=1，抬起flag=0 
		beginPaint();
		line(ox,oy,x,y);
		endPaint();	
		ox=x;
		oy=y;		
	}
}
int Setup()
{
	initWindow("Test",DEFAULT,DEFAULT,800,800);
	initConsole();
	printf("Hello\n");
	//回调函数获取事件来知道鼠标移动位置以及鼠标状态 
	registerMouseEvent(mouseListener); //注册了回调函数 
	return 0;
}
