/**
 * @file 		debug.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	18.4.2017
 * 
 * Debugging functions
 */
#include <stdio.h>
#include <sys/timeb.h>

#include "debug.h"
#include "shared_mem.h"
#include "process.h"
#include "center.h"

int setup_debug_res() {

#ifdef DEBUG
	// Clear output file
	FILE* fp = fopen(DEBUG_FILE, "w");

	if (fp == NULL) {
		fprintf(stderr, "Could not open file '%s'!\n", DEBUG_FILE);
		return -1;
	}

	if (fclose(fp) == EOF) {
		fprintf(stderr, "Could not close file '%s'!\n", DEBUG_FILE);
		return -1;
	}

	if ((_debug_counter_shm = (int*) create_shm(sizeof(int))) == NULL) 
		return -1;
	*_debug_counter_shm = 1;

	if ((_debug_sem_shm = create_shm(1)) == NULL)
		return -1;
	return 0;

#else
	return 0;
#endif
}

int debug(const char* msg) {

#ifdef DEBUG
	sem_wait(_debug_sem_shm);

	FILE* fp = fopen(DEBUG_FILE, "a");

	if (fp == NULL) {
		fprintf(stderr, "Could not open file '%s'!\n", DEBUG_FILE);
		return -1;
	}

	struct timeb time;
	ftime(&time);

	fprintf(fp, "%8d:%03d - %d\t: %c %d\t: %s : %d : %d\n", (int)time.time, time.millitm, (*_debug_counter_shm)++, proc_info.type, 
			proc_info.id, msg, get_adult_count(), get_child_count());

	if (fclose(fp) == EOF) {
		fprintf(stderr, "Could not close file '%s'!\n", DEBUG_FILE);
		return -1;
	}

	sem_post(_debug_sem_shm);
	return 0;
#else
	(void) msg;
	return 0;
#endif
}

/* end of debug.c */