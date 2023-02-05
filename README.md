# ASN1

Class: cscb09
Created: February 4, 2023 7:04 PM
Type: Assignment
session: 2023 winter

# **System Monitoring Tool**

---

A simple command line program written in C that reports different metrics of system utilization. The program provides information about basic system information, CPU utilization, memory utilization, and users’ information.

## ****Getting Started****

---

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### **Prerequisites**

You need a C compiler to build the program. GCC is recommended for this purpose. The program should be run on a Linux operating system. 

### **Installing**

1. Clone the repository to your local machine 
    
    ```bash
    git clone https://github.com/Chloepan33/System-Monitoring-Tool.git
    ```
    
2. Compile the program
    
    ```bash
    gcc system_monitoring_tool.c -o system_info
    ```
    
3. Run the program
    
    ```bash
    ./system_info
    ```
    

### Usage

Run the program directly 

```bash
./system_info
```

The program will output the following information:

```bash
Nbr of samples: 10 -- every 1 secs
----------------------------
### Memory ### (Phys.Used/Tot -- Virtual Used/Tot) 
6.36 GB / 8.14 GB  -- 8.24 GB / 10.14 GB
6.36 GB / 8.14 GB  -- 8.24 GB / 10.14 GB
6.36 GB / 8.14 GB  -- 8.24 GB / 10.14 GB
----------------------------
Number of cores: 3
CPU usage: 17.79%
----------------------------
### Sessions/users ### 
reboot ~ 5.4.0-137-generic
runlevel ~ 5.4.0-137-generic
LOGIN tty1 
diascass pts/1 70.24.6.98
hakimahm pts/2 tmux(1613).%0
ghurbur1 pts/16 
hakimahm pts/17 tmux(1613).%2
effendia pts/18 tmux(19982).%0
yangan28 pts/25 142.115.78.109
hakimahm pts/28 tmux(1613).%3
----------------------------
### System Information ### 
System Name:  Linux
Machine Name:  mathlab
Version:  #154-Ubuntu SMP Thu Jan 5 17:03:22 UTC 2023
Release:  5.4.0-137-generic
Architecture: x86_64
----------------------------
```

With ***10*** samples and sampled every ***1*** second, by default. 

The following command line arguments are available for use

- `--system`, which will only display system usage information
- `--user`, which will only display user usage information
- `--graphics`, which will provide graphical output
- `--sequential`, which will show information sequentially without requiring a screen refresh
- `--samples=N` , which allows a value ***N*** to be specified to indicate how many times statistics will be collected
- `--tdelay=T`, which specifies the frequency of sampling in ***T*** seconds

The program also takes positive integers as arguments. 

For example, by running the program

```bash
./system_info 5 2
```

The sample size will now be 5 and sampled every 2 seconds. 

If user input 3 integers or more, the program will display “`To many input integers!`” error message. 

If only 1 integer been entered, only the sample size will change to the input value while the sampling frequency remains unchanged as 1 sec. 

Most of the command line arguments could be combined. 

For example `./system_info --system --graphics --samples=5`, will result in displaying only the system information in graphical form with 5 samples. 

However, `--user`  combined with any of `--system`, `--graphics`, `--sequential` will be considered as invalid. 

The program will display “`Command combination invalid`” error message. 

Any other user input will lead to “`Invalid command line arguments`” error message. 

### More information

The program displaying information including user usage and system usage. 

For user usage, it displays all current login users’ information including its username, device name and host name for remote login. 

For system usage, it displays total utilization of the CPU and memory. 

Memory information are displayed in unit of GB, including total physical memory, used physical memory, total virtual memory and used virtual memory.

$$
\begin{align}\text{total physical memory} &= \text{totalram}\\ \text{used physical memory} &= (\text{totalram} - \text{freeram})  - (\text{bufferram} + \text{cachedram} + \text{SReclaimable}) \\   \text{total virtual memory} &= \text{totalram} + \text{total swap}\\  \text{used virtual memory }&= \text{used physical memory} + \text{totalswap - freeswap}\end{align}
$$

Calculated by reading file /proc/meminfo.  

For graphical representations. 

- for CPU utilization: “`|||`” are used to represent positive percentage increase
- for memory utilization: showing the variation of memory used
    - if memory increase but less then 0.01 GB, “`|o`” will be displayed, otherwise if memory increase “`######*`” will be shown
    - if memory decrease but less then 0.01 GB, “`|@`” will be displayed, otherwise if memory decrease “`:::::@`” will be shown

### How do I solve the problem

I first get to know what kinds of information is needed and should be presented in which form from the example video. I also learn the libraries and files provided by the handout. 

After having a rough idea of the whole picture, I divided all required informations into smaller modules and start to write function codes for it. For example, I decide that all basic system information should be in one function. 

After completing functions for displaying all information in its basic form. I modify them so that they can represent in sequential and graphical form. 

Then I start writing codes for the main function so that it could scan the user’s command line input and deciding which functions to call. 

Last I add documentation, improve it with error handling and test the code. 

Documentations for functions are included with the code.