#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <limits>
#include <cmath>
#include <string>
#include <string_view>
#include <algorithm>
#include <fstream>
#include <sstream>

#pragma warning(push, 0)
#include <FL/fl_draw.H>
#pragma warning(pop)

#if defined(__unix__)
#define __X11__
#endif

#ifdef _WIN32
#define DIR_SEP "\\"
#else
#define DIR_SEP "/"
#endif

#ifdef __APPLE__
#define CONTROL_KEY "\xE2\x8C\x83" // UTF-8 encoding of U+2303 "UP ARROWHEAD"
#define ALT_KEY "\xE2\x8C\xA5" // UTF-8 encoding of U+2325 "OPTION KEY"
#define SHIFT_KEY "\xE2\x87\xA7" // UTF-8 encoding of U+21E7 "UPWARDS WHITE ARROW"
#define COMMAND_KEY "\xE2\x8C\x98" // UTF-8 encoding of U+2318 "PLACE OF INTEREST SIGN"

#define COMMAND_KEY_PLUS "\xE2\x8C\x98" // UTF-8 encoding of U+2318 "PLACE OF INTEREST SIGN"
#define ALT_KEY_PLUS ALT_KEY
#define SHIFT_KEY_PLUS SHIFT_KEY
#define COMMAND_SHIFT_KEYS_PLUS SHIFT_KEY_PLUS COMMAND_KEY_PLUS
#define COMMAND_ALT_KEYS_PLUS ALT_KEY_PLUS COMMAND_KEY_PLUS

#define BACKSPACE_KEY NSBackspaceCharacter
#define DELETE_KEY    FL_COMMAND + NSBackspaceCharacter
#define TAB_KEY       NSTabCharacter
#define ESCAPE_KEY    NSEscapeCharacter
#define UP_KEY        NSUpArrowFunctionKey
#define DOWN_KEY      NSDownArrowFunctionKey
#define LEFT_KEY      NSLeftArrowFunctionKey
#define RIGHT_KEY     NSRightArrowFunctionKey

#define GLUE_KEY '?'
#define FULLSCREEN_KEY FL_COMMAND + FL_SHIFT + 'f'

#define TAB_SYMBOL "\xE2\x87\xA5"
#define RIGHT_SYMBOL "\xE2\x86\x92"
#else
#define CONTROL_KEY "Ctrl"
#define ALT_KEY "Alt"
#define SHIFT_KEY "Shift"
#define COMMAND_KEY CONTROL_KEY

#define COMMAND_KEY_PLUS CONTROL_KEY "+"
#define ALT_KEY_PLUS ALT_KEY "+"
#define SHIFT_KEY_PLUS SHIFT_KEY "+"
#define COMMAND_SHIFT_KEYS_PLUS COMMAND_KEY_PLUS SHIFT_KEY_PLUS
#define COMMAND_ALT_KEYS_PLUS COMMAND_KEY_PLUS ALT_KEY_PLUS

#define BACKSPACE_KEY FL_BackSpace
#define DELETE_KEY    FL_Delete
#define TAB_KEY       FL_Tab
#define ESCAPE_KEY    FL_Escape
#define UP_KEY        FL_Up
#define DOWN_KEY      FL_Down
#define LEFT_KEY      FL_Left
#define RIGHT_KEY     FL_Right

#define GLUE_KEY FL_SHIFT + '/'
#define FULLSCREEN_KEY FL_F + 11

#define TAB_SYMBOL "Tab"
#define RIGHT_SYMBOL "Right"
#endif

#ifndef _countof
#define _countof(a) (sizeof(a) / sizeof(a[0]))
#endif

#define RANGE(x) std::begin(x), std::end(x)

typedef uint8_t size8_t;
typedef uint16_t size16_t;
typedef uint32_t size32_t;
typedef uint64_t size64_t;

extern const std::string whitespace;

bool equals_ignore_case(std::string_view s, std::string_view p);
bool starts_with(std::string_view s, std::string_view p);
bool ends_with(std::string_view s, std::string_view p);
bool ends_with_ignore_case(std::string_view s, std::string_view p);
bool ends_with_ignore_case(std::wstring_view s, std::wstring_view p);
bool is_indented(std::string_view s);
bool is_hex(std::string_view s);
bool is_decimal(std::string_view s);
bool is_octal(std::string_view s);
bool is_binary(std::string_view s);
void trim(std::string &s, const std::string &t = whitespace);
void rtrim(std::string &s, const std::string &t = whitespace);
void lowercase(std::string &s);
bool leading_macro(std::istringstream &iss, std::string &macro, const char *v = NULL);
void remove_comment(std::string &s);
void remove_suffix(const char *n, char *s);
void before_suffix(const char *n, char *s);
void after_suffix(const char *n, char *s);
void remove_dot_ext(const char *f, char *s);
void add_dot_ext(const char *f, const char *ext, char *s);
int text_width(const char *l, int pad = 0);
bool file_exists(const char *f);
size_t file_size(const char *f);
size_t file_size(FILE *f);
int64_t file_modified(const char *f);
void open_ifstream(std::ifstream &ifs, const char *f);
void open_ofstream(std::ofstream &ofs, const char *f);
void draw_outlined_text(const char *l, int x, int y, int w, int h, Fl_Align a, Fl_Color c, Fl_Color s);

bool parse_value(std::string s, int32_t &v);

#endif
