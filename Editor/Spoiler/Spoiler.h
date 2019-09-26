#ifndef SPOILER_H
#define SPOILER_H
#include <QFrame>
#include <QGridLayout>
#include <QParallelAnimationGroup>
#include <QScrollArea>
#include <QToolButton>
#include <QWidget>
class Spoiler: public QWidget 
{
	Q_OBJECT
	private: QGridLayout* mainLayout;
    private: QToolButton* toggleButton;
    private: QParallelAnimationGroup* toggleAnimation;
    private: QScrollArea* contentArea;
    private: int animationDuration;
	public: explicit Spoiler(QString _text = "", int _animationDuration = 300, QWidget* _parent = 0);
	public: void SetContentLayout(QLayout* _layout);
	public: QToolButton* GetToolButton(void)const;
	public: void SetToggleButtonFixedSize(int _width, int _height);
	private slots: void ClickOnButton(bool _checked);
	public: void SetTitle(QString _text);
	public: QString GetTitle(void)const;
	public: bool IsCollapsed(void)const;
	public: void SetCollapseEnable(bool _enable);
};



#endif 
