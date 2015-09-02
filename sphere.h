#ifndef __SPHERE_H
#define __SPHERE_H

#include "entities.h"


struct snode;

typedef struct snode *sphere_node;

struct snode {
	sphere element;
	sphere_node next;
};


void append_sphere(const sphere *X, sphere_node *list);

void delete_sphere_list(sphere_node *list);


#endif
