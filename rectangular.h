#ifndef __RECTANGULAR_H
#define __RECTANGULAR_H

#include "entities.h"


struct rnode;

typedef struct rnode *rectangular_node;

struct rnode {
	rectangular element;
	rectangular_node next;
};


void append_rectangular(const rectangular *X, rectangular_node *list);

void delete_rectangular_list(rectangular_node *list);


#endif
