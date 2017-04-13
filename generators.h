/**
 * @file 		generators.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	9.4.2017
 */

#ifndef GENERATORS_H
#define GENERATORS_H

/**
 * @brief      Create child and adult process generators.
 *
 * @param      error_msg  The error message if error occures
 *
 * @return     0 on success, -1 on sys call errors
 */
int create_generators(char** error_msg);

/**
 * @brief      Generates child or adult processes based on process type
 *
 * @param[in]  argc  Argument count
 * @param[in]  args  The arguments
 *
 * @return     0 on success, otherwise nonzero
 */
int generate(int argc, int* args);

#endif // GENERATORS_H
/* end of generators.h */
