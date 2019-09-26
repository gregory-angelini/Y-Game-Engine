#include "TriggerListWidget.h"



TriggerListWidgetItem::TriggerListWidgetItem(QListWidget* _parent): QListWidgetItem(_parent), fps(0.0f), currentFrame(0), frameWidth(64), frameHeight(64), pause(true)
{	
	UpdateAnim();
}

	
void TriggerListWidgetItem::Pause(void)
{
	disconnect(&timer, SIGNAL(timeout()), this, SLOT(UpdateAnim()));
}



void TriggerListWidgetItem::Play(void)
{
	connect(&timer, SIGNAL(timeout()), this, SLOT(UpdateAnim()));
}




void TriggerListWidgetItem::SetFrames(QList<QPixmap> _frames)
{
	frames = _frames;

	if(!frames.isEmpty())
	{
		SetCurrentFrame(0);
	}
}



void TriggerListWidgetItem::SetFPS(float _fps)
{
	fps = _fps;

	if(fps > 0.0f)
	{
		timer.start(1000.0f / fps);
	}
	else
	{
		timer.stop();
	}
}



float TriggerListWidgetItem::GetFPS(void)const
{
	return fps;
}



void TriggerListWidgetItem::SetCurrentFrame(int _index)
{
	if(_index >= 0 && _index < GetAmountOfFrames())
	{
		currentFrame = _index;

		setIcon(frames.at(currentFrame));
	}
}



int TriggerListWidgetItem::GetCurrentFrame(void)const
{
	return currentFrame;
}

				

int TriggerListWidgetItem::GetAmountOfFrames(void)
{
	return frames.size();
}


void TriggerListWidgetItem::UpdateAnim(void)
{ 
	if(!frames.empty())
	{
		if(currentFrame > (frames.size() - 1))
		{
			currentFrame = 0;
		}

		setIcon(frames.at(currentFrame).scaled(frameWidth, frameHeight, Qt::KeepAspectRatio));
		
		currentFrame++;
	}
}




void TriggerListWidgetItem::SetFrameWidth(float _width)
{
	frameWidth = _width;
}
	


void TriggerListWidgetItem::SetFrameHeight(float _height)
{
	frameHeight = _height;
}



int TriggerListWidgetItem::GetFrameWidth(void)
{
	return frameWidth;
}



int TriggerListWidgetItem::GetFrameHeight(void)
{
	return frameHeight;
}


