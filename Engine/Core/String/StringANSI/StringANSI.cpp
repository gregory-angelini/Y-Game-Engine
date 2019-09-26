#include "StringANSI.h"
#include <algorithm>





int32 StringToInt(const StringANSI _text)
{
	std::stringstream ss(_text.c_str());
	int32 result;
	return ss >> result ? result : 0;
}

float StringToFloat(const StringANSI _text)
{
	std::stringstream ss(_text.c_str());
	float result;
	return ss >> result ? result : 0;
}



void ToUpper(StringANSI& _text)
{
	std::transform(_text.begin(), _text.end(), _text.begin(), toupper);
}


SimpleList<StringANSI> SplitString(StringANSI _text, StringANSI _separators)
{
	SimpleList<StringANSI> tokens;

	if(_text.length() > 0 && _separators.length() > 0)
	{
		char* buffer = new char[_text.length() + 1];

		strcpy(buffer, _text.c_str());

		char *token = strtok(buffer, _separators.c_str());

		while(token)
		{
			tokens.AddToTail(token);
			token = strtok(NIL, _separators.c_str());
		}
		delete[] buffer;
	}
	return tokens;
}


void ReplaceString(StringANSI& _string, StringANSI _oldString, StringANSI _newString) 
{
    size_t index = 0;

    while((index = _string.find(_oldString, index)) != std::string::npos) 
	{
		_string.replace(index, _oldString.length(), _newString);
         index += _newString.length();
    }
}
