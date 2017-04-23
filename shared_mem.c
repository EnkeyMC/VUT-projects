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

	if (shmctl(id, IPC_RMID, NULL) == -1) {
		return NULL;
	}

	_save_addr(mem);
	return mem;
}


int clean_shm() {
	int ret_code = 0;

	for (int i = 0; i < _shm_addrs_len; ++i)
	{
		if (shmdt(_shm_addrs[i]) == -1) {
			ret_code = -1;
		}
	}

	if (_shm_addrs != NULL)
		free(_shm_addrs);
	_shm_addrs = NULL;
	_shm_addrs_len = 0;

	return ret_code;
}


int _save_addr(void* shm_addr) {
	if (_shm_addrs == NULL) {
		_shm_addrs = (void**) malloc(sizeof(void*));

		if (_shm_addrs == NULL) {
			return -1;
		}

		_shm_addrs_len = 1;
	} else {
		_shm_addrs = (void**) realloc(_shm_addrs, sizeof(void*) * (++_shm_addrs_len));

		if (_shm_addrs == NULL) {
			return -1;
		}
	}

	_shm_addrs[_shm_addrs_len - 1] = shm_addr;
	return 0;
}

/* end of shared_mem.c */