#include "SplitTriangulation.h"


SplitTriangulation::Point::Point(void)
{}



SplitTriangulation::Point::Point(float _x, float _y): Vector2(_x, _y), index(0)
{}



SplitTriangulation::Point::Point(Vector2 _vector, uint32 _index): Vector2(_vector.x, _vector.y), index(_index)
{}



SplitTriangulation::Point::Point(Vector2 _vector): Vector2(_vector.x, _vector.y), index(0)
{}



bool SplitTriangulation::Point::operator == (const Point& _point)const
{
	if(x == _point.x && y == _point.y)
	{
		return true;
	}
	return false; 
}


bool SplitTriangulation::Point::operator != (const Point& _point)const
{
	return !(*this == _point);
}



SplitTriangulation::Edge::Edge(void)
{}



SplitTriangulation::Edge::Edge(Point _origin, Point _end): origin(_origin), end(_end)
{}


SplitTriangulation::Edge::~Edge(void)
{}



int32 SplitTriangulation::Edge::_GetPointPosition(Edge _line, Point _point)
{
	if(_line.origin == _point) { return ORIGIN; }
	if(_line.end == _point) { return END; }

	Vector2 a = _line.end - _line.origin;
	Vector2 b = _point - _line.origin;
	float sa = a.x * b.y - b.x * a.y;

	if(sa > 0.0f) { return LEFT; }
	if(sa < 0.0f) { return RIGHT; }
	if(a.GetLength() < b.GetLength()) { return BEYOND; }
	if((a.x * b.x < 0.0f) || (a.y * b.y < 0.0f)) { return BEHIND; }
	return BETWEEN;
}


bool SplitTriangulation::Edge::_IsIntersected(Edge _a, Edge _b, float& _t)
{
	Point a = _a.origin;
	Point b = _a.end;
	Point c = _b.origin;
	Point d = _b.end;
	Point n = Point((d - c).y, (c - d).x);
	float denom = Vector2::_GetDot(n, b - a);
	
	if(denom == 0.0f)
	{
		return false;
	}
	float num = Vector2::_GetDot(n, a - c);
	_t = -num / denom;
	return true;
}



void SplitTriangulation::Edge::Rotate(void)
{
	Vector2 m = (origin + end) * 0.5f;
	Vector2 v = end - origin;
	Vector2 n(v.y, -v.x);
	origin = Point(m - n * 0.5f);
	end = Point(m + n * 0.5f);
}



void SplitTriangulation::Edge::Flip(void)
{
	Common::_Swap(origin, end);
}


float SplitTriangulation::Edge::_GetDistance(Edge _edge, Point _point)
{
	Edge ab = _edge;
	ab.Flip();
	ab.Rotate();
	Point n(ab.end - ab.origin);
	n.Normalize();
	Edge f(_point, _point + n);

	float t;
	Edge::_IsIntersected(f, _edge, t);
	return t;
}




static int32 _GetAmountOfTriangles(int32 _vertices)
{
	if(_vertices >=3)
	{
		return _vertices - 2;
	}
	return 0;
}



MagicList<SplitTriangulation::Point> SplitTriangulation::_Split(MagicList<Point>& _vertices, Point* _a, Point* _c)
{
	MagicList<Point>::AddressDerived* a2 = _vertices.AddToRight(_a, new Point(Vector2(_a->x, _a->y), _a->index));
	MagicList<Point>::AddressDerived* c2 = _vertices.AddToLeft(_c, new Point(Vector2(_c->x, _c->y), _c->index));

	MagicList<Point> polygons; 
	_vertices.MoveNodeRangeTo(a2->GetNode(), c2->GetNode(), polygons);
	return polygons; 
}



bool SplitTriangulation::_IsPointInside(Point _a, Point _b, Point _c, Point _point)
{
	return((Edge::_GetPointPosition(Edge(_a, _b), _point) != Edge::LEFT) &&
           (Edge::_GetPointPosition(Edge(_b, _c), _point) != Edge::LEFT) &&
           (Edge::_GetPointPosition(Edge(_c, _a), _point) != Edge::LEFT));
}




bool SplitTriangulation::_IsClockwise(SimpleList<Vector3>& _vertices)
{ 
	if(_vertices.GetSize() >= 3)
	{ 
		SimpleList<Vector3>::Node* it = _vertices.GetHead();
		float s = 0.0f;
		Vector2 a, b;
		for(int32 i = 0; i < _vertices.GetSize() - 1; i++)
		{
			a = Vector3::_ToVector2(it->key);
			b = Vector3::_ToVector2(it->GetRight()->key);
			s += a.x * b.y - b.x * a.y;
			it = it->GetRight();
		}
		a = Vector3::_ToVector2(_vertices.GetTail()->key);
		b = Vector3::_ToVector2(_vertices.GetHead()->key);
		s += a.x * b.y - b.x * a.y;

		s *= 0.5f;
		if(s < 0.0f) { return true; }
	}
	return false;
}




SplitTriangulation::Point* SplitTriangulation::_FindIntrudingPoint(MagicList<Point>& _vertices, Point* _convex)
{ 
	Point* a = _vertices.GetLoopedLeftOf(_convex);
	Point* b = _convex;
	Point* c = _vertices.GetLoopedRightOf(_convex);

	Edge ca(*c, *a);
	float maxDist = -1.0f;
	Point* v = _vertices.GetLoopedRightOf(c);
	Point *d = NIL; 
	while(v != a) 
	{
		if(_IsPointInside(*a, *b, *c, *v))
		{
			float dist = Edge::_GetDistance(ca, *v);
			
			if(dist > maxDist)
			{
				d = v;
				maxDist = dist;
			}
		}
		v = _vertices.GetLoopedRightOf(v);
	}
	return d;
}




SplitTriangulation::Point* SplitTriangulation::_FindConvexPoint(MagicList<Point>& _vertices, Point* _convex)
{
	Point* a = _vertices.GetLoopedLeftOf(_convex);
	Point* b = _convex;
	Point* c = _vertices.GetLoopedRightOf(_convex);
  
	while(Edge::_GetPointPosition(Edge(*a, *b), *c) != Edge::RIGHT)
	{
		a = b;
		_convex = b = c;
		c = _vertices.GetLoopedRightOf(b);
	} 
	return _convex;
}



Vector3 SplitTriangulation::_ToVector3(Point* _point, float _z)
{
	return Vector3(_point->x, _point->y, _z);
}



bool SplitTriangulation::_VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Triangle>& _triangles, float _z)
{ 
	if(_vertices.GetSize() >= 3)
	{
		MagicList<Point> points;
		if(_IsClockwise(_vertices))
		{
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				points.AddToTail(new Point(it->key.x, it->key.y));
			}
		}
		else
		{
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				points.AddToHead(new Point(it->key.x, it->key.y));
			}
		}

		_VerticesToTriangles(points, _triangles, points.GetTail(), _z);
		return true;
	}
	return false;
}




void SplitTriangulation::_VerticesToTriangles(MagicList<Point>& _vertices, SimpleList<Triangle>& _triangles, Point* _convex, float _z)
{
	if(_vertices.GetSize() == 3)
	{
		_triangles.AddToTail(Triangle(_ToVector3(_vertices.GetHead(), _z), _ToVector3(_vertices.GetTail(), _z), _ToVector3(_vertices.GetTailAddress()->GetLeft()->GetNode(), _z)));
	}
	else
	{
		_convex = _FindConvexPoint(_vertices, _convex);

		Point* d = _FindIntrudingPoint(_vertices, _convex);
		
		if(d == NIL)
		{
			Point* c = _vertices.GetLoopedRightOf(_convex);
			Point* a = _vertices.GetLoopedLeftOf(_convex); 
			
			MagicList<Point> q = _Split(_vertices, a, c);

			_convex = a;

			_VerticesToTriangles(_vertices, _triangles, _convex, _z);
			
			_triangles.AddToTail(Triangle(_ToVector3(q.GetHead(), _z), _ToVector3(q.GetTail(), _z), _ToVector3(q.GetTailAddress()->GetLeft()->GetNode(), _z)));
		}
		else
		{
			MagicList<Point> q = _Split(_vertices, _convex, d);

		    _VerticesToTriangles(q, _triangles, q.GetTail(), _z);
			_VerticesToTriangles(_vertices, _triangles, _convex, _z);
		} 
	}
}





bool SplitTriangulation::_VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Vector3>& _triangles, float _z)
{ 
	if(_vertices.GetSize() >= 3)
	{
		MagicList<Point> points;
		if(_IsClockwise(_vertices))
		{
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				points.AddToTail(new Point(it->key.x, it->key.y));
			}
		}
		else
		{
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				points.AddToHead(new Point(it->key.x, it->key.y));
			}
		}

		_VerticesToTriangles(points, _triangles, points.GetTail(), _z);
		return true;
	}
	return false;
}




void SplitTriangulation::_VerticesToTriangles(MagicList<Point>& _vertices, SimpleList<Vector3>& _triangles, Point* _convex, float _z)
{
	if(_vertices.GetSize() == 3)
	{
		_triangles.AddToTail(_ToVector3(_vertices.GetHead(), _z));
		_triangles.AddToTail(_ToVector3(_vertices.GetTail(), _z));
		_triangles.AddToTail(_ToVector3(_vertices.GetTailAddress()->GetLeft()->GetNode(), _z));
	}
	else
	{
		_convex = _FindConvexPoint(_vertices, _convex);

		Point* d = _FindIntrudingPoint(_vertices, _convex);
		
		if(d == NIL)
		{
			Point* c = _vertices.GetLoopedRightOf(_convex);
			Point* a = _vertices.GetLoopedLeftOf(_convex); 
			
			MagicList<Point> q = _Split(_vertices, a, c);

			_convex = a;

			_VerticesToTriangles(_vertices, _triangles, _convex, _z);
			
			_triangles.AddToTail(_ToVector3(q.GetHead(), _z));
			_triangles.AddToTail(_ToVector3(q.GetTail(), _z));
			_triangles.AddToTail(_ToVector3(q.GetTailAddress()->GetLeft()->GetNode(), _z)); 
		}
		else
		{
			MagicList<Point> q = _Split(_vertices, _convex, d);

		    _VerticesToTriangles(q, _triangles, q.GetTail(), _z);
			_VerticesToTriangles(_vertices, _triangles, _convex, _z);
		} 
	}
}




bool SplitTriangulation::_VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<uint32>& _indices)
{ 
	if(_vertices.GetSize() >= 3)
	{
		MagicList<Point> points;
		int32 i = 0;
		if(_IsClockwise(_vertices))
		{
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				Point* point = new Point(it->key.x, it->key.y);
				point->index = i;
				points.AddToTail(point);
				i++;
			}
		}
		else
		{
			for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
			{
				Point* point = new Point(it->key.x, it->key.y);
				point->index = i;
				points.AddToHead(point);
				i++;
			}
		}

		_VerticesToTriangles(points, _indices, points.GetTail());
		return true;
	}
	return false;
}



void SplitTriangulation::_VerticesToTriangles(MagicList<Point>& _vertices, SimpleList<uint32>& _indices, Point* _convex)
{
	if(_vertices.GetSize() == 3)
	{
		_indices.AddToTail(_vertices.GetHead()->index);
		_indices.AddToTail(_vertices.GetTail()->index);
		_indices.AddToTail(_vertices.GetTailAddress()->GetLeft()->GetNode()->index);
	}
	else
	{
		_convex = _FindConvexPoint(_vertices, _convex);

		Point* d = _FindIntrudingPoint(_vertices, _convex);
		
		if(d == NIL)
		{
			Point* c = _vertices.GetLoopedRightOf(_convex);
			Point* a = _vertices.GetLoopedLeftOf(_convex); 
			
			MagicList<Point> q = _Split(_vertices, a, c);

			_convex = a;

			_VerticesToTriangles(_vertices, _indices, _convex);
			
			_indices.AddToTail(q.GetHead()->index);
			_indices.AddToTail(q.GetTail()->index);
			_indices.AddToTail(q.GetTailAddress()->GetLeft()->GetNode()->index); 
		}
		else
		{
			MagicList<Point> q = _Split(_vertices, _convex, d);

		    _VerticesToTriangles(q, _indices, q.GetTail());
			_VerticesToTriangles(_vertices, _indices, _convex);
		} 
	}
}