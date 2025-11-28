#include <cstddef>
#include <cstdlib>
#include <ios>
#include <iostream>
#include <istream>

namespace novikov {
  const char literal[] = "abs";
  const size_t literal_size = 4;
  const size_t alphabet_size = 26;
  const size_t ascii_size = 256;

  size_t countLatin(const char * str, size_t size)
  {
    size_t count = 0;
    char unique[alphabet_size] = {0};

    for (size_t i = 0; i < size; ++i) {
      if (std::isalpha(str[i])) {
        size_t j = 0;
        while (j < alphabet_size && unique[j] != std::tolower(str[i]) && unique[j] != 0) {
          ++j;
        }
        if (unique[j] == 0) {
          unique[j] = std::tolower(str[i]);
          ++count;
        }
      }
    }

    return count;
  }

  size_t countSame(const char * str, size_t size, const char * literal, size_t literal_size)
  {
    size_t count1[ascii_size] = {0};
    size_t count2[ascii_size] = {0};
    size_t sameCount = 0;

    for (size_t i = 0; i < size; ++i) {
      ++count1[static_cast< unsigned char >(str[i])];
    }

    for (size_t i = 0; i < literal_size; ++i) {
      ++count2[static_cast< unsigned char >(literal[i])];
    }

    for (size_t i = 0; i < ascii_size; ++i) {
      if (count1[i] > 0 && count2[i] > 0) {
        sameCount += count1[i] < count2[i] ? count1[i] : count2[i];
      }
    }

    return sameCount;
  }

  void extend(char ** str, size_t size, size_t & capacity)
  {
    char * temp = nullptr;
    if (size == capacity) {
      capacity *= 2;
      temp = static_cast< char * >(malloc(capacity));
      if (temp == nullptr) {
        * str = nullptr;
        return;
      }
      for (size_t i = 0; i < size; ++i) {
        temp[i] = (* str)[i];
      }
      free(* str);
      * str = temp;
    }
  }

  char * getline(std::istream & in, size_t & size)
  {
    bool is_skipws = in.flags() & std::ios_base::skipws;
    if (is_skipws) {
      in >> std::noskipws;
    }

    size_t capacity = 1;
    char * str = static_cast< char * >(malloc(capacity * sizeof(char)));

    if (str == nullptr) {
      return nullptr;
    }
    while (in) {
      extend(& str, size, capacity);
      if (str == nullptr) {
        return nullptr;
      }
      in >> str[size];
      if (in.eof()) {
        break;
      }
      ++size;
    }
    if (in.bad() || size == 0) {
      free(str);
      return nullptr;
    }

    str[size] = '\0';

    if (is_skipws) {
      in >> std::skipws;
    }
    return str;
  }
}

int main()
{
  size_t size = 0;
  char * str = novikov::getline(std::cin, size);

  if (str == nullptr) {
    free(str);
    std::cerr << "Reading failed\n";
    return 1;
  }

  size_t result1 = novikov::countLatin(str, size);
  size_t result2 = novikov::countSame(str, size, novikov::literal, novikov::literal_size);

  std::cout << result1 << "\n";
  std::cout << result2 << "\n";
  free(str);
}
