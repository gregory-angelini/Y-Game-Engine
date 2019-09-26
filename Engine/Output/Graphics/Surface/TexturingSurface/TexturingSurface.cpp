#include "TexturingSurface.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Output/Graphics/Renderer/PixelBuffer.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>



TexturingSurface::TexturingSurface(void): flipXEnable(false), flipYEnable(false)
{
	AddClassProperty(TEXTURING_SURFACE_CLASS);

	intersectionMode |= TexturingSurface::CHECK_ONLY_COLORED_PIXEL;
}




TexturingSurface& TexturingSurface::operator = (const TexturingSurface& _object)
{
	if(this == &_object)
	{
		return *this;
	}

	GraphicsSurface::operator = (_object);
 
	flipXEnable = _object.flipXEnable;
	flipYEnable = _object.flipYEnable;

	BindFrontTexture(_object.GetFrontTexture());
	BindBackTexture(_object.GetBackTexture());
	BindFrontTexturedAnimation(_object.GetFrontTexturedAnimation());
	BindBackTexturedAnimation(_object.GetBackTexturedAnimation());

	return *this;
}



TexturingSurface::~TexturingSurface(void)
{
	Destroy(); 
	Destroying();
}


void TexturingSurface::Destroy(void)
{
	GraphicsSurface::Destroy();

	if(frontTexture.IsNotEmpty())
	{ 
		FrontTextureIsDestroyed();

		frontTexture.Get()->Disconnect(Texture2D::DESTROY_TEXTURE_MESSAGE, this);
		frontTexture.Get()->Disconnect(Texture2D::INIT_TEXTURE_MESSAGE, this);

		frontTexture.Clear();
	}
	else if(frontTexturedAnimation.IsNotEmpty())
	{
		FrontTexturedAnimationIsDestroyed();

		frontTexturedAnimation.Get()->Disconnect(Texture2D::DESTROY_TEXTURE_MESSAGE, this);
		frontTexturedAnimation.Get()->Disconnect(Texture2D::INIT_TEXTURE_MESSAGE, this);

		frontTexturedAnimation.Clear();
	}

	if(backTexture.IsNotEmpty())
	{
		BackTextureIsDestroyed(); 

		backTexture.Get()->Disconnect(Texture2D::DESTROY_TEXTURE_MESSAGE, this);
		backTexture.Get()->Disconnect(Texture2D::INIT_TEXTURE_MESSAGE, this);

		backTexture.Clear();
	}
	else if(backTexturedAnimation.IsNotEmpty())
	{
		BackTexturedAnimationIsDestroyed();

		backTexturedAnimation.Get()->Disconnect(Texture2D::DESTROY_TEXTURE_MESSAGE, this);
		backTexturedAnimation.Get()->Disconnect(Texture2D::INIT_TEXTURE_MESSAGE, this);

		backTexturedAnimation.Clear();
	}
}


Texture2D* TexturingSurface::GetFrontTexture(void)const
{
	return frontTexture.Get();
}


void TexturingSurface::BindMesh(Mesh* _mesh)
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
			mesh.Get()->Connect(Mesh::DESTROY_MESH_MESSAGE, this, Caller<>(this, &TexturingSurface::MeshIsDestroyed));
			mesh.Get()->Connect(Mesh::INIT_MESH_MESSAGE, this, Caller<>(this, &TexturingSurface::MeshIsCreated));

			if(_mesh->IsExist())
			{ 
				MeshIsCreated();
			}
			if(IsReceiversExist(CHANGE_MESH_MESSAGE)) { SendPackage(CHANGE_MESH_MESSAGE, mesh.Get()->GetName()); }
		}
	}
} 


void TexturingSurface::MeshIsCreated(void)
{
	GraphicsSurface::MeshIsCreated();  

	if(mesh.Get()->GetShapeType() == Mesh::QUAD)
	{ 
		if(IsGraphicsTypeEnabled(TexturingSurface::BACK_TEXTURING_GRAPHICS_TYPE))
		{ 
			CreateBackIndices();
			CalculateBackTexelsForQuad();
		}

		if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
		{ 
			CalculateFrontTexelsForQuad();
		}
	}
	else if(mesh.Get()->GetShapeType() == Mesh::BOX)
	{
		if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
		{ 
			CalculateFrontTexelsForBox();
		}
	}
}


void TexturingSurface::MeshIsDestroyed(void)
{ 
	GraphicsSurface::MeshIsDestroyed(); 
	if(IsGraphicsTypeEnabled(TexturingSurface::BACK_TEXTURING_GRAPHICS_TYPE))
	{
		DestroyBackIndices();
		DestroyBackTexels();
	}

	if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
	{
		DestroyFrontTexels();
	}
}


void TexturingSurface::FrontTextureRenderTargetRoleIsChanged(void)
{ 

	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{
		if(mesh.Get()->GetShapeType() == Mesh::QUAD)
		{
			CalculateFrontTexelsForQuad();
		}
	}
}


void TexturingSurface::BackTextureRenderTargetRoleIsChanged(void)
{ 

	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{
		if(mesh.Get()->GetShapeType() == Mesh::QUAD)
		{
			CalculateBackTexelsForQuad();
		}
	}
}
 

bool TexturingSurface::IsXFlipEnable(void)const
{
	return flipXEnable;
}


bool TexturingSurface::IsYFlipEnable(void)const
{
	return flipYEnable;
}


void TexturingSurface::SetFlipXEnable(bool _flipXEnable)
{
	if(flipXEnable != _flipXEnable)
	{
		flipXEnable = _flipXEnable;

		if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
		{
			if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
			{
				if(mesh.Get()->GetShapeType() == Mesh::QUAD)
				{
					CalculateFrontTexelsForQuad();
					CalculateBackTexelsForQuad();
				}
			}
		}
	}
}


void TexturingSurface::SetFlipYEnable(bool _flipYEnable)
{
	if(flipYEnable != _flipYEnable)
	{
		flipYEnable = _flipYEnable;

		if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
		{
			if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
			{
				if(mesh.Get()->GetShapeType() == Mesh::QUAD)
				{
					CalculateFrontTexelsForQuad();
					CalculateBackTexelsForQuad();
				}
			}
		}
	}
}


void TexturingSurface::CalculateFrontTexelsForQuad(void)
{
	if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
	{ 
		if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
		{
			if(frontTexturedAnimation.IsNotEmpty())
			{
				{
					int32 amountOfFrames = frontTexturedAnimation.Get()->GetAmountOfFrames();

					ResizeFrontTexels(amountOfFrames, 4);
					int32 i1, i2, i3, i4;

					if(flipXEnable && flipYEnable)
					{
						i1 = 2, i2 = 3, i3 = 0, i4 = 1;
					}
					else if(flipXEnable && !flipYEnable)
					{
						i1 = 1, i2 = 0, i3 = 3, i4 = 2;
					}
					else if(!flipXEnable && flipYEnable)
					{
						i1 = 3, i2 = 2, i3 = 1, i4 = 0;
					}
					else if(!flipXEnable && !flipYEnable)
					{
						i1 = 0, i2 = 1, i3 = 2, i4 = 3;
					}
					for(int32 i = 0; i < amountOfFrames; ++i)
					{
						Atlas2D::Frame* frameInfo = frontTexturedAnimation.Get()->GetAtlas()->GetFrameInfo(i);
						frontTexels[i][i1] = frameInfo->normalizedPosition;
						frontTexels[i][i2] = frameInfo->normalizedPosition + Vector2(frameInfo->normalizedSize.x, 0.0f);
						frontTexels[i][i3] = frameInfo->normalizedPosition + Vector2(frameInfo->normalizedSize.x, frameInfo->normalizedSize.y);
						frontTexels[i][i4] = frameInfo->normalizedPosition + Vector2(0.0f, frameInfo->normalizedSize.y);
					}
				}
			}
			else if(frontTexture.IsNotEmpty())
			{
				if(frontTexture.Get()->GetTypeOfTextureMap() == Texture2D::QUAD_TEXTURE_MAP)
				{
					ResizeFrontTexels(1, 4);

					float normalizedWidth;
					float normalizedHeight;

					if(frontTexture.Get()->IsRenderTarget())
					{
						normalizedWidth = 1.0f;
						normalizedHeight = 1.0f;
					}
					else
					{
						normalizedWidth = frontTexture.Get()->GetImageSize().x / frontTexture.Get()->GetTextureSize().x;
						normalizedHeight = frontTexture.Get()->GetImageSize().y / frontTexture.Get()->GetTextureSize().y;
					}
					int32 i1, i2, i3, i4;

					if(flipXEnable && flipYEnable)
					{
						i1 = 2, i2 = 3, i3 = 0, i4 = 1;
					}
					else if(flipXEnable && !flipYEnable)
					{
						i1 = 1, i2 = 0, i3 = 3, i4 = 2;
					}
					else if(!flipXEnable && flipYEnable)
					{
						i1 = 3, i2 = 2, i3 = 1, i4 = 0;
					}
					else if(!flipXEnable && !flipYEnable)
					{
						i1 = 0, i2 = 1, i3 = 2, i4 = 3;
					}
					frontTexels[0][i1] = Vector2(0.0f, 0.0f);
					frontTexels[0][i2] = Vector2(normalizedWidth, 0.0f);
					frontTexels[0][i3] = Vector2(normalizedWidth, normalizedHeight);
					frontTexels[0][i4] = Vector2(0.0f, normalizedHeight);
				}
			}
		}
	}
}


 
void TexturingSurface::CalculateFrontTexelsForBox(void)
{
	if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
	{ 
		if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
		{
			if(frontTexturedAnimation.IsNotEmpty())
			{
				if(frontTexture.Get()->GetTypeOfTextureMap() == Texture2D::BOX_TEXTURE_MAP)
				{ 
				}
			}
			else if(frontTexture.IsNotEmpty())
			{
				if(frontTexture.Get()->GetTypeOfTextureMap() == Texture2D::BOX_TEXTURE_MAP)
				{  
					ResizeFrontTexels(1, 6 * 4);

					Vector3 boxSize = frontTexture.Get()->GetBoxTextureMapSize();
					float offset = 0.0f;
					frontTexels[0][0] = Vector2(0.0f, 0.0f);
					frontTexels[0][1] = Vector2(boxSize.x / frontTexture.Get()->GetTextureSize().x, 0.0f);
					frontTexels[0][2] = Vector2(boxSize.x / frontTexture.Get()->GetTextureSize().x, boxSize.y / frontTexture.Get()->GetTextureSize().y);
					frontTexels[0][3] = Vector2(0.0f, boxSize.y / frontTexture.Get()->GetTextureSize().y);

					offset += boxSize.y / frontTexture.Get()->GetTextureSize().y;
					frontTexels[0][5] = Vector2(0.0f, offset);
					frontTexels[0][4] = Vector2(boxSize.x / frontTexture.Get()->GetTextureSize().x, offset);
					frontTexels[0][7] = Vector2(boxSize.x / frontTexture.Get()->GetTextureSize().x, offset + boxSize.y / frontTexture.Get()->GetTextureSize().y);
					frontTexels[0][6] = Vector2(0.0f, offset + boxSize.y / frontTexture.Get()->GetTextureSize().y);

					offset += boxSize.y / frontTexture.Get()->GetTextureSize().y;
					frontTexels[0][9] = Vector2(0.0f, offset);
					frontTexels[0][8] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset);
					frontTexels[0][11] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset + boxSize.y / frontTexture.Get()->GetTextureSize().y);
					frontTexels[0][10] = Vector2(0.0f, offset + boxSize.y / frontTexture.Get()->GetTextureSize().y);

					offset += boxSize.y / frontTexture.Get()->GetTextureSize().y;
					frontTexels[0][13] = Vector2(0.0f, offset);
					frontTexels[0][12] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset);
					frontTexels[0][15] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset + boxSize.y / frontTexture.Get()->GetTextureSize().y);
					frontTexels[0][14] = Vector2(0.0f, offset + boxSize.y / frontTexture.Get()->GetTextureSize().y);

					offset += boxSize.y / frontTexture.Get()->GetTextureSize().y;
					frontTexels[0][17] = Vector2(0.0f, offset);
					frontTexels[0][16] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset);
					frontTexels[0][19] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset + boxSize.x / frontTexture.Get()->GetTextureSize().y);
					frontTexels[0][18] = Vector2(0.0f, offset + boxSize.x / frontTexture.Get()->GetTextureSize().y);

					offset += boxSize.x / frontTexture.Get()->GetTextureSize().y;
					frontTexels[0][20] = Vector2(0.0f, offset);
					frontTexels[0][21] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset);
					frontTexels[0][22] = Vector2(boxSize.z / frontTexture.Get()->GetTextureSize().x, offset + boxSize.x / frontTexture.Get()->GetTextureSize().y);
					frontTexels[0][23] = Vector2(0.0f, offset + boxSize.x / frontTexture.Get()->GetTextureSize().y);
				} 
			}
		}
	}
}



void TexturingSurface::CalculateBackTexelsForQuad(void)
{  
	if(IsGraphicsTypeEnabled(TexturingSurface::BACK_TEXTURING_GRAPHICS_TYPE))
	{
		if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
		{ 
			if(backTexturedAnimation.IsNotEmpty())
			{
				int32 amountOfFrames = backTexturedAnimation.Get()->GetAmountOfFrames();

				ResizeBackTexels(amountOfFrames, mesh.Get()->GetAmountOfVertices());
				int32 i1, i2, i3, i4;

				if(flipXEnable && flipYEnable)
				{
					i1 = 2, i2 = 3, i3 = 0, i4 = 1;
				}
				else if(flipXEnable && !flipYEnable)
				{
					i1 = 1, i2 = 0, i3 = 3, i4 = 2;
				}
				else if(!flipXEnable && flipYEnable)
				{
					i1 = 3, i2 = 2, i3 = 1, i4 = 0;
				}
				else if(!flipXEnable && !flipYEnable)
				{
					i1 = 0, i2 = 1, i3 = 2, i4 = 3;
				}
				for(int32 i = 0; i < amountOfFrames; ++i)
				{
					Atlas2D::Frame* frameInfo = backTexturedAnimation.Get()->GetAtlas()->GetFrameInfo(i);
					backTexels[i][i2] = frameInfo->normalizedPosition;
					backTexels[i][i1] = frameInfo->normalizedPosition + Vector2(frameInfo->normalizedSize.x, 0.0f);
					backTexels[i][i4] = frameInfo->normalizedPosition + Vector2(frameInfo->normalizedSize.x, frameInfo->normalizedSize.y);
					backTexels[i][i3] = frameInfo->normalizedPosition + Vector2(0.0f, frameInfo->normalizedSize.y);
				}
			}
			else if(backTexture.IsNotEmpty())
			{ 
				ResizeBackTexels(1, mesh.Get()->GetAmountOfVertices());

				float normalizedWidth;
				float normalizedHeight;

				if(backTexture.Get()->IsRenderTarget())
				{
					normalizedWidth = 1.0f;
					normalizedHeight = 1.0f;
				}
				else
				{
					normalizedWidth = backTexture.Get()->GetImageSize().x / backTexture.Get()->GetTextureSize().x;
					normalizedHeight = backTexture.Get()->GetImageSize().y / backTexture.Get()->GetTextureSize().y;
				}
				int32 i1, i2, i3, i4;

				if(flipXEnable && flipYEnable)
				{
					i1 = 2, i2 = 3, i3 = 0, i4 = 1;
				}
				else if(flipXEnable && !flipYEnable)
				{
					i1 = 1, i2 = 0, i3 = 3, i4 = 2;
				}
				else if(!flipXEnable && flipYEnable)
				{
					i1 = 3, i2 = 2, i3 = 1, i4 = 0;
				}
				else if(!flipXEnable && !flipYEnable)
				{
					i1 = 0, i2 = 1, i3 = 2, i4 = 3;
				}
				backTexels[0][i2] = Vector2(0.0f, 0.0f);
				backTexels[0][i1] = Vector2(normalizedWidth, 0.0f);
				backTexels[0][i4] = Vector2(normalizedWidth, normalizedHeight);
				backTexels[0][i3] = Vector2(0.0f, normalizedHeight);
			} 
		}
	}
}



void TexturingSurface::ResizeFrontTexels(int32 _rows, int32 _columns)
{
	if(_rows > 0 && _columns > 0)
	{
		frontTexels.resize(_rows);

		if(_columns > 0)
		{
			for(int32 i = 0; i < _rows; ++i)
			{
				frontTexels[i].resize(_columns, Vector2());
			}
		}
		else if(_columns == 0)
		{
			for(int32 i = 0; i < _rows; ++i)
			{
				frontTexels[i].clear();
			}
		}
	}
}


 
void TexturingSurface::ResizeBackTexels(int32 _rows, int32 _columns)
{
	if(_rows > 0 && _columns > 0)
	{
		backTexels.resize(_rows);

		if(_columns > 0)
		{
			for(int32 i = 0; i < _rows; ++i)
			{
				backTexels[i].resize(_columns, Vector2());
			}
		}
		else if(_columns == 0)
		{
			for(int32 i = 0; i < _rows; ++i)
			{
				backTexels[i].clear();
			}
		}
	}
}


 

vector<vector<Vector2>>& TexturingSurface::GetFrontTexels(void)
{
	return frontTexels;
}



vector<vector<Vector2>>& TexturingSurface::GetBackTexels(void)
{
	return backTexels;
}


void TexturingSurface::CreateBackIndices(void)
{
	backFaceIndices = mesh.Get()->indices;
	for(int32 j = 0; j < backFaceIndices.size(); ++j)
	{
		for(int32 i = 0; i + j < backFaceIndices.size() - 1; ++i)
		{
			if(i + 1 < backFaceIndices.size())
			{
				uint32 index = backFaceIndices[i + 1];
				backFaceIndices[i + 1] = backFaceIndices[i];
				backFaceIndices[i] = index;
			}
		}
	}
}


 
void TexturingSurface::DestroyFrontTexels(void)
{
	frontTexels.clear();
}



void TexturingSurface::DestroyBackTexels(void)
{
	backTexels.clear();
}


void TexturingSurface::DestroyBackIndices(void)
{
	backFaceIndices.clear();
}




void TexturingSurface::SetIntersectionMode(int32 _mode, bool _enable)
{
	GraphicsSurface::SetIntersectionMode(_mode, _enable);
	
	if(_mode == CHECK_ONLY_COLORED_PIXEL)
	{
		if(_enable) 
		{
			intersectionMode |= CHECK_ONLY_COLORED_PIXEL;
		}
		else
		{
			intersectionMode &= ~CHECK_ONLY_COLORED_PIXEL;
		}
	}
}


void TexturingSurface::ModifyGraphicsType(int64 _property, bool _enable)
{
	GraphicsSurface::ModifyGraphicsType(_property, _enable);

	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{
		if(IsGraphicsTypeEnabled(GraphicsSurface::SHOW_BOUNDING_VOLUME_GRAPHICS_TYPE))
		{
			if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
			{
				if(IsGraphicsTypeEnabled(GraphicsSurface::BACK_FACE_CULLING_GRAPHICS_TYPE))
				{ 
					draw = Caller<>(dynamic_cast<TexturingSurface*>(this), &TexturingSurface::DrawCulledTexturingShapeWithBoundingVolume);
				}
				else
				{
					if(IsGraphicsTypeEnabled(TexturingSurface::BACK_TEXTURING_GRAPHICS_TYPE))
					{
						draw = Caller<>(dynamic_cast<TexturingSurface*>(this), &TexturingSurface::DrawFrontBackTexturingShapeWithBoundingVolume);
					}
					else
					{ 
						draw = Caller<>(dynamic_cast<TexturingSurface*>(this), &TexturingSurface::DrawTexturingShapeWithBoundingVolume);
					}
				}
			}
		}
		else
		{
			if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
			{
				if(IsGraphicsTypeEnabled(GraphicsSurface::BACK_FACE_CULLING_GRAPHICS_TYPE))
				{
					draw = Caller<>(dynamic_cast<TexturingSurface*>(this), &TexturingSurface::DrawCulledTexturingShape); 
				}
				else
				{ 
					if(IsGraphicsTypeEnabled(TexturingSurface::BACK_TEXTURING_GRAPHICS_TYPE))
					{
						draw = Caller<>(dynamic_cast<TexturingSurface*>(this), &TexturingSurface::DrawFrontBackTexturingShape);
					}
					else
					{
						draw = Caller<>(dynamic_cast<TexturingSurface*>(this), &TexturingSurface::DrawTexturingShape);
					}
				}
			}
		}
	}
}



void TexturingSurface::DrawCulledTexturingShape(void)
{
	if(staticEnable)
	{
		if(displayListIndex)
		{
			glCallList(displayListIndex);
		}
		else
		{
			if(frontTexture.Get()->IsRenderTarget()) { return; }

			displayListIndex = glGenLists(1);
			glNewList(displayListIndex, GL_COMPILE);
			
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glColor4fv(GetColorRGBA().GetData());
			if(frontTexture.IsNotEmpty())
			{ 
				glBindTexture(GL_TEXTURE_2D, frontTexture.Get()->textureId);
			} 

			glBegin(GL_TRIANGLES);
			for(int32 i = 0; i < mesh.Get()->indices.size(); i++)
			{
				glTexCoord2fv(frontTexels[0][mesh.Get()->indices[i]].GetData());
				glVertex3fv(mesh.Get()->vertices[mesh.Get()->indices[i]].GetData());
			}
			
			glEnd();

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnableClientState(GL_VERTEX_ARRAY);

			glEndList();
		}
	}
	else
	{
		glColor4fv(GetColorRGBA().GetData());
		if(frontTexture.IsNotEmpty())
		{ 
			glBindTexture(GL_TEXTURE_2D, frontTexture.Get()->textureId);
			
			if(frontTexture.Get()->IsRenderTarget()) { Renderer::wglBindTexImageARBExt(frontTexture.Get()->pixelBuffer->buffer, WGL_FRONT_LEFT_ARB); }
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, frontTexturedAnimation.Get()->currentFrameInfo->texture->textureId);
		}
		glVertexPointer(Vector3::SIZE,
						GL_FLOAT,
						0,
						&mesh.Get()->vertices[0]);
		glTexCoordPointer(Vector2::SIZE,
						  GL_FLOAT,
						  0,
						  &frontTexels[frontTexture.IsNotEmpty() ? 0 : frontTexturedAnimation.Get()->GetCurrentFrame()][0]);
		glDrawElements(GL_TRIANGLES,
					   mesh.Get()->indices.size(),
					   GL_UNSIGNED_INT,
					   &mesh.Get()->indices[0]);
		if(frontTexture.IsNotEmpty())
		{ 
			if(frontTexture.Get()->IsRenderTarget()) { Renderer::wglReleaseTexImageARBExt(frontTexture.Get()->pixelBuffer->buffer, WGL_FRONT_LEFT_ARB); }
		}
	}
} 




 
void TexturingSurface::DrawCulledTexturingShapeWithBoundingVolume(void)
{
	DrawCulledTexturingShape();

	DrawBoundingVolume();
}




void TexturingSurface::DrawFrontBackTexturingShapeWithBoundingVolume(void)
{
	DrawFrontBackTexturingShape();

	DrawBoundingVolume();
}


void TexturingSurface::DrawTexturingShapeWithBoundingVolume(void)
{
	glDisable(GL_CULL_FACE);

	DrawCulledTexturingShape();

	glEnable(GL_CULL_FACE);

	DrawBoundingVolume();
}



void TexturingSurface::DrawFrontBackTexturingShape(void)
{
	glColor4fv(GetColorRGBA().GetData());

	DrawFrontTexturingShape();
	DrawBackTexturingShape();
}

 

void TexturingSurface::DrawFrontTexturingShape(void)
{
	if(frontTexture.IsNotEmpty())
	{
		glBindTexture(GL_TEXTURE_2D, frontTexture.Get()->textureId);

		if(frontTexture.Get()->IsRenderTarget()) { Renderer::wglBindTexImageARBExt(frontTexture.Get()->pixelBuffer->buffer, WGL_FRONT_LEFT_ARB); }
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, frontTexturedAnimation.Get()->currentFrameInfo->texture->textureId);
	}
	glVertexPointer(Vector3::SIZE,
				    GL_FLOAT,
					0,
					&mesh.Get()->vertices[0]);
	glTexCoordPointer(Vector2::SIZE,
					  GL_FLOAT,
					  0,
					  &frontTexels[frontTexture.IsNotEmpty() ? 0 : frontTexturedAnimation.Get()->GetCurrentFrame()][0]);
	glDrawElements(GL_TRIANGLES,
				   mesh.Get()->indices.size(),
				   GL_UNSIGNED_INT,
				   &mesh.Get()->indices[0]);
	if(frontTexture.IsNotEmpty())
	{ 
		if(frontTexture.Get()->IsRenderTarget()) { Renderer::wglReleaseTexImageARBExt(frontTexture.Get()->pixelBuffer->buffer, WGL_FRONT_LEFT_ARB); }
	}
} 

 
void TexturingSurface::DrawBackTexturingShape(void)
{
	if(backTexture.IsNotEmpty())
	{ 
		glBindTexture(GL_TEXTURE_2D, backTexture.Get()->textureId);

		if(backTexture.Get()->IsRenderTarget()) { Renderer::wglBindTexImageARBExt(backTexture.Get()->pixelBuffer->buffer, WGL_FRONT_LEFT_ARB); }
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, backTexturedAnimation.Get()->currentFrameInfo->texture->textureId);
	}
	glVertexPointer(Vector3::SIZE,
				    GL_FLOAT,
					0,
					&mesh.Get()->vertices[0]);
	glTexCoordPointer(Vector2::SIZE,
					  GL_FLOAT,
					  0,
					  &backTexels[backTexture.IsNotEmpty() ? 0 : backTexturedAnimation.Get()->GetCurrentFrame()][0]);
	glDrawElements(GL_TRIANGLES,
				   backFaceIndices.size(),
				   GL_UNSIGNED_INT,
				   &backFaceIndices[0]);
	if(backTexture.IsNotEmpty())
	{ 
		if(backTexture.Get()->IsRenderTarget()) { Renderer::wglReleaseTexImageARBExt(backTexture.Get()->pixelBuffer->buffer, WGL_FRONT_LEFT_ARB); }
	} 
} 



void TexturingSurface::DrawTexturingShape(void)
{
	glDisable(GL_CULL_FACE);

	DrawCulledTexturingShape();

	glEnable(GL_CULL_FACE);
}




bool TexturingSurface::IsIntersected(Line _line, Vector2* _pixelPosition, RGBA* _pixelColor)
{
	if(mesh.IsNotEmpty() && mesh.Get()->IsExist())
	{
		if(mesh.Get()->GetShapeType() == Mesh::QUAD)
		{ 
			if(IsGraphicsTypeEnabled(TexturingSurface::TEXTURED_SURFACE_GRAPHICS_TYPE))
			{
				Vector3 intersectionPoint;

				if(IsIntersected(_line, &intersectionPoint) != -1)
				{
					Texture2D* currentTexture;
					float textureWidth, textureHeight;
					Vector2 offset;

					if(frontTexture.IsNotEmpty() && frontTexture.Get()->IsExist())
					{ 
						currentTexture = frontTexture.Get();
						textureWidth = currentTexture->GetImageSize().x;
						textureHeight = currentTexture->GetImageSize().y;
					}
					else if(frontTexturedAnimation.IsNotEmpty() && frontTexturedAnimation.Get()->IsExist())
					{
						Atlas2D::Frame* frame = frontTexturedAnimation.Get()->GetCurrentFrameInfo();
							
						if(frame)
						{
							currentTexture = &frame->GetTexture();
							textureWidth = frame->GetSizeInPixels().x;
							textureHeight = frame->GetSizeInPixels().y;
							offset = frame->GetPositionInPixels();
						}
						else { return false; }
					}

					if(IsGraphicsTypeEnabled(TexturingSurface::BACK_TEXTURING_GRAPHICS_TYPE))
					{ 
						Triangle triangle;

						if(mesh.Get()->GetTriangle(0, triangle))
						{
							if(intersectionMode & AbstractSurface::APPLY_TRANSFORMATION) { triangle.Transform(GetTransformation()); }
				
							if(_line.GetDirection().GetDot(triangle.GetNormal()) > 0)
							{
								if(backTexture.IsNotEmpty() && backTexture.Get()->IsExist())
								{
									currentTexture = backTexture.Get();
									textureWidth = currentTexture->GetImageSize().x;
									textureHeight = currentTexture->GetImageSize().y;
									offset = Vector2();
								}
								else if(backTexturedAnimation.IsNotEmpty() && backTexturedAnimation.Get()->IsExist())
								{
									Atlas2D::Frame* frame = backTexturedAnimation.Get()->GetCurrentFrameInfo();
							
									if(frame)
									{
										currentTexture = &frame->GetTexture();
										textureWidth = frame->GetSizeInPixels().x;
										textureHeight = frame->GetSizeInPixels().y;
										offset = frame->GetPositionInPixels();
									}
									else { return false; }
								}
							}
						}
					} 

					Vector3 localPosition;

					if(intersectionMode & AbstractSurface::APPLY_TRANSFORMATION)
					{
						intersectionPoint = (Quaternion::_GetConjugate(GetRotation())).RotatePointAroundPivot(intersectionPoint, GetPosition());
						
						localPosition = intersectionPoint - GetPosition();
						localPosition.x *= 1.0f / GetScale().x;
						localPosition.y *= 1.0f / GetScale().y;
						localPosition.z *= 1.0f / GetScale().z;
					}
					else
					{
						localPosition = intersectionPoint - GetPosition();
					}

					float boundingVolumeWidth =  mesh.Get()->GetBoundingQuad().GetWidth();
					float boundingVolumeHeight = mesh.Get()->GetBoundingQuad().GetHeight();
					Vector2 pixelPosition(localPosition.x / boundingVolumeWidth * textureWidth, 
										  localPosition.y / boundingVolumeHeight * textureHeight);

					pixelPosition += Vector2(textureWidth, textureHeight) / 2.0f;


					RGBA pixelColor;

					if(currentTexture->GetPixel(offset + pixelPosition, pixelColor))
					{
						if(intersectionMode & TexturingSurface::CHECK_ONLY_COLORED_PIXEL)
						{
							if(pixelColor.alpha > 0)
							{
								if(_pixelPosition) { (*_pixelPosition) = pixelPosition; }
								if(_pixelColor) { (*_pixelColor) = pixelColor; }
								return true;
							}
							return false;
						}
						if(_pixelPosition) { (*_pixelPosition) = pixelPosition; }
						if(_pixelColor) { (*_pixelColor) = pixelColor; }
						return true;
					}
				}
			}
		}
	}
	return false;
}



void TexturingSurface::BindFrontTexture(Texture2D* _texture)
{
	if(frontTexture.Get() != _texture)
	{
		if(backTexture.IsNotEmpty() && backTexture.Get() == _texture) { return; }

		if(_texture && frontTexturedAnimation.IsNotEmpty())
		{
			BindFrontTexturedAnimation(NIL);
		}

		if(frontTexture.IsNotEmpty())
		{
			frontTexture.Get()->Disconnect(Texture2D::DESTROY_TEXTURE_MESSAGE, this);
			frontTexture.Get()->Disconnect(Texture2D::INIT_TEXTURE_MESSAGE, this);
			frontTexture.Get()->Disconnect(Texture2D::CHANGE_RENDER_TARGET_ROLE_MESSAGE, this);
			frontTexture.Get()->Disconnect(PolygonalSurface::CHANGE_HITBOX_MESSAGE, this);
			frontTexture.Get()->Disconnect(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE, this);
				
			FrontTextureIsDestroyed();
			frontTexture.Clear();
		} 
			
		if(_texture)
		{
			frontTexture.Attach(_texture);
			frontTexture.Get()->Connect(Texture2D::DESTROY_TEXTURE_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTextureIsDestroyed));
			frontTexture.Get()->Connect(Texture2D::INIT_TEXTURE_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTextureIsCreated));
			frontTexture.Get()->Connect(Texture2D::CHANGE_RENDER_TARGET_ROLE_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTextureRenderTargetRoleIsChanged));
			frontTexture.Get()->Connect(PolygonalSurface::CHANGE_HITBOX_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTextureHitboxIsChanged));
			frontTexture.Get()->Connect(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTextureHitboxEnableIsChanged));

			if(_texture->IsExist())
			{   
				FrontTextureIsCreated();
			}
			if(IsReceiversExist(CHANGE_FRONT_TEXTURE_MESSAGE)) { SendPackage(CHANGE_FRONT_TEXTURE_MESSAGE, frontTexture.Get()->GetName()); }
		}  
	} 
}


void TexturingSurface::FrontTextureHitboxIsChanged(StringANSI _name)
{
	if(frontTexture.Get()->IsHitboxEnabled())
	{ 
		BindHitbox(frontTexture.Get()->GetHitbox());
	}
}


void TexturingSurface::FrontTextureHitboxEnableIsChanged(bool _enable)
{
	if(_enable)
	{
		BindHitbox(frontTexture.Get()->GetHitbox());
	}
	else
	{
		if(hitbox.Get() == frontTexture.Get()->GetHitbox()) { BindHitbox(NIL); }
	}
} 


void TexturingSurface::FrontTextureIsDestroyed(void)
{
	ModifyGraphicsType(TEXTURED_SURFACE_GRAPHICS_TYPE, false); 
	
	if(frontTexture.Get()->IsHitboxEnabled())
	{
		if(hitbox.Get() == frontTexture.Get()->GetHitbox()) { BindHitbox(NIL); }
	}

	DestroyFrontTexels();
}
 


void TexturingSurface::FrontTextureIsCreated(void)
{
	ModifyGraphicsType(TEXTURED_SURFACE_GRAPHICS_TYPE, true); 

	if(frontTexture.Get()->IsHitboxEnabled())
	{ 
		BindHitbox(frontTexture.Get()->GetHitbox());
	} 

	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{
		if(mesh.Get()->GetShapeType() == Mesh::QUAD)
		{
			CalculateFrontTexelsForQuad();
		}
		else if(mesh.Get()->GetShapeType() == Mesh::BOX)
		{
			CalculateFrontTexelsForBox();
		}
	}
}


void TexturingSurface::BindFrontTexturedAnimation(TexturedAnimation* _texturedAnimation)
{ 
	if(frontTexturedAnimation.Get() != _texturedAnimation)
	{
		if(backTexturedAnimation.IsNotEmpty() && backTexturedAnimation.Get() == _texturedAnimation) { return; }

		if(_texturedAnimation && frontTexture.IsNotEmpty())
		{
			BindFrontTexture(NIL);
		}

		if(frontTexturedAnimation.IsNotEmpty())
		{
			frontTexturedAnimation.Get()->Disconnect(TexturedAnimation::DESTROY_TEXTURED_ANIMATION_MESSAGE, this);
			frontTexturedAnimation.Get()->Disconnect(TexturedAnimation::INIT_TEXTURED_ANIMATION_MESSAGE, this);
			frontTexturedAnimation.Get()->Disconnect(PolygonalSurface::CHANGE_HITBOX_MESSAGE, this);
			frontTexturedAnimation.Get()->Disconnect(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE, this);
				
			FrontTexturedAnimationIsDestroyed();
			frontTexturedAnimation.Clear();
		} 
			
		if(_texturedAnimation)
		{
			frontTexturedAnimation.Attach(_texturedAnimation);
			frontTexturedAnimation.Get()->Connect(TexturedAnimation::DESTROY_TEXTURED_ANIMATION_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTexturedAnimationIsDestroyed));
			frontTexturedAnimation.Get()->Connect(TexturedAnimation::INIT_TEXTURED_ANIMATION_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTexturedAnimationIsCreated));
			frontTexturedAnimation.Get()->Connect(PolygonalSurface::CHANGE_HITBOX_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTexturedAnimationHitboxIsChanged));
			frontTexturedAnimation.Get()->Connect(PolygonalSurface::CHANGE_HITBOX_ENABLE_MESSAGE, this, Caller<>(this, &TexturingSurface::FrontTexturedAnimationHitboxEnableIsChanged));
 
			if(_texturedAnimation->IsExist())
			{ 
				FrontTexturedAnimationIsCreated();
			} 
			if(IsReceiversExist(CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE)) { SendPackage(CHANGE_FRONT_TEXTURED_ANIMATION_MESSAGE, frontTexturedAnimation.Get()->GetName()); }
		} 
	} 
}


void TexturingSurface::FrontTexturedAnimationHitboxIsChanged(StringANSI _name)
{
	if(frontTexturedAnimation.Get()->IsHitboxEnabled())
	{ 
		BindHitbox(frontTexturedAnimation.Get()->GetHitbox());
	}
}


void TexturingSurface::FrontTexturedAnimationHitboxEnableIsChanged(bool _enable)
{
	if(_enable)
	{
		BindHitbox(frontTexturedAnimation.Get()->GetHitbox());
	}
	else
	{  
		if(frontTexturedAnimation.Get()->IsExist())
		{
			if(hitbox.Get() == frontTexturedAnimation.Get()->GetCurrentFrameInfo()->GetHitbox()) 
			{ 
				BindHitbox(NIL);
			}
		}
	}
}


void TexturingSurface::FrontTexturedAnimationIsDestroyed(void)
{
	ModifyGraphicsType(TEXTURED_SURFACE_GRAPHICS_TYPE, false); 

	frontTexturedAnimation.Get()->Disconnect(TexturedAnimation::CHANGE_ANIMATION_FRAME_MESSAGE, this);

	if(frontTexturedAnimation.Get()->IsHitboxEnabled())
	{
		if(frontTexturedAnimation.Get()->IsExist())
		{
			if(hitbox.Get() == frontTexturedAnimation.Get()->GetCurrentFrameInfo()->GetHitbox()) 
			{ 
				BindHitbox(NIL);
			}
		} 
	}

	DestroyFrontTexels();
}


void TexturingSurface::FrontTexturedAnimationIsCreated(void)
{ 
	ModifyGraphicsType(TEXTURED_SURFACE_GRAPHICS_TYPE, true); 

	frontTexturedAnimation.Get()->Connect(TexturedAnimation::CHANGE_ANIMATION_FRAME_MESSAGE, this, Caller<>(this, &TexturingSurface::CurrentFrameIsChanged));
	
	if(frontTexturedAnimation.Get()->IsHitboxEnabled())
	{
		BindHitbox(frontTexturedAnimation.Get()->GetHitbox());
	}

	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{ 
		if(mesh.Get()->GetShapeType() == Mesh::QUAD)
		{
			CalculateFrontTexelsForQuad();
		}
	    else if(mesh.Get()->GetShapeType() == Mesh::BOX)
		{
			CalculateFrontTexelsForBox();
		}
	}
}


TexturedAnimation* TexturingSurface::GetFrontTexturedAnimation(void)const
{
	return frontTexturedAnimation.Get();
}


void TexturingSurface::BindBackTexture(Texture2D* _texture)
{
	if(backTexture.Get() != _texture)
	{
		if(frontTexture.IsNotEmpty() && frontTexture.Get() == _texture) { return; }

		if(_texture && backTexturedAnimation.IsNotEmpty())
		{
			BindBackTexturedAnimation(NIL);
		}

		if(backTexture.IsNotEmpty())
		{
			backTexture.Get()->Disconnect(Texture2D::DESTROY_TEXTURE_MESSAGE, this);
			backTexture.Get()->Disconnect(Texture2D::INIT_TEXTURE_MESSAGE, this);
			backTexture.Get()->Disconnect(Texture2D::CHANGE_RENDER_TARGET_ROLE_MESSAGE, this);
				
			BackTextureIsDestroyed();
			backTexture.Clear();
		}
			
		if(_texture)
		{
			backTexture.Attach(_texture);
			backTexture.Get()->Connect(Texture2D::DESTROY_TEXTURE_MESSAGE, this, Caller<>(this, &TexturingSurface::BackTextureIsDestroyed));
			backTexture.Get()->Connect(Texture2D::INIT_TEXTURE_MESSAGE, this, Caller<>(this, &TexturingSurface::BackTextureIsCreated));
			backTexture.Get()->Connect(Texture2D::CHANGE_RENDER_TARGET_ROLE_MESSAGE, this, Caller<>(this, &TexturingSurface::BackTextureRenderTargetRoleIsChanged));
 
			if(_texture->IsExist())
			{
				BackTextureIsCreated();
			}
			if(IsReceiversExist(CHANGE_BACK_TEXTURE_MESSAGE)) { SendPackage(CHANGE_BACK_TEXTURE_MESSAGE, backTexture.Get()->GetName()); }
		} 
	}
} 



void TexturingSurface::BackTextureIsCreated(void)
{
	ModifyGraphicsType(BACK_TEXTURING_GRAPHICS_TYPE, true);  
 
	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{
		if(mesh.Get()->GetShapeType() == Mesh::QUAD)
		{
			CreateBackIndices();
			CalculateBackTexelsForQuad();
		}
	}
}  


void TexturingSurface::BackTextureIsDestroyed(void)
{
	ModifyGraphicsType(BACK_TEXTURING_GRAPHICS_TYPE, false); 

	DestroyBackIndices();
	DestroyBackTexels();
}


Texture2D* TexturingSurface::GetBackTexture(void)const
{
	return backTexture.Get();
}


void TexturingSurface::BindBackTexturedAnimation(TexturedAnimation* _texturedAnimation)
{
	if(backTexturedAnimation.Get() != _texturedAnimation)
	{
		if(frontTexturedAnimation.IsNotEmpty() && frontTexturedAnimation.Get() == _texturedAnimation) { return; }

		if(_texturedAnimation && backTexture.IsNotEmpty())
		{
			BindBackTexture(NIL);
		}

		if(backTexturedAnimation.IsNotEmpty())
		{
			backTexturedAnimation.Get()->Disconnect(TexturedAnimation::DESTROY_TEXTURED_ANIMATION_MESSAGE, this);
			backTexturedAnimation.Get()->Disconnect(TexturedAnimation::INIT_TEXTURED_ANIMATION_MESSAGE, this);
				
			BackTexturedAnimationIsDestroyed();
			backTexturedAnimation.Clear();
		}
			
		if(_texturedAnimation)
		{
			backTexturedAnimation.Attach(_texturedAnimation);
			backTexturedAnimation.Get()->Connect(TexturedAnimation::DESTROY_TEXTURED_ANIMATION_MESSAGE, this, Caller<>(this, &TexturingSurface::BackTexturedAnimationIsDestroyed));
			backTexturedAnimation.Get()->Connect(TexturedAnimation::INIT_TEXTURED_ANIMATION_MESSAGE, this, Caller<>(this, &TexturingSurface::BackTexturedAnimationIsCreated));

			if(_texturedAnimation->IsExist())
			{
				BackTexturedAnimationIsCreated();
			}
			if(IsReceiversExist(CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE)) { SendPackage(CHANGE_BACK_TEXTURED_ANIMATION_MESSAGE, backTexturedAnimation.Get()->GetName()); }
		} 
	}
}




void TexturingSurface::BackTexturedAnimationIsDestroyed(void)
{
	ModifyGraphicsType(BACK_TEXTURING_GRAPHICS_TYPE, false); 

	DestroyBackIndices();
	DestroyBackTexels();
}



void TexturingSurface::BackTexturedAnimationIsCreated(void)
{
	ModifyGraphicsType(BACK_TEXTURING_GRAPHICS_TYPE, true); 

	if(IsGraphicsTypeEnabled(GraphicsSurface::POLYGONAL_SURFACE_GRAPHICS_TYPE))
	{
		if(mesh.Get()->GetShapeType() == Mesh::QUAD)
		{
			CreateBackIndices();
			CalculateBackTexelsForQuad();
		}
	}
}
	



TexturedAnimation* TexturingSurface::GetBackTexturedAnimation(void)const
{
	return backTexturedAnimation.Get();
}



void TexturingSurface::CurrentFrameIsChanged(int32 _index)
{
	if(frontTexturedAnimation.Get()->IsPositionOffsetEnabled())
	{
		if(frontTexturedAnimation.Get()->GetPlayingMode() & PlayingObject::FORWARD_PLAYING)
		{
			DisplacePosition(frontTexturedAnimation.Get()->currentFrameInfo->positionOffset);
		}
		else
		{
			DisplacePosition(-frontTexturedAnimation.Get()->currentFrameInfo->positionOffset);
		}
	}
		
	if(frontTexturedAnimation.Get()->IsHitboxEnabled())
	{ 
		BindHitbox(frontTexturedAnimation.Get()->GetHitbox());
	} 
}

			 

bool TexturingSurface::SaveToFile(File& _file)
{
	if(GraphicsSurface::SaveToFile(_file))
	{ 
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



bool TexturingSurface::SaveAsToFile(StringANSI _name)
{
	File file(AssetLibrary::_GetDir() + AbstractObject::_GetDir() + _name + "." + AbstractObject::_GetFileSuffix(), File::REWRITE);

	if(!SaveAsToFile(file, _name))
	{
		file.Remove();
		return false;
	}
	return true; 
}



bool TexturingSurface::SaveAsToFile(File& _file, StringANSI _name)
{
	if(GraphicsSurface::SaveAsToFile(_file, _name))
	{ 
		SaveDerivedToFile(_file);
		return true;
	}
	return false;
}



void TexturingSurface::SaveDerivedToFile(File& _file)
{
	_file.Write(frontTexturedAnimation.IsNotEmpty()); 
			
	if(frontTexturedAnimation.IsNotEmpty())
	{
		bool animationIsCommonAsset = frontTexturedAnimation.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(animationIsCommonAsset);

		if(animationIsCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(frontTexturedAnimation.Get()->GetName(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
			{
				frontTexturedAnimation.Get()->SaveToFile();
			}
			_file.Write(frontTexturedAnimation.Get()->GetName().length());
			_file.WriteString(frontTexturedAnimation.Get()->GetName());
		}
		else
		{
			frontTexturedAnimation.Get()->SaveToFile(_file);
		}
	}
	_file.Write(backTexturedAnimation.IsNotEmpty());
			
	if(backTexturedAnimation.IsNotEmpty())
	{
		bool animationIsCommonAsset = backTexturedAnimation.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(animationIsCommonAsset);

		if(animationIsCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(backTexturedAnimation.Get()->GetName(), AssetLibrary::TEXTURED_ANIMATION_ASSET))
			{
				backTexturedAnimation.Get()->SaveToFile();
			}
			_file.Write(backTexturedAnimation.Get()->GetName().length());
			_file.WriteString(backTexturedAnimation.Get()->GetName());
		}
		else
		{
			backTexturedAnimation.Get()->SaveToFile(_file);
		}
	}
	_file.Write(frontTexture.IsNotEmpty());
			
	if(frontTexture.IsNotEmpty())
	{
		bool textureIsCommonAsset = frontTexture.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write( textureIsCommonAsset);

		if(textureIsCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(frontTexture.Get()->GetName(), AssetLibrary::TEXTURE_2D_ASSET))
			{
				frontTexture.Get()->SaveToFile();
			}
			_file.Write(frontTexture.Get()->GetName().length());
			_file.WriteString(frontTexture.Get()->GetName());
		}
		else
		{
			frontTexture.Get()->SaveToFile(_file);
		}
	}
	_file.Write(backTexture.IsNotEmpty());
			
	if(backTexture.IsNotEmpty())
	{
		bool textureIsCommonAsset = backTexture.Get()->GetAddress(&AssetLibrary::_GetAssets()) ? true : false;
		_file.Write(textureIsCommonAsset);

		if(textureIsCommonAsset)
		{
			if(!AssetLibrary::_IsAssetExist(backTexture.Get()->GetName(), AssetLibrary::TEXTURE_2D_ASSET))
			{
				backTexture.Get()->SaveToFile();
			}
			_file.Write(backTexture.Get()->GetName().length());
			_file.WriteString(backTexture.Get()->GetName());
		}
		else
		{
			backTexture.Get()->SaveToFile(_file);
		}

	}
	_file.Write(frontTexels.size());
		
	if(!frontTexels.empty())
	{  
		_file.Write(frontTexels[0].size());
		for(int32 i = 0; i < frontTexels.size(); ++i)
		{
			for(int32 j = 0; j < frontTexels[i].size(); ++j)
			{
				_file.Write(frontTexels[i][j]);
			}
		}
	}
	_file.Write(backTexels.size());
	
	if(!backTexels.empty())
	{
		_file.Write(backTexels[0].size());
		for(int32 i = 0; i < backTexels.size(); ++i)
		{
			for(int32 j = 0; j < backTexels[i].size(); ++j)
			{
				_file.Write(backTexels[i][j]);
			}
		}
	}
	_file.Write(flipXEnable);
	_file.Write(flipYEnable);
}
 

bool TexturingSurface::LoadFromFile(File& _file)
{
	if(GraphicsSurface::LoadFromFile(_file))
	{
		LoadDerivedFromFile(_file);
		return true;
	} 
	return false;
}
	

void TexturingSurface::LoadDerivedFromFile(File& _file)
{
	bool frontAnimationIsExist;
	_file.Read(frontAnimationIsExist);
			
	if(frontAnimationIsExist)
	{
		bool frontAnimationIsCommonAsset;
		_file.Read(frontAnimationIsCommonAsset);
		TexturedAnimation* animation;
				
		if(frontAnimationIsCommonAsset)
		{
			StringANSI name;
			int32 length;
			_file.Read(length);
			_file.ReadString(name, length);

			animation = dynamic_cast<TexturedAnimation*>(AssetLibrary::_LoadCommonAsset<TexturedAnimation>(name));
		}
		else
		{
			animation = dynamic_cast<TexturedAnimation*>(AssetLibrary::_LoadPrivateAsset<TexturedAnimation>(_file));
		}
		if(animation) { BindFrontTexturedAnimation(animation); }
	}
	bool backTexturedAnimationIsExist;
	_file.Read(backTexturedAnimationIsExist);
			
	if(backTexturedAnimationIsExist)
	{
		bool backTexturedAnimationIsCommonAsset;
		_file.Read(backTexturedAnimationIsCommonAsset);
		TexturedAnimation* animation;
				
		if(backTexturedAnimationIsCommonAsset)
		{
			StringANSI name;
			int32 length;
			_file.Read(length);
			_file.ReadString(name, length);

			animation = dynamic_cast<TexturedAnimation*>(AssetLibrary::_LoadCommonAsset<TexturedAnimation>(name));
		}
		else
		{
			animation = dynamic_cast<TexturedAnimation*>(AssetLibrary::_LoadPrivateAsset<TexturedAnimation>(_file));
		}
		if(animation) { BindBackTexturedAnimation(animation); }
	}
	bool frontTextureIsExist;
	_file.Read(frontTextureIsExist);
			
	if(frontTextureIsExist)
	{
		bool frontTextureIsCommonAsset;
		_file.Read(frontTextureIsCommonAsset);
		Texture2D* texture;
				
		if(frontTextureIsCommonAsset)
		{
			StringANSI name;
			int32 length;
			_file.Read(length);
			_file.ReadString(name, length);

			texture = dynamic_cast<Texture2D*>(AssetLibrary::_LoadCommonAsset<Texture2D>(name));
		}
		else
		{
			texture = dynamic_cast<Texture2D*>(AssetLibrary::_LoadPrivateAsset<Texture2D>(_file));
		}
		if(texture) { BindFrontTexture(texture); }
	}
	bool backTextureIsExist;
	_file.Read(backTextureIsExist);
			
	if(backTextureIsExist)
	{
		bool backTextureIsCommonAsset;
		_file.Read(backTextureIsCommonAsset);
		Texture2D* texture;
				
		if(backTextureIsCommonAsset)
		{
			StringANSI name;
			int32 length;
			_file.Read(length);
			_file.ReadString(name, length);

			texture = dynamic_cast<Texture2D*>(AssetLibrary::_LoadCommonAsset<Texture2D>(name));
		}
		else
		{
			texture = dynamic_cast<Texture2D*>(AssetLibrary::_LoadPrivateAsset<Texture2D>(_file));
		}
		if(texture) { BindBackTexture(texture); }
	}
	int32 rows;
	_file.Read(rows);

	if(rows > 0)
	{
		int32 columns;
		_file.Read(columns);

		ResizeFrontTexels(rows, columns);
		for(int32 i = 0; i < rows; ++i)
		{
			for(int32 j = 0; j < columns; ++j)
			{
				_file.Read(frontTexels[i][j]);
			}
		}
	}
	_file.Read(rows);

	if(rows > 0)
	{
		int32 columns;
		_file.Read(columns);

		ResizeBackTexels(rows, columns);
		for(int32 i = 0; i < rows; ++i)
		{
			for(int32 j = 0; j < columns; ++j)
			{
				_file.Read(backTexels[i][j]);
			}
		}
	}
	_file.Read(flipXEnable);
	_file.Read(flipYEnable);
}

