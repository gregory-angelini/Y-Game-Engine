// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>
#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>




// класс, реализующий графический прямоугольный параллелепипед
class MyBox: public GraphicsSurface,// графическая поверхность
	         public LogicObject// логический объект
{
	// DATA
	private: Box box;
	private: FloatLerp* floatLerp;// числовой интерполятор
	private: VectorLerp* vectorLerp;// векторный интерполятор


	// METHODS
	// инициализирующий конструктор
	public: MyBox(void)
	{
		box = Box(Vector3(), Vector3(0.5f, 0.5f, 0.5f));
		
		// форма...
		Mesh* mesh = new Mesh();
		mesh->CreateBox3D(box);
		mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
		BindMesh(mesh);
		

		// цвет...
		SetColor(RGBA(RGBA::GREEN_COLOR)); 


		// столкновения...
		SetCollisionCheckEnable(true);// разрешить участие в столкновениях
		Connect(PolygonalSurface::COLLISION_DETECTED_MESSAGE, this, Caller<>(this, &MyBox::CollisionStateIsChanged));

			
		// движение...
		floatLerp = new FloatLerp();
		floatLerp->Stop();// перевести в начало и остановить
		floatLerp->SetLerpMode(FloatLerp::UNITS_PER_SECOND);// режим интерполяции
		floatLerp->SetPlayingSpeed(0.5f);// скорость движения в единицах/секунду

		vectorLerp = new VectorLerp();
		vectorLerp->BindFloatLerp(floatLerp);
	} 


	// деструктор
	public: virtual ~MyBox(void)
	{
		Destroying();

		delete floatLerp;
		delete vectorLerp;
	}



	// реакция на столкновение/выход из столкновения
	public: void CollisionStateIsChanged(bool _enable)
	{
		if(_enable)
		{
			SetColor(RGBA(RGBA::RED_COLOR));
		}
		else
		{
			SetColor(RGBA(RGBA::GREEN_COLOR));
		}
	}

	// возвращает описание прямоугольного параллелепипеда
	public: Box GetBox(void)const
	{
		return box;
	}
		

	// обновляет состояние объекта
	// примечание: в качестве параметра принимается дельта - время, прошедшее с последнего вызова этой функции
	public: virtual void Update(float _delta = 0.0f)
	{
		SetRotation(GetRotation() * Quaternion(AxisAngle(Vector3::UNIT_Y, _delta)));// вращаем объект вокруг оси Y

		if(floatLerp->IsPaused() && floatLerp->IsFinish())// если достигнута целевая позиция
		{
			SetTarget(GetRandomPosition());// задать новую целевую позицию
		}
		else
		{
			floatLerp->Update(GetDeltaMultiplier() * _delta);// двигать в целевую позицию 

			SetPosition(vectorLerp->GetCurrent());// обновить текущую позицию объекта
		}
	}


	// задает новую целевую позицию для движения
	public: void SetTarget(Vector3 _target)
	{
		vectorLerp->Set(GetPosition(), _target);// задать исходную и целевую контрольные вершины
 
		floatLerp->SetTarget(Line::_GetLength(GetPosition(), _target) / 10.0f);// делаем так, чтобы скорость движения не зависила от дистанции до цели
		
		// начать воспроизведение сначала...
		floatLerp->Stop();
		floatLerp->Play();	 
	}

	// возвращает случайную позицию 
	public: Vector3 GetRandomPosition(void)
	{
		/* [-5, 5] - это размер пространства, в рамках которог может перемещаться объект */ 
		return Vector3(Random::_RandomFloat(-5.0f + GetBox().GetWidth() / 2.0f, (5.0f - GetBox().GetWidth() / 2.0f)), 
					   GetBox().GetHeight() / 2.0f, // над уровнем пола
					   Random::_RandomFloat(-5.0f + GetBox().GetLength() / 2.0f, (5.0f - GetBox().GetLength() / 2.0f)));
	}
};



// класс, реализующий графический квад 
class MyQuad: public GraphicsSurface
{
	// DATA
	private: Quad quad;// описание квада


	// METHODS  
	// инициализирующий конструктор
	public: MyQuad(void)
	{
		quad = Quad(Vector3(), Vector2(10.f, 10.0f));
		
		Mesh* mesh = new Mesh();
		mesh->CreateQuad2D(quad);
		BindMesh(mesh);

		SetColor(RGBA(RGBA::WHITE_COLOR)); 
		SetCollisionCheckEnable(true);// разрешить участие в столкновениях

		SetRotation(Quaternion(AxisAngle(Vector3::UNIT_X, Angle::_DegreesToRadians(-90.0f))));// вращаем вокруг оси X
	}


	// деструктор
	public: virtual ~MyQuad(void)
	{
		Destroying();
	}


	// возвращает описание квада 
	public: Quad GetQuad(void)const
	{
		return quad;
	}
};



//класс, реализующий графическую пирамиду 
class MyPyramid: public GraphicsSurface,// графическая поверхность
	             public LogicObject// логический объект
{
	// DATA
	private: float radius;// радиус пирамиды
	private: FloatLerp* floatLerp;// числовой интерполятор
	private: VectorLerp* vectorLerp;// векторный интерполятор


	// METHODS
	// инициализирующий конструктор
	public: MyPyramid(void)
	{
		radius = 0.5f;// радиус пирамиды

		// форма...
		Mesh* mesh = new Mesh();
		mesh->CreatePyramid3D(radius);
		mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
		BindMesh(mesh);
		
		
		// цвет...
		SetColor(RGBA(RGBA::GREEN_COLOR)); 


		// столкновения...
		SetCollisionCheckEnable(true);// разрешить участие в столкновениях
		Connect(PolygonalSurface::COLLISION_DETECTED_MESSAGE, this, Caller<>(this, &MyPyramid::CollisionStateIsChanged));


		// движение...
		floatLerp = new FloatLerp();
		floatLerp->Stop();// перевести в начало и остановить
		floatLerp->SetLerpMode(FloatLerp::UNITS_PER_SECOND);// режим интерполяции
		floatLerp->SetPlayingSpeed(0.5f);// скорость движения в единицах/секунду

		vectorLerp = new VectorLerp();
		vectorLerp->BindFloatLerp(floatLerp);
	} 


	// деструктор
	public: virtual ~MyPyramid(void)
	{
		Destroying();

		delete floatLerp;
		delete vectorLerp;
	}


	// реакция на столкновение/выход из столкновения
	public: void CollisionStateIsChanged(bool _enable)
	{
		if(_enable)
		{
			SetColor(RGBA(RGBA::RED_COLOR));
		}
		else
		{
			SetColor(RGBA(RGBA::GREEN_COLOR));
		}
	}


	// возвращает радиус пирамиды
	public: float GetRadius(void)const
	{
		return radius;
	}


	// обновляет состояние объекта
	// примечание: в качестве параметра принимается дельта - время, прошедшее с последнего вызова этой функции
	public: virtual void Update(float _delta = 0.0f)
	{
		SetRotation(GetRotation() * Quaternion(AxisAngle(Vector3::UNIT_Y, _delta)));// вращаем объект вокруг оси Y

		if(floatLerp->IsPaused() && floatLerp->IsFinish())// если достигнута целевая позиция
		{
			SetTarget(GetRandomPosition());// задать новую целевую позицию
		}
		else
		{
			floatLerp->Update(GetDeltaMultiplier() * _delta);// двигать в целевую позицию 

			SetPosition(vectorLerp->GetCurrent());// обновить текущую позицию объекта
		}
	}


	// задает новую целевую позицию для движения
	public: void SetTarget(Vector3 _target)
	{
		vectorLerp->Set(GetPosition(), _target);// задать исходную и целевую контрольные вершины
 
		floatLerp->SetTarget(Line::_GetLength(GetPosition(), _target) / 10.0f);// делаем так, чтобы скорость движения не зависила от дистанции до цели
		
		// начать воспроизведение сначала...
		floatLerp->Stop();
		floatLerp->Play();	 
	}


	// возвращает случайную позицию 
	public: Vector3 GetRandomPosition(void)
	{
		/* [-5, 5] - это размер пространства, в рамках которог может перемещаться объект */ 
		return Vector3(Random::_RandomFloat(-5.0f + GetRadius(), 5.0f - GetRadius()), 
					   GetRadius(), // над уровнем пола
					   Random::_RandomFloat(-5.0f + GetRadius(), 5.0f - GetRadius()));
	}
};



// пользовательское приложение
class Application: public Engine// движок
{
	// DATA
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;// шрифт
	private: Scene *scene, *scene2d;
	private: MyBox* box;// коробка
	private: MyQuad* quad;// квад
	private: MyPyramid* pyramid;// пирамида
	private: bool rightPressed;// если true, то правая кнопка мыши зажата
	private: SyncTimer timer;// таймер, отвечающий за длительность эффекта slow motion


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), box(NIL), rightPressed(false), quad(NIL), pyramid(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete pyramid;
		delete quad;
		delete box;

		delete font;

		delete camera2d;
		delete camera;

		delete scene2d;
		delete scene;
	} 



	// создает приложение
	public: bool Create(Engine::Define _define)
	{
		// создаем движок...
		if(Engine::Create(_define))
		{
			// сцена...
			{
				scene = new Scene();
				scene->Pick();
			}
		

			// камера...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;
				def.rotationEnable = false;
				def.zoomEnable = true;
				def.zoomNear = 10.0f;
				def.zoomFar = 100.0f;// увеличить дистанцию, на которую камера может отдаляться от сцены

				camera = new Camera3D();
				camera->Create(def);

				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
			}


			// двухмерная сцена...
			{
				scene2d = new Scene();

				scene->AddToRenderList(scene2d);
			}
						

			// двухмерная камера...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				def.distanceToViewingPoint = 1.0f;

				camera2d = new Camera3D();
				camera2d->Create(def); 
				
				scene2d->BindCamera(camera2d);
				//camera2d->Pick(); // выбрать камеру
			}

			
			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::WHITE_COLOR).GetRGBAf();// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}


			// таймер
			{
				timer.StartSec(3.0f);// длительность эффекта slow motion
				timer.SetMode(SyncTimer::SINGLE_SHOT);
				timer.Pause();
			}

			// графический объект...
			{
				box = new MyBox();
				box->SetPosition(box->GetRandomPosition());// задать объекту случайную позицию в рамках плоскости quad
				box->SetTarget(box->GetRandomPosition());// задать случайную целевую позицию

				scene->AddToRenderList(box);
				scene->AddToUpdateList(box);
			}

			// графический объект...
			{
				quad = new MyQuad();
				
				scene->AddToRenderList(quad);
			}

			// графический объект...
			{
				pyramid = new MyPyramid();
				
				pyramid->SetPosition(pyramid->GetRandomPosition());// задать объекту случайную позицию в рамках плоскости quad
				pyramid->SetTarget(pyramid->GetRandomPosition());// задать случайную целевую позицию

				scene->AddToRenderList(pyramid);
				scene->AddToUpdateList(pyramid);
			}


			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// цвет фона

			GraphicsLine::_SetLineWidth(3.0f);// толщина линий
			//GraphicsPoint::_SetPointSize(6.0f);// размер точек

			SetWireframeEnable(true);// каркасный режим визуализации

			MoveToCenter();// переместить окно в центр экрана
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}



	// функция обновления
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		PolygonalSurface::_CheckCollision(box, pyramid);// проверить столкновение между двумя объектами

		if(timer.Check(_delta))// если таймер достиг цели
		{
			// восстановить скорость объектов
			box->SetDeltaMultiplier(1.0f);
			pyramid->SetDeltaMultiplier(1.0f);
		}
	}



	// функция визуализации
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);
		
		
		// задать новую трансформацию сцены...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// вывод текста...
		int32 i = 0;

		font->DrawScreenText(Vector3(10, i+=20, -1), "box is intersected with pyramid: %d", box->IsCollisionDetected());
		
		if(!timer.IsPaused() && timer.RemainedTimeInSec() > 0.0f)// пока таймер не достиг целевой отметки
		{
			font->DrawScreenText(Vector3(10, i+=20, -1), "timer. slow motion: %.3f", timer.RemainedTimeInSec());
		}

		font->DrawScreenText(Vector3(10, i+=20, -1), "bounding volume type: %s", box->GetMesh()->GetBoundingVolumeType() == Mesh::BOUNDING_BOX ? "BOUNDING_BOX" : box->GetMesh()->GetBoundingVolumeType() == Mesh::BOUNDING_SPHERE ? "BOUNDING_SPHERE" : "");
		//font->DrawScreenText(Vector3(10, i+=20, -1), "texture-pos: %f/%f", object->texturePosition.x, object->texturePosition.y);


		
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of objects: %d", scene->AmountOfObjects()); 
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of logic objects: %d", scene->AmountOfLogicObjects()); 
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of graphics objects: %d", scene->AmountOfGraphicsObjects()); 
		


		// восстановить предыдущую трансформацию...
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::KEY_DOWN)
		{
			if(_event->key == Mouse::LEFT_BUTTON)
			{}
			else if(_event->key == Mouse::RIGHT_BUTTON)
			{
				// начало вращения камеры...
				if(!rightPressed)// выполняем только один раз
				{
					camera->SetRotationEnable(true); // разрешить вращение камеры
					rightPressed = true;

					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// перемещаем мышь в центр клиентской области окна 
					Mouse::_SetShowCursor(false);
				}
			}
		}
		else if(_event->type == Mouse::KEY_UP)
		{
			if(_event->key == Mouse::RIGHT_BUTTON)
			{
				// конец вращения камеры...
				if(rightPressed)// выполняем только один раз
				{
					camera->SetRotationEnable(false);// запретить вращение камеры 
					rightPressed = false;
					Mouse::_SetShowCursor(true);
				}
			}
		}

		Engine::MouseEventFilter(_event);
		return _event;
	}


	// обработка событий клавиатуры
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		if(_event->type == Keyboard::KEY_DOWN)
		{
			if(_event->key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event->key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_RESIZING);
			}
			else if(_event->key == Keyboard::ESC)
			{
				Destroy();
			}
			else if(_event->key == Keyboard::S)
			{
				// замедлить скорость объектов
				box->SetDeltaMultiplier(0.2f);
				pyramid->SetDeltaMultiplier(0.2f);

				timer.Restart();
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				box->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
				pyramid->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				box->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
				pyramid->GetMesh()->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
			}
		}

		Engine::KeyboardEventFilter(_event);
		return _event;
	}
};



// точка входа в программу
// соглашение о вызове функций WINAPI, указывает, что параметры в функцию WinMain передаются слева направо
int WINAPI WinMain(HINSTANCE hInstance,// дескриптор (идентификатор), генерируемый системой для данного приложения
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// описание движка...
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Collision detection";
	def.rendererDef.zBufferEnable = true;

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
