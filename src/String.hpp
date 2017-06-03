/*!
 * @file String.hpp
 * @brief Utility functions about string
 * @author koturn
 */
#ifndef STRING_HPP
#define STRING_HPP


#include <regex>
#include <sstream>
#include <string>


/*!
 * @brief Split string by one character
 *
 * @tparam F  Function type which equivalent to std::function<void(std::string)>
 * @param [in] str    Target string
 * @param [in] delim  Delimiter character
 * @param [in] f      Callback function
 */
template<typename F>
static inline void
split(const std::string& str, char delim, const F& f) noexcept
{
  std::istringstream iss(str);
  for (std::string token; std::getline(iss, token, delim);) {
    f(std::move(token));
  }
}


/*!
 * @brief Split string by a string
 *
 * @tparam F  Function type which equivalent to std::function<void(std::string)>
 * @param [in] str    Target string
 * @param [in] delim  Delimiter string
 * @param [in] f      Callback function
 */
template<typename F>
static inline void
split(const std::string& str, const char* delim, const F& f) noexcept
{
  std::string::size_type spos = 0, epos, delimlen = std::strlen(delim);
  while ((epos = str.find_first_of(delim, spos)) != std::string::npos) {
    f(str.substr(spos, epos - spos));
    spos = epos + delimlen;
  }
  f(str.substr(spos));
}



/*!
 * @brief Split string by a regex pattern
 *
 * @tparam F  Function type which equivalent to std::function<void(std::string)>
 * @param [in] str  Target string
 * @param [in] ptn  Regex pattern
 * @param [in] f    Callback function
 */
template<typename F>
static inline void
split(const std::string& str, const std::regex& ptn, const F& f) noexcept
{
  for (std::sregex_token_iterator itr = std::sregex_token_iterator(std::begin(str), std::end(str), ptn, -1), end; itr != end; ++itr) {
    f(*itr);
  }
}


/*!
 * @brief Split string by one character
 *
 * @param [in] str    Target string
 * @param [in] delim  Delimiter character
 *
 * @return std::vector<std::string> of splitted strings
 */
static inline std::vector<std::string>
split(const std::string& str, char delim) noexcept
{
  std::vector<std::string> tokens;
  split(str, delim, [&tokens](std::string&& token){
    tokens.push_back(std::move(token));
  });
  return tokens;
}


/*!
 * @brief Split string by a string
 *
 * @param [in] str    Target string
 * @param [in] delim  Delimiter string
 *
 * @return std::vector<std::string> of splitted strings
 */
static inline std::vector<std::string>
split(const std::string& str, const char* delim) noexcept
{
  std::vector<std::string> tokens;
  split(str, delim, [&tokens](std::string&& token){
    tokens.push_back(std::move(token));
  });
  return tokens;
}


/*!
 * @brief Split string by a regex pattern
 *
 * @tparam F  Function type which equivalent to std::function<void(std::string)>
 * @param [in] str  Target string
 * @param [in] ptn  Regex pattern
 *
 * @return std::vector<std::string> of splitted strings
 */
static inline std::vector<std::string>
split(const std::string& str, const std::regex& ptn) noexcept
{
  std::vector<std::string> tokens;
  split(str, ptn, [&tokens](std::string&& token){
    tokens.push_back(std::move(token));
  });
  return tokens;
}


#endif  // STRING_HPP
