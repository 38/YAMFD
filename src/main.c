#include <stdlib.h>
#include <smc.h>

int main(int argc, char** argv)
{
	if(smc_init() < 0) 
		exit(1);
	return 0;
}
