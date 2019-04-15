#include "Dialog.h"

Dialog::Dialog()
{
	initWithFile("images/dialWindow.png");
	autorelease();

	text = Label::createWithTTF("", "himalaya.ttf", 100);
	text->setColor(Color3B::BLACK);
	text->setAnchorPoint(Vec2(0, 1));

	int textLeftOffset = 130;
	int textTopOffset = 30;
	text->setPosition(Vec2(textLeftOffset, getContentSize().height - textTopOffset));
	text->setWidth(getContentSize().width - (textLeftOffset + 40));
	text->setHeight(getContentSize().height - 3 * textTopOffset);

	addChild(text);
}

void Dialog::setText(const std::string& text)
{
	this->text->setString(text);
}
