/* From CrowOS */

#include "stdlib.h"
#include "string.h"
#include "unistd.h"

typedef long Align;

union header 
{
  struct 
  {
    union header *ptr;
    size_t size;
  } s;

  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;

void free(void *ap) 
{
  if (ap == NULL)
  {
    return;
  }

  Header *bp, *p;

  bp = (Header *)ap - 1;

  for (p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if (bp + bp->s.size == p->s.ptr) {
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if (p + p->s.size == bp) {
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}

static Header *morecore(size_t nu) {
  char *p;
  Header *hp;

  if (nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if (p == (char *)-1)
    return 0;
  hp = (Header *)p;
  hp->s.size = nu;
  free((void *)(hp + 1));
  return freep;
}

void *malloc(size_t nbytes) {
  Header *p, *prevp;
  size_t nunits;

  nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1;
  if ((prevp = freep) == 0) {
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for (p = prevp->s.ptr;; prevp = p, p = p->s.ptr) {
    if (p->s.size >= nunits) {
      if (p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void *)(p + 1);
    }
    if (p == freep)
      if ((p = morecore(nunits)) == 0)
        return 0;
  }
}

void *calloc(size_t nmemb, size_t size) {
  void *data = malloc(nmemb * size);
  if (data == NULL)
    return NULL;
  memset(data, 0, nmemb * size);
  return data;
}

void *realloc(void *ptr, size_t size) {
  if (ptr == NULL)
    return malloc(size);
  if (size == 0) {
    free(ptr);
    return NULL;
  }

  Header *header = (Header *)ptr - 1;
  if (size == header->s.size)
    return ptr; // no resize

  void *new_data = malloc(size);
  // Copy the minimum from the size before and requested size
  memcpy(new_data, ptr, size < header->s.size ? size : header->s.size);
  return new_data;
}

int abs(int x)
{
    return (x < 0) ? -x : x;
}

int atoi(const char *s)
{
    int sign = 1;
    int result = 0;

    // pular espaços
    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\v' || *s == '\f')
    {
        s++;
    }

    // sinal
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    // números
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10 + (*s - '0');
        s++;
    }

    return result * sign;
}

double atof(const char *s)
{
    double result = 0.0;
    double frac = 0.0;
    double div = 1.0;
    int sign = 1;

    /* pula espaços */
    while (*s == ' ' || *s == '\t' || *s == '\n' ||
           *s == '\r' || *s == '\v' || *s == '\f')
        s++;

    /* sinal */
    if (*s == '-')
    {
        sign = -1;
        s++;
    }
    else if (*s == '+')
    {
        s++;
    }

    /* parte inteira */
    while (*s >= '0' && *s <= '9')
    {
        result = result * 10.0 + (*s - '0');
        s++;
    }

    /* parte decimal */
    if (*s == '.')
    {
        s++;

        while (*s >= '0' && *s <= '9')
        {
            frac = frac * 10.0 + (*s - '0');
            div *= 10.0;
            s++;
        }

        result += frac / div;
    }

    return result * sign;
}

typedef unsigned long long uint64;

uint64 __divdi3(uint64 num, uint64 den)
{
    uint64 res = 0;
    uint64 bit = 1;

    while (den <= num && (den << 1) > den)
    {
        den <<= 1;
        bit <<= 1;
    }

    while (bit)
    {
        if (num >= den)
        {
            num -= den;
            res |= bit;
        }

        den >>= 1;
        bit >>= 1;
    }

    return res;
}
