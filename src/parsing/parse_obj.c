#include "minirt.h"


//v -0.5  0.0 -0.5
//f 1 3 4

//test print

void print_mesh(t_mesh *mesh)//can use with sn to build obj file
{
    int	i;
	
	i = -1;
	// Print all vertices in OBJ format
    while (++i < mesh->num_verts)
        printf("v %f %f %f\n", mesh->verts[i].x, mesh->verts[i].y, mesh->verts[i].z);

    // Print all faces in OBJ format
    i = -1;
	while (++i < mesh->num_tris)
    {
		printf("f %d %d %d\n", mesh->triangles[i].v0 + 1, mesh->triangles[i].v1 + 1, mesh->triangles[i].v2 + 1);
		print_vec(mesh->triangles[i].norm);
	}
}


void	free_mesh(t_mesh *mesh)
{
	free(mesh->triangles);
	free(mesh->verts);
	free(mesh);
}

//count verts of a face for poly handle
int	count_verts(char ***obj, int k)
{
	int count;

	count = 0;
	while (obj[k][count + 1])
		count++;
	return (count);
}

t_mesh	*malloc_mesh(int verts, int faces)
{
	t_mesh *mesh;

	mesh = (t_mesh *)malloc(sizeof(t_mesh));
	if (!mesh)
		return (NULL);
	mesh->verts = (t_vec3 *)malloc(sizeof(t_vec3) * verts);
	if (!mesh->verts)
	{
		free(mesh);
		return (NULL);
	}
	mesh->triangles = (t_triangle *)malloc(sizeof(t_triangle) * faces);
	if (!mesh->triangles)
	{
		free(mesh->verts);
		free(mesh);
		return (NULL);
	}
	return (mesh);
}

int	fill_tri(t_mesh *mesh, int k, char ***obj, int *face) //t_vec3 *verts)
{
	//handle poly as well..
	int			i;
	int			line_verts;
	t_triangle	*tri;
	t_vec3		shared_n;

	line_verts = count_verts(obj, k);			
	if (line_verts < 3)
		return (0);
	tri = &mesh->triangles[(*face)];
	tri->v0 = ft_atoi(obj[k][1]) - 1;
	tri->v1 = ft_atoi(obj[k][2]) - 1;
	tri->v2 = ft_atoi(obj[k][3]) - 1;
	tri->edge1 = subtract_vec(mesh->verts[tri->v1], mesh->verts[tri->v0]);
	tri->edge2 = subtract_vec(mesh->verts[tri->v2], mesh->verts[tri->v0]);
	tri->norm = norm_vec(cross_prod(tri->edge2, tri->edge1));
	shared_n = tri->norm;
	(*face)++;
	i = 2;
	while (++i < line_verts)//must move through tris here
	{
		tri = &mesh->triangles[(*face)];
		tri->v0 = ft_atoi(obj[k][1]) - 1;
		tri->v1 = ft_atoi(obj[k][i]) - 1;
		tri->v2 = ft_atoi(obj[k][i + 1]) - 1;
		tri->edge1 = subtract_vec(mesh->verts[tri->v1], mesh->verts[tri->v0]);
		tri->edge2 = subtract_vec(mesh->verts[tri->v2], mesh->verts[tri->v0]);
		//tri->norm = norm_vec(cross_prod(tri->edge2, tri->edge1));
		tri->norm = shared_n;
		(*face)++;
	}
	return (line_verts - 2);
}

void	fill_mesh(char ***obj, t_mesh *mesh)
{
	int k;
	int	vert;
	int face;

	face = 0;
	vert = 0;
	k = -1;
	while (obj[++k])
	{
		if (obj[k][0][0] == 'v')
		{
			mesh->verts[vert].x = ft_atof(obj[k][1]);
			mesh->verts[vert].y = ft_atof(obj[k][2]); 
			mesh->verts[vert].z = ft_atof(obj[k][3]);
			vert++;
		}
		if (obj[k][0][0] == 'f')
		{
			fill_tri(mesh, k, obj, &face);
		}
	}
	//printf("faces through: %d\n", face);
}


//allocate and chain to fill

t_mesh	*create_mesh(char ***obj)
{
	int		verts;
	int		faces;
	int		k;
	t_mesh	*mesh;

	verts = 0;
	faces = 0;
	k = -1;
	while (obj[++k])
	{
		if (obj[k][0][0] == 'v')
			verts++;
		if (obj[k][0][0] == 'f')
			faces += count_verts(obj, k) - 2;
	}
	mesh = malloc_mesh(verts, faces);
	if (!mesh)
		return (NULL);
	fill_mesh(obj, mesh);
	mesh->num_verts = verts;
	mesh->num_tris = faces;

	/* printf("verts: %d\n", mesh->num_verts);
	
	printf("tris: %d\n", mesh->num_tris); */

	return (mesh);
}

t_mesh	*parse_obj(char *filename)
{
	char	***obj;
	t_mesh	*mesh;

	obj = split_file(filename);
	if (!obj)
		return (NULL);
	//consider validity check run..
	mesh = create_mesh(obj);
	free_3d_array_i(obj, ft_3darray_len(obj));
	if (!mesh)
		return (NULL);
	mesh->color = color(100, 100, 100);
	mesh->mat = get_mat(DEFAULT);

	//mesh->mat = get_mat(DEFAULT);
	//add color, material, maybe transform...(must find center)
	//print_mesh(mesh);
	//free_mesh(mesh);
	return (mesh);//return mesh here
}
//in rt will need .obj, center, color
