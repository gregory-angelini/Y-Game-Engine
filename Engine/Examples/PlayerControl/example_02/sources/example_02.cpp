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




// пользовательское приложение
class Application: public Engine,// движок
               	   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена
	

    private: PlayerControl *act1, *act2, *act3;// контроллеры
    private: Mouse::Event *evt5, *evt6;// события мыши

	private: bool act1Activated, act2Activated, act3Activated;// если true, то соответствующий контроллер был активирован
	
	// имитация таймера, в течении которого отображается сообщение об активации контроллера act1
    private: float act1Accum;// оставшееся время аккумулятора
	private: bool act1AccumEnabled;// если true, то аккумулятор работает
	private: float act1AccumFinishTimeInSec; // время работы аккумулятора в секундах

	// имитация таймера, в течении которого отображается сообщение об активации контроллера act2
    private: float act2Accum;// оставшееся время аккумулятора
	private: bool act2AccumEnabled;// если true, то аккумулятор работает
	private: float act2AccumFinishTimeInSec; // время работы аккумулятора в секундах

	// имитация таймера, в течении которого отображается сообщение об активации контроллера act3
    private: float act3Accum;// оставшееся время аккумулятора
	private: bool act3AccumEnabled;// если true, то аккумулятор работает
	private: float act3AccumFinishTimeInSec; // время работы аккумулятора в секундах


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), act1(NIL), act2(NIL), act3(NIL), act1Activated(false), act2Activated(false), act3Activated(false), act1Accum(0.0f), act1AccumEnabled(false), act1AccumFinishTimeInSec(0.8f), act2Accum(0.0f), act2AccumEnabled(false), act2AccumFinishTimeInSec(0.8f), act3Accum(0.0f), act3AccumEnabled(false), act3AccumFinishTimeInSec(0.8f), evt5(NIL), evt6(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete camera;

		delete font;

	    delete evt5;
		delete evt6;

		delete act1;
		delete act2;
		delete act3;


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
		
			{ 
				// действие sequence: A + S + Left click
				act1 = new PlayerControl();

				//act1->LoadFromFile("sequence");
				act1->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &Application::Act1Event));
				
			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->key = Keyboard::A;
				evt1->type = Keyboard::KEY_DOWN;
				act1->AddEvent(evt1);

							
				Keyboard::Event* evt2 = new Keyboard::Event();
				evt2->key = Keyboard::A;
				evt2->type = Keyboard::KEY_UP;
				act1->AddEvent(evt2);


				Keyboard::Event* evt3 = new Keyboard::Event();
				evt3->key = Keyboard::S;
				evt3->type = Keyboard::KEY_DOWN;
				act1->AddEvent(evt3);

						
				Keyboard::Event* evt4 = new Keyboard::Event();
				evt4->key = Keyboard::S;
				evt4->type = Keyboard::KEY_UP;
				act1->AddEvent(evt4);


				evt5 = new Mouse::Event();
				evt5->key = Mouse::LEFT_BUTTON;
				evt5->type = Mouse::KEY_DOWN;
				act1->AddEvent(evt5);

								
				evt6 = new Mouse::Event();
				evt6->key = Mouse::LEFT_BUTTON;
				evt6->type = Mouse::KEY_UP;
				act1->AddEvent(evt6);

				act1->SetTimingInSec(1.0f);// тайминг 1 секунда
				act1->Rename("sequence");
				act1->SetActivationMode(PlayerControl::SEQUENCE);
				scene->AddObject(act1);
				
				act1->SaveToFile();
			
				
			}

			{
				// действие combo: Q + W
				act2 = new PlayerControl();

				//act2->LoadFromFile("combo");
				act2->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &Application::Act2Event));

			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->key = Keyboard::Q;
				evt1->type = Keyboard::KEY_DOWN;
				act2->AddEvent(evt1);

				Keyboard::Event* evt2 = new Keyboard::Event();
				evt2->key = Keyboard::W;
				evt2->type = Keyboard::KEY_DOWN;
				act2->AddEvent(evt2);


				//act2->SetTiming(1.0f);// тайминг 1 секунда
				act2->Rename("combo");
				act2->SetActivationMode(PlayerControl::COMBO);
				scene->AddObject(act2);
				 
				act2->SaveToFile();
			}
				
			{
				// действие single: Space или Enter (альтернативное условие)
				act3 = new PlayerControl();

				//act3->LoadFromFile("single");
				act3->Connect(PlayerControl::INPUT_EVENT_MESSAGE, this, Caller<>(this, &Application::Act3Event));

		
				
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->key = Keyboard::SPACE;
				evt1->type = Keyboard::KEY_UP;
				act3->AddEvent(evt1);

				Keyboard::Event* evt2 = new Keyboard::Event();
				evt2->key = Keyboard::ENTER;
				evt2->type = Keyboard::KEY_PRESSED;
				act3->AddEvent(evt2);

				act3->Rename("single");
				act3->SetActivationMode(PlayerControl::SINGLE);
				scene->AddObject(act3);

				act3->SaveToFile();
			}


			// отклик клавиатуры
			//Keyboard().SetKeyPressedPeriodInSec(Keyboard().GetKeyPressedPeriodInSec() / 6.0f);
			
			// линии...
			GraphicsLine::_SetLineWidth(3.0f);// толщина линий


			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));

			MoveToCenter();// переместить окно в центр экрана
			//Mouse::_SetShowCursor(false);// скрыть курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}



	// реакция на активацию контроллера act1
	public: void Act1Event(PlayerControl* _handler)
	{
		act1Activated = true;
	}
	

	// реакция на активацию контроллера act2
	public: void Act2Event(PlayerControl* _handler)
	{
		act2Activated = true;
	}


	// реакция на активацию контроллера act3
	public: void Act3Event(PlayerControl* _handler)
	{
		act3Activated = true;
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
	

		if(act1Activated)
		{
			act1Activated = false;
						
			// запустить таймер
			act1Accum = act1AccumFinishTimeInSec;// время работы аккумулятора в секундах
			act1AccumEnabled = true; // включить аккумулятор
		}
			
		if(act2Activated)
		{
			act2Activated = false;
						
			// запустить таймер
			act2Accum = act2AccumFinishTimeInSec;// время работы аккумулятора в секундах
			act2AccumEnabled = true; // включить аккумулятор
		}

		if(act3Activated)
		{
			act3Activated = false;
			
			// запустить таймер
			act3Accum = act3AccumFinishTimeInSec;// время работы аккумулятора в секундах
			act3AccumEnabled = true; // включить аккумулятор
		}


		if(act1AccumEnabled) { act1Accum -= _delta; } // разрядить аккумулятор
		if(act1Accum > 0.0f)// пока аккумулятор не разряжен
		{
			font->DrawScreenText(Vector3(10, 70, -1), "%s", act1->GetName().c_str());
		}
		else if(act1AccumEnabled)// если аккумулятор работает
		{
			act1AccumEnabled = false;// выключить аккумулятор
		}


		if(act2AccumEnabled) { act2Accum -= _delta; } // разрядить аккумулятор
		if(act2Accum > 0.0f)// пока аккумулятор не разряжен
		{
			font->DrawScreenText(Vector3(10, 90, -1), "%s", act2->GetName().c_str());
		}
		else if(act2AccumEnabled)// если аккумулятор работает
		{
			act2AccumEnabled = false;// выключить аккумулятор
		}


		if(act3AccumEnabled) { act3Accum -= _delta; } // разрядить аккумулятор
		if(act3Accum > 0.0f)// пока аккумулятор не разряжен
		{
			font->DrawScreenText(Vector3(10, 110, -1), "%s", act3->GetName().c_str());
		}
		else if(act3AccumEnabled)// если аккумулятор работает
		{
			act3AccumEnabled = false;// выключить аккумулятор
		}

		font->DrawScreenText(Vector3(10, i+=20, -1), "%s timing: %f", act1->GetName().c_str(), act1->GetRemainedTimeInSec()); 
		font->DrawScreenText(Vector3(10, i+=20, -1), "%s timing: %f", act2->GetName().c_str(), act2->GetRemainedTimeInSec());

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
			else if(_event->key == Keyboard::DEL)
			{
				//act1->Destroy();
				delete evt5;
				act1->GetEvents().Remove(evt6, MagicContainer::REMOVE_NODE);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "PlayerControl.example_02";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
