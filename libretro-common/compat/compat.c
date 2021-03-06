/* Copyright  (C) 2010-2015 The RetroArch team
 *
 * ---------------------------------------------------------------------------------------
 * The following license statement only applies to this file (compat.c).
 * ---------------------------------------------------------------------------------------
 *
 * Permission is hereby granted, free of charge,
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <compat/strl.h>
#include <compat/posix_string.h>

#ifndef HAVE_STRL

/* Implementation of strlcpy()/strlcat() based on OpenBSD. */

size_t strlcpy(char *dest, const char *source, size_t size)
{
   size_t src_size = 0;
   size_t n = size;

   if (n)
      while (--n && (*dest++ = *source++)) src_size++;

   if (!n)
   {
      if (size) *dest = '\0';
      while (*source++) src_size++;
   }

   return src_size;
}

size_t strlcat(char *dest, const char *source, size_t size)
{
   size_t len = strlen(dest);
   dest += len;

   if (len > size)
      size = 0;
   else
      size -= len;

   return len + strlcpy(dest, source, size);
}

#endif

#ifdef _WIN32

#undef strcasecmp
#undef strdup
#undef isblank
#undef strtok_r
#include <ctype.h>
#include <stdlib.h>
#include <stddef.h>
#include <compat/strl.h>

#include <string.h>

int rarch_strcasecmp__(const char *a, const char *b)
{
   while (*a && *b)
   {
      int a_ = tolower(*a);
      int b_ = tolower(*b);
      if (a_ != b_)
         return a_ - b_;

      a++;
      b++;
   }

   return tolower(*a) - tolower(*b);
}

char *rarch_strdup__(const char *orig)
{
   size_t len = strlen(orig) + 1;
   char *ret = (char*)malloc(len);
   if (!ret)
      return NULL;

   strlcpy(ret, orig, len);
   return ret;
}

int rarch_isblank__(int c)
{
   return (c == ' ') || (c == '\t');
}

char *rarch_strtok_r__(char *str, const char *delim, char **saveptr)
{
   char *first = NULL;
   if (!saveptr || !delim)
      return NULL;

   if (str)
      *saveptr = str;

   do
   {
      char *ptr = NULL;
      first = *saveptr;
      while (*first && strchr(delim, *first))
         *first++ = '\0';

      if (*first == '\0')
         return NULL;

      ptr = first + 1;

      while (*ptr && !strchr(delim, *ptr))
         ptr++;

      *saveptr = ptr + (*ptr ? 1 : 0);
      *ptr     = '\0';
   } while (strlen(first) == 0);

   return first;
}

#endif

