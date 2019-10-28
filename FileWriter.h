#pragma once
#include <string>
#include <cstdio>
#include <fstream>
#include "Record.h"

enum class record_unique_fields{
    LOGIN,
    EMAIL
};

typedef  std::string (Record::*get_field_fn_pointer) () const;

class FileWriter {
public:
    explicit FileWriter(const std::string& filename);
    ~FileWriter() = default;
    bool add_record(const std::string &login, const std::string &name, const std::string &email);
    bool add_record(Record& rec);
    void print();
    void copy_file(const std::string& copy_name, std::ios::openmode mode = std::ios::out);
    void merge(const std::string& other_file, const std::string& result_filename);

    template <record_unique_fields E>
    bool delete_record(const std::string& str){
        std::ifstream file;
        std::ofstream new_file;
        std::string new_filename =  "new_"+_filename;
        try {
            file.open(_filename);
            new_file.open(new_filename, std::fstream::out | std::fstream::app);
        }
        catch (std::fstream::failure& e){
            std::cout << "files not open" << std::endl;
        }
        std::string line;
        bool deleted = false;
        get_field_fn_pointer get_field;
        switch(E){
            case record_unique_fields::LOGIN:{
                get_field = &Record::get_login;
                break;
            }
            case record_unique_fields::EMAIL:{
                get_field = &Record::get_email;
                break;
            }
            default: {
                return false;
            }
        }
        while (std::getline(file, line)) {
            if(!line.empty()) {
                try{
                    Record current_record(line);
                    if ((current_record.*get_field)() != str){
                        new_file << current_record.to_string();
                    }
                    else{
                        deleted = true;
                    }
                }
                catch (std::invalid_argument& e){
                    continue;
                }
            }
        }
        file.close();
        new_file.close();
        remove(_filename.c_str());
        rename(new_filename.c_str(), _filename.c_str()) ;
        return deleted;
    }
private:
    void append_records_from_file(const std::string& src_name);
    std::string _filename;
    int _max_id;
};

inline static void copy_from_to(const std::string &from, const std::string &to, std::ios::openmode mode);