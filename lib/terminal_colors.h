#ifndef _TERMINAL_COLORS_
#define _TERMINAL_COLORS_

#define STR(s) #s
#define XSTR(s) STR(s)

/* FOREGROUND */
#define RST "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

#define BLD "\x1B[1m"
#define UND "\x1B[4m"

// #define FGRN(x) KGRN x RST
// #define FYEL(x) KYEL x RST
// #define FBLU(x) KBLU x RST
// #define FMAG(x) KMAG x RST
// #define FCYN(x) KCYN x RST

#define BLKIO(x) BLD KBLU << x << RST
#define WHTIO(x) BLD KWHT << x << RST

#define EMPHIO(x) KCYN << x << RST

#endif /* _TERMINAL_COLORS_ */
