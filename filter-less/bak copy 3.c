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
            image[i][j].rgbtRed     = average;
            image[i][j].rgbtGreen   = average;
            image[i][j].rgbtBlue    = average;
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
            int sepia_red   = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepia_green = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepia_blue  = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            if (sepia_red > 0xff)
            {
                image[i][j].rgbtRed = 0xff;
            }
            else
            {
                image[i][j].rgbtGreen = sepia_green;
            }

            if (sepia_green > 0xff)
            {
                image[i][j].rgbtGreen = 0xff;
            }
            else
            {
                image[i][j].rgbtRed = sepia_red;
            }

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

    // itterate through each pixel, take values from copy and write to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // top left corner
            if (i == 0 && j == 0)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i + 1][j].rgbtRed    + copy[i][j + 1].rgbtRed    + copy[i + 1][j + 1].rgbtRed)   / 4.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i + 1][j].rgbtGreen  + copy[i][j + 1].rgbtGreen  + copy[i + 1][j + 1].rgbtGreen) / 4.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i + 1][j].rgbtBlue   + copy[i][j + 1].rgbtBlue   + copy[i + 1][j + 1].rgbtBlue)  / 4.0);
            }
            // bottom left corner
            else if (i == height - 1 && j == 0)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i - 1][j].rgbtRed    + copy[i][j + 1].rgbtRed    + copy[i - 1][j + 1].rgbtRed)   / 4.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i - 1][j].rgbtGreen  + copy[i][j + 1].rgbtGreen  + copy[i - 1][j + 1].rgbtGreen) / 4.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i - 1][j].rgbtBlue   + copy[i][j + 1].rgbtBlue   + copy[i - 1][j + 1].rgbtBlue)  / 4.0);
            }
            // top right corner
            else if (i == 0 && j == width - 1)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i][j - 1].rgbtRed    + copy[i + 1][j].rgbtRed    + copy[i + 1][j - 1].rgbtRed)   / 4.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i][j - 1].rgbtGreen  + copy[i + 1][j].rgbtGreen  + copy[i + 1][j - 1].rgbtGreen) / 4.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i][j - 1].rgbtBlue   + copy[i + 1][j].rgbtBlue   + copy[i + 1][j - 1].rgbtBlue)  / 4.0);
            }
            // bottom right corner
            else if (i == height - 1 && j == width - 1)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i - 1][j].rgbtRed    + copy[i][j - 1].rgbtRed    + copy[i - 1][j - 1].rgbtRed)    / 4.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i - 1][j].rgbtGreen  + copy[i][j - 1].rgbtGreen  + copy[i - 1][j - 1].rgbtGreen)  / 4.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i - 1][j].rgbtBlue   + copy[i][j - 1].rgbtBlue   + copy[i - 1][j - 1].rgbtBlue)   / 4.0);
            }
            // left edge
            else if (i > 0 && i < height - 1 && j == 0)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i - 1][j].rgbtRed    + copy[i + 1][j].rgbtRed    + copy[i][j + 1].rgbtRed   + copy[i - 1][j + 1].rgbtRed    + copy[i + 1][j + 1].rgbtRed)    / 6.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i - 1][j].rgbtGreen  + copy[i + 1][j].rgbtGreen  + copy[i][j + 1].rgbtGreen + copy[i - 1][j + 1].rgbtGreen  + copy[i + 1][j + 1].rgbtGreen)  / 6.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i - 1][j].rgbtBlue   + copy[i + 1][j].rgbtBlue   + copy[i][j + 1].rgbtBlue  + copy[i - 1][j + 1].rgbtBlue   + copy[i + 1][j + 1].rgbtBlue)   / 6.0);
            }
            // right edge
            else if (i > 0 && i < height - 1 && j == width - 1)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i - 1][j].rgbtRed    + copy[i + 1][j].rgbtRed    + copy[i][j - 1].rgbtRed    +  copy[i - 1][j - 1].rgbtRed   + copy[i + 1][j - 1].rgbtRed)   / 6.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i - 1][j].rgbtGreen  + copy[i + 1][j].rgbtGreen  + copy[i][j - 1].rgbtGreen  + copy[i - 1][j - 1].rgbtGreen  + copy[i + 1][j - 1].rgbtGreen) / 6.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i - 1][j].rgbtBlue   + copy[i + 1][j].rgbtBlue   + copy[i][j - 1].rgbtBlue   + copy[i - 1][j - 1].rgbtBlue   + copy[i + 1][j - 1].rgbtBlue)  / 6.0);
            }
            // top edge
            else if (i == 0 && j > 0 && j < width - 1)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i + 1][j].rgbtRed    + copy[i][j - 1].rgbtRed    + copy[i][j + 1].rgbtRed    + copy[i + 1][j - 1].rgbtRed    + copy[i + 1][j + 1].rgbtRed)   / 6.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i + 1][j].rgbtGreen  + copy[i][j - 1].rgbtGreen  + copy[i][j + 1].rgbtGreen  + copy[i + 1][j - 1].rgbtGreen  + copy[i + 1][j + 1].rgbtGreen) / 6.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i + 1][j].rgbtBlue   + copy[i][j - 1].rgbtBlue   + copy[i][j + 1].rgbtBlue   + copy[i + 1][j - 1].rgbtBlue   + copy[i + 1][j + 1].rgbtBlue)  / 6.0);
            }
            // bottom edge
            else if (i == height - 1 && j > 0 && j < width - 1)
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i - 1][j].rgbtRed    + copy[i][j - 1].rgbtRed    + copy[i][j + 1].rgbtRed    + copy[i - 1][j - 1].rgbtRed    + copy[i - 1][j + 1].rgbtRed)   / 6.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i - 1][j].rgbtGreen  + copy[i][j - 1].rgbtGreen  + copy[i][j + 1].rgbtGreen  + copy[i - 1][j - 1].rgbtGreen  + copy[i - 1][j + 1].rgbtGreen) / 6.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i - 1][j].rgbtBlue   + copy[i][j - 1].rgbtBlue   + copy[i][j + 1].rgbtBlue   + copy[i - 1][j - 1].rgbtBlue   + copy[i - 1][j + 1].rgbtBlue)  / 6.0);
            }
            // middle pixels
            else
            {
                image[i][j].rgbtRed     = round((copy[i][j].rgbtRed     + copy[i - 1][j].rgbtRed    + copy[i + 1][j].rgbtRed    + copy[i][j - 1].rgbtRed + copy[i][j + 1].rgbtRed       + copy[i - 1][j - 1].rgbtRed    + copy[i + 1][j - 1].rgbtRed    + copy[i - 1][j + 1].rgbtRed    + copy[i + 1][j + 1].rgbtRed)   / 9.0);
                image[i][j].rgbtGreen   = round((copy[i][j].rgbtGreen   + copy[i - 1][j].rgbtGreen  + copy[i + 1][j].rgbtGreen  + copy[i][j - 1].rgbtGreen + copy[i][j + 1].rgbtGreen   + copy[i - 1][j - 1].rgbtGreen  + copy[i + 1][j - 1].rgbtGreen  + copy[i - 1][j + 1].rgbtGreen  + copy[i + 1][j + 1].rgbtGreen) / 9.0);
                image[i][j].rgbtBlue    = round((copy[i][j].rgbtBlue    + copy[i - 1][j].rgbtBlue   + copy[i + 1][j].rgbtBlue   + copy[i][j - 1].rgbtBlue + copy[i][j + 1].rgbtBlue     + copy[i - 1][j - 1].rgbtBlue   + copy[i + 1][j - 1].rgbtBlue   + copy[i - 1][j + 1].rgbtBlue   + copy[i + 1][j + 1].rgbtBlue)  / 9.0);
            }
        }
    }

    return;
}