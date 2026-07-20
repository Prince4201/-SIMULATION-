#include "atmverse/models/Admin.h"
#include "atmverse/utils/Utility.h"

namespace atmverse {

Admin::Admin(const std::string& id, const std::string& name,
             const std::string& phone, const std::string& email,
             const std::string& adminId, const std::string& passHash,
             const std::string& passSalt, const std::string& level)
    : Person(id, name, phone, email), adminId_(adminId),
      passwordHash_(passHash), passwordSalt_(passSalt),
      accessLevel_(level), lastLogin_("") {}

bool Admin::validatePassword(const std::string& password) const {
    return util::verifyPin(password, passwordHash_, passwordSalt_);
}

json Admin::toJson() const {
    json j = Person::toJson();
    j["adminId"] = adminId_;
    j["passwordHash"] = passwordHash_;
    j["passwordSalt"] = passwordSalt_;
    j["accessLevel"] = accessLevel_;
    j["lastLogin"] = lastLogin_;
    return j;
}

void Admin::fromJson(const json& j) {
    Person::fromJson(j);
    adminId_ = j.value("adminId", "");
    passwordHash_ = j.value("passwordHash", "");
    passwordSalt_ = j.value("passwordSalt", "");
    accessLevel_ = j.value("accessLevel", "SuperAdmin");
    lastLogin_ = j.value("lastLogin", "");
}

} // namespace atmverse
