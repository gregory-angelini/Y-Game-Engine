

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
#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Shape/Sphere/Sphere3D/Sphere3D.h>
#include "MyNode.h"
#include "MyTree.h"


// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: GraphicsScene<Vector3D, Quaternion, Matrix4x4>* scene;// графическая сцена
	
	private: MyTree tree;// магическое бинарное дерево
	private: bool testEnabled;// если true, то будет работать автоматическое тестирование дерева
	private: MyTree treeCopy;// копия дерева

    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL), testEnabled(false)
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
				GraphicsScene<Vector3D, Quaternion, Matrix4x4>::Define def;
				def.logicSceneDef.scale = Size3D(1.0f, 1.0f, 1.0f);
				scene = new GraphicsScene<Vector3D, Quaternion, Matrix4x4>();
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
				scene->LogicScene::AddObject(font2d);
				font2d->Pick();
			}
	

			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
				//def.movementType = Camera3D::OVERLAND;
				def.rotationType = Camera3D::TRACKBALL_OBJECT_VIEWER;
				def.pitchMax = 0.0f;// запретить камере опускаться ниже пола (плоскости XZ)
				def.zoomMax = 15;
				def.zoomMin = 6;


				camera = new Camera3D();
				camera->Create(def);
				scene->LogicScene::AddObject(camera, true);
				camera->Pick("MagicBinaryTree.example_04");
			}

			// дерево...
			tree.SetAnimFPS(2);

			MoveToCenter();// разместить окно в центре экрана
			GraphicsPoint<Vector3D, ColorRGBA>::_SetPointSize(10);// размер точек 
			GraphicsLine<Vector3D, ColorRGBA>::_SetLineWidth(3.0f);// толщина линий

			Mouse::_MoveCursorToClientAreaCenter(this);// переместить курсор в центр клиентской области окна
			Mouse::_ShowCursor(false);// скрыть курсор
			return true;// вернуть успех
		}
		// иначе вернуть неудачу
		return false;
	}
	



	// функция кадра
	public: virtual void Update(float _delta)
	{
		if(testEnabled) { tree.Test(&MyNode::Key, 1, 1000); } 

		tree.UpdateHeight();// обновить высоту дерева
		tree.UpdateExcessHeight();// обновить количество лишних уровней высоты дерева

		// если тестирование отключено
		if(!testEnabled)
		{
			// анимировать очередь анимации
			tree.AnimSeqUpdate(_delta);
		}

	}


	// функция рендера
	public: virtual void Draw(float _delta)
	{
		ClearScene();// очистить сцену



		// scene->Draw();// отображение сцены
		
		
		// вывод дерева...
		tree.Draw();



		// вывод текста...
		camera->BeginScreen2D();// начать вывод двухмерной графики


		int i = 0;
		
		font2d->SetTextColor(ColorRGBA::WHITE_COLOR);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.height/perfect height: %d, %d", tree.StaticHeight(), tree.StaticHeight() - tree.StaticExcessHeight());
	    

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.excess height: %d", tree.StaticExcessHeight());
	    
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "tree.Size: %d", tree.Size());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "root node: %d", tree.Root());
	  
		

		font2d->SetTextColor(MyNode::MARK_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "marked node: %d", tree.Marked());
	  

		font2d->SetTextColor(MyNode::SELECT_COLOR);
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected node: %d", tree.Selected());




		if(tree.Selected())// если есть выделенный узел
		{
			// значение ключа
			font2d->SetTextColor(ColorRGBA::WHITE_COLOR);
			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.key: %d", tree.Selected()->Key());


			// левый потомок
			if(tree.Marked() && tree.ChildOf(tree.Selected(), 0) == tree.Marked()) 
			{ 
				font2d->SetTextColor(MyNode::MARK_COLOR); 
			}
			else if(tree.Selected() && tree.ChildOf(tree.Selected(), 0) == tree.Selected()) 
			{ 
				font2d->SetTextColor(MyNode::SELECT_COLOR); 
			}
			else 
			{ 
				// цвет левого потомка
				font2d->SetTextColor(MyNode::LEFT_COLOR);
			} 

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.left: %d", tree.ChildOf(tree.Selected(), 0));


			// правый потомок
			if(tree.Marked() && tree.ChildOf(tree.Selected(), 1) == tree.Marked()) 
			{ 
				font2d->SetTextColor(MyNode::MARK_COLOR); 
			}
			else if(tree.Selected() && tree.ChildOf(tree.Selected(), 1) == tree.Selected()) 
			{ 
				font2d->SetTextColor(MyNode::SELECT_COLOR); 
			}
			else 
			{ 
				// цвет правого потомка
				font2d->SetTextColor(MyNode::RIGHT_COLOR); 
			}

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.right: %d", tree.ChildOf(tree.Selected(), 1));


			// родитель
			if(tree.Marked() && tree.ParentOf(tree.Selected()) == tree.Marked()) 
			{ 
				font2d->SetTextColor(MyNode::MARK_COLOR); 
			}
			else if(tree.Selected() && tree.ParentOf(tree.Selected()) == tree.Selected()) 
			{ 
				font2d->SetTextColor(MyNode::SELECT_COLOR); 
			}
			else 
			{ 
				font2d->SetTextColor(ColorRGBA::WHITE_COLOR);
			}

			font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "selected.parent: %d", tree.ParentOf(tree.Selected()));
		}



		font2d->SetTextColor(ColorRGBA::WHITE_COLOR);

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "anim size: %d", tree.AnimSeqSize());

		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "anim fps: %d", tree.AnimFPS());
		font2d->DrawScreenText(Vector3D(10, ClientAreaSize().height - (i+=20), -1), "anim playing: %d", tree.IsPlaying());

		
		


		

	  
		
		// визуализировать сцену...
		camera->End2D();// начать вывод трехмерной графики	
	}


	// фильтр событий мыши
	public: virtual void MouseEventFilter(Mouse::Event _event)
	{
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
				MyNode* n = new MyNode(Random::_RandomInt32(1, 100));

				if(tree.Add(n, &MyNode::Key))// добавить в дерево
				{
					tree.Select(n);// выделить добавленный узел
				}
				else { delete n; } // иначе удалить узел-дубликат
			}
			else if(_event.key == Keyboard::F)
			{
				MyNode* finded = NIL;
				if(tree.Marked()) { finded = tree.Find(tree.Marked()->Key(), &MyNode::Key); } // найти отмеченный узел
				if(finded) { tree.Select(finded); } // выделить найденный узел
			}
			else if(_event.key == Keyboard::D)
			{ 
				tree.Del(tree.Selected());//исключить выделенный узел
			}
			else if(_event.key == Keyboard::R)
			{ 
				tree.Rebuild(tree.Marked(), 0, &MyNode::Key);
			}
			else if(_event.key == Keyboard::M)
			{
				tree.SetMark(tree.Selected());// отметить выделенный узел
			}
			else if(_event.key == Keyboard::S)
			{
				tree.Select(tree.Root());  
			}
			else if(_event.key == Keyboard::LEFT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.ChildOf(tree.Selected(), 0)); }
			}
			else if(_event.key == Keyboard::RIGHT_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.ChildOf(tree.Selected(), 1)); }
			}
			else if(_event.key == Keyboard::UP_ARROW)
			{
				if(tree.Selected()) { tree.Select(tree.ParentOf(tree.Selected())); } 
			}
			else if(_event.key == Keyboard::T)
			{
				testEnabled = !testEnabled;

				if(testEnabled)// если нужно запустить тест
				{
					tree.Select(NIL);
					tree.SetMark(NIL);
				}
			}
			else if(_event.key == Keyboard::NUM_9)
			{
				tree.ClearAnimSeq();// очистить список анимации

				MagicCaller<void> caller;
				caller.Set(&tree, &MyTree::AddToAnimSeq);

				tree.PrefixTraverse(caller, false);

				tree.PlayAnim();// разрешить анимирование объектов
			}
			else if(_event.key == Keyboard::NUM_8)
			{
				tree.ClearAnimSeq();// очистить список анимации

				MagicCaller<void> caller;
				caller.Set(&tree, &MyTree::AddToAnimSeq);

				tree.PostfixTraverse(caller, false);

				tree.PlayAnim();// разрешить анимирование объектов
			}
			else if(_event.key == Keyboard::NUM_7)
			{
				tree.ClearAnimSeq();// очистить список анимации

				MagicCaller<void> caller;
				caller.Set(&tree, &MyTree::AddToAnimSeq);
		
				tree.InfixTraverse(caller, false);

				tree.PlayAnim();// разрешить анимирование объектов
			}
			else if(_event.key == Keyboard::NUM_6)
			{
				tree.Select(tree.Extremum(false));
			}
			else if(_event.key == Keyboard::NUM_5)
			{
				tree.Select(tree.Extremum(true));
			}
			else if(_event.key == Keyboard::P)
			{
				if(tree.IsPlaying()) { tree.StopAnim(); }
				else { tree.PlayAnim(); } 
			}
			else if(_event.key == Keyboard::C)
			{
				int32 key;
				if(tree.Marked())// если помеченный узел существует
				{
					// запомнить ключ узла Marked 
					key = tree.Marked()->Key();
				}

				tree.Clear(tree.Selected(), MagicContainer::DELETE_NODE);// удалить поддерево с корнем Selected
				tree.Select(NIL);// сбросить выделенный узел
 
				if(tree.Marked())// если помеченный узел существует
				{
					// указатель на Marked может быть не актуален, в силу того что соответствующий узел мог быть удален
					if(tree.Find(key, &MyNode::Key) == NIL)// если узла с таким ключом не существует, значит узел был удален
					{
						tree.SetMark(NIL);// обнулить отмеченный узел
					}
				}
				tree.ClearAnimSeq();
			}
			else if(_event.key == Keyboard::J)// копирование деревьев
			{
				tree.SetMark(NIL);
				tree.Select(NIL); 
				tree.ClearAnimSeq();

				treeCopy = tree;
				tree.Clear(MagicContainer::DELETE_NODE);
				tree.CheckTree(tree.Root(), &MyNode::Key);
			}
			else if(_event.key == Keyboard::K)// копирование деревьев
			{
				tree.SetMark(NIL);
				tree.Select(NIL);
				tree.ClearAnimSeq();
				
				tree = treeCopy;
				tree.CheckTree(tree.Root(), &MyNode::Key);
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
	def.realTimeWorkerDef.windowDef.tittle = "MagicBinaryTree.example_04";
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
