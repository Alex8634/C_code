#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
//#include <unistd.h>

struct header { 
    uint16_t type; 
    uint32_t size; 
    uint16_t reserved1;     
    uint16_t reserved2;    
    uint32_t offset;        
    uint32_t dib_size;  
    uint32_t width;         
    uint32_t height;        
    uint16_t planes;       
    uint16_t bpp;            
    uint32_t compression;    
    uint32_t image_size;    
    uint32_t x_ppm;         
    uint32_t y_ppm;          
    uint32_t num_colors;     
    uint32_t important_colors; 
}; 

void func(int f, int bufSize, char *buffer, int ok)
{
    int rd,wt;
    if((rd=read(f,buffer,bufSize))>0 && ok)
    wt=write(f,buffer,bufSize);
}

int main(int argc, char *argv[])
{
  int f,bufSize=1;
  char buffer[1];
  f=open(argv[1],O_RDONLY );
  f=open("out.txt",O_WRONLY );
  struct header Header;
  if(argc!=2)
    {
     printf("Usage ./p fis.");
     exit(5);
    }
  
  if(f<0)
    {
        perror("eroare");
    }

  memset(&Header, 0, sizeof(Header));

  write(f,argv[1],bufSize);
  func(f,&Header.type,2,0);
  func(f,&Header.size,4,0); 
  func(f,&Header.reserved1,2,0); 
  func(f,&Header.reserved2,2,0); 
  func(f,&Header.offset,4,0); 
  func(f,&Header.dib_size,4,0); 
  func(f,&Header.width,4,1); 
  func(f,&Header.height,4,1); 
  func(f,&Header.planes,2,0); 
  func(f,&Header.bpp,2,0); 
  func(f,&Header.compression,4,0); 
  func(f,&Header.image_size,4,1); 
  func(f,&Header.x_ppm,4,0); 
  func(f,&Header.y_ppm,4,0); 
  func(f,&Header.num_colors,4,0); 
  func(f,&Header.important_colors,4,0);

  return 0;
}
