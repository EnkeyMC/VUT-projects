/**
 * @file 		shared_mem.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	14.4.2017
 */
#ifndef SHARED_MEM_H
#define SHARED_MEM_H

/**
 * Dynamic array of shared memory addresses
 */
void** _shm_addrs;

/**
 * Length of shared memory addresses array
 */
int _shm_addrs_len;

/**
 * @brief      Create and attach shared memory.
 *
 * @param[in]  size  Size of the memory to allocate
 *
 * @return     pointer to created shared memory, NULL on error
 */
void* create_shm(size_t size);

/**
 * @brief      Clean created shared memory.
 *
 * @return     0 on success, -1 on error
 */
int clean_shm();

/**
 * @brief      Saves shared memory address.
 *
 *             Handles memory allocation and reallocation.
 *
 * @param      shm_addr  The shared memory address
 *
 * @return     0 on success, -1 on error
 */
int _save_addr(void* shm_addr);

#endif // SHARED_MEM_H
/* end of shared_mem.h */