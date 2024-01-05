#include "helpers.h"
#include <math.h>
#include <stdio.h>

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
                    if ((i + k) >= 0 && (i + k) <= (height - 1) && (j + l) >= 0 && (j + l) <= (width - 1))
                    {
                        // add color values to respective sum
                        sumRed += copy[i + k][j + l].rgbtRed;
                        sumGreen += copy[i + k][j + l].rgbtGreen;
                        sumBlue += copy[i + k][j + l].rgbtBlue;
                        counter++;
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int pos;
    int GxsumRed, GysumRed;
    int GxsumGreen, GysumGreen;
    int GxsumBlue, GysumBlue;

    int Gx[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    // make a cop of orginal pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // get each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // reset values and position
            pos = 0;
            GxsumRed = 0, GysumRed = 0;
            GxsumGreen = 0, GysumGreen = 0;
            GxsumBlue = 0, GysumBlue = 0;

            // get each neighbouring pixel
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    // if inside border calculate sum
                    if ((i + k) >= 0 && (i + k) <= (height - 1) && (j + l) >= 0 && (j + l) <= (width - 1))
                    {
                        // calculate Gx
                        GxsumRed += copy[i + k][j + l].rgbtRed * Gx[pos];
                        GxsumGreen += copy[i + k][j + l].rgbtGreen * Gx[pos];
                        GxsumBlue += copy[i + k][j + l].rgbtBlue * Gx[pos];

                        // calculate Gy
                        GysumRed += copy[i + k][j + l].rgbtRed * Gy[pos];
                        GysumGreen += copy[i + k][j + l].rgbtGreen * Gy[pos];
                        GysumBlue += copy[i + k][j + l].rgbtBlue * Gy[pos];
                    }

                    // if outside border, consider it black
                    // black is 00, so no need to calculate

                    pos++;
                }
            }

            // calculate final value for each color and copying it into image
            image[i][j].rgbtRed = fmin(round(sqrt(pow(GxsumRed, 2) + pow(GysumRed, 2))), 255);
            image[i][j].rgbtGreen = fmin(round(sqrt(pow(GxsumGreen, 2) + pow(GysumGreen, 2))), 255);
            image[i][j].rgbtBlue = fmin(round(sqrt(pow(GxsumBlue, 2) + pow(GysumBlue, 2))), 255);
        }
    }
    return;
}