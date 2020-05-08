#ifndef _TEMPLATEDB_DB_H_
#define _TEMPLATEDB_DB_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <cstdio>
#include <functional>
#include "templatedb/operation.hpp"
#include <array>
//#include "operation.hpp"

namespace templatedb
{

typedef enum _status_code
{
    OPEN = 0,
    CLOSED = 1,
    ERROR_OPEN = 100,
} db_status;


class Value
{
public:
    std::vector<int> items;
    bool visible = true;

    Value() {}
    Value(bool _visible) {visible = _visible;}
    Value(std::vector<int> _items) { items = _items;}

    bool operator ==(Value const & other) const
    {
        return (visible == other.visible) && (items == other.items);
    }
};

class DB
{
public:

    std::fstream debug;


    db_status status;
    char file_name_format[54] ="daisydata/DaisyDB_Level_00000000_0000000000000000.txt";
    char blank[54] ="daisydata/DaisyDB_Level_00000000_0000000000000000.txt";
    char temporary_file[54] ="daisydata/tempory_storage.txt";
    // 24 33
    char manifest[9] = "MANIFEST";

    DB() {
      //std::cout<<"Construct";
      //debug.open("debug.txt",std::ios::app);
      int madeF = -1;
      while(madeF==-1){
        int num = rand()%10000;
        char folderName[11] = "daisydata";
        for(int i = 5;i<=8;i++){
          blank[i] = '0'+(num%10);

          folderName[i] = '0'+(num%10);
          file_name_format[i] = '0'+(num%10);
          temporary_file[i] = '0'+(num%10);

          num = num / 10;
        }
        madeF = mkdir(folderName,0777);
      }


      //debug << "HelloWorld\n";
      //debug.close();

    };
    ~DB() {close();};
    //bool operator<(const int t1[], const int t2[]);

    void blankFileName();
    void setFileName(int level, int component);
    void setManifestName(int level);
    bool checkFileExists(char fileName[]);
    bool isLevelFull(int lev);
    void offloadMainMemory();
    int  firstFreeComponent(int level);
    void moveLevels(int level);
    void temporary_function_loadDisktoMainMemory();
    void putinTemporaryStorage(int key, Value val);
    void merge_layer(int lay);
    void moveFileToMainMemory(int level, int file);
    void zeroOutBloomFilter(int level, int file);
    bool matches_hash(std::pair<int,int> placement,int i);
    void addOp(int write, int read);

    Value get(int key);
    void put(int key, Value val);
    std::vector<Value> scan();

    std::vector<Value> scan(int min_key, int max_key);
    void del(int key);
    size_t size();

    db_status open(std::string & fname);
    bool close();

    bool load_data_file(std::string & fname);

    std::vector<Value> execute_op(Operation op);

    double tier_level_swap_boundry = 0.5; // write/total
    double swap_cushion            = 0.2;
    double num_inserts = 0;
    double num_reads = 0;
    bool swap = false;
    bool mode = true; //true for tiering (write optimized), false for layering (read optimized)

private:
    std::fstream file;
    std::fstream diskFile;
    std::map<int, Value> table;
    std::map<int, Value> diskTable;
    size_t value_dimensions = 0;
    int number_of_deletes = 0;
    int largest_level_seen = 0;
    static constexpr int main_memory_max_size       = 256;
    static constexpr int component_file_size        = main_memory_max_size;
    static constexpr int num_bits_per_value         = 10;
    static constexpr double level_size_multiplier   = 2.0;
    static constexpr int bloom_filters_size         = component_file_size*num_bits_per_value;
    static constexpr int number_of_bloom_hashes     = 3;

    std::map<std::pair<int,int>,std::pair<int,int>> fence_pointers;
    std::map<std::pair<int,int>,std::array<bool,bloom_filters_size>> bloom_filters;
    std::hash<int> hashingFunc;
    bool write_to_file();
};

}   // namespace templatedb

#endif /* _TEMPLATEDB_DB_H_ */
