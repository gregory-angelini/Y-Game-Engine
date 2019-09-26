#include "AbstractObject.h"
#include <Engine/Core/Bitset/Bitset.h>


AbstractObject::ObjectType::ObjectType(void): properties(0)
{}


AbstractObject::ObjectType::ObjectType(int64 _properties): properties(_properties)
{}



StringANSI AbstractObject::ObjectType::ToString(void)const
{
	return Bitset::_ToString(properties);
}


int64 AbstractObject::ObjectType::GetProperties(void)const
{
	return properties;
}