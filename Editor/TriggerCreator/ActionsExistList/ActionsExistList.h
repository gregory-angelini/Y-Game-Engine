#ifndef ACTIONSEXISTLIST_H
#define ACTIONSEXISTLIST_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class ActionsExistList: public QListWidget
{
	Q_OBJECT
    private: QMenu* contextMenu;
	private: QAction* editActionAct;
	private: QPoint cursorPoint;

	private: QPixmap actionIcon;
	public: ActionsExistList(QWidget* _parent = 0);
	private: void CreateContextMenu(void);
	private: void UpdateContextMenu(void);
	private slots: void EditAction(void);
	public slots: void AddAction(QString _name);
	signals: void ActionIsEdited(QString _name);
	protected: virtual void mousePressEvent(QMouseEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
};



#endif 

