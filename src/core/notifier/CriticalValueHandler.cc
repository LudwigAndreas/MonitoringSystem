#include "CriticalValueHandler.h"

CriticalValueHandler::CriticalValueHandler() : 
	email_sender(), telegram_sender() {}

void CriticalValueHandler::SendMessage(FailedMetric fm) {
	email_sender.SendMessage(fm);
	telegram_sender.SendMessage(fm);
}