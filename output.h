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
#include <stdio.h>

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
 * Output file pointer
 */
FILE* _fp;

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
 * @brief      Close output file
 *
 * @return     0 on success, -1 on error
 */
int output_close_file();

/**
 * @brief      Write message to output file
 *
 * @param      msg   The message (MSG_* constants)
 */
void output_write(char* msg);

/**
 * @brief      Write message to output file with adult and child count
 *
 * @param      msg       The message
 * @param[in]  adults    Number of adults
 * @param[in]  children  Number of children
 */
void output_write_c(char* msg, int adults, int children);

#endif // OUTPUT_H
/* end of output.h */