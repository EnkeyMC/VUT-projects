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


int child_enter_center(bool wait) {
	// wait if enter is blocked
	sem_wait(_center_enter_sem_shm);
	sem_post(_center_enter_sem_shm);

	if (wait) {
		sem_wait(_center_sem_shm);
	} else {
		if (sem_trywait(_center_sem_shm) == -1 && errno == EAGAIN)
			return 1;
	}

	sem_wait(_center_info_sem_shm);
	(*_center_shm).nchild++;
	sem_post(_center_info_sem_shm);
	return 0;
}


int child_leave_center() {
	sem_wait(_center_info_sem_shm);
	(*_center_shm).nchild--;
	sem_post(_center_info_sem_shm);

	sem_post(_center_sem_shm);
	return 0;
}


int adult_enter_center() {
	// wait if entering is blocked
	sem_wait(_center_enter_sem_shm);
	sem_post(_center_enter_sem_shm);

	sem_wait(_center_info_sem_shm);
	(*_center_shm).nadult++;
	sem_post(_center_info_sem_shm);

	for (int i = 0; i < CHILDREN_PER_ADULT; i++)
		sem_post(_center_sem_shm);
	return 0;
}


int adult_leave_center(bool wait) {
	int i;
	for (i = 0; i < CHILDREN_PER_ADULT; i++) {
		if (wait) {
			sem_wait(_center_sem_shm);
		} else {
			if (sem_trywait(_center_sem_shm) == -1 && errno == EAGAIN) {
				break;
			}
		}
	}

	// If adult cannot leave, increment semaphore back and return 1
	if (!wait && i != CHILDREN_PER_ADULT) {
		for (int j = 0; j < i; i++) {
			sem_post(_center_sem_shm);
		}
		return 1;
	}

	sem_wait(_center_info_sem_shm);
	(*_center_shm).nadult--;
	sem_post(_center_info_sem_shm);
	return 0;
}


void block_enter(bool block) {
	if (block)
		sem_wait(_center_enter_sem_shm);
	else
		sem_post(_center_enter_sem_shm);
}


/* end of center.c */