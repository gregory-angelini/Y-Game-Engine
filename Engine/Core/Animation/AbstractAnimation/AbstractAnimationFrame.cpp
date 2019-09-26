#include "AbstractAnimation.h"
#include <Engine/Core/Surface/PolygonalSurface/PolygonalSurface.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>


AbstractAnimation::Frame::Frame(void)
{}



AbstractAnimation::Frame::~Frame(void)
{}



void AbstractAnimation::Frame::SetPositionOffset(Vector3 _offset)
{
	positionOffset = _offset;
}


Vector3 AbstractAnimation::Frame::GetPositionOffset(void)
{
	return positionOffset;
}



Hitbox* AbstractAnimation::Frame::GetHitbox(void)const
{
	return hitbox.Get();
}



StringANSI AbstractAnimation::Frame::GetName(void)
{
	return name;
}



void AbstractAnimation::Frame::SetName(StringANSI _name)
{
	name = _name;
}
	

void AbstractAnimation::Frame::BindHitbox(Hitbox* _hitbox)
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


bool AbstractAnimation::Frame::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(name.length());
		_file.WriteString(name);
		_file.Write(positionOffset);
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
		return true;
	}
	return false;
}


bool AbstractAnimation::Frame::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		_file.Read(positionOffset);
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

