#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "ics46goody.hpp"

namespace ics {

std::vector<std::string> split(const std::string& str, const std::string& pat){
  std::string s = str;
  if(!s.empty() && s[s.length()-1] == '\r')  //Remove CR before LF from PC-compatible files
      s.erase( s.length()-1, 1);
  std::vector<std::string> answer;
  int pos2;
  for (int pos = 0; ; pos=pos2+pat.size()) {
    pos2 = s.find(pat,pos);
    if (pos2 == -1) {
      if (s.substr(pos,s.size()+1-pos) != "")
        answer.push_back(s.substr(pos,s.size()+1-pos));
      break;
    }
    if (pos == 0 || pos2 != pos+int(pat.size())-1)  //skip multiple pat occurrences
      if (s.substr(pos,pos2-pos) != "")
        answer.push_back(s.substr(pos,pos2-pos));
  }
  return answer;
}


std::string join(const std::vector<std::string>& s, const std::string& pat){
  std::string answer;
  for (unsigned int i = 0; i<s.size(); ++i)
    answer.append(s[i] + (i != s.size()-1 ? pat : ""));
  return answer;
}


std::string prompt_string(std::string prompt,
                          std::string default_value,
                          std::string allowable[]) {
  std::string answer;
  for (;;) {
    std::cout << prompt;
    if (default_value != "")
      std::cout << "["+default_value+"]";
    std::cout << ": ";
    getline(std::cin,answer);
    if (answer == "" && default_value != "")
      answer = default_value;
    if (allowable == NULL)
      return answer;
    for (int i=0; allowable[i] != ""; ++i)
      if (answer == allowable[i])
        return answer;
    std::cout << "  " << answer << " illegal: string not allowed; re-enter" << std::endl;
  }

  return answer;  //For compiler
}


bool prompt_bool(std::string prompt,
                 int default_value) {
  std::string def = (default_value == 0 ? "[false]" : (default_value == 1 ? "[true]" : ""));
  for (;;) {
    std::string answer = ics::prompt_string(prompt+def+"?");
    if (answer == "" && (default_value == 0 || default_value == 1))
      return default_value;
    if (answer == "true")
      return true;
    if (answer == "false")
      return false;
    std::cout << "  " << answer << " illegal: boolean not correct; re-enter true or false" << std::endl;
  }
  return false;  //For compiler
}


int prompt_int(std::string prompt, int default_value) {
  int no_default = std::numeric_limits<int>::max();
  for (;;) {
    try {
      std::ostringstream s_def;
      if (default_value != no_default)
        s_def << default_value;

      std::string answer = ics::prompt_string(prompt + (default_value == no_default ? "" : "["+s_def.str()+"]"));

      int v;
      if (answer == "")
        if (default_value != no_default)
          v = default_value;
        else {
          std::cout << "  illegal: must supply a value; re-enter" << std::endl;
          continue;
        }
      else {
        char* p;
        strtol(answer.c_str(), &p, 10) ;

        if (*p != 0) {
          std::cout << "  " << answer << " illegal: non-integer entered; re-enter" << std::endl;
          continue;
        }
          v = atoi(answer.c_str());
      }
      return v;
    } catch (const std::exception& e) {
      std::cout << "illegal" << std::endl;
    }
  }
  return 0;  //For compiler
}


void safe_open(std::ifstream& f, const std::string& prompt, const std::string& default_name) {
  for (;;) {
    std::string file_name = prompt_string(prompt,default_name);
    f.open(file_name.c_str());
    if (!f.fail())
      break;
    std::cout << "Opening " << file_name << " failed." << std::endl;
  }
}

int rand_range(int low, int high) {
  return low + std::rand() % (high-low+1);
}


}
