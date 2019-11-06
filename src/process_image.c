#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

#include <stdio.h>
float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in   [Done by Ahmed Araby]

    // padding
    // clapping in cols
    if(x>=im.w)
	x=im.w-1;
    else if(x<0)
	x=0;

    // clapping in rows
    if(y>=im.h)
	y=im.h-1;
    else if(y<0)
	y=0;

    int channel_size=im.h*im.w;
    int index=c*channel_size+y*im.w+x;
    float pixel=im.data[index];

    return pixel;
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in    [Done by Ahmed Araby]

    // boundery checking
    if(x<0 || x>=im.w)
	return ;
    else if(y<0 || y>=im.h)
	return ;
    else if(c<0 || c>=im.c)
	return ;

    // set the pixel
    int channel_size=im.h*im.w;
    int index=c*channel_size+y*im.w+x;
    im.data[index]=v;
    return ;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in   [Done by Ahmed Araby]
    for(int y=0; y<im.h; y++)
	for(int x=0; x<im.w; x++)
		for(int c=0; c<im.c; c++)
		{
		    float pixel=get_pixel(im , x ,  y , c);
		    set_pixel(copy , x , y , c , pixel);
		}

    return copy;
}

image rgb_to_grayscale(image im)
{
    // [ Revised 26/8/19 - there is more accurate way to do this ]

   /*
	there is a more advanced and complicated way of doing this conversion
	try to implement it latter
	cuz this method if less accurate
   */
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in   [Done by Ahmed Araby]
    for(int y=0; y<im.h; y++)
	for(int x=0; x<im.w; x++)
		{
		    float red_pixel=get_pixel(im , x ,  y , 0);
		    float green_pixel=get_pixel(im , x , y, 1);
	 	    float blue_pixel=get_pixel(im , x , y , 2);

            float grayed_pixel=0.299*red_pixel+0.587*green_pixel+0.114*blue_pixel;
		    set_pixel(gray , x , y , 0 , grayed_pixel);
		}
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    // TODO(by me) assert on the channel to be shifter !!!!!!!!!!!!!!!!!!!!!!
    for (int y=0; y<im.h; y++)
	for(int x=0; x<im.w; x++)
	{
	    float pixel=get_pixel(im , x , y , c);
	    set_pixel(im, x, y , c , pixel+v);
	}

    return ;
}

void clamp_image(image im)
{
    // TODO Fill this in
    // clamping
    for(int y=0; y<im.h; y++)
	for(int x=0; x<im.w; x++)
	    for(int c=0; c<im.c; c++)
	    {
	        float pixel=get_pixel(im , x , y , c);
		if(pixel<0)
		   set_pixel(im , x , y , c , 0);
		else if(pixel>1)
		   set_pixel(im , x , y , c , 1);
	    }
     return;
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
}


void hsv_to_rgb(image im)
{
    // TODO Fill this in
}

void display_image(image im)
{
    printf("displaying image data \n");
    for(int y=0; y<im.h;  y++)
        for(int x=0; x<im.w; x++){		
	    float pixel=get_pixel(im , x , y , 0);
            if(pixel==1)
	            printf("%f \n" , pixel);
	}
    printf("DONE \n");
    return ;
}
