#ifndef __RAYTRACING_H
#define __RAYTRACING_H


#include "entities.h"
#include "sphere.h"
#include "rectangular.h"
#include "light.h"

void raytracing(unsigned char *pixels, color background_color,
	rectangular_node rectangulars, sphere_node spheres,
	light_node lights, const viewpoint *view,
	 int width,  int height);
#endif
