
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include<termios.h>
#include<signal.h>
#include <sys/wait.h>
#include <string.h>
#define BUFOR 150

static int pid_table[3];
FILE *file_ptr;
int tunel;
struct sigaction act;
sigset_t set1;
int pomc=-10;
int pipe_table[2];
int pom;


static void hdl (int sig, siginfo_t *siginfo, void *context);




int main(int argc, char *argv[])
{
      sigemptyset(&set1);
      act.sa_sigaction = &hdl;
      sigaction(SIGUSR1, &act, NULL);
      sigaction(SIGCONT, &act, NULL);
      sigaction(SIGUSR2, &act, NULL);
      sigaction(SIGINT, &act, NULL);
      sigaction(SIGSTKFLT, &act, NULL);
      sigaction(SIGTRAP, &act, NULL);
      fflush(stdout);
      fflush(stdin);
      printf("\e[2J\e[H");
      sigset_t mask1;
      pom=pipe(pipe_table);
		  mkfifo("FIFO",0666);
		  tunel=open("FIFO",O_RDWR);
      printf("\n1. STDIN\n2. /DEV/URANDOM\n3. FILE\n");
      char choice=getc(stdin);
      printf("\b\b");
      char file_name[30];
      sleep(1);

          if(fork()==0)
          {
            char p1_bufor;
            pid_table[0]=getpid();
            char *standart=NULL;
            int i,j =0;
            long unsigned int bytes_read = 1;
            long unsigned int nbytes = 100;
            char *new_string=NULL;
            char *helper = new_string;
            switch(choice)

            {
            case '1':
               new_string = (char *)malloc(nbytes+1);
               puts("Please enter a line of text");
               // fflush(stdin)
               bytes_read = getline(&new_string, &nbytes, stdin);
               printf("\tREADING [%lu bytes]", bytes_read-1); // -1 aby opuscic znak zatwierdzenia ENTEREM
               i= j =0;
               while(i!=3) { printf(" ."); sleep(1) ;fflush(stdout); i++;                 }
               printf("\n");
               fflush(stdout);
               for(i=0; i<bytes_read-1; i++)  write(tunel, &new_string[i], sizeof(char)+1);
               sleep(1);
               raise(SIGSTKFLT);
               break;

            case '2':
                file_ptr = fopen("/dev/urandom", "r");
                printf("\tREADING [/dev/urandom]:");
                int i=0;
                while(i!=3) {printf(" ."); sleep(1) ;fflush(stdout); i++;}
                fflush(stdout);
                sleep(1);
                unsigned char p1_bufor;
                short o;
                while (fgets(&p1_bufor, sizeof(p1_bufor)+1, file_ptr))   write(tunel, &p1_bufor, sizeof(p1_bufor)+1);
                break;

            case '3':
                printf("ENTER FILE PATH: ");
                scanf("%s", &file_name[0]);
                printf("\tREADING [%s]:", *&file_name);
                while(i!=3) { printf(" ."); sleep(1) ;fflush(stdout); i++; }
                sleep(1);
                printf("\n\n");
                file_ptr = fopen(file_name, "r");
                while((p1_bufor=getc(file_ptr))!=EOF)  write(tunel, &p1_bufor, sizeof(p1_bufor)+1);
                sleep(1);
                raise(SIGSTKFLT);
                break;
              }
          }


        if(fork()==0)
        {
          pid_table[1]=getpid();
          unsigned char char_bufor;
          short inted_bufor;
          while(read(tunel,&char_bufor,sizeof(char_bufor)+1))
          {
            inted_bufor = (short)char_bufor;
            write(pipe_table[1],&inted_bufor,sizeof(inted_bufor));
          }
          return 0;
        }

				if(fork()==0)
        {
          pid_table[2]=getpid();
          short displayed_bufor;
          short o=0;
          while(read(pipe_table[0], &displayed_bufor, sizeof(displayed_bufor)))
          {
            printf("%02X ", displayed_bufor);
            fflush(stdout);
            o++;
            if(o==15)
              {
                printf("\n");
                o=0;
              }
            }
          return 0;
        }


while(pomc!=1)
{

}

        fflush(stdout);
        printf("\nTERMINATED SUCCESSFULLY!\n\n\n");
        fflush(stdout);
        sleep(3);

	return 0;
}

static void hdl (int sig, siginfo_t *siginfo, void *context)
{
   switch(sig)
   {
     int i;
      case SIGUSR1:
      sigaction(SIGUSR1, &act, NULL);
      sigaction(SIGCONT, &act, NULL);
      sigaction(SIGUSR2, &act, NULL);
      sigaction(SIGINT, &act, NULL);
      sigaction(SIGTERM, &act, NULL);
      fflush(stdout);
      sigsuspend(&set1);
      break;

      case SIGINT:
        kill(getpid(), SIGUSR1);
       // for(i = 0; i<=2; i++) if(getpid()!=pid_table[i])
       break;

      case SIGUSR2:
        for(i = 2; i>=0; i--)  kill(pid_table[i], SIGCONT);
      break;

      case SIGCONT:

      break;

      case SIGSTKFLT:
      unlink("FIFO");
      for(i=2; i>=0; i--) if(getpid()!=pid_table[i]) kill(pid_table[i], SIGTRAP);
      raise(SIGTRAP);
      close(tunel);

      break;

      case SIGTRAP:

      if(getpid()==pid_table[2])
      {
        close (pipe_table[1]);
        close (pipe_table[0]);
      }
      pomc = 1;
      printf("\n\n");
      exit(0);
      break;
   }
}
