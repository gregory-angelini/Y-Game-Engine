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
class MyGraphicsObject: public GraphicsSurface, // графическая поверхность
	                    public LogicObject// логический объект
{
	// DATA
	public: bool boundingBoxIntersection;// если true, то селектирующий луч пересекает aabb фигуры
    private: Line result;// результирующий отрезок, пересекающий aabb фигуры
	private: Line selectRay;// селектирующий луч камеры
	private: Vector3 intersectedPoint;// точка пересечения фигуры с селектирующим лучом
	public: bool shapeIntersection;// если true, то произошло столкновение селектирующего луча с гранями фигуры
	private: float curPos;// текущая позиция объекта на круге (0-360)


	// METHODS
	// конструктор без параметров
	public: MyGraphicsObject(void): boundingBoxIntersection(false), shapeIntersection(false), curPos(0.0f)
	{}


	// ОБНОВЛЕНИЕ...
	// обновляет состояние объекта
	// примечание: в качестве параметра принимается дельта - время, прошедшее с последнего вызова этой функции
	public: virtual void Update(float _delta)
	{
		if(!boundingBoxIntersection) 
		{ 
			curPos += _delta * 10.0f;// ускорить передвижение в 10 раз
			
			if(curPos > 360.0f) { curPos = 0.0f; } 

			// перемещать по кругу...
			Vector3 pos = GetPosition();
			float radius = 0.5f;// радиус круга, вдоль которого будет вращаться объект
			float angle = Angle::DOUBLE_PI * (float)curPos / 360.0f;// вычисляем вершину окружности
			
			// ковертируем угол в декартовы координаты и прибавляем к ним координаты центра
			pos.x = cosf(angle) * radius;
			pos.y = sinf(angle) * radius;

			SetPosition(pos);


			// вращать по всем осям...
			SetRotation(GetRotation() * Quaternion(Euler(_delta * 0.2, _delta * 0.2, _delta * 0.2))); 
		}
	}


	// фильтр событий мыши
	public: Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::KEY_DOWN)// если нажата клавиша
		{
			if(_event->key == Mouse::LEFT_BUTTON)// левый клик
			{
				selectRay = Camera3D::_GetPicked()->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked()));// вычислить селектирующий луч камеры
				
				boundingBoxIntersection = GetTransformedAABB().IsIntersected(selectRay, &result);// вычислить отрезок, пересекающий прямоугольник 

				if(boundingBoxIntersection) 
				{ 
					if(IsIntersected(selectRay, intersectedPoint) != -1)
					{
						shapeIntersection = true;
					}
					else 
					{ 
						shapeIntersection = false; 
					}
				}
				else { shapeIntersection = false; } 
			}
		}
		return _event;
	}


	// ВИЗУАЛИЗАЦИЯ...
	// отображает полигональную сетку на экран 
	// примечание: перед визуализацией aabb масштабируется, поэтому красный отрезок немного не совпадает с пересекаемой областью
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


		GraphicsSurface::Draw();
	
		if(boundingBoxIntersection)// если селектирующий луч пересекает ограничивающий объем пирамиды
		{
			// отобразить селектирующий луч камеры
			GraphicsLine::_Draw(selectRay.origin, result.origin, RGBA(RGBA::YELLOW_COLOR));

			// отобразить часть селектирующего луча, проходящего через aabb пирамиды
			GraphicsLine::_Draw(result.origin, result.end, RGBA(RGBA::RED_COLOR));

			// отобразить точку селектирующего луча пересечения с пирамидой
			GraphicsLine::_Draw(selectRay.end, result.end, RGBA(RGBA::YELLOW_COLOR));

			if(shapeIntersection) 
			{ 
				GraphicsPoint::_Draw(intersectedPoint, RGBA(RGBA::RED_COLOR)); 
			} 
		}
		else// иначе нет пересечения
		{
			// отобразить селектирующий луч камеры
			GraphicsLine::_Draw(selectRay.origin, selectRay.end, RGBA(RGBA::YELLOW_COLOR));
		}
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
	private: MyGraphicsObject* object;// текстурированная полигональная сетка
	private: bool rightPressed;// если true, то правая кнопка мыши зажата



    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), object(NIL), rightPressed(false)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
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
				scene2d->BindCamera(camera2d);

				scene->AddToRenderList(scene2d);
			}
						

			// двухмерная камера...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				camera2d = new Camera3D();
				camera2d->Create(def);

				scene2d->BindCamera(camera2d);
			}

			
			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::WHITE_COLOR);// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}


			// тестируемый объект...
			{
				object = new MyGraphicsObject();
				
				Mesh* mesh = new Mesh();
				//mesh->CreateQuad2D(Quad(Vector3(), Vector2(0.5, 0.5)));
				mesh->CreatePyramid3D(0.25f/2.0f);
				object->BindMesh(mesh);

				object->SetColor(RGBA(RGBA::WHITE_COLOR));
				//object->SetBBColor(RGBA(RGBA::BLACK_COLOR));
				object->SetBackFaceCullingEnable(false);// отключить отсечение задних граней
				object->SetBoundingVolumeShowEnable(true);// включить отображение ограничивающего объема

				scene->AddToRenderList(object);
				scene->AddToUpdateList(object);
			}

		
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// цвет фона

			GraphicsLine::_SetLineWidth(2.0f);// толщина линий
			GraphicsPoint::_SetPointSize(6.0f);// размер точек

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

		font->DrawScreenText(Vector3(10, i+=20, -1), "bounding volume show enable: %d", object->IsBoundingVolumeShowEnabled());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "bounding volume show mode: %s", GetDrawModeString(object->GetBoundingVolumeShowMode()).c_str());

		
		font->DrawScreenText(Vector3(10, i+=20, -1), "aabb intersection: %d", object->boundingBoxIntersection); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "shape intersection: %d", object->shapeIntersection); 
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

		Engine::MouseEventFilter(_event);

		object->MouseEventFilter(_event);

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
			else if(_event->key == Keyboard::E)
			{
				object->SetBoundingVolumeShowEnable(!object->IsBoundingVolumeShowEnabled());
			}
			else if(_event->key == Keyboard::UP_ARROW)
			{
				object->SetScale(object->GetScale() * 1.05);
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				object->SetScale(object->GetScale() * 0.95);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "AABox.example_01";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
