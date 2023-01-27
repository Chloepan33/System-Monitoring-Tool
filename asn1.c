#include <stdio.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <utmp.h>
#include <unistd.h>
#include <string.h>


void ShowSystemInfo(){
   struct utsname uts;
   if (uname(&uts) < 0){
      perror("SystemInfo error");
   }   
   else {
      printf("###System Information###\n");
      printf("System Name:  %s\n", uts.sysname);
      printf("Machine Name:  %s\n", uts.nodename);
      printf("Version:  %s\n", uts.version);
      printf("Release:  %s\n", uts.release);
      printf("Architecture: %s\n", uts.machine);
    }

}

void ShowMemoryUsage(){
   struct rusage r_usage;
   if (getrusage(RUSAGE_SELF,&r_usage)<0){
      perror("MemoryUsage error");
   }
   else{
      printf("Memory usage: %ld kilobytes\n",r_usage.ru_maxrss);
   }

}

void ShowMemory(){
   struct sysinfo info;
   if(sysinfo(&info)<0){
      perror("Memory error");
   }
   else{
      long double PhysTotal = info.totalram * info.mem_unit;
      long double PhysUsed = (info.totalram - info.freeram) * info.mem_unit;
      long double VirTotal = (info.totalram + info.totalswap) * info.mem_unit;
      long double VirUsed = (info.totalram + info.totalswap - info.freeram - info.freeswap) * info.mem_unit;
      printf("%.2Lf GB / %.2Lf GB  -- %.2Lf GB / %.2Lf GB\n", PhysTotal* 1e-9, PhysUsed* 1e-9, VirTotal* 1e-9, VirUsed* 1e-9);
      // printf ("Total RAM : %.2lf\n", info.totalram * info.mem_unit * 1e-9);
      // printf("Used RAM : %.2lf\n", (info.totalram - info.freeram)*info.mem_unit * 1e-9);
      // printf ("Total Free : %.2lf\n", (info.totalram + info.totalswap)* info.mem_unit * 1e-9);
      // printf("Used Free : %.2lf\n", (info.totalram + info.totalswap - info.freeram - info.freeswap)*info.mem_unit * 1e-9);
   }

}

void ShowUser(){
   struct utmp *data;
   data = getutent();
   while(data != NULL){
      if(strlen(data->ut_name)!= 0){
         printf("%s %s %s\n" , data->ut_name,data->ut_line,data->ut_host);
      }
      data = getutent();
   }
}

void ShowCpu(){
   int core_num = sysconf(_SC_NPROCESSORS_ONLN);
   printf("Number of cores: %d\n", core_num);
   int pre[4];
   int aft[4];
   char cpu[10];
   FILE * fp;
   fp = fopen("/proc/stat", "r");
   fscanf(fp,"%s %d %d %d %d",
          cpu,&pre[0],&pre[1],&pre[2],&pre[3]);
   sleep(1);
   rewind(fp);
   fscanf(fp,"%s %d %d %d %d",
          cpu,&aft[0],&aft[1],&aft[2],&aft[3]);
   fclose(fp);
   printf("CPU usage:%d%%\n",100 - (aft[3] - pre[3])/core_num);
}

void ShowAllBasic(int sample_size,int period){
   printf("Nbr of samples: %d -- every %d secs\n", sample_size, period);
   ShowMemoryUsage();
   printf("----------------------------\n");
   printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot) \n");
   for(int i = 0; i < sample_size; i++){
      ShowMemory();
      sleep(period);
   }
   printf("----------------------------\n");
   printf("### Sessions/users ### \n");
   ShowUser();
   printf("----------------------------\n");
   ShowCpu();
   printf("----------------------------\n");
   printf("### System Information ### \n");
   ShowSystemInfo();
   printf("----------------------------\n");
   
}

int main(int argc, char *argv[]) {
   if (argc == 1){
      ShowAllBasic(10,1);
   }
   else if (argc == 2){
      printf("you have 2 arg\n");
   }
   else{
      printf("invalid number of inputs\n");
   }

   
   
}