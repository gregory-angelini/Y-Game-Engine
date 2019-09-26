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
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Material/AnimatedMaterial/AnimatedMaterial.h>
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Helpers/Mesh/ColoredMesh/ColoredMesh.h>


// класс, реализующий трехмерную цветную фигуру
class ColoredMesh3D: public ColoredMesh<Vector3D, Quaternion, Matrix4x4>,// цветная полигональная сетка
	                 public LogicObject // логический объект
{
	// идентификаторы фигуры
	public: enum
	{
		BOX, // прямоугольник
		QUAD,// квад (плоскость)
		ICOSPHERE, // икосфера
		PYRAMID// пирамида
	};

	// DATA
	private: int32 detailLevel;// уровень детализации фигуры
	private: int32 shape;// идентификатор фигуры
	private: ShapeType type;// тип фигуры


	// METHODS
	// конструктор по умолчанию
	// примечание: по умолчанию создается икосфера
	public: ColoredMesh3D(void): detailLevel(0), shape(ICOSPHERE), type(IColoredMaterial::ANGULAR_TYPE)
	{}
 

	// ОБНОВЛЕНИЕ...
	// обновление состояния объекта...
	public: virtual void Update(float _delta)
	{
		SetRotation(GetRotation() * Quaternion(Vector3D::UNIT_Y, 0.01f));
	}


	// МАТЕРИАЛ...
	// создает цветной материал для текущей полигональной сетки типа _type, и возвращает true в случае успеха
	// примечание: если цветной материал уже существует, то вернется false
	// примечание: полигональная сетка должна существовать
	public: virtual bool CreateColoredMaterial(ShapeType _type)
	{
		type = _type;// запомнить тип фигуры
		bool result = ColoredMaterial::CreateColoredMaterial(_type);
		SetColors();// задать вершинам/полигонам новые цвета
		return result;
	}


	// задать цвета для вершин/полигонов
	private: void SetColors(void)
	{ 	
		if(type == IColoredMaterial::ANGULAR_TYPE)// остроугольная фигура
		{
			// задать случайные цвета
			for(int32 i = 0; i < AmountOfPolygons(); ++i)
			{
				if(i % 2)// для четных индексов
				{ 
					IColoredMaterial::SetColor(i, RGBA(RGBA::RED_COLOR));
				}
				else// для нечетных 
				{
					IColoredMaterial::SetColor(i, RGBA(RGBA::BLUE_COLOR));
				}
			}
		}
		else if(type == IColoredMaterial::CURVED_TYPE)// округлая фигура
		{
			for(int32 i = 0; i < GetAmountOfColors(); ++i)
			{
				if(i % 2)// для четных индексов
				{ 
					IColoredMaterial::SetColor(i, RGBA(RGBA::RED_COLOR));
				}
				else// для нечетных 
				{
					IColoredMaterial::SetColor(i, RGBA(RGBA::BLUE_COLOR));
				}
			}
		}
	}


	// возвращает тип фигуры
	public: StringANSI GetShapeTypeString(void)const
	{
		if(IsAngularShape())
		{
			return "ANGULAR_MATERIAL";
		}
		else if(IsCurvedShape())
		{
			return "CURVED_MATERIAL";
		}
	
		return "";
	}

	
 
	// ФИГУРА...
	// создает фигуру _shape типа _type
	public: void CreateShape(int32 _shape, ShapeType _type)
	{
		switch(_shape)
		{
			case QUAD:
			{
				SetCullingEnable(false);
				CreateQuad2D(Quad<Vector3D>(Vector3D(0,0,0), Vector2D(1.0f, 1.0f)));
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
			case BOX:
			{
				SetCullingEnable(true);
				CreateBox3D(Box3D(Vector3D(0, 0, 0), Vector3D(1.0f, 1.0f, 1.0f)));	
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
			case PYRAMID:
			{
				SetCullingEnable(true);
				CreatePyramid3D(1);
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
			case ICOSPHERE:
			{
				SetCullingEnable(true);
				CreateIcosphere3D(1);
				CreateColoredMaterial(_type);
				shape = _shape;
				type = _type;
				detailLevel = 0;
				break;
			}
		}
	}
 

	// возвращает идентификатор фигуры
	public: int32 GetShape(void)const
	{
		return shape;
	} 
	

	// возвращает идентификатор фигуры в виде строки
	public: StringANSI GetShapeString(void)
	{
		switch(shape)
		{
			case QUAD:
			{
				return "QUAD";
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
		return "";
	}

	// создает следующую фигуру
	public: void SetNextShape(void)
	{
		CreateShape(shape + 1, type); 
	}


	// создает предыдущую фигуру 
	public: void SetBackShape(void)
	{
		CreateShape(shape - 1, type);
	}


	// возвращает текущий уровень детализации фигуры
	// примечание: 0 - минимальный уровень детатализации
	public: int32 GetDetailLevel(void)
	{
		return detailLevel;
	}


	// увеличивает уровень детализации фигуры
	public: void AddDetailLevel(void)
	{
		detailLevel++;// увеличить уровень детализации
			
		TotalPolygonsDivide4();// разделить полигоны фигуры на 4

		// дополнительные действия
		switch(shape)
		{
			case QUAD:
			{
				CreateColoredMaterial(type);
				break;
			}
			case BOX:
			{
				CreateColoredMaterial(type);
				break;
			}
			case PYRAMID:
			{
				CreateColoredMaterial(type);
				break;
			}
			case ICOSPHERE:
			{
				TotalVertexNormalize();// нормализовать все вершины
				CreateColoredMaterial(type);
				break;
			}
			default: break;
		}
	}
};
 

// пользовательское приложение
class Application: public Engine,// движок
         	       public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D *font2d;// шрифт
	private: Scene* scene;// сцена
    private: ColoredMesh3D* mesh;// цветная сетка


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), mesh(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{} 



	// создает приложение
	public: bool Create(Engine::Define _define)
	{
		// создаем движок...
		if(Engine::Create(_define))
		{
			// сцена...
			{
				scene = new Scene();
				scene->Rename("scene");// имя сцены
				AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
			}


			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 20;// размер шрифта
				def.fontColor = RGBA(RGBA::WHITE_COLOR);// цвет шрифта

				font2d = new Font2D();
				font2d->Rename("font2d");
				font2d->Create(def);
				scene->AddObject(font2d);// добавить в сцену
				font2d->Pick();// выбрать шрифт
			}



			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
				def.zoomMin = 4;
				def.zoomMax = 10;

				camera = new Camera3D();
				camera->Create(def);
				
				camera->Pick(Title()); // выбрать камеру 
				
				// задать изометрический ракурс камеры
				camera->UpdateTrackballObjectViewer(-Angle::HALF_PI/3.0f, 0.0f, 0.0f);
				camera->SetRotationEnable(false);// запретить вращение камеры
				
				AssetLibrary::_GetAssets().AddObject(camera);// добавить в сцену
			}


			// цветная сетка...
			{
				mesh = new ColoredMesh3D();
				mesh->Rename("mesh");
				mesh->CreateShape(ColoredMesh3D::BOX, IColoredMaterial::CURVED_TYPE);// создать фигуру
				mesh->SetColoredMaterialEnable(true);// включить цветной материал
				scene->AddGraphicsObject(mesh);// добавить в графическую сцену	
				scene->AddLogicObject(mesh);// добавить в логическую сцену
			}

			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			
			MoveToCenter();// переместить окно в центр крана
			Mouse::_MoveCursorToCenter(this);// переместить курсор в центр клиентской области окна	
			//Mouse::_ShowCursor(false);// скрыть курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}

	

	// функция кадра
	public: virtual void Update(float _delta)
	{
		scene->Update(_delta);
	}



	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


		
		camera->BeginScreen2D();// вывод двухмерной графики


		// вывод текста...
		int32 i = 0;


	

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape detail: %d", mesh->GetDetailLevel()); 
	    font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape: %s", mesh->GetShapeString().c_str()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape type: %s", mesh->GetShapeTypeString().c_str());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "colored material enabled: %d", mesh->IsColoredMaterialEnabled());
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "camera position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z); 


		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape attributes: %s", mesh->StringAttributes().c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape vertices: %d", mesh->AmountOfVertices()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape polygons: %d", mesh->AmountOfPolygons()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "shape vertex colors: %d", mesh->GetAmountOfColors()); 



		camera->BeginPerspective3D();// вывод трехмерной графики

		scene->Draw();// отобразить объекты сцены
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Mouse().ClearEventList();// очистить буфер событий
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
				Destroy();// уничтожить окно
			} 
			else if(_event->key == Keyboard::D)
			{
				mesh->AddDetailLevel();
			}
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				mesh->SetBackShape();
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				mesh->SetNextShape();
			}
			else if(_event->key == Keyboard::UP_ARROW)
			{
				mesh->CreateColoredMaterial(IColoredMaterial::ANGULAR_TYPE);
			}
			else if(_event->key == Keyboard::DOWN_ARROW)
			{
				mesh->CreateColoredMaterial(IColoredMaterial::CURVED_TYPE);
			}
			else if(_event->key == Keyboard::E)
			{
				mesh->SetColoredMaterialEnable(!mesh->IsColoredMaterialEnabled());
			}
		}


		Keyboard().ClearEventList();// очистить буфер событий
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
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "ColoredMesh.example_01";

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
