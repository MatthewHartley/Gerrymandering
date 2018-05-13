#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

int main(int argc,char **argv)
{
  srand(time(NULL));
  int i,j,p;
  double x,y,d,q,r,theta;
  for(i=0;i<10000;i++)
    {
      q=((double)rand())/RAND_MAX;
      theta=q*2*M_PI;
      q=((double)rand())/RAND_MAX;
      r=sqrt(q)*50;
      x=r*cos(theta);
      y=r*sin(theta);
      p=rand()%4+1;
      d=.2*p;
      printf("%lf %lf %i %lf %lf\n",x,y,p,d,p-d);
    }
}
