#ifndef EXPORTED_H_INCLUDED
#define EXPORTED_H_INCLUDED

// Define EXPORTED for any platform
#ifdef _WIN32
#define EXPORTED  __declspec(dllexport)
#else
# define EXPORTED
#endif


#endif // EXPORTED_H_INCLUDED
