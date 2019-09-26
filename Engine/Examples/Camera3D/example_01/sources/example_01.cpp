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
#include <Engine/Input/Controls/Controls.h>



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
class MyQuad: public GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>
{
	// DATA
	public: float height;// высота пола по оси Y
	public: float lineSize;// толщина линий в пикселях
	public: float floorWidth, floorHeight;// размерость пола
	
	// METHODS
	// конструктор по умолчанию
	public: MyQuad(void): floorWidth(10.0f), floorHeight(10.0f), lineSize(3.0f), height(-0.3f)
	{


		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>::Define def;
		def.scale = Size3D(1.0f, 1.0f, 1.0f);
		def.name = "floor";
		def.position = Vector3D(0, height, 0);
		def.rotation = Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_X, Angle::_DegreesToRadians(-90.0f)));
		Create(def);


		CreateQuad3D(Box2D<Vector3D>(Vector3D(), Size2D(floorWidth, floorHeight)));// размер плоскости
				
		for(int32 i = 0; i < 5; ++i)// уровень дробления полигона
		{ 
			TotalPolygonsDivide4(); 
		} 

		CreateColors();
		SetColor(ColorRGBA(255,255,128,255)); // цвет
	}

	// функция визуализации
    public: virtual void Draw(void)
	{
		Renderer::FaceDrawMode saveMode = Renderer::_Picked()->GetFaceDrawMode(Renderer::FRONT);// сохранить режим визуализации
		Renderer::_Picked()->SetFaceDrawMode(Renderer::FRONT, Renderer::LINE);// отобразить каркас
		float saveLineWidth = GraphicsLine<Vector3D, ColorRGBA>::_LineWidth();// сохранить толщину линий
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(lineSize);// толщина линий для осей 

		GraphicsMesh<Vector3D, uint32, ColorRGBA, Quaternion, Size3D>::Draw();

		
		Renderer::_Picked()->SetFaceDrawMode(Renderer::FRONT, saveMode);// восстановить режим визуализации для фронтальных граней
		

		// вывод начала координат...
		GraphicsLine<Vector3D, ColorRGBA>::_Draw(Vector3D(0.0f, height, 0.0f), Vector3D(0.0f, 1.0f-height, 0.0f), ColorRGBA::BLUE_COLOR);
		
		GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(saveLineWidth);// толщина линий для осей 
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
	private: Quaternion q;// кватернион поворота и интерполированный кватернион того же поворота в момент времени t
	private: QuaternionSlerper slerper; // путь кватерниона из исходного положения в quaternion по кратчайшей дуге, используя SLERP-интерполяцию
	private: bool slerperEnable;// если true, то запустить проигрывание slerp-интерполяции
	private: float t;// порядковый номер узла интерполяции slerp
	

	private: MyQuad* floor;// пол
    private: SimpleList<GraphicsPoint<Vector3D, ColorRGBA>> pointList;// список целевых точек
	private: int32 amountOfPoints; // количество целевых вершин
    private: float pointSize;// размер целевых точек в квадратных пикселях
    private: int32 pointId;// номер точки, на которую будет наведена камера при клике
	private: int32 currentPoint;// номер текущий точки, на которую нацелена камера


    private: Vector3D worldZOld, worldXOld, worldYOld;// система координат камеры перед интерполяцией
	private: Matrix4x4 defaultRotationMatrix;// матрица вращения камеры по умолчанию
	private: bool moveToDefaultState;// если true, значит включить интерполяцию возврата к ориентации по умолчанию 


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), floor(NIL), slerperEnable(false), t(0.0f), pointSize(8.0f), pointId(0), currentPoint(-1), moveToDefaultState(false)
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
				def.controllerEnabled = false;
				def.movementType = Camera3D::OVERLAND;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);

				defaultRotationMatrix = camera->RotationMatrix();// ориентация камеры по умолчанию
			}

		
			// создать пол...
			{
				floor = new MyQuad();
				scene->AddObject(floor, false, true);
			}
			 

			// сгенерировать целевые вершины...
			{
				float radius = 9;// радиус вершин (расстояние до начала координат)
				amountOfPoints = 10; // количество целевых вершин
		
				for(int i = 0; i < amountOfPoints; ++i)
				{
					GraphicsPoint<Vector3D, ColorRGBA> p;
					p.SetColor(ColorRGBA::GREEN_COLOR);// цвет
			
					// создавать вершины в верхней полусфере
					p.SetPosition(Vector3D(Spherical(radius, Random::_RandomFloat(0, Angle::PI/15), Random::_RandomFloat(-Angle::PI, Angle::PI))));// координаты

					pointList.AddToTail(p);// добавить вершину
				}
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
				moveToDefaultState = false;
			} 
			else// иначе запустить проигрывание анимации
			{
				q = slerper.Interpolate(t);// t измеряется от 0 (identity) до 1 (quaternion)
			
				t += _delta / 2.0f;// перейти к следующему узлу интерполяции SLERP
			}
		}
	}

	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену

		// применить результат интерполяции...
		if(slerperEnable && !moveToDefaultState) // если работает интерполяциял
		{
			// если трансформируется матрица вида напрямую...
			camera->SetRotation(Matrix4x4::_FromQuaternion(q));
		} 
		else if(moveToDefaultState)
		{

			// если меняем ориентацию камеры через трансформацию ее осей...
			// двигаем оси камеры из текущего положения в сторону их начальной ориентации (worldXOld, worldYOld, worldZOld) с помощью обратного кватерниона
			camera->SetRotation(Matrix4x4(q * worldXOld, q * worldYOld, q * worldZOld, Vector3D()));
		}


		// вывод гизмо...
		camera->Begin2D();// начать вывод двухмерной графики

	
		// отобразить компас...
		gizmo.vectorX = camera->RotationMatrix().GetX();
		gizmo.vectorY = camera->RotationMatrix().GetY();
		gizmo.vectorZ = camera->Look();
		gizmo.Draw();
			


		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики
		scene->Draw();



		// вывод целевых вершин...
		float savePointSize = GraphicsPoint<Vector3D, ColorRGBA>::_PointSize();// сохранить размер точек
		GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(pointSize);
		
		// обойти все целевые вершины
		for(int i = 0; i < amountOfPoints; ++i)
		{
			SimpleList<GraphicsPoint<Vector3D, ColorRGBA>>::Node * n = pointList.Get(i);

			if(i == currentPoint)// если выводим точку, на которую нацелена камера
			{
				n->data.SetColor(ColorRGBA::RED_COLOR);// пометить целевую точку
			}
			else { n->data.SetColor(ColorRGBA::GREEN_COLOR); } // обычный цвет для других точек

			n->data.Draw();// вывод вершины
		}
		GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(savePointSize);// восстановить размер точек

		

		// вывод текста...
		camera->Begin2D();// начать вывод двухмерной графики

		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 20, -1), "moving: %d", slerperEnable);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 40, -1), "target points: %d", amountOfPoints);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 60, -1), "current point: %d", pointId);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 80, -1), "camera->position: x: %.2f, y: %.2f, z: %.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
	    
		
		StringANSI string;  
		if(camera->GetMovementType() == Camera3D::OVERLAND) { string = "OVERLAND"; }
		else if(camera->GetMovementType() == Camera3D::FLIGHT) { string = "FLIGHT"; } 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 100, -1), "camera->MovementType: %s", string.c_str());
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 120, -1), "view matrix:");
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 140, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m11, camera->ViewMatrix().m12, camera->ViewMatrix().m13);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 160, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m21, camera->ViewMatrix().m22, camera->ViewMatrix().m23);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 180, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m31, camera->ViewMatrix().m32, camera->ViewMatrix().m33);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 200, -1), "|%.2f|%.2f|%.2f|", camera->ViewMatrix().m41, camera->ViewMatrix().m42, camera->ViewMatrix().m43);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - 220, -1), "camera->look: %.2f/%.2f/%.2f", camera->Look().x, camera->Look().y, camera->Look().z);
	
		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		if(_event.type == Mouse::KEY_DOWN && !slerperEnable)// если нажата клавиша мыши, и если нет активной интерполяции
		{
			if(_event.key == Mouse::LEFT_BUTTON)// левый клик
			{
				Matrix4x4 s1 = camera->RotationMatrix();// взять ориентацию мировой матрицы

				Matrix4x4 s2;// новая матрица вращения
				// создать матрицу взгляда из координат точки pointId в координаты камеры, а не наоборот, т.к. камера всегда ориентирована вдоль отрицательной оси WorldRotationMatrix
				s2.LookAt(pointList.Get(pointId)->data.GetPosition(), camera->GetPosition(), Vector3D(0.0f, 1.0f, 0.0f));
				
				slerperEnable = true;// запустить проигрывание интерполяции	
				slerper.Set(Quaternion(s1), Quaternion(s2));// строим интерполяцию поворота из текущей ориентации камеры в целевую				

				currentPoint = pointId;
		

				// перейти к следующей целевой точке; ходим по кругу
				if(pointId + 1 < amountOfPoints) { pointId++; } 
				else { pointId = 0; }
			}
			else if(_event.key == Mouse::RIGHT_BUTTON)// правый клик
			{ 
				camera->LookAt(camera->GetPosition(), pointList.Get(pointId)->data.GetPosition(), Vector3D::UNIT_Y);// ориентируем камеру в сторону точки pointId
				
				currentPoint = pointId;

				// перейти к следующей целевой точке; ходим по кругу
				if(pointId + 1 < amountOfPoints) { pointId++; } 
				else { pointId = 0; } 
			}
			else if(_event.key == Mouse::MIDDLE_BUTTON)// средний клик
			{ 
				Matrix4x4 s1 = camera->RotationMatrix();// взять ориентацию мировой матрицы
				
				Matrix4x4 s2;

				// создать матрицу взгляда из координат точки 0,0,0 (начало координат) в координаты камеры, а не наоборот, т.к. камера всегда ориентирована вдоль отрицательной оси WorldRotationMatrix
				s2.LookAt(Vector3D(), camera->GetPosition(), Vector3D(0.0f, 1.0f, 0.0f));
				slerperEnable = true;// запустить проигрывание интерполяции	
				
				slerper.Set(Quaternion(s1), Quaternion(s2));

		
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
			else if(_event.key == Keyboard::UP_ARROW)
			{
				camera->Pitch(0.01f);
			}
			else if(_event.key == Keyboard::DOWN_ARROW)
			{
				camera->Pitch(-0.01f);
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				camera->Yaw(0.01f);
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				camera->Yaw(-0.01f);
			}
			else if(_event.key == Keyboard::ENTER)
			{
				camera->Roll(0.01f);
			}
			else if(_event.key == Keyboard::SHIFT)
			{
				camera->Roll(-0.01f);
			}
			else if(_event.key == Keyboard::L && !slerperEnable)
			{
				moveToDefaultState = true;// включить возврат к ориентации камеры по умолчанию

			    slerperEnable = true;// запустить проигрывание интерполяции

				// если меняем ориентацию камеры через трансформацию ее осей...
				Quaternion q1(camera->RotationMatrix());// конвертируем текущую ориентацию камеры в кватернион
				Quaternion q2(defaultRotationMatrix);// конвертируем изначальную ориентацию камеры в кватернион
				q1.Inverse();// меняем направление кватерниона на противоположное; изначально q1 указывает поворот из начального состояния (DefaultWorldMatrix) в текущее (CameraRotationMatrix); инверсия делает кватернион из текущего состояния в начальное

				// строим интерполяцию поворота из начальной ориентации в направлении, противоположном q1
				slerper.Set(q2, q1); 
				
				//запомнить ориентацию осей камеры перед интерполяцией
				worldZOld = camera->RotationMatrix().GetZ();
				worldXOld = camera->RotationMatrix().GetX();
				worldYOld = camera->RotationMatrix().GetY(); 
			}
			else if(_event.key == Keyboard::M)
			{
				if(camera->GetMovementType() == Camera3D::FLIGHT)
				{
					camera->SetMovementType(Camera3D::OVERLAND);
				}
				else if(camera->GetMovementType() == Camera3D::OVERLAND)
				{
					camera->SetMovementType(Camera3D::FLIGHT);
				}
			}
			else if(_event.key == Keyboard::K)
			{
				camera->Move(camera->Look(), 4.0f);
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
	def.realTimeWorkerDef.windowDef.tittle = "Camera3D.example_01";
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
