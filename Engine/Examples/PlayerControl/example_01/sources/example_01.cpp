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
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>


// класс, который реализует графический квад
class MyGraphicsQuad: public GraphicsObject,// графический объект
	                  public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: float distToWindowBorder;// расстояние границ квада до границ окна
	private: RGBA color;// цвет рамок
	private: Quad rect;// прямоугольник, описывающий координаты и размер квада


	// METHODS
	// конструктор без параметров
	public: MyGraphicsQuad(void): distToWindowBorder(30.0f)
	{
		color = RGBA(RGBA::GREEN_COLOR);
	}


	// деструктор
	public: virtual ~MyGraphicsQuad(void)
	{
		Destroying();
	}


	// РАЗМЕР...
	// возвращает прямоугольник, описывающий координаты и размер квада
	public: Quad GetRect(void)const
	{
		return rect;
	}


	// ИЗМЕНЕНИЕ...
	// обновить размер квада
	// примечание: _size - это размер вьюпорта окна
	public: void SetSize(Vector2 _size)
	{
		_size.x -= distToWindowBorder * 2;
		_size.y -= distToWindowBorder * 2;
		rect.SetSize(_size);
		
		// сместить прямоугольник в центр клиентской области окна
		Vector2 clientWindowCenter = Renderer::_GetPicked()->ScreenToClient(Renderer::_GetPicked()->ClientAreaCenter());// вычислить центр окна в клиентской системе координат
		rect.SetCenter(Vector3(clientWindowCenter.x, clientWindowCenter.y, 0.0f));// задать новый центр прямоугольника
	}


	// ВИЗУАЛИЗАЦИЯ...
	// визуализировать прямоугольник
	public: virtual void Draw(void)
	{
		// нижняя горизонтальная линия
		GraphicsLine::_Draw(rect.GetVertex4(), rect.GetVertex3(), color);// рисуем слева направо

		// верхняя горизонтальная линия
		GraphicsLine::_Draw(rect.GetVertex1(), rect.GetVertex2(), color);// рисуем слева направо

		// левая вертикальная линия
		GraphicsLine::_Draw(rect.GetVertex1(), rect.GetVertex4(), color);// рисуем сверху вниз

		// правая вертикальная линия
		GraphicsLine::_Draw(rect.GetVertex2(), rect.GetVertex3(), color);// рисуем сверху вниз
	}

		
	// СОХРАНЕНИЕ И ЗАГРУЗКА...
	// загружает объект из файла по пути _path, и возвращает true в случае успеха
	// примечание: если _auto равен true, то загрузка объекта производится по имени, из стандартной директории
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
};

// графическая точка
class MyGraphicsPoint: public GraphicsPoint,// графическая точка
	                   public LogicObject,//логический объект
					   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: MagicList<PlayerControl> actionList;// список действий над точкой
	
	private: MyGraphicsQuad* quad;// ограничивающий прямоугольник
	private: Vector3 direction;// направление движения точи
    private: float moveSpeed;// размер одного шага движения (при нажатии на клавишу движения)
	private: bool keyPressed;// если true, то произошло нажатие клавиш

	private: int32 size;// размер точки (по ширине и высоте) в пикселях
	private: float scale;// коэффициент масштабирования точки
	private: float maxScale;// максимальный масштаб точки
	private: float minScale;// минимальный масштаб точки


	// METHODS
	// конструктор без параметров
	public: MyGraphicsPoint(void): size(3), moveSpeed(300.0f), maxScale(5.0f), minScale(1.0f), scale(1.0f), quad(NIL), keyPressed(false)
	{
		size = 10.0f;
		SetSize(size * scale);// размер точек

		{// создать действие
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName); 
			Mouse::Event* mouseEvent = new Mouse::Event();
			mouseEvent->type = Mouse::WHEEL;
			mouseEvent->key = Mouse::NO_KEY;
			action->AddEvent(mouseEvent);// добавить событие в список условий активации
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::ChangeSize));
			actionList.AddToTail(action);
		}


		{// создать действие
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::Z; 
			action->AddEvent(keyboardEvent);// добавить событие в список условий активации
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::ChangeColor));
			actionList.AddToTail(action); 
		}


		{// создать действие
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED;  
			keyboardEvent->key = Keyboard::UP_ARROW; 
			action->AddEvent(keyboardEvent);// добавить событие в список условий активации
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveUp));
			actionList.AddToTail(action);
		}

		{// создать действие
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::DOWN_ARROW; 
			action->AddEvent(keyboardEvent);// добавить событие в список условий активации
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveDown));
			actionList.AddToTail(action);
		}
		
		
		{// создать действие
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::LEFT_ARROW; 
			action->AddEvent(keyboardEvent);// добавить событие в список условий активации
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveLeft));
			actionList.AddToTail(action);
		}
		
		{// создать действие
			PlayerControl* action = new PlayerControl();
			//action->Rename(_actionName);
			Keyboard::Event* keyboardEvent = new Keyboard::Event();
			keyboardEvent->type = Keyboard::KEY_PRESSED; 
			keyboardEvent->key = Keyboard::RIGHT_ARROW; 
			action->AddEvent(keyboardEvent);// добавить событие в список условий активации
			action->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &MyGraphicsPoint::MoveRight));
			actionList.AddToTail(action);
		}

		MoveToWindowCenter();// переместить точку в центр клиентской области окна
	}


	// деструктор
	public: virtual ~MyGraphicsPoint(void)
	{
		Destroying();
	}


	// создает ограничивающий прямоугольник _quad для точки
	public: void SetQuad(MyGraphicsQuad* _quad)
	{
		quad = _quad;
	}


	// РЕАКЦИЯ НА СОБЫТИЯ...
	// изменение цвета точки на случайный
	private: void ChangeColor(PlayerControl* _action)
	{
		SetColor(RGBA::_GetRandom(155, 255));// задать случайный цвет 
	}


	// изменение размера точки
	private: void ChangeSize(PlayerControl* _action)
	{
		Mouse::Event* mEvent = dynamic_cast<Mouse::Event*>(_action->GetActivatedEvents().GetHead());

		if(mEvent && mEvent->type == Mouse::WHEEL)
		{
			float offset = (float)mEvent->wheel / 3.0f;

			if((scale + offset) <= maxScale && (scale + offset) >= minScale)// контроль диапазона
			{
				scale += offset;
				GraphicsPoint::_SetPointSize(size * scale);// размер точек
			}
		}
	}

	// движение вверх
	private: void MoveUp(PlayerControl* _action)
	{
		direction += Vector3(0.0f, -1.0f, 0.0f);
		keyPressed = true;
	}


	// движение вниз
	private: void MoveDown(PlayerControl* _action)
	{
		direction += Vector3(0.0f, 1.0f, 0.0f);
		keyPressed = true;
	}


	// движение влево
	private: void MoveLeft(PlayerControl* _action)
	{
		direction += Vector3(-1.0f, 0.0f, 0.0f);
		keyPressed = true;
	}


    // движение вправо
	private: void MoveRight(PlayerControl* _action)
	{
		direction += Vector3(1.0f, 0.0f, 0.0f);
		keyPressed = true;
	}


	// РАЗМЕР...
	// возвращает размер точки в пикселях
	public: int32 GetSize(void)const
	{
		return size;
	}


	// задает новый размер точки (в пикселях)
	public: void SetSize(int32 _size)
	{
		GraphicsPoint::_SetPointSize(_size);// размер точек
	}



	// ИЗМЕНЕНИЕ СОСТОЯНИЯ...
	// обновляет состояние точки 
	public: virtual void Update(float _delta)
	{
		if(keyPressed)// если было совершено нажатие клавиши
		{ 
			// обновить целевую позицию
			SetPosition(GetPosition() + Vector3::_Normalize(direction) * moveSpeed * _delta); 
			direction.Set(0.0f, 0.0f, 0.0f);// обнулить вектор движения
			keyPressed = false;// выключить движение
		}

		if(quad && !quad->GetRect().IsPointInside(GetPosition()))// если точка вышла за пределы квада
		{
			SetPosition(quad->GetRect().ClampPoint(GetPosition()));// ограничить координаты точки в рамках квада
		}
	}


	// двигает точку в центр клиентской области окна
	public: void MoveToWindowCenter(void)
	{
		Vector2 winCenter = Renderer::_GetPicked()->ScreenToClient(Renderer::_GetPicked()->ClientAreaCenter());// взять центр клиентской области окна в экранных координатах, и конвертировать в клиентские экранные
		winCenter.y = Renderer::_GetPicked()->GetViewportSize().y - winCenter.y;// конвертируем клиентские экранные координаты в двухмерные, с началом в левом нижнем углу
		SetPosition(Vector3(winCenter.x, winCenter.y, 0));// поместить в центр клиентской области окна
	}

		
	// СОХРАНЕНИЕ И ЗАГРУЗКА...
	// загружает объект из файла по пути _path, и возвращает true в случае успеха
	// примечание: если _auto равен true, то загрузка объекта производится по имени, из стандартной директории
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}

				
	// загружает объект из файла _file, и возвращает true в случае успеха
	public: virtual bool LoadFromFile(File& _file)
	{
		return false;
	}


	// сохраняет объект в конец файла _file, и возвращает true в случае успеха
	public: virtual bool SaveToFile(File& _file)
	{
		return false;
	}
};


// пользовательское приложение
class Application: public Engine// движок
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена
	
	private: MyGraphicsPoint* point; // точка, управляемая пользователем
	private: MyGraphicsQuad* quad;// графический прямоугольник
	
	
    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), quad(NIL), point(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete camera;

		delete font;

		delete quad;
		delete point;

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
				//scene->Rename("scene");// имя сцены
				//AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
				scene->Pick();
			}


			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.color = RGBA(RGBA::WHITE_COLOR);// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}


			// камера...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				def.pitchClampEnable = false;// отключить блокировку вращения вокруг оси X
				
				camera = new Camera3D();
				camera->Set(def);
				
				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
			}
		

			// графический прямоугольник...
			{
				quad = new MyGraphicsQuad();
				//quad->Rename("quad");// имя
				quad->SetSize(Renderer::_GetPicked()->GetViewportSize());// размер прямоугольника
				scene->AddGraphicsObject(quad);// добавить в графическую сцену
				Connect(Window::RESIZE_WINDOW_MESSAGE, quad, Caller<>(quad, &MyGraphicsQuad::SetSize));// подписаться на событие изменения размеров окна
			}


			// графическая точка...
			{
				point = new MyGraphicsPoint();
				point->SetQuad(quad);// задать ограничивающий прямоугольник для точки

				scene->AddObject(point);// добавить в сцену
				scene->AddLogicObject(point);// добавить в логическую сцену
				scene->AddGraphicsObject(point);// добавить в графическую сцену
			}

			// отклик клавиатуры
			//Keyboard().SetKeyPressedPeriodInSec(Keyboard().GetKeyPressedPeriodInSec() / 6.0f);
			
			// линии...
			GraphicsLine::_SetLineWidth(3.0f);// толщина линий


			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));

			MoveToCenter();// переместить окно в центр экрана
			Mouse::_SetShowCursor(false);// скрыть курсор
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
		camera->BeginTransform(Renderer::PROJECTION);
		camera->BeginTransform(Renderer::MODELVIEW);


		// вывод текста...
		int32 i = 0;

			
		//font->DrawScreenText(Vector3(10, i+=20, -1), "quaternion: %d/%d/%d/%d", point->uparrow, point->downarrow, point->leftarrow, point->rightarrow);

		/*
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.buildIn: %d", qLerp->IsFloatLerpBuildInEnabled());

		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.mode: %d", qLerp->GetFloatLerp()->GetLerpMode());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing duration: %f", qLerp->GetFloatLerp()->GetPlayingDurationInSec());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing speed: %f", qLerp->GetFloatLerp()->GetPlayingSpeed());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.playing: %d", qLerp->GetFloatLerp()->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.target: %f", qLerp->GetFloatLerp()->GetTarget());
		font->DrawScreenText(Vector3(10, i+=20, -1), "floatLerp.current: %f", qLerp->GetFloatLerp()->GetCurrent());


		*/ 
		
		// восстановить предыдущую трансформацию...
		camera->EndTransform(Renderer::PROJECTION);
		camera->EndTransform(Renderer::MODELVIEW);
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "PlayerControl.example_01";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
