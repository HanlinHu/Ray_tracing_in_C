#include <stdlib.h>
#include <stdio.h>

#include "entities.h"
#include "rectangular.h"



void append_rectangular(const rectangular *X, rectangular_node *list)
{
	rectangular_node tmp, newNode;
	int i;

	/* allocate the node */
	newNode =(rectangular_node)malloc(sizeof(struct rnode));

	/* copy the struct elements */
	COPY_OBJECT_FILL(newNode->element.rectangular_fill, X->rectangular_fill);

	for (i = 0; i < 4; i++) {
		COPY_POINT3(newNode->element.vertices[i], X->vertices[i]);

		COPY_POINT3(newNode->element.normal, X->normal);
	}

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


void delete_rectangular_list(rectangular_node *list)
{
	rectangular_node pos, tmp;

	pos = *list;
	while (pos != NULL) {
		tmp = pos->next;
		free(pos);
		pos = tmp;
	}

	*list = NULL;
}
