#include <stdlib.h>
#include <stdio.h>

#include "entities.h"
#include "light.h"


void append_light(const light *X, light_node *list)
{
	light_node tmp, newNode;

	/* allocate the lnode */
	newNode = (light_node)malloc(sizeof(struct lnode));

	COPY_POINT3(newNode->element.position, X->position);
	COPY_COLOR(newNode->element.light_color, X->light_color);
	newNode->element.intensity = X->intensity;

	/* insert it */
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


void delete_light_list(light_node *list)
{
	light_node pos, tmp;

	pos = *list;
	while (pos != NULL) {
		tmp = pos->next;
		free(pos);
		pos = tmp;
	}

	*list = NULL;
}
