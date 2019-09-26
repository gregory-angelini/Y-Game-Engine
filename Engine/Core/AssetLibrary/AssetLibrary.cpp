#include "AssetLibrary.h"
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Texture/Texture2D/Texture2D.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/Time/Timer/Timer.h>
StringANSI AssetLibrary::assetsDir = "assets/";
AssetScene AssetLibrary::assets;



AssetLibrary::AssetLibrary(void)
{}



AssetScene& AssetLibrary::_GetAssets(void)
{
	return assets;
}



AbstractObject* AssetLibrary::_LoadCommonAsset(File& _file, int32 _type)
{ 

	AbstractObject* asset = NIL;

	switch(_type)
	{
		case AssetLibrary::TEXTURE_2D_ASSET:
		{ 
			asset = Texture2D::_LoadFromFile(_file);
			break;
		}
					
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			asset = Atlas2D::_LoadFromFile(_file);
			break;
		}
					
		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			asset = TexturedAnimation::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::OBJECT_ASSET:
		{
			asset = AbstractObject::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			asset = PlayerControl::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::MESH_ASSET:
		{
			asset = Mesh::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::HITBOX_ASSET:
		{
			asset = Hitbox::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::SCENE_ASSET:
		{
			asset = ObjectScene::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::TRIGGER_ASSET:
		{
			asset = Trigger::_LoadFromFile(_file);
			break;
		}
		 
		case AssetLibrary::VARIABLE_ASSET:
		{
			asset = AbstractVariable::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::LIST_ASSET:
		{
			asset = AbstractList::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::EVENT_ASSET:
		{
			asset = AbstractEvent::_LoadFromFile(_file);
			break;
		}
 
		case AssetLibrary::ACTION_ASSET:
		{
			asset = AbstractAction::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			asset = FloatLerp::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			asset = QuaternionLerp::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			asset = VectorLerp::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::CAMERA_ASSET:
		{
			asset = Camera3D::_LoadFromFile(_file);
			break;
		}

		case AssetLibrary::TIMER_ASSET:
		{
			asset = Timer::_LoadFromFile(_file);
			break;
		}
	} 

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



AbstractObject* AssetLibrary::_LoadCommonAsset(StringANSI _name, int32 _type)
{
	AbstractObject* asset = assets.FindObject(_name);

	if(asset)
	{
		return asset;
	}

	if(_type == UNKNOWN_ASSET)
	{
		return NIL;
	}

	switch(_type)
	{
		case AssetLibrary::TEXTURE_2D_ASSET:
		{
			asset = Texture2D::_LoadFromFile(_name);
			break;
		}
					
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			asset = Atlas2D::_LoadFromFile(_name);
			break;
		}
					
		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			asset = TexturedAnimation::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::OBJECT_ASSET:
		{
			asset = AbstractObject::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			asset = PlayerControl::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::MESH_ASSET:
		{
			asset = Mesh::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::HITBOX_ASSET:
		{
			asset = Hitbox::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::SCENE_ASSET:
		{
			asset = ObjectScene::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::TRIGGER_ASSET:
		{
			asset = Trigger::_LoadFromFile(_name);
			break;
		}
		 
		case AssetLibrary::VARIABLE_ASSET:
		{
			asset = AbstractVariable::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::LIST_ASSET:
		{
			asset = AbstractList::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::EVENT_ASSET:
		{
			asset = AbstractEvent::_LoadFromFile(_name);
			break;
		}
	
		case AssetLibrary::ACTION_ASSET:
		{
			asset = AbstractAction::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			asset = FloatLerp::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			asset = QuaternionLerp::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			asset = VectorLerp::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::CAMERA_ASSET:
		{
			asset = Camera3D::_LoadFromFile(_name);
			break;
		}

		case AssetLibrary::TIMER_ASSET:
		{
			asset = Timer::_LoadFromFile(_name);
			break;
		}
	} 

	if(asset)
	{
		assets.AddObject(asset);
	}
	return asset;
}


AbstractObject* AssetLibrary::_LoadPrivateAsset(File& _file, int32 _type)
{ 
	if(_type == UNKNOWN_ASSET)
	{
		return NIL;
	}

	switch(_type)
	{
		case AssetLibrary::TEXTURE_2D_ASSET:
		{
			return Texture2D::_LoadFromFile(_file);
		}
				
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			return Atlas2D::_LoadFromFile(_file);
		}
				
		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			return TexturedAnimation::_LoadFromFile(_file);
		}

		case AssetLibrary::OBJECT_ASSET:
		{
			return AbstractObject::_LoadFromFile(_file);
		}

		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			return PlayerControl::_LoadFromFile(_file);
		}

		case AssetLibrary::MESH_ASSET:
		{
			return Mesh::_LoadFromFile(_file);
		}

		case AssetLibrary::HITBOX_ASSET:
		{
			return Hitbox::_LoadFromFile(_file);
		}

		case AssetLibrary::SCENE_ASSET:
		{
			return ObjectScene::_LoadFromFile(_file);
		}

		case AssetLibrary::TRIGGER_ASSET:
		{
			return Trigger::_LoadFromFile(_file);
		}

		case AssetLibrary::VARIABLE_ASSET:
		{
			return AbstractVariable::_LoadFromFile(_file);
		}

		case AssetLibrary::LIST_ASSET:
		{
			return AbstractList::_LoadFromFile(_file);
		}

		case AssetLibrary::EVENT_ASSET:
		{
			return AbstractEvent::_LoadFromFile(_file);
		}

		case AssetLibrary::ACTION_ASSET:
		{
			return AbstractAction::_LoadFromFile(_file);
		}

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			return FloatLerp::_LoadFromFile(_file);
		}

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			return QuaternionLerp::_LoadFromFile(_file);
		}

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			return VectorLerp::_LoadFromFile(_file);
		}

		case AssetLibrary::CAMERA_ASSET:
		{
			return Camera3D::_LoadFromFile(_file);
		}

		case AssetLibrary::TIMER_ASSET:
		{
			return Timer::_LoadFromFile(_file);
		}
	}
	return NIL;
}




AbstractObject* AssetLibrary::_LoadPrivateAsset(StringANSI _name, int32 _type)
{
	switch(_type)
	{
		case AssetLibrary::TEXTURE_2D_ASSET:
		{
			return Texture2D::_LoadFromFile(_name);
		}
				
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			return Atlas2D::_LoadFromFile(_name);
		}
				
		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			return TexturedAnimation::_LoadFromFile(_name);
		}

		case AssetLibrary::OBJECT_ASSET:
		{
			return AbstractObject::_LoadFromFile(_name);
		}

		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			return PlayerControl::_LoadFromFile(_name);
		}

		case AssetLibrary::MESH_ASSET:
		{
			return Mesh::_LoadFromFile(_name);
		}

		case AssetLibrary::HITBOX_ASSET:
		{
			return Hitbox::_LoadFromFile(_name);
		}

		case AssetLibrary::SCENE_ASSET:
		{
			return ObjectScene::_LoadFromFile(_name);
		}

		case AssetLibrary::TRIGGER_ASSET:
		{
			return Trigger::_LoadFromFile(_name);
		}

		case AssetLibrary::VARIABLE_ASSET:
		{
			return AbstractVariable::_LoadFromFile(_name);
		}

		case AssetLibrary::LIST_ASSET:
		{
			return AbstractList::_LoadFromFile(_name);
		}

		case AssetLibrary::EVENT_ASSET:
		{
			return AbstractEvent::_LoadFromFile(_name);
		}

		case AssetLibrary::ACTION_ASSET:
		{
			return AbstractAction::_LoadFromFile(_name);
		}

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			return FloatLerp::_LoadFromFile(_name);
		}

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			return QuaternionLerp::_LoadFromFile(_name);
		}

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			return VectorLerp::_LoadFromFile(_name);
		}

		case AssetLibrary::CAMERA_ASSET:
		{
			return Camera3D::_LoadFromFile(_name);
		}

		case AssetLibrary::TIMER_ASSET:
		{
			return Timer::_LoadFromFile(_name);
		}
	}
	return NIL;
}



StringANSI AssetLibrary::_GetFileSuffixOfAssetType(int32 _type)
{
	switch(_type)
	{
		case AssetLibrary::TEXTURE_2D_ASSET:
		{
			return Texture2D::_GetFileSuffix();
		}
				
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			return Atlas2D::_GetFileSuffix();
		}
				
		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			return TexturedAnimation::_GetFileSuffix();
		}
		
		case AssetLibrary::OBJECT_ASSET:
		{
			return AbstractObject::_GetFileSuffix();
		}
			
		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			return PlayerControl::_GetFileSuffix();
		}

		case AssetLibrary::MESH_ASSET:
		{
			return Mesh::_GetFileSuffix();
		}

		case AssetLibrary::HITBOX_ASSET:
		{
			return Hitbox::_GetFileSuffix();
		}

		case AssetLibrary::SCENE_ASSET:
		{
			return ObjectScene::_GetFileSuffix();
		}

		case AssetLibrary::TRIGGER_ASSET:
		{
			return Trigger::_GetFileSuffix();
		}
				
		case AssetLibrary::VARIABLE_ASSET: 
		{
			return AbstractVariable::_GetFileSuffix();
		}

		case AssetLibrary::LIST_ASSET:
		{
			return AbstractList::_GetFileSuffix();
		}

		case AssetLibrary::EVENT_ASSET:
		{
			return AbstractEvent::_GetFileSuffix();
		}

		case AssetLibrary::ACTION_ASSET:
		{
			return AbstractAction::_GetFileSuffix();
		}

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			return FloatLerp::_GetFileSuffix();
		}

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			return QuaternionLerp::_GetFileSuffix();
		}

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			return VectorLerp::_GetFileSuffix();
		}

		case AssetLibrary::CAMERA_ASSET:
		{
			return Camera3D::_GetFileSuffix();
		}

		case AssetLibrary::TIMER_ASSET:
		{
			return Timer::_GetFileSuffix();
		}
	}
	return "";
}



bool AssetLibrary::_IsAssetNameCollision(StringANSI _name)
{
	int32 count = 0;

	if(_IsAssetExist(_name, TEXTURE_2D_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, ATLAS_2D_ASSET))
	{
		count++;
	}

	if(_IsAssetExist(_name, TEXTURED_ANIMATION_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, TRIGGER_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, EVENT_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, ACTION_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, VARIABLE_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, LIST_ASSET))
	{
		count++;
	}

	if(_IsAssetExist(_name, OBJECT_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, PLAYER_CONTROL_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, MESH_ASSET))
	{
		count++;
	}
		
	if(_IsAssetExist(_name, HITBOX_ASSET))
	{
		count++;
	}

	if(_IsAssetExist(_name, SCENE_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, FLOAT_LERP_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, QUATERNION_LERP_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, VECTOR_LERP_ASSET))
	{
		count++;
	}
	
	if(_IsAssetExist(_name, CAMERA_ASSET))
	{
		count++;
	}

	if(_IsAssetExist(_name, TIMER_ASSET))
	{
		count++;
	}

	if(count > 1) 
	{ 
		return true; 
	}
	return false;
}


int32 AssetLibrary::_IsAssetExist(StringANSI _name)
{
	if(_IsAssetExist(_name, TEXTURE_2D_ASSET))
	{
		return TEXTURE_2D_ASSET;
	}
	else if(_IsAssetExist(_name, ATLAS_2D_ASSET))
	{
		return ATLAS_2D_ASSET;
	}
	else if(_IsAssetExist(_name, TEXTURED_ANIMATION_ASSET))
	{
		return TEXTURED_ANIMATION_ASSET;
	}
	else if(_IsAssetExist(_name, TRIGGER_ASSET))
	{
		return TRIGGER_ASSET;
	}
	else if(_IsAssetExist(_name, EVENT_ASSET))
	{
		return EVENT_ASSET;
	}
	else if(_IsAssetExist(_name, ACTION_ASSET))
	{
		return ACTION_ASSET;
	}
	else if(_IsAssetExist(_name, VARIABLE_ASSET))
	{
		return VARIABLE_ASSET;
	}
	else if(_IsAssetExist(_name, LIST_ASSET))
	{
		return LIST_ASSET;
	}
	else if(_IsAssetExist(_name, OBJECT_ASSET))
	{
		return OBJECT_ASSET;
	}
	else if(_IsAssetExist(_name, PLAYER_CONTROL_ASSET))
	{
		return PLAYER_CONTROL_ASSET;
	}
	else if(_IsAssetExist(_name, MESH_ASSET))
	{
		return MESH_ASSET;
	}
	else if(_IsAssetExist(_name, HITBOX_ASSET))
	{
		return HITBOX_ASSET;
	}
	else if(_IsAssetExist(_name, SCENE_ASSET))
	{
		return SCENE_ASSET;
	}
	else if(_IsAssetExist(_name, FLOAT_LERP_ASSET))
	{
		return FLOAT_LERP_ASSET;
	}
	else if(_IsAssetExist(_name, QUATERNION_LERP_ASSET))
	{
		return QUATERNION_LERP_ASSET;
	}
	else if(_IsAssetExist(_name, VECTOR_LERP_ASSET))
	{
		return VECTOR_LERP_ASSET;
	}
	else if(_IsAssetExist(_name, CAMERA_ASSET))
	{
		return CAMERA_ASSET;
	}
	else if(_IsAssetExist(_name, TIMER_ASSET))
	{
		return TIMER_ASSET;
	}
	return UNKNOWN_ASSET;
}




int32 AssetLibrary::_GetAssetType(int64 _properties)
{
	if(_properties & Texture2D::TEXTURE_2D_CLASS)
	{
		return TEXTURE_2D_ASSET;
	}		
	else if(_properties & Atlas2D::ATLAS_2D_CLASS)
	{
		return ATLAS_2D_ASSET;
	}
	else if(_properties & TexturedAnimation::TEXTURED_ANIMATION_CLASS)
	{
		return TEXTURED_ANIMATION_ASSET;
	}
	else if(_properties & PlayerControl::PLAYER_CONTROL_CLASS)
	{
		return PLAYER_CONTROL_ASSET;
	}
	else if(_properties & Mesh::MESH_CLASS)
	{
		return MESH_ASSET;
	}
	else if(_properties & Hitbox::HITBOX_CLASS)
	{
		return HITBOX_ASSET;
	}
	else if(_properties & ObjectScene::OBJECT_SCENE_CLASS)
	{
		return SCENE_ASSET;
	}
	else if(_properties & Trigger::TRIGGER_CLASS)
	{
		return TRIGGER_ASSET;
	}
	else if(_properties & AbstractEvent::EVENT_CLASS)
	{
		return EVENT_ASSET;
	}
	else if(_properties & AbstractAction::ACTION_CLASS)
	{
		return ACTION_ASSET;
	}
	else if(_properties & AbstractVariable::ABSTRACT_VARIABLE_CLASS)
	{
		return VARIABLE_ASSET; 
	}
	else if(_properties & AbstractList::ABSTRACT_LIST_CLASS)
	{
		return LIST_ASSET;
	}
	else if(_properties & FloatLerp::FLOAT_LERP_CLASS)
	{
		return FLOAT_LERP_ASSET;
	}
	else if(_properties & QuaternionLerp::QUATERNION_LERP_CLASS)
	{
		return QUATERNION_LERP_ASSET;
	}
	else if(_properties & VectorLerp::VECTOR_LERP_CLASS)
	{
		return VECTOR_LERP_ASSET;
	}
	else if(_properties & Camera3D::CAMERA_3D_CLASS)
	{
		return CAMERA_ASSET;
	}
	else if(_properties & Timer::TIMER_CLASS)
	{
		return TIMER_ASSET;
	}
	else if(_properties & AbstractObject::ABSTRACT_OBJECT_CLASS)
	{
		return OBJECT_ASSET;
	}
	return UNKNOWN_ASSET;
}



int32 AssetLibrary::_GetAssetTypeOfFileSuffix(StringANSI _suffix)
{
	if(_suffix == Texture2D::_GetFileSuffix())
	{
		return AssetLibrary::TEXTURE_2D_ASSET;
	}
	else if(_suffix == Atlas2D::_GetFileSuffix())
	{
		return AssetLibrary::ATLAS_2D_ASSET;
	}
	else if(_suffix == TexturedAnimation::_GetFileSuffix())
	{
		return AssetLibrary::TEXTURED_ANIMATION_ASSET;
	}	
	else if(_suffix == AbstractObject::_GetFileSuffix())
	{
		return AssetLibrary::OBJECT_ASSET;
	}
	else if(_suffix == PlayerControl::_GetFileSuffix())
	{
		return AssetLibrary::PLAYER_CONTROL_ASSET;
	}
	else if(_suffix == Mesh::_GetFileSuffix())
	{
		return AssetLibrary::MESH_ASSET;
	}
	else if(_suffix == Hitbox::_GetFileSuffix())
	{
		return AssetLibrary::HITBOX_ASSET;
	}
	else if(_suffix == ObjectScene::_GetFileSuffix())
	{
		return AssetLibrary::SCENE_ASSET;
	}
	else if(_suffix == Trigger::_GetFileSuffix())
	{
		return AssetLibrary::TRIGGER_ASSET;
	}
	else if(_suffix == AbstractVariable::_GetFileSuffix())
	{
		return AssetLibrary::VARIABLE_ASSET;
	} 
	else if(_suffix == AbstractList::_GetFileSuffix())
	{
		return AssetLibrary::LIST_ASSET;
	}
	else if(_suffix == AbstractEvent::_GetFileSuffix())
	{
		return AssetLibrary::EVENT_ASSET;
	}
	else if(_suffix == AbstractAction::_GetFileSuffix())
	{
		return AssetLibrary::ACTION_ASSET;
	}
	else if(_suffix == FloatLerp::_GetFileSuffix())
	{
		return AssetLibrary::FLOAT_LERP_ASSET;
	}
	else if(_suffix == QuaternionLerp::_GetFileSuffix())
	{
		return AssetLibrary::QUATERNION_LERP_ASSET;
	}
	else if(_suffix == VectorLerp::_GetFileSuffix())
	{
		return AssetLibrary::VECTOR_LERP_ASSET;
	}
	else if(_suffix == Camera3D::_GetFileSuffix())
	{
		return AssetLibrary::CAMERA_ASSET;
	}
	else if(_suffix == Timer::_GetFileSuffix())
	{
		return AssetLibrary::TIMER_ASSET;
	}

	return AssetLibrary::UNKNOWN_ASSET;
}



bool AssetLibrary::_IsAssetExist(StringANSI _name, int32 _type)
{
	if(File::_IsExist(AssetLibrary::_GetDir() + _GetDirOfAssetType(_type) + _name + "." + _GetFileSuffixOfAssetType(_type)))
	{
		return true;
	}
	return false;
}



StringANSI AssetLibrary::_GetDirOfAssetType(int32 _type)
{
	switch(_type)
	{
		case AssetLibrary::TEXTURE_2D_ASSET:
		{
			return Texture2D::_GetDir();
		}
				
		case AssetLibrary::ATLAS_2D_ASSET:
		{
			return Atlas2D::_GetDir();
		}
		
		case AssetLibrary::TEXTURED_ANIMATION_ASSET:
		{
			return TexturedAnimation::_GetDir();
		}
			
		case AssetLibrary::OBJECT_ASSET:
		{
			return AbstractObject::_GetDir();
		} 

		case AssetLibrary::PLAYER_CONTROL_ASSET:
		{
			return PlayerControl::_GetDir();
		}
		
		case AssetLibrary::MESH_ASSET:
		{
			return Mesh::_GetDir();
		}

		case AssetLibrary::HITBOX_ASSET:
		{
			return Hitbox::_GetDir();
		}

		case AssetLibrary::SCENE_ASSET:
		{
			return ObjectScene::_GetDir();
		}

		case AssetLibrary::TRIGGER_ASSET:
		{
			return Trigger::_GetDir();
		} 

		case AssetLibrary::VARIABLE_ASSET:
		{ 
			return AbstractVariable::_GetDir();
		} 
		
		case AssetLibrary::LIST_ASSET:
		{
			return AbstractList::_GetDir();
		} 

		case AssetLibrary::EVENT_ASSET:
		{
			return AbstractEvent::_GetDir();
		} 

		case AssetLibrary::ACTION_ASSET:
		{
			return AbstractAction::_GetDir();
		} 

		case AssetLibrary::FLOAT_LERP_ASSET:
		{
			return FloatLerp::_GetDir();
		} 

		case AssetLibrary::QUATERNION_LERP_ASSET:
		{
			return QuaternionLerp::_GetDir();
		} 

		case AssetLibrary::VECTOR_LERP_ASSET:
		{
			return VectorLerp::_GetDir();
		} 

		case AssetLibrary::CAMERA_ASSET:
		{
			return Camera3D::_GetDir();
		} 

		case AssetLibrary::TIMER_ASSET:
		{
			return Timer::_GetDir();
		} 
	}
	return "";
}



StringANSI AssetLibrary::_GetDir(void)
{
	return assetsDir;
}

