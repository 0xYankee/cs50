#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // For each pixel at height h and width w
    // Calculate the average of the RGB values and set the value to all RGB
    // Ensure it's a rounded number

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int avg = roundf((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = avg;
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtBlue = avg;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // For each pixel at height h and width w
    // Swap their values with the pixels at the opposite
    // Ensure it's a rounded number
    int half_width = roundf(width / 2.0);
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < half_width; w++)
        {
            int r = image[h][w].rgbtRed;
            int g = image[h][w].rgbtGreen;
            int b = image[h][w].rgbtBlue;

            int r_opposite = image[h][width - w - 1].rgbtRed;
            int g_opposite = image[h][width - w - 1].rgbtGreen;
            int b_opposite = image[h][width - w - 1].rgbtBlue;

            image[h][w].rgbtRed = r_opposite;
            image[h][w].rgbtGreen = g_opposite;
            image[h][w].rgbtBlue = b_opposite;

            image[h][width - w - 1].rgbtRed = r;
            image[h][width - w - 1].rgbtGreen = g;
            image[h][width - w - 1].rgbtBlue = b;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // For each pixel at height h and width w
    // Calculate the average of each RGB within the 3x3 box
    // Where only consider the case of each box if they are within the image size
    // Ensure it's a rounded number

    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Dupplicating original to a separate copy
            copy[h][w].rgbtRed = image[h][w].rgbtRed;
            copy[h][w].rgbtGreen = image[h][w].rgbtGreen;
            copy[h][w].rgbtBlue = image[h][w].rgbtBlue;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int r_sum = 0;
            int g_sum = 0;
            int b_sum = 0;
            int count = 0;
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (h + i >= 0 && w + j >= 0 && h + i < height && w + j < width)
                    {
                        // Taking only values from copy and not affecting calculations
                        r_sum += copy[h + i][w + j].rgbtRed;
                        g_sum += copy[h + i][w + j].rgbtGreen;
                        b_sum += copy[h + i][w + j].rgbtBlue;
                        count++;
                    }
                }
            }

            // Setting only values within original image
            image[h][w].rgbtRed = roundf((float)r_sum / count);
            image[h][w].rgbtGreen = roundf((float)g_sum / count);
            image[h][w].rgbtBlue = roundf((float)b_sum / count);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // For each pixel at height h and width w
    // Calculate the gx and gy values
    // Take the sum of all values, pixel outside of image is 0x00
    // If sum is close to 0, means no drastic change, means not an edge
    // If sum is a large positive/negative integer, means drastic change, means an edge

    int gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    RGBTRIPLE copy[height][width];
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Dupplicating original to a separate copy
            copy[h][w].rgbtRed = image[h][w].rgbtRed;
            copy[h][w].rgbtGreen = image[h][w].rgbtGreen;
            copy[h][w].rgbtBlue = image[h][w].rgbtBlue;
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int r_gx = 0;
            int g_gx = 0;
            int b_gx = 0;
            int r_gy = 0;
            int g_gy = 0;
            int b_gy = 0;
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if (h + i >= 0 && w + j >= 0 && h + i < height && w + j < width)
                    {
                        // For each RGB value, multiply by gx gy factor respectively and add to a sum
                        r_gx += copy[h + i][w + j].rgbtRed * gx[i + 1][j + 1];
                        g_gx += copy[h + i][w + j].rgbtGreen * gx[i + 1][j + 1];
                        b_gx += copy[h + i][w + j].rgbtBlue * gx[i + 1][j + 1];

                        r_gy += copy[h + i][w + j].rgbtRed * gy[i + 1][j + 1];
                        g_gy += copy[h + i][w + j].rgbtGreen * gy[i + 1][j + 1];
                        b_gy += copy[h + i][w + j].rgbtBlue * gy[i + 1][j + 1];
                    }
                }
            }

            int r = roundf((float)sqrt(r_gx * r_gx + r_gy * r_gy));
            int g = roundf((float)sqrt(g_gx * g_gx + g_gy * g_gy));
            int b = roundf((float)sqrt(b_gx * b_gx + b_gy * b_gy));

            image[h][w].rgbtRed = (r > 255) ? 255 : (r < 0) ? 0 : r;
            image[h][w].rgbtGreen = (g > 255) ? 255 : (g < 0) ? 0 : g;
            image[h][w].rgbtBlue = (b > 255) ? 255 : (b < 0) ? 0 : b;
        }
    }

    return;
}
