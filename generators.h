/**
 * @file 		generators.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	9.4.2017
 */

#ifndef GENERATORS_H
#define GENERATORS_H

#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>

#define GENERATOR_COUNT 2

/**
 * Indicator if all adults were generated
 */
bool* _adults_generated_shm;

/**
 * Semaphore to access _adults_generated_shm
 */
sem_t* _gen_access_sem_shm;

/**
 * Array of generator process IDs. Filled after calling create_generators().
 * Use get_gen_pids() to access it.
 */
pid_t _gen_pids[GENERATOR_COUNT];

/**
 * @brief      Allocate shared memory and semaphores
 *
 * @return     0 on success, -1 on error
 */
int setup_generators_res();

/**
 * @brief      Are all adults generated
 *
 * @return     Whether all adults are already genereated
 */
bool all_adults_generated();

/**
 * @brief      Create child and adult process generators.
 *
 * @param      error_msg  The error message if error occures
 *
 * @return     0 on success, -1 on sys call errors
 */
int create_generators(char** error_msg);

/**
 * @brief      Generates child or adult processes based on process type. Worker
 *             function for generating process.
 *
 * @param[in]  args  Already checked arguments of the program
 *
 * @return     0 on success, -1 on sys call errors
 */
int generate(int* args);

/**
 * @brief      Get array of generator process IDs
 *
 * @return     Array of generator pids
 */
pid_t* get_gen_pids();

#endif // GENERATORS_H
/* end of generators.h */
