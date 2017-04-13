/**
 * @file 		generators.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	9.4.2017
 */
#include <unistd.h>
#include <stdio.h>

#include "generators.h"
#include "process.h"

int create_generators(char** error_msg) {
	pid_t pid = fork();  // Create adult generator

	if (pid == 0) {  // Child process
		proc_info = get_proc_info(P_ADULT_GEN);
		return 0;
	} else if (pid == -1) {
		sprintf(*error_msg, "Error creating adult generator process.");
		return -1;
	}

	pid = fork();  // Create child generator

	if (pid == 0) {  // Child process
		proc_info = get_proc_info(P_CHILD_GEN);
		return 0;
	} else if (pid == -1) {
		sprintf(*error_msg, "Error creating child generator process.");
		return -1;
	}

	return 0;
}


int generate(int argc, int* args) { (void) argc; (void) args; return 0;}

/* end of generators.c */
