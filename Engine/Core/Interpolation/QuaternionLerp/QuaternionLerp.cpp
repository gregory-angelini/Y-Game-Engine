
#include "QuaternionLerp.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
StringANSI QuaternionLerp::quaternionLerpsSuffix = "q-lerp";
StringANSI QuaternionLerp::quaternionLerpsDir = "/lerps/q-lerps/";


QuaternionLerp::QuaternionLerp(void): segments(1)
{
	AddClassProperty(QUATERNION_LERP_CLASS);
}
 

QuaternionLerp::~QuaternionLerp(void)
{
	Destroy();
	Destroying();
}


Quaternion QuaternionLerp::GetCurrent(void)const
{
	return current;
}



void QuaternionLerp::SetAmountOfSegments(int32 _segments)
{
	if(_segments > 0)
	{
		segments = _segments;
	}
}



int32 QuaternionLerp::GetAmountOfSegments(void)const
{
	return segments;
}



Quaternion QuaternionLerp::GetDelta(void)const
{
	return delta;
}
		


void QuaternionLerp::Destroy(void)
{
	if(IsExist())
	{
		floatLerp.Get()->Disconnect(FloatLerp::STREAMING_FLOAT_T_MESSAGE, this);
		floatLerp.Get()->Disconnect(FloatLerp::FLOAT_LERP_START_MESSAGE, this);
		floatLerp.Get()->Disconnect(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, this);
		floatLerp.Clear();
	}
}


bool QuaternionLerp::IsExist(void)const
{
	return floatLerp.IsNotEmpty();
}


SimpleList<Quaternion>& QuaternionLerp::GetControlQuaternions(void)
{
	return controlQuaternions;
}



void QuaternionLerp::Set(Quaternion _from, Quaternion _to)
{
	controlQuaternions.Clear();
	controlQuaternions.AddToTail(_from);
	controlQuaternions.AddToTail(_to);
}



void QuaternionLerp::SetFrom(Quaternion _quaternion)
{
	if(controlQuaternions.IsNotEmpty())
	{
		controlQuaternions.GetHead()->key = _quaternion;
	}
}


void QuaternionLerp::SetTo(Quaternion _quaternion)
{
	if(controlQuaternions.IsNotEmpty())
	{
		controlQuaternions.GetTail()->key = _quaternion;
	}
}


void QuaternionLerp::BindFloatLerp(FloatLerp* _floatLerp)
{
	if(floatLerp.Get() != _floatLerp)
	{
		if(floatLerp.IsNotEmpty())
		{
			floatLerp.Get()->Disconnect(FloatLerp::STREAMING_FLOAT_T_MESSAGE, this);
			floatLerp.Get()->Disconnect(FloatLerp::FLOAT_LERP_START_MESSAGE, this);
			floatLerp.Get()->Disconnect(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, this);
			
			floatLerp.Clear();
		} 

		if(_floatLerp)
		{ 
			floatLerp.Attach(_floatLerp);
			floatLerp.Get()->Connect(FloatLerp::STREAMING_FLOAT_T_MESSAGE, this, Caller<>(this, &QuaternionLerp::CalculateNQuaternion));
			floatLerp.Get()->Connect(FloatLerp::FLOAT_LERP_START_MESSAGE, this, Caller<>(this, &QuaternionLerp::FloatLerpStartEvent));
			floatLerp.Get()->Connect(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, this, Caller<>(this, &QuaternionLerp::FloatLerpPlayingStateIsChanged));
		
			if(IsReceiversExist(CHANGE_FLOAT_LERP_MESSAGE)) { SendPackage(CHANGE_FLOAT_LERP_MESSAGE, floatLerp.Get()->GetName()); }
		}
	}
}


void QuaternionLerp::FloatLerpStartEvent(void)
{
	if(controlQuaternions.IsNotEmpty()) { current = controlQuaternions.GetHead()->key; }
}



void QuaternionLerp::FloatLerpPlayingStateIsChanged(int32 _state)
{
	if(_state == PlayingObject::STOP)
	{
		if(controlQuaternions.IsNotEmpty())
		{ 
			current = GetNQuaternion(0.0f);
			delta = Quaternion();
		}
	}
}



SimpleList<Quaternion> QuaternionLerp::GetPath(void)
{
	SimpleList<Quaternion> path;

	if(controlQuaternions.GetSize() > 1)
	{
		for(int32 i = 0; i < segments + 1; ++i)
		{
			float t = i / (float)segments;
	
			path.AddToTail(GetNQuaternion(t));
		}
		return path;
	}
	return controlQuaternions;
}



void QuaternionLerp::CalculateNQuaternion(float _t)
{ 
	if(controlQuaternions.GetSize() > 1)
	{
		Quaternion q = GetNQuaternion(_t);
		delta = Quaternion::_GetNormalize(current + q);
		current = q;
		
		if(IsReceiversExist(QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE)) { SendPackage(QuaternionLerp::STREAMING_QUATERNION_CURRENT_MESSAGE, current); }
		if(IsReceiversExist(QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE)) { SendPackage(QuaternionLerp::STREAMING_QUATERNION_DELTA_MESSAGE, delta); }
	}
}



Quaternion QuaternionLerp::GetNQuaternion(float _t)
{
	if(controlQuaternions.GetSize() > 1)
	{
		if(controlQuaternions.GetSize() > 2)
		{
			float step = 1.0f / (float)(controlQuaternions.GetSize() - 1);
		
			toIndex = ceil(_t / step);
			fromIndex = toIndex - 1; 

			float beginRange = (float)fromIndex * step;
			_t = (_t - beginRange) / step;
		}
		else
		{
			fromIndex = 0;
			toIndex = 1;
		}

		qFrom = controlQuaternions.Get(fromIndex)->key;
		qTo = controlQuaternions.Get(toIndex)->key;

		if(Quaternion::_GetDot(qFrom, qTo) < 0.0f) 
		{ 
			qTo = -qTo; 
		} 

		qTo -= qFrom;

		Quaternion quaternion = qFrom + qTo * _t;
		quaternion.Normalize();
		return quaternion;
	}
	return Quaternion(); 
}



Quaternion QuaternionLerp::GetFrom(void)const
{
	if(controlQuaternions.IsNotEmpty())
	{
		return controlQuaternions.GetHead()->key;
	}
	return Quaternion();
}


Quaternion QuaternionLerp::GetTo(void)const
{
	if(controlQuaternions.IsNotEmpty())
	{
		return controlQuaternions.GetTail()->key;
	}
	return Quaternion();
}



void QuaternionLerp::_SetFileSuffix(StringANSI _suffix)
{
	quaternionLerpsSuffix = _suffix;
}


StringANSI QuaternionLerp::_GetFileSuffix(void)
{
	return quaternionLerpsSuffix;
}


StringANSI QuaternionLerp::_GetDir(void)
{
	return quaternionLerpsDir;
}



bool QuaternionLerp::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + quaternionLerpsDir + GetName() + "." + quaternionLerpsSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool QuaternionLerp::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(segments);
		_file.Write(controlQuaternions.GetSize());
		for(SimpleList<Quaternion>::Node* it = controlQuaternions.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key);
		}
		_file.Write(floatLerp.IsNotEmpty());
				
		if(floatLerp.IsNotEmpty())
		{
			bool fLerpIsCommonAsset = floatLerp.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(fLerpIsCommonAsset);

			if(fLerpIsCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(floatLerp.Get()->GetName(), AssetLibrary::FLOAT_LERP_ASSET))
				{
					floatLerp.Get()->SaveToFile();
				}
				_file.Write(floatLerp.Get()->GetName().length());
				_file.WriteString(floatLerp.Get()->GetName());
			}
			else
			{
				floatLerp.Get()->SaveToFile(_file);
			}
		}
		return true;
	}
	return false;
}


bool QuaternionLerp::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + QuaternionLerp::_GetDir() + _name + "." + QuaternionLerp::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool QuaternionLerp::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(segments);
		_file.Write(controlQuaternions.GetSize());
		for(SimpleList<Quaternion>::Node* it = controlQuaternions.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key);
		}
		_file.Write(floatLerp.IsNotEmpty());
				
		if(floatLerp.IsNotEmpty())
		{
			bool fLerpIsCommonAsset = floatLerp.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
			_file.Write(fLerpIsCommonAsset);

			if(fLerpIsCommonAsset)
			{
				if(!AssetLibrary::_IsAssetExist(floatLerp.Get()->GetName(), AssetLibrary::FLOAT_LERP_ASSET))
				{
					floatLerp.Get()->SaveToFile();
				}
				_file.Write(floatLerp.Get()->GetName().length());
				_file.WriteString(floatLerp.Get()->GetName());
			}
			else
			{
				floatLerp.Get()->SaveToFile(_file);
			} 
		}
		return true;
	}
	return false;
}


bool QuaternionLerp::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + quaternionLerpsDir + _path + "." + quaternionLerpsSuffix;
	}
	
	File file(_path, File::READ);
	
	return LoadFromFile(file);
}


bool QuaternionLerp::LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Destroy();
		int32 version;
		_file.Read(version);
		_file.Read(properties);
		StringANSI name;
		int32 length;
		_file.Read(length);
		_file.ReadString(name, length);
		Rename(name);
		_file.Read(segments);
		int32 amountOfControlQuaternions;
		_file.Read(amountOfControlQuaternions);
		
		Quaternion rotation;
		for(int32 i = 0; i < amountOfControlQuaternions; ++i)
		{
			_file.Read(rotation);
			controlQuaternions.AddToTail(rotation);
		}
		bool floatLerpIsExist;
		_file.Read(floatLerpIsExist);
				
		if(floatLerpIsExist)
		{
			bool floatLerpIsCommonAsset;
			_file.Read(floatLerpIsCommonAsset);
			FloatLerp* buildInFloatLerp;
					
			if(floatLerpIsCommonAsset)
			{
				StringANSI name;
				int32 length;
				_file.Read(length);
				_file.ReadString(name, length);

				buildInFloatLerp = dynamic_cast<FloatLerp*>(AssetLibrary::_LoadCommonAsset<FloatLerp>(name));
			}
			else
			{
				buildInFloatLerp = dynamic_cast<FloatLerp*>(AssetLibrary::_LoadPrivateAsset<FloatLerp>(_file));
			}
			if(buildInFloatLerp) { BindFloatLerp(buildInFloatLerp); }
		}
		return true; 
	}
	return false;
}



QuaternionLerp* QuaternionLerp::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + quaternionLerpsDir + _path + "." + quaternionLerpsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


QuaternionLerp* QuaternionLerp::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		QuaternionLerp* lerp = new QuaternionLerp();

		if(lerp->LoadFromFile(_file))
		{
			return lerp;
		}
		delete lerp;
	}
	return NIL;
}
