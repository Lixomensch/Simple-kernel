#include "string.h"
#include <stddef.h>

void *memset(void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char *)s;
    while (n--)
    {
        *p++ = (unsigned char)c;
    }
    return s;
}

void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *sr = (const unsigned char *)src;
    while (n--)
    {
        *d++ = *sr++;
    }
    return dest;
}

size_t strlen(const char *s)
{
    size_t len = 0;
    while (s[len])
    {
        len++;
    }
    return len;
}

char *strcpy(char *dest, const char *src)
{
    char *ret = dest;
    while ((*dest++ = *src++));
    return ret;
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int strncmp(const char *s1, const char *s2, unsigned int n)
{
    while (n && *s1 && (*s1 == *s2))
    {
        ++s1;
        ++s2;
        --n;
    }
    if (n == 0)
    {
        return 0;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

void itoa(int num, char *str) {
  int i = 0;
  int is_negative = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return;
  }

  if (num < 0) {
    is_negative = 1;
    num = -num;
  }

  while (num != 0) {
    int rem = num % 10;
    str[i++] = rem + '0';
    num = num / 10;
  }

  if (is_negative) {
    str[i++] = '-';
  }

  str[i] = '\0';

  int start = 0;
  int end = i - 1;
  while (start < end) {
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    start++;
    end--;
  }
}
