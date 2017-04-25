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
			sem_wait(_proc_sem_shm);
			proc_info.id = (*_adult_id_shm)++;
			proc_info.p_work = &adult_work;
			sem_post(_proc_sem_shm);
			break;

		case P_CHILD:
			sem_wait(_proc_sem_shm);
			proc_info.id = (*_child_id_shm)++;
			proc_info.p_work = &child_work;
			sem_post(_proc_sem_shm);
			break;

		default:
			break;
	}
}


int setup_proc_res() {
	if ((_adult_id_shm = (int*) create_shm(sizeof(int))) == NULL)
		return -1;
	*_adult_id_shm = 1;

	if ((_child_id_shm = (int*) create_shm(sizeof(int))) == NULL)
		return -1;
	*_child_id_shm = 1;

	if ((_proc_sem_shm = create_sem(1)) == NULL)
		return -1;
	return 0;
}

/* end of process.c */
