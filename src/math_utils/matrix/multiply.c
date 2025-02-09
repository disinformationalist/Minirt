#include "minirt.h"

//multiply matrix and vector

t_vec3	mat_vec_mult(t_mat4 m, t_vec3 v)
{
	t_vec3	r;

	r.x = m.mat[0] * v.x + m.mat[4] * v.y + m.mat[8] * v.z + m.mat[12] * v.w;
	r.y = m.mat[1] * v.x + m.mat[5] * v.y + m.mat[9] * v.z + m.mat[13] * v.w;
	r.z = m.mat[2] * v.x + m.mat[6] * v.y + m.mat[10] * v.z + m.mat[14] * v.w;
	r.w = m.mat[3] * v.x + m.mat[7] * v.y + m.mat[11] * v.z + m.mat[15] * v.w;
	return (r);
}

//SIMD
//attempt simd, RESULT: seems slower for now try again after changing things to float,
//and check memory alignment access patterns for compat. try avx instructs.
//maybe works better if using row major.

/* t_vec3	mat_vec_mult(t_mat4 m, t_vec3 v)
{
	t_vec3	r;
	
	//load vec, load in rev order for simd
	__m128 vec = _mm_set_ps(v.w, v.z, v.y, v.x);
	
	//loading each row, in rev order
	__m128 row0 = _mm_set_ps(m.mat[12], m.mat[8], m.mat[4], m.mat[0]);
	__m128 row1 = _mm_set_ps(m.mat[13], m.mat[9], m.mat[5], m.mat[1]);
	__m128 row2 = _mm_set_ps(m.mat[14], m.mat[10], m.mat[6], m.mat[2]);
	__m128 row3 = _mm_set_ps(m.mat[15], m.mat[11], m.mat[7], m.mat[3]);
	
	//take dot products using built in dotp op
	//0xF1 specifies all 4 components to be calced.

	__m128 dot0 = _mm_dp_ps(row0, vec, 0xF1);
	__m128 dot1 = _mm_dp_ps(row1, vec, 0xF1);
	__m128 dot2 = _mm_dp_ps(row2, vec, 0xF1);
	__m128 dot3 = _mm_dp_ps(row3, vec, 0xF1);
	
	//extract results from SIMD register
	r.x = _mm_cvtss_f32(dot0);
	r.y = _mm_cvtss_f32(dot1);
	r.z = _mm_cvtss_f32(dot2);
	r.w = _mm_cvtss_f32(dot3);

	//trying with avx version (256)
} */


//multiply two matrices

t_mat4 mat_mult(t_mat4 m1, t_mat4 m2)
{
	t_mat4	res;
	int		i;
	int		j;
	int		k;

	i = -1;
	while (++i < 4)
	{
		j = -1;
		while (++j < 4)
		{
			res.mat[i + j * 4] = 0;
			k = -1;
			while (++k < 4)
				res.mat[i + j * 4] += m1.mat[i + k * 4] * m2.mat[k + j * 4];
		}
	}

	//SIMD, overhead may outweigh benefits, but try later anyway
	/* i = -1;
	while(++i < 16)
		res.mat[i] = 0.0f;
	i = -1;
	while ()
	{
		while()
	} */


	return (res);
}