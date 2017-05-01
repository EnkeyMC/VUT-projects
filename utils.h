/**
 * @file 		utils.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	8.4.2017
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <semaphore.h>

/**
 * @brief      Parse integer arguments
 *
 * @param[in]  argc           Argument count
 * @param[in]  argv           Argument values
 * @param[out] args_out       Array of parsed arguments
 * @param[in]  args_out_size  Size of args_out (number of arguments that should be parsed)
 * @param[out] error_msg      The error message if error occures while parsing
 *
 * @return     Number of arguments successfuly parsed
 */
int parse_args(const int argc, const char* const argv[], 
	int* args_out, const int args_out_size, 
	char** error_msg);

/**
 * @brief      Check if value is within range of <min;max>
 *
 * @param[in]  value  The value
 * @param[in]  min    The minimum
 * @param[in]  max    The maximum
 *
 * @return     True if in range, False otherwise.
 */
bool is_in_range(const int value, const int min, const int max);

/**
 * @brief      Simple wrapper to ge semaphore value
 *
 * @param      sem   The semaphore
 *
 * @return     The semaphore value.
 */
int get_sem_val(sem_t* sem);

#endif // UTILS_H
/* end of utils.h */