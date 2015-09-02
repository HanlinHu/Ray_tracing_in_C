#include <stdlib.h>
#include <stdio.h>

#include "entities.h"
#include "sphere.h"



void append_sphere(const sphere *X, sphere_node *list)
{
	sphere_node tmp, newNode;

	/* allocate the node */
	newNode = (sphere_node)malloc(sizeof(struct snode));

	/* copy the struct elements */
	COPY_OBJECT_FILL(newNode->element.sphere_fill, X->sphere_fill);

	newNode->element.radius = X->radius;
	COPY_POINT3(newNode->element.center, X->center);

	newNode->next = NULL;
	if (*list == NULL) { /* nothing in it yet */
		*list = newNode;
	}
	else {
		/* move to the end */
		for (tmp = *list; tmp->next != NULL; tmp = tmp->next)
			;
		tmp->next = newNode;
	}
}


void delete_sphere_list(sphere_node *list)
{
	sphere_node pos, tmp;

	pos = *list;
	while (pos != NULL) {
		tmp = pos->next;
		free(pos);
		pos = tmp;
	}

	*list = NULL;
}
