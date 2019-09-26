#ifndef GRAPH_H
#define GRAPH_H
#include <Engine/Output/Graphics/Object/GraphicsObject/GraphicsObject.h>
#include <Engine/Output/Graphics/Shape/Point/GraphicsPoint/GraphicsPoint.h>
#include <Engine/Core/Shape/Plane/Plane.h>
#include <Engine/Core/Shape/Quad/Quad.h>
#include <Engine/Output/Graphics/Font/Font2D/Font2D.h>
class Graph: public GraphicsObject
{
	public: class Function: public GraphicsPoint
	{
		friend class Graph;
		private: SimpleList<Vector2> history;
		private: float lineWidth;
		private: Graph* graph;
		private: float yMaximum;
		private: Vector4 xMarkerColor, yMarkerColor;
        private: float xMarkerWidth, yMarkerWidth;
		private: bool xMarkerShowEnable, yMarkerShowEnable;
		private: MagicPointer<Font2D> font;
		private: Function(void);
		private: Function(StringANSI _name);
		public: float GetYMaximum(void)const;
		public: void ClearHistory(void);
		public: float GetCurrentX(void);
		public: Vector3 GetCurrentXPoint(void);
		public: float GetCurrentY(void);
		public: Vector3 GetCurrentYPoint(void);
		public: void SetLineWidth(float _width);
		public: float GetLineWidth(void)const;
	    protected: virtual void Draw(void);
		public: void SetXMarkerColor(Vector4 _color);
		public: void SetXMarkerColor(RGBA _color);
		public: Vector4 GetXMarkerColor(void)const;
		public: void SetYMarkerColor(Vector4 _color);
		public: void SetYMarkerColor(RGBA _color);
		public: Vector4 GetYMarkerColor(void)const;
		public: void SetXMarkerWidth(float _width);
		public: float GetXMarkerWidth(void)const;
		public: void SetYMarkerWidth(float _width);
		public: float GetYMarkerWidth(void)const;
		public: void SetXMarkerShowEnable(bool _enable);
		public: bool IsXMarkerShowEnabled(void)const;
		public: void SetYMarkerShowEnable(bool _enable);
		public: bool IsYMarkerShowEnabled(void)const;
		public: void SetFont(Font2D* _font);
		protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
		protected: virtual bool LoadFromFile(File& _file);
	};
	public: class Mark
	{
		public: enum Axis
		{
			AXIS_X,
			AXIS_Y
		};
		public: float value;
		public: Vector4 color;
		public: Axis axis;
		public: Mark(void): value(0.0f), axis(AXIS_X)
		{
			color = RGBA(55, 111, 194, 255).GetRGBAf();
		}
		public: Mark(float _value, Axis _axis, Vector4 _color): value(_value), axis(_axis), color(_color)
		{}
		public: Mark(float _value, Axis _axis, RGBA _color): value(_value), axis(_axis)
		{
			color = _color.GetRGBAf();
		}
	};
	private: ObjectBinaryTree functions;
	private: Vector2 size;
	private: int32 columns;
	private: int32 rows;
	private: float lineWidth;
	private: Vector4 xColor, yColor;
	private: float scaleX, scaleY;
	private: bool xRangeShowEnable, yRangeShowEnable;
	private: StringANSI xAxisName, yAxisName;
	private: float overlayHeight;
	private: SimpleList<Mark> markList;
	private: MagicPointer<Font2D> font;
	public: Graph(void);
	public: Graph(Vector2 _size, int32 _rows, int32 _columns);
	public: virtual ~Graph(void);
	public: void SetRows(int32 _rows);
	public: int32 GetRows(void)const;
	public: void SetColumns(int32 _columns);
	public: int32 GetColumns(void)const;
	public: void SetSize(Vector2 _size);
	public: Vector2 GetSize(void)const;
	public: Quad GetQuad(void);
	public: Plane GetPlane(void);
	public: Function* CreateFunction(StringANSI _name);
	public: void AddVertex(StringANSI _name, Vector2 _point);
    public: Function* FindFunction(StringANSI _name);
	public: void ClearHistory(void);
	private: void ClearHistory(ObjectBinaryTree::AddressDerived* _address);
	public: SimpleList<Mark>& GetMarkList(void);
    public: void SetOverlayHeight(float _height);
    public: float GetOverlayHeight(void)const;
	public: void SetLineWidth(float _width);
	public: float GetLineWidth(void)const;
	public: void SetXRangeShowEnable(bool _enable);
	public: bool IsXRangeShowEnabled(void)const;
	public: void SetYRangeShowEnable(bool _enable);
	public: bool IsYRangeShowEnabled(void)const;
	public: void SetScaleX(float _scale);
	public: float GetScaleX(void)const;
	public: void SetScaleY(float _scale);
	public: float GetScaleY(void)const;
	public: void SetColorX(Vector4 _color);
	public: Vector4 GetColorX(void)const;
	public: void SetColorY(Vector4 _color);
	public: Vector4 GetColorY(void)const;
	public: virtual void Draw(void);
	private: void Draw(ObjectBinaryTree::AddressDerived* _address);
	public: void SetXAxisName(StringANSI _name);
    public: StringANSI GetXAxisName(void)const;
    public: void SetYAxisName(StringANSI _name);
	public: StringANSI GetYAxisName(void)const;
	public: void SetFont(Font2D* _font);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
};



#endif
