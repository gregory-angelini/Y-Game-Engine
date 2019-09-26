#include "AbstractObject.h"


AbstractObject::ObjectTypeProperty::ObjectTypeProperty(void): type(0)
{}



AbstractObject::ObjectTypeProperty::ObjectTypeProperty(int64 _type): type(_type)
{}


int64 AbstractObject::ObjectTypeProperty::GetType(void)const
{
	return type;
}

 

bool AbstractObject::ObjectTypeProperty::IsChild(ObjectTypeProperty* _property)
{
	return IsChild(childProperties.GetRootAddress(), _property);
}


bool AbstractObject::ObjectTypeProperty::IsChild(MagicBinaryTree<ObjectTypeProperty>::AddressDerived* _address, ObjectTypeProperty* _property)
{
	if(_address)
	{
		if(_address->GetNode()->GetType() == _property->GetType())
		{
			return true;
		}
		else
		{
			if(_address->GetNode()->IsChild(_property))
			{
				return true;
			}
		}

		if(IsChild(_address->GetChild(0), _property))
		{ return true; }

		if(IsChild(_address->GetChild(1), _property))
		{ return true; }
	}
	return false;
}


bool AbstractObject::ObjectTypeProperty::AddChildProperty(ObjectTypeProperty* _property)
{
	return (childProperties.Add(_property, &ObjectTypeProperty::GetType) ? true : false);
}

