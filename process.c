/**
 * @file 		process.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	10.4.2017
 */
#include <stdlib.h>

#include "process.h"
#include "generators.h"


int foo (int argc, int* args) {(void) argc; (void) args; return 0;}

process_t get_proc_info(char type) {
	process_t proc_info;
	proc_info.type = type;

	switch (type) {
		case P_MAIN:
			proc_info.id = 0;
			proc_info.p_work = NULL;
			break;

		case P_ADULT_GEN:
		case P_CHILD_GEN:
			proc_info.id = 0;
			proc_info.p_work = &generate;
			break;

		case P_ADULT:
			proc_info.id = 0;
			proc_info.p_work = &foo;
			break;

		case P_CHILD:
			proc_info.id = 0;
			proc_info.p_work = &foo;
			break;

		default:
			break;
	}
	return proc_info;
}

/* end of process.c */
