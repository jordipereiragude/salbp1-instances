#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPS 0.001

//simple program to evaluate order strength.

int main(int argc, char* argv[])
{
  FILE* in;
  int** p;
  int nt;
  double os;
  char str[256];
  int i,j,k,counter;
  double calculus;
  
  if(argc<2)
  {
    printf("missing file\n");
    exit(0);
  }
  //read
  in=fopen(argv[1],"rt");
  while (fgets(str, 256, in) != NULL)
  {
    if(str[1]=='n')
    {
      fgets(str, 256, in);
      sscanf(str,"%d",&nt);
      //printf("nt: %d\n",nt);
      p=(int**)malloc(sizeof(int)*nt*nt+sizeof(int *)*nt);
      for(i = 0 ; i < nt ; i++ ) p[i] = (int *)(p + nt) + i*nt;
      for(i=0;i<nt;i++)
      {
        for(j=0;j<nt;j++)
        {
          p[i][j]=0;
        }
      }
    }
    if(str[1]=='o')
    {
      fgets(str, 256, in);
      sscanf(str,"%lf",&os);
      //printf("os: %f\n",os);
    }
    if(str[1]=='p')
    {
      while (fgets(str, 256, in) != NULL)
      {
        if(str[0]=='<') break;
        if(str[0]=='\n') break;
        sscanf(str,"%d,%d\n",&i,&j);
        p[i-1][j-1]=1;
      }
    }
  }
  fclose(in);
  //transitive closure (works for these instances, not every instance)
  for(i=0;i<nt;i++)
  {
    for(j=(i+1);j<nt;j++)
    {
      if(p[i][j]==0)
      {
        for(k=(+1);k<j;k++)
        {
          if((p[i][k]==1)&&(p[k][j]==1))
          {
            p[i][j]=1;
            break;
          }
        }
      }
    }
  }
  counter=0;
  for(i=0;i<nt;i++)
  {
    for(j=(i+1);j<nt;j++) counter += p[i][j];
  }
  calculus=(double)counter/((nt*nt-nt)/2.0);
  if(fabs(calculus-os)>EPS) printf("reports %f in instance %f\n",calculus,os);
  else printf("OK reports %f in instance %f\n",calculus,os);
  return(0);
}
