#include "IMessageSender.h"

s21::IMessageSender::IMessageSender() {
	redi::ipstream is(hostname_command);
	is >> hostname;
}

