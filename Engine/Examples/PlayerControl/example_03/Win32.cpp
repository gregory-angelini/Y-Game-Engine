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
class VirtualKey: public GraphicsSurface,// цветная полигональная сетка
	              public MessageHandler<int32>::Receiver// получатель сообщений
{
	// дружественные классы
	friend class VirtualKeyboard;


	// DATA
	private: Quad quad;// квад, описывающий контур клавиши
	private: bool activated;// если true, то виртуальная клавиша активирована
	public: Keyboard::Event event;// событие клавиатуры, ассоциированное с данной виртуальной клавишей
	public: bool isValid;// если true, то клавиша определена
    private: Camera3D* camera;// камера


	// METHODS
	public: VirtualKey(Camera3D* _camera): camera(_camera), activated(false), isValid(false)
	{
		SetColor(RGBA(RGBA::RED_COLOR));
	}


	// конструктор по умолчанию
	public: VirtualKey(void): activated(false), isValid(false)
	{
		SetColor(RGBA(RGBA::RED_COLOR));
	}


	// виртуальный деструктор
	public: virtual ~VirtualKey(void)
	{
		Destroying();
	}


	// возвращает true если виртуальная клавиша активирована
    public: bool IsActivated(void)const
	{
		return activated;
	}

			
	// обработчик событий ввода от мыши
	public: void MouseEvent(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
				Vector3 intersectedPoint;

				// если произошел клик по данной виртуальной клавише...
				if(IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(Renderer::_GetPicked())), intersectedPoint) != -1)
				{
					activated = true;
					SetColor(RGBA(RGBA::GREEN_COLOR));
				}
				else // иначе промах
				{ 
					activated = false; 
					SetColor(RGBA(RGBA::RED_COLOR));
				}
			}
		}
	}


	// обработчик событий ввода от клавиатуры
	public: void KeyboardEvent(Keyboard::Event _event)
	{
		if(event.ExactEquals(_event))
		{
			activated = true;
			SetColor(RGBA(RGBA::GREEN_COLOR));
		}
		else 
		{ 
			activated = false; 
			SetColor(RGBA(RGBA::RED_COLOR));
		}
	}


	// возвращает идентификатор виртуальной клавиши
	public: Keyboard::Key GetKey(void)const
	{
		return event.key; 
	}
		

	// возвращает true если клавиша определена
	public: bool IsExist(void)const
	{
		return isValid;
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
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(0)), GetTransformation().TransformVector(GetMesh()->GetVertex(1)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(1)), GetTransformation().TransformVector(GetMesh()->GetVertex(2)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(2)), GetTransformation().TransformVector(GetMesh()->GetVertex(3)), GetColor());
		GraphicsLine::_Draw(GetTransformation().TransformVector(GetMesh()->GetVertex(3)), GetTransformation().TransformVector(GetMesh()->GetVertex(0)), GetColor());
	}
};



// класс, реализующий виртуальную клавиатуру
class VirtualKeyboard: public TexturingSurface// текстурированная полигональная сетка 
{
	// DATA
	private: MagicList<VirtualKey> virtualKeys;// список виртуальных клавиш
	private: MagicList<Keyboard::Event> lastKeyboardEvent;// последнее произошедшее событие клавиатуры
    private: Camera3D* camera;// камера

	// загрузка ресурсов
    private: Texture2D* texture;// текстура клавиатуры


	// METHODS
	// конструктор по умолчанию
	public: VirtualKeyboard(Camera3D* _camera): camera(_camera)
	{
		// создать текстуру, имитирующую клавиатуру...
		texture = new Texture2D(); 
		texture->LoadFromFile("keyboard");

		BindTexture(texture);
		Mesh* mesh = new Mesh();
		mesh->CreateQuad2D(Quad(Vector3(), Vector2(1498, 458) / 400.0f));
		BindMesh(mesh);
	}


	// деструктор
	public: virtual ~VirtualKeyboard(void)
	{
		Destroying();
 
		virtualKeys.Clear(MagicContainer::DELETE_NODE);

		delete texture;
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
			if(it->GetNode()->IsActivated())
			{
				return it->GetNode();
			}
		}
		return NIL;
	}


	// добавляет новую виртуальную клавишу _virtualKey
	public: void AddVirtualKey(VirtualKey* _virtualKey)
	{
		virtualKeys.AddToTail(_virtualKey);// добавить в список квадов
	}


	// СОХРАНЕНИЕ И ЗАГРУЗКА...	
	// сохраняет виртуальную клавиатуру по пути _path, и возвращает true в случае успеха
	public: virtual bool SaveToFile(StringANSI _path = "")
	{
		File file(_path, File::REWRITE);

		file.Write(virtualKeys.GetSize());// количество виртуальных клавиш 

		// сохранить все виртуальные клавиши...
		for(int32 i = 0; i < virtualKeys.GetSize(); ++i)
		{ 
			VirtualKey* virtualKey = virtualKeys[i];

			// квад, описывающий форму клавиши
			file.Write(virtualKey->quad);

			virtualKey->event.SaveToFile(file);
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
			// количество виртуальных клавиш...
			int32 amountOfVirtualKeys = 0;
			file.Read(amountOfVirtualKeys);

			// выполнить для каждой виртуальной клавиши...
			for(int32 i = 0; i < amountOfVirtualKeys; ++i)
			{
				VirtualKey* virtualKey = new VirtualKey(camera);  
				Mesh* mesh = new Mesh();
				
				// квад, описывающий форму клавиши
				file.Read(virtualKey->quad);
				mesh->CreateQuad2D(virtualKey->quad);// создать фигуру
				virtualKey->BindMesh(mesh);

				virtualKey->event.LoadFromFile(file);

				virtualKey->isValid = true;
				Renderer::_GetPicked()->Connect(Window::KEYBOARD_KEY_DOWN_MESSAGE, virtualKey, Caller<>(virtualKey, &VirtualKey::KeyboardEvent));
				Renderer::_GetPicked()->Connect(Window::MOUSE_KEY_DOWN_MESSAGE, virtualKey, Caller<>(virtualKey, &VirtualKey::MouseEvent));
				
				virtualKeys.AddToTail(virtualKey);// добавить в список квадов
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
	private: VirtualKey* lastVirtualKey;// последняя созданная виртуальная клавиша
	private: Vector3 pointUnderCursor;// точка в мировом пространстве, на которую указывает курсор


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), virtualKeyboard(NIL), rightPressed(false), fantomQuad(NIL), fantomQuadDrawing(false), lastVirtualKey(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		virtualKeyboard->SaveToFile("keyboard.script"); 

		delete virtualKeyboard;
		delete fantomQuad;

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
				//scene->Pick();
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
				//font->Pick();// выбрать шрифт
			}


			// фантомный квад...
			{
				fantomQuad = new GraphicsQuad();
				fantomQuad->Rename("fantomQuad");
				fantomQuad->SetColor(RGBA(RGBA::GREEN_COLOR));// цвет квада
				scene->AddObject(fantomQuad);
			}



			// камера...
			{
				Camera3D::Define def;
			
				def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;// обзор объекта
				//def.viewingPoint = virtualKeyboard->GetPosition();
				def.zoomEnable = true;
				def.rotationEnable = false;
				def.yawClampEnable = true;
				def.pitchClampEnable = true;

				def.pitchMax = Angle::_DegreesToRadians(40);
				def.pitchMin = Angle::_DegreesToRadians(-40);
				def.yawMax = Angle::_DegreesToRadians(40);
				def.yawMin = Angle::_DegreesToRadians(-40);

				camera = new Camera3D();
				camera->Create(def);
				
				//camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);

				// задать минимальный и максимальный зум камеры
				camera->SetZoomNear(camera->GetZoomNear() * 10);
				camera->SetZoomFar(camera->GetZoomFar() * 2);
			}
	


			// виртуальная клавиатура...
			{
				virtualKeyboard = new VirtualKeyboard(camera); 
				virtualKeyboard->Rename("virtualKeyboard");
				
				scene->AddGraphicsObject(virtualKeyboard);
				
					
				// загрузить виртуальную клавиатуру из файла
				virtualKeyboard->LoadFromFile("keyboard.script");   


				// выполнить для кадой виртуальной клавиши...
				for(int32 i = 0; i < virtualKeyboard->GetVirtualKeys().GetSize(); ++i)
				{
					VirtualKey* it = virtualKeyboard->GetVirtualKeys()[i];
								
					scene->AddGraphicsObject(it);// добавить клавишу в графическую сцену
				}
			}


			camera->SetPosition(virtualKeyboard->GetPosition());



			// камера для двухмерной графики...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// отключить блокировку вращения вокруг оси X
				
				camera2d = new Camera3D();
				camera2d->Create(def);
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
		//Engine::Update(_delta);
		scene->Update();
	}



	// функция визуализации
	public: virtual void Draw(float _delta)
	{
		ClearScene();

		//Engine::Draw(_delta);
		scene->Draw();
		
		pointUnderCursor = camera->GetPointUnderPixel(Mouse::_GetCursorPosition(this));


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

		//font->DrawScreenText(Vector3(10, ClientAreaSize().y - (i+=20), -1), "amount of virtual keys: %d", virtualKeyboard->GetVirtualKeys().GetSize()); 

	
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
		camera->MouseEventFilter(_event);

		
		if(_event->type == Mouse::MOVE)// если мышь двигается
		{
			//if(_event->type == Mouse::KEY_PRESSED)
			{
				if(Mouse().IsKeyPressed(Mouse::RIGHT_BUTTON))// начало вращения камеры
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
				else if(Mouse().IsKeyPressed(Mouse::LEFT_BUTTON))// начало рисования грани полигона
				{
					if(!fantomQuadDrawing)// выполнить только один раз
					{
						// обновить фантомный квад... 
						scene->AddGraphicsObject(fantomQuad);// добавить в графическую сцену

						Vector3 pos = pointUnderCursor;//обновить координаты вершины
							
						fantomQuad->SetVertex4(Vector3(pos.x, pos.y, 0.003));// задать левую верхнюю вершину	
						fantomQuad->SetVertex2(Vector3(pos.x, pos.y, 0.003));// задать правую нижнюю вершину
						fantomQuadDrawing = true;
					} 
				} 
			}

			if(fantomQuadDrawing)// если рисуется квад
			{
				Vector3 pos = pointUnderCursor;//обновить координаты вершины

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
						lastVirtualKey = new VirtualKey(camera);
						Mesh* mesh = new Mesh();
						mesh->CreateQuad2D(*(static_cast<Quad*>(fantomQuad)));// создать фигуру
						lastVirtualKey->BindMesh(mesh); 

						lastVirtualKey->SetColor(RGBA(RGBA::RED_COLOR));// задать цвет  
						lastVirtualKey->SetQuad(*(static_cast<Quad*>(fantomQuad)));// запомнить размер


						if(mesh->IsClockwise())// если вершины расположены по часовой стрелке
						{
							mesh->InverseVertices();// обратить направление вершин
						}
							 
						scene->GetDrawList().AddObject(lastVirtualKey);// добавить виртуальную клавишу в список визуализации
					}
				}
			}
		}


		Mouse().ClearEventList();// очистить буфер событий
		return _event;
	}


	// обработка событий клавиатуры
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		//Engine::KeyboardEventFilter(_event);

		if(_event->type == Keyboard::KEY_DOWN)
		{	
			if(lastVirtualKey)// взять последнюю виртуальную клавишу из очереди
			{
				// назначить для виртуальной клавиши hot-key...
				if(lastVirtualKey && !lastVirtualKey->IsExist())// выполнить только один раз для одной и той же клавиши
				{ 
					virtualKeyboard->AddVirtualKey(lastVirtualKey);// добавить клавишу в состав виртуальной клавиатуры
					

					// если произошло событие playerControl...
					//    активировать данную виртуальную клавишу, задав ей цвет GREEN_COLOR.
                    // если было активировано другое событие (не playerControl)...
					//    деактивировать данную виртуальную клавишу, задав ей цвет RED_COLOR.
						
					// =альтернативный способ активации=
					// если произошел левый клик мыши...
					//    проверить столкновение данной виртуальной клавиши с курсором...
					//    если произошло столкновение, то имитируем событие _event->key & Keyboard::KEY_DOWN и _event->key & Keyboard::KEY_UP...


					lastVirtualKey->isValid = true;

					// запомнить событие ввода...
					lastVirtualKey->event.key = _event->key;
					lastVirtualKey->event.type = Keyboard::KEY_DOWN;
 
					Connect(KEYBOARD_KEY_DOWN_MESSAGE, lastVirtualKey, Caller<>(lastVirtualKey, &VirtualKey::KeyboardEvent));
					Connect(MOUSE_KEY_DOWN_MESSAGE, lastVirtualKey, Caller<>(lastVirtualKey, &VirtualKey::MouseEvent));
				}
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(1300, 760);// размер окна
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "Virtual Keyboard";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
