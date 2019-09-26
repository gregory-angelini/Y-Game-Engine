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
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>
#include <Engine/Core/Parser/List/IntegerList.h> 
#include <Engine/Core/Parser/List/FloatList.h> 
#include <Engine/Core/Parser/List/BooleanList.h> 
#include <Engine/Core/Parser/List/StringList.h> 
#include <Engine/Core/Parser/List/Vector2List.h> 
#include <Engine/Core/Parser/List/Vector3List.h> 
#include <Engine/Core/Parser/List/QuaternionList.h> 
#include <Engine/Core/Parser/List/HMSList.h> 
#include <Engine/Core/Parser/List/RGBAList.h> 


// пользовательское приложение
class Application: public Engine,// движок
               	   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: Camera3D* camera2d;// камера
    private: Font2D* font;// шрифт
	private: Scene* scene;// сцена
	private: Trigger* trigger;// триггер
	private: AbstractObject* object;// объект

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera2d(NIL), font(NIL), scene(NIL), trigger(NIL), object(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete camera2d;
		delete font;
		delete scene;

		delete object;

		delete trigger;
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

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontWeight = 600;
				def.color = RGBA(RGBA::WHITE_COLOR);// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}

			// камера для двухмерной графики...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;


				camera2d = new Camera3D();
				camera2d->Create(def);
			}

			
			// тестовый объект...
			{
				object = new AbstractObject();
				object->Rename("unnamed");
				AssetLibrary::_GetAssets().AddObject(object);
			}


			// тестовый триггер...
			{
				trigger = new Trigger();


				trigger->SetActionExpr("act");
				trigger->SetEventExpr("");
			}


			// переменная 1...
			{
				Variable<StringANSI>* var = new Variable<StringANSI>("new_name_2");
				var->Rename("arg_var1");
				AssetLibrary::_GetAssets().AddObject(var);
			}

			// переменная 2...
			{
				Variable<StringANSI>* var = new Variable<StringANSI>("new_name_3");
				var->Rename("arg_var2");
				AssetLibrary::_GetAssets().AddObject(var);
			}


			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));


			MoveToCenter();// переместить окно в центр экрана
			//Mouse::_SetShowCursor(false);// скрыть курсор
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
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// вывод текста...
		int32 i = 0;


		font->DrawScreenText(Vector3(10, i+=20, -1), "object.name: %s", object->GetName().c_str());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "%f", camera->GetDistanceToViewingPoint());
	

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
			else if(_event->key == Keyboard::A)
			{
				delete AssetLibrary::_GetAssets().FindObject("act");

				AbstractAction* act = Trigger::_CreateAction(AbstractObject::CHANGE_NAME_MESSAGE);

				if(act)
				{
					act->Rename("act");
					act->SetStringKeyArg(AbstractAction::ARG1, "new_name_1");
					act->SetVariableArg(AbstractAction::TARGET, object->GetName());
					AssetLibrary::_GetAssets().AddObject(act);

					trigger->SetValue(true);
				}
			}
			else if(_event->key == Keyboard::B)
			{
				delete AssetLibrary::_GetAssets().FindObject("act");

				AbstractAction* act = Trigger::_CreateAction(AbstractObject::CHANGE_NAME_MESSAGE);

				if(act)
				{
					act->Rename("act");
					act->SetVariableArg(AbstractAction::ARG1, "arg_var1");
					act->SetVariableArg(AbstractAction::TARGET, object->GetName());
					AssetLibrary::_GetAssets().AddObject(act);

					trigger->SetValue(true);
				}
			}
			else if(_event->key == Keyboard::C)
			{
				delete AssetLibrary::_GetAssets().FindObject("act");

				AbstractAction* act = Trigger::_CreateAction(AbstractObject::CHANGE_NAME_MESSAGE);

				if(act)
				{
					act->Rename("act");
					act->SetTemplateArg(AbstractAction::ARG1, "$arg_var2");
					act->SetTemplateArg(AbstractAction::TARGET, StringANSI("$") + object->GetName());
					AssetLibrary::_GetAssets().AddObject(act);

					trigger->SetValue(true);
				}
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_01";
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
