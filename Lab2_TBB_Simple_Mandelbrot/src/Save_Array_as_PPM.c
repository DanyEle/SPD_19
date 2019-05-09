#include <stdlib.h>
#include <stdio.h>

/** C routine to save a 2d int array as an image to a simple graphic format **/
/** edited form of code from the rosetta code project **/
/** https://rosettacode.org/wiki/Bitmap/Write_a_PPM_file **/


int saveimg(int dimx, int dimy, const char * filename, int * matrix, int max_value)
{
  // filename must end with  .ppm
  // no parameter checking!!! you are on your own

  // const int dimx = 800, dimy = 800;
  int i, j;
  FILE *fp = fopen(filename, "wb"); /* b - binary mode */
  (void) fprintf(fp, "P6\n%d %d\n255\n", dimx, dimy);
  for (j = 0; j < dimy; ++j)
  {
    for (i = 0; i < dimx; ++i)
    {
      // here you may use any scaling function you prefer
      int val = matrix[i +j*dimy];
      if (max_value!=255)
	{ val = (val *255) / max_value; // scale to 0 - 255
	}
      //
      // rgb values are still rounded in case of mistakes
      static unsigned char color[3];
      color[0] = (val >>5) % 256;  /* red */
      color[1] = (val >>3) % 256;  /* green */
      color[2] = (val ) % 256;  /* blue */
      (void) fwrite(color, 1, 3, fp);
    }
  }
  (void) fclose(fp);
  return 0;
}
