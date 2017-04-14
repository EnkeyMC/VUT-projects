/**
 * @file 		process.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	9.4.2017
 */

#ifndef PROCESS_H
#define PROCESS_H

#include <semaphore.h>

// Process type constants
#define P_MAIN 'M'
#define P_ADULT_GEN 'G'
#define P_CHILD_GEN 'g'
#define P_ADULT 'A'
#define P_CHILD 'C'

/**
 * Pointer to worker function type
 */
typedef int (*worker_func) (int argc, int* args);

/**
 * Structure that holds information about process.
 */
typedef struct
{
	unsigned int id; 		///< Unique identifier of the process
	char type; 				///< Process type (see constants P_MAIN, P_ADULT,...)
	worker_func p_work; 	///< Worker function
} process_t;

/**
 * Global variable to identify the process type
 */
process_t proc_info;

/**
 * Adult process id counter
 */
int* _adult_id_shm;

/**
 * Child process id counter
 */
int* _child_id_shm;

/**
 * POSIX semaphore used to access id counters stored in shared memory
 */
sem_t* _proc_sem_shm;

/**
 * @brief      Generate unique id for process type and setup process info
 *
 * @param[in]  type  Type of process (see constants P_MAIN, P_ADULT,...)
 */
void set_proc_info(char type);

/**
 * @brief      Setup process resources (shared memory and semaphores)
 *
 * @return     0 on success, -1 on error
 */
int setup_proc_res();

#endif // PROCESS_H
/* end of process.h */
