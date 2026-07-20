#include "atmverse/models/Person.h"
#include "atmverse/utils/Utility.h"

namespace atmverse {

Person::Person(const std::string& id, const std::string& name,
               const std::string& phone, const std::string& email)
    : id_(id), name_(name), phone_(phone), email_(email),
      createdAt_(util::getCurrentDateTime()) {}

json Person::toJson() const {
    return {
        {"id", id_},
        {"name", name_},
        {"phone", phone_},
        {"email", email_},
        {"createdAt", createdAt_}
    };
}

void Person::fromJson(const json& j) {
    id_ = j.value("id", "");
    name_ = j.value("name", "");
    phone_ = j.value("phone", "");
    email_ = j.value("email", "");
    createdAt_ = j.value("createdAt", "");
}

} // namespace atmverse
