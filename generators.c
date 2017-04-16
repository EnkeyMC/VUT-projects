/**
 * @file 		generators.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	9.4.2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#include "generators.h"
#include "process.h"

int create_generators(char** error_msg) {
	pid_t pid = fork();  // Create adult generator

	if (pid == 0) {  // Child process
		set_proc_info(P_ADULT_GEN);
		return 0;
	} else if (pid == -1) {
		sprintf(*error_msg, "Error creating adult generator process.");
		return -1;
	} else {
		_gen_pids[0] = pid;
	}

	pid = fork();  // Create child generator

	if (pid == 0) {  // Child process
		set_proc_info(P_CHILD_GEN);
		return 0;
	} else if (pid == -1) {
		sprintf(*error_msg, "Error creating child generator process.");
		return -1;
	} else {
		_gen_pids[1] = pid;
	}

	return 0;
}


int generate(int* args) {
	int fork_count;  // Amount of childs or adults to generate
	int max_wait_time;  // Max time (in miliseconds) to wait between generating

	if (proc_info.type == P_ADULT_GEN) {
		fork_count = args[0];
		max_wait_time = args[2];
	} else {
		fork_count = args[1];
		max_wait_time = args[3];
	}

	srand(clock());  // Start random generator

	for (int i = 0; i < fork_count; i++) {
		if (max_wait_time != 0)
			usleep(rand() % (max_wait_time * 1000));  // Sleep for random time

		pid_t pid = fork();

		if (pid == 0) {
			if (proc_info.type == P_ADULT_GEN)
				set_proc_info(P_ADULT);  // Set process type to Adult
			else
				set_proc_info(P_CHILD);  // Set process type to Child

			(*proc_info.p_work)(args);  // Call worker function
			return 0;
		} else if (pid == -1) {
			return -1;
		}
	}

	for (int i = 0; i < fork_count; i++)
		wait(NULL);  // Wait for all generated processes to end

	return 0;
}


pid_t* get_gen_pids() {
	return _gen_pids;
}

/* end of generators.c */
