#pragma once
#include "Person.h"
#include <string>

namespace atmverse {

class Admin : public Person {
private:
    std::string adminId_;
    std::string passwordHash_;
    std::string passwordSalt_;
    std::string accessLevel_; // "SuperAdmin", "Manager"
    std::string lastLogin_;

public:
    Admin() = default;
    Admin(const std::string& id, const std::string& name,
          const std::string& phone, const std::string& email,
          const std::string& adminId, const std::string& passHash,
          const std::string& passSalt, const std::string& level = "SuperAdmin");

    // Getters
    const std::string& getAdminId() const { return adminId_; }
    const std::string& getPasswordHash() const { return passwordHash_; }
    const std::string& getPasswordSalt() const { return passwordSalt_; }
    const std::string& getAccessLevel() const { return accessLevel_; }
    const std::string& getLastLogin() const { return lastLogin_; }

    // Setters
    void setLastLogin(const std::string& dt) { lastLogin_ = dt; }
    void setPasswordHash(const std::string& h, const std::string& s) {
        passwordHash_ = h;
        passwordSalt_ = s;
    }

    // Operations
    bool validatePassword(const std::string& password) const;

    // Polymorphism
    std::string getRole() const override { return "Admin"; }
    json toJson() const override;
    void fromJson(const json& j) override;
};

} // namespace atmverse
