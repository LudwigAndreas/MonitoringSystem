#pragma once

#include <string>
#include <memory>

#include "../../modules/include/pstream.h"
#include "FailedMetric.h"

namespace s21 {

#if __linux__
#define hostname_command "cat /etc/hostname"
#elif __APPLE__
#define hostname_command "hostname"
#endif

class IMessageSender {
protected:
	std::string hostname;

public:
	IMessageSender();

	virtual void SendMessage(FailedMetric fm) = 0;
};

using IMessageSenderPtr = std::shared_ptr<IMessageSender>;
}