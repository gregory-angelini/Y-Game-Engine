#ifndef ASSETLIBRARY_H
#define ASSETLIBRARY_H
#include <Engine/Core/Object/AbstractObject/AbstractObject.h>
#include <Engine/Core/AssetLibrary/AssetScene/AssetScene.h>
#include <Engine/Output/Graphics/Animation/TexturedAnimation/TexturedAnimation.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Core/Parser/List/AbstractList.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
class AssetLibrary
{
	public: enum 
	{
		UNKNOWN_ASSET = 0,
		TEXTURE_2D_ASSET,
		ATLAS_2D_ASSET,
		TEXTURED_ANIMATION_ASSET,
		TRIGGER_ASSET,
		EVENT_ASSET,
		ACTION_ASSET,
		VARIABLE_ASSET,
		LIST_ASSET,
		OBJECT_ASSET,
		PLAYER_CONTROL_ASSET,
		MESH_ASSET,
		HITBOX_ASSET,
		SCENE_ASSET,
		FLOAT_LERP_ASSET,
		QUATERNION_LERP_ASSET,
		VECTOR_LERP_ASSET,
		CAMERA_ASSET,
		TIMER_ASSET
	};
	public: enum
	{
		LOAD_ASSET_MESSAGE = 32,
		CREATE_OBJECT_MESSAGE = 33,
		CHECK_OBJECT_EXIST_MESSAGE = 34,
		SET_PRIVATE_ASSET_MESSAGE = 294
	};
	private: static StringANSI assetsDir;
	private: static AssetScene assets;
	private: AssetLibrary(void);
	public: static AbstractObject* _LoadCommonAsset(StringANSI _name, int32 _type);
	public: static AbstractObject* _LoadCommonAsset(File& _file, int32 _type);
	public: template<typename Type>
	static Type* _LoadCommonAsset(StringANSI _name)
	{
		Type* asset = dynamic_cast<Type*>(assets.FindObject(_name));

		if(asset)
		{
			return asset;
		}
		asset = Type::_LoadFromFile(_name);

		if(asset)
		{
			assets.AddObject(asset);
		}
		return asset;
	}
	public: template<typename Type>
	static AbstractObject* _LoadCommonAsset(File& _file)
	{ 

		AbstractObject* asset = Type::_LoadFromFile(_file);
		
		if(asset)
		{
			if(assets.AddObject(asset))
			{
				return asset;
			}
			StringANSI assetName = asset->GetName();
			delete asset;
			return assets.FindObject(assetName);
		}
		return NIL; 
	}
	public: static AbstractObject* _LoadPrivateAsset(StringANSI _name, int32 _type);
	public: static AbstractObject* _LoadPrivateAsset(File& _file, int32 _type);
	public: template<typename Type>
    static Type* _LoadPrivateAsset(StringANSI _name)
	{
		return Type::_LoadFromFile(_name);
	}
	public: template<typename Type>
	static AbstractObject* _LoadPrivateAsset(File& _file)
	{
		return Type::_LoadFromFile(_file);
	}
	public: static bool _IsAssetExist(StringANSI _name, int32 _type);
	public: static StringANSI _GetFileSuffixOfAssetType(int32 _type);
	public: template<typename Type>
	static StringANSI _GetFileSuffixOfAssetType(void)
	{
		return Type::_GetFileSuffix();
	}
	public: static StringANSI _GetDirOfAssetType(int32 _type);
	public: template<typename Type>
	static StringANSI _GetDirOfAssetType(void)
	{
		return Type::_GetDir();
	}
	public: static int32 _IsAssetExist(StringANSI _name);
    public: static bool _IsAssetNameCollision(StringANSI _name);
	public: static int32 _GetAssetTypeOfFileSuffix(StringANSI _suffix);
	public: static int32 _GetAssetType(int64 _properties);
	public: static AssetScene& _GetAssets(void);
	public: static StringANSI _GetDir(void);
}; 




#endif 