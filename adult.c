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

int adult_work(int* args) {
	int sleep_time = args[4];

	output_write(MSG_STARTED);
	// Enter center
	adult_enter_center();

	// Simulate work in center
	if (sleep_time != 0) {
		srand(clock());
		usleep(rand() % (sleep_time * 1000));
	}

	adult_leave_center();

	adult_generated();  // Notify generator that adult has been generated

	return 0;
}

/* end of adult.c */