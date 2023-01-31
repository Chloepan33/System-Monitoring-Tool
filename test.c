#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


void moveUp(int positions){
   printf("\x1b[%dF", positions);
}

void moveDown(int positions){
   printf("\x1b[%dE", positions);
}

int main(){

    printf("start\n");
    for (int i = 0; i < 7; i++){
        printf("\n");
    }
 
   
    printf("end\n");
    moveUp(3);
    printf("2\n");
    moveDown(2);
    printf("1\n");
  
}


