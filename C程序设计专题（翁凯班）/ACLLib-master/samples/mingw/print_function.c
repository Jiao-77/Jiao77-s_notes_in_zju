#include "acllib.h"
#include <stdio.h>
#include <math.h>

#define WIDTH 900
#define LENGTH 900
#define PEN 3
#define LENGTH_OF_NUM 20
#define EXT 1e-4

double xturn (double x,double width);
double width (double begin,double end);
double xy (double x,int n,double pows[n+2]);
double length (double begin,double end,int n,double pows[n+2]);
double yturn (double y,double length);
double turn (double width,double length);
void print_coordinate_axis();

int Setup(){
    initConsole();
    int num;
    printf("请输入多项式次数：");
    scanf("%d",&num);
    double pows[num+2];
    for(int i = num;i >= 0;i --){
        printf("\n请输入第%d项的系数：",i);
        scanf("%lf",&pows[i]);
    }
    printf("\n请输入区间起点：");
    double begin, end;
    scanf("%lf",&begin);
    printf("\n请输入区间终点：");
    scanf("%lf",&end);
    if(begin > end){
        printf("请输入正确的区间\n");
        return 0;
    }
    double widthp = width(begin,end);
    double lengthp = length(begin,end,num,pows);
    initWindow("print function", 0, 0, WIDTH,LENGTH);
    beginPaint();
    setPenColor(BLACK);
    setPenWidth(1);
    setPenStyle(PEN_STYLE_SOLID);
    line(0, LENGTH / 2, WIDTH, LENGTH / 2);
    line(WIDTH / 2,0,WIDTH / 2,LENGTH);
    line(WIDTH / 2 - LENGTH_OF_NUM / 3, LENGTH_OF_NUM, WIDTH/2, 0);
    line(WIDTH / 2 + LENGTH_OF_NUM / 3, LENGTH_OF_NUM, WIDTH/2, 0);
    line(WIDTH,LENGTH/2 , WIDTH - LENGTH_OF_NUM,LENGTH / 2- LENGTH_OF_NUM / 3);
    line(WIDTH,LENGTH/2 , WIDTH - LENGTH_OF_NUM,LENGTH / 2+ LENGTH_OF_NUM / 3);
    for(int i = 1;i <= 10;i ++){
        line(WIDTH/10*i,LENGTH/2,WIDTH/10*i,LENGTH/2 - 10);
        line(WIDTH/2,LENGTH/10*i,WIDTH/2 + 10,LENGTH/10*i);
    }
    char bufferx[1000],x[6],buffery[1000],y[6];
    double pace = -1 * widthp;
    int k = 0,m = 0;
    for(pace , k;k < 5;k ++,pace += widthp/5)
    {
        sprintf(bufferx + 5 * k,"%5.2f",pace);
    }
    for(pace , k;k <= 10;k ++,pace += widthp/5)
    {
        sprintf(bufferx + 5 * k,"%5.1f",pace);
    }
    pace = -1 * lengthp;
    for(pace , m;m < 5;m ++,pace += lengthp/5)
    {
        sprintf(buffery + 5 * m,"%5.2f",pace);
    }
    for(pace , m;m <= 10;m ++,pace += lengthp/5)
    {
        sprintf(buffery + 5 * m,"%5.1f",pace);
    }
    for( k = 0; k <= 10; k++)
    {
        for(m = 0; m < 5; m++)
        {
            x[m] = bufferx[5*k + m];
            y[m] = buffery[5*k + m];
        }
        if(k == 5)paintText(WIDTH/10 * k, LENGTH/2-10-LENGTH_OF_NUM, x );
        else
        {
            paintText(WIDTH/10 * k, LENGTH/2-10-LENGTH_OF_NUM, x );
            paintText(WIDTH/2+10+LENGTH_OF_NUM, LENGTH/10*(10-k), y );
        }
    }
    paintText(WIDTH / 2 + 10, 0, "y");
    paintText(WIDTH - 20, LENGTH / 2 - 30, "x");
    for(double xi = begin;xi <= end;xi += EXT){
        double xp = (xi + widthp)/2/widthp*WIDTH;
        double yp = LENGTH - (xy(xi,num,pows) + lengthp)/2/lengthp*LENGTH;
        double xp1 = (xi + EXT + widthp)/2/widthp*WIDTH;
        double yp1 = LENGTH - (xy(xi+EXT,num,pows) + lengthp)/2/lengthp*LENGTH;
        line(xp,yp,xp1,yp1);
    }
    endPaint();
}

double width (double begin,double end)
{
    double width;
    if(fabs(begin) <= fabs(end)){
        width = fabs(end);
    }else{
        width = fabs(begin);
    }
    width *= 1.2;
    return width;
}

double xy (double x,int n,double pows[n+2])
{
    double y = pows[0];
    for (int i = n;i > 0;i--){
        y += pows[i]*powf(x,i); 
    }
    return y;
}

double length (double begin,double end,int n,double pows[n+2])
{
    double max = xy(begin,n,pows);
    for(double x = begin;x <= end;x += EXT){
        double yx = xy(x,n,pows);
        if(fabs(yx) > fabs(max)){
            max = yx;
        }
    }
    double length = fabs(max);
    length *= 1.2;
    return length;
}

double turn (double width,double length)
{
    double a1 = WIDTH/width;
    double a2 = LENGTH/length;
    if(a1 > a2){
        return a1;
    }else{
        return a2;
    }
}