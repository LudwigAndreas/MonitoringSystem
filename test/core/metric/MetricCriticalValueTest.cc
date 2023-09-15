//
// Created by Ludwig Andreas on 14.09.2023.
//

#include "test.h"
#include "metric/MetricCriticalValue.h"

TEST(MetricCriticalValueTest, Parsing) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value = std::make_shared<s21::MetricCriticalValue>(">=30");

  EXPECT_EQ(critical_value->GetConditionOperator(), ">=");
  EXPECT_EQ(critical_value->GetCriticalValue(), 30);
  EXPECT_EQ(critical_value->ToString(), ">=30");
}

TEST(MetricCriticalValueTest, ParsingError) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value = std::make_shared<s21::MetricCriticalValue>(">=asdf");

  EXPECT_EQ(critical_value->GetConditionOperator(), ">=");
  EXPECT_EQ(critical_value->GetCriticalValue(), 0);
  EXPECT_EQ(critical_value->ToString(), ">=0");

  std::shared_ptr<s21::MetricCriticalValue> critical_value2 = std::make_shared<s21::MetricCriticalValue>("a;dlnc");

  EXPECT_EQ(critical_value2->GetConditionOperator(), "");
  EXPECT_EQ(critical_value2->GetCriticalValue(), 0);
  EXPECT_EQ(critical_value2->ToString(), "0");
}

TEST(MetricCriticalValueTest, IsCritical) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value = std::make_shared<s21::MetricCriticalValue>(">=30");

  EXPECT_TRUE(critical_value->IsCritical(30));
  EXPECT_TRUE(critical_value->IsCritical(31));
  EXPECT_FALSE(critical_value->IsCritical(29));
  EXPECT_FALSE(critical_value->IsCritical(0));
}

TEST(MetricCriticalValueTest, IsCriticalString) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value = std::make_shared<s21::MetricCriticalValue>(">=30");

  EXPECT_TRUE(critical_value->IsCritical("30"));
  EXPECT_TRUE(critical_value->IsCritical("31"));
  EXPECT_FALSE(critical_value->IsCritical("29"));
  EXPECT_FALSE(critical_value->IsCritical("0"));

}

TEST(MetricCriticalValueTest, IsCriticalStringError) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value = std::make_shared<s21::MetricCriticalValue>(">=30");

  EXPECT_FALSE(critical_value->IsCritical("asdf"));
  EXPECT_FALSE(critical_value->IsCritical("asdfasdf"));
  EXPECT_FALSE(critical_value->IsCritical("0asd"));
  EXPECT_FALSE(critical_value->IsCritical(""));
}

TEST(MetricCriticalValueTest, Operators) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value_ge = std::make_shared<s21::MetricCriticalValue>(">=30");
  std::shared_ptr<s21::MetricCriticalValue> critical_value_le = std::make_shared<s21::MetricCriticalValue>("<=30");
  std::shared_ptr<s21::MetricCriticalValue> critical_value_e = std::make_shared<s21::MetricCriticalValue>("==30");
  std::shared_ptr<s21::MetricCriticalValue> critical_value_g = std::make_shared<s21::MetricCriticalValue>(">30");
  std::shared_ptr<s21::MetricCriticalValue> critical_value_l = std::make_shared<s21::MetricCriticalValue>("<30");
  std::shared_ptr<s21::MetricCriticalValue> critical_value_ne = std::make_shared<s21::MetricCriticalValue>("!=30");

  EXPECT_TRUE(critical_value_ge->IsCritical(30));
  EXPECT_TRUE(critical_value_ge->IsCritical(31));
  EXPECT_FALSE(critical_value_ge->IsCritical(29));

  EXPECT_TRUE(critical_value_le->IsCritical(30));
  EXPECT_TRUE(critical_value_le->IsCritical(29));
  EXPECT_FALSE(critical_value_le->IsCritical(31));

  EXPECT_TRUE(critical_value_e->IsCritical(30));
  EXPECT_FALSE(critical_value_e->IsCritical(29));
  EXPECT_FALSE(critical_value_e->IsCritical(31));

  EXPECT_TRUE(critical_value_g->IsCritical(31));
  EXPECT_FALSE(critical_value_g->IsCritical(30));
  EXPECT_FALSE(critical_value_g->IsCritical(29));

  EXPECT_TRUE(critical_value_l->IsCritical(29));
  EXPECT_FALSE(critical_value_l->IsCritical(30));
  EXPECT_FALSE(critical_value_l->IsCritical(31));

  EXPECT_TRUE(critical_value_ne->IsCritical(29));
  EXPECT_TRUE(critical_value_ne->IsCritical(31));
  EXPECT_FALSE(critical_value_ne->IsCritical(30));
}


TEST(MetricCriticalValueTest, GetSet) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value = std::make_shared<s21::MetricCriticalValue>(">=30");

  critical_value->SetConditionOperator("==");
  critical_value->SetCriticalValue(20);

  EXPECT_EQ(critical_value->GetConditionOperator(), "==");
  EXPECT_EQ(critical_value->GetCriticalValue(), 20);

  critical_value->SetConditionOperator(">");
  critical_value->SetCriticalValue(10);

  EXPECT_EQ(critical_value->GetConditionOperator(), ">");
  EXPECT_EQ(critical_value->GetCriticalValue(), 10);

  critical_value->SetConditionOperator("<=");
  critical_value->SetCriticalValue(5);

  EXPECT_EQ(critical_value->GetConditionOperator(), "<=");
  EXPECT_EQ(critical_value->GetCriticalValue(), 5);

  critical_value->SetConditionOperator("<");
  critical_value->SetCriticalValue(0);

  EXPECT_EQ(critical_value->GetConditionOperator(), "<");
  EXPECT_EQ(critical_value->GetCriticalValue(), 0);

  critical_value->SetConditionOperator("!=");
  critical_value->SetCriticalValue(10);

  EXPECT_EQ(critical_value->GetConditionOperator(), "!=");
  EXPECT_EQ(critical_value->GetCriticalValue(), 10);

}

TEST(MetricCriticalValueTest, ToString) {

  std::shared_ptr<s21::MetricCriticalValue> critical_value = std::make_shared<s21::MetricCriticalValue>(">=30");

  EXPECT_EQ(critical_value->ToString(), ">=30");

  critical_value->SetConditionOperator("==");
  critical_value->SetCriticalValue(20);

  EXPECT_EQ(critical_value->ToString(), "==20");

  critical_value->SetConditionOperator(">");
  critical_value->SetCriticalValue(10);

  EXPECT_EQ(critical_value->ToString(), ">10");

  critical_value->SetConditionOperator("<=");
  critical_value->SetCriticalValue(5);

  EXPECT_EQ(critical_value->ToString(), "<=5");

  critical_value->SetConditionOperator("<");
  critical_value->SetCriticalValue(0);

  EXPECT_EQ(critical_value->ToString(), "<0");

  critical_value->SetConditionOperator("!=");
  critical_value->SetCriticalValue(10);

  EXPECT_EQ(critical_value->ToString(), "!=10");

}