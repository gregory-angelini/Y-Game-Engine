#include "Atlas2D.h"
#include <Engine/Output/File/File.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


#ifdef SendMessage
   #undef SendMessage
#endif
StringANSI Atlas2D::atlasesSuffix = "atlas";
int32 Atlas2D::maxFrames = 999;
StringANSI Atlas2D::atlasesDir = "atlases/";


Atlas2D::Atlas2D(void): alignedFrameSize(true), filteringMode(Texture2D::POINT_FILTERING)
{
	AddClassProperty(ATLAS_2D_CLASS);
}



Atlas2D::~Atlas2D(void)
{ 
	Destroy();
	Destroying();
}


void Atlas2D::Destroy(void)
{ 
	if(IsExist())
	{
		if(IsReceiversExist(DESTROY_ATLAS_MESSAGE)) { SendMessage(DESTROY_ATLAS_MESSAGE); }
		textures.Clear(MagicContainer::DELETE_NODE);
		frames.clear();
	}
}


StringANSI Atlas2D::_GetDir(void)
{
	return atlasesDir;
}



bool Atlas2D::GetImage(int32 _index, PixelImage& _image)
{
	if(_index >= 0 && _index < frames.size())
	{
		frames[_index].texture->GetImage(frames[_index].positionInPixels, frames[_index].sizeInPixels, _image);
		return true;
	}
	return false;
}


int32 Atlas2D::GetIndexOfFrame(StringANSI _name)
{
	for(int32 i = 0; i < frames.size(); ++i)
	{
		if(frames[i].GetName() == _name)
		{
			return i;
		}
	}
	return -2;
}
 



Atlas2D::Frame* Atlas2D::GetFrameInfo(StringANSI _name)
{
	for(int32 i = 0; i < frames.size(); ++i)
	{
		if(frames[i].GetName() == _name)
		{
			return &frames[i];
		}
	}
	return NIL;
}


Atlas2D::Frame* Atlas2D::GetFrameInfo(int32 _index)
{
	if(_index >= 0 && _index < frames.size())
	{
		return &frames[_index];
	}

	return NIL;
}



bool Atlas2D::IsFrameSizeAligned(void)const
{
	return alignedFrameSize;
}


MagicList<Texture2D>& Atlas2D::GetTextures(void)
{
	return textures;
}


int32 Atlas2D::GetAmountOfFrames(void)const
{
	return frames.size();
}


bool Atlas2D::IsExist(void)const
{
	return textures.IsNotEmpty() && !frames.empty();
}

 
bool Atlas2D::LoadImagesFromFolder(StringANSI _path)
{
	Destroy();

	if(!File::_IsExist(_path))
	{
		AbstractObject::_WriteToLogFile(File::_GetName(_path), "Atlas2D::LoadImagesFromFolder: can't find '%s'", _path.c_str());
		return false;
	}
	int32 index = 1;
	SimpleList<PixelImage> frameList;
	SimpleList<StringANSI> nameList;
	StringANSI pathToFile;
	Vector2 frameSize;
	alignedFrameSize = true;

	while(1)
	{
		if(!File::_GeneratePathToFile(_path, "png", index, 1, Atlas2D::maxFrames, pathToFile))
		{
			AbstractObject::_WriteToLogFile(File::_GetName(_path), "Atlas2D::LoadImagesFromFolder: File::_GeneratePathToFile is failed");
			break;
		}

		if(!File::_IsExist(pathToFile))
		{
			break;
		}
		frameList.AddToTail(PixelImage());
		PixelImage& image = frameList.GetTail()->key;
		image.LoadImageFromPNGFile(pathToFile);
			
		if(image.IsExist())
		{
			nameList.AddToTail(File::_GetBaseName(pathToFile));

			if(index == 1)
			{ 
				frameSize = image.GetSize();
			}  
			else
			{
				if(image.GetSize() != frameSize)
				{
					alignedFrameSize = false;
				}
			}
		}
		else
		{
			frameList.RemoveTail();
			AbstractObject::_WriteToLogFile(File::_GetName(_path), "Atlas2D::LoadImagesFromFolder: PixelImage.LoadImageFromPNGFile is failed");
			break;
		}
			
		index++;
	}

	if(frameList.IsEmpty())
	{  
		return false; 
	} 

	int32 textureMaxSize = Texture2D::_GetTextureMaxSize();

	if(alignedFrameSize)
	{
		int32 maxFramesInRow = textureMaxSize / frameSize.x;
		int32 maxFramesInColumn = textureMaxSize / frameSize.y;
		int32 totalFrames = maxFramesInRow * maxFramesInColumn;

		if(totalFrames > 0)
		{
			CreateTexturesForAnimation(frameSize, maxFramesInRow, maxFramesInColumn, frameList, nameList);
			
			if(IsReceiversExist(INIT_ATLAS_MESSAGE)) { SendMessage(INIT_ATLAS_MESSAGE); }
			return true;  
		}
		else
		{
			AbstractObject::_WriteToLogFile(File::_GetName(_path), "Atlas2D::LoadImagesFromFolder: PixelImage.Size is too much");
		}
	}
	else
	{
	}
	return false;
} 


void Atlas2D::SetFilteringMode(int32 _mode)
{
	filteringMode = _mode;
	for(int32 i = 0; i < frames.size(); ++i)
	{
		frames[i].texture->SetFilteringMode(filteringMode);
	}
}


int32 Atlas2D::GetFilteringMode(void)
{
	return filteringMode;
}


void Atlas2D::RenameFrame(int32 _index, StringANSI _name)
{
	if(_index >= 0 && _index < frames.size())
	{
		frames[_index].name = _name;
	}
}


void Atlas2D::BindHitbox(int32 _index, Hitbox* _hitbox)
{
	if(_hitbox)
	{
		if(_index >= 0 && _index < frames.size())
		{
			frames[_index].BindHitbox(_hitbox);

			if(IsReceiversExist(PolygonalSurface::CHANGE_HITBOX_MESSAGE)) { SendPackage(PolygonalSurface::CHANGE_HITBOX_MESSAGE, _hitbox->GetName()); }
		}
	}
}

			 
Hitbox* Atlas2D::GetHitbox(int32 _index)const
{
	if(_index >= 0 && _index < frames.size())
	{
		return frames[_index].GetHitbox();
	}
	return NIL;
}



void Atlas2D::CreateTexturesForAnimation(Vector2 _frameSize, int32 _maxFramesInRow, int32 _maxFramesInColumn, SimpleList<PixelImage>& _frameList, SimpleList<StringANSI>& _nameList)
{
	if(_frameList.GetSize() <= (_maxFramesInRow * _maxFramesInColumn))
	{
		if(_frameList.GetSize() <= _maxFramesInRow)
		{
			Texture2D* texture = new Texture2D();
			texture->SetFilteringMode(filteringMode);
			texture->CreateQuadTextureMap(Vector2(_frameSize.x * _frameList.GetSize(), _frameSize.y), RGBA());
			textures.AddToTail(texture); 
			float normalizedWidth = _frameSize.x / texture->GetTextureSize().x;
			float normalizedHeight = _frameSize.y / texture->GetTextureSize().y;
			float offsetX = 0.0f, offsetY = 0.0f;
			Vector2 framePosition;
			for(int32 i = 0, j = _frameList.GetSize(); i < j; i++)
			{
				framePosition = Vector2(i * _frameSize.x, 0);
				texture->SetImage(framePosition, _frameList.GetHead()->key);

				Atlas2D::Frame frame;
				frame.normalizedPosition = Vector2(offsetX, offsetY);
				frame.normalizedSize = Vector2(normalizedWidth, normalizedHeight);
				frame.positionInPixels = framePosition;
				frame.sizeInPixels = _frameSize;
				frame.name = _nameList.GetHead()->key;
				frame.texture = texture;

				frames.push_back(frame);
				offsetX += normalizedWidth;
								
				_frameList.RemoveHead();
				_nameList.RemoveHead();
			}
		}
		else if(_frameList.GetSize() <= _maxFramesInColumn)
		{
			Texture2D* texture = new Texture2D();
			texture->SetFilteringMode(filteringMode);
			texture->CreateQuadTextureMap(Vector2(_frameSize.x, _frameSize.y * _frameList.GetSize()), RGBA());
			textures.AddToTail(texture); 
			float normalizedWidth = _frameSize.x / texture->GetTextureSize().x;
			float normalizedHeight = _frameSize.y / texture->GetTextureSize().y;
			float offsetX = 0.0f, offsetY = 0.0f;
			Vector2 framePosition;
			for(int32 i = 0, j = _frameList.GetSize(); i < j; i++)
			{
				framePosition = Vector2(0, i * _frameSize.y);
				texture->SetImage(framePosition, _frameList.GetHead()->key);

				Atlas2D::Frame frame;
				frame.normalizedPosition = Vector2(offsetX, offsetY);
				frame.normalizedSize = Vector2(normalizedWidth, normalizedHeight);
				frame.positionInPixels = framePosition;
				frame.sizeInPixels = _frameSize;
				frame.name = _nameList.GetHead()->key;
				frame.texture = texture;

				frames.push_back(frame);
				offsetY += normalizedHeight;
											
				_frameList.RemoveHead();
				_nameList.RemoveHead();
			}
		}
		else
		{ 
			int32 framesInRow = 0;
			int32 framesInColumn = 2;
			while(1)
			{
				framesInRow = _frameList.GetSize() / framesInColumn;

				if(framesInRow <= _maxFramesInRow)
				{
					Texture2D* texture = new Texture2D();
					texture->SetFilteringMode(filteringMode);
					texture->CreateQuadTextureMap(Vector2(framesInRow * _frameSize.x, framesInColumn * _frameSize.y), RGBA());
					textures.AddToTail(texture); 
					float normalizedWidth = _frameSize.x / texture->GetTextureSize().x;
					float normalizedHeight = _frameSize.y / texture->GetTextureSize().y;
					float offsetX = 0.0f, offsetY = 0.0f;
					Vector2 framePosition;
					for(int32 y = 0; y < framesInColumn; y++)
					{
						for(int32 x = 0; x < framesInRow; x++)
						{
							framePosition = Vector2(x * _frameSize.x, y * _frameSize.y);
							texture->SetImage(framePosition, _frameList.GetHead()->key);

							Atlas2D::Frame frame;
							frame.normalizedPosition = Vector2(offsetX, offsetY);
							frame.normalizedSize = Vector2(normalizedWidth, normalizedHeight);
							frame.positionInPixels = framePosition;
							frame.sizeInPixels = _frameSize;
							frame.name = _nameList.GetHead()->key;
							frame.texture = texture;

							frames.push_back(frame);
							offsetX += normalizedWidth;
							
							_frameList.RemoveHead();
							_nameList.RemoveHead();
						}
						offsetX = 0;
						offsetY += normalizedHeight;
					}
					break;
				}
				else
				{
					framesInColumn++;
				}
			} 
			if(_frameList.GetSize() > 0) { CreateTexturesForAnimation(_frameSize, _maxFramesInRow, _maxFramesInColumn, _frameList, _nameList); }
		} 
	}
	else
	{
		Texture2D* texture = new Texture2D();
		texture->SetFilteringMode(filteringMode);
		texture->CreateQuadTextureMap(Vector2(_maxFramesInRow * _frameSize.x, _maxFramesInColumn * _frameSize.y), RGBA());
		textures.AddToTail(texture); 
		float normalizedWidth = _frameSize.x / texture->GetTextureSize().x;
		float normalizedHeight = _frameSize.y / texture->GetTextureSize().y;
		float offsetX = 0.0f, offsetY = 0.0f;
		Vector2 framePosition;
		for(int32 y = 0; y < _maxFramesInColumn; y++)
		{
			for(int32 x = 0; x < _maxFramesInRow; x++)
			{
				framePosition = Vector2(x * _frameSize.x, y * _frameSize.y);
				texture->SetImage(framePosition, _frameList.GetHead()->key);
				 
				Atlas2D::Frame frame;
				frame.normalizedPosition = Vector2(offsetX, offsetY);
				frame.normalizedSize = Vector2(normalizedWidth, normalizedHeight);
				frame.positionInPixels = framePosition;
				frame.sizeInPixels = _frameSize;
				frame.name = _nameList.GetHead()->key;
				frame.texture = texture;

				frames.push_back(frame);
				offsetX += normalizedWidth;
							
				_frameList.RemoveHead();
				_nameList.RemoveHead();
			}
			offsetX = 0;
			offsetY += normalizedHeight;
		}
		CreateTexturesForAnimation(_frameSize, _maxFramesInRow, _maxFramesInColumn, _frameList, _nameList);
	}
}


void Atlas2D::_SetMaxFrames(int32 _value)
{
	if(_value > 0)
	{
		Atlas2D::maxFrames = _value;
	}
}


int32 Atlas2D::_GetMaxFrames(void)
{
	return Atlas2D::maxFrames;
} 



void Atlas2D::_SetFileSuffix(StringANSI _suffix)
{
	atlasesSuffix = _suffix;
}


StringANSI Atlas2D::_GetFileSuffix(void)
{
	return atlasesSuffix;
}



bool Atlas2D::SaveToFile(StringANSI _path)
{ 
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + atlasesDir + GetName() + "." + atlasesSuffix;
	}

	File file(_path, File::REWRITE);

	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool Atlas2D::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(filteringMode);
		_file.Write(textures.GetSize());
		for(int32 i = 0; i < textures.GetSize(); ++i)
		{
			PixelImage image;

			if(textures[i]->GetImage(image))
			{
				image.SaveImageToPNGFile(_file);
			}
		}
		_file.Write(frames.size());
		_file.Write(alignedFrameSize);
		for(int32 i = 0; i < frames.size(); ++i)
		{
			frames[i].SaveToFile(_file);
			_file.Write(textures.GetIndexOf(&frames[i].GetTexture()));
		}
		return true;
	}
	return false;
}




bool Atlas2D::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + Atlas2D::_GetDir() + _name + "." + Atlas2D::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Atlas2D::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(filteringMode);
		_file.Write(textures.GetSize());
		for(int32 i = 0; i < textures.GetSize(); ++i)
		{
			PixelImage image;

			if(textures[i]->GetImage(image))
			{
				image.SaveImageToPNGFile(_file);
			}
		}
		_file.Write(frames.size());
		_file.Write(alignedFrameSize);
		for(int32 i = 0; i < frames.size(); ++i)
		{
			frames[i].SaveToFile(_file);
			_file.Write(textures.GetIndexOf(&frames[i].GetTexture()));
		}
		return true;
	}
	return false;
}


bool Atlas2D::LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{  
		_path = AssetLibrary::_GetDir() + atlasesDir + _path + "." + atlasesSuffix;
	}

	File file(_path, File::READ);

	return LoadFromFile(file);
}


bool Atlas2D::LoadFromFile(File& _file)
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
		int32 amountOfTextures;
		_file.Read(amountOfTextures);
		for(int32 i = 0; i < amountOfTextures; ++i)
		{
			PixelImage image;
			image.LoadImageFromPNGFile(_file);
			
			Texture2D* texture = new Texture2D();
			texture->SetFilteringMode(filteringMode);
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, image);
			textures.AddToTail(texture);
		}
		int32 amountOfFrames;  
		_file.Read(amountOfFrames);
		_file.Read(alignedFrameSize);
		for(int32 i = 0; i < amountOfFrames; ++i)
		{
			Atlas2D::Frame frame;
			frame.LoadFromFile(_file);
			int32 indexOfTexture;
			_file.Read(indexOfTexture); 
			frame.texture = textures.Get(indexOfTexture);
			frames.push_back(frame);
		}

		if(IsReceiversExist(INIT_ATLAS_MESSAGE)) { SendMessage(INIT_ATLAS_MESSAGE); }
		return true;
	}
	return false;
}


Atlas2D* Atlas2D::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + atlasesDir + _path + "." + atlasesSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


Atlas2D* Atlas2D::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Atlas2D* atlas = new Atlas2D();

		if(atlas->LoadFromFile(_file))
		{
			return atlas;
		}
		delete atlas;
	}
	return NIL;
}
