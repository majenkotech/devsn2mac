#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_arp.h>
#include <net/if.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#include "../config.h"

int setMacAddress(uint8_t mac[6]) {
	struct ifreq ifr;
	int s;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == -1) {
		return -1;
	}

	strcpy(ifr.ifr_name, "eth0");
	ifr.ifr_hwaddr.sa_data[0] = mac[0];
	ifr.ifr_hwaddr.sa_data[1] = mac[1];
	ifr.ifr_hwaddr.sa_data[2] = mac[2];
	ifr.ifr_hwaddr.sa_data[3] = mac[3];
	ifr.ifr_hwaddr.sa_data[4] = mac[4];
	ifr.ifr_hwaddr.sa_data[5] = mac[5];
	ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
	return ioctl(s, SIOCSIFHWADDR, &ifr);
}

int main(char **argv, int argc) {
	int config_mem_fd;
	uint8_t address[6];

	config_mem_fd = open("/dev/mem", O_RDONLY | O_SYNC);
	if (config_mem_fd < 0) {
		fprintf(stderr, "Unable to open /dev/mem: %s\n", strerror(errno));
		return -1;
	}

	lseek(config_mem_fd, 0xBFC54020, SEEK_SET);
	read(config_mem_fd, address, 6);
	close(config_mem_fd);

	address[0] |= 0x02;	// Force locally administered
	address[0] &= 0xFE;	// Force unicast

	printf("Mac address: %02x:%02x:%02x:%02x:%02x:%02x\n",
		address[0],
		address[1],
		address[2],
		address[3],
		address[4],
		address[5]
	);


	if (setMacAddress(address) == -1) {
		fprintf(stderr, "Unable to set MAC address: %s\n", strerror(errno));
		return -1;
	}
	return 0;
}
