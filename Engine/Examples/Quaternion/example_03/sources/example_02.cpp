

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
  

    public: Vector3D vector;// тестируемый вектор
    public: ColorRGBA yellow;// цвет для тестируемого вектора
	public: bool drawRotate;// если true, то отобразить вектор rotate
	public: Vector3D rotate;// вращение вектора vector


	// METHODS
	// конструктор по умолчанию
	public: Gizmo(void): drawRotate(false)
	{
		// задать направление осям X,Y,Z
		vectorX.Set(1.0f, 0.0f, 0.0f);
		vectorY.Set(0.0f, 1.0f, 0.0f);
		vectorZ.Set(0.0f, 0.0f, 1.0f);

		cyan = ColorRGBA::CYAN_COLOR; 
		red = ColorRGBA::RED_COLOR;
		green = ColorRGBA::GREEN_COLOR;

		vector.Set(-1.0f, 0.0f, 0.0f);
		yellow = ColorRGBA::YELLOW_COLOR;
	}


	// визуализация Gizmo
	public: void Draw(void)
	{		
		// трансформации...
		//glMatrixMode(GL_MODELVIEW);// работаем с матрицей вида
		//glPushMatrix();// сохранить текущую матрицу в буфер  
		//glLoadIdentity();// загрузить единичную матрицу

		//glTranslatef(390.0f, 290.0f, -300.0f);// перенос

		//glScalef(150.0f, 150.0f, 150.0f);// масштабирование
		//glRotatef(10.0f, 1.0f, 0.0f, 0.0f);// поворот по оси X
		//glRotatef(45.0f, 0.0f, 1.0f, 0.0f);// поворот по оси Y

		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// толщина линий для осей 

		// визуализация осей трехмерной системы координат
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorX, red);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorY, cyan);
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, 0.0f, 0.0f), vectorZ, green);

		// отобразить тестируемый вектор... 

		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), vector, yellow);// тестируемый вектор
		
		if(drawRotate)
		{
			GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0,0,0), rotate, yellow);// тестируемый вектор
		}


		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(1.0f); // восстановить толщину линий 
		
		// трансформации...
		//glPopMatrix();// восстановить предыдущую матрицу из буфера  
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
	
   
    private: Quaternion quaternion;// тестируемый кватернион
	private: QuaternionLerper lerper; // путь кватерниона из исходного положения в quaternion по кратчайшей дуге, используя LERP-интерполяцию
	private: bool lerperEnable;// если true, то запустить проигрывание LERP-интерполяции
	private: float t;// порядковый номер узла интерполяции LERP
	private: Quaternion previousCameraRotation;// предыдущая ориентация камеры
	private: bool rotateAroundPivot;// если true, то вращать желтый вектор вокруг начала координат по оси Y
	private: float spinZ;


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), lerperEnable(false), t(0.0f), rotateAroundPivot(false), spinZ(0.0f)
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
				
				def.controllerType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
				def.pitchMax = 0.0f;// запретить камере опускаться ниже пола (плоскости XZ)
				def.viewingPoint = Vector3D();
				def.zoomMin = 7.0f;
				def.zoomMax = 9.0f;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
				camera->ConnectTo(Mouse::Event::TYPE, this);

				// задать изометрический ракурс камеры
				camera->UpdateTrackBallObjectViewer(-Angle::HALF_PI/3.0f, -Angle::HALF_PI/3.0f, 0.0f);
			}

			Mouse::_MoveCursorToClientAreaCenter(this);// переместить курсор в центр клиентской области окна
			Mouse::_ShowCursor(false);// скрыть курсор

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
		if(lerperEnable)// если нужно запустить проигрывание анимации с помощью LERP-интерполяции
		{
			if(t > 1.0f) // если анимаци закончилась
			{ 
				lerperEnable = false;// отключить интерполяцию LERP
				gizmo.drawRotate = false;
			    t = 0.0f; // сбросить аккумулятор (таймер анимации)
			} 
			else// иначе запустить проигрывание анимации
			{ 
				quaternion = lerper.Interpolate(t);// t измеряется от 0 (identity) до 1 (quaternion)
			
				t += _delta / 2.0f;// перейти к следующему узлу интерполяции LERP
			}
		}
	}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


		if(lerperEnable)// пока идет интерполяция
		{
			gizmo.rotate = quaternion.RotateVector(gizmo.vector);// вращаем вектор 
		}
		

		if(rotateAroundPivot)// если нужно вращать вокруг точки
		{
			Quaternion q1(AxisAngle(Vector3D::UNIT_Y, 0.1f));

			gizmo.vector = q1.RotatePointAroundPivot(gizmo.vector, Vector3D());// вращаем точку gizmo.vector по оси Y, вокруг начала координат
		
			// альтернативный способ 
			//Matrix4x4 m = Matrix4x4::_RotationAroundPivot(Matrix4x4(q1), Vector3D());
			//gizmo.vector = m * gizmo.vector;

			// альтернативный способ 
			//gizmo.vector = Matrix4x4(q1).RotatePointAroundPivot(gizmo.vector, Vector3D());
		}
		else
		{
			// находим дельту-кватернион между ориентацией желтого вектора по умолчанию и текущей ориентацией камеры, и вращаем с помощью этого кватерниона изначальный вектор gizmo.vector
			// желтый вектор всегда указывает в одном и том же направление
			gizmo.vector = Quaternion::_FromTo(Quaternion(), camera->GetRotation()).RotateVector(Vector3D(-1.0f, 0.0f, 0.0f));
		}
	
		// отобразить компас
		gizmo.Draw();

			
	
			
		// вывод текста...
		camera->Begin2D();// начать вывод двухмерной графики

	
		// отображение названий осей мировой системы координат
		Font2D::_Picked()->SetTextColor(ColorRGBA(ColorRGBA::RED_COLOR));
		
		Font2D::_Picked()->DrawScreenText(Vector3D(390.0f - 20, 210, -1), "X"); 

		Font2D::_Picked()->SetTextColor(ColorRGBA(ColorRGBA::CYAN_COLOR));
		Font2D::_Picked()->DrawScreenText(Vector3D(390.0f + 5, 210, -1), "Y"); 

		Font2D::_Picked()->SetTextColor(ColorRGBA(ColorRGBA::GREEN_COLOR));
		Font2D::_Picked()->DrawScreenText(Vector3D(390.0f + 30, 210, -1), "Z"); 
		Font2D::_Picked()->SetTextColor(ColorRGBA::WHITE_COLOR);
				

		int i = 0; 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "quaternion(x: %.2f, y: %.2f, z: %.2f, w: %.2f)", quaternion.x, quaternion.y, quaternion.z, quaternion.w); 
	


		Matrix4x4 R = Matrix4x4::_FromQuaternion(quaternion);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Rotation matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m11, R.m12, R.m13, R.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m21, R.m22, R.m23, R.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m31, R.m32, R.m33, R.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m41, R.m42, R.m43, R.m44);
		
		



		// визуализировать сцену...
		camera->Begin3D();// начать вывод трехмерной графики


		previousCameraRotation = camera->GetRotation();// запомнить текущее вращение камеры
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{	
	
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
			else if(_event.key == Keyboard::L)
			{
				lerperEnable = true;// запустить проигрывание интерполяции
				gizmo.drawRotate = true;

				Quaternion q = Quaternion::_ShortestArc(gizmo.vector, gizmo.vectorZ);// найти кватернион из направления gizmo.vector в направление gizmo.vectorZ
				
				// вращаем из положения q в положение по умолчанию
				lerper.Set(q, Quaternion());
			}
			else if(_event.key == Keyboard::R)
			{
				rotateAroundPivot = !rotateAroundPivot;
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
	def.realTimeWorkerDef.windowDef.tittle = "Quaternion.example_02";
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
