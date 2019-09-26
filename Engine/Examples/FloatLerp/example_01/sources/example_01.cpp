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
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Interpolation/FloatLerp/FloatLerp.h>
#include <Engine/Output/Graphics/Graph/Graph.h>
#include <Engine/Core/Interpolation/VectorLerp/VectorLerp.h>




// пользовательское приложение
class Application: public Engine,// движок
         	       public MessageHandler<int32>::Receiver// получатель сообщений
{
	// идентификаторы маркеров различных функций
	public: enum
	{
		NO_SELECT,// нет выделения
		UNITS_PER_SECOND,
		FIXED_TIME,
		PARABOLA,
		DAMPING, 
		MAGNETIC,
	};

	// DATA
	private: Camera3D* camera, *camera2d;// камера
    private: Font2D *font;// шрифт
	private: Scene* scene;// сцена

	private: FloatLerp* unitsPerSecondLerp, *fixedTimeLerp, *parabolaLerp, *dampingLerp, *magneticLerp;// интерполятор float-числа
	private: Graph* graph;// график функций
	private: bool rightPressed;// если true, то правая кнопка мыши зажата
	private: int32 index;// индекс текущего выделенного узла
    private: FloatLerp* selected;// выделенная вершина, информацию о которой нужно отобразить
	//private: float accum[5];// числа, которые были получены путем суммирования дельт функций; они должны соответствовать целевому значению интерполяторов
    private: TexturedMesh* clenchedHandCursor;// курсор "сжатая рука"
	private: TexturedMesh* unclenchedHandCursor;// курсор "разжатая рука"
	private: TexturedMesh* arrowCursor;// курсор "стрелка"
	private: Graph::Function *unitsPerSecond, *fixedTime, *parabola, *damping, *magnetic;// графические объекты, которыми представлены функции на графике
	private: int32 selectedMarker;// идентификатор выделенного маркера функции
	private: bool startMarkerMoving;// если true, то маркер перемещается с помощью мыши
	private: Vector3 startMarkerPosition;// стартовая позиция курсора на плоскости графика в момент перемещения маркера функции с помощью мыши


    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font(NIL), scene(NIL), unitsPerSecondLerp(NIL), fixedTimeLerp(NIL), parabolaLerp(NIL), dampingLerp(NIL), magneticLerp(NIL), graph(NIL), rightPressed(false), index(0), selected(NIL), clenchedHandCursor(NIL), unclenchedHandCursor(NIL), arrowCursor(NIL), unitsPerSecond(NIL), fixedTime(NIL), parabola(NIL), damping(NIL), magnetic(NIL), selectedMarker(NO_SELECT), startMarkerMoving(false)
	{
		/*
		for(int32 i = 0; i < 5; i++)
		{
			accum[i] = 0.0f;
		}
		*/ 
	}

			
	// деструктор
	public: virtual ~Application(void)
	{
		delete graph;

		delete unitsPerSecondLerp; 
		delete fixedTimeLerp; 
		delete parabolaLerp; 
		delete dampingLerp;
		delete magneticLerp;

		delete clenchedHandCursor;
		delete unclenchedHandCursor;
		delete arrowCursor;

		delete camera;
		delete camera2d;
		delete font;
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

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 16;// размер шрифта
				def.fontWeight = 600;
				def.color = RGBA(RGBA::GRAY_COLOR);// цвет шрифта

				font = new Font2D();
				font->Create(def);
				font->Pick();// выбрать шрифт
			}


			// камера...
			{
				Camera3D::Define def;
				def.rotationMode = Camera3D::SCROLL_OBJECT_VIEWER;
				def.rotationEnable = false;// запретить вращение камеры
				def.yawClampEnable = true;
				def.pitchClampEnable = true;
				def.zoomSpeed = 3.0f;
				def.zoomEnable = true;

				def.pitchMax = Angle::_DegreesToRadians(40);
				def.pitchMin = Angle::_DegreesToRadians(-40);
				def.yawMax = Angle::_DegreesToRadians(40);
				def.yawMin = Angle::_DegreesToRadians(-40);



				camera = new Camera3D();
				camera->Set(def);

				// задать минимальный и максимальный зум камеры
				camera->SetNearZoom(camera->GetNearZoom() * 100);
				camera->SetFarZoom(camera->GetFarZoom() * 100);
				camera->SetDistanceToViewingPoint(140.0f); 

				camera->Pick(); // выбрать камеру
				scene->BindCamera(camera);
			}
		
			
			// курсор "разжатая рука"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("unclenched");

				unclenchedHandCursor = new TexturedMesh();
				unclenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(24, 29)));
				unclenchedHandCursor->BindAnimation(anim);
				
				unclenchedHandCursor->ConvertTo2DObject();
				unclenchedHandCursor->SetShowEnable(true);// включить отображение
				//scene->AddGraphicsObject(unclenchedHandCursor);// добавить в графическую сцену
			}


			// курсор "сжатая рука"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("clenched");

				clenchedHandCursor = new TexturedMesh();
				clenchedHandCursor->CreateQuad2D(Quad(Vector3(), Vector2(22, 24)));
				clenchedHandCursor->BindAnimation(anim);

				clenchedHandCursor->SetShowEnable(true);// включить отображение
				clenchedHandCursor->ConvertTo2DObject();
			}

			
			// курсор "стрелка"...
			{
				TexturedAnimation* anim = AssetLibrary::_LoadCommonAsset<TexturedAnimation>("cursor");

				arrowCursor = new TexturedMesh();
				arrowCursor->CreateQuad2D(Quad(Vector3(14/2, 24/2, 0), Vector2(14, 24)));
				arrowCursor->BindAnimation(anim);

				arrowCursor->SetShowEnable(true);// включить отображение
				arrowCursor->ConvertTo2DObject();
				Engine::_PickCursor(arrowCursor);// показать курсор
			}

			// камера для двухмерной графики...
			{
				Camera3D::Define def;
				def.projectionMode = Camera3D::SCREEN_PROJECTION;

				def.pitchClampEnable = false;// отключить блокировку вращения вокруг оси X
				
				camera2d = new Camera3D();
				camera2d->Set(def);
			}


			unitsPerSecondLerp = new FloatLerp();
			unitsPerSecondLerp->Rename("UNITS_PER_SECOND");// задать имя 
			unitsPerSecondLerp->SetLerpMode(FloatLerp::UNITS_PER_SECOND);//единицы в секунду
			unitsPerSecondLerp->SetPlayingSpeed(0.7f);// скорость движения в единицах/секунду
			unitsPerSecondLerp->SetTarget(6.0f);// расстояние от 0 до 5 будет пройдено за 2 секунды
			unitsPerSecondLerp->Play();
			scene->AddLogicObject(unitsPerSecondLerp);


			fixedTimeLerp = new FloatLerp();
			fixedTimeLerp->Rename("FIXED_TIME");// задать имя 
			fixedTimeLerp->SetLerpMode(FloatLerp::FIXED_TIME);//единицы в секунду
			fixedTimeLerp->SetPlayingSpeed(7);// скорость движения в секундах
			fixedTimeLerp->SetTarget(15.0f);// расстояние до цели
			fixedTimeLerp->Play();
			scene->AddLogicObject(fixedTimeLerp);


			parabolaLerp = new FloatLerp();
			parabolaLerp->Rename("PARABOLA");// задать имя 
			parabolaLerp->SetLerpMode(FloatLerp::PARABOLA);//единицы в секунду
			parabolaLerp->SetPlayingSpeed(0.1f);// скорость движения 
			parabolaLerp->SetTarget(10.0f);// расстояние до цели
			parabolaLerp->Play();
			scene->AddLogicObject(parabolaLerp);

			//SetParabolaPlayingDurationInSec(0.1f);



			dampingLerp = new FloatLerp();
			dampingLerp->Rename("DAMPING");// задать имя 
			dampingLerp->SetLerpMode(FloatLerp::DAMPING);
			dampingLerp->SetPlayingSpeed(0.5f);// скорость движения 
			dampingLerp->SetTarget(10.0f);// расстояние до цели
			dampingLerp->Play();
			scene->AddLogicObject(dampingLerp);

			//SetDampingPlayingDurationInSec(0.1f);

	
			magneticLerp = new FloatLerp();
			magneticLerp->Rename("MAGNETIC");// задать имя 
			magneticLerp->SetLerpMode(FloatLerp::MAGNETIC);
			magneticLerp->SetPlayingSpeed(1.1f);// скорость движения 
			magneticLerp->SetTarget(00.0f);// расстояние до цели
			magneticLerp->Play();
			scene->AddLogicObject(magneticLerp);

			///SetMagneticPlayingDurationInSec(0.3f);


			// график функций
			{
				graph = new Graph(Vector2(100.0f, 100.0f), 16, 16);
				//graph->SetColor(RGBA(RGBA::GRAY_COLOR));// цвет графика
				//graph->SetXColor(RGBA(RGBA::RED_COLOR));// цвет пометок для оси X
				//graph->SetYColor(RGBA(RGBA::GREEN_COLOR));// цвет пометок для оси Y
				graph->SetScaleX(10.0f);
				//graph->SetScaleY(2.0f);
				//graph->SetCurrentValueShowEnable(false);
			    graph->SetXRangeShowEnable(true);
				graph->SetYRangeShowEnable(true);
				graph->SetXAxisName("Time");
				graph->SetYAxisName("Value");
				
				unitsPerSecond = graph->CreateFunction(unitsPerSecondLerp->GetName());
				//unitsPerSecond->SetColor(RGBA(55, 111, 194, 255));
				unitsPerSecond->SetXMarkerColor(RGBA(55, 111, 194, 255) + RGBA(0, 0, 5, 0));
				unitsPerSecond->SetYMarkerColor(RGBA(55, 111, 194, 255) + RGBA(0, 5, 0, 0));
				unitsPerSecond->SetShowEnable(false);
				unitsPerSecond->SetXMarkerWidth(5.0f);
				unitsPerSecond->SetYMarkerWidth(5.0f);
				
				
				// выделить интерполятор в качестве текущего
				graph->GetMarkList().AddToTail(Graph::Mark(unitsPerSecondLerp->GetTarget() / unitsPerSecondLerp->GetPlayingSpeed(), Graph::Mark::AXIS_X, RGBA(55, 111, 194, 255)));
				
				SelectFunction(FloatLerp::UNITS_PER_SECOND);// задать текущую функцию
				

				fixedTime = graph->CreateFunction(fixedTimeLerp->GetName());
				fixedTime->SetColor(RGBA(118, 185, 0, 255));
				fixedTime->SetXMarkerColor(RGBA(118, 185, 0, 255) + RGBA(0, 0, 5, 0));
				fixedTime->SetYMarkerColor(RGBA(118, 185, 0, 255) + RGBA(0, 5, 0, 0));
				fixedTime->SetShowEnable(false);
				fixedTime->SetXMarkerWidth(5.0f);
				fixedTime->SetYMarkerWidth(5.0f);
				


				parabola = graph->CreateFunction(parabolaLerp->GetName());
				parabola->SetColor(RGBA(163, 21, 21, 255));
				parabola->SetXMarkerColor(RGBA(163, 21, 21, 255) + RGBA(0, 0, 5, 0));
				parabola->SetYMarkerColor(RGBA(163, 21, 21, 255) + RGBA(0, 5, 0, 0));
				parabola->SetShowEnable(false);
				parabola->SetXMarkerWidth(5.0f);
				parabola->SetYMarkerWidth(5.0f);
				
				

				damping = graph->CreateFunction(dampingLerp->GetName());
				damping->SetColor(RGBA(230, 40, 230, 255));
				damping->SetXMarkerColor(RGBA(230, 40, 230, 255) + RGBA(0, 0, 5, 0));
				damping->SetYMarkerColor(RGBA(230, 40, 230, 255) + RGBA(0, 5, 0, 0));
				damping->SetShowEnable(false);
				damping->SetXMarkerWidth(5.0f);
				damping->SetYMarkerWidth(5.0f);
				
				
					
				magnetic = graph->CreateFunction(magneticLerp->GetName());
				magnetic->SetColor(RGBA(247, 170, 0, 255));
				magnetic->SetXMarkerColor(RGBA(247, 170, 0, 255) + RGBA(0, 0, 5, 0));
				magnetic->SetYMarkerColor(RGBA(247, 170, 0, 255) + RGBA(0, 5, 0, 0));
				magnetic->SetShowEnable(false);
				magnetic->SetXMarkerWidth(5.0f);
				magnetic->SetYMarkerWidth(5.0f);
				
				
				scene->AddGraphicsObject(graph);
			}

			

			Mouse::_SetShowCursor(false);// скрыть системный курсор

			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));
			
			MoveToCenter();// переместить окно в центр экрана
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}


	// выбирает функцию с индексом _index в качестве текущей
	public: void SelectFunction(int32 _index)
	{
		if(_index >= 0 && _index < 5)
		{
			if(selected)
			{
				graph->FindFunction(selected->GetName())->SetShowEnable(false);// скрыть 
			}

			index = _index;

			if(index == FloatLerp::UNITS_PER_SECOND)
			{
				selected = unitsPerSecondLerp;
			}
			else if(index == FloatLerp::PARABOLA)
			{
				selected = fixedTimeLerp;
			}
			else if(index == FloatLerp::FIXED_TIME)
			{
				selected = parabolaLerp;
			}
			else if(index == FloatLerp::DAMPING)
			{
				selected = dampingLerp;
			}
			else if(index == FloatLerp::MAGNETIC)
			{
				selected = magneticLerp;
			}
		
			graph->FindFunction(selected->GetName())->SetShowEnable(true);// показать

							
			// обновить целевую отметку на оси времени....
			if(selected->GetLerpMode() == FloatLerp::PARABOLA)
			{
				graph->GetMarkList()[0]->key = Graph::Mark(GetParabolaPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
			if(selected->GetLerpMode() == FloatLerp::MAGNETIC)
			{
				graph->GetMarkList()[0]->key = Graph::Mark(GetMagneticPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
			if(selected->GetLerpMode() == FloatLerp::DAMPING)
			{
				graph->GetMarkList()[0]->key = Graph::Mark(GetDampingPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
			else
			{
				graph->GetMarkList()[0]->key = Graph::Mark(selected->GetPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());	 
			}
		}
	}


	// функция обновления
	public: virtual void Update(float _delta)
	{
		Engine::Update(_delta);

		// обновить значения функций...
		if(unitsPerSecondLerp->IsPlaying())
		{
			//accum[0] += unitsPerSecondLerp->GetDelta();
			graph->AddVertex(unitsPerSecondLerp->GetName(), Vector2(_delta, unitsPerSecondLerp->GetCurrent()));
		}
		
		if(fixedTimeLerp->IsPlaying())
		{
			//accum[1] += fixedTimeLerp->GetDelta();
			graph->AddVertex(fixedTimeLerp->GetName(), Vector2(_delta, fixedTimeLerp->GetCurrent()));
		}

		if(parabolaLerp->IsPlaying())
		{
			//accum[2] += parabolaLerp->GetDelta();
			graph->AddVertex(parabolaLerp->GetName(), Vector2(_delta, parabolaLerp->GetCurrent()));
		}

		if(dampingLerp->IsPlaying())
		{
			//accum[3] += dampingLerp->GetDelta();
			graph->AddVertex(dampingLerp->GetName(), Vector2(_delta, dampingLerp->GetCurrent()));
		}

		if(magneticLerp->IsPlaying())
		{
			//accum[4] += magneticLerp->GetDelta();
			graph->AddVertex(magneticLerp->GetName(), Vector2(_delta, magneticLerp->GetCurrent()));
		}

		CheckCollisionWithMarkers();// проверить попытку захвата маркеров с помощью мыши
	}



	// возвращает строковое представление режима перемещения
	public: StringANSI LerpModeToString(FloatLerp::LerpMode _mode)
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


	// функция визуализации
	public: virtual void Draw(float _delta)
	{
		Engine::Draw(_delta);


		// задать новую трансформацию сцены...
		camera2d->BeginTransform(Renderer::PROJECTION);
		camera2d->BeginTransform(Renderer::MODELVIEW);


		// вывод текста...
		int32 i = 0;



		font->SetColor(graph->FindFunction(selected->GetName())->GetColor());// цвет текста 
		font->DrawScreenText(Vector3(10, i+=20, -1), "name: %s", selected->GetName().c_str());
		font->DrawScreenText(Vector3(10, i+=20, -1), "playing speed: %f", selected->GetPlayingSpeed());
		font->DrawScreenText(Vector3(10, i+=20, -1), "is playing: %d", selected->IsPlaying());
		font->DrawScreenText(Vector3(10, i+=20, -1), "target: %f", selected->GetTarget());
		font->DrawScreenText(Vector3(10, i+=20, -1), "current: %f", selected->GetCurrent());
		font->DrawScreenText(Vector3(10, i+=20, -1), "y maximum: %f", graph->FindFunction(selected->GetName())->GetYMaximum());
		font->DrawScreenText(Vector3(10, i+=20, -1), "delta: %f", selected->GetDelta());
		font->DrawScreenText(Vector3(10, i+=20, -1), "t: %f", selected->GetT());
		font->DrawScreenText(Vector3(10, i+=20, -1), "time to target (sec): %f", selected->GetTimeToTarget());
		//font->DrawScreenText(Vector3(10, i+=20, -1), "accum: %f", accum[selected->GetLerpMode()]);

		font->DrawScreenText(Vector3(10, i+=20, -1), "duration: %f", selected->GetPlayingDurationInSec());

		
		//font->DrawScreenText(Vector3(10, i+=20, -1), "%f", camera->GetDistanceToViewingPoint());
	

		// восстановить предыдущую трансформацию...
		camera2d->EndTransform(Renderer::PROJECTION);
		camera2d->EndTransform(Renderer::MODELVIEW);
	}


	// проверяет пересечение курсора и маркеров, и включает подготовку к захвату маркера в случае успеха
	public: void CheckCollisionWithMarkers(void)
	{
		if(!startMarkerMoving && !rightPressed)// если камера не вращается и маркер не перемещается
		{
			if(unitsPerSecond->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(UNITS_PER_SECOND);
				Engine::_PickCursor(unclenchedHandCursor);// показать курсор
			}
			else if(fixedTime->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(FIXED_TIME);
				Engine::_PickCursor(unclenchedHandCursor);// показать курсор
			}
			else if(parabola->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(PARABOLA);
				Engine::_PickCursor(unclenchedHandCursor);// показать курсор
			}
			else if(damping->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(DAMPING);
				Engine::_PickCursor(unclenchedHandCursor);// показать курсор
			}
			else if(magnetic->GetXMarkerColor() == GetPixelColorUnderCursor())
			{ 
				Select(MAGNETIC);
				Engine::_PickCursor(unclenchedHandCursor);// показать курсор
			}
			else
			{
				Unselect();
				Engine::_PickCursor(arrowCursor);// показать курсор
			}
		}
	}
		

	// возвращает продолжительность интерполяции в секундах для режима PARABOLA
	float GetParabolaPlayingDurationInSec(void)
	{
		/* ВЫЧИСЛЯЕМ ТЕКУЩУЮ ПРОДОЛЖИТЕЛЬНОСТЬ ИНТЕРПОЛЯЦИИ...
		 1. пока целевая позиция не достигнута...
			1.1 вычисляем новый узел интерполяции, используя 1/60 в качестве дельты времени...
			1.2 запоминаем текущее время интерполяции...
		 2. иначе возвращаем текущее время интерполяции. */ 

		float current = 0.0f, t = 0.0f, target = parabolaLerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

		float steps;

		while(current != target)
		{  
			steps = (target - current) / (parabolaLerp->GetPlayingSpeed() * delta);
					
			t += 1.0f / steps;
									 
			if(fabs(t) > 1.0f) 
			{ 
				current = target;
			}
			else 
			{
				current = current * (1.0f - t) + target * t;
			}
			duration += delta;
		}
		return duration;
	}



	// задает целевое врем интерполяции в секундах для режима PARABOLA
	void SetParabolaPlayingDurationInSec(float _duration)
	{
		/* ВЫЧИСЛЯЕМ ЦЕЛЕВУЮ СКОРОСТЬ ДЛЯ ЗАТРЕБОВАННОЙ ПРОДОЛЖИТЕЛЬНОСТИ ФУНКЦИИ...
		   Следующее решение справедливо для функций, где скорость воспроизведения 
		   уменьшает продолжительность интерполяции; для остальных функций порядок 
		   будет несколько иной.
		 1.1 пока целевое время меньше текущей продолжительности интерполяции...
			 1.1.1 увеличивает скорость интерполяции на N...
			 1.1.2 если целевое время стало больше текущей продолжительности интерполяции...
				   1.1.2.1 повышаем скорость интерполяции на N...
				   1.1.2.2 если K <= 0...
						   1.1.2.2.1 останавливаем вычисление...
						   иначе N = N / 10...
						   K = K - 1. 

		  target = 1
		  погрешность 0.1 уже с 16 секунд

		  target = 10
		  погрешность 0.1 уже с 30 секунд
		*/ 

		if(_duration >= 0.1f)
		{
			// чем больше playingSpeed - тем больше скорость функции, и значит тем меньше длительность интерполяции 
			int32 steps = 0; // количество масштабирований шага скорости; минимальный шаг будет составлять TINY
					
			if(parabolaLerp->GetPlayingSpeed() > 0.0f)
			{
				float scale[4] = { 0.1f, 0.001f, 0.0001f, 0.00001f };

				float step = 1.0f;// шаг изменения скорости движения
				float playingSpeed = 0.00001f;// начинаем с максимальной продолжительности интерполяции

				while(true)
				{
					playingSpeed += step;// увеличить скорость
					parabolaLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetParabolaPlayingDurationInSec())// если целевая продолжительность интерполяции стала больше текущей
					{
						playingSpeed -= step;// отменить увеличение скорости
						parabolaLerp->SetPlayingSpeed(playingSpeed);
									
						// повторяем цикл заново, но с меньшим шагом...
						if(steps < 4)// если счетчик не равен 0
						{
							step = scale[steps];// уменьшить размер шага
							++steps;
						}
						else
						{
							break;// прервать цикл
						}
					}
				}
			}
			if(parabolaLerp->GetPlayingSpeed() < 0.0f)
			{
				float scale[4] = { -0.1f, -0.001f, -0.0001f, -0.00001f };

				float step = -1.0f;// шаг изменения скорости движения
				float playingSpeed = -0.00001f;// начинаем с максимальной продолжительности интерполяции

				while(true)
				{
					playingSpeed += step;// увеличить скорость
					parabolaLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetParabolaPlayingDurationInSec())// если целевая продолжительность интерполяции стала больше текущей
					{
						playingSpeed -= step;// отменить увеличение скорости
						parabolaLerp->SetPlayingSpeed(playingSpeed);
									
						// повторяем цикл заново, но с меньшим шагом...
						if(steps < 4)// если счетчик не равен 0
						{
							step = scale[steps];// уменьшить размер шага
							++steps;
						}
						else
						{
							break;// прервать цикл
						}
					}
				}
			}
		}
	}

	
	// возвращает продолжительность интерполяции в секундах для режима MAGNETIC
	float GetMagneticPlayingDurationInSec(void)
	{
		/* ВЫЧИСЛЯЕМ ТЕКУЩУЮ ПРОДОЛЖИТЕЛЬНОСТЬ ИНТЕРПОЛЯЦИИ...
		 1. пока целевая позиция не достигнута...
			1.1 вычисляем новый узел интерполяции, используя 1/60 в качестве дельты времени...
			1.2 запоминаем текущее время интерполяции...
		 2. иначе возвращаем текущее время интерполяции. */ 

		float current = 0.0f, t = 0.0f, target = magneticLerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

		float steps;

		while(current != target)
		{
			steps = (target - current) / (magneticLerp->GetPlayingSpeed() * delta);
			
			t += 1.0f / steps;
							
			if(fabs(t) > 1.0f)
			{  
				current = target;
			}
			else 
			{ 
				current = target * t;
			}
			duration += delta;
		}
		return duration;
	}

	// задает целевое врем интерполяции в секундах для режима MAGNETIC
	void SetMagneticPlayingDurationInSec(float _duration)
	{
		/* ВЫЧИСЛЯЕМ ЦЕЛЕВУЮ СКОРОСТЬ ДЛЯ ЗАТРЕБОВАННОЙ ПРОДОЛЖИТЕЛЬНОСТИ ФУНКЦИИ...
		   Следующее решение справедливо для функций, где скорость воспроизведения 
		   уменьшает продолжительность интерполяции; для остальных функций порядок 
		   будет несколько иной.
		 N = 1
		 K = 7
		 1.1 пока целевое время меньше текущей продолжительности интерполяции...
			 1.1.1 увеличивает скорость интерполяции на N...
			 1.1.2 если целевое время стало больше текущей продолжительности интерполяции...
				   1.1.2.1 повышаем скорость интерполяции на N...
				   1.1.2.2 если K <= 0...
						   1.1.2.2.1 останавливаем вычисление...
						   иначе N = N / 10...
						   K = K - 1. 
		  target = 1
		  погрешность 0.1 уже с 77 секунд

		  target = 10
		  погрешность 0.1 уже с 231 секунд
		*/ 

		if(_duration >= 0.3f)
		{
			// чем больше playingSpeed - тем больше скорость функции, и значит тем меньше длительность интерполяции 
			int32 steps = 0; // количество масштабирований шага скорости; минимальный шаг будет составлять TINY
					
			if(magneticLerp->GetPlayingSpeed() > 0.0f)
			{
				float scale[4] = { 0.1f, 0.001f, 0.0001f, 0.00001f };

				float step = 1.0f;// шаг изменения скорости движения
				float playingSpeed = 0.00001f;// начинаем с максимальной продолжительности интерполяции

				while(true)
				{
					playingSpeed += step;// увеличить скорость
					magneticLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetMagneticPlayingDurationInSec())// если целевая продолжительность интерполяции стала больше текущей
					{
						playingSpeed -= step;// отменить увеличение скорости
						magneticLerp->SetPlayingSpeed(playingSpeed);
									
						// повторяем цикл заново, но с меньшим шагом...
						if(steps < 4)// если счетчик не равен 0
						{
							step = scale[steps];// уменьшить размер шага
							++steps;
						}
						else
						{
							break;// прервать цикл
						}
					}
				}
			}
			if(magneticLerp->GetPlayingSpeed() < 0.0f)
			{
				float scale[4] = { -0.1f, -0.001f, -0.0001f, -0.00001f };

				float step = -1.0f;// шаг изменения скорости движения
				float playingSpeed = -0.00001f;// начинаем с максимальной продолжительности интерполяции

				while(true)
				{
					playingSpeed += step;// увеличить скорость
					magneticLerp->SetPlayingSpeed(playingSpeed);

					if(_duration > GetMagneticPlayingDurationInSec())// если целевая продолжительность интерполяции стала больше текущей
					{
						playingSpeed -= step;// отменить увеличение скорости
						magneticLerp->SetPlayingSpeed(playingSpeed);
									
						// повторяем цикл заново, но с меньшим шагом...
						if(steps < 4)// если счетчик не равен 0
						{
							step = scale[steps];// уменьшить размер шага
							++steps;
						}
						else
						{
							break;// прервать цикл
						}
					}
				}
			}
		}
	}

	// возвращает продолжительность интерполяции в секундах для режима DAMPING
	float GetDampingPlayingDurationInSec(void)
	{
		/* ВЫЧИСЛЯЕМ ТЕКУЩУЮ ПРОДОЛЖИТЕЛЬНОСТЬ ИНТЕРПОЛЯЦИИ...
		 1. пока целевая позиция не достигнута...
			1.1 вычисляем новый узел интерполяции, используя 1/60 в качестве дельты времени...
			1.2 запоминаем текущее время интерполяции...
		 2. иначе возвращаем текущее время интерполяции. */ 

		float current = 0.0f, t = 0.0f, target = dampingLerp->GetTarget(), delta = 1.0f / 60.0f, duration = 0.0f;

		float step = 1.0f / fabs(dampingLerp->GetPlayingSpeed()) * delta;

		while(current != target)
		{
			float previous = current;
		 
			current = current * (1.0f - step) + target * step;
						
			if(fabs(current) >= fabs(target))
			{ 
				current = target; 
			}
			else if(fabs(current - previous) <= 0.000001f)
			{
				current = target;
			}
			duration += delta;
		}
		return duration;
	} 
		

	// задает целевое врем интерполяции в секундах для режима DAMPING
	void SetDampingPlayingDurationInSec(float _duration)
	{
		/* ВЫЧИСЛЯЕМ ЦЕЛЕВУЮ СКОРОСТЬ ДЛЯ ЗАТРЕБОВАННОЙ ПРОДОЛЖИТЕЛЬНОСТИ ФУНКЦИИ...
		   Следующее решение справедливо для функций, где скорость воспроизведения 
		   уменьшает продолжительность интерполяции; для остальных функций порядок 
		   будет несколько иной.

		 1.1 пока целевое время меньше текущей продолжительности интерполяции...
			 1.1.1 увеличивает скорость интерполяции на N...
			 1.1.2 если целевое время стало больше текущей продолжительности интерполяции...
				   1.1.2.1 повышаем скорость интерполяции на N...
				   1.1.2.2 если K <= 0...
						   1.1.2.2.1 останавливаем вычисление...
						   иначе N = N / 10...
						   K = K - 1. 
		  target = 1
		  погрешность 0.1 уже с - секунд

		  target = 10
		  погрешность 0.1 уже с - секунд
		*/ 

		if(_duration >= 0.1f)
		{
			/* чем больше playingSpeed - тем меньше скорость функции, и значит тем больше длительность интерполяции */
			int32 steps = 6; // количество масштабирований шага скорости; минимальный шаг будет составлять TINY
					
			if(dampingLerp->GetPlayingSpeed() > 0.0f)
			{
				float step = 1.0f;// шаг изменения скорости движения
				float playingSpeed = step; 
				dampingLerp->SetPlayingSpeed(playingSpeed);


				if(_duration > GetDampingPlayingDurationInSec())
				{
					while(_duration > GetDampingPlayingDurationInSec())// пока целевая продолжительность функции меньше текущей 
					{
						bool scale = false;

						if((playingSpeed + step) > 0.0f)// если новая скорость будет больше 0
						{
							playingSpeed += step;// увеличить скорость
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// уменьшить размер шага
						}

						if(_duration < GetDampingPlayingDurationInSec())// если целевая продолжительность функции стала больше текущей
						{
							playingSpeed -= step;// отменить увеличение скорости
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// уменьшить размер шага
						}

						if(scale)
						{
							// повторяем цикл заново, но с меньшим шагом...
							if(steps > 0)// если счетчик не равен 0
							{
								--steps;// уменьшить счетчик масштабирований
								step /= 10.0f;// уменьшить размер шага
							}
							else
							{
								break;// прервать цикл
							}
						}
					}
				}
				else if(_duration < GetDampingPlayingDurationInSec())
				{
					while(_duration < GetDampingPlayingDurationInSec())// пока целевая продолжительность функции больше текущей 
					{ 
						bool scale = false;

						if((playingSpeed - step) > 0.0f)// если новая скорость будет больше 0
						{
							playingSpeed -= step;// уменьшить скорость
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// уменьшить размер шага
						}

						if(_duration > GetDampingPlayingDurationInSec())// если целевая продолжительность функции стала меньше текущей
						{
							playingSpeed += step;// отменить уменьшение скорости
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// уменьшить размер шага
						}

						if(scale)
						{
							// повторяем цикл заново, но с меньшим шагом...
							if(steps > 0)// если счетчик не равен 0
							{
								--steps;// уменьшить счетчик масштабирований
								step /= 10.0f;// уменьшить размер шага
							}
							else
							{
								break;// прервать цикл
							}
						}
					}
				}
			} 
			else if(dampingLerp->GetPlayingSpeed() < 0.0f) 
			{ 
				float step = -1.0f;// шаг изменения скорости движения
				float playingSpeed = step;
				dampingLerp->SetPlayingSpeed(playingSpeed);

				if(_duration > GetDampingPlayingDurationInSec())
				{
					while(_duration > GetDampingPlayingDurationInSec())// пока целевая продолжительность функции меньше текущей 
					{
						bool scale = false;

						if((playingSpeed + step) < 0.0f)// если новая скорость будет больше 0
						{
							playingSpeed += step;// увеличить скорость
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// уменьшить размер шага
						}

						if(_duration < GetDampingPlayingDurationInSec())// если целевая продолжительность функции стала больше текущей
						{
							playingSpeed -= step;// отменить увеличение скорости
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// уменьшить размер шага
						}

						if(scale)
						{
							// повторяем цикл заново, но с меньшим шагом...
							if(steps > 0)// если счетчик не равен 0
							{
								--steps;// уменьшить счетчик масштабирований
								step /= 10.0f;// уменьшить размер шага
							}
							else
							{
								break;// прервать цикл
							}
						}
					}
				} 
				else if(_duration < GetDampingPlayingDurationInSec())
				{
					while(_duration < GetDampingPlayingDurationInSec())// пока целевая продолжительность функции больше текущей 
					{ 
						bool scale = false;

						if((playingSpeed - step) < 0.0f)// если новая скорость будет больше 0
						{
							playingSpeed -= step;// уменьшить скорость
							dampingLerp->SetPlayingSpeed(playingSpeed);
						}
						else
						{
							scale = true;// уменьшить размер шага
						}

						if(_duration > GetDampingPlayingDurationInSec())// если целевая продолжительность функции стала меньше текущей
						{
							playingSpeed += step;// отменить уменьшение скорости
							dampingLerp->SetPlayingSpeed(playingSpeed);
							scale = true;// уменьшить размер шага
						}

						if(scale)
						{
							// повторяем цикл заново, но с меньшим шагом...
							if(steps > 0)// если счетчик не равен 0
							{
								--steps;// уменьшить счетчик масштабирований
								step /= 10.0f;// уменьшить размер шага
							}
							else
							{
								break;// прервать цикл
							}
						}
					}
				}
			}
		}
	}



	// фильтр событий мыши
	public: virtual Mouse::Event* MouseEventFilter(Mouse::Event* _event)
	{
		Engine::MouseEventFilter(_event);

		if(_event->type == Mouse::KEY_DOWN)// если кнопка отжата
		{
			if(_event->key == Mouse::LEFT_BUTTON)// если левый клик
			{
				Vector3 currentMarkerMoving;

				if(graph->GetPlane().IsUnderCursor(currentMarkerMoving) != -1)// если есть пересечение курсора с плоскостью графика
				{
					// начало перемещения маркера...
					if(selectedMarker != NO_SELECT && !rightPressed)// если камера не вращается, и есть маркер под курсором
					{
						startMarkerMoving = true;
						Engine::_PickCursor(clenchedHandCursor);// показать курсор
						
						startMarkerPosition = currentMarkerMoving;// запомнить стартовую позицию
					}
				}
			}
		}
		else if(_event->type == Mouse::KEY_PRESSED)
		{
			if(_event->key == Mouse::RIGHT_BUTTON)// начало вращения камеры
			{
				// разрешить вращение камеры только при зажатом правом клике

				if(!rightPressed)// выполняем только один раз
				{
					Mouse::_MoveCursorToCenter(Renderer::_GetPicked());// перемещаем мышь в центр клиентской области окна 
					camera->SetRotationEnable(true);// разрешить вращение камеры
					rightPressed = true;
					Engine::_PickCursor(NIL);
					camera->SetZoomEnable(false);// запретить зуминг
					Unselect();
					//Mouse::_SetShowSystemCursor(false);// скрыть курсор
				}
			}
		}
		else if(_event->type == Mouse::KEY_UP)// иначе кнопка отпущена
		{
			if(_event->key == Mouse::LEFT_BUTTON)// если левый клик
			{
				// конец перемещения маркера...
				if(startMarkerMoving) 
				{ 
					startMarkerMoving = false; // завершить перемещение
					Engine::_PickCursor(unclenchedHandCursor);// показать курсор

					switch(selectedMarker)
					{
						case UNITS_PER_SECOND:
						{
							// перезагружаем интерполятор сначала
							unitsPerSecond->ClearHistory();// очистить историю функции
							// запустить функцию сначала
							unitsPerSecondLerp->Stop();
							unitsPerSecondLerp->Play();
						}
							
						case FIXED_TIME:
						{
							// перезагружаем интерполятор сначала
							fixedTime->ClearHistory();// очистить историю функции
							// запустить функцию сначала
							fixedTimeLerp->Stop();
							fixedTimeLerp->Play();
						}

						case PARABOLA:
						{
							// перезагружаем интерполятор сначала
							parabola->ClearHistory();// очистить историю функции
							// запустить функцию сначала
							parabolaLerp->Stop();
							parabolaLerp->Play();
						}

						case MAGNETIC:
						{
							// перезагружаем интерполятор сначала
							magnetic->ClearHistory();// очистить историю функции
							// запустить функцию сначала
							magneticLerp->Stop();
							magneticLerp->Play();
						}

						case DAMPING:
						{
							// перезагружаем интерполятор сначала
							damping->ClearHistory();// очистить историю функции
							// запустить функцию сначала
							dampingLerp->Stop();
							dampingLerp->Play();
						}
					}
				}
			}
			else if(_event->key == Mouse::RIGHT_BUTTON)// конец вращения камеры
			{
				if(rightPressed)// выполняем только один раз
				{
					camera->SetRotationEnable(false); // запретить вращение камеры
					rightPressed = false;
					Engine::_PickCursor(arrowCursor);
					camera->SetZoomEnable(true);// разрешить зуминг
					//Mouse::_SetShowSystemCursor(true);// показать курсор
				}
			}
		}	
		else if(_event->type == Mouse::WHEEL)// вращение колесика мыши
		{
			if(!rightPressed)// если камера не вращается
			{
				Unselect();
				Engine::_PickCursor(arrowCursor);// показать курсор
			}
		}
		else if(_event->type == Mouse::MOVE)// движение курсора
		{
			if(startMarkerMoving) // если маркер перемещается
			{
				Vector3 currentCursorPosition;

				if(graph->GetPlane().IsUnderCursor(currentCursorPosition) != -1)// если есть пересечение курсора с плоскостью графика
				{
					//Vector3 v = Vector3::_FromTo(startMarkerPosition, currentMarkerPosition);// вектор из начальной точки в текущую

					switch(selectedMarker)
					{
						case UNITS_PER_SECOND:
						{
							// нам нужно найти точку на графике во нутренних координатах, которая соответствует точке курсора на плоскости по оси X
							// формула трансформации внутренних координат в глобальные: t = a * b / c - d, тогда обратная формула: a = (t + d) / b * c, где a - внутренние координаты, t  - гобальные

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// мы используем такой коэффициент масштабирования, чтобы на каждую колонку приходилась 1 секунда
							Vector2 halfSize = graph->GetSize() / 2.0f;// половина от размера графика
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// теперь нам нужно найти такую скорость движения, чтобы цель была достигнула за targetTime секунд; для этого мы воспользуемся уравнением Target / Time = MoveSpeed
							unitsPerSecond->ClearHistory();// очистить историю функции
							unitsPerSecondLerp->SetPlayingDurationInSec(targetTime);
							// запустить функцию сначала
							unitsPerSecondLerp->Stop();
							unitsPerSecondLerp->Play();


							// вычисляем функцию на всех этапах времени; таким образом мы наглядно увидем кривую функции которая получится при текущих настройках
							while(unitsPerSecondLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								unitsPerSecondLerp->Update(fixedDelta);
								graph->AddVertex(unitsPerSecondLerp->GetName(), Vector2(fixedDelta, unitsPerSecondLerp->GetCurrent()));
							}
							// обновить целевую отметку на оси времени....
							graph->GetMarkList()[0]->key = Graph::Mark(selected->GetPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
						
						case FIXED_TIME:
						{
							// нам нужно найти точку на графике во нутренних координатах, которая соответствует точке курсора на плоскости по оси X
							// формула трансформации внутренних координат в глобальные: t = a * b / c - d, тогда обратная формула: a = (t + d) / b * c, где a - внутренние координаты, t  - гобальные

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// мы используем такой коэффициент масштабирования, чтобы на каждую колонку приходилась 1 секунда
							Vector2 halfSize = graph->GetSize() / 2.0f;// половина от размера графика
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// теперь нам нужно найти такую скорость движения, чтобы цель была достигнула за targetTime секунд
							fixedTime->ClearHistory();// очистить историю функции
							fixedTimeLerp->SetPlayingDurationInSec(targetTime);// эквивалентно SetMoveSpeed(targetTime)
							// запустить функцию сначала
							fixedTimeLerp->Stop();
							fixedTimeLerp->Play();


							// вычисляем функцию на всех этапах времени; таким образом мы наглядно увидем кривую функции которая получится при текущих настройках
							while(fixedTimeLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								fixedTimeLerp->Update(fixedDelta);
								graph->AddVertex(fixedTimeLerp->GetName(), Vector2(fixedDelta, fixedTimeLerp->GetCurrent()));
							}
							
							// обновить целевую отметку на оси времени....
							graph->GetMarkList()[0]->key = Graph::Mark(selected->GetPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}

						case PARABOLA:
						{
							// нам нужно найти точку на графике во нутренних координатах, которая соответствует точке курсора на плоскости по оси X
							// формула трансформации внутренних координат в глобальные: t = a * b / c - d, тогда обратная формула: a = (t + d) / b * c, где a - внутренние координаты, t  - гобальные

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// мы используем такой коэффициент масштабирования, чтобы на каждую колонку приходилась 1 секунда
							Vector2 halfSize = graph->GetSize() / 2.0f;// половина от размера графика
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// теперь нам нужно найти такую скорость движения, чтобы цель была достигнула за targetTime секунд
							parabola->ClearHistory();// очистить историю функции
							SetParabolaPlayingDurationInSec(targetTime);
							// запустить функцию сначала
							parabolaLerp->Stop();
							parabolaLerp->Play();


							// вычисляем функцию на всех этапах времени; таким образом мы наглядно увидем кривую функции которая получится при текущих настройках
							while(parabolaLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								parabolaLerp->Update(fixedDelta);
								graph->AddVertex(parabolaLerp->GetName(), Vector2(fixedDelta, parabolaLerp->GetCurrent()));
							}
							// обновить целевую отметку на оси времени....
							graph->GetMarkList()[0]->key = Graph::Mark(GetParabolaPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
						
						case DAMPING:
						{
							// нам нужно найти точку на графике во нутренних координатах, которая соответствует точке курсора на плоскости по оси X
							// формула трансформации внутренних координат в глобальные: t = a * b / c - d, тогда обратная формула: a = (t + d) / b * c, где a - внутренние координаты, t  - гобальные

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// мы используем такой коэффициент масштабирования, чтобы на каждую колонку приходилась 1 секунда
							Vector2 halfSize = graph->GetSize() / 2.0f;// половина от размера графика
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// теперь нам нужно найти такую скорость движения, чтобы цель была достигнула за targetTime секунд
							damping->ClearHistory();// очистить историю функции
							SetDampingPlayingDurationInSec(targetTime);
							// запустить функцию сначала
							dampingLerp->Stop();
							dampingLerp->Play();
							

							// вычисляем функцию на всех этапах времени; таким образом мы наглядно увидем кривую функции которая получится при текущих настройках
							while(dampingLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								dampingLerp->Update(fixedDelta);
								graph->AddVertex(dampingLerp->GetName(), Vector2(fixedDelta, dampingLerp->GetCurrent()));
							}
							// обновить целевую отметку на оси времени....
							graph->GetMarkList()[0]->key = Graph::Mark(GetDampingPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
						
						case MAGNETIC:
						{
							// нам нужно найти точку на графике во нутренних координатах, которая соответствует точке курсора на плоскости по оси X
							// формула трансформации внутренних координат в глобальные: t = a * b / c - d, тогда обратная формула: a = (t + d) / b * c, где a - внутренние координаты, t  - гобальные

							float scaleX = 1.0f * graph->GetSize().x / graph->GetColumns();// мы используем такой коэффициент масштабирования, чтобы на каждую колонку приходилась 1 секунда
							Vector2 halfSize = graph->GetSize() / 2.0f;// половина от размера графика
							float targetTime = (currentCursorPosition.x + halfSize.x) / scaleX * graph->GetScaleX();

							// теперь нам нужно найти такую скорость движения, чтобы цель была достигнула за targetTime секунд
							magnetic->ClearHistory();// очистить историю функции
							SetMagneticPlayingDurationInSec(targetTime);
							// запустить функцию сначала
							magneticLerp->Stop();
							magneticLerp->Play();
							

							// вычисляем функцию на всех этапах времени; таким образом мы наглядно увидем кривую функции которая получится при текущих настройках
							while(magneticLerp->IsPlaying())
							{
								float fixedDelta = 1.0f / 60.0f;
								magneticLerp->Update(fixedDelta);
								graph->AddVertex(magneticLerp->GetName(), Vector2(fixedDelta, magneticLerp->GetCurrent()));
							}
							// обновить целевую отметку на оси времени....
							graph->GetMarkList()[0]->key = Graph::Mark(GetMagneticPlayingDurationInSec(), Graph::Mark::AXIS_X, graph->FindFunction(selected->GetName())->GetColor());
							break;
						}
					}

												
					
					//startMarkerPosition = currentMarkerPosition;// обновить начальную точку
				}
			}
		}
		return _event;
	}


	// выделяет маркер функции _funcId
	public: void Select(int32 _funcId)
	{
		if(selectedMarker != _funcId)// если нужно выделить новый маркер
		{
			Unselect();// снять выделение с предыдущего маркера

			selectedMarker = _funcId;
			Engine::_PickCursor(unclenchedHandCursor);// показать курсор
		}
	}


	// снимает выделение с выделенного маркера
    public: void Unselect(void)
	{
		selectedMarker = NO_SELECT;
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
			else if(_event->key == Keyboard::C)
			{
				graph->ClearHistory();
			} 
			
			else if(_event->key == Keyboard::S)
			{
				graph->ClearHistory();
				
				unitsPerSecondLerp->Stop();
				unitsPerSecondLerp->Play();

				fixedTimeLerp->Stop();
				fixedTimeLerp->Play();

				parabolaLerp->Stop();
				parabolaLerp->Play();
				
				dampingLerp->Stop();
				dampingLerp->Play();

				magneticLerp->Stop();
				magneticLerp->Play();
			} 
			else if(_event->key == Keyboard::LEFT_ARROW)
			{
				index--;

				if(index < 0)
				{
					SelectFunction(4);
				}
				else { SelectFunction(index); }
			}
			else if(_event->key == Keyboard::RIGHT_ARROW)
			{
				index++;  

				if(index > 4) 
				{
					SelectFunction(0);
				}
				else { SelectFunction(index); }
			}
		}

		return Engine::KeyboardEventFilter(_event);
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
	def.rendererDef.realTimeWorkerDef.windowDef.title = "FloatLerp.example_01";
	//def.rendererDef.zBufferEnabled = false;
	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
