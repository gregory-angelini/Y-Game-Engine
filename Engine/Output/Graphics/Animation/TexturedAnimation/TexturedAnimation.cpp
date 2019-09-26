#include "TexturedAnimation.h"
#include <Engine/Output/File/File.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


#ifdef SendMessage
   #undef SendMessage
#endif
StringANSI TexturedAnimation::texturedAnimationsSuffix = "t-anim";
StringANSI TexturedAnimation::texturedAnimationsDir = "/animations/t-animations/";


TexturedAnimation::TexturedAnimation(void): currentFrameInfo(NIL), positionOffsetEnable(false), hitboxEnable(false)
{
	AddClassProperty(TEXTURED_ANIMATION_CLASS);
}



TexturedAnimation::~TexturedAnimation(void)
{ 
	Destroy();
	Destroying();
}



Hitbox* TexturedAnimation::GetHitbox(void)const
{
	if(atlas.IsNotEmpty())
	{
		Atlas2D::Frame* frame = atlas.Get()->GetFrameInfo(currentFrame);

		if(frame) { return frame->GetHitbox(); }
	}
	return NIL;
}



bool TexturedAnimation::IsHitboxEnabled(void)const
{
	return hitboxEnable;
}


void TexturedAnimation::SetHitboxEnable(bool _enable)
{
	hitboxEnable = _enable;

	if(IsReceiversExist(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE)) { SendPackage(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE, hitboxEnable); }
}


void TexturedAnimation::Destroy(void)
{
	if(atlas.IsNotEmpty())
	{
		if(IsReceiversExist(DESTROY_TEXTURED_ANIMATION_MESSAGE)) { SendMessage(DESTROY_TEXTURED_ANIMATION_MESSAGE); }
		
		atlas.Get()->Disconnect(Atlas2D::DESTROY_ATLAS_MESSAGE, this);
		atlas.Get()->Disconnect(Atlas2D::INIT_ATLAS_MESSAGE, this);

		atlas.Clear();
		amountOfFrames = 0;
		currentFrameSize = Vector2(0.0f, 0.0f);
		currentFrameInfo = NIL;
	}
}


Atlas2D* TexturedAnimation::GetAtlas(void)
{
	return atlas.Get();
}


bool TexturedAnimation::IsExist(void)const
{
	return atlas.IsNotEmpty() && atlas.Get()->IsExist();
}


Atlas2D::Frame* TexturedAnimation::GetCurrentFrameInfo(void)const
{
	return currentFrameInfo; 
}


void TexturedAnimation::SetCurrentFrame(StringANSI _frameName)
{
	if(atlas.IsNotEmpty())
	{
		SetCurrentFrame(atlas.Get()->GetIndexOfFrame(_frameName)); 
	}
}


StringANSI TexturedAnimation::GetNameOfCurrentFrame(void)const
{
	if(atlas.IsNotEmpty())
	{
		Atlas2D::Frame* frame = atlas.Get()->GetFrameInfo(currentFrame);

		if(frame) { return frame->GetName(); }
	}
	return "";
}


void TexturedAnimation::BindAtlas(Atlas2D* _atlas)
{
	if(atlas.Get() != _atlas)
	{
		if(atlas.IsNotEmpty())
		{
			atlas.Get()->Disconnect(Atlas2D::DESTROY_ATLAS_MESSAGE, this);
			atlas.Get()->Disconnect(Atlas2D::INIT_ATLAS_MESSAGE, this);
			atlas.Get()->Disconnect(PolygonalSurface::CHANGE_HITBOX_MESSAGE, this);
			
			AtlasIsDestroyed();
			atlas.Clear();
		} 

		if(_atlas)
		{ 
			atlas.Attach(_atlas);
			atlas.Get()->Connect(Atlas2D::DESTROY_ATLAS_MESSAGE, this, Caller<>(this, &TexturedAnimation::AtlasIsDestroyed));
			atlas.Get()->Connect(Atlas2D::INIT_ATLAS_MESSAGE, this, Caller<>(this, &TexturedAnimation::AtlasIsCreated));
			atlas.Get()->Connect(PolygonalSurface::CHANGE_HITBOX_MESSAGE, this, Caller<>(this, &TexturedAnimation::HitboxIsChanged));

			if(atlas.Get()->IsExist())
			{
				AtlasIsCreated();
			}	
			if(IsReceiversExist(CHANGE_ATLAS_MESSAGE)) { SendPackage(CHANGE_ATLAS_MESSAGE, atlas.Get()->GetName()); }
		}
	}
}


void TexturedAnimation::HitboxIsChanged(StringANSI _name)
{
	if(IsReceiversExist(PolygonalSurface::CHANGE_HITBOX_MESSAGE)) { SendPackage(PolygonalSurface::CHANGE_HITBOX_MESSAGE, _name); }
}


void TexturedAnimation::AtlasIsCreated(void)
{ 

	amountOfFrames = atlas.Get()->GetAmountOfFrames();
	currentFrameSize = atlas.Get()->GetFrameInfo(0)->sizeInPixels;
	SetCurrentFrame(Numerical<int32>::_GetMin(currentFrame, amountOfFrames - 1));
 
	if(IsReceiversExist(INIT_TEXTURED_ANIMATION_MESSAGE)) { SendMessage(INIT_TEXTURED_ANIMATION_MESSAGE); }
}


void TexturedAnimation::AtlasIsDestroyed(void)
{ 

	if(IsReceiversExist(DESTROY_TEXTURED_ANIMATION_MESSAGE)) { SendMessage(DESTROY_TEXTURED_ANIMATION_MESSAGE); }
	amountOfFrames = 0;
	currentFrameSize = Vector2(0.0f, 0.0f);
	currentFrameInfo = NIL;
}


Vector2 TexturedAnimation::GetCurrentFrameSize(void)const
{
	return currentFrameSize;
}


void TexturedAnimation::CurrentFrameIsChanged(void)
{
	if(atlas.IsNotEmpty())
	{
		if(currentFrameInfo = atlas.Get()->GetFrameInfo(currentFrame))
		{
			if(!atlas.Get()->IsFrameSizeAligned())
			{
				currentFrameSize = currentFrameInfo->sizeInPixels;
			} 
		}
		AbstractAnimation::CurrentFrameIsChanged();
	}
}


bool TexturedAnimation::IsPositionOffsetEnabled(void)const
{
	return positionOffsetEnable;
}


void TexturedAnimation::SetPositionOffsetEnable(bool _enable)
{
	if(positionOffsetEnable == _enable) { return; }
	positionOffsetEnable = _enable;

	if(IsReceiversExist(ENABLE_POSITION_OFFSET_MESSAGE)) { SendPackage(ENABLE_POSITION_OFFSET_MESSAGE, positionOffsetEnable); }
}



StringANSI TexturedAnimation::_GetDir(void)
{
	return texturedAnimationsDir;
}


void TexturedAnimation::_SetFileSuffix(StringANSI _suffix)
{
	texturedAnimationsSuffix = _suffix;
}


StringANSI TexturedAnimation::_GetFileSuffix(void)
{
	return texturedAnimationsSuffix;
}


bool TexturedAnimation::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + texturedAnimationsDir + GetName() + "." + texturedAnimationsSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool TexturedAnimation::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());

		AbstractAnimation::SaveDerivedToFile(_file);
		_file.Write(positionOffsetEnable);
		_file.Write(atlas.IsNotEmpty());
				
		if(atlas.IsNotEmpty())
		{
			bool atlasIsCommonAsset = atlas.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(atlasIsCommonAsset);

			if(atlasIsCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(atlas.Get()->GetName(), AssetLibrary::ATLAS_2D_ASSET))
				{
					atlas.Get()->SaveToFile();
				}
				_file.Write(atlas.Get()->GetName().length());
				_file.WriteString(atlas.Get()->GetName());
			}
			else
			{
				atlas.Get()->SaveToFile(_file);
			}
		}
		_file.Write(hitboxEnable);
		return true;
	}
	return false;
}



bool TexturedAnimation::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + TexturedAnimation::_GetDir() + _name + "." + TexturedAnimation::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool TexturedAnimation::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);

		AbstractAnimation::SaveDerivedToFile(_file);
		_file.Write(positionOffsetEnable);
		_file.Write(atlas.IsNotEmpty());
				
		if(atlas.IsNotEmpty())
		{
			bool atlasIsCommonAsset = atlas.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(atlasIsCommonAsset);

			if(atlasIsCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(atlas.Get()->GetName(), AssetLibrary::ATLAS_2D_ASSET))
				{
					atlas.Get()->SaveToFile();
				}
				_file.Write(atlas.Get()->GetName().length());
				_file.WriteString(atlas.Get()->GetName());
			}
			else
			{
				atlas.Get()->SaveToFile(_file);
			}
		}
		_file.Write(hitboxEnable);
		return true;
	}
	return false;
}



bool TexturedAnimation::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + texturedAnimationsDir + _path + "." + texturedAnimationsSuffix;
	}
	
	File file(_path, File::READ);
	
	return LoadFromFile(file);
}


bool TexturedAnimation::LoadFromFile(File& _file)
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

		AbstractAnimation::LoadDerivedFromFile(_file);
		bool positionOffsetIsEnabled;
		_file.Read(positionOffsetIsEnabled);
		SetPositionOffsetEnable(positionOffsetIsEnabled);
		bool atlasIsExist;
		_file.Read(atlasIsExist);
				
		if(atlasIsExist)
		{
			bool atlasIsCommonAsset;
			_file.Read(atlasIsCommonAsset);
			Atlas2D* buildInAtlas;
					
			if(atlasIsCommonAsset)
			{
				StringANSI name;
				int32 length;
				_file.Read(length);
				_file.ReadString(name, length);

				buildInAtlas = dynamic_cast<Atlas2D*>(AssetLibrary::_LoadCommonAsset<Atlas2D>(name));
			}
			else
			{
				buildInAtlas = dynamic_cast<Atlas2D*>(AssetLibrary::_LoadPrivateAsset<Atlas2D>(_file));
			}
			if(buildInAtlas) { BindAtlas(buildInAtlas); }
		}
		bool hitboxIsEnabled;
		_file.Read(hitboxIsEnabled);
		SetHitboxEnable(hitboxIsEnabled);
		return true; 
	}
	return false;
}



TexturedAnimation* TexturedAnimation::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + texturedAnimationsDir + _path + "." + texturedAnimationsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


TexturedAnimation* TexturedAnimation::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		TexturedAnimation* animation = new TexturedAnimation();

		if(animation->LoadFromFile(_file))
		{
			return animation;
		}
		delete animation;
	}
	return NIL;
}
