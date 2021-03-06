/*
 * Author: Alex Adams
 * Many methods taken from Brooke Chenoweth,
 * see https://www.cs.unm.edu/~bchenoweth/cs241
 */


#include <stdio.h>
#include <stdlib.h>

int getIntFromArray(unsigned char bytes[])
{
  int n =
    bytes[0] |
    bytes[1] << 8 |
    bytes[2] << 16 |
    bytes[3] << 24;
  return n;
}

int main(int argc, char *argv[])
{
  int pixelWidth;
  int pixelHeight;
  int i, j;
  FILE* in;
  unsigned char header[54];
  char* infilename;

  if (argc != 2)
  {
    printf("USAGE:\ndesteg {input file}\n");
    exit(1);
  }

  infilename = argv[1];

  in = fopen(infilename, "rb");

  /* read header into array */
  fread(header, 1, 54, in);

  /* is this really a bitmap? */
  if(header[0] != 'B' || header[1] != 'M')
  {
    printf("Input file is not a bitmap\n");
    exit(1);
  }

  /* Is the header size what we expect? */
  if(getIntFromArray(&header[10]) != 54)
  {
    printf("Unexpected header size\n");
    exit(1);
  }

  /* How many bits per pixel? Expecting 24*/
  if(!(header[28] == 24 && header[29] == 0))
  {
    printf("Unexpected number of bits/pixel\n");
    exit(1);
  }

  pixelWidth = getIntFromArray(&header[18]);
  pixelHeight = getIntFromArray(&header[22]);


  /* Read RGB data from original, copy with message*/
  for(i = 0; i < pixelHeight; ++i)
  {
    for(j = 0; j < pixelWidth; j++)
    {
      unsigned char bytes[4];
      unsigned int message;
      int k;

      /* color order is BGR */
      fread(bytes, 1, 4, in);

      /* set last bytes to message */
      for (k=0; k<4; k++)
      {
        bytes[k]<<=6;
        bytes[k]>>=(k*2);
      }
      message=bytes[0]|bytes[1]|bytes[2]|bytes[3];
      if(message == '\0') exit(0);
      else printf("%c", message);
    }
  }

  fclose(in);
  return 0;
}