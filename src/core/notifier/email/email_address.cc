#include "email_address.h"

namespace s21 {

EmailAddress::EmailAddress(const char *email) : email_{std::string(email)} {}

EmailAddress::EmailAddress(const std::string &email, const std::string &display_name)
    : email_(email.empty() ? "" : "<" + email + ">"),
        name_(display_name.empty() ? "" : "\"" + display_name + "\"") {}

std::string EmailAddress::Domain() const
{
    return email_.substr(email_.find('@') + 1);
}

EmailAddress::operator const char *() const
{
    return email_.c_str();
}

std::ostream &operator<<(std::ostream &out, const EmailAddress &email)
{
    if (!email.name_.empty())
        out << email.name_ << ' ';
    return out << email.email_;
}

std::ostream &operator<<(std::ostream &out, const EmailAddresses &emailAddresses)
{
    if (!emailAddresses.empty()) {
        out << *emailAddresses.cbegin();
		for (auto it = std::next(emailAddresses.cbegin()); it != emailAddresses.end(); it = std::next(it)) {
            out << ", " << *it;
		}
    }
    return out;
}

}