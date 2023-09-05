#pragma once

#include <string>
#include <memory>

#include "FailedMetric.h"

class IMessageSender {

public:
	IMessageSender() = default;

	virtual void sendMessage(FailedMetric fm) = 0;
};

using IMessageSenderPtr = std::shared_ptr<IMessageSender>;
