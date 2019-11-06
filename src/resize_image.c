#include <math.h>
#include "image.h"
float map_pixel(int r1 , int r2 , int x)  // old  , new  , (x || y)
{
    /*
        this is kind of neat way to work
        I get it's intuation but I can't map it for the
        system of linear equations
        but for testing purpose I will use the system of linear equations
    */
    float xx=(float)x*((float)r1/(float)r2);
    return xx;
}
float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in

    int xx=(int)(x+0.5);
    int yy=(int)(y+0.5);

    float pixel=get_pixel(im , xx , yy , c);
    return pixel;
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)

    // system of linear equation to predicate
    //old cordinate corresponding to the new cordinate
    float ah , bh;
    float aw , bw;

    // heigh polynomial (y)
    ah=(float)im.h/(float)h;
    bh=-0.5+0.5*ah;

    // width polynomial
    aw=(float)im.w/(float)w;
    bw=-0.5+0.5*aw;

    image new_im=make_image(w , h , im.c);
    int y=0 , x=0;

    for(y=0; y<h; y++)
        for(x=0; x<w; x++){
            float yy=ah*y+bh;
            float xx=aw*x+bw;
            for(int c=0; c<im.c; c++)
            {
                float pixel=nn_interpolate(im , xx , yy , c);
                set_pixel(new_im , x , y , c , pixel);
            }
        }
    return new_im;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    // [TODO by me we need to to padding]
    //  (I don't know exactly what he means by using padding here but !?)

    // right side of the square
    int right_upx , right_upy;
    int right_downx , right_downy;
    // left side of the square
    int left_upx , left_upy;
    int left_downx , left_downy;

    // square that my pixel is located inside it
    left_upx=floorf(x);
    left_upy=floorf(y);

    right_downx=floorf(x+1);  // better to be defined in terms of leftup pixel
    right_downy=floorf(y+1);

    // previous 4 lines were  been rounding using (int) and failed
    // when I used floor it worked will so why !????
    //  I didn't use ceil to be able to draw a square

    right_upy=left_upy;
    right_upx=left_upx+1;

    left_downx=left_upx;
    left_downy=left_upy+1;

    float vu_dis=y-(float)(left_upy);
    float hl_dis=x-(float)(left_upx);

    // first linear interpolation , left side of the square
    float pixel1 , pixel2;
    // upper left pixel
    pixel1=get_pixel(im , left_upx , left_upy , c);
    // lower left pixel
    pixel2=get_pixel(im , left_downx , left_downy , c);
    // interpolate
    float left_pixel=pixel1*((float)(1)-vu_dis)+pixel2*(vu_dis);

    // second linear interpolation , right side of the sqaure  , reuse pixel1 & pixel2
    // upper right pixel
    pixel1=get_pixel(im , right_upx , right_upy , c);
    // lower right pixel
    pixel2=get_pixel(im , right_downx , right_downy , c);
    // interpolate
    float right_pixel=pixel1*((float)(1)-vu_dis)+pixel2*(vu_dis);

    // third linear interpolation => bilinear interpolation
    float result_pixel=left_pixel*(1-hl_dis)+right_pixel*(hl_dis);

    return result_pixel;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
  // system of linear equation to predicate
    //old cordinate corresponding to the new cordinate
    float ah , bh;
    float aw , bw;

    // heigh polynomial (y)
    ah=(float)im.h/(float)h;
    bh=-0.5+0.5*ah;

    // width polynomial
    aw=(float)im.w/(float)w;
    bw=-0.5+0.5*aw;

    image new_im=make_image(w , h , im.c);
    int y=0 , x=0;

    for(y=0; y<h; y++)
        for(x=0; x<w; x++){
            float yy=ah*y+bh;
            float xx=aw*x+bw;
            for(int c=0; c<im.c; c++)
            {
                float pixel=bilinear_interpolate(im , xx , yy , c);
                set_pixel(new_im , x , y , c , pixel);
            }
        }
    return new_im;
}

