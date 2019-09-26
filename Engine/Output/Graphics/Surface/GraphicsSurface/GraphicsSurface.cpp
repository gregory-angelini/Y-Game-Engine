#include "GraphicsSurface.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Output/Graphics/Shape/Circle/GraphicsCircle/GraphicsCircle.h>



float GraphicsSurface::boundingVolumeScale = 1.1f;
float GraphicsSurface::boundingVolumeCornersLength = 5.0f;


GraphicsSurface::GraphicsSurface(void): staticEnable(false), displayListIndex(0)
{
	AddClassProperty(GRAPHICS_SURFACE_CLASS);

	draw = Caller<>(this, &GraphicsSurface::CustomDraw);
	SetBackFaceCullingEnable(true);
	boundingVolumeShowMode = SHOW_WIREFRAME_BOUNDING_VOLUME | SHOW_TRANSFORMED_BOUNDING_VOLUME | SHOW_BOUNDING_BOX;
	boundingVolumeColor = RGBA(RGBA::YELLOW_COLOR).GetRGBAf();
	SetBoundingVolumeShowEnable(false);
}



GraphicsSurface& GraphicsSurface::operator = (const GraphicsSurface& _object)
{
	if(this == &_object)
	{
		return *this;
	}

	if(displayListIndex)
	{
		glDeleteLists(displayListIndex, 1);
		displayListIndex = 0;
	}

	PolygonalSurface::operator = (_object);
	GraphicsObject::operator = (_object);

	SetBoundingVolumeShowMode(_object.boundingVolumeShowMode);
	SetBoundingVolumeColor(_object.boundingVolumeColor);
	SetBoundingVolumeShowEnable(_object.IsBoundingVolumeShowEnabled());
	SetStaticEnable(_object.staticEnable); 
	SetBackFaceCullingEnable(_object.IsBackFaceCullingEnabled());

	return *this;
}



GraphicsSurface::~GraphicsSurface(void)
{
	Destroy();
	Destroying();
}


void GraphicsSurface::Destroy(void)
{ 
	if(mesh.IsNotEmpty())
	{ 
		MeshIsDestroyed();

		mesh.Get()->Disconnect(Mesh::DESTROY_MESH_MESSAGE, this);
		mesh.Get()->Disconnect(Mesh::INIT_MESH_MESSAGE, this);

		mesh.Clear();
	}

	if(hitbox.IsNotEmpty())
	{
		HitboxIsDestroyed();

		hitbox.Get()->Disconnect(Hitbox::DESTROY_HITBOX_MESSAGE, this);
		hitbox.Get()->Disconnect(Hitbox::INIT_HITBOX_MESSAGE, this);

		hitbox.Clear();
	}

	intersectedPolygons.Clear();

	if(displayListIndex)
	{
		glDeleteLists(displayListIndex, 1);
		displayListIndex = 0;
	}
}



Vector3 GraphicsSurface::GetAbsolutePosition(void)
{
	if(parent.IsNotEmpty())
	{
		return parent.Get()->GetTransformation() * position;
	}
	else
	{
		if(graphicsContainer.IsNotEmpty())
		{
			return graphicsContainer.Get()->GetTransformation() * position;
		}
	}
	return position;
}



Quaternion GraphicsSurface::GetAbsoluteRotation(void)
{
	if(parent.IsNotEmpty())
	{
		return parent.Get()->GetAbsoluteRotation() * rotation;
	}
	else
	{
		if(graphicsContainer.IsNotEmpty())
		{
			return graphicsContainer.Get()->GetAbsoluteRotation() * rotation;
		}
	}
	return rotation;
}



Vector3 GraphicsSurface::GetAbsoluteScale(void)
{
	if(parent.IsNotEmpty())
	{
		return parent.Get()->GetAbsoluteScale() + scale;
	}
	else
	{
		if(graphicsContainer.IsNotEmpty())
		{
			return graphicsContainer.Get()->GetAbsoluteScale() + scale;
		}
	}
	return scale;
}


Matrix4x4 GraphicsSurface::GetTransformation(void)
{
	if(!transformationValid)
	{ 
		transformation.MakeTransform(GetScale(), GetRotation(), GetPosition());
		
		if(parent.IsNotEmpty())
		{
			transformation = parent.Get()->GetTransformation() * transformation; 
		}
		else
		{
			if(graphicsContainer.IsNotEmpty())
			{
				transformation = graphicsContainer.Get()->GetTransformation() * transformation; 
			}
		}
		SetTransformationValid();
	}
	return transformation;
}


void GraphicsSurface::SetBoundingVolumeColor(Vector4 _color)
{
	boundingVolumeColor = _color;
}


void GraphicsSurface::SetBoundingVolumeColor(RGBA _color)
{
	boundingVolumeColor = _color.GetRGBAf();
}


Vector4 GraphicsSurface::GetBoundingVolumeColor(void)
{
	return boundingVolumeColor;
}



void GraphicsSurface::SetBoundingVolumeShowEnable(bool _enable)
{
	ModifyGraphicsType(SHOW_BOUNDING_VOLUME_GRAPHICS_TYPE, _enable); 
}



bool GraphicsSurface::IsBoundingVolumeShowEnabled(void)const
{
	return IsGraphicsTypeEnabled(SHOW_BOUNDING_VOLUME_GRAPHICS_TYPE);
}



void GraphicsSurface::SetBoundingVolumeShowMode(int32 _mode)
{
	boundingVolumeShowMode = _mode;
}



int32 GraphicsSurface::GetBoundingVolumeShowMode(void)const
{
	return boundingVolumeShowMode;
}



void GraphicsSurface::SetStaticEnable(bool _enable)
{
	staticEnable = _enable;

	if(!_enable && displayListIndex)
	{
		glDeleteLists(displayListIndex, 1);
	}
}


bool GraphicsSurface::IsStaticEnabled(void)const
{
	return staticEnable;
}



void GraphicsSurface::SetBackFaceCullingEnable(bool _enable)	
{
	ModifyGraphicsType(GraphicsSurface::BACK_FACE_CULLING_GRAPHICS_TYPE, _enable);
}



bool GraphicsSurface::IsBackFaceCullingEnabled(void)const
{  
	return IsGraphicsTypeEnabled(GraphicsSurface::BACK_FACE_CULLING_GRAPHICS_TYPE);
}




void GraphicsSurface::ModifyGraphicsType(int64 _property, bool _enable)
{
	GraphicsObject::ModifyGraphicsType(_property, _enable);

	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{
		if(IsGraphicsTypeEnabled(GraphicsSurface::SHOW_BOUNDING_VOLUME_GRAPHICS_TYPE))
		{
			if(IsGraphicsTypeEnabled(GraphicsSurface::BACK_FACE_CULLING_GRAPHICS_TYPE))
			{   
				draw = Caller<>(this, &GraphicsSurface::DrawCulledPolygonalShapeWithBoundingVolume);
			}
			else
			{
				draw = Caller<>(this, &GraphicsSurface::DrawPolygonalShapeWithBoundingVolume);
			}
		}
		else
		{
			if(IsGraphicsTypeEnabled(GraphicsSurface::BACK_FACE_CULLING_GRAPHICS_TYPE))
			{
				draw = Caller<>(this, &GraphicsSurface::DrawCulledPolygonalShape);
			}
			else
			{
				draw = Caller<>(this, &GraphicsSurface::DrawPolygonalShape);
			}
		}
	}
	else
	{
		draw = Caller<>(this, &GraphicsSurface::CustomDraw);
	}
}
 


void GraphicsSurface::DrawCulledPolygonalShapeWithBoundingVolume(void)
{
	DrawCulledPolygonalShape();
	
	DrawBoundingVolume();
}
 

void GraphicsSurface::DrawPolygonalShapeWithBoundingVolume(void)
{
	glDisable(GL_CULL_FACE);
	
	DrawCulledPolygonalShape();

	glEnable(GL_CULL_FACE);

	DrawBoundingVolume();
}
 


void GraphicsSurface::DrawPolygonalShape(void)
{
	glDisable(GL_CULL_FACE);
	
	DrawCulledPolygonalShape();

	glEnable(GL_CULL_FACE);
}
 


void GraphicsSurface::Draw(void)
{
	if(showEnable)
	{
		BeginTransform();
		draw();
		EndTransform();
	}
}


void GraphicsSurface::_DrawTriangle(Triangle _triangle, Vector3 _color, float _alpha)
{	
	glDisable(GL_CULL_FACE);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glVertexPointer(Triangle::SIZE,
					GL_FLOAT,
					0,
					_triangle.GetData());
	glBegin(GL_TRIANGLES);
		glColor4fv(Vector4(_color, _alpha).GetData());
		glArrayElement(0);
		glArrayElement(1);
		glArrayElement(2);
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_CULL_FACE);
}



void GraphicsSurface::_DrawTriangle(Triangle _triangle, RGBA _color)
{
	glDisable(GL_CULL_FACE);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glVertexPointer(Triangle::SIZE,
					GL_FLOAT,
					0,
					_triangle.GetData());
	glBegin(GL_TRIANGLES);
		glColor4ubv(_color.GetData());
		glArrayElement(0);
		glArrayElement(1);
		glArrayElement(2);
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_CULL_FACE);
}



	
void GraphicsSurface::_DrawCulledTriangle(Triangle _triangle, Vector3 _color, float _alpha)
{	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glVertexPointer(Triangle::SIZE,
					GL_FLOAT,
					0,
					_triangle.GetData());
	glBegin(GL_TRIANGLES);
		glColor4fv(Vector4(_color, _alpha).GetData());
		glArrayElement(0);
		glArrayElement(1);
		glArrayElement(2);
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}



void GraphicsSurface::_DrawCulledTriangle(Triangle _triangle, RGBA _color)
{	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glVertexPointer(Triangle::SIZE,
					GL_FLOAT,
					0,
					_triangle.GetData());
	glBegin(GL_TRIANGLES);
		glColor4ubv(_color.GetData());
		glArrayElement(0);
		glArrayElement(1);
		glArrayElement(2);
	glEnd();
	
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}



void GraphicsSurface::CustomDraw(void)
{}




void GraphicsSurface::DrawTransformedBoundingBox(void)
{
	if(boundingVolumeShowMode & SHOW_SHORTEST_BOUNDING_VOLUME) 
	{
		Box box = mesh.Get()->GetBoundingBox();

		Vector3 width = Vector3(box.GetWidth() / boundingVolumeCornersLength, 0.0f, 0.0f);
		Vector3 height = Vector3(0.0f, box.GetHeight() / boundingVolumeCornersLength, 0.0f);
		Vector3 length = Vector3(0.0f, 0.0f, box.GetLength() / boundingVolumeCornersLength);
		GraphicsLine::_Draw(box.GetVertex1() * boundingVolumeScale, (box.GetVertex1() + width) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2() * boundingVolumeScale, (box.GetVertex2() - width) * boundingVolumeScale, boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex4() * boundingVolumeScale, (box.GetVertex4() + width) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3() * boundingVolumeScale, (box.GetVertex3() - width) * boundingVolumeScale, boundingVolumeColor);

				
		GraphicsLine::_Draw(box.GetVertex1() * boundingVolumeScale, (box.GetVertex1() - height) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex4() * boundingVolumeScale, (box.GetVertex4() + height) * boundingVolumeScale, boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex2() * boundingVolumeScale, (box.GetVertex2() - height) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3() * boundingVolumeScale, (box.GetVertex3() + height) * boundingVolumeScale, boundingVolumeColor);


		GraphicsLine::_Draw(box.GetVertex1() * boundingVolumeScale, (box.GetVertex1() - length) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2() * boundingVolumeScale, (box.GetVertex2() - length) * boundingVolumeScale, boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex4() * boundingVolumeScale, (box.GetVertex4() - length) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3() * boundingVolumeScale, (box.GetVertex3() - length) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex5() * boundingVolumeScale, (box.GetVertex5() + width) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6() * boundingVolumeScale, (box.GetVertex6() - width) * boundingVolumeScale, boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex8() * boundingVolumeScale, (box.GetVertex8() + width) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7() * boundingVolumeScale, (box.GetVertex7() - width) * boundingVolumeScale, boundingVolumeColor);

			
		GraphicsLine::_Draw(box.GetVertex5() * boundingVolumeScale, (box.GetVertex5() - height) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex8() * boundingVolumeScale, (box.GetVertex8() + height) * boundingVolumeScale, boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex6() * boundingVolumeScale, (box.GetVertex6() - height) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7() * boundingVolumeScale, (box.GetVertex7() + height) * boundingVolumeScale, boundingVolumeColor);


		GraphicsLine::_Draw(box.GetVertex5() * boundingVolumeScale, (box.GetVertex5() + length) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6() * boundingVolumeScale, (box.GetVertex6() + length) * boundingVolumeScale, boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex8() * boundingVolumeScale, (box.GetVertex8() + length) * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7() * boundingVolumeScale, (box.GetVertex7() + length) * boundingVolumeScale, boundingVolumeColor);
	}
	else if(boundingVolumeShowMode & SHOW_WIREFRAME_BOUNDING_VOLUME) 
	{
		Box box = mesh.Get()->GetBoundingBox();
		GraphicsLine::_Draw(box.GetVertex1() * boundingVolumeScale, box.GetVertex2() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2() * boundingVolumeScale, box.GetVertex3() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3() * boundingVolumeScale, box.GetVertex4() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex4() * boundingVolumeScale, box.GetVertex1() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex5() * boundingVolumeScale, box.GetVertex6() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6() * boundingVolumeScale, box.GetVertex7() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7() * boundingVolumeScale, box.GetVertex8() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex8() * boundingVolumeScale, box.GetVertex5() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex1() * boundingVolumeScale, box.GetVertex4() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex4() * boundingVolumeScale, box.GetVertex8() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex8() * boundingVolumeScale, box.GetVertex5() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex5() * boundingVolumeScale, box.GetVertex1() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2() * boundingVolumeScale, box.GetVertex3() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3() * boundingVolumeScale, box.GetVertex7() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7() * boundingVolumeScale, box.GetVertex6() * boundingVolumeScale, boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6() * boundingVolumeScale, box.GetVertex2() * boundingVolumeScale, boundingVolumeColor);
	}
}


void GraphicsSurface::DrawAlignedBoundingBox(void)
{
	if(boundingVolumeShowMode & SHOW_SHORTEST_BOUNDING_VOLUME) 
	{
		EndTransform();

		Box box = GetTransformedBox();

		Vector3 width = Vector3(box.GetWidth() / boundingVolumeCornersLength, 0.0f, 0.0f);
		Vector3 height = Vector3(0.0f, box.GetHeight() / boundingVolumeCornersLength, 0.0f);
		Vector3 length = Vector3(0.0f, 0.0f, box.GetLength() / boundingVolumeCornersLength);
		GraphicsLine::_Draw(box.GetVertex1(), (box.GetVertex1() + width), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2(), (box.GetVertex2() - width), boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex4(), (box.GetVertex4() + width), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3(), (box.GetVertex3() - width), boundingVolumeColor);


		GraphicsLine::_Draw(box.GetVertex1(), (box.GetVertex1() - height), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex4(), (box.GetVertex4() + height), boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex2(), (box.GetVertex2() - height), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3(), (box.GetVertex3() + height), boundingVolumeColor);


		GraphicsLine::_Draw(box.GetVertex1(), (box.GetVertex1() - length), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2(), (box.GetVertex2() - length), boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex4(), (box.GetVertex4() - length), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3(), (box.GetVertex3() - length), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex5(), (box.GetVertex5() + width), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6(), (box.GetVertex6() - width), boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex8(), (box.GetVertex8() + width), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7(), (box.GetVertex7() - width), boundingVolumeColor);

			
		GraphicsLine::_Draw(box.GetVertex5(), (box.GetVertex5() - height), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex8(), (box.GetVertex8() + height), boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex6(), (box.GetVertex6() - height), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7(), (box.GetVertex7() + height), boundingVolumeColor);


		GraphicsLine::_Draw(box.GetVertex5(), (box.GetVertex5() + length), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6(), (box.GetVertex6() + length), boundingVolumeColor);

		GraphicsLine::_Draw(box.GetVertex8(), (box.GetVertex8() + length), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7(), (box.GetVertex7() + length), boundingVolumeColor);

		BeginTransform();
	}
	else if(boundingVolumeShowMode & SHOW_WIREFRAME_BOUNDING_VOLUME) 
	{   
		EndTransform();

		Box box = GetTransformedBox();
		GraphicsLine::_Draw(box.GetVertex1(), box.GetVertex2(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2(), box.GetVertex3(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3(), box.GetVertex4(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex4(), box.GetVertex1(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex5(), box.GetVertex6(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6(), box.GetVertex7(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7(), box.GetVertex8(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex8(), box.GetVertex5(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex1(), box.GetVertex4(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex4(), box.GetVertex8(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex8(), box.GetVertex5(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex5(), box.GetVertex1(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex2(), box.GetVertex3(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex3(), box.GetVertex7(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex7(), box.GetVertex6(), boundingVolumeColor);
		GraphicsLine::_Draw(box.GetVertex6(), box.GetVertex2(), boundingVolumeColor);

		BeginTransform();
	}
}


 
 
void GraphicsSurface::DrawTransformedBoundingCircle(void)
{ 
	if(boundingVolumeShowMode & SHOW_SHORTEST_BOUNDING_VOLUME) 
	{ 
		Circle circle = mesh.Get()->GetBoundingCircle();  

		GraphicsCircle::_Draw(circle.center, circle.radius, 36, boundingVolumeColor);
	}
	else if(boundingVolumeShowMode & SHOW_WIREFRAME_BOUNDING_VOLUME) 
	{ 
		Circle circle = mesh.Get()->GetBoundingCircle();

		GraphicsCircle::_Draw(circle.center, circle.radius, 36, boundingVolumeColor);
	}
}



void GraphicsSurface::DrawAlignedBoundingCircle(void)
{
	if(boundingVolumeShowMode & SHOW_SHORTEST_BOUNDING_VOLUME) 
	{  
		EndTransform();

		Circle circle = GetTransformedCircle();  

		GraphicsCircle::_Draw(circle.center, circle.radius, 36, boundingVolumeColor);

		BeginTransform();
	}
	else if(boundingVolumeShowMode & SHOW_WIREFRAME_BOUNDING_VOLUME) 
	{ 
		EndTransform();

		Circle circle = GetTransformedCircle();  

		GraphicsCircle::_Draw(circle.center, circle.radius, 36, boundingVolumeColor);

		BeginTransform();
	}
}



void GraphicsSurface::DrawTransformedBoundingQuad(void)
{
	if(boundingVolumeShowMode & SHOW_SHORTEST_BOUNDING_VOLUME) 
	{ 
		Quad quad = mesh.Get()->GetBoundingQuad();  

		Vector3 width = Vector3(quad.GetWidth() / boundingVolumeCornersLength, 0.0f, 0.0f);
		Vector3 height = Vector3(0.0f, quad.GetHeight() / boundingVolumeCornersLength, 0.0f);

		Vector3 v1 = quad.GetVertex1(); v1.x *= boundingVolumeScale; v1.y *= boundingVolumeScale;
		Vector3 v2 = quad.GetVertex2(); v2.x *= boundingVolumeScale; v2.y *= boundingVolumeScale;
		Vector3 v3 = quad.GetVertex3(); v3.x *= boundingVolumeScale; v3.y *= boundingVolumeScale;
		Vector3 v4 = quad.GetVertex4(); v4.x *= boundingVolumeScale; v4.y *= boundingVolumeScale;
		GraphicsLine::_Draw(v1, (v1 + width), boundingVolumeColor);
		GraphicsLine::_Draw(v2, (v2 - width), boundingVolumeColor);

		GraphicsLine::_Draw(v4, (v4 + width), boundingVolumeColor);
		GraphicsLine::_Draw(v3, (v3 - width), boundingVolumeColor);


		GraphicsLine::_Draw(v1, (v1 + height), boundingVolumeColor);
		GraphicsLine::_Draw(v4, (v4 - height), boundingVolumeColor);

		GraphicsLine::_Draw(v2, (v2 + height), boundingVolumeColor);
		GraphicsLine::_Draw(v3, (v3 - height), boundingVolumeColor);
	}
	else if(boundingVolumeShowMode & SHOW_WIREFRAME_BOUNDING_VOLUME) 
	{ 
		Quad quad = mesh.Get()->GetBoundingQuad();

		Vector3 v1 = quad.GetVertex1(); v1.x *= boundingVolumeScale; v1.y *= boundingVolumeScale;
		Vector3 v2 = quad.GetVertex2(); v2.x *= boundingVolumeScale; v2.y *= boundingVolumeScale;
		Vector3 v3 = quad.GetVertex3(); v3.x *= boundingVolumeScale; v3.y *= boundingVolumeScale;
		Vector3 v4 = quad.GetVertex4(); v4.x *= boundingVolumeScale; v4.y *= boundingVolumeScale;
		GraphicsLine::_Draw(v1, v2, boundingVolumeColor);
		GraphicsLine::_Draw(v2, v3, boundingVolumeColor);
		GraphicsLine::_Draw(v3, v4, boundingVolumeColor);
		GraphicsLine::_Draw(v4, v1, boundingVolumeColor);
	}
}


void GraphicsSurface::DrawAlignedBoundingQuad(void)
{
	if(boundingVolumeShowMode & SHOW_SHORTEST_BOUNDING_VOLUME) 
	{  
		EndTransform();

		Quad quad = GetTransformedQuad();  

		Vector3 width = Vector3(quad.GetWidth() / boundingVolumeCornersLength, 0.0f, 0.0f);
		Vector3 height = Vector3(0.0f, quad.GetHeight() / boundingVolumeCornersLength, 0.0f);
		GraphicsLine::_Draw(quad.GetVertex1(), (quad.GetVertex1() + width), boundingVolumeColor);
		GraphicsLine::_Draw(quad.GetVertex2(), (quad.GetVertex2() - width), boundingVolumeColor);

		GraphicsLine::_Draw(quad.GetVertex4(), (quad.GetVertex4() + width), boundingVolumeColor);
		GraphicsLine::_Draw(quad.GetVertex3(), (quad.GetVertex3() - width), boundingVolumeColor);


		GraphicsLine::_Draw(quad.GetVertex1(), (quad.GetVertex1() + height), boundingVolumeColor);
		GraphicsLine::_Draw(quad.GetVertex4(), (quad.GetVertex4() - height), boundingVolumeColor);

		GraphicsLine::_Draw(quad.GetVertex2(), (quad.GetVertex2() + height), boundingVolumeColor);
		GraphicsLine::_Draw(quad.GetVertex3(), (quad.GetVertex3() - height), boundingVolumeColor);

		BeginTransform();
	}
	else if(boundingVolumeShowMode & SHOW_WIREFRAME_BOUNDING_VOLUME) 
	{ 
		EndTransform();

		Quad quad = GetTransformedQuad();
		GraphicsLine::_Draw(quad.GetVertex1(), quad.GetVertex2(), boundingVolumeColor);
		GraphicsLine::_Draw(quad.GetVertex2(), quad.GetVertex3(), boundingVolumeColor);
		GraphicsLine::_Draw(quad.GetVertex3(), quad.GetVertex4(), boundingVolumeColor);
		GraphicsLine::_Draw(quad.GetVertex4(), quad.GetVertex1(), boundingVolumeColor);

		BeginTransform();
	}
}


void GraphicsSurface::DrawBoundingVolume(void)
{  
	if(boundingVolumeShowMode & SHOW_BOUNDING_BOX)
	{
		if(boundingVolumeShowMode & SHOW_TRANSFORMED_BOUNDING_VOLUME)
		{
			DrawTransformedBoundingBox();
		}
		else 
		{ 
			DrawAlignedBoundingBox();
		} 
	}
	else if(boundingVolumeShowMode & SHOW_BOUNDING_QUAD)
	{
		if(boundingVolumeShowMode & SHOW_TRANSFORMED_BOUNDING_VOLUME)
		{
			DrawTransformedBoundingQuad();
		} 
		else 
		{   
			DrawAlignedBoundingQuad();
		}
	}
	else if(boundingVolumeShowMode & SHOW_BOUNDING_CIRCLE)
	{
		if(boundingVolumeShowMode & SHOW_TRANSFORMED_BOUNDING_VOLUME)
		{
			DrawTransformedBoundingCircle();
		} 
		else 
		{   
			DrawAlignedBoundingCircle();
		}
	}
}



void GraphicsSurface::DrawCulledPolygonalShape(void)
{
	if(staticEnable)
	{
		if(displayListIndex)
		{
			glCallList(displayListIndex);
		}
		else
		{
			displayListIndex = glGenLists(1);
			glNewList(displayListIndex, GL_COMPILE);
			
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
			glColor4fv(GetColorRGBA().GetData());

			glBegin(GL_TRIANGLES);
			for(int32 i = 0; i < mesh.Get()->indices.size(); i++)
			{
				glVertex3fv(mesh.Get()->vertices[mesh.Get()->indices[i]].GetData());
			}
			
			glEnd();

			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glEndList();
		}
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		glColor4fv(GetColorRGBA().GetData());
		glVertexPointer(Vector3::SIZE,
							GL_FLOAT,
							0,
							&mesh.Get()->vertices[0]);
		glDrawElements(GL_TRIANGLES,
					   mesh.Get()->indices.size(),
					   GL_UNSIGNED_INT,
					   &mesh.Get()->indices[0]);

		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

 

void GraphicsSurface::BindMesh(Mesh* _mesh)
{
	if(mesh.Get() != _mesh)
	{
		if(mesh.IsNotEmpty())
		{
			mesh.Get()->Disconnect(Mesh::DESTROY_MESH_MESSAGE, this);
			mesh.Get()->Disconnect(Mesh::INIT_MESH_MESSAGE, this);
				
			MeshIsDestroyed();
			mesh.Clear();
		}
			
		if(_mesh)
		{
			mesh.Attach(_mesh);
			mesh.Get()->Connect(Mesh::DESTROY_MESH_MESSAGE, this, Caller<>(this, &GraphicsSurface::MeshIsDestroyed));
			mesh.Get()->Connect(Mesh::INIT_MESH_MESSAGE, this, Caller<>(this, &GraphicsSurface::MeshIsCreated));

			if(_mesh->IsExist())
			{
				MeshIsCreated();
			}
			if(IsReceiversExist(CHANGE_MESH_MESSAGE)) { SendPackage(CHANGE_MESH_MESSAGE, mesh.Get()->GetName()); }
		}
	}
}


void GraphicsSurface::MeshIsDestroyed(void)
{
	ModifyGraphicsType(POLYGONAL_SURFACE_GRAPHICS_TYPE, false);
}




void GraphicsSurface::MeshIsCreated(void)
{
	ModifyGraphicsType(POLYGONAL_SURFACE_GRAPHICS_TYPE, true); 
}
	


void GraphicsSurface::_SetStippleEnable(bool _enable)
{
	if(_enable)
	{
		glEnable(GL_POLYGON_STIPPLE);
	}
	else
	{
		glDisable(GL_POLYGON_STIPPLE);
	}
}


bool GraphicsSurface::_IsStippleEnabled(void)
{
	GLboolean stipple = false;
	glGetBooleanv(GL_POLYGON_STIPPLE, &stipple);
	return (stipple == GL_TRUE);
}


void GraphicsSurface::_SetStipple(uint8* _stipple)
{
	glPolygonStipple(_stipple);
}



bool GraphicsSurface::SaveToFile(File& _file)
{
	if(PolygonalSurface::SaveToFile(_file))
	{ 
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}




bool GraphicsSurface::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool GraphicsSurface::SaveAsToFile(File& _file, StringANSI _name)
{
	if(PolygonalSurface::SaveAsToFile(_file, _name))
	{ 
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



void GraphicsSurface::SaveDerivedToFile(File& _file)
{
	_file.Write(GetColorRGBA());
	_file.Write(showEnable);
	_file.Write(IsBackFaceCullingEnabled());
	_file.Write(staticEnable);
}



bool GraphicsSurface::LoadFromFile(File& _file)
{
	if(PolygonalSurface::LoadFromFile(_file))
	{
		LoadDerivedFromFile(_file);
		return true;
	} 
	return false;
}
			  

void GraphicsSurface::LoadDerivedFromFile(File& _file)
{
	Vector4 color;
	_file.Read(color);
	SetColorRGBA(color);
	_file.Read(showEnable);
	bool backFaceCullingEnable;
	_file.Read(backFaceCullingEnable);
	SetBackFaceCullingEnable(backFaceCullingEnable);
	_file.Read(staticEnable);
}