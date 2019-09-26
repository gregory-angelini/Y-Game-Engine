
#include "TextureViewer.h"
#include <Editor/TextureCreator/TextureCreator.h>
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Color/RGBA/RGBA.h>
#include <Editor/Editor/Editor.h>
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h> 
#include <Engine/Output/Graphics/Shape/Quad/GraphicsQuad/GraphicsQuad.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsStrip/GraphicsStrip.h>



MagicPointer<Camera3D> TextureViewer::camera3d;
float TextureViewer::pointSizeInUnits = 0.007f;


TextureViewer::TextureViewer(TextureCreator* _textureCreator): mesh(NIL), sprite(NIL), textureCreator(_textureCreator), fantomVertex(NIL), fantomEdge(NIL), fantomVertexDrawing(false), fantomEdgeDrawing(false), currentVertex(NIL), selectArea(NIL), selectAreaDrawing(false), camera2d(NIL), fantomObject(NIL), fantomMesh(NIL), pivotShowEnable(false), vectorLerp(NIL), vectorLerpFloatLerp(NIL), vectorLerpCurrentVertex(NIL)
{}



TextureViewer::TextureViewer(void)
{}


bool TextureViewer::Create(Define _define)
{
	if(Engine::Create(_define))
	{
		pixelsPerMeter = SpaceUnitsDialog::_GetUnitsFromFile();
		gridSize = 25;
		pointsSizeInPixels = 8.0f;
		lineWidthInPixels = 4.2f;
		
		cameraMoveSpeed = 1.0f; 
		cameraNearScale = 0.1f;
	    cameraFarScale = 5.0f;
		ñamaraZoomSpeed = 10.0f;
		sightSize = 25.0f;
		vectorLerpCurrentVertexSize = 0.31f;
		{	
			scene3d.Attach(new Scene());

			scene3d.Get()->GetRenderList().SetRemoveMode(MagicContainer::REMOVE_NODE);
			scene3d.Get()->GetUpdateList().SetRemoveMode(MagicContainer::REMOVE_NODE);
		}
		{
			vectorLerpFloatLerp = new FloatLerp();
			
			vectorLerpFloatLerp->Connect(FloatLerp::FLOAT_LERP_FINISH_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpIsFinish));
            vectorLerpFloatLerp->Connect(FloatLerp::FLOAT_LERP_START_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpIsStart));

			vectorLerpFloatLerp->Connect(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpCurrent));
			vectorLerpFloatLerp->Connect(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpDelta));
			vectorLerpFloatLerp->Connect(FloatLerp::STREAMING_FLOAT_T_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpT));

			scene3d.Get()->AddToUpdateList(vectorLerpFloatLerp);
		}
		{
			vectorLerp = new VectorLerp();

			vectorLerp->SetAmountOfSegments(1);
			vectorLerp->SetLerpMode(VectorLerp::BAZIER);
			vectorLerp->SetAmountOfSegments(15);

			vectorLerp->Connect(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpCurrent));
			vectorLerp->Connect(VectorLerp::STREAMING_VECTOR_DELTA_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpDelta));


			vectorLerp->BindFloatLerp(vectorLerpFloatLerp);
		}
		{
			vectorLerpCurrentVertex = new GraphicsPointEx();
			
			vectorLerpCurrentVertex->SetColor(RGBA(120, 0, 0, 255));
			vectorLerpCurrentVertex->SetShowEnable(false);
			vectorLerp->Connect(VectorLerp::STREAMING_VECTOR_CURRENT_MESSAGE, vectorLerpCurrentVertex, Caller<>(vectorLerpCurrentVertex, &GraphicsPointEx::SetPosition));
		}
		{
			Font2D::_CheckSupportedFonts(this);

			Font2D::Define def;
			def.fontName = "Arial";
			def.fontSize = 16;
			def.fontWeight = 600;
			def.color = RGBA(RGBA::WHITE_COLOR).GetRGBAf();

			font = new Font2D();
			font->Create(def);
		}
		{
			sprite = new TexturingSurface();  
			
			mesh = new Mesh();
			mesh->SetBoundingVolumeType(Mesh::BOUNDING_QUAD);

			sprite->BindMesh(mesh);
			sprite->SetPosition(Vector3());

			scene3d.Get()->AddToLayer(sprite);
		}
		{
			Camera3D::Define def;
			def.projectionMode = Camera3D::SCREEN_PROJECTION;
			def.distanceToViewingPoint = 1.0f;  

			camera2d = new Camera3D();
			camera2d->Create(def);
		}
		{
			scene2d = new Scene();
			scene2d->BindCamera(camera2d);
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "TextureCreator/unclenched.png").toStdString());

			unclenchedHandCursor = new TexturingSurface();
			
			Mesh* mesh = new Mesh();
			mesh->CreateQuad(Quad(Vector3(0,0,-0.102), Vector2(24, 29)));
			unclenchedHandCursor->BindMesh(mesh);
			
			unclenchedHandCursor->BindFrontTexture(texture);
			unclenchedHandCursor->SetShowEnable(false);

			scene2d->AddToRenderList(unclenchedHandCursor);
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "TextureCreator/clenched.png").toStdString());

			clenchedHandCursor = new TexturingSurface();
			
			Mesh* mesh = new Mesh();
			mesh->CreateQuad(Quad(Vector3(0,0,-0.102), Vector2(22, 24)));
			clenchedHandCursor->BindMesh(mesh);

			clenchedHandCursor->BindFrontTexture(texture);
			clenchedHandCursor->SetShowEnable(false);

			scene2d->AddToRenderList(clenchedHandCursor);
		}
		{
			Texture2D* texture = new Texture2D();
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "TextureCreator/arrow.png").toStdString());

			arrowCursor = new TexturingSurface();
			
			Mesh* mesh = new Mesh();
			mesh->CreateQuad(Quad(Vector3(14/2, 24/2, -0.102f), Vector2(14, 24)));
			arrowCursor->BindMesh(mesh);

			arrowCursor->BindFrontTexture(texture);
			arrowCursor->SetShowEnable(true);
 
			scene2d->AddToRenderList(arrowCursor);
		}
		{
			selectArea = new GraphicsSurface();
			selectArea->SetColor(RGBA(RGBA::GREEN_COLOR));
			selectArea->SetAlpha(0.25f);

			Mesh* mesh = new Mesh();
			selectArea->BindMesh(mesh);
		}
		{
			plane = new Plane(); 
			plane->Set(Vector3(), Vector3(0,0,1));
		}
		{
			fantomObject = new GraphicsSurfaceEx();
	
			fantomObject->SetAlpha(0.45);
		}
		{
			fantomMesh = new Mesh();
	
			fantomObject->BindMesh(fantomMesh);
		}
		{
			fantomVertex = new GraphicsPointEx();
			fantomVertex->SetShowEnable(false);

			fantomVertex->SetAlpha(0.45);
			Renderer::_SetPointSize(pointsSizeInPixels);
		}
		{
			fantomEdge = new GraphicsLineEx();
			fantomEdge->SetShowEnable(false);

			fantomEdge->SetAlpha(0.4);
			Renderer::_SetLineWidth(lineWidthInPixels);
		}
		{
			Camera3D::Define def;
			def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;
			def.zoomEnable = true;
			def.rotationEnable = false;

			camera3d.Attach(new Camera3D());
			camera3d.Get()->Create(def);
		}

		SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));
		return true;
	}
	return false;
}




TextureViewer::~TextureViewer(void)
{
	delete vectorLerpFloatLerp;
	delete vectorLerp;
	
	delete vectorLerpCurrentVertex;
	delete font;
	delete mesh; 
	delete sprite;

	delete unclenchedHandCursor;
	delete clenchedHandCursor;
	delete arrowCursor;

	delete plane;
	delete fantomMesh;
	delete fantomObject;
	delete fantomVertex;
	delete fantomEdge;
	delete selectArea;

	delete camera2d;
	delete scene2d;
}



float TextureViewer::_GetPointSizeInUnits(void)
{
	return pointSizeInUnits;
}


void TextureViewer::ShowCursor(void)
{
	if(Mouse::_IsShowCursor())
	{
		Mouse::_SetShowCursor(false);
	}
}


void TextureViewer::HideCursor(void)
{
	if(!Mouse::_IsShowCursor())
	{
		Mouse::_SetShowCursor(true);
	}
}


TexturingSurface* TextureViewer::GetSprite(void)
{
	return sprite;
}


float TextureViewer::GetCameraMoveSpeed(void)
{
	return cameraMoveSpeed;
}


void TextureViewer::SetCameraMoveSpeed(float _speed)
{
	cameraMoveSpeed = _speed;
}



MagicList<TextureViewer::GraphicsPointEx>& TextureViewer::GetSelectedPoints(void)
{
	return selectedPoints;
}


MagicList<TextureViewer::GraphicsPointEx>& TextureViewer::GetPoints(void)
{
	return vertices;
}




MagicList<Mesh>& TextureViewer::GetShapes(void)
{
	return shapes;
}

 

int32 TextureViewer::GetGridSize(void)const
{
	return gridSize;
}


MagicList<TextureViewer::GraphicsSurfaceEx>& TextureViewer::GetObjects(void)
{
	return objects;
}



Camera3D* TextureViewer::GetCamera(void)const
{
	return camera3d.Get();
}


Scene* TextureViewer::GetScene(void)
{
	return scene3d.Get();
} 


Camera3D* TextureViewer::_GetCamera(void)
{
	return camera3d.Get();
}

 

void TextureViewer::MoveCamera(void)
{
	if(textureCreator->IsViewportFocus())
	{
		if(!pivotShowEnable)
		{ 
			if(Keyboard::_IsKeyPressed(Keyboard::UP_ARROW))
			{ 
				camera3d.Get()->SetViewingPoint(camera3d.Get()->GetViewingPoint() + Vector3(0, pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), 0) * cameraMoveSpeed);
			}
			else if(Keyboard::_IsKeyPressed(Keyboard::DOWN_ARROW))
			{
				camera3d.Get()->SetViewingPoint(camera3d.Get()->GetViewingPoint() + Vector3(0, -pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), 0) * cameraMoveSpeed);
			}

			if(Keyboard::_IsKeyPressed(Keyboard::LEFT_ARROW))
			{
				camera3d.Get()->SetViewingPoint(camera3d.Get()->GetViewingPoint() + Vector3(-pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), 0, 0) * cameraMoveSpeed);
			}
			else if(Keyboard::_IsKeyPressed(Keyboard::RIGHT_ARROW))
			{
				camera3d.Get()->SetViewingPoint(camera3d.Get()->GetViewingPoint() + Vector3(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), 0, 0) * cameraMoveSpeed);
			}
		}
	}
}


void TextureViewer::UpdateVectorLerpFloatLerpStatus(void)
{
	if(!vectorLerpFloatLerp->IsPlaying() && (vectorLerpFloatLerp->GetCurrent() != vectorLerpFloatLerp->GetTarget()))
	{
		textureCreator->PrepareVectorLerpFloatLerpToPlay();
	}
	else if(vectorLerpFloatLerp->IsPlaying())
	{ 
		vectorLerpCurrentVertex->SetShowEnable(true);
		textureCreator->PrepareVectorLerpFloatLerpToPause();
	}
	else if(!vectorLerpFloatLerp->IsPlaying() && (vectorLerpFloatLerp->GetCurrent() == vectorLerpFloatLerp->GetTarget()))
	{
		textureCreator->PrepareVectorLerpFloatLerpToPlay();
	}
	{
		if(vectorLerpFloatLerp->IsPlaying())
		{
			{ 
				textureCreator->UpdateVectorLerpFloatLerpElapsedTime(vectorLerpFloatLerp->GetElapsedTimeInSec());
			}
			{
			}
		}
	}
} 


void TextureViewer::Update(float _delta)
{
	scene3d.Get()->Update(_delta);
	
	MoveCamera();
	
	UpdateVectorLerpFloatLerpStatus();

	UpdateAnimationStatus();
}




void TextureViewer::UpdateAnimationStatus(void)
{
	if(textureCreator->GetEditorMode() == TextureCreator::TEXTURED_ANIMATION_EDITOR)
	{
		if(sprite->GetFrontTexturedAnimation() && sprite->GetFrontTexturedAnimation()->IsExist()) 
		{
			if(!sprite->GetFrontTexturedAnimation()->IsPlaying())
			{
				if(!sprite->GetFrontTexturedAnimation()->IsLastFrame())
				{
					textureCreator->PrepareToPlayAnimation();
				}
			}
			else if(sprite->GetFrontTexturedAnimation()->IsPlaying())
			{
				textureCreator->SetAnimationCurrentFrame(sprite->GetFrontTexturedAnimation()->GetCurrentFrame());
			
				textureCreator->UpdateAnimationElapsedTime(sprite->GetFrontTexturedAnimation()->GetElapsedTimeInSec());

				textureCreator->PrepareToPauseAnimation();
			}
			
			if(sprite->GetFrontTexturedAnimation()->IsLastFrame())
			{
				textureCreator->PrepareToPlayAnimation();
			}
		}
	}
}


void TextureViewer::VectorLerpFloatLerpIsFinish(void)
{
	if(vectorLerpCurrentVertex->IsShowEnabled())
	{
		vectorLerpCurrentVertex->SetShowEnable(false);
	}
}


void TextureViewer::VectorLerpFloatLerpIsStart(void)
{
	if(vectorLerpCurve.IsNotEmpty())
	{
		if(!vectorLerpCurrentVertex->IsShowEnabled())
		{
			vectorLerpCurrentVertex->SetShowEnable(true);
		}
	}
}


void TextureViewer::VectorLerpFloatLerpCurrent(float _value)
{
	textureCreator->SetVectorLerpFloatLerpCurrent(_value);
}


void TextureViewer::VectorLerpFloatLerpDelta(float _value)
{
	textureCreator->SetVectorLerpFloatLerpDelta(_value);
}


void TextureViewer::VectorLerpFloatLerpT(float _value)
{
	textureCreator->SetVectorLerpFloatLerpT(_value);
}


void TextureViewer::VectorLerpCurrent(Vector3 _value)
{
	textureCreator->SetVectorLerpCurrent(_value);
}


void TextureViewer::VectorLerpDelta(Vector3 _value)
{
	textureCreator->SetVectorLerpDelta(_value);
}


TextureViewer::GraphicsPointEx* TextureViewer::GetVectorLerpCurrentVertex(void)
{
	return vectorLerpCurrentVertex;
}


void TextureViewer::CursorSightDraw(void)
{
	camera2d->BeginTransform(Renderer::PROJECTION);
	camera2d->BeginTransform(Renderer::MODELVIEW);
		
	float preLineWidth = Renderer::_GetLineWidth();
	Renderer::_SetLineWidth(1.0f);

	GraphicsLine::_Draw(Vector3(Mouse::_GetCursorPosition(this).x - sightSize / 2.0f, GetViewportSize().y - Mouse::_GetCursorPosition(this).y, -1), 
		                Vector3(Mouse::_GetCursorPosition(this).x + sightSize / 2.0f, GetViewportSize().y - Mouse::_GetCursorPosition(this).y, -1), RGBA(RGBA::GREEN_COLOR));
	GraphicsLine::_Draw(Vector3(Mouse::_GetCursorPosition(this).x, GetViewportSize().y - Mouse::_GetCursorPosition(this).y - sightSize / 2.0f, -1), 
		                Vector3(Mouse::_GetCursorPosition(this).x, GetViewportSize().y - Mouse::_GetCursorPosition(this).y + sightSize / 2.0f, -1), RGBA(RGBA::GREEN_COLOR));
  
	Renderer::_SetLineWidth(preLineWidth);
	camera2d->EndTransform(Renderer::PROJECTION);
	camera2d->EndTransform(Renderer::MODELVIEW);

}


void TextureViewer::DrawSelectArea(void)
{
	if(selectAreaDrawing)
	{
		selectArea->Draw();

		float preLineWidth = Renderer::_GetLineWidth();
		Renderer::_SetLineWidth(1.0f);
	
		GraphicsQuad::_Draw(selectAreaQuad, RGBA(0, 120, 0, 255));
	
		Renderer::_SetLineWidth(preLineWidth);
	}
}



void TextureViewer::DrawMetricGrid(void)
{
	if(textureCreator->IsGridShowEnabled())
	{
		float preLineWidth = Renderer::_GetLineWidth();
		Renderer::_SetLineWidth(1.0f);

		if((textureCreator->GetEditorMode() == TextureCreator::TEXTURE_EDITOR && sprite->GetFrontTexture()) || (textureCreator->GetEditorMode() == TextureCreator::TEXTURED_ANIMATION_EDITOR && sprite->GetFrontTexturedAnimation()))
		{
			Vector2 size;
			
			if(textureCreator->GetEditorMode() == TextureCreator::TEXTURE_EDITOR)
			{
				size = sprite->GetFrontTexture()->GetImageSize();
			}
			else if(textureCreator->GetEditorMode() == TextureCreator::TEXTURED_ANIMATION_EDITOR)
			{
				size = sprite->GetFrontTexturedAnimation()->GetCurrentFrameSize();
			}

			int32 rows = size.x / pixelsPerMeter;
			int32 columns = size.y / pixelsPerMeter;
			for(int32 i = 0; i < rows * 2 + 1; i++)
			{
				GraphicsLine::_Draw(Vector3((-rows) + i, -columns, 0), Vector3((-rows) + i, columns, 0), RGBA(240, 240, 240, 140));
			}
			for(int32 j = 0; j < columns * 2 + 1; j++)
			{
				GraphicsLine::_Draw(Vector3(-rows, (-columns) + j, 0), Vector3(rows, (-columns) + j, 0), RGBA(240, 240, 240, 140));		
			}
		}
		else
		{
			int32 rows = gridSize;
			int32 columns = gridSize;
			for(int32 i = 0; i < rows * 2 + 1; i++)
			{
				GraphicsLine::_Draw(Vector3((-rows * pixelsPerMeter) + i * pixelsPerMeter, -columns * pixelsPerMeter, 0), Vector3((-rows * pixelsPerMeter) + i * pixelsPerMeter, columns * pixelsPerMeter, 0), RGBA(240, 240, 240, 140));
			}
			for(int32 j = 0; j < columns * 2 + 1; j++)
			{
				GraphicsLine::_Draw(Vector3(-rows * pixelsPerMeter, (-columns * pixelsPerMeter) + j * pixelsPerMeter, 0), Vector3(rows * pixelsPerMeter, (-columns * pixelsPerMeter) + j * pixelsPerMeter, 0), RGBA(240, 240, 240, 140));		
			}
		}

		Renderer::_SetLineWidth(preLineWidth);
	}
}



void TextureViewer::DrawGizmoXY(void)
{
	if(textureCreator->IsGizmoXYShowEnabled())
	{
		if((textureCreator->GetEditorMode() == TextureCreator::TEXTURE_EDITOR && sprite->GetFrontTexture()) || (textureCreator->GetEditorMode() == TextureCreator::TEXTURED_ANIMATION_EDITOR && sprite->GetFrontTexturedAnimation()))
		{
			Vector2 size = sprite->GetMesh()->GetBoundingQuad().GetSize();

			GraphicsLine::_Draw(Vector3(-size.x, 0, 0), Vector3(size.x, 0, 0), RGBA(163, 21, 21, 255));
			GraphicsLine::_Draw(Vector3(0, -size.y, 0), Vector3(0, size.y, 0), RGBA(118, 185, 0, 255));
		}
		else
		{
			GraphicsLine::_Draw(Vector3(-gridSize * pixelsPerMeter, 0, 0), Vector3(gridSize * pixelsPerMeter, 0, 0), RGBA(163, 21, 21, 255));
			GraphicsLine::_Draw(Vector3(0, -gridSize * pixelsPerMeter, 0), Vector3(0, gridSize * pixelsPerMeter, 0), RGBA(118, 185, 0, 255));
		}
	}
}


void TextureViewer::DrawQuad(void)
{
	if(textureCreator->IsQuadShowEnabled())
	{
		if((textureCreator->GetEditorMode() == TextureCreator::TEXTURE_EDITOR && sprite->GetFrontTexture()) || (textureCreator->GetEditorMode() == TextureCreator::TEXTURED_ANIMATION_EDITOR && sprite->GetFrontTexturedAnimation()))
		{
			Quad quad = sprite->GetMesh()->GetBoundingQuad();

			GraphicsQuad::_Draw(quad, RGBA(RGBA::GREEN_COLOR));
		}
	}
}



void TextureViewer::DrawHitboxPoints(void)
{
	if(textureCreator->GetCurrentBrush() == "POLYGON")
	{
		if(vertices.IsNotEmpty())
		{
			if(textureCreator->GetCurrentBuildMode() == "SPLIT")
			{
				if(textureCreator->IsLinesShowEnabled() || textureCreator->IsPointsShowEnabled())
				{
					vertices.GetHead()->SetColor(RGBA(0, 200, 0, 255));

					int32 i = 0;
					for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != vertices.GetTailAddress(); it = it->GetRight())
					{
						if(textureCreator->IsLinesShowEnabled()) 
						{
							if(intersectedLines.Find(i) != NIL)
							{
								GraphicsLine::_Draw(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition(), RGBA(120, 0, 0, 255));
							}
							else
							{
								GraphicsLine::_Draw(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition(), RGBA(RGBA::GREEN_COLOR));
							}
						}

						if(textureCreator->IsPointsShowEnabled()) 
						{
							it->GetNode()->Draw();
						}

						i++;
					}
					if(textureCreator->IsPointsShowEnabled()) 
					{
						vertices.GetTail()->Draw();
					}
					vertices.GetHead()->RestoreColor();
				}
			}
			else if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
			{
				if(vertices.GetSize() <= 2)
				{ 
					if(textureCreator->IsLinesShowEnabled() || textureCreator->IsPointsShowEnabled())
					{
						vertices.GetHead()->SetColor(RGBA(0, 200, 0, 255));
						for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != vertices.GetTailAddress(); it = it->GetRight())
						{
							if(textureCreator->IsLinesShowEnabled())
							{
								GraphicsLine::_Draw(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition(), RGBA(RGBA::GREEN_COLOR));
							}

							if(textureCreator->IsPointsShowEnabled()) 
							{
								it->GetNode()->Draw();
							}
						}
						if(textureCreator->IsPointsShowEnabled()) 
						{
							vertices.GetTail()->Draw();
						}

						vertices.GetHead()->RestoreColor();
					}
				}
				else
				{  
					fantomObject->Draw();

					vertices.GetHead()->SetColor(RGBA(0, 200, 0, 255));
					for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
					{
						it->GetNode()->Draw();
					}

					vertices.GetHead()->RestoreColor();
				}
			}
		}
	}
	else if(textureCreator->GetCurrentBrush() == "CIRCLE")
	{
		fantomObject->Draw();
	}
	else if(textureCreator->GetCurrentBrush() == "QUAD")
	{
		fantomObject->Draw();
	}
}




void TextureViewer::DrawVectorLerpPoints(void)
{
	if(vertices.IsNotEmpty())
	{
		if(textureCreator->IsLinesShowEnabled()) 
		{
			for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != vertices.GetTailAddress(); it = it->GetRight())
			{
				GraphicsLine::_Draw(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition(), RGBA(0, 120, 0, 255));
			}
		}
		DrawVectorLerpCurve();

		
		if(textureCreator->IsPointsShowEnabled()) 
		{
			for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				it->GetNode()->Draw();
			}
		}
	}
}



void TextureViewer::DrawVectorLerpCurve(void)
{
	if(vectorLerpCurve.IsNotEmpty())
	{
		if(textureCreator->IsVectorLerpShowCurveEnabled()) 
		{ 
			GraphicsStrip::_Draw(vectorLerpCurve, RGBA(234, 170, 68, 255));
		}
	}
}


void TextureViewer::DrawHitboxFantomEdge(void)
{
	if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
	{
		if(vertices.GetSize() <= 2)
		{
			fantomEdge->Draw();
		}
	} 
	else
	{
		fantomEdge->Draw();
	}
}


void TextureViewer::DrawVectorLerpFantomEdge(void)
{
	fantomEdge->SetColor(RGBA(0, 120, 0, 255));
	fantomEdge->Draw(); 
	fantomEdge->RestoreColor();
}



void TextureViewer::DrawPreviousAnimationFrame(void)
{
	if(textureCreator->GetEditorMode() == TextureCreator::TEXTURED_ANIMATION_EDITOR)
	{
		if(textureCreator->IsAnimationPreviousPositionShowEnabled())
		{ 
			if(sprite->GetFrontTexturedAnimation() && sprite->GetFrontTexturedAnimation()->IsExist())
			{
				Vector3 previousPosition = sprite->GetPosition() - sprite->GetFrontTexturedAnimation()->GetCurrentFrameInfo()->GetPositionOffset();

				Vector3 currentPosition = sprite->GetPosition();
				sprite->SetPosition(previousPosition);

				sprite->SetColor(RGBA(0, 255, 0, 90));
				scene3d.Get()->Draw();
				sprite->SetColor(RGBA(255, 255, 255, 255));
				
				sprite->SetPosition(currentPosition);
			}
		}
	}
}



void TextureViewer::Draw(float _delta)
{
	ClearScene();
	camera3d.Get()->BeginTransform(Renderer::PROJECTION);
	camera3d.Get()->BeginTransform(Renderer::MODELVIEW);
	

	DrawPreviousAnimationFrame();

	scene3d.Get()->Draw();
	for(MagicList<GraphicsObject>::AddressDerived* it = decorations.GetHeadAddress(); it != NIL; it = it->GetRight())
	{
		it->GetNode()->Draw();
	}
	DrawMetricGrid();
	DrawGizmoXY();
	DrawQuad();


	if(textureCreator->GetCurrentTabName() == "Vector lerp")
	{
		DrawVectorLerpPoints();
		if(fantomEdge->IsShowEnabled())
		{
			DrawVectorLerpFantomEdge();
		}
		if(vectorLerpCurrentVertex->IsShowEnabled() && vertices.IsNotEmpty())
		{
			vectorLerpCurrentVertex->Draw();
		}
	} 
	else if(textureCreator->GetCurrentTabName() == "Hitbox")
	{
		for(MagicList<GraphicsSurfaceEx>::AddressDerived* it = objects.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			if(it->GetNode()->GetFrameIndex() == textureCreator->GetAnimationCurrentFrame())
			{
				it->GetNode()->Draw();
			}
		}
		DrawHitboxPoints();
		if(fantomEdge->IsShowEnabled())
		{
			DrawHitboxFantomEdge(); 
		} 
	}
	if(fantomVertex->IsShowEnabled())
	{
		fantomVertex->Draw();
	}
	DrawSelectArea();
	camera3d.Get()->EndTransform(Renderer::PROJECTION);
	camera3d.Get()->EndTransform(Renderer::MODELVIEW);

		
	if(pivotShowEnable)
	{ 
		CursorSightDraw();
	} 
	else
	{
		scene2d->Draw();
	}
	camera2d->BeginTransform(Renderer::PROJECTION);
	camera2d->BeginTransform(Renderer::MODELVIEW);
	int32 i = 0;

	font->DrawScreenText(Vector3(10, i+=30, -1), "d: %f",vectorLerpFloatLerp->GetElapsedTimeInSec());
	camera2d->EndTransform(Renderer::PROJECTION);
	camera2d->EndTransform(Renderer::MODELVIEW);

}



TextureViewer::GraphicsPointEx* TextureViewer::CreateNewVertex(Vector3 _position)
{
	GraphicsPointEx* vertex = new GraphicsPointEx();
	vertex->SetPosition(_position);
	return vertex;
}


void TextureViewer::UpdateFantomMesh(void)
{
	SimpleList<Vector3> points;
	for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
	{
		points.AddToTail(it->GetNode()->GetPosition());
	}

	fantomMesh->CreateShape(points, Mesh::DELAUNAY);
}


VectorLerp* TextureViewer::GetVectorLerp(void)const
{
	return vectorLerp;
}



SimpleList<Vector3>& TextureViewer::GetVectorLerpCurve(void)
{
	return vectorLerpCurve;
}


void TextureViewer::HitboxSaveShapeAsMesh(StringANSI _name)
{
	Mesh* mesh = GetShapes().Get(textureCreator->GetSelectedShapeIndex());

	TextureShapeListWidget::ItemShape* it = textureCreator->GetSelectedShape();

	if(it) 
	{
		mesh->SetBoundingVolumeType(GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString()));
						
		if(mesh)
		{
			mesh->SaveAsToFile(_name);
		}
	}
}



int TextureViewer::GetIntBoundingVolumeType(StringANSI _shapeType)
{
	if(_shapeType == "QUAD")
	{
		return Mesh::BOUNDING_QUAD;
	}
	else if(_shapeType == "CIRCLE")
	{
		return Mesh::BOUNDING_CIRCLE;
	}
	else if(_shapeType == "BOX")
	{
		return Mesh::BOUNDING_BOX;
	}
	else if(_shapeType == "SPHERE")
	{
		return Mesh::BOUNDING_SPHERE;
	}
	return Mesh::BOUNDING_CIRCLE;
}


void TextureViewer::HitboxSaveShapeAsObject(StringANSI _name)
{
	Mesh* mesh = GetShapes().Get(textureCreator->GetSelectedShapeIndex());

	if(mesh)
	{
		GraphicsSurface* object = new GraphicsSurface();
		object->Rename(_name);
	
		TextureShapeListWidget::ItemShape* it = textureCreator->GetSelectedShape();
 
		if(it)
		{
			StringANSI shapeName = it->text(TextureShapeListWidget::ItemShape::NAME_PROPERTY).toStdString();
			int32 collisionGroupIndex = it->text(TextureShapeListWidget::ItemShape::COLLISION_GROUP_PROPERTY).toInt();
			bool collisionEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_ENABLE_PROPERTY) == "true" ? true : false;
			bool collisionPolygonsInfoEnable = it->text(TextureShapeListWidget::ItemShape::COLLISION_POLYGONS_INFO_PROPERTY) == "true" ? true : false;
			int32 boundingVolumeType = GetIntBoundingVolumeType(it->text(TextureShapeListWidget::ItemShape::BOUNDING_VOLUME_TYPE_PROPERTY).toStdString());
		
			mesh->Rename(shapeName);
			object->SetCollisionGroupIndex(collisionGroupIndex);
			object->SetCollisionEnable(collisionEnable);
			object->SetCollisionPolygonsInfoEnable(collisionPolygonsInfoEnable);
			mesh->SetBoundingVolumeType(boundingVolumeType);
		}

		object->BindMesh(mesh);

		if(AssetLibrary::_IsAssetExist(mesh->GetName(), AssetLibrary::MESH_ASSET))
		{
			AssetLibrary::_GetAssets().AddObject(mesh);
		}

		object->SaveToFile();

		delete object;
	}
}


void TextureViewer::VectorLerpSaveCurveAsList(StringANSI _name)
{
	if(vectorLerpCurve.IsNotEmpty())
	{
		List<Vector3>* list = new List<Vector3>();
		list->Rename(_name);
		for(SimpleList<Vector3>::Node* it = vectorLerpCurve.GetHead(); it != NIL; it = it->GetRight())
		{
			list->AddItem(new Variable<Vector3>(it->key), AbstractList::TAIL);
		}

		list->SaveToFile();
	}
}




void TextureViewer::VectorLerpSaveCurveAsObject(StringANSI _name)
{
	if(vectorLerpCurve.IsNotEmpty())
	{
		GraphicsStrip* object = new GraphicsStrip();
		object->Rename(_name);
		object->GetVertices() = vectorLerpCurve;
		object->SaveToFile();
	}
}



void TextureViewer::SavePoints(StringANSI _name)
{
	if(vertices.IsNotEmpty())
	{
		List<Vector3>* list = new List<Vector3>();
		list->Rename(_name);
		for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			list->AddItem(new Variable<Vector3>(it->GetNode()->GetPosition()), AbstractList::TAIL);
		}

		list->SaveToFile();
	}
}



void TextureViewer::UpdateVectorLerpCurve(void)
{
	vectorLerp->GetControlVectors().Clear();
	for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
	{
		vectorLerp->GetControlVectors().AddToTail(it->GetNode()->GetPosition());
	}

	vectorLerpCurve = vectorLerp->GetPath();

	if(vertices.GetSize() < 2)
	{
		vectorLerpFloatLerp->Pause();
		vectorLerpCurrentVertex->SetShowEnable(false);
	}

	vectorLerpCurrentVertex->SetPosition(vectorLerp->GetNVertex(vectorLerpFloatLerp->GetT()));
}



void TextureViewer::LoadPoints(StringANSI _name)
{
	AbstractList* list = AbstractList::_LoadFromFile(_name);

	if(list && list->GetValueType() == AbstractList::VECTOR_3_TYPE)
	{
		DestroyPoints();
		for(int32 i = 0; i < list->GetAmountOfItems(); i++)
		{
			Variable<Vector3>* it = dynamic_cast<Variable<Vector3>*>(list->GetItem(i));
			
			if(it)
			{
				GraphicsPointEx* point = CreateNewVertex(it->GetValue());
				vertices.AddToTail(point); 
			}
		}  

		if(textureCreator->GetCurrentTabName() == "Hitbox")
		{ 
			if(textureCreator->GetCurrentBuildMode() == "DELAUNAY") 
			{
				UpdateFantomMesh();
			} 
			else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
			{
				CheckEdgeIntersection();
			}
		}
		else if(textureCreator->GetCurrentTabName() == "Vector lerp")
		{ 
			UpdateVectorLerpCurve();
		}

		textureCreator->UpdateAmountOfPoints(vertices.GetSize());
	}
} 



void TextureViewer::LoadVectorLerpFloatLerp(StringANSI _name)
{
	vectorLerpFloatLerp->LoadFromFile(_name);

	textureCreator->UpdateVectorLerpFloatLerpInfo();
}


void TextureViewer::ClearDecorations(void)
{
	decorations.Clear(MagicContainer::DELETE_NODE);
}


MagicList<GraphicsObject>& TextureViewer::GetDecorations(void)
{
	return decorations;
}
 

void TextureViewer::LoadShape(StringANSI _name)
{
	AbstractObject* object = AbstractObject::_LoadFromFile(_name);

	if(object)
	{ 
		if(object->IsClassPropertyExist(PolygonalSurface::POLYGONAL_SURFACE_CLASS))
		{
			PolygonalSurface* it = dynamic_cast<PolygonalSurface*>(object);

			if(it && it->IsExist())
			{ 
				Mesh* mesh = it->GetMesh();

				if(mesh)
				{  
					GraphicsSurfaceEx* object = new GraphicsSurfaceEx();
					object->SetFrameIndex(textureCreator->GetAnimationCurrentFrame());

					object->BindMesh(mesh);

					shapes.AddToTail(mesh);
					objects.AddToTail(object);
					
					textureCreator->CreateShape(it->GetName().c_str(), it->GetCollisionGroupIndex(), it->IsCollisionEnabled(), it->IsCollisionPolygonsInfoEnabled(), mesh->GetBoundingVolumeType());
				}
			} 
		}
		delete object;
	}
}


void TextureViewer::LoadDecoration(StringANSI _name)
{
	AbstractObject* object = AbstractObject::_LoadFromFile(_name);

	if(object)
	{
		if(object->IsClassPropertyExist(GraphicsSurface::GRAPHICS_SURFACE_CLASS) ||
		   object->IsClassPropertyExist(GraphicsStrip::GRAPHICS_STRIP_CLASS))
		{
			GraphicsObject* it = dynamic_cast<GraphicsObject*>(object);

			if(it)
			{
				it->SetPosition(Vector3());
				it->SetShowEnable(true);
				
				decorations.AddToTail(it);
				return;
			}
		}
		delete object;
	}
}



void TextureViewer::LoadVectorLerp(StringANSI _name)
{
	DestroyPoints();

	vectorLerp->LoadFromFile(_name);

	if(vectorLerp->GetFloatLerp() && vectorLerp->GetFloatLerp() != vectorLerpFloatLerp)
	{
		delete vectorLerpFloatLerp; 
		vectorLerpFloatLerp = vectorLerp->GetFloatLerp();
					
		vectorLerpFloatLerp->Connect(FloatLerp::FLOAT_LERP_FINISH_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpIsFinish));
        vectorLerpFloatLerp->Connect(FloatLerp::FLOAT_LERP_START_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpIsStart));

		vectorLerpFloatLerp->Connect(FloatLerp::STREAMING_FLOAT_CURRENT_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpCurrent));
		vectorLerpFloatLerp->Connect(FloatLerp::STREAMING_FLOAT_DELTA_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpDelta));
		vectorLerpFloatLerp->Connect(FloatLerp::STREAMING_FLOAT_T_MESSAGE, this, Caller<>(this, &TextureViewer::VectorLerpFloatLerpT));

		textureCreator->UpdateVectorLerpFloatLerpInfo();
 
		scene3d.Get()->AddToUpdateList(vectorLerpFloatLerp);
	}
	for(SimpleList<Vector3>::Node* it = vectorLerp->GetControlVectors().GetHead(); it != NIL; it = it->GetRight())
	{
		GraphicsPointEx* point = CreateNewVertex(it->key);
		vertices.AddToTail(point);  
	}

	UpdateVectorLerpCurve();

	if(vectorLerpCurve.IsNotEmpty())
	{
		vectorLerpCurrentVertex->SetPosition(vectorLerp->GetCurrent());

		if(!vectorLerpCurrentVertex->IsShowEnabled())
		{
			vectorLerpCurrentVertex->SetShowEnable(true);
		}
	}
		
	textureCreator->UpdateAmountOfPoints(vertices.GetSize());
}


void TextureViewer::CreateSubPoints(void)
{
	for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != vertices.GetTailAddress(); it = it->GetRight())
	{
		if(it->GetNode()->IsSelected() && it->GetRight()->GetNode()->IsSelected())
		{
			Vector3 center = Line::_GetCenter(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition());
			float distance = Line::_GetLength(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition());

			if(fabs(distance) > (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) * 2.0f)
			{
				if(!IsVertexOverlay(center))
				{
					GraphicsPointEx* point = CreateNewVertex(center); 
					vertices.AddToRight(it->GetNode(), point);
				}
			}
		}
	} 
	
	if(textureCreator->GetCurrentTabName() == "Hitbox")
	{  
		if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
		{ 
			UpdateFantomMesh();
		}   
		else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
		{
			CheckEdgeIntersection();
		}
	}
	else if(textureCreator->GetCurrentTabName() == "Vector lerp")
	{
		UpdateVectorLerpCurve();
	}
	
	textureCreator->UpdateAmountOfPoints(vertices.GetSize());
}



bool TextureViewer::IsVertexOverlay(Vector3 _position)
{
	for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
	{
		if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(_position, Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
		{
			return true;
		}
	}
	return false;
}



void TextureViewer::ChangePivot(void)
{
	if(!pivotShowEnable)
	{
		pivotShowEnable = true;
	}
	else 
	{ 
		pivotShowEnable = false;
	}
} 



void TextureViewer::CheckEdgeIntersection(void)
{
	if(vertices.GetSize() > 3)
	{
		int32 i = 0, j = 0;
		intersectedLines.Clear();

		Line lastLine(vertices.GetHead()->GetPosition(), vertices.GetTail()->GetPosition());
		for(MagicList<GraphicsPointEx>::AddressDerived* it1 = vertices.GetHeadAddress(); it1 != vertices.GetTailAddress(); it1 = it1->GetRight())
		{
			j = 0;

			for(MagicList<GraphicsPointEx>::AddressDerived* it2 = vertices.GetHeadAddress(); it2 != vertices.GetTailAddress(); it2 = it2->GetRight())
			{
				if(i != j && (j - i) >= 2)
				{
					Line line1(it1->GetNode()->GetPosition(), it1->GetRight()->GetNode()->GetPosition());
			
					Line line2(it2->GetNode()->GetPosition(), it2->GetRight()->GetNode()->GetPosition());
			
					if(Line::_IsIntersected(line1, line2))
					{
						intersectedLines.AddToTail(i);
						intersectedLines.AddToTail(j);
					}
				}
				j++;
			}
			if(i != 0 && i != vertices.GetSize() - 2)
			{
				Line line1(it1->GetNode()->GetPosition(), it1->GetRight()->GetNode()->GetPosition());

				if(Line::_IsIntersected(line1, lastLine))
				{
					intersectedLines.AddToTail(i);
				}
			}
					
			i++;
		}	
	}
}




void TextureViewer::CheckConvex(void)
{
	if(vertices.GetSize() > 3)
	{
		for(int32 i = 0; i < vertices.GetSize(); ++i)
		{
			Vector3 p1 = vertices.Get(i)->GetPosition();
			Vector3 p2 = i - 1 < 0 ? vertices.Get(vertices.GetSize() - 1)->GetPosition()
								   : vertices.Get(i - 1)->GetPosition();
			Vector3 p3 = i + 1 < vertices.GetSize() ? vertices.Get(i + 1)->GetPosition()
													: vertices.Get(0)->GetPosition();
			Vector3 center((p2.x + p3.x) / 2.0f, (p2.y + p3.y) / 2.0f, 0.0f);
			SimpleList<Vector3> points;

			for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				points.AddToTail(it->GetNode()->GetPosition());
			}

			vertices.Get(i)->SetConvex(Mesh::_IsConvex(center, points));
		}
	}
}



void TextureViewer::SetHitbox(Hitbox* _hitbox)
{
	if(_hitbox)
	{
		for(MagicList<Hitbox::ShapeInfo>::AddressDerived* it = _hitbox->GetShapes().GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			CreateMesh(it->GetNode()->GetMesh(), it->GetNode()->GetName(), it->GetNode()->GetCollisionGroupIndex(), it->GetNode()->GetCollisionEnable(), it->GetNode()->GetCollisionPolygonsInfoEnable(), it->GetNode()->GetMesh()->GetBoundingVolumeType());
		}
	}
}


void TextureViewer::CreateMesh(Mesh* _mesh)
{
	if(_mesh)
	{
		if(_mesh->IsExist())
		{ 
			GraphicsSurfaceEx* object = new GraphicsSurfaceEx();
			object->SetFrameIndex(textureCreator->GetAnimationCurrentFrame());

			object->BindMesh(_mesh);

			shapes.AddToTail(_mesh);
			objects.AddToTail(object);
			
			DestroyPoints(); 

			textureCreator->CreateShape(_mesh->GetName().c_str(), 0, true, false, _mesh->GetBoundingVolumeType());
		}
	}
}


void TextureViewer::CreateMesh(Mesh* _mesh, StringANSI _name, int32 _collisionGroupIndex, bool _collisionEnable, bool _collisionPolygonsInfoEnable, int _boundingVolumeType)
{
	if(_mesh) 
	{
		if(_mesh->IsExist())
		{
			GraphicsSurfaceEx* object = new GraphicsSurfaceEx();
			object->SetFrameIndex(textureCreator->GetAnimationCurrentFrame());

			object->BindMesh(_mesh);

			shapes.AddToTail(_mesh);
			objects.AddToTail(object);
			
			DestroyPoints();

			textureCreator->CreateShape(_name.empty() ? _mesh->GetName().c_str() : _name.c_str(), _collisionGroupIndex, _collisionEnable, _collisionPolygonsInfoEnable, _boundingVolumeType);
		}
	}
}
 


void TextureViewer::CreateMesh(int32 _index)
{
	if(vertices.GetSize() > 2)
	{
		SimpleList<Vector3> points;

		for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
		{ 
			points.AddToTail(it->GetNode()->GetPosition());
		}

		if(_index == -1)
		{
			if(textureCreator->GetCurrentBuildMode() == "SPLIT")
			{
				if(intersectedLines.IsEmpty())
				{
					Mesh* mesh = new Mesh();
					mesh->SetBoundingVolumeType(Mesh::BOUNDING_QUAD);
					mesh->CreateShape(points, Mesh::SPLIT);
		 
					GraphicsSurfaceEx* object = new GraphicsSurfaceEx();
					object->SetFrameIndex(textureCreator->GetAnimationCurrentFrame());

					object->BindMesh(mesh);

					shapes.AddToTail(mesh);
					objects.AddToTail(object);
						
					DestroyPoints(); 

					textureCreator->CreateShape();
				} 
			}
			else if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
			{
				Mesh* mesh = new Mesh();
				mesh->SetBoundingVolumeType(Mesh::BOUNDING_QUAD);
				mesh->CreateShape(points, Mesh::DELAUNAY);
	
				GraphicsSurfaceEx* object = new GraphicsSurfaceEx();
				object->SetFrameIndex(textureCreator->GetAnimationCurrentFrame());

				object->BindMesh(mesh);

				shapes.AddToTail(mesh);
				objects.AddToTail(object);

				DestroyPoints();

				textureCreator->CreateShape();
			}
		}
		else
		{
			if(textureCreator->GetCurrentBuildMode() == "SPLIT")
			{ 
				if(intersectedLines.IsEmpty())
				{ 
					Mesh* mesh = shapes.Get(_index);
					GraphicsSurfaceEx* object = objects.Get(_index);

					if(mesh && object)
					{
						mesh->CreateShape(points, Mesh::SPLIT);

						object->BindMesh(mesh);

						DestroyPoints();
					}
				}
			}
			else if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
			{
				Mesh* mesh = shapes.Get(_index);
				GraphicsSurfaceEx* object = objects.Get(_index);

				if(mesh && object)
				{
					mesh->CreateShape(points, Mesh::DELAUNAY);

					object->BindMesh(mesh);

					DestroyPoints();
				}
			}
		}
	}
}



void TextureViewer::SetPixelsPerMeter(int32 _units)
{
	if(pixelsPerMeter >= 1)
	{
		pixelsPerMeter = _units;
	}
}


float TextureViewer::GetPixelsPerMeter(void)const
{
	return pixelsPerMeter;
}



float TextureViewer::GetCameraNearScale(void)const
{
	return cameraNearScale;
}

			
float TextureViewer::GetCameraFarScale(void)const
{
	return cameraFarScale;
}


float TextureViewer::GetCameraZoomSpeed(void)const
{
	return ñamaraZoomSpeed;
}



TextureViewer::GraphicsSurfaceEx* TextureViewer::GetEditingObject(void)const
{
	for(int32 i = 0; i < objects.GetSize(); i++)
	{
		if(!objects.Get(i)->IsExist())
		{ 
			return objects.Get(i);
		}
	}
	return NIL;
}


void TextureViewer::RelockSelectedShape(void)
{
	for(int32 i = 0; i < objects.GetSize(); i++)
	{
		if(!objects.Get(i)->IsExist())
		{ 
			if(vertices.GetSize() <= 2)
			{
				textureCreator->DestroyShape(i);
				DestroyPoints();
				break; 
			}

			if(intersectedLines.IsNotEmpty())
			{
				textureCreator->DestroyShape(i);
				DestroyPoints();
				break;
			}

			CreateMesh(i);
			break;
		}
	}
	if(vertices.GetSize() <= 2)
	{
		DestroyPoints();
	}
	else 
	{
		CreateMesh();
	} 
}



void TextureViewer::UnlockShape(int32 _index)
{
	GraphicsSurfaceEx* object = objects.Get(_index);
	Mesh* mesh = shapes.Get(_index);

	if(object && mesh)
	{
		DestroyPoints();
		for(int32 i = 0; i < mesh->GetAmountOfVertices(); i++)
		{
			GraphicsPointEx* point = CreateNewVertex(mesh->GetVertex(i));
			vertices.AddToTail(point);
		}

		if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
		{ 
			UpdateFantomMesh();
		}
		else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
		{
			CheckEdgeIntersection();
		}

		textureCreator->UpdateAmountOfPoints(vertices.GetSize());
		mesh->Destroy();
		object->Destroy();  

		scene3d.Get()->RemoveFromRenderList(object);
	}
}



void TextureViewer::DestroySelectedPoints(void)
{ 
	if(selectedPoints.GetSize() == vertices.GetSize())
	{
		DestroyPoints();
	}
	else
	{
		selectedPoints.Clear(MagicContainer::DELETE_NODE);
		currentVertex = NIL;
			
		if(textureCreator->GetCurrentTabName() == "Hitbox")
		{
			if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
			{ 
				UpdateFantomMesh();
			}
			else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
			{
				CheckEdgeIntersection();
			}
		} 
		else if(textureCreator->GetCurrentTabName() == "Vector lerp")
		{
			UpdateVectorLerpCurve();
		}
			
		textureCreator->UpdateAmountOfPoints(vertices.GetSize());
	}
}


void TextureViewer::DestroyPoints(void)
{
	intersectedLines.Clear();
	currentVertex = NIL;
	vertices.Clear(MagicContainer::DELETE_NODE);
	selectedPoints.Clear();
	fantomMesh->Destroy();
	vectorLerpFloatLerp->Pause();
	vectorLerpCurrentVertex->SetShowEnable(false);

	vectorLerp->GetControlVectors().Clear();
	vectorLerpCurve.Clear();
	
	textureCreator->UpdateAmountOfPoints(0);
}



void TextureViewer::UpdateCursorPosition(void)
{
	selectRay = camera3d.Get()->GetSelectRay(Mouse::_GetCursorPosition(this));

	if(plane->IsIntersected(selectRay, &intersectionPoint) != -1)
	{
		worldCursorPosition = Vector2(intersectionPoint.x, intersectionPoint.y);
		textureCreator->UpdateCursorPosition(worldCursorPosition);
	}
	Vector2 cursorPosition = Mouse::_GetCursorPosition(this);
	cursorPosition.y = GetViewportSize().y - cursorPosition.y;

	arrowCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y - 1), 0.0f)); 
	unclenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((unclenchedHandCursor->GetFrontTexture()->GetImageSize().y) / 2), 0.0f));
	clenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((clenchedHandCursor->GetFrontTexture()->GetImageSize().y) / 2), 0.0f)); 
}


void TextureViewer::ShowArrowCursor(void)
{
	clenchedHandCursor->SetShowEnable(false);
	arrowCursor->SetShowEnable(true);
	unclenchedHandCursor->SetShowEnable(false);
}


void TextureViewer::ShowClenchedCursor(void)
{
	clenchedHandCursor->SetShowEnable(true);
	arrowCursor->SetShowEnable(false);
	unclenchedHandCursor->SetShowEnable(false);
}



void TextureViewer::ShowUnclenchedCursor(void)
{
	clenchedHandCursor->SetShowEnable(false);
	arrowCursor->SetShowEnable(false);
	unclenchedHandCursor->SetShowEnable(true);
}


 
void TextureViewer::VectorLerpLeftButtonPressed(void)
{
	textureCreator->FreezeLastSelectedShape();
	if(currentVertex == NIL)
	{
		for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(intersectionPoint, Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
			{
				currentVertex = it->GetNode();
				startCurrentVertexPosition = it->GetNode()->GetPosition();
				previousCurrentVertexPosition = startCurrentVertexPosition;
				break;
			}
		}
		if(fantomVertexDrawing)
		{
			fantomVertex->SetShowEnable(false);
			fantomVertexDrawing = false;
			if(fantomEdgeDrawing)
			{
				fantomEdge->SetShowEnable(false);
				fantomEdgeDrawing = false;
			}
		} 
	}
	if(currentVertex == NIL && !fantomVertexDrawing && !selectAreaDrawing)
	{
		fantomVertex->SetShowEnable(true);
		fantomVertexDrawing = true;
	
		fantomVertex->SetPosition(intersectionPoint);
		if(!fantomEdgeDrawing)
		{
			if(vertices.IsNotEmpty())
			{
				fantomEdge->SetShowEnable(true);
				fantomEdgeDrawing = true;

				fantomEdge->Set(vertices.GetTail()->GetPosition(), fantomVertex->GetPosition());
			}
		} 
	}
}
       

void TextureViewer::HitboxLeftButtonPressed(void)
{
	if(textureCreator->GetCurrentBrush() == "POLYGON")
	{
		textureCreator->FreezeLastSelectedShape();
		if(currentVertex == NIL)
		{
			for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(intersectionPoint, Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
				{
					currentVertex = it->GetNode();
					startCurrentVertexPosition = it->GetNode()->GetPosition();
					previousCurrentVertexPosition = startCurrentVertexPosition;
					break;
				}
			}
			if(fantomVertexDrawing)
			{
				fantomVertex->SetShowEnable(false);
				fantomVertexDrawing = false;
				if(fantomEdgeDrawing)
				{
					fantomEdge->SetShowEnable(false);
					fantomEdgeDrawing = false;
				}
			} 
		}
		if(currentVertex == NIL && !fantomVertexDrawing && !selectAreaDrawing)
		{
			fantomVertex->SetShowEnable(true);
			fantomVertexDrawing = true;
		
			fantomVertex->SetPosition(intersectionPoint);
			if(!fantomEdgeDrawing)
			{
				if(vertices.IsNotEmpty())
				{
					fantomEdge->SetShowEnable(true);
					fantomEdgeDrawing = true;

					fantomEdge->Set(vertices.GetTail()->GetPosition(), fantomVertex->GetPosition());
				}
			}
		} 
	}
	else if(textureCreator->GetCurrentBrush() == "CIRCLE")
	{ 
		textureCreator->FreezeLastSelectedShape();

		if(!fantomVertexDrawing && !selectAreaDrawing)
		{
			fantomVertexDrawing = true;

			ShowUnclenchedCursor();

			circleRadius = (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) * 2.0f;

			startCurrentVertexPosition = intersectionPoint;
		}
	}
	else if(textureCreator->GetCurrentBrush() == "QUAD")
	{
		textureCreator->FreezeLastSelectedShape();

		if(!fantomVertexDrawing && !selectAreaDrawing)
		{
			fantomVertexDrawing = true;

			ShowUnclenchedCursor();

			quadSize.x = (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) * 2.0f;
			quadSize.y = quadSize.x;
			fantomMesh->CreateQuad(Quad(intersectionPoint, quadSize));
			
			startCurrentVertexPosition = intersectionPoint;
		}
	}
}
   


void TextureViewer::UpdatePivot(void)
{
	for(int32 i = 0; i < mesh->GetAmountOfVertices(); i++)
	{
		mesh->SetVertex(i, mesh->GetVertex(i) - worldCursorPosition.ToVector3());
	}
	for(int32 i = 0; i < shapes.GetSize(); i++)
	{
		Mesh* mesh = shapes.Get(i);
		for(int32 i = 0; i < mesh->GetAmountOfVertices(); i++)
		{
			mesh->SetVertex(i, mesh->GetVertex(i) - worldCursorPosition.ToVector3());
		}
	}
}

 

void TextureViewer::VectorLerpRightButtonPressed(void)
{
	if(!selectAreaDrawing)
	{
		selectAreaDrawing = true;

		currentVertex = NIL;

		bool ctrlIsPressed = Keyboard::_IsKeyPressed(Keyboard::CTRL);

		if(!ctrlIsPressed)
		{ 
			selectedPoints.Clear();
			for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				it->GetNode()->SetSelected(false);
			}
		}
		selectAreaQuad = Quad(intersectionPoint, Vector2());
		selectArea->GetMesh()->CreateQuad(selectAreaQuad);
		
		startSelectedAreaPosition = intersectionPoint;
		for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(intersectionPoint, Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
			{
				if(ctrlIsPressed && it->GetNode()->IsSelected())
				{
					selectedPoints.Remove(it->GetNode());
					it->GetNode()->SetSelected(false);
				}
				else
				{
					selectedPoints.AddToTail(it->GetNode());
					it->GetNode()->SetSelected(true);
				}
				break;
			}
		} 
	}
}


void TextureViewer::HitboxRightButtonPressed(void)
{
	if(textureCreator->GetCurrentBrush() == "POLYGON")
	{  
		if(vertices.IsEmpty())
		{
			if(objects.IsNotEmpty())
			{
				for(int32 i = 0; i < objects.GetSize(); i++)
				{
					if(objects.Get(i)->IsIntersected(selectRay) != -1)
					{
						textureCreator->SelectShape(i);
						break;
					}
				}
			}
		}
		else
		{
			if(!selectAreaDrawing)
			{
				selectAreaDrawing = true;

				currentVertex = NIL;

				bool ctrlIsPressed = Keyboard::_IsKeyPressed(Keyboard::CTRL);

				if(!ctrlIsPressed)
				{ 
					selectedPoints.Clear();
					for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
					{
						it->GetNode()->SetSelected(false);
					}
				}
				selectAreaQuad = Quad(intersectionPoint, Vector2());
				selectArea->GetMesh()->CreateQuad(selectAreaQuad);
				
				startSelectedAreaPosition = intersectionPoint;
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(intersectionPoint, Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
					{
						selectedPoints.AddToTail(it->GetNode());
						
						if(ctrlIsPressed && it->GetNode()->IsSelected())
						{
							selectedPoints.Remove(it->GetNode());
							it->GetNode()->SetSelected(false);
						}
						else
						{
							selectedPoints.AddToTail(it->GetNode());
							it->GetNode()->SetSelected(true);
						}
						break;
					}
				} 
			}
		} 
	}
	else if(textureCreator->GetCurrentBrush() == "CIRCLE")
	{
		if(vertices.IsEmpty())
		{
			if(objects.IsNotEmpty())
			{
				for(int32 i = 0; i < objects.GetSize(); i++)
				{
					if(objects.Get(i)->IsIntersected(selectRay) != -1)
					{
						textureCreator->SelectShape(i);
						break;
					}
				}
			}
		}
	}
	else if(textureCreator->GetCurrentBrush() == "QUAD")
	{
		if(vertices.IsEmpty())
		{
			if(objects.IsNotEmpty())
			{
				for(int32 i = 0; i < objects.GetSize(); i++)
				{
					if(objects.Get(i)->IsIntersected(selectRay) != -1)
					{
						textureCreator->SelectShape(i);
						break;
					}
				}
			}
		}
	}
}


void TextureViewer::HitboxMouseMove(void)
{
	if(textureCreator->GetCurrentBrush() == "POLYGON")
	{ 
		ShowArrowCursor();
		if(selectAreaDrawing)
		{
			Vector3 size = intersectionPoint - startSelectedAreaPosition;
			if(fabs(size.x) >= (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) &&
			   fabs(size.y) >= (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))
			{
				selectAreaQuad = Quad(intersectionPoint - (size / 2.0f), Vector2::_FromVector3(size));
				selectArea->GetMesh()->CreateQuad(selectAreaQuad);
				if(selectArea->GetMesh()->IsClockwise()) { selectArea->GetMesh()->InverseVertices(); }
				selectedPoints.Clear();
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(selectAreaQuad.IsPointInside(it->GetNode()->GetPosition()))
					{
						selectedPoints.AddToTail(it->GetNode());
						it->GetNode()->SetSelected(true);
					}
					else 
					{ 
						it->GetNode()->SetSelected(false);
					} 
				}
			}
		}
		else if(currentVertex)
		{ 
			ShowClenchedCursor();
			for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				it->GetNode()->DisplacePosition(intersectionPoint - previousCurrentVertexPosition);
			}

			previousCurrentVertexPosition = intersectionPoint;

			if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
			{ 
				UpdateFantomMesh();
			}
			else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
			{
				CheckEdgeIntersection();
			}
		}
		else if(fantomVertexDrawing)
		{
			fantomVertex->SetPosition(intersectionPoint);

			if(fantomEdgeDrawing)
			{ 
				fantomEdge->Set(vertices.GetTail()->GetPosition(), fantomVertex->GetPosition());
				int32 i = 0; 
				fantomEdge->SetIntersected(false);
				for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != vertices.GetTailAddress(); it = it->GetRight())
				{
					if(/*i != 0 &&*/i != vertices.GetSize() - 2)
					{
						Line line1(fantomEdge->origin, fantomEdge->end);
						Line line2(it->GetNode()->GetPosition(), it->GetRight()->GetNode()->GetPosition());
					
						if(Line::_IsIntersected(line1, line2))
						{
							intersectedLines.AddToTail(i);
							fantomEdge->SetIntersected(true);
						} 
					}
					i++; 
				}
			}
		} 
		else
		{
			if(selectedPoints.IsNotEmpty())
			{
				bool pointUnderCursor = false;
				for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(intersectionPoint, Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
					{
						pointUnderCursor = true;
						break;
					}
				}
				if(pointUnderCursor) { ShowUnclenchedCursor(); } 
			}
		}
	} 
	else if(textureCreator->GetCurrentBrush() == "CIRCLE")
	{ 
		ShowArrowCursor();

		if(fantomVertexDrawing)
		{ 
			ShowClenchedCursor();

			circleRadius = Line::_GetLength(startCurrentVertexPosition, intersectionPoint);

			if(fabs(circleRadius) > (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) * 2.0f)
			{ 
				fantomMesh->CreateCircle(Circle(startCurrentVertexPosition, circleRadius), textureCreator->GetCircleSegments());
			}
			else
			{
				circleRadius = (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) * 2.0f;
				fantomMesh->CreateCircle(Circle(startCurrentVertexPosition, circleRadius), textureCreator->GetCircleSegments());
			}
		}
	}
	else if(textureCreator->GetCurrentBrush() == "QUAD")
	{ 
		ShowArrowCursor();

		if(fantomVertexDrawing)
		{ 
			ShowClenchedCursor();

			Vector3 size = (intersectionPoint - startCurrentVertexPosition) * 2.0f;
			if(fabs(size.x) >= ((pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) * 2.0f) &&
			   fabs(size.y) >= ((pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) * 2.0f))
			{
				quadSize = Vector2::_FromVector3(size);
				quadSize.x = fabs(quadSize.x);
				quadSize.y = fabs(quadSize.y);

				fantomMesh->CreateQuad(Quad(startCurrentVertexPosition, quadSize));
			} 
		}
	}
} 



void TextureViewer::VectorLerpMouseMove(void)
{  
	ShowArrowCursor();
	if(selectAreaDrawing)
	{
		Vector3 size = intersectionPoint - startSelectedAreaPosition;
		if(fabs(size.x) >= (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()) &&
		   fabs(size.y) >= (pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))
		{
			selectAreaQuad = Quad(intersectionPoint - (size / 2.0f), Vector2::_FromVector3(size));
			selectArea->GetMesh()->CreateQuad(selectAreaQuad);
			if(selectArea->GetMesh()->IsClockwise()) { selectArea->GetMesh()->InverseVertices(); }
			selectedPoints.Clear();
			for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				if(selectAreaQuad.IsPointInside(it->GetNode()->GetPosition()))
				{
					selectedPoints.AddToTail(it->GetNode());
					it->GetNode()->SetSelected(true);
				}
				else 
				{ 
					it->GetNode()->SetSelected(false);
				} 
			}
		}
	}
	else if(currentVertex)
	{
		ShowClenchedCursor();
		for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			it->GetNode()->DisplacePosition(intersectionPoint - previousCurrentVertexPosition);
		}

		previousCurrentVertexPosition = intersectionPoint;

		UpdateVectorLerpCurve();
	}
	else if(fantomVertexDrawing)
	{
		fantomVertex->SetPosition(intersectionPoint);

		if(fantomEdgeDrawing)
		{ 
			fantomEdge->Set(vertices.GetTail()->GetPosition(), fantomVertex->GetPosition());
		}
	}
	else
	{
		if(selectedPoints.IsNotEmpty())
		{
			bool pointUnderCursor = false;
			for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(intersectionPoint, Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
				{
					pointUnderCursor = true;
					break;
				}
			}
			if(pointUnderCursor) { ShowUnclenchedCursor(); } 
		}
	}
}


void TextureViewer::HitboxLeftButtonReleased(void)
{
	if(textureCreator->GetCurrentBrush() == "POLYGON")
	{
		if(fantomVertexDrawing && !selectAreaDrawing)
		{
			bool polygonIsLocked = false;
			bool vertexIsOverlay = false;
			for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(fantomVertex->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
				{
					if(it->GetNode() == vertices.GetHead())
					{
						polygonIsLocked = true;
						break;
					}
					else 
					{ 
						vertexIsOverlay = true; 
						break;
					} 
				}
			}

			if(polygonIsLocked)
			{
				CreateMesh();
			}
			else if(!vertexIsOverlay)
			{
				GraphicsPointEx* point = CreateNewVertex(fantomVertex->GetPosition()); 
				vertices.AddToTail(point);
				
				if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
				{
					UpdateFantomMesh();
				} 
				else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
				{
					CheckEdgeIntersection();
				}

				textureCreator->UpdateAmountOfPoints(vertices.GetSize());
			}
			else
			{
				if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
				{
					UpdateFantomMesh();
				}   
				else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
				{
					CheckEdgeIntersection();
				}
			}
			fantomVertex->SetShowEnable(false);
			fantomVertexDrawing = false;
			if(fantomEdgeDrawing)
			{
				fantomEdge->SetShowEnable(false);
				fantomEdgeDrawing = false;
			}
		}
		else if(currentVertex) 
		{
			bool polygonIsLocked = false;
			bool vertexIsOverlay = false;
			for(MagicList<GraphicsPointEx>::AddressDerived* it1 = vertices.GetHeadAddress(); it1 != NIL; it1 = it1->GetRight())
			{
				for(MagicList<GraphicsPointEx>::AddressDerived* it2 = selectedPoints.GetHeadAddress(); it2 != NIL; it2 = it2->GetRight())
				{
					if(it1->GetNode() != it2->GetNode())
					{
						if(Quad(it1->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(it2->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
						{
							if((it1->GetNode() == vertices.GetHead() && it2->GetNode() == vertices.GetTail()) ||
							   (it2->GetNode() == vertices.GetHead() && it1->GetNode() == vertices.GetTail()))
							{
								polygonIsLocked = true;
							}
							else
							{
								vertexIsOverlay = true; 
								break;
							}
						}
					}
				}
			}

			if(vertexIsOverlay)
			{
				Vector3 offset = startCurrentVertexPosition - currentVertex->GetPosition();
				for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
				{
					it->GetNode()->DisplacePosition(offset);
				}
				
				if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
				{
					UpdateFantomMesh();
				}
				else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
				{
					CheckEdgeIntersection();
				}
			}
			else if(polygonIsLocked)
			{
				CreateMesh();
			}
			else
			{
				if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
				{
					UpdateFantomMesh();
				}
				else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
				{
					CheckEdgeIntersection();
				}
			}

			currentVertex = NIL;
		}
	}
	else if(textureCreator->GetCurrentBrush() == "CIRCLE")
	{  
		if(fantomVertexDrawing)
		{ 
			fantomVertexDrawing = false; 

			fantomMesh->Destroy();
			Mesh* mesh = new Mesh();
			mesh->SetBoundingVolumeType(Mesh::BOUNDING_QUAD);

			mesh->CreateCircle(Circle(startCurrentVertexPosition, circleRadius), textureCreator->GetCircleSegments());
		
			GraphicsSurfaceEx* object = new GraphicsSurfaceEx();
			object->SetFrameIndex(textureCreator->GetAnimationCurrentFrame());

			object->BindMesh(mesh);

			shapes.AddToTail(mesh);
			objects.AddToTail(object);

			textureCreator->CreateShape();
		}
	}
	else if(textureCreator->GetCurrentBrush() == "QUAD")
	{ 
		if(fantomVertexDrawing)
		{ 
			fantomVertexDrawing = false; 

			fantomMesh->Destroy();
			Mesh* mesh = new Mesh();
			mesh->SetBoundingVolumeType(Mesh::BOUNDING_QUAD);

			mesh->CreateQuad(Quad(startCurrentVertexPosition, quadSize));
		
			GraphicsSurfaceEx* object = new GraphicsSurfaceEx();
			object->SetFrameIndex(textureCreator->GetAnimationCurrentFrame());

			object->BindMesh(mesh);

			shapes.AddToTail(mesh);
			objects.AddToTail(object);

			textureCreator->CreateShape();
		}
	}
}
 
 
 
void TextureViewer::VectorLerpLeftButtonReleased(void)
{
	if(fantomVertexDrawing && !selectAreaDrawing)
	{
		bool vertexIsOverlay = false;
		for(MagicList<GraphicsPointEx>::AddressDerived* it = vertices.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			if(Quad(it->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(fantomVertex->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
			{
				vertexIsOverlay = true; 
				break;
			}
		}

		if(!vertexIsOverlay)
		{
			GraphicsPointEx* point = CreateNewVertex(fantomVertex->GetPosition());
			vertices.AddToTail(point);
			
			UpdateVectorLerpCurve();

			textureCreator->UpdateAmountOfPoints(vertices.GetSize());
		} 
		else 
		{
		}
		fantomVertex->SetShowEnable(false);
		fantomVertexDrawing = false;
		if(fantomEdgeDrawing)
		{
			fantomEdge->SetShowEnable(false);
			fantomEdgeDrawing = false;
		}
	}
	else if(currentVertex) 
	{ 
		bool vertexIsOverlay = false;
		for(MagicList<GraphicsPointEx>::AddressDerived* it1 = vertices.GetHeadAddress(); it1 != NIL; it1 = it1->GetRight())
		{
			for(MagicList<GraphicsPointEx>::AddressDerived* it2 = selectedPoints.GetHeadAddress(); it2 != NIL; it2 = it2->GetRight())
			{
				if(it1->GetNode() != it2->GetNode())
				{
					if(Quad(it1->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint())).IsIntersected(Quad(it2->GetNode()->GetPosition(), Vector2(pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint(), pointSizeInUnits * camera3d.Get()->GetDistanceToViewingPoint()))))
					{
						vertexIsOverlay = true; 
						break;
					}
				}
			}
		}

		if(vertexIsOverlay)
		{
			Vector3 offset = startCurrentVertexPosition - currentVertex->GetPosition();
			for(MagicList<GraphicsPointEx>::AddressDerived* it = selectedPoints.GetHeadAddress(); it != NIL; it = it->GetRight())
			{
				it->GetNode()->DisplacePosition(offset);
			}
		}

		currentVertex = NIL;
	}
}


void TextureViewer::HitboxRightButtonReleased(void)
{
	if(textureCreator->GetCurrentBrush() == "POLYGON")
	{
		if(selectAreaDrawing)
		{
			selectAreaDrawing = false;
		}
	}
}


void TextureViewer::VectorLerpRightButtonReleased(void)
{
	if(selectAreaDrawing)
	{
		selectAreaDrawing = false;
	}
}


Mouse::Event* TextureViewer::MouseEventFilter(Mouse::Event* _event)
{
	camera3d.Get()->MouseEventFilter(_event);
	

	if(_event->GetEventType() == Mouse::KEY_DOWN)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{	
			if(textureCreator->GetCurrentTabName() == "Vector lerp")
			{
				VectorLerpLeftButtonPressed();
			}
			else if(textureCreator->GetCurrentTabName() == "Hitbox")
			{
				if(pivotShowEnable)
				{
					UpdatePivot();
				}
				else
				{
					HitboxLeftButtonPressed();
				}
			}
		}
		else if(_event->GetKey() == Mouse::RIGHT_BUTTON)
		{ 
			if(textureCreator->GetCurrentTabName() == "Vector lerp")
			{
				VectorLerpRightButtonPressed();
			}
			else if(textureCreator->GetCurrentTabName() == "Hitbox")
			{
				if(pivotShowEnable)
				{}
				else 
				{
					HitboxRightButtonPressed();
				}
			}
		}
	}
	else if(_event->GetEventType() == Mouse::MOVE)
	{
		UpdateCursorPosition();

		if(textureCreator->GetCurrentTabName() == "Vector lerp")
		{
			VectorLerpMouseMove();
		}
		else if(textureCreator->GetCurrentTabName() == "Hitbox")
		{
			HitboxMouseMove();
		} 
	} 
	else if(_event->GetEventType() == Mouse::KEY_UP)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{
			if(textureCreator->GetCurrentTabName() == "Vector lerp")
			{
				VectorLerpLeftButtonReleased();
			}
			else if(textureCreator->GetCurrentTabName() == "Hitbox")
			{
				HitboxLeftButtonReleased();
			} 
		} 
		else if(_event->GetKey() == Mouse::RIGHT_BUTTON)
		{
			if(textureCreator->GetCurrentTabName() == "Vector lerp")
			{
				VectorLerpRightButtonReleased();
			}
			else if(textureCreator->GetCurrentTabName() == "Hitbox")
			{
				HitboxRightButtonReleased();
			} 
		}
	} 
	else if(_event->GetEventType() == Mouse::WHEEL)
	{
		UpdateCameraZoom();
	}

	return _event;
}



void TextureViewer::CurrentBuildModeIsChanged(StringANSI _name)
{
	if(_name == "DELAUNAY")
	{
		UpdateFantomMesh();
	}
	else if(_name == "SPLIT")
	{
		fantomMesh->Destroy();
	}
}

 
void TextureViewer::CurrentTabIsChanged(StringANSI _name)
{
	if(_name == "Vector lerp")
	{
		intersectedLines.Clear();

		UpdateVectorLerpCurve();
	}
	else if(_name == "Hitbox") 
	{
		if(textureCreator->GetCurrentBrush() == "POLYGON")
		{
			if(textureCreator->GetCurrentBuildMode() == "DELAUNAY")
			{
				UpdateFantomMesh();
			} 
			else if(textureCreator->GetCurrentBuildMode() == "SPLIT")
			{
				CheckEdgeIntersection();
			}
		}
	}
}



void TextureViewer::UpdateCameraZoom(void)
{
	Ratio::_SetKnown<float>(Ratio::WHOLE, camera3d.Get()->GetDistanceToViewingPoint());
	Ratio::_SetKnown<float>(Ratio::PART, textureCreator->GetZoom100());
	textureCreator->SetZoomValue(Ratio::_GetUnknown<float>(Ratio::PART_RATIO));
}



Keyboard::Event* TextureViewer::KeyboardEventFilter(Keyboard::Event* _event)
{
	if(_event->GetEventType() == Keyboard::KEY_DOWN && !_event->IsAutoRepeat())
	{
		if(_event->GetKey() == Keyboard::DEL) 
		{
			DestroySelectedPoints();
		}
		if(_event->GetKey() == Keyboard::R) 
		{
			vectorLerpFloatLerp->SetT(0.5);
		}
		if(_event->GetKey() == Keyboard::T) 
		{
			vectorLerpFloatLerp->SetT(0.9);
		}
		if(_event->GetKey() == Keyboard::Y) 
		{
			vectorLerpFloatLerp->SetT(1);
		}
		
	}

	GetKeyboard().ClearEventList();
	return _event;
}
