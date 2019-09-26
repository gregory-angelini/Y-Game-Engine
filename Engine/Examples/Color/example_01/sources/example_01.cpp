// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Shape/Box/GraphicsBox3DFixed/GraphicsBox3DFixed.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>





// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene<Vector3D, Quaternion, Matrix4x4>* scene;// графическая сцена
	private: GraphicsPoint<Vector2D, RGBA> graphicsPoint;// графическая точка
	private: float sizeOfPoint;// размер точки
	private: bool randomize;// если true, то цвет точки будет вычислен автоматически

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), sizeOfPoint(0), randomize(false)
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
			MoveToCenter();// переместить окно в центр крана
			Mouse::_MoveCursorToClientAreaCenter(this);// переместить курсор в центр клиентской области окна	

			// сцена...
			{
				GraphicsScene<Vector3D, Quaternion, Matrix4x4>::Define def;
				scene = new GraphicsScene<Vector3D, Quaternion, Matrix4x4>();
				scene->Create(def);
			}


			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;
				def.textColor = RGBA::GRAY_COLOR;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}

			
			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationEnabled = false;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, false, false);
				camera->Pick("Color.example_01");
			}

			// цветная точка...
			{
				graphicsPoint.SetPosition(ScreenToClient(ClientAreaCenter()));
				graphicsPoint.SetColor(RGBA::BLACK_COLOR);
				sizeOfPoint = 100.0f;
			}

			SetBackgroundColor(RGBA::WHITE_COLOR);// белый цвет фона
			
			GraphicsPoint<Vector2D, RGBA>::_SetPointSize(sizeOfPoint);// толщина точек
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}
	

	// функция кадра
	public: virtual void Update(float _delta)
	{
		if(randomize) { graphicsPoint.SetColor(Color<RGBA>::_Random()); } 
	}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену




		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики

		// вывести точку
		graphicsPoint.Draw();

		int32 i = 0;


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "graphics point.color.key: %u", graphicsPoint.GetColor().Key()); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "graphics point.color[red/green/blue]: %d/%d/%d", graphicsPoint.GetColor()[RGBA::RED], graphicsPoint.GetColor()[RGBA::GREEN], graphicsPoint.GetColor()[RGBA::BLUE]); 

	    //font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "current shape: %s", NameOfShape(curShape).c_str()); 
	



		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики
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
			else if(_event.key == Keyboard::R)
			{
				randomize = !randomize;
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() - 256));
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() + 256));
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() - 256 * 256));
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				graphicsPoint.SetColor(RGBA(graphicsPoint.GetColor().Key() + 256 * 256));
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
	def.realTimeWorkerDef.windowDef.tittle = "Color.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
	//def.faceDrawMode[Renderer::BACK] = Renderer::LINE;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;

	
	Application* app = new Application;// создать окно
	
	if(app->Application::Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
