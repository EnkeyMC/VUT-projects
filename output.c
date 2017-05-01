/**
 * @file 		output.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	16.4.2017
 */
#include <string.h>

#include "output.h"
#include "shared_mem.h"
#include "process.h"
#include "center.h"
#include "debug.h"


int setup_output_res() {
	// Open output file
	_fp = fopen(FILENAME, "w");

	if (_fp == NULL) {
		perror("Error opening output file");
		return -1;
	}

	setvbuf(_fp, NULL, _IONBF, 0); // Set buffer to no buffer

	if ((_output_counter_shm = (int*) create_shm(sizeof(int))) == NULL) 
		return -1;
	*_output_counter_shm = 1;

	if ((_output_access_sem_shm = create_sem(1)) == NULL)
		return -1;
	return 0;
}


int output_close_file() {
	if (fclose(_fp) == EOF) {
		perror("Error closing output file");
		return -1;
	}
	return 0;
}


void output_write(char* msg) {
	sem_wait(_output_access_sem_shm);

	fprintf(_fp, "%d\t: %c %d\t: %s\n", (*_output_counter_shm)++, proc_info.type, proc_info.id, msg);

	sem_post(_output_access_sem_shm);
}


void output_write_c(char* msg, int adults, int children) {
	sem_wait(_output_access_sem_shm);

	fprintf(_fp, "%d\t: %c %d\t: %s : %d : %d\n", (*_output_counter_shm)++, proc_info.type, 
			proc_info.id, msg, adults, children);

	sem_post(_output_access_sem_shm);
}

/* end of output.c */