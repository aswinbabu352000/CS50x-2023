#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average = 0;

    // loops to get pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loops to get pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sepia_red = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepia_green = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepia_blue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            // red
            if (sepia_red > 0xff)
            {
                image[i][j].rgbtRed = 0xff;
            }

            else
            {
                image[i][j].rgbtRed = sepia_red;
            }

            // green
            if (sepia_green > 0xff)
            {
                image[i][j].rgbtGreen = 0xff;
            }

            else
            {
                image[i][j].rgbtGreen = sepia_green;
            }

            // blue
            if (sepia_blue > 0xff)
            {
                image[i][j].rgbtBlue = 0xff;
            }

            else
            {
                image[i][j].rgbtBlue = sepia_blue;
            }
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp = {0, 0, 0};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = image[i][j];
            image[i][j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    // copying pixels to a new 2D array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }



    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
             RGBTRIPLE sum = copy[i][j];
             float divisor = 0;
             bool corner = false;
             bool edge = false;

            // top left corner
            if (i = 0 && j = 0)
            {
                RGBTRIPLE one_down = copy[i + 1][j];
                RGBTRIPLE one_right = copy[i][j + 1];
                RGBTRIPLE top_right = copy[i - 1][j + 1];

                sum += one_right + one_down + top_right;
                corner = true;
            }

            // bottom left corner
            else if (i = height - 1 && j = 0)
            {
                RGBTRIPLE one_up = copy[i - 1][j];
                RGBTRIPLE one_right = copy[i][j + 1];
                RGBTRIPLE top_right = copy[i - 1][j + 1];

                sum += one_up + one_right + top_right;
                corner = true;
            }

            //top right corner
            else if (i = 0 && j = width - 1)
            {
                RGBTRIPLE one_left = copy[i][j - 1];
                RGBTRIPLE one_down = copy[i + 1][j];
                RGBTRIPLE bottom_left = copy[i + 1][j - 1];

                sum += one_down + one_left + bottom_left;
                corner = true;
            }

            // bottom right corner
            else if (i = height - 1 && j = width - 1)
            {
                RGBTRIPLE one_up = copy[i - 1][j];
                RGBTRIPLE one_left = copy[i][j - 1];
                RGBTRIPLE top_left = copy[i - 1][j - 1];

                sum += one_up + one_left + top_left;
                corner = true;
            }

            // left edge
            else if (i > 0 && i < height - 1 && j = 0)
            {
                RGBTRIPLE one_up = copy[i - 1][j];
                RGBTRIPLE one_down = copy[i + 1][j];
                RGBTRIPLE one_right = copy[i][j + 1];
                RGBTRIPLE top_right = copy[i - 1][j + 1];
                RGBTRIPLE bottom_right = copy[i + 1][j + 1];

            }

             // right edge
            else if (i > 0 && i < height - 1 && j = width - 1)
            {
                RGBTRIPLE one_up = copy[i - 1][j];
                RGBTRIPLE one_down = copy[i + 1][j];
                RGBTRIPLE one_left = copy[i][j - 1];
                RGBTRIPLE top_left = copy[i - 1][j - 1];
                RGBTRIPLE bottom_left = copy[i + 1][j - 1];


            }

             // top edge
            else if (i = 0 && j > 0 && j < width - 1)
            {
                RGBTRIPLE one_down = copy[i + 1][j];
                RGBTRIPLE one_left = copy[i][j - 1];
                RGBTRIPLE one_right = copy[i][j + 1];
                RGBTRIPLE bottom_left = copy[i + 1][j - 1];
                RGBTRIPLE bottom_right = copy[i + 1][j + 1];

            }

             // bottom edge
            else if (i = height - 1 && j > 0 && j < width - 1)
            {
                RGBTRIPLE one_up = copy[i - 1][j];
                RGBTRIPLE one_left = copy[i][j - 1];
                RGBTRIPLE one_right = copy[i][j + 1];
                RGBTRIPLE top_left = copy[i - 1][j - 1];
                RGBTRIPLE top_right = copy[i - 1][j + 1];

            }



            // not edge not corner
            else
            {
                RGBTRIPLE one_up = copy[i - 1][j];
                RGBTRIPLE one_down = copy[i + 1][j];
                RGBTRIPLE one_left = copy[i][j - 1];
                RGBTRIPLE one_right = copy[i][j + 1];
                RGBTRIPLE top_left = copy[i - 1][j - 1];
                RGBTRIPLE bottom_left = copy[i + 1][j - 1];
                RGBTRIPLE top_right = copy[i - 1][j + 1];
                RGBTRIPLE bottom_right = copy[i + 1][j + 1];


            }





                if (corner == true)
                {
                    divisor = 4.0;
                }
                else if (edge == true)
                {
                    divisor = 6.0;
                }
                else
                {
                    divisor = 9.0;
                }


                // FINAL CALCULATIONS
                image[i][j].rbtRed = round (sum.rbtRed / divisor);
                image[i][j].rbtGreen = round (sum.rbtGreen / divisor);
                image[i][j].rbtBlue = round (sum.rbtBlue / divisor);


            }
        }

    return;
}
