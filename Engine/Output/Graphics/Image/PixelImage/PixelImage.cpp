#include "PixelImage.h"
#include "png.h"
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>





PixelImage::PixelImage(void)
{}



PixelImage::PixelImage(StringANSI _path)
{ 
	LoadImageFromPNGFile(_path);
}



PixelImage::PixelImage(File& _file)
{
	LoadImageFromPNGFile(_file);
}


PixelImage::PixelImage(Vector2 _size, RGBA _color)
{
	Resize(_size, _color);
}



PixelImage::~PixelImage(void)
{
	Destroy();
}



void PixelImage::Destroy(void)
{
	pixels.clear();
}


bool PixelImage::IsExist(void)const
{
	return !pixels.empty();
}


float PixelImage::GetWidth(void)const
{
	return size.x;
}


float PixelImage::GetHeight(void)const
{
	return size.y;
}




void PixelImage::Fill(RGBA _color)
{
	for(int32 i = 0; i < pixels.size(); ++i)
	{
		pixels[i] = _color;
	}
}


bool PixelImage::Resize(Vector2 _size, RGBA _color)
{
	if(_size.x < 0.0f || _size.y < 0.0f) { return true; }
	
	if(IsExist())
	{ 
		if(size != _size)
		{
			vector<RGBA> cPixels = pixels;
			int32 width = 0, height = 0;

			if(_size.x < size.x)
			{
				width = _size.x;
			}
			else
			{
				width = size.x;
				pixels.clear();
			}

			if(_size.y < size.y)
			{
				height = _size.y;
			}
			else
			{
				height = size.y;
				pixels.clear();
			}

			pixels.resize((int32)(_size.x * _size.y), _color);
			for(int32 j = 0; j < height; j++)
			{
				memcpy(&pixels[0] + j * (int32)_size.x,
					   &cPixels[0] + j * (int32)size.x,
					   width * 4);
			}
			size = _size;
		}
	}
	else
	{
		size = _size;
		pixels.resize((int32)(size.x * size.y), _color);
	}
	return true;
}


bool PixelImage::SetImage(Vector2 _position, PixelImage& _image)
{
	if(IsExist() && _image.IsExist())
	{
		if(_position.x < 0.0f || _position.y < 0.0f) { return false; }

		if((_position.x + _image.GetWidth()) > size.x || (_position.y + _image.GetHeight()) > size.y) { return false; }
		for(int32 i = 0; i < (int32)_image.GetSize().y; i++)
		{
			memcpy(&pixels[(int32)_position.x + ((int32)_position.y + i) * (int32)size.x],
				   &_image.pixels[0 + i * (int32)_image.size.x],
				   (int32)_image.size.x * 4);
		}
		return true;
	}
	return false;
}




const RGBA& PixelImage::GetPixel(Vector2 _position)const
{
	return pixels[(int32)_position.x + (int32)_position.y * (int32)size.x];
}


const RGBA& PixelImage::GetPixel(int32 _index)const
{
	return pixels[_index];
}


bool PixelImage::GetPixel(Vector2 _position, RGBA& _color)
{
	if(_position.x >= 0 && _position.x < (int32)size.x &&
	   _position.y >= 0 && _position.y < (int32)size.y)
	{
		_color = pixels[(int32)_position.x + (int32)_position.y * (int32)size.x];
		return true;
	}
	return false;
}


RGBA& PixelImage::GetPixel(Vector2 _position)
{
	return pixels[(int32)_position.x + (int32)_position.y * (int32)size.x];
}


RGBA& PixelImage::GetPixel(int32 _index)
{
	return pixels[_index];
}


Vector2 PixelImage::GetSize(void)const
{
	return size;
}

 
bool PixelImage::LoadImageFromPNGFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		png_byte header[8];
		fread(header, 1, 8, _file.GetFile());
	    
		if(png_sig_cmp(header, 0, 8))
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: can't read header of '%s'", _file.GetPath().c_str());
			return false; 
		}
		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	    
		if(!png) 
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: can't create png_struct for '%s'", _file.GetPath().c_str());
			return false;
		}
		png_infop info = png_create_info_struct(png);
	   
		if(!info) 
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: can't create png_info1 for '%s'", _file.GetPath().c_str());
			png_destroy_read_struct(&png, (png_infopp)NULL, (png_infopp)NULL); 
			return false;
		}
		png_infop end_info = png_create_info_struct(png);
	    
		if(!end_info) 
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: can't create png_info2 for '%s'", _file.GetPath().c_str());
			png_destroy_read_struct(&png, &info, (png_infopp)NULL);
			return false;
		}
		if(setjmp(png_jmpbuf(png)))
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: libpng error for '%s'", _file.GetPath().c_str());
			png_destroy_read_struct(&png, &info, &end_info);
			return false;
		}
		png_init_io(png, _file.GetFile());
		png_set_sig_bytes(png, 8);
		png_read_info(png, info);
		int32 colorDepth;
		int32 colorFormat;
		png_uint_32 width, height;
		png_get_IHDR(png, info, &width, &height, &colorDepth, &colorFormat, NULL, NULL, NULL);
	    
		if(colorDepth != 8)
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: colorDepth is not 8 for '%s'", _file.GetPath().c_str());
			return false;
		}
		png_read_update_info(png, info);
		int rowbytes = png_get_rowbytes(png, info);
	 
		rowbytes += 3 - ((rowbytes - 1) % 4);
		png_byte *image_data = new png_byte[rowbytes * height];
	    
		if(!image_data)  
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: can't create image_data for '%s'", _file.GetPath().c_str());
			png_destroy_read_struct(&png, &info, &end_info);
			return false;
		}
		png_bytep *row_pointers = new png_bytep[height];
		
		if(!row_pointers) 
		{
			AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: can't create row_pointers for '%s'", _file.GetPath().c_str());
			png_destroy_read_struct(&png, &info, &end_info);
			delete[] image_data;
			return false;
		}
		for(int32 i = 0; i < height; ++i)
		{
			row_pointers[height - 1 - i] = image_data + i * rowbytes;
		}
		png_read_image(png, row_pointers);
		png_read_end(png, 0);
		if(Resize(Vector2(width, height), RGBA()))
		{
			if(colorFormat == PNG_COLOR_TYPE_RGB_ALPHA)
			{
				for(int32 j = 0; j < height; j++)
				{
					memcpy(&pixels[0] + j * width,
						   image_data + j * rowbytes,
						   width * 4);
				}
			}
			else if(colorFormat == PNG_COLOR_TYPE_RGB)
			{
				int32 sizeOfPixel = 3;
				for(int32 j = 0; j < height; j++)
				{
					for(int32 i = 0; i < width; i++)
					{
						memcpy(&pixels[i + j * width],
							   image_data + (i * sizeOfPixel) + j * rowbytes,
							   sizeOfPixel);
					}
				}
			}
			else
			{
				AbstractObject::_WriteToLogFile(_file.GetName(), "PixelImage::LoadImageFromPNGFile: colorFormat is not defined for '%s'", _file.GetPath().c_str());
			}
		}
		png_destroy_read_struct(&png, &info, &end_info);   
		delete[] image_data;
		delete[] row_pointers;
		return true;
	}
	return false;
}




bool PixelImage::LoadImageFromPNGFile(StringANSI _path) 
{
	return LoadImageFromPNGFile(File(_path, File::READ));
}


 
bool PixelImage::SaveImageToPNGFile(File& _file)const
{ 
	if(IsExist() && _file.IsExist() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NIL, NIL, NIL);

		if(!png) { return false; }

		png_infop info = png_create_info_struct(png);
	    
		if(!info) 
		{
			png_destroy_write_struct(&png, &info);
			return false;
		}

		png_init_io(png, _file.GetFile());
		png_set_IHDR(png, info, size.x, size.y, 8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
	    
		if(!palette)
		{
			png_destroy_write_struct(&png, &info);
			return false;
		}

		png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
		png_write_info(png, info);
		png_set_packing(png);

		png_bytepp rows = (png_bytepp)png_malloc(png, (int32)size.y * sizeof(png_bytep));
	   
		for(int32 i = 0; i < size.y; ++i)
		{
			rows[i] = (png_bytep)(&pixels[0] + ((int32)size.y - i - 1) * (int32)size.x);
		}

		png_write_image(png, rows);
		png_write_end(png, info);
		png_free(png, palette);
		png_destroy_write_struct(&png, &info);

		delete[] rows;
		return true;
	}
	return false;
}



bool PixelImage::SaveImageToPNGFile(StringANSI _path)const
{
	return SaveImageToPNGFile(File(_path, File::REWRITE));
}