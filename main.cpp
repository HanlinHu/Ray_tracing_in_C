
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gmath.h"
#include "entities.h"
#include "light.h"
#include "ppm.h"
#include "rectangular.h"
#include "sphere.h"
#include "checker.h"
#include "raytracing.h"

#define DEFAULT_OUTPUT_FILE	"Test.ppm"

#define ROWS 512
#define COLS 512


int main(int argc, char **argv)
{
	/* Format light <double>
		{	
			color[3] ----> scale (0,1)
			lrp[3]
			intensity ip	
		}*/
	light light1 = {
		1.0, 1.0, 1.0,		
		-10.0, 10.0, 2.0,
		200.0
	};

	light light2 = {
		1.0, 0.0, 0.0,
		0.0, 10.0, 2.0,
		200.0
	};
	
	light light3 = {
		0.0, 1.0, 0.0,
		1.0, 2.0, 10.0,
		200.0
	};

	/* Format viewpoint <double>
		{
			vrp[3]
			vpn[3]
			vup[3]
		}*/
	viewpoint view ={
		//1.0, 2.0, 3.5, 
		//0.0, -1.0, -2.5, 
		//0.0, -1.0, 0.0
		
		//- 3.5, 2.0, 1.0,
		//2.5, -0.5, 0.0,
		//0.0, 1.0, 0.0,

		 -2.0, 4.0, 4.0,
		 4.0, -4.0, -4.0,
		 0.0, 4.0, 4.0

	};

	/* Format sphere
	{
		center [3]
		radius double
		object_fill
		{	color[3]
			kd double
			ks double
			double T;  T is transmittance (fraction of light passed per unit)
			double index_of_refraction;
			double phong_power;
		}
	}*/
	sphere sphere1 ={
		1.4, 0.3, 0.4,	//position
		0.3,			//radius
		1.0, 1.0, 1.0,//color
		0.75,
		0.5,
		0.2,
		20.0,
		5.0};

	sphere sphere2 = {
		0.0, 2.0, 1.0,	//position
		0.2,			//radius
		1.0, 1.0, 1.0,//color
		0.75,
		0.5,
		0.5,
		20.0,
		10.0 };

	sphere sphere3 = {
		0.0, 1.75, 0.1,	//position
		0.1,			//radius
		1.0, 1.0, 1.0,//color
		1.0,
		1.0,
		0.0,
		0.0,
		10.0 };

	sphere sphere4 = {
		0.2, 0.3, 1.6,	//position
		0.3,			//radius
		1.0, 1.0, 1.0,//color
		0.8,
		0.2,
		0.8,
		10.0,
		10.0 };


	/* Format rectangular
	{
		vertices[4][3];
		normal[3];
		object_fill
		{	color[3]
			kd double
			ks double
			double T;
			double index_of_refraction;
			double phong_power;
		}
	}*/
	rectangular rectangular1 ={
		0.0, 0.0, 0.0,	/* v0 */
		0.0, 0.0, 2.0,	/* v1 */
		2.0, 0.0, 2.0,	/* v2 */
		2.0, 0.0, 0.0,	/* v3 */
		0.0, 1.0, 0.0,
		0.1, 0.1, 0.1, /*colour*/
		0.8,		//kd
		0.5,		//ks
		0.0,		//T
		0.0,		//index
		5.0			//phong power
	};




	rectangular rectangular2 = {
		0.0, 0.0, 0.0,	/* v0 */
		0.0, 2.0, 0.0,	/* v1 */
		2.0, 2.0, 0.0,	/* v2 */
		2.0, 0.0, 0.0,	/* v3 */
		0.0, 0.0, 1.0,	/*normal*/
		0.1, 0.1, 0.1,	/*colour*/
		0.8,	
		0.5,
		0.0,
		0.0,
		5.0
	};

	rectangular rectangular3 = {
		2.0, 0.0, 0.0,	/* v0 */
		2.0, 0.0, 2.0,	/* v1 */
		2.0, 2.0, 2.0,	/* v2 */
		2.0, 2.0, 0.0,	/* v3 */
		-1.0, 0.0, 0.0,
		0.1, 0.1, 0.1,
		0.8,
		0.5,
		0.0,
		0.0,
		5.0
	};

	unsigned char *pixels;
	color background_color = {0.0, 0.1, 0.1};
	light_node lights = NULL;
	rectangular_node rectangulars = NULL;
	sphere_node spheres = NULL;
	
	append_light(&light1, &lights);
	append_light(&light2, &lights);
	append_light(&light3, &lights);
	
	append_sphere(&sphere1, &spheres);
	append_sphere(&sphere2, &spheres);
	append_sphere(&sphere3, &spheres);
	append_sphere(&sphere4, &spheres);
	
	append_rectangular(&rectangular1, &rectangulars);
	append_rectangular(&rectangular2, &rectangulars);
	append_rectangular(&rectangular3, &rectangulars);

	FILE *output = NULL;

	/* read input */

	/* allocate by the given resolution */
	pixels = (unsigned char*) malloc(sizeof(unsigned char)* ROWS
		* COLS * 3);

	/* do the ray tracing with the given geometry */
	raytracing(pixels, background_color, rectangulars, spheres, lights, &view, ROWS, COLS);

	output = fopen(DEFAULT_OUTPUT_FILE, "wb");
	/* write the results */
	write_ppm_file(output, pixels, ROWS, COLS);
	fclose(output);

	/* free the linked lists */
	delete_rectangular_list(&rectangulars);
	delete_sphere_list(&spheres);
	delete_light_list(&lights);
	return 0;
}
