
#include "File.h"
#include <stdio.h>
#include <io.h>
#include <stdarg.h>
#include <windows.h>

bool File::_IsExist(StringANSI _pathToFile)
{
	if(access(_pathToFile.c_str(), 0) == 0)
	{ 
		return true;
	}
	return false;
}



bool File::_CreateFile(StringANSI _pathToFile)
{ 
	if(_IsExist(_pathToFile))
	{
		return true;
	}
	else
	{
		FILE* file = _Open(_pathToFile, File::REWRITE);

		if(file)
		{
			_Close(file);
			return true;
		}
	}
	return false;
}


bool File::_Close(FILE* _file)
{ 
	if(_file)
	{
		if(fclose(_file) == 0)
		{ 
			return true;
		}
	}
	return false; 
}


bool File::_CreateFolder(StringANSI _path)
{ 
	if(_IsExist(_path)) { return false; } 
	return (bool)CreateDirectoryA(_path.c_str(), NIL);
}


bool File::_RemoveFolder(StringANSI _path)
{
	return (bool)RemoveDirectoryA(_path.c_str());
}

bool File::_CopyFile(StringANSI _pathToFile, StringANSI _targetPath, bool _rewrite)
{
	if(_IsExist(_pathToFile))
	{
		return CopyFileA(_pathToFile.c_str(),
		                 _targetPath.c_str(),
					    (BOOL)_rewrite);
	}
	return false;
}


FILE* File::_Open(StringANSI _pathToFile, OpenMode _accessMode)
{ 

	if(_pathToFile.empty()) { return 0; }
	switch(_accessMode)
	{

		case READ: 
		{
			return fopen(_pathToFile.c_str(), "rb");
		}

		case WRITE:
		{
			FILE* file = fopen(_pathToFile.c_str(), "r+b");
			
			if(file)
			{
				_SetPos(file, 0, END);
			}
			return file;
		}

		case REWRITE:
		{
			return fopen(_pathToFile.c_str(), "wb");
		}
	}

	return NIL;
}




bool File::_Remove(StringANSI _pathToFile)
{
	if(_IsExist(_pathToFile))
	{
		if(remove(_pathToFile.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}





bool File::_Rename(StringANSI _pathToFile, StringANSI _newName)
{
	if(_IsExist(_pathToFile))
	{
		_newName = _GetName(_newName);

		_newName = _GetDir(_pathToFile) + _newName;

		if(!_IsExist(_newName))
		{ 
			if(rename(_pathToFile.c_str(), _newName.c_str()) == 0)
			{
				return true;
			}
		}
	}

	return false;
}




bool File::_Relocate(StringANSI _pathToFile, StringANSI _newDir, bool _replace)
{
	if(_IsExist(_pathToFile))
	{
		StringANSI pathToNewFile = _GetDir(_newDir) + _GetName(_pathToFile);
			
		if(pathToNewFile == _pathToFile)
		{
			return false;
		}
		if(_replace && _IsExist(pathToNewFile))
		{
			_Remove(pathToNewFile);
		}
				
		if(rename(_pathToFile.c_str(), pathToNewFile.c_str()) == 0)
		{
			return true;
		}
	}

	return false;
}



StringANSI File::_GetSuffix(StringANSI _pathToFile)
{
	int32 index = _pathToFile.find_last_of(".");
		
	if(index != -1)
	{
		return _pathToFile.substr(index + 1, _pathToFile.length() - 1);
	}
	return "";
}


StringANSI File::_GetBaseName(StringANSI _pathToFile)
{
	int32 index = _pathToFile.find_last_of("/\\");
		
	if(index != -1)
	{
		_pathToFile = _pathToFile.substr(index + 1);
	}
	index = _pathToFile.find_last_of(".");
	
	if(index != -1)
	{
		_pathToFile.erase(index, _pathToFile.length() - index);
	}
	return _pathToFile;
}



StringANSI File::_GetName(StringANSI _pathToFile)
{
	int32 index = _pathToFile.find_last_of("/\\");
		
	if(index != -1)
	{
		return _pathToFile.substr(index + 1);
	}
	return _pathToFile;
}



StringANSI File::_GetDir(StringANSI _pathToFile)
{
	int32 index = _pathToFile.find_last_of("/\\");
		
	if(index != -1)
	{
		return _pathToFile.substr(0, index + 1);
	}
	return "";
}



int64 File::_GetPos(FILE* _file)
{
	if(_file)
	{
		int64 pos;

		if(fgetpos(_file, &pos) == 0)
		{
			return pos;
		}
	}
	return -1;
}


void File::_MoveToNewLine(FILE* _file)
{
	if(_file)
	{
		char string[WORD_SIZE];
		while(fgets(string, sizeof(string) - 1, _file))
		{
			if(string[strlen(string) - 1] == '\n')
			{
				break;
			}	
		}
	}
}




int64 File::_GetSize(FILE* _file)
{
	if(_file)
	{
		int64 pos = _GetPos(_file);
		if(fseek(_file, 0, SEEK_END) == 0)
		{
			int64 size = _GetPos(_file);
			fseek(_file, pos, SEEK_SET);
			return size;
		}
	}

	return -1;
}




int32 File::_ReadString(FILE* _file, StringANSI& _data, int32 _size)
{
	if(_file)
	{
		if(feof(_file))
		{ 
			return -1;
		}

		char* buffer = new char[_size + 1];

		int32 symbols = fread(buffer, sizeof(char), _size, _file);
		
		buffer[_size] = '\0';
		
		_data = buffer;

		delete []buffer;

		return symbols;
	}
	return 0;
}
	


bool File::_SetPos(FILE* _file, int32 _newPos, int32 _mode)
{
	if(_file)
	{
		if(fseek(_file, _newPos, _mode) == 0)
		{
			return true;
		}
	}

	return false;
}

	

int32 File::_IsEmpty(FILE* _file)
{
	if(_file)
	{
		if(_GetSize(_file) == 0)
		{
			return 1;
		}
		return 0; 
	}

	return -1;
}


int32 File::_ReadFile(FILE* _file, StringANSI&_data)
{
	if(_file)
	{
		if(feof(_file))
		{ 
			return -1;
		}

		char symbol;
		int32 symbols = 0;
		while(fscanf(_file, "%c", &symbol) != EOF)
		{ 
			_data += symbol;
			++symbols;
		}

		return symbols;
	}

	return 0;
}




int32 File::_WriteString(FILE* _file, StringANSI _data)
{
	if(_file)
	{ 
		int32 result = fprintf(_file, "%s", _data.c_str());
		if(result > 0) 
		{
			return result;
		}
	}

	return 0;
}




int32 File::_ReadWord(FILE* _file, StringANSI& _data)
{
	if(_file)
	{
		if(feof(_file))
		{ 
			return -1;
		}

		char string[WORD_SIZE] = { 0 };

		if(fscanf(_file, "%s", &string) > EOF)
		{ 
			_data = string;
		}

		return _data.length();
	}

	return 0;
}




bool File::_GetIntKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, int32& _result)
{
	char buffer[256];

	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))
	{
		_result = atoi(buffer);
		return true;
	}
	return false;
}


bool File::_GetFloatKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, float& _result)
{
	char buffer[256];
	
	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))
	{ 
		_result = (float)atof(buffer);
		return true;
	}
	return false;
}


bool File::_GetStringKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, StringANSI& _result)
{
	char buffer[256];

	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))
	{
		_result = buffer;
		return true;
	}
	return false;
}



bool File::_GetBoolKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, bool& _result)
{
	char buffer[8];

	if(_relative) { _path = ".\\" + _path; }

	if(GetPrivateProfileStringA(_section.c_str(), _key.c_str(), "", buffer, sizeof(buffer), _path.c_str()))
	{
		if(StringANSI("true") == buffer)
		{
			_result = true;
			return true;
		}
		else if(StringANSI("false") == buffer)
		{
			_result = false;
			return true;
		}
	}
	return false;
}


bool File::_SetIntKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, int32 _value)
{
	char buffer[256];

	sprintf(buffer, "%d", _value);

	if(_relative) { _path = ".\\" + _path; }

	if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), buffer, _path.c_str()))
	{
		return true;
	}
	return false;
}


bool File::_SetFloatKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, float _value)
{
	char buffer[256];

	sprintf(buffer, "%f", _value);

	if(_relative) { _path = ".\\" + _path; }

	if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), buffer, _path.c_str()))
	{
		return true;
	}
	return false;
}


bool File::_SetStringKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, StringANSI _value)
{
	char buffer[256];

	sprintf(buffer, "%s", _value.c_str());

	if(_relative) { _path = ".\\" + _path; }

	if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), buffer, _path.c_str()))
	{
		return true;
	}
	return false;
}


bool File::_SetBoolKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, bool _value)
{
	char trueString[] = "true";
	char falseString[] = "false";

	if(_relative) { _path = ".\\" + _path; }

	if(_value)
	{
		if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), trueString, _path.c_str()))
		{
			return true;
		}
	}
	else
	{
		if(WritePrivateProfileStringA(_section.c_str(), _key.c_str(), falseString, _path.c_str()))
		{
			return true;
		}
	}
	return false;
}