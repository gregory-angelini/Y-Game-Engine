
#include "Spoiler.h"


#include <QPropertyAnimation>

#include "Spoiler.h"

Spoiler::Spoiler(QString _text, int _animationDuration, QWidget *parent) : QWidget(parent), animationDuration(_animationDuration) 
{
	toggleButton = new QToolButton(this);
	toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setArrowType(Qt::RightArrow);
    toggleButton->setText(_text);
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);
	connect(toggleButton, SIGNAL(clicked(bool)), this, SLOT(ClickOnButton(bool)));
	contentArea = new QScrollArea(this);
    contentArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");
    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    contentArea->setMaximumHeight(0);
    contentArea->setMinimumHeight(0);
	toggleAnimation = new QParallelAnimationGroup(this);
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));
	mainLayout = new QGridLayout(this);
    mainLayout->setVerticalSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(toggleButton, 0, 0, 1, 1, Qt::AlignLeft);
    mainLayout->addWidget(contentArea, 1, 0, 3, 3);
    setLayout(mainLayout);
} 



void Spoiler::ClickOnButton(bool _checked)
{
	toggleButton->setArrowType(_checked ? Qt::DownArrow : Qt::RightArrow);
    toggleAnimation->setDirection(_checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggleAnimation->start();
}




void Spoiler::SetTitle(QString _text)
{
	toggleButton->setText(_text);
}


QString Spoiler::GetTitle(void)const
{
	return toggleButton->text();
}




void Spoiler::SetToggleButtonFixedSize(int _width, int _height)
{
	toggleButton->setFixedSize(_width, _height);
}



QToolButton* Spoiler::GetToolButton(void)const
{
	return toggleButton;
}



void Spoiler::SetCollapseEnable(bool _enable)
{
	toggleButton->setChecked(!_enable);

	toggleButton->setArrowType(toggleButton->isChecked() ? Qt::DownArrow : Qt::RightArrow);
    toggleAnimation->setDirection(toggleButton->isChecked() ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggleAnimation->start();
}



bool Spoiler::IsCollapsed(void)const
{
	return !toggleButton->isChecked(); 
}


void Spoiler::SetContentLayout(QLayout* _layout) 
{
    delete contentArea->layout();
    contentArea->setLayout(_layout);
    
	if(_layout)
	{
		int collapsedHeight = sizeHint().height() - contentArea->maximumHeight();
		int contentHeight = _layout->sizeHint().height();
	    
		for(int i = 0; i < toggleAnimation->animationCount() - 1; ++i) 
		{
			QPropertyAnimation* spoilerAnimation = static_cast<QPropertyAnimation*>(toggleAnimation->animationAt(i));
			spoilerAnimation->setDuration(animationDuration);
			spoilerAnimation->setStartValue(collapsedHeight);
			spoilerAnimation->setEndValue(collapsedHeight + contentHeight);
		}

		QPropertyAnimation* contentAnimation = static_cast<QPropertyAnimation*>(toggleAnimation->animationAt(toggleAnimation->animationCount() - 1));
		contentAnimation->setDuration(animationDuration);
		contentAnimation->setStartValue(0);
		contentAnimation->setEndValue(contentHeight);
	}
}