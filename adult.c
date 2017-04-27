/**
 * @file 		adult.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	14.4.2017
 */
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "adult.h"
#include "center.h"
#include "output.h"
#include "generators.h"
#include "debug.h"
#include "process.h"

int adult_work(int* args) {
	debugf("A%d: Working", proc_info.id);
	int sleep_time = args[4];

	output_write(MSG_STARTED);
	// Enter center
	adult_enter_center();

	// Simulate work in center
	if (sleep_time != 0) {
		srand(clock() * getpid());
		usleep(rand() % (sleep_time * 1000));
	}

	adult_leave_center();

	adult_generated();  // Notify generator that adult has been generated

	// Open center semaphore to prevent deadlocks
	if (all_adults_generated()) {
		for (int i = 0; i < *_childs_to_generate_shm; i++) {
			debug("post");
			sem_post(_center_sem_shm);
		}
	}

	// Let generators know you are ready to finish
	proc_finished(); 
	sem_post(gen_notify_sem_shm);
	// Wait till you can leave
	sem_wait(gen_let_finish_sem_shm);
	// Finish
	output_write(MSG_FINISHED);

	return 0;
}

/* end of adult.c */