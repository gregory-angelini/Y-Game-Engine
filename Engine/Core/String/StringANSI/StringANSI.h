#ifndef STRINGANSI_H
#define STRINGANSI_H
#include <string>
#include <sstream>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>

typedef std::string StringANSI;
template<typename Type>
StringANSI NumberToString(Type _number)
{
	std::stringstream ss;
	ss << _number;
	return ss.str();
}
int32 StringToInt(const StringANSI _text);
float StringToFloat(const StringANSI _text);
void ToUpper(StringANSI& _text);
SimpleList<StringANSI> SplitString(StringANSI _text, StringANSI _separators);
void ReplaceString(StringANSI& _string, StringANSI _oldString, StringANSI _newString);


#endif