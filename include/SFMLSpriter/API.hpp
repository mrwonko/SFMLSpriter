#ifndef SPRITER_SFML_API_HPP
#define SPRITER_SFML_API_HPP

#ifdef _WIN32
    #ifdef SPRITER_SFML_BUILD
        #define SPRITER_SFML_API __declspec(dllexport)
    #else
        #define SPRITER_SFML_API __declspec(dllimport)
    #endif
#else
    #define SPRITER_SFML_API
#endif

#endif
