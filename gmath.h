#define QUARTER(x) (x / 4.0)

#define SQUARE(x) (x * x)

#define MAX(a, b) (a > b ? a : b)

#define MIN(a, b) (a < b ? a : b)

void normalize(double *v);

void cross_product(const double *v1, const double *v2, double *out);

void normal_cross_product(const double *v1, const double *v2, double *out);

void triangle_normal_vector(double *vn, const double *a, const double *b,
	const double *c);

double point_in_triangle(const double *v1, const double *v2, const double *v3, double *p);

double dot_product(const double *v1, const double *v2);

void add_vector(const double *a, const double *b, double *out);

void subtract_vector(const double *a, const double *b, double *out);

void multiply_vectors(const double *a, const double *b, double *out);

void multiply_vector(const double *a, double b, double *out);

void divide_vector(const double *a, double b, double *out);

double vectors_equal(const double *a, const double *b);

double podouble_in_triangle(const double *v1, const double *v2, const double *v3, double *p);

double vector_length(const double *a);

void calculate_y_of_podouble_in_triangle(const double *v1, const double *v2,
	const double *v3, double *p);

double angle_between_vectors(const double *a, const double *b);

double radians_to_degrees(double radians);

double degrees_to_radians(double degrees);

void rotate_vector(double angle, const double *a, double *out);

double distance_between_points(double x1, double y1, double x2, double y2);

#endif
