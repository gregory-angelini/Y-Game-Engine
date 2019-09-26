#include "Font2D.h"


Font2D::FontType::FontType(void)
{}


Font2D::FontType::FontType(StringANSI _name, StringANSI _type): type(_type), name(_name)
{}


StringANSI Font2D::FontType::Name(void)const
{
	return name;
}

StringANSI Font2D::FontType::Type(void)const
{
	return type;
}