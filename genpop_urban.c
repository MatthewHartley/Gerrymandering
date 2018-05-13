#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<limits.h>

int main(int argc,char **argv)
{
  srand(time(NULL));
  int i,j,p;
  double x,y,d;
  for(i=0;i<10000;i++)
    {
      x=0;
      for(j=0;j<12;j++)
	x+=((double)rand())/RAND_MAX;
      x-=6;
      x*=10.0/6.0;
      y=0;
      for(j=0;j<12;j++)
	y+=((double)rand())/RAND_MAX;
      y-=6;
      y*=10.0/6.0;
      p=rand()%4+1;
      d=.8*(double)p;
      printf("%lf %lf %i %lf %lf\n",x+10-20*(i%2),y,p,d,p-d);
    }
}
