#include <stdio.h>
#include <math.h>


#define PHI 1.61803398875 // Golden ratio
#define MAX_DEPTH 5       // Recursion depth limit

// Icosahedral face-center positions (normalized)

double OFFSETS[12][3] = {
    {0, 1, PHI}, {0, -1, -PHI}, {0, 1, -PHI}, {0, -1, PHI},
    {1, PHI, 0}, {-1, -PHI, 0}, {1, -PHI, 0}, {-1, PHI, 0},
    {PHI, 0, 1}, {-PHI, 0, -1}, {PHI, 0, -1}, {-PHI, 0, 1}
};

const double OFFSETS_6[6][3] = {
    {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}
};

const double OFFSETS_9[9][3] = {
    {0, 1, PHI}, {0, -1, -PHI}, {0, 1, -PHI},  
    {1, PHI, 0}, {-1, -PHI, 0}, {1, -PHI, 0}, 
    {PHI, 0, 1}, {-PHI, 0, -1}, {PHI, 0, -1}
};

const int OPPOSITE_INDEX[12] = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10};

void normalize_offsets(int num) {
		double length = sqrt(1 + PHI * PHI);
    for (int i = 0; i < num; i++) {
        /* double length = sqrt(OFFSETS[i][0] * OFFSETS[i][0] +
                             OFFSETS[i][1] * OFFSETS[i][1] +
                             OFFSETS[i][2] * OFFSETS[i][2]); */
        for (int j = 0; j < 3; j++) {
            ((double (*)[3])OFFSETS)[i][j] /= length;
        }
    }
}

double dot_product(double a[3], double b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Normalize a vector
void normalize(double v[3]) {
    double length = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    for (int i = 0; i < 3; i++) {
        v[i] /= length;
    }
}

void generate_sphereflake_9(double x, double y, double z, double radius, int depth, int parent_index, FILE *file) {
    if (depth > MAX_DEPTH || radius < 0.01) return;  // Base case

    // Print the sphere in RT format
    fprintf(file, "sp %.3f,%.3f,%.3f %.3f 220,155,43\n", x, y, z, radius * 2);

    // Compute new radius for child spheres
    double new_radius = radius / 3.0;
    double distance = radius + new_radius;

    // Place 9 child spheres
    for (int i = 0; i < 9; i++) {
        // Skip the child sphere that would overlap with the parent
        if (depth > 0 && parent_index != -1) {
            double dp = OFFSETS_9[i][0] * -OFFSETS_9[parent_index][0] +
                        OFFSETS_9[i][1] * -OFFSETS_9[parent_index][1] +
                        OFFSETS_9[i][2] * -OFFSETS_9[parent_index][2];

            if (dp > 0.99) continue;  // Skip the sphere opposite to the parent
        }

        double pos[3] = {
            x + OFFSETS_9[i][0] * distance,
            y + OFFSETS_9[i][1] * distance,
            z + OFFSETS_9[i][2] * distance
        };

        // Recursive call for the next level
        generate_sphereflake_9(pos[0], pos[1], pos[2], new_radius, depth + 1, i, file);
    }
}

// Recursive function to generate the sphereflake
void generate_sphereflake(double x, double y, double z, double radius, int depth, int parent_index, FILE *file) {
    if (depth > MAX_DEPTH || radius < 0.01) return;  // Base case: Stop recursion

    // Print the sphere in RT format
 //   printf("sp %.3f,%.3f,%.3f %.3f 220,155,43\n", x, y, z, radius * 2);
 fprintf(file, "sp %.3f,%.3f,%.3f %.3f 220,155,43\n", x, y, z, radius * 2);  // Write to file
    // Compute new radius for child spheres
    double new_radius = radius / 3.0;//2.0

    // Distance from parent sphere center to child sphere center
    double distance = radius + new_radius;

    // Place 12 child spheres
    for (int i = 0; i < 12; i++) {
       
	   if (depth >= 1 && parent_index != -1) {
            double dp = dot_product(OFFSETS[i], OFFSETS[parent_index]);
            if (dp < -0.95)  // Nearly opposite direction
                continue;}
	   
	   
	    double pos[3] = {OFFSETS[i][0], OFFSETS[i][1], OFFSETS[i][2]};
       // normalize(pos);  // Ensure unit length
        for (int j = 0; j < 3; j++) {
            pos[j] *= distance;  // Scale to correct tangent distance
        }

        // Recursive call for the next level
        generate_sphereflake(x + pos[0], y + pos[1], z + pos[2], new_radius, depth + 1, i, file);
    }
}

void generate_sphereflake_6(double x, double y, double z, double radius, int depth, int parent_index, FILE *file) {
    if (depth > MAX_DEPTH || radius < 0.01) return;  // Base case: Stop recursion

    // Write sphere to RT file (diameter = radius * 2)
    fprintf(file, "sp %.3f,%.3f,%.3f %.3f 220,155,43\n", x, y, z, radius * 2);

    // Compute new radius for child spheres
    double new_radius = radius / 2.2;
    double distance = radius + new_radius;  // Ensure perfect tangency

    for (int i = 0; i < 6; i++) {
        // Skip the sphere that would be placed in the "backward" direction
        if (depth >= 1 && parent_index != -1) {
            double dot_product = OFFSETS_6[i][0] * -OFFSETS_6[parent_index][0] +
                                 OFFSETS_6[i][1] * -OFFSETS_6[parent_index][1] +
                                 OFFSETS_6[i][2] * -OFFSETS_6[parent_index][2];

            if (dot_product > 0.9) continue;  // Skip the sphere in the opposite direction
        }

        // Calculate position
        double pos[3] = {OFFSETS_6[i][0], OFFSETS_6[i][1], OFFSETS_6[i][2]};
        for (int j = 0; j < 3; j++) {
            pos[j] *= distance;
        }

        // Recursive call, passing the current sphere's index as the parent index
        generate_sphereflake_6(x + pos[0], y + pos[1], z + pos[2], new_radius, depth + 1, i, file);
    }
}


int main() 
{
    FILE *file = fopen("rt_files/sphere_flake_9pnt.rt", "a");  // Open file in append mode
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
	// Start with the central sphere at the origin
	//normalize_offsets(12);
   // generate_sphereflake(0.0, 0.0, 0.0, 1.0, 0,  -1, file);

	normalize_offsets(9);
    generate_sphereflake_9(0.0, 0.0, 0.0, 1.0, 0, -1, file);//TRYTHIS

   // generate_sphereflake_6(0.0, 0.0, 0.0, 1.0, 0, -1, file);



    fclose(file);  // Close file
    
	//bvh sphere test grid below
/* 	double radius = .05;
	double dia = radius * 2;
	int lim = 10;
	int i, j;
	int k = -1;
	double x = -2;
	double y = -2;
	double z = -2;
	while (++k < lim)
	{
		j = -1;
		y = -2;
		while (++j < lim)
		{
			i = -1;
			x = -2;
			while (++i < lim)
			{
   				 printf("sp %.3f,%.3f,%.3f %.3f 220,155,43\n", x += dia, y, z, dia);

			}
			y += dia;
		}
		z += dia;
	} */
	return 0;
}