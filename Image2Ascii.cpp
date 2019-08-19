#include <iostream>
#include <string>
#include <fstream>

typedef struct
{
	uint8_t r, g, b, a;
} rgb32;
#pragma pack(2)
typedef struct
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
} bitmap_file_header;
#pragma pack()
#pragma pack(2)
typedef struct
{
	uint32_t biSize;
	int32_t biWidth;
	int32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int16_t biXPelsPerMeter;
	int16_t biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
} bitmap_info_header;
#pragma pack()
#pragma pack(2)
typedef struct
{
	bitmap_file_header fHeader;
	bitmap_info_header iHeader;
} bitmap_header;
#pragma pack()

#include "bitmap.h"
#include "userInputFunctions.h"


int main()
{
	//Get the name of the file to open
	char filename[20];
	setFileName(filename);

	bitmap bmp(filename);  //convert bmp to ADT to acess pixels

	char paintSymbols[] = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";//there are 65 symbols
	int symbolCount = 65;

	std::ofstream fout("output.txt"); //Open a file for writing

	//Initialize variables that will be used in loop
	rgb32* pixel;
	float brightness;
	int charIndex;

	//Loop through each pixel in image
	for (int r = 0; r < bmp.getHeight(); r++)
	{
		for (int c = 0; c < bmp.getWidth(); c++)
		{
			pixel = bmp.getPixel(c, r); //Get pixel at location (r,c)
			brightness = (pixel->r + pixel->g + pixel->b) / 3; //Get the average magnitude of the color values for brightness
			charIndex = (int)(brightness * symbolCount / 255); //Find the char in paintSymbols corresponding the the brightness value
			fout << paintSymbols[charIndex];	//Add that character to the file
		}
		fout << std::endl;	//After every row add a newline to the file.
	}
	fout.close();
	std::cout << "See output.txt for ascii conversion.";
}