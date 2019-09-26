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
#include <Engine/Core/Time/Timer/SyncTimer/SyncTimer.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>


// реализует графический прямоугольник
class GraphicsQuad: public GraphicsObject// графический объект
{
	// DATA
	private: Quad quad;// прямоугольник, описывающий координаты и размер квада


	// METHODS
	// конструктор по умолчанию
	public: GraphicsQuad(void)
	{}


	// возвращает ссылку на описание квада
	public: Quad& GetQuad(void)
	{
		return quad;
	}



	// ВИЗУАЛИЗАЦИЯ...
	// визуализирует квад
	public: virtual void Draw(void)
	{
		if(IsShowEnabled())
		{
			// нижняя горизонтальная линия
			GraphicsLine::_Draw(quad.GetVertex4(), quad.GetVertex3(), GetColor());// рисуем слева направо

			// верхняя горизонтальная линия
			GraphicsLine::_Draw(quad.GetVertex3(), quad.GetVertex2(), GetColor());// рисуем слева направо

			// левая вертикальная линия
			GraphicsLine::_Draw(quad.GetVertex2(), quad.GetVertex1(), GetColor());// рисуем сверху вниз

			// правая вертикальная линия
			GraphicsLine::_Draw(quad.GetVertex1(), quad.GetVertex4(), GetColor());// рисуем сверху вниз
		}
	}
};



// класс, реализующий движущийся объект
class MovingObjectEx: public TexturedSprite,// текстурированный спрайт
	                  public VectorLerp,// векторный интерполятор
					  public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: float currentSpeed;// текущая скорость 
	private: float maxSpeed;// максимальная скорость


	// METHODS
	// конструктор без параметров
	public: MovingObjectEx(void): currentSpeed(0.0f), maxSpeed(0.0f)
	{}


	// СКОРОСТЬ ОБЪЕКТА...
	// возвращает текущую скорость объекта
	public: float GetCurrentSpeed(void)
	{
		return currentSpeed;
	}


	// возвращает максимальную скорость объекта
	public: float GetMaxSpeed(void)
	{
		return maxSpeed;
	}


	// реакция на вычисление новой целевой позиции
	public: void UpdateTargetPosition(Vector3D _position)
	{
		currentSpeed = (_position - GetPosition()).Length() * (1.0f / Renderer::_Picked()->GetDeltaTimeInSec());

		if(currentSpeed > maxSpeed)
		{
			maxSpeed = currentSpeed;
		}
	}


	// задает целевую позицию для объекта
	// примечание: список контрольных вершин будет обновлен
	// примечание: movingEnable будет включен
	public: void SetTargetPosition(Vector3D _target)
	{
		// траектория движения к цели будет представлять собой прямую от текущей позиции к целевой
		GetControlVertices().Clear();// очистить список контрольных вершин
		GetControlVertices().AddToTail(GetPosition());
		GetControlVertices().AddToTail(_target);
		SetTarget((GetTargetPosition() - GetPosition()).Length()); // использовать расстояние до цели как целевую позицию числового интерполятора
		Start();// начать движение к цели
	}

	
	// возвращает целевую позицию объекта
	public: Vector3D GetTargetPosition(void)
	{
		if(GetControlVertices().IsNotEmpty())
		{
			return GetControlVertices().GetTail()->key;
		}
		return Vector3D();
	}
	

	// возвращает стартовую позицию объекта
	public: Vector3D GetStartPosition(void)
	{
		if(GetControlVertices().IsNotEmpty())
		{
			return GetControlVertices().GetHead()->key;
		}
		return Vector3D();
	}

	// ОБНОВЛЕНИЕ СОСТОЯНИЯ...
	// обновляет состояние объекта
	// примечание: движет объект к целевой позиции
	public: void Update(float _delta)
	{
		if(!IsFreezeEnabled())// если объект разморожен
		{
			if(MoveToTarget(_delta))// если объект совершил движение
			{
				SetPosition(CalculateNVertex(GetT())); // вычислить текущий узел интерполяции (кривой; функции)
			}
		}
	}
	// СОХРАНЕНИЕ И ЗАГРУЗКА...
	// сохраняет объект в конец файла _file, и возвращает true в случае успеха
	// примечание: если файл не открыт на запись, то возвращается false
	protected: virtual bool SaveToFile(File& _file)
	{
		return true;
	}
			
	// загружает объект из файла _file, и возвращает true в случае успеха
	// примечание: если файл не существует или не открыт на чтение, то вернется false
	protected: virtual bool LoadFromFile(File& _file)
	{
		return true;
	}
};



// пользовательское приложение
class Application: public Engine// движок
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена
	private: MovingObjectEx* point1, *point2, *point3, *point4, *point5;// движущиеся объекты
    private: MovingObjectEx* selected;// выделенный объект, информацию о котором нужно отобразить
	private: int32 index;// индекс текущего выделенного узла
    private: GraphicsQuad* quad;// графический квад, в рамках которого могут двигаться объекты
	private: float deep;// глубина по Z, на которой размещены наши движущиеся объекты
	private: Vector3D targetWorldPos;// целевая позиция для всех движущихся объектов
	private: Texture2D* texture;// текстура для движущихся точек

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), point1(NIL), point2(NIL), point3(NIL), point4(NIL), point5(NIL), selected(NIL), index(0), quad(NIL), deep(3.0f), texture(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete texture;
		delete camera;
		delete font;
		delete point1;
		delete point2;
		delete point3;
		delete point4;
		delete point5;
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
				AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
				scene->Pick();
			}

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontColor = RGBA(RGBA::GRAY_COLOR);// цвет шрифта

				font = new Font2D();
				font->Rename("font2d");
				font->Create(def);
				scene->AddObject(font);// добавить в сцену
				font->Pick();// выбрать шрифт
			}


			// камера...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::TRACKBALL_FIRST_PERSON;
				def.rotationEnabled = false;// запретить вращение камеры
				//def.distanceToViewingPoint = 1.45f;
				def.zoomEnabled = false;// запретить зуминг

				camera = new Camera3D();
				camera->Create(def);
				camera->Pick(); // выбрать камеру
			}
		

			// графический квад...
			{
				quad = new GraphicsQuad();
				quad->GetQuad().SetSize(Vector2D(2.0f, 2.0f));// обновить размер плоскости
				quad->SetColor(RGBA(RGBA::GREEN_COLOR));

				quad->GetQuad().SetCenter(Vector3D(0.0f, 0.0f, -deep + 0.3f));// задать центр плоскости

				scene->AddGraphicsObject(quad);// добавить в графическую сцену
			}


			{// текстура
				texture = new Texture2D();
				texture->Create(Vector2D(8, 8));
			}

			point1 = new MovingObjectEx();
			point1->Rename("UNITS_PER_SECOND");// задать имя 
				
			point1->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point1->BindTexture(texture);
			point1->SetTexturedMaterialEnable(true);

			point1->SetMoveSpeed(0.5f);// скорость движения в единицах/секунду
			point1->SetTarget(5.0f);// расстояние от 0 до 5 будет пройдено за 2 секунды
			point1->SetColor(RGBA(55, 111, 194, 255));
			point1->SetMovingMode(FloatLerp::UNITS_PER_SECOND);//единицы в секунду
			point1->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point1->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point1, Caller<>(point1, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point1);
			scene->AddGraphicsObject(point1); 


			point2 = new MovingObjectEx();
			point2->Rename("PARABOLA");// задать имя 

			point2->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
		
			point2->BindTexture(texture);
			point2->SetTexturedMaterialEnable(true);

			point2->SetMoveSpeed(0.1f);// скорость движения 
			point2->SetTarget(15.0f);// расстояние до цели
			point2->SetColor(RGBA(118, 185, 0, 255));
			point2->SetMovingMode(FloatLerp::PARABOLA);//единицы в секунду
			point2->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point2->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point2, Caller<>(point2, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point2);
			scene->AddGraphicsObject(point2);


			point3 = new MovingObjectEx();
			point3->Rename("FIXED_TIME");// задать имя 
				
			point3->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point3->BindTexture(texture);
			point3->SetTexturedMaterialEnable(true);

			point3->SetMoveSpeed(7);// скорость движения в секундах
			point3->SetTarget(15.0f);// расстояние до цели
			point3->SetColor(RGBA(163, 21, 21, 255));
			point3->SetMovingMode(FloatLerp::FIXED_TIME);//единицы в секунду
			point3->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point3->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point3, Caller<>(point3, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point3);
			scene->AddGraphicsObject(point3);

			
			point4 = new MovingObjectEx();
			point4->Rename("DAMPING");// задать имя 

			point4->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point4->BindTexture(texture);
			point4->SetTexturedMaterialEnable(true);

			point4->SetMoveSpeed(3.0f);// скорость движения 
			point4->SetTarget(5.0f);// расстояние до цели
			point4->SetColor(RGBA(RGBA::MAGENTA_COLOR));
			point4->SetMovingMode(FloatLerp::DAMPING);
			point4->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point4->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point4, Caller<>(point4, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point4);
			scene->AddGraphicsObject(point4);

	
			point5 = new MovingObjectEx();
			point5->Rename("MAGNETIC");// задать имя 

			point5->CreateQuad2D(Quad(Vector3D(), Vector2D(0.03f, 0.03f)));
			
			point5->BindTexture(texture);
			point5->SetTexturedMaterialEnable(true);

			point5->SetMoveSpeed(0.2f);// скорость движения 
			point5->SetTarget(10.0f);// расстояние до цели
			point5->SetColor(RGBA(247, 170, 0, 255));
			point5->SetMovingMode(FloatLerp::MAGNETIC);
			point5->SetPosition(Vector3D(0.0f, 0.0f, -deep));
			point5->Connect(VectorLerp::COMPUTE_NEW_VECTOR_MESSAGE, point5, Caller<>(point5, &MovingObjectEx::UpdateTargetPosition));
			scene->AddLogicObject(point5);
			scene->AddGraphicsObject(point5);


	
			GraphicsLine::_SetLineWidth(3.0f);// толщина линий

			SelectFunction(FloatLerp::UNITS_PER_SECOND);

			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			MoveToCenter();// переместить окно в центр экрана
			Mouse::_ShowCursor(true);// показать курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}


	// выбирает функцию с индексом _index в качестве текущей
	public: void SelectFunction(int32 _index)
	{
		if(_index >= 0 && _index < 5)
		{
			index = _index;

			if(index == FloatLerp::UNITS_PER_SECOND)
			{
				selected = point1;
			}
			else if(index == FloatLerp::PARABOLA)
			{
				selected = point2;
			}
			else if(index == FloatLerp::FIXED_TIME)
			{
				selected = point3;
			}
			else if(index == FloatLerp::DAMPING)
			{
				selected = point4;
			}
			else if(index == FloatLerp::MAGNETIC)
			{
				selected = point5;
			}
		}
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

		

		// если целевая позиция существует
		if(selected->IsMovingEnabled())
		{
			GraphicsLine::_Draw(Vector3D(selected->GetTargetPosition().x, selected->GetTargetPosition().y + 0.05f, targetWorldPos.z), 
				                Vector3D(selected->GetTargetPosition().x, selected->GetTargetPosition().y - 0.05f, targetWorldPos.z), selected->GetColor());
					
			GraphicsLine::_Draw(Vector3D(selected->GetTargetPosition().x - 0.05f, selected->GetTargetPosition().y, targetWorldPos.z), 
							    Vector3D(selected->GetTargetPosition().x + 0.05f, selected->GetTargetPosition().y, targetWorldPos.z), selected->GetColor());
		}

	
		/*

		Vector2D windowPoint = Mouse::_CursorPosition(this);
		float vx = windowPoint.x;
		float vy = Renderer::_Picked()->ClientAreaSize().y - windowPoint.y - 1; 
		float vz;
	
		glReadPixels(vx, vy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &vz);// вычислить глубину пикселя vx/vy в трехмерном пространстве, и поместить в vz; если видимый пиксель отсутствует, то возвращается 1.0, что соответствует дальней отсекающей плоскости


		*/ 

		camera->BeginScreen2D();

		
		// вывод текста...
		int32 i = 0;



		font->SetFontColor(selected->GetColor());// цвет текста 
		font->DrawScreenText(Vector3D(10, i+=20, -1), "name: %s", selected->GetName().c_str());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "move speed: %f", selected->GetMoveSpeed());

		font->DrawScreenText(Vector3D(10, i+=20, -1), "current speed: %f", selected->GetCurrentSpeed());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "max speed: %f", selected->GetMaxSpeed());
		
	
		font->DrawScreenText(Vector3D(10, i+=20, -1), "is moving: %d", selected->IsMovingEnabled());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "time to target (sec): %f", selected->GetTimeToTarget());
		font->DrawScreenText(Vector3D(10, i+=20, -1), "target position: %.3f/%.3f/%.3f", selected->GetTargetPosition().x, selected->GetTargetPosition().y, selected->GetTargetPosition().z);
		font->DrawScreenText(Vector3D(10, i+=20, -1), "current position: %.3f/%.3f/%.3f", selected->GetPosition().x, selected->GetPosition().y, selected->GetPosition().z);
		//font->DrawScreenText(Vector3D(10, i+=20, -1), "vz %.3f", vz);
	}


			
	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->key == Mouse::LEFT_BUTTON)
		{
			// вычислить позицию курсора в мировых координатах...
			Line ray = camera->GetSelectRay();
			targetWorldPos = ray.begin + (ray.GetDirection() * (deep));// находим точку на линии line, которая расположена вдоль оси Z на глубине deep 

			if(quad->GetQuad().IsPointInside(targetWorldPos))// если курсор находится в области quad
			{ 
				// обновить целевую позицию для всех движущихся точек
				point1->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z));
				point2->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z)); 
				point3->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z)); 
				point4->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z));
				point5->SetTargetPosition(Vector3D(targetWorldPos.x, targetWorldPos.y, targetWorldPos.z));
			}
		}
		else if(_event->type == Mouse::WHEEL)
		{
			// обновить скорость движения для всех движущихся точек
			point1->SetMoveSpeed(point1->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point2->SetMoveSpeed(point2->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point3->SetMoveSpeed(point3->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point4->SetMoveSpeed(point4->GetMoveSpeed() + (float)_event->wheel / 3.0f);
			point5->SetMoveSpeed(point5->GetMoveSpeed() + (float)_event->wheel / 3.0f);
		}

		return Engine::MouseEventFilter(_event);
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
				Destroy();// уничтожить окно
			}  
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				index--;

				if(index < 0)
				{
					SelectFunction(4);
				}
				else { SelectFunction(index); }
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				index++;  

				if(index > 4) 
				{
					SelectFunction(0);
				}
				else { SelectFunction(index); }
			}
		}

		return Engine::KeyboardEventFilter(_event);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "VectorLerp.example_03";
	def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
