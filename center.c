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
#include "debug.h"
#include "generators.h"


int setup_center_res() {
	if ((_center_shm = (center_t*) create_shm(sizeof(center_t))) == NULL)
		return -1;

	if ((_center_sem_shm = create_sem(0)) == NULL)
		return -1;

	if ((_center_enter_sem_shm = create_sem(1)) == NULL)
		return -1;

	if ((_center_info_sem_shm = create_sem(1)) == NULL)
		return -1;

	if ((_center_transaction_sem_shm = create_sem(1)) == NULL) 
		return -1;

	return 0;
}


void child_enter_center() {
	block_enter(true);
	block_enter(false);

	if (get_child_count() > 3 * get_adult_count()) {
		output_write(MSG_WAITING);
		sem_wait(_center_sem_shm);
		output_write(MSG_ENTER);
	} else {
		sem_wait(_center_sem_shm);
		output_write(MSG_ENTER);
	}
}


void child_leave_center() {
	sem_post(_center_sem_shm);
	output_write(MSG_LEAVE);
}


void adult_enter_center() {
	block_enter(true);
	block_enter(false);

	sem_wait(_center_info_sem_shm);
	_center_shm->nadult++;
	sem_post(_center_info_sem_shm);

	for (int i = 0; i < CHILDREN_PER_ADULT; ++i)
	{
		sem_post(_center_sem_shm);
	}

	output_write(MSG_ENTER);
}


void adult_leave_center() {
	block_enter(true);
	output_write(MSG_TRYING);

	if (get_child_count() > 3 * (get_adult_count() - 1)) {
		output_write(MSG_WAITING);

		block_enter(false);
		sem_wait(_center_info_sem_shm);
		for (int i = 0; i < CHILDREN_PER_ADULT; ++i)
		{
			sem_wait(_center_sem_shm);
		}

		_center_shm->nadult--;
		sem_post(_center_info_sem_shm);
		output_write(MSG_LEAVE);
	} else {
		sem_wait(_center_info_sem_shm);
		for (int i = 0; i < CHILDREN_PER_ADULT; ++i)
		{
			sem_wait(_center_sem_shm);
		}

		_center_shm->nadult--;
		sem_post(_center_info_sem_shm);
		output_write(MSG_LEAVE);
		block_enter(false);
	}
}


void block_enter(bool block) {
	if (block){
		debug("blocking enter");
		sem_wait(_center_enter_sem_shm);
		debug("entered");
	}
	else {
		debug("unblocking enter");
		sem_post(_center_enter_sem_shm);
	}
}


int get_adult_count() {
	int count;
	sem_wait(_center_info_sem_shm);
	count = _center_shm->nadult;
	sem_post(_center_info_sem_shm);
	return count;
}


int get_child_count() {
	int count;
	sem_getvalue(_center_sem_shm, &count);
	count = get_adult_count() * 3 - count;
	count = count < 0 ? 0 : count;
	return count;
}


/* end of center.c */