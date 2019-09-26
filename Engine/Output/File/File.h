#ifndef FILE_H
#define FILE_H
#include <Engine/Core/String/StringANSI/StringANSI.h>
#include <Engine/Core/Numerical/Numerical.h>
class File
{
    public: enum OpenMode
	{
		READ  = 1,
		REWRITE = 2,
		WRITE = 3
	};
	public: enum 
	{
		ORIGIN = SEEK_SET,
		CURRENT = SEEK_CUR,
		END = SEEK_END
	};
	private: enum
	{
		WORD_SIZE = 128
	};
    private: FILE* file;
    private: StringANSI pathToFile;
    private: int32 accessMode;
    public: File(StringANSI _pathToFile, OpenMode _accessMode);
	public: File(StringANSI _pathToFile);
	public: File(void);
    public: virtual ~File(void);
	public: StringANSI GetName(void);
	public: static StringANSI _GetName(StringANSI _pathToFile);
	public: StringANSI GetBaseName(void);
	public: static StringANSI _GetBaseName(StringANSI _pathToFile);
	public: StringANSI GetSuffix(void);
	public: static StringANSI _GetSuffix(StringANSI _pathToFile);
	public: StringANSI GetDir(void);
	public: static StringANSI _GetDir(StringANSI _pathToFile);
	public: FILE* GetFile(void)const;
	public: StringANSI GetPath(void)const;
	public: int32 GetAccessMode(void)const;
	public: static bool _GeneratePathToFile(StringANSI _pathToFolder, StringANSI _suffix, int32 _index, int32 _lowerBound, int32 _upperBound, StringANSI& _pathToFile);
	public: static bool _CreateFile(StringANSI _pathToFile);
    public: bool Close(void);
    public: static bool _Close(FILE* _file);
    public: bool Open(StringANSI _pathToFile, OpenMode _accessMode = WRITE);
    public: bool Open(OpenMode _accessMode = WRITE);
    public: static FILE* _Open(StringANSI _pathToFile, OpenMode _accessMode = WRITE);
	public: bool IsOpened(void)const;
	public: bool IsExist(void)const;
	public: static bool _IsExist(StringANSI _pathToFile);
	public: static bool _Remove(StringANSI _pathToFile);
	public: bool Remove(void);
	public: static bool _Rename(StringANSI _pathToFile, StringANSI _newName);
	public: bool Rename(StringANSI _newName);
    public: bool Relocate(StringANSI _newDir, bool _replace = false);
    public: static bool _Relocate(StringANSI _pathToFile, StringANSI _newDir, bool _replace = false);
    public: static bool _CopyFile(StringANSI _pathToFile, StringANSI _targetPath, bool _rewrite = false);
    public: static bool _CreateFolder(StringANSI _path);
    public: static bool _RemoveFolder(StringANSI _path);
	public: static int32 _IsEmpty(FILE* _file);
	public: int32 IsEmpty(void);
	public: static int64 _GetSize(FILE* _file);
	public: int64 GetSize(void);
	public: static int64 _GetPos(FILE* _file);
	public: int64 GetPos(void);
    public: static bool _SetPos(FILE* _file, int32 _newPos, int32 _mode = ORIGIN);
	public: bool SetPos(int32 _newPos, int32 _mode = ORIGIN);
	public: static void _MoveToNewLine(FILE* _file);
	public: void MoveToNewLine(void);
	public: static int32 _WriteString(FILE* _file, StringANSI _data);
	public: int32 WriteString(StringANSI _data);
	public: static int32 _ReadString(FILE* _file, StringANSI& _data, int32 _size);
	public: int32 ReadString(StringANSI& _data, int32 _size);
	public: static int32 _ReadWord(FILE* _file, StringANSI& _data);
	public: int32 ReadWord(StringANSI& _data);
	public: static int32 _ReadFile(FILE* _file, StringANSI& _data);
	public: int32 ReadFile(StringANSI& _data);
	public: int32 WriteFormatString(StringANSI _string, ...);
	public: template<typename Arg> int32 ReadFormatString(StringANSI _string, Arg& _arg)
	{
		if(file)
		{
			if(feof(file))
			{ 
				return -1;
			}

			return fscanf(file, _string.c_str(), &_arg);
		}
		return 0;
	}
	public: template<typename Arg1, typename Arg2> int32 ReadFormatString(StringANSI _string, Arg1& _arg1, Arg2& _arg2)
	{
		if(file)
		{
			if(feof(file))
			{ 
				return -1;
			}

			return fscanf(file, _string.c_str(), &_arg1, &_arg2);
		}
		return 0;
	}
	public: template<typename Arg1, typename Arg2, typename Arg3> int32 ReadFormatString(StringANSI _string, Arg1& _arg1, Arg2& _arg2, Arg3& _arg3)
	{
		if(file)
		{
			if(feof(file))
			{ 
				return -1;
			}

			return fscanf(file, _string.c_str(), &_arg1, &_arg2, &_arg3);
		}
		return 0;
	}
	public: template<typename Arg1, typename Arg2, typename Arg3, typename Arg4> int32 ReadFormatString(StringANSI _string, Arg1& _arg1, Arg2& _arg2, Arg3& _arg3, Arg4& _arg4)
	{
		if(file)
		{
			if(feof(file))
			{ 
				return -1;
			}

			return fscanf(file, _string.c_str(), &_arg1, &_arg2, &_arg3, &_arg4);
		}
		return 0;
	}
	public: template<typename Type> int32 Write(Type _data)
	{
		if(file && accessMode == WRITE || accessMode == REWRITE)
		{
			return fwrite(&_data, sizeof(Type), 1, file);
		}

		return 0;
	}
	public: template<typename Type> int32 Read(Type& _data)
	{
		if(file && accessMode == READ)
		{
			if(feof(file))
			{ 
				return -1;
			}

			return fread(&_data, sizeof(Type), 1, file);
		}

		return 0;
	}
	public: template<typename Type> int32 Write(Type& _data, int32 _size)
	{
		if(file && accessMode == WRITE || accessMode == REWRITE)
		{
			if(_size > 0)
			{
				return fwrite(&_data, sizeof(Type), _size, file);
			}
		}
		return 0;
	}
	public: template<typename Type> int32 Read(Type& _data, int32 _size)
	{
		if(file && accessMode == READ)
		{
			if(_size > 0)
			{
				if(feof(file))
				{ 
					return -1;
				}

				return fread(&_data, sizeof(Type), _size, file);
			}
		}
		return 0;
	}
	public: static bool _GetIntKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, int32& _result);
	public: static bool _GetFloatKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, float& _result);
	public: static bool _GetStringKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, StringANSI& _result);
	public: static bool _GetBoolKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, bool& _result);
	public: static bool _SetIntKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, int32 _value);
	public: static bool _SetFloatKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, float _value);
	public: static bool _SetStringKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, StringANSI _value);
	public: static bool _SetBoolKey(StringANSI _path, bool _relative, StringANSI _section, StringANSI _key, bool _value);
};





#endif 
