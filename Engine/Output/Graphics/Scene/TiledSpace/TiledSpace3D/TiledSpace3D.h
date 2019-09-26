#ifndef TILEDSPACE3D_H
#define TILEDSPACE3D_H
#include <Engine/Core/Container/List/MagicList/MagicList.h>
#include <Engine/Output/Graphics/Scene/GraphicsScene/GraphicsScene.h>
#include <Engine/Core/Scene/ObjectScene/ObjectScene.h>
#include <Engine/Output/Graphics/Scene/GraphicsContainer/GraphicsContainer.h>
class TiledSpace3D: public GraphicsContainer
{
	public: enum: int64
	{
		TILED_SPACE_3D_CLASS = 68719476736
	};
	public: enum
	{
		CREATE_TILED_SPACE_MESSAGE = 277,
		GET_TILE_CENTER_MESSAGE = 278,
		CREATE_BRUSH_MESSAGE = 279,
		TILED_SPACE_FILL_MESSAGE = 285,
		CREATE_SOLID_TILE_MESSAGE = 286,
		CREATE_EMPTY_TILE_MESSAGE = 287,
		CHANGE_TILED_SPACE_SIZE_MESSAGE = 289,
		SET_PUBLIC_TILE_MESSAGE = 293,
		REMOVE_BRUSH_MESSAGE = 295,
		MOVE_TILE_FROM_TO_MESSAGE = 303,
		CHECK_TILE_EXIST_MESSAGE = 305,
		CHECK_TILE_SOLID_MESSAGE = 306,
		CHECK_TILE_PASS_MESSAGE = 307,
		CHANGE_TILE_PASS_MESSAGE = 308,
		GET_BRUSH_OF_TILE_MESSAGE = 339,
		COPY_TILE_MESSAGE = 340,
		SWAP_TWO_TILES_MESSAGE = 341
	};
	public: class Tile
	{
		friend class TiledSpace3D;
		private: MagicPointer<TexturingSurface> object;
	    private: Vector3 tilePosition;
		private: Vector3 position;
		private: TiledSpace3D* tiledSpace;
		private: bool isPassed;
        private: int32 brushId;
		public: Tile(void);
		public: ~Tile(void);
		public: bool IsPassed(void)const;
		public: bool IsSolid(void)const;
		public: bool IsEmpty(void)const;
		public: void SetPassEnable(bool _enable);
		private: bool SetObject(TexturingSurface* _object);
		public: TexturingSurface* GetObject(void);
		public: int32 GetBrushId(void)const;
		private: void SetBrushId(int32 _brushId);
		public: void Draw(void);
		public: Vector3 GetPosition(void)const;
		private: void SetPosition(Vector3 _position);
		public: Vector3 GetSize(void)const;
		public: Vector3 GetTilePosition(void)const;
		private: void SetTilePosition(Vector3 _position);
		public: TiledSpace3D* GetTiledSpace(void);
		private: void SetTiledSpace(TiledSpace3D* _tiledSpace);
	};
	private: class Brush: public MagicNode
	{
		friend class TiledSpace3D;
		private: MagicPointer<TexturingSurface> sample;
		private: int32 brushId;
		private: bool isPassed;
		public: public: Brush(void);
		public: Brush(TexturingSurface* _sample, bool _passEnable, int32 _brushId);
		public: int32 GetBrushId(void)const;
		public: bool IsPassed(void)const;
		public: TexturingSurface* GetSample(void)const;
		public: bool IsValid(void)const;
		public: bool SaveToFile(File& _file);
		public: bool LoadFromFile(File& _file); 
	};
	friend class RenderList;
    private: Tile* tiledSpace;
	private: Vector3 spaceSize;
	private: int32 amountOfTiles;
	private: Vector3 tileSize;
	private: MagicBinaryTree<Brush> brushes;
	public: TiledSpace3D(void);
	public: virtual ~TiledSpace3D(void);
	protected: virtual void ResetTransformationValid(void);
	public: virtual void Destroy(void);
	public: bool CreateBrush(TexturingSurface* _sample, bool _passEnable, int32 _brushId);
	private: bool AddBrush(Brush* _brush);
	public: TexturingSurface* GetBrush(int32 _brushId);
	public: void RemoveBrush(int32 _brushId);
	public: int32 GetAmountOfBrushes(void)const;
	public: void Create(Vector3 _spaceSize, Vector3 _tileSize);
	public: bool IsExist(void)const;
	public: Vector3 GetSpaceSize(void)const;
	public: int32 GetAmountOfTiles(void)const;
	public: int32 TileIndexOf(Vector3 _position);
	public: Vector3 TilePositionOf(int32 _index);
	public: Vector3 TileCenterOf(Vector3 _position);
	public: Vector3 GetTileSize(void)const;
	public: bool IsTileExist(Vector3 _position)const;
	public: bool IsTileExist(int32 _index)const;
	public: bool IsTileSolid(Vector3 _position);
	public: bool IsTileSolid(int32 _index);
	public: Tile* GetTile(Vector3 _position);
	public: Tile* GetTile(int32 _index);
	public: virtual void Draw(void);
	public: bool MoveTileFromTo(Vector3 _from, Vector3 _to);
	public: bool CopyTile(Vector3 _from, Vector3 _to);
	public: bool SwapTile(Vector3 _from, Vector3 _to);
	public: bool Fill(int32 _plane, int32 _index, int32 _brushId);
	public: bool AddTile(Vector3 _position, int32 _brushId);
	public: bool RemoveTile(Vector3 _position);
	private: bool IsParentScene(GraphicsScene* _scene);
	private: bool IsParentScene(TiledSpace3D* _tiledSpace);
	public: virtual bool SaveToFile(StringANSI _path = "");
	public: virtual bool SaveToFile(File& _file);
	public: virtual bool SaveAsToFile(StringANSI _name);
	public: virtual bool SaveAsToFile(File& _file, StringANSI _name);
	protected: void SaveDerivedToFile(File& _file);
	protected: void SaveBrushToFile(MagicBinaryTree<Brush>::AddressDerived* _address, File& _file);
	protected: void SaveTile(Tile* _tile, File& _file);
    protected: virtual bool LoadFromFile(StringANSI _path, bool _auto = true);
	protected: virtual bool LoadFromFile(File& _file);
	protected: void LoadDerivedFromFile(File& _file);
	protected: void LoadBrushFromFile(File& _file);
	protected: void LoadTile(File& _file, int32 _index);
};


#endif 