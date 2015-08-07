#ifndef _TOSTRINGCONVERTER_H_
#define _TOSTRINGCONVERTER_H_

#include <string>

#ifndef _Bool
#define _Bool bool
#endif

namespace cling {

  // void pointer
  std::string printValue(void *ptr);

  // Bool
  std::string printValue(bool val);

  // Chars
  std::string printValue(char val);

  std::string printValue(signed char val);

  std::string printValue(unsigned char val);

  // Ints
  std::string printValue(short val);

  std::string printValue(unsigned short val);

  std::string printValue(int val);

  std::string printValue(unsigned int val);

  std::string printValue(long val);

  std::string printValue(unsigned long val);

  std::string printValue(long long val);

  std::string printValue(unsigned long long val);

  // Reals
  std::string printValue(float val);

  std::string printValue(double val);

  std::string printValue(long double val);

  // Char pointers
  std::string printValue(const char *const val);

  std::string printValue(char *val);

  // std::string
  std::string printValue(const std::string &val);

  // Arrays
  template<typename T, size_t N>
  std::string printValue(const T (&obj)[N]) {
    std::string str = "{ ";

    for(int i = 0; i < N; ++i) {
      str = str + printValue(obj[i]);
      if (i < N) str = str + ", ";
    }

    return str + " }";
  }

  // Maps declaration
  template<typename CollectionType>
  auto printValue_impl(const CollectionType &obj, short)
      -> decltype(
          ++(obj.begin()), obj.end(),
          obj.begin()->first, obj.begin()->second,
          std::string());

  // Collections like vector, set, deque etc. declaration
  template<typename CollectionType>
  auto printValue_impl(const CollectionType &obj, int)
      -> decltype(
          ++(obj.begin()), obj.end(),
          *(obj.begin()),
          std::string());

  // Fallback for collections declaration
  template<typename CollectionType>
  std::string printValue_impl(const CollectionType &obj, long);

  // Collections entry function
  template<typename CollectionType>
  auto printValue(const CollectionType &obj)
  -> decltype(printValue_impl(obj, 0), std::string()) {
    return printValue_impl(obj, (short)0);  // short -> int -> long = priority order
  }

  // Maps
  template<typename CollectionType>
  auto printValue_impl(const CollectionType &obj, short)
      -> decltype(
          ++(obj.begin()), obj.end(),
          obj.begin()->first, obj.begin()->second,
          std::string())
  {
    std::string str = "{ ";

    auto iter = obj.begin();
    auto iterEnd = obj.end();
    while (iter != iterEnd) {
      str = str + printValue(iter->first);
      str = str + " => ";
      str = str + printValue(iter->second);
      ++iter;
      if (iter != iterEnd) str = str + ", ";
    }

    return str + " }";
  }

  // Collections like vector, set, deque etc.
  template<typename CollectionType>
  auto printValue_impl(const CollectionType &obj, int)
      -> decltype(
          ++(obj.begin()), obj.end(),
          *(obj.begin()),
          std::string())
  {
    std::string str = "{ ";

    auto iter = obj.begin();
    auto iterEnd = obj.end();
    while (iter != iterEnd) {
      str = str + printValue(*iter);
      ++iter;
      if (iter != iterEnd) str = str + ", ";
    }

    return str + " }";
  }

  // Fallback for collections
  template<typename CollectionType>
  std::string printValue_impl(const CollectionType &obj, long) {
    return printValue((void *) &obj);
  }

}

#endif
