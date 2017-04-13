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

#define GENERATOR_COUNT 2

/**
 * Array of generator process IDs. Filled after calling create_generators().
 * Use get_gen_pids() to access it.
 */
pid_t _gen_pids[GENERATOR_COUNT];

/**
 * @brief      Create child and adult process generators.
 *
 * @param      error_msg  The error message if error occures
 *
 * @return     0 on success, -1 on sys call errors
 */
int create_generators(char** error_msg);

/**
 * @brief      Generates child or adult processes based on process type. Worker function for generating process.
 *
 * @param[in]  argc  Argument count
 * @param[in]  args  Already checked arguments of the program
 *
 * @return     0 on success, otherwise nonzero
 */
int generate(int argc, int* args);

/**
 * @brief      Get array of generator process IDs
 *
 * @return     Array of generator pids
 */
pid_t* get_gen_pids();

#endif // GENERATORS_H
/* end of generators.h */
