#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int fd, i;
  char path[] = "myfile";
  char data[512];

  //printf(1, "writing starting\n");
  memset(data, 'A', sizeof(data));

  /**for(i = 0; i < 4; i++)
    if(fork() > 0)
      break;

  printf(1, "write %d\n", i);*/

  //path[8] += i;
  fd = open(path, O_CREATE | O_RDWR);
  for(i = 0; i < 20; i++)
//    printf(fd, "%d\n", i);
    write(fd, data, sizeof(data));
  close(fd);

/*  printf(1, "read\n");

  fd = open(path, O_RDONLY);
  for (i = 0; i < 20; i++)
    read(fd, data, sizeof(data));
  close(fd);
*/
  //wait();

  exit();
}
