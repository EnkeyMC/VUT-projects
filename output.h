/**
 * @file 		output.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	16.4.2017
 */
#ifndef OUTPUT_H
#define OUTPUT_H

#include <semaphore.h>

#define FILENAME "proj2.out"

#define MSG_STARTED "started"
#define MSG_ENTER "enter"
#define MSG_TRYING "trying to leave"
#define MSG_LEAVE "leave"
#define MSG_WAITING "waiting"
#define MSG_FINISHED "finished"

/**
 * Semaphore to control access to output
 */
sem_t* _output_access_sem_shm;

/**
 * Counter in shared memory
 */
int* _output_counter_shm;

/**
 * @brief      Allocate shared memory and create semaphores
 *
 * @return     0 on success, -1 on error
 */
int setup_output_res();

/**
 * @brief      Write message to output file
 *
 * @param      msg   The message (MSG_* constants)
 *
 * @return     0 on success, -1 on error
 */
int output_write(char* msg);

#endif // OUTPUT_H
/* end of output.h */