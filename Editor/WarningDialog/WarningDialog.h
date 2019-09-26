#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H
#include <QDialog>
#include <QtGui>
#include <QMainWindow>
class WarningDialog: public QDialog 
{
    Q_OBJECT
    private: QGridLayout* layout;
	private: QPushButton* buttonOk;
	private: QPushButton* buttonCancel;
	private: QLabel* message;
	public: WarningDialog(QString _title, QString _message, int _width, int _height, QString _buttonOkName = "Ok", QString _buttonCancelName = "Cancel", QWidget* _parent = 0);
	public: void SetFocusOk(void);
    public: void SetFocusCancel(void);
};


#endif 
