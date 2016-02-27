#pragma once


#define SUGOI_ERROR -1

#define NULL 0
#define SUGOI_UNUSED(x)  x
#define memzero_(toSet, size) memset(toSet, 0x0, size)

// todo, cplepel:  add the mac version this is microsoft specific
#define forceinline_  __forceinline

#define SUGOI_LINE_NUMBER __LINE__

#define SUGOI_FILE __FILE__

#define SUGOI_PTR_ADD(ptr, offset) \
    ((char *)(ptr) + (offset))

#define SUGOI_PTR_SUB(ptr, offset) \
    ((char *)(ptr) - (offset))

#define dycast_ dynamic_cast
#define stcast_ static_cast
#define recast_ reinterpret_cast
#define cocast_ const_cast

#define UniqueSymbol2_(x) x##__LINE__
#define UniqueSymbol1_(x) UniqueSymbol2_(x)
#define UniqueSymbol_(x) UniqueSymbol1_(x)

#define ConcatenateTokens_(a, b) __ConcatenateTokens2_(a, b)
#define __ConcatenateTokens2_(a, b) a##b

#define SUGOI_OFFSET_OF(type, member) \
	((unsigned) (&((((type *)NULL))->member)))

#define ArrayCount_(token) \
	sizeof(token) / sizeof (token[0])

#define Stringify_(token) __Stringify2_(token)
#define __Stringify2_(token) #token

// All of these output during compilation, can be double clicked to jump to in file
#define SUGOI_TODO(msg) \
    __pragma(message(__FILE__ "(" Stringify_(__LINE__) ") : TODO [ "msg" ]"))

#define SUGOI_WARNING(msg) \
    __pragma(message(__FILE__ "(" Stringify_(__LINE__) ") : WARNING [ "msg" ]")

#define SUGOI_NOTE(msg) \
    __pragma(message(__FILE__ "(" Stringify_(__LINE__) ") : NOTE [ "msg" ]"))

#define SUGOI_DISABLE_WARNING(WARN_ID) \
    __pragma(warning(push)) \
    __pragma(warning( disable : WARN_ID))

#define SUGOI_END_DISABLE() \
    __pragma(warning(pop))

#define SUGOI_PERM_DISABLE(WARNING) \
    __pragma(warning(disable:WARNING))



#define SUGOI_TOKENIZE(PREPEND, SYMBOL) PREPEND##SYMBOL