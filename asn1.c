#include <stdio.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>


void SystemInfo(){
   struct utsname uts;
   if (uname(&uts) < 0){
      perror("SystemInfo error");
   }   
   else {
      printf("###System Information");
      printf("System Name:  %s\n", uts.sysname);
      printf("Machine Name:  %s\n", uts.nodename);
      printf("Version:  %s\n", uts.version);
      printf("Release:  %s\n", uts.release);
      printf("Architecture: %s\n", uts.machine);
    }

}

void MemoryUsage(){
   struct rusage r_usage;
   if (getrusage(RUSAGE_SELF,&r_usage)<0){
      perror("MemoryUsage error");
   }
   else{
      printf("Memory usage: %ld kilobytes\n",r_usage.ru_maxrss);
   }

}

void Memory(){
   struct sysinfo info;
   if(sysinfo(&info)<0){
      perrorz("Memory error");
   }
   else{
      printf
   }

}

int main() {
   SystemInfo();
   MemoryUsage();
}