#include <stdio.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <utmp.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Moves the cursor up.
 *
 * @param positions Number of lines to move up
 * @return void
 */
void moveUp(int positions)
{
   printf("\x1b[%dF", positions);
}

/**
 * @brief Moves the cursor down.
 *
 * @param positions Number of lines to move down
 * @return void
 */
void moveDown(int positions)
{
   printf("\x1b[%dE", positions);
}

/**
 * @brief Displaying the system information, including (in the order of)
 *    System Name,
 *    Machine Name,
 *    Version,
 *    Release,
 *    Architecture.
 *
 * If fail to get system information, show error message
 *
 * @return void
 */
void ShowSystemInfo()
{
   printf("----------------------------\n");
   struct utsname uts;
   if (uname(&uts) < 0)
   {
      perror("SystemInfo error");
   }
   else
   {
      printf("### System Information ### \n");
      printf("System Name:  %s\n", uts.sysname);
      printf("Machine Name:  %s\n", uts.nodename);
      printf("Version:  %s\n", uts.version);
      printf("Release:  %s\n", uts.release);
      printf("Architecture: %s\n", uts.machine);
   }
   printf("----------------------------\n");
}

/**
 * @brief Displaying memory used by the current program in unit of kilobytes
 *
 * If fail to get memory usage, show error message
 *
 * @return void
 */
void ShowMemoryUsage()
{
   struct rusage r_usage;
   if (getrusage(RUSAGE_SELF, &r_usage) < 0)
   {
      perror("MemoryUsage error");
   }
   else
   {
      printf("Memory usage: %ld kilobytes\n", r_usage.ru_maxrss);
   }
}

/**
 * @brief Displaying memory variation represented by graph
 * 
 * @param pre previous memory size
 * @param post current memory size
 * 
 * @return void
 */

void MemroyGraph(double pre, double post)
{

   double diff = post - pre;
   printf("|");
   if (diff >= 0)
   {
      if (diff < 0.01 || pre < 0)
      {
         printf("o");
      }
      else
      {
         for (int i = 0; i < (int)diff * 10; i++)
         {
            printf("#");
         }
         printf("*");
      }
   }
   else
   {
      if (diff >= -0.01)
      {
         printf("@");
      }
      else
      {
         for (int i = 0; i < (int)-diff * 10; i++)
         {
            printf(":");
         }
         printf("@");
      }
      diff = -diff;
   }
   printf(" %.2f (%.2f)\n", diff, post);
}

/**
 * @brief Displaying memory information, in unit of GB, including
 *    total physical memory,
 *    used physical memory,
 *    total virtual memory,
 *    used virtual memory.
 *
 *    Memories are calculated by reading file /proc/meminfo.
 *    total physical memory = totalram
 *    used physical memory = (totalram - freeram)  - (bufferram + cachedram + SReclaimable)
 *    total virtual memory = totalram + total swap
 *    used virtual memory = used physical memory + totalswap - freeswap
 *
 * @param pre value of previous used memory size
 * @param graph_state to indicate whether or not to show graphics
 * @return the used physical memory
 *
 */
double ShowMemory(double pre, int graph_state)
{
   FILE *meminfo = fopen("/proc/meminfo", "r");
   char line[200];
   long totalram, freeram, bufferram, cachedram, totalswap, freeswap, sre;
   long phys_used, total_phys, virtual_used, total_virtual;

   while (fgets(line, sizeof(line), meminfo))
   {
      if (sscanf(line, "MemTotal: %ld kB", &totalram) == 1)
      {
         totalram *= 1024;
      }
      else if (sscanf(line, "MemFree: %ld kB", &freeram) == 1)
      {
         freeram *= 1024;
      }
      else if (sscanf(line, "Buffers: %ld kB", &bufferram) == 1)
      {
         bufferram *= 1024;
      }
      else if (sscanf(line, "Cached: %ld kB", &cachedram) == 1)
      {
         cachedram *= 1024;
      }
      else if (sscanf(line, "SwapTotal: %ld kB", &totalswap) == 1)
      {
         totalswap *= 1024;
      }
      else if (sscanf(line, "SwapFree: %ld kB", &freeswap) == 1)
      {
         freeswap *= 1024;
      }
      else if (sscanf(line, "SReclaimable: %ld kB", &sre) == 1)
      {
         sre *= 1024;
         fclose(meminfo);
      }
   }
   total_phys = totalram;
   phys_used = (totalram - freeram) - (bufferram + cachedram + sre);
   total_virtual = totalram + totalswap;
   virtual_used = phys_used + totalswap - freeswap;
   printf("%.2f GB / %.2f GB  -- %.2f GB / %.2f GB", phys_used * 1e-9, total_phys * 1e-9, virtual_used * 1e-9, total_virtual * 1e-9);
   if (graph_state == 0)
   {
      printf("\n");
   }
   else
   {
      MemroyGraph(pre * 1e-9, phys_used * 1e-9);
   }
   return (double)phys_used;
}

/**
 * @brief Displaying information of current login users, including
 *    username,
 *    device name,
 *    host name for remote login.
 *
 * @return void
 */
void ShowUser()
{
   printf("----------------------------\n");
   printf("### Sessions/users ### \n");
   struct utmp *data;
   data = getutent();
   while (data != NULL)
   {
      if (strlen(data->ut_name) != 0)
      {
         printf("%s %s %s\n", data->ut_name, data->ut_line, data->ut_host);
      }
      data = getutent();
   }
}

/**
 * @brief Displaying the number of cores of current system.
 *
 * @return void
 */
void ShowCore()
{
   printf("----------------------------\n");
   int core_num = sysconf(_SC_NPROCESSORS_ONLN);
   printf("Number of cores: %d\n", core_num);
}

/**
 * @brief Displaying the utilization percentage of CPU by reading file /proc/stat
 *
 * @param period indicate how frequently to sample in seconds
 * 
 * @return the utilization percentage of CPU
 */
double ShowCpu(int period)
{
   int core_num = sysconf(_SC_NPROCESSORS_ONLN);
   unsigned long long pre[4];
   unsigned long long aft[4];
   unsigned long long diff[4];
   char cpu[10];
   FILE *fp;
   fp = fopen("/proc/stat", "r");
   fscanf(fp, "%s %llu %llu %llu %llu",
          cpu, &pre[0], &pre[1], &pre[2], &pre[3]);
   sleep(period);
   rewind(fp);
   fscanf(fp, "%s %llu %llu %llu %llu",
          cpu, &aft[0], &aft[1], &aft[2], &aft[3]);
   fclose(fp);
   for (int i = 0; i < 4; i++)
   {
      diff[i] = aft[i] - pre[i];
   }
   unsigned long long percent = diff[0] + diff[1] + diff[2];
   unsigned long long total = percent + diff[3];
   printf("CPU usage: %.2f%%\n", (double)percent / (double)total * 100);
   return (double)percent / (double)total * 100;
}

/**
 * @brief Displaying cpu usage in graphic form
 *
 *    The number of symbol "|" is equals to number of percentage
 *
 * @param cpu used cpu percentage
 * 
 * @return void
 */
void CpuGraph(double cpu)
{
   printf("\t");
   for (int i = 0; i < (int)cpu; i++)
   {
      printf("|");
   }
   printf("%.2f\n", cpu * 0.01);
}

/**
 * @brief Displaying system information including memory usage and cpu usage
 *
 * @param sample_size indicate how many times the statistics are going to be collected
 * @param period indicate how frequently to sample in seconds
 * @param graphic_state indicate whether or not to show graphics
 * 
 * @return void
 */
void ShowSystem(int sample_size, int period, int graphic_state)
{

   printf("----------------------------\n");
   printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot) \n");
   for (int i = 0; i < sample_size; i++)
   {
      printf("\n");
   }
   ShowCore();
   moveUp(sample_size + 2);
   double pre = -1;
   for (int m = sample_size + 1; m > 1; m--)
   {
      pre = ShowMemory(pre, graphic_state);
      moveDown(m);
      double cpu = ShowCpu(period);
      if (graphic_state == 0)
      {
         moveUp(m + 1);
      }
      else if (graphic_state == 1)
      {
         moveDown(sample_size + 1 - m);
         CpuGraph(cpu);
         moveUp(sample_size + 3);
      }
   }
   moveDown(sample_size * graphic_state + 3);
}


/**
 * @brief Displaying system information including memory usage and cpu usage in sequantial form
 *
 * @param sample_size indicate how many times the statistics are going to be collected
 * @param period indicate how frequently to sample in seconds
 * @param graphic_state indicate whether or not to show graphics
 * 
 * @return void
 */
void ShowSequentials(int sample_size, int period, int graph_state)
{
   double pre = -1;
   for (int i = 0; i < sample_size; i++)
   {
      printf(">>> iteration %d\n", i + 1);
      ShowMemoryUsage();
      printf("----------------------------\n");
      printf("### Memory ### (Phys.Used/Tot -- Virtual Used/Tot) \n");
      for (int m = 0; m < i; m++){
         printf("\n");
      }
      pre = ShowMemory(pre, graph_state);
       for (int c = 1; c < sample_size - i; c++){
         printf("\n");
      }
      printf("----------------------------\n");
      double cpu = ShowCpu(period);
      if(graph_state == 1){
         CpuGraph(cpu);
      }
      printf("----------------------------\n");
   }
}

int main(int argc, char *argv[])
{
   int sample_size = 10;
   int period = 1;

   int count_int = 0;
   int tem_int = 0;

   int system_state = 0;
   int user_state = 0;
   int graphic_state = 0;
   int sequential_state = 0;

   if (argc == 1)
   {
      printf("Nbr of samples: %d -- every %d secs\n", sample_size, period);
      ShowSystem(sample_size, period, graphic_state);
      ShowUser();
      ShowSystemInfo();
   }
   else
   {
      for (int i = 1; i < argc; i++)
      {
         if (strcmp(argv[i], "--system") == 0)
         {
            system_state = 1;
         }
         else if (strcmp(argv[i], "--user") == 0)
         {
            user_state = 1;
         }
         else if (strcmp(argv[i], "--graphics") == 0)
         {
            graphic_state = 1;
         }
         else if (strcmp(argv[i], "--sequential") == 0)
         {
            sequential_state = 1;
         }
         else if (sscanf(argv[i], "--samples=%d", &sample_size) == 1 && (sample_size > 0))
         {
            printf("The current sample size is %d\n", sample_size);
         }
         else if (sscanf(argv[i], "--tdelay=%d", &period) == 1 && (period > 0))
         {
            printf("The current sample frequency is %d sec\n", period);
         }
         else if(sscanf(argv[i], "%d", &tem_int) == 1 && (tem_int > 0)){
            if (count_int == 2){
               printf("To many input integers!\n");
               exit(0);
            }
            else if (count_int == 1){
               period = tem_int;
               tem_int = 0;
               count_int = 2;
            }
            else if(count_int == 0){
               sample_size = tem_int;
               tem_int = 0;
               count_int = 1;
            }
         }
         else
         {
            printf("Invalid command line arguments\n");
            exit(0);
         }
      }
      printf("Nbr of samples: %d -- every %d secs\n", sample_size, period);
      if (user_state == 1)
      {
         if (system_state == 1 || sequential_state == 1 || graphic_state == 1)
         {
            printf("command combinition invalid\n");
            exit(0);
         }
         else
         {
            ShowUser();
         }
      }
      else if (system_state == 1)
      {
         if (sequential_state == 1)
         {
            ShowSequentials(sample_size, period, graphic_state);
         }
         else
         {
            ShowSystem(sample_size, period, graphic_state);
         }
      }
      else if (sequential_state == 1)
      {
         ShowSequentials(sample_size, period, graphic_state);
         ShowUser();
      }
      else
      {
         ShowSystem(sample_size, period, graphic_state);
         ShowUser();
      }
      ShowSystemInfo();
   }
}