#include <gtest/gtest.h>
#include "FileWriter.h"

class TestFW: public  ::testing::Test {
protected:
    void SetUp(){
        fw = new FileWriter("ttt.txt");

    }
    void TearDown(){ delete fw; }
    FileWriter* fw;

};

TEST_F(TestFW, CreateNewFile){
    std::string test_line = "1;ak;Kira;ttt@gmail.com";
    fw->add_record("ak", "Kira", "ttt@gmail.com");
    std::ifstream file("ttt.txt");
    std::string line;
    int number_of_lines = 0;
    while(std::getline(file, line)){
           ASSERT_STREQ(test_line.c_str(), line.c_str());
           number_of_lines++;
    }
    ASSERT_EQ(fw->get_max_id(), 1);
    ASSERT_EQ(number_of_lines, 1);
}

TEST_F(TestFW, AppendToExistedFile){
    fw->add_record("ds", "dan", "dan@gmail.com");
    std::vector<std::string> lines = {"1;ak;Kira;ttt@gmail.com", "2;ds;dan;dan@gmail.com"};
    std::ifstream file("ttt.txt");
    std::string line;
    for(const auto& i: lines){
        if( std::getline(file, line))
            ASSERT_STREQ(i.c_str(), line.c_str());
        else FAIL();
    }
    if (std::getline(file, line)){
        FAIL();
    }
}
TEST_F(TestFW,DeleteRecordByLogin){
    fw->delete_record<record_unique_fields::LOGIN>("ds");
    std::string test_line = "1;ak;Kira;ttt@gmail.com";
    std::ifstream file(fw->get_filename());
    std::string line;
    int number_of_lines = 0;
    while(std::getline(file, line)){
        ASSERT_STREQ(test_line.c_str(), line.c_str());
        number_of_lines++;
    }
    ASSERT_EQ(number_of_lines, 1);
}

TEST_F(TestFW, DeleteRecordByEmail){
    TestFW_AppendToExistedFile_Test();
    fw->delete_record<record_unique_fields::EMAIL>("dan@gmail.com");
    std::string test_line = "1;ak;Kira;ttt@gmail.com";
    std::ifstream file(fw->get_filename());
    std::string line;
    int number_of_lines = 0;
    while(std::getline(file, line)){
        ASSERT_STREQ(test_line.c_str(), line.c_str());
        number_of_lines++;
    }
    ASSERT_EQ(number_of_lines, 1);

}

TEST_F(TestFW, AddExistingRecord){
    fw->add_record("ak", "jhvjh", "aasd@gmail.com");
    fw->add_record("kl", "jhvjh", "ttt@gmail.com");
    fw->add_record("ak", "jhvjh", "ttt@gmail.com");
    std::vector<std::string> lines = {"1;ak;Kira;ttt@gmail.com"};
    std::ifstream file("ttt.txt");
    std::string line;
    for(const auto& i: lines){
        if( std::getline(file, line))
            ASSERT_STREQ(i.c_str(), line.c_str());
        else FAIL();
    }
    if (std::getline(file, line)){
        FAIL();
    }
}

TEST_F(TestFW, AddIncorrectRecord){
    ASSERT_THROW(Record rc("xv;dfsf"), std::invalid_argument);
}

TEST_F(TestFW, CopyFile){
    fw->copy_file("copy_ttt.txt");
    std::ifstream file1(fw->get_filename());
    std::ifstream file2("copy_ttt.txt");
    std::string line1, line2;
    while(std::getline(file1, line1) && std::getline(file2, line2)){
        if (line1 != line2){
            FAIL();
        }
    }
    if (std::getline(file1, line1) || std::getline(file2, line2)){
        FAIL();
    }
}

TEST_F(TestFW, Merge){
    FileWriter fw2("aaa.txt");
    fw2.add_record("asd", "Wat", "wat@wat.com");
    fw2.add_record("ak", "jhvjh", "ttt@gmail.com");
    fw->merge("aaa.txt", "result.txt");
    std::vector<std::string> lines = {"1;ak;Kira;ttt@gmail.com", "2;asd;Wat;wat@wat.com"};
    std::ifstream temp("result.txt");
    std::string line;
    for(const auto& i: lines){
        if( std::getline(temp, line))
            ASSERT_STREQ(i.c_str(), line.c_str());
        else FAIL();
    }
    if (std::getline(temp, line)){
        FAIL();
    }
}

TEST(ExtraTest, DeleteFiles){
    std::vector<std::string> files = {"aaa.txt", "ttt.txt", "copy_ttt.txt", "result.txt"};
    for (const auto& i: files){
        std::fstream f(i);
        if( f.good()){
            f.close();
            remove(i.c_str());
        }
    }
    SUCCEED();
}

TEST(TestFW_IncorrectFile, NewFile){
    std::ofstream file;
    file.open("wrong.txt");
    file << "1;ak;Kira;ttt@gmail.com\n" << "\n" << "123;asd\n" << "456;bfbd;fefs;\n" << "as123;asdsa;asd;gvf\n";
    SUCCEED();
}

TEST(TestFW_IncorrectFile, GetFile){
    FileWriter* fw;
    fw = new FileWriter("wrong.txt");
    ASSERT_EQ(fw->get_max_id(), 1);
    remove("wrong.txt");
}

int main(int argc, char* argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}