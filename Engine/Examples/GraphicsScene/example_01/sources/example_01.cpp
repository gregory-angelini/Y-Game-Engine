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
#include <Engine/Output/Graphics/Shape/Mesh/ColoredMesh/ColoredMesh.h>
#include <Engine/Core/Bitset/Bitset.h>
#include <Engine/Output/Graphics/Atlas/Atlas2D/Atlas2D.h>
#include <Engine/Output/Graphics/Shape/Mesh/TexturedMesh/TexturedMesh.h>
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
	private: Camera3D* camera, *camera2;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene, *scene2;// сцена
	
	private: TexturedMesh* mesh;// текстурированная полигональная сетка
	private: ColoredMesh* box;// трехмерная коробка
	private: Texture2D* texture;// текстура сцены



    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), scene2(NIL), camera2(NIL), mesh(NIL), texture(NIL), box(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete texture;

		delete mesh;
		delete box;

		delete camera;
		delete camera2;

		delete font;

		delete scene2;
		delete scene;
	} 



	// создает приложение
	public: bool Create(Engine::Define _define)
	{
		// создаем движок...
		if(Engine::Create(_define))
		{
			// внешняя сцена...
			{
				scene = new Scene();
				scene->Pick();
			}
		

			// текстура для пиксельного буфера...
			{
				texture = new Texture2D();
				PixelImage img(Vector2(800, 600), RGBA(RGBA::WHITE_COLOR));
				texture->Create(img);
			}

						
			// текстурированный квад для внешней сцены...
			{
				mesh = new TexturedMesh();
				mesh->CreateQuad2D(Quad(Vector3(), Vector2(1,1)));
				
				mesh->BindTexture(texture);

				scene->AddGraphicsObject(mesh);
			}


			// сцена для пиксельного буфера...
			{
				scene2 = new Scene();
				scene2->BindTexture(texture);
				scene->AddGraphicsObject(scene2); 
			}


			// камера для пиксельного буфера...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_FIRST_PERSON;
				def.aspectRatioAutoEnable = false;
				def.aspectRatio = texture->GetImageSize().x / texture->GetImageSize().y;// задаем соотношение сторон вьюпорта, в качестве которого выступает текстура
				//def.rotationEnable = true;
				//def.zoomEnable = true;

				camera2 = new Camera3D();
				camera2->Set(def);

				scene2->BindCamera(camera2);
			}


			// трехмерная коробка для пиксельного буфера...
			{
				box = new ColoredMesh();
				box->CreateBox3D(Box3D(Vector3(), Vector3(0.5,0.5,0.5)));
				box->SetPosition(Vector3(0,0,-2));
				box->SetColor(RGBA(RGBA::RED_COLOR));

				scene2->AddGraphicsObject(box);
			}
  

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontWeight = Font2D::WEIGHT_BOLD;
				def.color = RGBA(RGBA::GRAY_COLOR);// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}


			// камера для внешней сцены...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				def.rotationEnable = true;
				def.zoomEnable = true;

				camera = new Camera3D();
				camera->Set(def);

				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
			}
			

			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));

			MoveToCenter();// переместить окно в центр экрана
			Mouse::_MoveCursorToCenter(this);
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}



	// функция обновления
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		// вращаем коробку вокруг оси Y
		box->SetRotation(box->GetRotation() * Quaternion(Matrix4x4::_RotationY(_delta)));
	}



	// функция визуализации
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);
	}


	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);
		return _event;
	}


	// обработка событий клавиатуры
	public: virtual Keyboard::Event* KeyboardEventFilter(Keyboard::Event* _event)
	{
		Engine::KeyboardEventFilter(_event);

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
			else if(_event->key == Keyboard::D)
			{
				texture->Destroy();
			}
			else if(_event->key == Keyboard::S)
			{
				PixelImage img;
				texture->GetImage(img);
				img.SaveImageToPNGFile("pbuffer.png");
			}
		} 

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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "GraphicsScene.example_01";
	//def.rendererDef.zBufferEnabled = true;

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
