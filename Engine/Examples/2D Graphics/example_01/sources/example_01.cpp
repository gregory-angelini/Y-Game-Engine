

#include "Win32.h"
#include <Engine/Output/Graphics/Renderer/Renderer.h>

#include <Engine/Math/Angle/Angle.h>
#include <glaux.h>// для текстур

#include <Engine/Math/Ratio/Ratio.h>
#include <Engine/Output/Graphics/Camera/Camera3D/Camera3D.h> 
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
#include <Engine/Helpers/Scene/Scene.h>
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Output/Graphics/Shape/Mesh/GraphicsMesh/GraphicsMesh.h>

#include <Engine/Core/Time/Timer/AsyncTimer/AsyncTimer.h>
#include <Engine/Core/Shape/Sphere3D/Sphere3D.h>
#include <Engine/Core/Shape/Circle/Circle.h>
#include <Engine/Core/Shape/Triangle/Triangle.h>
#include <Engine/Core/Shape/Quad/Quad.h>
#include <Engine/Core/Transformation/ModelView/ModelView.h>
#include <Engine/Core/Bitset/Bitset.h>


// реализует графический треугольник в двухмерном пространстве
class GraphicsTriangle2D: public Triangle<Vector2D>, // треугольник
	                      public GraphicsObject, // графический объект
						  public ModelView<Vector2D, float, Matrix3x3<Vector2D>>// модельные трансформации
{
	//DATA
	public: RGBA color;// цвет граней



	// METHODS
	// конструктор по умолчанию
	public: GraphicsTriangle2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// отображает объект на экран
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(v1), Transformation().TransformVector(v2), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(v2), Transformation().TransformVector(v3), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(v3), Transformation().TransformVector(v1), color);
	}
};


// реализует графический треугольник в трехмерном пространстве
class GraphicsTriangle3D: public Triangle<Vector3D>, // треугольник
	                      public GraphicsObject, // графический объект
						  public ModelView<Vector3D, Quaternion, Matrix4x4>// модельные трансформации
{
	//DATA
	public: RGBA color;// цвет граней



	// METHODS
	// конструктор по умолчанию
	public: GraphicsTriangle3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// отображает объект на экран
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(v1), Transformation().TransformVector(v2), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(v2), Transformation().TransformVector(v3), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(v3), Transformation().TransformVector(v1), color);
	}
};



// реализует графический квад в двухмерном пространстве
class GraphicsQuad2D: public Quad<Vector2D>, // квад
	                  public GraphicsObject, // графический объект
					  public ModelView<Vector2D, float, Matrix3x3<Vector2D>>// модельные трансформации
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsQuad2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// отображает объект на экран
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex1()), Transformation().TransformVector(Vertex2()), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex2()), Transformation().TransformVector(Vertex3()), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex3()), Transformation().TransformVector(Vertex4()), color);
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(Vertex4()), Transformation().TransformVector(Vertex1()), color);
	}
};


// реализует графический квад в трехмерном пространстве
class GraphicsQuad3D: public Quad<Vector3D>, // квад
	                  public GraphicsObject, // графический объект
					  public ModelView<Vector3D, Quaternion, Matrix4x4>// модельные трансформации
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsQuad3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// отображае тобъект на экран
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex1()), Transformation().TransformVector(Vertex2()), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex2()), Transformation().TransformVector(Vertex3()), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex3()), Transformation().TransformVector(Vertex4()), color);
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(Vertex4()), Transformation().TransformVector(Vertex1()), color);
	}
};



// реализует графическую линию в двухмерном пространстве
class GraphicsLine2D: public GraphicsLine<Vector2D> // графическая линия
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsLine2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);


		// задать шаблон линии
		int32 stipple = 0;

		for(int32 i = 0; i < sizeof(stipple) * 8; ++i)// обходим все биты stipple
		{
			if(i % 2 == 0)// каждый второй бит делаем включенным
			{
				Bitset::_SetBit(stipple, i, true);
			}
		}

		GraphicsLine<Vector2D>::_SetStipple(stipple, 5);// создать шаблон линии
	}
			

	// отображает объект на экран
	public: virtual void Draw(void)
	{
		GraphicsLine<Vector2D>::_SetStippleEnable(true);
		GraphicsLine<Vector2D>::Draw();
		GraphicsLine<Vector2D>::_SetStippleEnable(false);
	}
};

// реализует графическую линию в трехмерном пространстве
class GraphicsLine3D: public GraphicsLine<Vector3D> // графическая линия
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsLine3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);
	}
};



// реализует графический круг в двухмерном пространстве
class GraphicsCircle2D: public Circle<Vector2D>, // окружность
	                    public GraphicsObject, // графический объект
					    public ModelView<Vector2D, float, Matrix3x3<Vector2D>>// модельные трансформации
{
	//DATA
	public: RGBA color;// цвет граней
	private: vector<Vector2D> vertices;// массив вершин


	// METHODS
	// конструктор по умолчанию
	public: GraphicsCircle2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	
	// строит аппроксимацию окружности, состоящую из _segments вершин
	// примечание: чем больше значение _segments, тем более гладкой (приближенной к форме окружности) будет аппроксимация окружности
	// примечание: если _segments меньше 3, то функция будет прервана
	public: void BuildCircle(int32 _segments)
	{
		Circle<Vector2D>::BuildCircle(_segments, vertices);
	}

	// отображает объект на экран
	public: virtual void Draw(void)
	{
		for(int i = 0; i < vertices.size() - 1; ++i)
		{
			GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(vertices[i]), Transformation().TransformVector(vertices[i + 1]), color);
		}

		// рисуем замыкающую грань
		GraphicsLine<Vector2D>::_Draw(Transformation().TransformVector(vertices[vertices.size() - 1]), Transformation().TransformVector(vertices[0]), color);
	}
};



// реализует графический круг в трехмерном пространстве
class GraphicsCircle3D: public Circle<Vector3D>, // окружность
	                    public GraphicsObject, // графический объект
					    public ModelView<Vector3D, Quaternion, Matrix4x4>// модельные трансформации
{
	//DATA
	public: RGBA color;// цвет граней
	private: vector<Vector3D> vertices;// массив вершин


	// METHODS
	// конструктор по умолчанию
	public: GraphicsCircle3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
	}

	// строит аппроксимацию окружности, состоящую из _segments вершин
	// примечание: чем больше значение _segments, тем более гладкой (приближенной к форме окружности) будет аппроксимация окружности
	// примечание: если _segments меньше 3, то функция будет прервана
	public: void BuildCircle(int32 _segments)
	{
		Circle<Vector3D>::BuildCircle(_segments, vertices);
	}

	// отображает объект на экран
	public: virtual void Draw(void)
	{
		for(int i = 0; i < vertices.size() - 1; ++i)
		{
			GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(vertices[i]), Transformation().TransformVector(vertices[i + 1]), color);
		}

		// рисуем замыкающую грань
		GraphicsLine<Vector3D>::_Draw(Transformation().TransformVector(vertices[vertices.size() - 1]), Transformation().TransformVector(vertices[0]), color);
	}
};


// реализует графическую полигональную сетку в двухмерном пространстве
class GraphicsMesh2D: public GraphicsMesh<Vector2D, float, Matrix4x4>// графический мэш; мы используем Matrix4x4 а не Matrix3x3<Vector2D>
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsMesh2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetWireframeEnable(true);
	}
		
	// отображает полигональную сетку на экран 
	public: virtual void Draw(void)
	{
		BeginTransform();
		GraphicsMesh::Draw();
		EndTransform();
	}
};



// реализует графическую полигональную сетку в трехмерном пространстве
class GraphicsMesh3D: public GraphicsMesh<Vector3D, Quaternion, Matrix4x4>// графический мэш
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsMesh3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetWireframeEnable(true);
	}
				
			
	// отображает полигональную сетку на экран 
	public: virtual void Draw(void)
	{
		BeginTransform();
		GraphicsMesh::Draw();
		EndTransform();
	}
};



// реализует графическую точку в двухмерном пространстве
class GraphicsPoint2D: public GraphicsPoint<Vector2D>// графическая точка 
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsPoint2D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);
	}
};


// реализует графическую точку в трехмерном пространстве
class GraphicsPoint3D: public GraphicsPoint<Vector3D>// графическая 
{
	//DATA
	public: RGBA color;// цвет граней


	// METHODS
	// конструктор по умолчанию
	public: GraphicsPoint3D(void)
	{
		color = Color<RGBA>::_Random(50, 200);
		SetColor(color);
	}
};

// класс, реализующий пользовательское окно
class Application: public Renderer
{
	// DATA
	private: Camera3D* camera;// камера
    private: Font2D* font2d;// шрифт
	private: Scene* scene;// графическая сцена
	private: StringANSI currentProjectionType;// имя текущего типа проекции



    // METHODS
    // инициализирующий конструктор
	public: Application(void): camera(NIL), font2d(NIL), scene(NIL)
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
				scene->AddObject(font2d);
				font2d->Pick();
			}
			
			// двухмерный треугольник...
			{
				GraphicsTriangle2D* triangle2D = new GraphicsTriangle2D();

				// описываем вершины против часовой стрелке, в двухмерной декартовой системе координат
				triangle2D->v1 = Vector2D(0, 20);
				triangle2D->v2 = Vector2D(20, -20);
				triangle2D->v3 = Vector2D(-20, -20);
				
				// трансформации
				triangle2D->SetPosition(Vector2D(250, 250)); 
				triangle2D->SetScale(Vector2D(2, 2));
				triangle2D->SetRotation(Angle::HALF_PI);


				// альтернативный способ трансформации
				//triangle2D->Transform(Matrix3x3<Vector2D>(Vector2D(250, 250), Vector2D(2, 2), Angle::HALF_PI));


				triangle2D->Rename("triangle2D");
				scene->AddObject(triangle2D);// добавить в сцену
				scene->AddGraphicsObject(triangle2D);
			}

			// трехмерный треугольник...
			{
				GraphicsTriangle3D* triangle3D = new GraphicsTriangle3D();

				// описываем вершины против часовой стрелке, в двухмерной декартовой системе координат
				triangle3D->v1 = Vector3D(0, 20, 0);
				triangle3D->v2 = Vector3D(20, -20, 0);
				triangle3D->v3 = Vector3D(-20, -20, 0);

				// трансформации
				triangle3D->SetPosition(Vector3D(100, 100, -10)); 
				triangle3D->SetScale(Vector3D(3, 3, 0));
				triangle3D->SetRotation(Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI));
				
				// альтернативный способ трансформации
				//triangle3D->Transform(Matrix4x4(Vector3D(100, 100, -10), Vector3D(3, 3, 0), Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI)));


				triangle3D->Rename("triangle3D");
				scene->AddObject(triangle3D);// добавить в сцену
				scene->AddGraphicsObject(triangle3D);
			}

		

			// двухмерный прямоугольник...
			{ 
				GraphicsQuad2D* quad2D = new GraphicsQuad2D();

				// описываем вершины против часовой стрелке, в двухмерной декартовой системе координат
				quad2D->Set(Vector2D(-20, 20), Vector2D(20, 20), Vector2D(20, -20), Vector2D(-20, -20));
				
				// трансформации
				quad2D->SetPosition(Vector2D(70, 350)); 
				quad2D->SetRotation(Angle::HALF_PI/2);
				quad2D->SetScale(Vector2D(1, 1));
				
				quad2D->Rename("quad2D");
				scene->AddObject(quad2D);// добавить в сцену
				scene->AddGraphicsObject(quad2D);
			}
				 
			// трехмерный прямоугольник...
			{
				GraphicsQuad3D* quad3D = new GraphicsQuad3D();

				// описываем вершины против часовой стрелке, в двухмерной декартовой системе координат
				quad3D->Set(Vector3D(-20, 20, 0), Vector3D(20, 20, 0), Vector3D(20, -20, 0), Vector3D(-20, -20, 0));
				
				// трансформации
				quad3D->SetPosition(Vector3D(70, 550, -10)); 
				quad3D->SetScale(Vector3D(1.5, 1.5, 0));
				//quad3D->SetRotation(Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI/2));
				
				quad3D->Rename("quad3D");	
				scene->AddObject(quad3D);// добавить в сцену
				scene->AddGraphicsObject(quad3D);
			} 


			// двухмерная линия...
			{
				GraphicsLine2D* line2D = new GraphicsLine2D();
				line2D->Set(Vector2D(-40+370, 20+450), Vector2D(40+370+100, 20+450+40+50));
				

				line2D->Rename("line2D");
				scene->AddObject(line2D);// добавить в сцену
				scene->AddGraphicsObject(line2D);
			}

			// трехмерная линия...
			{
				GraphicsLine3D* line3D = new GraphicsLine3D();
				line3D->Set(Vector3D(-20+470, 20+150, -10), Vector3D(20+470, 20+150, -10));
				

				line3D->Rename("line3D");
				scene->AddObject(line3D);// добавить в сцену
				scene->AddGraphicsObject(line3D);
			}

			// двухмерный круг...
			{
				GraphicsCircle2D* circle2D = new GraphicsCircle2D();
				circle2D->Set(Vector2D(0, 0), 40.0f);
				circle2D->BuildCircle(32); 
				
				// трансформации
				circle2D->SetPosition(Vector2D(170, 350)); 
				circle2D->SetScale(Vector2D(1, 2));// строим эллипс 
				
				// альтернативный способ трансформации
				//circle2D->Transform(Matrix4x4(Vector2D(170, 350), Vector2D(1, 2), Quaternion()));


				circle2D->Rename("circle2D");
				scene->AddObject(circle2D);// добавить в сцену
				scene->AddGraphicsObject(circle2D);
			} 

			// трехмерный круг...
			{
				GraphicsCircle3D* circle3D = new GraphicsCircle3D();
				circle3D->Set(Vector3D(0, 0, 0), 40.0f);
				circle3D->BuildCircle(22);
				
				// трансформации
				circle3D->SetPosition(Vector3D(290, 150, -1)); 
				circle3D->SetScale(Vector3D(1, 1, 1));
				
				circle3D->Rename("circle3D");
				scene->AddObject(circle3D);// добавить в сцену
				scene->AddGraphicsObject(circle3D);
			}

			// двухмерная сетка...
			{  
				GraphicsMesh2D* mesh2D = new GraphicsMesh2D();
				SimpleList<Vector2D> poly;// множество вершин, описывающая полигоны

				// ромб
				// описываем вершины против часовой стрелки, в двухмерной декартовой системе координат
				poly.AddToTail(Vector2D(20, 0));
				poly.AddToTail(Vector2D(0, -20));
				poly.AddToTail(Vector2D(-20, 0));

				poly.AddToTail(Vector2D(-20, 0));
				poly.AddToTail(Vector2D(0, 20));
				poly.AddToTail(Vector2D(20, 0));

				mesh2D->CreateShape(poly);// создает сетку из треугольников, согласно описанию poly
				
			
				//mesh2D->AutoResizeColors();
				mesh2D->SetColor(mesh2D->color);


				// трансформации
				mesh2D->SetPosition(Vector2D(380, 250)); 
				mesh2D->SetScale(Vector2D(2, 2));
				mesh2D->SetRotation(Angle::HALF_PI);
				


				mesh2D->Rename("mesh2D.1");
				scene->AddObject(mesh2D);// добавить в сцену
				scene->AddGraphicsObject(mesh2D);
			}


			// двухмерная сетка...
			{
				GraphicsMesh2D* mesh2D = new GraphicsMesh2D();

				mesh2D->CreateCircle2D(Circle<Vector2D>(Vector2D(), 50), 30);// создать окружность

				//mesh2D->AutoResizeColors();
				mesh2D->SetColor(mesh2D->color);


				// трансформации
				mesh2D->SetPosition(Vector2D(580, 450)); 
				mesh2D->SetScale(Vector2D(1, 1));// строим эллипс
				mesh2D->SetRotation(Angle::HALF_PI);
				


				mesh2D->Rename("mesh2D.2");
				scene->AddObject(mesh2D);// добавить в сцену
				scene->AddGraphicsObject(mesh2D);
			}

			// двухмерная сетка...
			{
				GraphicsMesh2D* mesh2D = new GraphicsMesh2D();


				mesh2D->CreateQuad2D(ScreenQuad(Vector2D(), Size2D(40, 40)));


				//mesh2D->AutoResizeColors();
				mesh2D->SetColor(mesh2D->color);


				// трансформации
				mesh2D->SetPosition(Vector2D(180, 490)); 
				mesh2D->SetScale(Vector2D(1, 1));// строим эллипс
				mesh2D->SetRotation(Angle::HALF_PI);
				


				mesh2D->Rename("mesh2D.3");
				scene->AddObject(mesh2D);// добавить в сцену
				scene->AddGraphicsObject(mesh2D);
			}

			// трехмерная сетка...
			{
				GraphicsMesh3D* mesh3D = new GraphicsMesh3D();
				
				// описываем вершины квада против часовой стрелки, в двухмерной декартовой системе координат
				mesh3D->CreateQuad2D(Quad<Vector3D>(Vector3D(), Size3D(40,40,20)));// создать квад
				mesh3D->TotalPolygonsDivide4();// увеличиваем количество полигонов в 4 раза

				//mesh3D->AutoResizeColors();
				mesh3D->SetColor(mesh3D->color);


				// трансформации
				mesh3D->SetPosition(Vector3D(480, 350, -1)); 
				mesh3D->SetScale(Vector3D(2, 2, 0));// строим эллипс
				mesh3D->SetRotation(Quaternion(Vector3D::UNIT_Z, Angle::HALF_PI));
				

				mesh3D->Rename("mesh3D.1");
				scene->AddObject(mesh3D);// добавить в сцену
				scene->AddGraphicsObject(mesh3D);
			}
			
					
			// трехмерная сетка...
			{
				GraphicsMesh3D* mesh3D = new GraphicsMesh3D();
				
				// описываем вершины квада против часовой стрелки, в двухмерной декартовой системе координат
				mesh3D->CreateCircle2D(Circle<Vector3D>(Vector3D(), 50), 30);// создать окружность
				mesh3D->TotalPolygonsDivide4();// увеличиваем количество полигонов в 4 раза

				//mesh3D->AutoResizeColors();
				mesh3D->SetColor(mesh3D->color);


				// трансформации
				mesh3D->SetPosition(Vector3D(570, 430, -1)); 
				

				mesh3D->Rename("mesh3D.2");
				scene->AddObject(mesh3D);// добавить в сцену
				scene->AddGraphicsObject(mesh3D);
			}

			// двухмерная точка...
			{
				GraphicsPoint2D* point2D = new GraphicsPoint2D();
	

				// трансформации
				point2D->SetPosition(Vector2D(140, 440)); 


				point2D->Rename("point2D");
				scene->AddObject(point2D);// добавить в сцену
				scene->AddGraphicsObject(point2D);
			}


			// трехмерная точка...
			{
				GraphicsPoint3D* point3D = new GraphicsPoint3D();
	

				// трансформации
				point3D->SetPosition(Vector3D(Renderer::_Picked()->ScreenToClient(Renderer::_Picked()->ClientAreaCenter()).x, Renderer::_Picked()->ScreenToClient(Renderer::_Picked()->ClientAreaCenter()).y, -1)); 


				point3D->Rename("point3D");
				scene->AddObject(point3D);// добавить в сцену
				scene->AddGraphicsObject(point3D);
			}

			// камера...
			{
				Camera3D::Define def;
				def.name = "camera";
		
				def.projectionType = Camera3D::ORTHOGRAPHIC_PROJECTION;
				def.rotationEnabled = false;
				camera = new Camera3D();
				camera->Create(def);
				camera->Pick(Title()); 
				scene->AddObject(camera);
				
				
				currentProjectionType = "ORTHOGRAPHIC";
			}

			GraphicsPoint<Vector3D>::_SetPointSize(6);// размер точек 
			GraphicsLine<Vector3D>::_SetLineWidth(3);// толщина линий

			//Mouse::_ShowCursor(false);// скрыть курсор
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

	    // вывод текста...
		camera->BeginScreen2D();
		

		scene->Draw();// отображение сцены

		//camera->End2D();// начать вывод трехмерной графики	
 

		// вывод текста...
		//camera->BeginScreen2D();
		

		//int i = 0;
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "vertices: %d", 0 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 1 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 2 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 3 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 4 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %d", 5 % 2);
		//font2d->DrawScreenText(Vector3D(10, i+=20, -1), "indices: %s", Bitset::_String(stipple).c_str());
	    

		//camera->End2D();// начать вывод трехмерной графики	
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
			if(_event.key == Keyboard::F1)
			{
				SetWindowStyle(Window::FULLSCREEN);
			}
			else if(_event.key == Keyboard::F2)
			{
				SetWindowStyle(Window::WND_CLASSIC);
			}
			else if(_event.key == Keyboard::ESC)
			{
				Destroy();// уничтожить окно
			} 
			else if(_event.key == Keyboard::C)
			{
				SetFrontFaceClockwise(!FrontFaceClockwise());
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
	def.realTimeWorkerDef.windowDef.tittle = "2D Graphics.example_01";
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
