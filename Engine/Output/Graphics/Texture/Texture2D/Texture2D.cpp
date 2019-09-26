#include "Texture2D.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Output/Graphics/Renderer/PixelBuffer.h>


#ifdef SendMessage
   #undef SendMessage
#endif
StringANSI Texture2D::texturesSuffix = "texture";
StringANSI Texture2D::texturesDir = "textures/";





Texture2D::Texture2D(void): textureId(0), cachingEnable(false), pixelBuffer(NIL), hitboxEnable(false), filteringMode(POINT_FILTERING), magFilter(GL_NEAREST), minFilter(GL_NEAREST), typeOfTextureMap(QUAD_TEXTURE_MAP)
{
	AddClassProperty(TEXTURE_2D_CLASS);
}


Texture2D::~Texture2D(void)
{
	Destroy();
	Destroying();
}


void Texture2D::Destroy(void)
{
	if(IsExist())
	{ 
		if(IsReceiversExist(DESTROY_TEXTURE_MESSAGE)) { SendMessage(DESTROY_TEXTURE_MESSAGE); }
		glDeleteTextures(1, &textureId);
		textureId = 0;
		pixelImageCache.Destroy();
	} 
	hitbox.Clear();
}



void Texture2D::BindHitbox(Hitbox* _hitbox)
{
	if(hitbox.Get() != _hitbox)
	{
		if(hitbox.IsNotEmpty())
		{ 
			hitbox.Clear();
		}
			
		if(_hitbox)
		{	
			hitbox.Attach(_hitbox);

			if(IsReceiversExist(PolygonalSurface::CHANGE_HITBOX_MESSAGE)) { SendPackage(PolygonalSurface::CHANGE_HITBOX_MESSAGE, hitbox.Get()->GetName()); }
		}
	}
}


Vector3 Texture2D::GetBoxTextureMapSize(void)const
{
	return boxImageSize;
}


Vector2 Texture2D::GetQuadTextureMapSize(void)const
{
	return imageSize;
}


Hitbox* Texture2D::GetHitbox(void)const
{
	return hitbox.Get();
}


bool Texture2D::IsHitboxEnabled(void)const
{
	return hitboxEnable;
}


void Texture2D::SetHitboxEnable(bool _enable)
{
	hitboxEnable = _enable;

	if(IsReceiversExist(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE)) { SendPackage(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE, hitboxEnable); }
}


bool Texture2D::IsExist(void)const
{
	return textureId > 0;
}


int32 Texture2D::GetTypeOfTextureMap(void)const
{
	return typeOfTextureMap;
}


uint32 Texture2D::GetTextureId(void)const
{
	return textureId;
}



Vector2 Texture2D::GetImageSize(void)const
{
	return imageSize;
}




bool Texture2D::FromScreen(Vector2 _position, Vector2 _size, Vector2 _offset)
{
	if(IsExist())
	{
		if(_position.x < 0.0f || _position.y < 0.0f) { return false; }
			
		if(_size.x > 0.0f && _size.y > 0.0f)
		{				
			if((_position.x + _size.x) > Renderer::_GetPicked()->GetClientAreaSize().x || (_position.y + _size.y) > Renderer::_GetPicked()->GetClientAreaSize().y) { return false; }

			if(_offset.x < 0.0f || _offset.y < 0.0f) { return false; }

			if((_offset.x + _size.x) > imageSize.x || (_offset.y + _size.y) > imageSize.y) { return false; }
				
			glBindTexture(GL_TEXTURE_2D, (GLuint)textureId);
			glCopyTexSubImage2D(GL_TEXTURE_2D,
							    0,
								_offset.x,
								_offset.y,
								_position.x,
								_position.y,
								_size.x, 
								_size.y);

			if(cachingEnable) { GetImage(pixelImageCache); }
			return true;
		}
	}
	return false;
}




void Texture2D::SetRenderTarget(PixelBuffer* _pixelBuffer)
{
	pixelBuffer = _pixelBuffer;

	if(IsReceiversExist(CHANGE_RENDER_TARGET_ROLE_MESSAGE)) { SendMessage(CHANGE_RENDER_TARGET_ROLE_MESSAGE); }
}


bool Texture2D::IsRenderTarget(void)const
{
	return (pixelBuffer != NIL) ? true : false;
}



bool Texture2D::CreateRenderTarget(Vector2 _imageSize)
{  
	if(IsExist())
	{ 
		glDeleteTextures(1, &textureId);
		textureId = 0;
		pixelImageCache.Destroy();
	} 

	if(_imageSize.x > 0.0f && _imageSize.y > 0.0f)
	{
		if(_imageSize.x > _GetTextureMaxSize() || _imageSize.y > _GetTextureMaxSize())
		{
			WriteToLogFile("Texture2D::CreateRenderTarget: texture size is overflow '%d'", _GetTextureMaxSize());
			return false; 
		} 
		
		imageSize = _imageSize;
		textureSize = imageSize;
 
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, (GLuint)textureId);
			
		ApplyFilteringMode();
		PixelImage image(textureSize, RGBA());
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 (int32)textureSize.x,
					 (int32)textureSize.y,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 image.GetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		if(IsExist())
		{
			if(cachingEnable) { GetImage(pixelImageCache); }
			return true;   
		}
	}
	return false;
}


bool Texture2D::CreateQuadTextureMap(Vector2 _imageSize, RGBA _color)
{
	if(IsExist())
	{ 
		if(IsReceiversExist(DESTROY_TEXTURE_MESSAGE)) { SendMessage(DESTROY_TEXTURE_MESSAGE); }
		glDeleteTextures(1, &textureId);
		textureId = 0;
		pixelImageCache.Destroy();
	} 
 
	if(_imageSize.x > 0.0f && _imageSize.y > 0.0f)
	{
		if(_imageSize.x > _GetTextureMaxSize() || _imageSize.y > _GetTextureMaxSize())
		{
			WriteToLogFile("Texture2D::CreateQuadTextureMap: texture size is overflow '%d'", _GetTextureMaxSize());
			return false; 
		} 

		typeOfTextureMap = QUAD_TEXTURE_MAP;
		imageSize = _imageSize;
		textureSize = Vector2(Common::_GetNextPowerOf2((int32)imageSize.x), Common::_GetNextPowerOf2((int32)imageSize.y));
 
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, (GLuint)textureId);
			
		ApplyFilteringMode();
		PixelImage image(textureSize, _color);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 (int32)textureSize.x,
					 (int32)textureSize.y,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 image.GetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		if(IsExist())
		{
			if(cachingEnable) { GetImage(pixelImageCache); }
			if(IsReceiversExist(INIT_TEXTURE_MESSAGE)) { SendMessage(INIT_TEXTURE_MESSAGE); }
			return true;
		}
	}
	return false;
}


bool Texture2D::Create(int32 _typeOfTextureMap, StringANSI _path)
{ 
	if(IsExist())
	{ 
		if(IsReceiversExist(DESTROY_TEXTURE_MESSAGE)) { SendMessage(DESTROY_TEXTURE_MESSAGE); }
		glDeleteTextures(1, &textureId);
		textureId = 0;
		pixelImageCache.Destroy();
	} 
	
	PixelImage image(_path);

	if(image.IsExist())
	{
		if(image.GetWidth() > _GetTextureMaxSize() || image.GetHeight() > _GetTextureMaxSize())
		{
			WriteToLogFile("Texture2D::Create: texture size is overflow '%d'", _GetTextureMaxSize());
			return false; 
		}  
		
		typeOfTextureMap = _typeOfTextureMap;
		imageSize = image.GetSize();
		textureSize = Vector2(Common::_GetNextPowerOf2((int32)imageSize.x), Common::_GetNextPowerOf2((int32)imageSize.y));
  
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, (GLuint)textureId);
			
		ApplyFilteringMode();

		if(imageSize != textureSize)
		{
			PixelImage newImage(textureSize, RGBA(RGBA::INVISIBLE_COLOR));
			for(int32 j = 0; j < imageSize.y; j++)
			{
				memcpy(newImage.GetData() + j * (int32)textureSize.x,
					   image.GetData() + j * (int32)imageSize.x,
					   (int32)imageSize.x * 4);
			}
			glTexImage2D(GL_TEXTURE_2D,
						 0,
						 GL_RGBA,
						 (int32)textureSize.x,
						 (int32)textureSize.y,
						 0,
						 GL_RGBA,
						 GL_UNSIGNED_BYTE,
						 newImage.GetData());
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,
						 0,
						 GL_RGBA,
						 (int32)textureSize.x,
						 (int32)textureSize.y,
						 0,
						 GL_RGBA,
						 GL_UNSIGNED_BYTE,
						 image.GetData());
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		if(IsExist())
		{
			if(cachingEnable) { GetImage(pixelImageCache); }
			if(IsReceiversExist(INIT_TEXTURE_MESSAGE)) { SendMessage(INIT_TEXTURE_MESSAGE); }
			return true;
		}
	}
	return false;
}



bool Texture2D::Create(int32 _typeOfTextureMap, PixelImage& _image)
{ 
	if(IsExist())
	{ 
		if(IsReceiversExist(DESTROY_TEXTURE_MESSAGE)) { SendMessage(DESTROY_TEXTURE_MESSAGE); }
		glDeleteTextures(1, &textureId);
		textureId = 0;
		pixelImageCache.Destroy();
	} 

	if(_image.IsExist())
	{
		if(_image.GetWidth() > _GetTextureMaxSize() || _image.GetHeight() > _GetTextureMaxSize())
		{
			WriteToLogFile("Texture2D::Create: texture size is overflow '%d'", _GetTextureMaxSize());
			return false; 
		} 
		
		typeOfTextureMap = _typeOfTextureMap;
		imageSize = _image.GetSize();
		textureSize = Vector2(Common::_GetNextPowerOf2((int32)imageSize.x), Common::_GetNextPowerOf2((int32)imageSize.y));
  
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, (GLuint)textureId);
			
		ApplyFilteringMode();

		if(imageSize != textureSize)
		{
			PixelImage newImage(textureSize, RGBA(RGBA::INVISIBLE_COLOR));
			for(int32 j = 0; j < imageSize.y; j++)
			{
				memcpy(newImage.GetData() + j * (int32)textureSize.x,
					   _image.GetData() + j * (int32)imageSize.x,
					   (int32)imageSize.x * 4);
			}
			glTexImage2D(GL_TEXTURE_2D,
						 0,
						 GL_RGBA,
						 (int32)textureSize.x,
						 (int32)textureSize.y,
						 0,
						 GL_RGBA,
						 GL_UNSIGNED_BYTE,
						 newImage.GetData());
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,
						 0,
						 GL_RGBA,
						 (int32)textureSize.x,
						 (int32)textureSize.y,
						 0,
						 GL_RGBA,
						 GL_UNSIGNED_BYTE,
						 _image.GetData());
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		if(IsExist())
		{
			if(cachingEnable) { GetImage(pixelImageCache); }
			if(IsReceiversExist(INIT_TEXTURE_MESSAGE)) { SendMessage(INIT_TEXTURE_MESSAGE); }
			return true;
		}
	}
	return false;
}


void Texture2D::CreateBoxTextureMap(Vector3 _size, RGBA _color)
{
	if(IsExist())
	{ 
		if(IsReceiversExist(DESTROY_TEXTURE_MESSAGE)) { SendMessage(DESTROY_TEXTURE_MESSAGE); }
		glDeleteTextures(1, &textureId);
		textureId = 0;
		pixelImageCache.Destroy();
	} 
 
	if(_size.x > 0.0f && _size.y > 0.0f && _size.z > 0.0f)
	{
		float width = Numerical<float>::_GetMax(_size.x, _size.z);
		float height = _size.y * 4.0f + _size.x * 2.0f;

		if(width > _GetTextureMaxSize() || height > _GetTextureMaxSize())
		{
			WriteToLogFile("Texture2D::SetBoxTextureMap: texture size is overflow '%d'", _GetTextureMaxSize());
			return; 
		} 
		
		typeOfTextureMap = BOX_TEXTURE_MAP;
		boxImageSize = _size;
		imageSize = Vector2(width, height);
		textureSize = Vector2(Common::_GetNextPowerOf2((int32)imageSize.x), Common::_GetNextPowerOf2((int32)imageSize.y));
 
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, (GLuint)textureId);
			
		ApplyFilteringMode();
		PixelImage image(textureSize, _color);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 (int32)textureSize.x,
					 (int32)textureSize.y,
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 image.GetData());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		if(IsExist())
		{
			if(cachingEnable) { GetImage(pixelImageCache); }
			if(IsReceiversExist(INIT_TEXTURE_MESSAGE)) { SendMessage(INIT_TEXTURE_MESSAGE); }
		}
	}
}


bool Texture2D::SetBoxTextureMap(int32 _faceId, Texture2D* _texture)
{
	if(_texture && _texture->IsExist() && typeOfTextureMap == BOX_TEXTURE_MAP)
	{
		switch(_faceId)
		{
			case BOX_FRONT_FACE:
			{
				if(_texture->GetImageSize().x != boxImageSize.x || _texture->GetImageSize().y != boxImageSize.y)
				{
					WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
					return false;  
				}

				PixelImage image;
				_texture->GetImage(image);
				SetImage(Vector2(0.0f, 0.0f), image);
				return true;
			}

			case BOX_BACK_FACE:
			{
				if(_texture->GetImageSize().x != boxImageSize.x || _texture->GetImageSize().y != boxImageSize.y)
				{
					WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
					return false;  
				}

				PixelImage image;
				_texture->GetImage(image);
				SetImage(Vector2(0.0f, boxImageSize.y), image);
				return true;
			}

			case BOX_LEFT_FACE:
			{
				if(_texture->GetImageSize().x != boxImageSize.z || _texture->GetImageSize().y != boxImageSize.y)
				{
					WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
					return false;  
				}

				PixelImage image;
				_texture->GetImage(image);
				SetImage(Vector2(0.0f, boxImageSize.y * 2.0f), image);
				return true;
			}

			case BOX_RIGHT_FACE:
			{
				if(_texture->GetImageSize().x != boxImageSize.z || _texture->GetImageSize().y != boxImageSize.y)
				{
					WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
					return false;  
				}

				PixelImage image;
				_texture->GetImage(image);
				SetImage(Vector2(0.0f, boxImageSize.y * 3.0f), image);
				return true;
			}

			case BOX_TOP_FACE:
			{
				if(_texture->GetImageSize().x != boxImageSize.z || _texture->GetImageSize().y != boxImageSize.x)
				{
					WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
					return false;  
				}

				PixelImage image;
				_texture->GetImage(image);
				SetImage(Vector2(0.0f, boxImageSize.y * 4.0f), image);
				return true;
			}

			case BOX_BOTTOM_FACE:
			{
				if(_texture->GetImageSize().x != boxImageSize.z || _texture->GetImageSize().y != boxImageSize.x)
				{
					WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
					return false;  
				}

				PixelImage image;
				_texture->GetImage(image);
				SetImage(Vector2(0.0f, boxImageSize.y * 4.0f + boxImageSize.x), image);
				return true;
			}
		}
	}
	return false;
}

 
bool Texture2D::SetBoxTextureMap(int32 _faceId, StringANSI _path)
{ 
	if(typeOfTextureMap == BOX_TEXTURE_MAP)
	{
		switch(_faceId)
		{
			case BOX_FRONT_FACE:
			{
				PixelImage image(_path);

				if(image.IsExist())
				{
					if(image.GetWidth() != boxImageSize.x || image.GetHeight() != boxImageSize.y)
					{
						WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
						return false; 
					} 
				}

				SetImage(Vector2(0.0f, 0.0f), image);
				return true;
			}

			case BOX_BACK_FACE:
			{
				PixelImage image(_path);

				if(image.IsExist())
				{
					if(image.GetWidth() != boxImageSize.x || image.GetHeight() != boxImageSize.y)
					{
						WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
						return false; 
					} 
				}

				SetImage(Vector2(0.0f, boxImageSize.y), image);
				return true;
			}

			case BOX_LEFT_FACE:
			{
				PixelImage image(_path);

				if(image.IsExist())
				{
					if(image.GetWidth() != boxImageSize.z || image.GetHeight() != boxImageSize.y)
					{
						WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
						return false; 
					} 
				}

				SetImage(Vector2(0.0f, boxImageSize.y * 2.0f), image);
				return true;
			}

			case BOX_RIGHT_FACE:
			{
				PixelImage image(_path);

				if(image.IsExist())
				{
					if(image.GetWidth() != boxImageSize.z || image.GetHeight() != boxImageSize.y)
					{
						WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
						return false; 
					} 
				}

				SetImage(Vector2(0.0f, boxImageSize.y * 3.0f), image);
				return true;
			}

			case BOX_TOP_FACE:
			{
				PixelImage image(_path);

				if(image.IsExist())
				{
					if(image.GetWidth() != boxImageSize.z || image.GetHeight() != boxImageSize.x)
					{
						WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
						return false; 
					} 
				}

				SetImage(Vector2(0.0f, boxImageSize.y * 4.0f), image);
				return true;
			}

			case BOX_BOTTOM_FACE:
			{
				PixelImage image(_path);

				if(image.IsExist())
				{
					if(image.GetWidth() != boxImageSize.z || image.GetHeight() != boxImageSize.x)
					{
						WriteToLogFile("Texture2D::SetBoxTextureMap: image size is invalid");
						return false; 
					} 
				}

				SetImage(Vector2(0.0f, boxImageSize.y * 4.0f + boxImageSize.x), image);
				return true;
			}
		}
	}
	return false;
}


Vector2 Texture2D::GetTextureSize(void)const
{
	return textureSize;
} 



bool Texture2D::IsCachingEnabled(void)const
{
	return cachingEnable;
}


void Texture2D::SetCachingEnable(bool _enable)
{
	if(cachingEnable && !_enable)
	{
		pixelImageCache.Destroy();
		cachingEnable = _enable;
	}
	else if(!cachingEnable && _enable)
	{
		GetImage(pixelImageCache);
		cachingEnable = _enable;
	}
}



bool Texture2D::GetImage(PixelImage& _image)
{
	if(IsExist())
	{ 
		_image.Resize(imageSize, RGBA());
		
		if(imageSize != textureSize)
		{
			if(cachingEnable)
			{
				for(int32 j = 0; j < (int32)imageSize.y; j++)
				{
					memcpy(_image.GetData() + j * (int32)imageSize.x,
						   pixelImageCache.GetData() + j * (int32)pixelImageCache.GetSize().x,
						   (int32)imageSize.x * 4);
				}
			}
			else
			{
				PixelImage textureImage(textureSize, RGBA());

				glBindTexture(GL_TEXTURE_2D, textureId);

				glGetTexImage(GL_TEXTURE_2D,
							  0,
							  GL_RGBA,
							  GL_UNSIGNED_BYTE,
							  textureImage.GetData());
				for(int32 j = 0; j < imageSize.y; j++)
				{
					memcpy(_image.GetData() + j * (int32)imageSize.x,
						   textureImage.GetData() + j * (int32)textureSize.x,
						   (int32)imageSize.x * 4);
				}
			}
		}
		else
		{
			if(cachingEnable)
			{
				memcpy(_image.GetData(),
					   pixelImageCache.GetData(),
					   (int32)imageSize.x * (int32)imageSize.y * 4);
			}
			else
			{
				if(IsRenderTarget())
				{ 
					glBindTexture(GL_TEXTURE_2D, textureId);

					Renderer::wglBindTexImageARBExt(pixelBuffer->buffer, WGL_FRONT_LEFT_ARB);
				
					glGetTexImage(GL_TEXTURE_2D,
								  0,
								  GL_RGBA,
								  GL_UNSIGNED_BYTE,
								  _image.GetData());

					Renderer::wglReleaseTexImageARBExt(pixelBuffer->buffer, WGL_FRONT_LEFT_ARB);
				}
				else
				{
					glBindTexture(GL_TEXTURE_2D, textureId);

					glGetTexImage(GL_TEXTURE_2D,
								  0,
								  GL_RGBA,
								  GL_UNSIGNED_BYTE,
								  _image.GetData());
				}
			}
		}
		return true; 
	}
	return false;
}



bool Texture2D::GetPixel(Vector2 _position, RGBA& _color)
{
	if(IsExist())
	{
		if(_position.x < 0.0f || _position.y < 0.0f) { return false; }

		if((_position.x + 1) > imageSize.x || (_position.y + 1) > imageSize.y) { return false; }

		if(cachingEnable)
		{
			memcpy(_color.GetData(),
				   pixelImageCache.GetData() + (int32)_position.y * (int32)pixelImageCache.GetSize().x + (int32)_position.x,
				   4);
		}
		else
		{
			if(IsRenderTarget())
			{ 
				glBindTexture(GL_TEXTURE_2D, textureId);

				Renderer::wglBindTexImageARBExt(pixelBuffer->buffer, WGL_FRONT_LEFT_ARB);
				PixelImage textureImage(imageSize, RGBA());

				glGetTexImage(GL_TEXTURE_2D,
							  0,
							  GL_RGBA,
							  GL_UNSIGNED_BYTE,
							  textureImage.GetData());
				memcpy(_color.GetData(), 
					   textureImage.GetData() + (int32)_position.y * (int32)imageSize.x + (int32)_position.x,
					   4);

				Renderer::wglReleaseTexImageARBExt(pixelBuffer->buffer, WGL_FRONT_LEFT_ARB);
			}
			else
			{
				PixelImage textureImage(textureSize, RGBA());

				glBindTexture(GL_TEXTURE_2D, textureId);

				glGetTexImage(GL_TEXTURE_2D,
							  0,
							  GL_RGBA,
							  GL_UNSIGNED_BYTE,
							  textureImage.GetData());
				memcpy(_color.GetData(), 
					   textureImage.GetData() + (int32)_position.y * (int32)textureSize.x + (int32)_position.x,
					   4);
			}
		}
		return true; 
	}
	return false;
} 



bool Texture2D::GetImage(Vector2 _position, Vector2 _size, PixelImage& _image)
{
	if(IsExist())
	{ 
		if(_position.x < 0.0f || _position.y < 0.0f) { return false; }

		if(_size.x > 0.0f && _size.y > 0.0f)
		{
			if((_position.x + _size.x) > imageSize.x || (_position.y + _size.y) > imageSize.y) { return false; }

			_image.Resize(_size, RGBA());
			
			const int32 lengthSize = (int32)_size.x;

			if(cachingEnable)
			{
				for(int32 j = 0; j < (int32)_size.y; j++)
				{
					memcpy(_image.GetData() + j * lengthSize,
						   pixelImageCache.GetData() + (j + (int32)_position.y) * (int32)pixelImageCache.GetSize().x + (int32)_position.x,
						   lengthSize * 4);
				}
			}
			else
			{
				if(IsRenderTarget())
				{ 
					glBindTexture(GL_TEXTURE_2D, textureId);

					Renderer::wglBindTexImageARBExt(pixelBuffer->buffer, WGL_FRONT_LEFT_ARB);
					PixelImage textureImage(imageSize, RGBA());

					glGetTexImage(GL_TEXTURE_2D,
								  0,
								  GL_RGBA,
								  GL_UNSIGNED_BYTE,
								  textureImage.GetData());
					for(int32 j = 0; j < (int32)_size.y; j++)
					{
						memcpy(_image.GetData() + j * lengthSize,
							   textureImage.GetData() + (j + (int32)_position.y) * (int32)imageSize.x + (int32)_position.x,
							   lengthSize * 4);
					}	

					Renderer::wglReleaseTexImageARBExt(pixelBuffer->buffer, WGL_FRONT_LEFT_ARB);
				}
				else
				{
					PixelImage textureImage(textureSize, RGBA());

					glBindTexture(GL_TEXTURE_2D, textureId);

					glGetTexImage(GL_TEXTURE_2D,
								  0,
								  GL_RGBA,
								  GL_UNSIGNED_BYTE,
								  textureImage.GetData());
					for(int32 j = 0; j < (int32)_size.y; j++)
					{
						memcpy(_image.GetData() + j * lengthSize,
							   textureImage.GetData() + (j + (int32)_position.y) * (int32)textureSize.x + (int32)_position.x,
							   lengthSize * 4);
					}	
				}
			}
			return true; 
		}
	}
	return false;
} 

	



bool Texture2D::SetPixel(Vector2 _position, RGBA _color)
{
	if(IsExist())
	{
		if(_position.x < 0.0f || _position.y < 0.0f) { return false; }

		if(_position.x < imageSize.x && _position.y < imageSize.y)
		{
			glBindTexture(GL_TEXTURE_2D, textureId);
				
			if(filteringMode == TRILINEAR_FILTERING || filteringMode == ANISOTROPY_FILTERING) { glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); }
			
			glTexSubImage2D(GL_TEXTURE_2D,
							0,
							_position.x, 
							_position.y,
							1, 
							1, 
							GL_RGBA,
							GL_UNSIGNED_BYTE,
							_color.GetData());

			if(cachingEnable) { pixelImageCache.GetPixel(_position) = _color; }
			return true; 
		}
	}
	return false;
}


bool Texture2D::SetImage(PixelImage& _image)
{
	return SetImage(Vector2(), _image);
}


bool Texture2D::SetImage(Vector2 _position, PixelImage& _image)
{
	if(IsExist() && _image.IsExist())
	{
		if(_position.x < 0.0f || _position.y < 0.0f) { return false; }

		if((_position.x + _image.GetWidth()) > imageSize.x || (_position.y + _image.GetHeight()) > imageSize.y) { return false; }

		glBindTexture(GL_TEXTURE_2D, textureId);
		
		if(filteringMode == TRILINEAR_FILTERING || filteringMode == ANISOTROPY_FILTERING) { glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); }
		
		glTexSubImage2D(GL_TEXTURE_2D,
						0,
						_position.x, 
						_position.y,
						_image.GetWidth(), 
						_image.GetHeight(), 
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						_image.GetData());

		if(cachingEnable) { pixelImageCache.SetImage(_position, _image); }
		return true; 
	}
	return false;
}





int32 Texture2D::_GetTextureMaxSize(void)
{
	int32 maxSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
	return maxSize;
}



int32 Texture2D::GetFilteringMode(void)
{
	return filteringMode;
}



void Texture2D::SetFilteringMode(int32 _mode)
{
	if(filteringMode != _mode)
	{
		filteringMode = _mode;

		switch(_mode)
		{
			case POINT_FILTERING:
			{
				magFilter = GL_NEAREST;
				minFilter = GL_NEAREST;
				ApplyFilteringMode();
				break;
			}

			case BILINEAR_FILTERING:
			{
				magFilter = GL_LINEAR;
				minFilter = GL_LINEAR;
				ApplyFilteringMode();
				break;
			}

			case TRILINEAR_FILTERING:
			case ANISOTROPY_FILTERING:
			{
				magFilter = GL_LINEAR;
				minFilter = GL_LINEAR_MIPMAP_LINEAR;
				ApplyFilteringMode();
				break; 
			}
		}
	}
}



void Texture2D::ApplyFilteringMode(void)
{
	if(IsExist() && !IsRenderTarget())
	{
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		if(filteringMode == ANISOTROPY_FILTERING) 
		{
			if(Renderer::_GetTextureMaxAnisotropy() != 0)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, Renderer::_GetTextureMaxAnisotropy());

				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
				glTexSubImage2D(GL_TEXTURE_2D,
								0,
								0, 
								0,
								0, 
								0, 
								GL_RGBA,
								GL_UNSIGNED_BYTE,
								NIL);
			}
		}
		else if(filteringMode == TRILINEAR_FILTERING) 
		{ 
			if(Renderer::_GetTextureMaxAnisotropy() != 0)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
			}

			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
			glTexSubImage2D(GL_TEXTURE_2D,
							0,
							0, 
							0,
							0, 
							0, 
							GL_RGBA,
							GL_UNSIGNED_BYTE,
							NIL);
		}
		else
		{
			if(Renderer::_GetTextureMaxAnisotropy() != 0)
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
			}
		}
	}
}


StringANSI Texture2D::_GetDir(void)
{
	return texturesDir;
}


void Texture2D::_SetFileSuffix(StringANSI _suffix)
{
	texturesSuffix = _suffix;
}


StringANSI Texture2D::_GetFileSuffix(void)
{
	return texturesSuffix;
}



bool Texture2D::SaveToFile(StringANSI _path)
{ 
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + texturesDir + GetName() + "." + texturesSuffix;
	}

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Texture2D::SaveToFile(File& _file)
{ 
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 2;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(filteringMode);
		_file.Write(magFilter);
		_file.Write(minFilter);
		_file.Write(hitboxEnable);
		_file.Write(hitbox.IsNotEmpty());
				
		if(hitbox.IsNotEmpty())
		{
			bool hitboxIsCommonAsset = hitbox.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(hitboxIsCommonAsset);

			if(hitboxIsCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(hitbox.Get()->GetName(), AssetLibrary::MESH_ASSET))
				{
					hitbox.Get()->SaveToFile();
				}
				_file.Write(hitbox.Get()->GetName().length());
				_file.WriteString(hitbox.Get()->GetName());
			}
			else
			{
				hitbox.Get()->SaveToFile(_file);
			}
		}
		_file.Write(textureSize);
		_file.Write(imageSize);
		_file.Write(cachingEnable);
		_file.Write(typeOfTextureMap);
		_file.Write(boxImageSize);
		if(cachingEnable)
		{ 
			if(pixelImageCache.SaveImageToPNGFile(_file))
			{
				return true;
			}
		}
		else
		{
			PixelImage image;
					
			if(GetImage(image))
			{
				if(image.SaveImageToPNGFile(_file))
				{
					return true;
				}
			}
		}
	}
	return false;
}




bool Texture2D::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + Texture2D::_GetDir() + _name + "." + Texture2D::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Texture2D::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 2; 
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(filteringMode);
		_file.Write(magFilter);
		_file.Write(minFilter);
		_file.Write(hitboxEnable);
		_file.Write(hitbox.IsNotEmpty());
				
		if(hitbox.IsNotEmpty())
		{
			bool hitboxIsCommonAsset = hitbox.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(hitboxIsCommonAsset);

			if(hitboxIsCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(hitbox.Get()->GetName(), AssetLibrary::HITBOX_ASSET))
				{ 
					hitbox.Get()->SaveToFile();
				}
				_file.Write(hitbox.Get()->GetName().length());
				_file.WriteString(hitbox.Get()->GetName());
			}
			else
			{
				hitbox.Get()->SaveToFile(_file);
			}
		}
		_file.Write(textureSize);
		_file.Write(imageSize);
		_file.Write(cachingEnable);
		_file.Write(typeOfTextureMap);
		_file.Write(boxImageSize);
		if(cachingEnable)
		{ 
			if(pixelImageCache.SaveImageToPNGFile(_file))
			{
				return true;
			}
		}
		else
		{
			PixelImage image;
					
			if(GetImage(image))
			{
				if(image.SaveImageToPNGFile(_file))
				{
					return true;
				}
			}
		}
		return true;
	}
	return false;
}


bool Texture2D::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + texturesDir + _path + "." + texturesSuffix;
	}

	File file(_path, File::READ);
	
	return LoadFromFile(file);
}


bool Texture2D::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Destroy();
		int32 version;
		_file.Read(version);
		_file.Read(properties);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		Rename(name);
		_file.Read(filteringMode);
		_file.Read(magFilter);
		_file.Read(minFilter);
		bool hitboxIsEnabled;
		_file.Read(hitboxIsEnabled);
		SetHitboxEnable(hitboxIsEnabled);
		bool hitboxIsExist;
		_file.Read(hitboxIsExist);
				
		if(hitboxIsExist)
		{
			bool hitboxIsCommonAsset;
			_file.Read(hitboxIsCommonAsset);
			Hitbox* buildInHitbox;
					
			if(hitboxIsCommonAsset)
			{
				StringANSI name;
				int32 length;
				_file.Read(length);
				_file.ReadString(name, length);

				buildInHitbox = dynamic_cast<Hitbox*>(AssetLibrary::_LoadCommonAsset<Hitbox>(name));
			}
			else
			{
				buildInHitbox = dynamic_cast<Hitbox*>(AssetLibrary::_LoadPrivateAsset<Hitbox>(_file));
			}
			if(buildInHitbox) { BindHitbox(buildInHitbox); }
		}
		_file.Read(textureSize);
		_file.Read(imageSize);
		_file.Read(cachingEnable);
		_file.Read(typeOfTextureMap);
		_file.Read(boxImageSize);
		PixelImage image;
	
		if(image.LoadImageFromPNGFile(_file))
		{ 
			if(Create(typeOfTextureMap, image))
			{ 
				return true;
			}
		}
	}
	return false;
}

 

Texture2D* Texture2D::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + texturesDir + _path + "." + texturesSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


Texture2D* Texture2D::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Texture2D* texture = new Texture2D();

		if(texture->LoadFromFile(_file))
		{
			return texture;
		}
		delete texture;
	}
	return NIL;
}
