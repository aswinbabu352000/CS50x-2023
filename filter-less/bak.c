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
    RGBTRIPLE temp;
    temp.rgbtRed = 0x00;
    temp.rgbtBlue = 0x00;
    temp.rgbtGreen = 0x00;

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

            // not edge or corner
            if (i != 0 || j != 0 || i != height || j != width)
            {
                RGBTRIPLE one_up = copy[i - 1][j];
                RGBTRIPLE one_down = copy[i + 1][j];
                RGBTRIPLE one_left = copy[i][j - 1];
                RGBTRIPLE one_right = copy[i][j + 1];
                RGBTRIPLE top_left = copy[i - 1][j - 1];
                RGBTRIPLE bottom_left = copy[i + 1][j - 1];
                RGBTRIPLE top_right = copy[i - 1][j + 1];
                RGBTRIPLE bottom_right = copy[i + 1][j + 1];

                image[i][j].rgbtRed =
                    round((copy[i][j].rgbtRed + one_up.rgbtRed + one_down.rgbtRed + one_left.rgbtRed + one_right.rgbtRed +
                           top_left.rgbtRed + bottom_left.rgbtRed + top_right.rgbtRed + bottom_right.rgbtRed) /
                          9.0);
                image[i][j].rgbtGreen =
                    round((copy[i][j].rgbtGreen + one_up.rgbtGreen + one_down.rgbtGreen + one_left.rgbtGreen + one_right.rgbtGreen +
                           top_left.rgbtGreen + bottom_left.rgbtGreen + top_right.rgbtGreen + bottom_right.rgbtGreen) /
                          9.0);
                image[i][j].rgbtBlue =
                    round((copy[i][j].rgbtBlue + one_up.rgbtBlue + one_down.rgbtBlue + one_left.rgbtBlue + one_right.rgbtBlue +
                           top_left.rgbtBlue + bottom_left.rgbtBlue + top_right.rgbtBlue + bottom_right.rgbtBlue) /
                          9.0);
            }


            // top left corner
            if (j == 0)
            {

                    image[i][j].rgbtRed = round(((total_red - top_right.rgbtRed) - one_up.rgbtRed) / 4.0);
                    image[i][j].rgbtGreen = round(((total_green - top_right.rgbtGreen) - one_up.rgbtGreen) / 4.0);
                    image[i][j].rgbtBlue = round(((total_blue - top_right.rgbtBlue) - one_up.rgbtBlue) / 4.0);
            }

                // bottom left corner
            else if (j == 0)
            {
                    image[i][j].rgbtRed = round(((total_red - bottom_right.rgbtRed) - one_down.rgbtRed) / 4.0);
                    image[i][j].rgbtGreen = round(((total_green - bottom_right.rgbtGreen) - one_down.rgbtGreen) / 4.0);
                    image[i][j].rgbtBlue = round(((total_blue - bottom_right.rgbtBlue) - one_down.rgbtBlue) / 4.0);
            }

            // not corner but edge

            else
            {
                    image[i][j].rgbtRed = round(total_red / 6.0);
                    image[i][j].rgbtGreen = round(total_green / 6.0);
                    image[i][j].rgbtBlue = round(total_blue / 6.0);
            }
            }
        }
    }

    return;
}
