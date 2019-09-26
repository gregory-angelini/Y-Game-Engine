#ifndef SPLITTRIANGULATION_H
#define SPLITTRIANGULATION_H
#include <Engine/Core/Container/List/SimpleList/SimpleList.h>
#include <Engine/Core/Shape/Triangle/Triangle.h>
#include <Engine/Core/Container/Tree/SimpleBinaryTree/SimpleBinaryTree.h>
#include <Engine/Core/Container/List/MagicList/MagicList.h>
class SplitTriangulation
{
    private: class Point: public MagicNode,
		                  public Vector2
	{
		public: uint32 index;
		public: Point(void);
		public: Point(float _x, float _y);
		public: Point(Vector2 _vector);
		public: Point(Vector2 _vector, uint32 _index);
		public: bool operator == (const Point& _point)const;
		public: bool operator != (const Point& _point)const;
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
		public: static int32 _GetPointPosition(Edge _line, Point _point);
	    public: static float _GetDistance(Edge _edge, Point _point);
		public: static bool _IsIntersected(Edge _a, Edge _b, float& _t);
	};
	private: static MagicList<Point> _Split(MagicList<Point>& _vertices, Point* _a, Point* _c);
	private: static bool _IsPointInside(Point _a, Point _b, Point _c, Point _point);
	private: static Point* _FindIntrudingPoint(MagicList<Point>& _vertices, Point* _convex);
	private: static Point* _FindConvexPoint(MagicList<Point>& _vertices, Point* _convex);
	private: static Vector3 _ToVector3(Point* _point, float _z);
	private: static bool _IsClockwise(SimpleList<Vector3>& _vertices);
	public: static int32 _GetAmountOfTriangles(int32 _vertices);
	public: static bool _VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Triangle>& _triangles, float _z = 0.0f);
	public: static bool _VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Vector3>& _triangles, float _z = 0.0f);
	public: static bool _VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<uint32>& _indices);
	private: static void _VerticesToTriangles(MagicList<Point>& _vertices, SimpleList<Triangle>& _triangles, Point* _convex, float _z);
	private: static void _VerticesToTriangles(MagicList<Point>& _vertices, SimpleList<Vector3>& _triangles, Point* _convex, float _z);
	private: static void _VerticesToTriangles(MagicList<Point>& _vertices, SimpleList<uint32>& _indices, Point* _convex);
};



#endif 