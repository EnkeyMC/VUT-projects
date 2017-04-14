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
#include "child.h"
#include "adult.h"
#include "shared_mem.h"


void set_proc_info(char type) {
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
			proc_info.p_work = &adult_work;
			break;

		case P_CHILD:
			proc_info.id = 0;
			proc_info.p_work = &child_work;
			break;

		default:
			break;
	}
}


int setup_proc_res() {
	if ((_adult_id_shm = create_shm(sizeof(int))) == NULL)
		return -1;
	*_adult_id_shm = 1;

	if ((_child_id_shm = create_shm(sizeof(int))) == NULL)
		return -1;
	*_child_id_shm = 1;

	if ((_proc_sem_shm = create_shm(sizeof(sem_t))) == NULL)
		return -1;

	return sem_init(_proc_sem_shm, 1, 1);
}

/* end of process.c */
