#pragma once
#include <string>
#include "json.hpp"

namespace atmverse {

using json = nlohmann::json;

// ─── Abstract Base Class ─────────────────────────────────────
class Person {
protected:
    std::string id_;
    std::string name_;
    std::string phone_;
    std::string email_;
    std::string createdAt_;

public:
    Person() = default;
    Person(const std::string& id, const std::string& name,
           const std::string& phone, const std::string& email);
    virtual ~Person() = default;

    // Getters
    const std::string& getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const std::string& getPhone() const { return phone_; }
    const std::string& getEmail() const { return email_; }
    const std::string& getCreatedAt() const { return createdAt_; }

    // Setters
    void setName(const std::string& name) { name_ = name; }
    void setPhone(const std::string& phone) { phone_ = phone; }
    void setEmail(const std::string& email) { email_ = email; }

    // Polymorphism
    virtual std::string getRole() const = 0;
    virtual json toJson() const;
    virtual void fromJson(const json& j);
};

} // namespace atmverse
