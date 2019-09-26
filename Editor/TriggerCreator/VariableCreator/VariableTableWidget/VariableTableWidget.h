#ifndef VARIABLETABLEWIDGET_H
#define VARIABLETABLEWIDGET_H
#include <Editor/ErrorDialog/ErrorDialog.h>
#include <Editor/UniversalDelegate/UniversalDelegate.h>
#include <Engine/Core/Numerical/Numerical.h>
#include <Engine/Core/Trigger/Trigger.h>
class VariableTableWidget: public QTableWidget
{
	Q_OBJECT
	public: enum
	{
		MAX_ROWS = 5,
		MAX_COLUMNS = 1
	};
	public: enum
	{
		NAME_PROPERTY = 0,
		TYPE_PROPERTY,
		VALUE_PROPERTY,
		LOWER_BOUND_PROPERTY,
		UPPER_BOUND_PROPERTY
	};
	public: enum
	{
		VALUE = 0,
	};
	private: UniversalDelegate* delegate;
	private: QList<QString> createVariableComboList;
	private: bool propertyValid[MAX_ROWS];
	private: bool propertyEnabled[MAX_ROWS];
	public: VariableTableWidget(QWidget* _parent = 0);
	public: virtual ~VariableTableWidget(void);
    public: QString IsOk(void);
	public: void SetCellDelegate(int _row, int _editor);
	public: void SetDefaultState(void);
	public: bool IsValid(void)const;
	private slots: void UpdateRow(QTableWidgetItem* _it);
	private: void UpdatePropertyStatus(void);
	public: void SetPropertyValid(int _row, bool _valid);
	public: void SetPropertyEnable(int _row, bool _enable);
	public: void SetCellText(int _row, int _column, QString _value);
	public: QString GetCellText(int _row, int _column);
	private: void UpdateItem(QTableWidgetItem* _it, int _row);
	private: int GetEditorTypeForCell(QTableWidgetItem* _it);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	protected: virtual void mouseDoubleClickEvent(QMouseEvent* _event);
};


#endif 

