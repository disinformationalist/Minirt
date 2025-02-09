#include "minirt.h"

void	print_gui_guide(void)
{
	ft_putchar_fd('\n', 1);
	ft_putstr_color_fd(1, "+---------------------------------------------------------+\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                        GUI GUIDE                        |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                                                         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| * Its mostly self explanatory, but here are few notes * |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                                                         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Changes made will take effect immediatly using a     |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> low-res mode. [S] toggles stash until closing(F4)    |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Everything operates upon the current 'on' object     |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Rectangles are pressable buttons                     |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Knobs are click and drag                             |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Can click 'to shape' areas to go to diff obj lists   |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| as well as cam and lights, lights show diff controls    |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                                                         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Only Position, Rotate, FOV are available on Camera   |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Next/Prev: go to next object of same type            |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Duplicate: makes an exact duplicate of the current   |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| object, and sets it to 'on', must move it to see it,    |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| if the list is empty, a default object is made.         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                                                         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Texture/Presets must keep clicking to go through all |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Rotate, Position, Scale open control dials           |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                                                         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| ----------------- Using Scaling Dials ------------------|\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                                                         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Scale shape in xyz, work outward in.                 |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Outer scales all three x, y , z                      |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Mids scale pairs      xz, yz, xy                     |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "| -> Inner scale individual x, y ,z                       |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "|                                                         |\n", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "+---------------------------------------------------------+\n", BRIGHT_GREEN);
}


void	print_guide(void)
{
	ft_putchar_fd('\n', 1);
	ft_putstr_color_fd(1, "                  ╔╦═════════════╦╗\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "                  ╠╣  KEY GUIDE  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╔╦════════════════╩╩═════════════╩╩════════════════╦╗\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [F2]       => Print this guide                  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣", BRIGHT_BLUE);
	ft_putstr_color_fd(1, " [F4]       => OPENS/CLOSES GUI CONTROLS         ", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [F5]       => Print GUI guide                   ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [F3]       => Export image to png               ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣", BRIGHT_BLUE);
	ft_putstr_color_fd(1, " [SPACE]    => Supersample on/off(antialiasing)  ", BRIGHT_GREEN);
	ft_putstr_color_fd(1, "╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [UP ⬆]     => Raise supersample level(max 10)   ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Down ⬇]   => Lower supersample level(min 2)    ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [Left ]    => Lower Low Res level(min 3)        ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [right ]   => Raise Low Res level(max 50)       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ samples = level² samples per pixel (max = 100)  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ starts initialized at 4 (16 samples per pixel)  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [C]        => Caps on/off cylinder/hyperboloid  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD DIV] => Radial sine wave on plane         ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD X]   => Frost surface(with glass to frost)╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 0]   => Low resolution mode on/off        ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [X]        => Stash changes made in gui (they   ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ activate all at once upon closing gui)          ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ ------- Below are also available in gui ------- ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NUM 1-5]  => Switch to different object types  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ 1 = spheres,   2 = planes, 3 cylinders,         ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ 4 hyperboloids 5 = cubes,  9 = lights, 0 = cam  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NUM 9]    => Switch to light list              ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NUM 0]    => Switch to cam                     ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD +]   => next object in current list       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD -]   => prev object in current list       ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [NPAD 1-9] => transmute object material(presets)╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [N]        => Destroy current object            ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [M]        => Duplicate object(default on emtpy)╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [AD,WS,QE] => Rotate along    x,y,z             ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [JL,KI,UO] => Translate along x,y,z             ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [L  CNTRL] => Bump on/off (with texture)        ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [R  CNTRL] => Object shadow on/off              ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ ---------- MOUSE HOOKS (also in gui) -----------╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [L CLICK]  => Sets clicked on obj to current    ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ (hold for low res  obj drag in x, y,            ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ hold [LSHIFT] and drag up or down for z)        ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ [R CLICK]  => Switch to color, textures, checker╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣ (mouse wheel rotates through colors, textures)  ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╠╣                                                 ╠╣\n", BRIGHT_BLUE);
	ft_putstr_color_fd(1, "╚╩═════════════════════════════════════════════════╩╝\n", BRIGHT_BLUE);
}
//╔ ╗ ╚ ╝ ═ ║, ╦ ╩ ╠ ╣


