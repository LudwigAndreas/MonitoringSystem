//
// Created by Ludwig Andreas on 10.09.2023.
//

#ifndef MONITORINGSYSTEM_MOCKPROPERTIES_H_
#define MONITORINGSYSTEM_MOCKPROPERTIES_H_

#include "config/properties.h"

class MockProperties : public s21::Properties {
  public:

  MockProperties()
      : Properties() {}
  // Constructor that takes the same parameters as the real Properties class
  MockProperties(const std::string& separator, const std::string& comment_symb, const std::map<std::string, std::string>& properties)
      : Properties(separator, comment_symb, properties) {}

  // Mock the Load method
  MOCK_METHOD(void, Load, (const std::string&));

  // Mock the Get method
  MOCK_METHOD(std::string, Get, (const std::string&));

  // Mock the SetProperty method
  MOCK_METHOD(void, SetProperty, (const std::string&, const std::string&));

  // Mock the Save method
  MOCK_METHOD(void, Save, ());

  // Mock the StringPropertyNames method
  MOCK_METHOD(std::set<std::string>, StringPropertyNames, ());
};

#endif //MONITORINGSYSTEM_MOCKPROPERTIES_H_
