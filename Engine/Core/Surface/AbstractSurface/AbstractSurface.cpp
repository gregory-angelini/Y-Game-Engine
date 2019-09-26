#include "AbstractSurface.h"


AbstractSurface::AbstractSurface(void): intersectionMode(AbstractSurface::CHECK_ONLY_FRONT_FACE | AbstractSurface::APPLY_TRANSFORMATION)
{
	AddClassProperty(ABSTRACT_SURFACE_CLASS);
}

	

AbstractSurface& AbstractSurface::operator = (const AbstractSurface& _object)
{
	if(this == &_object)
	{
		return *this;
	}

	SetIntersectionMode(_object.GetIntersectionMode());

	return *this;
}



AbstractSurface::~AbstractSurface(void)
{
	Destroying();
}



int32 AbstractSurface::GetIntersectionMode(void)const
{
	return intersectionMode;
}


void AbstractSurface::SetIntersectionMode(int32 _mode)
{
	intersectionMode = _mode;
}

	
void AbstractSurface::SetIntersectionMode(int32 _mode, bool _enable)
{
	if(_mode == CHECK_ONLY_FRONT_FACE)
	{
		if(_enable) 
		{
			intersectionMode |= CHECK_ONLY_FRONT_FACE;
		}
		else
		{
			intersectionMode &= ~CHECK_ONLY_FRONT_FACE;
		}
	}
	else if(_mode == APPLY_TRANSFORMATION)
	{
		if(_enable) 
		{
			intersectionMode |= APPLY_TRANSFORMATION;
		}
		else
		{
			intersectionMode &= ~APPLY_TRANSFORMATION;
		}
	}
}

