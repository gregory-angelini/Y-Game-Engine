#include "PolygonalSurface.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
MagicBinaryTree<PolygonalSurface::CollisionGroup> PolygonalSurface::collisionGroups;



PolygonalSurface::PolygonalSurface(void): collisionGroupIndex(0), collisionEnable(false), collisionPolygonsInfoEnable(false)
{
	parent.SetRemoveMode(MagicContainer::REMOVE_NODE);

	AddClassProperty(POLYGONAL_SURFACE_CLASS);

	SetScale(Vector3(1.0f, 1.0f, 1.0f));
}


PolygonalSurface& PolygonalSurface::operator = (const PolygonalSurface& _object)
{
	if(this == &_object)
	{
		return *this;
	}

	AbstractSurface::operator = (_object);
	ModelView::operator = (_object);

	BindMesh(_object.GetMesh());
	SetCollisionGroupIndex(_object.GetCollisionGroupIndex());
	SetCollisionEnable(_object.IsCollisionEnabled());
	SetCollisionPolygonsInfoEnable(_object.IsCollisionPolygonsInfoEnabled());
	intersectedPolygons = _object.intersectedPolygons;
	BindHitbox(_object.GetHitbox());

	return *this;
}


PolygonalSurface::~PolygonalSurface(void)
{
	if(collisionGroup.IsNotEmpty() && collisionGroup.Get()->GetSize() == 1 && !collisionGroup.Get()->IsDestroying())
	{ 

		collisionGroup.Get()->RemoveObject(this);
		
		if(collisionGroup.Get()->IsEmpty())
		{
			delete collisionGroup.Get();
		}
	}  

	Destroy();
	Destroying();
}



void PolygonalSurface::Destroy(void)
{
	if(mesh.IsNotEmpty())
	{
		mesh.Clear();
	}

	if(hitbox.IsNotEmpty())
	{
		HitboxIsDestroyed();

		hitbox.Get()->Disconnect(Hitbox::DESTROY_HITBOX_MESSAGE, this);
		hitbox.Get()->Disconnect(Hitbox::INIT_HITBOX_MESSAGE, this);

		hitbox.Clear();
	}

	intersectedPolygons.Clear();
}


void PolygonalSurface::UpdateChildrens(void)
{
	childrens.Clear(MagicContainer::DELETE_NODE);

	if(hitbox.IsNotEmpty())
	{
		MagicList<Hitbox::ShapeInfo>& shapes = hitbox.Get()->GetShapes();
		for(MagicList<Hitbox::ShapeInfo>::AddressDerived* it = shapes.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			if(it->GetNode()->mesh.IsNotEmpty())
			{
				PolygonalSurface* shape = new PolygonalSurface();
				shape->parent.Attach(this);
				shape->Rename(it->GetNode()->name);
				shape->BindMesh(it->GetNode()->mesh.Get());
				shape->mesh.SetRemoveMode(MagicContainer::REMOVE_NODE);
				shape->SetCollisionGroupIndex(it->GetNode()->collisionGroupIndex);
				shape->SetCollisionEnable(it->GetNode()->collisionEnable);
				shape->SetCollisionPolygonsInfoEnable(it->GetNode()->collisionPolygonsInfoEnable);
				childrens.AddToTail(shape);
			}
		}
	}
}


void PolygonalSurface::BindHitbox(Hitbox* _hitbox)
{
	if(hitbox.Get() != _hitbox)
	{
		if(hitbox.IsNotEmpty())
		{
			hitbox.Get()->Disconnect(Hitbox::DESTROY_HITBOX_MESSAGE, this);
			hitbox.Get()->Disconnect(Hitbox::INIT_HITBOX_MESSAGE, this);
			hitbox.Clear();
		}
			
		if(_hitbox)
		{
			hitbox.Attach(_hitbox);
			hitbox.Get()->Connect(Hitbox::DESTROY_HITBOX_MESSAGE, this, Caller<>(this, &PolygonalSurface::HitboxIsDestroyed));
			hitbox.Get()->Connect(Hitbox::INIT_HITBOX_MESSAGE, this, Caller<>(this, &PolygonalSurface::HitboxIsCreated));

			UpdateChildrens();

			if(IsReceiversExist(CHANGE_HITBOX_MESSAGE)) { SendPackage(CHANGE_HITBOX_MESSAGE, hitbox.Get()->GetName()); }
		}
	}
}

	
void PolygonalSurface::HitboxIsCreated(void)
{ 

	UpdateChildrens();
}



void PolygonalSurface::HitboxIsDestroyed(void)
{ 

	childrens.Clear(MagicContainer::DELETE_NODE);
}



Hitbox* PolygonalSurface::GetHitbox(void)const
{
	return hitbox.Get();
}



int32 PolygonalSurface::_GetAmountOfCollisionGroups(void)
{
	return collisionGroups.GetSize();
}


void PolygonalSurface::SetCollisionEnable(bool _enable)
{
	if(collisionEnable != _enable)
	{
		collisionEnable = _enable;

		if(_enable) { _AddToCollisionGroup(this); }
		else { _RemoveFromCollisionGroup(this); }
	}
}


bool PolygonalSurface::IsCollisionEnabled(void)const
{
	return collisionEnable;
}


bool PolygonalSurface::_AddToCollisionGroup(PolygonalSurface* _object)
{
	if(_object && _object->collisionEnable && _object->collisionGroupIndex >= 0)
	{
		if(_object->collisionGroup.IsNotEmpty())
		{
			if(_object->collisionGroup.Get()->GetIndex() == _object->collisionGroupIndex) { return true; }
			if(_object->collisionGroup.Get()->RemoveObject(_object))
			{  
				if(_object->collisionGroup.Get()->IsEmpty()) { delete _object->collisionGroup.Get(); }
			}
		}

		CollisionGroup* it = collisionGroups.Find(_object->collisionGroupIndex, &CollisionGroup::GetIndex);

		if(it == NIL)
		{ 
			it = new CollisionGroup(_object->collisionGroupIndex);
			collisionGroups.Add(it, &CollisionGroup::GetIndex);
		}

		if(it->AddObject(_object))
		{
			_object->collisionGroup.Attach(it);
			return true;
		}
	}
	return false;
}


bool PolygonalSurface::_RemoveFromCollisionGroup(PolygonalSurface* _object)
{
	if(_object && _object->collisionGroup.IsNotEmpty())
	{
		if(_object->collisionGroup.Get()->RemoveObject(_object))
		{
			if(_object->collisionGroup.Get()->IsEmpty()) { delete _object->collisionGroup.Get(); }
			return true;
		} 
	}
	return false;
}

  
PolygonalSurface* PolygonalSurface::GetRoot(void)const
{ 
	return parent.IsNotEmpty() ? parent.Get() : const_cast<PolygonalSurface*>(this); 
}



void PolygonalSurface::CollisionDetectedSingleReaction(PolygonalSurface* _targetObject, Mesh* _sourceMesh, Mesh* _targetMesh)
{
	if(IsReceiversExist(COLLISION_BETWEEN_OBJECTS_MESSAGE)) { SendPackage(COLLISION_BETWEEN_OBJECTS_MESSAGE, &CollisionInfo(_targetObject, _sourceMesh->GetName(), _targetMesh->GetName(), intersectedPolygons, _targetObject->intersectedPolygons)); }
}	



void PolygonalSurface::CollisionDetectedChainReaction(PolygonalSurface* _targetObject, Mesh* _sourceMesh, Mesh* _targetMesh)
{
	PolygonalSurface* root = GetRoot();

	if(root->IsReceiversExist(COLLISION_BETWEEN_OBJECTS_MESSAGE)) { root->SendPackage(COLLISION_BETWEEN_OBJECTS_MESSAGE, &CollisionInfo(_targetObject->GetRoot(), parent.IsNotEmpty() ? GetName() : _sourceMesh->GetName(), _targetObject->parent.IsNotEmpty() ? _targetObject->GetName() : _targetMesh->GetName(), intersectedPolygons, _targetObject->intersectedPolygons)); }
}	




bool PolygonalSurface::_CheckCollision(PolygonalSurface* _object, int32 _groupIndex1, int32 _groupIndex2)
{
	if(_object && _groupIndex1 >= 0 && _groupIndex2 >= 0)
	{
		CollisionGroup* group = collisionGroups.Find(_groupIndex2, &CollisionGroup::GetIndex);

		if(group)
		{
			for(MagicList<PolygonalSurface>::AddressDerived* it1 = _object->childrens.GetHeadAddress(); it1 != NIL; it1 = it1->GetRight())
			{
				PolygonalSurface* sourceObject = it1->GetNode();

				if(sourceObject->hitbox.IsEmpty())
				{
					if(sourceObject->GetCollisionGroupIndex() == _groupIndex1)
					{
						for(CollisionGroup::AddressDerived* it2 = group->GetHeadAddress(); it2 != NIL; it2 = it2->GetRight())
						{
							PolygonalSurface* targetObject = it2->GetNode();

							if(targetObject->hitbox.IsEmpty())
							{
								if(sourceObject->parent.Get() != targetObject->parent.Get())
								{
									if(sourceObject->IsIntersected(targetObject))
									{
										sourceObject->CollisionDetectedChainReaction(targetObject, sourceObject->GetMesh(), targetObject->GetMesh());
										targetObject->CollisionDetectedChainReaction(sourceObject, targetObject->GetMesh(), sourceObject->GetMesh());
										return true;
									} 
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}


bool PolygonalSurface::_CheckCollision(PolygonalSurface* _sourceObject, PolygonalSurface* _targetObject)
{
	if(_sourceObject && _targetObject)
	{
		if(_sourceObject->IsIntersected( _targetObject))
		{
			_sourceObject->CollisionDetectedSingleReaction(_targetObject, _sourceObject->GetMesh(), _targetObject->GetMesh());
			_targetObject->CollisionDetectedSingleReaction(_sourceObject, _targetObject->GetMesh(), _sourceObject->GetMesh());
			return true;
		} 
	}
	return false;
}


void PolygonalSurface::_CheckCollision(int32 _groupIndex1, int32 _groupIndex2)
{
	if(_groupIndex1 >= 0 && _groupIndex2 >= 0)
	{
		CollisionGroup* group1 = collisionGroups.Find(_groupIndex1, &CollisionGroup::GetIndex);

		if(group1)
		{
			CollisionGroup* group2 = _groupIndex1 == _groupIndex2 ? group1 : collisionGroups.Find(_groupIndex2, &CollisionGroup::GetIndex);

			if(group2)
			{
				for(CollisionGroup::AddressDerived* it1 = group1->GetHeadAddress(); it1 != NIL; it1 = it1->GetRight())
				{
					PolygonalSurface* sourceObject = it1->GetNode();

					if(sourceObject->hitbox.IsEmpty())
					{
						for(CollisionGroup::AddressDerived* it2 = group2->GetHeadAddress(); it2 != NIL; it2 = it2->GetRight())
						{
							PolygonalSurface* targetObject = it2->GetNode();

							if(targetObject->hitbox.IsEmpty())
							{
								if(sourceObject->parent.Get() != targetObject->parent.Get())
								{
									if(sourceObject->IsIntersected(targetObject))
									{
										sourceObject->CollisionDetectedChainReaction(targetObject, sourceObject->GetMesh(), targetObject->GetMesh());
										targetObject->CollisionDetectedChainReaction(sourceObject, targetObject->GetMesh(), sourceObject->GetMesh());
									} 
								}
							}
						}
					}
				}
			}
		}
	}
}



void PolygonalSurface::SetCollisionGroupIndex(int32 _groupIndex)
{
	if(_groupIndex >= 0)
	{
		collisionGroupIndex = _groupIndex;

		_AddToCollisionGroup(this);
	} 
}


int32 PolygonalSurface::GetCollisionGroupIndex(void)const
{
	return collisionGroupIndex;
}




bool PolygonalSurface::IsBoundingVolumeIntersected(PolygonalSurface* _object)
{
	if(_object)
	{
		Mesh* sourceMesh = mesh.Get();
		Mesh* targetMesh = _object->mesh.Get();

		if(sourceMesh && targetMesh)
		{
			if(sourceMesh->IsExist() && targetMesh->IsExist())
			{
				if(sourceMesh->GetBoundingVolumeType() == Mesh::BOUNDING_QUAD &&
				   targetMesh->GetBoundingVolumeType() == Mesh::BOUNDING_QUAD)
				{ 
					if(GetTransformedQuad().IsIntersected(_object->GetTransformedQuad()))
					{
						return true;
					}
				}
				else if(sourceMesh->GetBoundingVolumeType() == Mesh::BOUNDING_BOX &&
				        targetMesh->GetBoundingVolumeType() == Mesh::BOUNDING_BOX)
				{
					if(GetTransformedBox().IsIntersected(_object->GetTransformedBox()))
					{
						return true;
					}
				}
				else if(sourceMesh->GetBoundingVolumeType() == Mesh::BOUNDING_SPHERE &&
				        targetMesh->GetBoundingVolumeType() == Mesh::BOUNDING_SPHERE)
				{
					if(GetTransformedSphere().IsIntersected(_object->GetTransformedSphere()))
					{
						return true;
					}
				}
				else if(sourceMesh->GetBoundingVolumeType() == Mesh::BOUNDING_CIRCLE &&
				        targetMesh->GetBoundingVolumeType() == Mesh::BOUNDING_CIRCLE)
				{ 
					if(GetTransformedCircle().IsIntersected(_object->GetTransformedCircle()))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

							


bool PolygonalSurface::IsIntersected(PolygonalSurface* _object)
{
	if(_object)
	{
		if(IsBoundingVolumeIntersected(_object))
		{
			Triangle triangle1, triangle2;
			bool fastMode = true;
			bool intersection = false;
			Mesh* sourceMesh = mesh.Get();
			Mesh* targetMesh = _object->mesh.Get();
			if(collisionPolygonsInfoEnable) { intersectedPolygons.Clear(); fastMode = false; }
			if(_object->collisionPolygonsInfoEnable) { _object->intersectedPolygons.Clear(); fastMode = false; }
			for(int32 i = 0; i < sourceMesh->GetAmountOfPolygons(); ++i)
			{
				if(sourceMesh->GetTriangle(i, triangle1))
				{
					if(intersectionMode & AbstractSurface::APPLY_TRANSFORMATION) { triangle1.Transform(GetTransformation()); }
					for(int32 j = 0; j < targetMesh->GetAmountOfPolygons(); ++j)
					{
						if(targetMesh->GetTriangle(j, triangle2))
						{
							if(intersectionMode & AbstractSurface::APPLY_TRANSFORMATION) { triangle2.Transform(_object->GetTransformation()); }
							if(triangle1.IsIntersected(triangle2))
							{
								if(fastMode)
								{ 
									return true;
								}
								else
								{
									intersection = true;
									if(collisionPolygonsInfoEnable) { intersectedPolygons.AddToTail(i); }  
									if(_object->collisionPolygonsInfoEnable) { _object->intersectedPolygons.AddToTail(j); }
								}
							}
						}
					}
				}
			} 
			return intersection;
		}
		else
		{
			if(collisionPolygonsInfoEnable) { intersectedPolygons.Clear();  }
			if(_object->collisionPolygonsInfoEnable) { _object->intersectedPolygons.Clear(); }
		}
	}
	return false;
}
 

bool PolygonalSurface::IsExist(void)const
{
	return mesh.IsNotEmpty() && mesh.Get()->IsExist();
}






Vector3 PolygonalSurface::GetAbsolutePosition(void)
{
	if(parent.IsNotEmpty())
	{
		return parent.Get()->GetTransformation() * position;
	}
	return position;
}



Quaternion PolygonalSurface::GetAbsoluteRotation(void)
{
	if(parent.IsNotEmpty())
	{
		return parent.Get()->GetAbsoluteRotation() * rotation;
	}
	return rotation;
}



Vector3 PolygonalSurface::GetAbsoluteScale(void)
{
	if(parent.IsNotEmpty())
	{
		return parent.Get()->GetAbsoluteScale() + scale;
	}
	return scale;
}


Matrix4x4 PolygonalSurface::GetTransformation(void)
{
	if(!transformationValid)
	{
		transformation.MakeTransform(GetScale(), GetRotation(), GetPosition());
		 
		if(parent.IsNotEmpty())
		{
			transformation = parent.Get()->GetTransformation() * transformation; 
		}
		SetTransformationValid();
	}
	return transformation;
}


void PolygonalSurface::ResetTransformationValid(void)
{
	ModelView::ResetTransformationValid();
	for(MagicList<PolygonalSurface>::AddressDerived* it = childrens.GetHeadAddress(); it != NIL; it = it->GetRight())
	{
		it->GetNode()->ResetTransformationValid();
	}
}



Box PolygonalSurface::GetTransformedBox(void)
{
	if(mesh.IsNotEmpty() && mesh.Get()->IsExist())
	{
		SimpleList<Vector3> vertices;
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex1());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex2());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex3());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex4());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex5());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex6());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex7());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingBox().GetVertex8());
		float xMin = vertices.GetHead()->key.x;
		float yMin = vertices.GetHead()->key.y;
		float zMin = vertices.GetHead()->key.z;

		float xMax = vertices.GetHead()->key.x;
		float yMax = vertices.GetHead()->key.y;
		float zMax = vertices.GetHead()->key.z;
		for(SimpleList<Vector3>::Node* it = vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			xMin = Numerical<float>::_GetMin(xMin, it->key.x);
			yMin = Numerical<float>::_GetMin(yMin, it->key.y);
			zMin = Numerical<float>::_GetMin(zMin, it->key.z);
			xMax = Numerical<float>::_GetMax(xMax, it->key.x);
			yMax = Numerical<float>::_GetMax(yMax, it->key.y);
			zMax = Numerical<float>::_GetMax(zMax, it->key.z);
		}
		return Box(xMin, yMin, zMin, xMax, yMax, zMax);
	}
	return Box();
}



Circle PolygonalSurface::GetTransformedCircle(void)
{
	Circle circle;

	if(mesh.IsNotEmpty() && mesh.Get()->IsExist())
	{
		circle = mesh.Get()->GetBoundingCircle();
		circle.center = GetTransformation() * circle.center;
		circle.radius *= Numerical<float>::_GetMax(GetScale().x, GetScale().y);
	}
	return circle;
}



Sphere PolygonalSurface::GetTransformedSphere(void)
{
	Sphere sphere;

	if(mesh.IsNotEmpty() && mesh.Get()->IsExist())
	{
		sphere = mesh.Get()->GetBoundingSphere();
		sphere.center = GetTransformation() * sphere.center;
		sphere.radius *= Numerical<float>::_GetMax(GetScale().x, GetScale().y, GetScale().z);
	}
	return sphere;
}


Quad PolygonalSurface::GetTransformedQuad(void)
{
	if(mesh.IsNotEmpty() && mesh.Get()->IsExist())
	{
		SimpleList<Vector3> vertices;
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingQuad().GetVertex1());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingQuad().GetVertex2());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingQuad().GetVertex3());
		vertices.AddToTail(GetTransformation() * mesh.Get()->GetBoundingQuad().GetVertex4());
		float xMin = vertices.GetHead()->key.x;
		float yMin = vertices.GetHead()->key.y;
		float xMax = vertices.GetHead()->key.x;
		float yMax = vertices.GetHead()->key.y;
		float z = (GetTransformation() * mesh.Get()->GetBoundingQuad().GetCenter()).z;
		for(SimpleList<Vector3>::Node* it = vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			xMin = Numerical<float>::_GetMin(xMin, it->key.x);
			yMin = Numerical<float>::_GetMin(yMin, it->key.y);
			xMax = Numerical<float>::_GetMax(xMax, it->key.x);
			yMax = Numerical<float>::_GetMax(yMax, it->key.y);
		}
		return Quad(Vector3(xMin, yMin, z), Vector3(xMax, yMax, z));
	}
	return Quad();
}



bool PolygonalSurface::IsBoundingVolumeIntersected(Line _line)
{
	if(mesh.IsNotEmpty() && mesh.Get()->IsExist())
	{
		if(mesh.Get()->GetBoundingVolumeType() == Mesh::BOUNDING_BOX)
		{
			return GetTransformedBox().IsIntersected(_line);
		}
		else if(mesh.Get()->GetBoundingVolumeType() == Mesh::BOUNDING_QUAD)
		{
			return GetTransformedQuad().IsIntersected(_line);
		}
		else if(mesh.Get()->GetBoundingVolumeType() == Mesh::BOUNDING_SPHERE)
		{
			return GetTransformedSphere().IsIntersected(_line);
		}
		else if(mesh.Get()->GetBoundingVolumeType() == Mesh::BOUNDING_CIRCLE)
		{
			return GetTransformedCircle().IsIntersected(_line);
		}
	} 
	return false;
}
 

int32 PolygonalSurface::IsIntersected(Line _line, Vector3* _intersectionPoint)
{
	if(IsBoundingVolumeIntersected(_line))
	{
		Triangle triangle;
		for(int32 i = 0; i < mesh.Get()->GetAmountOfPolygons(); ++i)
		{
			if(mesh.Get()->GetTriangle(i, triangle))
			{
				if(intersectionMode & AbstractSurface::APPLY_TRANSFORMATION) { triangle.Transform(GetTransformation()); }
				
				if(triangle.IsIntersected(_line, intersectionMode, _intersectionPoint)) { return i; }
			}
		}
	}
	return -1;
}




				
bool PolygonalSurface::IsIntersected(Line _line, int32 _index, Vector3* _intersectionPoint)
{
	if(IsBoundingVolumeIntersected(_line))
	{
		Triangle triangle;

		if(mesh.Get()->GetTriangle(_index, triangle))
		{ 
			if(intersectionMode & AbstractSurface::APPLY_TRANSFORMATION) { triangle.Transform(GetTransformation()); }
			
			return triangle.IsIntersected(_line, intersectionMode, _intersectionPoint); 
		}
	}
	return false;
}



void PolygonalSurface::BindMesh(Mesh* _mesh)
{
	if(mesh.Get() != _mesh)
	{
		if(mesh.IsNotEmpty())
		{ 
	
			mesh.Clear();
		}
			
		if(_mesh)
		{
			mesh.Attach(_mesh);

			if(IsReceiversExist(CHANGE_MESH_MESSAGE)) { SendPackage(CHANGE_MESH_MESSAGE, mesh.Get()->GetName()); }
		}
	}
}	
	


Mesh* PolygonalSurface::GetMesh(void)const
{
	return mesh.Get();
}



SimpleList<int32>& PolygonalSurface::GetIntersectedPolygons(void)
{
	return intersectedPolygons;
}



void PolygonalSurface::SetCollisionPolygonsInfoEnable(bool _enable)
{
	collisionPolygonsInfoEnable = _enable;
}



bool PolygonalSurface::IsCollisionPolygonsInfoEnabled(void)const
{
	return collisionPolygonsInfoEnable;
}



bool PolygonalSurface::SaveToFile(File& _file)
{
	if(AbstractObject::SaveToFile(_file))
	{
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



bool PolygonalSurface::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool PolygonalSurface::SaveAsToFile(File& _file, StringANSI _name)
{
	if(AbstractObject::SaveAsToFile(_file, _name))
	{ 
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



void PolygonalSurface::SaveDerivedToFile(File& _file)
{
	int32 version = 1;
	_file.Write(version);
	_file.Write(GetPosition());
	_file.Write(GetScale());
	_file.Write(GetRotation());
	_file.Write(intersectionMode);
	_file.Write(collisionGroupIndex);
	_file.Write(collisionEnable);
	_file.Write(collisionPolygonsInfoEnable);
	if(collisionPolygonsInfoEnable)
	{
		_file.Write(intersectedPolygons.GetSize());
		for(SimpleList<int32>::Node* it = intersectedPolygons.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key);
		}
	}
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
}

bool PolygonalSurface::LoadFromFile(File& _file)
{ 
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{ 
		LoadDerivedFromFile(_file);
		return true;
	} 
	return false;
}



void PolygonalSurface::LoadDerivedFromFile(File& _file)
{
	int32 version = 1;
	_file.Read(version);

	childrens.Clear(MagicContainer::DELETE_NODE);
	intersectedPolygons.Clear();
	Vector3 position;
	_file.Read(position);
	SetPosition(position);
	Vector3 scale;
	_file.Read(scale);
	SetScale(scale);
	Quaternion rotation;
	_file.Read(rotation);
	SetRotation(rotation);
	_file.Read(intersectionMode);
	int32 collisionGroupIndex;
	_file.Read(collisionGroupIndex); 
	SetCollisionGroupIndex(collisionGroupIndex);
	bool collisionEnable;
	_file.Read(collisionEnable);
	SetCollisionEnable(collisionEnable);
	_file.Read(collisionPolygonsInfoEnable);
	if(collisionPolygonsInfoEnable) 
	{
		int32 amountOfPolygons;
		_file.Read(amountOfPolygons);
		for(int32 index, i = 0; i < amountOfPolygons; i++)
		{
			_file.Read(index);
			intersectedPolygons.AddToTail(index); 
		}
	}
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
		if(buildInMesh) { BindMesh(buildInMesh); }
	}
}