

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






// реализует гизмо
class Gizmo
{
	// DATA
	public: GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>* icosphere;// икосфера

	public: Vector3D vectorX;// направление оси X
	public: Vector3D vectorY;// направление оси Y
	public: Vector3D vectorZ;// направление оси Z
	public: Vector3D vector;// тестируемый вектор
 	
    public: ColorRGBA yellow;// цвет для тестируемого вектора
	public: ColorRGBA cyan;// цвет для оси Y
    public: ColorRGBA red; // цвет для оси X
	public: ColorRGBA green;// цвет для оси Z

	private: float scaleStep;// величина одного шага масштабирования



	// METHODS
	// конструктор по умолчанию
    public: Gizmo(void): icosphere(NIL)
	{
		// задать направление осям X,Y,Z
		vectorX.Set(1.0f, 0.0f, 0.0f);
		vectorY.Set(0.0f, 1.0f, 0.0f);
		vectorZ.Set(0.0f, 0.0f, 1.0f);
		vector.Set(-1.0f, 0.0f, 0.0f);


		scaleStep = 1.2f;
		yellow = ColorRGBA(ColorRGBA::YELLOW_COLOR);
		cyan = ColorRGBA(ColorRGBA::CYAN_COLOR); 
		red = ColorRGBA(ColorRGBA::RED_COLOR);
		green = ColorRGBA(ColorRGBA::GREEN_COLOR);

		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>::Define def;
		def.scale = Size3D(1, 1, 1);
		def.name = "icosphere";
		icosphere = new GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>();
		icosphere->Create(def);
		icosphere->CreateIcosphere3D(3);
		
		// создать буфер цветов
		icosphere->CreateColors();
		icosphere->SetColor(ColorRGBA(ColorRGBA::WHITE_COLOR));
	}


	// визуализация Gizmo
	public: void Draw(void)
	{
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// толщина линий для осей 

		// визуализация осей трехмерной системы координат
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vectorX, red);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vectorY, cyan);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vectorZ, green);

		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vector, yellow);// тестируемый вектор

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(1.0f); // толщина линий для каркаса геосферы 1

		icosphere->Draw();// отобразить сферу
	}

	// нормализация тестируемого вектора
	public: void Normalize(void)
	{
		vector.Normalize();
	}
				
	// масштабирование тестируемого вектора в сторону увеличения
	public: void ScalePlus(void)
	{
		vector *= scaleStep;
	}
				
	// масштабирование тестируемого вектора в сторону уменьшения
	public: void ScaleMinus(void)
	{
		vector /= scaleStep;
	}


	// изменить направление тестируемого вектора на противоположное
	public: void Inverse(void)
	{
		vector.Inverse();
	}

  

	// поворот тестируемого вектора относительно оси X по часовой стрелке
	public: void RotateXMinus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_X, -0.1f);  
	}
		

	// поворот тестируемого вектора относительно оси X против часовой стрелки
	public: void RotateXPlus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_X, 0.1f); 
	}

		
	// поворот тестируемого вектора относительно оси Y против часовой стрелки
	public: void RotateYPlus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Y, 0.1f); 
	}

	// поворот тестируемого вектора относительно оси Y по часовой стрелке
	public: void RotateYMinus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Y, -0.1f);   
	}
		

		
	// поворот тестируемого вектора относительно оси Z по часовой стрелке
	public: void RotateZMinus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Z, -0.1f);   
	}
		

	// поворот тестируемого вектора относительно оси Z против часовой стрелки
	public: void RotateZPlus(void)
	{
		vector.RotateAxisAngle(Vector3D::UNIT_Z, 0.1f);   
	}
};

// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// графическая сцена

    private: Gizmo* gizmo;// гизмо
    private: float vectorTheta, vectorPhi;



    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), gizmo(NIL), vectorTheta(0.0f), vectorPhi(0.0f)
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
				GraphicsScene<Vector3D, Quaternion, Size3D>::Define def;
				def.logicSceneDef.scale = Size3D(1.0f, 1.0f, 1.0f);
				scene = new GraphicsScene<Vector3D, Quaternion, Size3D>();
				scene->Create(def);
			}

			// гизмо...
			gizmo = new Gizmo();
			scene->AddObject(gizmo->icosphere, false, true);

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.textColor = ColorRGBA::WHITE_COLOR;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;

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
				def.pitchMax = 0.0f;// запретить камере опускаться ниже пола (плоскости XZ)
				def.viewingPoint = gizmo->icosphere->GetPosition();
				def.zoomMin = 5.0f;
				def.zoomMax = 6.0f;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
				camera->ConnectTo(Mouse::Event::TYPE, this);

								
				// задать изометрический ракурс камеры
				camera->UpdateTrackBallObjectViewer(-Angle::HALF_PI/3.0f, -Angle::HALF_PI/3.0f, 0.0f);
			}
			
			Mouse::_MoveCursorToClientAreaCenter(this);// переместить курсор в центр клиентской области окна
			Mouse::_ShowCursor(false);// скрыть курсор

			Flush();//выполнить все ранее отложенные события
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}
	




	// функция кадра
	public: virtual void Update(float _delta)
	{}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


		

		// вывод сцены
		gizmo->Draw();

		
	

		// вывод текста...
		camera->Begin2D();// начать вывод двухмерной графики

				
		int i = 0; 

		font2d->SetTextColor(ColorRGBA(ColorRGBA::WHITE_COLOR));
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector(x: %.3f, y: %.3f, z: %.3f)", gizmo->vector.x, gizmo->vector.y, gizmo->vector.z);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.Length: %.3f", gizmo->vector.Length());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, xAxis): %.3f", gizmo->vector.Dot(gizmo->vectorX));  


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, yAxis): %.3f", gizmo->vector.Dot(gizmo->vectorY));  
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, zAxis): %.3f", gizmo->vector.Dot(gizmo->vectorZ));  


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.AngleX: %.3f", gizmo->vector.AngleX()); 


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.AngleY: %.3f", gizmo->vector.AngleY());


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.AngleZ: %.3f", gizmo->vector.AngleZ()); 



		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.theta: %.3f", gizmo->vector.ToSpherical().vertical); 

		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.phi: %.3f", gizmo->vector.ToSpherical().horizontal);
	

		// визуализировать сцену...
		camera->Begin3D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
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
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				vectorTheta -= 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				vectorTheta += 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				//gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}	
			else if(_event.key == Keyboard::UP_ARROW)
			{
				vectorPhi -= 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				//gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				vectorPhi += 0.1f;

				gizmo->vector.FromSpherical(Spherical(1, vectorTheta, vectorPhi));

				//gizmo->vector.FromSpherical(Spherical(1, gizmo->vector.ToSpherical().vertical, gizmo->vector.ToSpherical().horizontal));
			}
			else if(_event.key == Keyboard::N)
			{
				gizmo->Normalize();// нормализировать оси
			}
			else if(_event.key == Keyboard::P)
			{
				gizmo->ScalePlus();// масштабирование в сторону увеличения
			}
			else if(_event.key == Keyboard::M)
			{
				gizmo->ScaleMinus();// масштабирование в сторону уменьшения
			}
			else if(_event.key == Keyboard::I)
			{
				gizmo->Inverse();// инвертировать направление
			}
			else if(_event.key == Keyboard::Q)
			{
				gizmo->RotateXPlus();
			}
			else if(_event.key == Keyboard::W)
			{
				gizmo->RotateXMinus();
			}	
			else if(_event.key == Keyboard::A)
			{
				gizmo->RotateYPlus();
			}
			else if(_event.key == Keyboard::S)
			{
				gizmo->RotateYMinus();
			}
			else if(_event.key == Keyboard::Z)
			{
				gizmo->RotateZPlus();
			}
			else if(_event.key == Keyboard::X)
			{
				gizmo->RotateZMinus();
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
	def.realTimeWorkerDef.windowDef.tittle = "Vector3D.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
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
