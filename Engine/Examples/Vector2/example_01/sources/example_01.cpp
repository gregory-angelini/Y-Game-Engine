

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






// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene<Vector3D, Quaternion, Size3D>* scene;// графическая сцена


    private: Vector2D xAxis;// ось X
	private: Vector2D yAxis;// ось Y
	private: Vector2D vector;// тестируемый вектор
	private: float scale;// коэффициент масштабирования вектора

    private: int32 circlePoints;// количество сегментов (степень аппроксимации окружности)
	private: float radius;// радиус круга
	private: float zoom;// зум камеры

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), circlePoints(100), radius(1.0f), vector(0.0f, -1.0f), xAxis(1.0f, 0.0f), yAxis(0.0f, 1.0f), scale(1.1f), zoom(10.0f)
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
				def.position = Vector3D(0, 0, 10);

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera, true, false);
				camera->ConnectTo(ResizeEvent::TYPE, this);
			}

						
			GraphicsPoint<Vector2D, ColorRGBA>::_SetPointSize(6.0);// размер точек
			GraphicsLine<Vector2D, ColorRGBA>::_SetLineWidth(2.0);// толщина линий

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


		
	
		// вывод координатных осей...
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), vector, ColorRGBA(ColorRGBA::YELLOW_COLOR));// тестируемый вектор
		GraphicsPoint<Vector2D, ColorRGBA>::_Draw(vector, ColorRGBA(ColorRGBA::YELLOW_COLOR));

		// координатные оси X,Y
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), xAxis, ColorRGBA(ColorRGBA::RED_COLOR));
		GraphicsPoint<Vector2D, ColorRGBA>::_Draw(xAxis, ColorRGBA::RED_COLOR);
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), yAxis, ColorRGBA(ColorRGBA::CYAN_COLOR));
		GraphicsPoint<Vector2D, ColorRGBA>::_Draw(yAxis, ColorRGBA(ColorRGBA::CYAN_COLOR));
		


		// вывод перпендикулярных векторов
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), Vector2D::_Cross(vector)*2, ColorRGBA(ColorRGBA::WHITE_COLOR));
		GraphicsLine<Vector2D, ColorRGBA>::_Draw(Vector2D(0.0, 0.0), -Vector2D::_Cross(vector)*2, ColorRGBA(ColorRGBA::WHITE_COLOR));






		// вывод круга...
		glColor3f(1.0f, 1.0f, 1.0f);// белый цвет круг
		glBegin(GL_LINE_LOOP);// циклическая линия: конец одной линии - это начало следующей; конец последней линии замыкается с началом начальной линии

		// вычислить сегменты окружности
		for(int i = 0; i < circlePoints; ++i)
		{
			/* примечание: строится замкнутая поверхность, где совпадения начальной и конечной точек - не проверяется,
			   и скорее всего, в результате арифметических округлений, эти координаты будут различаться, тогда могут
		 	   появиться видимые трещины, и круг не будет замкнут. для гарантированного исправления ошибки, 
			   при i = circlePoints-1, синус и косинус должны быть равны 0; или же должна строиться циклическая
			   линия (наше решение) */  
			float angle = Angle::DOUBLE_PI * i / circlePoints;// вычисляем точку окружности
			glVertex2f(cos(angle) * radius, sin(angle) * radius);// ковертируем угол в декартовы координаты
		}
		glEnd();// закончить рисование круга
		

		
	

		// вывод текста...
		camera->Begin2D();// начать вывод двухмерной графики

				
		int i = 0; 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, xAxis): %f", vector.Dot(xAxis));  


		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "Dot(vector, yAxis): %f", vector.Dot(yAxis));  


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.length: %f", vector.Length());


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector(x:%f, y:%f)", vector.x, vector.y);


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.Angle(xAxis): %f radians)", vector.Angle(xAxis));
		

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.Angle(yAxis): %f radians)", vector.Angle(yAxis));


		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "vector.angleZ: %f", Vector2D::_AngleZ(vector));

	
	   

		// визуализировать сцену...
		camera->Begin3D();// начать вывод трехмерной графики
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
		// зум камеры...
		if(_event.type == Mouse::MOVE)
		{}
		else if(_event.type == Mouse::KEY_DOWN)
		{
			if(_event.key == Mouse::LEFT_BUTTON)
			{
			}
			else if(_event.key == Mouse::RIGHT_BUTTON)
			{
			}
		}
		else if(_event.type = Mouse::WHEEL)
		{
			if(Keyboard::_IsKeyPressed(Keyboard::S))
			{
				if(_event.wheel > 0)
				{
					vector*=scale;
				}
				else if(_event.wheel < 0)
				{
					vector/=scale;
				}	
			}
			else // зум камеры
			{
				zoom += _event.wheel; 
				camera->SetPosition(Vector3D(camera->GetPosition().x, camera->GetPosition().y, zoom));
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
				vector.Inverse();
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				vector.Rotate(Angle::DOUBLE_PI/circlePoints);
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				vector.Rotate(-Angle::DOUBLE_PI/circlePoints);
			}
			else if(_event.key == Keyboard::C)
			{
				vector.Cross();
			}
			else if(_event.key == Keyboard::N)
			{
				vector.Normalize();
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
	def.realTimeWorkerDef.windowDef.tittle = "Vector2D.example_01";
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
