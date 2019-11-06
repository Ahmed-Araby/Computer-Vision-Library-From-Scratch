from uwimg import *













# assignment 3=02
"""
im = load_image("data/Rainier1.png")
# the issue ware in his treshold 
detect_and_draw_corners(im, 2, 1, 3)
save_image(im, "corners") 
a = load_image("data/Rainier1.png")
b = load_image("data/Rainier2.png")
m = find_and_draw_matches(a, b, 2, .7, 3)
save_image(m, "matches")
"""
im=load_image("data/circle.png")

# no gaussian 
f=make_gaussian_filter(1)   # extrach low frequency details 
im=convolve_image(im , f , 2)
# edge 
gray=rgb_to_grayscale(im)
res=sobel_image(gray)
feature_normalize(res[0])
#tresh_hold_sobel(res[0] , .3)   # this was a very critical factor without having the image binary it will produce strange results 
hough_circle_transform(im , res[0] , res[1] , 100 , 120 , 175)
save_image(im , "hough_circle_transform")

