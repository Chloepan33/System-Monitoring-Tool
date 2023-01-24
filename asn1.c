#include <stdio.h>
#include <sys/utsname.h>

void SystemInfo(){
   struct utsname uts;
   if (uname(&uts) < 0)
    perror("uname() error");
    else {
      printf("###System Information");
      printf("System Name:  %s\n", uts.sysname);
      printf("Machine Name:  %s\n", uts.machine);
      printf("Version:  %s\n", uts.version);
      printf("Release:  %s\n", uts.release);
      // what is arcitecture 
      //  printf("Nodename: %s\n", uts.nodename);
    }

}

int main() {
   SystemInfo();
}