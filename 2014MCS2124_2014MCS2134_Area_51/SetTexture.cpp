#ifndef _Header_H
#define _Header_H
#include "header.h"
#endif

#include <iostream>


/* storage for one texture  */
int texture[50];

int Backgroundtexture[4];

/* Image type - contains height, width, and data */
struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};
typedef struct Image Image;

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only. 
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int ImageLoad(char *filename, Image *image) {
    FILE *file=NULL;
    unsigned long size=0;                 // size of the image in bytes.
    unsigned long i=0;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1)
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "r"))==NULL)
    {
    printf("File Not Found : %s\n",filename);
    return 0;
    }
   
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
    printf("Error reading width from %s.\n", filename);
    return 0;
    }
    printf("Width of %s: %lu\n", filename, image->sizeX);
   
    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
    printf("Error reading height from %s.\n", filename);
    return 0;
    }
    printf("Height of %s: %lu\n", filename, image->sizeY);
   
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) {
    printf("Error reading planes from %s.\n", filename);
    return 0;
    }
    if (planes != 1) {
    printf("Planes from %s is not 1: %u\n", filename, planes);
    return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) {
    printf("Error reading bpp from %s.\n", filename);
    return 0;
    }
    if (bpp != 24) {
    printf("Bpp from %s is not 24: %u\n", filename, bpp);
    return 0;
    }
   
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
    printf("Error allocating memory for color-corrected image data\n");
    return 0;  
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
    printf("Error reading image data from %s.\n", filename);
    return 0;
    }

    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
    temp = image->data[i];
    image->data[i] = image->data[i+2];
    image->data[i+2] = temp;
    }
   
   fclose(file);
    // we're done.
    return 1;
}

void LoadImage(char *filename,int i) {
   // Load Texture
    Image *image1;
    
    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
    printf("Error allocating space for image");
    exit(0);
    }

    if (!ImageLoad(filename, image1)) {
    exit(1);
    }        

    // Create Texture   
    glGenTextures(1, &texture[i]);
    glBindTexture(GL_TEXTURE_2D, texture[i]);   // 2d texture (x and y size)

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture

    // 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image,
    // border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    free(image1);
}

// Load Bitmaps And Convert To Textures
void LoadGLTextures()
{
    LoadImage("playGround.bmp",0);
    /*LoadImage("p1/p1_1.bmp",1);
    LoadImage("p1/p1_2.bmp",2);
    LoadImage("p1/p1_3.bmp",3);*/
    LoadImage("StartPage1.bmp",23);
    LoadImage("select_hero_and_select_screen.bmp",24);
    LoadImage("hide.bmp",26);
    LoadImage("Win.bmp",41); //Win image
    LoadImage("Lose.bmp",42); //Lose image
}

/************************************** WORKING WITH PNG FILES  ******************************************/

void LoadGLTexturesWithPNGImages(char *filename,int i) {
       // Load file and decode image.
      std::vector<unsigned char> image;
      unsigned width, height;
      unsigned error = lodepng::decode(image, width, height, filename);

      if(i==0)
      {
        std::cout<<"Start Page width:"<<width<<" Height:"<<height<<std::endl; 
      }

      // If there's an error, display it.
      if(error != 0)
      {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1;
      }

        // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
      size_t u2 = 1; while(u2 < width) u2 *= 2;
      size_t v2 = 1; while(v2 < height) v2 *= 2;
      // Ratio for power of two version compared to actual version, to render the non power of two image with proper size.
      double u3 = (double)width / u2;
      double v3 = (double)height / v2;

      // Make power of two version of the image.
      std::vector<unsigned char> image2(u2 * v2 * 4);
      for(size_t y = 0; y < height; y++)
      for(size_t x = 0; x < width; x++)
      for(size_t c = 0; c < 4; c++)
      {
        image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
      }
    // Create Texture   
    glGenTextures(1, &texture[i]);
    glBindTexture(GL_TEXTURE_2D, texture[i]);   // 2d texture (x and y size)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 4, u2, v2, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image2[0]);
}


void funLoadGLTextures(char *filename,int tex[],int i)
{
    LoadGLTexturesWithPNGImages(filename,25);
    tex[i]=texture[25];
}


void LoadGLTexturesWithPNGImages()
{
    //LoadGLTexturesWithPNGImages("StartPage1.png",0);
    //LoadGLTexturesWithPNGImages("arena.png",1);
    //LoadGLTexturesWithPNGImages("arena.png",2);
    //LoadGLTexturesWithPNGImages("arena.png",3);
    /*LoadGLTexturesWithPNGImages("stone1.png",4);

    LoadGLTexturesWithPNGImages("fire_1/fire_1.png",5);
    LoadGLTexturesWithPNGImages("fire_1/fire_2.png",6);
    LoadGLTexturesWithPNGImages("fire_1/fire_3.png",7);
    LoadGLTexturesWithPNGImages("fire_1/fire_4.png",8);
    LoadGLTexturesWithPNGImages("fire_1/fire_5.png",9);
    LoadGLTexturesWithPNGImages("fire_1/fire_6.png",10);
    LoadGLTexturesWithPNGImages("fire_1/fire_7.png",11);*/


    LoadGLTexturesWithPNGImages("fire_2/fire_1.png",12);
    LoadGLTexturesWithPNGImages("fire_2/fire_2.png",13);
    LoadGLTexturesWithPNGImages("fire_2/fire_3.png",14);
    LoadGLTexturesWithPNGImages("fire_2/fire_4.png",15);
    LoadGLTexturesWithPNGImages("fire_2/fire_5.png",16);
    LoadGLTexturesWithPNGImages("fire_2/fire_6.png",17);
    LoadGLTexturesWithPNGImages("fire_2/fire_7.png",18);

/*    LoadGLTexturesWithPNGImages("Castle1.png",19);
    LoadGLTexturesWithPNGImages("Castle4.png",20);*/

    /*LoadGLTexturesWithPNGImages("obstacle_1.png",21);       //pond*/

   /* LoadGLTexturesWithPNGImages("arena.png",22); // arena*/
    LoadGLTexturesWithPNGImages("white1.png",27); // arena
    LoadGLTexturesWithPNGImages("green1.png",28); // arena
    LoadGLTexturesWithPNGImages("red1.png",29); // arena
    LoadGLTexturesWithPNGImages("orange1.png",37);

    LoadGLTexturesWithPNGImages("me.png",30);
    LoadGLTexturesWithPNGImages("teammate.png",31);
    LoadGLTexturesWithPNGImages("mycastle.png",32);
    LoadGLTexturesWithPNGImages("opponent1.png",33);
    LoadGLTexturesWithPNGImages("opponent2.png",34);
    LoadGLTexturesWithPNGImages("opponent_castle1.png",35);


    // 36 -> Area 51
    LoadGLTexturesWithPNGImages("title.png",36);
    // 37 -> team name 1
    LoadGLTexturesWithPNGImages("virus.png",38);
    // 38 -> team name 2
    LoadGLTexturesWithPNGImages("antivirus.png",39);

    LoadGLTexturesWithPNGImages("Attack.png",40);  // attack image
}