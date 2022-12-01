#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "ioctl_test.h"

int main(){
  int answer = 543;
  struct mystruct test = {4, "Johannes"};
  int dev = open("/dev/ex6b_module", 00);
  if(dev == -1){
    printf("openinng was not possible\n");
    return -1;
  }
  
  ioctl(dev, RD_VALUE, &answer);
  printf("The answer is %d\n", answer);

  answer = 123;

  ioctl(dev, WR_VALUE, &answer);
  ioctl(dev, RD_VALUE, &answer);
  printf("The answer is now %d\n", answer);

  ioctl(dev, GREETER, &test);

  


  printf("opening was successfull!\n");
  close(dev);
  return 0;


}
