/**
 * Read the content of the memory allocation csv and produce a report.
 */

/**
 * Copyright: Willy Clarke
 * License  : MIT (see below)
 */

#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

namespace
{
/**
 * @brief Store a combination between an allocation and deallocation of memory.
 */
struct alloc_data
{
   size_t BytesAllocated{};
   unsigned long long MemoryAddr{};
};

/**
 * A function to split a string by a given delimiter
 */
std::vector<std::string> split(const std::string &s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}
};  // end of anonymous namespace

int main(int const argc, char const *argv[])
{
   if (argc < 2)
   {
      std::cerr << std::string(argv[0]) << " ==> Error : csv filename needed." << std::endl;

      return EXIT_FAILURE;
   }

   if (("--help" == std::string(argv[1])) || ("-h" == std::string(argv[1])))
   {
      std::cerr << "Usage\n" << std::string(argv[0]) << " filename\n" << std::endl;
      std::cout
          << "Expected file content:\nM,100,64\nF,100\n\nThis means that 64 bytes where allocated at address 100\n"
          << "and that the memory at address 100 was freed.\n" << std::endl;

      return EXIT_SUCCESS;
   }

   std::string FileName{};

   try
   {
      std::string FileNameTst{argv[1]};
      FileName = FileNameTst;
   }
   catch (...)
   {
      std::cerr << std::string(argv[0]) << ": failed to get filename." << std::endl;
      return EXIT_FAILURE;
   }

   // Open the file
   std::ifstream ifsFile(FileName);

   if (!ifsFile.is_open())
   {
      std::cerr << "Could not open the file " << FileName << "." << std::endl;
      return EXIT_FAILURE;
   }

   std::map<unsigned long long, alloc_data> mapAllocs{};

   std::string Line{};
   // Read line by line
   while (std::getline(ifsFile, Line))
   {
      // Split each line by commas
      std::vector<std::string> Tokens = split(Line, ',');

      // Now, tokens[0], tokens[1], tokens[2] hold the individual values

      bool StoreAllocation{};
      bool RemoveAllocation{};
      alloc_data AllocData{};

      /**
       * Go through the tokens for each line.
       * Figure out the allocations and the free of memory.
       * When an allocation is found the map is updated.
       * When a free is found the entry in the map is deleted.
       */
      for (auto const &Token : Tokens)
      {
         /**
          * The second thing to be stored is the number of bytes.
          */
         if (AllocData.MemoryAddr)
         {
            AllocData.BytesAllocated = std::stoull(Token);
         }

         /**
          * The first thing to be stored is the Memory address.
          */
         if (StoreAllocation && !AllocData.MemoryAddr)
         {
            AllocData.MemoryAddr = std::stoull(Token);
         }

         /**
          * Delete the entry when a free has been detected.
          */
         if (RemoveAllocation)
         {
            auto It = mapAllocs.find(std::stoull(Token));
            if (It != mapAllocs.end())
            {
               mapAllocs.erase(It);
            }
            else
            {
               std::cerr << "Found free at memory address " << Token << " without allocation" << std::endl;
            }
         }

         /**
          * Found that a memory allocation has been done.
          */
         if ("M" == Token)
         {
            StoreAllocation = true;
         }

         /**
          * Found that the memory was freed.
          */
         if ("F" == Token)
         {
            RemoveAllocation = true;
         }
      }

      if (AllocData.MemoryAddr)
      {
         mapAllocs[AllocData.MemoryAddr] = AllocData;
      }
   }

   // Close the file
   ifsFile.close();

   if (!mapAllocs.empty())
   {
      std::cout << "Number of Allocations that was not freed: " << mapAllocs.size() << std::endl;
      std::cout << "The following allocations were not freed:" << std::endl;
   }
   else
   {
      std::cout << "No memory leaks found based on input from " << FileName << std::endl;
   }

   for (const auto &[key, value] : mapAllocs)
   {
      std::cout << '[' << key << "] = " << value.BytesAllocated << std::endl;
   }

   return EXIT_SUCCESS;
}
/**
* The MIT License (MIT)
Copyright © 2023 <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the “Software”), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Ref: https://mit-license.org
*/
