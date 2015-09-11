#ifndef __LIGHT_H
#define __LIGHT_H
#include "entities.h"
//typedef light light;

struct lnode;

typedef struct lnode *light_node;

struct lnode {
	light element;
	light_node next;
};


void append_light(const light *X, light_node *list);

void delete_light_list(light_node *list);


#endif
