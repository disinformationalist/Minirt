#include "minirt.h"

	/*** PNG COLOR TYPE GUIDE ***/
/*
| Color Type | Value | Channels | BPP |
| ---------- | ----- | -------- | --- |
| GRAY       | 0     | 1        | 1   |
| RGB        | 2     | 3        | 3   |
| PALETTE    | 3     | 1        | 1   |
| GRAY+ALPHA | 4     | 2        | 2   |
| RGBA       | 6     | 4        | 4   | 
*/


/* export_png takes the desired name for the image file(eg: my_img.png), a
t_img struct, and the width and height of the image to export and lastly a 
pointer to an array of png_text structs. If no text is desired to be placed 
in the image metadata, just write NULL this param. 
RETURN VALUE: 0 on success, and -1 on failure, 
freeing all internal memory resources always. */

/* set_png_text_metadata stores txt but doesnt display, as that is not 
supported by gnome veiwer. however, you can use a terminal tool and cmd 
to display text stored for the image here. will place txt in first row as 
<text[0].key> <text[0].text>, next row as <text[1].key> <text[1].text> etc...
adjust this function according to needs. EX: Greetings	 : Enjoy this image
this function can be used in place of passing a value to the last param.*/

/* TERMINAL TOOL VIEWING METADATA: sudo apt-get install libimage-exiftool-perl
Command line input: exiftool image_1.png */

/* void	set_png_text_metadata(png_structp png_ptr, png_infop info)
{
	png_text	text[1];

	text[0].compression = PNG_TEXT_COMPRESSION_NONE;
	text[0].key = "Greetings:";
	text[0].text = "Enjoy this image";
	png_set_text(png_ptr, info, text, 1);
} */

//gamma correct for srgb

static inline uint8_t to_srgb(uint8_t linear_val)
{
	float lin = linear_val / 255.0f;
	float srgb;

	if (lin <= 0.0031308f)
		srgb = 12.92f * lin;
	else
		srgb = 1.055f * powf(lin, 1.0f / 2.4f) - 0.055f;

	int corrected = (int)(srgb * 255.0f + 0.5f);
	if (corrected < 0) corrected = 0;
	if (corrected > 255) corrected = 255;
	return (uint8_t)corrected;
}

//This is where pixel values are set into the image, original version
/* 
int	set_png_pixels(t_png_io *png_img, t_img *img, int height, int width)
{
	png_byte	*row;

	png_img->y = -1;
	while (++png_img->y < height)
	{
		row = png_malloc(png_img->png_ptr, sizeof(uint8_t) * \
		width * png_img->pixel_size);
		if (!row)
		{
			perror("Error: export_png row malloc failure\n");
			clean_memory(png_img, png_img->y, true);
			return (-1);
		}
		png_img->row_pointers[png_img->y] = row;
		png_img->x = -1;
		while (++png_img->x < width)
		{
			get_pixel(&png_img->temp_pixel, img, png_img->x, png_img->y);
			*row++ = png_img->temp_pixel.red;
			*row++ = png_img->temp_pixel.green;
			*row++ = png_img->temp_pixel.blue;
			if (png_img->pixel_size == 4)
				*row++ = png_img->temp_pixel.alpha;

			// *row++ = 255;//for when alpha in use

			//the correction makes to bright leave out and just apply tag after ihdr as is.
			// *row++ = to_srgb(png_img->temp_pixel.red);
			// *row++ = to_srgb(png_img->temp_pixel.green);
			// *row++ = to_srgb(png_img->temp_pixel.blue);
			// *row++ = 255;
		}
	}
	return (0);
} */

//version of above from chat for all types

static inline uint8_t to_gray(uint8_t r, uint8_t g, uint8_t b)
{
    // integer approx of 0.299*r + 0.587*g + 0.114*b
    return (uint8_t)((77 * r + 150 * g + 29 * b + 128) >> 8);
}

int set_png_pixels(t_png_io *png_img, t_img *img, int height, int width)
{
    png_byte *row;

    for (png_img->y = 0; png_img->y < height; png_img->y++)
    {
        row = png_malloc(png_img->png_ptr, (size_t)width * png_img->pixel_size);
        if (!row)
        {
            perror("Error: export_png row malloc failure\n");
            clean_memory(png_img, png_img->y, true);
            return -1;
        }
        png_img->row_pointers[png_img->y] = row;

        for (png_img->x = 0; png_img->x < width; png_img->x++)
        {
            get_pixel(&png_img->temp_pixel, img, png_img->x, png_img->y);

            uint8_t r = png_img->temp_pixel.red;
            uint8_t g = png_img->temp_pixel.green;
            uint8_t b = png_img->temp_pixel.blue;
            uint8_t a = png_img->temp_pixel.alpha;

            switch (png_img->color_type)
            {
                case PNG_COLOR_TYPE_RGB:
                    *row++ = r;
                    *row++ = g;
                    *row++ = b;
                    break;

                case PNG_COLOR_TYPE_RGBA:
                    *row++ = r;
                    *row++ = g;
                    *row++ = b;
                    *row++ = 255;//a;
                    break;

                case PNG_COLOR_TYPE_GRAY:
                {
                    uint8_t y8 = to_gray(r, g, b);
                    *row++ = y8;
                    break;
                }

                case PNG_COLOR_TYPE_GRAY_ALPHA:
                {
                    uint8_t y8 = to_gray(r, g, b);
                    *row++ = y8;
                    *row++ = a;
                    break;
                }
				//not used
                case PNG_COLOR_TYPE_PALETTE:
                    // You must write palette *indices* (0..N-1)
                    // Needs a palette mapping step (see below)
                    //*row++ = (png_byte)png_img->temp_index; // compute per pixel
					return error_3(png_img, "Palette PNG not supported");
                    break;

                default:
                    // Should never happen if IHDR is set correctly
                    clean_memory(png_img, png_img->y + 1, true);
                    return -1;
            }
        }
    }
    return 0;
}


int	init_png_structs(t_png_io *png_img, const char *filename)
{
	png_img->fp = fopen(filename, "wb");
	if (!png_img->fp)
	{
		perror("Error: export_png could not open file\n");
		free(png_img);
		return (-1);
	}
	png_img->png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, \
	NULL, NULL);
	if (!png_img->png_ptr)
		return (error_1(png_img, \
		"Error: export_png could not create png write struct\n"));
	png_img->info = png_create_info_struct(png_img->png_ptr);
	if (!png_img->info)
	{
		png_destroy_write_struct(&png_img->png_ptr, NULL);
		return (error_1(png_img, \
		"Error: export _png could not create png info struct\n"));
	}
	return (0);
}

static inline void	write_img(t_png_io *png_img)
{
	int i;
	int len;

	if (png_img->text != NULL)
	{
		len = 0;
		while (png_img->text[len].text != NULL)
			len++;
		png_set_text(png_img->png_ptr, png_img->info, png_img->text, len);
		i = -1;
		while (++i < len)
		{
			free(png_img->text[i].text);
			free(png_img->text[i].key);
		}
		free(png_img->text[i].key);
		free(png_img->text);
	}
	png_init_io(png_img->png_ptr, png_img->fp);
	png_set_rows(png_img->png_ptr, png_img->info, png_img->row_pointers);
	png_write_png(png_img->png_ptr, png_img->info, \
	PNG_TRANSFORM_IDENTITY, NULL);
}

//-----consider creating a check for t(a) channel and create rgb or rgba
//will have to adjust png_set_ihdr, png color type

int	export_png(const char *filename, t_img *img, int width, int height, png_text *text, t_pixel_format fmt)
{
	t_png_io	*png_img;

	/* t_pixel p; */
/* get_pixel(&p, img, width/2, height/2); //test pixel
printf("A=%u R=%u G=%u B=%u\n", p.alpha, p.red, p.green, p.blue); */
	png_img = (t_png_io *)malloc(sizeof(t_png_io));
	if (!png_img)
		return (-1);
	init_vars(png_img, fmt);
	if (png_img->pixel_size < 0)
    	return (error_1(png_img, "Invalid/unsupported pixel format\n"));
	if (init_png_structs(png_img, filename) == -1)
		return (-1);
	png_set_IHDR(png_img->png_ptr, png_img->info, width, height, \
	png_img->depth, png_img->color_type, PNG_INTERLACE_NONE, \
	PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	//if (width * height > 36000000)
	//png_set_filter(png_img->png_ptr, 0, PNG_FILTER_PAETH); // good for posters
	//png_set_compression_level(png_img->png_ptr, 9);//adding for smaller large files

	png_set_sRGB(png_img->png_ptr, png_img->info, PNG_sRGB_INTENT_PERCEPTUAL);
	png_img->row_pointers = png_malloc(png_img->png_ptr, height \
	* sizeof(png_byte *));
	if (!png_img->row_pointers)
	{
		png_destroy_write_struct(&png_img->png_ptr, &png_img->info);
		return (error_1(png_img, "Error: row ptrs malloc failure\n"));
	}
	if (set_png_pixels(png_img, img, height, width) == -1)
		return (-1);
	png_img->text = text;
	write_img(png_img);
	clean_memory(png_img, height, true);
	return (0);
}
