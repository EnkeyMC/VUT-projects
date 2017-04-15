/**
 * @file 		shared_mem.c
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	14.4.2017
 */

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

#include "shared_mem.h"


void* create_shm(size_t size) {
	int id = shmget(IPC_PRIVATE, size, IPC_CREAT | IPC_EXCL | 0666);

	if (id == -1) {
		return NULL;
	}

	void* mem = shmat(id, NULL, 0);

	if (mem == (void*) -1) {
		return NULL;
	}
	_save_id(id);
	return mem;
}


int clean_shm() {
	int ret_code = 0;

	for (int i = 0; i < _shm_ids_len; ++i)
	{
		if (shmctl(_shm_ids[i], IPC_RMID, NULL) == -1) {
			ret_code = -1;
		}
	}

	if (_shm_ids != NULL)
		free(_shm_ids);
	_shm_ids = NULL;
	_shm_ids_len = 0;

	return ret_code;
}


int _save_id(int id) {
	if (_shm_ids == NULL) {
		_shm_ids = (int*) malloc(sizeof(int));

		if (_shm_ids == NULL) {
			return -1;
		}

		_shm_ids_len = 1;
	} else {
		_shm_ids = (int*) realloc(_shm_ids, sizeof(int) * (++_shm_ids_len));

		if (_shm_ids == NULL) {
			return -1;
		}
	}

	_shm_ids[_shm_ids_len - 1] = id;
	return 0;
}

/* end of shared_mem.c */