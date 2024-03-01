#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
//#include <unistd.h>
//#include <dirent.h> //nu merge libraria

typedef struct header { 
  char *filename;
  uint32_t width;         
  uint32_t height;        
  time_t lastDate;
  uint32_t size, userId, linkNumber;
  char userRights[4], groupRights[4], othersRights[4];
} header; 

header Header;

void getPermis(struct stat st)
{
    if((st.st_mode & S_IWUSR) && (st.st_mode & S_IRUSR) && (st.st_mode & S_IXUSR))
    { 
        strcpy(Header.userRights, "RWX");
    }
    else if((st.st_mode & S_IRUSR) && (st.st_mode & S_IXUSR)) 
    { 
      strcpy(Header.userRights, "R-X");
    }
    else if((st.st_mode & S_IRUSR) && (st.st_mode & S_IWUSR)) 
    { 
        strcpy(Header.userRights, "RW-");
    }
    else if((st.st_mode & S_IWUSR))
    { 
        strcpy(Header.userRights, "-W-");
    }
    else if(st.st_mode & S_IRUSR)
    {
        strcpy(Header.userRights, "R--");
    }
    else if(st.st_mode & S_IXUSR)
    { 
        strcpy(Header.userRights, "--X");
    }
    else 
    {
        strcpy(Header.userRights, "---");
    }

    if((st.st_mode & S_IRGRP) && (st.st_mode & S_IWGRP) && (st.st_mode & S_IXGRP))
    {
        strcpy(Header.groupRights, "RWX");
    }
    else if((st.st_mode & S_IRGRP) && (st.st_mode & S_IXGRP)) 
    { 
        strcpy(Header.groupRights, "R-X");
    }
    else if((st.st_mode & S_IRGRP) && (st.st_mode & S_IWGRP)) 
    { 
        strcpy(Header.groupRights, "RW-");
    }
    else if((st.st_mode & S_IWGRP))
    { 
        strcpy(Header.groupRights, "-W-");
    }
    else if(st.st_mode & S_IRGRP)
    { 
        strcpy(Header.groupRights, "R--");
    }
    else if(st.st_mode & S_IXGRP)
    { 
        strcpy(Header.groupRights, "--X");
    }
        else 
    {
        strcpy(Header.groupRights, "---");
    }

    if((st.st_mode & S_IWOTH) && (st.st_mode & S_IROTH) && (st.st_mode & S_IXOTH))
    { 
        strcpy(Header.othersRights, "RWX");
    }
    else if((st.st_mode & S_IROTH) && (st.st_mode & S_IXOTH)) 
    { 
        strcpy(Header.othersRights, "R-X");
    }
    else if((st.st_mode & S_IROTH) && (st.st_mode & S_IWOTH)) 
    { 
        strcpy(Header.othersRights, "RW-");
    }
    else if((st.st_mode & S_IWOTH))
    { 
        strcpy(Header.othersRights, "-W-");
    }
    else if(st.st_mode & S_IROTH)
    { 
        strcpy(Header.othersRights, "R--");
    }
    else if(st.st_mode & S_IXOTH)
    {
        strcpy(Header.othersRights, "--X");
    }
    else 
    {
        strcpy(Header.othersRights, "---"); 
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("arg gresit");
        exit(1);
    }

    DIR *director = opendir(argv[1]);
    struct dirent *dirEntry;
    if(director == NULL){
        printf("nu e un dir\n");
        exit(2);
    }

    struct stat st;
    char buffer[255];
    memset(&Header, 0, sizeof(Header));
    while(dirEntry = readdir(director))
    {
        char path[256];
        snprintf(path, 256, "%s/%s", argv[1], dirEntry->d_name);
        if(stat(path, &st)<0)
        {
            exit(EXIT_FAILURE);
        }

        int f = open(path, O_RDONLY);
        int g = open("statistica.txt", O_WRONLY , S_IRUSR | S_IWUSR);
       
        if(f<0)
        {
            perror("eroare");
        }
        if(g<0)
        {
            perror("eroare");
        }

        Header.size = st.st_size;
        Header.userId = st.st_uid;
        Header.filename = argv[1];
        //Header.lastDate = st.st_mtim.tv_sec; //nu o gaseste
        Header.linkNumber = st.st_nlink;
        lseek(f,18, SEEK_SET);
        read(f,&Header.width,4); 
        lseek(f,22, SEEK_SET);
        read(f,&Header.height,4); 
        getPermis(st);
  
        printf(" %s \n", dirEntry->d_name);

        if(S_ISREG(st.st_mode))
        {
            char *bmp;
            if((bmp=strstr(dirEntry->d_name, ".bmp"))!=NULL)//daca tipul fisierului e bmp
            { 
                sprintf(buffer, "nume fisier: %s\n inaltime: %d\n lungime: %d\n dimensiune: %d\n identificatorul utilizatorului: ID %d\n  contorul de legaturi: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s\n",
                    Header.filename, Header.width, Header.height, Header.size, Header.userId, Header.linkNumber, Header.userRights, Header.groupRights, Header.othersRights);
                write(g, buffer, strlen(buffer));
            }
            else {
                sprintf(buffer, "nume fisier: %s\n inaltime: %d\n lungime: %d\n dimensiune: %d\n identificatorul utilizatorului: ID %d\n  contorul de legaturi: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s\n",
                    Header.filename, Header.width, Header.height, Header.size, Header.userId, Header.linkNumber, Header.userRights, Header.groupRights, Header.othersRights);
                write(g, buffer, strlen(buffer));
            }
        }
        else if(S_ISDIR(st.st_mode))//daca e dir
        {
            sprintf(buffer, "nume director: %s\n identificatorul utilizatorului: ID %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s\n",
                Header.filename, Header.userId, Header.linkNumber, Header.userRights, Header.groupRights, Header.othersRights);
            write(g, buffer, strlen(buffer));
        }
        else if(S_ISLNK(st.st_mode))//daca e leg simb
        {
            sprintf(buffer, "nume legatura: %s\n dimensiune: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s\n",
                Header.filename, Header.size, Header.userRights, Header.groupRights, Header.othersRights);
            write(g, buffer, strlen(buffer));     
        }
        close(f);
        close(g);
        
    }
    return 0;
}