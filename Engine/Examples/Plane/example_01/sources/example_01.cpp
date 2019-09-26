// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// для текстур

#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Shape/Box/GraphicsBox3DFixed/GraphicsBox3DFixed.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Input/Controls/Controls.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Math/Plane/Plane.h>


// графическая плоскость
class GraphicsPlane: public GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D, Matrix4x4>,// графический мэш
	                 public Plane// плоскость
{
	// DATA
	private: GraphicsLine<Vector3D, ColorRGBA> graphicsNormal;// графическая нормаль
    private: Vector3D intersectionPoint;// точка пересечения луча и полигональной сетки; актуально только при intersected = true
	private: bool intersection;// если true, значит есть точка пересечения с плоскостью

	// METHODS
	// конструктор по умолчанию
	public: GraphicsPlane(void): intersection(false)
	{
		// плоскость...
		normal = Vector3D::UNIT_X;// направление нормали

		// нормаль плоскости...
		graphicsNormal.SetColor(ColorRGBA::YELLOW_COLOR); 
				

		// квад... 
		CreateQuad2D(Quad<Vector3D>(Vector3D(0,0,0), Size2D(1.0f, 1.0f)));// создать квад

		SetPosition(Vector3D(0, 0, -2));// задать позицию плоскости
		SetRotation(Quaternion::_ShortestArc(p0, normal * p0.Length()));// задать ориентацию плоскости в направлении normal

		ResizeColors();
		SetColor(ColorRGBA::GREEN_COLOR);// цвет плоскости
	}

	// задание новых координат для плоскости
	public: virtual void SetPosition(Vector3D _position)
	{
		p0 = _position;
		ModelView<Vector3D, Quaternion, Size3D, Matrix4x4>::SetPosition(p0);
		graphicsNormal.Set(p0, p0 + normal * 0.19);
	}

	// задание новой ориентации для плоскости
	public: virtual void SetRotation(Quaternion _rotation)
	{
		ModelView<Vector3D, Quaternion, Size3D, Matrix4x4>::SetRotation(_rotation);

		normal = -_rotation.AxisZ();// обновить нормаль (ориентацию) плоскости
		graphicsNormal.Set(p0, p0 + normal * 0.19);
	}



	// визуализация плоскости
	public: virtual void Draw(void)
	{
		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D, Matrix4x4>::Draw();
		graphicsNormal.Draw();// визуализировать нормаль
		
		// вывод точки пересечения...
		if(intersection)
		{
			GraphicsPoint<Vector3D, ColorRGBA>::_Draw(intersectionPoint, ColorRGBA::RED_COLOR);
		}
	}

	// возвращает точку пересечения луча и полигональной сетки
	public: Vector3D GetIntersectionPoint(void)
	{
		return intersectionPoint;
	}

	// проверить столкновение полигонов сферы с отрезком _line
	public: bool CheckIntersect(Line<Vector3D> _line)
	{
		return (intersection = Plane::IsIntersected(_line, intersectionPoint));// проверить пересечение отрезка _line с плоскостью this
	}
};



// класс, реализующий пользовательское окно
class Application: public Renderer
{
	public: enum
	{
		BOX, // прямоугольник
		QUAD,// квад (плоскость)
		ICOSPHERE, // икосфера
		PYRAMID,// пирамида
		CUSTOM// пользовательская фигура
	};

	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene<Vector3D, Quaternion, Size3D, Matrix4x4>* scene;// графическая сцена
	private: GraphicsPlane* plane;//графическая плоскость
	private: bool rightReleased, rightPressed;// сенсоры, регистрирующие событие "отжатия" и "зажатия" правой кнопки мыши
	private: Line<Vector3D> selectLine;// селектирующий луч
	private: GraphicsPoint<Vector3D, ColorRGBA> *p1, *p2, *p3;// тестовые вершины
	private: float sightSize;// размер прицела (длина каждой линии в пикселях)


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), plane(NIL), p1(NIL), p2(NIL), p3(NIL), sightSize(20.0f), rightReleased(false), rightPressed(false)
	{}

			
	// виртуальный деструктор
	public: virtual ~Application(void)
	{
		delete scene;
	} 

	// создает приложение
	public: bool Create(Renderer::Define _define)
	{
		// создать графическое ядро
		if(Renderer::Create(_define))
		{
			// сцена...
			{
				GraphicsScene<Vector3D, Quaternion, Size3D, Matrix4x4>::Define def;
				def.logicSceneDef.scale = Size3D(1.0f, 1.0f, 1.0f);
				scene = new GraphicsScene<Vector3D, Quaternion, Size3D, Matrix4x4>();
				scene->Create(def);
			}

	 

			// графическая плоскость...
			{
				plane = new GraphicsPlane;

				plane->Rename("plane");
				plane->SetScale(Size3D(0.5f, 0.5f, 0.5f));
				
				scene->AddObject(plane, false, true);// добавить в сцену			
			}
			

			// точки...
			{
				p1 = new GraphicsPoint<Vector3D, ColorRGBA>();
				p1->Rename("p1");
				p1->SetPosition(plane->p0);
				p1->SetColor(ColorRGBA::MAGENTA_COLOR);
				scene->AddObject(p1, false, true);// добавить в сцену		
				 
			
				p2 = new GraphicsPoint<Vector3D, ColorRGBA>();
				p2->Rename("p2");
				p2->SetPosition(plane->p0 + plane->normal * 0.5);
				p2->SetColor(ColorRGBA::GREEN_COLOR);
				scene->AddObject(p2, false, true);// добавить в сцену		

								
				p3 = new GraphicsPoint<Vector3D, ColorRGBA>();
				p3->Rename("p3");
				p3->SetPosition(plane->p0 + -plane->normal * 0.5);
				p3->SetColor(ColorRGBA::BLUE_COLOR);
				scene->AddObject(p3, false, true);// добавить в сцену		
			}

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.textColor = ColorRGBA::WHITE_COLOR;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = ColorRGBA::GRAY_COLOR;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}


			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.controllerType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
				def.viewingPoint = plane->GetPosition();
				def.distanceToViewingPoint = 4.0f;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
				camera->ConnectTo(Mouse::Event::TYPE, this);
				
				// задать изометрический ракурс камеры
				//camera->UpdateTrackBallObjectViewer(-Angle::HALF_PI/3.0f, -Angle::HALF_PI/3.0f, 0.0f);
			}


			SetFaceCullMode(Renderer::BACK, false);
			SetBackgroundColor(ColorRGBA::WHITE_COLOR);// белый цвет фона
			
			GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// толщина линий
			GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(8.0f);// размер точек


			MoveToCenter();// переместить окно в центр крана
			Mouse::_MoveCursorToClientAreaCenter(this);// переместить курсор в центр клиентской области окна
			
			Mouse::_ShowCursor(false);// скрыть курсор

			Flush();//выполнить все ранее отложенные события
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}
	


	// отобразить прицел на месте курсора
	public: void DrawSight(void)
	{
		// начать вывод двухмерной графики
		camera->BeginScreen2D();
  
		Renderer::_SetCurrentStack(Renderer::MODELVIEW);
		Renderer::_PushMatrix();
		Renderer::_LoadIdentityMatrix(); 
		Renderer::_Translate(Vector3D(0.0f, 0.0f, -1.0f));// где '-1' - расстояние по оси Z от точки наблюдения вглубь экрана
 

		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(Mouse::_CursorClientAreaPosition(this).x - sightSize/2.0f, Mouse::_CursorClientAreaPosition(this).y), 
			                                     Vector2D(Mouse::_CursorClientAreaPosition(this).x + sightSize/2.0f,  Mouse::_CursorClientAreaPosition(this).y), ColorRGBA::RED_COLOR);// рисуем горизонтальную линию слева направо
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(Mouse::_CursorClientAreaPosition(this).x, Mouse::_CursorClientAreaPosition(this).y - sightSize/2.0f), 
			                                     Vector2D(Mouse::_CursorClientAreaPosition(this).x, Mouse::_CursorClientAreaPosition(this).y + sightSize/2.0f), ColorRGBA::RED_COLOR);// рисуем вертикальную линию снизу вверх


		Renderer::_PopMatrix();

		// начать вывод трехмерной графики
		camera->End2D();
	}



	// отобразить селектирующий луч
    public: void DrawSelectRay(void)
	{	
		// отобразить селектирующий луч
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(selectLine.begin, selectLine.end, ColorRGBA::RED_COLOR);
	}


	// функция кадра
	public: virtual void Update(float _delta)
	{
		// разрешить вращение камеры только при зажатом правом клике
		if(Mouse::_IsKeyPressed(Mouse::RIGHT_BUTTON))
		{
			if(!rightPressed)// выполняем только один раз
			{
				camera->SetControllerEnable(true);
				rightPressed = true;
				rightReleased = false;
			}
		}
		else 
		{ 
			if(!rightReleased)// выполняем только один раз
			{
				camera->SetControllerEnable(false); 
				rightReleased = true;
				rightPressed = false;
			}
		}
	}


	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену

		if(rightReleased) { DrawSight(); }// вывести прицел камеры


		scene->Draw();


		
		DrawSelectRay();// отобразить селектирующий луч


		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики

		int32 i = 0;

	
		font2d->SetTextColor(ColorRGBA::MAGENTA_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "distance to p1: %.2f", plane->DistanceTo(p1->GetPosition())); 
		font2d->SetTextColor(ColorRGBA::GREEN_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "distance to p2: %.2f", plane->DistanceTo(p2->GetPosition())); 
		font2d->SetTextColor(ColorRGBA::BLUE_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "distance to p3: %.2f", plane->DistanceTo(p3->GetPosition())); 
		


		font2d->SetTextColor(ColorRGBA::BLACK_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "plane.normal(x/y/z): %.2f/%.2f/%.2f", plane->normal.x, plane->normal.y, plane->normal.z); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "plane.p0(x/y/z): %.2f/%.2f/%.2f", plane->p0.x, plane->p0.y, plane->p0.z); 


		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeinByte: %d", mesh->SizeInByte()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "sizeInKbyte: %.5f", MemoryCounter::_FromByteToKB(mesh->SizeInByte())); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "detail level: %d", level); 
	
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "current shape: %s", NameOfShape(curShape).c_str()); 
	



		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
				selectLine = camera->GetSelectRay(Mouse::_CursorClientAreaPosition(this));// вычислить селектирующий луч

				plane->CheckIntersect(selectLine);// проверить столкновение луча с плоскостью

				// ограничить длину луча из начальной точки в сторону конечной точки таким образом, чтобы получился отрезок между точкой пересечения и началом луча
				selectLine.SetLengthFromBeginPoint(Line<Vector3D>::_GetLength(selectLine.begin, plane->GetIntersectionPoint()));
			}
			else if(_event.key == Mouse::RIGHT_BUTTON)
			{
				Mouse::_MoveCursorToClientAreaCenter(Renderer::_Picked());// перемещаем мышь в центр клиентской области окна 
			}
		}
		Mouse().ClearEventList();// очистить буфер событий
	}


	// обработка событий клавиатуры
	public: virtual void KeyboardEventFilter(Keyboard::Event _event)
	{
		if(_event.type == Keyboard::KEY_DOWN)
		{
			if(_event.key == Keyboard::NUM_1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::NUM_2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			} 
			else if(_event.key == Keyboard::X)
			{
				plane->SetRotation(plane->GetRotation() * Quaternion(Vector3D::UNIT_X, 0.1f));
			}
			else if(_event.key == Keyboard::Y)
			{
				plane->SetRotation(plane->GetRotation() * Quaternion(Vector3D::UNIT_Y, 0.1f));
			}
			else if(_event.key == Keyboard::Z)
			{
				plane->SetRotation(plane->GetRotation() * Quaternion(Vector3D::UNIT_Z, 0.1f));
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x - 0.1, plane->GetPosition().y, plane->GetPosition().z));
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x + 0.1, plane->GetPosition().y, plane->GetPosition().z));
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x, plane->GetPosition().y + 0.1, plane->GetPosition().z));
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				plane->SetPosition(Vector3D(plane->GetPosition().x, plane->GetPosition().y - 0.1, plane->GetPosition().z));
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий
	}
};



// главная точка входа в программу
// соглашение о вызове функций WINAPI, указывает, что параметры в функцию WinMain передаются слева направо
int WINAPI WinMain(HINSTANCE hInstance,// дескриптор (идентификатор), генерируемый системой для данного приложения
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	// описание графического ядра
	Renderer::Define def;
	def.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна 
	def.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.realTimeWorkerDef.windowDef.tittle = "Plane.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = false;

	
	Application* app = new Application;// создать окно
	
	if(app->Application::Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
