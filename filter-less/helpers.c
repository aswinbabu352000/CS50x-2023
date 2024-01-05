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
            int sepia_red   = min(round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue), 255);
            int sepia_green = min(round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue), 255);
            int sepia_blue  = min(round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue), 255);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // create a temp RGBTRIPLE variable for swapping values
    RGBTRIPLE temp;

    // itterate through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // swap values
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
    // creating copy of the image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // itterate through each pixel, take values from copy and write to image
    float sumRed;
    float sumGreen;
    float sumBlue;
    float counter;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            counter = 0;

            // nested loop to get each neighbour pixels (including that pixel itself)
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // check if each neighbouring pixel is inside image boundary
                    if ((i + k) >= 0 && (i + k) <= (height - 1) && (j + l) >= 0  && (j + l) <= (width - 1))
                    {
                        // add color values to respective sum
                        sumRed += copy[i + k][j + l].rgbtRed;
                        sumGreen += copy[i + k][j + l].rgbtGreen;
                        sumBlue += copy[i + k][j + l].rgbtBlue;
                        counter ++;
                    }
                }
            }

            // find averaage for each color and round it
            image[i][j].rgbtRed = round(sumRed / counter);
            image[i][j].rgbtGreen = round(sumGreen / counter);
            image[i][j].rgbtBlue = round(sumBlue / counter);
        }
    }

    return;
}