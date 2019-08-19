#pragma once

class bitmap
{
private:
	bitmap_header header;
	uint8_t* pixels;
public:
	bitmap(const char* path);
	~bitmap();
	void save(const char* path);
	rgb32* getPixel(uint32_t x, uint32_t y) const;
	void setPixel(rgb32* pixel, uint32_t x, uint32_t y);
	uint32_t getWidth() const;
	uint32_t getHeight() const;
};
bitmap::bitmap(const char* path) : header(), pixels(nullptr)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (file)
	{
		file.read(reinterpret_cast<char*>(&header.fHeader), sizeof(header.fHeader));
		if (header.fHeader.bfType != 0x4d42)
		{
			throw std::runtime_error("Invalid format. Only bitmaps are supported.");
		}
		file.read(reinterpret_cast<char*>(&header.iHeader), sizeof(header.iHeader));
		if (header.iHeader.biCompression != 0)
		{
			throw std::runtime_error("Invalid bitmap. Only uncompressed bitmaps are supported.");
		}
		if (header.iHeader.biBitCount != 24)
		{
			throw std::runtime_error("Invalid bitmap. Only 24bit bitmaps are supported.");
		}
		file.seekg(header.fHeader.bfOffBits, std::ios::beg);
		pixels = new uint8_t[header.fHeader.bfSize - header.fHeader.bfOffBits];
		file.read(reinterpret_cast<char*>(&pixels[0]), header.fHeader.bfSize - header.fHeader.bfOffBits);
		uint8_t * temp = new uint8_t[header.iHeader.biWidth * header.iHeader.biHeight * sizeof(rgb32)];
		uint8_t * in = pixels;
		rgb32 * out = reinterpret_cast<rgb32*>(temp);
		int padding = (header.iHeader.biSizeImage - header.iHeader.biWidth * header.iHeader.biHeight * 3) / header.iHeader.biHeight;
		for (int i = 0; i < header.iHeader.biHeight; ++i, in += padding)
		{
			for (int j = 0; j < header.iHeader.biWidth; ++j)
			{
				out->b = *(in++);
				out->g = *(in++);
				out->r = *(in++);
				out->a = 0xFF;
				++out;
			}
		}
		delete[] pixels;
		pixels = temp;
	}
}
bitmap::~bitmap()
{
	delete[] pixels;
}
void bitmap::save(const char* path)
{
	std::ofstream file(path, std::ios::out | std::ios::binary);
	if (file)
	{
		file.write(reinterpret_cast<char*>(&header.fHeader), sizeof(header.fHeader));
		file.write(reinterpret_cast<char*>(&header.iHeader), sizeof(header.iHeader));
		file.seekp(header.fHeader.bfOffBits, std::ios::beg);
		uint8_t* temp = new uint8_t[header.fHeader.bfSize - header.fHeader.bfOffBits];
		uint8_t* out = temp;
		rgb32* in = reinterpret_cast<rgb32*>(pixels);
		int padding = (header.iHeader.biSizeImage - header.iHeader.biWidth * header.iHeader.biHeight * 3) / header.iHeader.biHeight;
		for (int i = 0; i < header.iHeader.biHeight; ++i, out += padding)
		{
			for (int j = 0; j < header.iHeader.biWidth; ++j)
			{
				*(out++) = in->b;
				*(out++) = in->g;
				*(out++) = in->r;
				++in;
			}
		}
		file.write(reinterpret_cast<char*>(&temp[0]), header.fHeader.bfSize - header.fHeader.bfOffBits);
		delete[] temp;
	}
}
rgb32* bitmap::getPixel(uint32_t x, uint32_t y) const
{
	rgb32* temp = reinterpret_cast<rgb32*>(pixels);
	return &temp[(header.iHeader.biHeight - 1 - y) * header.iHeader.biWidth + x];
}
void bitmap::setPixel(rgb32 * pixel, uint32_t x, uint32_t y)
{
	rgb32* temp = reinterpret_cast<rgb32*>(pixels);
	memcpy(&temp[(header.iHeader.biHeight - 1 - y) * header.iHeader.biWidth + x], pixel, sizeof(rgb32));
};
uint32_t bitmap::getWidth() const
{
	return header.iHeader.biWidth;
}
uint32_t bitmap::getHeight() const
{
	return header.iHeader.biHeight;
}