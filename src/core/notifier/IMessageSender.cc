#include "IMessageSender.h"

IMessageSender::IMessageSender() {
	redi::ipstream is(hostname_command);
	is >> hostname;
}

