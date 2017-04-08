/**
 * @file 		utils.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	8.4.2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "utils.h"

int parse_args(const int argc, const char* const argv[], 
	int* args_out, const int args_out_size, 
	char** error_msg) 
{
	if (argc - 1 != args_out_size) {
		*error_msg = "Invalid number of arguments.";
		return 0;
	}

	int parsed_arg_count = 0;
	long int tmp;  // variable to check output of strtol
	char* endptr;  // variable to check if strtol succeeded

	for (int i = 1; i < argc; ++i)
	{
		tmp = strtol(argv[i], &endptr, 10);

		// check if strtol failed
		if (*endptr != 0 || ((tmp == LONG_MAX || tmp == LONG_MIN) && errno == ERANGE)) {
			sprintf(*error_msg, "Argument %s is not a valid number or is out of range.", argv[i]);
			return parsed_arg_count;
		}

		// check if argument is not in range of int
		if (!is_in_range(tmp, INT_MIN, INT_MAX)) {
			sprintf(*error_msg, "Argument %s is out of range.", argv[i]);
			return parsed_arg_count;
		}

		args_out[parsed_arg_count] = (int) tmp;
		parsed_arg_count++;
	}

	return parsed_arg_count;
}


bool is_in_range(const int value, const int min, const int max) {
	return value >= min && value <= max;
}


/* end of utils.c */