/**
 * @file 		child.h
 * @package 	IOS_project_2
 * @author 		Martin Omacht
 * @email 		xomach00@stud.fit.vutbr.cz
 * @date    	14.4.2017
 */
#ifndef CHILD_H
#define CHILD_H

/**
 * @brief      Child process worker function. Simulates entering and leaving
 *             center.
 *
 * @param[in]  args  Already checked arguments of the program
 *
 * @return     0 on success, otherwise nonzero
 */
int child_work(int* args);

#endif // CHILD_H
/* end of child.h */