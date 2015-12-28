#include "types.h"
//#include "defs.h"
#include "param.h"
#include "fs.h"
#include "file.h"
#include "spinlock.h"
#include "user.h"
#include "stat.h"

int
main(){
	//fd = open("/hxo", O_CREATE);
	int fd;
	fd = open("xy", 1);
	write(fd, "abc", 3);
	//link("x/y", "x/z");
	//unlink("x/y");
	//write(fd, "abc", 3);
	//fd.close();
	return(0);
}
