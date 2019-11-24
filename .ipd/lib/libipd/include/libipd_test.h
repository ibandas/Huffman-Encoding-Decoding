#pragma once

#include <stdbool.h>

// CHECK(A) checks that `A` evaluates to true.
// (Returns value of `A` in case you want it.)
//
// Example:
//
//     CHECK( isinf(1.0 / 0.0) );
//     CHECK( isnan(0.0 / 0.0) );
//
// The parentheses around `A` in the first argument to `really_check` ensure
// that if `A` contains a comma then it isn’t considered as separate arguments
// to `really_check`. The `#A` for the next argument turns into the code that
// you write for `A` but turned into a string literal. The next two arguments
// are filled in by the C preprocessor with the filename and line number where
// `CHECK` is used. A function can't find out what file it was used it, but
// a C preprocessor macro can, and then it can pass that information to a
// function to do the real work. The function is defined below.
#define CHECK(A)            really_check((A), #A, __FILE__, __LINE__)


// CHECK_CHAR(A, B)    checks that A and B evaluate to the same  char.
// CHECK_INT(A, B)     checks that A and B evaluate to the same  long.
// CHECK_UINT(A, B)    checks that A and B evaluate to the same  unsigned long.
// CHECK_DOUBLE(A, B)  checks that A and B evaluate to the same  double.
// CHECK_STRING(A, B)  checks that A and B evaluate to the same  string.
//   (CHECK_STRING checks for NULLs and considers them errors.)
//
// Examples:
//
//   char const *s = "hello, world";
//
//   CHECK_CHAR( s[5], ',' );
//   CHECK_UINT( strlen(s), 12 );
//   CHECK_STRING( s + 2, "llo, world" );
//
#define CHECK_CHAR(...)     CHECK_WITH(char, __VA_ARGS__)
#define CHECK_INT(...)      CHECK_WITH(int, __VA_ARGS__)
#define CHECK_UINT(...)     CHECK_WITH(uint, __VA_ARGS__)
#define CHECK_DOUBLE(...)   CHECK_WITH(double, __VA_ARGS__)
#define CHECK_STRING(...)   CHECK_WITH(string, __VA_ARGS__)


// Helper for dispatching type-specific checks above to functions below.
// Note that `T` stands for “tag,” not “type,” since it might not be a type.
#define CHECK_WITH(T, A, B) really_check_##T((A),(B),#A,#B,__FILE__,__LINE__)


// Helper function used by `CHECK` macro above.
bool really_check(
        bool condition,         // did the check pass?
        char const* assertion,  // source code of condition checked
        char const* file,       // file name where `CHECK` was used
        int line);              // line number in `file`

// Helper function used by `CHECK_INT` macro above.
bool really_check_int(
        long have,              // number we got
        long want,              // number we expected
        char const* expr_have,  // source expression producing `have`
        char const* expr_want,  // source expression producing `want`
        char const* file,
        int line);

// Helper function used by `CHECK_UINT` macro above.
bool really_check_uint(
        unsigned long have,
        unsigned long want,
        char const* expr_have,
        char const* expr_want,
        char const* file,
        int line);

// Helper function used by `CHECK_DOUBLE` macro above.
bool really_check_double(
        double have,
        double want,
        char const* expr_have,
        char const* expr_want,
        char const* file,
        int line);

// Helper function used by `CHECK_CHAR` macro above.
bool really_check_char(
        char have,
        char want,
        char const* expr_have,
        char const* expr_want,
        char const* file,
        int line);

// Helper function used by `CHECK_STRING` macro above.
bool really_check_string(
        char const* have,
        char const* want,
        char const* expr_have,
        char const* expr_want,
        char const* file,
        int line);

