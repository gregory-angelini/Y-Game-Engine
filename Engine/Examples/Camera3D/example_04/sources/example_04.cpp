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
#include <Engine/Output/Graphics/Shape/Mesh/ColoredMesh/ColoredMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Shape/Mesh/TexturedMesh/TexturedMesh.h>
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




// графическая икосфера
class Icosphere: public ColoredMesh// цветная полигональная сетка
{
	// DATA
	public: Triangle triangle;// текущий треугольник

	public: bool drawOnlyCenterOfTriangles;// если true, то отображать только центр треугольников
	public: RGBA normalColor;// цвет нормалей
	public: bool drawNormalOfTriangles;// если true, то отображать нормали треугольников
	
    private: Vector3 centerOfPolygon;// центр полигона
	private: RGBA polygonCenterColor;// цвет центра треугольников

	private: int32 intersectedTriangleIndex;// индекс пересеченного треугольника
    private: Vector3 intersectionPoint;// точка пересечения луча и полигональной сетки; актуально только при intersected = true
    private: RGBA intersectedColor;// цвет пересеченного треугольника


	// METHODS
	// конструктор по умолчанию
	public: Icosphere(void): drawOnlyCenterOfTriangles(false), drawNormalOfTriangles(false), intersectedTriangleIndex(-1)
	{
		CreateIcosphere3D(3);

		//SetWireframeEnable(true);// каркасный режим визуализации
	
		SetColor(RGBA(RGBA::YELLOW_COLOR));
		polygonCenterColor = RGBA(RGBA::RED_COLOR);
		normalColor = RGBA(RGBA::RED_COLOR);
		intersectedColor = RGBA(RGBA::YELLOW_COLOR);
	}


	// возвращает точку пересечения луча и полигональной сетки
	public: Vector3 GetIntersectionPoint(void)
	{
		return intersectionPoint;
	}


	// проверяет столкновение полигонов сферы с отрезком _line и возвращает true в случае успеха
	public: bool CheckIntersect(Line _line)
	{
		return ((intersectedTriangleIndex = IsIntersected(_line, intersectionPoint)) >= 0);// проверить пересечение между всеми полигонами сферы и отрезком _line
	}

	
	// отображение икосферы по треугольникам
	public: void virtual Draw(void)
	{
		int32 polygons = GetAmountOfPolygons();// количество полигонов в сфере

		// визуализировать все полигоны
		for(int i = 0; i < polygons; ++i)
		{
			if(GetTriangle(i, triangle))// взять треугольник под индексом i
			{
				centerOfPolygon = triangle.GetCenter();// взять центр полигона

				if(drawOnlyCenterOfTriangles)// если нужно отображать только центр треугольников
				{
					// отобразить центр треугольника
					GraphicsPoint::_Draw(centerOfPolygon, polygonCenterColor);
				}			 	
				else if(drawNormalOfTriangles)// если нужно отображать нормали треугольников
				{
					Renderer::_GetPicked()->SetWireframeEnable(false);

					_DrawCulledTriangle(triangle, GetColor());// отобразить треугольник i

					// отобразить нормали
					GraphicsLine::_Draw(centerOfPolygon, 
										centerOfPolygon + triangle.GetNormal() * 0.1,
										normalColor);
					
					Renderer::_GetPicked()->SetWireframeEnable(true);
				}
				else// иначе просто отображаем треугольники
				{
					if(intersectedTriangleIndex >= 0)// если есть пересечение луча и треугольника
					{
						if(i == intersectedTriangleIndex)// если i - это пересеченный треугольник
						{
							Renderer::_GetPicked()->SetWireframeEnable(false);

							_DrawCulledTriangle(triangle, intersectedColor);// отобразить треугольник i

							Renderer::_GetPicked()->SetWireframeEnable(true);
						}
						else// иначе i не пересекается с лучом
						{
							_DrawCulledTriangle(triangle, GetColor());// отобразить треугольник i
						}
					}
					else// иначе нет пересечений
					{
						_DrawCulledTriangle(triangle, GetColor());// отобразить треугольник i
					}
				}
			}
		}
	} 
};



// класс реализующий точку в трехмерном пространстве
class Point3D: public Point// трехмерная позиция
{
	// DATA
	public: StringANSI name;// имя точки

	// METHODS
	// конструктор по умолчанию
	public: Point3D(void)
	{}

	// инициализирующий конструктор 
	public: Point3D(Vector3 _point, StringANSI _name): name(_name)
	{
		SetPosition(_point);
	}
};


// пользовательское приложение
class Application: public Engine// движок
{
	// DATA
	private: Camera3D* camera, *camera2d;// камера
    private: Font2D *font;// шрифт
	private: Scene *scene;// сцена
	private: Icosphere* sphere;// сфера

	private: float sightSize;// размер прицела (длина каждой линии в пикселях)
	private: Line selectLine;// селектирующий луч
	private: SimpleList<Point3D> points;// список с вершинами
	private: bool rightPressed;// если true, то разрешено вращение камеры


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), camera2d(NIL), sphere(NIL), sightSize(20.0f), rightPressed(false)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete font;
		delete scene;
		delete camera2d;
		delete camera;
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
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				//def.rotationEnable = true;
				def.zoomEnable = true;
				def.nearZoom = 3.0f;
				def.farZoom = 7.0f;
				def.SetMiddleDistanceToViewingPoint();

				camera = new Camera3D();
				camera->Set(def);

				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
			}


			// сфера...
			{
				sphere = new Icosphere();
		
				scene->AddObject(sphere);
				scene->AddGraphicsObject(sphere);
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

			

			// двухмерная камера...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				camera2d = new Camera3D();
				camera2d->Set(def);

				//scene2d->BindCamera(camera2d);
			}

			SetWireframeEnable(true);// задать каркасный режим отображения

			GraphicsPoint::_SetPointSize(6);// размер точек 
			Mouse::_MoveCursorToCenter(this);// переместить курсор в центр клиентской области окна
			Mouse::_SetShowCursor(false);// скрыть курсор
			
			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));

			MoveToCenter();// переместить окно в центр экрана
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}


			
	// отображает на экран именованные точки
	public: void DrawPoints(void)
	{
		camera->BeginTransform(Renderer::PROJECTION);
		camera->BeginTransform(Renderer::MODELVIEW);

		// обойти все точки...
		for(int32 i = 0; i < points.GetSize(); ++i)
		{
			Vector3 point = points[i]->key.GetPosition();

			// отобразить зеленую точку
			GraphicsPoint::_Draw(point, RGBA(RGBA::GREEN_COLOR));

			Vector2 screenPoint;
			
			camera->Project(point, screenPoint);// вычислить экранные координаты точки point
			{
				// вывод текста...
				// задать новую трансформацию сцены...
				camera2d->BeginTransform(Renderer::PROJECTION);
				camera2d->BeginTransform(Renderer::MODELVIEW);

				//camera->BeginScreen2D();// начать вывод двухмерной графики
				
				font->DrawScreenText(Vector3(screenPoint.x,  ClientAreaSize().y - screenPoint.y, -1), "%s", points[i]->key.name.c_str());
				
				//camera->BeginPerspective3D();
				// восстановить предыдущую трансформацию...
				camera2d->EndTransform(Renderer::MODELVIEW);
				camera2d->EndTransform(Renderer::PROJECTION);
			}
		}
		camera->EndTransform(Renderer::MODELVIEW);
		camera->EndTransform(Renderer::PROJECTION);
	}


	// отобразить прицел на месте курсора
	public: void DrawSight(void)
	{
		// начать вывод двухмерной графики
		//camera->BeginScreen2D();
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);
  
		Renderer::_SetCurrentStack(Renderer::MODELVIEW);
		Renderer::_PushMatrix();
		Renderer::_LoadIdentityMatrix(); 
		Renderer::_LoadMatrix(Matrix4x4::_Translation(Vector3(0.0f, 0.0f, -1.0f)));// где '-1' - расстояние по оси Z от точки наблюдения вглубь экрана

		GraphicsLine::_Draw(Vector3(Mouse::_GetCursorPosition(this).x - sightSize/2.0f, Mouse::_GetCursorPosition(this).y, 0), 
			                Vector3(Mouse::_GetCursorPosition(this).x + sightSize/2.0f,  Mouse::_GetCursorPosition(this).y, 0), RGBA(RGBA::GREEN_COLOR));// рисуем горизонтальную линию слева направо
		GraphicsLine::_Draw(Vector3(Mouse::_GetCursorPosition(this).x, Mouse::_GetCursorPosition(this).y - sightSize/2.0f, 0), 
			                Vector3(Mouse::_GetCursorPosition(this).x, Mouse::_GetCursorPosition(this).y + sightSize/2.0f, 0), RGBA(RGBA::GREEN_COLOR));// рисуем вертикальную линию снизу вверх
		Renderer::_PopMatrix();

		// начать вывод трехмерной графики
		//camera->BeginPerspective3D();
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
	}


	// отобразить селектирующий луч
    public: void DrawSelectRay(void)
	{	
		camera->BeginTransform(Renderer::PROJECTION);
		camera->BeginTransform(Renderer::MODELVIEW);

		// отобразить селектирующий луч
		GraphicsLine::_SetLineWidth(3);
		GraphicsLine::_Draw(selectLine.begin, selectLine.end, RGBA(RGBA::GREEN_COLOR));
		GraphicsLine::_SetLineWidth(1); 

		camera->EndTransform(Renderer::MODELVIEW);
		camera->EndTransform(Renderer::PROJECTION);
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
		
		DrawPoints();// отобразить точки

		DrawSelectRay();// отобразить селектирующий луч

				
		if(!rightPressed) { DrawSight(); }// вывести прицел камеры


		// задать новую трансформацию сцены...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// вывод текста...
		int32 i = 0;

		//font->DrawScreenText(Vector3(10, i+=20, -0.1), "--: %d", 9);

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "DistanceToViewingPoint: %.2f", camera->DistanceToViewingPoint());
	    
		
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sphere.AmountOfPolygons: %d", sphere->AmountOfPolygons());
	   
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera.position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	    
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "select ray.begin(%.2f/%.2f/%.2f)", selectLine.begin.x, selectLine.begin.y, selectLine.begin.z);
	   // font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "select ray.end(%.2f/%.2f/%.2f)", selectLine.end.x, selectLine.end.y, selectLine.end.z);


		// восстановить предыдущую трансформацию...
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
	}




	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::KEY_DOWN)// если нажата клавиша
		{
			if(_event->key == Mouse::LEFT_BUTTON)// если левый клик мыши
			{
				//Renderer::_SetCurrentStack(Renderer::PROJECTION);

				selectLine = camera->GetSelectRay(Mouse::_GetCursorPosition(this));// вычислить селектирующий луч

				if(sphere->CheckIntersect(selectLine))// проверить столкновение луча с полигональной сеткой
				{
					// ограничить длину луча из начальной точки в сторону конечной точки таким образом, чтобы получился отрезок между точкой пересечения и началом луча
					selectLine.SetLengthFromBeginToEnd(Line::_GetLength(selectLine.begin, sphere->GetIntersectionPoint()));
 
					points.AddToTail(Point3D(sphere->GetIntersectionPoint(), "point" + NumberToString(points.GetSize() + 1)));
				}
			}
		}
		else if(_event->type == Mouse::KEY_PRESSED)// если кнопка зажата
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// правый клик
			{
				// начало вращения камеры...
				if(!rightPressed)// выполняем только один раз
				{
					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// перемещаем мышь в центр клиентской области окна 

					camera->SetRotationEnable(true);// разрешить вращение камеры
					rightPressed = true;
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
					camera->SetRotationEnable(false); // запретить вращение камеры
					rightPressed = false;
				}
			}
		}

		Engine::MouseEventFilter(_event);
		return _event;
	}


	// обработка событий клавиатуры
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		Engine::KeyboardEventFilter(_event);

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
				Destroy();// уничтожить окно
			} 
			else if(_event->key == Keyboard::N)
			{
				sphere->drawNormalOfTriangles = true;// отображать нормали
			}
			else if(_event->key == Keyboard::C)
			{
				sphere->drawOnlyCenterOfTriangles = true;// отображать центр треугольников
			}
		} 
		if(_event->type == Keyboard::KEY_UP)
		{
			if(_event->key == Keyboard::N)
			{
				sphere->drawNormalOfTriangles = false;
			}
			else if(_event->key == Keyboard::C)
			{
				sphere->drawOnlyCenterOfTriangles = false;
			}
		}

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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Camera3D.example_04";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
