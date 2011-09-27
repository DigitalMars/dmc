#define GC_VERSION_MAJOR 6
#define GC_VERSION_MINOR 0
#define GC_ALPHA_VERSION 8

#   define GC_NOT_ALPHA 0xff

#ifndef GC_NO_VERSION_VAR

unsigned GC_version = ((GC_VERSION_MAJOR << 16) | (GC_VERSION_MINOR << 8) | GC_ALPHA_VERSION);

#endif /* GC_NO_VERSION_VAR */ 
