#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "templatedb/db.hpp"
#include <fstream>


class DBTest : public ::testing::Test
{

struct command_key_val{
  int command;//0 = PUT, 1 = GET, 2 = SCAN,3 DELETE
  int key;
  templatedb::Value val;
};
protected:



    void SetUp() override
    {}

};


TEST(DBTest, DidNotCrash)
{
  templatedb::DB db;
  std::fstream file;
  file.open("running_data.data",std::ios::in);

  int key;
  std::string line;
  bool vis;
  std::getline(file, line); // First line is rows, col
  while (std::getline(file, line))
  {
      //std::cout<<"Line 153: Inside getline loop\n";
      std::stringstream linestream(line);
      std::string item;

      std::getline(linestream, item, ',');
      vis = stoi(item);

      std::getline(linestream, item, ',');
      key = stoi(item);
      std::vector<int> items;
      while(std::getline(linestream, item, ','))
      {
          items.push_back(stoi(item));
      }
      db.put(key,templatedb::Value(items));

  }
    EXPECT_EQ(0, 0);
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
