#include <vector>
#include <stdexcept>
#include <regex>
#include "Record.h"

Record::Record(const int id, const std::string &login, const std::string &name, const std::string &email):  _id(id),
                                                                                                            _login(login), _name(name), _email(email)
{ /* EMPTY */}

Record::Record(const std::string &line) {
    std::vector<std::string> tokens;
    std::regex delim(";");
    std::sregex_token_iterator iter(line.begin(), line.end(), delim, -1);
    std::sregex_token_iterator end;
    while (iter != end){
        if (*iter != "") {
            tokens.push_back(*iter);
            ++iter;
        } else {
        throw std::invalid_argument("Not enough data: " + line);
        }
    }
    if (tokens.size() != 4){
        throw std::invalid_argument("Invalid record: " + line);
    }
    _id = std::stoi(tokens[0]);
    _login = tokens[1];
    _name = tokens[2];
    _email = tokens[3];
}

int Record::get_id() const {
    return _id;
}

std::string Record::get_login() const {
    return _login;
}

std::string Record::get_name() const {
    return _name;
}

std::string Record::get_email() const {
    return _email;
}

std::string Record::to_string() const {
    return std::string(std::to_string(_id) + ';' + _login + ';' + _name + ';' + _email + '\n');
}

void Record::set_id(int i) {
    _id = i;
}

bool emails_are_duplicates(const Record& r1, const Record& r2){
    return r1.get_email() == r2.get_email();
}

bool logins_are_duplicates (const Record& r1, const Record& r2){
    return r1.get_login() == r2.get_login();
}
