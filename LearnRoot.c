#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <linux/fb.h>
#include <linux/fb.h>

void* get_system_ram_address(){
	FILE *fd;
	char name[BUFSIZ];
	void *startAddress,*endAddress;
	void *systemRam;
	int ret;
	fd = fopen("/proc/iomem","r");
	if(fd==NULL){
		printf("Failed to open /proc/iomem with error: %s\n", strerror(errno));
		return NULL;
	}

	while(ret=fscanf(fd,"%p-%p : %[^\n]", &startAddress, &endAddress,name)!=EOF){
		// printf("0x%08x-0x%08x : %s\n", startAddress, endAddress, name);
		if(!strcmp(name,"System RAM")){
			systemRam = startAddress;
			continue;
		}
		if(!strncmp(name, "Kernel", 6)){
			break;
		}
	}
	return systemRam;
}

int main(int argc, char* argv[]){
	int fd; //File Descriptor to mmap
	int line_size, buffer_size;
	void* address;
	struct fb_var_screeninfo fixed_info;

	fd = open("/dev/graphics/fb0",O_RDWR);
	if(fd==-1){
		printf("Failed to open /dev/graphics/fb0 with error: %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	ioctl (fd, FBIOGET_VSCREENINFO, &fixed_info);
	line_size = fixed_info.xres * fixed_info.bits_per_pixel / 8;
	buffer_size = line_size * fixed_info.yres;

	address = mmap(NULL,buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	void* system_ram_address = get_system_ram_address();
	printf("MMAP Address: 0X%08x\n", address);
	printf("System RAM: 0X%08x\n", system_ram_address);

	exit(EXIT_SUCCESS);
}