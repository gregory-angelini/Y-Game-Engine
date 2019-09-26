#ifndef IGNOREEVENTLIST_H
#define IGNOREEVENTLIST_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class IgnoreEventList: public QListWidget
{
	Q_OBJECT
    private: QMenu* contextMenu;
	private: QAction* removeEventAct;
	private: QPoint cursorPoint;

	private: QPixmap actionIcon;
	private: QPixmap eventIcon;
	private: QPixmap triggerIcon;
	public: IgnoreEventList(QWidget* _parent = 0);
	public: QList<QPair<int, QString>> GetIgnoreList(void)const;
	private: void CreateContextMenu(void);
	private: void UpdateContextMenu(void);
	public slots: void RemoveSelectedItems(void);
	public: void AddEvent(int _category, QString _name);
	public: QPixmap GetIcon(int _assetType);
	protected: virtual void mousePressEvent(QMouseEvent* _event);
};



#endif 

