#include "EmailSender.h"

#include <iostream>

EmailSender::EmailSender() {}

std::string EmailSender::prepareMessage(FailedMetric fm) {
  std::ostringstream ss;
  ss << "To: ";
  for (auto receiver: receivers) {
    ss << receiver << ", ";
  }
  ss << "\r\n"
     << "Subject: Failed Metric \""
     << fm.metric_name << "\"\r\n\r\n"
     << "Metric \""
     << fm.metric_name
     << "\" failed at "
     << fm.date
     << " with value of "
     << fm.value
     << " and critical value threshold \""
     << fm.critical_value
     << "\".\r\n";
  return ss.str();
}

void EmailSender::sendMessage(FailedMetric fm) {
  CURL *curl;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = NULL;
  struct upload_status upload_ctx = {0};
  message = prepareMessage(fm);

  curl = curl_easy_init();
  if (curl) {
    /* Set username and password */
    curl_easy_setopt(curl, CURLOPT_USERNAME, __email);
    curl_easy_setopt(curl, CURLOPT_PASSWORD, __password);

    curl_easy_setopt(curl, CURLOPT_URL, __server);

    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long) CURLUSESSL_ALL);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, __email);

    for (auto receiver: receivers)
      recipients = curl_slist_append(recipients, receiver.c_str());
    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    curl_easy_setopt(curl, CURLOPT_READFUNCTION, &EmailSender::payloadSource);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK) {
      // std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res);
    }

    /* Free the list of recipients */
    curl_slist_free_all(recipients);

    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
}

size_t EmailSender::payloadSource(char *ptr,
                                  size_t size,
                                  size_t nmemb,
                                  void *userp) {
  struct upload_status *upload_ctx = (struct upload_status *) userp;
  const char *data;
  size_t room = size * nmemb;

  if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
    return 0;
  }

  data =
      &message.c_str()[upload_ctx->bytes_read]; // maybe needs to be converted to c string

  if (data) {
    size_t len = strlen(data);
    if (room < len)
      len = room;
    memcpy(ptr, data, len);
    upload_ctx->bytes_read += len;

    return len;
  }

  return 0;
}

std::set<std::string> EmailSender::getReceivers() {
  return receivers;
}

void EmailSender::addReceiver(std::string username) {
  receivers.insert(username);
}

void EmailSender::removeReceiver(std::string username) {
  receivers.erase(username);
}