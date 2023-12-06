#include "helpers.h"
#include <math.h>
#include <stdlib.h>
BYTE verif(int X);
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //lines
    for (int i = 0; i < height; i++)
    {
        //colomuns
        for (int j = 0; j < width; j++)
        {
            //converting
            if (!(image[i][j].rgbtRed == image[i][j].rgbtGreen && image[i][j].rgbtRed == image[i][j].rgbtBlue))
            {
                image[i][j].rgbtRed = round((float)(image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3);
                image[i][j].rgbtGreen = image[i][j].rgbtRed;
                image[i][j].rgbtBlue = image[i][j].rgbtRed;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int tempr;
    int tempg;
    int tempb;
    //parcouring the pixels
    for (int i = 0; i < height; i++)
    {

        for (int j = 0; j < width / 2; j++)
        {
            tempr = 0;
            tempg = 0;
            tempb = 0;
            //applying the effect
            tempr = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtRed = tempr;
            tempb = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][width - j - 1].rgbtBlue = tempb;
            tempg = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][width - j - 1].rgbtGreen = tempg;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image1[height][width];
    //parcouring the pixels
    for (int i = 0; i < height; i++)
    {
        int k = 0;
        for (int j = 0; j < width; j++)
        {
            //top-left corner
            if (i == 0 && j == 0)
            {
                image1[i][j].rgbtRed = round((float)(image[i + 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed +
                                                     image[i][j].rgbtRed) / 4);
                image1[i][j].rgbtGreen = round((float)(image[i + 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen +
                                                       image[i][j].rgbtGreen) / 4);
                image1[i][j].rgbtBlue = round((float)(image[i + 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue +
                                                      image[i][j].rgbtBlue) / 4);
            }
            //bottum-right corner
            else if (i == (height - 1) && j == (width - 1))
            {
                image1[i][j].rgbtRed = round((float)(image[i - 1][j].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed +
                                                     image[i][j].rgbtRed) / 4);
                image1[i][j].rgbtGreen = round((float)(image[i - 1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen +
                                                       image[i][j].rgbtGreen) / 4);
                image1[i][j].rgbtBlue = round((float)(image[i - 1][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue +
                                                      image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue) / 4);
            }
            //top-right corner
            else if (i == 0 && j == width - 1)
            {
                image1[i][j].rgbtRed = round((float)(image[i + 1][j].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed +
                                                     image[i][j].rgbtRed) / 4);
                image1[i][j].rgbtGreen = round((float)(image[i + 1][j].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen +
                                                       image[i][j].rgbtGreen) / 4);
                image1[i][j].rgbtBlue = round((float)(image[i + 1][j].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue +
                                                      image[i][j].rgbtBlue) / 4);
            }
            //bottum-left corner
            else if (i == height - 1 && j == 0)
            {
                image1[i][j].rgbtRed = round((float)(image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed +
                                                     image[i][j].rgbtRed) / 4);
                image1[i][j].rgbtGreen = round((float)(image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen +
                                                       image[i][j].rgbtGreen) / 4);
                image1[i][j].rgbtBlue = round((float)(image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue +
                                                      image[i][j].rgbtBlue) / 4);
            }
            //top edge
            else if (i == 0)
            {


                image1[i][j].rgbtRed = round((float)(image[i][j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed +
                                                     image[i + 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i][j].rgbtRed) / 6);
                image1[i][j].rgbtGreen = round((float)(image[i][j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen +
                                                       image[i + 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i][j].rgbtGreen) / 6);
                image1[i][j].rgbtBlue = round((float)(image[i][j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue +
                                                      image[i + 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i][j].rgbtBlue) / 6);

            }
            //left edge
            else if (j == 0)
            {
                image1[i][j].rgbtRed = round((float)(image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed +
                                                     image[i + 1][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 6);
                image1[i][j].rgbtGreen = round((float)(image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen +
                                                       image[i + 1][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 6);
                image1[i][j].rgbtBlue = round((float)(image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue +
                                                      image[i + 1][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 6);

            }
            //right edge
            else if (j == width - 1)
            {
                image1[i][j].rgbtRed = round((float)(image[i - 1][j].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed +
                                                     image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed + image[i][j].rgbtRed) / 6);
                image1[i][j].rgbtGreen = round((float)(image[i - 1][j].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen +
                                                       image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i][j].rgbtGreen) / 6);
                image1[i][j].rgbtBlue = round((float)(image[i - 1][j].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue +
                                                      image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i][j].rgbtBlue) / 6);

            }
            //bottm edge
            else if (i == height - 1)
            {
                image1[i][j].rgbtRed = round((float)(image[i][j - 1].rgbtRed + image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed +
                                                     image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed + image[i][j].rgbtRed) / 6);
                image1[i][j].rgbtGreen = round((float)(image[i][j - 1].rgbtGreen + image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen +
                                                       image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i][j].rgbtGreen) / 6);
                image1[i][j].rgbtBlue = round((float)(image[i][j - 1].rgbtBlue + image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue +
                                                      image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i][j].rgbtBlue) / 6);


            }

            //middle pixels
            else
            {
                image1[i][j].rgbtRed = round((float)(image[i - 1][j - 1].rgbtRed + image[i - 1][j].rgbtRed + image[i - 1][j + 1].rgbtRed +
                                                     image[i][j + 1].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i + 1][j].rgbtRed +
                                                     image[i + 1][j - 1].rgbtRed + image[i][j - 1].rgbtRed + image[i][j].rgbtRed) / 9);
                image1[i][j].rgbtGreen = round((float)(image[i - 1][j - 1].rgbtGreen + image[i - 1][j].rgbtGreen + image[i - 1][j + 1].rgbtGreen +
                                                       image[i][j + 1].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen +
                                                       image[i + 1][j - 1].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j].rgbtGreen) / 9);
                image1[i][j].rgbtBlue = round((float)(image[i - 1][j - 1].rgbtBlue + image[i - 1][j].rgbtBlue + image[i - 1][j + 1].rgbtBlue +
                                                      image[i][j + 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue +
                                                      image[i + 1][j - 1].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j].rgbtBlue) / 9);

            }
        }

    }
    //applying the values
    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            image[i][j].rgbtRed = image1[i][j].rgbtRed;
            image[i][j].rgbtGreen = image1[i][j].rgbtGreen;
            image[i][j].rgbtBlue = image1[i][j].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE Gx[height][width];
    RGBTRIPLE Gy[height][width];
    int Xx, Yx, Zx, Xy, Yy, Zy;
    //parcouring the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Xx = 0;
            Yx = 0;
            Zx = 0;
            Xy = 0;
            Yy = 0;
            Zy = 0;
            //top-left
            if (i == 0 && j == 0)
            {
                Xx = (image[i + 1][j + 1].rgbtRed + (image[i][j + 1].rgbtRed * 2));

                Yx = (image[i + 1][j + 1].rgbtGreen + (image[i][j + 1].rgbtGreen * 2));

                Zx = (image[i + 1][j + 1].rgbtBlue + (image[i][j + 1].rgbtBlue * 2));


                Xy = ((image[i + 1][j].rgbtRed) * (2) + (image[i + 1][j + 1].rgbtRed));

                Yy = (image[i + 1][j].rgbtGreen * (2) + (image[i + 1][j + 1].rgbtGreen));

                Zy = (image[i + 1][j].rgbtBlue * (2) + (image[i + 1][j + 1].rgbtBlue));

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);







            }
            //bottum-right corner
            else if (i == (height - 1) && j == (width - 1))
            {
                Xx = ((image[i][j - 1].rgbtRed) * (-2) - (image[i - 1][j - 1].rgbtRed));

                Yx = (image[i][j - 1].rgbtGreen * (-2) - (image[i - 1][j - 1].rgbtGreen));
                Zx = (image[i][j - 1].rgbtBlue * (-2) - (image[i - 1][j - 1].rgbtBlue));

                Xy = (image[i - 1][j].rgbtRed * (-2) - (image[i - 1][j - 1].rgbtRed));
                Yy = (image[i - 1][j].rgbtGreen * (-2) - (image[i - 1][j - 1].rgbtGreen));
                Zy = (image[i - 1][j].rgbtBlue * (-2) - (image[i - 1][j - 1].rgbtBlue));

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);



            }
            //top-right corner
            else if (i == 0 && j == width - 1)
            {
                Xx = (image[i][j - 1].rgbtRed * (-2) - (image[i + 1][j - 1].rgbtRed));
                Yx = (image[i][j - 1].rgbtGreen * (-2) - (image[i + 1][j - 1].rgbtGreen));
                Zx = (image[i][j - 1].rgbtBlue * (-2) - (image[i + 1][j - 1].rgbtBlue));

                Xy = (image[i + 1][j - 1].rgbtRed + (image[i + 1][j].rgbtRed * 2));
                Yy = (image[i + 1][j - 1].rgbtGreen + (image[i + 1][j].rgbtGreen * 2));
                Zy = (image[i + 1][j - 1].rgbtBlue + (image[i + 1][j].rgbtBlue * 2));

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);


            }
            //bottum-left corner
            else if (i == height - 1 && j == 0)
            {
                Xx = (image[i - 1][j + 1].rgbtRed + (image[i][j + 1].rgbtRed * 2));
                Yx = (image[i - 1][j + 1].rgbtGreen + (image[i][j + 1].rgbtGreen * 2));
                Zx = (image[i - 1][j + 1].rgbtBlue + (image[i][j + 1].rgbtBlue * 2));

                Xy = (image[i - 1][j].rgbtRed * (-2) - (image[i - 1][j + 1].rgbtRed));
                Yy = (image[i - 1][j].rgbtGreen * (-2) - (image[i - 1][j + 1].rgbtGreen));
                Zy = (image[i - 1][j].rgbtBlue * (-2) - (image[i - 1][j + 1].rgbtBlue));

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);

            }
            //top edge
            else if (i == 0)
            {
                Xx = image[i][j - 1].rgbtRed * (-2) - image[i + 1][j - 1].rgbtRed + image[i][j + 1].rgbtRed * (2) + image[i + 1][j + 1].rgbtRed;
                Yx = image[i][j - 1].rgbtGreen * (-2) - image[i + 1][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen *
                     (2) + image[i + 1][j + 1].rgbtGreen;
                Zx = image[i][j - 1].rgbtBlue * (-2) - image[i + 1][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue * (2) + image[i + 1][j + 1].rgbtBlue;

                Xy = image[i + 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed * (2) + image[i + 1][j + 1].rgbtRed;
                Yy = image[i + 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen * (2) + image[i + 1][j + 1].rgbtGreen;
                Zy = image[i + 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue * (2) + image[i + 1][j + 1].rgbtBlue;

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);

            }
            //left edge
            else if (j == 0)
            {
                Xx = image[i - 1][j + 1].rgbtRed + image[i][j + 1].rgbtRed * (2) + image[i + 1][j + 1].rgbtRed;
                Yx = image[i - 1][j + 1].rgbtGreen + image[i][j + 1].rgbtGreen * (2) + image[i + 1][j + 1].rgbtGreen;
                Zx = image[i - 1][j + 1].rgbtBlue + image[i][j + 1].rgbtBlue * (2) + image[i + 1][j + 1].rgbtBlue;

                Xy = image[i - 1][j].rgbtRed * (-2) - image[i - 1][j + 1].rgbtRed + image[i + 1][j].rgbtRed * (2) + image[i + 1][j + 1].rgbtRed;
                Yy = image[i - 1][j].rgbtGreen * (-2) - image[i - 1][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen *
                     (2) + image[i + 1][j + 1].rgbtGreen;
                Zy = image[i - 1][j].rgbtBlue * (-2) - image[i - 1][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue * (2) + image[i + 1][j + 1].rgbtBlue;

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);

            }
            //right edge
            else if (j == width - 1)
            {
                Xx = (-image[i - 1][j - 1].rgbtRed) + image[i][j - 1].rgbtRed * (-2) - image[i + 1][j - 1].rgbtRed;
                Yx = (-image[i - 1][j - 1].rgbtGreen) + image[i][j - 1].rgbtGreen * (-2) - image[i + 1][j - 1].rgbtGreen;
                Zx = (-image[i - 1][j - 1].rgbtBlue) + image[i][j - 1].rgbtBlue * (-2) - image[i + 1][j - 1].rgbtBlue;

                Xy = image[i - 1][j].rgbtRed * (-2) - image[i - 1][j - 1].rgbtRed + image[i + 1][j].rgbtRed * (2) + image[i + 1][j - 1].rgbtRed;
                Yy = image[i - 1][j].rgbtGreen * (-2) - image[i - 1][j - 1].rgbtGreen + image[i + 1][j].rgbtGreen *
                     (2) + image[i + 1][j - 1].rgbtGreen;
                Zy = image[i - 1][j].rgbtBlue * (-2) - image[i - 1][j - 1].rgbtBlue + image[i + 1][j].rgbtBlue * (2) + image[i + 1][j - 1].rgbtBlue;

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);
            }
            //bottum edge
            else if (i == height - 1)
            {
                Xx = image[i][j - 1].rgbtRed * (-2) - image[i - 1][j - 1].rgbtRed + image[i][j + 1].rgbtRed * (2) + image[i - 1][j + 1].rgbtRed;
                Yx = image[i][j - 1].rgbtGreen * (-2) - image[i - 1][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen *
                     (2) + image[i - 1][j + 1].rgbtGreen;
                Zx = image[i][j - 1].rgbtBlue * (-2) - image[i - 1][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue * (2) + image[i - 1][j + 1].rgbtBlue;

                Xy = (-image[i - 1][j - 1].rgbtRed) - image[i - 1][j].rgbtRed * (2) - image[i - 1][j + 1].rgbtRed;
                Yy = (-image[i - 1][j - 1].rgbtGreen) - image[i - 1][j].rgbtGreen * (2) - image[i - 1][j + 1].rgbtGreen;
                Zy = (-image[i - 1][j - 1].rgbtBlue) - image[i - 1][j].rgbtBlue * (2) - image[i - 1][j + 1].rgbtBlue;

                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);


            }
            //midlle pixel
            else
            {
                Xx = (-image[i - 1][j - 1].rgbtRed) + image[i][j - 1].rgbtRed * (-2) - image[i + 1][j - 1].rgbtRed + image[i + 1][j + 1].rgbtRed
                     + image[i][j + 1].rgbtRed * (2) + image[i - 1][j + 1].rgbtRed;
                Yx = (-image[i - 1][j - 1].rgbtGreen) + image[i][j - 1].rgbtGreen * (-2) - image[i + 1][j - 1].rgbtGreen + image[i + 1][j +
                        1].rgbtGreen + image[i][j + 1].rgbtGreen * (2) + image[i - 1][j + 1].rgbtGreen;
                Zx = (-image[i - 1][j - 1].rgbtBlue) + image[i][j - 1].rgbtBlue * (-2) - image[i + 1][j - 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                     + image[i][j + 1].rgbtBlue * (2) + image[i - 1][j + 1].rgbtBlue;

                Xy = (-image[i - 1][j - 1].rgbtRed) + image[i - 1][j].rgbtRed * (-2) + image[i + 1][j - 1].rgbtRed + image[i + 1][j + 1].rgbtRed +
                     image[i + 1][j].rgbtRed * (2) - image[i - 1][j + 1].rgbtRed;
                Yy = (-image[i - 1][j - 1].rgbtGreen) + image[i - 1][j].rgbtGreen * (-2) + image[i + 1][j - 1].rgbtGreen + image[i + 1][j +
                        1].rgbtGreen + image[i + 1][j].rgbtGreen * (2) - image[i - 1][j + 1].rgbtGreen;
                Zy = (-image[i - 1][j - 1].rgbtBlue) + image[i - 1][j].rgbtBlue * (-2) + image[i + 1][j - 1].rgbtBlue + image[i + 1][j + 1].rgbtBlue
                     + image[i + 1][j].rgbtBlue * (2) - image[i - 1][j + 1].rgbtBlue;
                //applying the values
                Gx[i][j].rgbtRed = verif(Xx);
                Gx[i][j].rgbtGreen = verif(Yx);
                Gx[i][j].rgbtBlue = verif(Zx);
                Gy[i][j].rgbtRed = verif(Xy);
                Gy[i][j].rgbtGreen = verif(Yy);
                Gy[i][j].rgbtBlue = verif(Zy);
            }
        }



    }
    //applying the values to the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //red values
            if (sqrt((Gx[i][j].rgbtRed * Gx[i][j].rgbtRed) + (Gy[i][j].rgbtRed * Gy[i][j].rgbtRed)) < 255
                &&  sqrt((Gx[i][j].rgbtRed * Gx[i][j].rgbtRed) + (Gy[i][j].rgbtRed * Gy[i][j].rgbtRed)) > 0)
            {
                image[i][j].rgbtRed = round(sqrt((Gx[i][j].rgbtRed * Gx[i][j].rgbtRed) + (Gy[i][j].rgbtRed * Gy[i][j].rgbtRed)));
            }
            else if (sqrt((Gx[i][j].rgbtRed * Gx[i][j].rgbtRed) + (Gy[i][j].rgbtRed * Gy[i][j].rgbtRed)) >= 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = 0;
            }
            //green values
            if (sqrt((Gx[i][j].rgbtGreen * Gx[i][j].rgbtGreen) + (Gy[i][j].rgbtGreen * Gy[i][j].rgbtGreen)) < 255
                &&  sqrt((Gx[i][j].rgbtGreen * Gx[i][j].rgbtGreen) + (Gy[i][j].rgbtGreen * Gy[i][j].rgbtGreen)) > 0)
            {
                image[i][j].rgbtGreen = round(sqrt((Gx[i][j].rgbtGreen * Gx[i][j].rgbtGreen) + (Gy[i][j].rgbtGreen * Gy[i][j].rgbtGreen)));
            }
            else if (sqrt((Gx[i][j].rgbtGreen * Gx[i][j].rgbtGreen) + (Gy[i][j].rgbtRed * Gy[i][j].rgbtGreen)) >= 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = 0;
            }
            //blue values
            if (sqrt((Gx[i][j].rgbtBlue * Gx[i][j].rgbtBlue) + (Gy[i][j].rgbtBlue * Gy[i][j].rgbtBlue)) < 255
                &&  sqrt((Gx[i][j].rgbtBlue * Gx[i][j].rgbtBlue) + (Gy[i][j].rgbtBlue * Gy[i][j].rgbtBlue)) > 0)
            {
                image[i][j].rgbtBlue = round(sqrt((Gx[i][j].rgbtBlue * Gx[i][j].rgbtBlue) + (Gy[i][j].rgbtBlue * Gy[i][j].rgbtBlue)));
            }
            else if (sqrt((Gx[i][j].rgbtBlue * Gx[i][j].rgbtBlue) + (Gy[i][j].rgbtBlue * Gy[i][j].rgbtBlue)) >= 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = 0;
            }


        }
    }

    return;
}
//fonction for making values between 0 and 255
BYTE verif(int X)
{
    if (X < -255  || X > 255)
    {
        return 255;
    }
    else
    {
        return abs(X);
    }



}