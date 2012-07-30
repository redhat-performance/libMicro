/*
 * Copyright 2012 Red Hat, Inc.
 *
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the "License").  You may not use this file except
 * in compliance with the License.
 *
 * See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>

void sigint(int sig)
{
	fprintf(stderr, "Keyboard Interrupt\n");
	return;
}

int main(int argc, char **argv)
{
	int fd;
	int32_t target = 0;
	int ret;

	if ((fd = open("/dev/cpu_dma_latency", O_WRONLY)) == -1) {
		if (errno != ENOENT)
			fprintf(stderr, "Failed to open /dev/cpu_dma_latency: (%d) %s\n", errno, strerror(errno));
		exit(-1);
	}

	if ((ret = write(fd, &target, sizeof(target))) <= 0) {
		fprintf(stderr, "Failed to write zero to /dev/cpu_dma_latency: (%d) %s\n", errno, strerror(errno));
		exit(-2);
	}
	signal(SIGINT, sigint);
	printf("pausing with /dev/cpu_dma_latency open\n");
	pause();
	printf("closing /dev/cpu_dma_latency\n");
	close(fd);
	exit(0);
}
