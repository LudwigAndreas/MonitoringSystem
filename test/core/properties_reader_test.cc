//
// Created by Ludwig Andreas on 04.08.2023.
//
#include "../test.h"

#include <fstream>
#include <iostream>
#include "thread"

#include "core/config/PropertiesReader.h"

 class PropertiesReaderTest : public ::testing::Test {
  protected:
  static void WriteTestFile(const std::string& file_path,
                            const std::string& content) {
    std::ofstream output_file(file_path);
    output_file << content;
    output_file.flush();
    output_file.close();
    if (output_file.is_open())
      std::cerr << "still opened" << std::endl;
  }
};

TEST_F(PropertiesReaderTest, file_update_test) {
  std::string file_path = "test_file.properties";
  std::string file_content = "agent.name=cpu_agent_name_linux\n"
                            "agent.type=cpu_agent\n"
                            "agent.update_time_ms=350\n"
                            "agent.metrics=[\"cpu\", \"processes\"]\n"
                            "agent.metrics.cpu.return=double\n"
                            "agent.metrics.cpu.critical_value=\">=20\"\n"
                            "agent.metrics.processes.return=\"double\"\n"
                            "agent.metrics.processes.critical_value=\">=20\"";
  WriteTestFile(file_path, file_content);
  s21::PropertiesReader properties_reader = s21::PropertiesReader(file_path);
  EXPECT_EQ(properties_reader.GetValue("agent.update_time_ms"), "350");

  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::string updated_content = "agent.name=cpu_agent_name_linux\n"
                             "agent.type=cpu_agent\n"
                             "agent.update_time_ms=150\n"
                             "agent.metrics=[\"cpu\", \"processes\"]\n"
                             "agent.metrics.cpu.return=int\n"
                             "agent.metrics.cpu.critical_value=\"<20\"\n"
                             "agent.metrics.processes.return=\"double\"\n"
                             "agent.metrics.processes.critical_value=\">=20\"";
  WriteTestFile(file_path, updated_content);
  properties_reader.Update();
  EXPECT_EQ(properties_reader.GetValue("agent.update_time_ms"), "150");
  properties_reader.Update();
  EXPECT_EQ(properties_reader.GetValue("agent.update_time_ms"), "150");
}

