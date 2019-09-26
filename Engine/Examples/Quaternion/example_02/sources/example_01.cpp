

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
#include <Engine/Input/Controls/Controls.h>
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Math/Matrix/Matrix3x3/Matrix3x3.h>





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

		glTranslatef(690.0f, 90.0f, -300.0f);// перенос

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
class MyBox: public GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>
{
	// METHODS
	// конструктор по умолчанию
	public: MyBox(void)
	{}

	// визуализация прямоугольника
    public: virtual void Draw(void)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::FRONT_FACE);
				
		glColor3f(0.0f, 1.0f, 0.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::BACK_FACE);
				
		glColor3f(0.0f, 0.0f, 1.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::LEFT_FACE);
					
		glColor3f(1.0f, 0.0f, 1.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::RIGHT_FACE);
						
		glColor3f(1.0f, 1.0f, 0.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::TOP_FACE);
					
		glColor3f(0.0f, 1.0f, 1.0f);
		GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::Draw(GraphicsBox3DFixed<Vector3D, ColorRGBA, Quaternion>::BOTTOM_FACE);
	}
};

// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// графическая сцена
    private: Gizmo gizmo;// компас, отображающий мировую систему координат камеры
	private: MyBox* box;// трехмерный прямоугольник, имитирующий коробку

			 	
   
    private: Quaternion quaternion;// тестируемый кватернион
	private: float angleX, angleY, angleZ;// угол вращения кватерниона по осям, в радианах
	private: QuaternionSlerper slerper; // путь кватерниона из исходного положения в quaternion по кратчайшей дуге, используя SLERP-интерполяцию
	private: bool slerperEnable;// если true, то запустить проигрывание SLERP-интерполяции
	private: float t;// порядковый номер узла интерполяции SLERP

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), box(NIL), angleX(0.0f), angleY(0.0f), angleZ(0.0f), slerperEnable(false), t(0.0f)
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
				GraphicsScene<Vector3D, Quaternion, Size3D>::Define def;
				def.logicSceneDef.scale = Size3D(1.0f, 1.0f, 1.0f);
				scene = new GraphicsScene<Vector3D, Quaternion, Size3D>();
				scene->Create(def);
			}

			// куб...
			{
				GraphicsMesh<Vector3D, uint8, ColorRGBA, Quaternion, Size3D>::Define def;
				def.name = "box3d";
				def.scale = Size3D(1,1,1);  
					
				box = new MyBox();
				box->Create(def);
				box->CreateBox3D(Box3D(Vector3D(0, 0, 0), Size3D(0.5f, 0.5f, 0.5f)));
	

				// буфер цветов
				box->CreateColors(); 

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
				def.position = Vector3D(0,0,10);
				def.controllerEnabled = false;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
			}

			Flush();//выполнить все ранее отложенные события
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}
	




	// функция кадра
	public: virtual void Update(float _delta)
	{
		// длительность анимации равна 2 секунды (t аккумулирует дельту _delta, и при t = 1, проходит 1 секунда времени; 
		// мы делим _delta на два, чтобы увеличить проигрывание анимации в два раза, в результате чего t заполняется за 2 секунды)
		if(slerperEnable)// если нужно запустить проигрывание анимации с помощью SLERP-интерполяции
		{
			if(t > 1.0f) // если анимаци закончилась
			{ 
				slerperEnable = false;// отключить интерполяцию SLERP
			    t = 0.0f; // сбросить аккумулятор (таймер анимации)
			} 
			else// иначе запустить проигрывание анимации
			{ 
				quaternion = slerper.Interpolate(t);// t измеряется от 0 (identity) до 1 (quaternion)
			
				t += _delta / 2.0f;// перейти к следующему узлу интерполяции SLERP
			}
		}
	}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


		box->SetRotation(quaternion);

		// трансформация всей сцены
		//_SetCurrentStack(Renderer::MODELVIEW);
		//_PushMatrix();
		//_LoadIdentityMatrix();
		//_LoadMatrix(T * R * S);// обновить поворот
		box->Draw();// отобразить бокс
		//_PopMatrix();
		 
		

		// вывод текста...
		camera->Begin2D();// начать вывод двухмерной графики

				
		// отобразить компас...
		gizmo.vectorX = quaternion.RotateVector(Vector3D::UNIT_X);
		gizmo.vectorY = quaternion.RotateVector(Vector3D::UNIT_Y);
		gizmo.vectorZ = quaternion.RotateVector(Vector3D::UNIT_Z);
		gizmo.Draw();

			
	
				
		int i = 0; 
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "box.Rotation (X: %.2f, Y: %.2f, Z: %.2f)", angleX, angleY, angleZ); 


		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quaternion(x: %.2f, y: %.2f, z: %.2f, w: %.2f)", quaternion.x, quaternion.y, quaternion.z, quaternion.w); 
	
	
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "AxisAngle X/Y/Z/Angle: %.2f/%.2f/%.2f/%.2f", quaternion.ToAxisAngle().axis.x, quaternion.ToAxisAngle().axis.y, quaternion.ToAxisAngle().axis.z, quaternion.ToAxisAngle().angle);
			
	
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quaternion.Norm: %.2f", quaternion.Norm());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quaternion.Length: %.2f", quaternion.Length());

	


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "rotation to matrix:");

		Matrix4x4 R = Matrix4x4::_FromQuaternion(quaternion);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Rotation matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m11, R.m12, R.m13, R.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m21, R.m22, R.m23, R.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m31, R.m32, R.m33, R.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m41, R.m42, R.m43, R.m44);
		
		

		
		
		Quaternion q;
		q.FromEuler(Euler(7, Angle::PI, Angle::DOUBLE_PI));
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "pitch: %.2f, yaw: %.2f, roll: %.2f", quaternion.GetPitch(), quaternion.GetYaw(), quaternion.GetRoll());
		



		// визуализировать сцену...
		camera->Begin3D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(Keyboard::_IsKeyPressed(Keyboard::X))
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					quaternion *= Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_X, 0.1));
					angleX += 0.1;
				}
				else if(_event.wheel < 0)
				{
					quaternion *= Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_X, -0.1));
					angleX -= 0.1;

				}
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::Y))
		{
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					quaternion *= Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Y, 0.1));
					angleY += 0.1;
				}
				else if(_event.wheel < 0)
				{
					quaternion *= Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Y, -0.1));
					angleY -= 0.1;
				}
			}
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::Z))
		{	
			// если смещается колесико мыши
			if(_event.type == Mouse::WHEEL)
			{
				if(_event.wheel > 0)
				{
					quaternion *= Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, 0.1));
					angleZ += 0.1;
				}
				else if(_event.wheel < 0)
				{
					quaternion *= Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, -0.1));
					angleZ -= 0.1;
				}	
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
			else if(_event.key == Keyboard::I)
			{
				quaternion.Identity();
			}
			else if(_event.key == Keyboard::N)
			{
				quaternion.Normalize();
			}
			else if(_event.key == Keyboard::C)
			{
				quaternion.Conjugate();
			}	
			else if(_event.key == Keyboard::L)
			{
				slerperEnable = true;// запустить проигрывание интерполяции
				slerper.Set(quaternion, Quaternion());// вращаем из положения quaternion в положение по умолчанию
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
	def.realTimeWorkerDef.windowDef.tittle = "Quaternion.example_01";
	//def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;
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
