#ifndef DELAUNAYTRIANGULATION_H
#define DELAUNAYTRIANGULATION_H
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/Shape/Triangle/Triangle.h>
#include <Engine/Core/Container/Tree/SimpleBinaryTree/SimpleBinaryTree.h>
class DelaunayTriangulation
{
	private: class Point: public Vector2
	{
		public: uint32 index;
		public: Point(void);
		public: Point(float _x, float _y);
		public: Point(Vector2 _vector);
		public: bool operator == (const Point& _point)const;
		public: bool operator < (const Point& _point)const;
		public: bool operator > (const Point& _point)const;
	};
	private: class Edge
	{
		public: enum 
		{
			LEFT,
			RIGHT,
			BEYOND,
			BEHIND,
			BETWEEN,
			ORIGIN,
			END
		};
		public: Point origin;
		public: Point end;
		public: Edge(void);
		public: Edge(Point _origin, Point _end);
		public: virtual ~Edge(void);
		public: void Rotate(void);
		public: void Flip(void);
		public: static bool _IsIntersected(Edge _a, Edge _b, float& _t);
		public: static int32 _GetPointPosition(Edge _line, Point _point);
		public: bool operator == (const Edge& _edge)const;
		public: bool operator < (const Edge& _edge)const;
		public: bool operator > (const Edge& _edge)const;
	};
	private: static bool _GetMate(Edge _edge, Point _vertices[], int32 _size, Point& _point);
	private: static Edge _GetHullEdge(Point _vertices[], int32 _size);
	private: static void _UpdateFrontier(SimpleBinaryTree<Edge>& _frontier, Point _v1, Point _v2);
	public: static int32 _GetAmountOfTriangles(int32 _vertices);
	private:static bool _IsCollinear(Point _p1, Point _p2, Point _p3);
	public: static bool _VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Triangle>& _triangles, float _z = 0.0f);
	public: static bool _VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Vector3>& _triangles, float _z = 0.0f);
	public: static bool _VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<uint32>& _indices);
};



#endif 