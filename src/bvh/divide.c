#include "minirt.h"


//split a box into 2 make return a 2 box structure

t_boxes	*split_bounds(t_box inbox)//, int prev_axis)
{
	t_boxes *res;
	double	dx;
	double	dy;
	double	dz;
	t_point	min;
	t_point	max;
	double	biggest;

	min = inbox.min;
	max = inbox.max;
	dx = fabs(max.x - min.x);
	dy = fabs(max.y - min.y);
	dz = fabs(max.z - min.z);
	biggest = fmax(fmax(dx, dy), dz);
	if (dx == biggest)
	{
		min.x = min.x + dx / 2.0; 
		max.x = min.x;
		//res->axis = 0;
	}
	else if (dy == biggest)
	{
		min.y = min.y + dy / 2.0;
		max.y = min.y;
		//res->axis = 1;
	}
	else if (dz == biggest)
	{
		min.z = min.z + dz / 2.0;
		max.z = min.z;
		//res->axis = 2;
	}
	res = boxes(box(inbox.min, max), box(min, inbox.max));
	if (!res)
		return (NULL);
	return (res);
}

//remove a child node(non group)

void	remove_curr_child(t_group *g, t_shape **current)
{
	t_shape	*to_destroy;
	t_shape	*prev_sp;
	t_shape	*next_sp;

	if (!current || !*current)
		return ;
	to_destroy = *current;
	next_sp = to_destroy->next;
	if (to_destroy->next == to_destroy)
	{
		g->shapes = NULL;
		*current = NULL;
	}
	else
	{
		prev_sp = to_destroy->prev;
		prev_sp->next = next_sp;
		next_sp->prev = prev_sp;
		if (g->shapes == to_destroy)
			g->shapes = next_sp;
	}
	free(to_destroy);
	*current = next_sp;
	g->count--;
	to_destroy = NULL;
}

//traverse group and place left or right group, remove from top group

void	assign_lr(t_group *group_in, t_group *left, t_group *right, t_boxes lr)
{
	t_shape	*curr;
	t_box	*cbox;
	bool	flag;

	if (!group_in->shapes)
		return ;
	curr = group_in->shapes;
	while (true)
	{
		flag = 0;
		if (curr->type == GROUP)
			assign_lr((t_group *)curr, left, right, lr);
		else
		{
			cbox = curr->box;
			if (contains_box(*(lr.left), *cbox))
			{
				add_child(left, curr->shape, curr->type, curr->tran, curr->i_tran, cbox);
				remove_curr_child(group_in, &curr);
				flag = 1;
			}
			else if (contains_box(*lr.right, *cbox))
			{
				add_child(right, curr->shape, curr->type, curr->tran, curr->i_tran, cbox);
				remove_curr_child(group_in, &curr);
				flag = 1;
			}
		}
		if (!group_in->shapes)
		{
			group_in->count = 0;
			break ;
		}
		else if (!flag)
		{
			curr = curr->next;
			if (curr == group_in->shapes)
				break ;
		}	
	}
}

//trying overlap group "other"

void		build_other(t_group *in, t_group *other)
{
	t_shape	*curr;
	bool	flag;

	curr = in->shapes;
	while (true)
	{
		flag = 0;
		if (curr->type != GROUP)
		{
			add_child(other, curr->shape, curr->type, curr->tran, curr->i_tran, curr->box);
			remove_curr_child(in, &curr);
			flag = 1;
		}
		if (!in->shapes)
		{
			in->count = 0;
			break ;
		}
		else if (!flag)
		{
			curr = curr->next;
			if (curr == in->shapes)
				break ;
		}	
	}
}

void	divide(t_group *group_in, int threshold)//, int prev_ax
{
	t_boxes *lr;
	t_group *left;
	t_group *right;
	int		g_count;
	int		depth;

	if (group_in->count < threshold)
		return ;
	g_count = 0;
	left = group();
	right = group();
	depth = group_in->depth + 1;
	lr = split_bounds(*(group_in->box));
	//prev_ax = lr->prev_axis;
	assign_lr(group_in, left, right, *lr);
	if (left->shapes)
	{
		left->depth = depth;
		add_child(group_in, left, GROUP, left->tran, identity(), bog(left));
		g_count++;
	}
	else
	{
		free(left);
		left = NULL;
	}
	if (right->shapes)
	{
		right->depth = depth;
		add_child(group_in, right, GROUP, right->tran, identity(), bog(right));
		g_count++;
	}
	else
	{
		free(right);
		right = NULL;
	}
	free(lr->left);
	free(lr->right);
	free(lr);
	lr = NULL;
	if (left)
		divide(left, threshold);
	if (right)
		divide(right, threshold);
	//try another bvh of outliers here...
	//can at least place outlier in group with its own tight box. must loop through current group build, and divide. test
	//works awesome! 10 to 15 times faster than without, for some renders like sp flake.
	if (group_in->count - g_count > 2)
	{
		t_group *other = group();
		build_other(group_in, other);
		other->depth = depth;
		add_child(group_in, other, GROUP, group_in->tran, identity(), bog(other));
		//this is for protection from endless loop, stack == BOOM when no change to bound split.
		if (left || right)// may try rotating between split axes instead..
			divide(other, threshold);
	}
}

