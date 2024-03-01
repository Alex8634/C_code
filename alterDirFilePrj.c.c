#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>


typedef struct header { 
  char *filename;
  uint32_t width;         
  uint32_t height;        
  uint32_t size, userId, linkNumber;
  char userRights[4], groupRights[4], othersRights[4];
} header; 

header Header;

char buffer[256];
int proc=0;

void get_filename_only(char *filext) 
{
    int i;
    for (i = 0; i <=strlen(filext)-1; i++)
    {
        if (filext[i] == '.')
        {
            filext[i] = 0;
        }
         
    }
    
}

void func_write(int stat_file, int var, char* filename, uint32_t width, uint32_t height, uint32_t size, uint32_t userId, uint32_t linkNumber, char* userRights, char* groupRights, char* othersRights)
{ 
    if (var==0)
    {
        sprintf(buffer, "nume fisier: %s\n inaltime: %d\n lungime: %d\n dimensiune: %d\n identificatorul utilizatorului: ID %d\n  contorul de legaturi: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s\n",
            filename, width, height, size, userId, linkNumber, userRights, groupRights, othersRights);
        write(stat_file, &buffer, strlen(buffer));
    }
    else if (var==1)
    {
        sprintf(buffer, "nume director: %s\n identificatorul utilizatorului: ID %d\n contorul de legaturi: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s\n",
            filename, userId, linkNumber, userRights, groupRights, othersRights);
        write(stat_file, &buffer, strlen(buffer));    
    }
    else
    {
        sprintf(buffer, "nume legatura: %s\n dimensiune: %d\n drepturi de acces user: %s\n drepturi de acces grup: %s\n drepturi de acces altii: %s\n",
            filename, size, userRights, groupRights, othersRights);
        write(stat_file, &buffer, strlen(buffer));
    }
}

void get_permit(struct stat st)
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

typedef struct {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
} PixPax;

void convert_img(char* path)
{
    int file = open(path, O_RDWR);
    if (file < 0) {
        perror("eroare deschidere");
        exit(-1);
    }
    char header[54];
    read(file, header, sizeof(char) * 54);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    PixPax pixel;
    lseek(file, 54, SEEK_SET);  

    for (int i = 0; i < width * height; ++i) {
        read(file, &pixel, sizeof(PixPax));

        pixel.red = (unsigned char)(0.299 * pixel.red);
        pixel.blue = (unsigned char)(0.114 * pixel.blue);
        pixel.green = (unsigned char)(0.587 * pixel.green);

        lseek(file, -sizeof(PixPax), SEEK_CUR);
        write(file, &pixel, sizeof(PixPax));
    }
    
    close(file);
}

int main(int argc, char* argv[]){

    if(argc != 4){
        perror("arg gresit");
        exit(1);
    }
    char* c = argv[3];
    DIR *director_in = opendir(argv[1]);
    //DIR *director_out = opendir(argv[2]);

    struct dirent *dirEntry;

    if(director_in == NULL){
        perror("nu e un dir\n");
        exit(1);
    }

    //if(director_out == NULL)
    //{
    //    perror("nu e un dir\n");
    //    exit(1);
    //}
    char path[512];
    dirEntry = readdir(director_in);
    sprintf(path,"%s/%s", argv[1], dirEntry->d_name);
    int f = open(path, O_RDONLY);
    if(f<0)
    {
        perror("eroare");
    }

    char out_path[512];
    char filext[100];

    strcpy(filext, dirEntry->d_name);
    get_filename_only(filext);
    sprintf(out_path, "%s/%s_%s", argv[2], filext, "statistica.txt");

    int stat_file = open(out_path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(stat_file<0)
    {
        perror("eroare");
    }
    int pid;

    struct stat st;
    memset(&Header, 0, sizeof(Header));

    while((dirEntry = readdir(director_in)))
    {
        
        char *bmp;
        if((bmp=strstr(dirEntry->d_name, ".bmp"))!=NULL)//daca tipul fisierului e bmp
        {
            proc++;
            if ((pid = fork()) < 0)
            {
                perror("eroare");
                exit(-1);
            }
            if (pid == 0)
            {
                convert_img(path);
                exit(1);
            }
        }

        proc++;
        lstat(path, &st);
        //printf(" %s \n", dirEntry->d_name);

        if(S_ISLNK(st.st_mode))//daca e leg simb
        {            
            if((pid=fork())<0)
                {
                    perror("eroare");
                    exit(1);
                }
            if(pid==0)
                {
                    Header.size = st.st_size;
                    Header.userId = st.st_uid;
                    Header.filename = filext;
                    Header.linkNumber = st.st_nlink;
                    lseek(f,18, SEEK_SET);
                    read(f,&Header.width,4); 
                    lseek(f,22, SEEK_SET);
                    read(f,&Header.height,4); 
                    get_permit(st);
                    func_write(stat_file,2,Header.filename, Header.width, Header.height, Header.size, Header.userId, Header.linkNumber, Header.userRights, Header.groupRights, Header.othersRights);
                    exit(0); 
                }  
        }
        else 
        {
            stat(path, &st);
            if(S_ISREG(st.st_mode))
            {
                if((bmp=strstr(dirEntry->d_name, ".bmp"))!=NULL)//daca tipul fisierului e bmp
                {    
                    if((pid=fork())<0)
                    {
                        perror("eroare");
                        exit(1);
                    }
                    if(pid==0)
                    {
                        Header.size = st.st_size;
                        Header.userId = st.st_uid;
                        Header.filename = filext;
                        Header.linkNumber = st.st_nlink;
                        lseek(f,18, SEEK_SET);
                        read(f,&Header.width,4); 
                        lseek(f,22, SEEK_SET);
                        read(f,&Header.height,4); 
                        get_permit(st);
                        func_write(stat_file,0,Header.filename, Header.width, Header.height, Header.size, Header.userId, Header.linkNumber, Header.userRights, Header.groupRights, Header.othersRights);
                        exit(0); 
                    }
                }
                else 
                {
                    int pipe_1[2];
                    int pipe_2[2];

                    if (pipe(pipe_1) < 0) 
                    {
                        perror("eroare");
                        exit(1);
                    }
                    if (pipe(pipe_2) < 0) 
                    {
                        perror("eroare");
                        exit(1);
                    }

                    if ((pid = fork()) < 0)
                    {
                        perror("eroare");
                        exit(1);
                    }
                    if (pid == 0)
                    {
                        close(pipe_1[0]);
                        close(pipe_2[0]);
                        close(pipe_2[1]);

                        int fil_1 = lseek(f, 0, SEEK_END);

                        lseek(f, 0, SEEK_SET);
                        Header.size = st.st_size;
                        Header.userId = st.st_uid;
                        Header.filename = filext;
                        Header.linkNumber = st.st_nlink;
                        lseek(f,18, SEEK_SET);
                        read(f,&Header.width,4); 
                        lseek(f,22, SEEK_SET);
                        read(f,&Header.height,4); 
                        get_permit(st);
                        func_write(stat_file,0,Header.filename, Header.width, Header.height, Header.size, Header.userId, Header.linkNumber, Header.userRights, Header.groupRights, Header.othersRights);
                        exit(0); 


                        char fil_2[256];
                        read(f, fil_2, fil_1);
                        write(pipe_1[1], fil_2, fil_1);

                        close(pipe_1[1]);
                        exit(1);
                    }

                    proc++;

                    if ((pid = fork()) < 0)
                    {
                        perror("eroare\n");
                        exit(1);
                    }
                    if (pid == 0)
                    {
                        close(pipe_2[0]);
                        close(pipe_1[1]);
                        dup2(pipe_1[0], 0);
                        dup2(pipe_2[1], 1);
                        close(pipe_1[0]);
                        close(pipe_2[1]);
                        execlp("sh", "sh","script.sh", c, (char *)NULL);
                        perror("eroare");
                        exit(1);
                    }

                    close(pipe_1[0]);
                    close(pipe_1[1]);
                    close(pipe_2[1]);
                    
                    char correct_propositions[2];
                    read(pipe_2[0], correct_propositions, sizeof(char));
                    correct_propositions[1] = 0;
                    printf("Au fost identificate in total %s propozitii corecte care contincaracterul %s\n", correct_propositions, c);

                    close(pipe_2[0]);
                }
            }
        
            else if(S_ISDIR(st.st_mode))
            {
                if((pid=fork())<0)
                {
                    perror("eroare");
                    exit(1);
                }
                if(pid==0)
                {
                    Header.size = st.st_size;
                    Header.userId = st.st_uid;
                    Header.filename = filext;
                    Header.linkNumber = st.st_nlink;
                    lseek(f,18, SEEK_SET);
                    read(f,&Header.width,4); 
                    lseek(f,22, SEEK_SET);
                    read(f,&Header.height,4); 
                    get_permit(st);
                    func_write(stat_file,1,Header.filename, Header.width, Header.height, Header.size, Header.userId, Header.linkNumber, Header.userRights, Header.groupRights, Header.othersRights);
                    exit(0); 
                }
            }
        }  
    }

    int status, pidf,i;
    for (i = 0; i <= proc-1; i++)
    {
        if ((pidf = wait(&status)) < 0)
        {
            perror("eroare\n");
            exit(-1);
        }
        if (WIFEXITED(status))
        {
            printf("S-a încheiat procesul cu pid-ul %d si codul %d\n", pidf, WEXITSTATUS(status));
        }
    }

    close(f);
    close(stat_file);
    closedir(director_in);
    
    return 0;
}
