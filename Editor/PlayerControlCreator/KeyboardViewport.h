#ifndef KEYBOARDVIEWPORT_H
#define KEYBOARDVIEWPORT_H
#include <QtGui/QWidget>
#include <QDialog>
#include <Editor/AnimatedLabel/AnimatedLabel.h>
#include <Editor/PlayerControlCreator/KeyboardViewer/KeyboardViewer.h>
#include <Editor/PlayerControlCreator/PlayerControlEventListWidget/PlayerControlEventListWidget.h>
#include <Editor/Viewport/Viewport.h>
class KeyboardViewport: public QDialog 
{
	Q_OBJECT
	private: KeyboardViewer* engine;
	private: Viewport* viewport;
	private: QTimer* intervalTimer;
    public: KeyboardViewport(QWidget* _parent = 0);
    public: virtual ~KeyboardViewport(void);
	public: int32 GetKeyId(void)const;
	public: void SetKeyId(int32 _key);
	protected: virtual void keyPressEvent(QKeyEvent* _event);
	private: void CreateEngine(void);
	private: void CreateViewport(void);
	private slots: void CallFrame(void);
	public: void EnginePause(void);
	public: void EnginePlay(void);
	private: virtual void resizeEvent(QResizeEvent* _event);
    protected: virtual void closeEvent(QCloseEvent *_event);

};

#endif 

