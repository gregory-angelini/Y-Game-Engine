#include "DelaunayTriangulation.h"


DelaunayTriangulation::Point::Point(void)
{}



DelaunayTriangulation::Point::Point(float _x, float _y): Vector2(_x, _y), index(0)
{}



DelaunayTriangulation::Point::Point(Vector2 _vector): Vector2(_vector.x, _vector.y), index(0)
{}



bool DelaunayTriangulation::Point::operator == (const Point& _point)const
{
	if(x == _point.x && y == _point.y)
	{
		return true;
	}
	return false; 
}



bool DelaunayTriangulation::Point::operator < (const Point& _point)const
{
	return x < _point.x || (x == _point.x && y < _point.y);
}



bool DelaunayTriangulation::Point::operator > (const Point& _point)const
{
	return x > _point.x || (x == _point.x && y > _point.y);
}




DelaunayTriangulation::Edge::Edge(void)
{}



DelaunayTriangulation::Edge::Edge(Point _origin, Point _end): origin(_origin), end(_end)
{}


DelaunayTriangulation::Edge::~Edge(void)
{}



void DelaunayTriangulation::Edge::Rotate(void)
{
	Vector2 m = (origin + end) * 0.5f;
	Vector2 v = end - origin;
	Vector2 n(v.y, -v.x);
	origin = Point(m - n * 0.5f);
	end = Point(m + n * 0.5f);
}



void DelaunayTriangulation::Edge::Flip(void)
{
	Common::_Swap(origin, end);
}




bool DelaunayTriangulation::Edge::_IsIntersected(Edge _a, Edge _b, float& _t)
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




int32 DelaunayTriangulation::Edge::_GetPointPosition(Edge _line, Point _point)
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



bool DelaunayTriangulation::Edge::operator == (const Edge& _edge)const
{
	if(origin == _edge.origin && end == _edge.end)
	{
		return true;
	}
	return false; 
}



bool DelaunayTriangulation::Edge::operator < (const Edge& _edge)const
{
	if(origin < _edge.origin ||
	   origin > _edge.origin ||
	   end > _edge.end) 
	{ 
		return true; 
	}
	return false;
}
		

bool DelaunayTriangulation::Edge::operator > (const Edge& _edge)const
{
	if(end < _edge.end) 
	{ 
		return true; 
	}
	return false;
}



static int32 _DelaunayTriangulation(int32 _vertices)
{
	if(_vertices >=3)
	{
		return _vertices - 2;
	}
	return 0;
}


bool DelaunayTriangulation::_GetMate(Edge _edge, Point _vertices[], int32 _size, Point& _point)
{ 
	Point *d = NIL;
	float t, minDist = Numerical<float>::_UpperValue();
	Edge f = _edge; 
	f.Rotate();
	for(int32 i = 0; i < _size; i++)
	{
		if(Edge::_GetPointPosition(_edge, _vertices[i]) == Edge::RIGHT)
		{
			Edge g(_edge.end, _vertices[i]);
			g.Rotate(); 
			Edge::_IsIntersected(f, g, t);

			if(t < minDist)
			{
				d = &_vertices[i];
				minDist = t;
			}
		}
	}

	if(d)
	{
		_point = *d;
		return true;
	}
	return false;
}



DelaunayTriangulation::Edge DelaunayTriangulation::_GetHullEdge(Point _vertices[], int32 _size)
{ 
	int32 m = 0;
	int32 i;
  
	for(i = 1; i < _size; i++)
	{
		if(_vertices[i] < _vertices[m])
		{
			m = i;
		}
	}

	Common::_Swap(_vertices[0], _vertices[m]);

	for(m = 1, i = 2; i < _size; i++) 
	{
		int32 position = Edge::_GetPointPosition(Edge(_vertices[0], _vertices[m]), _vertices[i]);
	    
		if(position == Edge::LEFT || position == Edge::BETWEEN)
		{
			m = i;
		}
	}
	Common::_Swap(_vertices[0], _vertices[m]);

	return Edge(_vertices[m], _vertices[0]);
}


void DelaunayTriangulation::_UpdateFrontier(SimpleBinaryTree<Edge>& _frontier, Point _v1, Point _v2)
{
	Edge edge(_v1, _v2);
	if(_frontier.Remove(edge))
	{}
	else
	{
		edge.Flip();
		_frontier.Add(edge);
	}
}



bool DelaunayTriangulation::_VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Triangle>& _triangles, float _z)
{    
	if(_vertices.GetSize() >= 3)
	{
		int32 size = _vertices.GetSize();
		Point* points = new Point[size];
		int32 i = 0;

		for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			points[i++] = Point(it->key.x, it->key.y);
		}

		Point point;
		SimpleBinaryTree<Edge> frontier;

		Edge edge = _GetHullEdge(points, size);
		frontier.Add(edge);
		while(!frontier.IsEmpty())
		{
			edge = frontier.GetExtremum(false)->key;
			frontier.RemoveExtremum(false);

			if(_GetMate(edge, points, size, point))
			{
				_UpdateFrontier(frontier, point, edge.origin);
				_UpdateFrontier(frontier, edge.end, point);
				_triangles.AddToTail(Triangle(Vector3(point.x, point.y, _z), Vector3(edge.end.x, edge.end.y, _z), Vector3(edge.origin.x, edge.origin.y, _z)));
			}
		}
		delete[] points;
		return true;
	}
	return false; 
}



bool DelaunayTriangulation::_IsCollinear(Point _p1, Point _p2, Point _p3)
{
	return (_p1.y - _p2.y) * (_p1.x - _p3.x) == (_p1.y - _p3.y) * (_p1.x - _p2.x);
}


bool DelaunayTriangulation::_VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<Vector3>& _triangles, float _z)
{
	if(_vertices.GetSize() >= 3)
	{
		int32 size = _vertices.GetSize();
		Point* points = new Point[size];
		int32 i = 0;

		for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			points[i++] = Point(it->key.x, it->key.y);
		}

		Point point;
		SimpleBinaryTree<Edge> frontier;

		Edge edge = _GetHullEdge(points, size);
		frontier.Add(edge);
		while(!frontier.IsEmpty())
		{
			edge = frontier.GetExtremum(false)->key;
			frontier.RemoveExtremum(false);

			if(_GetMate(edge, points, size, point))
			{
				_UpdateFrontier(frontier, point, edge.origin);
				_UpdateFrontier(frontier, edge.end, point);
				
				_triangles.AddToTail(Vector3(point.x, point.y, _z));
				_triangles.AddToTail(Vector3(edge.end.x, edge.end.y, _z));
				_triangles.AddToTail(Vector3(edge.origin.x, edge.origin.y, _z));
			}
		}
		delete[] points;
		return true;
	}
	return false; 
}


bool DelaunayTriangulation::_VerticesToTriangles(SimpleList<Vector3>& _vertices, SimpleList<uint32>& _indices)
{
	if(_vertices.GetSize() >= 3)
	{
		int32 size = _vertices.GetSize();
		Point* points = new Point[size];
		int32 i = 0;

		for(SimpleList<Vector3>::Node* it = _vertices.GetHead(); it != NIL; it = it->GetRight())
		{
			points[i] = Point(it->key.x, it->key.y);
			points[i].index = i;
			i++;
		}

		Point point;
		SimpleBinaryTree<Edge> frontier;

		Edge edge = _GetHullEdge(points, size);
		frontier.Add(edge);
		while(!frontier.IsEmpty())
		{
			edge = frontier.GetExtremum(false)->key;
			frontier.RemoveExtremum(false);

			if(_GetMate(edge, points, size, point))
			{
				_UpdateFrontier(frontier, point, edge.origin);
				_UpdateFrontier(frontier, edge.end, point);
				
				_indices.AddToTail(point.index);
				_indices.AddToTail(edge.end.index);
				_indices.AddToTail(edge.origin.index);
			}
		}
		delete[] points;
		return true;
	}
	return false;
}