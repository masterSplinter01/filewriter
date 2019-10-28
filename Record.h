#pragma once
#include <string>

class Record{
public:
    Record(const int id, const std::string& login, const std::string& name, const std::string& email);
    explicit Record(std::string& line);
    ~Record() = default;
    int get_id() const;
    void set_id(int id);
    std::string get_login() const;
    std::string get_name() const;
    std::string get_email() const;
    std::string to_string() const;
private:
    int _id;
    std::string _login;
    std::string _name;
    std::string _email;
};


bool emails_are_duplicates(const Record& r1, const Record& r2);
bool logins_are_duplicates (const Record& r1, const Record& r2);
