#include <stdlib.h>
#include <stdio.h>
#include "algorithm.h"
#include "cbmp.h"


// Function to convert to gray-scale

void gray_scale(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS])
{
    
    for (int x = 0; x < BMP_WIDTH; x++){
      for (int y = 0; y < BMP_HEIGTH; y++){
        for (int c = 0; c < BMP_CHANNELS; c++){
          int colour = (input_image[x][y][0] + input_image[x][y][1] + input_image[x][y][2])/3;
          output_image[x][y][c] = colour;
        }
      }
    }
    
}



void binary_threshold(unsigned char grayscale_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
    for (int x = 0; x < BMP_WIDTH; x++){
      for (int y = 0; y < BMP_HEIGTH; y++){
        for (int c = 0; c < BMP_CHANNELS; c++){
          if (grayscale_image[x][y][1] > threshold){
            output_image[x][y][c] = 255;
            }
          else{
            output_image[x][y][c] = 0;

            }
          }
        }
      }
  }

void erosion(unsigned char binary_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int *countPtr){
  *countPtr = 0;
  erode_edges(output_image);
  
  for (int x = 1; x < BMP_WIDTH-1; x++){
    for (int y = 1; y < BMP_HEIGTH-1; y++){

      int top = binary_image[x][y-1][0];
      int low = binary_image[x][y+1][0];
      int right = binary_image[x+1][y][0];
      int left = binary_image[x-1][y][0];
      
      
      for (int c = 0; c < BMP_CHANNELS; c++){
        // pixels corresponding to a ’1’ in the structuring element are white
        if (binary_image[x][y][0] == 255 && top == 255 && low == 255 && right == 255 && left == 255){
          output_image[x][y][c] = 255;
          *countPtr += 1;
          //printf("%d", *countPtr);
        }
        else{
          output_image[x][y][c] = 0;
        }
      }
    }
  }
}

void erosion_strict(unsigned char binary_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int *countPtr){
  *countPtr = 0;
  erode_edges(output_image);

  for (int x = 1; x <= BMP_WIDTH-1; x++){
    for (int y = 1; y <= BMP_HEIGTH-1; y++){
      for (int c = 0; c < BMP_CHANNELS; c++){
        int top = binary_image[x][y-1][0];
        int low = binary_image[x][y+1][0];
        int right = binary_image[x+1][y][0];
        int left = binary_image[x-1][y][0];
        int topleft = binary_image[x-1][y+1][0];
        int topright = binary_image[x+1][y+1][0];
        int lowright = binary_image[x+1][y-1][0];
        int lowleft = binary_image[x-1][y-1][0];
        // pixels corresponding to a ’1’ in the structuring element are white
        if(binary_image[x][y][0] == 255 && top == 255 && low == 255 && right == 255 && left == 255 && topleft == 255 && topright == 255 && lowleft == 255 && lowright == 255){
          output_image[x][y][c] = 255;
          *countPtr += 1;
        }
        else{
          output_image[x][y][c] = 0;
        }
      }
    }
  }
}

void dilation(unsigned char binary_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){

  for (int x = 1; x < BMP_WIDTH-1; x++){
    for (int y = 1; y < BMP_HEIGTH-1; y++){
      for (int c = 0; c < BMP_CHANNELS; c++){
        if (binary_image[x][y][0] == 255){
          output_image[x][y][c] = 255;
          output_image[x][y-1][c] = 255;
          output_image[x][y+1][c] = 255;
          output_image[x-1][y][c] = 255;
          output_image[x+1][y][c] = 255;
          output_image[x+1][y-1][c] = 255;
          output_image[x+1][y+1][c] = 255;
          output_image[x-1][y-1][c] = 255;
          output_image[x-1][y+1][c] = 255;
        }
        else{
          output_image[x][y][c] = 0;
        }
      }
    }
  }
}

void copy_image(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for(int x = 0; x < BMP_WIDTH; x++){
    for(int y = 0; y < BMP_HEIGTH; y++){
      for(int c = 0; c < BMP_CHANNELS; c++)
        output_image[x][y][c] = input_image[x][y][c];

    }
  }
}

// Need to also capture the cells that are not included by the for-loops, close to the edges

void capture(unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int *capturePtr){
  for(int x = 6; x <= BMP_WIDTH-6; x++){
    for(int y = 6; y <= BMP_HEIGTH-6; y++){
      int countCaptureWindow = 0;
      int countExclusionWindow = 0;    
        
      
      for(int i = x-5; i <= x+5; i++){
        for(int j = y-5; j <= y+5; j++){
          // Check if there are white pixels in the capturing window
          if(input_image[i][j][0] == 255){
            countCaptureWindow += 1;
          }
        }
      }

      // Check if there are white pixels in the exclusionWindow
      if(countCaptureWindow > 0){
        for(int i = x-6; i <= x+6; i++){
          if(input_image[i][y+6][0] == 255){
            countExclusionWindow += 1;
          }
          if(input_image[i][y-6][0] == 255){
            countExclusionWindow += 1;
          }
        }
        for(int j = y-6; j <= y+6; j++){
          if(input_image[x+6][j][0] == 255){
            countExclusionWindow += 1;
          }
          if(input_image[x-6][j][0] == 255){
            countExclusionWindow += 1;
          }
        }
      }
      // if there are no white pixels in exclusion window, but there are in the capture window
      if(countExclusionWindow == 0 && countCaptureWindow != 0){

        for(int i = x-5; i <= x+5; i++){
          for(int j = y-5; j <= y+5; j++){
            for(int c = 0; c < BMP_CHANNELS; c++){
              input_image[i][j][c] = 0;
              
            }
          }
        }
        *capturePtr += 1;
        colorCells(output_image, x, y);
        printf("x: ");
        printf("%d", x);
        printf(",y: ");
        printf("%d", y);
        printf("\n");
      }
    }
  }
}

void colorCells(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS], int x, int y){
  for(int i = x-6; i <= x+6; i++){
    for(int j = y-1; j <= y+1; j++){
      output_image[i][j][0] = 255;
      output_image[i][j][1] = 0;
      output_image[i][j][2] = 0;
    }
  }
  for(int i = x-1; i <= x+1; i++){
    for(int j = y-6; j <= y+6; j++){
      output_image[i][j][0] = 255;
      output_image[i][j][1] = 0;
      output_image[i][j][2] = 0;
    }
  }
}

void erode_edges(unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS]){
  for(int x = 0; x <= 1; x++){
    for(int y = 0; y < BMP_HEIGTH; y++){
      for (int c = 0; c < BMP_CHANNELS; c++){
        output_image[x][y][c] = 0;
      }
    }
  }
  for(int x = 948; x <= 949; x++){
    for(int y = 0; y < BMP_HEIGTH; y++){
      for (int c = 0; c < BMP_CHANNELS; c++){
        output_image[x][y][c] = 0;
      }
    }
  }
  for(int x = 0; x < BMP_WIDTH; x++){
    for(int y = 0; y <= 1; y++){
      for (int c = 0; c < BMP_CHANNELS; c++){
        output_image[x][y][c] = 0;
      }
    }
  }
  for(int x = 0; x < BMP_WIDTH; x++){
    for(int y = 948; y <= 949; y++){
      for (int c = 0; c < BMP_CHANNELS; c++){
        output_image[x][y][c] = 0;
      }
    }
  }
}

