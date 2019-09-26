

#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// для текстур

#include <Engine/Math/Ratio/Ratio.h>

#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Output/Graphics/GraphicsScene/GraphicsScene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>

#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Shape/Sphere/Sphere3D/Sphere3D.h>
#include <Engine/Helpers/Scene/Scene.h>




// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: Scene* scene;// графическая сцена
	private: GraphicsMesh<Vector3D, Quaternion, Matrix4x4>* box;// коробка
	private: StringANSI currentProjectionType;// имя текущего типа проекции
	private: float screenAxisXOffset;// смещение по экранной оси X; для правильного отображения текста, когда ось X растет справа налево

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), box(NIL), screenAxisXOffset(0.0f)
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
				scene = new Scene();
			}


			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.textColor = RGBA(RGBA::WHITE_COLOR);
				def.name = "font2d";
				def.fontName = "Haettenschweiler";
				def.fontSize = 20;

				font2d = new Font2D();
				font2d->Create(def);
				scene->AddObject(font2d, false, false);
				font2d->Pick();
			}

			// куб...
			{
				GraphicsMesh<Vector3D, Quaternion, Matrix4x4>::Define def;
				def.meshDef.name = "box3d";
				
				
				
				box = new GraphicsMesh<Vector3D, Quaternion, Matrix4x4>();
				box->Create(def);
				box->CreateBox3D(Box3D(Vector3D(0, 0, 0), Size3D(2.5f, 2.5f, 7.0f)));
				
				scene->AddObject(box, false, true);// добавить в сцену
			} 

			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
		
				def.movementType = Camera3D::OVERLAND;
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
				def.pitchMax = 0.0f;// запретить камере опускаться ниже пола (плоскости XZ)
			    def.viewingPoint = box->GetPosition();
			    def.zoomMin = 15.0f;
				def.zoomMax = 25.0f;
				def.orthoScale = 5.0;// делаем масштаб в два раза больше размера box
				


				camera = new Camera3D();
				camera->Create(def);
				camera->Pick("Camera3D.example_05"); 
				scene->AddObject(camera);


				// задать изометрический ракурс камеры
				camera->UpdateTrackballObjectViewer(-0.06, 0.1, 0.0f);

				currentProjectionType = "PERSPECTIVE";
			}


			GraphicsPoint<Vector3D>::_SetPointSize(6);// размер точек 
			
			Mouse::_ShowCursor(false);// скрыть курсор
			MoveToCenter();// переместить окно в центр экрана
			Mouse::_MoveCursorToCenter(this);// переместить курсор в центр клиентской области окна
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


		if(currentProjectionType.compare("ORTHOGRAPHIC")==0)
		{
			camera->BeginOrtho2D();
		}
		else if(currentProjectionType.compare("PERSPECTIVE")==0)
		{
			camera->BeginPerspective3D();// начать вывод трехмерной графики	
		}
		
		scene->Draw();// отображение сцены
		
		//if(camera->GetProjectionType() == Camera3D::ORTHOGRAPHIC)
		//{
		//	camera->End2D();// начать вывод трехмерной графики	
		//}

		
		
 
		// вывод текста...
		camera->BeginScreen2D();
		

		int i = 0;
		font2d->DrawScreenText(Vector3D(screenAxisXOffset + 10, i+=20, -1), "camera.position: %.2f/%.2f/%.2f", camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
		font2d->DrawScreenText(Vector3D(screenAxisXOffset + 10, i+=20, -1), "camera.projection type: %s", currentProjectionType.c_str());
	}

			


	// фильтр событий мыши
	public: virtual bool MouseEventFilter(Mouse::Event _event)
	{
		Mouse().ClearEventList();// очистить буфер событий
		return true;
	}


	// обработка событий клавиатуры
	public: virtual bool KeyboardEventFilter(Keyboard::Event _event)
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
			else if(_event.key == Keyboard::O)
			{
				currentProjectionType = "ORTHOGRAPHIC";
			}
			else if(_event.key == Keyboard::P)
			{
				currentProjectionType = "PERSPECTIVE";
			}
			else if(_event.key == Keyboard::Q)
			{
				camera->SetScreenProjectionOrigin(Camera3D::LEFT_TOP_CORNER);
				screenAxisXOffset = 0;
			}
			else if(_event.key == Keyboard::W)
			{
				camera->SetScreenProjectionOrigin(Camera3D::RIGHT_TOP_CORNER);
				screenAxisXOffset = 240;
			}
			else if(_event.key == Keyboard::A)
			{
				camera->SetScreenProjectionOrigin(Camera3D::LEFT_BOTTOM_CORNER);
				screenAxisXOffset = 0;
			}
			else if(_event.key == Keyboard::S)
			{
				camera->SetScreenProjectionOrigin(Camera3D::RIGHT_BOTTOM_CORNER);
				screenAxisXOffset = 240;
			}
		}

		Keyboard().ClearEventList();// очистить буфер событий
		return true;
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
	def.realTimeWorkerDef.windowDef.tittle = "Camera3D.example_05";
	def.realTimeWorkerDef.unsuspended = false;
	//def.realTimeWorkerDef.sleep = false;
	
	//def.faceDrawMode[Renderer::FRONT] = Renderer::LINE;


	//def.faceDrawMode[Renderer::BACK] = Renderer::POINT;
	//def.faceCullMode[Renderer::FRONT] = false;
	//def.faceCullMode[Renderer::BACK] = false;
	//def.frontFaceClockwise = true;
	//def.realTimeWorkerDef.windowDef.eventBuffering = true;


	Application* app = new Application;// создать окно
	
	if(app->Application::Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
