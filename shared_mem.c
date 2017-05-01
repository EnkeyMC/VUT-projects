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
#include "output.h"


void* create_shm(size_t size) {
	int id = shmget(IPC_PRIVATE, size, IPC_CREAT | IPC_EXCL | 0666);

	if (id == -1) {
		perror("Error creating shared memory");
		return NULL;
	}

	void* mem = shmat(id, NULL, 0);

	if (mem == (void*) -1) {
		perror("Error attaching shared memory");
		return NULL;
	}

	if (shmctl(id, IPC_RMID, NULL) == -1) {
		perror("Error marking shared memory to remove");
		return NULL;
	}

	_save_addr(mem);
	return mem;
}


sem_t* create_sem(int init_value) {
	sem_t* sem;
	if ((sem = (sem_t*) create_shm(sizeof(sem_t))) == NULL)
		return NULL;

	if (sem_init(sem, 1, init_value) == -1) {
		perror("Error initializing semaphore");
		return NULL;
	}

	_save_sem_addr(sem);
	return sem;
}


int clean_shm() {
	int ret_code = 0;

	for (int i = 0; i < _sem_addrs_len; ++i)
	{
		if (sem_destroy(_sem_addrs[i]) == -1) {
			perror("Error destroying semaphores");
			ret_code = -1;
		}
	}

	for (int i = 0; i < _shm_addrs_len; ++i)
	{
		if (shmdt(_shm_addrs[i]) == -1) {
			perror("Error detaching shared memory");
			ret_code = -1;
		}
	}

	if (_shm_addrs != NULL)
		free(_shm_addrs);
	_shm_addrs = NULL;
	_shm_addrs_len = 0;

	if (_sem_addrs != NULL)
		free(_sem_addrs);
	_sem_addrs = NULL;
	_sem_addrs_len = 0;

	if (output_close_file() == -1)
		ret_code = -1;

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


int _save_sem_addr(sem_t* sem_addr) {
	if (_sem_addrs == NULL) {
		_sem_addrs = (sem_t**) malloc(sizeof(sem_t*));

		if (_sem_addrs == NULL) {
			return -1;
		}

		_sem_addrs_len = 1;
	} else {
		_sem_addrs = (sem_t**) realloc(_sem_addrs, sizeof(sem_t*) * (++_sem_addrs_len));

		if (_sem_addrs == NULL) {
			return -1;
		}
	}

	_sem_addrs[_sem_addrs_len - 1] = sem_addr;
	return 0;
}

/* end of shared_mem.c */