#include "TiledSpace3D.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>




TiledSpace3D::Brush::Brush(void): brushId(-1), isPassed(true)
{}


TiledSpace3D::Brush::Brush(TexturingSurface* _sample, bool _passEnable, int32 _brushId): isPassed(_passEnable), brushId(_brushId)
{
	sample.Attach(_sample);
}


int32 TiledSpace3D::Brush::GetBrushId(void)const
{
	return brushId;
}


bool TiledSpace3D::Brush::IsPassed(void)const
{
	return isPassed;
}


TexturingSurface* TiledSpace3D::Brush::GetSample(void)const
{
	return sample.Get();
}


bool TiledSpace3D::Brush::IsValid(void)const
{
	return sample.Get() != NIL;
}


bool TiledSpace3D::Brush::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(IsValid()); 

		if(IsValid())
		{
			_file.Write(brushId);
			_file.Write(isPassed);
			int32 assetType = AssetLibrary::_GetAssetType(GetSample()->GetClassProperties());
			_file.Write(assetType);
			bool isCommonAsset = GetSample()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(isCommonAsset);
			if(isCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(GetSample()->GetName(), assetType))
				{
					GetSample()->SaveToFile();
				}
				_file.Write(GetSample()->GetName().length());
				_file.WriteString(GetSample()->GetName());
			}
			else
			{
				GetSample()->SaveToFile(_file);
			}
		}
		return true;
	}
	return false;
}



bool TiledSpace3D::Brush::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		bool isValid;
		_file.Read(isValid);

		if(isValid)
		{
			_file.Read(brushId);
			_file.Read(isPassed);
			int32 assetType;
			_file.Read(assetType);
			bool isCommonAsset;
			_file.Read(isCommonAsset);
			if(isCommonAsset)
			{
				StringANSI name;
				int32 length;
				_file.Read(length);
				_file.ReadString(name, length);

				sample.Attach(dynamic_cast<TexturingSurface*>(AssetLibrary::_LoadCommonAsset(name, assetType)));
			}
			else
			{
				sample.Attach(dynamic_cast<TexturingSurface*>(AssetLibrary::_LoadPrivateAsset(_file, assetType)));
			}
		}
		return true;
	}
	return false;
}