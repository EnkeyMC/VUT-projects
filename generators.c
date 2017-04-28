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
#include "shared_mem.h"
#include "debug.h"
#include "center.h"

int setup_generators_res() {
	if ((_adults_generated_shm = (int*) create_shm(sizeof(int))) == NULL)
		return -1;
	*_adults_generated_shm = 0;

	if ((_adults_to_generate_shm = (int*) create_shm(sizeof(int))) == NULL)
		return -1;
	*_adults_to_generate_shm = -1;

	if ((_childs_to_generate_shm = (int*) create_shm(sizeof(int))) == NULL)
		return -1;
	*_childs_to_generate_shm = -1;

	if ((_finished_proc_count_shm = (int*) create_shm(sizeof(int))) == NULL)
		return -1;
	*_finished_proc_count_shm = 0;

	if ((_gen_access_sem_shm = create_sem(1)) == NULL)
		return -1;

	if ((gen_notify_sem_shm = create_sem(0)) == NULL)
		return -1;

	if ((gen_let_finish_sem_shm = create_sem(0)) == NULL)
		return -1;

	return 0;
}


bool all_adults_generated() {
	bool ret_val;
	sem_wait(_gen_access_sem_shm);
	ret_val = (*_adults_generated_shm) == (*_adults_to_generate_shm);
	sem_post(_gen_access_sem_shm);
	return ret_val;
}


void adult_generated() {
	debug("Adult generated");
	sem_wait(_gen_access_sem_shm);
	(*_adults_generated_shm)++;
	sem_post(_gen_access_sem_shm);
}


void proc_finished() {
	sem_wait(_gen_access_sem_shm);
	(*_finished_proc_count_shm)++;
	sem_post(_gen_access_sem_shm);
}


bool all_proc_finished() {
	bool ret_val;
	sem_wait(_gen_access_sem_shm);
	ret_val = *_finished_proc_count_shm == *_adults_to_generate_shm + *_childs_to_generate_shm;
	sem_post(_gen_access_sem_shm);
	return ret_val;
}


int create_generators() {
	debugs("Creating generators");
	pid_t pid = fork();  // Create adult generator

	if (pid == 0) {  // Child process
		set_proc_info(P_ADULT_GEN);
		return 0;
	} else if (pid == -1) {
		perror("Error generating adult generator");
		return -1;
	} else {
		_gen_pids[0] = pid;
	}

	pid = fork();  // Create child generator

	if (pid == 0) {  // Child process
		set_proc_info(P_CHILD_GEN);
		return 0;
	} else if (pid == -1) {
		perror("Error generating child generator");
		return -1;
	} else {
		_gen_pids[1] = pid;
	}
	debugs("Generated");
	return 0;
}


int generate(int* args) {
	int fork_count;  // Amount of childs or adults to generate
	int max_wait_time;  // Max time (in miliseconds) to wait between generating
	debugs("Generating");
	if (proc_info.type == P_ADULT_GEN) {
		fork_count = args[0];
		sem_wait(_gen_access_sem_shm);
		*_adults_to_generate_shm = fork_count;
		sem_post(_gen_access_sem_shm);
		max_wait_time = args[2];
	} else {
		fork_count = args[1];
		sem_wait(_gen_access_sem_shm);
		*_childs_to_generate_shm = fork_count;
		sem_post(_gen_access_sem_shm);
		max_wait_time = args[3];
	}

	srand(clock() * getpid());  // Start random generator

	for (int i = 0; i < fork_count; i++) {
		if (max_wait_time != 0)
			usleep(rand() % (max_wait_time * 1000));  // Sleep for random time
		debugs("Generating...");
		pid_t pid = fork();

		if (pid == 0) {
			if (proc_info.type == P_ADULT_GEN)
				set_proc_info(P_ADULT);  // Set process type to Adult
			else
				set_proc_info(P_CHILD);  // Set process type to Child

			proc_info.p_work(args);  // Call worker function
			return 0;
		} else if (pid == -1) {
			perror("Error generating child or adult");
			return -1;
		}
	}

	if (proc_info.type == P_ADULT_GEN) { // Only 1 generator will synchronize finishing
		// wait till all processes finish
		debug("Waiting for generated processes");
		sem_wait(gen_notify_sem_shm);

		sem_wait(_gen_access_sem_shm);
		int nforks = *_adults_to_generate_shm + *_childs_to_generate_shm; // Number of all processes generated
		sem_post(_gen_access_sem_shm);

		// Let all generated processes to finish at once
		for (int i = 0; i < nforks; i++)
			sem_post(gen_let_finish_sem_shm);
	}

	for (int i = 0; i < fork_count; i++)
		wait(NULL);  // Wait for all generated processes to end

	return 0;
}


pid_t* get_gen_pids() {
	return _gen_pids;
}

/* end of generators.c */
