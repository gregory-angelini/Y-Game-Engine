

#include "File.h"
#include <stdio.h>
#include <stdarg.h>



File::File(StringANSI _pathToFile, File::OpenMode _accessMode): file(0)
{
	Open(_pathToFile, _accessMode);
}


File::File(StringANSI _pathToFile): pathToFile(_pathToFile), file(0), accessMode(0)
{}



File::File(void): file(0), pathToFile(""), accessMode(0)
{}


 bool File::IsOpened(void)const
 {
	 if(file) { return true; }
	 else { return false; } 
 }



File::~File(void)
{
	_Close(file);
}



FILE* File::GetFile(void)const
{
	return file;
}



int32 File::GetAccessMode(void)const
{
	if(file) { return accessMode; }
	return 0;
}


StringANSI File::GetPath(void)const
{
	return pathToFile; 
}
	

StringANSI File::GetName(void)
{
	return _GetName(pathToFile);
}


StringANSI File::GetSuffix(void)
{
	return _GetSuffix(pathToFile);
}



StringANSI File::GetBaseName(void)
{
	return _GetBaseName(pathToFile);
}


StringANSI File::GetDir(void)
{
	return _GetDir(pathToFile);
}


bool File::Close(void)
{
	return _Close(file);
}



bool File::Open(StringANSI _pathToFile, OpenMode _accessMode)
{
	if(file)
	{
		if(accessMode == _accessMode && pathToFile == _pathToFile) { return true; } 

		if(!_Close(file))
		{
			return false;
		}
	}
	if(file = _Open(_pathToFile, _accessMode))
	{
		pathToFile = _pathToFile;
		accessMode = _accessMode;
		return true;
	}
	return false;
}



bool File::Open(OpenMode _accessMode)
{
	if(file)
	{
		if(accessMode == _accessMode) { return true; } 
		
		if(!_Close(file))
		{
			return false;
		}
	}
	if(file = _Open(pathToFile, _accessMode))
	{
		accessMode = _accessMode;
		return true;
	}
	return false;
}


bool File::IsExist(void)const
{
	return _IsExist(pathToFile);
}



bool File::Remove(void)
{
	Close();

	return _Remove(pathToFile); 
}



bool File::Rename(StringANSI _newName)
{
	return _Rename(pathToFile, _newName);
}


bool File::Relocate(StringANSI _newDir, bool _replace)
{
	return _Relocate(pathToFile, _newDir, _replace);
}



int64 File::GetPos(void)
{
	return _GetPos(file);
}


int64 File::GetSize(void)
{
	return _GetSize(file);
}



bool File::SetPos(int32 _newPos, int32 _mode)
{
	return _SetPos(file, _newPos, _mode);
}


void File::MoveToNewLine(void)
{
	_MoveToNewLine(file);
}



int32 File::IsEmpty(void)
{
	return _IsEmpty(file);
}



int32 File::ReadFile(StringANSI& _data)
{
	if(accessMode == READ)
	{
		return _ReadFile(file, _data);
	}
	return 0; 
	
}





int32 File::WriteFormatString(StringANSI _string, ...)
{
	if(accessMode == WRITE || accessMode == REWRITE)
	{
		if(file)
		{
			va_list argList;
			va_start(argList, _string);
			int32 symbols = vfprintf(file, _string.c_str(), argList);
			va_end(argList);
			return symbols;
		}
	}

	return -1;
}





int32 File::WriteString(StringANSI _data)
{
	if(accessMode == WRITE || accessMode == REWRITE)
	{
		return _WriteString(file, _data);
	}
	return 0;
}
 


int32 File::ReadWord(StringANSI& _data)
{
	if(accessMode == READ)
	{ 
		return _ReadWord(file, _data);
	}
	return 0;
}


int32 File::ReadString(StringANSI& _data, const int32 _size)
{
	if(accessMode == READ)
	{
		_ReadString(file, _data, _size); 
	}
		
	return 0;
} 


bool File::_GeneratePathToFile(StringANSI _pathToFolder, StringANSI _suffix, int32 _index, int32 _lowerBound, int32 _upperBound, StringANSI& _pathToFile)
{
	if(_lowerBound < 0 || _upperBound < 0) { return false; }
	if(_lowerBound >= _upperBound) { return false; }

	if(_index >= _lowerBound && _index <= _upperBound)
	{
		int32 upperBoundDigitSize = Numerical<int32>::_AmountOfDigits(_upperBound);
		int32 indexDigitSize = Numerical<int32>::_AmountOfDigits(_index);
		StringANSI padName(upperBoundDigitSize, '0');
		padName.replace(padName.length() - indexDigitSize, indexDigitSize, NumberToString(_index));
		if(_suffix.length()) { _suffix.insert(0, 1, '.'); }
		_pathToFile = _pathToFolder + "/" + padName + _suffix;
		return true;
	}
	return false;
} 
