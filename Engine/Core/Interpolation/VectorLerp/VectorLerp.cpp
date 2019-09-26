
#include "VectorLerp.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
StringANSI VectorLerp::vectorLerpsSuffix = "v-lerp";
StringANSI VectorLerp::vectorLerpsDir = "/lerps/v-lerps/";



VectorLerp::VectorLerp(void): segments(1)
{
	AddClassProperty(VECTOR_LERP_CLASS);
	
	calculateNVertex = &VectorLerp::CalculateBazierNVertex;
	lerpMode = BAZIER;
}
 

VectorLerp::~VectorLerp(void)
{
	Destroy();
	Destroying();
}


Vector3 VectorLerp::GetCurrent(void)const
{
	return current;
}


Vector3 VectorLerp::GetDelta(void)const
{
	return delta;
}


void VectorLerp::Destroy(void)
{
	if(IsExist())
	{
		floatLerp.Get()->Disconnect(FloatLerp::STREAMING_FLOAT_T_MESSAGE, this);
		floatLerp.Get()->Disconnect(FloatLerp::FLOAT_LERP_START_MESSAGE, this);
		floatLerp.Get()->Disconnect(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, this);
		floatLerp.Clear();
	}
}


bool VectorLerp::IsExist(void)const
{
	return floatLerp.IsNotEmpty();
}



void VectorLerp::BindFloatLerp(FloatLerp* _floatLerp)
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
			floatLerp.Get()->Connect(FloatLerp::STREAMING_FLOAT_T_MESSAGE, this, Caller<>(this, &VectorLerp::CalculateNVertex));
		    floatLerp.Get()->Connect(FloatLerp::FLOAT_LERP_START_MESSAGE, this, Caller<>(this, &VectorLerp::FloatLerpStartEvent));
			floatLerp.Get()->Connect(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, this, Caller<>(this, &VectorLerp::FloatLerpPlayingStateIsChanged));
		
			if(IsReceiversExist(CHANGE_FLOAT_LERP_MESSAGE)) { SendPackage(CHANGE_FLOAT_LERP_MESSAGE, floatLerp.Get()->GetName()); }
		}
	}
}


void VectorLerp::FloatLerpStartEvent(void)
{
	if(controlVectors.IsNotEmpty()) { current = controlVectors.GetHead()->key; }
}



void VectorLerp::FloatLerpPlayingStateIsChanged(int32 _state)
{
	if(_state == PlayingObject::STOP)
	{
		if(controlVectors.IsNotEmpty())
		{ 
			current = GetNVertex(0.0f);
			delta = Vector3();
		}
	}
}


void VectorLerp::SetAmountOfSegments(int32 _segments)
{
	if(_segments > 0)
	{
		segments = _segments;
	}
}



int32 VectorLerp::GetAmountOfSegments(void)const
{
	return segments;
}


SimpleList<Vector3>& VectorLerp::GetControlVectors(void)
{
	return controlVectors;
}



void VectorLerp::SetLerpMode(int32 _mode)
{
	if(lerpMode != _mode)
	{
		switch(_mode)
		{
			case BAZIER:
			{ 
				calculateNVertex = &VectorLerp::CalculateBazierNVertex;
				lerpMode = _mode;
				break;
			}

			case LAGRANGE:
			{ 
				calculateNVertex = &VectorLerp::CalculateLagrangeNVertex;
				lerpMode = _mode;
				break;
			}

			case RING:
			{
				calculateNVertex = &VectorLerp::CalculateRingNVertex;
				lerpMode = _mode;
				break;
			}
		}
	}
}


int32 VectorLerp::GetLerpMode(void)const
{
	return lerpMode;
}


SimpleList<Vector3> VectorLerp::GetPath(void)
{
	if(lerpMode == BAZIER)
	{
		return BuildBazier();
	}
	else if(lerpMode == LAGRANGE)
	{
		return BuildLagrange();
	}
	else if(lerpMode == RING)
	{
		return BuildRing();
	}
	return SimpleList<Vector3>();
}




Vector3 VectorLerp::GetRingNVertex(float _t)
{ 
	if(controlVectors.GetSize() == 2)
	{
		Circle circle(controlVectors.GetHead()->key, controlVectors.GetTail()->key);
				
		if(_t == 1.0f) { _t -= Numerical<float>::TINY; }
		return Vector3(circle.center.x + circle.radius * cosf(Angle::_DegreesToRadians(360.0f * _t)),
			           circle.center.y + circle.radius * sinf(Angle::_DegreesToRadians(360.0f * _t)),
					   controlVectors.GetHead()->key.z);
	}
	else if(controlVectors.GetSize() == 3)
	{
		Circle circle(controlVectors.GetHead()->key, controlVectors.GetHead()->GetRight()->key, controlVectors.GetTail()->key);
		return Vector3(circle.center.x + circle.radius * cosf(Angle::_DegreesToRadians(360.0f * _t)),
			           circle.center.y + circle.radius * sinf(Angle::_DegreesToRadians(360.0f * _t)),
					   controlVectors.GetHead()->key.z);
	}
	return Vector3();
}


void VectorLerp::CalculateRingNVertex(float _t)
{
	Vector3 v = GetRingNVertex(_t);
	delta = v - current;
	current = v;

	if(IsReceiversExist(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE)) { SendPackage(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE, current); }
	if(IsReceiversExist(VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE)) { SendPackage(VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE, delta); }
}


 
SimpleList<Vector3> VectorLerp::BuildLagrange(void)
{
	SimpleList<Vector3> path;
 
	if(controlVectors.GetSize() > 1)
	{
		for(int32 i = 0; i < segments + 1; ++i)
		{
			float t = i / (float)segments;
		
			path.AddToTail(GetLagrangeNVertex(t));
		}
		return path;
	}
	return controlVectors;
}


SimpleList<Vector3> VectorLerp::BuildBazier(void)
{
	SimpleList<Vector3> path;

	if(controlVectors.GetSize() > 1)
	{
		for(int32 i = 0; i < segments + 1; ++i)
		{
			float t = i / (float)segments;
	
			path.AddToTail(GetBazierNVertex(t));
		}
		return path;
	}
	return controlVectors;
}




SimpleList<Vector3> VectorLerp::BuildRing(void)
{
	SimpleList<Vector3> path;

	if(controlVectors.GetSize() > 1)
	{ 

		for(int32 i = 0; i < segments + 1; ++i)
		{
			float t = i / (float)segments;
	
			path.AddToTail(GetRingNVertex(t));
		}
		return path;
	}
	return controlVectors;
}



Vector3 VectorLerp::GetLagrangeNVertex(float _t)
{
	SimpleList<float> xList;
	SimpleList<Vector3>::Node* it = controlVectors.GetHead();

	while(it)
	{
		xList.AddToTail(it->key.x);
		it = it->GetRight();
	}

	float xMin = xList.GetExtremum(false)->key;
	float xMax = xList.GetExtremum(true)->key;

	float length = xMax - xMin;
	double y = 0.0;
	float x = xMin + _t * length;
	float z;

	it = controlVectors.GetHead();
	for(int32 i = 0; i < controlVectors.GetSize(); i++) 
	{
		y += it->key.y * LagrangePolynomial(x, i, it->key.x);
		
		if(it->GetLeft())
		{
			z = (it->GetLeft()->key.z + it->key.z) / 2.0f;
		}
		it = it->GetRight();
	}
	return Vector3(x, y, z);
}


void VectorLerp::CalculateLagrangeNVertex(float _t)
{
	Vector3 v = GetLagrangeNVertex(_t);
	delta = v - current;
	current = v;

	if(IsReceiversExist(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE)) { SendPackage(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE, current); }
	if(IsReceiversExist(VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE)) { SendPackage(VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE, delta); }
}



Vector3 VectorLerp::GetBazierNVertex(float _t)
{
	float u = 1 - _t;
	int32 N = controlVectors.GetSize() - 1;

	Vector3 p0 = controlVectors.GetHead()->key;
	p0 = p0 * pow(u, N);
	
	Vector3 pz = controlVectors.GetTail()->key;
	pz = pz* pow(_t, N);

	Vector3 p = p0;
	SimpleList<Vector3>::Node* it = controlVectors.GetHead();

	for(int32 i = 0; i < controlVectors.GetSize(); ++i)
	{
		if(i == 0 || i == N)
		{
			it = it->GetRight();
			continue;
		}
		Vector3 pi = it->key;
		p += pi * N * powf(u, N - i) * powf(_t, i);

		it = it->GetRight();
	}

	return p + pz;
}


void VectorLerp::CalculateBazierNVertex(float _t)
{
	Vector3 v = GetBazierNVertex(_t);
	delta = v - current;
	current = v;

	if(IsReceiversExist(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE)) { SendPackage(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE, current); }
	if(IsReceiversExist(VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE)) { SendPackage(VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE, delta); }
}



double VectorLerp::LagrangePolynomial(float _x, int32 _index, float _xi) 
{
	double numerator = 1.0f;
	double denominator = 1.0f;
	SimpleList<Vector3>::Node* it = controlVectors.GetHead();
	for(int32 i = 0; i < controlVectors.GetSize(); ++i) 
	{
		if(i == _index)
		{
			it = it->GetRight();
			continue;
		}
  
		numerator *= _x - it->key.x;
		it = it->GetRight();
	}
	it = controlVectors.GetHead();
	for(int32 i = 0; i < controlVectors.GetSize(); ++i) 
	{
		if(_xi != it->key.x)
		{
			denominator *= _xi - it->key.x;
		}
		it = it->GetRight();
	}

	return numerator / denominator;
}


void VectorLerp::CalculateNVertex(float _t)
{
	if(controlVectors.GetSize() > 1 && _t >= 0.0f && _t <= 1.0f)
	{
		(this->*calculateNVertex)(_t);
	}
}


Vector3 VectorLerp::GetNVertex(float _t)
{
	if(controlVectors.GetSize() > 1 && _t >= 0.0f && _t <= 1.0f)
	{
		if(lerpMode == BAZIER)
		{
			return GetBazierNVertex(_t);
		}
		else if(lerpMode == LAGRANGE)
		{
			return GetLagrangeNVertex(_t);
		}
		else if(lerpMode == RING)
		{
			return GetRingNVertex(_t);
		}
	}
	return Vector3();
}



void VectorLerp::Set(Vector3 _from, Vector3 _to)
{
	controlVectors.Clear();
	controlVectors.AddToTail(_from);
	controlVectors.AddToTail(_to);
}



void VectorLerp::SetFrom(Vector3 _vector)
{
	if(controlVectors.IsNotEmpty())
	{
		controlVectors.GetHead()->key = _vector;
	}
}


void VectorLerp::SetTo(Vector3 _vector)
{
	if(controlVectors.IsNotEmpty())
	{
		controlVectors.GetTail()->key = _vector;
	}
}


Vector3 VectorLerp::GetFrom(void)const
{
	if(controlVectors.IsNotEmpty())
	{
		return controlVectors.GetHead()->key;
	}
	return Vector3();
}


Vector3 VectorLerp::GetTo(void)const
{
	if(controlVectors.IsNotEmpty())
	{
		return controlVectors.GetTail()->key;
	}
	return Vector3();
}



void VectorLerp::_SetFileSuffix(StringANSI _suffix)
{
	vectorLerpsSuffix = _suffix;
}


StringANSI VectorLerp::_GetFileSuffix(void)
{
	return vectorLerpsSuffix;
}


StringANSI VectorLerp::_GetDir(void)
{
	return vectorLerpsDir;
}



bool VectorLerp::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + vectorLerpsDir + GetName() + "." + vectorLerpsSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool VectorLerp::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(segments);
		_file.Write(controlVectors.GetSize());
		for(SimpleList<Vector3>::Node* it = controlVectors.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key);
		}
		_file.Write(lerpMode);
		_file.Write(current);
		_file.Write(delta);
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


bool VectorLerp::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + VectorLerp::_GetDir() + _name + "." + VectorLerp::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}


bool VectorLerp::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(segments);
		_file.Write(controlVectors.GetSize());
		for(SimpleList<Vector3>::Node* it = controlVectors.GetHead(); it != NIL; it = it->GetRight())
		{
			_file.Write(it->key);
		}
		_file.Write(lerpMode);
		_file.Write(current);
		_file.Write(delta);
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



bool VectorLerp::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + vectorLerpsDir + _path + "." + vectorLerpsSuffix;
	}
	
	File file(_path, File::READ);
	
	return LoadFromFile(file);
}



bool VectorLerp::LoadFromFile(File& _file)
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
		int32 amountOfControlVectors;
		_file.Read(amountOfControlVectors);
		
		Vector3 vertex;
		for(int32 i = 0; i < amountOfControlVectors; ++i)
		{
			_file.Read(vertex);
			controlVectors.AddToTail(vertex);
		}
		int32 newLerpMode;
		_file.Read(newLerpMode); 
		SetLerpMode(newLerpMode);
		_file.Read(current);
		_file.Read(delta);
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



VectorLerp* VectorLerp::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + vectorLerpsDir + _path + "." + vectorLerpsSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


VectorLerp* VectorLerp::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		VectorLerp* lerp = new VectorLerp();

		if(lerp->LoadFromFile(_file))
		{
			return lerp;
		}
		delete lerp;
	}
	return NIL;
}
