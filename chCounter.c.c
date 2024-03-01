#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>


int main(int argc, char *argv[])
{



  int n1=0,n2=0,n3=0,n4=0,n5=0;
  int rd,f,bufSize=1;
  char buffer[bufSize];
  char ch= argv[3][0];
  f=open(argv[1],O_RDONLY );
  
  //g=open( char argv[2],O_WRONLY );
  if(argc!=4)
    {
      exit(5);
    }
  
  if(f<0)
    {perror("fals");}
  // if(g<0)
  // {perror("nu prea");}

  while((rd=read(f,buffer,bufSize))>0)
  for(int i=0;i<rd;i++)
    {
      if(buffer[0]==ch)
	n4++;
      else if(isalpha(buffer[0]))
	{if(islower(buffer[0]))n1++;
	  else n2++;}
      else if (isdigit(buffer[0]))
	n3++;
      n5++;
    }
 
  printf("%d\n",n1);
  printf("%d\n",n2);
  printf("%d\n",n3);
  printf("%d\n",n4);  	
  printf("%d\n",n5);  	
  return 0;
}
