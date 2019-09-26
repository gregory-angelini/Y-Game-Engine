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



// класс, реализующий квад в трехмерном пространстве
class GraphicsQuad: public GraphicsObject,// графический объект
	                public Quad// квад в трехмерном пространстве
{
	// METHODS
	// конструктор без параметров
    public: GraphicsQuad(void)
	{}

	// инициализирующий конструктор
	public: GraphicsQuad(Vector3 _leftBottom, Vector3 _rightTop): Quad(_leftBottom, _rightTop)
	{}


	// деструктор
    public: virtual ~GraphicsQuad(void)
	{
		Destroying();
	}


	// визуализация квада
	public: virtual void Draw(void)
	{
		GraphicsLine::_Draw(GetVertex1(), GetVertex2(), GetColor());
		GraphicsLine::_Draw(GetVertex2(), GetVertex3(), GetColor());
		GraphicsLine::_Draw(GetVertex3(), GetVertex4(), GetColor());
		GraphicsLine::_Draw(GetVertex4(), GetVertex1(), GetColor());
	}


	// СОХРАНЕНИЕ И ЗАГРУЗКА...
	// загружает объект из файла по пути _path, и возвращает true в случае успеха
	// примечание: если _auto равен true, то загрузка объекта производится по имени, из стандартной директории
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{
		return false;
	}
};



// класс, реализующий виртуальную клавишу
class VirtualKey: public ColoredMesh// цветная полигональная сетка
{
	// дружественные классы
	friend class VirtualKeyboard;


	// DATA
	public: PlayerControl* playerControl;// обработчик ввода с клавиатуры
	public: Trigger* trigger1;// триггер-ретранслятор событий ввода
    public: Trigger* trigger2;// триггер-рефлекс, активирующий клавишу
	public: Trigger* trigger3;// триггер-рефлекс, деактивирующий клавишу 
	public: Trigger* trigger4;// триггер, создающий состояние виртуальной клавиши
	public: Trigger* trigger5;// альтернативный способ активации клавиши
	public: Trigger* trigger6;
	private: Quad quad;// квад, описывающий контур клавиши



	// METHODS
	// конструктор по умолчанию
	public: VirtualKey(void)
	{
		SetColor(RGBA(RGBA::RED_COLOR));

		playerControl = new PlayerControl();
		trigger1 = new Trigger();
		trigger2 = new Trigger();
		trigger3 = new Trigger();
		trigger4 = new Trigger();
		trigger5 = new Trigger();
		trigger6 = new Trigger();
	}


	// виртуальный деструктор
	public: virtual ~VirtualKey(void)
	{
		Destroying();

		delete playerControl;
		delete trigger1;
		delete trigger2;
		delete trigger3;
		delete trigger4;
		delete trigger5;
		delete trigger6;
	}


	// возвращает идентификатор виртуальной клавиши
	public: Keyboard::Key GetKey(void)const
	{
		Keyboard::Event* event = dynamic_cast<Keyboard::Event*>(playerControl->GetEvents().GetHead());
		if(event) { return event->key; }
		return Keyboard::NO_KEY;
	}
		

	// возвращает true если клавиша определена
	public: bool IsExist(void)const
	{
		Keyboard::Event* event = dynamic_cast<Keyboard::Event*>(playerControl->GetEvents().GetHead());
		if(event) { return true; }
		return false;
	}


	// задает форму виртуальной клавише
	public: void SetQuad(Quad _quad)
	{
		quad = _quad;
	}

	// визуализация виртуальной клавиши
	public: virtual void Draw(void)
	{
		// учитываем модельно-видовую трансформацию...
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(0)), GetTransformation().TransformVector(GetVertex(1)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(1)), GetTransformation().TransformVector(GetVertex(2)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(2)), GetTransformation().TransformVector(GetVertex(3)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetVertex(3)), GetTransformation().TransformVector(GetVertex(0)), GetColor());
	}
};



// класс, реализующий виртуальную клавиатуру
class VirtualKeyboard: public TexturedMesh// текстурированная полигональная сетка 
{
	// DATA
	private: MagicList<VirtualKey> virtualKeys;// список виртуальных клавиш
	private: MagicList<Keyboard::Event> lastKeyboardEvent;// последнее произошедшее событие клавиатуры
	private: MagicList<AbstractObject> events; // список всех событий (условий)
	private: MagicList<AbstractObject> actions; // список всех действий
	
	// загрузка ресурсов
    private: TexturedAnimation* texture;// текстура клавиатуры
	private: Variable<int32>* keyCounter;// счетчик виртуальных клавиш



	// METHODS
	// конструктор по умолчанию
	public: VirtualKeyboard(void)
	{
		// создать текстуру, имитирующую клавиатуру...
		texture = new TexturedAnimation(); 
		texture->LoadFromFile("keyboard");

		BindAnimation(texture);
		CreateQuad2D(Quad(Vector3(), Vector2(1498, 458) / 400.0f));


		// переменные...
		{
			// счетчик виртуальных клавиш
			keyCounter = new Variable<int32>(0);
			keyCounter->Rename("keyCount");
			AssetLibrary::_GetAssets().AddObject(keyCounter);	
		}

		AssetLibrary::_GetAssets().Connect(AssetLibrary::CREATE_OBJECT_MESSAGE, this, Caller<>(this, &VirtualKeyboard::AddObjects));// подписаться на появление объектов	
	}


	// деструктор
	public: virtual ~VirtualKeyboard(void)
	{
		Destroying();
 
		virtualKeys.Clear(MagicContainer::DELETE_NODE);

		delete texture;
	}

	
	// реакция на создание новых действий и событий
	private: void AddObjects(AbstractObject* _object)
	{
		if(_object->IsPropertyExist(IEvent::EVENT_CLASS))
		{
			events.AddToTail(_object);
		}
		else if(_object->IsPropertyExist(IAction::ACTION_CLASS))
		{
			actions.AddToTail(_object);
		}
	}


	// возвращает последнюю добавленную виртуальную клавишу
	public: VirtualKey* GetLastVirtualKey(void)
	{
		return virtualKeys.GetTail();
	}


	// возвращает список всех виртуальных клавиш
	public: MagicList<VirtualKey>& GetVirtualKeys(void)
	{
		return virtualKeys;
	}


	// возвращает последнюю нажатую (выделенную) виртуальную клавишу
	public: VirtualKey* GetLastSelectedVirtualKey(void) 
	{ 
		MagicList<VirtualKey>::AddressDerived* it = virtualKeys.GetHeadAddress();

		// обойти все виртуальные клавиши
		for(MagicList<VirtualKey>::AddressDerived* it = virtualKeys.GetHeadAddress(); it != NIL; it = it->GetRight())
		{
			if(it->GetNode()->trigger2->GetValue())// если триггер активирован
			{
				return it->GetNode();
			}
		}
		return NIL;
	}


	// добавляет новую виртуальную клавишу _vKey
	public: void AddVirtualKey(VirtualKey* _vKey)
	{
		virtualKeys.AddToTail(_vKey);// добавить в список квадов
	}

		
	// возвращает индекс (порядковый номер) текущей клавиши
	public: int32 GetCurrentKeyIndex(void)const
	{
		return keyCounter->GetValue();
	}

	// увеличивает индекс (порядковый номер) текущей клавиши
	public: void UpdateCurrentKeyIndex(void)
	{
		keyCounter->SetValue(keyCounter->GetValue() + 1);
	}  


	// СОХРАНЕНИЕ И ЗАГРУЗКА...	
	// сохраняет виртуальную клавиатуру по пути _path, и возвращает true в случае успеха
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		File file(_path, File::REWRITE);

		// сохранить все события...
		file.Write(events.GetSize());// количество событий (условий)

		for(int32 i = 0; i < events.GetSize(); ++i)
		{
			events[i]->SaveToFile(file);
		}
		//events.Clear(MagicContainer::DELETE_NODE);

		// сохранить все действия...
		file.Write(actions.GetSize());// количество действий

		for(int32 i = 0; i < actions.GetSize(); ++i)
		{
			actions[i]->SaveToFile(file);
		}
		//actions.Clear(MagicContainer::DELETE_NODE);

		file.Write(virtualKeys.GetSize());// количество виртуальных клавиш 

		// сохранить все виртуальные клавиши...
		for(int32 i = 0; i < virtualKeys.GetSize(); ++i)
		{ 
			VirtualKey* vKey = virtualKeys[i];

			// имя виртуальной клавиши
			file.Write(vKey->GetName().size());
			file.WriteString(vKey->GetName());

			// квад, описывающий форму клавиши
			file.Write(vKey->quad);

			// обработчик ввода для клавиши
			file.Write(vKey->playerControl->IsExist()); 

			if(vKey->playerControl->IsExist())
			{ vKey->playerControl->SaveToFile(file); }

			// поведение клавиши 
			vKey->trigger1->SaveToFile(file);

			vKey->trigger2->SaveToFile(file);

			vKey->trigger3->SaveToFile(file);

			vKey->trigger4->SaveToFile(file);

			vKey->trigger5->SaveToFile(file);

			vKey->trigger6->SaveToFile(file);
		}
		return true;
	}


	// загружает виртуальную клавиатуру из файла по пути _path, и возвращает true в случае успеха
	// примечание: если контроллер уже определен, то вернется false
	// примечание: если _auto равен true, то загрузка контроллера производится по имени, из соответствующей директории
    public: virtual bool LoadFromFile(StringANSI _path, bool _auto = true)
	{ 
		File file(_path, File::READ);

		if(file.IsOpened())// если файл открыт
		{
			// загрузить все события...
			int32 amountOfEvents;// количество событий (условий)
			file.Read(amountOfEvents);

			// выполнить для каждого события...
			for(int32 i = 0; i < amountOfEvents; ++i)
			{
				IEvent* event = IEvent::_LoadFromFile(file);
				AssetLibrary::_GetAssets().AddObject(event);
			}

			// загрузить все действия...
			int32 amountOfActions;// количество действий
			file.Read(amountOfActions);
		
			// выполнить для каждого действия... 
			for(int32 i = 0; i < amountOfActions; ++i)
			{
				IAction* action = IAction::_LoadFromFile(file);
				AssetLibrary::_GetAssets().AddObject(action);
			}

			// количество виртуальных клавиш...
			int32 amountOfVirtualKeys = 0;
			file.Read(amountOfVirtualKeys);

			// выполнить для каждой виртуальной клавиши...
			for(int32 i = 0; i < amountOfVirtualKeys; ++i)
			{
				VirtualKey* vKey = new VirtualKey(); 

				// имя виртуальной клавиши
				StringANSI name;
				int32 length;
				file.Read(length);
				file.ReadString(name, length);
				vKey->Rename(name);

				// квад, описывающий форму клавиши
				file.Read(vKey->quad);
				vKey->CreateQuad2D(vKey->quad);// создать фигуру
										
				/* виртуальная клавиша должна логически существовать до того, как будут загружены ее триггеры,
			       т.к. некоторые из них (такие как дезактивация клавиши) будут выполнены сразу же */ 
				AssetLibrary::_GetAssets().AddObject(vKey);// добавить в библиотеку ассетов
				
				// обработчик ввода для клавиши
				bool exist; 
				file.Read(exist);
				if(exist) { vKey->playerControl->LoadFromFile(file); }

				// поведение клавиши
				vKey->trigger1->LoadFromFile(file);

				vKey->trigger2->LoadFromFile(file);

				vKey->trigger3->LoadFromFile(file);

				vKey->trigger4->LoadFromFile(file);
		
				vKey->trigger5->LoadFromFile(file);

				vKey->trigger6->LoadFromFile(file);

				virtualKeys.AddToTail(vKey);// добавить в список квадов

				AssetLibrary::_GetAssets().AddObject(vKey->playerControl); 
				AssetLibrary::_GetAssets().AddObject(vKey->trigger1); 
				AssetLibrary::_GetAssets().AddObject(vKey->trigger2); 
				AssetLibrary::_GetAssets().AddObject(vKey->trigger3);  
				AssetLibrary::_GetAssets().AddObject(vKey->trigger4);
				AssetLibrary::_GetAssets().AddObject(vKey->trigger5);
				AssetLibrary::_GetAssets().AddObject(vKey->trigger6);
			} 
		}
		return true;
	}
};



// пользовательское приложение
class Application: public Engine,// движок
               	   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
    private: Camera3D* camera, *camera2d;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена
	
	private: VirtualKeyboard* virtualKeyboard;// виртуальная клавиатура
	private: bool rightPressed;// если true, то правая кнопка мыши зажата
	private: GraphicsQuad* fantomQuad;// фантомный квад; используется для рисования квада
	private: bool fantomQuadDrawing; // если true, то фантомный квад наносится на экран
	private: PlayerControl* leftClickController;// обработчик события ввода
	private: VirtualKey* lastVirtualKey;// последняя созданная виртуальная клавиша
	


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), virtualKeyboard(NIL), rightPressed(false), fantomQuad(NIL), fantomQuadDrawing(false), leftClickController(NIL), lastVirtualKey(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		virtualKeyboard->SaveToFile("keyboard.script"); 

		delete virtualKeyboard;
		delete fantomQuad;
		delete leftClickController;

		delete camera;
		delete camera2d;

		delete font;

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

			// событие ввода...
			{
				leftClickController = new PlayerControl();
			
				Mouse::Event* evt1 = new Mouse::Event();
				evt1->key = Mouse::LEFT_BUTTON;
				evt1->type = Mouse::KEY_DOWN;
				leftClickController->AddEvent(evt1);
				
				leftClickController->Rename("leftClick");
				leftClickController->SetActivationMode(PlayerControl::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(leftClickController);
			}

			// фантомный квад...
			{
				fantomQuad = new GraphicsQuad();
				fantomQuad->Rename("fantomQuad");
				fantomQuad->SetColor(RGBA(RGBA::GREEN_COLOR));// цвет квада
				AssetLibrary::_GetAssets().AddObject(fantomQuad);
			}


			// виртуальная клавиатура...
			{
				virtualKeyboard = new VirtualKeyboard(); 
				virtualKeyboard->Rename("virtualKeyboard");
				
				scene->AddGraphicsObject(virtualKeyboard);
				AssetLibrary::_GetAssets().AddObject(virtualKeyboard);
					
				// загрузить виртуальную клавиатуру из файла
				virtualKeyboard->LoadFromFile("keyboard.script");   


				// выполнить для кадой виртуальной клавиши...
				for(int32 i = 0; i < virtualKeyboard->GetVirtualKeys().GetSize(); ++i)
				{
					VirtualKey* it = virtualKeyboard->GetVirtualKeys()[i];
								
					scene->AddGraphicsObject(it);// добавить клавишу в графическую сцену
				}
			}

			// камера...
			{
				Camera3D::Define def;
			
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;// обзор объекта
				def.viewingPoint = virtualKeyboard->GetPosition();
				def.rotationEnable = false;
				def.yawClampEnable = true;
				def.pitchClampEnable = true;
				def.zoomEnable = true;

				def.pitchMax = Angle::_DegreesToRadians(40);
				def.pitchMin = Angle::_DegreesToRadians(-40);
				def.yawMax = Angle::_DegreesToRadians(40);
				def.yawMin = Angle::_DegreesToRadians(-40);

				camera = new Camera3D();
				camera->Set(def);
				
				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);

				// задать минимальный и максимальный зум камеры
				camera->SetNearZoom(camera->GetNearZoom() * 10);
				camera->SetFarZoom(camera->GetFarZoom() * 2);
				
				AssetLibrary::_GetAssets().AddObject(camera);// добавить в сцену
			}
	

			// камера для двухмерной графики...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// отключить блокировку вращения вокруг оси X
				
				camera2d = new Camera3D();
				camera2d->Set(def);
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
	

		if(virtualKeyboard->GetLastSelectedVirtualKey())
		{
			font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "last key: %s", Keyboard::_GetKeyName(virtualKeyboard->GetLastSelectedVirtualKey()->GetKey()).c_str()); 
		}
		else 
		{ 
			font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "last key: ");
		}
		
	
		//font2d->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "current mode: %s", toolMode == EDIT ? "EDIT" : "READ"); 

		font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "amount of virtual keys: %d", virtualKeyboard->GetVirtualKeys().GetSize()); 

	
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
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);

		if(_event->type == Mouse::KEY_PRESSED)
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// начало вращения камеры
			{
				// разрешить вращение камеры только при зажатом правом клике

				if(!rightPressed)// выполняем только один раз
				{
					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// перемещаем мышь в центр клиентской области окна 
					camera->SetRotationEnable(true);// разрешить вращение камеры
					rightPressed = true;
					Mouse::_SetShowCursor(false);// скрыть курсор
				}
			}
			else if(_event->key == Mouse::LEFT_BUTTON)// начало рисования грани полигона
			{
				if(!fantomQuadDrawing)// выполнить только один раз
				{
					// обновить фантомный квад...
					scene->AddGraphicsObject(fantomQuad);// добавить в графическую сцену

					Vector3 pos = GetPointUnderCursor();//обновить координаты вершины
						
					fantomQuad->SetVertex4(Vector3(pos.x, pos.y, 0.003));// задать левую верхнюю вершину	
					fantomQuad->SetVertex2(Vector3(pos.x, pos.y, 0.003));// задать правую нижнюю вершину
					fantomQuadDrawing = true;
				} 
			} 
		}
		else if(_event->type == Mouse::MOVE)// если мышь двигается
		{
			if(fantomQuadDrawing)// если рисуется квад
			{
				Vector3 pos = GetPointUnderCursor();//обновить координаты вершины

				fantomQuad->SetVertex2(Vector3(pos.x, pos.y, 0.003));// задать правую нижнюю вершину
			}
		}
		else if(_event->type == Mouse::KEY_UP)// иначе кнопка отпущена
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// конец вращения камеры
			{
				if(rightPressed)// выполняем только один раз
				{
					camera->SetRotationEnable(false); // запретить вращение камеры
					rightPressed = false;
					Mouse::_SetShowCursor(true);// показать курсор
				}
			}
			else if(_event->key == Mouse::LEFT_BUTTON)// конец рисования грани полигона
			{
				// спрятать фантомный квад...
				if(fantomQuadDrawing) // если рисуется квад
				{
					scene->GetDrawList().Remove(fantomQuad);// убрать фантомный квад из графической сцены
					fantomQuadDrawing = false;
						
					if(fantomQuad->GetSize().x > 0.03f && fantomQuad->GetSize().y > 0.03f)// защита от создания квадов меньше 0.03 х 0.03
					{
						// защита от создания квадов без хот-кея...
						if(lastVirtualKey)// если существует хотя бы одна виртуальная клавиша
						{
							if(!lastVirtualKey->IsExist())// если последняя виртуальная клавиша не определена
							{
								delete lastVirtualKey;// удалить неполноценную виртуальную клавишу
								lastVirtualKey = NIL;
							}
						}

						// иначе клавиша определена...
						// создать реальный квад на базе фантомного...
						lastVirtualKey = new VirtualKey();
  
						lastVirtualKey->CreateQuad2D(*(static_cast<Quad*>(fantomQuad)));// создать фигуру
						lastVirtualKey->SetColor(RGBA(RGBA::RED_COLOR));// задать цвет 
						lastVirtualKey->SetQuad(*(static_cast<Quad*>(fantomQuad)));// запомнить размер

						if(lastVirtualKey->IsClockwise())// если вершины расположены по часовой стрелке
						{
							lastVirtualKey->InverseVertices();// обратить направление вершин
						}
							 
						scene->GetDrawList().AddObject(lastVirtualKey);// добавить виртуальную клавишу в список визуализации
					}
				}
			}
		}
		return _event;
	}


	// обработка событий клавиатуры
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		Engine::KeyboardEventFilter(_event);

		if(_event->type == Keyboard::KEY_DOWN)
		{	
			if(lastVirtualKey)// взять последнюю виртуальную клавишу из очереди
			{
				// назначить для виртуальной клавиши hot-key...
				if(lastVirtualKey && !lastVirtualKey->IsExist())// выполнить только один раз для одной и той же клавиши
				{ 
					lastVirtualKey->Rename("virtualKey_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// задать имя виртуальной клавише
					virtualKeyboard->AddVirtualKey(lastVirtualKey);// добавить клавишу в состав виртуальной клавиатуры
					AssetLibrary::_GetAssets().AddObject(lastVirtualKey);


					// создать обработчик ввода...
					Keyboard::Event* evt1 = new Keyboard::Event();
					evt1->key = _event->key;
					evt1->type = Keyboard::KEY_DOWN;
 
					lastVirtualKey->playerControl->AddEvent(evt1);// добавить событие ввода
					lastVirtualKey->playerControl->Rename("playerControl_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// переименовать обработчик
					lastVirtualKey->playerControl->SetActivationMode(PlayerControl::SINGLE);// режим ввода
					AssetLibrary::_GetAssets().AddObject(lastVirtualKey->playerControl);
                    
						
					
					// ИМИТАЦИЯ РЕЖИМА OVERLAP ДЛЯ СОБЫТИЙ, НЕ ПОДДЕРЖИВАЮЩИХ ТАКОГО РЕЖИМА...
					// создать ретранслятор...
					lastVirtualKey->trigger1->Rename("trigger1_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// задать имя триггеру
					lastVirtualKey->trigger1->SetActionExpr("inputEventMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// реакции


					// создать условие
					IEvent* event1 = Trigger::_CreateEvent(PlayerControl::INPUT_EVENT_MESSAGE, lastVirtualKey->playerControl->GetName());
					event1->Rename("inputEvent" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(event1);

					
					// создать действие
					//IAction* action1 = Trigger::_CreateTemplateAction(IVariable::CREATE_VARIABLE_MESSAGE, "#var" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()), IVariable::_GetValueType<StringANSI>(), event1->GetName());
					//action1->Rename("createVariable" + NumberToString(virtualKeyboard->GetCurrentKeyIndex())); 
					//AssetLibrary::_GetAssets().AddObject(action1);

					
					// создать действие
					IAction* action2 = Trigger::_CreateAction(MessageHandler<int32>::SEND_MESSAGE, virtualKeyboard->GetName(), event1->GetName()); 
					action2->Rename("inputEventMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action2);

					lastVirtualKey->trigger1->SetEventExpr("inputEvent" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// условия активации

					
				
					
					// активировать клавишу
					lastVirtualKey->trigger2->Rename("trigger2_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger2->SetActionExpr("enableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// реакции


					// создать условие  
					IEvent* event2 = Trigger::_CreateEvent(MessageHandler<int32>::SEND_MESSAGE, virtualKeyboard->GetName(), event1->GetName());
					event2->SetOverlappingEnable(true);// режим перекрытия
					event2->Rename("inputMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(event2);

 
					// создать действие
					IAction* action3 = Trigger::_CreateAction(Color::CHANGE_COLOR_MESSAGE, lastVirtualKey->GetName(), RGBA(RGBA::GREEN_COLOR));
					action3->Rename("enableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex())); 
					AssetLibrary::_GetAssets().AddObject(action3);
					
					lastVirtualKey->trigger2->SetEventExpr("inputMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// условия активации
				
					
					
					// деактивировать клавишу...
					lastVirtualKey->trigger3->Rename("trigger3_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger3->SetActionExpr("disableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// реакции

					// создать действие
					IAction* action4 = Trigger::_CreateAction(Color::CHANGE_COLOR_MESSAGE, lastVirtualKey->GetName(), RGBA(RGBA::RED_COLOR));
					action4->Rename("disableColor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action4);

					lastVirtualKey->trigger3->SetEventExpr("~inputMessage" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// условия активации
					
					


					// создание переменной, отображающей состояние виртуальной клавиши...
					lastVirtualKey->trigger4->Rename("trigger4_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger4->SetActionExpr("createUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// реакции

					// создать действие 
					IAction* action5 = Trigger::_CreateAction(IVariable::CREATE_VARIABLE_MESSAGE, StringANSI("underCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex())), IVariable::_GetValueType<bool>(), false);
					action5->Rename("createUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action5);
									
					lastVirtualKey->trigger4->SetEventExpr("");// условия активации
  

					

					// альтернативный способ активации клавиши
					lastVirtualKey->trigger5->Rename("trigger5_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger5->SetActionExpr("checkUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// реакции


					// создать условие
					IEvent* event3 = Trigger::_CreateEvent(PlayerControl::INPUT_EVENT_MESSAGE, leftClickController->GetName());// клик мышкой
					event3->Rename("mouseClick" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(event3);

					
					// создать действие
					IAction* action6 = Trigger::_CreateAction(AbstractSurface::CHECK_SURFACE_UNDER_CURSOR_MESSAGE, lastVirtualKey->GetName(), "underCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()), "", "");
					action6->Rename("checkUnderCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action6);

					lastVirtualKey->trigger5->SetEventExpr("mouseClick" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// условия активации
					 


					// альтернативный способ выбора виртуальной клавиши...
					lastVirtualKey->trigger6->Rename("trigger6_" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					lastVirtualKey->trigger6->SetActionExpr("createInputEvent1" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()) + ", createInputEvent2" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// реакции

					
					// создать действие
					IAction* action7 = Trigger::_CreateAction(Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE, _event->key, Keyboard::KEY_DOWN);// сгенерировать событие клавиатуры
					action7->Rename("createInputEvent1" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action7);
	

					// создать действие
					IAction* action8 = Trigger::_CreateAction(Keyboard::CREATE_KEYBOARD_EVENT_MESSAGE, _event->key, Keyboard::KEY_UP);// сгенерировать событие клавиатуры
					action8->Rename("createInputEvent2" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));
					AssetLibrary::_GetAssets().AddObject(action8);
						
					lastVirtualKey->trigger6->SetEventExpr("underCursor" + NumberToString(virtualKeyboard->GetCurrentKeyIndex()));// условия активации
				

					virtualKeyboard->UpdateCurrentKeyIndex();// увеличить счетчик клавиш		
				}
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(1200, 760);// размер окна
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Trigger.example_04";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
