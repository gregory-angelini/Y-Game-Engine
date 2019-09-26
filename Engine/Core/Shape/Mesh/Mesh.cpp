#include "Mesh.h"
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Shape/Mesh/DelaunayTriangulation/DelaunayTriangulation.h>
#include <Engine/Core/Shape/Mesh/SplitTriangulation/SplitTriangulation.h>


#ifdef SendMessage
   #undef SendMessage
#endif
StringANSI Mesh::meshesSuffix = "mesh";
StringANSI Mesh::meshesDir = "/meshes/";



Mesh::Mesh(void): amountOfPolygons(0), shapeType(NO_SHAPE), boundingVolumeType(BOUNDING_CIRCLE)
{
	AddClassProperty(MESH_CLASS);
}


Mesh::~Mesh(void)
{
	Destroy();
	Destroying();
}



void Mesh::SetBoundingVolumeType(int32 _type)
{
	boundingVolumeType = _type;
}


int32 Mesh::GetBoundingVolumeType(void)const
{
	return boundingVolumeType;
}


Box Mesh::GetBoundingBox(void)
{
	return boundingBox;
}


Sphere Mesh::GetBoundingSphere(void)
{
	return boundingSphere;
}


Quad Mesh::GetBoundingQuad(void)
{
	return Quad(boundingBox.GetCenter(), boundingBox.GetSize().ToVector2());
}


Circle Mesh::GetBoundingCircle(void)
{
	return Circle(boundingSphere.center, boundingSphere.radius);
}


bool Mesh::IsExist(void)const
{
	return amountOfPolygons > 0;
}


void Mesh::Destroy(void)
{
	if(IsExist())
	{
		if(IsReceiversExist(DESTROY_MESH_MESSAGE)) { SendMessage(DESTROY_MESH_MESSAGE); }
		
		shapeType = NO_SHAPE; 

		DestroyVertices();
		DestroyIndices();
	}
}




void Mesh::ResizeIndices(int32 _size)
{ 
	if(_size > 0)
	{
		indices.resize(_size, 0);
	}
	else if(_size == 0)
	{
		indices.clear();
	}

	amountOfPolygons = indices.size() / VERTICES_IN_POLYGON;
}


void Mesh::SetIndex(int32 _index, uint32 _value)
{
	if(_index >= 0 && _index < indices.size())
	{
		indices[_index] = _value;
	}
} 


uint32 Mesh::GetIndex(int32 _index)
{
	if(_index >= 0 && _index < indices.size())
	{
		return indices[_index];
	}
	return 0;
}


void Mesh::DestroyIndices(void)
{
	ResizeIndices(0);
}
 


bool Mesh::IsIndicesExist(void)const
{
	return !indices.empty();
}


int32 Mesh::GetAmountOfIndices(void)const
{
	return indices.size();
}



void Mesh::ResizeVertices(int32 _size)
{
	if(_size > 0)
	{
		vertices.resize(_size, Vector3());
	}
	else if(_size == 0)
	{
		vertices.clear();
	}
}


void Mesh::DestroyVertices(void)
{
	ResizeVertices(0); 
}



bool Mesh::IsVerticesExist(void)const
{
	return !vertices.empty();
}

 

int32 Mesh::GetAmountOfVertices(void)const
{
	return vertices.size();
}



void Mesh::SetVertex(int32 _index, Vector3 _vertex)
{
	if(!vertices.empty())
	{
		if(_index >= 0 && _index < vertices.size())
		{
			vertices[_index] = _vertex;
		}
				
		UpdateBoundingVolume();
	}
}



void Mesh::UpdateBoundingVolume(void)
{
	if(amountOfPolygons > 0)
	{
		float xMin = vertices[0].x;
		float yMin = vertices[0].y;
		float zMin = vertices[0].z;

		float xMax = vertices[0].x;
		float yMax = vertices[0].y;
		float zMax = vertices[0].z;
		for(int32 i = 1; i < vertices.size(); ++i)
		{
			xMin = Numerical<float>::_GetMin(xMin, vertices[i].x);
			yMin = Numerical<float>::_GetMin(yMin, vertices[i].y);
			zMin = Numerical<float>::_GetMin(zMin, vertices[i].z);
			xMax = Numerical<float>::_GetMax(xMax, vertices[i].x);
			yMax = Numerical<float>::_GetMax(yMax, vertices[i].y);
			zMax = Numerical<float>::_GetMax(zMax, vertices[i].z);
		}
			
		Vector3 minPoint = Vector3(xMin, yMin, zMin);
		Vector3 maxPoint = Vector3(xMax, yMax, zMax);
		boundingBox.leftBottom = minPoint;
		boundingBox.rightTop = maxPoint;
		boundingSphere.center = Line::_GetCenter(minPoint, maxPoint);
		boundingSphere.radius = fabs(Line::_GetLength(minPoint, maxPoint) / 2.0f);
	}
}



Vector3 Mesh::GetVertex(int32 _index)const
{
	if(_index >= 0 && _index < vertices.size())
	{
		return vertices[_index];
	}
	return Vector3();
}


bool Mesh::CreateShape(SimpleList<Vector3>& _triangles)
{
	if(_triangles.GetSize() >= 3)
	{
		SimpleList<Vector3> verticesList = _triangles;
		verticesList.Unique();
		ResizeVertices(verticesList.GetSize());
		{
			SimpleList<Vector3>::Node* it = verticesList.GetHead();
			int32 i = 0;

			while(it)
			{
				vertices[i++] = it->key;
				it = it->GetRight();
			}
		}
		ResizeIndices(_triangles.GetSize());

		uint32 index;
		uint32 i = 0;
		SimpleList<Vector3>::Node* it = _triangles.GetHead();
			
		while(it)
		{
			index = verticesList.IndexOf(it->key);
			indices[i++] = index;
				
			it = it->GetRight();
		}
		
		UpdateBoundingVolume();
		shapeType = CUSTOM;

		if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
		return true;
	} 
	return false; 
}
 


bool Mesh::CreateShape(SimpleList<Triangle>& _triangles)
{
	if(_triangles.IsNotEmpty())
	{
		SimpleList<Vector3> vertices;

		for(SimpleList<Triangle>::Node* it = _triangles.GetHead(); it != NIL; it = it->GetRight())
		{
			vertices.AddToTail(it->key.v1);
			vertices.AddToTail(it->key.v2);
			vertices.AddToTail(it->key.v3);
		}
		return CreateShape(vertices); 
	}
	return false;
}



bool Mesh::CreateShape(SimpleList<Vector3>& _vertices, int32 _triangulationMode)
{
	if(_vertices.GetSize() >= 3)
	{
		if(_triangulationMode == SPLIT)
		{
			ResizeVertices(_vertices.GetSize());
			{
				SimpleList<Vector3>::Node* it = _vertices.GetHead();
				int32 i = 0;

				while(it)
				{
					vertices[i++] = it->key;
					it = it->GetRight();
				}
			}

			SimpleList<uint32> indicesList;

			if(!SplitTriangulation::_VerticesToTriangles(_vertices, indicesList))
			{ 
				ResizeVertices(0);
				return false;
			}
			ResizeIndices(indicesList.GetSize());
			{
				SimpleList<uint32>::Node* it = indicesList.GetHead();
				uint32 i = 0;

				while(it)
				{
					indices[i++] = it->key;
					it = it->GetRight();
				}
			}
		}
		else if(_triangulationMode == DELAUNAY)
		{
			ResizeVertices(_vertices.GetSize());
			{
				SimpleList<Vector3>::Node* it = _vertices.GetHead();
				int32 i = 0;

				while(it)
				{
					vertices[i++] = it->key;
					it = it->GetRight();
				}
			}

			SimpleList<uint32> indicesList;

			if(!DelaunayTriangulation::_VerticesToTriangles(_vertices, indicesList))
			{
				ResizeVertices(0);
				return false;
			}
			ResizeIndices(indicesList.GetSize());
			{
				SimpleList<uint32>::Node* it = indicesList.GetHead();
				uint32 i = 0;

				while(it)
				{
					indices[i++] = it->key;
					it = it->GetRight();
				}
			}
		}
		
		UpdateBoundingVolume();
		shapeType = CUSTOM;

		if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
		return true;
	}
	return false;
}
 



bool Mesh::IsClockwise(void)
{ 
	if(amountOfPolygons)
	{
		for(int32 i = 0; i < amountOfPolygons; ++i)
		{
			Vector3 v1 = vertices[indices[i * VERTICES_IN_POLYGON + 0]];
			Vector3 v2 = vertices[indices[i * VERTICES_IN_POLYGON + 1]];
			Vector3 v3 = vertices[indices[i * VERTICES_IN_POLYGON + 2]];

			float dx1 = v2.x - v1.x;
			float dx2 = v3.x - v2.x;

			float dy1 = v2.y - v1.y;
			float dy2 = v3.y - v2.y;

			float r = dx1 * dy2 - dx2 * dy1;
				
			if(r > 0)
			{
				return false;
			}
			return true;
		}
	}
	return false;
}





bool Mesh::IsCountClockwise(void)
{ 
	if(amountOfPolygons)
	{
		for(int32 i = 0; i < amountOfPolygons; ++i)
		{
			Vector3 v1 = vertices[indices[i * VERTICES_IN_POLYGON + 0]];
			Vector3 v2 = vertices[indices[i * VERTICES_IN_POLYGON + 1]];
			Vector3 v3 = vertices[indices[i * VERTICES_IN_POLYGON + 2]];

			float dx1 = v2.x - v1.x;
			float dx2 = v3.x - v2.x;

			float dy1 = v2.y - v1.y;
			float dy2 = v3.y - v2.y;

			float r = dx1 * dy2 - dx2 * dy1;
				
			if(r < 0)
			{
				return false;
			}
			return true;
		}
	}
	return false;
}


void Mesh::InverseVertices(void) 
{
	for(int32 j = 0; j < indices.size(); ++j)
	{
		for(int32 i = 0; i + j < indices.size() - 1; ++i)
		{
			if(i + 1 < indices.size())
			{
				uint32 index = indices[i + 1];
				indices[i + 1] = indices[i];
				indices[i] = index;
			}
		}
	}
}



int32 Mesh::GetAmountOfPolygons(void)const
{ 
	return amountOfPolygons;
}


bool Mesh::TotalPolygonsDivide4(void)
{
	if(vertices.empty() || indices.empty()) { return false; }
 
	vector<uint32> newIndices;
	map<uint64, uint32> newVertices;
	map<uint64, uint32>::iterator it;
	uint32 index12, index23, index31, a, b;
	int32 indexTypeBitSize = sizeof(uint32) * 8;
	uint64 key;
	Vector3 v1, v2, v3;
	uint32 index1, index2, index3;
	int32 polygons = amountOfPolygons;
	int32 amountOfVertices = vertices.size() - 1;
	for(int32 i = 0; i < polygons; ++i)
	{
		index1 = indices[i * VERTICES_IN_POLYGON + 0];
		index2 = indices[i * VERTICES_IN_POLYGON + 1];
		index3 = indices[i * VERTICES_IN_POLYGON + 2];
		v1 = GetVertex(index1);
		v2 = GetVertex(index2);
		v3 = GetVertex(index3);
		a = index1; b = index2;
		if(Numerical<uint32>::_GetMax(a, b) == b) { Common::_Swap(a, b); }
		key = uint64(b) | (uint64(a) << indexTypeBitSize);
		it = newVertices.find(key);
		if(it == newVertices.end()) 
		{
			vertices.push_back((v1 + v2) / 2.0f);
				
			index12 = ++amountOfVertices;
			newVertices.insert(make_pair(key, index12));
		}
		else
		{
			index12 = it->second;
		}
		a = index2; b = index3;
		if(Numerical<uint32>::_GetMax(a, b) == b) { Common::_Swap(a, b); }
		key = uint64(b) | (uint64(a) << indexTypeBitSize);
		it = newVertices.find(key);
		if(it == newVertices.end()) 
		{
			vertices.push_back((v2 + v3) / 2.0f);

			index23 = ++amountOfVertices;
			newVertices.insert(make_pair(key, index23));
		}
		else
		{
			index23 = it->second;
		}
		a = index3; b = index1;
		if(Numerical<uint32>::_GetMax(a, b) == b) { Common::_Swap(a, b); }
		key = uint64(b) | (uint64(a) << indexTypeBitSize);
		it = newVertices.find(key);
		if(it == newVertices.end()) 
		{
			vertices.push_back((v3 + v1) / 2.0f);

			index31 = ++amountOfVertices;
			newVertices.insert(make_pair(key, index31));
		}
		else
		{
			index31 = it->second;
		}
		newIndices.push_back(index1);  newIndices.push_back(index12); newIndices.push_back(index31);
		newIndices.push_back(index2);  newIndices.push_back(index23); newIndices.push_back(index12);
		newIndices.push_back(index3);  newIndices.push_back(index31); newIndices.push_back(index23);
		newIndices.push_back(index12); newIndices.push_back(index23); newIndices.push_back(index31);
	}
	indices = newIndices; 
	amountOfPolygons = indices.size() / VERTICES_IN_POLYGON;
	return true; 
}



void Mesh::TotalVertexNormalize(void)
{
	for(int32 i = 0; i < vertices.size(); ++i)
	{
		vertices[i].Normalize();
	}

	UpdateBoundingVolume();
}


int32 Mesh::GetShapeType(void)const
{
	return shapeType;
}



void Mesh::CreateQuad(Quad _quad)
{
	ResizeVertices(4);
	vertices[0] = _quad.GetVertex1();
	vertices[1] = _quad.GetVertex2();
	vertices[2] = _quad.GetVertex3();
	vertices[3] = _quad.GetVertex4();
	ResizeIndices(6);
	indices[0] = 0; 
	indices[1] = 1; 
	indices[2] = 2;
	indices[3] = 2; 
	indices[4] = 3; 
	indices[5] = 0;
	UpdateBoundingVolume();
	shapeType = QUAD;

	if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
} 



void Mesh::CreateTriangle(Triangle _triangle)
{
	ResizeVertices(3);
	vertices[0] = _triangle.v1; 
	vertices[1] = _triangle.v2;
	vertices[2] = _triangle.v3;
	ResizeIndices(3);
	indices[0] = 0; 
	indices[1] = 1; 
	indices[2] = 2;

	UpdateBoundingVolume();
	shapeType = TRIANGLE;

	if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
} 
 

void Mesh::CreateCircle(Circle _circle, int32 _segments)
{
	if(_segments < 3) { return; }

	ResizeVertices(_segments + 1);
	Vector3 center = _circle.center;
	vertices[0] = center;

	Vector3 vertex;
	for(int32 i = 1; i < vertices.size(); ++i)
	{
		float angle = Angle::DOUBLE_PI * (float)i / _segments;
		vertex.x = center.x + cosf(angle) * _circle.radius;
		vertex.y = center.y + sinf(angle) * _circle.radius;

		vertices[i] = vertex;
	}

	ResizeIndices(_segments * 3);
	for(int32 v1 = 1, v2 = 0, v3 = 0, j = 1, i = 0; i < indices.size(); i++)
	{
		if(v1)
		{
			indices[i] = 0;
			v1 = 0; v2 = 1;
		}
		else if(v2)
		{
			indices[i] = j;
			v2 = 0; v3 = 1;
		}
		else if(v3)
		{
			if(i == indices.size() - 1)
			{ 
				indices[i] = 1;
			} 
			else
			{
				indices[i] = ++j;
				v3 = 0; v1 = 1;
			}
		}
	}
	UpdateBoundingVolume();
	shapeType = CIRCLE;

	if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
} 



void Mesh::CreateBox(Box _box)
{
	ResizeVertices(6 * 4);
	vertices[0] = _box.GetVertex4();
	vertices[1] = _box.GetVertex3();
	vertices[2] = _box.GetVertex2();
	vertices[3] = _box.GetVertex1();
	vertices[4] = _box.GetVertex8(); 
	vertices[5] = _box.GetVertex7();
	vertices[6] = _box.GetVertex6();
	vertices[7] = _box.GetVertex5();
	vertices[8] = _box.GetVertex4(); 
	vertices[9] = _box.GetVertex8();
	vertices[10] = _box.GetVertex5();
	vertices[11] = _box.GetVertex1();
	vertices[12] = _box.GetVertex7(); 
	vertices[13] = _box.GetVertex3();
	vertices[14] = _box.GetVertex2();
	vertices[15] = _box.GetVertex6();
	vertices[16] = _box.GetVertex2(); 
	vertices[17] = _box.GetVertex1();
	vertices[18] = _box.GetVertex5();
	vertices[19] = _box.GetVertex6();
	vertices[20] = _box.GetVertex8(); 
	vertices[21] = _box.GetVertex7();
	vertices[22] = _box.GetVertex3();
	vertices[23] = _box.GetVertex4();
	ResizeIndices(6 * VERTICES_IN_POLYGON * 2);
	indices[0 * VERTICES_IN_POLYGON * 2 + 0] = 1; indices[0 * VERTICES_IN_POLYGON * 2 + 1] = 2; indices[0 * VERTICES_IN_POLYGON * 2 + 2] = 3; 
	indices[0 * VERTICES_IN_POLYGON * 2 + 3] = 3; indices[0 * VERTICES_IN_POLYGON * 2 + 4] = 0; indices[0 * VERTICES_IN_POLYGON * 2 + 5] = 1;
	indices[1 * VERTICES_IN_POLYGON * 2 + 0] = 6; indices[1 * VERTICES_IN_POLYGON * 2 + 1] = 5; indices[1 * VERTICES_IN_POLYGON * 2 + 2] = 4;
	indices[1 * VERTICES_IN_POLYGON * 2 + 3] = 4; indices[1 * VERTICES_IN_POLYGON * 2 + 4] = 7; indices[1 * VERTICES_IN_POLYGON * 2 + 5] = 6;
	indices[2 * VERTICES_IN_POLYGON * 2 + 0] = 9; indices[2 * VERTICES_IN_POLYGON * 2 + 1] = 8; indices[2 * VERTICES_IN_POLYGON * 2 + 2] = 11;
	indices[2 * VERTICES_IN_POLYGON * 2 + 3] = 11; indices[2 * VERTICES_IN_POLYGON * 2 + 4] = 10; indices[2 * VERTICES_IN_POLYGON * 2 + 5] = 9;
	indices[3 * VERTICES_IN_POLYGON * 2 + 0] = 14; indices[3 * VERTICES_IN_POLYGON * 2 + 1] = 13; indices[3 * VERTICES_IN_POLYGON * 2 + 2] = 12;
	indices[3 * VERTICES_IN_POLYGON * 2 + 3] = 12; indices[3 * VERTICES_IN_POLYGON * 2 + 4] = 15; indices[3 * VERTICES_IN_POLYGON * 2 + 5] = 14;
	indices[4 * VERTICES_IN_POLYGON * 2 + 0] = 18; indices[4 * VERTICES_IN_POLYGON * 2 + 1] = 17; indices[4 * VERTICES_IN_POLYGON * 2 + 2] = 16;
	indices[4 * VERTICES_IN_POLYGON * 2 + 3] = 16; indices[4 * VERTICES_IN_POLYGON * 2 + 4] = 19; indices[4 * VERTICES_IN_POLYGON * 2 + 5] = 18;
	indices[5 * VERTICES_IN_POLYGON * 2 + 0] = 23; indices[5 * VERTICES_IN_POLYGON * 2 + 1] = 20; indices[5 * VERTICES_IN_POLYGON * 2 + 2] = 21;
	indices[5 * VERTICES_IN_POLYGON * 2 + 3] = 21; indices[5 * VERTICES_IN_POLYGON * 2 + 4] = 22; indices[5 * VERTICES_IN_POLYGON * 2 + 5] = 23;
	UpdateBoundingVolume();
	shapeType = BOX;

	if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
} 




void Mesh::CreateIcosahedron(void)
{
	ResizeVertices(12); 
	double X = .525731112119133606;
	double Z = .850650808352039932;

	vertices[0].x = (float)-X; vertices[0].y = 0.0f; vertices[0].z = (float)Z;
	vertices[1].x = (float)X; vertices[1].y = 0.0f; vertices[1].z = (float)Z;
	vertices[2].x = (float)-X; vertices[2].y = 0.0f; vertices[2].z = (float)-Z;
	vertices[3].x = (float)X; vertices[3].y = 0.0f; vertices[3].z = (float)-Z;

	vertices[4].x = 0.0f; vertices[4].y = (float)Z; vertices[4].z = (float)X;
	vertices[5].x = 0.0f; vertices[5].y = (float)Z; vertices[5].z = (float)-X;
	vertices[6].x = 0.0f; vertices[6].y = (float)-Z; vertices[6].z = (float)X;
	vertices[7].x = 0.0f; vertices[7].y = (float)-Z; vertices[7].z = (float)-X;

	vertices[8].x = (float)Z; vertices[8].y = (float)X; vertices[8].z = 0.0f;
	vertices[9].x = (float)-Z; vertices[9].y = (float)X; vertices[9].z = 0.0f;
	vertices[10].x = (float)Z; vertices[10].y = (float)-X; vertices[10].z = 0.0f;
	vertices[11].x = (float)-Z; vertices[11].y = (float)-X; vertices[11].z = 0.0f;
	ResizeIndices(20 * VERTICES_IN_POLYGON);
	indices[0  * VERTICES_IN_POLYGON + 0] = 1;   indices[0 * VERTICES_IN_POLYGON + 1] = 4;   indices[0 * VERTICES_IN_POLYGON + 2] = 0;
	indices[1  * VERTICES_IN_POLYGON + 0] = 4;   indices[1 * VERTICES_IN_POLYGON + 1] = 9;   indices[1 * VERTICES_IN_POLYGON + 2] = 0;
	indices[2  * VERTICES_IN_POLYGON + 0] = 4;   indices[2 * VERTICES_IN_POLYGON + 1] = 5;   indices[2 * VERTICES_IN_POLYGON + 2] = 9;
	indices[3  * VERTICES_IN_POLYGON + 0] = 8;   indices[3 * VERTICES_IN_POLYGON + 1] = 5;   indices[3 * VERTICES_IN_POLYGON + 2] = 4;
	indices[4  * VERTICES_IN_POLYGON + 0] = 1;   indices[4 * VERTICES_IN_POLYGON + 1] = 8;   indices[4 * VERTICES_IN_POLYGON + 2] = 4;

	indices[5  * VERTICES_IN_POLYGON + 0] = 1;   indices[5 * VERTICES_IN_POLYGON + 1] = 10;  indices[5 * VERTICES_IN_POLYGON + 2] = 8;
	indices[6  * VERTICES_IN_POLYGON + 0] = 10;  indices[6 * VERTICES_IN_POLYGON + 1] = 3;   indices[6 * VERTICES_IN_POLYGON + 2] = 8;
	indices[7  * VERTICES_IN_POLYGON + 0] = 8;   indices[7 * VERTICES_IN_POLYGON + 1] = 3;   indices[7 * VERTICES_IN_POLYGON + 2] = 5;
	indices[8  * VERTICES_IN_POLYGON + 0] = 3;   indices[8 * VERTICES_IN_POLYGON + 1] = 2;   indices[8 * VERTICES_IN_POLYGON + 2] = 5;
	indices[9  * VERTICES_IN_POLYGON + 0] = 3;   indices[9 * VERTICES_IN_POLYGON + 1] = 7;   indices[9 * VERTICES_IN_POLYGON + 2] = 2;

	indices[10 * VERTICES_IN_POLYGON + 0] = 3;  indices[10 * VERTICES_IN_POLYGON + 1] = 10; indices[10 * VERTICES_IN_POLYGON + 2] = 7;
	indices[11 * VERTICES_IN_POLYGON + 0] = 10; indices[11 * VERTICES_IN_POLYGON + 1] = 6;  indices[11 * VERTICES_IN_POLYGON + 2] = 7;
	indices[12 * VERTICES_IN_POLYGON + 0] = 6;  indices[12 * VERTICES_IN_POLYGON + 1] = 11; indices[12 * VERTICES_IN_POLYGON + 2] = 7;
	indices[13 * VERTICES_IN_POLYGON + 0] = 6;  indices[13 * VERTICES_IN_POLYGON + 1] = 0;  indices[13 * VERTICES_IN_POLYGON + 2] = 11;
	indices[14 * VERTICES_IN_POLYGON + 0] = 6;  indices[14 * VERTICES_IN_POLYGON + 1] = 1;  indices[14 * VERTICES_IN_POLYGON + 2] = 0;

	indices[15 * VERTICES_IN_POLYGON + 0] = 10; indices[15 * VERTICES_IN_POLYGON + 1] = 1;  indices[15 * VERTICES_IN_POLYGON + 2] = 6;
	indices[16 * VERTICES_IN_POLYGON + 0] = 11; indices[16 * VERTICES_IN_POLYGON + 1] = 0;  indices[16 * VERTICES_IN_POLYGON + 2] = 9;
	indices[17 * VERTICES_IN_POLYGON + 0] = 2;  indices[17 * VERTICES_IN_POLYGON + 1] = 11; indices[17 * VERTICES_IN_POLYGON + 2] = 9;
	indices[18 * VERTICES_IN_POLYGON + 0] = 5;  indices[18 * VERTICES_IN_POLYGON + 1] = 2;  indices[18 * VERTICES_IN_POLYGON + 2] = 9;
	indices[19 * VERTICES_IN_POLYGON + 0] = 11; indices[19 * VERTICES_IN_POLYGON + 1] = 2;  indices[19 * VERTICES_IN_POLYGON + 2] = 7;
	UpdateBoundingVolume();
	shapeType = ICOSAHEDRON;

	if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
}



void Mesh::CreateIcosphere(int32 _level)
{ 
	CreateIcosahedron();
	for(int32 i = 0; i < _level; ++i)
	{
		TotalPolygonsDivide4();
	} 
	TotalVertexNormalize();
	UpdateBoundingVolume();
	shapeType = ICOSPHERE;

	if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
}
 

void Mesh::CreatePyramid(float _radius)
{
	ResizeVertices(5);

	vertices[0] = Vector3(0.0f, _radius, 0.0f);
    vertices[1] = Vector3(-_radius, -_radius, _radius);
    vertices[2] = Vector3(_radius, -_radius, _radius);
	vertices[3] = Vector3(_radius, -_radius, -_radius);
	vertices[4] = Vector3(-_radius, -_radius, -_radius);
	ResizeIndices(6 * VERTICES_IN_POLYGON);
	indices[0  * VERTICES_IN_POLYGON + 0] = 0;   indices[0 * VERTICES_IN_POLYGON + 1] = 1;   indices[0 * VERTICES_IN_POLYGON + 2] = 2;
	indices[1  * VERTICES_IN_POLYGON + 0] = 0;   indices[1 * VERTICES_IN_POLYGON + 1] = 2;   indices[1 * VERTICES_IN_POLYGON + 2] = 3;
	indices[2  * VERTICES_IN_POLYGON + 0] = 0;   indices[2 * VERTICES_IN_POLYGON + 1] = 3;   indices[2 * VERTICES_IN_POLYGON + 2] = 4;
	indices[3  * VERTICES_IN_POLYGON + 0] = 0;   indices[3 * VERTICES_IN_POLYGON + 1] = 4;   indices[3 * VERTICES_IN_POLYGON + 2] = 1;
	indices[4  * VERTICES_IN_POLYGON + 0] = 3;   indices[4 * VERTICES_IN_POLYGON + 1] = 2;   indices[4 * VERTICES_IN_POLYGON + 2] = 1;
	indices[5  * VERTICES_IN_POLYGON + 0] = 1;   indices[5 * VERTICES_IN_POLYGON + 1] = 4;   indices[5 * VERTICES_IN_POLYGON + 2] = 3;
	UpdateBoundingVolume();
	shapeType = PYRAMID;

	if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
}
 

bool Mesh::_IsConvex(Vector3 _point, const SimpleList<Vector3>& _polygon)
{ 
	static const int32 q_patt[2][2] = { {0, 1}, {3, 2} };

	if(_polygon.GetSize() < 3)
	{
		return false;
	}
	
	Vector3 previousPoint = _polygon.GetTail()->key;

	previousPoint.x -= _point.x;
	previousPoint.y -= _point.y;

	int32 previousPointQ = q_patt[previousPoint.y < 0.0f][previousPoint.x < 0.0f];

	int32 w = 0;
	for(SimpleList<Vector3>::Node* it = _polygon.GetHead(); it != NIL; it = it->GetRight())
	{
		Vector3 currentPoint = it->key;

		currentPoint.x -= _point.x;
		currentPoint.y -= _point.y;

		int32 currentPointQ = q_patt[currentPoint.y < 0.0f][currentPoint.x < 0.0f];

		switch(currentPointQ - previousPointQ)
		{
			case -3: 
			{
				++w; 
				break;
			}

			case 3:
			{
				--w; 
				break;
			}

			case -2: 
			{
				if(previousPoint.x * currentPoint.y >= previousPoint.y * currentPoint.x) 
				{
					++w; 
				}
				break;
			}

			case 2: 
			{
				if(!(previousPoint.x * currentPoint.y >= previousPoint.y * currentPoint.x)) 
				{
					--w; 
				}
				break;
			}
		}

		previousPoint = currentPoint;
		previousPointQ = currentPointQ;
	}
	return w == 0;
}

 
bool Mesh::GetTriangle(int32 _index, Triangle& _triangle)
{
	if(_index >= 0 && _index < amountOfPolygons)
	{
		_triangle.v1 = vertices[indices[_index * VERTICES_IN_POLYGON + 0]];
		_triangle.v2 = vertices[indices[_index * VERTICES_IN_POLYGON + 1]];
		_triangle.v3 = vertices[indices[_index * VERTICES_IN_POLYGON + 2]];
		return true;
	}
	return false;
}



StringANSI Mesh::_GetDir(void)
{
	return meshesDir;
}


void Mesh::_SetFileSuffix(StringANSI _suffix)
{
	meshesSuffix = _suffix;
}


StringANSI Mesh::_GetFileSuffix(void)
{
	return meshesSuffix;
}



bool Mesh::SaveToFile(StringANSI _path)
{
	if(_path.empty())
	{
		_path = AssetLibrary::_GetDir() + meshesDir + GetName() + "." + meshesSuffix;
	}

	File file(_path, File::REWRITE);
		
	if(!SaveToFile(file))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Mesh::SaveToFile(File& _file)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(GetName().length());
		_file.WriteString(GetName());
		_file.Write(shapeType);
		_file.Write(vertices.size());
		if(!vertices.empty()) { _file.Write(vertices[0], vertices.size()); }
		_file.Write(indices.size());
		if(!indices.empty()) { _file.Write(indices[0], indices.size()); }
		_file.Write(boundingVolumeType);
		_file.Write(boundingBox);
		_file.Write(boundingSphere);
		return true;
	}
	return false;
}



bool Mesh::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + Mesh::_GetDir() + _name + "." + Mesh::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool Mesh::SaveAsToFile(File& _file, StringANSI _name)
{
	if(_file.IsOpened() && (_file.GetAccessMode() == File::WRITE || _file.GetAccessMode() == File::REWRITE))
	{
		int32 version = 1;
		_file.Write(version);
		_file.Write(properties);
		_file.Write(_name.length());
		_file.WriteString(_name);
		_file.Write(shapeType);
		_file.Write(vertices.size());
		if(!vertices.empty()) { _file.Write(vertices[0], vertices.size()); }
		_file.Write(indices.size());
		if(!indices.empty()) { _file.Write(indices[0], indices.size()); }
		_file.Write(boundingVolumeType);
		_file.Write(boundingBox); 
		_file.Write(boundingSphere);
		return true;
	}
	return false;
}



bool Mesh::LoadFromFile(StringANSI _path, bool _auto)
{ 
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + meshesDir + _path + "." + meshesSuffix;
	}
	
	File file(_path, File::READ);
	
	return LoadFromFile(file);
}



bool Mesh::LoadFromFile(File& _file)
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
		_file.Read(shapeType);
		int32 amountOfVertices;
		_file.Read(amountOfVertices);
		ResizeVertices(amountOfVertices);
		if(amountOfVertices > 0) { _file.Read(vertices[0], amountOfVertices); }
		int32 amountOfIndices;
		_file.Read(amountOfIndices);
		ResizeIndices(amountOfIndices);
		amountOfPolygons = indices.size() / VERTICES_IN_POLYGON;
		if(amountOfIndices) { _file.Read(indices[0], amountOfIndices); }
		_file.Read(boundingVolumeType);
		_file.Read(boundingBox); 
		_file.Read(boundingSphere);

		if(IsReceiversExist(INIT_MESH_MESSAGE)) { SendMessage(INIT_MESH_MESSAGE); }
		return true;
	} 
	return false;
}




Mesh* Mesh::_LoadFromFile(StringANSI _path, bool _auto)
{
	if(_auto)
	{
		_path = AssetLibrary::_GetDir() + meshesDir + _path + "." + meshesSuffix;
	}

	File file(_path, File::READ);

	return _LoadFromFile(file);
}


Mesh* Mesh::_LoadFromFile(File& _file)
{
	if(_file.IsOpened() && _file.GetAccessMode() == File::READ)
	{
		Mesh* mesh = new Mesh();

		if(mesh->LoadFromFile(_file))
		{
			return mesh;
		}
		delete mesh;
	}
	return NIL;
}
