/**
 * Code for override of operator new/delete and malloc/free
 * License: MIT, see bottom of file.
 * Copyright: Willy Clarke.
 * Many thanks to J. Sorber for teaching these things.
 *
 * Usage:
 * The easiest way of overriding is to include this cpp file into the
 * compilation unit that houses main.
 */

#ifdef FLUFFY_OVR_MEMALLOC

#include <dlfcn.h>

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

#warning "Memory allocation is overriden for test purpose"

typedef void *(*malloc_like_function)(size_t);
typedef void (*free_like_function)(void *);

static malloc_like_function sysmalloc = nullptr;
static free_like_function sysfree = nullptr;
static bool Init = false;
static FILE *fp = nullptr;
static char const *ptrLogFileName = "memalloc.log";

void InitCheck()
{
   if (!Init)
   {
      sysmalloc = (malloc_like_function)dlsym(RTLD_NEXT, "malloc");
      sysfree = (free_like_function)dlsym(RTLD_NEXT, "free");
      fp = fopen(ptrLogFileName, "w");
      Init = true;
   }
}

void *malloc(size_t size)
{
   InitCheck();
   void *ptr = sysmalloc(size);
   fprintf(fp, "M,%lu,%lu\n", (uintptr_t)ptr, size);
   return ptr;
}

void free(void *ptr)
{
   InitCheck();
   fprintf(fp, "F,%lu\n", (uintptr_t)ptr);
   sysfree(ptr);
}

void *operator new(size_t size)
{
   void *p = malloc(size);
   if (!p)
   {
      throw std::bad_alloc();
   }
   return p;
}

void operator delete(void *p) noexcept
{
   free(p);
}
#endif

// int main()
// {
// int *pInt = (int *)malloc(sizeof(int));
// std::cout << "Hello world. Log file : " << ptrLogFileName << std::endl;
// if (pInt)
// {
//    *pInt = 42;
//    std::cout << "The integer value is " << *pInt << std::endl;
//    free(pInt);
// }
//
// int *pInt2 = new int;
// delete (pInt2);
//
// int *pInt3 = new int[10];
// delete[] pInt3;

//    struct my_obj
//    {
//       int X{};
//    };
//
//    auto pInt4 = std::make_shared<my_obj>();
//    return 0;
// }

/**
* The MIT License (MIT)
Copyright © 2023 <copyright holders>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the “Software”), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Ref: https://mit-license.org
*/
