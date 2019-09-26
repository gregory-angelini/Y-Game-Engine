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
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Helpers/Sprite/Sprite.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Trigger.h>




// класс, реализующий пользовательское окно
class Application: public Engine
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: Scene* scene;// сцена
	private: AsyncTimer timer;// синхронный таймер
	

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL)
	{}

			

	// удаляет пользовательские ресурсы
	public: virtual void DestroyUserResources(void)
	{
		delete scene;
	}


	// создает пользовательские ресурсы и возвращает true в случае успеха
	public: virtual bool CreateUserResources(void)
	{
		// сцена...
		{	
			scene = new Scene();
		}


		// шрифт...
		{
			Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

			Font2D::Define def;
			def.name = "font2d";
			def.fontName = "Haettenschweiler";
			def.fontSize = 20;
			def.textColor.SetRGBAi(RGBA::WHITE_COLOR);

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
			def.rotationEnabled = false;


			camera = new Camera3D();
			camera->Create(def);
			camera->Pick(Tittle()); 

			
			scene->AddObject(camera);
		}
		
		// таймер
		{
			timer.StartSec(3.0f);
		}


		//Mouse::_ShowCursor(false);// скрыть курсор
		return true;// вернуть успех
	}


		
	// возвращает режим таймера в виде символьной строки
	public: StringANSI NameOfMode(AsyncTimer::Mode _mode)
	{
		switch(_mode)
		{
			case AsyncTimer::SINGLE_SHOT:
			{
				return "SINGLE_SHOT";
			}
			case AsyncTimer::EVER_SHOT:
			{
				return "EVER_SHOT";
			}
			case AsyncTimer::INTERVAL:
			{
				return "INTERVAL";
			}
		}
		return "";
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

	
		// вывод текста...
		camera->BeginPerspective3D();// начать вывод двухмерной графики

		scene->Draw();
	
	
		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики

		int32 i = 0;
		
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.IsPaused: %d", timer.IsPaused()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.ElapsedTime: %.2f", timer.ElapsedTimeInSec()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.RemainedTime: %.2f", timer.RemainedTimeInSec()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.FinishTime: %.2f", timer.FinishTimeInSec()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.Mode: %s", NameOfMode(timer.CurrentMode()).c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "timer.Check: %d", timer.Check()); 
		
	}




	// фильтр событий мыши
	public: virtual bool MouseEventFilter(Mouse::Event _event)
	{
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
			else if(_event.key == Keyboard::P)
			{
				if(timer.IsPaused())
				{
					timer.Resume();
				}
				else 
				{
					timer.Pause();
				}
			}
			else if(_event.key == Keyboard::R)
			{
				timer.Restart();
			}
			else if(_event.key == Keyboard::C)
			{
				timer.Reset();
			}
			else if(_event.key == Keyboard::S)
			{
				timer.StartMs(1000);
			}
			else if(_event.key == Keyboard::NUM_1)
			{
				timer.SetMode(AsyncTimer::SINGLE_SHOT);
			}
			else if(_event.key == Keyboard::NUM_2)
			{
				timer.SetMode(AsyncTimer::EVER_SHOT);
			}
			else if(_event.key == Keyboard::NUM_3)
			{
				timer.SetMode(AsyncTimer::INTERVAL);
			}
			else if(_event.key == Keyboard::F)
			{
				timer.SetFinishTimeInSec(1.0f);
			}
			else if(_event.key == Keyboard::L)
			{
				AsyncTimer::Define def(4, 1, AsyncTimer::INTERVAL, false);
				timer.Create(def);
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
	Engine::Define def;
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "AsyncTimer.example_01";


	Application* app = new Application();// создать окно
	
	app->Create(def);

	app->Loop();

	delete app;
	return 0;
}
