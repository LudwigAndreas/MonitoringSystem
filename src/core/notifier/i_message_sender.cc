#include "i_message_sender.h"

s21::IMessageSender::IMessageSender() {
  redi::ipstream is(hostname_command);
  is >> hostname;
}
