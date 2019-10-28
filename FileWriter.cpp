#include <iostream>
#include <cmath>
#include <set>
#include "FileWriter.h"

FileWriter::FileWriter(const std::string &filename): _filename(filename), _max_id(0) {
    std::ifstream file;
    file.open(_filename);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            try {
                Record current_record(line);
                if (_max_id < current_record.get_id()) {
                    _max_id = current_record.get_id();
                }
            }
            catch (std::invalid_argument& e){
                continue;
            }
        }
    }
    file.close();
}

bool FileWriter::add_record(const std::string &login, const std::string &name, const std::string &email) {
    try {
        Record added_record(_max_id + 1, login, name, email);
        return add_record(added_record);
    }
    catch (std::invalid_argument& e) {
        std::cerr << "Invalid record" << std::endl;
        return false;
    }
}

bool FileWriter::add_record(Record &rec) {
    try {
        std::ifstream file(_filename, std::fstream::in );
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                try {
                    Record current_record(line);
                    if (emails_are_duplicates(rec, current_record)) {
                        std::cout << "Email duplicate: " << rec.get_email() << '\n';
                        return false;
                    }
                    if (logins_are_duplicates(rec, current_record)) {
                        std::cout << "Login duplicate: " << rec.get_login() << '\n';
                        return false;
                    }
                }
                catch (std::invalid_argument& e){
                    continue;
                }
            }
        }
        file.close();
        rec.set_id(++_max_id);
        std::ofstream ofile(_filename, std::ios_base::app);
        ofile << rec.to_string();
        ofile.close();
        return true;
    }
    catch (std::fstream::failure& e) {
        std::cerr << "Opening file error: " << _filename << std::endl;
        exit(1);
    }
}

void FileWriter::print() {
    std::ifstream file;
    try{
        file.open(_filename);
    }
    catch (std::fstream::failure& e) {
        std::cerr << "Opening file error: " << _filename << std::endl;
        exit(1);
    }
    std::cout << _filename << std::endl;
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << '\n';
    }
    file.close();

}

inline static void copy_from_to(const std::string &from, const std::string &to, std::ios::openmode mode) {
    try {
        std::ifstream src(from, std::ios::binary);
        std::ofstream dest(to, std::ios::binary | mode);
        std::copy(std::istreambuf_iterator<char>(src),
                  std::istreambuf_iterator<char>(),
                  std::ostreambuf_iterator<char>(dest)
        );
    }
    catch (std::fstream::failure& e) {
        std::cerr << "Copying file error" << std::endl;
        exit(1);
    }
}

void FileWriter::copy_file(const std::string &copy_name, std::ios::openmode mode) {
    copy_from_to(_filename, copy_name, mode);
}

void FileWriter::merge(const std::string &other_file, const std::string& result_filename) {
    try{
        auto block_size = 65536;

        copy_file(result_filename);
        FileWriter result_fw(result_filename);

        std::ifstream file1;
        try {
            file1.open(_filename);
        }
        catch (std::fstream::failure &e) {
            std::cerr << "Opening file error: " << _filename << std::endl;
            exit(1);
        }

        std::string temp1 = "temp1_" + other_file;
        std::string temp2 = "temp2_" + other_file;
        copy_from_to(other_file, temp1, std::ios::out);

        bool finished = false;
        while (!finished) {
            std::string line;
            std::set<std::string> logins;
            std::set<std::string> emails;
            for (int i = 0; i < block_size && !finished; ++i) {
                finished |= !std::getline(file1, line);
                if (!finished) {
                    try {
                        Record record(line);
                        logins.insert(record.get_login());
                        emails.insert(record.get_email());
                    }
                    catch (std::invalid_argument &e) {
                        continue;
                    }
                }
            }
            finished |= file1.eof();
            std::ifstream temp_file1;
            std::ofstream temp_file2;
            try{
                temp_file1.open(temp1);
                temp_file2.open(temp2);
            }
            catch (std::fstream::failure& e){
                std::cerr << "temp files error" << std::endl;
            }
            int last_id = _max_id + 1;
            std::string temp_line;
            while (std::getline(temp_file1, temp_line)) {
                try {
                    Record rec(temp_line);
                    if (finished)
                        rec.set_id(last_id++);
                    if (!logins.count(rec.get_login()) && !emails.count(rec.get_email())) {
                        temp_file2 << rec.to_string();
                    } else {
                        std::cout << "Duplicate ignored: " << rec.to_string();
                    }
                }
                catch (std::invalid_argument &e) {
                    std::cerr << e.what()<< std::endl;
                    continue;
                }
            }
            temp_file1.close();
            temp_file2.close();
            std::swap(temp1, temp2);
        }
        copy_from_to(temp1, result_filename, std::ios::app);
        remove(temp1.c_str());
        remove(temp2.c_str());
    }
    catch (std::fstream::failure& e){
        std::cerr <<"files error" << std::endl;
    }
}
