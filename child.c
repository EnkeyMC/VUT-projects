/**
 * @file 		child.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	14.4.2017
 */
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "child.h"
#include "center.h"
#include "output.h"

int child_work(int* args) {
	int sleep_time = args[5];

	output_write(MSG_STARTED);
	child_enter_center();

	// Simulate work in center
	if (sleep_time != 0) {
		srand(clock());
		usleep(rand() % (sleep_time * 1000));
	}

	output_write(MSG_TRYING);

	child_leave_center();

	return 0;
}

/* end of child.c */