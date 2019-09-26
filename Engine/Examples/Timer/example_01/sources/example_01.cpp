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
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>
#include <Engine/Core/Parser/List/IntegerList/IntegerList.h> 
#include <Engine/Core/Parser/List/FloatList/FloatList.h> 
#include <Engine/Core/Parser/List/BooleanList/BooleanList.h> 
#include <Engine/Core/Parser/List/StringList/StringList.h> 
#include <Engine/Core/Parser/List/Vector2List/Vector2List.h> 
#include <Engine/Core/Parser/List/Vector3List/Vector3List.h> 
#include <Engine/Core/Parser/List/QuaternionList/QuaternionList.h> 
#include <Engine/Core/Shape/Mesh/DelaunayTriangulation/DelaunayTriangulation.h>
#include <Engine/Core/Shape/Mesh/SplitTriangulation/SplitTriangulation.h>
#include <Engine/Core/Time/Timer/Timer.h>



// пользовательское приложение
class Application: public Engine,// движок
               	   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
    private: Camera3D *camera2d;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена

    private: Timer *timer1, *timer2, *timer3, *timer4, *timer5;// таймеры
    private: bool timer1IsFinished, timer2IsFinished, timer3IsFinished, timer4IsFinished, timer5IsFinished;
		
	// имитация таймера, в течении которого отображается сообщение о достижении таймером timer4 целевого времени
    private: float timer4Accum;// оставшееся время аккумулятора
	private: bool timer4AccumEnabled;// если true, то аккумулятор работает
	private: float timer4AccumFinishTimeInSec; // время работы аккумулятора в секундах

	// имитация таймера, в течении которого отображается сообщение о достижении таймером timer5 целевого времени
    private: float timer5Accum;// оставшееся время аккумулятора
	private: bool timer5AccumEnabled;// если true, то аккумулятор работает
	private: float timer5AccumFinishTimeInSec; // время работы аккумулятора в секундах


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera2d(NIL), font(NIL), scene(NIL), timer1(NIL), timer2(NIL), timer3(NIL), timer4(NIL), timer5(NIL), timer1IsFinished(false), timer2IsFinished(false), timer3IsFinished(false), timer4IsFinished(false), timer5IsFinished(false), timer4Accum(0.0f), timer4AccumEnabled(false), timer4AccumFinishTimeInSec(0.8f), timer5Accum(0.0f), timer5AccumEnabled(false), timer5AccumFinishTimeInSec(0.8f)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete camera2d;

		delete font;

		delete timer1;
		delete timer2;
		delete timer3;
		delete timer4;
		delete timer5;

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
				def.color = RGBA(RGBA::GRAY_COLOR).GetRGBAf();// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}
		

			// камера для двухмерной графики...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				def.distanceToViewingPoint = 1.0f;
				def.rotationEnable = false;// запретить вращение камеры

				camera2d = new Camera3D();
				camera2d->Create(def);
			}

			// таймеры...
			{
				// далее представлены различные способы инициализации таймера

				timer1 = new Timer(Timer::Define(6.0f, 0.0, PlayingObject::SINGLE_SHOT_PLAYING, true));// создать таймер с временем жизни 6 секунд
				timer1->Rename("timer1"); 
				scene->AddToUpdateList(timer1);
				timer1->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &Application::Timer1IsFinished));// обработка события
				
			
				timer2 = new Timer(Timer::Define(6.0f, 0.0f, PlayingObject::SINGLE_SHOT_PLAYING, true));// создать таймер с временем жизни 6 секунд
				timer2->Rename("timer2");
				scene->AddToUpdateList(timer2);
				timer2->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &Application::Timer2IsFinished));// обработка события


				timer3 = new Timer();
				timer3->Rename("timer3");
				scene->AddToUpdateList(timer3);
				timer3->SetPlayingDurationInHMS(HMS(0, 0, 9));
				timer3->Play();
				timer3->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &Application::Timer3IsFinished));// обработка события
				timer3->Play();


				timer4 = new Timer();
				timer4->Rename("timer4");
				scene->AddToUpdateList(timer4);
				timer4->SetPlayingDurationInSec(3.0f);
				timer4->Play();
				timer4->SetPlayingMode(PlayingObject::LOOP_PLAYING);
				timer4->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &Application::Timer4IsFinished));// обработка события
				timer4->Play();


				timer5 = new Timer(Timer::Define(12.0f, 0.0f, PlayingObject::LOOP_PLAYING, true));// создать таймер с временем жизни 12 секунд
				timer5->Rename("timer5");
				scene->AddToUpdateList(timer5);
				timer5->Connect(Timer::TIMER_FINISH_MESSAGE, this, Caller<>(this, &Application::Timer5IsFinished));// обработка события
			}

			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));// белый цвет

			MoveToCenter();// переместить окно в центр экрана
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}


    // реакция на завершения работы таймера 
	public: void Timer1IsFinished(void)
	{
		timer1IsFinished = true;
	} 
				

	// реакция на завершения работы таймера 
    public: void Timer2IsFinished(void)
	{
		timer2IsFinished = true;
	} 

		
	// реакция на завершения работы таймера 
    public: void Timer3IsFinished(void)
	{
		timer3IsFinished = true;
	} 


	// реакция на завершения работы таймера 
    public: void Timer4IsFinished(void)
	{
		timer4IsFinished = true;
	} 

				
	// реакция на завершения работы таймера 
    public: void Timer5IsFinished(void)
	{
		timer5IsFinished = true;
	} 



	// функция обновления
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);
	}



	// функция визуализации
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену

		// задать новую трансформацию сцены...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// вывод текста...
		int32 i = 0;

			
	
		font->DrawScreenText(Vector3(10, i+=20, -1), "timer1.name: %s", timer1->GetName().c_str()); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "timer1.durationInSec: %.2f", timer1->GetPlayingDurationInSec());
		font->DrawScreenText(Vector3(10, i+=20, -1), "timer1.paused: %d", timer1->IsPaused());
		font->DrawScreenText(Vector3(10, i+=20, -1), "timer1.elapsedTimeInSec: %.2f", timer1->GetElapsedTimeInSec());
		font->DrawScreenText(Vector3(10, i+=20, -1), "timer1.remainedTimeInSec: %.2f", timer1->GetRemainedTimeInSec());	
	

		
		if(timer1IsFinished)
			font->DrawScreenText(Vector3(10, i+=20, -1), "TIMER1 IF FINISHED");


		if(timer2IsFinished)
			font->DrawScreenText(Vector3(10, i+=20, -1), "TIMER2 IF FINISHED");
	

		if(timer3IsFinished)
			font->DrawScreenText(Vector3(10, i+=20, -1), "TIMER3 IF FINISHED");
	

		if(timer4AccumEnabled) { timer4Accum -= _delta; } // разрядить аккумулятор

		if(timer4IsFinished)
		{
			timer4IsFinished = false;

			// запустить таймер
			timer4Accum = timer4AccumFinishTimeInSec;// время работы аккумулятора в секундах
			timer4AccumEnabled = true; // включить аккумулятор
		}

		if(timer4Accum > 0.0f)// пока аккумулятор не разряжен
		{
			font->DrawScreenText(Vector3(10, i+=20, -1), "TIMER4 IF FINISHED");
		}
		else if(timer4AccumEnabled)// если аккумулятор работает
		{
			timer4AccumEnabled = false;// выключить аккумулятор
		}

		
		if(timer5AccumEnabled) { timer5Accum -= _delta; } // разрядить аккумулятор

		if(timer5IsFinished)
		{
			timer5IsFinished = false;

			// запустить таймер
			timer5Accum = timer5AccumFinishTimeInSec;// время работы аккумулятора в секундах
			timer5AccumEnabled = true; // включить аккумулятор
		}

		if(timer5Accum > 0.0f)// пока аккумулятор не разряжен
		{
			font->DrawScreenText(Vector3(10, i+=20, -1), "TIMER5 IF FINISHED");
		}
		else if(timer5AccumEnabled)// если аккумулятор работает
		{
			timer5AccumEnabled = false;// выключить аккумулятор
		}


		// восстановить предыдущую трансформацию...
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
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
			else if(_event->key == Keyboard::P)
			{
				if(timer1->IsPaused()) 
				{ 
					timer1IsFinished = false;
					timer1->Play(); 
				}
				else
				{ 
					timer1->Pause(); 
				}
			}
			else if(_event->key == Keyboard::S)
			{
				timer1->Stop();
			}
			else if(_event->key == Keyboard::SPACE)
			{
				timer1->SetPlayingDurationInSec(timer1->GetPlayingDurationInSec() + 1.0f);
			}
			else if(_event->key == Keyboard::BACKSPACE)
			{
				timer1->SetPlayingDurationInSec(timer1->GetPlayingDurationInSec() - 1.0f);
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);// размер окна
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Timer.example_01";
	//def.rendererDef.drawCallController.SetMode(CallController::UNLIM);
	//def.rendererDef.vSync = true;
	//def.rendererDef.zBufferEnable = false;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
