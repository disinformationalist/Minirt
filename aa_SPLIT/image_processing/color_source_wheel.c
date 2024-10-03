#include "image_processing.h"
// trying to use interpolation
/* unsigned int	get_color_source(double x, double y, int *colors, int num_colors)// add linear interpolation here!!!!
{
	double raw_angle;
	int	angle;
	int interpolate;

	//int angle2;
	unsigned int color;
	//use ceil();

	raw_angle = (atan2(y, x) * 180.0 / M_PI + 360);
	
	angle = (int)floor(raw_angle + 360) % 360;

	interpolate = (int)(255 * (raw_angle - floor(raw_angle)));

	//angle2 = (int)ceil((atan2(y, x) * 180.0 / M_PI + 360) + 360) % 360;

	
	
	color = interpolate_two_colors(colors[(angle) % num_colors], colors[(angle + 1) % num_colors], interpolate);
	
	return (color);
	//return ((colors[(angle) % num_colors]));//add to angle + col_ to shift wheel around
} */

unsigned int	get_color_source(double x, double y, int *colors, int num_colors, int rot)
{
	//old version
	int	angle;

	//int segment;//

	angle = (int)floor((atan2(y, x) * 180.0 / M_PI + 360) + 360) % 360;
	
	//segment = angle / (360 / num_colors);
	//return ((colors[(angle) % num_colors]));//add to angle + col_ to shift wheel around
	return (colors[(angle + rot) % num_colors]);
}


//setting sections and interpolate, not much different.
/* unsigned int	get_color_source(double x, double y, int *colors, int num_colors)
{
	   double raw_angle;
    int angle;
    int section_size;
    int segment, next_segment;
    double fractional_part;
    unsigned int color;

    raw_angle = atan2(y, x) * 180.0 / M_PI;
    if (raw_angle < 0) {
        raw_angle += 360;
    }
    
    angle = (int)floor(raw_angle) % 360;

    section_size = 360 / num_colors;
    segment = angle / section_size;
    next_segment = (segment + 1) % num_colors;

    fractional_part = (raw_angle - (segment * section_size)) / section_size;

    color = interpolate_two_colors(colors[segment], colors[next_segment], fractional_part);

    return color;
	//return ((colors[(angle) % num_colors]));//add to angle + col_ to shift wheel around
} */