#ifndef SFML_SPRITER_API_HPP
#define SFML_SPRITER_API_HPP

#ifdef _WIN32
    #ifdef SFML_SPRITER_BUILD
        #define SFML_SPRITER_API __declspec(dllexport)
    #else
        #define SFML_SPRITER_API __declspec(dllimport)
    #endif
#else
    #define SFML_SPRITER_API
#endif

#endif
