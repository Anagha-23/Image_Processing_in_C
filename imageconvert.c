#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMP_HEADER_SIZE 54
#define MAX_WIDTH 2000
#define MAX_HEIGHT 2000

unsigned char image[MAX_WIDTH][MAX_HEIGHT][3];

int main() {
    char filename[100];
    printf("Enter the filename: ");
    gets(filename);

    FILE *infile = fopen(filename, "rb");
    if (infile == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    unsigned char header[BMP_HEADER_SIZE];
    fread(header, sizeof(unsigned char), BMP_HEADER_SIZE, infile);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int bpp = *(short*)&header[28];

    if (width > MAX_WIDTH || height > MAX_HEIGHT || bpp != 24) {
        printf("Unsupported image format or size\n");
        return 1;
    }

    int padding = (4 - (width * 3) % 4) % 4;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fread(&image[x][y], sizeof(unsigned char), 3, infile);
        }
        fseek(infile, padding, SEEK_CUR);
    }

    fclose(infile);
    // "C:\Users\anubh\OneDrive\Desktop\mysuru trip\IMG_20230218_164408_494.jpg"
    // Grayscale filter
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char gray = 0.3 * image[x][y][0] + 0.59 * image[x][y][1] + 0.11 * image[x][y][2];
            image[x][y][0] = gray;
            image[x][y][1] = gray;
            image[x][y][2] = gray;
        }
    }

    FILE *outfile = fopen("output.bmp", "wb");
    if (outfile == NULL) {
        printf("Error creating output file\n");
        return 1;
    }

    fwrite(header, sizeof(unsigned char), BMP_HEADER_SIZE, outfile);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fwrite(&image[x][y], sizeof(unsigned char), 3, outfile);
        }
        for (int i = 0; i < padding; i++) {
            fputc(0, outfile);
        }
    }

    fclose(outfile);

    return 0;
}
