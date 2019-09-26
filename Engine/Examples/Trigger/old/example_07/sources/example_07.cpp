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
#include <Engine/Helpers/Engine/Engine.h>
#include <Engine/Core/InputHandler/InputHandler.h>
#include <Engine/Core/AssetLibrary/AssetLibrary.h>
#include <Engine/Core/Trigger/Trigger.h>
#include <Engine/Output/GUI/WinAPI/WinAPI.h>
#include <Engine/Core/Time/TimingHandler/TimingHandler.h>
#include <Engine/Helpers/Sprite/TexturedSprite/TexturedSprite.h>
#include <Engine/Core/Parser/VariableList/VariableList.h>
#include <Engine/Math/Plane/Plane.h>


// пользовательское приложение
class Application: public Engine,// движок
         	       public MessageHandler<int32>::Receiver// получатель сообщений
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D *font2d;// шрифт
	private: Scene* scene;// сцена



    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL)
	{}

			
	// деструктор
	public: virtual ~Application(void)
	{} 



	// создает приложение
	public: bool Create(Engine::Define _define)
	{
		// создаем движок...
		if(Engine::Create(_define))
		{
			// сцена...
			{
				scene = new Scene();
				scene->Rename("scene");// имя сцены
				AssetLibrary::_GetAssets().AddObject(scene);// добавить сцену в библиотеку ассетов, чтобы не потерялась
			}


			GraphicsPoint<Vector3D>::_SetPointSize(15);

			// шрифт...
			{
				Font2D::_CheckSupportedFonts(this);// сгенерировать список поддерживаемых шрифтов

				Font2D::Define def;
				def.fontName = "Arial";// имя шрифта
				def.fontSize = 20;// размер шрифта
				def.fontColor = RGBA(RGBA::BLACK_COLOR);// цвет шрифта

				font2d = new Font2D();
				font2d->Rename("font2d");
				font2d->Create(def);
				scene->AddObject(font2d);// добавить в сцену
				font2d->Pick();// выбрать шрифт
			}



			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;// обзор объекта
				def.rotationEnabled = false;
				def.zoomEnabled = false;

				camera = new Camera3D();
				camera->Create(def);
				scene->AddObject(camera);// добавить в сцену
				camera->Pick(Title()); // выбрать камеру
				
				camera->SetZoomMax(3.3);// максимальное расстояние между камерой и целевым объектом
				//camera->SetZoomMin(3.0);
				camera->SetDistanceToViewingPoint(3.3f);
			}



			{
				// левый клик мыши
				InputHandler* handler = new InputHandler();
			
				Mouse::Event* evt1 = new Mouse::Event();
				evt1->key = Mouse::LEFT_BUTTON;
				evt1->type = Mouse::KEY_DOWN;
				handler->AddEvent(evt1);
				
				handler->Rename("mouseLeftClick");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}

			
			{
				// дижение мыши
				InputHandler* handler = new InputHandler();
			
				Mouse::Event* evt1 = new Mouse::Event();
				evt1->type = Mouse::MOVE;
				handler->AddEvent(evt1);
				
				handler->Rename("mouseMove");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}

	
			{
				// движение влево
				InputHandler* handler = new InputHandler();
			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->type = Keyboard::KEY_PRESSED;
				evt1->key = Keyboard::LEFT_ARROW;
				handler->AddEvent(evt1);
				
				handler->Rename("left");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}
		
			{
				// движение вправо
				InputHandler* handler = new InputHandler();
			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->type = Keyboard::KEY_PRESSED;
				evt1->key = Keyboard::RIGHT_ARROW;
				handler->AddEvent(evt1);
				
				handler->Rename("right");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}

			{
				// движение вперед
				InputHandler* handler = new InputHandler();
			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->type = Keyboard::KEY_PRESSED;
				evt1->key = Keyboard::UP_ARROW;
				handler->AddEvent(evt1);
				
				handler->Rename("up");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}

			{
				// движение назад
				InputHandler* handler = new InputHandler();
			
				Keyboard::Event* evt1 = new Keyboard::Event();
				evt1->type = Keyboard::KEY_PRESSED;
				evt1->key = Keyboard::DOWN_ARROW;
				handler->AddEvent(evt1);
				
				handler->Rename("down");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}

			// переменные...
			{
				Variable<StringANSI>* var1 = new Variable<StringANSI>("base");
				var1->Rename("baseVar");
				AssetLibrary::_GetAssets().AddObject(var1);

				
				Variable<StringANSI>* var2 = new Variable<StringANSI>("tower");
				var2->Rename("towerVar");
				AssetLibrary::_GetAssets().AddObject(var2);


				Variable<int32>* var3 = new Variable<int32>(AssetLibrary::TEXTURE_2D_ASSET);
				var3->Rename("textureAssetType");
				AssetLibrary::_GetAssets().AddObject(var3);


				Variable<bool>* var4 = new Variable<bool>(true);
				var4->Rename("enableVar");
				AssetLibrary::_GetAssets().AddObject(var4);



				Variable<Vector2D>* var5 = new Variable<Vector2D>(Vector2D(53.0/100.0, 95.0/100.0));
				var5->Rename("baseSize");
				AssetLibrary::_GetAssets().AddObject(var5);



				Variable<Vector2D>* var7 = new Variable<Vector2D>(Vector2D(41.0/100.0, 132.0/100.0));
				var7->Rename("towerSize");
				AssetLibrary::_GetAssets().AddObject(var7);


		
				Variable<Vector3D>* var8 = new Variable<Vector3D>(Vector3D(0, 0, -1));
				var8->Rename("towerStartPos");
				AssetLibrary::_GetAssets().AddObject(var8);

				
				Variable<Vector3D>* var9 = new Variable<Vector3D>(Vector3D(0, 0, -1));
				var9->Rename("baseStartPos");
				AssetLibrary::_GetAssets().AddObject(var9);

					
				Variable<Quaternion>* var10 = new Variable<Quaternion>(Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, 0.05f)));
				var10->Rename("rotateToLeft");
				AssetLibrary::_GetAssets().AddObject(var10);

					
				Variable<Quaternion>* var11 = new Variable<Quaternion>(Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, -0.05f)));
				var11->Rename("rotateToRight");
				AssetLibrary::_GetAssets().AddObject(var11);

					
				Variable<Vector3D>* var12 = new Variable<Vector3D>(-Vector3D::UNIT_Y);
				var12->Rename("startDir");
				AssetLibrary::_GetAssets().AddObject(var12);


				Variable<Quaternion>* var13 = new Variable<Quaternion>;
				var13->Rename("curRotate");
				AssetLibrary::_GetAssets().AddObject(var13);

				
				Variable<float>* var14 = new Variable<float>(0.01f);
				var14->Rename("moveSpeed");
				AssetLibrary::_GetAssets().AddObject(var14);


				Variable<Vector3D>* var15 = new Variable<Vector3D>;
				var15->Rename("newPos");
				AssetLibrary::_GetAssets().AddObject(var15);


				Variable<Vector3D>* var16 = new Variable<Vector3D>;
				var16->Rename("curPos");  
				AssetLibrary::_GetAssets().AddObject(var16);

					
				Variable<Plane>* var17 = new Variable<Plane>(Plane(Vector3D(0,0,-1), Vector3D::UNIT_Z));
				var17->Rename("ground");
				AssetLibrary::_GetAssets().AddObject(var17);

				
						
				Variable<Vector3D>* var19 = new Variable<Vector3D>;
				var19->Rename("targetPoint");  
				AssetLibrary::_GetAssets().AddObject(var19);
			}

				
			
			
			{
				// создать башню танка
				Object* object = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS | Scaling<Vector3D>::SCALING_3D_CLASS));
				object->Rename("tankTower");
				AssetLibrary::_GetAssets().AddObject(object);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(object);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(object)); // добавить в графическую сцену
			}



			{
				// загрузка башни танка...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("tankTowerLoader");

			
				
				/*
				trigger1->Rename("tankTowerLoader");
				trigger1->SetActionExpr("loadTower, towerChangeTexture, towerEnableTexturedMaterial, towerCreateQuad, towerSetPosition");


				// действие
				IAction* action1 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "tower", AssetLibrary::TEXTURE_2D_ASSET);
				action1->Rename("loadTower");   
				AssetLibrary::_GetAssets().AddObject(action1);


				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "tankTower", "tower");
				action2->Rename("towerChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "tankTower", true);
				action3->Rename("towerEnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "tankTower", Vector2D(41.0/100.0, 132.0/100.0));
				action4->Rename("towerCreateQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "tankTower", Vector3D(0, 0, -1));
				action5->Rename("towerSetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	


				trigger1->SetEventExpr("");
				trigger1->SaveToFile();
				*/ 
				
				
			
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}

			

			{
				// создать тело танка
				Object* object = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS | Scaling<Vector3D>::SCALING_3D_CLASS));
				object->Rename("tankBase");
				AssetLibrary::_GetAssets().AddObject(object);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(object);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(object)); // добавить в графическую сцену
			}



			{
				// загрузка тела танка...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("tankBaseLoader");

				/*
				trigger1->Rename("tankBaseLoader");
				trigger1->SetActionExpr("loadBase, baseChangeTexture, baseEnableTexturedMaterial, baseCreateQuad, baseSetPosition");


				// действие
				IAction* action1 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "base", AssetLibrary::TEXTURE_2D_ASSET);
				action1->Rename("loadBase");   
				AssetLibrary::_GetAssets().AddObject(action1);


				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "tankBase", "base");
				action2->Rename("baseChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "tankBase", true);
				action3->Rename("baseEnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "tankBase", Vector2D(53.0/100.0, 95.0/100.0));
				action4->Rename("baseCreateQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "tankBase", Vector3D(0, 0, -1));
				action5->Rename("baseSetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

				trigger1->SetEventExpr("");
				trigger1->SaveToFile();
				*/ 
			
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}

			{
				// поведение тела
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("towerControl1");

				/*
				trigger1->Rename("towerControl1");
				trigger1->SetActionExpr("rotateBaseToLeft");


				// условие
				IEvent* event1 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "left");
				event1->Rename("leftPressed");
				AssetLibrary::_GetAssets().AddObject(event1);	


				// действие
				IAction* action1 = Trigger::_CreateAction(Rotation<Quaternion>::DISPLACE_ROTATION_MESSAGE, "tankBase", Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, 0.05f)));
				action1->Rename("rotateBaseToLeft");   
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("leftPressed");
				trigger1->SaveToFile();
			    */ 
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	

				

				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("towerControl2");

				/*
				trigger2->Rename("towerControl2");
				trigger2->SetActionExpr("rotateBaseToRight");


				// условие
				IEvent* event2 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "right");
				event2->Rename("rightPressed");
				AssetLibrary::_GetAssets().AddObject(event2);	


				// действие
				IAction* action2 = Trigger::_CreateAction(Rotation<Quaternion>::DISPLACE_ROTATION_MESSAGE, "tankBase", Quaternion::_FromAxisAngle(AxisAngle(Vector3D::UNIT_Z, -0.05f)));
				action2->Rename("rotateBaseToRight");   
				AssetLibrary::_GetAssets().AddObject(action2);
 
               
				trigger2->SetEventExpr("rightPressed");
				trigger2->SaveToFile();
				*/ 
				
			
		
				AssetLibrary::_GetAssets().AddObject(trigger2);	
				

		
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("towerControl3");

				/*
				trigger3->Rename("towerControl3");
				trigger3->SetActionExpr("getRotation, computeNewPos1, displacePos, getCurPos, updateTowerPos");


				// условие
				IEvent* event3 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "up");
				event3->Rename("upPressed");
				AssetLibrary::_GetAssets().AddObject(event3);	



				// действие
				IAction* action3 = Trigger::_CreateAction(Rotation<Quaternion>::GET_ROTATION_MESSAGE, "tankBase", "curRotate");
				action3->Rename("getRotation");   
				AssetLibrary::_GetAssets().AddObject(action3);

				

				// действие
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "newPos", "curRotate * startDir * moveSpeed", Variable<Vector3D>::VECTOR_3D_TYPE); 
				action4->Rename("computeNewPos1");
				AssetLibrary::_GetAssets().AddObject(action4);


				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::DISPLACE_POSITION_MESSAGE, "#tankBase", "#newPos");
				action5->Rename("displacePos");   
				AssetLibrary::_GetAssets().AddObject(action5); 


				// действие
				IAction* action6 = Trigger::_CreateAction(Position<Vector3D>::GET_POSITION_MESSAGE, "tankBase", "curPos");
				action6->Rename("getCurPos");   
				AssetLibrary::_GetAssets().AddObject(action6);


				// действие
				IAction* action7 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#tankTower", "#curPos");
				action7->Rename("updateTowerPos");   
				AssetLibrary::_GetAssets().AddObject(action7);


				trigger3->SetEventExpr("upPressed");
				trigger3->SaveToFile();
				*/ 
				
		
				AssetLibrary::_GetAssets().AddObject(trigger3);	
				
				

					
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("towerControl4");

				/*
				trigger4->Rename("towerControl4");
				trigger4->SetActionExpr("getRotation, computeNewPos2, displacePos, getCurPos, updateTowerPos");


				// условие
				IEvent* event4 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "down");
				event4->Rename("downPressed");
				AssetLibrary::_GetAssets().AddObject(event4);	



				// действие
				IAction* action8 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "newPos", "curRotate * startDir * -moveSpeed", Variable<Vector3D>::VECTOR_3D_TYPE); 
				action8->Rename("computeNewPos2");
				AssetLibrary::_GetAssets().AddObject(action8);



				trigger4->SetEventExpr("downPressed"); 
				trigger4->SaveToFile();
				
				*/ 
		
				AssetLibrary::_GetAssets().AddObject(trigger4);	
				


				
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("towerControl5");

				/*
				trigger5->Rename("towerControl5");
				trigger5->SetActionExpr("getPointUnderCursor");


				// условие
				IEvent* event5 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "mouseMove");
				event5->Rename("mouseMoveEvent");
				AssetLibrary::_GetAssets().AddObject(event5);	


				// действие
				IAction* action9 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_PLANE_UNDER_CURSOR_MESSAGE, "ground", "", "targetPoint"); 
				action9->Rename("getPointUnderCursor");
				AssetLibrary::_GetAssets().AddObject(action9);



				trigger5->SetEventExpr("mouseMoveEvent");
				trigger5->SaveToFile();
				*/ 
		
				AssetLibrary::_GetAssets().AddObject(trigger5);	 
			}
			
			


			// цвет фона...
			SetBackgroundColor(RGBA(RGBA::WHITE_COLOR));
			
			MoveToCenter();// переместить окно в центр крана
			Mouse::_MoveCursorToCenter(this);// переместить курсор в центр клиентской области окна	
			//Mouse::_ShowCursor(false);// скрыть курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}

	

	// функция кадра
	public: virtual void Update(float _delta)
	{
		scene->Update(_delta);
		Trigger::_Execute();
	}



	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену

		camera->BeginPerspective3D();// вывод трехмерной графики

		scene->Draw();// отобразить объекты сцены
	
	
		//GraphicsPoint<Vector3D>::_Draw(point, RGBA(RGBA::GREEN));
	
		
		camera->BeginScreen2D();// вывод двухмерной графики

		// вывод текста...
		int32 i = 0;


		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "point: %f/%f/%f", point.x, point.y, point.z);

		
		Object* obj = AssetLibrary::_GetAssets().FindObject("tankTower");
	
		if(obj)
		{
			Position<Vector3D>* it = dynamic_cast<Position<Vector3D>*>(obj);
			Vector3D v = it->GetPosition();
			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "base: %f/%f/%f", v.x, v.y, v.z);
		}
		


		obj = AssetLibrary::_GetAssets().FindObject("targetPoint");
	
		if(obj)
		{
			Variable<Vector3D>* it = dynamic_cast<Variable<Vector3D>*>(obj);
			Vector3D v = it->GetValue();
			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "cursor: %f/%f/%f", v.x, v.y, v.z);
		}
		
		
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
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "Trigger.example_07";
	def.rendererDef.zBufferEnabled = false;// необходимо для того, чтобы объекты с одинаковой глубиной отображались без отсечения одного из них


	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
