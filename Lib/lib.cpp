#include "lib.hpp"

void descendSortAndReplacement(std::string &in) {
  std::vector<char> ch(in.begin(), in.end());
  std::sort(ch.begin(), ch.end(), [](char a, char b) { return a > b; });
  for (size_t i = 0; i < ch.size(); i++) {
    if (ch[i] % 2 == 0) {
      ch[i] = 'K';
      ch.emplace(ch.begin() + i + 1, 'B');
      i++;
    }
  }
  in.assign(ch.begin(), ch.end());
}

int sumNum(const std::string &in) {
  int sum = 0;
  for (size_t i = 0; i < in.length(); i++) {
    char ch = in[i];
    if (ch >= '0' && ch <= '9') {
      sum += (ch - '0');
    }
  }
  return sum;
}

bool strCheck(const std::string &in) {
  int len = in.length();
  if (len > 2 && len % 32 == 0)
    return true;
  return false;
}