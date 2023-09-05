#include "CriticalValueHandler.h"

CriticalValueHandler::CriticalValueHandler() : 
	email_sender(), telegram_sender() {}

void CriticalValueHandler::sendMessage(FailedMetric fm) {
	email_sender.sendMessage(fm);
	telegram_sender.sendMessage(fm);
}