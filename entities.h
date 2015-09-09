#ifndef __ENTITIES_H
#define __ENTITIES_H

typedef double point3[3];

typedef double point4[3];

typedef double color[3];


typedef struct {
	color light_color;
	point3 position;
	double intensity;
} light;

/*
color RGB is in terms of 0.0 to 1.0.
Kd is the diffuse component, 
Ks the specular, 
T is transmittance (fraction of light passed per
unit)
phong_power is the Phong cosine power for highlights,
*/
typedef struct {
	color fill_color;
	double Kd;
	double Ks;
	double T;
	double index_of_refraction;
	double phong_power;
} object_fill;


typedef struct {
	point3 center;
	double radius;
	object_fill sphere_fill;
} sphere;

typedef struct{
	point3 vertices[4];
	point3 normal;
	object_fill rectangular_fill;
} rectangular;

typedef struct {
	point3 vrp;
	point3 vpn;
	point3 vup;
} viewpoint;

#define COPY(a,b) {a = b;}

#define COPY_OBJECT_FILL(a,b)	{ (a).fill_color[0] = (b).fill_color[0]; \
	(a).fill_color[1] = (b).fill_color[1]; \
	(a).fill_color[2] = (b).fill_color[2]; \
	(a).Kd = (b).Kd; \
	(a).Ks = (b).Ks; \
	(a).phong_power = (b).phong_power; \
	(a).T = (b).T; \
	(a).index_of_refraction = (b).index_of_refraction; }

#define COPY_POINT3(a,b)	{ (a)[0] = (b)[0]; \
	(a)[1] = (b)[1]; \
	(a)[2] = (b)[2]; }

#define COPY_POINT4(a,b)	{ (a)[0] = (b)[0]; \
	(a)[1] = (b)[1]; \
	(a)[2] = (b)[2]; \
	(a)[3] = (b)[3]; }

#define COPY_COLOR(a,b)		{ (a)[0] = (b)[0]; \
	(a)[1] = (b)[1]; \
	(a)[2] = (b)[2]; }

#define SET_COLOR(r,R,G,B)	{ (r)[0] = (R); \
	(r)[1] = (G); \
	(r)[2] = (B); }

#define SET_COLOR(r,R,G,B)	{ (r)[0] = (R); \
	(r)[1] = (G); \
	(r)[2] = (B); }

#define SET_POINT3(r,X,Y,Z)     { (r)[0] = (X); \
	(r)[1] = (Y); \
	(r)[2] = (Z); }

#define SET_POINT4(r,X,Y,Z,R)     { (r)[0] = (X); \
	(r)[1] = (Y); \
	(r)[2] = (Z); \
	(r)[3] = (R); }

#endif
