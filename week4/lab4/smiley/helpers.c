#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    //loop for the widht
    for (int i = 0; i < width; i++)
    {
        //loop for the height
        for (int j = 0; j < height; j++)
        {
            //checking if the pixel is black
            if (image[j][i].rgbtBlue == 0x00 && image[j][i].rgbtRed == 0x00 && image[j][i].rgbtGreen == 0x00)
            {
                //changing the black pixels
                image[j][i].rgbtGreen = 0xff;
                image[j][i].rgbtRed = 0xff;
            }
        }
    }
}

