#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<math.h>
#include<time.h>
#include<limits.h>

class house{
 private:
  double x,y;
  int population;
  double dems, reps;
  house *next,*last;
 public:
  house(double ax, double ay, int apop, double adems,double areps)
  {
    x=ax;
    y=ay;
    population=apop;
    dems=adems;
    reps=areps;
    next=NULL;
    last=NULL;
  };
  house(house *copy)
    {
      x=copy->get_x();
      y=copy->get_y();
      population=copy->get_pop();
      dems=copy->get_dems();
      reps=copy->get_reps();
      next=NULL;
      last=NULL;
    }
  double get_x()
  {
    return x;
  };
  double get_y()
  {
    return y;
  };
  int get_pop()
  {
    return population;
  };
  double get_reps()
  {
    return reps;
  };
  double get_dems()
  {
    return dems;
  };
  void set_next(house *anext)
  {
    next=anext;
  };
  house* get_next()
  {
    return next;
  };
  void set_last(house *alast)
  {
    last=alast;
  };
  house* get_last()
  {
    return last;
  };
  void print()
    {
      printf("%lf %lf %i\n",x,y,population);
    };
  ~house()
    {
      house *temp=this;
      while(temp->get_next()!=NULL)
	temp=temp->get_next();
      while(temp->get_last()!=NULL)
	{
	  temp=temp->get_last();
	  free(temp->get_next());
	}
    }
};

class district{
 private:
  double x,y;
  int population;
  double dems,reps;
  house *first;
 public:
  district(double ax,double ay)
    {
      x=ax;
      y=ay;
      first=NULL;
      population=0;
      dems=0;
      reps=0;
    }
  double get_x()
  {
    return x;
  }
  double get_y()
  {
    return y;
  }
  double get_dems()
  {
    return dems;
  }
  double get_reps()
  {
    return reps;
  }
  double get_distance(house *h)
  {
    return sqrt((this->x - h->get_x())*(this->x - h->get_x()) +
		(this->y - h->get_y())*(this->y - h->get_y()));
  }
  double get_distance(district *d)
  {
    return sqrt((this->x - d->get_x())*(this->x - d->get_x()) +
		(this->y - d->get_y())*(this->y - d->get_y()));
  }
  void add_house(house *h)
  {
    house *temp=new house(h);

    if(first!=NULL)
      {
	first->set_last(temp);
	temp->set_next(first);
      }
    first=temp;
    //population+=first->get_pop();
      
  }
  void add_pop(house *h)
  {
    population+=h->get_pop();
    dems+=h->get_dems();
    reps+=h->get_reps();
  }
  int get_pop()
  {
    return population;
  }
  house* get_houses()
  {
    return first;
  }
  void print()
  {
    printf("%10lf %10lf %7i %8lf %8lf\n",x,y,population,dems,reps);
  }
  void print(int avg, int stddev)
  {
    int mydev= (population-avg)*(population-avg);
    int percent = (10000*mydev)/stddev;
    printf("%10lf %10lf %7i %8.1lf %8.1lf %5.2f\n",
	   x,y,population,dems,reps,fabs(percent/100.0));
  }

  ~district()
    {
      delete first;
    }
};


  
int main(int argc, char **argv)
{
  srand(time(NULL));
  int i,j,k;
  const int ndistricts=3;
  const int target_demDistricts=1;
  district *districts[ndistricts];
  double distances[ndistricts];
  double distmin;
  int minindex,nhouses,centhouse;
  FILE *housefile,*outfile,*picturefile;
  char line[100];
  double x,y,dist,fdist,xdist,newx[ndistricts],newy[ndistricts],alpha;
  int pop=0;
  double dems,reps;
  house *first=NULL,*last=NULL,*temp;
  double xmin=DBL_MAX,xmax=-DBL_MAX,ymin=DBL_MAX,ymax=-DBL_MAX;
  long long stddev,mindev=LLONG_MAX;
  int demDistricts,repDistricts,tieDistricts;
  if(argc>=2)
    housefile=fopen(argv[1],"r");
  else
    {
      printf("Please specify a population file\n");
      return 1;
    }
  if(argc>=3)
    outfile=fopen(argv[2],"w");
  else
    {
      printf("Please specify an outfile\n");
      return 1;
    }
  if(argc>=4)
    {
      picturefile=fopen(argv[3],"w");
    }
  while(fgets(line,100,housefile)!=NULL)
    {
      sscanf(line,"%lf %lf %i %lf %lf",&x,&y,&pop,&dems,&reps);
      temp=new house(x,y,pop,dems,reps);
      if(first==NULL)
	{
	  first=temp;
	  last=temp;
	}
      else
	{
	  last->set_next(temp);
	  temp->set_last(last);
	  last=temp;
	}
    }
  /*
  temp=first;
  while(temp!=NULL)
    {
      printf("%lf %lf %i\n",temp->get_x(),temp->get_y(),temp->get_pop());
      temp=temp->get_next();
    }
  */
  nhouses=0;
  temp=first;
  while(temp!=NULL)
    {
      if(xmin>temp->get_x())
	xmin=temp->get_x();
      if(xmax<temp->get_x())
	xmax=temp->get_x();
      if(ymin>temp->get_y())
	ymin=temp->get_y();
      if(ymax<temp->get_y())
	ymax=temp->get_y();
      temp=temp->get_next();
      nhouses++;
    }
  for(i=0;i<ndistricts;i++)
    {
      centhouse=rand()%nhouses;
      temp=first;
      for(j=0;j<centhouse;j++)
	temp=temp->get_next();
      x=temp->get_x();
      y=temp->get_y();
      districts[i]=new district(x,y);
    }
  pop=0;
  reps=0;
  dems=0;
  temp=first;
  while(temp!=NULL)
    {
      pop+=temp->get_pop();
      temp=temp->get_next();
    }
  temp=first;
  while(temp!=NULL)
    {
      dems+=temp->get_dems();
      temp=temp->get_next();
    }
  temp=first;
  while(temp!=NULL)
    {
      reps+=temp->get_reps();
      temp=temp->get_next();
    }
  
  temp=first;
  while(temp!=NULL)
    {
      for(i=0;i<ndistricts;i++)
	distances[i]=districts[i]->get_distance(temp);
      	    
      distmin=DBL_MAX;
      for(i=0;i<ndistricts;i++)
	if(distmin>distances[i])
	  {
	    distmin=distances[i];
	    minindex=i;
	  }

      //temp->print();
      //printf("Still Working\n");
      districts[minindex]->add_pop(temp);
      temp=temp->get_next();
    }
  //for(i=0;i<ndistricts;i++)
  //  districts[i]->print();
  //  printf("\n");
  stddev=mindev;
  do
    {
      //alpha=pow(10,-sqrt(1.0/ndistricts)*sqrt(stddev)/50);
      for(i=0;i<ndistricts;i++)
	{
	  newx[i]=0;
	  newy[i]=0;
	  for(j=0;j<ndistricts;j++)
	    {
	      if(i!=j)
		{
		  dist=districts[i]->get_distance(districts[j]);
		  if((districts[i]->get_pop()-pop/ndistricts)*(districts[j]->get_pop()-pop/ndistricts)<0)
		    fdist=(districts[j]->get_pop()-pop/ndistricts)*districts[i]->get_pop()/(pop*dist*dist);
		  else
		    fdist=0;

		  if(fdist>1)
		    fdist=1;
		  if(fdist<-1)
		    fdist=-1;
		  fdist*=0.01;
		  if(dist<.01)
		    fdist=5;
		  xdist=0;
		  if(demDistricts!=target_demDistricts)
		    xdist=fabs((districts[j]->get_dems()-districts[j]->get_reps())*(districts[i]->get_dems()-districts[i]->get_reps()))/(pop*dist*dist);
		  if(xdist>1)
		    xdist=1;
		  if(fdist<-1)
		    xdist=-1;
		  xdist/=stddev;
		  if(dist<.01)
		    xdist=0;
		  
		  newx[i]+=(districts[j]->get_x()-districts[i]->get_x())
		    *fdist;
		  newy[i]+=(districts[j]->get_y()-districts[i]->get_y())
		    *fdist;
		  newx[i]+=(districts[j]->get_y()-districts[i]->get_y())
		    *xdist;
		  newy[i]-=(districts[j]->get_x()-districts[i]->get_x())
		    *xdist;

		  //printf("%lf\n",fdist);
		  //printf("%lf\n",(districts[j]->get_x()-districts[i]->get_x())*(districts[i]->get_pop()*(districts[j]->get_pop()-pop/ndistricts))/(pop*dist*dist*dist));
		}
	    }
	  //	  printf("%lf %lf\n",newx[i],newy[i]);
	}
      for(i=0;i<ndistricts;i++)
	{
	  newx[i]+=districts[i]->get_x();
	  newy[i]+=districts[i]->get_y();
	  delete districts[i];
	  districts[i] = new district(newx[i],newy[i]);
	}
      
      temp=first;
      while(temp!=NULL)
	{
	  for(i=0;i<ndistricts;i++)
	    distances[i]=districts[i]->get_distance(temp);
	  
	  distmin=DBL_MAX;
	  for(i=0;i<ndistricts;i++)
	    if(distmin>distances[i])
	      {
		distmin=distances[i];
		minindex=i;
	      }
	  
	  //temp->print();
	  //printf("Still Working\n");
	  districts[minindex]->add_pop(temp);
	  temp=temp->get_next();
	}
      stddev=0;
      for(i=0;i<ndistricts;i++)
	stddev+=(districts[i]->get_pop()-pop/ndistricts)
	  *(districts[i]->get_pop()-pop/ndistricts);
      for(i=0;i<ndistricts;i++)
	if(districts[i]->get_pop()==0)
	  stddev=-1;
      if(stddev<mindev)
	mindev=stddev;
      //for(i=0;i<ndistricts;i++)
      //districts[i]->print(pop/ndistricts,stddev);
      demDistricts=0;
      repDistricts=0;
      tieDistricts=0;
      for(i=0;i<ndistricts;i++)
	{
	  if(districts[i]->get_dems()==districts[i]->get_reps())
	    tieDistricts++;
	  else if (districts[i]->get_dems()>districts[i]->get_reps())
	    demDistricts++;
	  else
	    repDistricts++;
	}
      for(i=0;i<80;i++)
	printf("\b");
      printf("%f %f %i %i %i %lf %lf",sqrt(1.0/ndistricts)*sqrt(stddev),
	     sqrt(1.0/ndistricts)*sqrt(mindev),
	     tieDistricts,demDistricts,repDistricts,
	     dems,reps);
      //      scanf("%lf",&x);
    } while (stddev>16*ndistricts || demDistricts!=target_demDistricts);
  printf("\n");
  //delete first;
  if (stddev!=-1)
    {
      temp=first;
      while(temp!=NULL)
	{
	  for(i=0;i<ndistricts;i++)
	    distances[i]=districts[i]->get_distance(temp);
	  
	  distmin=DBL_MAX;
	  for(i=0;i<ndistricts;i++)
	    if(distmin>distances[i])
	      {
		distmin=distances[i];
		minindex=i;
	      }
	  
	  //temp->print();
	  //printf("Still Working\n");
	  districts[minindex]->add_house(temp);
	  temp=temp->get_next();
	}
      for(i=0;i<ndistricts;i++)
	{
	  temp=districts[i]->get_houses();
	  while(temp!=NULL)
	    {
	      fprintf(outfile,"%lf %lf %i %i\n",temp->get_x(),
		      temp->get_y(),
		      temp->get_pop(),i);
	      temp=temp->get_next();
	    }
	}
      
      if(argc==4)
	for(i=0;i<1000;i++)
	  {
	    for(j=0;j<1000;j++)
	      {
		
		x=xmin+i*(xmax-xmin+0.5)/(1000.0);
		y=ymin+j*(xmax-xmin+0.5)/(1000.0);
		temp=new house(x,y,0,0,0);
		
		distmin=DBL_MAX;
		for(k=0;k<ndistricts;k++)
		  {
		    dist=districts[k]->get_distance(temp);
		    if(dist<distmin)
		      {
			distmin=dist;
			minindex=k;
		      }
		  }
		//printf("Still Working\n");
		delete temp;
		
		fprintf(picturefile,"%lf %lf %i %lf\n",x,y,minindex,districts[minindex]->get_dems()/districts[minindex]->get_pop());
		
		
	      }
	    fprintf(picturefile,"\n");
	  }
    }
}
