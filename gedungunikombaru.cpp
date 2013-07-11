#include <windows.h> //for Code::Blocks user
#include <GL/glut.h>
#include <stdio.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdlib.h>

GLuint texture[20];

struct Image {
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};

typedef struct Image Image;

#define checkImageWidth 100
#define checkImageHeight 100

GLubyte checkImage[checkImageWidth][checkImageHeight][3];

char title[] = "Gedung Unikom Baru";  // penamaan judul window
int windowWidth  = 1024;     // mode lebar layar normal
int windowHeight = 768;     // mode tinggi layar normal
int windowPosX   = 50;      // posisi tampilan layar pojok k1ri atas titik x
int windowPosY   = 50;      // posisi tampilan layar pojok k1ri atas titik y
bool fullScreenMode = false; // Full-screen or windowed mode?

float rotx , roty , tx=0 , ty, tz=0,y ,zz,p;
int i;

/*----------------------------------------------------------------------------*/

// Prosedure interaksi pada keyboard
void myKeyboard(unsigned char key, int x, int y)
{
 
    switch (key) {
      case 27:    // ESC: exit atau keluar dari program
         exit(0); 
         break;
      case 'a':   // geser kekiri
         tx+=0.1;
         break;
      case 'd' :   // geser kekanan
         tx-=0.1;
         break;
     case 'w':   // maju 
         tz+=0.1;
         break;
     case 's':   // mundur 
         tz-=0.1;
         break;
      case 'y':   // memutarkan kekiri
         roty+=1;
         break;
      case 't':   // memutar kekanan
         roty-=1;
         break;
      case 'q':   // memutar kekanan
         p-=0.1;
         break;
      case 'e':   // memutar kekanan
         p+=0.1;
         break;
      default:
         break;
   }
}
// cek gambar
void makeCheckImage(void){
    int i, j, c;
    for (i = 0; i < checkImageWidth; i++) {
        for (j = 0; j < checkImageHeight; j++) {
            c = ((((i&0x8)==0)^((j&0x8)==0)))*255;
            checkImage[i][j][0] = (GLubyte) c;
            checkImage[i][j][1] = (GLubyte) c;
            checkImage[i][j][2] = (GLubyte) c;
        }
    }
}
// load gambar
int ImageLoad(char *filename, Image *image) {
    FILE *file;
    unsigned long size; // size of the image in bytes.
    unsigned long i; // standard counter.
    unsigned short int planes; // number of planes in image (must be 1)
    unsigned short int bpp; // number of bits per pixel (must be 24)
    char temp; // temporary color storage for bgr-rgb conversion.

    // make sure the file is there.
    if ((file = fopen(filename, "rb"))==NULL){
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
    //printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
        printf("Error reading height from %s.\n", filename);
        return 0;
    }

    //printf("Height of %s: %lu\n", filename, image->sizeY);
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

    // read the bitsperpixel
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
        printf("Error allocating memory for color-corrected image data");
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
    // we're done.
    return 1;
}
//-----------------------lokasi ruang untuk teksture----------------------------
//rumput
Image * loadTexture1(){
    Image *image1;

    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));

    if (image1 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/rumput.bmp", image1)) {
        exit(1);
    }
    return image1;
}

//tembok
Image * loadTexture2(){
    Image *image2;

    // allocate space for texture
    image2 = (Image *) malloc(sizeof(Image));

    if (image2 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/dalam.bmp", image2)) {
        exit(1);
    }
    return image2;
}
//tembok1
Image * loadTexture3(){
    Image *image3;

    // allocate space for texture
    image3 = (Image *) malloc(sizeof(Image));

    if (image3 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/samping.bmp", image3)) {
        exit(1);
    }
    return image3;
}
//tembok2
Image * loadTexture4(){
    Image *image4;

    // allocate space for texture
    image4 = (Image *) malloc(sizeof(Image));

    if (image4 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/sebelah.bmp", image4)) {
        exit(1);
    }
    return image4;
}
//tembok3
Image * loadTexture5(){
    Image *image5;

    // allocate space for texture
    image5 = (Image *) malloc(sizeof(Image));

    if (image5 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/depan.bmp", image5)) {
        exit(1);
    }
    return image5;
}
//tembok4
Image * loadTexture6(){
    Image *image6;

    // allocate space for texture
    image6 = (Image *) malloc(sizeof(Image));

    if (image6 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/sebelahtinggi.bmp", image6)) {
        exit(1);
    }
    return image6;
}
//tembok5
Image * loadTexture7(){
    Image *image7;

    // allocate space for texture
    image7 = (Image *) malloc(sizeof(Image));

    if (image7 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/belakang.bmp", image7)) {
        exit(1);
    }
    return image7;
}
//tembok6
Image * loadTexture8(){
    Image *image8;

    // allocate space for texture
    image8 = (Image *) malloc(sizeof(Image));

    if (image8 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/depanatas.bmp", image8)) {
        exit(1);
    }
    return image8;
}
//daun pohon
Image * loadTexture9(){
    Image *image9;

    // allocate space for texture
    image9 = (Image *) malloc(sizeof(Image));

    if (image9 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/daun2.bmp", image9)) {
        exit(1);
    }
    return image9;
}
//batang pohon
Image * loadTexture10(){
    Image *image10;

    // allocate space for texture
    image10 = (Image *) malloc(sizeof(Image));

    if (image10 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/batang.bmp", image10)) {
        exit(1);
    }
    return image10;
}
//background
Image * loadTexture11(){
    Image *image11;

    // allocate space for texture
    image11 = (Image *) malloc(sizeof(Image));

    if (image11 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/pemandangan.bmp", image11)) {
        exit(1);
    }
    return image11;
}
//parkiran
Image * loadTexture12(){
    Image *image12;

    // allocate space for texture
    image12 = (Image *) malloc(sizeof(Image));

    if (image12 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/parkiran.bmp", image12)) {
        exit(1);
    }
    return image12;
}
//Trotoar
Image * loadTexture13(){
    Image *image13;

    // allocate space for texture
    image13 = (Image *) malloc(sizeof(Image));

    if (image13 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/trotoar.bmp", image13)) {
        exit(1);
    }
    return image13;
}
//jalan 
Image * loadTexture14(){
    Image *image14;

    // allocate space for texture
    image14 = (Image *) malloc(sizeof(Image));

    if (image14 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/jalan.bmp", image14)) {
        exit(1);
    }
    return image14;
}
//dalam 
Image * loadTexture15(){
    Image *image15;

    // allocate space for texture
    image15 = (Image *) malloc(sizeof(Image));

    if (image15 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/dalamtinggi.bmp", image15)) {
        exit(1);
    }
    return image15;
}
//belakangsamping
Image * loadTexture16(){
    Image *image16;

    // allocate space for texture
    image16 = (Image *) malloc(sizeof(Image));

    if (image16 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/dalamtinggi.bmp", image16)) {
        exit(1);
    }
    return image16;
}
//atas samping
Image * loadTexture17(){
    Image *image17;

    // allocate space for texture
    image17 = (Image *) malloc(sizeof(Image));

    if (image17 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/atassamping.bmp", image17)) {
        exit(1);
    }
    return image17;
}
//atassebelah
Image * loadTexture18(){
    Image *image18;

    // allocate space for texture
    image18 = (Image *) malloc(sizeof(Image));

    if (image18 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/atassebelah.bmp", image18)) {
        exit(1);
    }
    return image18;
}
//atasdalam
Image * loadTexture19(){
    Image *image19;

    // allocate space for texture
    image19 = (Image *) malloc(sizeof(Image));

    if (image19 == NULL) {
        printf("Error allocating space for image");
        exit(0);
    }
    if (!ImageLoad("gambar/dalamatas.bmp", image19)) {
        exit(1);
    }
    return image19;
}

/*----------------------------------------------------------------------------*/

void init(void)
{ int n;
 	 glClearColor(0.0, 0.0, 0.0, 0.0);
	 glEnable(GL_DEPTH_TEST);
	 glDepthFunc(GL_LESS);

    Image *image1 = loadTexture1();
    Image *image2 = loadTexture2();
    Image *image3 = loadTexture3();
    Image *image4 = loadTexture4();
    Image *image5 = loadTexture5();
    Image *image6 = loadTexture6();
    Image *image7 = loadTexture7();
    Image *image8 = loadTexture8();
    Image *image9 = loadTexture9();
    Image *image10 = loadTexture10();
    Image *image11 = loadTexture11();
    Image *image12 = loadTexture12();
    Image *image13 = loadTexture13();
    Image *image14 = loadTexture14();
    Image *image15 = loadTexture15();
    Image *image16 = loadTexture16();
    Image *image17 = loadTexture17();
    Image *image18 = loadTexture18();
    Image *image19 = loadTexture19();
    

if(image1 == NULL||image2 == NULL||image3 == NULL||image4 == NULL||
    image5 == NULL||image6 == NULL||image7 == NULL||image8 == NULL||
    image9 == NULL||image10 == NULL||image11 == NULL||image12 == NULL||
    image13 == NULL||image14 == NULL||image15 == NULL||image16 == NULL||
    image17 == NULL||image18 == NULL||image19 == NULL) {
        printf("Image was not returned from loadTexture\n");
        exit(0);
    }

    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    
    // Create Texture dalamatas
    glGenTextures(19, texture);
    glBindTexture(GL_TEXTURE_2D, texture[19]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image19->sizeX, image19->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image19->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture Atassebelah
    glGenTextures(18, texture);
    glBindTexture(GL_TEXTURE_2D, texture[18]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image18->sizeX, image18->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image18->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture Atassamping
    glGenTextures(17, texture);
    glBindTexture(GL_TEXTURE_2D, texture[17]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image17->sizeX, image17->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image17->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture Dalamtinggi
    glGenTextures(16, texture);
    glBindTexture(GL_TEXTURE_2D, texture[16]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image16->sizeX, image16->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image16->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture dalamtinggi
    glGenTextures(15, texture);
    glBindTexture(GL_TEXTURE_2D, texture[15]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image15->sizeX, image15->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image15->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture jalan
    glGenTextures(14, texture);
    glBindTexture(GL_TEXTURE_2D, texture[14]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image14->sizeX, image14->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image14->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture trotoar
    glGenTextures(13, texture);
    glBindTexture(GL_TEXTURE_2D, texture[13]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image13->sizeX, image14->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image13->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture parkiran
    glGenTextures(12, texture);
    glBindTexture(GL_TEXTURE_2D, texture[12]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image12->sizeX, image12->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image12->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);    
    // Create Texture background
    glGenTextures(11, texture);
    glBindTexture(GL_TEXTURE_2D, texture[11]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image11->sizeX, image11->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image11->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture batang pohon
    glGenTextures(10, texture);
    glBindTexture(GL_TEXTURE_2D, texture[10]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image10->sizeX, image10->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image10->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
   // Create Texture daun pohon
    glGenTextures(9, texture);
    glBindTexture(GL_TEXTURE_2D, texture[9]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image9->sizeX, image9->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image9->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture tembok
    glGenTextures(8, texture);
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image8->sizeX, image8->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image8->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture tembok
    glGenTextures(7, texture);
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image7->sizeX, image7->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image7->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture tembok
    glGenTextures(6, texture);
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image6->sizeX, image6->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image6->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture tembok
    glGenTextures(6, texture);
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image5->sizeX, image5->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image5->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture //tembok 2
    glGenTextures(5, texture);
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image4->sizeX, image4->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image4->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
     // Create Textur //Tembok 1
    glGenTextures(4, texture);
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image3->sizeX, image3->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image3->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    // Create Texture tembok
    glGenTextures(3, texture);
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image2->sizeX, image2->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image2->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
         // Create Texture rumput
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); //scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); //scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0,
    GL_RGB, GL_UNSIGNED_BYTE, image1->data);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, checkImageWidth,
    checkImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,&checkImage[0][0][0]);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_FLAT);
}
/*----------------------------------------------------------------------------*/
void rumput(){
     glBindTexture(GL_TEXTURE_2D, texture[1]);
     glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);glVertex3f(-3, -0.5, -3);
		glTexCoord2f(0.5, 0.0);glVertex3f(-3, -0.5, 1.02);
		glTexCoord2f(0.5, 0.5);glVertex3f(3, -0.5, 1.02);
		glTexCoord2f(0.0, 0.5);glVertex3f(3, -0.5, -3);		
	glEnd();
}
//----------------------Procedure dinding gedung--------------------------------
void dinding_gedung()
{    
     
     //tembok bagian depan
     //tembok depan kiri1
     glBindTexture(GL_TEXTURE_2D, texture[3]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-1.0,0.4,0.5);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-1.0,-0.5,0.5); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,-0.5,0.5); //titik C
     glEnd();
     
     //tembok depan kiri1 dalam1
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,0.4,0.48);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.8,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.8,-0.5,0.5); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,-0.5,0.48); //titik F
     glEnd();
     
     //tembok depan kiri1 dalam depan
     glBindTexture(GL_TEXTURE_2D, texture[2]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,0.38,0.48);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.8,0.38,0.48);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.8,-0.5,0.48); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,-0.5,0.48); //titik C
     glEnd();
      
       //tembok depan kiri1 dalam2
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,0.4,0.5);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,0.4,0.48);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,-0.5,0.48); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,-0.5,0.5); //titik F
     glEnd();
     
     //tembok depan kiri2
     glBindTexture(GL_TEXTURE_2D, texture[4]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.4,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,0.4,0.5);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,-0.5,0.5); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.4,-0.5,0.5); //titik C
     glEnd();
     
     //tembok depan kiri2 dalam
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,0.38,0.5);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,0.38,0.5);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,-0.5,0.5); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,-0.5,0.5); //titik C
     glEnd();
     
     //tembok depan kiri2 luar
     glBindTexture(GL_TEXTURE_2D, texture[5]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,0.4,0.5);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,0.4,0.54);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,-0.5,0.54); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,-0.5,0.5); //titik F
     glEnd();
       
      //tembok depan tengah1
     glBindTexture(GL_TEXTURE_2D, texture[5]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.35,0.4,0.54);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,0.4,0.54);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,-0.5,0.54); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,-0.5,0.54); //titik C
     glEnd();
     
     //tembok depan kanan2 luar
     glBindTexture(GL_TEXTURE_2D, texture[5]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.35,0.4,0.5);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.35,0.4,0.54);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,-0.5,0.54); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,-0.5,0.5); //titik F
     glEnd();
     
     //tembok depan kanan2 dalam
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,0.38,0.5);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(0.35,0.38,0.5);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,-0.5,0.5); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.4,-0.5,0.5); //titik C
     glEnd();
     
     //tembok depan kanan2
     glBindTexture(GL_TEXTURE_2D, texture[4]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,0.4,0.5);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,-0.5,0.5); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.4,-0.5,0.5); //titik C
     glEnd();
     
     //tembok depan kanan1 dalam2
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,0.4,0.5);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,0.4,0.48);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,-0.5,0.48); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,-0.5,0.5); //titik F
     glEnd();
     
     //tembok depan kanan1 dalam depan
     glBindTexture(GL_TEXTURE_2D, texture[2]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,0.38,0.48);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(0.8,0.38,0.48);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(0.8,-0.5,0.48); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,-0.5,0.48); //titik C
     glEnd();
     
     //tembok depan kanan1 dalam1
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,0.4,0.48);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.8,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.8,-0.5,0.5); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,-0.5,0.48); //titik F
     glEnd();
     
     //tembok depan kanan1
     glBindTexture(GL_TEXTURE_2D, texture[3]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(1.0,0.4,0.5);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(1.0,-0.5,0.5); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,-0.5,0.5); //titik C
     glEnd();
     
//------------------------------------------------------------------------------     
     //tembok bagian samping
     //samping kanan
     //samping kanan1
     glBindTexture(GL_TEXTURE_2D, texture[3]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(1.0,0.4,0.3);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(1.0,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(1.0,-0.5,0.5); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(1.0,-0.5,0.3); //titik F
     glEnd();
     
     //samping kanan1 dalam
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(1.0,0.4,0.3);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.95,0.4,0.3);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.95,-0.5,0.3); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(1.0,-0.5,0.3); //titik F
     glEnd();
     
     //samping kanan1 depan
     glBindTexture(GL_TEXTURE_2D, texture[4]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.95,0.4,-0.1);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.95,0.4,0.3);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.95,-0.5,0.3); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.95,-0.5,-0.1); //titik F
     glEnd();
     
     //samping kanan2 tinggi
     glBindTexture(GL_TEXTURE_2D, texture[6]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.95,1.0,-0.7);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.95,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.95,-0.5,-0.1); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.95,-0.5,-0.7); //titik F
     glEnd();
     
     //samping kiri
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-1.0,0.4,0.3);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-1.0,0.4,0.5);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-1.0,-0.5,0.5); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-1.0,-0.5,0.3); //titik F
     glEnd();
     
     //samping kiri1 dalam
     glBindTexture(GL_TEXTURE_2D, texture[2]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-1.0,0.4,0.3);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.95,0.4,0.3);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.95,-0.5,0.3); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-1.0,-0.5,0.3); //titik F
     glEnd();
     
     //samping kiri1 depan
     glBindTexture(GL_TEXTURE_2D, texture[4]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.95,0.4,-0.1);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.95,0.4,0.3);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.95,-0.5,0.3); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.95,-0.5,-0.1); //titik F
     glEnd();
     
     //samping kiri2 tinggi
     glBindTexture(GL_TEXTURE_2D, texture[6]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.95,1.0,-0.7);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.95,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.95,-0.5,-0.1); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.95,-0.5,-0.7); //titik F
     glEnd();
//-----------------------------------------------------------------------------     
     //bagian belakang
     //belakang kiri1 depan
     glBindTexture(GL_TEXTURE_2D, texture[6]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.95,1.0,-0.7);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.7,1.0,-0.7);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.7,-0.5,-0.7); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.95,-0.5,-0.7); //titik F
     glEnd();
     
     //belakang kiri1 dalam
     glBindTexture(GL_TEXTURE_2D, texture[16]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.7,1.0,-0.7);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.7,1.0,-0.9);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.7,-0.5,-0.9); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.7,-0.5,-0.7); //titik F
     glEnd();
     
     //belakang kiri2 depan
     glBindTexture(GL_TEXTURE_2D, texture[6]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.7,1.0,-0.9);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.45,1.0,-0.9);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.45,-0.5,-0.9); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.7,-0.5,-0.9); //titik F
     glEnd();
     
     //belakang kiri2 dalam
     glBindTexture(GL_TEXTURE_2D, texture[15]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.45,1.0,-0.9);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.45,1.0,-1.0);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.45,-0.5,-1.0); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.45,-0.5,-0.9); //titik F
     glEnd();
     
     //belakang kanan1 depan
     glBindTexture(GL_TEXTURE_2D, texture[6]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.95,1.0,-0.7);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.7,1.0,-0.7);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.7,-0.5,-0.7); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.95,-0.5,-0.7); //titik F
     glEnd();
     
     //belakang kanan1 dalam
     glBindTexture(GL_TEXTURE_2D, texture[16]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.7,1.0,-0.7);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.7,1.0,-0.9);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.7,-0.5,-0.9); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.7,-0.5,-0.7); //titik F
     glEnd();
     
     //belakang kanan2 depan
     glBindTexture(GL_TEXTURE_2D, texture[6]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.7,1.0,-0.9);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.45,1.0,-0.9);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.45,-0.5,-0.9); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.7,-0.5,-0.9); //titik F
     glEnd();
     
     //belakang kanan2 dalam
     glBindTexture(GL_TEXTURE_2D, texture[15]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.45,1.0,-0.9);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.45,1.0,-1.0);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.45,-0.5,-1.0); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.45,-0.5,-0.9); //titik F
     glEnd();
     
     //belakang tengah
     glBindTexture(GL_TEXTURE_2D, texture[7]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.45,1.0,-1.0);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.45,1.0,-1.0);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.45,-0.5,-1.0); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.45,-0.5,-1.0); //titik F
     glEnd();
//-----------------------------------------------------------------------------     
     //tembok bagian atas depan
     //tembok atas depan kiri1
     glBindTexture(GL_TEXTURE_2D, texture[18]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.95,1.0,-0.1);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.95,0.38,-0.1); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,0.38,-0.1); //titik C
     glEnd();
     
     //tembok atas depan kiri1 dalam1
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,1.0,-0.18);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.8,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.8,0.38,-0.1); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,0.38,-0.18); //titik F
     glEnd();
     
     //tembok atas depan kiri1 dalam depan
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,0.98,-0.18);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.8,0.98,-0.18);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.8,0.38,-0.18); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,0.38,-0.18); //titik C
     glEnd();
      
       //tembok atas depan kiri1 dalam2
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,1.0,-0.1);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,1.0,-0.18);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,0.38,-0.18); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,0.38,-0.1); //titik F
     glEnd();
     
     //tembok atas depan kiri2
     glBindTexture(GL_TEXTURE_2D, texture[17]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.4,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,1.0,-0.1);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,0.38,-0.1); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.4,0.38,-0.1); //titik C
     glEnd();
     
     //tembok atas depan kiri2 dalam
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,0.98,-0.1);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,0.98,-0.1);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,0.38,-0.1); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,0.38,-0.1); //titik C
     glEnd();
     
     //tembok atas depan kiri2 luar
     glBindTexture(GL_TEXTURE_2D, texture[8]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,1.0,-0.1);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,1.0,-0.04);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,0.38,-0.04); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,0.38,-0.1); //titik F
     glEnd();
       
      //tembok atas depan tengah1
     glBindTexture(GL_TEXTURE_2D, texture[8]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.35,1.0,-0.04);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,1.0,-0.04);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,0.38,-0.04); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,0.38,-0.04); //titik C
     glEnd();
     
     //tembok atas depan kanan2 luar
     glBindTexture(GL_TEXTURE_2D, texture[8]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.35,1.0,-0.1);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.35,1.0,-0.04);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,0.38,-0.04); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,0.38,-0.1); //titik F
     glEnd();
     
     //tembok atas depan kanan2 dalam
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,0.98,-0.1);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(0.35,0.98,-0.1);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,0.38,-0.1); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.4,0.38,-0.1); //titik C
     glEnd();
     
     //tembok atas depan kanan2
     glBindTexture(GL_TEXTURE_2D, texture[17]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,1.0,-0.1);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,0.38,-0.1); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.4,0.38,-0.1); //titik C
     glEnd();
     
     //tembok atas depan kanan1 dalam2
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,1.0,-0.1);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,1.0,-0.18);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,0.38,-0.18); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,0.38,-0.1); //titik F
     glEnd();
     
     //tembok atas depan kanan1 dalam depan
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,0.98,-0.18);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(0.8,0.98,-0.18);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(0.8,0.38,-0.18); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,0.38,-0.18); //titik C
     glEnd();
     
     //tembok atas depan kanan1 dalam1
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,1.0,-0.18);  //titik E
       glTexCoord2f(0.0, 1.0);glVertex3f(0.8,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 0.0);glVertex3f(0.8,0.38,-0.1); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,0.38,-0.18); //titik F
     glEnd();
     
     //tembok atas depan kanan1
     glBindTexture(GL_TEXTURE_2D, texture[18]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,1.0,-0.1);  //titik D
       glTexCoord2f(0.0, 1.0);glVertex3f(0.95,1.0,-0.1);  //titik A       
       glTexCoord2f(0.0, 0.0);glVertex3f(0.95,0.38,-0.1); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,0.38,-0.1); //titik C
     glEnd();
     
//--------------------------Procedure atap--------------------------------------
     //atapbawah
     
     //atapdepanbawahtengah     
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,0.4,-0.04);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,0.4,0.54);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,0.4,0.54); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,0.4,-0.04); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiri     
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.35,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.35,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanan     
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalam
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,0.38,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,0.38,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanandalam
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.4,0.38,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,0.38,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalamluar
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.4,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.4,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanandalamluar
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.4,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.4,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalamluarbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.4,0.4,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,0.4,-0.102);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,0.4,0.5); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanandalamluarbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,0.4,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,0.4,-0.102);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,0.4,0.5); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalamluarbangetbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanandalamluarbangetbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalamluarbangetsekali
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,0.38,-0.18);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,0.38,-0.18);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,0.38,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,0.38,0.5); //titik C
     glEnd();
          
     //atapdepanbawahtengahkanandalamluarbangetsekali
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,0.38,-0.18);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,0.38,-0.18);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,0.38,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,0.38,0.5); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalamluarbangetbangetluarbinasa
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.8,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.8,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanandalamluarbangetbangetluarbinasa
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.8,0.38,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,0.38,0.5);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.8,0.4,-0.102); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalamluarbangetwahkeren
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.95,0.4,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,0.4,-0.102);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.95,0.4,0.5); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanandalamluarbangetwahkeren
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.95,0.4,-0.102);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,0.4,-0.102);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.95,0.4,0.5); //titik C
     glEnd();
     
     //atapdepanbawahtengahkiridalamluarbangetwahkerenbreww
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(1.0,0.4,0.3);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.95,0.4,0.3);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.95,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(1.0,0.4,0.5); //titik C
     glEnd();
     
     //atapdepanbawahtengahkanandalamluarbangetwahkerenbreww
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-1.0,0.4,0.3);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.95,0.4,0.3);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.95,0.4,0.5); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-1.0,0.4,0.5); //titik C
     glEnd();
     
//-------------------------------atapatas---------------------------------------
     //atapdepanatastengah     
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,1.0,-1.0);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,1.0,-0.04);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,1.0,-0.04); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,1.0,-1.0); //titik C
     glEnd();
    
     //atapdepanatastengahkiri     
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,0.98,-0.04);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,0.98,-1.0);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.35,1.0,-1.0); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.35,1.0,-0.04); //titik C
     glEnd();
    
     //atapdepanatastengahkanan     
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,0.98,-0.04);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,0.98,-1.0);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.35,1.0,-1.0); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.35,1.0,-0.04); //titik C
     glEnd();
       
     //atapdepanatastengahkiridalam
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.35,0.98,-0.1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.35,0.98,-1.0);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,0.98,-1.0); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,0.98,-0.1); //titik C
     glEnd();
    
     //atapdepanatastengahkanandalam
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.35,0.98,-1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.35,0.98,-0.1);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.4,0.98,-0.1); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,0.98,-1); //titik C
     glEnd();
      
     //atapdepanatastengahkiridalamluar
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.4,0.98,-0.1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.4,0.98,-1);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.4,1,-1); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,1,-0.1); //titik C
     glEnd();
     
     //atapdepanatastengahkanandalamluar
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,0.98,-0.1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.4,0.98,-1);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.4,1,-1); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,1,-0.1); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.4,1,-0.1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,1,-0.1);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,1,-0.7); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,1,-0.7); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbangetwow
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.4,1,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.7,1,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.7,1,-0.9); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,1,-0.9); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.4,1,-0.9);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.45,1,-0.9);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.45,1,-1); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.4,1,-1); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbangetwow
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,1,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.7,1,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.7,1,-0.9); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,1,-0.9); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,1,-0.9);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.45,1,-0.9);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.45,1,-1); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,1,-1); //titik C
     glEnd();
     
     //atapdepanatastengahkanandalamluarbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.4,1,-0.1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,1,-0.1);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,1,-0.7); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.4,1,-0.7); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbangetbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,0.98,-0.1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.76,0.98,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.76,1,-0.7); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,1,-0.1); //titik C
     glEnd();
     
     //atapdepanatastengahkanandalamluarbangetbanget
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,0.98,-0.1);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.76,0.98,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.76,1,-0.7); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,1,-0.1); //titik C
     glEnd();
    
     //atapdepanatastengahkiridalamluarbangetsekali
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.76,0.98,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,0.98,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,0.98,-0.18); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.76,0.98,-0.18); //titik C
     glEnd();
           
     //atapdepanatastengahkanandalamluarbangetsekali
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.76,0.98,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,0.98,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,0.98,-0.18); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.76,0.98,-0.18); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbangetbangetluarbinasa
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.8,0.98,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,0.98,-0.18);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,1,-0.14); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.8,1,-0.7); //titik C
     glEnd();
     
     //atapdepanatastengahkanandalamluarbangetbangetluarbinasa
     glBindTexture(GL_TEXTURE_2D, texture[19]);
     
      glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.8,0.98,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,0.98,-0.18);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,1,-0.14); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.8,1,-0.7); //titik C
     glEnd();
     
     //atapdepanatastengahkiridalamluarbangetwahkeren
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(0.95,1,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(0.8,1,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(0.8,1,-0.1); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(0.95,1,-0.1); //titik C
     glEnd();
     
     //atapdepanatastengahkanandalamluarbangetwahkeren
     glBindTexture(GL_TEXTURE_2D, texture[19]);
    
     glBegin(GL_QUADS);
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.95,1,-0.7);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.8,1,-0.7);  //titik A       
       glTexCoord2f(1.0, 1.0);glVertex3f(-0.8,1,-0.1); //titik B
       glTexCoord2f(0.0, 1.0);glVertex3f(-0.95,1,-0.1); //titik C
     glEnd();
     
     
     
}


//--------------------------Procedure pohon-------------------------------------
void daun()
{
     //bawah
     glBegin(GL_POLYGON);
       glVertex3f(-0.15,-0.3,0.2);  //titik A
       glVertex3f(0.15,-0.3,0.2); //titik B
       glVertex3f(0.2,-0.3,0.0); //titik C
       glVertex3f(0.15,-0.3,-0.2);  //titik D
       glVertex3f(-0.15,-0.3,-0.2);  //titik E
       glVertex3f(-0.2,-0.3,0.0);  //titik F
     glEnd();
     
     //depan
     glBindTexture(GL_TEXTURE_2D, texture[9]);
     glBegin(GL_POLYGON);       
       glTexCoord2f(0.5, 1.0);glVertex3f(0,0.2,0);  //titik G
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.15,-0.3,0.2);  //titik A
       glTexCoord2f(1.0, 0.0);glVertex3f(0.15,-0.3,0.2); //titik B
       glEnd(); 
       
     //kanan depan
     glBindTexture(GL_TEXTURE_2D, texture[9]);
     glBegin(GL_POLYGON);       
       glTexCoord2f(0.5, 1.0);glVertex3f(0,0.2,0);  //titik G
       glTexCoord2f(0.0, 0.0);glVertex3f(0.15,-0.3,0.2); //titik B
       glTexCoord2f(1.0, 0.0);glVertex3f(0.2,-0.3,0.0); //titik C
     glEnd(); 
      
     //kanan belakanggl
     glBegin(GL_POLYGON); 
       glTexCoord2f(0.5, 1.0);glVertex3f(0,0.2,0);  //titik G
       glTexCoord2f(0.0, 0.0);glVertex3f(0.2,-0.3,0.0); //titik C
       glTexCoord2f(1.0, 0.0);glVertex3f(0.15,-0.3,-0.2);  //titik D
     glEnd();  
     
     //belakang
     glBindTexture(GL_TEXTURE_2D, texture[9]);
     glBegin(GL_POLYGON);
       glTexCoord2f(0.5, 1.0);glVertex3f(0,0.2,0);  //titik G
       glTexCoord2f(0.0, 0.0);glVertex3f(0.15,-0.3,-0.2);  //titik D
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.15,-0.3,-0.2);  //titik E
       glEnd(); 
       
     //kiri belakang
     glBindTexture(GL_TEXTURE_2D, texture[9]);
     glBegin(GL_POLYGON);
       glTexCoord2f(0.5, 1.0);glVertex3f(0,0.2,0);  //titik G
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.15,-0.3,-0.2);  //titik E
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.2,-0.3,0.0);  //titik F
     glEnd();
      
     //kiri depan 
     glBindTexture(GL_TEXTURE_2D, texture[9]);
     glBegin(GL_POLYGON);
       glBindTexture(GL_TEXTURE_2D, texture[9]);
       glTexCoord2f(0.5, 1.0);glVertex3f(0,0.2,0);  //titik G
       glTexCoord2f(0.0, 0.0);glVertex3f(-0.2,-0.3,0.0);  //titik F
       glTexCoord2f(1.0, 0.0);glVertex3f(-0.15,-0.3,0.2);  //titik A
     glEnd();
}

void pohon(){
     GLUquadricObj *p = gluNewQuadric();
     //batang
     glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, texture[10]);
        glTranslatef(0.0,-0.5,0.0);
        glRotatef(180, 0.0, 1.0, 1.0);
        glScalef(1, 1, 5);
        gluCylinder(p, 0.04, 0.04, 0.04, 32, 32);
     glPopMatrix();
        //daun
       daun();
     y=0.12;
     for(i=1;i<2;i++){
         glPushMatrix();
            glTranslatef(0.0,y, 0.0);
            daun();
         glPopMatrix();
         y=y+0.12;
      }
}

//--------------------------Procedure jalan -----------------------------------
void jalan()
{
    glBindTexture(GL_TEXTURE_2D, texture[14]);
     glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);glVertex3f(-3,-0.49,0.56); //atas kiri
        glTexCoord2f(1.0, 0.0);glVertex3f(3,-0.49,0.56); //atas kanan
        glTexCoord2f(1.0, 1.0);glVertex3f(3,-0.49,1);//kanan bawah 
        glTexCoord2f(0.0, 1.0);glVertex3f(-3,-0.49,1); //kiri bawah
    glEnd();
}
void trotoar()
{
    glBindTexture(GL_TEXTURE_2D, texture[13]);
     glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);glVertex3f(-3,-0.49,0.54); //atas kiri
        glTexCoord2f(1.0, 0.0);glVertex3f(3,-0.49,0.54); //atas kanan
        glTexCoord2f(1.0, 1.0);glVertex3f(3,-0.49,0.56);//kanan bawah 
        glTexCoord2f(0.0, 1.0);glVertex3f(-3,-0.49,0.56); //kiri bawah
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[13]);
     glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);glVertex3f(-3,-0.49,1); //atas kiri
        glTexCoord2f(1.0, 0.0);glVertex3f(3,-0.49,1); //atas kanan
        glTexCoord2f(1.0, 1.0);glVertex3f(3,-0.49,1.02);//kanan bawah 
        glTexCoord2f(0.0, 1.0);glVertex3f(-3,-0.49,1.02); //kiri bawah
    glEnd();
}
//---------------------------Procedure Parkiran---------------------------------
void parkiran()
{
    glBindTexture(GL_TEXTURE_2D, texture[12]);
     glBegin(GL_QUADS);
        glTexCoord2f(1.0, 1.0);glVertex3f(1,-0.49,0.54); //atas kiri
        glTexCoord2f(0.0, 1.0);glVertex3f(1.7,-0.49,0.54); //atas kanan
        glTexCoord2f(0.0, 0.0);glVertex3f(1.7,-0.49,-0.54);//kanan bawah 
        glTexCoord2f(1.0, 0.0);glVertex3f(1,-0.49,-0.54); //kiri bawah
    glEnd();
}
//---------------------------procedure background-------------------------------
void background(){
     glBindTexture(GL_TEXTURE_2D, texture[11]);
     	glBegin(GL_QUADS);
        glTexCoord2f(1.0, 1.0);glVertex3f(-3,5,-3); //atas kiri
        glTexCoord2f(0.0, 1.0);glVertex3f(3,5,-3); //atas kanan
        glTexCoord2f(0.0, 0.0);glVertex3f(3,-0.5,-3);//kanan bawah 
        glTexCoord2f(1.0, 0.0);glVertex3f(-3,-0.5,-3); //kiri bawah
        	
	glEnd();
}
//--------------------------Procedure kamera------------------------------------
void kamera(){
    gluLookAt (0 ,1.5 , 5 ,0 , p , 1 , 0 , 4 , 1 ) ;
    glTranslatef( tx , ty , tz ) ;
    glRotatef(roty , 0 , 1 , 0 ) ;
}
/*----------------------------------------------------------------------------*/
void display(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_COLOR_MATERIAL); //mengaktifkan fungsi untuk memberikan warna pada material
    glColorMaterial(GL_FRONT, GL_DIFFUSE); //parameter-parameter fungsi warna material
    // Set the camera orientation 
    glMatrixMode(GL_MODELVIEW) ;
    glLoadIdentity ( ) ;
    kamera();
    
    rumput();
    dinding_gedung();
    //pohon kiri
    glPushMatrix();
        glTranslatef(-1.5,0,0.35);
        pohon();
    glPopMatrix();
     //pohon kanan
    glPushMatrix();
        glTranslatef(-1.8,0,0.35);
        pohon();
    glPopMatrix();
    
    //jalan
    jalan();
    trotoar();
    //parkiran
    parkiran();
    
    //gambar background 
    background();
    glutSwapBuffers();
    
}
void resize(int w1, int h1){
    glViewport(0,0,w1,h1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(float) w1/(float) h1, 1.0,300.0);
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();

}


   
//special-key event untuk fullscreen atau normal
void specialKey(int key, int x, int y) {
    switch (key) {
      case GLUT_KEY_F1:    // F1: Toggle between full-screen and windowed mode
         fullScreenMode = !fullScreenMode;         // Toggle state
         if (fullScreenMode) {                     // Full-screen mode
            windowPosX   = glutGet(GLUT_WINDOW_X); // Save parameters
            windowPosY   = glutGet(GLUT_WINDOW_Y);
            windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
            windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();                      // Switch into full screen
         } else {                                         // Windowed mode
            glutReshapeWindow(windowWidth, windowHeight); // Switch into windowed mode
            glutPositionWindow(windowPosX, windowPosX);   // Position top-left corner
         }
         break;
      default:
         break;
   }
}
/*----------------------------------------------------------------------------*/
//program utama
int main (int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);
    glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
    glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
    glutCreateWindow(title);     // Create window with the given title
    glutFullScreen();           // Pull into full screen
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(myKeyboard); // memanggil Prosedure interaksi pada keyboard
    glutSpecialFunc(specialKey); // memanggil special-key event

    glutIdleFunc(display); 
    init();
    glutMainLoop();
    return 0;
}
