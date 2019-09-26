// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Helpers/Scene/Scene.h>



// класс, реализующий пользовательское окно
class Application: public Renderer
{
	public: enum
	{
		BOX, // прямоугольник
		QUAD,// квад (плоскость)
		CIRCLE,// круг (плоскость)
		ICOSPHERE, // икосфера
		PYRAMID// пирамида
	};

	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: Scene* scene;// графическая сцена
	private: GraphicsMesh<Vector3D, Quaternion, Matrix4x4>* mesh;
	private: int32 level;// уровень детализации сетки
	private: bool wire;// если true, то включить каркасный режим визуализации
	private: int32 curShape;// текущая фигура

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), mesh(NIL), level(0), wire(false), curShape(BOX)
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
				scene = new Scene();
			}

			// полигональная сетка...
			{
				mesh = new GraphicsMesh<Vector3D, Quaternion, Matrix4x4>();

				RebuildMesh(curShape);// создать мэш согласно текущей установленной фигуре
				mesh->Rename("mesh");
				mesh->SetScale(Size3D(0.5f, 0.5f, 0.5f));
				
				scene->AddObject(mesh, false, true);// добавить в сцену			
			}
			


			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = RGBA(RGBA::GRAY_COLOR);

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d);
				font2d->Pick();
			}


			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
				def.viewingPoint = mesh->GetPosition();
				def.zoomMin = 5;
				def.zoomMax = 10;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera);
				camera->Pick(Tittle());
				
				// задать изометрический ракурс камеры
				camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, -Angle::HALF_PI/3.0f, 0.0f);
			}


			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));// белый цвет фона
			
			GraphicsLine<Vector3D>::_SetLineWidth(4.0f);// толщина линий

			MoveToCenter();// переместить окно в центр крана
			Mouse::_MoveCursorToCenter(this);// переместить курсор в центр клиентской области окна	
			Mouse::_ShowCursor(false);// скрыть курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}
	
	// создает полигональную сетку согласно текущей установленной фигуре _shape
	public: void RebuildMesh(int32 _shapeId)
	{
		switch(_shapeId)
		{
			case QUAD:
			{
				//mesh->SetCullingEnable(false); 
				mesh->CreateQuad2D(Quad<Vector3D>(Vector3D(0,0,0), Size2D(1.0f, 1.0f)));
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}

			case CIRCLE:
			{
				//mesh->SetCullingEnable(false);
				mesh->CreateCircle2D(Circle<Vector3D>(Vector3D(), 1), 10);
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}

			case BOX:
			{
				mesh->SetCullingEnable(true); 
				mesh->CreateBox3D(Box3D(Vector3D(0, 0, 0), Size3D(1.0f, 1.0f, 1.0f)));
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}
			case PYRAMID:
			{
				mesh->SetCullingEnable(true); 
				mesh->CreatePyramid3D(1);
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}
			case ICOSPHERE:
			{
				mesh->SetCullingEnable(true); 
				mesh->CreateIcosphere3D(0);
				mesh->SetColor(Color<RGBA>::_Random());
				curShape = _shapeId;
				break;
			}
			default: break;
		}
	}

	
	// возвращает имя фигуры с идентификатором _shapeId
	public: StringANSI NameOfShape(int32 _shapeId)
	{
		switch(_shapeId)
		{
			case QUAD:
			{
				return "QUAD";
			}
			case CIRCLE:
			{
				return "CIRCLE";
			}
			case BOX:
			{
				return "BOX";
			}
			case PYRAMID:
			{
				return "PYRAMID";
			}
			case ICOSPHERE:
			{
				return "ICOSPHERE";
			}
			default: break;
		}
		return "NON";
	}


	// создает следующую фигуру
	public: void SetNextShape(int32 _shapeId)
	{
		RebuildMesh(_shapeId + 1); 
	}

	// создает предыдущую фигуру 
	public: void SetBackShape(int32 _shapeId)
	{
		RebuildMesh(_shapeId - 1); 
	}

	// увеличивает уровень детализации полигональной сетки
	public: void AddMeshDetail(void)
	{
		if(mesh) // если сетки существует
		{ 
			level++;// увеличить уровень детализации
			
			
			mesh->TotalPolygonsDivide4();// разделить полигоны фигуры на 4
		} 
	}

	// функция кадра
	public: virtual void Update(float _delta)
	{}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


		camera->BeginPerspective3D();

		scene->Draw();


		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики

		int32 i = 0;
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "polygons: %d", mesh->AmountOfPolygons()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vertices: %d", mesh->AmountOfVertices()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "indices: %d", mesh->AmountOfIndices());  

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "detail level: %d", level); 
	
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "current shape: %s", NameOfShape(curShape).c_str()); 
	



		// визуализировать сцену...
		//camera->End2D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual bool MouseEventFilter(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
				if(!wire) 
				{
					mesh->SetWireframeEnable(true);
					wire = true;
				}
			}
			else if(wire)
			{
				mesh->SetWireframeEnable(false);
				wire = false;
			}
		}

		Mouse().ClearEventList();// очистить буфер событий
		return true;
	}


	// обработка событий клавиатуры
	public: virtual bool KeyboardEventFilter(Keyboard::Event _event)
	{
		if(_event.type == Keyboard::KEY_DOWN)
		{
			if(_event.key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			} 
			else if(_event.key == Keyboard::D)
			{
				AddMeshDetail();
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				SetBackShape(curShape);
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				SetNextShape(curShape);
			}

		}

		Keyboard().ClearEventList();// очистить буфер событий
		return true;
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
	def.realTimeWorkerDef.windowDef.tittle = "GraphicsMesh.example_01";
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
