
#include "Graph.h"
#include <Engine/Output/Graphics/Shape/Line/GraphicsLine/GraphicsLine.h>


Graph::Graph(void): size(1.0f, 1.0f), columns(1), rows(2), lineWidth(1.0f), scaleX(1.0f), scaleY(1.0f), xRangeShowEnable(true), yRangeShowEnable(true), overlayHeight(0.2f)
{
	xColor = RGBA(RGBA::GRAY_COLOR).GetRGBAf();
	yColor = RGBA(RGBA::GRAY_COLOR).GetRGBAf();
	SetColorRGBA(RGBA(RGBA::GRAY_COLOR).GetRGBAf());
	SetScale(Vector3(1.0f, 1.0f, 1.0f));
}


Graph::Graph(Vector2 _size, int32 _rows, int32 _columns): lineWidth(1.0f), scaleX(1.0f), scaleY(1.0f), xRangeShowEnable(true), yRangeShowEnable(true), overlayHeight(0.2f)
{
	SetRows(_rows);
	SetColumns(_columns);
	SetSize(_size);

	xColor = RGBA(RGBA::GRAY_COLOR).GetRGBAf();
	yColor = RGBA(RGBA::GRAY_COLOR).GetRGBAf();
	SetColorRGBA(RGBA(RGBA::GRAY_COLOR).GetRGBAf());
	SetScale(Vector3(1.0f, 1.0f, 1.0f));

	xAxisName = "X";
	yAxisName = "Y";
}


Graph::~Graph(void)
{
	Destroying();
}



SimpleList<Graph::Mark>& Graph::GetMarkList(void)
{
	return markList;
}


void Graph::SetOverlayHeight(float _height)
{
	overlayHeight = _height;
}



float Graph::GetOverlayHeight(void)const
{
	return overlayHeight;
}



void Graph::SetScaleX(float _scale)
{
	if(_scale > 0.0f)
	{
		scaleX = _scale;
	}
}


float Graph::GetScaleX(void)const
{
	return scaleX;
}


void Graph::SetScaleY(float _scale)
{
	if(_scale > 0.0f)
	{
		scaleY = _scale;
	}
}

	
float Graph::GetScaleY(void)const
{
	return scaleY;
}


void Graph::SetColorX(Vector4 _color)
{
	xColor = _color;
}


Vector4 Graph::GetColorX(void)const
{
	return xColor;
}


void Graph::SetColorY(Vector4 _color)
{
	yColor = _color;
}


Vector4 Graph::GetColorY(void)const
{
	return yColor;
}


void Graph::SetRows(int32 _rows)
{
	if(_rows > 0 && _rows % 2 == 0)
	{
		rows = _rows;
	}
}


int32 Graph::GetRows(void)const
{
	return rows;
}

			
void Graph::SetColumns(int32 _columns)
{
	if(_columns > 0)
	{
		columns = _columns;
	}
}


int32 Graph::GetColumns(void)const
{
	return columns;
}


void Graph::SetSize(Vector2 _size)
{
	if(_size.x > 0.0f && _size.y > 0.0f)
	{
		size = _size;
	}
} 



Vector2 Graph::GetSize(void)const
{
	return size;
}



void Graph::SetLineWidth(float _width)
{
	lineWidth = _width;
}
		


float Graph::GetLineWidth(void)const
{
	return lineWidth;
}



Graph::Function* Graph::CreateFunction(StringANSI _name)
{ 
	Function* function = new Function(_name);

	if(functions.AddObject(function))
	{
		function->graph = this;
		return function;
	}
	delete function;
	return NIL;
}


void Graph::AddVertex(StringANSI _name, Vector2 _point)
{
	Function* function = dynamic_cast<Function*>(functions.FindObject(_name));

	if(function)
	{
		function->history.AddToTail(_point);
	}
}


Graph::Function* Graph::FindFunction(StringANSI _name)
{
	return dynamic_cast<Function*>(functions.FindObject(_name));
}


void Graph::ClearHistory(void)
{
	ClearHistory(functions.GetRootAddress());
}


void Graph::ClearHistory(ObjectBinaryTree::AddressDerived* _address)
{
	if(_address)
	{
		ClearHistory(_address->GetChild(0));
		ClearHistory(_address->GetChild(1));
		dynamic_cast<Function*>(_address->GetNode())->history.Clear();
	}
}



void Graph::SetXRangeShowEnable(bool _enable)
{
	xRangeShowEnable = _enable;
}


bool Graph::IsXRangeShowEnabled(void)const
{
	return xRangeShowEnable;
}


void Graph::SetYRangeShowEnable(bool _enable)
{
	yRangeShowEnable = _enable;
}


bool Graph::IsYRangeShowEnabled(void)const
{
	return yRangeShowEnable;
}



Plane Graph::GetPlane(void)
{
	return Plane(GetPosition(), GetPosition() - Vector3(size.x, 0.0f, 0.0f), GetPosition() - Vector3(0.0f,size.y, 0.0f));
}





Quad Graph::GetQuad(void)
{
	return Quad(GetPosition(), size);
}


void Graph::Draw(void)
{
	if(showEnable)
	{
		BeginTransform();
		float preLineWidth = Renderer::_GetLineWidth();
		Renderer::_SetLineWidth(lineWidth);

		Vector2 halfSize = size / 2.0f;
		float cellHeight = size.y / rows;

		for(int32 i = 0; i < rows + 1; ++i)
		{
			GraphicsLine::_Draw(Vector3(-halfSize.x,          halfSize.y - i * cellHeight, 0.0f), 
								Vector3(-halfSize.x + size.x, halfSize.y - i * cellHeight, 0.0f), 
								GetColorRGBA());
		}
		float cellWidth = size.x / columns;

		for(int32 i = 0; i < columns + 1; ++i)
		{
			GraphicsLine::_Draw(Vector3(-halfSize.x + i * cellWidth, halfSize.y, 0.0f), 
								Vector3(-halfSize.x + i * cellWidth, halfSize.y - size.y, 0.0f), 
								GetColorRGBA());
		}


		Renderer::_SetLineWidth(lineWidth * 2.0f);
		GraphicsLine::_Draw(Vector3(-halfSize.x, 0.0f, 0.0f), 
							Vector3(halfSize.x, 0.0f, 0.0f), 
							xColor);

		if(font.Get()) 
		{ 
			font.Get()->SetColorRGBA(xColor);
			font.Get()->DrawWorldText(Vector3(halfSize.x + (cellWidth / 3.0f), 0.0f, 0.0f), xAxisName.c_str());
		}
		GraphicsLine::_Draw(Vector3(-halfSize.x, halfSize.y, 0.0f), 
							Vector3(-halfSize.x, -halfSize.y, 0.0f), 
							yColor);

		if(font.Get()) 
		{ 
			font.Get()->SetColorRGBA(yColor);
			font.Get()->DrawWorldText(Vector3(-halfSize.x - (cellWidth / 1.0f), halfSize.y + (cellHeight / 2.0f), 0.0f), yAxisName.c_str());
		}
		float secondsInTile = size.x / columns;
		float unitsInTile = size.y / rows;

		for(SimpleList<Mark>::Node* it = markList.GetHead(); it != NIL; it = it->GetRight())
		{ 
			if(it->key.axis == Mark::AXIS_X)
			{
				Vector3 v1 = Vector3(0, halfSize.y, 0.0f);
				Vector3 v2 = Vector3(0, halfSize.y - size.y, 0.0f);

				v1 += Vector3((it->key.value * secondsInTile) / scaleX, 0.0f, 0.001f) - Vector3(halfSize.x, 0.0f, 0.0f); 
				v2 += Vector3((it->key.value * secondsInTile) / scaleX, 0.0f, 0.001f) - Vector3(halfSize.x, 0.0f, 0.0f); 

				GraphicsLine::_Draw(GetQuad().ClampPoint(v1), GetQuad().ClampPoint(v2), it->key.color);
			}
			else
			{
				Vector3 v1 = Vector3(-halfSize.x, 0.0f, 0.0f);
				Vector3 v2 = Vector3(-halfSize.x + size.x, 0.0f, 0.0f);

				v1 += Vector3(0.0f, (it->key.value * unitsInTile) / scaleY, 0.001f); 
				v2 += Vector3(0.0f, (it->key.value * unitsInTile) / scaleY, 0.001f);

				GraphicsLine::_Draw(GetQuad().ClampPoint(v1), GetQuad().ClampPoint(v2), it->key.color);
			}
		}
		Draw(functions.GetRootAddress());
		if(font.Get()) 
		{ 
			if(xRangeShowEnable)
			{
				font.Get()->SetColorRGBA(xColor);
				for(int32 i = 0; i < columns; i++)
				{
					Vector3 point = Vector3(((size.x / columns) * i) - halfSize.x + (cellWidth * 0.1f), -halfSize.y - cellHeight, 0.0f);
					font.Get()->DrawWorldText(point, "%d", i * (int32)scaleX); 
				}
			}
				
			if(yRangeShowEnable)
			{
				font.Get()->SetColorRGBA(yColor);
				for(int32 i = 1; i < rows / 2 + 1; i++)
				{
					Vector3 point = Vector3(-halfSize.x - (cellWidth * 1.3f), ((size.y / rows) * (i - 1) + (cellHeight * 0.3f)), 0.0f);
					font.Get()->DrawWorldText(point, " %d", i * (int32)scaleY); 
				}
				for(int32 i = 1; i < rows / 2 + 1; i++)
				{
					Vector3 point = Vector3(-halfSize.x - (cellWidth * 1.3f), -((size.y / rows) * i), 0.0f);
					font.Get()->DrawWorldText(point, "-%d", i * (int32)scaleY); 
				}
			}
		}
		
		Renderer::_SetLineWidth(preLineWidth);
		EndTransform();
	}
}
	

void Graph::Draw(ObjectBinaryTree::AddressDerived* _address)
{
	if(_address)
	{
		Draw(_address->GetChild(0));
		Draw(_address->GetChild(1));
		dynamic_cast<Function*>(_address->GetNode())->Draw();
	}
}



void Graph::SetXAxisName(StringANSI _name)
{
	xAxisName = _name;
}

	
StringANSI Graph::GetXAxisName(void)const
{
	return xAxisName;
}

		
void Graph::SetYAxisName(StringANSI _name)
{
	yAxisName = _name;
}


StringANSI Graph::GetYAxisName(void)const
{
	return yAxisName;
}



bool Graph::LoadFromFile(StringANSI _path, bool _auto)
{
	return false;
}


	
bool Graph::LoadFromFile(File& _file)
{
	return false;
}



void Graph::SetFont(Font2D* _font)
{
	font.Clear();
	font.Attach(_font);
}
