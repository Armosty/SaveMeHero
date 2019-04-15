#ifndef __DIALOG_H__
#define __DIALOG_H_

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class Dialog : public Sprite
{

private:

	Label *text;

public:

	Dialog();
	void setText(const std::string& text);

};

#endif