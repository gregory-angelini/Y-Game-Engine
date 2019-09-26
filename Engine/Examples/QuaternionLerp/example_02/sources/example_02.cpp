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
#include <Engine/Core/Control/PlayerControl/PlayerControl.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>
#include <Engine/Core/Interpolation/QuaternionLerp/QuaternionLerp.h>



// пользовательское приложение
class Application: public Engine,// движок
               	   public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: Scene* scene, *scene2d;// сцена
    private: Camera3D* camera, *camera2d;// камера
    private: Font2D *font;// шрифт
	private: GraphicsSurface* box;// трехмерная коробка
    private: FloatLerp* fl;// числовой интерполятор
	private: QuaternionLerp* ql;// кватернионный интерполятор
	private: float time;// длительность интерполяции в секундах


    // METHODS
    // инициализирующий конструктор
	public: Application(void): scene(NIL), scene2d(NIL), camera(NIL), camera2d(NIL), font(NIL), box(NIL), fl(NIL), ql(NIL), time(0.0f)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete font;

		delete box;

		delete fl;
		delete ql;

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
				//def.rotationEnable = true;
				def.zoomEnable = true;

				camera = new Camera3D();
				camera->Create(def);

				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
			}


			// двухмерная сцена...
			{
				scene2d = new Scene();
				scene2d->BindCamera(camera2d);

				scene->AddToRenderList(scene2d);
			}
						

			// двухмерная камера...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				camera2d = new Camera3D();
				camera2d->Create(def);

				scene2d->BindCamera(camera2d);
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

			{// числовой интерполятор
				fl = new FloatLerp();
				//fl->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);
				//fl->SetPlayingDurationInSec(10.0f);
				fl->SetTarget(10.0f);
				fl->SetPlayingSpeed(0.01f);
				fl->SetLerpMode(FloatLerp::PARABOLA);

				
				scene->AddToUpdateList(fl);
			}

			{// кватернионный интерполятор
				ql = new QuaternionLerp();
				ql->BindFloatLerp(fl);
				ql->GetControlQuaternions().AddToTail(Quaternion());
				ql->GetControlQuaternions().AddToTail(Quaternion(Matrix4x4::_RotationZ(Angle::HALF_PI)));
				ql->GetControlQuaternions().AddToTail(Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)) * Quaternion(Matrix4x4::_RotationZ(Angle::HALF_PI)));
				ql->GetControlQuaternions().AddToTail(Quaternion(Quaternion(Matrix4x4::_RotationX(Angle::HALF_PI)) * Quaternion(Matrix4x4::_RotationY(Angle::HALF_PI)) * Matrix4x4::_RotationZ(Angle::HALF_PI)));
			}
			
			{// коробка
				box = new GraphicsSurface();

				Mesh* mesh = new Mesh();
				mesh->CreateBox3D(Box(Vector3(), Vector3(0.5, 0.5, 0.5)));
				
				box->BindMesh(mesh);

				box->SetColor4(RGBA(RGBA::WHITE_COLOR).GetRGBAf());
				scene->AddToRenderList(box);
			}


			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::BLACK_COLOR).GetRGBAf());

			SetWireframeEnable(true);// включить каркасный режим визуализации

			MoveToCenter();// переместить окно в центр экрана
			//Mouse::_SetShowCursor(false);// скрыть курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}


	// возвращает текущий режим воспроизведения в виде строки
	public: StringANSI GetPlayingModeString(int32 _mode)const
	{
		StringANSI str;

		if(_mode & PlayingObject::FORWARD_PLAYING)
		{
			str += "FORWARD_PLAYING | ";
		}
		if(_mode & PlayingObject::BACKWARD_PLAYING)
		{
			str += "BACKWARD_PLAYING | ";
		}
		if(_mode & PlayingObject::SINGLE_SHOT_PLAYING)
		{
			str += "SINGLE_SHOT_PLAYING | ";
		}
		if(_mode & PlayingObject::LOOP_PLAYING)
		{
			str += "LOOP_PLAYING | ";
		}
		if(_mode & PlayingObject::PING_PONG_PLAYING)
		{
			str += "PING_PONG_PLAYING";
		}
		if(_mode & PlayingObject::ONEWAY_PLAYING)
		{
			str += "ONEWAY_PLAYING";
		}
		return str;
	}

	// возвращает строковое представление режима перемещения
	public: StringANSI LerpModeToString(int32 _mode)
	{
		switch(_mode)
		{
			case FloatLerp::UNITS_PER_SECOND:
			{
				return "UNITS_PER_SECOND";
			}

			case FloatLerp::PARABOLA:
			{
				return "PARABOLA";
			}

			case FloatLerp::FIXED_TIME:
			{
				return "FIXED_TIME";
			}

			case FloatLerp::DAMPING:
			{
				return "DAMPING";
			}
			
			case FloatLerp::MAGNETIC:
			{
				return "MAGNETIC";
			}
		}
		return "";
	}


	// функция обновления
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		box->SetRotation(ql->GetCurrent());
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

		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp playing: %d", fl->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp current: %f", fl->GetCurrent());
		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp delta: %f", fl->GetDelta());
		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp t: %f", fl->GetT());

		font->DrawScreenText(Vector3(10, i+=20, -1), "f-lerp mode: %s", LerpModeToString(fl->GetLerpMode()).c_str());
		font->DrawScreenText(Vector3(10, i+=20, -1), "playing mode: %s", GetPlayingModeString(fl->GetPlayingMode()).c_str()); 

	
		//if(fl->IsPlaying()) { time += _delta; }// если интерполятор воспроизводится

		font->DrawScreenText(Vector3(10, i+=20, -1), "time: %f", fl->GetElapsedTimeInSec()); 



		// восстановить предыдущую трансформацию...
		camera2d->EndTransform(Renderer::MODELVIEW);
		camera2d->EndTransform(Renderer::PROJECTION);
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
			else if(_event->key == Keyboard::P)
			{
				if(fl->IsPlaying()) 
				{
					fl->Pause(); 
				}
				else
				{
					if(fl->IsFinish()) 
					{ 
						fl->Stop(); 
						time = 0.0f;
					} 
					fl->Play();
				}
			}
			else if(_event->key == Keyboard::B)
			{
				fl->SetPlayingMode(PlayingObject::BACKWARD_PLAYING);
			}
			else if(_event->key == Keyboard::F)
			{
				fl->SetPlayingMode(PlayingObject::FORWARD_PLAYING);
			}
			else if(_event->key == Keyboard::L)
			{
				fl->SetPlayingMode(PlayingObject::LOOP_PLAYING);
			}
			else if(_event->key == Keyboard::S)
			{
				fl->SetPlayingMode(PlayingObject::SINGLE_SHOT_PLAYING);
			}
			else if(_event->key == Keyboard::G)
			{
				fl->SetPlayingMode(PlayingObject::PING_PONG_PLAYING);
			}
			else if(_event->key == Keyboard::O)
			{
				fl->SetPlayingMode(PlayingObject::ONEWAY_PLAYING);
			}
			else if(_event->key == Keyboard::NUM_1)
			{
				fl->SetLerpMode(FloatLerp::PARABOLA);
				time = 0.0f;
				fl->SetPlayingSpeed(0.001f);
			}
			else if(_event->key == Keyboard::NUM_2)
			{
				fl->SetLerpMode(FloatLerp::FIXED_TIME);
				time = 0.0f;
				fl->SetPlayingDurationInSec(3.0f);
			}
			else if(_event->key == Keyboard::NUM_3)
			{
				fl->SetLerpMode(FloatLerp::UNITS_PER_SECOND);
				time = 0.0f;
				fl->SetPlayingDurationInSec(2.0f);
			}
			else if(_event->key == Keyboard::NUM_4)
			{
				fl->SetLerpMode(FloatLerp::DAMPING);
				time = 0.0f;
				fl->SetPlayingSpeed(2.15f);
			}
			else if(_event->key == Keyboard::NUM_5)
			{
				fl->SetLerpMode(FloatLerp::MAGNETIC);
				time = 0.0f;
				fl->SetPlayingSpeed(2.0f);
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
	def.rendererDef.realTimeWorkerDef.windowDef.clientAreaSize.Set(800, 600);// размер окна
	def.rendererDef.realTimeWorkerDef.windowDef.hSmallIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)); // загрузить иконку small.ico из ресурсов
	def.rendererDef.realTimeWorkerDef.windowDef.title = "QuaternionLerp.example_02";
	//def.rendererDef.zBufferEnabled = true;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
