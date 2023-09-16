//
// Created by Ludwig Andreas on 14.09.2023.
//
#include "config/properties.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "test.h"

class PropertiesTest : public ::testing::Test {
 protected:
  void SetUp() override {
    properties_ = std::make_shared<s21::Properties>();
    std::filesystem::path path_to_file(file_name);
    std::filesystem::remove_all(path_to_file);

    std::ofstream outfile(file_name);
    outfile.close();
  }

  void TearDown() override { std::filesystem::remove_all(file_name); }

  std::string file_name = "test.properties";
  std::shared_ptr<s21::Properties> properties_;
};

TEST_F(PropertiesTest, LoadProperties) {
  std::ofstream outfile(file_name);
  outfile << "test1=test1\n";
  outfile << "test2=test2\n";
  outfile << "test3=test3\n";
  outfile.close();

  properties_->Load(file_name);

  EXPECT_EQ(properties_->GetProperty("test1"), "test1");
  EXPECT_EQ(properties_->GetProperty("test2"), "test2");
  EXPECT_EQ(properties_->GetProperty("test3"), "test3");
}

TEST_F(PropertiesTest, GetProperty) {
  std::ofstream outfile(file_name);
  outfile << "test1=test1\n";
  outfile << "test2=test2\n";
  outfile << "test3=test3\n";
  outfile.close();

  properties_->Load(file_name);

  EXPECT_EQ(properties_->GetProperty("test1"), "test1");
  EXPECT_EQ(properties_->GetProperty("test2"), "test2");
  EXPECT_EQ(properties_->GetProperty("test3"), "test3");
  EXPECT_EQ(properties_->GetProperty("test4"), "");
  EXPECT_EQ(properties_->Get("test4"), "");
  EXPECT_EQ(properties_->GetProperty("test4", ""), "");
  EXPECT_EQ(properties_->GetProperty("test3", ""), "test3");
}

TEST_F(PropertiesTest, SaveProperties) {
  properties_->Load(file_name);
  properties_->SetProperty("test1", "test1");
  properties_->SetProperty("test2", "test2");
  properties_->SetProperty("test3", "test3");
  properties_->Save();

  ASSERT_TRUE(std::filesystem::exists(file_name));
  std::ifstream infile(file_name);
  std::string line;
  std::getline(infile, line);
  EXPECT_EQ(line, "test1 = test1");
  std::getline(infile, line);
  EXPECT_EQ(line, "test2 = test2");
  std::getline(infile, line);
  EXPECT_EQ(line, "test3 = test3");
}

TEST_F(PropertiesTest, SaveWithContentProperties) {
  std::ofstream outfile(file_name);
  outfile << "test1=test1\n";
  outfile << "test2= \"test2\"\n";
  outfile << "test3= \"test3\n";
  outfile << "# comment\n";
  outfile.close();
  properties_->Load(file_name);
  properties_->SetProperty("test4", "test4");
  properties_->SetProperty("test5", "test5");
  properties_->SetProperty("test6", "test6");
  properties_->Save();

  EXPECT_TRUE(std::filesystem::exists(file_name));
  std::ifstream infile(file_name);
  std::string line;
  std::getline(infile, line);
  EXPECT_EQ(line, "test1 = \"test1\"");
  std::getline(infile, line);
  EXPECT_EQ(line, "test2 = \"test2\"");
  std::getline(infile, line);
  EXPECT_EQ(line, "test3 = \"test3\"");
  std::getline(infile, line);
  EXPECT_EQ(line, "# comment");
  std::getline(infile, line);
  EXPECT_EQ(line, "test4 = test4");
  std::getline(infile, line);
  EXPECT_EQ(line, "test5 = test5");
  std::getline(infile, line);
  EXPECT_EQ(line, "test6 = test6");
}

TEST_F(PropertiesTest, GetNames) {
  std::ofstream outfile(file_name);
  outfile << "test1=test1\n";
  outfile << "test2=test2\n";
  outfile << "test3=test3\n";
  outfile.close();

  properties_->Load(file_name);

  EXPECT_EQ(properties_->GetProperty("test1"), "test1");
  EXPECT_EQ(properties_->GetProperty("test2"), "test2");
  EXPECT_EQ(properties_->GetProperty("test3"), "test3");
  std::set<std::string> names = properties_->StringPropertyNames();
  EXPECT_EQ(names.size(), 3);
  EXPECT_TRUE(names.find("test1") != names.end());
  EXPECT_TRUE(names.find("test2") != names.end());
  EXPECT_TRUE(names.find("test3") != names.end());
  EXPECT_TRUE(names.find("test4") == names.end());
}

TEST_F(PropertiesTest, ErrorHandling) {
  properties_->Load("NotExistingFile");
  EXPECT_EQ(properties_->GetProperty("test1"), "");
  EXPECT_EQ(properties_->GetProperty("test2"), "");
  EXPECT_EQ(properties_->GetProperty("test3"), "");

  properties_->Save();
  EXPECT_FALSE(std::filesystem::exists("NotExistingFile"));

  std::ofstream outfile(file_name);
  outfile << "\"test1=test1\n";
  outfile << "test2\"=test2\n";
  outfile << "test3=\"test3\n";
  outfile << "#test4=test4\n";
  outfile << "test5#=test5\n";
  outfile << "test6=#test6\n";
  outfile.close();
  properties_->Load(file_name);
  EXPECT_EQ(properties_->GetProperty("test1"), "");
  EXPECT_EQ(properties_->GetProperty("test2"), "");
  EXPECT_EQ(properties_->GetProperty("test3"), "test3");
  EXPECT_EQ(properties_->GetProperty("test4"), "");
  EXPECT_EQ(properties_->GetProperty("test5"), "");
  EXPECT_EQ(properties_->GetProperty("test6"), "");
}

TEST(AllArgsPropertiesTest, AllArgsConstructor) {
  auto properties_ = std::make_shared<s21::Properties>(
      ":=", "//", std::map<std::string, std::string>());
  std::string filename = "test.properties";
  std::filesystem::path path_to_file(filename);
  std::filesystem::remove_all(path_to_file);

  std::ofstream outfile(filename);
  outfile << "test0:=\"\"\n";
  outfile << "\"test1:=test1\n";
  outfile << "test2\":=test2\n";
  outfile << "test3:= \"test3\n";
  outfile << "//test4:=test4\n";
  outfile << "test5//:=test5\n";
  outfile << "test6:=//test6\n";
  outfile << "test7:=test7//\n";
  outfile << "test8:=test8\n";
  outfile << "test9:==test9\n";
  outfile << "test10:=:=:=:=:=test10\n";
  outfile.close();
  properties_->Load(filename);
  EXPECT_EQ(properties_->GetProperty("test0"), "");
  EXPECT_EQ(properties_->GetProperty("test1"), "");
  EXPECT_EQ(properties_->GetProperty("test2"), "");
  EXPECT_EQ(properties_->GetProperty("test3"), "test3");
  EXPECT_EQ(properties_->GetProperty("test4"), "");
  EXPECT_EQ(properties_->GetProperty("test5"), "");
  EXPECT_EQ(properties_->GetProperty("test6"), "");
  EXPECT_EQ(properties_->GetProperty("test7"), "test7");
  EXPECT_EQ(properties_->GetProperty("test8"), "test8");
  EXPECT_EQ(properties_->GetProperty("test9"), "=test9");
  EXPECT_EQ(properties_->GetProperty("test10"), ":=:=:=:=test10");
}