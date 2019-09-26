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



			// событие ввода...
			{
				InputHandler* handler = new InputHandler();
			
				Mouse::Event* evt1 = new Mouse::Event();
				evt1->key = Mouse::LEFT_BUTTON;
				evt1->type = Mouse::KEY_DOWN;
				handler->AddEvent(evt1);
				
				handler->Rename("mouseLeftClick");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}

			// событие ввода...
			{
				InputHandler* handler = new InputHandler();
			
				Mouse::Event* evt1 = new Mouse::Event();
				evt1->type = Mouse::MOVE;
				handler->AddEvent(evt1);
				
				handler->Rename("mouseMove");
				handler->SetActivationMode(InputHandler::SINGLE);
				
				AssetLibrary::_GetAssets().AddObject(handler);
			}
			// переменные...
			{
				Variable<Vector3D>* var1 = new Variable<Vector3D>(Vector3D(-1, 0.9, 0));
				var1->Rename("pos1");
				AssetLibrary::_GetAssets().AddObject(var1);


				Variable<Vector3D>* var2 = new Variable<Vector3D>(Vector3D(-0.35, 0.9, 0));
				var2->Rename("pos2");
				AssetLibrary::_GetAssets().AddObject(var2);


				Variable<Vector3D>* var3 = new Variable<Vector3D>(Vector3D(0.3, 0.9, 0));
				var3->Rename("pos3");
				AssetLibrary::_GetAssets().AddObject(var3);


				Variable<Vector3D>* var4 = new Variable<Vector3D>(Vector3D(-1, 0, 0));
				var4->Rename("pos4");
				AssetLibrary::_GetAssets().AddObject(var4);


				Variable<Vector3D>* var5 = new Variable<Vector3D>(Vector3D(-0.35, 0, 0));
				var5->Rename("pos5");
				AssetLibrary::_GetAssets().AddObject(var5);


				Variable<Vector3D>* var6 = new Variable<Vector3D>(Vector3D(0.3, 0, 0));
				var6->Rename("pos6");
				AssetLibrary::_GetAssets().AddObject(var6);
	

				Variable<Vector3D>* var7 = new Variable<Vector3D>(Vector3D(-1, -0.9, 0));
				var7->Rename("pos7");
				AssetLibrary::_GetAssets().AddObject(var7);


				Variable<Vector3D>* var8 = new Variable<Vector3D>(Vector3D(-0.35, -0.9, 0));
				var8->Rename("pos8");
				AssetLibrary::_GetAssets().AddObject(var8);


				Variable<Vector3D>* var9 = new Variable<Vector3D>(Vector3D(0.3, -0.9, 0));
				var9->Rename("pos9");
				AssetLibrary::_GetAssets().AddObject(var9);
			}


			{
				Variable<Vector3D>* var1 = new Variable<Vector3D>(Vector3D());
				var1->Rename("cardPos1");
				AssetLibrary::_GetAssets().AddObject(var1);


				Variable<Vector3D>* var2 = new Variable<Vector3D>(Vector3D());
				var2->Rename("cardPos2");
				AssetLibrary::_GetAssets().AddObject(var2);

			
				Variable<Vector3D>* var3 = new Variable<Vector3D>(Vector3D());
				var3->Rename("cardPos3");
				AssetLibrary::_GetAssets().AddObject(var3);
	

				Variable<Vector3D>* var4 = new Variable<Vector3D>(Vector3D());
				var4->Rename("cardPos4");
				AssetLibrary::_GetAssets().AddObject(var4);


				Variable<Vector3D>* var5 = new Variable<Vector3D>(Vector3D());
				var5->Rename("cardPos5");
				AssetLibrary::_GetAssets().AddObject(var5);

			
				Variable<Vector3D>* var6 = new Variable<Vector3D>(Vector3D());
				var6->Rename("cardPos6");
				AssetLibrary::_GetAssets().AddObject(var6);


				Variable<Vector3D>* var7 = new Variable<Vector3D>(Vector3D());
				var7->Rename("cardPos7");
				AssetLibrary::_GetAssets().AddObject(var7);


				Variable<Vector3D>* var8 = new Variable<Vector3D>(Vector3D());
				var8->Rename("cardPos8");
				AssetLibrary::_GetAssets().AddObject(var8);

			
				Variable<Vector3D>* var9 = new Variable<Vector3D>(Vector3D());
				var9->Rename("cardPos9");
				AssetLibrary::_GetAssets().AddObject(var9);
			}

			{
				Variable<Quaternion>* var1 = new Variable<Quaternion>(Matrix4x4::_RotationY(Angle::PI));
				var1->Rename("rotateToBack");
				AssetLibrary::_GetAssets().AddObject(var1);
					
		
				Variable<Quaternion>* var2 = new Variable<Quaternion>(Quaternion());
				var2->Rename("rotateToFront");
				AssetLibrary::_GetAssets().AddObject(var2);

					
				Variable<Vector2D>* var3 = new Variable<Vector2D>(Vector2D(3.0f/5.0f,4.3f/5.0f));
				var3->Rename("cardSize");
				AssetLibrary::_GetAssets().AddObject(var3);

				 		
				Variable<StringANSI>* var4 = new Variable<StringANSI>("positions");
				var4->Rename("positionsVar");
				AssetLibrary::_GetAssets().AddObject(var4);

						
				Variable<int32>* var5 = new Variable<int32>(IVariableList::TAIL);
				var5->Rename("tail");
				AssetLibrary::_GetAssets().AddObject(var5);


				Variable<int32>* var6 = new Variable<int32>(Variable<Vector3D>::VECTOR_3D_TYPE);
				var6->Rename("listTypeVar");
				AssetLibrary::_GetAssets().AddObject(var6);


				Variable<int32>* var7 = new Variable<int32>(0);
				var7->Rename("0");
				AssetLibrary::_GetAssets().AddObject(var7);

				
				Variable<int32>* var8 = new Variable<int32>(1);
				var8->Rename("1");
				AssetLibrary::_GetAssets().AddObject(var8);

				
				Variable<int32>* var9 = new Variable<int32>(2);
				var9->Rename("2");
				AssetLibrary::_GetAssets().AddObject(var9);


				Variable<int32>* var10 = new Variable<int32>(3);
				var10->Rename("3");
				AssetLibrary::_GetAssets().AddObject(var10);

					
				Variable<int32>* var11 = new Variable<int32>(4);
				var11->Rename("4");
				AssetLibrary::_GetAssets().AddObject(var11);

				
				Variable<int32>* var12 = new Variable<int32>(5);
				var12->Rename("5");
				AssetLibrary::_GetAssets().AddObject(var12);

				
				Variable<int32>* var13 = new Variable<int32>(6);
				var13->Rename("6");
				AssetLibrary::_GetAssets().AddObject(var13);

				
				Variable<int32>* var14 = new Variable<int32>(7);
				var14->Rename("7");
				AssetLibrary::_GetAssets().AddObject(var14);


				Variable<int32>* var15 = new Variable<int32>(8);
				var15->Rename("8");
				AssetLibrary::_GetAssets().AddObject(var15);		

				
				Variable<StringANSI>* var16 = new Variable<StringANSI>("backFace");
				var16->Rename("backFaceVar");
				AssetLibrary::_GetAssets().AddObject(var16);


				Variable<StringANSI>* var17 = new Variable<StringANSI>("front1");
				var17->Rename("front1Var");
				AssetLibrary::_GetAssets().AddObject(var17);

				
				Variable<int32>* var18 = new Variable<int32>(AssetLibrary::TEXTURE_2D_ASSET);
				var18->Rename("textureAssetType");
				AssetLibrary::_GetAssets().AddObject(var18);

				
				Variable<bool>* var19 = new Variable<bool>(true);
				var19->Rename("enableVar");
				AssetLibrary::_GetAssets().AddObject(var19);

					
				Variable<int32>* var20 = new Variable<int32>(9);
				var20->Rename("swapsVar");
				AssetLibrary::_GetAssets().AddObject(var20);

					
				Variable<StringANSI>* var21 = new Variable<StringANSI>("front2");
				var21->Rename("front2Var");
				AssetLibrary::_GetAssets().AddObject(var21);

				
				Variable<StringANSI>* var22 = new Variable<StringANSI>("front3");
				var22->Rename("front3Var");
				AssetLibrary::_GetAssets().AddObject(var22);
			} 


			// состояние карт
			{
				Variable<bool>* var1 = new Variable<bool>(false);
				var1->Rename("card1UnderCursor");  
				AssetLibrary::_GetAssets().AddObject(var1);

	
				Variable<bool>* var3 = new Variable<bool>(false);
				var3->Rename("card1Visibled");
				AssetLibrary::_GetAssets().AddObject(var3);

					
				Variable<bool>* var4 = new Variable<bool>(false);
				var4->Rename("card2UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var4);

				
				Variable<bool>* var5 = new Variable<bool>(false);
				var5->Rename("card2Visibled");
				AssetLibrary::_GetAssets().AddObject(var5);
				
						
				Variable<bool>* var6 = new Variable<bool>(false);
				var6->Rename("card3UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var6);

				
				Variable<bool>* var7 = new Variable<bool>(false);
				var7->Rename("card3Visibled");
				AssetLibrary::_GetAssets().AddObject(var7);


				Variable<bool>* var8 = new Variable<bool>(false);
				var8->Rename("card4UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var8);

				
				Variable<bool>* var9 = new Variable<bool>(false);
				var9->Rename("card4Visibled");
				AssetLibrary::_GetAssets().AddObject(var9);


				Variable<bool>* var10 = new Variable<bool>(false);
				var10->Rename("card5UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var10);

				
				Variable<bool>* var11 = new Variable<bool>(false);
				var11->Rename("card5Visibled");
				AssetLibrary::_GetAssets().AddObject(var11);

				
				Variable<bool>* var12 = new Variable<bool>(false);
				var12->Rename("card6UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var12);

				
				Variable<bool>* var13 = new Variable<bool>(false);
				var13->Rename("card6Visibled");
				AssetLibrary::_GetAssets().AddObject(var13);
				
				
				Variable<StringANSI>* var14 = new Variable<StringANSI>("front1Opened");
				var14->Rename("front1Opened");
				AssetLibrary::_GetAssets().AddObject(var14);


				Variable<StringANSI>* var15 = new Variable<StringANSI>("front2Opened");
				var15->Rename("front2Opened");
				AssetLibrary::_GetAssets().AddObject(var15);


				Variable<StringANSI>* var16 = new Variable<StringANSI>("front3Opened");
				var16->Rename("front3Opened");
				AssetLibrary::_GetAssets().AddObject(var16);


						
				Variable<bool>* var17 = new Variable<bool>(false);
				var17->Rename("card7UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var17);

				
				Variable<bool>* var18 = new Variable<bool>(false);
				var18->Rename("card7Visibled");
				AssetLibrary::_GetAssets().AddObject(var18);

				
				Variable<bool>* var19 = new Variable<bool>(false);
				var19->Rename("card8UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var19);

				
				Variable<bool>* var20 = new Variable<bool>(false);
				var20->Rename("card8Visibled");
				AssetLibrary::_GetAssets().AddObject(var20);

					
				Variable<bool>* var21 = new Variable<bool>(false);
				var21->Rename("card9UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(var21);

				
				Variable<bool>* var22 = new Variable<bool>(false);
				var22->Rename("card9Visibled");
				AssetLibrary::_GetAssets().AddObject(var22);
				

				Variable<bool>* var23 = new Variable<bool>(false);
				var23->Rename("card1UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var23);
				
	
				Variable<Vector3D>* var24 = new Variable<Vector3D>(Vector3D(1.1, 1.1, 0));
				var24->Rename("scaleMax");
				AssetLibrary::_GetAssets().AddObject(var24);
				
					
				Variable<Vector3D>* var25 = new Variable<Vector3D>(Vector3D(1.0, 1.0, 0));
				var25->Rename("scaleMin");
				AssetLibrary::_GetAssets().AddObject(var25);


				Variable<bool>* var26 = new Variable<bool>(false);
				var26->Rename("card2UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var26);
				

				Variable<bool>* var27 = new Variable<bool>(false);
				var27->Rename("card3UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var27);

		
				Variable<bool>* var28 = new Variable<bool>(false);
				var28->Rename("card4UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var28);
				

				Variable<bool>* var29 = new Variable<bool>(false);
				var29->Rename("card5UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var29);

				
				Variable<bool>* var30 = new Variable<bool>(false);
				var30->Rename("card6UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var30);
				

				Variable<bool>* var31 = new Variable<bool>(false);
				var31->Rename("card7UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var31);

				
				Variable<bool>* var32 = new Variable<bool>(false);
				var32->Rename("card8UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var32);


				
				Variable<bool>* var33 = new Variable<bool>(false);
				var33->Rename("card9UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(var33);
			}
			



			{ 
				// создать список
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("listCreator");

				/*
			
		
				trigger1->Rename("listCreator");
				trigger1->SetActionExpr("createList, addToList1, addToList2, addToList3, addToList4, addToList5, addToList6, addToList7, addToList8, addToList9, randomize, getCardPos1, getCardPos2, getCardPos3, getCardPos4, getCardPos5, getCardPos6, getCardPos7, getCardPos8, getCardPos9");


				// действие
				IAction* action1 = Trigger::_CreateAction(IVariableList::CREATE_LIST_MESSAGE, "positions", Variable<Vector3D>::VECTOR_3D_TYPE);      
				action1->Rename("createList");
				AssetLibrary::_GetAssets().AddObject(action1);	  


				// действие 
				IAction* action2 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos1", IVariableList::TAIL);    
				action2->Rename("addToList1");
				AssetLibrary::_GetAssets().AddObject(action2);	 

					
				
				// действие
				IAction* action3 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos2", IVariableList::TAIL); 
				action3->Rename("addToList2");
				AssetLibrary::_GetAssets().AddObject(action3);	

				
					
				// действие
				IAction* action4 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos3", IVariableList::TAIL); 
				action4->Rename("addToList3");
				AssetLibrary::_GetAssets().AddObject(action4);	


				// действие
				IAction* action5 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos4", IVariableList::TAIL);
				action5->Rename("addToList4");
				AssetLibrary::_GetAssets().AddObject(action5);	

					
				// действие
				IAction* action6 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos5", IVariableList::TAIL);
				action6->Rename("addToList5");
				AssetLibrary::_GetAssets().AddObject(action6);	

					
				// действие
				IAction* action7 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos6", IVariableList::TAIL);
				action7->Rename("addToList6");
				AssetLibrary::_GetAssets().AddObject(action7);	


					
				// действие
				IAction* action8 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos7", IVariableList::TAIL);
				action8->Rename("addToList7");
				AssetLibrary::_GetAssets().AddObject(action8);	

					
				// действие
				IAction* action9 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos8", IVariableList::TAIL);
				action9->Rename("addToList8");
				AssetLibrary::_GetAssets().AddObject(action9);	

					
				// действие
				IAction* action10 = Trigger::_CreateAction(IVariableList::ADD_TO_LIST_MESSAGE, "positions", "pos9", IVariableList::TAIL);
				action10->Rename("addToList9");
				AssetLibrary::_GetAssets().AddObject(action10);	
				

			 
				// действие
				IAction* action11 = Trigger::_CreateAction(IVariableList::RANDOMIZE_LIST_MESSAGE, "positions", 9); 
				action11->Rename("randomize");  
				AssetLibrary::_GetAssets().AddObject(action11);	

				
				// действие
				IAction* action12 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos1", 0);
				action12->Rename("getCardPos1");  
				AssetLibrary::_GetAssets().AddObject(action12);	

			
					
				// действие
				IAction* action13 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos2", 1); 
				action13->Rename("getCardPos2");
				AssetLibrary::_GetAssets().AddObject(action13);

					
				// действие
				IAction* action14 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos3", 2);
				action14->Rename("getCardPos3");
				AssetLibrary::_GetAssets().AddObject(action14);


				// действие
				IAction* action15 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos4", 3);
				action15->Rename("getCardPos4");
				AssetLibrary::_GetAssets().AddObject(action15);	

					
				// действие
				IAction* action16 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos5", 4);
				action16->Rename("getCardPos5");
				AssetLibrary::_GetAssets().AddObject(action16);	

					
				// действие
				IAction* action17 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos6", 5);
				action17->Rename("getCardPos6");
				AssetLibrary::_GetAssets().AddObject(action17);	
	

				// действие
				IAction* action18 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos7", 6);
				action18->Rename("getCardPos7");
				AssetLibrary::_GetAssets().AddObject(action18);	

										
				// действие
				IAction* action19 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos8", 7);
				action19->Rename("getCardPos8");
				AssetLibrary::_GetAssets().AddObject(action19);	

										
				// действие
				IAction* action20 = Trigger::_CreateAction(IVariableList::GET_FROM_LIST_MESSAGE, "positions", "cardPos9", 8);
				action20->Rename("getCardPos9");
				AssetLibrary::_GetAssets().AddObject(action20);	
			
            

				trigger1->SetEventExpr("");
				trigger1->SaveToFile();
				*/ 
			

				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}


			
		
			{
				// загрузка общих ресурсов
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("TextureLoader");

				
				/*
				
				trigger1->Rename("TextureLoader");
				trigger1->SetActionExpr("cardLoadBackFace, card1LoadTexture, card2LoadTexture, card3LoadTexture");

								
				// действие
				IAction* action1 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "backFace", AssetLibrary::TEXTURE_2D_ASSET);// включить текстурированный материал
				action1->Rename("cardLoadBackFace");  
				AssetLibrary::_GetAssets().AddObject(action1);


				// действие 
				IAction* action2 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "front1", AssetLibrary::TEXTURE_2D_ASSET);
				action2->Rename("card1LoadTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	

	
				// действие 
				IAction* action3 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "front2", AssetLibrary::TEXTURE_2D_ASSET);
				action3->Rename("card2LoadTexture");
				AssetLibrary::_GetAssets().AddObject(action3);	

					
				// действие 
				IAction* action4 = Trigger::_CreateAction(AssetLibrary::LOAD_ASSET_MESSAGE, "front3", AssetLibrary::TEXTURE_2D_ASSET);
				action4->Rename("card3LoadTexture");
				AssetLibrary::_GetAssets().AddObject(action4);	



				trigger1->SetEventExpr(""); 
				trigger1->SaveToFile();
				*/ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	
			} 

			
			{// общие ресурсы
				// условие
				IEvent* event1 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "mouseLeftClick");
				event1->Rename("click"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// условие
				IEvent* event2 = Trigger::_CreateEvent(InputHandler::INPUT_EVENT_MESSAGE, "mouseMove");
				event2->Rename("mouseMoveEvent"); 
				AssetLibrary::_GetAssets().AddObject(event2);
			} 
				
			
			
			{
				// создать первую карту
				Object* card1 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card1->Rename("card1");
				AssetLibrary::_GetAssets().AddObject(card1);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card1);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card1)); // добавить в графическую сцену
			}
			
			{
				// создать обратную сторону карты
				Object* card1 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card1->Rename("card1BackFace");
				AssetLibrary::_GetAssets().AddObject(card1);


				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card1);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card1)); // добавить в графическую сцену
			}


			{
				// загрузка первой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card1Creator");

				
				/*
				trigger1->Rename("card1Creator");
				trigger1->SetActionExpr("card1ChangeTexture, card1EnableTexturedMaterial, card1CreatQuad, card1SetPosition, cardBackFace1ChangeTexture, cardBackFace1EnableTexturedMaterial, cardBackFace1CreatQuad, cardBackFace1SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card1", "front1");
				action2->Rename("card1ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card1", true);// включить текстурированный материал
				action3->Rename("card1EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card1", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card1CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card1", "#cardPos1");
				action5->Rename("card1SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card1BackFace", "backFace");  
				action7->Rename("cardBackFace1ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card1BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace1EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card1BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace1CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card1BackFace", "#cardPos1");
				action10->Rename("cardBackFace1SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
				*/ 
			
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}

 
			{
				// поведение первой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard1_part1");
				
				/*
				
				trigger1->Rename("clickOnCard1_part1");
				trigger1->SetActionExpr("checkCard1UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card1BackFace", "card1UnderCursor", "", "");    
				action1->Rename("checkCard1UnderCursor"); 
				AssetLibrary::_GetAssets().AddObject(action1);  


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
				 
			
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger1);	


				

				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard1_part2");
				
				/*
				trigger2->Rename("clickOnCard1_part2"); 
				trigger2->SetActionExpr("deactivatedCard1, sendFront1Opened, openCard1_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card1Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard1_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front1", "front1Opened");
				action3->Rename("sendFront1Opened"); 
				AssetLibrary::_GetAssets().AddObject(action3);

								
				// действия
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card1UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard1"); 
				AssetLibrary::_GetAssets().AddObject(action4); 

				

				trigger2->SetEventExpr("card1UnderCursor & ~card1Visibled");  
				trigger2->SaveToFile();
				*/ 
			
				AssetLibrary::_GetAssets().AddObject(trigger2);	



				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard1_part3");
				
				/*
				trigger3->Rename("clickOnCard1_part3");
				trigger3->SetActionExpr("openCard1_2, openCard1_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card1", Quaternion());
				action5->Rename("openCard1_2"); 
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card1BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard1_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card1Visibled");  
				trigger3->SaveToFile();
				*/ 
			
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard1_part4"); 
				
				/*
				trigger4->Rename("clickOnCard1_part4");
				trigger4->SetActionExpr("closeCard1_1");	 
				 

				// условие
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front1Opened");
				event1->SetOverlappingEnable(true); 
				event1->Rename("card1VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card1Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard1_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card1Visibled & ~card1VisibledMessage");  
				trigger4->SaveToFile(); 
			    */ 
			     
				AssetLibrary::_GetAssets().AddObject(trigger4);	



	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard1_part5"); 
				

				/*
				trigger5->Rename("clickOnCard1_part5");
				trigger5->SetActionExpr("closeCard1_2, closeCard1_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card1", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard1_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card1BackFace", Quaternion()); 
				action9->Rename("closeCard1_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card1Visibled");  
				trigger5->SaveToFile();
			    */

				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}
				
	
		

			{
				// создать вторую карту...
				Object* card2 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card2->Rename("card2");
				AssetLibrary::_GetAssets().AddObject(card2);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card2);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card2)); // добавить в графическую сцену
			}
				
			{
				// создать обратную сторону карты
				Object* card2 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card2->Rename("card2BackFace");
				AssetLibrary::_GetAssets().AddObject(card2);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card2);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card2)); // добавить в графическую сцену
			}

			{
				// загрузка второй карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card2Creator");

			
				/* 
				trigger1->Rename("card2Creator");
				trigger1->SetActionExpr("card2ChangeTexture, card2EnableTexturedMaterial, card2CreatQuad, card2SetPosition, cardBackFace2ChangeTexture, cardBackFace2EnableTexturedMaterial, cardBackFace2CreatQuad, cardBackFace2SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие 
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card2", "front1");
				action2->Rename("card2ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card2", true);// включить текстурированный материал
				action3->Rename("card2EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card2", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card2CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card2", "#cardPos2");
				action5->Rename("card2SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card2BackFace", "backFace");  
				action7->Rename("cardBackFace2ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card2BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace2EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card2BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace2CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card2BackFace", "#cardPos2");
				action10->Rename("cardBackFace2SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
				*/ 
			
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}

			
	
			{
				// поведение второй карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard2_part1");
				
				
				/*
				trigger1->Rename("clickOnCard2_part1");
				trigger1->SetActionExpr("checkCard2UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card2BackFace", "card2UnderCursor", "", "");    
				action1->Rename("checkCard2UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			    */ 
			     
				AssetLibrary::_GetAssets().AddObject(trigger1);	


				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard2_part2");
				
				
				/*
				trigger2->Rename("clickOnCard2_part2"); 
				trigger2->SetActionExpr("deactivatedCard2, sendFront1Opened, openCard2_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card2Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard2_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE,  "front1", "front1Opened");
				action3->Rename("sendFront1Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card2UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard2");
				AssetLibrary::_GetAssets().AddObject(action4);

				

				trigger2->SetEventExpr("card2UnderCursor & ~card2Visibled");  
				trigger2->SaveToFile();
			    */


				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard2_part3");
				
				/*
				
				trigger3->Rename("clickOnCard2_part3");
				trigger3->SetActionExpr("openCard2_2, openCard2_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card2", Quaternion());
				action5->Rename("openCard2_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card2BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard2_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card2Visibled");  
				trigger3->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard2_part4"); 
				
				/*
				
				trigger4->Rename("clickOnCard2_part4");
				trigger4->SetActionExpr("closeCard2_1");	 
				 

				// условие
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front1Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card2VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card2Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard2_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card2Visibled & ~card2VisibledMessage");  
				trigger4->SaveToFile(); 
			    */

				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard2_part5"); 
				
				
				/*
				trigger5->Rename("clickOnCard2_part5");
				trigger5->SetActionExpr("closeCard2_2, closeCard2_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card2", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard2_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card2BackFace", Quaternion()); 
				action9->Rename("closeCard2_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card2Visibled");  
				trigger5->SaveToFile();
			    */ 
				 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}

			
			
			 
			{
				// создать третью карту...
				Object* card3 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card3->Rename("card3");
				AssetLibrary::_GetAssets().AddObject(card3);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card3);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card3)); // добавить в графическую сцену
			}
				
			{
				// создать обратную сторону карты
				Object* card3 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card3->Rename("card3BackFace");
				AssetLibrary::_GetAssets().AddObject(card3);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card3);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card3)); // добавить в графическую сцену
			}

			{
				// загрузка третьей карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card3Creator");

				/*
				trigger1->Rename("card3Creator");
				trigger1->SetActionExpr("card3ChangeTexture, card3EnableTexturedMaterial, card3CreatQuad, card3SetPosition, cardBackFace3ChangeTexture, cardBackFace3EnableTexturedMaterial, cardBackFace3CreatQuad, cardBackFace3SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие 
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card3", "front1");
				action2->Rename("card3ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card3", true);// включить текстурированный материал
				action3->Rename("card3EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card3", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card3CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card3", "#cardPos3");
				action5->Rename("card3SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card3BackFace", "backFace");  
				action7->Rename("cardBackFace3ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card3BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace3EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card3BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace3CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card3BackFace", "#cardPos3");
				action10->Rename("cardBackFace3SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
			    */ 
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}
		
			
			
			{ 
				// поведение третьей карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard3_part1");
				
				
				/* 
				trigger1->Rename("clickOnCard3_part1");
				trigger1->SetActionExpr("checkCard3UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card3BackFace", "card3UnderCursor", "", "");    
				action1->Rename("checkCard3UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			     
				AssetLibrary::_GetAssets().AddObject(trigger1);	

                */ 
				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard3_part2");
				
				/* 
				
				trigger2->Rename("clickOnCard3_part2"); 
				trigger2->SetActionExpr("deactivatedCard3, sendFront1Opened, openCard3_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card3Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard3_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front1", "front1Opened");
				action3->Rename("sendFront1Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card3UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard3");
				AssetLibrary::_GetAssets().AddObject(action4);

				

				trigger2->SetEventExpr("card3UnderCursor & ~card3Visibled");  
				trigger2->SaveToFile();
			    */

				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard3_part3");
				
				
				/*
				trigger3->Rename("clickOnCard3_part3");
				trigger3->SetActionExpr("openCard3_2, openCard3_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card3", Quaternion());
				action5->Rename("openCard3_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card3BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard3_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card3Visibled");  
				trigger3->SaveToFile();
			    */

				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard3_part4"); 
				
				/*
				
				trigger4->Rename("clickOnCard3_part4");
				trigger4->SetActionExpr("closeCard3_1");	 
				 

				// условие
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front1Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card3VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card3Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard3_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card3Visibled & ~card3VisibledMessage");  
				trigger4->SaveToFile();  
			    */

				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard3_part5"); 
				
				/*
				trigger5->Rename("clickOnCard3_part5");
				trigger5->SetActionExpr("closeCard3_2, closeCard3_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card3", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard3_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card3BackFace", Quaternion()); 
				action9->Rename("closeCard3_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card3Visibled");  
				trigger5->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}

			
			


			{
				// создать четвертую карту...
				Object* card4 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card4->Rename("card4");
				AssetLibrary::_GetAssets().AddObject(card4);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card4);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card4)); // добавить в графическую сцену
			}
				
			{
				// создать обратную сторону карты
				Object* card4 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card4->Rename("card4BackFace");
				AssetLibrary::_GetAssets().AddObject(card4);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card4);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card4)); // добавить в графическую сцену
			}

		
			{
				// загрузка четвертой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card4Creator");

			
				/*
				
				trigger1->Rename("card4Creator");
				trigger1->SetActionExpr("card4ChangeTexture, card4EnableTexturedMaterial, card4CreatQuad, card4SetPosition, cardBackFace4ChangeTexture, cardBackFace4EnableTexturedMaterial, cardBackFace4CreatQuad, cardBackFace4SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card4", "front2"); 
				action2->Rename("card4ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card4", true);// включить текстурированный материал
				action3->Rename("card4EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card4", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card4CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card4", "#cardPos4"); 
				action5->Rename("card4SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card4BackFace", "backFace");  
				action7->Rename("cardBackFace4ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card4BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace4EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card4BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace4CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card4BackFace", "#cardPos4");
				action10->Rename("cardBackFace4SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
			    */ 
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}


		
			
			{  
				// поведение четвертой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard4_part1");
				
				
				/*
				trigger1->Rename("clickOnCard4_part1");
				trigger1->SetActionExpr("checkCard4UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card4BackFace", "card4UnderCursor", "", "");    
				action1->Rename("checkCard4UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			    */

				AssetLibrary::_GetAssets().AddObject(trigger1);	


				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard4_part2");
				
				
				/*
				trigger2->Rename("clickOnCard4_part2"); 
				trigger2->SetActionExpr("deactivatedCard4, sendFront2Opened, openCard4_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card4Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard4_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front1", "front2Opened");
				action3->Rename("sendFront2Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card4UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard4");
				AssetLibrary::_GetAssets().AddObject(action4);

				

				trigger2->SetEventExpr("card4UnderCursor & ~card4Visibled");  
				trigger2->SaveToFile(); 
			    */

				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard4_part3");
				
				
				/*
				trigger3->Rename("clickOnCard4_part3");
				trigger3->SetActionExpr("openCard4_2, openCard4_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card4", Quaternion());
				action5->Rename("openCard4_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card4BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard4_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card4Visibled");  
				trigger3->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard4_part4"); 
				
				
				/*
				trigger4->Rename("clickOnCard4_part4");
				trigger4->SetActionExpr("closeCard4_1");	 
				 

				// условие 
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front2Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card4VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card4Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard4_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card4Visibled & ~card4VisibledMessage");  
				trigger4->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard4_part5"); 
				
				
				/*
				trigger5->Rename("clickOnCard4_part5");
				trigger5->SetActionExpr("closeCard4_2, closeCard4_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card4", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard4_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card4BackFace", Quaternion()); 
				action9->Rename("closeCard4_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card4Visibled");  
				trigger5->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}
			
			
		
		
	
			{
				// создать пятую карту...
				Object* card5 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card5->Rename("card5");
				AssetLibrary::_GetAssets().AddObject(card5);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card5);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card5)); // добавить в графическую сцену
			}
			
			{
				// создать обратную сторону карты
				Object* card5 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card5->Rename("card5BackFace");
				AssetLibrary::_GetAssets().AddObject(card5);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card5);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card5)); // добавить в графическую сцену
			}

			{
				// загрузка пятой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card5Creator");

			
				/*
				trigger1->Rename("card5Creator");
				trigger1->SetActionExpr("card5ChangeTexture, card5EnableTexturedMaterial, card5CreatQuad, card5SetPosition, cardBackFace5ChangeTexture, cardBackFace5EnableTexturedMaterial, cardBackFace5CreatQuad, cardBackFace5SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card5", "front2"); 
				action2->Rename("card5ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card5", true);// включить текстурированный материал
				action3->Rename("card5EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card5", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card5CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card5", "#cardPos5"); 
				action5->Rename("card5SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card5BackFace", "backFace");  
				action7->Rename("cardBackFace5ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card5BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace5EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card5BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace5CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card5BackFace", "#cardPos5");
				action10->Rename("cardBackFace5SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
			    */ 
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}


			 
			
			{  
				// поведение пятой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard5_part1");
				
				
				/*
				trigger1->Rename("clickOnCard5_part1");
				trigger1->SetActionExpr("checkCard5UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card5BackFace", "card5UnderCursor", "", "");    
				action1->Rename("checkCard5UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			     
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger1);	


				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard5_part2");
				
				
				/* 
				trigger2->Rename("clickOnCard5_part2"); 
				trigger2->SetActionExpr("deactivatedCard5, sendFront2Opened, openCard5_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card5Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard5_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front2Opened", "front1");
				action3->Rename("sendFront2Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card5UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard5");
				AssetLibrary::_GetAssets().AddObject(action4);

				

				trigger2->SetEventExpr("card5UnderCursor & ~card5Visibled");  
				trigger2->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard5_part3");
				
				
				/*
				trigger3->Rename("clickOnCard5_part3");
				trigger3->SetActionExpr("openCard5_2, openCard5_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card5", Quaternion());
				action5->Rename("openCard5_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card5BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard5_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card5Visibled");  
				trigger3->SaveToFile();  
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard5_part4"); 
				
				
				/* 
				trigger4->Rename("clickOnCard5_part4");
				trigger4->SetActionExpr("closeCard5_1");	 
				 

				// условие 
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front2Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card5VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card5Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard5_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card5Visibled & ~card5VisibledMessage");  
				trigger4->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard5_part5"); 
				
				
				/*
				trigger5->Rename("clickOnCard5_part5");
				trigger5->SetActionExpr("closeCard5_2, closeCard5_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card5", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard5_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card5BackFace", Quaternion()); 
				action9->Rename("closeCard5_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card5Visibled");  
				trigger5->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}


	
			 
			{
				// создать шестую карту...
				Object* card6 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card6->Rename("card6");
				AssetLibrary::_GetAssets().AddObject(card6);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card6);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card6)); // добавить в графическую сцену
			}
			
			{
				// создать обратную сторону карты
				Object* card6 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card6->Rename("card6BackFace");
				AssetLibrary::_GetAssets().AddObject(card6);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card6);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card6)); // добавить в графическую сцену
			}

			{
				// загрузка шестой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card6Creator");

			
				/*
				trigger1->Rename("card6Creator");
				trigger1->SetActionExpr("card6ChangeTexture, card6EnableTexturedMaterial, card6CreatQuad, card6SetPosition, cardBackFace6ChangeTexture, cardBackFace6EnableTexturedMaterial, cardBackFace6CreatQuad, cardBackFace6SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card6", "front2"); 
				action2->Rename("card6ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card6", true);// включить текстурированный материал
				action3->Rename("card6EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card6", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card6CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card6", "#cardPos6"); 
				action5->Rename("card6SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card6BackFace", "backFace");  
				action7->Rename("cardBackFace6ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card6BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace6EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card6BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace6CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card6BackFace", "#cardPos6");
				action10->Rename("cardBackFace6SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
			    */ 
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}


			
			
			{  
				// поведение шестой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard6_part1");
				
				/*
				
				trigger1->Rename("clickOnCard6_part1");
				trigger1->SetActionExpr("checkCard6UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card6BackFace", "card6UnderCursor", "", "");    
				action1->Rename("checkCard6UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger1);	


				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard6_part2");
				
				/*
				trigger2->Rename("clickOnCard6_part2"); 
				trigger2->SetActionExpr("deactivatedCard6, sendFront2Opened, openCard6_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card6Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard6_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front1", "front2Opened");
				action3->Rename("sendFront2Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card6UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard6");
				AssetLibrary::_GetAssets().AddObject(action4);

				

				trigger2->SetEventExpr("card6UnderCursor & ~card6Visibled");  
				trigger2->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard6_part3");
				
				
				/*
				trigger3->Rename("clickOnCard6_part3");
				trigger3->SetActionExpr("openCard6_2, openCard6_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card6", Quaternion());
				action5->Rename("openCard6_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card6BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard6_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card6Visibled");  
				trigger3->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard6_part4"); 
				
				
				/*
				trigger4->Rename("clickOnCard6_part4");
				trigger4->SetActionExpr("closeCard6_1");	 
				 

				// условие 
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front2Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card6VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card6Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard6_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card6Visibled & ~card6VisibledMessage");  
				trigger4->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard6_part5"); 
				
				/*
				trigger5->Rename("clickOnCard6_part5");
				trigger5->SetActionExpr("closeCard6_2, closeCard6_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card6", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard6_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card6BackFace", Quaternion()); 
				action9->Rename("closeCard6_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card6Visibled");  
				trigger5->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}
		 
			

			{
				// создать седьмую карту...
				Object* card7 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card7->Rename("card7");
				AssetLibrary::_GetAssets().AddObject(card7);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card7);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card7)); // добавить в графическую сцену
			}


			{
				// создать обратную сторону карты
				Object* card7 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card7->Rename("card7BackFace");
				AssetLibrary::_GetAssets().AddObject(card7);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card7);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card7)); // добавить в графическую сцену
			}

			{
				// загрузка седьмой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card7Creator");

			
				/*
				
				trigger1->Rename("card7Creator");
				trigger1->SetActionExpr("card7ChangeTexture, card7EnableTexturedMaterial, card7CreatQuad, card7SetPosition, cardBackFace7ChangeTexture, cardBackFace7EnableTexturedMaterial, cardBackFace7CreatQuad, cardBackFace7SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card7", "front3"); 
				action2->Rename("card7ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card7", true);// включить текстурированный материал
				action3->Rename("card7EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card7", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card7CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card7", "#cardPos7"); 
				action5->Rename("card7SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card7BackFace", "backFace");  
				action7->Rename("cardBackFace7ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card7BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace7EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card7BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace7CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card7BackFace", "#cardPos7");
				action10->Rename("cardBackFace7SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
			    */ 
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}


		
			
			{ 
				// поведение седьмой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard7_part1");
				
				
				/*
				trigger1->Rename("clickOnCard7_part1");
				trigger1->SetActionExpr("checkCard7UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card7BackFace", "card7UnderCursor", "", "");    
				action1->Rename("checkCard7UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger1);	


				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard7_part2");
				
				
				/*
				trigger2->Rename("clickOnCard7_part2"); 
				trigger2->SetActionExpr("deactivatedCard7, sendFront3Opened, openCard7_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card7Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard7_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front1", "front3Opened");
				action3->Rename("sendFront3Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card7UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard7");
				AssetLibrary::_GetAssets().AddObject(action4);

				

				trigger2->SetEventExpr("card7UnderCursor & ~card7Visibled");  
				trigger2->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard7_part3");
				
				/*
				
				trigger3->Rename("clickOnCard7_part3");
				trigger3->SetActionExpr("openCard7_2, openCard7_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card7", Quaternion());
				action5->Rename("openCard7_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card7BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard7_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card7Visibled");  
				trigger3->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard7_part4"); 
				
				
				/*
				trigger4->Rename("clickOnCard7_part4");
				trigger4->SetActionExpr("closeCard7_1");	 
				 

				// условие 
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front3Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card7VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card7Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard7_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card7Visibled & ~card7VisibledMessage");  
				trigger4->SaveToFile(); 
		        */ 
			     
				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard7_part5"); 
				
				/* 
				
				trigger5->Rename("clickOnCard7_part5");
				trigger5->SetActionExpr("closeCard7_2, closeCard7_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card7", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard7_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card7BackFace", Quaternion()); 
				action9->Rename("closeCard7_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card7Visibled");  
				trigger5->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}
			
		


			{
				// создать восьмую карту...
				Object* card8 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card8->Rename("card8");
				AssetLibrary::_GetAssets().AddObject(card8);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card8);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card8)); // добавить в графическую сцену
			}

			{
				// создать обратную сторону карты
				Object* card8 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card8->Rename("card8BackFace");
				AssetLibrary::_GetAssets().AddObject(card8);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card8);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card8)); // добавить в графическую сцену
			}

			
			{ 
				// загрузка восьмой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card8Creator");

			
				
				/*
				trigger1->Rename("card8Creator");
				trigger1->SetActionExpr("card8ChangeTexture, card8EnableTexturedMaterial, card8CreatQuad, card8SetPosition, cardBackFace8ChangeTexture, cardBackFace8EnableTexturedMaterial, cardBackFace8CreatQuad, cardBackFace8SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card8", "front3"); 
				action2->Rename("card8ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card8", true);// включить текстурированный материал
				action3->Rename("card8EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card8", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card8CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card8", "#cardPos8"); 
				action5->Rename("card8SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card8BackFace", "backFace");  
				action7->Rename("cardBackFace8ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card8BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace8EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card8BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace8CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card8BackFace", "#cardPos8");
				action10->Rename("cardBackFace8SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
			    */ 
		
				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}


			
			{  
				// поведение восьмой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard8_part1");
				
				/*
				
				trigger1->Rename("clickOnCard8_part1");
				trigger1->SetActionExpr("checkCard8UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card8BackFace", "card8UnderCursor", "", "");    
				action1->Rename("checkCard8UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger1);	


				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard8_part2");
				
				/*
				
				trigger2->Rename("clickOnCard8_part2"); 
				trigger2->SetActionExpr("deactivatedCard8, sendFront3Opened, openCard8_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card8Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard8_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front1", "front3Opened");
				action3->Rename("sendFront3Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card8UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard8");
				AssetLibrary::_GetAssets().AddObject(action4);

				

				trigger2->SetEventExpr("card8UnderCursor & ~card8Visibled");  
				trigger2->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard8_part3");
				
				/*
				
				trigger3->Rename("clickOnCard8_part3");
				trigger3->SetActionExpr("openCard8_2, openCard8_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card8", Quaternion());
				action5->Rename("openCard8_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card8BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard8_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card8Visibled");  
				trigger3->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard8_part4"); 
				
				
				/*
				trigger4->Rename("clickOnCard8_part4");
				trigger4->SetActionExpr("closeCard8_1");	 
				 

				// условие 
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front3Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card8VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card8Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard8_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card8Visibled & ~card8VisibledMessage");  
				trigger4->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard8_part5"); 
				
				
				/*
				trigger5->Rename("clickOnCard8_part5");
				trigger5->SetActionExpr("closeCard8_2, closeCard8_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE,  "card8", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard8_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card8BackFace", Quaternion()); 
				action9->Rename("closeCard8_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card8Visibled");  
				trigger5->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}
		
			
	
			{
				// создать девятую карту...
				Object* card9 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card9->Rename("card9");
				AssetLibrary::_GetAssets().AddObject(card9);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card9);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card9)); // добавить в графическую сцену
			}

			{
				// создать обратную сторону карты
				Object* card9 = Object::_CreateObject(Object::_CreateObjectType(ITexturedSprite::TEXTURED_SPRITE_CLASS | Position<Vector3D>::POSITION_3D_CLASS));
				card9->Rename("card9BackFace");
				AssetLibrary::_GetAssets().AddObject(card9);

				TexturedSprite<Vector3D, Quaternion, Matrix4x4>* sprite = dynamic_cast<TexturedSprite<Vector3D, Quaternion, Matrix4x4>*>(card9);
				scene->AddGraphicsObject(dynamic_cast<GraphicsObject*>(card9)); // добавить в графическую сцену
			}

			{ 
				// загрузка девятую карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("card9Creator");

			
				/*
				
				trigger1->Rename("card9Creator");
				trigger1->SetActionExpr("card9ChangeTexture, card9EnableTexturedMaterial, card9CreatQuad, card9SetPosition, cardBackFace9ChangeTexture, cardBackFace9EnableTexturedMaterial, cardBackFace9CreatQuad, cardBackFace9SetPosition");


				// событие
				IEvent* event1 = Trigger::_CreateEvent(Trigger::TRIGGER_FINISH_MESSAGE, "listCreator");
				event1->Rename("listInit");
				AssetLibrary::_GetAssets().AddObject(event1);
				
						

				// действие
				IAction* action2 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card9", "front3"); 
				action2->Rename("card9ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action2);	


				// действие
				IAction* action3 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card9", true);// включить текстурированный материал
				action3->Rename("card9EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action3);

				
				// действие
				IAction* action4 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card9", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action4->Rename("card9CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action4);	



				// действие
				IAction* action5 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE, "#card9", "#cardPos9"); 
				action5->Rename("card9SetPosition");
				AssetLibrary::_GetAssets().AddObject(action5);	

	

				// действие
				IAction* action7 = Trigger::_CreateAction(ITexturedMaterial::CHANGE_TEXTURE_MESSAGE, "card9BackFace", "backFace");  
				action7->Rename("cardBackFace9ChangeTexture");
				AssetLibrary::_GetAssets().AddObject(action7);	


				// действие
				IAction* action8 = Trigger::_CreateAction(ITexturedMaterial::ENABLE_MATERIAL_MESSAGE, "card9BackFace", true);// включить текстурированный материал
				action8->Rename("cardBackFace9EnableTexturedMaterial");
				AssetLibrary::_GetAssets().AddObject(action8);

				
					
				// действие
				IAction* action9 = Trigger::_CreateAction(IMesh::CREATE_QUAD_3D_MESSAGE, "card9BackFace", Vector2D(3.0f/5.0f,4.3f/5.0f));
				action9->Rename("cardBackFace9CreatQuad");
				AssetLibrary::_GetAssets().AddObject(action9);	

		
				// действие
				IAction* action10 = Trigger::_CreateTemplateAction(Position<Vector3D>::REPLACE_POSITION_MESSAGE,  "#card9BackFace", "#cardPos9");
				action10->Rename("cardBackFace9SetPosition");
				AssetLibrary::_GetAssets().AddObject(action10);	



				trigger1->SetEventExpr("listInit");
				trigger1->SaveToFile();
		        */ 
		        

				AssetLibrary::_GetAssets().AddObject(trigger1);	
			}
		 


			
			{  
				// поведение девятой карты...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("clickOnCard9_part1");
				
				/*
				
				trigger1->Rename("clickOnCard9_part1");
				trigger1->SetActionExpr("checkCard9UnderCursor");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card9BackFace", "card9UnderCursor", "", "");    
				action1->Rename("checkCard9UnderCursor");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("click"); 
				trigger1->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger1);	


				
				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("clickOnCard9_part2");
				
				
				/*
				trigger2->Rename("clickOnCard9_part2"); 
				trigger2->SetActionExpr("deactivatedCard9, sendFront3Opened, openCard9_1");	 
				 

				// действия  
				IAction* action2 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card9Visibled", "enableVar", Variable<bool>::BOOLEAN_TYPE);
				action2->Rename("openCard9_1");
				AssetLibrary::_GetAssets().AddObject(action2);


				// действия
				IAction* action3 = Trigger::_CreateAction(IMessageHandler::SEND_MESSAGE, "front1", "front3Opened");
				action3->Rename("sendFront3Opened");
				AssetLibrary::_GetAssets().AddObject(action3);

					
				// действия  
				IAction* action4 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card9UnderCursor", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action4->Rename("deactivatedCard9");
				AssetLibrary::_GetAssets().AddObject(action4);  

				

				trigger2->SetEventExpr("card9UnderCursor & ~card9Visibled");  
				trigger2->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("clickOnCard9_part3");
				
				
				/*
				trigger3->Rename("clickOnCard9_part3");
				trigger3->SetActionExpr("openCard9_2, openCard9_3");	 
				 

				// действие
				IAction* action5 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card9", Quaternion());
				action5->Rename("openCard9_2");  
				AssetLibrary::_GetAssets().AddObject(action5);	

				
				// действие
				IAction* action6 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card9BackFace", Matrix4x4::_RotationY(Angle::PI)); 
				action6->Rename("openCard9_3"); 
				AssetLibrary::_GetAssets().AddObject(action6);	
				

				trigger3->SetEventExpr("card9Visibled");  
				trigger3->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	


				
				
				Trigger* trigger4 = new Trigger();
				trigger4->LoadFromFile("clickOnCard9_part4"); 
				
				
				/*
				trigger4->Rename("clickOnCard9_part4");
				trigger4->SetActionExpr("closeCard9_1");	 
				 

				// условие 
				IEvent* event1 = Trigger::_CreateEvent(IMessageHandler::SEND_MESSAGE, "front1", "front3Opened");
				event1->SetOverlappingEnable(true);
				event1->Rename("card9VisibledMessage"); 
				AssetLibrary::_GetAssets().AddObject(event1);
				 

				// действия  
				IAction* action7 = Trigger::_CreateAction(IVariable::CHANGE_VALUE_MESSAGE, "card9Visibled", "~enableVar", Variable<bool>::BOOLEAN_TYPE);
				action7->Rename("closeCard9_1");
				AssetLibrary::_GetAssets().AddObject(action7);

			
				trigger4->SetEventExpr("card9Visibled & ~card9VisibledMessage");  
				trigger4->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger4);	


	
				Trigger* trigger5 = new Trigger();
				trigger5->LoadFromFile("clickOnCard9_part5"); 
				
				/*
				trigger5->Rename("clickOnCard9_part5");
				trigger5->SetActionExpr("closeCard9_2, closeCard9_3");	  
				 

				// действие
				IAction* action8 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card9", Matrix4x4::_RotationY(Angle::PI));
				action8->Rename("closeCard9_2");  
				AssetLibrary::_GetAssets().AddObject(action8);	
					
				
				// действие
				IAction* action9 = Trigger::_CreateAction(Rotation<Quaternion>::REPLACE_ROTATION_MESSAGE, "card9BackFace", Quaternion()); 
				action9->Rename("closeCard9_3");
				AssetLibrary::_GetAssets().AddObject(action9);	

			
				trigger5->SetEventExpr("~card9Visibled");  
				trigger5->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger5);	
			}
	
			
			
			{
				// условия победы...
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("front1_2_3_opened");
				
				/*
				
				trigger1->Rename("front1_2_3_opened");
				trigger1->SetActionExpr("destroyCloseCard1, destroyCloseCard2, destroyCloseCard3");
				 


				IAction* action1 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard1_1");     
				action1->Rename("destroyCloseCard1");
				AssetLibrary::_GetAssets().AddObject(action1);

					
				IAction* action2 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard2_1");     
				action2->Rename("destroyCloseCard2");
				AssetLibrary::_GetAssets().AddObject(action2);

					
				IAction* action3 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard3_1");    
				action3->Rename("destroyCloseCard3");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger1->SetEventExpr("card1Visibled & card2Visibled & card3Visibled"); 
				trigger1->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger1);	
				

				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("front4_5_6_opened");
				
				
				/*
				trigger2->Rename("front4_5_6_opened");
				trigger2->SetActionExpr("destroyCloseCard4, destroyCloseCard5, destroyCloseCard6");
				 


				IAction* action4 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard4_1");    
				action4->Rename("destroyCloseCard4");
				AssetLibrary::_GetAssets().AddObject(action4);

					
				IAction* action5 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard5_1");     
				action5->Rename("destroyCloseCard5");
				AssetLibrary::_GetAssets().AddObject(action5);

					
				IAction* action6 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard6_1");    
				action6->Rename("destroyCloseCard6");
				AssetLibrary::_GetAssets().AddObject(action6);


				trigger2->SetEventExpr("card4Visibled & card5Visibled & card6Visibled"); 
				trigger2->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);

				
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("front7_8_9_opened");
				
				/*
				
				trigger3->Rename("front7_8_9_opened");
				trigger3->SetActionExpr("destroyCloseCard7, destroyCloseCard8, destroyCloseCard9");
				 


				IAction* action7 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard7_1");    
				action7->Rename("destroyCloseCard7");
				AssetLibrary::_GetAssets().AddObject(action7);

					
				IAction* action8 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard8_1");     
				action8->Rename("destroyCloseCard8");
				AssetLibrary::_GetAssets().AddObject(action8);

					
				IAction* action9 = Trigger::_CreateAction(Object::DESTROY_OBJECT_MESSAGE, "closeCard9_1");    
				action9->Rename("destroyCloseCard9");
				AssetLibrary::_GetAssets().AddObject(action9);


				trigger3->SetEventExpr("card7Visibled & card8Visibled & card9Visibled"); 
				trigger3->SaveToFile();
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);
			}


			
			{
				// эффект выделения для карты 1
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard1_1");
				
				
				/*
				trigger1->Rename("scaleCard1_1");
				trigger1->SetActionExpr("checkCard1UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card1BackFace", "card1UnderCursorForScale", "", "");    
				action1->Rename("checkCard1UnderCursorForScale"); 
				AssetLibrary::_GetAssets().AddObject(action1); 


				trigger1->SetEventExpr("mouseMoveEvent");  
				trigger1->SaveToFile();
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard1_2");
				
				/*
				
				trigger2->Rename("scaleCard1_2");
				trigger2->SetActionExpr("scaleCard1a, scaleCard1b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card1BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard1a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card1", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard1b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card1UnderCursorForScale"); 
				trigger2->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard1_3");
				
				/*
				
				trigger3->Rename("scaleCard1_3");
				trigger3->SetActionExpr("scaleCard1c, scaleCard1d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card1BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard1c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card1", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard1d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card1UnderCursorForScale"); 
				trigger3->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}


			
			{
				// эффект выделения для карты 2
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard2_1");
				
				/*
				
				trigger1->Rename("scaleCard2_1");
				trigger1->SetActionExpr("checkCard2UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card2BackFace", "card2UnderCursorForScale", "", "");    
				action1->Rename("checkCard2UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard2_2");
				
				/*
				
				trigger2->Rename("scaleCard2_2");
				trigger2->SetActionExpr("scaleCard2a, scaleCard2b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card2BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard2a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card2", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard2b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card2UnderCursorForScale"); 
				trigger2->SaveToFile(); 
				*/ 

				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard2_3");
				
				/*
				
				trigger3->Rename("scaleCard2_3");
				trigger3->SetActionExpr("scaleCard2c, scaleCard2d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card2BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard2c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card2", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard2d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card2UnderCursorForScale"); 
				trigger3->SaveToFile();
				*/ 
			
				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}

		 
			{
				// эффект выделения для карты 3
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard3_1");
				
				
				/*
				trigger1->Rename("scaleCard3_1");
				trigger1->SetActionExpr("checkCard3UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card3BackFace", "card3UnderCursorForScale", "", "");    
				action1->Rename("checkCard3UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard3_2");
				
				/*
				
				trigger2->Rename("scaleCard3_2");
				trigger2->SetActionExpr("scaleCard3a, scaleCard3b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card3BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard3a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card3", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard3b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card3UnderCursorForScale"); 
				trigger2->SaveToFile(); 
				*/ 
			
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard3_3");
				
				
				/*
				trigger3->Rename("scaleCard3_3");
				trigger3->SetActionExpr("scaleCard3c, scaleCard3d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card3BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard3c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card3", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard3d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card3UnderCursorForScale"); 
				trigger3->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}

			
			 
			{
				// эффект выделения для карты 4
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard4_1");
				
				/*
				trigger1->Rename("scaleCard4_1");
				trigger1->SetActionExpr("checkCard4UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card4BackFace", "card4UnderCursorForScale", "", "");    
				action1->Rename("checkCard4UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
				*/ 
			

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard4_2");
				
				/*
				
				trigger2->Rename("scaleCard4_2");
				trigger2->SetActionExpr("scaleCard4a, scaleCard4b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card4BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard4a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card4", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard4b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card4UnderCursorForScale"); 
				trigger2->SaveToFile(); 
				 
		        */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard4_3");
				
				
				/*
				trigger3->Rename("scaleCard4_3");
				trigger3->SetActionExpr("scaleCard4c, scaleCard4d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card4BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard4c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card4", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard4d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card4UnderCursorForScale"); 
				trigger3->SaveToFile(); 
				*/ 
			
				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}

			
			{
				// эффект выделения для карты 5
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard5_1");
				
				/*
				
				trigger1->Rename("scaleCard5_1");
				trigger1->SetActionExpr("checkCard5UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card5BackFace", "card5UnderCursorForScale", "", "");    
				action1->Rename("checkCard5UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard5_2");
				
				
				/*
				trigger2->Rename("scaleCard5_2");
				trigger2->SetActionExpr("scaleCard5a, scaleCard5b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card5BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard5a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card5", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard5b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card5UnderCursorForScale"); 
				trigger2->SaveToFile(); 
				*/
			
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard5_3");
				
				/*
				
				trigger3->Rename("scaleCard5_3");
				trigger3->SetActionExpr("scaleCard5c, scaleCard5d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card5BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard5c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card5", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard5d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card5UnderCursorForScale"); 
				trigger3->SaveToFile(); 
				*/ 
			
				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}


			{
				// эффект выделения для карты 6
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard6_1");
				
				/*
				
				trigger1->Rename("scaleCard6_1");
				trigger1->SetActionExpr("checkCard6UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card6BackFace", "card6UnderCursorForScale", "", "");    
				action1->Rename("checkCard6UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
				*/ 
			

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard6_2");
				
				
				/*
				trigger2->Rename("scaleCard6_2");
				trigger2->SetActionExpr("scaleCard6a, scaleCard6b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card6BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard6a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card6", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard6b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card6UnderCursorForScale"); 
				trigger2->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard6_3");
				
				/*
				
				trigger3->Rename("scaleCard6_3");
				trigger3->SetActionExpr("scaleCard6c, scaleCard6d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card6BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard6c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card6", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard6d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card6UnderCursorForScale"); 
				trigger3->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}

			
			
			{ 
				// эффект выделения для карты 7
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard7_1");
				
				
				/*
				trigger1->Rename("scaleCard7_1");
				trigger1->SetActionExpr("checkCard7UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card7BackFace", "card7UnderCursorForScale", "", "");    
				action1->Rename("checkCard7UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard7_2");
				
				/* 
				
				trigger2->Rename("scaleCard7_2");
				trigger2->SetActionExpr("scaleCard7a, scaleCard7b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card7BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard7a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card7", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard7b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card7UnderCursorForScale"); 
				trigger2->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard7_3");
				
				/*
				
				trigger3->Rename("scaleCard7_3");
				trigger3->SetActionExpr("scaleCard7c, scaleCard7d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card7BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard7c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card7", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard7d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card7UnderCursorForScale"); 
				trigger3->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}

			{ 
				// эффект выделения для карты 8
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard8_1");
				
				
				/*
				trigger1->Rename("scaleCard8_1");
				trigger1->SetActionExpr("checkCard8UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card8BackFace", "card8UnderCursorForScale", "", "");    
				action1->Rename("checkCard8UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard8_2");
				
				/*
				
				trigger2->Rename("scaleCard8_2");
				trigger2->SetActionExpr("scaleCard8a, scaleCard8b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card8BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard8a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card8", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard8b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card8UnderCursorForScale"); 
				trigger2->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard8_3");
				
				/*
				
				trigger3->Rename("scaleCard8_3");
				trigger3->SetActionExpr("scaleCard8c, scaleCard8d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card8BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard8c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card8", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard8d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card8UnderCursorForScale"); 
				trigger3->SaveToFile(); 
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger3);	
			}

			
			
			{ 
				// эффект выделения для карты 9
				Trigger* trigger1 = new Trigger();
				trigger1->LoadFromFile("scaleCard9_1");
				
				/*
				
				trigger1->Rename("scaleCard9_1");
				trigger1->SetActionExpr("checkCard9UnderCursorForScale");
				 


				IAction* action1 = Trigger::_CreateAction(AbstractSurface<Vector3D>::CHECK_MESH_UNDER_CURSOR_MESSAGE, "card9BackFace", "card9UnderCursorForScale", "", "");    
				action1->Rename("checkCard9UnderCursorForScale");
				AssetLibrary::_GetAssets().AddObject(action1);


				trigger1->SetEventExpr("mouseMoveEvent"); 
				trigger1->SaveToFile();
			    */ 

				AssetLibrary::_GetAssets().AddObject(trigger1);	



				Trigger* trigger2 = new Trigger();
				trigger2->LoadFromFile("scaleCard9_2");
				
				/*
				
				trigger2->Rename("scaleCard9_2");
				trigger2->SetActionExpr("scaleCard9a, scaleCard9b");
				 


				IAction* action2 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card9BackFace", Vector3D(1.1, 1.1, 0));    
				action2->Rename("scaleCard9a");
				AssetLibrary::_GetAssets().AddObject(action2);


				IAction* action3 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card9", Vector3D(1.1, 1.1, 0));    
				action3->Rename("scaleCard9b");
				AssetLibrary::_GetAssets().AddObject(action3);


				trigger2->SetEventExpr("card9UnderCursorForScale"); 
				trigger2->SaveToFile(); 
			    */ 
				AssetLibrary::_GetAssets().AddObject(trigger2);	


					
				Trigger* trigger3 = new Trigger();
				trigger3->LoadFromFile("scaleCard9_3");
				
				/*
				
				trigger3->Rename("scaleCard9_3");
				trigger3->SetActionExpr("scaleCard9c, scaleCard9d");
				 


				IAction* action4 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card9BackFace", Vector3D(1.0, 1.0, 0));    
				action4->Rename("scaleCard9c");
				AssetLibrary::_GetAssets().AddObject(action4);


				IAction* action5 = Trigger::_CreateAction(Scaling<Vector3D>::REPLACE_SCALE_MESSAGE, "card9", Vector3D(1.0, 1.0, 0));    
				action5->Rename("scaleCard9d");
				AssetLibrary::_GetAssets().AddObject(action5);


				trigger3->SetEventExpr("~card9UnderCursorForScale"); 
				trigger3->SaveToFile(); 
			     
		        */ 
				AssetLibrary::_GetAssets().AddObject(trigger3);	
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
	
	
		
		//camera->BeginScreen2D();// вывод двухмерной графики

		// вывод текста...
		int32 i = 0;


	
		
		/*
		Object* obj = AssetLibrary::_GetAssets().FindObject("positions");
	
		if(obj)
		{
			VariableList<Variable<int32>>* list = dynamic_cast<VariableList<Variable<int32>>*>(obj);
			
			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "size: %d", list->GetSize());
			font2d->DrawScreenText(Vector3D(10, i+=20, -1), "%d, %d", list->Get(0)->GetValue(), list->Get(1)->GetValue()); 
		}
		*/ 
		
		//camera->BeginPerspective3D();// вывод трехмерной графики
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
	def.rendererDef.realTimeWorkerDef.windowDef.tittle = "Trigger.example_06";

	Application* app = new Application();// создать окно
	
	if(app->Create(def))
	{
		app->Loop();// запустить цикл событий
	}
	delete app;
	return 0;
}
