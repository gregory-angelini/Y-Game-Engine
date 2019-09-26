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



// класс, реализующий сообщение
class Message: public MagicNode// магический узел
{
	// дружественные классы
	friend class Chat;

	// DATA
	private: StringANSI text;// содержание сообщения

	// METHODS
    // инициализирующий конструктор
	public: Message(StringANSI _text): text(_text)
	{}


	// возвращает текст сообщения
	public: StringANSI GetText(void)const
	{
		return text;
	}
};



// класс, реализующий среду для беседы в текстовом формате
class Chat: public GraphicsObject,// графический объект
	        public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: MagicList<Message> history;// история сообщений в хронологическом порядке
	private: Quad borderRect;// границы (рамки) чата 
	private: RGBA borderColor;// цвет рамки чата
	private: int32 messageLimit;// максимальное количество видимых сообщений, которые могут быть отображены в чате
	//private: int32 borderWidth;// толщина рамки чата в пикселях
    private: MagicList<Message>::AddressDerived* begin;// сообщение, с которого начинается визуализация 
	private: Font2D *font;// шрифт, которым будет отображен текст
	private: StringANSI message;// сообщение пользователя
	private: AbstractObject* gregory;// пользователь Gregory

	// METHODS
	// конструктор без параметров
	public: Chat(void): borderColor(RGBA(RGBA::WHITE_COLOR)), messageLimit(19), /*borderWidth(5),*/ begin(NIL), font(NIL), gregory(NIL)
	{}


	// деструктор
	public: virtual ~Chat(void)
	{
		delete gregory;
	}


	// создает чат с цветом рамки _borderColor, толщиной рамки _borderSize, и лимитом видимых сообщений _messageLimit
	public: void Create(Font2D* _font, RGBA _borderColor, int32 _borderWidth)
	{
		borderRect = Quad(Vector3(350, 250, -1), Vector2(500, 400));
		borderColor = _borderColor;
		//borderWidth = _borderWidth;
		GraphicsLine::_SetLineWidth(_borderWidth);
		font = _font;

		// Gregory
		gregory = new AbstractObject();
		gregory->Rename("Gregory");
		AssetLibrary::_GetAssets().AddObject(gregory);

		// подписаться на прием сообщений от самого себя
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &Chat::SendMessageToChat));
	}


	// принимает ввод символа
	// примечание: _character должен содержать один символ
	public: void Put(StringANSI _character)
	{
		message += _character;
	}


    // завершает ввод сообщения
    public: void PutFinish(void)
	{
		if(!message.empty())// если строка не пуста
		{
			AddMessage(new Message("[Gregory]:" + message));
			gregory->SendPackage(MessageHandler<int32>::SEND_MESSAGE, message);// сообщить в чат и подписчикам Gregory о том, что Gregory послал сообщение message
			message.clear();
		}
	}

	// отменяет ввод последнего символа
	public: void PutUndo(void)
	{
		if(!message.empty())// если строка не пуста
		{ 
			message.resize(message.size() - 1); // удалить последний символ
		}
	}


			
	// посылает сообщение _message в чат
	private: void SendMessageToChat(StringANSI _message)
	{
		if(!_message.empty())// если строка не пуста
		{
			AddMessage(new Message(_message));// добавить сообщение в чат
		}
	}


	// добавляет сообщение _message в конец очереди
	public: void AddMessage(Message* _message)
	{
		if(begin == NIL)
		{
			begin = history.AddToTail(_message);
		}
		else 
		{
			// отображаем только последние 10 сообщений...
			MagicList<Message>::AddressDerived* it = history.AddToTail(_message);// добавляем в конец очереди
			
			if((it->GetIndex() - begin->GetIndex()) == messageLimit)
			{
				begin = begin->GetRight();
			}
		}
	}


	// визуализация чата
	public: virtual void Draw(void)
	{
		// отобразить рамку чата
		GraphicsLine::_Draw(borderRect.GetVertex4(), borderRect.GetVertex3(), borderColor);// верхняя грань
		GraphicsLine::_Draw(borderRect.GetVertex3(), borderRect.GetVertex2(), borderColor);// верхняя грань
		GraphicsLine::_Draw(borderRect.GetVertex2(), borderRect.GetVertex1(), borderColor);// нижняя грань
		GraphicsLine::_Draw(borderRect.GetVertex1(), borderRect.GetVertex4(), borderColor);// левая грань

		int32 i = 7; 

		// визуализировать все сообщения чата начиная с begin
		for(MagicList<Message>::AddressDerived* it = begin; it != NIL; it = it->GetRight())
		{
			font->DrawScreenText(Vector3(borderRect.GetVertex4().x + 10, borderRect.GetVertex1().y + (i+=20), -1), "%s", it->GetNode()->GetText().c_str()); 
		}

		font->DrawScreenText(Vector3(borderRect.GetVertex1().x + 10, borderRect.GetVertex4().y + 20, -1), "message: %s", message.c_str()); 
	}
};


// пользовательское приложение
class Application: public Engine,// движок
               	   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: Camera3D/* *camera,*/ *camera2d;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена
	private: Chat* chat;// чат


    // METHODS
    // инициализирующий конструктор
	public: Application(void): font(NIL), /*camera(NIL), */camera2d(NIL), scene(NIL), chat(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete font;
		delete chat;
		delete camera2d;
		//delete camera;
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
				scene->Rename("scene");// имя сцены
				AssetLibrary::_GetAssets().AddObject(scene);
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
				/*
				Camera3D::Define def;
				camera = new Camera3D();
				camera->Set(def);
				
				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
				*/ 
			}
		
		
			// камера для двухмерной графики...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				
				camera2d = new Camera3D();
				camera2d->Set(def);
				camera2d->Pick(); // выбрать камеру
				scene->BindCamera(camera2d);
			}
			
			// чат...
			{
				chat = new Chat();
				chat->Rename("Chat");
				chat->Create(font, RGBA(RGBA::WHITE_COLOR), 3);// создать чат
				
				//scene->AddObject(chat);
				scene->AddGraphicsObject(chat); 
				AssetLibrary::_GetAssets().AddObject(chat); 
			}


			{
				// John behaviour
				// первый рефлекс =========================================================
				Trigger* johnHelloReflex1 = new Trigger();
				johnHelloReflex1->Rename("john_gregory_hello_reflex1");
				johnHelloReflex1->SetEventExpr("(hello | gregoryHelloToJohn) & ~bye");
				johnHelloReflex1->SetActionExpr("johnHelloToGregory");
				

				// условия 
				AbstractEvent* helloEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Hello");
				helloEvent->Rename("hello"); 
				AssetLibrary::_GetAssets().AddObject(helloEvent);

			
				// условия
				AbstractEvent* gregoryHelloToJohnEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Hello John");
				gregoryHelloToJohnEvent->Rename("gregoryHelloToJohn");
				AssetLibrary::_GetAssets().AddObject(gregoryHelloToJohnEvent);
	
			
				// условия
				AbstractEvent* byeEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Bye");
				byeEvent->Rename("bye");
				AssetLibrary::_GetAssets().AddObject(byeEvent); 


				// действия
				AbstractAction* johnHelloToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[John]: Hello Gregory!");  
				johnHelloToGregoryAction->Rename("johnHelloToGregory"); 
				AssetLibrary::_GetAssets().AddObject(johnHelloToGregoryAction);
				
				
				AssetLibrary::_GetAssets().AddObject(johnHelloReflex1); 


				// второй рефлекс ==============================================
				Trigger* johnHelloReflex2 = new Trigger();
				johnHelloReflex2->Rename("john_gregory_hello_reflex2");
				johnHelloReflex2->SetEventExpr("(gregoryCallToJohn) & (~bye)");
				johnHelloReflex2->SetActionExpr("johnAnswerToGregory");
				

				// условия
				AbstractEvent* gregoryCallToJohnEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "John");
				gregoryCallToJohnEvent->Rename("gregoryCallToJohn");
				AssetLibrary::_GetAssets().AddObject(gregoryCallToJohnEvent);

			
				// действия
				AbstractAction* johnAnswerToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[John]: I'm here!"); 
				johnAnswerToGregoryAction->Rename("johnAnswerToGregory");
				AssetLibrary::_GetAssets().AddObject(johnAnswerToGregoryAction); 
				
		
				AssetLibrary::_GetAssets().AddObject(johnHelloReflex2);



				// третий рефлекс ==============================================
				Trigger* johnHelloReflex3 = new Trigger();
				johnHelloReflex3->Rename("john_gregory_bye_reflex");
				johnHelloReflex3->SetEventExpr("bye");
				johnHelloReflex3->SetActionExpr("johnByeToGregory");
				johnHelloReflex3->SetRepeatLimit(1);
				

				// действия 
				AbstractAction* johnByeToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[John]: Bye Gregory!");
				johnByeToGregoryAction->Rename("johnByeToGregory");
				AssetLibrary::_GetAssets().AddObject(johnByeToGregoryAction);
				

				AssetLibrary::_GetAssets().AddObject(johnHelloReflex3);
			}

			{
			
				// Marie behaviour
				// первый рефлекс ===============================================
				Trigger* marieHelloReflex1 = new Trigger();
				marieHelloReflex1->Rename("marie_gregory_hello_reflex");
				marieHelloReflex1->SetEventExpr("(hello ^ helloToMarie) & ~marieJealous & ~bye");
				marieHelloReflex1->SetActionExpr("marieHelloToGregory, marieSilence");// action не существует; комбинация действий; порядок перечисления имеет значение
				

  
				// условия
				AbstractEvent* helloToMarieEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Gregory", "Hello Marie");
				helloToMarieEvent->Rename("helloToMarie"); 
				AssetLibrary::_GetAssets().AddObject(helloToMarieEvent);
							

				// действия
				AbstractAction* marieHelloToGregoryAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[Marie]: Hello!"); 
				marieHelloToGregoryAction->Rename("marieHelloToGregory");
				AssetLibrary::_GetAssets().AddObject(marieHelloToGregoryAction);
				

				// действия 
				AbstractAction* marieSilenceAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "[Marie]: ..."); 
				marieSilenceAction->Rename("marieSilence"); 
				AssetLibrary::_GetAssets().AddObject(marieSilenceAction);


				AssetLibrary::_GetAssets().AddObject(marieHelloReflex1);


				// второй рефлекс =============================================================
				Trigger* marieHelloReflex2 = new Trigger();
				marieHelloReflex2->Rename("marieJealous");
				marieHelloReflex2->SetEventExpr("(gregoryCallToJohn | gregoryHelloToJohn)");  
				marieHelloReflex2->SetActionExpr("marieSilence");// использование действия в нескольких триггерах одновременно
				

				AssetLibrary::_GetAssets().AddObject(marieHelloReflex2);
			}


			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));

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
		//camera2d->BeginTransform(Renderer::PROJECTION);
		//camera2d->BeginTransform(Renderer::MODELVIEW);


	
		// вывод текста...
		int32 i = 0;
	

		//font->DrawScreenText(Vector3(10, i+=20, -1), "current mode"); 

	
		//font2d->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "current mode: %s", toolMode == EDIT ? "EDIT" : "READ"); 

	

	
		
		// восстановить предыдущую трансформацию...
		//camera2d->EndTransform(Renderer::PROJECTION);
		//camera2d->EndTransform(Renderer::MODELVIEW);
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
			else if(_event->key == Keyboard::BACKSPACE)
			{
				chat->PutUndo();// отменить последний введенный символ
			}
			else if(_event->key == Keyboard::ENTER)
			{
				chat->PutFinish();// очистить строку
			}
			else// иначе нажата любая другая клавиша
			{ 
				chat->Put(Keyboard::_FormatKeyName(_event));// взять имя последней нажатой клавиши
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);// размер окна
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_02";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
