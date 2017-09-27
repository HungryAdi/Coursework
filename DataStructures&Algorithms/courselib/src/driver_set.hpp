#ifndef DRIVER_SET_HPP_
#define DRIVER_SET_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include "ics46goody.hpp"
#include "ics_exceptions.hpp"
#include "array_set.hpp"


namespace ics {

typedef ics::ArraySet<std::string> SetType;

class DriverSet {
  public:
    DriverSet(){process_commands("");}

  private:
    SetType s;

    SetType prompt_set(std::string preface, std::string message = "  Enter element for s2") {
      SetType s2;
      for (;;) {
        std::string e = ics::prompt_string(preface + message + "(QUIT to quit)");
        if (e == "QUIT")
          break;
        s2.insert(e);
      }
      return s2;
    }

    std::string menu_prompt (std::string preface) {
      std::cout << std::endl << std::endl;
      std::cout << preface+"set s = " << s.str() << std::endl;
      std::cout << preface+"Mutators           Accessors              General" << std::endl;
      std::cout << preface+"  i  - insert        m  - empty             lf - load from file"    << std::endl;
      std::cout << preface+"  I  - insert_all    s  - size              l{ - load from {}"      << std::endl;
      std::cout << preface+"  e  - erase         c  - contains          it - iterator commands" << std::endl;
      std::cout << preface+"  E  - erase_all     C  - contains          q  - quit"              << std::endl;
      std::cout << preface+"  x  - clear         <  - <<"        << std::endl;
      std::cout << preface+"  R  - retain_all    r  - relations" << std::endl;
      std::cout << preface+"  =  - ="      << std::endl;

      std::string allowable[] = {"i","I","e","E","x","R","=","m","s","c","C","<","r","lf","l{","it","q",""};
      std::cout << std::endl << std::endl;
      return ics::prompt_string(preface+"Enter set command","",allowable);
    }

  void process_iterator_commands(SetType& s, std::string preface) {
    std::string allowable[] = {"<","e","*","+","i","c","*a","ea","f","q",""};
    SetType::Iterator i = s.begin();
    for (;;)
      try {
        std::cout << std::endl;
        std::cout << preface+"i = " << i.str() << std::endl;
        std::string i_command = ics::prompt_string(preface+
            "Enter iterator command(<[<]/e[rase]/*/+[+i]/i[++]/c[ommands]/*a[ll]/ea[ll]/f[or]/q[uit])","",allowable);
        if (i_command == "<")
          std::cout << preface+"  << = " << i << std::endl;
        else if (i_command == "e")
          std::cout << preface+"  erase = " << i.erase() << std::endl;
        else if (i_command == "*")
          std::cout << preface+"  * = " << *i << std::endl;
        else if (i_command == "+")
          std::cout << preface+"  ++i returned = " << ++i << std::endl;
        else if (i_command == "i")
          std::cout << preface+"  i++ returned = " << i++ << std::endl;
        else if (i_command == "c")
          process_commands(preface);
        else if (i_command == "*a") {
          std::cout << preface+"  initially i = " << i << std::endl;
          for (; i != s.end(); ++i)
            std::cout << preface+"  *(all) = " << *i << std::endl;
          std::cout << preface+"  finally i = " << i << std::endl;
        }
        else if (i_command == "ea") {
          std::cout << preface+"  initially i = " << i << std::endl;
         for (; i != s.end(); ++i)
            std::cout << preface+"  erase(all) = " << i.erase() << std::endl;
          std::cout << preface+"  finally i = " << i << std::endl;
        }
        else if (i_command == "f") {
          for (auto v : s)
            std::cout << preface+"  *(all) = " << v << std::endl;
        }
        else if (i_command == "q")
          break;

      } catch (ics::IcsError& e) {
        std::cout << preface+"  " << e.what() << std::endl;
      }
  };


  void process_commands(std::string preface) {
    for (;;) try {
      std::string command = menu_prompt(preface);

      if (command == "i") {
        std::string e = ics::prompt_string(preface+"  Enter element to add");
        std::cout << preface+"  insert = " << s.insert(e) << std::endl;
      }

      else if (command == "I") {
        SetType s2(prompt_set(preface));
        std::cout << "  insert = " << s.insert_all(s2) << std::endl;;
      }

      else if (command == "e") {
        std::string e = ics::prompt_string(preface+"  Enter element to erase");
        std::cout << preface+"  erase = " << s.erase(e) << std::endl;
      }

      else if (command == "E") {
        SetType s2(prompt_set(preface));
        std::cout << "  erase = " << s.erase_all(s2) << std::endl;;
      }

      else if (command == "x")
        s.clear();

      else if (command == "R") {
        SetType s2(prompt_set(preface));
        std::cout << "  retain = " << s.retain_all(s2) << std::endl;
      }

      else if (command == "=") {
        SetType s2(prompt_set(preface));
        s = s2;
        std::cout << "  s now = " << s << std::endl;
      }

      else if (command == "m")
        std::cout << preface+"  empty = " << s.empty();

      else if (command == "s")
        std::cout << preface+"  size = " << s.size() << std::endl;


      else if (command == "c") {
        std::string e = ics::prompt_string(preface+"  Enter element to check");
        std::cout << preface+"  contains = " << s.contains(e) << std::endl;
      }

      else if (command == "C") {
        SetType s2(prompt_set(preface));
        std::cout << "  contains = " << s.contains_all(s2) << std::endl;
      }

      else if (command == "<")
        std::cout << preface+"  << = " << s << std::endl;

      else if (command == "r") {
        std::cout << preface+"  s == s = " << (s == s) << std::endl;
        std::cout << preface+"  s != s = " << (s != s) << std::endl;
        std::cout << preface+"  s <= s = " << (s <= s) << std::endl;
        std::cout << preface+"  s <  s = " << (s <  s) << std::endl;
        std::cout << preface+"  s >  s = " << (s >  s) << std::endl;
        std::cout << preface+"  s >= s = " << (s >= s) << std::endl;

        SetType s2(prompt_set(preface));
        std::cout << preface+"  s = " << s << " ?? s2 = " << s2 << std::endl;
        std::cout << preface+"  s == s2 = " << (s == s2) << std::endl;
        std::cout << preface+"  s != s2 = " << (s != s2) << std::endl;
        std::cout << preface+"  s <= s2 = " << (s <= s2) << std::endl;
        std::cout << preface+"  s <  s2 = " << (s <  s2) << std::endl;
        std::cout << preface+"  s >  s2 = " << (s >  s2) << std::endl;
        std::cout << preface+"  s >= s2 = " << (s >= s2) << std::endl;
      }

      else if (command == "lf") {
        std::ifstream in_set;
        ics::safe_open(in_set,preface+"  Enter file name to read", "loadset.txt");
        std::string e;
        while (getline(in_set,e))
          s.insert(e);
        in_set.close();
      }

      else if (command == "l{")
        s = SetType({"c","b","d","b","e","a","c"});

      else if (command == "it")
        process_iterator_commands(s, "it:  "+preface);

      else if (command == "q")
        break;

      else
        std::cout << preface+"\""+command+"\" is unknown command" << std::endl;

    } catch (ics::IcsError& e) {
      std::cout << preface+"  " << e.what() << std::endl;
    }

  };

};

}

#endif /* DRIVER_SET_HPP_ */
