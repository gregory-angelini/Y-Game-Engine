
#include "Graph.h"
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>
#include <Engine/Core/Time/HMS/HMS.h>
#include <Engine/Core/Shape/Quad/Quad.h>




Graph::Function::Function(void): graph(NIL), lineWidth(3.0f), yMaximum(0.0f), xMarkerWidth(3.0f), yMarkerWidth(3.0f), xMarkerShowEnable(true), yMarkerShowEnable(true)
{
	SetColorRGBA(RGBA(55, 111, 194, 255).GetRGBAf());
}


Graph::Function::Function(StringANSI _name): graph(NIL), lineWidth(3.0f), yMaximum(0.0f), xMarkerWidth(3.0f), yMarkerWidth(3.0f), xMarkerShowEnable(true), yMarkerShowEnable(true)
{
	SetColorRGBA(RGBA(55, 111, 194, 255).GetRGBAf());
	Rename(_name);
}

	
void Graph::Function::SetLineWidth(float _width)
{
	lineWidth = _width;
}
			

float Graph::Function::GetLineWidth(void)const
{
	return lineWidth;
}


float Graph::Function::GetYMaximum(void)const
{
	return yMaximum;
}


Vector3 Graph::Function::GetCurrentXPoint(void)
{
	if(history.IsNotEmpty()) 
	{
		float secondsInTile = 1.0f * graph->size.x / graph->columns;

		Vector2 halfSize = graph->size / 2.0f;

		return Vector3((GetCurrentX() * secondsInTile) / graph->scaleX, 0.0f, 0.0f) - Vector3(halfSize.x, 0.0f, 0.0f);
	}
	return Vector3();
}


Vector3 Graph::Function::GetCurrentYPoint(void)
{
	if(history.IsNotEmpty()) 
	{
		float unitsInTile = HMS::SECOND_IN_MINUTE * graph->size.y / graph->rows;

		Vector2 halfSize = graph->size / 2.0f;
	
		return Vector3(0.0f, ( history.GetTail()->key.y * unitsInTile) / graph->scaleY, 0.0f) - Vector3(halfSize.x, 0.0f, 0.0f);
	}
	return Vector3();
}


float Graph::Function::GetCurrentX(void)
{
	if(history.IsNotEmpty())
	{
		float result = 0.0f;
		for(SimpleList<Vector2>::Node* it = history.GetHead(); it != NIL; it = it->GetRight())
		{
			result += it->key.x;
		}
		return result;
	}
	return 0.0f;
}


float Graph::Function::GetCurrentY(void)
{
	if(history.IsNotEmpty())
	{
		float result = 0.0f;
		for(SimpleList<Vector2>::Node* it = history.GetHead(); it != NIL; it = it->GetRight())
		{
			result += it->key.y;
		}
		return result;
	}
	return 0.0f;
}




void Graph::Function::Draw(void)
{
	if(showEnable)
	{
		float preLineWidth = Renderer::_GetLineWidth();
		Renderer::_SetLineWidth(lineWidth);
			 
		float secondsInTile = graph->size.x / graph->columns;
		float unitsInTile = HMS::SECOND_IN_MINUTE * graph->size.y / graph->rows;
		float x = 0.0f, y = 0.0f;
		Vector2 halfSize = graph->size / 2.0f;
		Vector3 point1, point2;

		float cellWidth = graph->size.x / graph->columns;
		float cellHeight = graph->size.y / graph->rows;

		Vector3 height = graph->GetPlane().GetNormal() * graph->overlayHeight;
		Quad quad(graph->GetPosition(), graph->size);
		for(SimpleList<Vector2>::Node* it = history.GetHead(); it != NIL; it = it->GetRight())
		{
			x += it->key.x;
			y = it->key.y;
			{
				point2 = Vector3((x * secondsInTile) / graph->scaleX, (y * unitsInTile) / graph->scaleY, 0.0f) - Vector3(halfSize.x, 0.0f, 0.0f) + height;

				if(it != history.GetHead())
				{
					float speed = y * HMS::SECOND_IN_MINUTE;
					
					if(speed > yMaximum)
					{
						yMaximum = speed; 
					}
					point1 = quad.ClampPoint(point1);
					point2 = quad.ClampPoint(point2);

					GraphicsLine::_Draw(point1, point2, GetColorRGBA());
				}
				else
				{
					point1 = -Vector3(halfSize.x, 0.0f, 0.0f);
					float speed = y * HMS::SECOND_IN_MINUTE;
					
					if(speed > yMaximum)
					{
						yMaximum = speed; 
					}
					Quad quad(graph->GetPosition(), graph->size);
					point1 = quad.ClampPoint(point1);
					point2 = quad.ClampPoint(point2);

					GraphicsLine::_Draw(point1, point2, GetColorRGBA());
				}
						
				point1 = point2;
			}
		}
		

		height *= 1.001f;

		if(xMarkerShowEnable)
		{
			if(history.GetSize() > 1)
			{
				Renderer::_SetLineWidth(xMarkerWidth);

				point1 = Vector3((x * secondsInTile) / graph->scaleX, 0.0f, 0.0f) - Vector3(halfSize.x, 0.0f, 0.0f) + height;
				point2 = point1;
				Quad quad(graph->GetPosition(), graph->size);
				point1 = quad.ClampPoint(point1);
				point2 = quad.ClampPoint(point2);
				point1.y += -cellHeight / 5.0f;
				point2.y += cellHeight / 5.0f;

				GraphicsLine::_Draw(point1, point2, xMarkerColor);
				
				if(font.Get()) 
				{ 
					font.Get()->SetColorRGBA(GetColorRGBA());
					font.Get()->DrawWorldText(point1 + Vector3(0.0f, -cellHeight / 5.0f, 0.0f), " %.4f", x); 
				}
			}
		}

		if(yMarkerShowEnable)
		{
			if(history.GetSize() > 1)
			{ 
				Renderer::_SetLineWidth(yMarkerWidth);

				point1 = Vector3(0.0f, (y * unitsInTile) / graph->scaleY, 0.0f) - Vector3(halfSize.x, 0.0f, 0.0f) + height;
				point2 = point1;
				Quad quad(graph->GetPosition(), graph->size);
				point1 = quad.ClampPoint(point1);
				point2 = quad.ClampPoint(point2);
				point1.x += -cellWidth / 5.0f;
				point2.x += cellWidth / 5.0f;

				GraphicsLine::_Draw(point1, point2, yMarkerColor);

				if(font.Get()) 
				{ 
					font.Get()->SetColorRGBA(GetColorRGBA());
					font.Get()->DrawWorldText(point1 + Vector3(0.0f, cellHeight / 5.0f, 0.0f), "%.4f", y); 
				}
			}
		}

		Renderer::_SetLineWidth(preLineWidth);
	}
}


void Graph::Function::SetXMarkerColor(Vector4 _color)
{
	xMarkerColor = _color;
}


void Graph::Function::SetXMarkerColor(RGBA _color)
{
	xMarkerColor = _color.GetRGBAf();
}


Vector4 Graph::Function::GetXMarkerColor(void)const
{
	return xMarkerColor;
}


void Graph::Function::SetYMarkerColor(Vector4 _color)
{
	yMarkerColor = _color;
}


void Graph::Function::SetYMarkerColor(RGBA _color)
{
	yMarkerColor = _color.GetRGBAf();
}


Vector4 Graph::Function::GetYMarkerColor(void)const
{
	return yMarkerColor;
}


void Graph::Function::ClearHistory(void)
{
	history.Clear();
}



void Graph::Function::SetYMarkerShowEnable(bool _enable)
{
	yMarkerShowEnable = _enable;
}


bool Graph::Function::IsYMarkerShowEnabled(void)const
{
	return yMarkerShowEnable;
}



void Graph::Function::SetXMarkerShowEnable(bool _enable)
{
	xMarkerShowEnable = _enable;
}


bool Graph::Function::IsXMarkerShowEnabled(void)const
{
	return xMarkerShowEnable;
}



void Graph::Function::SetXMarkerWidth(float _width)
{
	xMarkerWidth = _width;
}


float Graph::Function::GetXMarkerWidth(void)const
{
	return xMarkerWidth;
}


void Graph::Function::SetYMarkerWidth(float _width)
{
	yMarkerWidth = _width;
}


float Graph::Function::GetYMarkerWidth(void)const
{
	return yMarkerWidth;
}


bool Graph::Function::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}

					
bool Graph::Function::LoadFromFile(File& _file)
{
	return false;
}



void Graph::Function::SetFont(Font2D* _font)
{
	font.Clear();
	font.Attach(_font);
}
