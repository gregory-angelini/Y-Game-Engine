
#include "FloatLerpViewer.h"
#include <Editor/FloatLerpCreator/FloatLerpCreator.h>
#include <Editor/Editor/Editor.h>



FloatLerpViewer::FloatLerpViewer(FloatLerpCreator* _floatLerpCreator): floatLerpCreator(_floatLerpCreator), camera(NIL), font(NIL), scene(NIL), graph(NIL), rightPressed(false), index(0), lerp(NIL), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), arrowCursor(NIL), graphLerp(NIL), lerpMarker(false), startMarkerMoving(false)
{}



bool FloatLerpViewer::Create(Engine::Define _define)
{
	if(Engine::Create(_define))
	{
		minCameraScale = 0.1f;
		maxCamaraScale = 200.0f;
		ñamaraZoomSpeed = 10.0f;
		{
			scene = new Scene();
		}
		{
			Font2D::_CheckSupportedFonts(this);

			Font2D::Define def;
			def.fontName = "Arial";
			def.fontSize = 16;
			def.fontWeight = 600;
			def.color = RGBA(RGBA::GRAY_COLOR).GetRGBAf();

			font = new Font2D();
			font->Create(def);
			scene->AddToLayer(font);
		}
		{
			Camera3D::Define def;
			def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;
			def.rotationEnable = false;
			def.yawClampEnable = true;
			def.pitchClampEnable = true;
			def.zoomSpeed = 3.0f;
			def.zoomEnable = true;

			def.pitchMax = Angle::_DegreesToRadians(40);
			def.pitchMin = Angle::_DegreesToRadians(-40);
			def.yawMax = Angle::_DegreesToRadians(40);
			def.yawMin = Angle::_DegreesToRadians(-40);


			camera = new Camera3D();
			camera->Create(def);
			camera->SetZoomNear(camera->GetZoomNear() * 100);
			camera->SetZoomFar(camera->GetZoomFar() * 100);
			camera->SetDistanceToViewingPoint(140.0f);
			scene->BindCamera(camera);
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
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/unclenched.png").toAscii().data());

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
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/clenched.png").toAscii().data());

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
			texture->Create(Texture2D::QUAD_TEXTURE_MAP, QString(Editor::_GetWorkDir() + Editor::_GetResDir() + "DesignToolBar/cursor.png").toAscii().data());

			arrowCursor = new TexturingSurface();
			
			Mesh* mesh = new Mesh();
			mesh->CreateQuad(Quad(Vector3(14/2, 24/2, -0.102), Vector2(14, 24)));
			arrowCursor->BindMesh(mesh);

			arrowCursor->BindFrontTexture(texture);

			arrowCursor->SetShowEnable(true);

			scene2d->AddToRenderList(arrowCursor);
		}

	
		lerp = new FloatLerp();
		lerp->SetLerpMode(FloatLerp::FIXED_TIME);
		lerp->SetPlayingSpeed(1.0f);
		lerp->SetTarget(1.0f);
		scene->AddToUpdateList(lerp);
		{
			graph = new Graph(Vector2(100.0f, 100.0f), 16, 16);
			graph->SetXRangeShowEnable(false);
			graph->SetYRangeShowEnable(false);
			graph->SetXAxisName("Time");
			graph->SetYAxisName("Speed");
			graph->SetFont(font);
			

				
			graphLerp = graph->CreateFunction("func");
			graphLerp->SetXMarkerColor(RGBA(55, 111, 194, 255) + RGBA(0, 0, 5, 0));
			graphLerp->SetYMarkerColor(RGBA(55, 111, 194, 255) + RGBA(0, 5, 0, 0));
			graphLerp->SetXMarkerWidth(5.0f);
			graphLerp->SetYMarkerWidth(5.0f);
			graphLerp->SetFont(font);
			graph->GetMarkList().AddToTail(Graph::Mark(0, Graph::Mark::AXIS_X, RGBA(55, 111, 194, 255)));
			
			UpdateMark();

			scene->AddToRenderList(graph);
		}
	

		float maxSize = Numerical<float>::_GetMax(graph->GetSize().x, graph->GetSize().y);
					
		camera->SetZoomFar(maxSize + Angle::_RadiansToDegrees(camera->GetFov()) + maxCamaraScale);
		camera->SetZoomNear(maxSize * minCameraScale);
		camera->SetZoomSpeed(maxSize/ñamaraZoomSpeed);

		camera->SetDistanceToViewingPoint(floatLerpCreator->GetZoom100());
		SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));
		return true;
	}
	return false;
}



			
FloatLerpViewer::~FloatLerpViewer(void)
{
	delete graph;

	delete clenchedHandCursor;
	delete unclenchedHandCursor;
	delete arrowCursor;

	delete camera;
	delete camera2d;
	delete font;
	delete scene;
	scene2d;
} 



float FloatLerpViewer::GetParabolaPlayingDurationInSec(void)
{ 

	float current = 0.0f, t = 0.0f, target = lerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

	{
		float steps;

		while(current < target)
		{
			steps = (target - current) / (lerp->GetPlayingSpeed() * delta);
						
			t += 1.0f / steps;
										 
			if(t > 1.0f) 
			{ 
				current = target;
			}
			else 
			{
				current = current * (1.0f - t) + target * t;
			}
			duration += delta;
		}
	}
	return duration;
}



void FloatLerpViewer::SetParabolaPlayingDurationInSec(float _duration)
{ 

	if(_duration >= 0.1f)
	{
		int32 steps = 0;
		{
			float scale[4] = { 0.1f, 0.001f, 0.0001f, 0.00001f };

			float step = 1.0f;
			float playingSpeed = 0.00001f;

			while(true)
			{
				playingSpeed += step;
				lerp->SetPlayingSpeed(playingSpeed);

				if(_duration > GetParabolaPlayingDurationInSec())
				{
					playingSpeed -= step;
					lerp->SetPlayingSpeed(playingSpeed);
					if(steps < 4)
					{
						step = scale[steps];
						++steps;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}

	
float FloatLerpViewer::GetMagneticPlayingDurationInSec(void)
{ 

	float current = 0.0f, t = 0.0f, target = lerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

	
	{
		float steps;

		while(current < target)
		{
			steps = (target - current) / (lerp->GetPlayingSpeed() * delta);
				
			t += 1.0f / steps;
								
			if(t > 1.0f)
			{  
				current = target;
			}
			else 
			{ 
				current = target * t;
			}
			duration += delta;
		}
	}
	return duration;
}



void FloatLerpViewer::SetMagneticPlayingDurationInSec(float _duration)
{ 

	if(_duration >= 0.3f)
	{
		int32 steps = 0;
		{
			float scale[4] = { 0.1f, 0.001f, 0.0001f, 0.00001f };

			float step = 1.0f;
			float playingSpeed = 0.00001f;

			while(true)
			{  
				playingSpeed += step;
				lerp->SetPlayingSpeed(playingSpeed);

				if(_duration > GetMagneticPlayingDurationInSec())
				{
					playingSpeed -= step;
					lerp->SetPlayingSpeed(playingSpeed);
					if(steps < 4)
					{
						step = scale[steps];
						++steps;
					}
					else
					{
						break;
					}
				}
			}
		}
	}
}


float FloatLerpViewer::GetDampingPlayingDurationInSec(void)
{ 

	float current = 0.0f, t = 0.0f, target = lerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

	
	{
		float step = 1.0f / lerp->GetPlayingSpeed() * delta;

		while(current < target)
		{
			float previous = current;
			 
			current = current * (1.0f - step) + target * step;
							
			if(current >= target)
			{ 
				current = target; 
			}
			else if(current - previous <= 0.000001f)
			{
				current = target;
			}
			duration += delta;
		}
	}
	return duration;
} 
		

void FloatLerpViewer::SetDampingPlayingDurationInSec(float _duration)
{ 

	if(_duration >= 0.1f)
	{
		int32 steps = 6;
		{
			float step = 1.0f;
			float playingSpeed = step; 
			lerp->SetPlayingSpeed(playingSpeed);

			if(_duration > GetDampingPlayingDurationInSec())
			{
				while(_duration > GetDampingPlayingDurationInSec())
				{  
					bool scale = false;

					if((playingSpeed + step) > 0.0f)
					{
						playingSpeed += step;
						lerp->SetPlayingSpeed(playingSpeed);
					}
					else
					{
						scale = true;
					}

					if(_duration < GetDampingPlayingDurationInSec())
					{
						playingSpeed -= step;
						lerp->SetPlayingSpeed(playingSpeed);
						scale = true;
					}

					if(scale)
					{
						if(steps > 0)
						{
							--steps;
							step /= 10.0f;
						}
						else
						{
							break;
						}
					}
				}
			}
			else if(_duration < GetDampingPlayingDurationInSec())
			{ 
				while(_duration < GetDampingPlayingDurationInSec())
				{   
					bool scale = false;

					if((playingSpeed - step) > 0.0f)
					{
						playingSpeed -= step;
						lerp->SetPlayingSpeed(playingSpeed);
					}
					else
					{
						scale = true;
					}

					if(_duration > GetDampingPlayingDurationInSec())
					{
						playingSpeed += step;
						lerp->SetPlayingSpeed(playingSpeed);
						scale = true;
					}

					if(scale)
					{
						if(steps > 0)
						{
							--steps;
							step /= 10.0f;
						}
						else
						{
							break;
						}
					}
				}
			}
		} 
	}
}


void FloatLerpViewer::ShowCursor(void)
{
	if(Mouse::_IsShowCursor())
	{
		Mouse::_SetShowCursor(false);
	}
}


void FloatLerpViewer::HideCursor(void)
{
	if(!Mouse::_IsShowCursor())
	{
		Mouse::_SetShowCursor(true);
	}
} 


void FloatLerpViewer::Update(float _delta)
{
	scene->Update(_delta);
	if(lerp->IsPlaying())
	{
		graph->AddVertex("func", Vector2(_delta, lerp->GetDelta()));
	}

	CheckCollisionWithMarkers();
	if(!lerp->IsPlaying() && (lerp->GetCurrent() != lerp->GetTarget()))
	{
		floatLerpCreator->PrepareToPlay();
	}
	else if(lerp->IsPlaying())
	{ 
		floatLerpCreator->PrepareToPause();
	}
	else if(!lerp->IsPlaying() && (lerp->GetCurrent() == lerp->GetTarget()))
	{
		floatLerpCreator->PrepareToPlay();
	}

	floatLerpCreator->UpdateCurrent(); 
} 


void FloatLerpViewer::Draw(float _delta)
{

	ClearScene();

	scene->Draw();
	if(lerp->GetPlayingMode() & PlayingObject::LOOP_PLAYING)
	{
		UpdateMark();
	} 
	GetPixelColor(Mouse::_GetCursorPosition(this), pixelColorUnderCursor);
	scene2d->Draw();
} 


void FloatLerpViewer::CheckCollisionWithMarkers(void)
{
	if(!startMarkerMoving && !rightPressed)
	{
		if(graphLerp->GetXMarkerColor() == pixelColorUnderCursor.GetRGBAf())
		{ 
			Select();

			clenchedHandCursor->SetShowEnable(false);
			arrowCursor->SetShowEnable(false);
			unclenchedHandCursor->SetShowEnable(true);
		}
		else
		{
			Unselect();
	
			clenchedHandCursor->SetShowEnable(false);
			arrowCursor->SetShowEnable(true);
			unclenchedHandCursor->SetShowEnable(false);
		}
	}
}
			


Graph* FloatLerpViewer::GetGraph(void)const
{
	return graph;
}


Camera3D* FloatLerpViewer::GetCamera(void)const
{
	return camera;
}


void FloatLerpViewer::ShowCurveFromStartToCurrent(void)
{
	bool loop = !lerp->IsSingleShotPlayingEnabled();
	if(loop) { lerp->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING); }

	graphLerp->ClearHistory();

	float time = lerp->GetElapsedTimeInSec();
	lerp->Stop();
	lerp->Play();
	while(lerp->IsPlaying() && lerp->GetElapsedTimeInSec() <= time)
	{
		float fixedDelta = 1.0f / 60.0f;
		lerp->Update(fixedDelta);
		graph->AddVertex("func", Vector2(fixedDelta, lerp->GetDelta()));
	}

	if(loop) { lerp->SetPlayingMode(PlayingObject::LOOP_PLAYING); }
} 



Mouse::Event* FloatLerpViewer::MouseEventFilter(Mouse::Event* _event)
{
	camera->MouseEventFilter(_event);

	if(_event->GetEventType() == Mouse::KEY_DOWN)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{
			Vector3 currentMarkerMoving;

			if(graph->GetPlane().IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &currentMarkerMoving) != -1)
			{
				if(lerpMarker && !rightPressed)
				{
					startMarkerMoving = true;

					clenchedHandCursor->SetShowEnable(true);
					arrowCursor->SetShowEnable(false);
					unclenchedHandCursor->SetShowEnable(false);

					startMarkerPosition = currentMarkerMoving;
				}
			}
		}
	}
	else if(_event->GetEventType() == Mouse::KEY_UP)
	{
		if(_event->GetKey() == Mouse::LEFT_BUTTON)
		{
			if(startMarkerMoving) 
			{ 
				startMarkerMoving = false;

				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
				unclenchedHandCursor->SetShowEnable(true);

				floatLerpCreator->UpdatePlayingSpeed();
			}
		}
		else if(_event->GetKey() == Mouse::RIGHT_BUTTON)
		{
			if(rightPressed)
			{
				camera->SetRotationEnable(false);
				rightPressed = false;

				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(true);
				unclenchedHandCursor->SetShowEnable(false);


				Vector2 position(floatLerpCreator->GetViewport()->x(), floatLerpCreator->GetViewport()->y());
				position += Vector2(floatLerpCreator->x(), floatLerpCreator->y());
				
				Vector2 size(floatLerpCreator->GetViewport()->width(), floatLerpCreator->GetViewport()->height());
				size += Vector2(floatLerpCreator->GetViewport()->x(), floatLerpCreator->GetViewport()->y());
				size += Vector2(0, 16);

				Mouse::_SetCursorPosition(position + size / 2.0f, 0);

				camera->SetZoomEnable(true);
			}
		}
	}	
	else if(_event->GetEventType() == Mouse::WHEEL)
	{
		if(!rightPressed)
		{
			Unselect();

		    clenchedHandCursor->SetShowEnable(false);
			arrowCursor->SetShowEnable(true);
			unclenchedHandCursor->SetShowEnable(false);

	
			Ratio::_SetKnown<float>(Ratio::WHOLE, floatLerpCreator->GetZoom100());
			Ratio::_SetKnown<float>(Ratio::PART, camera->GetDistanceToViewingPoint());
			floatLerpCreator->SetZoomValue(Ratio::_GetUnknown<float>(Ratio::PART_RATIO));
			floatLerpCreator->SetZoomValue(Ratio::_GetUnknown<float>(Ratio::PART_RATIO));
		}
	}
	else if(_event->GetEventType() == Mouse::MOVE)
	{
		if(GetMouse().IsKeyPressed(Mouse::RIGHT_BUTTON))
		{

			if(!rightPressed)
			{
				Mouse::_MoveCursorToCenter(Renderer::_GetPicked());
				camera->SetRotationEnable(true);
				rightPressed = true;
				clenchedHandCursor->SetShowEnable(false);
				arrowCursor->SetShowEnable(false);
				unclenchedHandCursor->SetShowEnable(false);

				camera->SetZoomEnable(false);
				Unselect();
			}
		}
		Vector2 cursorPosition = Mouse::_GetCursorPosition(Renderer::_GetPicked());
		cursorPosition.y = Renderer::_GetPicked()->GetViewportSize().y - cursorPosition.y;

		arrowCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - (arrowCursor->GetFrontTexture()->GetImageSize().y-1), 0.0f));
		unclenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((unclenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));
		clenchedHandCursor->SetPosition(Vector3(cursorPosition.x, cursorPosition.y - ((clenchedHandCursor->GetFrontTexture()->GetImageSize().y-1)/2), 0.0f));


		if(startMarkerMoving)
		{
			Vector3 currentCursorPosition;

			if(graph->GetPlane().IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), &currentCursorPosition) != -1)
			{
				if(lerp->GetLerpMode() == FloatLerp::UNITS_PER_SECOND)
				{
					
					{

						float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();
						Vector2 halfSize = graph->GetSize() / 2.0f;
						float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();
						graphLerp->ClearHistory();
						lerp->SetPlayingDurationInSec(targetTime);
						lerp->Stop();
						lerp->Play();
						bool loop = !lerp->IsSingleShotPlayingEnabled();
						if(loop) { lerp->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING); }

						while(lerp->IsPlaying())
						{
							float fixedDelta = 1.0f / 60.0f;
							lerp->Update(fixedDelta);
							graph->AddVertex("func", Vector2(fixedDelta, lerp->GetDelta()));
						}	

						if(loop) { lerp->SetPlayingMode(PlayingObject::LOOP_PLAYING); }
					}
				}
						
				if(lerp->GetLerpMode() == FloatLerp::FIXED_TIME)
				{
					
					{

						float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();
						Vector2 halfSize = graph->GetSize() / 2.0f;
						float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();
						graphLerp->ClearHistory();
						lerp->SetPlayingDurationInSec(targetTime);
						lerp->Stop();
						lerp->Play();

						bool loop = !lerp->IsSingleShotPlayingEnabled();
						if(loop) { lerp->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING); }
						while(lerp->IsPlaying())
						{
							float fixedDelta = 1.0f / 60.0f;
							lerp->Update(fixedDelta);
							graph->AddVertex("func", Vector2(fixedDelta, lerp->GetDelta()));
						}
						
						if(loop) { lerp->SetPlayingMode(PlayingObject::LOOP_PLAYING); }
					}
				}

				if(lerp->GetLerpMode() == FloatLerp::PARABOLA)
				{
					
					{

						float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();
						Vector2 halfSize = graph->GetSize() / 2.0f;
						float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();
						graphLerp->ClearHistory();
						SetParabolaPlayingDurationInSec(targetTime);
						lerp->Stop();
						lerp->Play();

						bool loop = !lerp->IsSingleShotPlayingEnabled();
						if(loop) { lerp->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING); }
						while(lerp->IsPlaying())
						{
							float fixedDelta = 1.0f / 60.0f;
							lerp->Update(fixedDelta);
							graph->AddVertex("func", Vector2(fixedDelta, lerp->GetDelta()));
						}
						
						if(loop) { lerp->SetPlayingMode(PlayingObject::LOOP_PLAYING); }
					}
				}
					
				if(lerp->GetLerpMode() == FloatLerp::DAMPING)
				{
					
					{

						float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();
						Vector2 halfSize = graph->GetSize() / 2.0f;
						float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();
						graphLerp->ClearHistory();
						SetDampingPlayingDurationInSec(targetTime);
						lerp->Stop();
						lerp->Play();

						bool loop = !lerp->IsSingleShotPlayingEnabled();
						if(loop) { lerp->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING); }
						while(lerp->IsPlaying())
						{
							float fixedDelta = 1.0f / 60.0f;
							lerp->Update(fixedDelta);
							graph->AddVertex("func", Vector2(fixedDelta, lerp->GetDelta()));
						}
						
						if(loop) { lerp->SetPlayingMode(PlayingObject::LOOP_PLAYING); }
					}
				}
						
				if(lerp->GetLerpMode() == FloatLerp::MAGNETIC)
				{
					
					{

						float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();
						Vector2 halfSize = graph->GetSize() / 2.0f;
						float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();
						graphLerp->ClearHistory();
						SetMagneticPlayingDurationInSec(targetTime);
						lerp->Stop();
						lerp->Play();
	
						bool loop = !lerp->IsSingleShotPlayingEnabled();
						if(loop) { lerp->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING); }
						while(lerp->IsPlaying())
						{
							float fixedDelta = 1.0f / 60.0f;
							lerp->Update(fixedDelta);
							graph->AddVertex("func", Vector2(fixedDelta, lerp->GetDelta()));
						}
						
						if(loop) { lerp->SetPlayingMode(PlayingObject::LOOP_PLAYING); }
					}
				}
								
				UpdateMark();
				floatLerpCreator->UpdatePlayingSpeed();
			}
		}
	}

	GetMouse().ClearEventList();
	return _event;
} 


void FloatLerpViewer::ShowCurveForAllLerps(void)
{
	graphLerp->ClearHistory();
	lerp->Stop();
	
	bool loop = !lerp->IsSingleShotPlayingEnabled();
	if(loop) { lerp->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING); }

	{
		lerp->Play();
		while(lerp->IsPlaying()) 
		{
			float fixedDelta = 1.0f / 60.0f;
			lerp->Update(fixedDelta);
			graph->AddVertex("func", Vector2(fixedDelta, lerp->GetDelta()));	
		}
	}
	
	if(loop) { lerp->SetPlayingMode(PlayingObject::LOOP_PLAYING); }
}


void FloatLerpViewer::SetPlayingDurationInSec(float _duration)
{
	if(lerp->GetLerpMode() == FloatLerp::DAMPING)
	{
		SetDampingPlayingDurationInSec(_duration);
	}
	else if(lerp->GetLerpMode() == FloatLerp::MAGNETIC)
	{
		SetMagneticPlayingDurationInSec(_duration);
	}
	else if(lerp->GetLerpMode() == FloatLerp::PARABOLA)
	{
		SetParabolaPlayingDurationInSec(_duration);
	}
	else
	{
		lerp->SetPlayingDurationInSec(_duration);
	}
}



float FloatLerpViewer::GetPlayingDurationInSec(void)
{
	float mod = 1.0f;

	if(lerp->GetPlayingMode() & PlayingObject::PING_PONG_PLAYING)
	{
		mod = 2.0f;
	}
	if(lerp->GetPlayingMode() & PlayingObject::LOOP_PLAYING)
	{
	}

	if(lerp->GetLerpMode() == FloatLerp::DAMPING)
	{
		return GetDampingPlayingDurationInSec() * mod;
	}
	else if(lerp->GetLerpMode() == FloatLerp::MAGNETIC)
	{
		return GetMagneticPlayingDurationInSec() * mod;
	}
	else if(lerp->GetLerpMode() == FloatLerp::PARABOLA)
	{
		return GetParabolaPlayingDurationInSec() * mod;
	}
	else
	{
		return lerp->GetPlayingDurationInSec() * mod;
	}		
}


void FloatLerpViewer::UpdateMark(void)
{
	if(lerp->GetPlayingMode() & PlayingObject::LOOP_PLAYING)
	{
		graph->GetMarkList()[0]->key = Graph::Mark(graphLerp->GetCurrentX(), Graph::Mark::AXIS_X, graphLerp->GetColorRGBA());
	}
	else
	{
		graph->GetMarkList()[0]->key = Graph::Mark(GetPlayingDurationInSec(), Graph::Mark::AXIS_X, graphLerp->GetColorRGBA());
	}
	{
	}
}


void FloatLerpViewer::Select(void)
{
	Unselect();

	lerpMarker = true;

	clenchedHandCursor->SetShowEnable(false);
	arrowCursor->SetShowEnable(false);
	unclenchedHandCursor->SetShowEnable(true);
}


void FloatLerpViewer::Unselect(void)
{
	lerpMarker = false;
}


Keyboard::Event* FloatLerpViewer::KeyboardEventFilter(Keyboard::Event* _event)
{
	GetKeyboard().ClearEventList();

	return _event;
}



FloatLerp* FloatLerpViewer::GetFloatLerp(void)const
{
	return lerp;
}


void FloatLerpViewer::SetTarget(float _target)
{
	float duration = lerp->GetPlayingDurationInSec();

	lerp->SetTarget(_target);

	SetPlayingDurationInSec(duration);
}


void FloatLerpViewer::SetPlayingSpeed(float _speed)
{
	lerp->SetPlayingSpeed(_speed);
}


void FloatLerpViewer::PlayLerp(void)
{
	lerp->Play();
}


void FloatLerpViewer::StopLerp(void)
{
	lerp->Stop();
}



void FloatLerpViewer::PauseLerp(void)
{
	lerp->Pause();
}


void FloatLerpViewer::ClearHistory(void)
{
	graphLerp->ClearHistory();
}
