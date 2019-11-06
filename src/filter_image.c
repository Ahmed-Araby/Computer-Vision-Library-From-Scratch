#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

// theses value are from my book
const float pi=3.1415926; // max accuracy float can hold as float have 7 decimal precision
const float e=2.71828;

void l1_normalize(image im)
{
    // TODO

    // make summation = 1 as we will use theses as proportional cofficients

    // get normalization cofficient
    float sum=0;
    for(int y=0; y<im.h; y++)
        for(int x=0; x<im.w; x++)
            for(int c=0; c<im.c; c++)
            {
                float pixel=get_pixel(im , x , y , c);
                sum+=pixel;
            }

    // normalize
     for(int y=0; y<im.h; y++)
        for(int x=0; x<im.w; x++)
            for(int c=0; c<im.c; c++)
            {
                float v=get_pixel(im , x , y , c);
                v=v/sum;
                set_pixel(im , x , y , c , v);
            }
    return ;
}

image make_box_filter(int w)
{
    // TODO
    if((w&1)==0)  // odd so we can get center cell
        ++w;

    float v=(float)1/(float)(w*w);

    image filter=make_image(w , w , 1);
    int y=0 , x=0;
    for(y=0; y<w; y++)
        for(x=0; x<w; x++)
        {
            set_pixel(filter , x , y , 0 , v);
        }

    return filter;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    // this is better to be called cross correlation because we don't flib the kernal
    // for some pixels we will have the kernl off the boundery
    // get_pixel will handel this by pading-clapping

    // number of rows , cols above and bellow , before and after the center pixel
    assert("kernal can't have more chanels than image " && (im.c == filter.c || filter.c == 1));

    image filtered=make_image(im.w , im.h , im.c);
    int k=(filter.h-1)/2; // 2*k+1= w & h of the filter
    for(int y=0; y<im.h; y++)
        for(int x=0; x<im.w; x++)
        for(int c=0; c<im.c; c++)
        {
            int fc=0;
            if(im.c == filter.c)
                fc=c;

            // convolve
            float sum=0;
            for(int i=-k; i<=k; i++) // y axis
                for(int j=-k; j<=k; j++)  // x axis
                {
                    //float fv=get_pixel(filter , i+k , j+k , fc);   // bug image move in cols and he moves in rows
                    float fv=get_pixel(filter , j+k , i+k , fc);  // this change affect my response very much
                    float imv=get_pixel(im , x+j , y+i , c); // x+i , y+j !?
                    sum+=fv*imv;
                }
            set_pixel(filtered , x , y , c , sum);
        }

    if(preserve==0 && filtered.c==3)  // called sum between channels
        filtered=rgb_to_grayscale(filtered);   // this was a bug I was not receving the grayed scale image

    return filtered;
}

image make_highpass_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_sharpen_filter()
{
    // TODO
    return make_image(1,1,1);
}

image make_emboss_filter()
{
    // TODO
    return make_image(1,1,1);
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO
float calc_gaussian_second_part(int x , int y , int center , int sigma)
{
    // this will eveluate the part of e raised to the power
    // as the first part is always as it's so we can save computation
    int disx=x-center;
    int disy=y-center;

    sigma=sigma*sigma;
    disx=disx*disx;
    disy=disy*disy;


    float res=(float)(disx+disy)/(float)(2*sigma);
    res=exp(-res);

    return res;

}
image make_gaussian_filter(float sigma)
{
    // TODO
    int sz=6*sigma;
    if((sz&1)==0)
        ++sz;
    int center=sz/2;
    // assert on center

    image kernal=make_image(sz , sz , 1);
    float f=(float)1/((float)TWOPI*sigma*sigma);

    for(int y=0; y<sz; y++)
        for(int x=0; x<sz; x++)
        {
            float s=calc_gaussian_second_part(x , y , center , sigma);
            float v=f*s;
            set_pixel(kernal , x , y , 0 , v);
        }
    l1_normalize(kernal);
    return kernal;
}

// TODO  [By me: implement seprable gaussian filter ]
image seprable_gaussian_filter(float sigma)
{
    return make_image(1 , 1 , 1);
}
image add_image(image a, image b)
{
    // TODO
    assert((a.h==b.h && a.w==b.w && a.c==b.c) && "both images have to have same meta data ");
    image img=make_image(a.w, a.h , a.c);
    for(int y=0; y<a.h; y++)
        for(int x=0; x<a.w; x++)
            for(int c=0; c<a.c; c++)
            {
                float sum=get_pixel(a , x , y , c)+get_pixel(b , x , y , c);
                set_pixel(img , x , y , c , sum);
            }

    //clamp_image(img);

    return img;
}

image sub_image(image a, image b)
{
    // TODO
    assert((a.h==b.h && a.w==b.w && a.c==b.c) && "both images have to have same meta data ");
    image img=make_image(a.w  , a.h , a.c);
    for(int y=0; y<a.h; y++)
        for(int x=0; x<a.w; x++)
            for(int c=0; c<a.c; c++)
            {
                float sub=get_pixel(a , x , y , c)-get_pixel(b , x , y , c);
                set_pixel(img , x , y , c , sub);
            }
     // this was one of the bugs !
    //  clamp_image(img);

    return img;
}

// Am I to normalize the kernal !????

image make_gx_filter()
{
    // TODO
    // make the kernal more general work on more than one channel
    image kernalx=make_image(3 , 3 , 1);
    // first row
    set_pixel(kernalx , 0 , 0 , 0 , -1);
    set_pixel(kernalx , 0 , 1 , 0 , 0);
    set_pixel(kernalx , 0 , 2 , 0 , 1);

    // second row
    set_pixel(kernalx , 1 , 0 , 0 , -2);
    set_pixel(kernalx , 1 , 1 , 0 , 0);
    set_pixel(kernalx , 1 , 2 , 0 , 2);

    // third row
    set_pixel(kernalx , 2 , 0 , 0 , -1);
    set_pixel(kernalx , 2 , 1 , 0 , 0);
    set_pixel(kernalx , 2 , 2 , 0 , 1);

    return kernalx;
}

image make_gy_filter()
{
    /*
        I have changed the direction of the derivative in the Y axis
        I change it  to work well with I interpret the angles

        currently +ve direction of change in Y is down (reversed to how I interpret it as angles = 1)
        I guess my mistake was that I did not change all the signs
        of last and first row
    */
    // TODO
    image kernaly=make_image(3 , 3 , 1);
    // first row
    set_pixel(kernaly , 0 , 0 , 0 , -1);
    set_pixel(kernaly , 0 , 1 , 0 , -2);
    set_pixel(kernaly , 0 , 2 , 0 , -1);

    // second row
    set_pixel(kernaly , 1 , 0 , 0 , 0);
    set_pixel(kernaly , 1 , 1 , 0 , 0);
    set_pixel(kernaly , 1 , 2 , 0 , 0);

    // third row
    set_pixel(kernaly , 2 , 0 , 0 , 1);
    set_pixel(kernaly , 2 , 1 , 0 , 2);
    set_pixel(kernaly , 2 , 2 , 0 , 1);

    return kernaly;
}

float get_min(image im)
{
    assert(im.c==1 && "image have to be gray scale ");

    float min_v=100000;
    int y=0 , x=0;

    for(y=0; y<im.h; y++)
        for(x=0; x<im.w; x++)
        {
            float pixel_v=get_pixel(im , x , y , 0);
            min_v=fmin(min_v , pixel_v);
        }
    return min_v;

}

float get_max(image im)
{
    assert(im.c==1 && "image have to be gray scale ");

    float max_v=-100000;
    int y=0 , x=0;

    for(y=0; y<im.h; y++)
        for(x=0; x<im.w; x++)
        {
            float pixel_v=get_pixel(im , x , y , 0);
            max_v=fmax(max_v , pixel_v);
        }
    return max_v;
}

void feature_normalize(image im)
{
    // TODO

    assert(im.c==1 && "image have to be gray scale ");

    float min_v=get_min(im);
    float max_v=get_max(im);
    float range=max_v-min_v;

    // min-max normalization  , this is better than clapping >1=1 , <0=0
    // what is the difference !??

    int y=0 , x=0;
    for(y=0; y<im.h; y++)
        for(x=0; x<im.w; x++)
        {
            if(range!=0)
            {
                float pixel_v=get_pixel(im , x , y , 0);
                float norm_v=(pixel_v-min_v)/range;
                set_pixel(im , x , y , 0 , norm_v);
            }
            else
            {
                set_pixel(im , x , y , 0 , 0);
            }
        }

    return ;
}
/*
	sobel edge detector
*/

void tresh_hold_sobel(image im , float maxi)
{
    assert(im.c==1 && " image have to be gray scale to be tresh holded ");
    int y=0 , x=0;
    for(y=0; y<im.h; y++)
        for(x=0; x<im.w; x++)
        {
            float pixel=get_pixel(im , x, y , 0);
            if(pixel>=maxi)
                set_pixel(im , x , y , 0 , 1.0);
            else
                set_pixel(im , x , y , 0 , 0.0);
        }
    return ;
}

image *sobel_image(image im)
{
    // TODO
    // dynamically allocated arrays using calloc have life time over all the program
    // untill we delete it
    /*
        Steps
        1- convert to gray scale
        2- get x and y gradiants
        3- calc magnitude so edges been represented by
           high clifs only and no need for second derivative
        4- calc direction of gradiants vector so we get intuation of
           high and low intensity positions that cuz the edge
        when do we have to do feature normalization !?????
        been encansulated in struct do deep copy
    */


    image *img_arr=calloc(2 , sizeof(image));
    //assert(img_arr==NULL && "there is no enough memory to allocate (sobel_image function )");

    image gx_filter=make_gx_filter();
    image gy_filter=make_gy_filter();

    // x and y gradiants
    image gx_im=convolve_image(im , gx_filter , 0);
    image gy_im=convolve_image(im , gy_filter , 0);

    image mag=make_image(im.w , im.h , 1);
    image dir=make_image(im.w , im.h , 1);

    int y=0 , x=0;
    for(y=0; y<mag.h; y++)
        for(x=0; x<mag.w; x++)
        {
            float pixelx=get_pixel(gx_im , x , y , 0);
            float pixely=get_pixel(gy_im  , x , y , 0);

            // after all it's a ration no need to feature normalization first
            float vec_dir=atan2(pixely , pixelx);//*180.0000/pi; // y/x , convert into degrees

            set_pixel(dir , x , y , 0 , vec_dir);

            set_pixel(mag , x , y , 0 , sqrt((pixelx*pixelx)+(pixely*pixely)));
        }
    /*
    [ Feature Normalization ]
    and I guess we can even do treshold before feature normalize so we can get rid of the noise
    we really need feature normalzie here  *************************************
    as other function should assume that mag is normalzied before they use it
    */

    img_arr[0]=mag;
    img_arr[1]=dir;
    return img_arr;
}















/*

// canny edge detector
void calc_angle(image dir)
{

    int y=0 , x=0;
    for(y=0; y<dir.h; y++)
        for(x=0; x<dir.w; x++)
        {

            float angle=get_pixel(dir , x , y , 0)*(180.000/pi); // radians to degree
            // only 1 iteratio needed
            while(angle<0)
              angle+=360.00;
            while(angle>360)
              angle-=360.00;
            set_pixel(dir , x , y , 0 , angle);
        }
    return ;
}

void min_max_suppression(image mag , image dir)
{
    /*
        the algorithm says if it's greater than neighbours
        then leave it as it's
        other wise it's not an edge

        this helps us in connectivity using the treshold


    int y=0 , x=0;
    float center_pixel;
    float leftp , rightp;
    float upp , downp;
    float leftup , rightup;
    float leftdp , rightdp;

    // this will let non edges alive
    //feature_normalize(mag);  // was A BUG it cuzs the non edged to be colored

    for(y=0; y<mag.h; y++)
        for(x=0; x<mag.w; x++)
        {
            float angle=get_pixel(dir , x , y , 0);  // direction of the gradient
            center_pixel=get_pixel(mag , x , y , 0);

            //if(center_pixel==0)  // plan area
                //continue;
            // angle = 0  , 180 , 360 horizontal
            if(angle>=340 || (angle>=0 && angle<25) || (angle>=160 && angle<205))  // horizontal
            {
                leftp=get_pixel(mag , x-1 , y , 0);
                rightp=get_pixel(mag , x+1 , y , 0);

                if(center_pixel > leftp && center_pixel > rightp)  // = *******  bug
                    //set_pixel(mag , x , y , 0 , 1.0);   // this was a bug that need trashold sobel to pass
                    continue;
                else
                    set_pixel(mag , x , y , 0 , 0.0);
            }
            // angle = 45 , 225 main diagonal
            else if((angle>=25 && angle<70)  || (angle>=205 && angle<250))
            {
                // y increase to dwon direction
                rightup=get_pixel(mag , x+1 , y-1 , 0);
                leftdp=get_pixel(mag , x-1 , y+1 , 0);

                if(center_pixel>rightup && center_pixel>leftdp)
                    //set_pixel(mag , x , y , 0 , 1.0);
                    continue;
                else
                    set_pixel(mag , x , y , 0 , 0.0);
            }

            // angle = 90 , 270 vertical
            else if((angle>=70 && angle<115) || (angle>=250 && angle<295))
            {
                upp=get_pixel(mag , x , y-1 , 0);
                downp=get_pixel(mag , x , y+1 , 0);
                if(center_pixel>upp && center_pixel>downp)
                    //set_pixel(mag , x , y , 0 , 1.0);
                    continue;
                else
                    set_pixel(mag , x , y , 0 , 0.0);
            }

            // angle = 135 , 315   off diagonal
            else if((angle>=115 && angle<160) || (angle>=295 && angle<340))
            {
                leftup=get_pixel(mag , x-1 , y-1 , 0);
                rightdp=get_pixel(mag , x+1 , y+1 , 0);
                if(center_pixel>leftup && center_pixel>rightdp)
                    //set_pixel(mag , x , y , 0 , 1.0);
                    continue;
                else
                    set_pixel(mag , x , y , 0, 0.0);
            }
        }
    return ;
}


image treshold_canny(image mag)
{
    assert(mag.c==1 && "[treshold_canny]image have to be gray scale");

    /*

    I should get a way to make them adapted and automatically determinded
    one of them is calculating the mean
    and I guess nural network could work here !!!!!!!!!


    long double sum=0 , maxp=0;
    int y=0 , x=0;
    for(y=0; y<mag.h; y++)
        for(x=0; x<mag.w; x++)
        {
            float pixel=get_pixel(mag , x , y , 0); // mag is binary image
            //sum=sum+(long double) pixel;
            maxp=fmax(maxp , pixel);
        }

    //sum=sum/((long double)mag.h*mag.w); // mean
    //float maxi=sum+0.25*sum , mini=sum-0.25*sum;
    // theses strange parameters comes from github code
    float  maxi=maxp*0.09;
    float mini=maxi*.05;

    image tell=make_image(mag.w , mag.h , 1);
    for(y=0; y<mag.h; y++)
        for(x=0; x<mag.w; x++)
        {
            float pixel=get_pixel(mag , x , y , 0);

            // 0 not edge , 1 weak , 2 strong

            int kind=0;
            if(pixel>=maxi)
                kind=2;
            else if(pixel > mini)
                kind=1;

            set_pixel(tell , x , y , 0 , kind);  // int and float issue ****************************
        }
    return tell;
}

void connectivity_canny(image mag ,image tell)
{
    assert(mag.c==1 && tell.c==1 && "[connectivity_canny] deal with gray scale images only ");

    /*
        Here I MAKE weak connected to  weak connected to strign is a strong one
        this could differ from application to anotehr


    int y=0 , x=0;
    float l , u , lu , ru , c;
    for(y=0; y<mag.h; y++)
        for(x=0; x<mag.w; x++)
        {
            c=get_pixel(tell , x , y , 0);
            l=get_pixel(tell , x-1 , y , 0);
            u=get_pixel(tell , x , y-1 , 0);
            lu=get_pixel(tell , x-1 , y-1 , 0);
            ru=get_pixel(tell , x+1 , y-1 , 0);

            if(c==2)
                set_pixel(mag , x , y , 0 , 1.0);

            else if(c==1 && (l==2 || u==2 || lu==2 || ru==2))
            { //  weak become strong
                set_pixel(mag , x , y , 0 , 1.0);
                //set_pixel(tell , x , y , 0 , 1.0); // for other connected to you
            }
            else
                set_pixel(mag , x , y , 0 , 0.0);
        }
    return ;
}

image *canny(image im)
{
    // [TO DO]
    /*
    // smooth as additional step from me is this needed !??

    image gauess_kernal=make_gaussian_filter(1);
    im=convolve_image(im , gauess_kernal , 1);


    image* arr_img=sobel_image(im);
    /*
    //do we need to do this !!!!???????
    tresh_hold_sobel(arr_img[0] , .4);  // tres hold before feature_normalization !!!!??????


    calc_angle(arr_img[1]);
    min_max_suppression(arr_img[0] , arr_img[1]);
    image tell=treshold_canny(arr_img[0]);
    connectivity_canny(arr_img[0] , tell); // it cuzes alot of problems


    return arr_img;
}



// colors


image colorize(image mag , image dir)
{
    image colored_dir=make_image(dir.w , dir.h , 3);

    int y=0 , x=0;
    for(y=0; y<colored_dir.h; y++)
        for(x=0; x<colored_dir.w; x++)
        {

            float angle=get_pixel(dir , x , y , 0)*(180.000/pi);
            float mag_v=get_pixel(mag , x , y , 0);

            while(angle<0)
              angle+=360.00;
            while(angle>360)
              angle-=360.00;


            float red=0 , green=0 , blue=0;
            if(mag_v!=0)
            {
                if(angle>=0 && angle<=90)
                    red=1;
                else if(angle>90 && angle<=180)
                {
                    red=1;
                    green=1;
                }
                else if(angle>180 && angle<=270)
                    blue=1;
                else
                    green=.5;
            }
            set_pixel(colored_dir , x , y , 0 , red);
            set_pixel(colored_dir , x , y , 1 , green);
            set_pixel(colored_dir , x , y , 2 , blue);
        }
    return colored_dir;
}

image colorize_sobel(image im)
{
    /*
        this could be done in another way using HSV representation of the images
        as Hue get represented by the angle
        and saturation and value get represented by the magnitude of the gradient I guess !??
        look at the github read me of HW01 of the course

    // TODO
    image *img_arr=sobel_image(im);
    // I guess this is waste of memory **********************************? not alot , it's the same image pointer
    image mag=img_arr[0];  // copy or reference !??? *********************************************
    image dir=img_arr[1];
    feature_normalize(mag);  // min values become 0

    image colored_dir=colorize(mag , dir);
    return colored_dir;
}

image colorize_canny(image im)
{
    image* arr_img=canny(im);
    feature_normalize(arr_img[0]);  // min values become 0   [NOt tested yet ]
    image colored_dir_canny=colorize(arr_img[0] , arr_img[1]);
    return colored_dir_canny;
}
*/






/*
Hough line transform from udacity course
*/


void draw_line(image im , image acum)
{
    float maxi1=-1; // this was "+" I guess it could be a bug
    float maxi2=-1;
    int y=0 , y1=0  , y2=0 , x=0;
    int rho1=0;
    int rho2=0;

    /*
        [Bug lesson]
        theta was int and this was the bug that cuz the line to be drawn
        in a strange position
    */

    float theta1=0;
    float theta2=0;

    for(y=0; y<acum.h; y++)  // rho
        for(x=0; x<acum.w; x++) // theta
        {
            float pixle=get_pixel(acum , x , y , 0);
            if(pixle>maxi1)
            {
                maxi2=maxi1;
                maxi1=pixle;

                rho2=rho1;
                rho1=y;

                theta2=theta1;
                theta1=x;
            }
            else if(pixle>maxi2)
            {
                maxi2=pixle;
                rho2=y;
                theta2=x;
            }
        }
    //printf("%f \n"  ,maxi);
    theta1=theta1*pi/180.0;
    theta2=theta2*pi/180.0;

    // draw the line
    // y = (int) (rho - x * cosf(theta)) / sinf(theta);
    for(x=0; x<im.w; x++)
    {
        y1=(int)((rho1-x*cosf(theta1))/sinf(theta1));
        y2=(int)((rho2-x*cosf(theta2))/sinf(theta2));

        /*
            I don't have a clue of the values that could be generated
            in y1 and y2 so I DO THIS NAIVE checking with no reason
            just to keep my self safe from boundries checking

            dig deep in it
        */

        /*
            [Make the line more thick]
            second problem  the line is very thing that it don't appear in
            the high resolution images
        */

        if(y1>=0 && y1<im.h) // bug
        {
            printf("inside \n");
            set_pixel(im , x , y1, 0 , 0);
            set_pixel(im , x , y1, 1 , 1);
            set_pixel(im , x , y1, 2 , 0);
        }
        if(y2>=0 && y2<im.h)
        {
            printf("inside \n");
            set_pixel(im , x , y2, 0 , 1);
            set_pixel(im , x , y2, 1 , 0);
            set_pixel(im , x , y2, 2 , 0);
        }
    }

    return ;
}




image hough_line_transform(image im , image mag)
{
    //detect the edges
    //image* arr_img=sobel_image(im);
    //feature_normalize(arr_img[0]);

    // calculate the value of the tresh hold
    // make edge image be a binary edge image
    //tresh_hold_sobel(arr_img[0] , .6);
    //image mag=arr_img[0];

    // accumulator
    int diagonal=sqrt(im.h*im.h+im.w*im.w);
    image acum=make_image(361 , diagonal+1 , 1);

    int y=0 , x=0;
    for(y=0; y<acum.h; y++)
	for(x=0; x<acum.w; x++)
		set_pixel(acum , x , y , 0 , 0.0);

    for(y=0; y<mag.h; y++)
        for(x=0; x<mag.w; x++)
        {
            float pixel=get_pixel(mag , x , y , 0);

            if(pixel!=0)  // edge pixel
            {
                int theta=0;
                for(theta=0; theta<360; theta++) // bin size = 1
                {
                    /*
                        sinf and cosf work on radians angles
                    */

                    float ttheta=((float)theta)*pi/180.0;  // radians
                    float rho=(float)x*cosf(ttheta)+(float)y*sinf(ttheta);
                    /*
                        [ Question ]
                        I want to know what to do with the negative rho
                        when I convert it to +ve I guess it over voted for some row

                        and deleted one other potional line
                    */

                    //rho=abs(rho);
                    // vote
                    if(rho>=0 && rho<=diagonal)  // so that we don't vote for wrong positions
                    {
                        rho=(int)rho; // offset
                        float tpixel=get_pixel(acum , theta , rho , 0);
                        set_pixel(acum , theta , rho , 0 , tpixel+1);
                    }
                }
            }
        }
	draw_line(im , acum);
    return im;
}

/*
    Hough Transform for detecting a circle


    check for overflows as we square many values !!!????????
*/
void draw_circle(image im , image acum , int min_rad , int max_rad , int tresh)
{
    /*
    [range : min_rad , max_rad)
    I guess we can detect the max possible number of votes for a specific circle
    from the length of it's radius it will be <=4r ( there is a logic behind it ).

    [ edge detetcion ]
    very important note using sobel as edge detector brining me many problems as
    the edges are thick which may produce  strange , repeated behaviour
    so work on canny as soon as possible

    [   Complexity   ]
    :
    */
    int r , a , b;
    int y=0 , x=0 , c=0;
    int theta=0;

    printf("acum : %d %d %d \n" , acum.w , acum.h , acum.c);

    for(y=0; y<acum.h; y++)
        for(x=0; x<acum.w; x++)
            for(c=min_rad; c<fmin(acum.c , max_rad); c++)  // set by the user
            {
                //tresh=2*c;  // may be set by the user
                float vote=get_pixel(acum , x , y , c);
                if(vote>=tresh)  // valid circle
                {
                    // draw the circle
                    r=c;
                    a=x;
                    b=y;

                    //printf("circle : %d %d %d %d \n" , vote , r , a , b);
                    printf("circle \n");

                    /*
                        [ TO DO ]
                        we can make this better using the gradient information
                        to make the range of theta smaller
                    */

                    for(theta=0; theta<360; theta++)
                    {
                        float ttheta=theta;
                        ttheta=ttheta*pi/180.00;
                        // generate the point around the circle
                        int xx=(int)((float)a+r*cosf(ttheta));
                        int yy=(int)((float)b+r*sinf(ttheta));

                        if(xx>=0 && xx<im.w && yy>=0 && yy<im.h)
                        {
                            // red
                            set_pixel(im , xx , yy , 0 , 1);
                            set_pixel(im , xx , yy , 1 , 0);
                            set_pixel(im , xx , yy , 2 , 0);
                        }
                    }
                }
            }
    return ;
}
void hough_circle_transform(image im , image mag , image dir , int min_rad , int max_rad , int tresh)
{
    int y=0 , x=0 , c=0;
    int diagonal=sqrt(im.w*im.w+im.h*im.h);
    diagonal=diagonal/2; // max possible circle

    image acum=make_image(im.w , im.h , diagonal+1);  // no need for initialization thanks for calloc

    for(y=0; y<mag.h; y++)
        for(x=0; x<mag.w; x++)
        {
            float pixel=get_pixel(mag, x , y , 0);
            if(pixel==0)
                continue;

            int theta=0 , r=0;
            for(r=min_rad; r<max_rad; r++)
                for(theta=0; theta<360; theta++)
                {
                    float ttheta=theta;
                    ttheta=ttheta*pi/180.00;

                    int a , b;
                    a=(int) ((float)x-r*cosf(ttheta));
                    b=(int) ((float)y-r*sinf(ttheta));

                    if(a>=0 && a<im.w && b>=0 && b<im.h)
                    {
                        pixel=get_pixel(acum , a , b , r);
                        set_pixel(acum, a , b , r , pixel+1);
                    }
                }
        }
    draw_circle(im , acum , min_rad , max_rad , tresh);
    return ;
}
