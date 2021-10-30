#pragma once

/**
 * Se definido, indica que está no modo de compilação para testes unitários.
*/
#define TEST_MODE 1

/**
 * Se definido, indica que está no modo de compilação para integração com o Electron
*/
//#define PRODUCTION_MODE 1

// Tudo que deve aparecer somente no teste unitário
#ifdef TEST_MODE
#define TEST(T) T;
#else
#define TEST(T)
#endif

// Tudo que deve aparecer somente na integração com o Electron
#ifdef PRODUCTION_MODE
#define PRODUCTION(T) T;
#else
#define PRODUCTION(T)
#endif
