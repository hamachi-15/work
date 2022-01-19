#include "Telegram.h"

Telegram::Telegram(int sender, int receiver, Message message)
{
	this->sender = sender;
	this->receiver = receiver; 
	this->message_box.message = message.message;
	this->message_box.hit_position = message.hit_position;
	this->message_box.territory_tag = message.territory_tag;
}
