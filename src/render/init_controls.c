#include "minirt.h"

void	null_init2(t_control *new)
{
	new->menu = NULL;
	new->r = NULL;
	new->g = NULL;
	new->b = NULL;
	new->bron = NULL;
	new->sil = NULL;
	new->ct = NULL;
	new->pt = NULL;
	new->rot = NULL;
	new->rot_dials = NULL;
	new->dtx = NULL;
	new->dty = NULL;
	new->dtz = NULL;
	new->pos = NULL;
	new->pos_dials = NULL;
	new->dtpx = NULL;
	new->dtpy = NULL;
	new->dtpz = NULL;
	new->rot_open = false;
	new->pos_open = false;
	new->sca_open = false;
}

t_control	*make_controls(void *con)
{
	t_control	*new;

	new = (t_control *)malloc(sizeof(t_control));
	if (!new)
		return (NULL);
	null_init2(new);
	new->menu = import_png(con, \
	"src/events/menu/images/obj_control.png", &new->m_width, &new->m_height);
	if (!new->menu)
		return (NULL);
	new->r = import_png(con, \
	"src/events/menu/images/red.png", &new->k_width, &new->k_height);
	if (!new->r)
		return (NULL);
	new->g = import_png(con, \
	"src/events/menu/images/green.png", &new->k_width, &new->k_height);
	if (!new->g)
		return (NULL);
	new->b = import_png(con, \
	"src/events/menu/images/blue.png", &new->k_width, &new->k_height);
	if (!new->b)
		return (NULL);
	new->bron = import_png(con, \
	"src/events/menu/images/bronze.png", &new->k_width, &new->k_height);
	if (!new->bron)
		return (NULL);
	new->sil = import_png(con, \
	"src/events/menu/images/silver.png", &new->k_width, &new->k_height);
	if (!new->sil)
		return (NULL);
	new->ct = import_png(con, \
	"src/events/menu/images/ct.png", &new->ct_width, &new->ct_height);
	if (!new->ct)
		return (NULL);
	new->pt = import_png(con, \
	"src/events/menu/images/pt.png", &new->pt_width, &new->pt_height);
	if (!new->pt)
		return (NULL);
	//rotation
	new->rot = import_png(con, \
	"src/events/menu/images/rotknob.png", &new->k_width, &new->k_height);
	if (!new->rot)
		return (NULL);
	new->dtx = import_png(con, \
	"src/events/menu/images/dtx.png", &new->dt_width, &new->dt_height);
	if (!new->dtx)
		return (NULL);
	new->dty = import_png(con, \
	"src/events/menu/images/dty.png", &new->dt_width, &new->dt_height);
	if (!new->dty)
		return (NULL);
	new->dtz = import_png(con, \
	"src/events/menu/images/dtz.png", &new->dt_width, &new->dt_height);
	if (!new->dtz)
		return (NULL);
	new->rot_dials = import_png(con, \
	"src/events/menu/images/rot.png", &new->d_width, &new->d_height);
	if (!new->pt)
		return (NULL);
	new->dials_ys =	new->m_height - new->d_height;
	//position
	new->pos = import_png(con, \
	"src/events/menu/images/posknob.png", &new->k_width, &new->k_height);
	if (!new->pos)
		return (NULL);
	new->dtpx = import_png(con, \
	"src/events/menu/images/dtpx.png", &new->dt_width, &new->dt_height);
	if (!new->dtpx)
		return (NULL);
	new->dtpy = import_png(con, \
	"src/events/menu/images/dtpy.png", &new->dt_width, &new->dt_height);
	if (!new->dtpy)
		return (NULL);
	new->dtpz = import_png(con, \
	"src/events/menu/images/dtpz.png", &new->dt_width, &new->dt_height);
	if (!new->dtpz)
		return (NULL);
	new->pos_dials = import_png(con, \
	"src/events/menu/images/pos.png", &new->d_width, &new->d_height);
	if (!new->pos_dials)
		return (NULL);
	return (new);
}