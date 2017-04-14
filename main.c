/**
 * @file 		main.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	7.4.2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/wait.h>

#include "utils.h"
#include "process.h"
#include "generators.h"

#define ARG_COUNT 6

#define EXIT_ARG_ERR 1
#define EXIT_SYS_CALL_ERR 2

#define TIME_MAX 5000

/**
 * @brief      Check if arguments are in range
 *
 * @param      args  The arguments
 *
 * @return     True if all arguments are in range, false otherwise (Error message is printed to stderr)
 */
bool check_args(int* args) {
	for (int i = 0; i < 2; i++) {
		if (!is_in_range(args[i], 1, INT_MAX)) {
			fprintf(stderr, "Argument '%d' has to be greater than 0.\n", args[i]);
			return false;
		}
	}

	for (int i = 2; i < ARG_COUNT; i++) {
		if (!is_in_range(args[i], 0, TIME_MAX)) {
			fprintf(stderr, "Argument '%d' has to be in interval <%d;%d>.\n", args[i], 0, TIME_MAX);
			return false;
		}
	}
	return true;
}

/**
 * @brief      Entry point of application
 *
 * @param 	   argc  Argument count
 * @param      argv  Arguments passed from command line
 *
 * @return     Exit code
 */
int main(int argc, char const *argv[])
{
	// Executers: P_MAIN
	set_proc_info(P_MAIN);
	/*
	 * Array for parsed arguments
	 *  0: A je počet procesů adult; A > 0.
	 *  1: C je počet procesů child; C > 0.
	 *  2: AGT je maximální hodnota doby (v milisekundách), po které je generován nový proces adult; AGT >= 0 && AGT < 5001.
	 *  3: CGT je maximální hodnota doby (v milisekundách), po které je generován nový proces child; CGT >= 0 && CGT < 5001.
	 *  4: AWT je maximální hodnota doby (v milisekundách), po kterou proces adult simuluje činnost v centru; AWT >= 0 && AWT < 5001.
	 *  5: CWT je maximální hodnota doby (v milisekundách), po kterou proces child simuluje činnost v centru; CWT >= 0 && CWT < 5001.
	 */
	int arguments[ARG_COUNT];
	char* error_msg;

	int arg_parsed_count = parse_args(argc, argv, arguments, ARG_COUNT, &error_msg);
	
	if (arg_parsed_count != ARG_COUNT) {
		fprintf(stderr, "%s\n", error_msg);
		return EXIT_ARG_ERR;
	}

	if (!check_args(arguments)) {
		return EXIT_ARG_ERR;
	}

	int ret_code = create_generators(&error_msg);
	// Executers: P_MAIN, P_ADULT_GEN, P_CHILD_GEN
	if (ret_code == -1) {
		fprintf(stderr, "%s\n", error_msg);
		return EXIT_SYS_CALL_ERR;
	}

	// Start worker functions
	if (proc_info.p_work != NULL) {
		ret_code = (*proc_info.p_work)(ARG_COUNT, arguments);
		// Executers: All
		if (ret_code == -1) {
			fprintf(stderr, "Error generating child and adult processes.\n");
			return EXIT_SYS_CALL_ERR;
		}
	}

	if (proc_info.type == P_MAIN) {
		// Wait for generators to finish
		for (int i = 0; i < GENERATOR_COUNT; i++) {
			waitpid(get_gen_pids()[i], NULL, 0);
		}
	}

	printf("%c: exited\n", proc_info.type);

	return EXIT_SUCCESS;
} // main()

/* end of main.c */