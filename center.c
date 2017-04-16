/**
 * @file 		center.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	16.4.2017
 */
#include <stdlib.h>
#include <errno.h>

#include "center.h"
#include "shared_mem.h"
#include "output.h"


int setup_center_res() {
	if ((_center_shm = (center_t*) create_shm(sizeof(center_t))) == NULL)
		return -1;

	if ((_center_sem_shm = (sem_t*) create_shm(sizeof(sem_t))) == NULL)
		return -1;

	if (sem_init(_center_sem_shm, 1, 0) == -1)
		return -1;

	if ((_center_enter_sem_shm = (sem_t*) create_shm(sizeof(sem_t))) == NULL)
		return -1;

	if (sem_init(_center_enter_sem_shm, 1, 1) == -1)
		return -1;

	if ((_center_info_sem_shm = (sem_t*) create_shm(sizeof(sem_t))) == NULL)
		return -1;

	return sem_init(_center_info_sem_shm, 1, 1);
}


void child_enter_center() {
	// wait if enter is blocked
	sem_wait(_center_enter_sem_shm);
	sem_post(_center_enter_sem_shm);

	if (sem_trywait(_center_sem_shm) == -1 && errno == EAGAIN){
		output_write(MSG_WAITING);
		sem_wait(_center_sem_shm);
	}
	output_write(MSG_ENTER);

	sem_wait(_center_info_sem_shm);
	(*_center_shm).nchild++;
	sem_post(_center_info_sem_shm);
}


void child_leave_center() {
	sem_wait(_center_info_sem_shm);
	(*_center_shm).nchild--;
	sem_post(_center_info_sem_shm);

	output_write(MSG_LEAVE);

	sem_post(_center_sem_shm);
}


void adult_enter_center() {
	// wait if entering is blocked
	sem_wait(_center_enter_sem_shm);
	sem_post(_center_enter_sem_shm);

	sem_wait(_center_info_sem_shm);
	(*_center_shm).nadult++;
	sem_post(_center_info_sem_shm);

	output_write(MSG_ENTER);

	for (int i = 0; i < CHILDREN_PER_ADULT; i++)
		sem_post(_center_sem_shm);
}


void adult_leave_center() {
	block_enter(true);
	output_write(MSG_TRYING);

	int i;
	for (i = 0; i < CHILDREN_PER_ADULT; i++) {
		output_write("Trying in loop");
		if (sem_trywait(_center_sem_shm) == -1 && errno == EAGAIN) {
			break;
		}
	}
	output_write("Tried");
	// If adult cannot leave, increment semaphore back
	if (i != CHILDREN_PER_ADULT) {
		output_write("Gotta wait");
		for (int j = 0; j < i; i++) {
			output_write("Unlocking");
			sem_post(_center_sem_shm);
		}

		output_write(MSG_WAITING);
		block_enter(false);
		// Wait again for children to leave
		for (i = 0; i < CHILDREN_PER_ADULT; i++) {
			output_write("Waiting...");
			sem_wait(_center_sem_shm);
		}

		output_write(MSG_LEAVE);
	} else {
		output_write(MSG_LEAVE);
		block_enter(false);
	}

	sem_wait(_center_info_sem_shm);
	(*_center_shm).nadult--;
	sem_post(_center_info_sem_shm);
}


void block_enter(bool block) {
	if (block)
		sem_wait(_center_enter_sem_shm);
	else
		sem_post(_center_enter_sem_shm);
}


int get_adult_count() {
	int count;
	sem_wait(_center_info_sem_shm);
	count = (*_center_shm).nadult;
	sem_post(_center_info_sem_shm);
	return count;
}


int get_child_count() {
	int count;
	sem_wait(_center_info_sem_shm);
	count = (*_center_shm).nchild;
	sem_post(_center_info_sem_shm);
	return count;
}


/* end of center.c */