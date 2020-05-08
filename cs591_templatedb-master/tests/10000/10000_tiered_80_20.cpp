#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "templatedb/db.hpp"
#include <fstream>
#include <time.h>

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
  //std::fstream f;
  //f.open("log.txt",std::ios::app);
  //f <<"Start";
  //f.close();
  //std::cout<<"Start Test\n";
  int num_ops = 10000;  //num of ops in test
  double percent_reads = 0.8;
  bool mode_imp = true; //true == tiered
  bool swap  = false;
  int size = 10000;

  srand(time(0));

  templatedb::DB db;
  db.mode = mode_imp;
  db.swap = swap;
  std::fstream file;
  file.open("test_10000_2.data",std::ios::in);

  std::vector<std::pair <int,templatedb::Value>> keyvals;
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
      std::pair<int,templatedb::Value> p;
      p.first = key;
      p.second = templatedb::Value(items);
      keyvals.push_back(p);

  }

    for(int i = 0;i<num_ops;i++){
      int pos = rand()%size;

      if(rand()%100<(percent_reads*100)){
        db.get(keyvals[pos].first);
      }else{
        if(rand()%2==0){
          db.put(keyvals[pos].first,keyvals[pos].second);
        }else{
          //db.put(keyvals[pos].first,keyvals[pos].second);
          db.del(keyvals[pos].first);
        }
      }
    }




    EXPECT_EQ(0, 0);
}



int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
