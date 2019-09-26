#include "Atlas2D.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



Atlas2D::Frame::Frame(void): texture(NIL)
{}


Atlas2D::Frame::~Frame(void)
{}


Texture2D& Atlas2D::Frame::GetTexture(void)
{
	return *texture;
}


Vector2 Atlas2D::Frame::GetNormalizedPosition(void)
{
	return normalizedPosition;
}


Vector2 Atlas2D::Frame::GetNormalizedSize(void)
{
	return normalizedSize;
}

				
Vector2 Atlas2D::Frame::GetPositionInPixels(void)
{
	return positionInPixels;
}


Vector2 Atlas2D::Frame::GetSizeInPixels(void)
{
	return sizeInPixels;
}


void Atlas2D::Frame::BindHitbox(Hitbox* _hitbox)
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
		}
	}
}


Hitbox* Atlas2D::Frame::GetHitbox(void)const
{
	return hitbox.Get();
}



bool Atlas2D::Frame::SaveToFile(File& _file)
{
	if(AbstractAnimation::Frame::SaveToFile(_file))
	{
		_file.Write(normalizedPosition);
		_file.Write(normalizedSize);
		_file.Write(positionInPixels);
		_file.Write(sizeInPixels);
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
		return true;
	}
	return false;
}


bool Atlas2D::Frame::LoadFromFile(File& _file)
{
	if(AbstractAnimation::Frame::LoadFromFile(_file))
	{
		_file.Read(normalizedPosition);
		_file.Read(normalizedSize);
		_file.Read(positionInPixels);
		_file.Read(sizeInPixels);
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
		return true;
	}
	return false;
}


