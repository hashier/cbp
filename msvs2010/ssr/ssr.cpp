// Simple Search and Replace Tool Command Line Tool

// Author: Ross MacGregor
// Date: July, 2005
// License: Freeware, Unrestricted Use

#include "stdafx.h"

#include <cassert>
#include <fstream>
#include <string>
#include <sstream>

int replace(int occurrence, std::string const & search, std::string const & replace, std::string & s);

int _tmain(int argc, _TCHAR* argv[])
{
  if( argc < 5 || argc > 6 )
  {
      std::cerr <<  "Usage: ssr <occurrence> <search_text> <replace_text>\n"
                    "           <input_file> [<output_file>]\n"
                    "  <occurrence> - An integer specifying which occurrence to replace,\n"
                    "                 use 0 for all.\n"
                    "  Use '' to represent \" in search and replace text.\n"
                    "  Use /SSR_QUOTE/ to represent ' in search and replace text.\n"
                    "  Use /SSR_NL/ to represent a new line in search and replace text.\n"
                    "  Use /SSR_TAB/ to represent a tab in search and replace text.\n"
                    "\n"
                    "Version: 1.2\n";
      return 1;
  }

  int occurrence(atoi(argv[1]));
  std::string search_text(argv[2]);
  std::string replace_text(argv[3]);
  std::string input_file_name(argv[4]);
  std::string output_file_name;
  
  bool const inplace_edit = argc == 5;
  bool const replace_all = occurrence == 0;

  if( inplace_edit )
  {
    output_file_name = input_file_name;
    output_file_name += ".ssr";
  }
  else
  {
    output_file_name = argv[5];
  }

  replace(0, "''", "\"", search_text);
  replace(0, "/SSR_QUOTE/", "'", search_text);
  replace(0, "/SSR_NL/", "\n", search_text);
  replace(0, "/SSR_TAB/", "\t", search_text);

  replace(0, "''", "\"", replace_text);
  replace(0, "/SSR_QUOTE/", "'", replace_text);
  replace(0, "/SSR_NL/", "\n", replace_text);
  replace(0, "/SSR_TAB/", "\t", replace_text);
 
  std::ifstream input_file(input_file_name.c_str());
  std::ofstream output_file(output_file_name.c_str());
  std::string line;

  if( !input_file )
  {
    std::cerr << "Error opening file, "
              << input_file_name
              << "\n";
    return 2;
  }

  if( !output_file )
  {
    std::cerr << "Error opening file, "
              << output_file_name
              << "\n"; 
    return 3;
  }

  std::stringstream s;
  while( std::getline(input_file, line) )
  {
    s << line << std::endl;
  }
  std::string ss = s.str();
  replace(0, search_text, replace_text, ss);
  output_file << ss;

  //BUGGY - KANN KEINE \n SUCHEN
  //while( std::getline(input_file, line) )
  //{
  //  if( replace_all )
  //  {
  //    replace(0, search_text, replace_text, line);
  //  }
  //  else if( occurrence > 0 )
  //  {
  //    int found = replace(occurrence, search_text, replace_text, line);
  //    occurrence -= found;
  //  }
  //  output_file << line;
  //  output_file << "\n";
  //}

  input_file.close();
  output_file.close();

  if( inplace_edit )
  {
    if( remove(input_file_name.c_str()) != 0 )
    {
      std::cerr << "Error deleting file, "
                << input_file_name
                << "\n"; 
      remove(output_file_name.c_str());
      return 4;
    }

    if( rename(output_file_name.c_str(), input_file_name.c_str()) != 0 )
    {
      std::cerr << "Error renaming file, "
                << output_file_name
                << " to "
                << input_file_name
                << "\n"; 
      return 5;
    }
  }

	return 0;
}

int replace(int occurrence, std::string const & search, std::string const & replace, std::string & s)
{
  assert(occurrence >= 0);

  int found = 0;

  std::string::size_type pos = s.find(search);
  while( pos != std::string::npos )
  {
    ++found;
    
    if( found == occurrence || occurrence == 0 )
    {
      s.replace(pos, search.length(), replace);
    }

    pos = s.find(search, pos + replace.length());
  }

  return found;
}
