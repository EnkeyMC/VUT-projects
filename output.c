/**
 * @file 		output.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	16.4.2017
 */
#include <stdio.h>
#include <string.h>

#include "output.h"
#include "shared_mem.h"
#include "process.h"
#include "center.h"
#include "debug.h"


int setup_output_res() {
	// Clear output file
	FILE* fp = fopen(FILENAME, "w");

	if (fp == NULL) {
		fprintf(stderr, "Could not open file '%s'!\n", FILENAME);
		return -1;
	}

	if (fclose(fp) == EOF) {
		fprintf(stderr, "Could not close file '%s'!\n", FILENAME);
		return -1;
	}

	if ((_output_counter_shm = (int*) create_shm(sizeof(int))) == NULL) 
		return -1;
	*_output_counter_shm = 1;

	if ((_output_access_sem_shm = (sem_t*) create_shm(sizeof(sem_t))) == NULL)
		return -1;
	return sem_init(_output_access_sem_shm, 1, 1);
}


int output_write(char* msg) {
	debug(msg);
	sem_wait(_output_access_sem_shm);

	FILE* fp = fopen(FILENAME, "a");

	if (fp == NULL) {
		fprintf(stderr, "Could not open file '%s'!\n", FILENAME);
		return -1;
	}

	if (strcmp(msg, MSG_WAITING) == 0)
		fprintf(fp, "%d\t: %c %d\t: %s : %d : %d\n", (*_output_counter_shm)++, proc_info.type, 
			proc_info.id, msg, get_adult_count(), get_child_count());
	else
		fprintf(fp, "%d\t: %c %d\t: %s\n", (*_output_counter_shm)++, proc_info.type, proc_info.id, msg);

	if (fclose(fp) == EOF) {
		fprintf(stderr, "Could not close file '%s'!\n", FILENAME);
		return -1;
	}

	sem_post(_output_access_sem_shm);
	return 0;
}

/* end of output.c */