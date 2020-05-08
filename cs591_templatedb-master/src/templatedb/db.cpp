#include "templatedb/db.hpp"
//#include "db.hpp"
//#include <bits/stdc++.h>

#include<string>
#include <sstream>
#include <time.h>
#include <cmath>
using namespace templatedb;




//transforms file name back to unaltered state;
void DB::blankFileName(){
  int size = sizeof(file_name_format)/sizeof(file_name_format[0]);
  for(int i=0;i<size;i++){
    file_name_format[i] = blank[i];
  }
}

void DB::zeroOutBloomFilter(int level, int file){
  std::pair<int, int> tempor;
  tempor.first = level;
  tempor.second = file;
  int size = bloom_filters[tempor].size();
  for(int i =0;i<size;i++){
    bloom_filters[tempor][i] = 0;
  }
}

void DB::setFileName(int level, int component){
  blankFileName();
  int lcopy = level;
  int ccopy = component;
  for(int i = 31;i>=24;i--){
    file_name_format[i] = '0'+(lcopy%10);
    lcopy = lcopy / 10;
  }
  for(int i = 48;i>=33;i--){
    file_name_format[i] = '0'+(ccopy%10);
    ccopy = ccopy / 10;
  }
}

void DB::setManifestName(int level){
  blankFileName();
  int lcopy = level;
  for(int i = 24;i<32;i++){
    file_name_format[i] = '0'+(lcopy%10);
    lcopy = lcopy / 10;
  }

  file_name_format[33] = 'M';
  file_name_format[34] = 'A';
  file_name_format[35] = 'N';
  file_name_format[36] = 'I';
  file_name_format[37] = 'F';
  file_name_format[38] = 'E';
  file_name_format[39] = 'S';
  file_name_format[40] = 'T';

}

void DB::merge_layer(int lay){
  int f = 0;
  diskTable.clear();
  setFileName(lay,f);
  std::ifstream myReadFile;
  while(checkFileExists(file_name_format)){
    int key;
    std::string line;
    bool vis;

    myReadFile.open(file_name_format);
    //diskFile.seekg(0, std::ios::beg);
    std::getline(myReadFile, line); // First line is rows, col
    while (std::getline(myReadFile, line))
    {
      //std::cout<<"Read from "<<file_name_format<<"\n";
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
        if(vis){
          this->putinTemporaryStorage(key, Value(items));
        }else{
          this->putinTemporaryStorage(key, Value(false));
        }

        if (value_dimensions == 0)
            value_dimensions = items.size();
    }

    myReadFile.close();

    f++;
    setFileName(lay,f);
  }

  int current_file = -1;
  int counter = 0;
  std::ofstream myWriteFile;
  std::pair<int,int> tempPair;
  for(auto item: diskTable){
    if((counter/component_file_size) > current_file){
      current_file = counter/component_file_size;
      setFileName(lay,current_file);
      tempPair.first = lay;
      tempPair.second = current_file;
      fence_pointers[tempPair].first = item.first;
      fence_pointers[tempPair].second = item.first;


      if(current_file!=0)
        myWriteFile.close();

      myWriteFile.open(file_name_format);
      //myWriteFile.seekg(0, std::ios::beg);
      std::string header = std::to_string(component_file_size) + ',' + std::to_string(value_dimensions) + '\n';
      myWriteFile << header;
    }

    if(item.first>fence_pointers[tempPair].second){
      fence_pointers[tempPair].second = item.first;
    }

    /////////////////
    int prev = item.first;
    for(int i = 0;i<number_of_bloom_hashes;i++){
      int hash = hashingFunc(prev);
      bloom_filters[tempPair][hash%bloom_filters_size] = 1;
      prev  = hash;
    }
      std::ostringstream line;
      if(item.second.visible){
        std::copy(item.second.items.begin(), item.second.items.end() - 1, std::ostream_iterator<int>(line, ","));
        line << item.second.items.back();
        std::string value(line.str());
        myWriteFile << item.second.visible << ',' << item.first << ',' << value << '\n';
      }else{
        myWriteFile << item.second.visible << ',' << item.first << ',' << '0' << '\n';
      }
      counter++;
  }

  if(current_file<(f-1)){
    for(int i = current_file+1;i<f;i++){
      setFileName(lay,i);
      std::ofstream tempofile;
      tempofile.open(file_name_format);
      tempofile<<"0,0\n";
      diskFile.close();
    }
  }

  diskTable.clear();

}

bool DB::checkFileExists(char fileName[]){
  std::ifstream ifile;
  ifile.open(fileName);
  bool rtrn;
   if(ifile) {
     //std::cout<<"Line 60: "<<fileName<<" exists\n";
      rtrn = true;
      ifile.close();
   } else {
     //std::cout<<"Line 64: "<<fileName<<" does not exist\n";
      rtrn = false;
   }
   return rtrn;
}

bool DB::isLevelFull(int lev){
  setFileName(lev, pow(level_size_multiplier,lev)-1);
  return checkFileExists(file_name_format);
}

void DB::offloadMainMemory(){
    diskFile.open(blank,std::ios::app);
    //diskFile.clear();
    //diskFile.seekg(0, std::ios::beg);
    //diskFile << "HelloWorld"<<'\n';

    std::string header = std::to_string(table.size()) + ',' + std::to_string(value_dimensions) + '\n';
    diskFile << header;
    int counter = 0;
    std::pair <int,int> p;
    p.first = 0;
    p.second = 0;
    int count = 0;
    for(auto item: table)
    {
      count++;

      if(counter==0){
        fence_pointers[p].first = item.first;
      }else if(counter==component_file_size-1){
        fence_pointers[p].second = item.first;
      }



      counter++;
        std::ostringstream line;
        if(item.second.visible){
          std::copy(item.second.items.begin(), item.second.items.end() - 1, std::ostream_iterator<int>(line, ","));
          line << item.second.items.back();
          std::string value(line.str());
          diskFile << item.second.visible << ',' << item.first << ',' << value << '\n';
        }else{
          diskFile << item.second.visible << ',' << item.first << ',' << '0' << '\n';
        }
    }

    diskFile.close();
    table.clear();



    //return true;
}

int DB::firstFreeComponent(int level){
  int max = pow(level_size_multiplier,level);
  for(int i = 0;i<max;i++){
    setFileName(level,i);
    //std::cout<<"Reached line 99:  "<<file_name_format<<"\n";
    if(!checkFileExists(file_name_format)){
      return i;
    }
  }
  return -1;
}

void DB::moveLevels(int level){
  //std::cout<<"Check 0\n";
  if(isLevelFull(level+1)){
    //std::cout<<"Check 0.5\n";
    moveLevels(level+1);
  }
  //std::cout<<"Check 1\n";
  if(level+1>largest_level_seen){
    largest_level_seen = level+1;
  }
  int n = firstFreeComponent(level+1);
  //std::cout<<"Check 2  "<<n;
  int max = pow(level_size_multiplier,level)-1;
  //std::cout<<"Check 3\n";

  //merge levels in a tiered, write optimized way
  if(mode){
    merge_layer(level);
  }

  for(int i = n;i<=n+max;i++){
    char previousFile[54];
    setFileName(level,i-n);
    int size = sizeof(previousFile)/sizeof(previousFile[0]);
    for(int o = 0;o<size;o++){
      previousFile[o] = file_name_format[o];
    }
    setFileName(level+1,i);
    //std::cout<<previousFile<<"\n";
    //std::cout<<file_name_format<<"\n";
    //std::cout<<"\n\n\n\nPrevious name:  "<<previousFile<<"\n"<<"New Name:  "<<file_name_format<<"\n\n\n\n";
    std::pair<int,int> pairPrev;
    std::pair<int,int> pairNew;
    pairPrev.first = level;
    pairPrev.second = i-n;
    pairNew.first = level+1;
    pairNew.second = i;
    fence_pointers[pairNew] = fence_pointers[pairPrev];
    bloom_filters[pairNew] = bloom_filters[pairPrev];
    rename(previousFile,file_name_format);

  }

  //merge the levels in a layered, read optimized way
  if(!mode){
    merge_layer(level+1);
  }


}

void DB::moveFileToMainMemory(int level, int file){
  std::ifstream temp_input;
  setFileName(level,file);
  if(checkFileExists(file_name_format)){
    int key;
    std::string line;
    bool vis;
    temp_input.open(file_name_format,std::ios::in);
    std::getline(temp_input, line); // First line is rows, col
    while (std::getline(temp_input, line))
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
        if(vis){
          this->putinTemporaryStorage(key, Value(items));
        }else{
          this->putinTemporaryStorage(key, Value(false));
        }

        if (value_dimensions == 0)
            value_dimensions = items.size();
    }


  }
  temp_input.close();
}

void DB::temporary_function_loadDisktoMainMemory(){
    for(int counter = largest_level_seen;counter>0;counter--){
      for(int counter2 = pow(level_size_multiplier,counter); counter2>=0;counter2--){
        setFileName(counter,counter2);
        if(checkFileExists(file_name_format)){
          int key;
          std::string line;
          bool vis;
          diskFile.open(file_name_format,std::ios::in);
          std::getline(diskFile, line); // First line is rows, col
          while (std::getline(diskFile, line))
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
              if(vis){
                this->putinTemporaryStorage(key, Value(items));
              }else{
                this->putinTemporaryStorage(key, Value(false));
              }

              if (value_dimensions == 0)
                  value_dimensions = items.size();
          }


        }
      }
    }

}

void DB::addOp(int write, int read){


  if(swap){
    num_inserts+=write;
    num_reads+=read;
    if(num_inserts/(num_inserts+num_reads)>=(tier_level_swap_boundry+swap_cushion)){
      mode = true;
    }
    if(num_inserts/(num_inserts+num_reads)>=(1-(tier_level_swap_boundry+swap_cushion))){
      mode = false;
    }

  }
  //if(((num_inserts/2)+(num_reads/2))>)

}

bool DB::matches_hash(std::pair<int,int> placement,int i){
  int prev = i;
  for(int i = 0;i<number_of_bloom_hashes;i++){
    int hash = hashingFunc(prev);
    if(bloom_filters[placement][hash]!=1){
      return false;
    }
    prev  = hash;
  }
  return true;
}

Value DB::get(int key)
{

    addOp(0,1);
    if (table.count(key))
        return table[key];


    //std::cout<<"Made it to 179: loading disk\n";
    std::pair<int,int> fence;
    for(int i = 0;i<largest_level_seen;i++){
      int size = pow(level_size_multiplier,i);
      for(int o = 0;o<size;o++){
        setFileName(i,o);
        if(checkFileExists(file_name_format)){
          fence.first = i;
          fence.second = o;
          if(fence_pointers[fence].first<=key && fence_pointers[fence].second>=key){
            if(matches_hash(fence,key)){
              diskTable.clear();

              moveFileToMainMemory(i,o);

              if(diskTable.count(key))
                return diskTable[key];
            }

          }
          diskTable.clear();
        }else{
          //o=size;
          //i++;
        }
      }
    }

    //temporary_function_loadDisktoMainMemory();
    //if(diskTable.count(key))
        //return diskTable[key];
    diskTable.clear();
    return Value(false);
}

void DB::put(int key, Value val)
{
    addOp(1,0);
    if(table.size()==main_memory_max_size){
      offloadMainMemory();
      moveLevels(0);
    }
    table[key] = val;

    int prev = key;
    std::pair<int,int> tempor;
    tempor.first = 0;
    tempor.second = 0;
    for(int i = 0;i<number_of_bloom_hashes;i++){
      int hash = hashingFunc(prev);
      bloom_filters[tempor][hash%bloom_filters_size] = 1;
      prev  = hash;
    }


    //offloadMainMemory();
}

void DB::putinTemporaryStorage(int key, Value val)
{
    diskTable[key] = val;


    //offloadMainMemory();
}

std::vector<Value> DB::scan()
{
    addOp(0,1);
    std::vector<Value> return_vector;
    temporary_function_loadDisktoMainMemory();
    //std::cout<<"\n\nTotal Table Size: "<<table.size()<<"\n";
    for (auto pair: table)
    {
        diskTable[pair.first] =pair.second;
        //return_vector.push_back(pair.second);
    }
    //std::cout<<"\n\nTotal Table Size: "<<table.size()<<"\n";
    //std::cout<<"\n\nTotal Disktable Size: "<<diskTable.size()<<"\n";
    for (auto pair: diskTable)
    {
      if(pair.second.visible)
        return_vector.push_back(pair.second);
    }
    //std::cout<<"\n\nTotal Disktable Size: "<<diskTable.size()<<"\n";
    //std::cout<<blank<<"\n";
    diskTable.clear();
    //std::cout<<"Scan size: "<<return_vector.size()<<"\n\n";
    return return_vector;
}

std::vector<Value> DB::scan(int min_key, int max_key)
{
    addOp(0,1);
    std::vector<Value> return_vector;
    temporary_function_loadDisktoMainMemory();
    //std::cout<<"\n\nTotal Table Size: "<<table.size()<<"\n";
    for (auto pair: table)
    {
        diskTable[pair.first] =pair.second;
        //return_vector.push_back(pair.second);
    }
    //std::cout<<"\n\nTotal Table Size: "<<table.size()<<"\n";
    //std::cout<<"\n\nTotal Disktable Size: "<<diskTable.size()<<"\n";
    for (auto pair: diskTable)
    {
        if ((pair.first >= min_key) && (pair.first <= max_key))//&&pair.second.visible)
            return_vector.push_back(pair.second);
    }
    //std::cout<<"\n\nTotal Disktable Size: "<<diskTable.size()<<"\n";
    //std::cout<<blank<<"\n";
    diskTable.clear();
    //std::cout<<"Scan Min-Max size: "<<return_vector.size()<<"\n\n";
    return return_vector;
}

void DB::del(int key)
{
    addOp(1,0);

    Value v = Value(false);
    put(key, v);
    //table[key] = Value(false);
    number_of_deletes++;

    //table.erase(key);
}

size_t DB::size()
{
    temporary_function_loadDisktoMainMemory();
    for (auto pair: table)
    {
        diskTable[pair.first] = pair.second;
        //return_vector.push_back(pair.second);
    }
    int size = 0;
    for (auto pair: diskTable)
    {
        if (pair.second.visible){
          size++;
        }
    }
    //std::cout<<"\n\nLine 272: Size returned = "<<size<<"\n\n";
    return size;
}

std::vector<Value> DB::execute_op(Operation op)
{
    std::vector<Value> results;
    if (op.type == GET)
    {
        results.push_back(this->get(op.key));
    }
    else if (op.type == PUT)
    {
        this->put(op.key, Value(op.args));
    }
    else if (op.type == SCAN)
    {
        results = this->scan(op.key, op.args[0]);
    }
    else if (op.type == DELETE)
    {
        this->del(op.key);
    }

    return results;
}

bool DB::load_data_file(std::string & fname)
{
    std::ifstream fid(fname);
    if (fid.is_open())
    {
        int key;
        std::string line;
        std::getline(fid, line); // First line is rows, col
        while (std::getline(fid, line))
        {
            std::stringstream linestream(line);
            std::string item;

            std::getline(linestream, item, ',');
            key = stoi(item);
            std::vector<int> items;
            while(std::getline(linestream, item, ','))
            {
                items.push_back(stoi(item));
            }
            this->put(key, Value(items));
        }
    }
    else
    {
        fprintf(stderr, "Unable to read %s\n", fname.c_str());
        return false;
    }

    return true;
}

db_status DB::open(std::string & fname)
{

    this->file.open(fname, std::ios::in | std::ios::out);
    if (file.is_open())
    {
        this->status = OPEN;
        // New file implies empty file
        if (file.peek() == std::ifstream::traits_type::eof())
            return this->status;

        int key;
        bool vis;
        std::string line;
        std::getline(file, line); // First line is rows, col
        while (std::getline(file, line))
        {

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
            if(vis){
              this->put(key, Value(items));
            }else{
              this->put(key, Value(false));
            }
            if (value_dimensions == 0)
                value_dimensions = items.size();
        }
    }
    else if (!file) // File does not exist
    {
        this->file.open(fname, std::ios::out);
        this->status = OPEN;
    }
    else
    {
        file.close();
        this->status = ERROR_OPEN;
    }

    return this->status;
}

bool DB::close()
{


  //std::cout<<"\nLine 408: Database closed "<<file_name_format<<"\n";
    if (file.is_open())
    {
        this->write_to_file();
        file.close();
    }
    this->status = CLOSED;

    return true;
}

bool DB::write_to_file()
{
    file.clear();
    file.seekg(0, std::ios::beg);

    std::string header = std::to_string(table.size()) + ',' + std::to_string(value_dimensions) + '\n';
    file << header;

    for(auto item: table)
    {
        std::ostringstream line;
        if(item.second.visible){
          std::copy(item.second.items.begin(), item.second.items.end() - 1, std::ostream_iterator<int>(line, ","));
          line << item.second.items.back();
          std::string value(line.str());
          file << item.second.visible << ',' << item.first << ',' << value << '\n';
        }else{
          file << item.second.visible << ',' << item.first << ',' << '0' << '\n';
        }

    }

    return true;
}
