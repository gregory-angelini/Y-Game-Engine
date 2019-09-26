// Win32.cpp: определяет точку входа для приложения.
//


#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// для текстур

#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Shape/Box/GraphicsBox3DFixed/GraphicsBox3DFixed.h> 
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>


// класс, реализующий навигационный компас
class Gizmo
{
	// DATA
	public: Vector3D vectorX;// направление оси X
	public: Vector3D vectorY;// направление оси Y
	public: Vector3D vectorZ;// направление оси Z
 
	public: ColorRGBA cyan;// цвет для оси Y
    public: ColorRGBA red; // цвет для оси X
	public: ColorRGBA green;// цвет для оси Z
  

	// METHODS
	// конструктор по умолчанию
    public: Gizmo(void)
	{
		// задать направление осям X,Y,Z
		vectorX.Set(1.0f, 0.0f, 0.0f);
		vectorY.Set(0.0f, 1.0f, 0.0f);
		vectorZ.Set(0.0f, 0.0f, 1.0f);

		cyan = ColorRGBA::CYAN_COLOR; 
		red = ColorRGBA::RED_COLOR;
		green = ColorRGBA::GREEN_COLOR;
	}


	// визуализация Gizmo
	public: void Draw(void)
	{		
		// трансформации...
		glMatrixMode(GL_MODELVIEW);// работаем с матрицей вида
		glPushMatrix();// сохранить текущую матрицу в буфер  
		glLoadIdentity();// загрузить единичную матрицу

		glTranslatef(690.0f, 90.0f, -30.0f);// перенос

		glScalef(50.0f, 50.0f, 50.0f);// масштабирование
		glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// поворот по оси X
		glRotatef(45.0f, 0.0f, 1.0f, 0.0f);// поворот по оси Y

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// толщина линий для осей 

		// визуализация осей трехмерной системы координат
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorX, red);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorY, cyan);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorZ, green);

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(1.0f); // восстановить толщину линий 
		
		// трансформации...
		glPopMatrix();// восстановить предыдущую матрицу из буфера  

		// отображение названий осей мировой системы координат
		Font2D::_Picked()->SetTextColor(red);
		Font2D::_Picked()->DrawScreenText(Vector3D(610 + 10+50, 50, -1), "X"); 

		Font2D::_Picked()->SetTextColor(cyan);
		Font2D::_Picked()->DrawScreenText(Vector3D(610 + 10+25+50, 50, -1), "Y"); 

		Font2D::_Picked()->SetTextColor(green);
		Font2D::_Picked()->DrawScreenText(Vector3D(610 + 10+50+50, 50, -1), "Z"); 
		Font2D::_Picked()->SetTextColor(ColorRGBA::WHITE_COLOR);

	}
};

// класс, реализующий трехмерную коробку
class MyQuad: public GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Matrix4x4>
{
	// DATA
	public: float height;// высота пола по оси Y
	public: float lineSize;// толщина линий в пикселях
	public: float floorWidth, floorHeight;// размерость пола
	
	// METHODS
	// конструктор по умолчанию
	public: MyQuad(void): floorWidth(10.0f), floorHeight(10.0f), lineSize(3.0f), height(-0.3f)
	{


		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Matrix4x4>::Define def;
		def.meshDef.name = "floor";
		def.meshDef.position = Vector3D(0, height, 0);
		def.meshDef.rotation = Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_X, Angle::_DegreesToRadians(-90.0f)));
		Create(def);


		CreateQuad2D(Quad<Vector3D>(Vector3D(), Size2D(floorWidth, floorHeight)));// размер плоскости
				
		for(int32 i = 0; i < 5; ++i)// уровень дробления полигона
		{ 
			TotalPolygonsDivide4(); 
		} 

		AutoResizeColors();
		SetColor(ColorRGBA(255,255,128,255)); // цвет
	}

	// функция визуализации
    public: virtual void Draw(void)
	{
		Renderer::FaceDrawMode saveMode = Renderer::_Picked()->GetFaceDrawMode(Renderer::FRONT);// сохранить режим визуализации
		Renderer::_Picked()->SetFaceDrawMode(Renderer::FRONT, Renderer::LINE);// отобразить каркас
		float saveLineWidth = GraphicsLine<Vector3D, ColorRGBA>::_LineWidth();// сохранить толщину линий
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(lineSize);// толщина линий для осей 

		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Matrix4x4>::Draw();

		
		Renderer::_Picked()->SetFaceDrawMode(Renderer::FRONT, saveMode);// восстановить режим визуализации для фронтальных граней
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(saveLineWidth);// восстановить толщину линий для осей 
	} 
};

// реализует графическую точку с ограниченным временем жизни
class MyPoint: public GraphicsPoint<Vector3D, ColorRGBA>,// графическая точка
	           public LogicObject// логический объект
{
	// DATA
	private: AsyncTimer timer;// таймер 
	
	// METHODS
	// конструктор по умолчанию
	public: MyPoint(void)
	{}

	// инициализирующий конструктор
    public: MyPoint(float _time, Vector3D _position)
	{
		SetPosition(_position);
		timer.StartSec(_time);
		SetColor(ColorRGBA::RED_COLOR);
	}

	// возвращает true, если время жизни точки еще не вышло
	public: bool IsAlive(void)
	{
		return !timer.Check();
	}

	// РАЗМЕР...
	// возвращает размер (в байтах) динамических данных, которыми обладает объект
	public: virtual int32 DynamicSizeInByte(void)const
	{
		return GraphicsPoint<Vector3D, ColorRGBA>::DynamicSizeInByte() + LogicObject::DynamicSizeInByte();
	}

	// возвращает размер (в байтах) статических данных, которыми обладает объект
	public: virtual int32 StaticSizeInByte(void)const
	{
		return sizeof(*this);
	}
};


// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene<Vector3D, Quaternion, Matrix4x4>* scene;// графическая сцена


	private: Gizmo gizmo;// компас, отображающий мировую систему координат камеры
	private: Quaternion q;// кватернион поворота и интерполированный кватернион того же поворота в момент времени t
	private: QuaternionSlerper slerper; // путь кватерниона из исходного положения в quaternion по кратчайшей дуге, используя LERP-интерполяцию
	private: bool lerperEnable;// если true, то запустить проигрывание lerp-интерполяции
	private: float t;// порядковый номер узла интерполяции lerp
	

	private: MyQuad* floor;// пол
	private: GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Matrix4x4>* box;// трехмерный прямоугольник, имитирующий коробку
    private: float sightSize;// размер прицела (длина каждой линии в пикселях)

	private: MagicList<MyPoint> pointList;//список графических точек
	private: bool plumeVisible;// если true, то отобразить шлейф курсора


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), floor(NIL), lerperEnable(false), t(0.0f), sightSize(20.0f), box(NIL), plumeVisible(false)
	{}

			
	// виртуальный деструктор
	public: virtual ~Application(void)
	{
		delete scene;
	} 

	// создает приложение
	public: bool Create(Renderer::Define _define)
	{
		// создать графическое ядро
		if(Renderer::Create(_define))
		{
			// сцена...
			{
				GraphicsScene<Vector3D, Quaternion, Matrix4x4>::Define def;
				scene = new GraphicsScene<Vector3D, Quaternion, Matrix4x4>();
				scene->Create(def);
			}

			// куб...
			{
				GraphicsMesh<Vector3D, uint8, ColorRGBA, Quaternion, Matrix4x4>::Define def;
				def.meshDef.name = "box3d";

				
				box = new GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion, Matrix4x4>();
				box->Create(def);
				

				box->CreateBox3D(Box3D(Vector3D(0, 0, 0), Size3D(0.5f, 0.5f, 0.5f)));

				// буфер цветов
				box->AutoResizeColors(); 

				box->SetColor(0, ColorRGBA::_Random());
				box->SetColor(1, ColorRGBA::_Random());
				box->SetColor(2, ColorRGBA::_Random());
				box->SetColor(3, ColorRGBA::_Random());

				box->SetColor(4, ColorRGBA::_Random());
				box->SetColor(5, ColorRGBA::_Random());
				box->SetColor(6, ColorRGBA::_Random());
				box->SetColor(7, ColorRGBA::_Random());
				scene->AddObject(box, false, true);// добавить в сцену
			} 

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.textColor = ColorRGBA::WHITE_COLOR;
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}


			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.movementType = Camera3D::OVERLAND;
				def.position = Vector3D(0,0,2.2);
				def.screenProjectionOrigin = Camera3D::LEFT_BOTTOM_CORNER;


				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->Pick("Camera3D.example_02");
			
			}

		
			// создать пол...
			{
				floor = new MyQuad();
				scene->AddObject(floor, false, true);
			}

			Mouse::_MoveCursorToClientAreaCenter(this);// переместить курсор в центр клиентской области окна
			Mouse::_ShowCursor(false);// скрыть курсор
			
			Flush();// выполнить все отложенные события
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}
	


	// отобразить шлейф курсора
	public: void DrawCursorPlume(void)
	{
		if(plumeVisible)// если нужно отобразить шлейф курсора
		{
			GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(3.0f);		
			MagicList<MyPoint>::LogicAdressDerived* it = pointList.HeadAdress();
			while(it)
			{
				if(it->Node()->IsAlive())// если точка "жива"
				{
					it->Node()->Draw();
					it = it->Right();
				}
				else // иначе время жизни вышло
				{
					MyPoint* node = it->Node();
					it = it->Right();
					delete node;// удалить узел
					
				}
			}
			GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(1.0f);
		}
	}


	// отобразить прицел по центру экрана
	public: void DrawSight(void)
	{
		// начать вывод двухмерной графики
		camera->BeginScreen2D();
 


		ColorRGBA green = ColorRGBA::GREEN_COLOR;// зеленый цвет

		Vector2D center = Size2D::_ToVector2D(ClientAreaSize() / 2.0f);// вычислить центр вьюпорта

	
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(center.x - sightSize/2.0f, center.y), Vector2D(center.x + sightSize/2.0f, center.y), green);// рисуем горизонтальную линию слева направо
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(center.x, center.y - sightSize/2.0f), Vector2D(center.x, center.y + sightSize/2.0f), green);// рисуем вертикальную линию снизу вверх


		Renderer::_PopMatrix();

		// начать вывод трехмерной графики
		camera->End2D();
	}






	// функция кадра
	public: virtual void Update(float _delta)
	{}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


		// вывод гизмо...
		camera->BeginScreen2D();// начать вывод двухмерной графики

	
		// отобразить компас...
		gizmo.vectorX = camera->RotationMatrix().GetX();
		gizmo.vectorY = camera->RotationMatrix().GetY();
		gizmo.vectorZ = camera->Look();
		gizmo.Draw();
			


		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики
		scene->Draw();



		DrawSight();// вывести прицел камеры

		
		

		DrawCursorPlume();// отобразить шлейф курсора


		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики

		int i = 0;
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "roll/isClamped/rollMin/rollMax: %.2f/%d/%.2f/%.2f", camera->GetRotation().GetRoll(), camera->IsRollClampEnabled(), camera->RollMin(), camera->RollMax());
	    
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "yaw/isClamped/yawMin/yawMax: %.2f/%d/%.2f/%.2f", camera->GetRotation().GetYaw(), camera->IsYawClampEnabled(), camera->YawMin(), camera->YawMax());
	    
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "pitch/isClamped/pitchMin/pitchMax: %.2f/%d/%.2f/%.2f", camera->GetRotation().GetPitch(), camera->IsPitchClampEnabled(), camera->PitchMin(), camera->PitchMax());
	    
		

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera->position: x: %.2f, y: %.2f, z: %.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	    
		
		StringANSI string;  
		if(camera->GetMovementType() == Camera3D::OVERLAND) { string = "OVERLAND"; }
		else if(camera->GetMovementType() == Camera3D::FLIGHT) { string = "FLIGHT"; } 
		else if(camera->GetMovementType() == Camera3D::SPECTATOR) { string = "SPECTATOR"; } 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera->MovementType: %s", string.c_str());
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "view matrix:");
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m11, camera->ViewMatrix().m12, camera->ViewMatrix().m13);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m21, camera->ViewMatrix().m22, camera->ViewMatrix().m23);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m31, camera->ViewMatrix().m32, camera->ViewMatrix().m33);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m41, camera->ViewMatrix().m42, camera->ViewMatrix().m43);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "camera->look: %.2f/%.2f/%.2f", camera->Look().x, camera->Look().y, camera->Look().z);
	
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "controller sensitivity: %.2f", camera->RotationSensitivity());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "controller smoothing enabled : %d", camera->IsRotationSmoothingEnabled());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "controller history size: %d", camera->RotationHistorySize());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),"controller smoothing modifier: %.2f", camera->RotationSmoothingModifier());
 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),"controller enabled: %d", camera->IsRotationEnabled());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),"controller zoom enabled: %d", camera->IsZoomEnabled());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),"amount of points: %d", pointList.Size());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),"camera->fov: %.2f", camera->Fov());



		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		// зум камеры...
		if(_event.type == Mouse::MOVE && plumeVisible)
		{
			pointList.AddToTail(new MyPoint(3.0f, camera->Look()*2 + camera->GetPosition()));// создать точку с ограниченным временем жизни
		}
		else if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
				camera->SetRotationSensitivity(camera->RotationSensitivity() + 0.1);
			}
			else if(_event.key == Mouse::RIGHT_BUTTON)
			{
				camera->SetRotationSensitivity(camera->RotationSensitivity() - 0.1);
			}
		}

		Mouse().ClearEventList();// очистить буфер событий
	}


	// обработка событий клавиатуры
	public: virtual void KeyboardEventFilter(Keyboard::Event _event)
	{
		if(_event.type == Keyboard::KEY_DOWN)
		{
			if(_event.key == Keyboard::NUM_1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::NUM_2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			}
			else if(_event.key == Keyboard::A)
			{
				camera->Strafe(-0.1f);
			}
			else if(_event.key == Keyboard::D)
			{
				camera->Strafe(0.1f);
			}
			else if(_event.key == Keyboard::W)
			{
				camera->Walk(0.1f);
			}
			else if(_event.key == Keyboard::S)
			{
				camera->Walk(-0.1f);
			}
			else if(_event.key == Keyboard::Q)
			{
				camera->Fly(0.1f);
			}
			else if(_event.key == Keyboard::E)
			{
				camera->Fly(-0.1f);
			}
			else if(_event.key == Keyboard::M)
			{
				if(camera->GetMovementType() == Camera3D::FLIGHT)
				{
					camera->SetMovementType(Camera3D::OVERLAND);
				}
				else if(camera->GetMovementType() == Camera3D::OVERLAND)
				{
					camera->SetMovementType(Camera3D::SPECTATOR);
				}
				else if(camera->GetMovementType() == Camera3D::SPECTATOR)
				{
					camera->SetMovementType(Camera3D::FLIGHT);
				}
			}
			else if(_event.key == Keyboard::K)
			{
				camera->MoveTo(camera->Look(), 4.0f);
			}
			else if(_event.key == Keyboard::R)
			{
				camera->SetRollClampEnable(!camera->IsRollClampEnabled());
			}
			else if(_event.key == Keyboard::Y)
			{
				camera->SetYawClampEnable(!camera->IsYawClampEnabled());
			}
			else if(_event.key == Keyboard::P)
			{
				camera->SetPitchClampEnable(!camera->IsPitchClampEnabled());
			}
			else if(_event.key == Keyboard::C)
			{
				camera->SetRotationSmoothingEnabled(!camera->IsRotationSmoothingEnabled());
			}
			else if(_event.key == Keyboard::V)
			{
				plumeVisible = !plumeVisible;
			}
			else if(_event.key == Keyboard::ADD)
			{
				camera->SetRotationSmoothingModifier(camera->RotationSmoothingModifier() + 0.1);
			}
			else if(_event.key == Keyboard::SUBTRACT)
			{
				camera->SetRotationSmoothingModifier(camera->RotationSmoothingModifier() - 0.1);
			}
			else if(_event.key == Keyboard::MINUS)
			{
				camera->SetRotationHistorySize(camera->RotationHistorySize() - 10);
			}
			else if(_event.key == Keyboard::EQUALS)
			{
				camera->SetRotationHistorySize(camera->RotationHistorySize() + 10);
			}
			else if(_event.key == Keyboard::I)
			{
				camera->Inverse();
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий
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
	Renderer::Define def;
	def.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна 
	def.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.realTimeWorkerDef.windowDef.tittle = "Camera3D.example_02";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::POINT;
	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = false;

	
	Application* app = new Application;// создать окно
	
	if(app->Application::Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
