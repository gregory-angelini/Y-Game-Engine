#ifndef VARIABLECREATOR_H
#define VARIABLECREATOR_H
#include <QTreeWidget>
#include <QListView>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QScrollArea>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <Editor/TriggerCreator/VariableCreator/VariableTableWidget/VariableTableWidget.h>
class VariableCreator: public QDialog 
{
	Q_OBJECT
	private: VariableTableWidget *variableTable;
	private: AbstractVariable* variable;
	private: bool fastCreatingEnable;

	private: QPushButton* importButton;
	private: QPushButton* saveAsButton;

    private: QToolButton* helpButton;
    private: QPushButton *okButton;
    private: QPushButton *cancelButton;
	public: VariableCreator(QWidget* _parent = 0);
	public: virtual ~VariableCreator(void);
	public: void SetType(int _type);
	public: void SetFastCreating(bool _state);
    private slots: void IsOk(void);
	public slots: void Import(void);
	public slots: void SaveAs(void);
	private: bool CreateVariable(void);
	public: void SetObjectName(QString _name);
	public: void SetVariable(QString _name);
	public: AbstractVariable* GetVariable(void)const;
	private slots: void ShowDoc(void);
	public: QString GetObjectName(void)const;
	public: void SetNameDisable(bool _disable);
	signals: void VariableIsEdited(QString _name);
	public: QString Vector3ToString(Vector3 _vector);
	public: Vector3 StringToVector3(QString _vector);
	public: QString Vector2ToString(Vector2 _vector);
	public: Vector2 StringToVector2(QString _vector);
	public: QString QuaternionToString(Quaternion _quaternion);
	public: Quaternion StringToQuaternion(QString _quaternion);
	public: QString GetVariableStringType(int32 _type);
};


#endif 

