/**
 * @file 		center.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	16.4.2017
 */
#ifndef CENTER_H
#define CENTER_H

#include <semaphore.h>
#include <stdbool.h>

#define CHILDREN_PER_ADULT 3

/**
 * Structure that holds amount of adults and childs in center
 */
typedef struct
{
	int nadult;  ///< Number of adults in center
	int nchild;  ///< Number of childs in center
} center_t;

/**
 * Global variable in shared memory holding center information
 */
center_t* _center_shm;

/**
 * Global semaphore in shared memory to access information about center
 */
sem_t* _center_info_sem_shm;

/**
 * Global semaphore in shared memory to access center
 */
sem_t* _center_sem_shm;

/**
 * Semaphore to block entering to center
 */
sem_t* _center_enter_sem_shm;

/**
 * @brief      Allocates shared memory and creates semaphores
 *
 * @return     0 on success, -1 on error
 */
int setup_center_res();

/**
 * @brief      Try to enter the center as a child
 *
 * @param[in]  wait  Whether to wait or not if child can't enter now
 *
 * @return     0 on success, -1 on error, 1 child cannot enter now
 */
int child_enter_center(bool wait);

/**
 * @brief      Leave the center as a child
 *
 * @return     0 on success, -1 on error
 */
int child_leave_center();

/**
 * @brief      Enter center as an adult
 *
 * @return     0 on success, -1 on error
 */
int adult_enter_center();

/**
 * @brief      Leave the cetner as an adult
 *
 * @param[in]  wait  Whether to wait or not if adult can't leave now
 *
 * @return     0 on success, -1 on error, 1 adult cannot leave now
 */
int adult_leave_center(bool wait);

/**
 * @brief      Set blocking of other processes from entering center
 *
 * @param[in]  block  Set blocking to true or false
 */
void block_enter(bool block);

#endif // CENTER_H
/* end of center.h */