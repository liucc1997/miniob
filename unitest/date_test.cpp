/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by wangyunlai.wyl on 2021
//

#include "storage/common/date.h"
#include "gtest/gtest.h"

TEST(test_date, test_date_main) {

  // 
  int years = 2012, months = 3, days = 15;
  int v = datetint(years, months, days);
  int y, m, d;
  int rc = inttdate(v, y, m, d);
  ASSERT_EQ(rc, 0);
  ASSERT_EQ(y, 2012);
  ASSERT_EQ(m, 3);
  ASSERT_EQ(d, 15);
}

TEST(test_date, charstdate_function) {
  int y, m, d;
  int r;
  r = charstdate("2022-12-31", y, m, d);
  printf("<LCC> %d %d-%d-%d\n", r, y, m, d);
  ASSERT_EQ(r, 0);
  ASSERT_EQ(y, 2022);
  ASSERT_EQ(m, 12);
  ASSERT_EQ(d, 31);
  r = charstdate("   1970-8-31    ", y, m, d);
  printf("<LCC> %d %d-%d-%d\n", r, y, m, d);
  ASSERT_EQ(r, 0);
  ASSERT_EQ(y, 1970);
  ASSERT_EQ(m, 8);
  ASSERT_EQ(d, 31);
  r = charstdate("19707-277-391", y, m, d);
  printf("<LCC> %d %d-%d-%d\n", r, y, m, d);
  ASSERT_EQ(r, INVALID_DATE);
  ASSERT_EQ(y, 1970);
  ASSERT_EQ(m, 8);
  ASSERT_EQ(d, 31);
  r = charstdate("1970dsnm-391", y, m, d);
  printf("<LCC> %d %d-%d-%d\n", r, y, m, d);
  ASSERT_EQ(r, INVALID_DATE);
  ASSERT_EQ(y, 1970);
  ASSERT_EQ(m, 8);
  ASSERT_EQ(d, 31);
  r = charstdate("20221222", y, m, d);
  ASSERT_EQ(r, INVALID_DATE);
  r = charstdate("2022-0229", y, m, d);
  ASSERT_EQ(r, INVALID_DATE);  

}

TEST(test_date, date_class) {
  Date d("2012-02-9");
  int v = d.date_value_;
  Date d2(v);
  printf("<LCC> %d %d\n", d.date_value_, d.date_value_);
  ASSERT_EQ(d.date_value_, d.date_value_);
  printf("<LCC> %s %s\n", d.to_string(), d.to_string());
  ASSERT_STREQ(d.to_string(), d2.to_string());

  Date d3(2012, 1, 1);
  ASSERT_STREQ(d3.to_string(), "2012-01-01");

  Date d4(d3.date_value_);
  ASSERT_STREQ(d4.to_string(), "2012-01-01");

  Date d5("2011-07-100");
  ASSERT_EQ(d5.date_value_, INVALID_DATE);
  ASSERT_STREQ(d5.to_string(), nullptr);

  Date d6(2011, -1, 100);
  ASSERT_EQ(d6.date_value_, INVALID_DATE);
  ASSERT_STREQ(d6.to_string(), nullptr);

  char* s11 = "2019-12-20"; 
  char* s12 = "2019-12-21"; 
  Date d11(s11);
  Date d12(s12);
  printf("<LCC> %s %d %s\n", s11, d11.date_value_, d11.to_string());
  printf("<LCC> %s %d %s\n", s12, d12.date_value_, d12.to_string());
  ASSERT_STREQ(d11.to_string(), s11);

}
int main(int argc, char **argv) {


  // 分析gtest程序的命令行参数
  testing::InitGoogleTest(&argc, argv);

  // 调用RUN_ALL_TESTS()运行所有测试用例
  // main函数返回RUN_ALL_TESTS()的运行结果
  return RUN_ALL_TESTS();
}