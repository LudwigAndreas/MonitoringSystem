#include "Email.h"

CURLcode Email::send(const std::string &url,
                     const std::string &userName,
                     const std::string &password)
{
    CURLcode ret = CURLE_OK;

    struct curl_slist *recipients = NULL;

    CURL *curl = curl_easy_init();

    StringData textData { setPayloadText_() };

    if (curl) {
        std::ostringstream cc;
        cc << cc_;

        curl_easy_setopt(curl, CURLOPT_USERNAME,     userName.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD,     password.c_str());
        curl_easy_setopt(curl, CURLOPT_URL,          url     .c_str());

        curl_easy_setopt(curl, CURLOPT_USE_SSL,      (long)CURLUSESSL_ALL);

        curl_easy_setopt(curl, CURLOPT_MAIL_FROM,    (const char *)from_);

        recipients = curl_slist_append(recipients,   (const char *)to_);
        recipients = curl_slist_append(recipients,   cc.str().c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT,    recipients);
        
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadSource_);
        curl_easy_setopt(curl, CURLOPT_READDATA,     &textData);
        curl_easy_setopt(curl, CURLOPT_UPLOAD,       1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE,      0L);

        ret = curl_easy_perform(curl);

        if (ret != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(ret)
                      << std::endl;
        }

        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    return ret;
}

std::string Email::dateTimeNow_()
{
    const int RFC5322_TIME_LEN = 32;

    std::string ret;
    ret.resize(RFC5322_TIME_LEN);

    time_t tt;

#ifdef _MSC_VER
    time(&tt);
    tm *t = localtime(&tt);
#else
    tm tv, *t = &tv;
    tt = time(&tt);
    localtime_r(&tt, t);
#endif

    strftime(&ret[0], RFC5322_TIME_LEN, "%a, %d %b %Y %H:%M:%S %z", t);

    return ret;
}

std::string Email::generateMessageId_() const
{
    const size_t MESSAGE_ID_LEN = 37;

    tm t;
    time_t tt;
    time(&tt);

#ifdef _MSC_VER
    gmtime_s(&t, &tt);
#else
    gmtime_r(&tt, &t);
#endif

    std::string ret;
    ret.resize(MESSAGE_ID_LEN);
    size_t dateLen = std::strftime(&ret[0], MESSAGE_ID_LEN, "%Y%m%d%H%M%S", &t);

    static const std::string alphaNum {
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz" };

    std::mt19937 gen;
    std::uniform_int_distribution<> distr(0, alphaNum.length() - 1);
    std::generate_n(ret.begin() + dateLen,
                    MESSAGE_ID_LEN - dateLen,
                    [&]() { return alphaNum[distr(gen)]; });

    return ret;
}

size_t Email::payloadSource_(void *ptr, size_t size, size_t nmemb, void *userp)
{
    StringData  *text = reinterpret_cast<StringData *>(userp);
    size_t      len   = std::min(size * nmemb, text->bytes_left);

    // if (size * nmemb < 1) {
    //     return 0;
    // }
    // if (text->bytes_left > 0) {

    if (len > 0) {
        text->bytes_left -= len;
        text->msg.copy(reinterpret_cast<char *>(ptr), len);
        return len;
    }
    return 0;

    // StringData *text = reinterpret_cast<StringData *>(userp);

    // if ((size == 0) || (nmemb == 0) || ((size*nmemb) < 1) || (text->bytes_left == 0)) {
    //     return 0;
    // }

    // if ((nmemb * size) >= text->msg.size()) {
    //     text->bytes_left = 0;
    //     return text->msg.copy(reinterpret_cast<char *>(ptr), text->msg.size());
    // }

    // return 0;
}

std::string Email::setPayloadText_()
{

    std::ostringstream oss;
    oss << "Date: " << dateTimeNow_() << "\r\n"
        << "To: "   << to_   << "\r\n"
        << "From: " << from_ << "\r\n";
    if (!cc_.empty())
        oss << "Cc: "   << cc_   << "\r\n";

    oss << "Message-ID: <"  << generateMessageId_() << "@" << from_.domain()    << ">\r\n"
        << "Subject: "      << subject_                                         << "\r\n"
        << "\r\n"           << body_                                            << "\r\n\r\n";

    return oss.str();
}

std::ostream &operator<<(std::ostream &out, const EmailAddresses &emailAddresses)
{
    if (!emailAddresses.empty()) {
        out << *emailAddresses.begin();
		for (auto it = std::next(emailAddresses.cbegin()); it != emailAddresses.end(); it = std::next(it)) {
            out << ", " << *it;
		}
    }

    return out;
}