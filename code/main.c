#include <stdio.h>
#include <stdlib.h>
#include "utils.h"  //random stuff I use
#include "albp.h"   //albp-centric code
#include <unistd.h> //getopt
#include <assert.h>
#include <getopt.h>

int numTries=1;

int main(int argc, char* argv[])
{
  instance inst;
  characteristics def;
  char c;
  int ret;
  static int isBimodal=NO;
  static int withChains=NO;
  static int withBottlenecks=NO;



  def.mean1=100;
  def.sd1=50;
  def.mean2=500;
  def.sd2=125;
  def.prob1=0.0;
  def.m=20;
  def.M=1000;
  def.OS=0.2;
  def.delta=0.001;
  def.numChains=5;
  def.tasksInChain=4;
  def.numBottlenecks=5;
  def.precSucBottlenecks=4;
  def.numLevels=0;
  def.nt=100;
  def.numSolitary=0;
  def.cycle=1000;
  

  while (1)
  {
    static struct option long_options[] =
    {          
      {"isBimodal", no_argument,       &isBimodal, 1},
      {"withChains", no_argument,       &withChains, 1},
      {"withBottlenecks", no_argument,       &withBottlenecks, 1},
      {"help", no_argument,       0, 'h'},

      {"mean1", required_argument,       0 , 'a'},
      {"sd1", required_argument,       0 , 'b'},
      {"mean2", required_argument,       0 , 'c'},
      {"sd2", required_argument,       0 , 'd'},
      {"prob1", required_argument,       0 , 'e'},
      {"minTaskTime", required_argument,       0 , 'f'},
      {"maxTaskTime", required_argument,       0 , 'g'},
      {"OS", required_argument,       0 , 'o'},
      {"deltaOS", required_argument,       0 , 'i'},
      {"numChains", required_argument,       0 , 'j'},
      {"tasksInChain", required_argument,       0 , 'k'},
      {"numBottlenecks", required_argument,       0 , 'l'},
      {"tasksInBottlenecks", required_argument,       0 , 'm'},
      {"numLevels", required_argument,       0 , 'p'},
      {"numTasks", required_argument,       0 , 'n'},
      {"numSolitary", required_argument, 0, 's'},
      {"seed",required_argument,0,'r'},
      {"cycle",required_argument,0,'q'},
    };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long (argc, argv, "a:b:c:d:e:f:g:h:i:j:k:l:m:n:s:r:q:",long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
    {
    case 0:
      break;

    case 'q':
      def.cycle=atoi(optarg);
      break;

    case 's':
      def.numSolitary=atoi(optarg);
      break;

    case 'n':
      def.nt=atoi(optarg);
      break;

    case 'o':
      def.OS=atof(optarg);
      break;
      
    case 'a':
      def.mean1=atof(optarg);
      break;

    case 'b':
      def.sd1=atof(optarg);
      break;

    case 'c':
      def.mean2=atof(optarg);
      break;

    case 'd':
      def.sd2=atof(optarg);
      break;

    case 'e':
      def.prob1=atof(optarg);
      break;

    case 'f':
      def.m=atoi(optarg);
      break;

    case 'g':
      def.M=atoi(optarg);
      break;
    
    case 'h':
      printf("%s:\n",argv[0]);
      printf("\t--fileName file\n");
      printf("\t--isBimodal\n");
      printf("\t--withChains\n");
      printf("\t--withBottlenecks\n");
      printf("\t--help\n");
      printf("\t--numTasks x\n");
      printf("\t--mean1 x\n");
      printf("\t--sd1 x\n");
      printf("\t--mean2 x\n");
      printf("\t--sd2 x\n");
      printf("\t--prob1 x\n");
      printf("\t--minTask x\n");
      printf("\t--maxTask x\n");
      printf("\t--OS x\n");
      printf("\t--deltaOS x\n");
      printf("\t--numChains x\n");
      printf("\t--tasksInChain x\n");
      printf("\t--numBottlenecks x\n");
      printf("\t--tasksInBottlenecks (number of predecessors and successors of bottleneck task) x\n");
      printf("\t--numLevels x\n");
      printf("\t--numSolitary x\n");
      printf("\t--cycle x\n");
      printf("\t--seed x\n");
      return(0);

    case 'i':
      def.delta=atof(optarg);
      break;

    case 'j':
      def.numChains=atoi(optarg);
      break;

    case 'k':
      def.tasksInChain=atoi(optarg);
      break;

    case 'l':
      def.numBottlenecks=atoi(optarg);
      break;

    case 'm':
      def.precSucBottlenecks=atoi(optarg);
      break;

    case 'p':
      def.numLevels=atoi(optarg);
      break;

    case 'r':
      initializeRandom(atoi(optarg));
      break;
      
    default:
      printf("Error\n");
      return(0);
    }
  }
  def.isBimodal=isBimodal;
  def.withChains=withChains;
  def.withBottlenecks=withBottlenecks;
  ret=generateGraph(&inst,&def);
  if (ret==0) return(0);
  ret=generateTaskTimes(&inst,&def);
  
  //print to stdout
  printf("<number of tasks>\n%d\n\n",inst.nt);
  printf("<cycle time>\n%d\n\n",inst.c);
  printf("<order strength>\n%0.3f\n\n",inst.os);
  printf("<task times>\n");
  for(int i=1;i<=inst.nt;i++)
  {
    printf("%d %d\n",i,inst.t[i]);
  }
  printf("\n<precedence relations>\n");
  for(int i=1;i<=inst.nt;i++)
  {
    for(int j=1;j<=inst.is[i][0];j++)
    {
      printf("%d,%d\n",i,inst.is[i][j]);
    }
  }
  printf("\n<end>\n");



  return(0);
}


