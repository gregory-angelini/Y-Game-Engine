#include "Hitbox.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



Hitbox::ShapeInfo::ShapeInfo(void): collisionGroupIndex(0), collisionEnable(false), collisionPolygonsInfoEnable(false)
{}



Hitbox::ShapeInfo::ShapeInfo(Mesh* _mesh, StringANSI _name, int32 _collisionGroupIndex, bool _collisionEnable, bool _collisionPolygonsInfoEnable): name(_name), collisionGroupIndex(_collisionGroupIndex), collisionEnable(_collisionEnable), collisionPolygonsInfoEnable(_collisionPolygonsInfoEnable)
{
	mesh.Attach(_mesh); 
}

	

Mesh* Hitbox::ShapeInfo::GetMesh(void)const
{
	return mesh.Get();
}



StringANSI Hitbox::ShapeInfo::GetName(void)const
{
	return name;
}


int32 Hitbox::ShapeInfo::GetCollisionGroupIndex(void)const
{
	return collisionGroupIndex;
}



bool Hitbox::ShapeInfo::GetCollisionEnable(void)const
{
	return collisionEnable;
}



bool Hitbox::ShapeInfo::GetCollisionPolygonsInfoEnable(void)const
{
	return collisionPolygonsInfoEnable;
}



bool Hitbox::ShapeInfo::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		_file.Write(mesh.IsNotEmpty());
		
		if(mesh.IsNotEmpty())
		{
			bool meshIsCommonAsset = mesh.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(meshIsCommonAsset);

			if(meshIsCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(mesh.Get()->GetName(), AssetLibrary::MESH_ASSET))
				{
					mesh.Get()->SaveToFile();
				}
				_file.Write(mesh.Get()->GetName().length());
				_file.WriteString(mesh.Get()->GetName());
			}
			else
			{
				mesh.Get()->SaveToFile(_file);
			}
		}
		_file.Write(name.length());
		_file.WriteString(name);
		_file.Write(collisionGroupIndex);
		_file.Write(collisionEnable);
		_file.Write(collisionPolygonsInfoEnable);
		return true;
	}
	return false;
}


bool Hitbox::ShapeInfo::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		bool meshIsExist;
		_file.Read(meshIsExist);
				
		if(meshIsExist)
		{
			bool meshIsCommonAsset;
			_file.Read(meshIsCommonAsset);
			Mesh* buildInMesh;
					
			if(meshIsCommonAsset)
			{
				StringANSI name;
				int32 length;
				_file.Read(length);
				_file.ReadString(name, length);

				buildInMesh = dynamic_cast<Mesh*>(AssetLibrary::_LoadCommonAsset<Mesh>(name));
			}
			else
			{
				buildInMesh = dynamic_cast<Mesh*>(AssetLibrary::_LoadPrivateAsset<Mesh>(_file));
			}
			if(buildInMesh) { mesh.Attach(buildInMesh); }
		}
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		_file.Read(collisionGroupIndex);
		_file.Read(collisionEnable);
		_file.Read(collisionPolygonsInfoEnable);
		return true;
	}
	return false;
}

