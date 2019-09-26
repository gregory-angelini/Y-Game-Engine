#ifndef EVENTSEXISTLIST_H
#define EVENTSEXISTLIST_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Input/InputEvent/InputEvent.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
class EventsExistList: public QListWidget
{
	Q_OBJECT
    private: QMenu* contextMenu;
	private: QAction* editEventAct;
	private: QPoint cursorPoint;
	
	private: QPixmap eventIcon;
	private: QPixmap triggerIcon;
	private: QPixmap variableIcon;
	public: EventsExistList(QWidget* _parent = 0);
	private: void CreateContextMenu(void);
	private: void UpdateContextMenu(void);
	private slots: void EditEvent(void);
	public slots: void AddEvent(QString _name);
	public slots: void AddTrigger(QString _name);
	public slots: void AddVariable(QString _name);
	signals: void EventIsEdited(QString _name);
	signals: void TriggerIsEdited(QString _name);
	signals: void VariableIsEdited(QString _name);
	protected: virtual void mousePressEvent(QMouseEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
};



#endif 

