/**
 * @file 		process.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	10.4.2017
 */

#include "process.h"
#include "generators.h"

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

			break;

		case P_CHILD:

			break;

		default:
			break;
	}
	return proc_info;
}

/* end of process.c */
