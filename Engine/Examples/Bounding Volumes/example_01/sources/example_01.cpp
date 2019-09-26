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
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>


// пользовательское приложение
class Application: public Engine// движок
{
	// DATA
	private: Camera3D* camera, *camera2d;
    private: Font2D *font;// шрифт
	private: Scene *scene, *scene2d;
	private: GraphicsSurface* shape;// фигура
	private: bool rightPressed;// если true, то правая кнопка мыши зажата
	private: GraphicsPoint* intersectedPoint;// точка пересечения селектирующего луча и фигуры
	private: Mesh* mesh; // полигональная сетка


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), camera2d(NIL), font(NIL), scene(NIL), scene2d(NIL), shape(NIL), intersectedPoint(NIL), rightPressed(false), mesh(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete mesh;
		delete shape;
		delete intersectedPoint;

		delete font;

		delete camera2d;
		delete camera;

		delete scene2d;
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
				scene->Pick();
			}
		

			// камера...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::OBJECT_VIEWER_ROTATION;
				def.rotationEnable = false;
				def.zoomEnable = true;

				camera = new Camera3D();
				camera->Create(def);

				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
			}


			// двухмерная сцена...
			{
				scene2d = new Scene();

				scene->AddToRenderList(scene2d);
			}
						

			// двухмерная камера...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;
				def.distanceToViewingPoint = 1.0f;

				camera2d = new Camera3D();
				camera2d->Create(def); 
				
				scene2d->BindCamera(camera2d);
				//camera2d->Pick(); // выбрать камеру
			}

			
			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::WHITE_COLOR).GetRGBAf();// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}


			// графический объект...
			{
				shape = new GraphicsSurface();
				
				mesh = new Mesh();
				mesh->CreateBox3D(Box(Vector3(), Vector3(0.5, 0.5, 0.5)));
				mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);

				shape->BindMesh(mesh);
		
				shape->SetColor(RGBA(RGBA::SKY_COLOR));
				shape->SetBoundingVolumeShowEnable(true);//включить отображение ограничивающего объема
				scene->AddToRenderList(shape);
			}


			// точка пересечения...
			{
				intersectedPoint = new GraphicsPoint();
				intersectedPoint->SetColor(RGBA(RGBA::GREEN_COLOR));
				intersectedPoint->SetShowEnable(false);

				scene->AddToRenderList(intersectedPoint);
			}


			SetBackgroundColor(RGBA(RGBA::GRAY_COLOR));// цвет фона

			//GraphicsLine::_SetLineWidth(2.0f);// толщина линий
			GraphicsPoint::_SetPointSize(6.0f);// размер точек

			//SetWireframeEnable(true);// каркасный режим визуализации

			MoveToCenter();// переместить окно в центр экрана
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

		font->DrawScreenText(Vector3(10, i+=20, -1), "%d", intersectedPoint->IsShowEnabled());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel position: %d/%d", (int32)object->GetIntersectedPixelPosition().x, (int32)object->GetIntersectedPixelPosition().y);
		//font->DrawScreenText(Vector3(10, i+=20, -1), "intersected pixel color: %d/%d/%d/%d", object->GetIntersectedPixelColor().red, object->GetIntersectedPixelColor().green, object->GetIntersectedPixelColor().blue, object->GetIntersectedPixelColor().alpha);
		//font->DrawScreenText(Vector3(10, i+=20, -1), "texture-pos: %f/%f", object->texturePosition.x, object->texturePosition.y);


		
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of objects: %d", scene->AmountOfObjects()); 
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of logic objects: %d", scene->AmountOfLogicObjects()); 
		//font->DrawScreenText(Vector3(10, i+=20, 0), "scene.amount of graphics objects: %d", scene->AmountOfGraphicsObjects()); 
		


		// восстановить предыдущую трансформацию...
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		if(_event->type == Mouse::KEY_DOWN)
		{
			if(_event->key == Mouse::LEFT_BUTTON)
			{
				Vector3 point;

				if(shape->IsIntersected(camera->GetSelectRay(Mouse::_GetCursorPosition(this)), &point) != -1)
				{
					intersectedPoint->SetPosition(point);// обновить позицию точки
					intersectedPoint->SetShowEnable(true);
				}
				else
				{
					intersectedPoint->SetShowEnable(false);
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)
			{
				// начало вращения камеры...
				if(!rightPressed)// выполняем только один раз
				{
					camera->SetRotationEnable(true); // разрешить вращение камеры
					rightPressed = true;

					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// перемещаем мышь в центр клиентской области окна 
					Mouse::_SetShowCursor(false);
				}
			}
		}
		else if(_event->type == Mouse::KEY_UP)
		{
			if(_event->key == Mouse::RIGHT_BUTTON)
			{
				// конец вращения камеры...
				if(rightPressed)// выполняем только один раз
				{
					camera->SetRotationEnable(false);// запретить вращение камеры 
					rightPressed = false;
					Mouse::_SetShowCursor(true);
				}
			}
		}

		Engine::MouseEventFilter(_event);
		return _event;
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
				Destroy();
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				mesh->CreateBox3D(Box(Vector3(), Vector3(0.5, 0.5, 0.5)));
				mesh->SetBoundingVolumeType(Mesh::BOUNDING_SPHERE);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				mesh->CreateIcosphere3D(2);
				mesh->SetBoundingVolumeType(Mesh::BOUNDING_BOX);
			}

		}

		Engine::KeyboardEventFilter(_event);
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
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "TexturingSurface.example_01";
	def.rendererDef.zBufferEnable = false;

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
