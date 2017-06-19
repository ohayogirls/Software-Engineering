/*
算法思想：
1.本算法的主要思想是计算所有情况寻找最优解，假定所有情况是有限的，即圆能放的位置是有限的；
2.所谓圆能放的位置是有限的是指其圆心x、y坐标的精度只能取到小数点后两位，所以区域内圆心的位置有10000个，通过计算每个位
置满足条件的最大半径r，从而找出这些点的最大半径；
3.在寻找最大半径时，每个位置的最大半径的精度只能取到小数点后五位；
4.满足条件的最大圆将放入一个链表中，这样一来之后的圆在进行位置判断时可以与已放入的圆进行比较，从而判断其是否满足条件；
*/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include<windows.h>//sleep函数 
#include <math.h>

//气球
typedef struct ball
{
	double x;       //球心x坐标
	double y;		//球心y坐标
	double z;       //球心z坐标
	double r;		//球半径
}Ball;

//用来储存已经放置的气球
typedef struct ballList
{
	struct ballList * next;
	Ball ball;
}BallList;

void insert(Ball ball);						//插入气球
double distance(Ball b1, Ball b2);          //判断气球之间的距离
double abs_(double num);					    //取绝对值
int judge(Ball b);	//判断新加入的气球是否符合规则
void putBall1();
void putBall2();
DWORD WINAPI putball1(LPVOID pParam);
DWORD WINAPI putball2(LPVOID pParam);
//void putBall();								改变气球的初始位置，求的满足条件的气球
void putPoint(double x, double y, double z);			//放置点

BallList *head = NULL;
double step = 0.01;							//改变气球位置的最小单位
int num = 0;                                //放置气球的个数
double sumr = 0;							//用来记录r^2之和
int n;
int main()
{
	int  pointNum, i;

	printf("请输入空间内所要放置的气球数量: ");
	scanf("%d", &n);
	printf("请输入空间内点的数量: ");
	scanf("%d", &pointNum);
	for (i = 0; i < pointNum; i++)
	{
		printf("请输入第%d个点的坐标(其间用空格隔开)：", i + 1);
		double x, y, z;
		scanf("%lf %lf%lf", &x, &y, &z);
		putPoint(x, y, z);
	}


	printf("\nx坐标\t y坐标\tz坐标\t 半径\n");
	
		HANDLE handle1 = CreateThread(NULL, 0, putball1, NULL, 0, NULL);
		HANDLE handle2 = CreateThread(NULL, 0, putball2, NULL, 0, NULL);
	

	
	ballList *tmp = head, *kz = head;
	for (i = 0; i < pointNum; i++)
	{
		kz = kz->next;
	}
	while (kz)
	{
		Ball ball = tmp->ball;
		printf("%.3lf\t %.3lf\t %.3lf\t %.3lf \n", ball.x, ball.y, ball.z, ball.r);
		tmp = tmp->next;
		kz = kz->next;
	}
	printf("\nr^2之和为:\t %lf\n", sumr);
	Sleep(3);//两秒后退出
	return 0;
}

void putPoint(double x, double y, double z)
{
	Ball ball = { x, y, z,0 };
	insert(ball);
}

void insert(Ball ball)
{
	ballList * newBall = (ballList *)malloc(sizeof(ballList));
	newBall->ball = ball;
	newBall->next = head;
	head = newBall;
}

void putBall1()
{
	Ball ball = { -1 + step, -1 + step,-1 + step, 0 };
	Ball maxBall = ball;
	int i, j, k;
	for (i = 0; ball.x < 1; ++i)
	{
		ball.x += 2 * step;
		ball.y = -1 + step;
		ball.z = -1 + step;
		for (j = 0; ball.y < 1; ++j)
		{
			ball.y += step;
			ball.z = -1 + step;
			for (k = 0; ball.z < 1; ++k) {
				ball.z += step;
				ball.r = 0;
				double rstep = 0.1;
				while (rstep > 0.001)
				{
					if (ball.r > maxBall.r)
					{
						maxBall = ball;
					}
					ball.r += rstep;
					if (!judge(ball))
					{
						ball.r -= rstep;
						rstep /= 10;
					}
				}
			}
		}
	}
}
	void putBall2()
	{
		Ball ball = { -1 + step, -1 + step,-1 + step, 0 };
		Ball maxBall = ball;
		int i, j, k;
		//ball.x += step;
		for (i = 0; ball.x < 1; ++i)
		{
			ball.x +=  step;
			ball.y = -1 + step;
			ball.z = -1 + step;
			for (j = 0; ball.y < 1; ++j)
			{
				ball.y += step;
				ball.z = -1 + step;
				for (k = 0; ball.z < 1; ++k) {
					ball.z += step;
					ball.r = 0;
					double rstep = 0.1;
					while (rstep > 0.01)
					{
						if (ball.r > maxBall.r)
						{
							maxBall = ball;
						}
						ball.r += rstep;
						if (!judge(ball))
						{
							ball.r -= rstep;
							rstep /= 10;
						}
					}
				}
			}
		}
		

	if (judge(maxBall))
	{
		insert(maxBall);
		num++;
		sumr += maxBall.r * maxBall.r;
		//  printf("%.3lf\t %.3lf\t %.3lf\t %.3lf\t %lf \n",  maxBall.x, maxBall.y,maxBall.z ,maxBall.r, sumr);
	}

}
	/*DWORD WINAPI putball1(LPVOID pParam) {
		for (int i = 0; i < n; i++)
			putBall1();
		return 0;
	}*/
	DWORD WINAPI putball2(LPVOID pParam) {
		for (int i = 0; i < n; i++)
			putBall2();
		return 0;
	}
int judge(Ball b)
{
	if ((abs(b.x) + b.r) > 1 || (abs(b.y) + b.r) > 1 || (abs(b.z) + b.r)>1)
	{
		return false;
	}
	ballList *tmp = head;
	while (tmp)
	{
		Ball ball = tmp->ball;
		if (distance(b, ball) < b.r + ball.r)
		{
			return false;
		}
		tmp = tmp->next;
	}
	return true;
}

double abs_(double num)
{
	if (num > 0)
		return num;
	return 0 - num;
}

double distance(Ball b1, Ball b2)
{
	double x1 = b1.x;
	double y1 = b1.y;
	double z1 = b1.z;
	double x2 = b2.x;
	double y2 = b2.y;
	double z2 = b2.z;
	return pow((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2)*(z1 - z2), 0.5);
}
