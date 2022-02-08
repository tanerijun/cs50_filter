#include <stdio.h>
#include <math.h>
#include "helpers.h"

// Function prototypes
float avg(int n[], int size);
void rgb_cap(int *xp);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Find average pixel value
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;
            int apv = round((red + green + blue) / 3);

            // Assign apv to rgb
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = apv;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop through each pixel
    // The mission is to switch the 1st pixel with the last pixel, 2nd pixel with the second last, ...
    for (int i = 0; i < height; i++)
    {
        // Loop until the middle of the pixels array
        for (int j = 0; j < (int)(width/2); j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width-j];
            image[i][width-j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary array containing the modified pixel
    // We musn't change the value of the pixel directly through the first loop because we need to calculate using the original value of the pixel
    // Ex: If you're on pixel (2,2), you still need the original value of pixel (1, 1), (1, 2), ...
    RGBTRIPLE blurred[height][width];

    // Approach:
    // NW N NE
    // W  X E
    // SW S SE
    // To find the pixel value for X, we'll have to look for the average value of x and the 8 pixels around it.

    // Loop through every pixel to fill the temporary (blurred) array
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // If pixel is in the corners, check only the 3 pixels around it
            // Upper left corner
            if (i == 0 && j == 0)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE E = image[i][j+1];
                RGBTRIPLE S = image[i+1][j];
                RGBTRIPLE SE = image[i+1][j+1];
                int R[] = {X.rgbtRed, E.rgbtRed, S.rgbtRed, SE.rgbtRed};
                int G[] = {X.rgbtGreen, E.rgbtGreen, S.rgbtGreen, SE.rgbtGreen};
                int B[] = {X.rgbtBlue, E.rgbtBlue, S.rgbtBlue, SE.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 4);
                blurred[i][j].rgbtGreen = avg(G, 4);
                blurred[i][j].rgbtBlue = avg(B, 4);

            }
            // Upper right corner
            else if (i == 0 && j == width - 1)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE W = image[i][j-1];
                RGBTRIPLE S = image[i+1][j];
                RGBTRIPLE SW = image[i+1][j+1];
                int R[] = {X.rgbtRed, W.rgbtRed, S.rgbtRed, SW.rgbtRed};
                int G[] = {X.rgbtGreen, W.rgbtGreen, S.rgbtGreen, SW.rgbtGreen};
                int B[] = {X.rgbtBlue, W.rgbtBlue, S.rgbtBlue, SW.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 4);
                blurred[i][j].rgbtGreen = avg(G, 4);
                blurred[i][j].rgbtBlue = avg(B, 4);
            }
            // Lower left corner
            else if (i == height - 1 && j == 0)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE E = image[i][j+1];
                RGBTRIPLE N = image[i-1][j];
                RGBTRIPLE NE = image[i-1][j+1];
                int R[] = {X.rgbtRed, E.rgbtRed, N.rgbtRed, NE.rgbtRed};
                int G[] = {X.rgbtGreen, E.rgbtGreen, N.rgbtGreen, NE.rgbtGreen};
                int B[] = {X.rgbtBlue, E.rgbtBlue, N.rgbtBlue, NE.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 4);
                blurred[i][j].rgbtGreen = avg(G, 4);
                blurred[i][j].rgbtBlue = avg(B, 4);
            }
            // Lower right corner
            else if (i == height - 1 && j == width - 1)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE W = image[i][j-1];
                RGBTRIPLE N = image[i-1][j];
                RGBTRIPLE NW = image[i-1][j-1];
                int R[] = {X.rgbtRed, W.rgbtRed, N.rgbtRed, NW.rgbtRed};
                int G[] = {X.rgbtGreen, W.rgbtGreen, N.rgbtGreen, NW.rgbtGreen};
                int B[] = {X.rgbtBlue, W.rgbtBlue, N.rgbtBlue, NW.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 4);
                blurred[i][j].rgbtGreen = avg(G, 4);
                blurred[i][j].rgbtBlue = avg(B, 4);
            }
            // If pixel is in the edge, check the 5 pixels around it
            // Top edge
            else if (i == 0)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE W = image[i][j-1];
                RGBTRIPLE SW = image[i+1][j+1];
                RGBTRIPLE S = image[i+1][j];
                RGBTRIPLE SE = image[i+1][j+1];
                RGBTRIPLE E = image[i][j+1];
                int R[] = {X.rgbtRed, W.rgbtRed, SW.rgbtRed, S.rgbtRed, SE.rgbtRed, E.rgbtRed};
                int G[] = {X.rgbtGreen, W.rgbtGreen, SW.rgbtGreen, S.rgbtGreen, SE.rgbtGreen, E.rgbtGreen};
                int B[] = {X.rgbtBlue, W.rgbtBlue, SW.rgbtBlue, S.rgbtBlue, SE.rgbtBlue, E.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 6);
                blurred[i][j].rgbtGreen = avg(G, 6);
                blurred[i][j].rgbtBlue = avg(B, 6);
            }
            // Left edge
            else if (j == 0)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE N = image[i-1][j];
                RGBTRIPLE NE = image[i-1][j+1];
                RGBTRIPLE E = image[i][j+1];
                RGBTRIPLE SE = image[i+1][j+1];
                RGBTRIPLE S = image[i+1][j];
                int R[] = {X.rgbtRed, N.rgbtRed, NE.rgbtRed, E.rgbtRed, SE.rgbtRed, S.rgbtRed};
                int G[] = {X.rgbtGreen, N.rgbtGreen, NE.rgbtGreen, E.rgbtGreen, SE.rgbtGreen, S.rgbtGreen};
                int B[] = {X.rgbtBlue, N.rgbtBlue, NE.rgbtBlue, E.rgbtBlue, SE.rgbtBlue, S.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 6);
                blurred[i][j].rgbtGreen = avg(G, 6);
                blurred[i][j].rgbtBlue = avg(B, 6);
            }
            // Bottom edge
            else if (i == height - 1)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE W = image[i][j-1];
                RGBTRIPLE NW = image[i-1][j-1];
                RGBTRIPLE N = image[i-1][j];
                RGBTRIPLE NE = image[i-1][j+1];
                RGBTRIPLE E = image[i][j+1];
                int R[] = {X.rgbtRed, W.rgbtRed, NW.rgbtRed, N.rgbtRed, NE.rgbtRed, E.rgbtRed};
                int G[] = {X.rgbtGreen, W.rgbtGreen, NW.rgbtGreen, N.rgbtGreen, NE.rgbtGreen, E.rgbtGreen};
                int B[] = {X.rgbtBlue, W.rgbtBlue, NW.rgbtBlue, N.rgbtBlue, NE.rgbtBlue, E.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 6);
                blurred[i][j].rgbtGreen = avg(G, 6);
                blurred[i][j].rgbtBlue = avg(B, 6);
            }
            // Right edge
            else if (j == width - 1)
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE N = image[i-1][j];
                RGBTRIPLE NW = image[i-1][j-1];
                RGBTRIPLE W = image[i][j-1];
                RGBTRIPLE SW = image[i+1][j-1];
                RGBTRIPLE S = image[i+1][j];
                int R[] = {X.rgbtRed, N.rgbtRed, NW.rgbtRed, W.rgbtRed, SW.rgbtRed, S.rgbtRed};
                int G[] = {X.rgbtGreen, N.rgbtGreen, NW.rgbtGreen, W.rgbtGreen, SW.rgbtGreen, S.rgbtGreen};
                int B[] = {X.rgbtBlue, N.rgbtBlue, NW.rgbtBlue, W.rgbtBlue, SW.rgbtBlue, S.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 6);
                blurred[i][j].rgbtGreen = avg(G, 6);
                blurred[i][j].rgbtBlue = avg(B, 6);
            }
            // All the other pixels
            else
            {
                RGBTRIPLE X = image[i][j];
                RGBTRIPLE N = image[i-1][j];
                RGBTRIPLE NE = image[i-1][j+1];
                RGBTRIPLE E = image[i][j+1];
                RGBTRIPLE SE = image[i+1][j+1];
                RGBTRIPLE S = image[i+1][j];
                RGBTRIPLE SW = image[i+1][j-1];
                RGBTRIPLE W = image[i][j-1];
                RGBTRIPLE NW = image[i-1][j-1];
                int R[] = {X.rgbtRed, N.rgbtRed, NE.rgbtRed, E.rgbtRed, SE.rgbtRed, S.rgbtRed, SW.rgbtRed, W.rgbtRed, NW.rgbtRed};
                int G[] = {X.rgbtGreen, N.rgbtGreen, NE.rgbtGreen, E.rgbtGreen, SE.rgbtGreen, S.rgbtGreen, SW.rgbtGreen, W.rgbtGreen, NW.rgbtGreen};
                int B[] = {X.rgbtBlue, N.rgbtBlue, NE.rgbtBlue, E.rgbtBlue, SE.rgbtBlue, S.rgbtBlue, SW.rgbtBlue, W.rgbtBlue, NW.rgbtBlue};
                // Store average pixel value in temp array
                blurred[i][j].rgbtRed = avg(R, 9);
                blurred[i][j].rgbtGreen = avg(G, 9);
                blurred[i][j].rgbtBlue = avg(B, 9);
            }
        }
    }

    // Loop through every pixel assigning blurred value to the original pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blurred[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Edge detection algorithm: Sobel filter algorithm
    // Compute the new value of each pixel by taking a weighted sum of the values for the surrounding pixels.
    // Since edges between objects could take place in both a vertical and a horizontal direction, we’ll actually compute two weighted sums.
    // Gx: -1 0 1   Gy: -1 -2 -1
    //     -2 0 2        0  0  0
    //     -1 0 1        1  2  1

    // Temporary array containing the modified pixel
    RGBTRIPLE modified[height][width];

    // Loop over the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // If the pixel is in the corner or the edge, we'll think of the non existant pixel as a solid black pixel (rgb(0, 0, 0))
            RGBTRIPLE black;
            black.rgbtRed = black.rgbtGreen = black.rgbtBlue = 0;

            RGBTRIPLE X = image[i][j];
            RGBTRIPLE N = black;
            RGBTRIPLE NE = black;
            RGBTRIPLE E = black;
            RGBTRIPLE SE = black;
            RGBTRIPLE S = black;
            RGBTRIPLE SW = black;
            RGBTRIPLE W = black;
            RGBTRIPLE NW = black;

            // Top left corner
            if (i == 0 && j == 0)
            {
                E = image[i][j+1];
                SE = image[i+1][j+1];
                S = image[i+1][j];
            }
            // Top right corner
            else if (i == 0 && j == width - 1)
            {
                W = image[i][j-1];
                SW = image[i+1][j-1];
                S = image[i+1][j];
            }
            // Bottom left corner
            else if (i == height - 1 && j == 0)
            {
                N = image[i-1][j];
                NE = image[i-1][j+1];
                E = image[i][j+1];
            }
            // Bottom right corner
            else if (i == height - 1 && j == width - 1)
            {
                N = image[i-1][j];
                NW = image[i-1][j-1];
                W = image[i][j-1];
            }
            // Top edge
            else if (i == 0)
            {
                E = image[i][j+1];
                SE = image[i+1][j-1];
                S = image[i+1][j];
                SW = image[i+1][j-1];
                W = image[i][j-1];
            }
            // Right edge
            else if (j == width - 1)
            {
                S = image[i+1][j];
                SW = image[i+1][j-1];
                W = image[i][j-1];
                NW = image[i-1][j-1];
                N = image[i-1][j];
            }
            // Bottom edge
            else if (i == height - 1)
            {
                W = image[i][j-1];
                NW = image[i-1][j-1];
                N = image[i-1][j];
                NE = image[i-1][j+1];
                E = image[i][j+1];
            }
            // Left edge
            else if (j == 0)
            {
                N = image[i-1][j];
                NE = image[i-1][j+1];
                E = image[i][j+1];
                SE = image[i+1][j+1];
                S = image[i+1][j];
            }
            // Other pixels
            else
            {
                N = image[i-1][j];
                NE = image[i-1][j+1];
                E = image[i][j+1];
                SE = image[i+1][j+1];
                S = image[i+1][j];
                SW = image[i+1][j-1];
                W = image[i][j-1];
                NW = image[i-1][j-1];
            }

            // Calculate the weighted sum
            int RGx = (-1 * NW.rgbtRed) + (0 * N.rgbtRed) + (1 * NE.rgbtRed) + (-2 * W.rgbtRed) + (0 + X.rgbtRed) + (2 * E.rgbtRed) + (-1 * SW.rgbtRed) + (0 * S.rgbtRed) + (1 * SE.rgbtRed);
            int RGy = (-1 * NW.rgbtRed) + (-2 * N.rgbtRed) + (-1 * NE.rgbtRed) + (0 * W.rgbtRed) + (0 + X.rgbtRed) + (0 * E.rgbtRed) + (1 * SW.rgbtRed) + (2 * S.rgbtRed) + (1 * SE.rgbtRed);
            int GGx = (-1 * NW.rgbtGreen) + (0 * N.rgbtGreen) + (1 * NE.rgbtGreen) + (-2 * W.rgbtGreen) + (0 + X.rgbtGreen) + (2 * E.rgbtGreen) + (-1 * SW.rgbtGreen) + (0 * S.rgbtGreen) + (1 * SE.rgbtGreen);
            int GGy = (-1 * NW.rgbtGreen) + (-2 * N.rgbtGreen) + (-1 * NE.rgbtGreen) + (0 * W.rgbtGreen) + (0 + X.rgbtGreen) + (0 * E.rgbtGreen) + (1 * SW.rgbtGreen) + (2 * S.rgbtGreen) + (1 * SE.rgbtGreen);
            int BGx = (-1 * NW.rgbtBlue) + (0 * N.rgbtBlue) + (1 * NE.rgbtBlue) + (-2 * W.rgbtBlue) + (0 + X.rgbtBlue) + (2 * E.rgbtBlue) + (-1 * SW.rgbtBlue) + (0 * S.rgbtBlue) + (1 * SE.rgbtBlue);
            int BGy = (-1 * NW.rgbtBlue) + (-2 * N.rgbtBlue) + (-1 * NE.rgbtBlue) + (0 * W.rgbtBlue) + (0 + X.rgbtBlue) + (0 * E.rgbtBlue) + (1 * SW.rgbtBlue) + (2 * S.rgbtBlue) + (1 * SE.rgbtBlue);

            // Calculate new RGB value
            int R = sqrt(pow(RGx, 2) + pow(RGy, 2));
            int G = sqrt(pow(GGx, 2) + pow(GGy, 2));
            int B = sqrt(pow(BGx, 2) + pow(BGy, 2));

            // Cap the value at 255
            rgb_cap(&R);
            rgb_cap(&G);
            rgb_cap(&B);

            // Assign new RGB value to temp array
            modified[i][j].rgbtRed = R;
            modified[i][j].rgbtGreen = G;
            modified[i][j].rgbtBlue = B;
        }
    }

    // Loop through the image once more, this time replacing each pixel with the modified version
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = modified[i][j];
        }
    }

    return;
}

// Calculate Average
float avg(int n[], int size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += n[i];
    }
    return (sum / size);
}

// Cap RGB value at 255
void rgb_cap(int *xp)
{
    if (*xp > 255)
    {
        *xp = 255;
    }
}
