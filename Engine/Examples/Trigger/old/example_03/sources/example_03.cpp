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

	// METHODS
	// конструктор без параметров
	public: Chat(void): borderColor(RGBA(RGBA::WHITE_COLOR)), messageLimit(19), /*borderWidth(5),*/ begin(NIL), font(NIL)
	{}


	// деструктор
	public: virtual ~Chat(void)
	{}


	// создает чат с цветом рамки _borderColor, толщиной рамки _borderSize, и лимитом видимых сообщений _messageLimit
	public: void Create(Font2D* _font, RGBA _borderColor, int32 _borderWidth)
	{
		borderRect = Quad(Vector3(350, 250, 0), Vector2(500, 400));
		borderColor = _borderColor;
		//borderWidth = _borderWidth;
		GraphicsLine::_SetLineWidth(_borderWidth);
		font = _font;

		// подписаться на прием сообщений от самого себя
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &Chat::SendMessageToChat));
	}


	// принимает ввод символа
	// примечание: _character должен содержать один символ
	public: void Put(StringANSI _character)
	{
		message += _character;
	}


	// посылает сообщение _message в чат
	private: void SendMessageToChat(StringANSI _message)
	{
		if(!_message.empty())// если строка не пуста
		{
			AddMessage(new Message(_message));// добавить сообщение в чат
		}
	}


    // завершает ввод сообщения
    public: void PutFinish(void)
	{
		if(!message.empty())// если строка не пуста
		{
			AddMessage(new Message(message));
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

		Disconnect(MessageHandler<int32>::SEND_MESSAGE, this);// отписаться от себя чтобы избежать бесконечного цикла при отправке сообщения другим
		SendPackage(MessageHandler<int32>::SEND_MESSAGE, _message->GetText());// сообщить всем что Chat послал сообщение message
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &Chat::SendMessageToChat)); // подписаться на себя чтобы визуализировать сообщения от других подписчиков
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



// класс, реализующий генератор гоблинов
class GoblinGenerator: public AbstractObject,// базовые свойства объекта
	                   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// METHODS
	// конструктор без параметров
	public: GoblinGenerator(void)
	{
		// создать переменные...
		Variable<bool>* goblinExistVariable = new Variable<bool>(false);
		goblinExistVariable->Rename("goblinExist");
		AssetLibrary::_GetAssets().AddObject(goblinExistVariable);


		Variable<bool>* falseVariable = new Variable<bool>(false);
		falseVariable->Rename("FALSE");
		AssetLibrary::_GetAssets().AddObject(falseVariable);

	
		AbstractObject* currentTimer = new AbstractObject();
		currentTimer->Rename("CurrentTimer");
		AssetLibrary::_GetAssets().AddObject(currentTimer);


		{
			// старт системы...
			Trigger* goblinMessenger1 = new Trigger();
			goblinMessenger1->Rename("goblin_messenger1");
			goblinMessenger1->SetActionExpr("restart1, restart2, restart3");
				
	
			// действия
			IAction* restart1Action = Trigger::_CreateAction(TimingHandler::CREATE_TIMER_MESSAGE, "startTimer");
			restart1Action->Rename("restart1");
			AssetLibrary::_GetAssets().AddObject(restart1Action); 


			// действия
			IAction* restart2Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE, "startTimer", 4.0f);
			restart2Action->Rename("restart2");
			AssetLibrary::_GetAssets().AddObject(restart2Action);


			// действия
			IAction* restart3Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, "startTimer", PlayingObject::PLAY);
			restart3Action->Rename("restart3");
			AssetLibrary::_GetAssets().AddObject(restart3Action);



			goblinMessenger1->SetEventExpr("");
			
			AssetLibrary::_GetAssets().AddObject(goblinMessenger1);
		}

		
		{
			/* имитирует режим перекрытия для события TIMER_FINISH */ 
			// оповещение о приближении гоблина...
			Trigger* tr1 = new Trigger();
			tr1->Rename("tr1");
			tr1->SetActionExpr("startTimerSend");
		
			
			// условия 
			IEvent* startTimerIsFinishEvent = Trigger::_CreateEvent(Timer::TIMER_FINISH_MESSAGE, "startTimer");
			startTimerIsFinishEvent->Rename("startTimerIsFinish");
			AssetLibrary::_GetAssets().AddObject(startTimerIsFinishEvent);


			// действия
			IAction* disableStartTimerAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "startTimer"); 
			disableStartTimerAction->Rename("startTimerSend");
			AssetLibrary::_GetAssets().AddObject(disableStartTimerAction);
			

			tr1->SetEventExpr("startTimerIsFinish");   

			AssetLibrary::_GetAssets().AddObject(tr1);  
		}



		{
			// оповещение о приближении гоблина...
			Trigger* goblinMessenger2 = new Trigger();
			goblinMessenger2->Rename("goblin_messenger2");
			goblinMessenger2->SetActionExpr("goblinWarning1Message, goblinPrepareToCreate1, goblinPrepareToCreate2, goblinPrepareToCreate3");
		

			// действия
			IAction* goblinWarning1MessageAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is coming..."); 
			goblinWarning1MessageAction->Rename("goblinWarning1Message");
			AssetLibrary::_GetAssets().AddObject(goblinWarning1MessageAction);	

			
			// действия
			IAction* goblinPrepareToCreate1Action = Trigger::_CreateAction(TimingHandler::CREATE_TIMER_MESSAGE, "createGoblinTimer");
			goblinPrepareToCreate1Action->Rename("goblinPrepareToCreate1");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToCreate1Action); 


			// действия 
			IAction* goblinPrepareToCreate2Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE, "createGoblinTimer", 4.0f);
			goblinPrepareToCreate2Action->Rename("goblinPrepareToCreate2");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToCreate2Action);	
	
				
			// действия 
			IAction* goblinPrepareToCreate3Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, "createGoblinTimer", Timer::PLAY);
			goblinPrepareToCreate3Action->Rename("goblinPrepareToCreate3");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToCreate3Action);	
	

			// условия
			IEvent* startTimerMessageEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "startTimer");// перекрытие позволит деактивировать данное событие при активации других событий чата вроде "гоблин ушел, гоблин приближается" и т.д. что необходимо для того, чтобы триггер goblin_messenger8 не ожидал пока гоблин появится чтобы убить его
			startTimerMessageEvent->SetOverlappingEnable(true);
			startTimerMessageEvent->Rename("startTimerMessage");
			AssetLibrary::_GetAssets().AddObject(startTimerMessageEvent);


			goblinMessenger2->SetEventExpr("~goblinExist & startTimerMessage");

			AssetLibrary::_GetAssets().AddObject(goblinMessenger2);  
		} 


		{
			/* имитирует режим перекрытия для события TIMER_FINISH */ 
			// оповещение о приближении гоблина...
			Trigger* tr2 = new Trigger();
			tr2->Rename("tr2");
			tr2->SetActionExpr("createGoblinTimerSend");
		
			
			// условия 
			IEvent* startTimerIsFinishEvent = Trigger::_CreateEvent(Timer::TIMER_FINISH_MESSAGE, "createGoblinTimer");
			startTimerIsFinishEvent->Rename("createGoblinTimerIsFinish");
			AssetLibrary::_GetAssets().AddObject(startTimerIsFinishEvent);


			// действия
			IAction* disableStartTimerAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "createGoblinTimer"); 
			disableStartTimerAction->Rename("createGoblinTimerSend");
			AssetLibrary::_GetAssets().AddObject(disableStartTimerAction);
			

			tr2->SetEventExpr("createGoblinTimerIsFinish");   

			AssetLibrary::_GetAssets().AddObject(tr2);  
		}

		{
			// создание гоблина ...
			Trigger* goblinMessenger3 = new Trigger();
			goblinMessenger3->Rename("goblin_messenger3"); 
			goblinMessenger3->SetActionExpr("createGoblin, checkGoblinExist"); 
				

			// действия
			IAction* createGoblinAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "GoblinGenerator", "goblin");
			createGoblinAction->Rename("createGoblin");   
			AssetLibrary::_GetAssets().AddObject(createGoblinAction);


			// действия
			IAction* checkGoblinExistAction = Trigger::_CreateAction(AssetLibrary::CHECK_OBJECT_EXIST_MESSAGE, "goblin", "goblinExist");
			checkGoblinExistAction->Rename("checkGoblinExist"); 
			AssetLibrary::_GetAssets().AddObject(checkGoblinExistAction);

						
			// условия
			IEvent* createGoblinTimerMessageEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "createGoblinTimer");
			createGoblinTimerMessageEvent->SetOverlappingEnable(true);
			createGoblinTimerMessageEvent->Rename("createGoblinTimerMessage");
			AssetLibrary::_GetAssets().AddObject(createGoblinTimerMessageEvent);


			goblinMessenger3->SetEventExpr("createGoblinTimerMessage");  


			AssetLibrary::_GetAssets().AddObject(goblinMessenger3);
		}


		{
			// оповещение о создании гоблина...
			Trigger* goblinMessenger4 = new Trigger();
			goblinMessenger4->Rename("goblin_messenger4");
			goblinMessenger4->SetActionExpr("goblinWarning2Message, goblinPrepareToGone1, goblinPrepareToGone2, goblinPrepareToGone3");
				

			// действия
			IAction* goblinWarning2MessageAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is blocking your way...");  
			goblinWarning2MessageAction->Rename("goblinWarning2Message");
			AssetLibrary::_GetAssets().AddObject(goblinWarning2MessageAction);
			

			// действия
			IAction* goblinPrepareToGone1Action = Trigger::_CreateAction(TimingHandler::CREATE_TIMER_MESSAGE, "leaveGoblinTimer");
			goblinPrepareToGone1Action->Rename("goblinPrepareToGone1");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToGone1Action);


			// действия  
			IAction* goblinPrepareToGone2Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_DURATION_IN_SEC_MESSAGE, "leaveGoblinTimer", 4.0f); 
			goblinPrepareToGone2Action->Rename("goblinPrepareToGone2");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToGone2Action);	


			// действия  
			IAction* goblinPrepareToGone3Action = Trigger::_CreateAction(PlayingObject::CHANGE_PLAYING_STATE_MESSAGE, "leaveGoblinTimer", Timer::PLAY); 
			goblinPrepareToGone3Action->Rename("goblinPrepareToGone3");
			AssetLibrary::_GetAssets().AddObject(goblinPrepareToGone3Action);	

				
			// условия 
			IEvent* goblinIsCreatedEvent = Trigger::_CreateEvent(AssetLibrary::CREATE_OBJECT_MESSAGE, "goblin"); 
			goblinIsCreatedEvent->Rename("goblinIsCreated");
			AssetLibrary::_GetAssets().AddObject(goblinIsCreatedEvent); 
			


			goblinMessenger4->SetEventExpr("goblinIsCreated");


			
			AssetLibrary::_GetAssets().AddObject(goblinMessenger4);
		} 
	


		{
			/* имитирует режим перекрытия для события TIMER_FINISH */ 
			// оповещение о приближении гоблина...
			Trigger* tr3 = new Trigger();
			tr3->Rename("tr3");
			tr3->SetActionExpr("leaveGoblinTimerSend");
		
			
			// условия 
			IEvent* startTimerIsFinishEvent = Trigger::_CreateEvent(Timer::TIMER_FINISH_MESSAGE, "leaveGoblinTimer");
			startTimerIsFinishEvent->Rename("leaveGoblinTimerIsFinish");
			AssetLibrary::_GetAssets().AddObject(startTimerIsFinishEvent);


			// действия
			IAction* disableStartTimerAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "leaveGoblinTimer"); 
			disableStartTimerAction->Rename("leaveGoblinTimerSend");
			AssetLibrary::_GetAssets().AddObject(disableStartTimerAction);
			

			tr3->SetEventExpr("leaveGoblinTimerIsFinish");   

			AssetLibrary::_GetAssets().AddObject(tr3);  
		}

		{
			// подготовка к уходу гоблина...
			Trigger* goblinMessenger5 = new Trigger();
			goblinMessenger5->Rename("goblin_messenger5");  
				

			// условия
			IEvent* leaveGoblinTimerMessageEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "CurrentTimer", "leaveGoblinTimer");
			leaveGoblinTimerMessageEvent->SetOverlappingEnable(true);
			leaveGoblinTimerMessageEvent->Rename("leaveGoblinTimerMessage");
			AssetLibrary::_GetAssets().AddObject(leaveGoblinTimerMessageEvent);


			goblinMessenger5->SetEventExpr("leaveGoblinTimerMessage & goblinExist");
	
	

			AssetLibrary::_GetAssets().AddObject(goblinMessenger5);
		}



		{
			// оповещение об уходе гоблине...
			Trigger* goblinMessenger6 = new Trigger();
			goblinMessenger6->Rename("goblin_messenger6");  
			goblinMessenger6->SetActionExpr("goblinGoneMessage, destroyGoblin");    
				
			
 
			// действия 
			IAction* goblinGoneMessageAction = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is gone...");
			goblinGoneMessageAction->Rename("goblinGoneMessage");
			AssetLibrary::_GetAssets().AddObject(goblinGoneMessageAction);

				
			// действия
			IAction* destroyGoblinAction = Trigger::_CreateAction(AbstractObject::DESTROY_OBJECT_MESSAGE, "goblin");
			destroyGoblinAction->Rename("destroyGoblin");
			AssetLibrary::_GetAssets().AddObject(destroyGoblinAction);


			goblinMessenger6->SetEventExpr("goblin_messenger5"); 



			AssetLibrary::_GetAssets().AddObject(goblinMessenger6);
		}

		{
			// оповещение об убийстве гоблине...
			Trigger* goblinMessenger8 = new Trigger();
			goblinMessenger8->Rename("goblin_messenger8");
			goblinMessenger8->SetActionExpr("rubbush, killGoblinMessage, destroyGoblin");// действие rubbish не существует, однако это не помешает выполнению остальных действий триггера

				
			
			// действия
			IAction* killGoblinMessageMessage = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, "Chat", "goblin is dead...");
			killGoblinMessageMessage->Rename("killGoblinMessage");
			AssetLibrary::_GetAssets().AddObject(killGoblinMessageMessage);

						
			// условия
			IEvent* killGoblinEvent = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, "Chat", "kill goblin");// перекрытие позволит деактивировать данное событие при активации других событий чата вроде "гоблин ушел, гоблин приближается" и т.д. что необходимо для того, чтобы триггер goblin_messenger8 не ожидал пока гоблин появится чтобы убить его
			killGoblinEvent->SetOverlappingEnable(true);
			killGoblinEvent->Rename("killGoblin");
			AssetLibrary::_GetAssets().AddObject(killGoblinEvent);


			goblinMessenger8->SetEventExpr("killGoblin & goblinExist");  


			AssetLibrary::_GetAssets().AddObject(goblinMessenger8);	
		}



		{
			// перезагрузка системы...
			Trigger* goblinMessenger7 = new Trigger();
			goblinMessenger7->Rename("goblin_messenger7");
			goblinMessenger7->SetActionExpr("checkGoblinExist, restart1, restart2, restart3"); 
			
		
			// условия
			IEvent* goblinIsDeadEvent = Trigger::_CreateEvent(AbstractObject::DESTROY_OBJECT_MESSAGE, "goblin");
			goblinIsDeadEvent->Rename("goblinIsDead");
			AssetLibrary::_GetAssets().AddObject(goblinIsDeadEvent); 

			goblinMessenger7->SetEventExpr("goblinIsDead");

		 
			AssetLibrary::_GetAssets().AddObject(goblinMessenger7); 
		}


        // подписаться на самого себя
		Connect(MessageHandler<int32>::SEND_MESSAGE, this, Caller<>(this, &GoblinGenerator::CreateGoblin));
	} 
	


	// создает гоблина с уникальным именем и помещает его в библиотеку ассетов
	public: void CreateGoblin(StringANSI _name)
	{
		AbstractObject* goblin = new AbstractObject();
		goblin->Rename(_name);// задать объекту имя
		AssetLibrary::_GetAssets().AddObject(goblin);
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
	private: GoblinGenerator* goblinGenerator;// генератор гоблинов


    // METHODS
    // инициализирующий конструктор
	public: Application(void): font(NIL), /*camera(NIL), */camera2d(NIL), scene(NIL), chat(NIL), goblinGenerator(NIL)
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

			// генератор гоблинов...
			{
				goblinGenerator = new GoblinGenerator();
				goblinGenerator->Rename("GoblinGenerator");
				AssetLibrary::_GetAssets().AddObject(goblinGenerator); 
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_03";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
