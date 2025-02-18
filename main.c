//To compile (linux/mac): gcc cbmp.c main.c -o main.out -std=c99
//To run (linux/mac): ./main.out example.bmp example_inv.bmp

//To compile (win): gcc cbmp.c main.c -o main.exe -std=c99
//To run (win): main.exe example.bmp example_inv.bmp


// Comments: duplicated detection - probably because of threshold
// decreased thres -> more iterations 

#include <stdlib.h>
#include <stdio.h>
#include "cbmp.h"
#include "algorithm.h"
#include <time.h>



//Declaring the array to store the image (unsigned char = unsigned 8 bit)
unsigned char input_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char output_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];
unsigned char final_image[BMP_WIDTH][BMP_HEIGTH][BMP_CHANNELS];



int count = 1;
int *countPtr = &count;

int captured = 0;
int *capturePtr = &captured;

clock_t start, end;
double cpu_time_used;

//Main function
int main(int argc, char** argv)
{
  //argc counts how may arguments are passed
  //argv[0] is a string with the name of the program
  //argv[1] is the first command line argument (input image)
  //argv[2] is the second command line argument (output image)

  //Checking that 2 arguments are passed
  if (argc != 3)
  {
      fprintf(stderr, "Usage: %s <output file path> <output file path>\n", argv[0]);
      exit(1);
  }

  printf("Program to detect cells\n");

  start = clock();
  //Load image from file
  read_bitmap(argv[1], input_image);

  //Run gray_scale
  gray_scale(input_image, output_image);
  copy_image(output_image, final_image);

  //Run binary threshold
  binary_threshold(final_image, output_image);
  copy_image(output_image, input_image);
  int i = 0;

  /// First a few iterations of erosion_strict()
  for(int i = 0; i < 6; i++){
    erosion_strict(input_image, output_image, countPtr);
    copy_image(output_image, input_image);
    capture(input_image, final_image, capturePtr);
    i += 1;
  }

  /// Then continue with erosion() until there are no more cells to erode
  while (*countPtr > 0){
    erosion(input_image, output_image, countPtr);
    copy_image(output_image, input_image);
    capture(input_image, final_image, capturePtr);
    i += 1;
  }
  end = clock();
  

  printf("Total captured cells: ");
  printf("%d", *capturePtr);
  printf("\nNumber of iterations: ");
  printf("%d", i);
  printf("\n");


  
  //Save image to file
  write_bitmap(final_image, argv[2]);

  
  cpu_time_used = end - start;
  printf("Total time: %f ms\n", cpu_time_used * 1000.0 / CLOCKS_PER_SEC);

  printf("Done!\n");
  return 0;
}
