#include <iostream>
#include "FileWriter.h"
int main() {
    FileWriter file("ttt.txt");
    FileWriter file2("aaa.txt");
    file.add_record("ak", "Kira", "ttt@gmail.com");
    file.add_record("ds", "dan", "dan@gmail.com");
    file2.add_record("asd", "Wat", "wat@wat.com");
    file2.add_record("ak", "jhvjh", "ttt@gmail.com");
    file.print();
    file2.print();

    file.merge("aaa.txt", "lalala.txt");
    FileWriter lalala("lalala.txt");
    lalala.print();

    return 0;
}