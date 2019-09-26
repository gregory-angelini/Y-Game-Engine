#ifndef NAMECREATOR_H
#define NAMECREATOR_H
#include <QDialog>
#include <Editor/ErrorDialog/ErrorDialog.h>
class NameCreator: public QDialog
{
	Q_OBJECT
    private: QLabel* nameLabel;
    private: QLineEdit* nameLineEdit;
	private: int nameMaxLength;

    private: QPushButton *cancelButton;
    private: QPushButton *okButton;

	private: QList<QString> nameList;
	private: bool assetLibraryCheckEnable;
    public: NameCreator(QWidget* _parent = 0);
	public: void SetName(QString _name);
    public: QString GetName(void);
	public: void SetNameList(QList<QString>& _nameList);
	public: void SetAssetLibraryCheckEnable(bool _state);
	public: bool IsAssetLibraryCheckEnabled(void)const;
    private slots: void IsOk(void);
	public: signals: void PickName(QString _name);
	public: QObject* ConvertToQObject(void);
};


#endif 