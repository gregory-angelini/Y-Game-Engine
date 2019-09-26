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



// класс, реализующий пользовательскую графическую полигональную сетку
class TexturedObject: public TexturingSurface, // текстурируемая поверхность
	                  public LogicObject// логический объект
{
	// DATA
	private: Line selectRay;// селектирующий луч камеры
	private: Vector2 pixelPosition;// координаты пикселя, на который указывает селектирующий луч
	private: float curPos;// текущая позиция объекта на круге (0-360)
	private: bool intersection;// если true, то произошло пересечение селектирующего луча с полигональной сеткой
	private: RGBA pixelColor;// цвет пикселя, на который указывает селектирующий луч
	private: bool pauseEnable;// если true, то нужно остановить перемещение и вращение объекта


	// METHODS
	// конструктор без параметров
	public: TexturedObject(void): curPos(0.0f), intersection(false), pauseEnable(false)
	{}


	// возвращает true если задана пауза
	public: bool IsPauseEnabled(void)const
	{
		return pauseEnable;
	}


	// включает/выключает паузу
	public: void SetPauseEnable(bool _enable)
	{
		pauseEnable = _enable;
	}


	// возвращает цвет пикселя, на который указывает селектирующий луч
	public: RGBA GetIntersectedPixelColor(void)const
	{
		return pixelColor;
	}

	// возвращает позицию пикселя, на который указывает селектирующий луч
	public: Vector2 GetIntersectedPixelPosition(void)const
	{
		return pixelPosition;
	}


	// возвращает true если селектирующий луч пересекает полигональную сетку
	public: bool IsQuadIntersected(void)const
	{
		return intersection;
	}


	// ОБНОВЛЕНИЕ...
	// обновляет состояние объекта
	// примечание: в качестве параметра принимается дельта - время, прошедшее с последнего вызова этой функции
	public: virtual void Update(float _delta)
	{
		if(!pauseEnable)
		{ 
			curPos += _delta * 10.0f;// ускорить передвижение в 10 раз
			
			if(curPos > 360.0f) { curPos = 0.0f; } 

			// перемещать по кругу...
			float radius = 0.5f;// радиус круга, вдоль которого будет вращаться объект
			float angle = Angle::DOUBLE_PI * (float)curPos / 360.0f;// вычисляем вершину окружности
			
			// ковертируем угол в декартовы координаты
			SetPosition(Vector3(cosf(angle) * radius, sinf(angle) * radius, 0));

			// вращать по всем осям...
			SetRotation(GetRotation() * Quaternion(Euler(0, _delta * 0.2, 0))); 
		}
	}


	// фильтр событий мыши
	public: Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::MOVE)// если курсор переместился
		{
			selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));// вычислить селектирующий луч камеры
			intersection = IsIntersected(selectRay, &pixelPosition, &pixelColor);
		} 

		return _event;
	}


	// ВИЗУАЛИЗАЦИЯ...
	// отображает полигональную сетку на экран 
    // примечание: BeginTransform и EndTransform будут вызованы автоматически
	public: virtual void Draw(void)
	{
		GraphicsPoint::_SetPointSize(2.0f);// размер точек

		Vector3 vertex;// вершина
		float radius = 0.5f;// радиус круга, вдоль которого будет вращаться объект

		// вычислить остальные вершины окружности...
		for(int32 i = 1; i < (360/2) + 1; ++i)
		{ 
			float angle = Angle::DOUBLE_PI * (float)i / (360/2);// вычисляем вершину окружности
				
			// ковертируем угол в декартовы координаты и прибавляем к ним координаты центра
			vertex.x = cosf(angle) * radius;
			vertex.y = sinf(angle) * radius;

			GraphicsPoint::_Draw(vertex, RGBA(RGBA::RED_COLOR));
		}
		GraphicsPoint::_SetPointSize(6.0f);// размер точек

		/*
		// отобразить локальную точку пересечения курсора и квада...
		selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));// вычислить селектирующий луч камеры

		Vector3 intersectedPoint;// точка пересечения

		if(IsIntersected(selectRay, intersectedPoint) != -1)// если произошло пересечение отрезка и полигональной сетки
		{
			// позиция квада
			//GraphicsPoint::_Draw(GetPosition(), RGBA(RGBA::GREEN_COLOR));
	
			// точка пересечения
			//GraphicsPoint::_Draw(intersectedPoint, RGBA(219, 108, 47, 255));// оранжевый цвет

			// учитываем поворот
			intersectedPoint = (Quaternion::_GetConjugate(GetRotation())).RotatePointAroundPivot(intersectedPoint, GetPosition());

			// учитываем перенос
			Vector3 localPosition = intersectedPoint - GetPosition();// найти локальную точку пересечения, в рамках полигональной сетки

			// учитываем масштабирование
			localPosition.x *= 1.0f / GetScale().x;
			localPosition.y *= 1.0f / GetScale().y;
			localPosition.z *= 1.0f / GetScale().z;

			// локальная точка пересечения
			BeginTransform(); 
			GraphicsPoint::_Draw(Vector3(localPosition.x, localPosition.y, 0.01f), RGBA(RGBA::YELLOW_COLOR));
			EndTransform(); 
		}
		*/ 

		TexturingSurface::Draw();
	}


	// сохраняет объект в конец файла _file, и возвращает true в случае успеха
	// примечание: если файл не открыт на запись, то возвращается false
	public: virtual bool SaveToFile(File& _file)
	{
		return false;
	}


	// загружает объект из файла _file, и возвращает true в случае успеха
	// примечание: если файл не существует или не открыт на чтение, то вернется false
	protected: virtual bool LoadFromFile(File& _file)
	{
		return false;
	}
};

// пользовательское приложение
class Application: public Engine// движок
{
	// DATA
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;// шрифт
	private: Scene *scene, *scene2d;
	private: TexturedObject* object;// текстурируемый объект
	private: bool rightPressed;// если true, то правая кнопка мыши зажата
	private: Texture2D* texture1, *texture2;
	private: TexturedAnimation* anim1, *anim2;



    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), object(NIL), rightPressed(false), texture1(NIL), anim1(NIL), texture2(NIL), anim2(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete texture1;
		delete texture2;
		delete anim1;
		delete anim2;
		delete object;

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
				//def.rotationEnable = true;
				//def.farZoom = 11;
				def.zoomNear = 1.1f;
				def.zoomEnable = true;

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

			// текстура...
			{
				texture1 = new Texture2D();
				texture1->Create("restartButtonTexture1.png");
			}

			// текстура...
			{
				texture2 = new Texture2D();
				texture2->Create("restartButtonTexture2.png");
			}

			// анимация...
			{
				anim1 = new TexturedAnimation();

				Atlas2D* atlas = new Atlas2D();
				atlas->LoadImagesFromFolder("restartButtonAnim1");
				anim1->BindAtlas(atlas);

				anim1->SetFPS(1.0f);
				anim1->Play();
				anim1->SetPlayingMode(PlayingObject::LOOP_PLAYING);
				scene->AddToUpdateList(anim1);
			}

					
			// анимация...
			{
				anim2 = new TexturedAnimation();

				Atlas2D* atlas = new Atlas2D();
				atlas->LoadImagesFromFolder("restartButtonAnim2");
				anim2->BindAtlas(atlas);

				anim2->SetFPS(1.0f);
				anim2->Play();
				anim2->SetPlayingMode(PlayingObject::LOOP_PLAYING);
				scene->AddToUpdateList(anim2);
			}


			// текстурируемый объект объект...
			{
				object = new TexturedObject();
				
				Mesh* mesh = new Mesh();
				mesh->CreateQuad2D(Quad(Vector3(), Vector2(0.5f, 0.5f)));
				object->BindMesh(mesh);
				
		
				object->BindFrontTexture(texture1);
				object->BindBackTexture(texture2);

				//object->BindFrontTexturedAnimation(anim1);
				//object->BindBackTexturedAnimation(anim2);



				//object->CreatePyramid3D(0.25f/2.0f);

				//object->SetPosition(Vector3(400, 300, -1));

				//object->SetColor(RGBA(RGBA::WHITE_COLOR));
				//object->SetBBColor(RGBA(RGBA::BLACK_COLOR));
				
					
				object->SetIntersectionMode(AbstractSurface::CHECK_ONLY_FRONT_FACE, false);// отключить проверку только фронтальных граней, добавив к ним и задние
				
				object->SetBackFaceCullingEnable(false);// отключить отсечение задних граней
				object->SetBoundingVolumeShowEnable(true);//включить отображение ограничивающего объема

				scene->AddToRenderList(object);
				scene->AddToUpdateList(object);
			}

		
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// цвет фона

			//GraphicsLine::_SetLineWidth(2.0f);// толщина линий
			GraphicsPoint::_SetPointSize(6.0f);// размер точек

			//SetWireframeEnable(true);// каркасный режим визуализации

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

		font->DrawScreenText(Vector3(10, i+=20, -1), "quad intersection: %d", object->IsQuadIntersected());
		font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel position: %d/%d", (int32)object->GetIntersectedPixelPosition().x, (int32)object->GetIntersectedPixelPosition().y);
		font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel color: %d/%d/%d/%d", object->GetIntersectedPixelColor().red, object->GetIntersectedPixelColor().green, object->GetIntersectedPixelColor().blue, object->GetIntersectedPixelColor().alpha);
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
		if(_event->type == Mouse::KEY_DOWN)// если нажата клавиша
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// правый клик
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
		else if(_event->type == Mouse::KEY_UP)// если кнопка отпущена
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// если правый клик
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

		object->MouseEventFilter(_event);

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
			else if(_event->key == Keyboard::UP_ARROW)
			{
				object->SetScale(object->GetScale() * 1.05);
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				object->SetScale(object->GetScale() * 0.95);
			}
			else if(_event->key == Keyboard::SPACE)
			{
				object->SetPauseEnable(!object->IsPauseEnabled());
			}
			else if(_event->key == Keyboard::T)
			{
				object->BindFrontTexture(texture1);
				object->BindBackTexture(texture2);
			}
			else if(_event->key == Keyboard::A)
			{
				object->BindFrontTexturedAnimation(anim1);
				object->BindBackTexturedAnimation(anim2);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "TexturingSurface.example_01";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
