#include "acllib.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define penWidth 3
#define width 1600
#define length 900
#define start 0
#define length_of_number 10
#define range 4
#define color BLACK
#define precision 1e-4

float turn1(int i);
int turn12(float num);
int turn22(float num);
void print_coordinate_axis();
void print_function(float target[], int n);
float f(int n, float target[], float x);
float jifen(float target[], int n);

int Setup()
{
    initConsole();
    int i = 0, j = 0;
    int num = 4;
    float result[ num ];
    result[0] = 1;
    result[1] = 1;
    result[2] = 1;
    result[3] = 1;
	initWindow("test", start, start, width, length);
    beginPaint();
    print_coordinate_axis();
    print_function(result, num);
    endPaint();
    //printf("%f\n",jifen(result, num));
	return 0;
}

void print_coordinate_axis()
{
    setPenColor(color);
    setPenWidth(1);
    setPenStyle(PEN_STYLE_SOLID);
    line(0, length / 2, width, length / 2);
    line(width / 2,0,width / 2,length);
    line(width / 2 - length_of_number / 3, length_of_number, width/2, 0);
    line(width / 2 + length_of_number / 3, length_of_number, width/2, 0);
    line(width,length/2 , width - length_of_number,length / 2- length_of_number / 3);
    line(width,length/2 , width - length_of_number,length / 2+ length_of_number / 3);
    int n;
    setTextColor(color);
    setTextSize(20);
    for(n = 1; n < 5 * 2; n++)
    {
        if( n == 5 )continue;
        line(width / 10 * n, length / 2, width / 10 * n, length/2 - length_of_number);
        line(width/2, length / 10 * n, width/2 + length_of_number, length / 10 * n);
    }
    float pace = -1 * (float)range;
    int k = 0, m = 0;
    char buffer[100];
    char c[5];
    c[4] = 0;
    for( pace, k; k < 5; k++, pace += (float)range / 5)
    {
        sprintf(buffer+4*k, "%4.2f", pace);
    }
    for( pace, k; k <= 10; k++, pace += (float)range / 5)
    {
        sprintf(buffer+4*k, "%4.1f", pace);
    }
    for( k = 0; k <= 10; k++)
    {
        for(m = 0; m < 4; m++)
        {
            c[m] = buffer[4*k + m];
        }
        if(k == 5)paintText(width/10 * k, length/2-10-length_of_number, c );
        else
        {
            paintText(width/10 * (10-k), length/2-10-length_of_number, c );
            paintText(width/2+10+length_of_number, length/10*(10-k), c );
        }
    }
    paintText(width / 2 + 10, 0, "y");
    paintText(width - 20, length / 2 - 30, "x");
}

void print_function(float target[], int n)
{
    int i, j, k;
    for(i = turn22(target[n - 2]); i <= turn22(target[n - 1]); i++)
    {
        for(j = 0; j < penWidth; j++)
        {
            for(k = 0; k < penWidth; k++)
            {
                line(i + j,  turn12(f(n, target,turn1(i))) + k, i + j + 1, turn12(f(n, target,turn1(i + 1))) + k);
            }
        }
    }
}

float f(int n, float target[], float x)
{
    int i;
    float result = 0;
    for(i = n - 3; i >= 0; i--)
    {
        result += target[n - i - 3] * pow(x, i);
    }
    return result;
}

float turn1(int i)
{
    int n = i - width / 2;
    float num = (float)n / width * 2 * range;
    return num;
}

int turn12(float num)
{
    float n = num * length / 2 / range;
    int i = length / 2 - (int)n;
    return i;
}

int turn22(float num)
{
    float n = num * width / 2 / range;
    int i = width / 2 + (int)n;
    return i;
}

float jifen(float target[], int n)
{
    double result = 0;
    float temp = 0;
    float i = target[n - 2];
    int j;
    for ( ; i < target[n - 1]; i += precision)
    {
        temp = 0;
        for( j = 0; j <= n - 3; j++)
        {
            temp += target[j] * pow( i, n - 3 - j);
        }
        result += temp * precision;
    }
    return result;
}
