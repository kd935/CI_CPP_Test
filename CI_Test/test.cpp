#include "pch.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <filesystem>
#include <algorithm>

using namespace std;
using namespace filesystem;

 path file_path {"test.txt"};
 string text{"Hello world!"};
 short number = 333;

TEST(Test, WritingFile) {
  ofstream file {file_path, ios::binary};
  ostream_iterator<char> it{file};

  copy(text.begin(), text.end(), it);
  copy_n(reinterpret_cast<char*>(&number), sizeof(short), it);

  file.close();

  EXPECT_TRUE(exists(file_path));
  EXPECT_EQ(file_size(file_path), size(text) * sizeof(char) + sizeof(short));
}

TEST(Test, ReadingFile) {
  ifstream file {file_path, ios::binary};
  noskipws(file);

  istream_iterator<char> begin_string{file};

  string read_text{};
  short read_number{};

  copy_n(begin_string, size(text) * sizeof(char), back_inserter(read_text));

  istream_iterator<char> begin_short{file};
  copy(begin_short, {}, reinterpret_cast<char*>(&read_number));

  file.close();
  
  EXPECT_EQ(text, read_text);
  EXPECT_EQ(number, read_number);
  EXPECT_TRUE(remove(file_path));
}