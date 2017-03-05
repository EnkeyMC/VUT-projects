//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
	this->pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{
	Element_t* ele = this->pHead;
	Element_t* nextEle;

	while (ele != NULL) {
		nextEle = ele->pNext;
		delete ele;
		ele = nextEle;
	}
}

void PriorityQueue::Insert(int value)
{
	Element_t* inserted = new Element_t;
	inserted->pNext = NULL;
	inserted->pPrev = NULL;
	inserted->value = value;

	if (this->pHead == NULL) {
		this->pHead = inserted;
	} else {
		Element_t* ele = this->pHead;
		
		while (ele != NULL) {
			if (ele->value >= value) {
				Element_t* prev = ele->pPrev;

				if (prev != NULL)
					prev->pNext = inserted;
				else
					this->pHead = inserted;
				
				ele->pPrev = inserted;
				inserted->pPrev = prev;
				inserted->pNext = ele;

				ele = NULL;  // END LOOP
			} else if (ele->pNext == NULL) {
				ele->pNext = inserted;
				inserted->pPrev = ele;

				ele = NULL;  // END LOOP
			} else {
				ele = ele->pNext;  // CONTINUE LOOP
			}
		}
	}
}

bool PriorityQueue::Remove(int value)
{
	Element_t* tmp = this->pHead;

	while (tmp != NULL) {
		if (tmp->value == value) {
			if (tmp->pPrev == NULL) {
				this->pHead = tmp->pNext;
			}else{
				tmp->pPrev->pNext = tmp->pNext;
			}

			if (tmp->pNext != NULL) {
				tmp->pNext->pPrev = tmp->pPrev;
			}

			delete tmp;

			return true;
		}

		tmp = tmp->pNext;
	}

    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	Element_t* tmp = this->pHead;

	while (tmp != NULL) {
		if (tmp->value == value) {
			return tmp;
		}

		tmp = tmp->pNext;
	}

    return NULL;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    return this->pHead;
}

/*** Konec souboru tdd_code.cpp ***/
