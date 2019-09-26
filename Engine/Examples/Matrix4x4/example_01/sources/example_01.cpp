

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

			 	
    private: float xPos, yPos, zPos;// позиция куба 
    private: float spinX, spinY, spinZ;// угол вращения куба по осям X,Y,Z
	private: float step;// размер шага угла вращения
	private: float scaleX, scaleY, scaleZ; // коэффициенты масштабирования по осям X,Y,Z
    Matrix4x4 T, S, R;

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), box(NIL), xPos(0.0f), yPos(0.0f), zPos(0.0f), spinX(0.0f), spinY(0.0f), spinZ(0.0f), step(0.1f), scaleX(1.0f), scaleY(1.0f), scaleZ(1.0f)
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
	{}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену


	
		// матрица перемещения
		T.Translation(Vector3D(xPos, yPos, zPos));


		// матрица масштабирования
		S.Scaling(Vector3D(scaleX, scaleY, scaleZ));


		// матрица вращения
		/* один такой поворот – поворачивает всю систему координат, следовательно, второй поворот происходит вокруг уже повернутой 
		   оси, и тоже поворачивает систему координат, вокруг которой происходит третий поворот */ 
		
		//R = Matrix3x3::_RotationX(spinX) * Matrix3x3::_RotationY(spinY) * Matrix3x3::_RotationZ(spinZ); // альтернативный способ задания ориентации по трем осям
		R = Matrix3x3::_FromAxisAngle(AxisAngle(Vector3D::UNIT_X, spinX)) * Matrix3x3::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Y, spinY)) * Matrix3x3::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, spinZ));
  

			
		// индивидуальная трансформация
		box->SetPosition(Vector3D(xPos, yPos, zPos)); // обновить позицию
		box->SetRotation(R.ToQuaternion());
		box->SetScale(Size3D(scaleX, scaleY, scaleZ));
		

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
		gizmo.vectorX = (T * R * S).GetX();
		gizmo.vectorY = (T * R * S).GetY();
		gizmo.vectorZ = (T * R * S).GetZ();
		gizmo.Draw();

			
	
				
		int i = 0; 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "box.Position (X: %.2f, Y: %.2f, Z: %.2f)", xPos, yPos, zPos); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "box.Rotation (X: %.2f, Y: %.2f, Z: %.2f)", spinX, spinY, spinZ); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "box.Scaling (X: %.2f, Y: %.2f, Z: %.2f)", scaleX, scaleY, scaleZ); 

		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Scaling matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m11, S.m12, S.m13, S.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m21, S.m22, S.m23, S.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m31, S.m32, S.m33, S.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", S.m41, S.m42, S.m43, S.m44); 

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Rotation matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m11, R.m12, R.m13, R.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m21, R.m22, R.m23, R.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m31, R.m32, R.m33, R.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", R.m41, R.m42, R.m43, R.m44);
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1),  "Translation matrix:"); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m11, T.m12, T.m13, T.m14); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m21, T.m22, T.m23, T.m24); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m31, T.m32, T.m33, T.m34); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "|%.2f|%.2f|%.2f|%.2f|", T.m41, T.m42, T.m43, T.m44);


		// визуализировать сцену...
		camera->Begin3D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(Keyboard::_IsKeyPressed(Keyboard::E))
		{
			if(_event.wheel > 0)
			{
				scaleX += step;
			}
			else if(_event.wheel < 0)
			{
				scaleX -= step;
			}

			if(scaleX > 10.0f) { scaleX = 10.0f; }
			else if (scaleX < 0.1f) { scaleX = 0.1f; }
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::D))
		{
			if(_event.wheel > 0)
			{
				scaleY += step;
			}
			else if(_event.wheel < 0)
			{
				scaleY -= step;
			}

			if(scaleY > 10.0f) { scaleY = 10.0f; }
			else if (scaleY < 0.1f) { scaleY = 0.1f; }
		}
		else if(Keyboard::_IsKeyPressed(Keyboard::C))
		{
			if(_event.wheel > 0)
			{
				scaleZ += step;
			}
			else if(_event.wheel < 0)
			{
				scaleZ -= step;
			}

			if(scaleZ > 10.0f) { scaleZ = 10.0f; }
			else if (scaleZ < 0.1f) { scaleZ = 0.1f; }
		}
		else if(_event.type == Mouse::WHEEL)
		{
			if(_event.wheel > 0)
			{
				zPos += 1.0f;
			}
			else if(_event.wheel < 0)
			{
				zPos -= 1.0f;
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
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				xPos--;
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				xPos++;
			}	
				else if(_event.key == Keyboard::UP_ARROW)
			{
				yPos++;
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				yPos--; 
			}	
			else if(_event.key == Keyboard::Q)
			{
				spinX += step;
				// контролируем выход за пределых 360 градусов
				if(spinX > Angle::DOUBLE_PI) { spinX = spinX - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::W)
			{
				spinX -= step;
				// контролируем выход за пределых 360 градусов
				if(spinX > Angle::DOUBLE_PI) { spinX = spinX - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::A)
			{
				spinY += step;
				// контролируем выход за пределых 360 градусов
				if(spinY > Angle::DOUBLE_PI) { spinY = spinY - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::S)
			{
				spinY -= step;
				// контролируем выход за пределых 360 градусов
				if(spinY > Angle::DOUBLE_PI) { spinY = spinY - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::Z)
			{
				spinZ += step;
				// контролируем выход за пределых 360 градусов
				if(spinZ > Angle::DOUBLE_PI) { spinZ = spinZ - Angle::DOUBLE_PI; }
			}
			else if(_event.key == Keyboard::X)
			{
				spinZ -= step;
				// контролируем выход за пределых 360 градусов
				if(spinZ > Angle::DOUBLE_PI) { spinZ = spinZ - Angle::DOUBLE_PI; }
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
	def.realTimeWorkerDef.windowDef.tittle = "Matrix4x4.example_01";
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
