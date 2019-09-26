#include "PolygonalSurface.h"



PolygonalSurface::CollisionGroup::CollisionGroup(void): isDestroying(false)
{}


PolygonalSurface::CollisionGroup::CollisionGroup(int32 _groupIndex):  groupIndex(_groupIndex), isDestroying(false)
{}


PolygonalSurface::CollisionGroup::~CollisionGroup(void)
{
	isDestroying = true;
}
  

int32 PolygonalSurface::CollisionGroup::GetIndex(void)const
{
	return groupIndex;
}


bool PolygonalSurface::CollisionGroup::IsDestroying(void)const
{
	return isDestroying;
}



bool PolygonalSurface::CollisionGroup::AddObject(PolygonalSurface* _object)
{
	if(AddToTail(_object))
	{
		return true;
	}
	return false;
}


bool PolygonalSurface::CollisionGroup::RemoveObject(PolygonalSurface* _object)
{
	MagicNode::Address* address = _object->GetAddress(this);

	if(address)
	{
		delete address;
		return true;
	}
	return false;
}

