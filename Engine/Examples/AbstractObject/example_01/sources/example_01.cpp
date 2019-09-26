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
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Helpers/Sprite/AnimatedSprite/AnimatedSprite.h>
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Math/Vector/Vector3D/Vector3D.h>



// класс, реализующий пользовательское окно
class Application: public Engine
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: Scene* scene;// графическая сцена
	private: Object::ObjectType objectObjectType, sceneObjectType;// типы объектов
	private: Object* sprite;// анимированный спрайт

	

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), sprite(NIL)
	{}


	// виртуальный деструктор
	public: virtual ~Application(void)
	{}


	// уничтожает пользовательские ресурсы 
	public: virtual void DestroyUserResources(void)
	{}
	

	// создает пользовательские ресурсы и возвращает true в случае успеха
	public: virtual bool CreateUserResources(void)
	{
		// сцена...
		{
			/*
			// для создания объекта
			sceneObjectType = Object::_CreateObjectType(Object::OBJECT_CLASS | ObjectScene::OBJECT_SCENE_CLASS | LogicScene::LOGIC_SCENE_CLASS | Scene::SCENE_CLASS | LogicScene::LOGIC_OBJECT_CLASS | GraphicsObject::GRAPHICS_OBJECT_CLASS | GraphicsScene::GRAPHICS_SCENE_CLASS);
			scene = dynamic_cast<Scene*>(Object::_CreateObject(sceneObjectType));
			
			if(scene) // если удалось создать объект
			{
				WinAPI::_MessageBox("","Scene is created");
				scene->Rename("scene"); // имя сцены
				AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
				scene->SaveToFile();// сохранить в файл
			}
			*/ 
		
			


	
			// для загрузки
			scene = dynamic_cast<Scene*>(AssetLibrary::_LoadCommonAsset("scene", AssetLibrary::OBJECT_ASSET));

		
		}


		// анимированный спрайт...
		{

			/*
			// для создания объекта
			objectObjectType = Object::_CreateObjectType(IAnimatedSprite::ANIMATED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS);
			sprite = Object::_CreateObject(objectObjectType);
		    
			if(sprite)// если удалось создать объект
			{
				WinAPI::_MessageBox("","Animated sprite is created");
				sprite->Rename("sprite");
				AssetLibrary::_GetAssets().AddObject(sprite);

				sprite->SaveToFile();// сохранить спрайт в файл
			}
			*/ 

		

	
		
		
			// для загрузки объекта
			sprite = AssetLibrary::_LoadCommonAsset<Object>("sprite");


			AssetLibrary::_LoadCommonAsset<TexturedAnimation>("run_right_anim");
			AssetLibrary::_LoadCommonAsset<Atlas2D>("run_right_source");
		 


			// для обеих случаев
			AnimatedSprite<Vector3D, Quaternion, Matrix4x4>* it = dynamic_cast<AnimatedSprite<Vector3D, Quaternion, Matrix4x4>*>(sprite);


			if(it)// если удалось загрузить спрайт
			{
				it->CreateQuad2D(Quad<Vector3D>(Vector3D(), Vector2D(100, 100)));// создать квад 
					
				it->SetAnimatedMaterialEnable(true);// включить материал
				
				
				TexturedAnimation* anim = dynamic_cast<TexturedAnimation*>(AssetLibrary::_GetAssets().FindObject("run_right_anim"));
				
				if(anim)
				{
					Atlas2D* atlas = dynamic_cast<Atlas2D*>(AssetLibrary::_GetAssets().FindObject("run_right_source"));
					
					if(atlas)
					{
						anim->BindAtlas(atlas);
						anim->Play();
						it->BindAnimation(anim);
					}
				}

					
				if(scene)// если сцена существует
				{
					scene->AddLogicObject(it);
					scene->AddGraphicsObject(it); 
				}
			}
		}
			

		// шрифт...
		{
			Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

			Font2D::Define def;
			def.fontName = "Haettenschweiler";// имя шрифта
			def.fontSize = 20;// размер шрифта
			def.fontColor = RGBA(RGBA::WHITE_COLOR);// цвет шрифта

			font2d = new Font2D();
			font2d->Rename("font");
			font2d->Create(def);
			scene->AddObject(font2d);// добавить в сцену
			font2d->Pick();// выбрать шрифт
		}

	 
		// камера...
		{
			Camera3D::Define def;
			def.name = "camera";
			def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
			if(sprite) { def.viewingPoint = (dynamic_cast<Position<Vector3D>*>(sprite))->GetPosition(); }
			def.rotationEnabled = false;

			camera = new Camera3D();
			camera->Create(def);
			scene->AddObject(camera);// добавить в сцену
			camera->Pick(Title()); // выбрать камеру

			// задать минимальный и максимальный зум камеры
			float maxSize = Common::_Max(100, 100);
					
			camera->SetZoomMax(maxSize + Angle::_RadiansToDegrees(camera->Fov()));
			camera->SetZoomMin(maxSize * 0.1);
			camera->SetZoomSpeed(maxSize/10);// задать скорость зуминга 1х10
			camera->SetZFar(maxSize + Angle::_RadiansToDegrees(camera->Fov()) + 1);// задать дальность прорисовки камеры
			camera->SetDistanceToViewingPoint(camera->ZoomMax());// задать расстояние до обозреваемого объекта
		}
 

		// цвет фона...
		SetBackgroundColor(RGBA(RGBA::BLACK_COLOR));
			

		MoveToCenter();// переместить окно в центр крана
		Mouse::_MoveCursorToCenter(this);// переместить курсор в центр клиентской области окна	
		return true;// вернуть успех
	}



	// функция кадра
	public: virtual void Update(float _delta)
	{
		Trigger::_ExecuteActions(); 
		scene->Update(_delta);
	}


	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену

		camera->BeginPerspective3D();// вывод трехмерной графики

		scene->Draw();// отобразить объекты сцены
	
	
	
		// вывод текста...
		camera->BeginScreen2D();// вывод двухмерной графики

		int32 i = 0;
		
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "scene class type: %s", sceneObjectType.ToString().c_str()); 
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "object class type: %s", objectObjectType.ToString().c_str()); 

		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "object class type: %s", sprite->GetStringObjectType().c_str()); 
		//font2d->DrawScreenText(Vector3D(10, ClientAreaSize().y - (i+=20), -1), "object class type: %s", Object::_CreateObjectType(sprite->GetObjectType()).ToString().c_str()); 
		
		

		camera->BeginPerspective3D();// вывод трехмерной графики
	}



	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Mouse().ClearEventList();// очистить буфер событий
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
				Destroy();// уничтожить окно
			} 
		} 

		Keyboard().ClearEventList();// очистить буфер событий
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
	def.rendererDef.realTimeWorkerDef.windowDef.SetTemplateStyle(Window::WND_RESIZING);// стиль окна 
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "Object.example_01";

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
