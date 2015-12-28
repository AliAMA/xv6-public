#include "types.h"
#include "defs.h"
#include "param.h"
#include "fs.h"
#include "file.h"
#include "spinlock.h"

int
main(){
	fd = open("/hxo", O_CREATE);
	write(fd, "abc", 3);
	fd.close();
}
