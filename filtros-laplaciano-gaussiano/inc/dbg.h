#ifndef __DBG_H__
#define __DBG_H__

/* Ref. http://c.learncodethehardway.org/book/ex20.html */

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#endif // __DBG_H_INCLUDED__
