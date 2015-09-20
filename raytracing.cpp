#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "entities.h"
#include "raytracing.h"


#define MAX_REFLECTION_BOUNCES	3
#define MAX_DISTANCE 1000000000000.0
#define MIN_DISTANCE 0.00001

/*
	Output: 1 means hit, otherwise 0; distance t; suface normal
	Input: e and d from Ray expression e + td; Sphere; 
*/

int raySphereIntersection(const point3 ray_e,
	const point3 ray_d, const sphere *sph, point3 surface_normal,
	double *t0, double *t1)
{
	double A, B, C, D;
	double distance;
	point3 S,p;
	subtract_vector(ray_e, sph->center, S);
	A = dot_product(ray_d, ray_d);
	B = -2.0*dot_product(S, ray_d);
	C = dot_product(S, S) - sph->radius * sph->radius;
	D = B*B - 4 * A*C;
	if (D >= 0)
	{
	int sign = (C < 0.0) ? 1 : -1;
	
	distance = (-B + sign*sqrt(D)) / 2.f;
	//if (distance < *t0 || distance > *t1)
	//	return 0;
	*t1 = distance;

	/* calculate the surface normal */
	/* p(t) = e + td */
	multiply_vector(ray_d, distance, p);
	add_vector(p, ray_e, p);

	subtract_vector(p, sph->center, surface_normal);
	multiply_vector(surface_normal, -1.0, surface_normal);
	normalize(surface_normal);

	return 1;
	}else
	return 0;
}

//Find max amplitude of Surface Normal
int findMax(double a[3])
{
	double temp, max;
	int i, index;
	index = 0;
	max = sqrt(SQUARE(a[0]));//max = abs(a[0]);
	for (i = 0; i < 3; i++)
	{
		temp = sqrt(SQUARE(a[i])); //temp = abs(a[i]);
		if (temp > max) {
			max = temp;
			index = i;
		}
	}
	return index;
}
//Get the projection plane by compare the x,y,z absolute value of surface normal
void findProjPlane(double a[3], double b[4][3], double d[4][2]) // Surface Normal
{
	int index;

	index = findMax(a);

	if (index == 0)
	{
		d[0][0] = b[0][1];
		d[0][1] = b[0][2];
		d[1][0] = b[1][1];
		d[1][1] = b[1][2];
		d[2][0] = b[2][1];
		d[2][1] = b[2][2];
		d[3][0] = b[3][1];
		d[3][1] = b[3][2];
	}
	else if (index == 1)
	{
		d[0][0] = b[0][0];
		d[0][1] = b[0][2];
		d[1][0] = b[1][0];
		d[1][1] = b[1][2];
		d[2][0] = b[2][0];
		d[2][1] = b[2][2];
		d[3][0] = b[3][0];
		d[3][1] = b[3][2];
	}
	else if (index == 2)
	{
		d[0][0] = b[0][0];
		d[0][1] = b[0][1];
		d[1][0] = b[1][0];
		d[1][1] = b[1][1];
		d[2][0] = b[2][0];
		d[2][1] = b[2][1];
		d[3][0] = b[3][0];
		d[3][1] = b[3][1];
	}
}
//Find projection point
void findProjPoint(double a[3], point3 p, double dp[2])
{
	int index;
	index = findMax(a);

	if (index == 0)
	{
		dp[0] = p[1];
		dp[1] = p[2];
	}
	else if (index == 1)
	{
		dp[0] = p[0];
		dp[1] = p[2];
	}
	else if (index == 2)
	{
		dp[0] = p[0];
		dp[1] = p[1];
	}
}

//PNPOLY - Point Inclusion in Polygon Test
//by W. Randolph Franklin (WRF)
//http://www.ecse.rpi.edu/~wrf/Research/Short_Notes/pnpoly.html
int pnpoly(int nvert, double *vertx, double *verty, double testx, double testy)
{
	int i, j, c = 0;
	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((verty[i]>testy) != (verty[j]>testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
			c = !c;
	}
	return c;
}

/*
	Output: 1 means hit, otherwise 0;
	Input: e and d from Ray expression e + td; sphere;
*/
int rayRectangularIntersection(const point3 ray_e,
	const point3 ray_d,  rectangular *rec, point3 surface_normal,
	double *t0, double *t1)
{

	double temp1, temp2, distance;
	double projectPlane[4][2];
	double projPoint[2];
	double normal[3];
	point3 e,d,p;
	double D;
	int c;
	D = -1 * (rec->normal[0] * rec->vertices[0][0] + rec->normal[1] * rec->vertices[0][1] + rec->normal[2] * rec->vertices[0][2]);  // Put Vertax #1 to calculate value of D
	temp1 = dot_product(rec->normal, ray_e) + D;
	temp2 = dot_product(rec->normal, ray_d);
	distance = -1 * (temp1 / temp2);

	if (distance < *t0 || distance > *t1)
		return 0;

	*t1 = distance;//The distance

	/*p = e + t*d*/
	COPY_POINT3(e, ray_e);
	COPY_POINT3(d, ray_d);
	multiply_vector(d, distance, p);
	add_vector(e, p,p);

	COPY_POINT3(surface_normal, rec->normal);
	COPY_POINT3(normal, rec->normal);
	
	findProjPoint(normal, p, projPoint);
	findProjPlane(normal, rec->vertices, projectPlane);

	double S0[4];
	double S1[4];
	S0[0] = projectPlane[0][0];
	S0[1] = projectPlane[1][0];
	S0[2] = projectPlane[2][0];
	S0[3] = projectPlane[3][0];

	S1[0] = projectPlane[0][1];
	S1[1] = projectPlane[1][1];
	S1[2] = projectPlane[2][1];
	S1[3] = projectPlane[3][1];

	c = pnpoly(4, S0, S1, projPoint[0], projPoint[1]);
	if ((temp2 > 0.0f) || (temp2 = 0.0f))
		return 0;  	// Ray and Polygon parallel, intersection rejection
	else
	{
		if (c != 0)
			return 1;	
		else
			return 0;
	}
}

/*
	Output: Local Color
	Input:  Light Color, Surface Color, Kd, Ks, diffuse, spectular
*/
static void localColor(color localColor,const double intensity ,const color light_color, double diffuse, double specular, const object_fill *fill)
{
	color ambi{ 0.1, 0.1, 0.1 }; 
	//color ambi{ 0.0, 0.0, 0.0 };
	color diff, spec, lightCo,surface;

	double ip;

	/* Local Color = ambient*surface + light * ( kd * surface * diffuse + ks * specular)*/
	
	/*The second part*/
	COPY_COLOR(diff, fill->fill_color);
	multiply_vector(diff, fill->Kd, diff);
	multiply_vector(diff, diffuse, diff);
	COPY_COLOR(lightCo, light_color);
	multiply_vectors(diff, lightCo, diff);
//	COPY(ip, intensity);
//	ip = intensity / 255;	//Because final pixel color multiple 255 in ray tracing function. 
//	multiply_vector(diff, ip, diff);

	/*The third part*/
	COPY_COLOR(spec, light_color);
	multiply_vector(spec, fill->Ks, spec);
	multiply_vector(spec, specular, spec);
//	multiply_vector(spec, ip, spec);

	/*The first part*/ /* add them together to a whole*/
	COPY_COLOR(surface, fill->fill_color);
	multiply_vectors(ambi,surface, ambi);
	add_vector(diff, ambi, diff);
	add_vector(diff, spec, diff);
	add_vector(localColor, diff, localColor);

}

/*
Output: diffuse and specular
Input:  direction of the ray into intersection d, direction of intersection to light, surface normal
*/
static void compute_specular_and_diffuse(double *diffuse, double *specular,
	const point3 d, const point3 l, const point3 n,
	double phong_pow)
{
	point3 h, d_copy, l_copy, middle, r;
	double tmp;

	/*Calculate vector to eye V*/
	COPY_POINT3(d_copy, d);
	multiply_vector(d_copy, -1, d_copy);
	normalize(d_copy);

	/*Calculate vector to light L*/
	COPY_POINT3(l_copy, l);
	multiply_vector(l_copy, -1, l_copy);
	normalize(l_copy);

	/*Calculate reflection direction R*/
	tmp = dot_product(n, l_copy);
	multiply_vector(n, tmp, middle);
	multiply_vector(middle, 2, middle);
	subtract_vector(middle, l_copy, r);
	normalize(r);

	/* diffuse = max(0, dot_product(n, -l)) */
	*diffuse = MAX(0, dot_product(n, l_copy));  

	/* specular = (dot_product(r, -d))^p */
	*specular = pow(MAX(0, dot_product(r, d_copy)), phong_pow); 
}


/*
	Output: direction of reflected ray r
	Input:  direction of primary ray into intersection d, surface normal at intersection n
*/
static void reflection(point3 r, const point3 d, const point3 n)
{
	/* r = d - 2(d . n)n */
	multiply_vector(n, -2.0 * dot_product(d, n), r);
	add_vector(r, d, r);
}

/*

*/
static void refraction(point3 t, const point3 d, const point3 n,
	double from, double to)
{
	/* t = (n_f(d - n(d.n)))/n_t - n*sqrt(1 - (n_f^2(1 - (d.n)^2))/n_t^2) */
	point3 tmp;
	double x;

	/* first part */
	multiply_vector(n, dot_product(d, n), tmp);
	subtract_vector(d, tmp, t);
	multiply_vector(t, from, t);
	multiply_vector(t, 1.0 / to, t);

	/* second part */
	x = sqrt(1 - ((SQUARE(from) * (1 - SQUARE(dot_product(d, n))))
		/ SQUARE(to)));
	multiply_vector(n, x, tmp);
	subtract_vector(t, tmp, t);
}


/*
	Output: distance t
	Input: a linked list of rectangulars, 
			a linked list of spheres,
			e and d from Ray expression e + td;
*/
static double ray_hit_object(const point3 e, const point3 d, double t0, double t1,
	 point3 normal, const rectangular_node rectangulars,
	rectangular_node *hit_rectangular, const rectangular_node last_rectangular,
	const sphere_node spheres, sphere_node *hit_sphere,
	const sphere_node last_sphere)
{
	rectangular_node rec;
	sphere_node sphere;
	/* set these to not hit */
	*hit_rectangular = NULL;
	*hit_sphere = NULL;

	for (rec = rectangulars; rec != NULL; rec = rec->next) {
		if (rec == last_rectangular)
			continue;

		if (rayRectangularIntersection(e, d, &(rec->element), normal,
			&t0, &t1))
			/* hit is closest so far */
			*hit_rectangular = rec;
	}

	/* check the spheres */
	for (sphere = spheres; sphere != NULL; sphere = sphere->next) {
		if (sphere == last_sphere)
			continue;

		if (raySphereIntersection(e, d, &(sphere->element), normal,
			&t0, &t1))
			*hit_sphere = sphere;
	}

	/* the sphere would have to have been closer */
	if (*hit_sphere)
		*hit_rectangular = NULL;

	return t1;
}

/*
	Output: direction of ray d;
	Input:  basic vectors of Mcw, index of pixel in image plane, vrp, vpn, vup;
*/
static void rayConstruction(point3 d, const point3 u, const point3 v,
	const point3 w, unsigned int i, unsigned int j,
	const viewpoint *view,unsigned int width,unsigned int height)
{
	double xmin = 0.0175;
	double ymin = -0.0175;
	double xmax = -0.0175;
	double ymax = 0.0175;
	double focal = 0.05;

	double u_s, v_s, w_s;
	point3 u_tmp, v_tmp, w_tmp, s;

	w_s = focal;
	u_s = ymax + ((ymin - ymax)
		* (float)i  / (width - 1));
	v_s = xmax + ((xmin - xmax)
		* (float)j/ (height - 1));

	/* s = e + u_s * u + v_s * v + w_s * w */
	multiply_vector(u, u_s, u_tmp);
	multiply_vector(v, v_s, v_tmp);
	multiply_vector(w, w_s, w_tmp);
	add_vector(view->vrp, u_tmp, s);
	add_vector(s, v_tmp, s);
	add_vector(s, w_tmp, s);

	/* p(t) = e + td = e + t(s - e) */
	subtract_vector(s, view->vrp, d);
	normalize(d);
}


/* e = eye position
* g = gaze direction
* t = view-up vector */
static void calculateBasisVectors(point3 u, point3 v, point3 w,
	const viewpoint *view)
{
	/* w  */
	COPY_POINT3(w, view->vpn);	
	normalize(w);

	/* u = (t x w) / (|t x w|) */
	cross_product(view->vup, w, u);
	normalize(u);

	/* v = w x u */
	cross_product(w, u, v);

	normalize(v);
}

/*
	protect color value overflow
*/
static void protect_color_overflow(color c)
{
	int i;
	for (i = 0; i < 3; i++)
	if (c[i] > 1.0)
		c[i] = 1.0;
}

/*
	Output: color, bounces_left
	Input: light, spheres, rectangulars, ray
*/
unsigned int ray_color(const point3 e, int t,
	const point3 d, const rectangular_node rectangulars,
	const sphere_node spheres, const light_node lights,
	color object_color, int bounces_left,
	const rectangular_node last_rectangular,
	const sphere_node last_sphere)
{
	rectangular_node hit_rec = NULL, light_hit_rec = NULL;
	sphere_node hit_sphere = NULL, light_hit_sphere = NULL;
	light_node light;
	double t1 = MAX_DISTANCE, diffuse, specular, index;
	point3 p, surface_normal, l, ignore_me, r;
	object_fill fill;

	color reflection_part;
	//color refraction_part;
	color refraction_part;
	/* might be a reflection ray, so check how many times we've bounced */
	if (bounces_left < 0) {
		SET_COLOR(object_color, 0.0, 0.0, 0.0);
		return 0;
	}

	/* check for intersection with a sphere or a rectangular */
	t1 = ray_hit_object(e, d, t, MAX_DISTANCE, surface_normal, rectangulars,
		&hit_rec, last_rectangular, spheres, &hit_sphere,
		last_sphere);

	if (!hit_rec && !hit_sphere)
		return 0;

	/* p = e + t*d */

	multiply_vector(d, t1, p);
	add_vector(e, p, p);

	/* pick the fill of the object that was hit */
	if (hit_rec)
		fill = hit_rec->element.rectangular_fill;
	else
		fill = hit_sphere->element.sphere_fill;

	/* assume it's a shadow */
	SET_COLOR(object_color, 0.0, 0.0, 0.0);

	for (light = lights; light != NULL; light = light->next) {
	/* calculate the intersection vector pointing at the light */
	subtract_vector(p, light->element.position, l);

	/* check for intersection with an object. use ignore_me
	* because we don't care about this normal */

	ray_hit_object(p, l, MIN_DISTANCE, MAX_DISTANCE, ignore_me,
		rectangulars, &light_hit_rec, hit_rec,
		spheres, &light_hit_sphere, hit_sphere);

	/* the light was blocked by any objects */
	if (light_hit_rec || light_hit_sphere)
		continue;
		compute_specular_and_diffuse(&diffuse, &specular, d, l,
			surface_normal, fill.phong_power);

		localColor(object_color, light->element.intensity, light->element.light_color,
			diffuse, specular, &fill);

		/* totalColor = localColor + Ks*reflection + T*refraction */
		/* Here I do reflection first, refraction maybe later*/

		if (fill.Ks > 0) {
			reflection(r, d, surface_normal);

			/* if we hit something, add the color
			* that's a result of that */
			if (ray_color(p, MIN_DISTANCE , r, rectangulars, spheres,
				light, reflection_part,
				bounces_left - 1,
				hit_rec, hit_sphere)) {
				multiply_vector(reflection_part, fill.Ks,
					reflection_part);
				add_vector(object_color, reflection_part,
					object_color);
			}
		}

		/* calculate refraction ray */
		if (fill.T > 0.0 && fill.index_of_refraction > 0.0) {
			index = 1.0;
			if (last_rectangular)
				index = last_rectangular->element.rectangular_fill.index_of_refraction;
			else if (last_sphere)
				index = last_sphere->element.sphere_fill.index_of_refraction;
			refraction(r, d, surface_normal, index,
				fill.index_of_refraction);

			multiply_vector(d, -t1, p);
			add_vector(e, p, p);

			if (ray_color(p, MIN_DISTANCE, r, rectangulars, spheres,
				lights, refraction_part,
				bounces_left - 1, hit_rec,
				hit_sphere)) {
				multiply_vector(refraction_part, fill.T,
					refraction_part);
				add_vector(object_color, refraction_part,
					object_color);
			}

		}
		protect_color_overflow(object_color);
	}
	return 1;
}

/*
	Output: ppm file written in binary
	Input:  width and height of image, viewpoint, index of pixels, background_color(this is not ambient light), rectangulars, spheres, lights
*/
void raytracing(unsigned char *pixels, color background_color,
	rectangular_node rectangulars, sphere_node spheres,
	light_node lights, const viewpoint *view,
	int width, int height)
{
	int i, j;
	point3 u, v, w, d;
	color object_color = { 0.0, 0.0, 0.0 };

	/* calculate u, v, w */
	calculateBasisVectors(u, v, w, view);

	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			rayConstruction(d, u, v, w, i, j, view, width, height);
			/*
				The first node is NULL
				Output pixels with PPM format
			*/
			if (ray_color(view->vrp, 0.0, d, rectangulars, spheres,
				lights, object_color,
				MAX_REFLECTION_BOUNCES, NULL,
				NULL)) {
				pixels[((i + (j*width)) * 3) + 0] = object_color[0] * 255;
				pixels[((i + (j*width)) * 3) + 1] = object_color[1] * 255;
				pixels[((i + (j*width)) * 3) + 2] = object_color[2] * 255;
			}
			else {
				pixels[((i + (j*width)) * 3) + 0] = background_color[0] * 255;
				pixels[((i + (j*width)) * 3) + 1] = background_color[1] * 255;
				pixels[((i + (j*width)) * 3) + 2] = background_color[2] * 255;
			}
		}
	}
}
