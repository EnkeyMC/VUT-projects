/**
 * @file 		debug.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	18.4.2017
 * 
 * Debugging functions
 */
#ifndef DEBUG_H
#define DEBUG_H

#include <semaphore.h>
#define DEBUG_FILE "proj2.debug"

int* _debug_counter_shm;
sem_t* _debug_sem_shm;

int setup_debug_res();
int debug(const char* msg);

#endif // DEBUG_H
/* end of debug.h */