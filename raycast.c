#include "stdarg.h"
#include "ctype.h"
#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"
#include "errno.h"
#include "math.h"
#include "zlib.h"
#include "dirent.h"
#include "unistd.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "SDL2/SDL.h"

/* Embed: /home/runner/work/raycasting/raycasting/ocen/std/og/interface.c */
/**
 * This is a simple interface to register a main loop function.
 * It's written at the C level since we want some compile-time
 * conditional logic dependong on the target platform, which
 * we cannot do natively in Ocen right now.
*/

// NOTE: Emscripten expects a callback that doesn't return anything,
//       but we want to be able to return a boolean to indicate when
//       the main loop should stop. We'll use a wrapper function to
//       handle this, and store the actual callback in a global variable.


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>

_Bool (*callback)(void) = NULL;
// EMS no-return callback wrapper
void ems_callback() {
    if (callback) callback();
}
#endif

void c_set_main_loop(_Bool (*func)(void)) {
#ifdef __EMSCRIPTEN__
    callback = func;
    emscripten_set_main_loop(ems_callback, 0, 1);
#else
    while (func()) {}
#endif
}


/* Embed: /home/runner/work/raycasting/raycasting/ocen/std/./prelude.h */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>

#ifndef OC_NO_BACKTRACE
#include <execinfo.h>
#endif

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

void dump_backtrace() {
#ifndef OC_NO_BACKTRACE
  void *array[40];
  size_t size = backtrace(array, 40);
  char **strings = backtrace_symbols(array, size);
  printf("\nBacktrace:\n");
  for (size_t i = 1; i < size; i++) {
    printf("%s\n", strings[i]);
  }
  free(strings);
#endif
}

#ifdef __APPLE__
  #define oc_trap __builtin_debugtrap
#else
  #define oc_trap __builtin_trap
#endif

void ae_assert_fail(char *dbg_msg, char *msg) {
  printf("--------------------------------------------------------------------------------\n");
  printf("%s\n", dbg_msg);
  if (msg) {
    printf("  Message: %s\n", msg);
  }
  printf("--------------------------------------------------------------------------------\n");
  fflush(stdout);
  dump_backtrace();
  oc_trap();
}


/* Enums */
typedef enum CellType {
  CellType_Empty,
  CellType_Wall,
} CellType;

char *CellType_dbg(CellType this) {
  switch (this) {
    case CellType_Empty: return "Empty";
    case CellType_Wall: return "Wall";
    default: return "<unknown>";
  }
}

typedef enum Direction {
  Direction_Vertical,
  Direction_Horizontal,
} Direction;

char *Direction_dbg(Direction this) {
  switch (this) {
    case Direction_Vertical: return "Vertical";
    case Direction_Horizontal: return "Horizontal";
    default: return "<unknown>";
  }
}

typedef enum std_Endian {
  std_Endian_Little,
  std_Endian_Big,
} std_Endian;

char *std_Endian_dbg(std_Endian this) {
  switch (this) {
    case std_Endian_Little: return "Little";
    case std_Endian_Big: return "Big";
    default: return "<unknown>";
  }
}

char *std_fs_SeekMode_dbg(i32 this) {
  switch (this) {
    case SEEK_SET: return "Set";
    case SEEK_CUR: return "Cur";
    case SEEK_END: return "End";
    default: return "<unknown>";
  }
}

typedef enum std_fs_EntryType {
  std_fs_EntryType_File,
  std_fs_EntryType_Directory,
  std_fs_EntryType_SymbolicLink,
  std_fs_EntryType_Unknown,
} std_fs_EntryType;

char *std_fs_EntryType_dbg(std_fs_EntryType this) {
  switch (this) {
    case std_fs_EntryType_File: return "File";
    case std_fs_EntryType_Directory: return "Directory";
    case std_fs_EntryType_SymbolicLink: return "SymbolicLink";
    case std_fs_EntryType_Unknown: return "Unknown";
    default: return "<unknown>";
  }
}

typedef enum std_image_RotationDirection {
  std_image_RotationDirection_Clockwise,
  std_image_RotationDirection_CounterClockwise,
} std_image_RotationDirection;

char *std_image_RotationDirection_dbg(std_image_RotationDirection this) {
  switch (this) {
    case std_image_RotationDirection_Clockwise: return "Clockwise";
    case std_image_RotationDirection_CounterClockwise: return "CounterClockwise";
    default: return "<unknown>";
  }
}

char *std_image_png_ColorType_dbg(int this) {
  switch (this) {
    case 0: return "Grayscale";
    case 2: return "RGB";
    case 3: return "Palette";
    case 4: return "GrayscaleAlpha";
    case 6: return "RGBA";
    default: return "<unknown>";
  }
}

char *std_image_png_InterlaceMethod_dbg(int this) {
  switch (this) {
    case 0: return "None";
    case 1: return "Adam7";
    default: return "<unknown>";
  }
}

typedef enum std_og_state_MouseWheel {
  std_og_state_MouseWheel_Down,
  std_og_state_MouseWheel_Right,
  std_og_state_MouseWheel_Up,
  std_og_state_MouseWheel_Left,
} std_og_state_MouseWheel;

char *std_og_state_MouseWheel_dbg(std_og_state_MouseWheel this) {
  switch (this) {
    case std_og_state_MouseWheel_Down: return "Down";
    case std_og_state_MouseWheel_Right: return "Right";
    case std_og_state_MouseWheel_Up: return "Up";
    case std_og_state_MouseWheel_Left: return "Left";
    default: return "<unknown>";
  }
}

char *std_sdl_Key_dbg(SDL_Scancode this) {
  switch (this) {
    case SDL_SCANCODE_A: return "A";
    case SDL_SCANCODE_B: return "B";
    case SDL_SCANCODE_C: return "C";
    case SDL_SCANCODE_D: return "D";
    case SDL_SCANCODE_E: return "E";
    case SDL_SCANCODE_F: return "F";
    case SDL_SCANCODE_G: return "G";
    case SDL_SCANCODE_H: return "H";
    case SDL_SCANCODE_I: return "I";
    case SDL_SCANCODE_J: return "J";
    case SDL_SCANCODE_K: return "K";
    case SDL_SCANCODE_L: return "L";
    case SDL_SCANCODE_M: return "M";
    case SDL_SCANCODE_N: return "N";
    case SDL_SCANCODE_O: return "O";
    case SDL_SCANCODE_P: return "P";
    case SDL_SCANCODE_Q: return "Q";
    case SDL_SCANCODE_R: return "R";
    case SDL_SCANCODE_S: return "S";
    case SDL_SCANCODE_T: return "T";
    case SDL_SCANCODE_U: return "U";
    case SDL_SCANCODE_V: return "V";
    case SDL_SCANCODE_W: return "W";
    case SDL_SCANCODE_X: return "X";
    case SDL_SCANCODE_Y: return "Y";
    case SDL_SCANCODE_Z: return "Z";
    case SDL_SCANCODE_UP: return "Up";
    case SDL_SCANCODE_DOWN: return "Down";
    case SDL_SCANCODE_LEFT: return "Left";
    case SDL_SCANCODE_RIGHT: return "Right";
    case SDL_SCANCODE_SPACE: return "Space";
    case SDL_SCANCODE_ESCAPE: return "Escape";
    case SDL_SCANCODE_COMMA: return "Comma";
    case SDL_SCANCODE_PERIOD: return "Dot";
    default: return "<unknown>";
  }
}

char *std_sdl_MouseButton_dbg(u8 this) {
  switch (this) {
    case SDL_BUTTON_LEFT: return "Left";
    case SDL_BUTTON_MIDDLE: return "Middle";
    case SDL_BUTTON_RIGHT: return "Right";
    case SDL_BUTTON_X1: return "Forward";
    case SDL_BUTTON_X2: return "Backward";
    default: return "<unknown>";
  }
}

char *std_sdl_EventType_dbg(SDL_EventType this) {
  switch (this) {
    case SDL_QUIT: return "Quit";
    case SDL_KEYDOWN: return "KeyDown";
    case SDL_KEYUP: return "KeyUp";
    case SDL_MOUSEBUTTONDOWN: return "MouseDown";
    case SDL_MOUSEBUTTONUP: return "MouseUp";
    case SDL_MOUSEWHEEL: return "Wheel";
    case SDL_WINDOWEVENT: return "Window";
    case SDL_USEREVENT: return "User";
    default: return "<unknown>";
  }
}

char *std_sdl_BlendMode_dbg(SDL_BlendMode this) {
  switch (this) {
    case SDL_BLENDMODE_NONE: return "None";
    case SDL_BLENDMODE_BLEND: return "Blend";
    case SDL_BLENDMODE_ADD: return "Add";
    case SDL_BLENDMODE_MOD: return "Mod";
    default: return "<unknown>";
  }
}

char *std_sdl_WindowMode_dbg(int this) {
  switch (this) {
    case 0: return "Windowed";
    case SDL_WINDOW_FULLSCREEN: return "Fullscreen";
    case SDL_WINDOW_FULLSCREEN_DESKTOP: return "Borderless";
    default: return "<unknown>";
  }
}

char *std_sdl_CursorType_dbg(SDL_SystemCursor this) {
  switch (this) {
    case SDL_SYSTEM_CURSOR_ARROW: return "Arrow";
    case SDL_SYSTEM_CURSOR_IBEAM: return "IBeam";
    case SDL_SYSTEM_CURSOR_WAIT: return "Wait";
    case SDL_SYSTEM_CURSOR_CROSSHAIR: return "Crosshair";
    case SDL_SYSTEM_CURSOR_WAITARROW: return "WaitArrow";
    case SDL_SYSTEM_CURSOR_SIZENWSE: return "SizeNWSE";
    case SDL_SYSTEM_CURSOR_SIZENESW: return "SizeNESW";
    case SDL_SYSTEM_CURSOR_SIZEWE: return "SizeWE";
    case SDL_SYSTEM_CURSOR_SIZENS: return "SizeNS";
    case SDL_SYSTEM_CURSOR_SIZEALL: return "SizeAll";
    case SDL_SYSTEM_CURSOR_NO: return "No";
    case SDL_SYSTEM_CURSOR_HAND: return "Hand";
    default: return "<unknown>";
  }
}

/* Constants */
#define NUM_RAYS (512)
#define MOVE_SPEED (4.0f)
#define TURN_SPEED (3.0f)
#define MOUSE_TURN_MULTIPLIER (0.15f)
#define FOV_ZOOM_FACTOR (1.1f)
#define JUMP_VEL (4.0f)
#define JUMP_TIME (1.0f)
#define GRAVITY (((4.0f * JUMP_VEL) / JUMP_TIME))
#define CELL_HIGHIGHT_THICKNESS (5)
#define CELL_HIGHIGHT_LENGTH (10)
#define EPS (0.0001f)
#define std_math_TAU ((M_PI * 2.0f))
#define std_og_state_FRAMETIMES_COUNT (200)
/* Struct typedefs */
typedef struct std_vec_Vec2__1 std_vec_Vec2__1;
typedef struct Game Game;
typedef struct std_CharIterator std_CharIterator;
typedef struct std_sv_SV std_sv_SV;
typedef struct std_sv_SVLineIterator std_sv_SVLineIterator;
typedef struct std_sv_SVSplitIterator std_sv_SVSplitIterator;
typedef struct std_sv_SVWordIterator std_sv_SVWordIterator;
typedef struct std_sv_SVNumberIterator__0 std_sv_SVNumberIterator__0;
typedef struct std_sv_SVNumberIterator__1 std_sv_SVNumberIterator__1;
typedef struct std_sv_SVPredicateIterator std_sv_SVPredicateIterator;
typedef struct std_fs_FileMetadata std_fs_FileMetadata;
typedef struct std_fs_DirectoryEntry std_fs_DirectoryEntry;
typedef struct std_fs_DirectoryIterator std_fs_DirectoryIterator;
typedef struct std_image_Color std_image_Color;
typedef struct std_image_Image std_image_Image;
typedef struct std_image_qoi_RGBA std_image_qoi_RGBA;
typedef struct std_buffer_BytesReader std_buffer_BytesReader;
typedef struct std_image_qoi_QOIDecoder std_image_qoi_QOIDecoder;
typedef struct std_image_qoi_QOIEncoder std_image_qoi_QOIEncoder;
typedef struct std_buffer_Buffer std_buffer_Buffer;
typedef struct std_image_png_Chunk std_image_png_Chunk;
typedef struct std_image_png_PNG std_image_png_PNG;
typedef struct std_vec_Vec2__0 std_vec_Vec2__0;
typedef struct std_vec_Vec2__2 std_vec_Vec2__2;
typedef struct std_vec_Vec2__3 std_vec_Vec2__3;
typedef struct std_vec_Vec2__4 std_vec_Vec2__4;
typedef struct std_vec_Vec2__5 std_vec_Vec2__5;
typedef struct std_vec_Vec3__0 std_vec_Vec3__0;
typedef struct std_vec_Vec3__1 std_vec_Vec3__1;
typedef struct std_vec_Vec3__2 std_vec_Vec3__2;
typedef struct std_vec_Vec3__3 std_vec_Vec3__3;
typedef struct std_vec_Vec3__4 std_vec_Vec3__4;
typedef struct std_vec_Vec3__5 std_vec_Vec3__5;
typedef struct std_og_Rect std_og_Rect;
typedef struct std_og_Texture std_og_Texture;
typedef struct std_og_state_KeyState std_og_state_KeyState;
typedef struct std_og_state_MouseState std_og_state_MouseState;

/* Struct definitions */
struct std_vec_Vec2__1 {
  f32 x;
  f32 y;
};

struct Game {
  u32 rows;
  u32 cols;
  u32 grid_size;
  CellType *grid;
  f32 angle;
  f32 fov;
  std_vec_Vec2__1 pos;
  f32 jump;
  f32 up_vel;
  bool map_enabled;
  std_image_Image *wall_img;
  std_image_Image *ground_img;
  std_og_Texture *tx;
};

struct std_CharIterator {
  char *data;
  u32 len;
  u32 pos;
};

struct std_sv_SV {
  char *data;
  u32 len;
};

struct std_sv_SVLineIterator {
  std_sv_SV sv;
};

struct std_sv_SVSplitIterator {
  std_sv_SV sv;
  std_sv_SV delim;
};

struct std_sv_SVWordIterator {
  std_sv_SV sv;
};

struct std_sv_SVNumberIterator__0 {
  std_sv_SV sv;
  u64 cur_num;
  bool found;
};

struct std_sv_SVNumberIterator__1 {
  std_sv_SV sv;
  i64 cur_num;
  bool found;
};

struct std_sv_SVPredicateIterator {
  std_sv_SV sv;
  u32 (*pred)(std_sv_SV);
  bool found;
  std_sv_SV res;
};

struct std_fs_FileMetadata {
  u64 size;
  u64 atime;
  u64 mtime;
  u64 ctime;
  u32 uid;
  u32 gid;
  u64 dev;
  u32 mode;
};

struct std_fs_DirectoryEntry {
  std_fs_EntryType type;
  char *name;
};

struct std_fs_DirectoryIterator {
  DIR *dir;
  struct dirent *dp;
  bool skip_self_and_parent;
};

struct std_image_Color {
  u8 r;
  u8 g;
  u8 b;
};

struct std_image_Image {
  u32 width;
  u32 height;
  std_image_Color *data;
};

struct std_image_qoi_RGBA {
  u8 r;
  u8 g;
  u8 b;
  u8 a;
};

struct std_buffer_BytesReader {
  u8 *data;
  u32 size;
  u32 index;
};

struct std_image_qoi_QOIDecoder {
  std_buffer_BytesReader io;
  u32 width;
  u32 height;
  u8 channels;
  u8 colorspace;
  u32 pixels_done;
  std_image_qoi_RGBA prev;
  std_image_qoi_RGBA seen[64];
  std_image_Image *img;
};

struct std_image_qoi_QOIEncoder {
  std_buffer_Buffer *buf;
  std_image_Image *img;
  std_image_qoi_RGBA seen[64];
  std_image_qoi_RGBA prev;
  u32 pixels_done;
};

struct std_buffer_Buffer {
  u8 *data;
  u32 size;
  u32 capacity;
};

struct std_image_png_Chunk {
  std_sv_SV type;
  std_buffer_Buffer data;
  bool is_critical;
  bool is_private;
};

struct std_image_png_PNG {
  u32 width;
  u32 height;
  u8 bit_depth;
  int color_type;
  int interlace_method;
  u32 S;
};

struct std_vec_Vec2__0 {
  u64 x;
  u64 y;
};

struct std_vec_Vec2__2 {
  f64 x;
  f64 y;
};

struct std_vec_Vec2__3 {
  i32 x;
  i32 y;
};

struct std_vec_Vec2__4 {
  i64 x;
  i64 y;
};

struct std_vec_Vec2__5 {
  u32 x;
  u32 y;
};

struct std_vec_Vec3__0 {
  f32 x;
  f32 y;
  f32 z;
};

struct std_vec_Vec3__1 {
  f64 x;
  f64 y;
  f64 z;
};

struct std_vec_Vec3__2 {
  i32 x;
  i32 y;
  i32 z;
};

struct std_vec_Vec3__3 {
  i64 x;
  i64 y;
  i64 z;
};

struct std_vec_Vec3__4 {
  u32 x;
  u32 y;
  u32 z;
};

struct std_vec_Vec3__5 {
  u64 x;
  u64 y;
  u64 z;
};

struct std_og_Rect {
  i32 x;
  i32 y;
  i32 w;
  i32 h;
};

struct std_og_Texture {
  SDL_Texture *tx;
  i32 w;
  i32 h;
  u8 *pixels;
  i32 pitch;
};

struct std_og_state_KeyState {
  bool keys[SDL_NUM_SCANCODES];
};

struct std_og_state_MouseState {
  std_vec_Vec2__3 vec;
  std_vec_Vec2__3 pos;
  bool buttons[16];
  bool wheel_dirs[8];
};


/* function declarations */
Game Game_make(u32 rows, u32 cols, u32 grid_size);
void Game_draw(Game *this);
void Game_set_cell(Game *this, u32 row, u32 col, CellType cell);
bool Game_ray_check_intersection(Game *this, f32 x, f32 y, f32 dx, f32 dy, f32 *dist, u32 *cell_idx, u32 steps);
void Game_draw_ray(Game *this, std_vec_Vec2__1 v0, std_vec_Vec2__1 v1, SDL_Color color);
void Game_check_ray(Game *this, i32 i, f32 angle);
std_vec_Vec2__3 Game_to_screen(Game *this, std_vec_Vec2__1 v);
void Game_cast_rays(Game *this);
i32 Game_get_cell_size(Game *this);
void Game_highlight_cell(Game *this, u32 row, u32 col);
void Game_toggle_cell(Game *this, u32 row, u32 col);
std_vec_Vec2__1 Game_to_world(Game *this, std_vec_Vec2__3 v);
CellType Game_get_cell_or_empty(Game *this, i32 x, i32 y);
void Game_move(Game *this, f32 angle_off, f32 dt);
bool Game_step(Game *this);
bool step(void);
i32 main(void);
void std_panic(char *msg) __attribute__((noreturn));
u32 str_to_u32(char *this);
bool str_eq(char *this, char *other);
char *std_format(char *fmt, ...);
char *str_substring(char *this, u32 start, u32 len);
bool str_ends_with(char *this, char *suffix);
bool str_starts_with(char *this, char *prefix);
void str_strip_trailing_whitespace(char *this);
void str_remove_last_n(char *this, u32 n);
void str_replace_with(char **this, char *other);
void str_free(char **this);
std_CharIterator str_chars(char *this, u32 start);
bool std_CharIterator_has_value(std_CharIterator *this);
u32 std_CharIterator_next(std_CharIterator *this);
char std_CharIterator_cur(std_CharIterator *this);
bool char_is_digit(char this);
bool char_is_hex_digit(char this);
u32 char_get_digit(char this);
u8 char_get_hex_digit(char this);
bool char_is_alpha(char this);
bool char_is_alnum(char this);
bool char_is_print(char this);
bool char_is_space(char this);
bool char_is_upper(char this);
bool char_is_lower(char this);
i32 i32_min(i32 this, i32 b);
i32 i32_max(i32 this, i32 b);
i8 i8_min(i8 this, i8 b);
i8 i8_max(i8 this, i8 b);
u64 u64_min(u64 this, u64 other);
u64 u64_max(u64 this, u64 other);
u32 u32_min(u32 this, u32 other);
u32 u32_max(u32 this, u32 other);
u8 u8_min(u8 this, u8 other);
u8 u8_max(u8 this, u8 other);
char *std_shift_args(i32 *argc, char ***argv, char *where);
void std_mem_set_allocator(void *allocator, void *(*alloc_fn)(void *, u32), void (*free_fn)(void *, void *), void *(*realloc_fn)(void *, void *, u32, u32));
void std_mem_reset_to_default_allocator(void);
CellType *std_mem_alloc__0(u32 count);
char *std_mem_alloc__1(u32 count);
std_image_Image *std_mem_alloc__2(u32 count);
std_image_Color *std_mem_alloc__3(u32 count);
u8 *std_mem_alloc__4(u32 count);
void std_mem_free(void *ptr);
u8 *std_mem_realloc__0(u8 *ptr, u32 old_count, u32 new_count);
void *std_mem_impl_my_calloc(void *state, u32 size);
void *std_mem_impl_my_realloc(void *state, void *ptr, u32 old_size, u32 size);
void std_mem_impl_my_free(void *state, void *ptr);
bool std_sv_SV_is_empty(std_sv_SV *this);
std_sv_SV std_sv_SV_from_buf(std_buffer_Buffer buf);
std_sv_SV std_sv_SV_from_str(char *s);
std_sv_SV std_sv_SV_get(std_sv_SV this, u32 n);
char std_sv_SV_at(std_sv_SV this, u32 n);
std_sv_SV std_sv_SV_chop_left(std_sv_SV *this, u32 n);
std_sv_SV std_sv_SV_chop_right(std_sv_SV *this, u32 n);
void std_sv_SV_trim_left(std_sv_SV *this);
void std_sv_SV_trim_right(std_sv_SV *this);
void std_sv_SV_trim(std_sv_SV *this);
i32 std_sv_SV_find_str(std_sv_SV *this, char *s);
std_sv_SV std_sv_SV_chop_by_delim(std_sv_SV *this, char delim);
std_sv_SV std_sv_SV_chop_by_sv(std_sv_SV *this, std_sv_SV delim);
std_sv_SV std_sv_SV_chop_by_str(std_sv_SV *this, char *delim);
std_sv_SV std_sv_SV_chop_word(std_sv_SV *this);
u64 std_sv_SV_chop_unsigned(std_sv_SV *this, char *fn_name);
u64 std_sv_SV_chop_u64(std_sv_SV *this);
u32 std_sv_SV_chop_u32(std_sv_SV *this);
u16 std_sv_SV_chop_u16(std_sv_SV *this);
u8 std_sv_SV_chop_u8(std_sv_SV *this);
f32 std_sv_SV_chop_f32(std_sv_SV *this);
i64 std_sv_SV_chop_signed(std_sv_SV *this, char *fn_name);
i64 std_sv_SV_chop_i64(std_sv_SV *this);
i32 std_sv_SV_chop_i32(std_sv_SV *this);
i16 std_sv_SV_chop_i16(std_sv_SV *this);
i8 std_sv_SV_chop_i8(std_sv_SV *this);
std_sv_SV std_sv_SV_chop_line(std_sv_SV *this);
void std_sv_SV_print(std_sv_SV *this, char *msg);
bool std_sv_SV_eq(std_sv_SV this, std_sv_SV other);
bool std_sv_SV_eq_str(std_sv_SV this, char *other);
u32 std_sv_SV_hash(std_sv_SV this);
void std_sv_SV_only_chop_left(std_sv_SV *this, u32 n);
bool std_sv_SV_starts_with_sv(std_sv_SV this, std_sv_SV other);
bool std_sv_SV_starts_with_str(std_sv_SV this, char *other);
bool std_sv_SV_ends_with_sv(std_sv_SV this, std_sv_SV other);
bool std_sv_SV_ends_with_str(std_sv_SV this, char *other);
std_sv_SV std_sv_SV_slice(std_sv_SV this, u32 off, u32 end);
u64 std_sv_SV_count(std_sv_SV this, char c);
bool std_sv_SV_contains(std_sv_SV this, char c);
std_CharIterator std_sv_SV_chars(std_sv_SV this, u32 start);
std_sv_SVLineIterator std_sv_SV_lines(std_sv_SV this);
std_sv_SVSplitIterator std_sv_SV_split_sv(std_sv_SV this, std_sv_SV delim);
std_sv_SVSplitIterator std_sv_SV_split_str(std_sv_SV this, char *delim);
std_sv_SVWordIterator std_sv_SV_split_words(std_sv_SV this);
std_sv_SVNumberIterator__0 std_sv_SV_iter_nums(std_sv_SV this);
std_sv_SVNumberIterator__0 std_sv_SV_iter_u64(std_sv_SV this);
std_sv_SVNumberIterator__1 std_sv_SV_iter_i64(std_sv_SV this);
std_sv_SVPredicateIterator std_sv_SV_iter_pred(std_sv_SV this, u32 (*pred)(std_sv_SV));
std_buffer_BytesReader std_sv_SV_reader(std_sv_SV this);
char *std_sv_SV_copy_data_to_cstr(std_sv_SV this);
bool std_sv_SVLineIterator_has_value(std_sv_SVLineIterator *this);
std_sv_SV std_sv_SVLineIterator_cur(std_sv_SVLineIterator *this);
void std_sv_SVLineIterator_next(std_sv_SVLineIterator *this);
bool std_sv_SVSplitIterator_has_value(std_sv_SVSplitIterator *this);
std_sv_SV std_sv_SVSplitIterator_cur(std_sv_SVSplitIterator *this);
void std_sv_SVSplitIterator_next(std_sv_SVSplitIterator *this);
bool std_sv_SVWordIterator_has_value(std_sv_SVWordIterator *this);
std_sv_SV std_sv_SVWordIterator_cur(std_sv_SVWordIterator *this);
void std_sv_SVWordIterator_next(std_sv_SVWordIterator *this);
std_sv_SVPredicateIterator std_sv_SVPredicateIterator_make(std_sv_SV sv, u32 (*pred)(std_sv_SV));
bool std_sv_SVPredicateIterator_has_value(std_sv_SVPredicateIterator *this);
std_sv_SV std_sv_SVPredicateIterator_cur(std_sv_SVPredicateIterator *this);
void std_sv_SVPredicateIterator_next(std_sv_SVPredicateIterator *this);
u64 std_sv_SVNumberIterator__0_cur(std_sv_SVNumberIterator__0 *this);
void std_sv_SVNumberIterator__0_next(std_sv_SVNumberIterator__0 *this);
bool std_sv_SVNumberIterator__0_has_value(std_sv_SVNumberIterator__0 *this);
std_sv_SVNumberIterator__0 std_sv_SVNumberIterator__0_make(std_sv_SV sv);
i64 std_sv_SVNumberIterator__1_cur(std_sv_SVNumberIterator__1 *this);
void std_sv_SVNumberIterator__1_next(std_sv_SVNumberIterator__1 *this);
bool std_sv_SVNumberIterator__1_has_value(std_sv_SVNumberIterator__1 *this);
std_sv_SVNumberIterator__1 std_sv_SVNumberIterator__1_make(std_sv_SV sv);
char *std_fs_realpath(char *path);
FILE *std_fs_File_open(char *path, char *mode);
i32 std_fs_File_read(FILE *this, void *buf, u32 size);
i32 std_fs_File_write(FILE *this, void *buf, u32 size);
u32 std_fs_File_size(FILE *this);
void std_fs_File_puts(FILE *this, char *s);
void std_fs_File_truncate(FILE *this, i64 size);
void std_fs_File_seek(FILE *this, i64 offset, i32 mode);
std_fs_FileMetadata std_fs_file_info(char *path);
bool std_fs_file_exists(char *path);
void std_fs_write_file_bytes(char *path, void *data, u32 size);
void std_fs_write_file_str(char *path, char *data);
void std_fs_write_file(char *path, std_buffer_Buffer data);
std_buffer_Buffer std_fs_read_file(char *path);
bool std_fs_directory_exists(char *path);
void std_fs_create_directory(char *path, bool exists_ok);
std_fs_DirectoryIterator std_fs_iterate_directory(char *path, bool skip_self_and_parent);
bool std_fs_DirectoryIterator_has_value(std_fs_DirectoryIterator *this);
void std_fs_DirectoryIterator_next(std_fs_DirectoryIterator *this);
std_fs_DirectoryEntry std_fs_DirectoryIterator_cur(std_fs_DirectoryIterator *this);
char *std_libc_get_err(void);
std_image_Image *std_image_Image_new(u32 width, u32 height);
void std_image_Image_free(std_image_Image *this);
std_image_Color std_image_Image_get(std_image_Image *this, u32 x, u32 y);
void std_image_Image_set(std_image_Image *this, u32 x, u32 y, std_image_Color col);
void std_image_Image_save(std_image_Image *this, char *filename);
std_image_Image *std_image_Image_load(char *filename);
f32 std_image_interp(f32 t, f32 a, f32 b);
f32 std_image_bilinear_interp(f32 t0, f32 t1, f32 a, f32 b, f32 c, f32 d);
std_image_Color std_image_Image_get_interpolated(std_image_Image *this, f32 xf, f32 yf);
std_image_Image *std_image_Image_resize(std_image_Image *this, u32 new_width, u32 new_height);
std_image_Image *std_image_Image_rotate(std_image_Image *this, std_image_RotationDirection dir);
void std_image_Image_clear(std_image_Image *this);
std_image_Image *std_image_Image_copy(std_image_Image *this);
void std_image_ppm_encode(std_image_Image *img, std_buffer_Buffer *data);
std_image_Image *std_image_ppm_decode(std_buffer_Buffer *data);
u32 std_image_qoi_RGBA_hash(std_image_qoi_RGBA this);
bool std_image_qoi_RGBA_eq(std_image_qoi_RGBA this, std_image_qoi_RGBA other);
std_image_qoi_QOIDecoder std_image_qoi_QOIDecoder_make(std_buffer_Buffer *data);
std_image_Image *std_image_qoi_QOIDecoder_decode(std_image_qoi_QOIDecoder *this);
void std_image_qoi_QOIDecoder_handle_pixel(std_image_qoi_QOIDecoder *this, std_image_qoi_RGBA pix);
void std_image_qoi_QOIDecoder_decode_pixels(std_image_qoi_QOIDecoder *this);
void std_image_qoi_QOIDecoder_decode_op_rgb(std_image_qoi_QOIDecoder *this);
void std_image_qoi_QOIDecoder_decode_op_rgba(std_image_qoi_QOIDecoder *this);
void std_image_qoi_QOIDecoder_decode_op_luma(std_image_qoi_QOIDecoder *this, u8 first);
void std_image_qoi_QOIDecoder_decode_op_index(std_image_qoi_QOIDecoder *this, u8 first);
void std_image_qoi_QOIDecoder_decode_op_diff(std_image_qoi_QOIDecoder *this, u8 first);
void std_image_qoi_QOIDecoder_decode_op_run(std_image_qoi_QOIDecoder *this, u8 first);
std_image_qoi_QOIEncoder std_image_qoi_QOIEncoder_make(std_image_Image *img, std_buffer_Buffer *buffer);
void std_image_qoi_QOIEncoder_encode_header(std_image_qoi_QOIEncoder *this);
std_image_qoi_RGBA std_image_qoi_QOIEncoder_img_pix(std_image_qoi_QOIEncoder *this, u32 idx);
void std_image_qoi_QOIEncoder_handle_pixel(std_image_qoi_QOIEncoder *this, std_image_qoi_RGBA pix, u32 num);
void std_image_qoi_QOIEncoder_encode_pixel(std_image_qoi_QOIEncoder *this, std_image_qoi_RGBA pix);
void std_image_qoi_QOIEncoder_encode_pixels(std_image_qoi_QOIEncoder *this);
void std_image_qoi_QOIEncoder_encode_end(std_image_qoi_QOIEncoder *this);
void std_image_qoi_QOIEncoder_encode(std_image_qoi_QOIEncoder *this);
void std_image_qoi_encode(std_image_Image *img, std_buffer_Buffer *data);
std_image_Image *std_image_qoi_decode(std_buffer_Buffer *data);
void std_image_png_make_crc_table(void);
u32 std_image_png_get_crc(u8 *buf, u32 len, u32 init);
std_image_png_Chunk std_image_png_Chunk_parse(std_buffer_BytesReader *io);
void std_image_png_Chunk_encode(std_image_png_Chunk *this, std_buffer_Buffer *out);
u8 std_image_png_paeth_predictor(u8 ua, u8 ub, u8 uc);
void std_image_png_PNG_decode_scanline(std_image_png_PNG *this, std_buffer_Buffer *out, u32 scanline, std_sv_SV sv);
void std_image_png_encode(std_image_Image *img, std_buffer_Buffer *data);
std_image_Image *std_image_png_decode(std_buffer_Buffer *data);
std_vec_Vec2__0 std_vec_Vec2__0_subrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_sub(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__1 std_vec_Vec2__0_normalized(std_vec_Vec2__0 this);
std_vec_Vec2__1 std_vec_Vec2__1_subrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_sub(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_normalized(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_multrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_add(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
f32 std_vec_Vec2__1_length_sq(std_vec_Vec2__1 this);
std_vec_Vec2__2 std_vec_Vec2__1_to_f64(std_vec_Vec2__1 this);
std_vec_Vec2__2 std_vec_Vec2__2_subrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_sub(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__1 std_vec_Vec2__2_normalized(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_multrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_add(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
f64 std_vec_Vec2__2_length_sq(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_to_f64(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_mult(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__2 std_vec_Vec2__2_addrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__1 std_vec_Vec2__2_to_f32(std_vec_Vec2__2 this);
std_vec_Vec2__3 std_vec_Vec2__2_to_i32(std_vec_Vec2__2 this);
std_vec_Vec2__3 std_vec_Vec2__3_subrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_sub(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__1 std_vec_Vec2__3_normalized(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_multrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_add(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
i32 std_vec_Vec2__3_length_sq(std_vec_Vec2__3 this);
std_vec_Vec2__2 std_vec_Vec2__3_to_f64(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_mult(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__3 std_vec_Vec2__3_addrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__1 std_vec_Vec2__3_to_f32(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_to_i32(std_vec_Vec2__3 this);
std_vec_Vec2__4 std_vec_Vec2__3_to_i64(std_vec_Vec2__3 this);
std_vec_Vec2__4 std_vec_Vec2__4_subrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_sub(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__1 std_vec_Vec2__4_normalized(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_multrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_add(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
i64 std_vec_Vec2__4_length_sq(std_vec_Vec2__4 this);
std_vec_Vec2__2 std_vec_Vec2__4_to_f64(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_mult(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__4 std_vec_Vec2__4_addrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__1 std_vec_Vec2__4_to_f32(std_vec_Vec2__4 this);
std_vec_Vec2__3 std_vec_Vec2__4_to_i32(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_to_i64(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_reflect(std_vec_Vec2__4 this, std_vec_Vec2__4 normal);
std_vec_Vec2__4 std_vec_Vec2__4_divs(std_vec_Vec2__4 this, i64 val);
bool std_vec_Vec2__4_eq(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__4 std_vec_Vec2__4_subs(std_vec_Vec2__4 this, i64 val);
std_vec_Vec2__5 std_vec_Vec2__4_to_u32(std_vec_Vec2__4 this);
std_vec_Vec2__5 std_vec_Vec2__5_subrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_sub(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__1 std_vec_Vec2__5_normalized(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_multrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_add(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
u32 std_vec_Vec2__5_length_sq(std_vec_Vec2__5 this);
std_vec_Vec2__2 std_vec_Vec2__5_to_f64(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_mult(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__5 std_vec_Vec2__5_addrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__1 std_vec_Vec2__5_to_f32(std_vec_Vec2__5 this);
std_vec_Vec2__3 std_vec_Vec2__5_to_i32(std_vec_Vec2__5 this);
std_vec_Vec2__4 std_vec_Vec2__5_to_i64(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_reflect(std_vec_Vec2__5 this, std_vec_Vec2__5 normal);
std_vec_Vec2__5 std_vec_Vec2__5_divs(std_vec_Vec2__5 this, u32 val);
bool std_vec_Vec2__5_eq(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__5 std_vec_Vec2__5_subs(std_vec_Vec2__5 this, u32 val);
std_vec_Vec2__5 std_vec_Vec2__5_to_u32(std_vec_Vec2__5 this);
std_vec_Vec2__0 std_vec_Vec2__5_to_u64(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_mults(std_vec_Vec2__5 this, u32 val);
u32 std_vec_Vec2__5_hash(std_vec_Vec2__5 this);
std_vec_Vec2__5 std_vec_Vec2__5_adds(std_vec_Vec2__5 this, u32 val);
std_vec_Vec2__5 std_vec_Vec2__5_negate(std_vec_Vec2__5 this);
f32 std_vec_Vec2__5_length(std_vec_Vec2__5 this);
u32 std_vec_Vec2__5_dot(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
u32 std_vec_Vec2__5_cross(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__1 std_vec_Vec2__5_rotate(std_vec_Vec2__5 *this, f32 angle);
std_vec_Vec2__5 std_vec_Vec2__5_div(std_vec_Vec2__5 this, std_vec_Vec2__5 other);
std_vec_Vec2__5 std_vec_Vec2__5_divrs(u32 val, std_vec_Vec2__5 this);
std_vec_Vec2__0 std_vec_Vec2__4_to_u64(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_mults(std_vec_Vec2__4 this, i64 val);
u32 std_vec_Vec2__4_hash(std_vec_Vec2__4 this);
std_vec_Vec2__4 std_vec_Vec2__4_adds(std_vec_Vec2__4 this, i64 val);
std_vec_Vec2__4 std_vec_Vec2__4_negate(std_vec_Vec2__4 this);
f32 std_vec_Vec2__4_length(std_vec_Vec2__4 this);
i64 std_vec_Vec2__4_dot(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
i64 std_vec_Vec2__4_cross(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__1 std_vec_Vec2__4_rotate(std_vec_Vec2__4 *this, f32 angle);
std_vec_Vec2__4 std_vec_Vec2__4_div(std_vec_Vec2__4 this, std_vec_Vec2__4 other);
std_vec_Vec2__4 std_vec_Vec2__4_divrs(i64 val, std_vec_Vec2__4 this);
std_vec_Vec2__3 std_vec_Vec2__3_reflect(std_vec_Vec2__3 this, std_vec_Vec2__3 normal);
std_vec_Vec2__3 std_vec_Vec2__3_divs(std_vec_Vec2__3 this, i32 val);
bool std_vec_Vec2__3_eq(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__3 std_vec_Vec2__3_subs(std_vec_Vec2__3 this, i32 val);
std_vec_Vec2__5 std_vec_Vec2__3_to_u32(std_vec_Vec2__3 this);
std_vec_Vec2__0 std_vec_Vec2__3_to_u64(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_mults(std_vec_Vec2__3 this, i32 val);
u32 std_vec_Vec2__3_hash(std_vec_Vec2__3 this);
std_vec_Vec2__3 std_vec_Vec2__3_adds(std_vec_Vec2__3 this, i32 val);
std_vec_Vec2__3 std_vec_Vec2__3_negate(std_vec_Vec2__3 this);
f32 std_vec_Vec2__3_length(std_vec_Vec2__3 this);
i32 std_vec_Vec2__3_dot(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
i32 std_vec_Vec2__3_cross(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__1 std_vec_Vec2__3_rotate(std_vec_Vec2__3 *this, f32 angle);
std_vec_Vec2__3 std_vec_Vec2__3_div(std_vec_Vec2__3 this, std_vec_Vec2__3 other);
std_vec_Vec2__3 std_vec_Vec2__3_divrs(i32 val, std_vec_Vec2__3 this);
std_vec_Vec2__4 std_vec_Vec2__2_to_i64(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_reflect(std_vec_Vec2__2 this, std_vec_Vec2__2 normal);
std_vec_Vec2__2 std_vec_Vec2__2_divs(std_vec_Vec2__2 this, f64 val);
bool std_vec_Vec2__2_eq(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__2 std_vec_Vec2__2_subs(std_vec_Vec2__2 this, f64 val);
std_vec_Vec2__5 std_vec_Vec2__2_to_u32(std_vec_Vec2__2 this);
std_vec_Vec2__0 std_vec_Vec2__2_to_u64(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_mults(std_vec_Vec2__2 this, f64 val);
u32 std_vec_Vec2__2_hash(std_vec_Vec2__2 this);
std_vec_Vec2__2 std_vec_Vec2__2_adds(std_vec_Vec2__2 this, f64 val);
std_vec_Vec2__2 std_vec_Vec2__2_negate(std_vec_Vec2__2 this);
f32 std_vec_Vec2__2_length(std_vec_Vec2__2 this);
f64 std_vec_Vec2__2_dot(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
f64 std_vec_Vec2__2_cross(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__1 std_vec_Vec2__2_rotate(std_vec_Vec2__2 *this, f32 angle);
std_vec_Vec2__2 std_vec_Vec2__2_div(std_vec_Vec2__2 this, std_vec_Vec2__2 other);
std_vec_Vec2__2 std_vec_Vec2__2_divrs(f64 val, std_vec_Vec2__2 this);
std_vec_Vec2__1 std_vec_Vec2__1_mult(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_addrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_to_f32(std_vec_Vec2__1 this);
std_vec_Vec2__3 std_vec_Vec2__1_to_i32(std_vec_Vec2__1 this);
std_vec_Vec2__4 std_vec_Vec2__1_to_i64(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_reflect(std_vec_Vec2__1 this, std_vec_Vec2__1 normal);
std_vec_Vec2__1 std_vec_Vec2__1_divs(std_vec_Vec2__1 this, f32 val);
bool std_vec_Vec2__1_eq(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_subs(std_vec_Vec2__1 this, f32 val);
std_vec_Vec2__5 std_vec_Vec2__1_to_u32(std_vec_Vec2__1 this);
std_vec_Vec2__0 std_vec_Vec2__1_to_u64(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_mults(std_vec_Vec2__1 this, f32 val);
u32 std_vec_Vec2__1_hash(std_vec_Vec2__1 this);
std_vec_Vec2__1 std_vec_Vec2__1_adds(std_vec_Vec2__1 this, f32 val);
std_vec_Vec2__1 std_vec_Vec2__1_negate(std_vec_Vec2__1 this);
f32 std_vec_Vec2__1_length(std_vec_Vec2__1 this);
f32 std_vec_Vec2__1_dot(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
f32 std_vec_Vec2__1_cross(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_rotate(std_vec_Vec2__1 *this, f32 angle);
std_vec_Vec2__1 std_vec_Vec2__1_div(std_vec_Vec2__1 this, std_vec_Vec2__1 other);
std_vec_Vec2__1 std_vec_Vec2__1_divrs(f32 val, std_vec_Vec2__1 this);
std_vec_Vec2__0 std_vec_Vec2__0_multrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_add(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
u64 std_vec_Vec2__0_length_sq(std_vec_Vec2__0 this);
std_vec_Vec2__2 std_vec_Vec2__0_to_f64(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_mult(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__0 std_vec_Vec2__0_addrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec2__1 std_vec_Vec2__0_to_f32(std_vec_Vec2__0 this);
std_vec_Vec2__3 std_vec_Vec2__0_to_i32(std_vec_Vec2__0 this);
std_vec_Vec2__4 std_vec_Vec2__0_to_i64(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_reflect(std_vec_Vec2__0 this, std_vec_Vec2__0 normal);
std_vec_Vec2__0 std_vec_Vec2__0_divs(std_vec_Vec2__0 this, u64 val);
bool std_vec_Vec2__0_eq(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__0 std_vec_Vec2__0_subs(std_vec_Vec2__0 this, u64 val);
std_vec_Vec2__5 std_vec_Vec2__0_to_u32(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_to_u64(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_mults(std_vec_Vec2__0 this, u64 val);
u32 std_vec_Vec2__0_hash(std_vec_Vec2__0 this);
std_vec_Vec2__0 std_vec_Vec2__0_adds(std_vec_Vec2__0 this, u64 val);
std_vec_Vec2__0 std_vec_Vec2__0_negate(std_vec_Vec2__0 this);
f32 std_vec_Vec2__0_length(std_vec_Vec2__0 this);
u64 std_vec_Vec2__0_dot(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
u64 std_vec_Vec2__0_cross(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__1 std_vec_Vec2__0_rotate(std_vec_Vec2__0 *this, f32 angle);
std_vec_Vec2__0 std_vec_Vec2__0_div(std_vec_Vec2__0 this, std_vec_Vec2__0 other);
std_vec_Vec2__0 std_vec_Vec2__0_divrs(u64 val, std_vec_Vec2__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_subrs(f32 val, std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_sub(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_rotateX(std_vec_Vec3__0 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__0_normalized(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_multrs(f32 val, std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_add(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
f32 std_vec_Vec3__0_length_sq(std_vec_Vec3__0 this);
std_vec_Vec3__1 std_vec_Vec3__0_to_f64(std_vec_Vec3__0 this);
std_vec_Vec3__1 std_vec_Vec3__1_subrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_sub(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__0 std_vec_Vec3__1_rotateX(std_vec_Vec3__1 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__1_normalized(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_multrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_add(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
f64 std_vec_Vec3__1_length_sq(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_to_f64(std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__1_rotateY(std_vec_Vec3__1 *this, f32 angle);
std_vec_Vec3__1 std_vec_Vec3__1_mult(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_addrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__1_to_f32(std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__1_rotateZ(std_vec_Vec3__1 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__1_to_i32(std_vec_Vec3__1 this);
std_vec_Vec3__2 std_vec_Vec3__2_subrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_sub(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__0 std_vec_Vec3__2_rotateX(std_vec_Vec3__2 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__2_normalized(std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_multrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_add(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
i32 std_vec_Vec3__2_length_sq(std_vec_Vec3__2 this);
std_vec_Vec3__1 std_vec_Vec3__2_to_f64(std_vec_Vec3__2 this);
std_vec_Vec3__0 std_vec_Vec3__2_rotateY(std_vec_Vec3__2 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__2_mult(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_addrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__0 std_vec_Vec3__2_to_f32(std_vec_Vec3__2 this);
std_vec_Vec3__0 std_vec_Vec3__2_rotateZ(std_vec_Vec3__2 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__2_to_i32(std_vec_Vec3__2 this);
std_vec_Vec3__3 std_vec_Vec3__2_to_i64(std_vec_Vec3__2 this);
std_vec_Vec3__3 std_vec_Vec3__3_subrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_sub(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__0 std_vec_Vec3__3_rotateX(std_vec_Vec3__3 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__3_normalized(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_multrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_add(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
i64 std_vec_Vec3__3_length_sq(std_vec_Vec3__3 this);
std_vec_Vec3__1 std_vec_Vec3__3_to_f64(std_vec_Vec3__3 this);
std_vec_Vec3__0 std_vec_Vec3__3_rotateY(std_vec_Vec3__3 *this, f32 angle);
std_vec_Vec3__3 std_vec_Vec3__3_mult(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_addrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__0 std_vec_Vec3__3_to_f32(std_vec_Vec3__3 this);
std_vec_Vec3__0 std_vec_Vec3__3_rotateZ(std_vec_Vec3__3 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__3_to_i32(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_to_i64(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_reflect(std_vec_Vec3__3 this, std_vec_Vec3__3 normal);
std_vec_Vec3__3 std_vec_Vec3__3_divs(std_vec_Vec3__3 this, i64 val);
bool std_vec_Vec3__3_eq(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_subs(std_vec_Vec3__3 this, i64 val);
std_vec_Vec3__4 std_vec_Vec3__3_to_u32(std_vec_Vec3__3 this);
std_vec_Vec3__4 std_vec_Vec3__4_subrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_sub(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__0 std_vec_Vec3__4_rotateX(std_vec_Vec3__4 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__4_normalized(std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_multrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_add(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
u32 std_vec_Vec3__4_length_sq(std_vec_Vec3__4 this);
std_vec_Vec3__1 std_vec_Vec3__4_to_f64(std_vec_Vec3__4 this);
std_vec_Vec3__0 std_vec_Vec3__4_rotateY(std_vec_Vec3__4 *this, f32 angle);
std_vec_Vec3__4 std_vec_Vec3__4_mult(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_addrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__0 std_vec_Vec3__4_to_f32(std_vec_Vec3__4 this);
std_vec_Vec3__0 std_vec_Vec3__4_rotateZ(std_vec_Vec3__4 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__4_to_i32(std_vec_Vec3__4 this);
std_vec_Vec3__3 std_vec_Vec3__4_to_i64(std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_reflect(std_vec_Vec3__4 this, std_vec_Vec3__4 normal);
std_vec_Vec3__4 std_vec_Vec3__4_divs(std_vec_Vec3__4 this, u32 val);
bool std_vec_Vec3__4_eq(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_subs(std_vec_Vec3__4 this, u32 val);
std_vec_Vec3__4 std_vec_Vec3__4_to_u32(std_vec_Vec3__4 this);
std_vec_Vec3__5 std_vec_Vec3__4_to_u64(std_vec_Vec3__4 this);
std_vec_Vec3__5 std_vec_Vec3__5_subrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_sub(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__0 std_vec_Vec3__5_rotateX(std_vec_Vec3__5 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__5_normalized(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_multrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_add(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
u64 std_vec_Vec3__5_length_sq(std_vec_Vec3__5 this);
std_vec_Vec3__1 std_vec_Vec3__5_to_f64(std_vec_Vec3__5 this);
std_vec_Vec3__0 std_vec_Vec3__5_rotateY(std_vec_Vec3__5 *this, f32 angle);
std_vec_Vec3__5 std_vec_Vec3__5_mult(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_addrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__0 std_vec_Vec3__5_to_f32(std_vec_Vec3__5 this);
std_vec_Vec3__0 std_vec_Vec3__5_rotateZ(std_vec_Vec3__5 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__5_to_i32(std_vec_Vec3__5 this);
std_vec_Vec3__3 std_vec_Vec3__5_to_i64(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_reflect(std_vec_Vec3__5 this, std_vec_Vec3__5 normal);
std_vec_Vec3__5 std_vec_Vec3__5_divs(std_vec_Vec3__5 this, u64 val);
bool std_vec_Vec3__5_eq(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_subs(std_vec_Vec3__5 this, u64 val);
std_vec_Vec3__4 std_vec_Vec3__5_to_u32(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_to_u64(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_mults(std_vec_Vec3__5 this, u64 val);
u32 std_vec_Vec3__5_hash(std_vec_Vec3__5 this);
std_vec_Vec3__5 std_vec_Vec3__5_adds(std_vec_Vec3__5 this, u64 val);
std_vec_Vec3__5 std_vec_Vec3__5_negate(std_vec_Vec3__5 this);
f32 std_vec_Vec3__5_length(std_vec_Vec3__5 this);
u64 std_vec_Vec3__5_dot(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
u64 std_vec_Vec3__5_index(std_vec_Vec3__5 this, u32 idx);
std_vec_Vec3__5 std_vec_Vec3__5_cross(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_div(std_vec_Vec3__5 this, std_vec_Vec3__5 other);
std_vec_Vec3__5 std_vec_Vec3__5_divrs(u64 val, std_vec_Vec3__5 this);
std_vec_Vec3__4 std_vec_Vec3__4_mults(std_vec_Vec3__4 this, u32 val);
u32 std_vec_Vec3__4_hash(std_vec_Vec3__4 this);
std_vec_Vec3__4 std_vec_Vec3__4_adds(std_vec_Vec3__4 this, u32 val);
std_vec_Vec3__4 std_vec_Vec3__4_negate(std_vec_Vec3__4 this);
f32 std_vec_Vec3__4_length(std_vec_Vec3__4 this);
u32 std_vec_Vec3__4_dot(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
u32 std_vec_Vec3__4_index(std_vec_Vec3__4 this, u32 idx);
std_vec_Vec3__4 std_vec_Vec3__4_cross(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_div(std_vec_Vec3__4 this, std_vec_Vec3__4 other);
std_vec_Vec3__4 std_vec_Vec3__4_divrs(u32 val, std_vec_Vec3__4 this);
std_vec_Vec3__5 std_vec_Vec3__3_to_u64(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_mults(std_vec_Vec3__3 this, i64 val);
u32 std_vec_Vec3__3_hash(std_vec_Vec3__3 this);
std_vec_Vec3__3 std_vec_Vec3__3_adds(std_vec_Vec3__3 this, i64 val);
std_vec_Vec3__3 std_vec_Vec3__3_negate(std_vec_Vec3__3 this);
f32 std_vec_Vec3__3_length(std_vec_Vec3__3 this);
i64 std_vec_Vec3__3_dot(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
i64 std_vec_Vec3__3_index(std_vec_Vec3__3 this, u32 idx);
std_vec_Vec3__3 std_vec_Vec3__3_cross(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_div(std_vec_Vec3__3 this, std_vec_Vec3__3 other);
std_vec_Vec3__3 std_vec_Vec3__3_divrs(i64 val, std_vec_Vec3__3 this);
std_vec_Vec3__2 std_vec_Vec3__2_reflect(std_vec_Vec3__2 this, std_vec_Vec3__2 normal);
std_vec_Vec3__2 std_vec_Vec3__2_divs(std_vec_Vec3__2 this, i32 val);
bool std_vec_Vec3__2_eq(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_subs(std_vec_Vec3__2 this, i32 val);
std_vec_Vec3__4 std_vec_Vec3__2_to_u32(std_vec_Vec3__2 this);
std_vec_Vec3__5 std_vec_Vec3__2_to_u64(std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_mults(std_vec_Vec3__2 this, i32 val);
u32 std_vec_Vec3__2_hash(std_vec_Vec3__2 this);
std_vec_Vec3__2 std_vec_Vec3__2_adds(std_vec_Vec3__2 this, i32 val);
std_vec_Vec3__2 std_vec_Vec3__2_negate(std_vec_Vec3__2 this);
f32 std_vec_Vec3__2_length(std_vec_Vec3__2 this);
i32 std_vec_Vec3__2_dot(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
i32 std_vec_Vec3__2_index(std_vec_Vec3__2 this, u32 idx);
std_vec_Vec3__2 std_vec_Vec3__2_cross(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_div(std_vec_Vec3__2 this, std_vec_Vec3__2 other);
std_vec_Vec3__2 std_vec_Vec3__2_divrs(i32 val, std_vec_Vec3__2 this);
std_vec_Vec3__3 std_vec_Vec3__1_to_i64(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_reflect(std_vec_Vec3__1 this, std_vec_Vec3__1 normal);
std_vec_Vec3__1 std_vec_Vec3__1_divs(std_vec_Vec3__1 this, f64 val);
bool std_vec_Vec3__1_eq(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_subs(std_vec_Vec3__1 this, f64 val);
std_vec_Vec3__4 std_vec_Vec3__1_to_u32(std_vec_Vec3__1 this);
std_vec_Vec3__5 std_vec_Vec3__1_to_u64(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_mults(std_vec_Vec3__1 this, f64 val);
u32 std_vec_Vec3__1_hash(std_vec_Vec3__1 this);
std_vec_Vec3__1 std_vec_Vec3__1_adds(std_vec_Vec3__1 this, f64 val);
std_vec_Vec3__1 std_vec_Vec3__1_negate(std_vec_Vec3__1 this);
f32 std_vec_Vec3__1_length(std_vec_Vec3__1 this);
f64 std_vec_Vec3__1_dot(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
f64 std_vec_Vec3__1_index(std_vec_Vec3__1 this, u32 idx);
std_vec_Vec3__1 std_vec_Vec3__1_cross(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_div(std_vec_Vec3__1 this, std_vec_Vec3__1 other);
std_vec_Vec3__1 std_vec_Vec3__1_divrs(f64 val, std_vec_Vec3__1 this);
std_vec_Vec3__0 std_vec_Vec3__0_rotateY(std_vec_Vec3__0 *this, f32 angle);
std_vec_Vec3__0 std_vec_Vec3__0_mult(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_addrs(f32 val, std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_to_f32(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_rotateZ(std_vec_Vec3__0 *this, f32 angle);
std_vec_Vec3__2 std_vec_Vec3__0_to_i32(std_vec_Vec3__0 this);
std_vec_Vec3__3 std_vec_Vec3__0_to_i64(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_reflect(std_vec_Vec3__0 this, std_vec_Vec3__0 normal);
std_vec_Vec3__0 std_vec_Vec3__0_divs(std_vec_Vec3__0 this, f32 val);
bool std_vec_Vec3__0_eq(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_subs(std_vec_Vec3__0 this, f32 val);
std_vec_Vec3__4 std_vec_Vec3__0_to_u32(std_vec_Vec3__0 this);
std_vec_Vec3__5 std_vec_Vec3__0_to_u64(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_mults(std_vec_Vec3__0 this, f32 val);
u32 std_vec_Vec3__0_hash(std_vec_Vec3__0 this);
std_vec_Vec3__0 std_vec_Vec3__0_adds(std_vec_Vec3__0 this, f32 val);
std_vec_Vec3__0 std_vec_Vec3__0_negate(std_vec_Vec3__0 this);
f32 std_vec_Vec3__0_length(std_vec_Vec3__0 this);
f32 std_vec_Vec3__0_dot(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
f32 std_vec_Vec3__0_index(std_vec_Vec3__0 this, u32 idx);
std_vec_Vec3__0 std_vec_Vec3__0_cross(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_div(std_vec_Vec3__0 this, std_vec_Vec3__0 other);
std_vec_Vec3__0 std_vec_Vec3__0_divrs(f32 val, std_vec_Vec3__0 this);
f32 f32_inf(void);
f64 f64_inf(void);
f32 f32_min(f32 this, f32 b);
f32 f32_max(f32 this, f32 b);
f64 f64_min(f64 this, f64 b);
f64 f64_max(f64 this, f64 b);
f32 f32_clamp(f32 this, f32 min, f32 max);
f32 f32_clamp01(f32 this);
f64 f64_clamp(f64 this, f64 min, f64 max);
f64 f64_clamp01(f64 this);
bool f32_is_close(f32 this, f32 other, f32 thresh);
bool f64_is_close(f64 this, f64 other, f64 thresh);
i8 i8_abs(i8 this);
i16 i16_abs(i16 this);
i32 i32_abs(i32 this);
i64 i64_abs(i64 this);
f32 std_math_degrees(f32 radians);
f32 std_math_radians(f32 degrees);
std_buffer_Buffer std_buffer_Buffer_make(u32 capacity);
std_buffer_Buffer std_buffer_Buffer_from_str(char *s);
std_buffer_Buffer std_buffer_Buffer_from_sv(std_sv_SV sv);
std_buffer_Buffer std_buffer_Buffer_from_sized_str(char *s, u32 size);
void std_buffer_Buffer_resize_if_necessary(std_buffer_Buffer *this, u32 new_size);
void std_buffer_hex_dump(u8 *data, u32 size);
void std_buffer_Buffer_hex_dump(std_buffer_Buffer this);
u8 std_buffer_Buffer_get_byte_at(std_buffer_Buffer this, u32 index);
void std_buffer_Buffer_write_buf(std_buffer_Buffer *this, std_buffer_Buffer *buf);
void std_buffer_Buffer_write_buf_f(std_buffer_Buffer *this, std_buffer_Buffer *buf);
void std_buffer_Buffer_write_str(std_buffer_Buffer *this, char *s);
void std_buffer_Buffer_write_str_f(std_buffer_Buffer *this, char *s);
void std_buffer_Buffer_write_char(std_buffer_Buffer *this, char c);
void std_buffer_Buffer_write_bytes(std_buffer_Buffer *this, void *bytes, u32 size);
void std_buffer_Buffer_write_sv(std_buffer_Buffer *this, std_sv_SV sv);
void std_buffer_Buffer_write_i64(std_buffer_Buffer *this, i64 value, std_Endian endian);
void std_buffer_Buffer_write_u64(std_buffer_Buffer *this, u64 value, std_Endian endian);
void std_buffer_Buffer_write_i32(std_buffer_Buffer *this, i32 value, std_Endian endian);
void std_buffer_Buffer_write_u32(std_buffer_Buffer *this, u32 value, std_Endian endian);
void std_buffer_Buffer_write_i16(std_buffer_Buffer *this, i16 value, std_Endian endian);
void std_buffer_Buffer_write_u16(std_buffer_Buffer *this, u16 value, std_Endian endian);
void std_buffer_Buffer_write_i8(std_buffer_Buffer *this, i8 value);
void std_buffer_Buffer_write_u8(std_buffer_Buffer *this, u8 value);
std_buffer_BytesReader std_buffer_Buffer_reader(std_buffer_Buffer this);
char *std_buffer_Buffer_new_str(std_buffer_Buffer this);
char *std_buffer_Buffer_str(std_buffer_Buffer this);
std_sv_SV std_buffer_Buffer_sv(std_buffer_Buffer this);
std_buffer_Buffer std_buffer_Buffer_copy(std_buffer_Buffer *this);
void std_buffer_Buffer_clear(std_buffer_Buffer *this);
void std_buffer_Buffer_free(std_buffer_Buffer *this);
std_buffer_BytesReader std_buffer_BytesReader_make(u8 *data, u32 size);
i64 std_buffer_BytesReader_read_i64(std_buffer_BytesReader *this, std_Endian endian);
u64 std_buffer_BytesReader_read_u64(std_buffer_BytesReader *this, std_Endian endian);
i32 std_buffer_BytesReader_read_i32(std_buffer_BytesReader *this, std_Endian endian);
u32 std_buffer_BytesReader_read_u32(std_buffer_BytesReader *this, std_Endian endian);
i16 std_buffer_BytesReader_read_i16(std_buffer_BytesReader *this, std_Endian endian);
u16 std_buffer_BytesReader_read_u16(std_buffer_BytesReader *this, std_Endian endian);
i8 std_buffer_BytesReader_read_i8(std_buffer_BytesReader *this);
u8 std_buffer_BytesReader_read_u8(std_buffer_BytesReader *this);
void std_buffer_BytesReader_read_bytes(std_buffer_BytesReader *this, void *_dst, u32 count);
std_sv_SV std_buffer_BytesReader_read_bytes_sv(std_buffer_BytesReader *this, u32 count);
bool std_buffer_BytesReader_is_empty(std_buffer_BytesReader *this);
std_buffer_Buffer std_zlib_compress(std_buffer_Buffer data);
std_buffer_Buffer std_zlib_compress_str(char *s);
std_buffer_Buffer std_zlib_decompress_bytes(u8 *data, u32 size, u32 *bytes_consumed);
std_buffer_Buffer std_zlib_decompress(std_buffer_Buffer data, u32 *bytes_consumed);
u32 std_traits_hash_hash_bytes(u8 *data, u32 len);
u32 std_traits_hash_pair_hash(u32 a, u32 b);
u32 std_traits_hash_ptr_hash(void *ptr);
u32 str_hash(char *this);
u32 u8_hash(u8 this);
u32 i8_hash(i8 this);
u32 u16_hash(u16 this);
u32 i16_hash(i16 this);
u32 u32_hash(u32 this);
u32 i32_hash(i32 this);
u32 u64_hash(u64 this);
u32 i64_hash(i64 this);
u32 char_hash(char this);
u32 f32_hash(f32 this);
u32 f64_hash(f64 this);
std_og_Rect std_og_Rect_from_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size);
bool std_og_Rect_contains(std_og_Rect this, std_vec_Vec2__3 pos);
u32 std_og_get_ticks(void);
void std_og_init(u32 width, u32 height, char *title, bool fullscreen);
void std_og_toggle_fullscreen(void);
bool std_og_is_running(void);
void std_og_clear(SDL_Color col);
void std_og_draw_rect(i32 x, i32 y, i32 w, i32 h, SDL_Color col);
void std_og_draw_rect_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col);
void std_og_draw_line(i32 x1, i32 y1, i32 x2, i32 y2, SDL_Color col);
void std_og_draw_line_v(std_vec_Vec2__3 p1, std_vec_Vec2__3 p2, SDL_Color col);
void std_og_draw_pixel(i32 x, i32 y, SDL_Color col);
void std_og_draw_pixel_v(std_vec_Vec2__3 pos, SDL_Color col);
f32 std_og_get_frame_time(void);
void std_og_quit(void);
bool std_og_is_key_pressed(SDL_Scancode k);
bool std_og_is_key_released(SDL_Scancode k);
bool std_og_is_key_down(SDL_Scancode k);
bool std_og_is_key_up(SDL_Scancode k);
bool std_og_is_mouse_pressed(u8 b);
bool std_og_is_mouse_released(u8 b);
bool std_og_is_mouse_down(u8 b);
bool std_og_is_mouse_up(u8 b);
std_vec_Vec2__3 std_og_get_window_size(void);
bool std_og_is_scroll(std_og_state_MouseWheel w);
std_vec_Vec2__3 std_og_get_mouse_pos(void);
std_vec_Vec2__3 std_og_get_mouse_delta(void);
void std_og_capture_mouse(bool capture);
void std_og_grab_input(bool grab);
void std_og_show_cursor(bool show);
void std_og_set_cursor(SDL_SystemCursor c);
void std_og_Texture_draw_pixel(std_og_Texture this, i32 x, i32 y, SDL_Color col);
void std_og_Texture_draw_rect(std_og_Texture this, i32 x, i32 y, i32 w, i32 h, SDL_Color col);
void std_og_Texture_draw_rect_v(std_og_Texture this, std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col);
void std_og_Texture_draw_pixel_v(std_og_Texture this, std_vec_Vec2__3 pos, SDL_Color col);
std_og_Texture *std_og_get_buffer(void);
void std_og_draw_buffer(void);
void std_og_set_main_loop(bool (*callback)(void));
bool std_og_state_KeyState_get(std_og_state_KeyState *this, SDL_Scancode k);
bool std_og_state_MouseState_get_button(std_og_state_MouseState *this, u8 b);
bool std_og_state_MouseState_get_wheel(std_og_state_MouseState *this, std_og_state_MouseWheel w);
void std_og_state_update_window_size(void);
void std_og_state_add_frametime(f32 frametime);
f32 std_og_state_get_avg_frametime(void);
void std_og_utils_handle_sdl_events(void);
bool std_og_internal_og_main_fn(void);
Game game = {0};
void *std_mem_state_allocator = NULL;
void *(*std_mem_state_alloc_fn)(void *, u32) = std_mem_impl_my_calloc;
void *(*std_mem_state_realloc_fn)(void *, void *, u32, u32) = std_mem_impl_my_realloc;
void (*std_mem_state_free_fn)(void *, void *) = std_mem_impl_my_free;
u32 std_image_png_crc_table[256] = {0};
bool std_image_png_crc_table_computed = false;
SDL_Window *std_og_state_window = NULL;
SDL_Renderer *std_og_state_renderer = NULL;
u32 std_og_state_prev_time = 0;
f32 std_og_state_frame_time = 0.0f;
bool std_og_state_running = true;
std_vec_Vec2__3 std_og_state_window_size = {0};
char *std_og_state_title = "Window";
SDL_SystemCursor std_og_state_cursor_type = SDL_SYSTEM_CURSOR_ARROW;
SDL_Cursor *std_og_state_cursor = NULL;
std_og_state_KeyState std_og_state_prev_keys = {0};
std_og_state_KeyState std_og_state_keys = {0};
std_og_state_MouseState std_og_state_prev_mouse = {0};
std_og_state_MouseState std_og_state_mouse = {0};
f32 std_og_state_frametimes[std_og_state_FRAMETIMES_COUNT] = {0};
u32 std_og_state_frametime_index;
f32 std_og_state_frametime_total = 0.0f;
char std_og_state_window_title_fps_buf[256] = {0};
u64 std_og_state_frame_num = ((u64)0);
std_og_Texture std_og_state_buffer = (std_og_Texture){.tx=NULL, .w=0, .h=0, .pixels=NULL, .pitch=0};
bool (*std_og_internal_user_main_fn)(void) = NULL;
SDL_Color std_og_colors_RED = (SDL_Color){.r=255, .g=0, .b=0, .a=255};
SDL_Color std_og_colors_GREEN = (SDL_Color){.r=0, .g=255, .b=0, .a=255};
SDL_Color std_og_colors_BLUE = (SDL_Color){.r=0, .g=0, .b=255, .a=255};
SDL_Color std_og_colors_BLACK = (SDL_Color){.r=0, .g=0, .b=0, .a=255};
SDL_Color std_og_colors_WHITE = (SDL_Color){.r=255, .g=255, .b=255, .a=255};
SDL_Color std_og_colors_YELLOW = (SDL_Color){.r=255, .g=255, .b=0, .a=255};
SDL_Color std_og_colors_CYAN = (SDL_Color){.r=0, .g=255, .b=255, .a=255};
/* function implementations */
Game Game_make(u32 rows, u32 cols, u32 grid_size) {
  CellType *grid = std_mem_alloc__0((rows * cols));
  Game game = (Game){.rows=rows, .cols=cols, .grid_size=grid_size, .grid=grid, .angle=0.0f, .fov=std_math_radians(60.0f), .pos=(std_vec_Vec2__1){.x=(((f32)rows) / 2.0f), .y=(((f32)cols) / 2.0f)}, .jump=0.0f, .up_vel=0.0f, .map_enabled=false, .wall_img=NULL, .ground_img=NULL, .tx=NULL};
  for (u32 i = 0; i < game.rows; i++) {
    Game_set_cell(&game, i, 0, CellType_Wall);
    Game_set_cell(&game, i, (game.cols - 1), CellType_Wall);
  }
  for (u32 i = 0; i < game.cols; i++) {
    Game_set_cell(&game, 0, i, CellType_Wall);
    Game_set_cell(&game, (game.rows - 1), i, CellType_Wall);
  }
  Game_set_cell(&game, 11, 11, CellType_Wall);
  Game_set_cell(&game, 12, 12, CellType_Wall);
  Game_set_cell(&game, 13, 13, CellType_Wall);
  Game_set_cell(&game, 2, 2, CellType_Wall);
  Game_set_cell(&game, 2, 3, CellType_Wall);
  Game_set_cell(&game, 2, 4, CellType_Wall);
  Game_set_cell(&game, 2, 5, CellType_Wall);
  game.wall_img=std_image_Image_load("assets/stone.png");
  game.ground_img=std_image_Image_load("assets/ground.png");
  return game;
}

void Game_draw(Game *this) {
  if (!this->map_enabled) {
    return;
  }
  for (u32 i = 0; i < this->rows; i++) {
    for (u32 j = 0; j < this->cols; j++) {
      CellType cell = this->grid[((i * this->cols) + j)];
      std_vec_Vec2__3 loc = Game_to_screen(this, (std_vec_Vec2__1){.x=((f32)j), .y=((f32)i)});
      std_og_draw_rect(((i32)loc.x), ((i32)loc.y), (((i32)Game_get_cell_size(this)) - 1), (((i32)Game_get_cell_size(this)) - 1), ({ SDL_Color __yield_0;
        switch (cell) {
          case CellType_Empty: {
            __yield_0 = (SDL_Color){.r=0, .g=0, .b=0, .a=255};
          } break;
          case CellType_Wall: {
            __yield_0 = (SDL_Color){.r=255, .g=255, .b=255, .a=255};
          } break;
        }
;__yield_0; }));
    }
  }
  if (((0.0f <= this->pos.x) && (this->pos.x < ((f32)this->cols))) && ((0.0f <= this->pos.y) && (this->pos.y < ((f32)this->rows)))) {
    std_og_draw_rect_v(std_vec_Vec2__3_subs(std_vec_Vec2__3_to_i32(Game_to_screen(this, this->pos)), 5), (std_vec_Vec2__3){.x=10, .y=10}, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
    std_vec_Vec2__1 to = std_vec_Vec2__1_add(this->pos, std_vec_Vec2__1_rotate(&(std_vec_Vec2__1){.x=0.5f, .y=0.0f}, this->angle));
    Game_draw_ray(this, this->pos, to, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  }
}

void Game_set_cell(Game *this, u32 row, u32 col, CellType cell) {
  this->grid[((row * this->cols) + col)]=cell;
}

bool Game_ray_check_intersection(Game *this, f32 x, f32 y, f32 dx, f32 dy, f32 *dist, u32 *cell_idx, u32 steps) {
  for (u32 i = 0; i < steps; i++) {
    i32 cx = ((i32)x);
    i32 cy = ((i32)y);
    if (((0 <= cx) && (cx < ((i32)this->cols))) && ((0 <= cy) && (cy < ((i32)this->rows)))) {
      i32 idx = ((cy * ((i32)this->cols)) + cx);
      CellType cell = this->grid[idx];
      if (cell==CellType_Wall) {
        std_vec_Vec2__1 hit_pos = (std_vec_Vec2__1){.x=x, .y=y};
        f32 t = std_vec_Vec2__1_length(std_vec_Vec2__1_sub(hit_pos, this->pos));
        if (t < (*dist)) {
          (*dist)=t;
          (*cell_idx)=((u32)idx);
          return true;
        } else {
          return false;
        }
      }
    }
    x+=dx;
    y+=dy;
  }
  return false;
}

void Game_draw_ray(Game *this, std_vec_Vec2__1 v0, std_vec_Vec2__1 v1, SDL_Color color) {
  if (!(((0.0f <= v0.x) && (v0.x < ((f32)this->cols))) && ((0.0f <= v0.y) && (v0.y < ((f32)this->rows))))) {
    return;
  }
  if (v1.x < 0.0f) {
    v1=std_vec_Vec2__1_add(v0, std_vec_Vec2__1_mults(std_vec_Vec2__1_normalized(std_vec_Vec2__1_sub(v1, v0)), v0.x));
  }
  if (v1.x >= ((f32)this->cols)) {
    v1=std_vec_Vec2__1_add(v0, std_vec_Vec2__1_mults(std_vec_Vec2__1_normalized(std_vec_Vec2__1_sub(v1, v0)), (((f32)this->cols) - v0.x)));
  }
  if (v1.y < 0.0f) {
    v1=std_vec_Vec2__1_add(v0, std_vec_Vec2__1_mults(std_vec_Vec2__1_normalized(std_vec_Vec2__1_sub(v1, v0)), v0.y));
  }
  if (v1.y >= ((f32)this->rows)) {
    v1=std_vec_Vec2__1_add(v0, std_vec_Vec2__1_mults(std_vec_Vec2__1_normalized(std_vec_Vec2__1_sub(v1, v0)), (((f32)this->rows) - v0.y)));
  }
  std_og_draw_line_v(std_vec_Vec2__3_to_i32(Game_to_screen(this, v0)), std_vec_Vec2__3_to_i32(Game_to_screen(this, v1)), color);
}

void Game_check_ray(Game *this, i32 i, f32 angle) {
  std_vec_Vec2__1 dir = std_vec_Vec2__1_rotate(&(std_vec_Vec2__1){.x=1.0f, .y=0.0f}, angle);
  f32 tan = tanf(angle);
  bool hit = false;
  f32 hit_dist = f32_inf();
  Direction hit_side;
  u32 hit_idx;
  {
    f32 x = 0.0f;
    f32 dx = ({ f32 __yield_0;
      if (dir.x > 0.0f) {
        x=ceilf(this->pos.x);
        __yield_0 = (1.0f + EPS);
      } else if (dir.x < 0.0f) {
        x=(floorf(this->pos.x) - EPS);
        __yield_0 = (-1.0f - EPS);
      } else {
        __yield_0 = 0.0f;
      }
;__yield_0; });
    if (dx != 0.0f) {
      f32 dy = (dx * tan);
      f32 y = (this->pos.y + ((x - this->pos.x) * tan));
      if (Game_ray_check_intersection(this, x, y, dx, dy, &hit_dist, &hit_idx, this->cols)) {
        hit=true;
        hit_side=Direction_Vertical;
      }
    }
  }
  {
    f32 y = 0.0f;
    f32 dy = ({ f32 __yield_0;
      if (dir.y > 0.0f) {
        y=ceilf(this->pos.y);
        __yield_0 = (1.0f + EPS);
      } else if (dir.y < 0.0f) {
        y=(floorf(this->pos.y) - EPS);
        __yield_0 = (-1.0f - EPS);
      } else {
        __yield_0 = 0.0f;
      }
;__yield_0; });
    if (dy != 0.0f) {
      f32 dx = (dy / tan);
      f32 x = (this->pos.x + ((y - this->pos.y) / tan));
      if (Game_ray_check_intersection(this, x, y, dx, dy, &hit_dist, &hit_idx, this->cols)) {
        hit=true;
        hit_side=Direction_Horizontal;
      }
    }
  }
  std_vec_Vec2__1 hit_pos = std_vec_Vec2__1_add(this->pos, std_vec_Vec2__1_mults(dir, hit_dist));
  if (this->map_enabled) {
    if (hit) {
      Game_draw_ray(this, this->pos, hit_pos, (SDL_Color){.r=0, .g=255, .b=0, .a=255});
    }
    return;
  }
  std_vec_Vec2__3 window_size = std_og_get_window_size();
  i32 screen_height = window_size.y;
  i32 screen_width = window_size.x;
  i32 col = ((i32)((((f32)i) / ((f32)NUM_RAYS)) * ((f32)screen_width)));
  i32 width = ((i32)f32_max(ceilf((((f32)screen_width) / ((f32)NUM_RAYS))), 1.0f));
  i32 height = (hit ? ((i32)ceilf(((((f32)screen_height) / hit_dist) / cosf((angle - this->angle))))) : 0);
  i32 jump_height = ((i32)(((this->jump * ((f32)screen_height)) / hit_dist) / cosf((angle - this->angle))));
  i32 x0 = i32_max(col, 0);
  width=i32_min(width, ((i32)screen_width));
  i32 y0 = (((((i32)screen_height) / 2) - (height / 2)) + jump_height);
  height=height;
  f32 u = 0.0f;
  f32 brightness = 1.0f;
  switch (hit_side) {
    case Direction_Vertical: {
      u=fabsf(fmodf(hit_pos.y, 1.0f));
      if (dir.x < 0.0f) {
        u=(1.0f - u);
      }
      brightness=0.8f;
    } break;
    case Direction_Horizontal: {
      u=fabsf(fmodf(hit_pos.x, 1.0f));
      if (dir.y > 0.0f) {
        u=(1.0f - u);
      }
      brightness=0.5f;
    } break;
  }
  if (hit) {
    CellType cell = this->grid[hit_idx];
    std_image_Image *tex = ({ std_image_Image *__yield_0;
      switch (cell) {
        case CellType_Wall: {
          __yield_0 = this->wall_img;
        } break;
        case CellType_Empty: {
          __yield_0 = NULL;
        } break;
      }
;__yield_0; });
    i32 start = 0;
    if (y0 < 0) {
      start=-y0;
    }
    for (i32 dy = start; dy < height; dy+=width) {
      i32 y = (y0 + dy);
      if (y >= ((i32)screen_height)) {
        break;
      }
      f32 v = fabsf((((f32)dy) / ((f32)height)));
      SDL_Color color = ({ SDL_Color __yield_0;
        if (((bool)tex)) {
          std_image_Color c = std_image_Image_get(tex, ((u32)(u * ((f32)(tex->width - 1)))), ((u32)(v * ((f32)(tex->height - 1)))));
          __yield_0 = (SDL_Color){.r=((i32)(((f32)c.r) * brightness)), .g=((i32)(((f32)c.g) * brightness)), .b=((i32)(((f32)c.b) * brightness)), .a=255};
        } else {
          __yield_0 = (SDL_Color){.r=((i32)((u * 255.0f) * brightness)), .g=((i32)((v * 255.0f) * brightness)), .b=((i32)(0.0f * brightness)), .a=255};
        }
;__yield_0; });
      std_og_Texture_draw_rect((*this->tx), x0, y, width, width, color);
    }
  }
  f32 view_distance = ((0.5f + this->jump) / cosf(fmodf((this->angle - angle), std_math_TAU)));
  f32 weight = (view_distance * ((f32)screen_height));
  f32 deg = -angle;
  for (i32 y = (y0 + height); y < ((i32)screen_height); y++) {
    f32 dy = (((f32)y) - (((f32)screen_height) / 2.0f));
    f32 tx = (((f32)this->pos.x) + ((cosf(deg) * weight) / dy));
    f32 ty = (((f32)this->pos.y) - ((sinf(deg) * weight) / dy));
    i32 cell_row = ((i32)ty);
    i32 cell_col = ((i32)tx);
    f32 u = fabsf((tx - ((f32)cell_col)));
    f32 v = fabsf((ty - ((f32)cell_row)));
    std_image_Color c = std_image_Image_get(this->ground_img, ((u32)(u * ((f32)(this->ground_img->width - 1)))), ((u32)(v * ((f32)(this->ground_img->height - 1)))));
    std_og_Texture_draw_rect((*this->tx), x0, y, width, 1, (SDL_Color){.r=((i32)c.r), .g=((i32)c.g), .b=((i32)c.b), .a=255});
  }
}

std_vec_Vec2__3 Game_to_screen(Game *this, std_vec_Vec2__1 v) {
  f32 cell_size = ((f32)Game_get_cell_size(this));
  std_vec_Vec2__3 res = std_vec_Vec2__1_to_i32(std_vec_Vec2__1_mults(v, cell_size));
  std_vec_Vec2__3 window_sz = std_og_get_window_size();
  f32 grid_width = (((f32)this->cols) * cell_size);
  f32 grid_height = (((f32)this->rows) * cell_size);
  if (grid_width < ((f32)window_sz.x)) {
    res.x+=(((i32)(((f32)window_sz.x) - grid_width)) / 2);
  }
  if (grid_height < ((f32)window_sz.y)) {
    res.y+=(((i32)(((f32)window_sz.y) - grid_height)) / 2);
  }
  return res;
}

void Game_cast_rays(Game *this) {
  f32 start_angle = (this->angle - (this->fov / 2.0f));
  f32 angle_step = (this->fov / ((f32)NUM_RAYS));
  for (i32 i = 0; i < NUM_RAYS; i++) {
    f32 angle = (start_angle + (((f32)i) * angle_step));
    Game_check_ray(this, i, angle);
  }
}

i32 Game_get_cell_size(Game *this) {
  std_vec_Vec2__3 window_size = std_og_get_window_size();
  i32 col_size = (window_size.x / ((i32)this->cols));
  i32 row_size = (window_size.y / ((i32)this->rows));
  return ((i32)i32_min(col_size, row_size));
}

void Game_highlight_cell(Game *this, u32 row, u32 col) {
  if (!this->map_enabled) {
    return;
  }
  std_vec_Vec2__3 pos = Game_to_screen(this, (std_vec_Vec2__1){.x=((f32)col), .y=((f32)row)});
  i32 x = ((i32)pos.x);
  i32 y = ((i32)pos.y);
  i32 N = ((i32)Game_get_cell_size(this));
  i32 T = CELL_HIGHIGHT_THICKNESS;
  i32 L = CELL_HIGHIGHT_LENGTH;
  std_og_draw_rect((x - T), (y - T), T, L, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  std_og_draw_rect((x - T), (y - T), L, T, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  std_og_draw_rect((((x + N) - L) + T), (y - T), L, T, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  std_og_draw_rect((x + N), (y - T), T, L, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  std_og_draw_rect((x - T), (((y + N) - L) + T), T, L, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  std_og_draw_rect((x - T), (y + N), L, T, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  std_og_draw_rect((((x + N) - L) + T), (y + N), L, T, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
  std_og_draw_rect((x + N), (((y + N) - L) + T), T, L, (SDL_Color){.r=255, .g=0, .b=0, .a=255});
}

void Game_toggle_cell(Game *this, u32 row, u32 col) {
  CellType cell = this->grid[((row * this->cols) + col)];
  this->grid[((row * this->cols) + col)]=({ CellType __yield_0;
    switch (cell) {
      case CellType_Empty: {
        __yield_0 = CellType_Wall;
      } break;
      case CellType_Wall: {
        __yield_0 = CellType_Empty;
      } break;
    }
;__yield_0; });
}

std_vec_Vec2__1 Game_to_world(Game *this, std_vec_Vec2__3 v) {
  std_vec_Vec2__3 window_size = std_og_get_window_size();
  f32 cell_size = ((f32)Game_get_cell_size(this));
  f32 grid_width = (((f32)this->cols) * cell_size);
  f32 grid_height = (((f32)this->rows) * cell_size);
  if (grid_width < ((f32)window_size.x)) {
    v.x-=(((i32)(((f32)window_size.x) - grid_width)) / 2);
  }
  if (grid_height < ((f32)window_size.y)) {
    v.y-=(((i32)(((f32)window_size.y) - grid_height)) / 2);
  }
  return std_vec_Vec2__1_divs(std_vec_Vec2__3_to_f32(v), cell_size);
}

CellType Game_get_cell_or_empty(Game *this, i32 x, i32 y) {
  if (((0 <= x) && (x < ((i32)this->cols))) && ((0 <= y) && (y < ((i32)this->rows)))) {
    return this->grid[((y * ((i32)this->cols)) + x)];
  }
  return CellType_Empty;
}

void Game_move(Game *this, f32 angle_off, f32 dt) {
  std_vec_Vec2__1 dir = std_vec_Vec2__1_rotate(&(std_vec_Vec2__1){.x=1.0f, .y=0.0f}, (this->angle + angle_off));
  std_vec_Vec2__3 pos = std_vec_Vec2__1_to_i32(this->pos);
  std_vec_Vec2__3 tgt = std_vec_Vec2__1_to_i32(std_vec_Vec2__1_add(this->pos, std_vec_Vec2__1_mults(dir, 0.2f)));
  if (Game_get_cell_or_empty(this, pos.x, tgt.y)==CellType_Empty) {
    this->pos.y+=((dir.y * MOVE_SPEED) * dt);
  }
  if (Game_get_cell_or_empty(this, tgt.x, pos.y)==CellType_Empty) {
    this->pos.x+=((dir.x * MOVE_SPEED) * dt);
  }
}

bool Game_step(Game *this) {
  f32 dt = std_og_get_frame_time();
  if (std_og_is_key_pressed(SDL_SCANCODE_ESCAPE)) {
    return false;
  }
  std_vec_Vec2__3 mouse_delta = std_og_get_mouse_delta();
  if (!this->map_enabled) {
    this->angle+=(((((f32)mouse_delta.x) * TURN_SPEED) * MOUSE_TURN_MULTIPLIER) * dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_W)) {
    Game_move(this, 0.0f, dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_S)) {
    Game_move(this, M_PI, dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_A)) {
    Game_move(this, (-M_PI / 2.0f), dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_D)) {
    Game_move(this, (M_PI / 2.0f), dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_F)) {
    std_og_toggle_fullscreen();
  }
  if (std_og_is_key_down(SDL_SCANCODE_LEFT)) {
    this->angle-=(TURN_SPEED * dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_RIGHT)) {
    this->angle+=(TURN_SPEED * dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_UP)) {
    Game_move(this, 0.0f, dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_DOWN)) {
    Game_move(this, M_PI, dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_COMMA)) {
    this->fov+=((this->fov * FOV_ZOOM_FACTOR) * dt);
  }
  if (std_og_is_key_down(SDL_SCANCODE_PERIOD)) {
    this->fov-=((this->fov / FOV_ZOOM_FACTOR) * dt);
  }
  if (std_og_is_key_pressed(SDL_SCANCODE_M)) {
    this->map_enabled=!this->map_enabled;
    if (this->map_enabled) {
      std_og_grab_input(false);
      std_og_show_cursor(true);
    } else {
      std_og_grab_input(true);
      std_og_show_cursor(false);
    }
  }
  if (std_og_is_key_pressed(SDL_SCANCODE_SPACE) && this->jump==0.0f) {
    this->up_vel+=JUMP_VEL;
  }
  this->jump+=(this->up_vel * dt);
  this->up_vel-=(GRAVITY * dt);
  if (this->jump <= 0.0f) {
    this->jump=0.0f;
    this->up_vel=0.0f;
  }
  std_og_clear((SDL_Color){.r=24, .g=24, .b=24, .a=255});
  if (!this->map_enabled) {
    this->tx=std_og_get_buffer();
    memset(this->tx->pixels, ((u8)0x18), ((u32)(this->tx->pitch * this->tx->h)));
  }
  Game_draw(this);
  Game_cast_rays(this);
  if (this->map_enabled) {
    std_vec_Vec2__1 mouse = Game_to_world(this, std_og_get_mouse_pos());
    if (((0.0f <= mouse.x) && (mouse.x < ((f32)this->cols))) && ((0.0f <= mouse.y) && (mouse.y < ((f32)this->rows)))) {
      u32 row = ((u32)mouse.y);
      u32 col = ((u32)mouse.x);
      Game_highlight_cell(this, row, col);
      if (std_og_is_mouse_pressed(SDL_BUTTON_LEFT)) {
        Game_toggle_cell(this, row, col);
      }
    }
  }
  if (!this->map_enabled) {
    std_og_draw_buffer();
  }
  return true;
}

bool step(void) {
  return Game_step(&game);
}

i32 main(void) {
  game=Game_make(20, 20, 0);
  std_og_init(960, 640, "RayCast", false);
  std_og_grab_input(true);
  std_og_show_cursor(false);
  std_og_set_main_loop(step);
  std_og_quit();
}

void std_panic(char *msg) {
  printf("%s""\n", msg);
  dump_backtrace();
  oc_trap();
  exit(1);
}

u32 str_to_u32(char *this) {
  return ((u32)atoi(this));
}

bool str_eq(char *this, char *other) {
  void *a = ((void *)this);
  void *b = ((void *)other);
  if (a==b) {
    return true;
  }
  if (!(((bool)a) && ((bool)b))) {
    return false;
  }
  return strcmp(this, other)==0;
}

char *std_format(char *fmt, ...) {
  va_list args = {0};
  va_start(args, fmt);
  u32 size = vsnprintf(NULL, 0, fmt, args);
  va_end(args);
  va_start(args, fmt);
  char *s = std_mem_alloc__1(((u32)(size + 1)));
  vsnprintf(s, (size + 1), fmt, args);
  s[size]='\0';
  va_end(args);
  return s;
}

char *str_substring(char *this, u32 start, u32 len) {
  char *new_str = std_mem_alloc__1(((u32)(len + 1)));
  memcpy(new_str, &this[start], len);
  return new_str;
}

bool str_ends_with(char *this, char *suffix) {
  u32 suffix_len = strlen(suffix);
  u32 len = strlen(this);
  if (suffix_len > len) {
    return false;
  }
  for (u32 i = 0; i < suffix_len; i+=1) {
    if (this[((len - suffix_len) + i)] != suffix[i]) {
      return false;
    }
  }
  return true;
}

bool str_starts_with(char *this, char *prefix) {
  u32 prefix_len = strlen(prefix);
  u32 len = strlen(this);
  if (prefix_len > len) {
    return false;
  }
  for (u32 i = 0; i < prefix_len; i+=1) {
    if (this[i] != prefix[i]) {
      return false;
    }
  }
  return true;
}

void str_strip_trailing_whitespace(char *this) {
  for (u32 i = (strlen(this) - 1); i >= 0; i-=1) {
    if (this[i] != ' ') {
      break;
    }
    this[i]='\0';
  }
}

void str_remove_last_n(char *this, u32 n) {
  this[(strlen(this) - n)]='\0';
}

void str_replace_with(char **this, char *other) {
  std_mem_free((*this));
  (*this)=other;
}

void str_free(char **this) {
  std_mem_free((*this));
  (*this)=NULL;
}

std_CharIterator str_chars(char *this, u32 start) {
  return (std_CharIterator){.data=this, .len=strlen(this), .pos=start};
}

bool std_CharIterator_has_value(std_CharIterator *this) {
  return this->pos < this->len;
}

u32 std_CharIterator_next(std_CharIterator *this) {
  return this->pos++;
}

char std_CharIterator_cur(std_CharIterator *this) {
  return this->data[this->pos];
}

bool char_is_digit(char this) {
  return isdigit(this);
}

bool char_is_hex_digit(char this) {
  return isxdigit(this);
}

u32 char_get_digit(char this) {
  return ((u32)this) - ((u32)'0');
}

u8 char_get_hex_digit(char this) {
  if (('0' <= this) && (this <= '9')) {
    return ((u8)this) - ((u8)'0');
  }
  if (('a' <= this) && (this <= 'f')) {
    return (((u8)this) - ((u8)'a')) + ((u8)10);
  }
  if (('A' <= this) && (this <= 'F')) {
    return (((u8)this) - ((u8)'A')) + ((u8)10);
  }
  std_panic(std_format("[-] Invalid hex digit: '%c' (0x%02x)", this, this));
}

bool char_is_alpha(char this) {
  return isalpha(this);
}

bool char_is_alnum(char this) {
  return isalnum(this);
}

bool char_is_print(char this) {
  return isprint(this);
}

bool char_is_space(char this) {
  return isspace(this);
}

bool char_is_upper(char this) {
  return isupper(this);
}

bool char_is_lower(char this) {
  return islower(this);
}

i32 i32_min(i32 this, i32 b) {
  return ((this < b) ? this : b);
}

i32 i32_max(i32 this, i32 b) {
  return ((this > b) ? this : b);
}

i8 i8_min(i8 this, i8 b) {
  return ((this < b) ? this : b);
}

i8 i8_max(i8 this, i8 b) {
  return ((this > b) ? this : b);
}

u64 u64_min(u64 this, u64 other) {
  return ((this < other) ? this : other);
}

u64 u64_max(u64 this, u64 other) {
  return ((this > other) ? this : other);
}

u32 u32_min(u32 this, u32 other) {
  return ((this < other) ? this : other);
}

u32 u32_max(u32 this, u32 other) {
  return ((this > other) ? this : other);
}

u8 u8_min(u8 this, u8 other) {
  return ((this < other) ? this : other);
}

u8 u8_max(u8 this, u8 other) {
  return ((this > other) ? this : other);
}

char *std_shift_args(i32 *argc, char ***argv, char *where) {
  if ((*argc)==0) {
    printf("Expected argument but got none\n");
    exit(1);
  }
  char *arg = (*argv)[0];
  (*argv)=((*argv) + 1);
  (*argc)=((*argc) - 1);
  return arg;
}

void std_mem_set_allocator(void *allocator, void *(*alloc_fn)(void *, u32), void (*free_fn)(void *, void *), void *(*realloc_fn)(void *, void *, u32, u32)) {
  std_mem_state_allocator=allocator;
  std_mem_state_alloc_fn=alloc_fn;
  std_mem_state_realloc_fn=realloc_fn;
  std_mem_state_free_fn=free_fn;
}

void std_mem_reset_to_default_allocator(void) {
  std_mem_set_allocator(NULL, std_mem_impl_my_calloc, std_mem_impl_my_free, std_mem_impl_my_realloc);
}

CellType *std_mem_alloc__0(u32 count) {
  return ((CellType *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(CellType)))));
}

char *std_mem_alloc__1(u32 count) {
  return ((char *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(char)))));
}

std_image_Image *std_mem_alloc__2(u32 count) {
  return ((std_image_Image *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(std_image_Image)))));
}

std_image_Color *std_mem_alloc__3(u32 count) {
  return ((std_image_Color *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(std_image_Color)))));
}

u8 *std_mem_alloc__4(u32 count) {
  return ((u8 *)std_mem_state_alloc_fn(std_mem_state_allocator, (count * ((u32)sizeof(u8)))));
}

void std_mem_free(void *ptr) {
  if (std_mem_state_free_fn != NULL) {
    std_mem_state_free_fn(std_mem_state_allocator, ptr);
    return;
  }
}

u8 *std_mem_realloc__0(u8 *ptr, u32 old_count, u32 new_count) {
  u32 old_size = (old_count * ((u32)sizeof(u8)));
  u32 new_size = (new_count * ((u32)sizeof(u8)));
  if (std_mem_state_realloc_fn != NULL) {
    return std_mem_state_realloc_fn(std_mem_state_allocator, ptr, old_size, new_size);
  }
  if(!(new_size >= old_size)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/mem.oc:63:12: Assertion failed: `new_size >= old_size`", "Cannot shrink memory in default allocator"); }
  u8 *new_ptr = std_mem_alloc__4(new_count);
  memcpy(new_ptr, ptr, old_size);
  std_mem_free(ptr);
  return new_ptr;
}

void *std_mem_impl_my_calloc(void *state, u32 size) {
  return calloc(size, 1);
}

void *std_mem_impl_my_realloc(void *state, void *ptr, u32 old_size, u32 size) {
  return realloc(ptr, size);
}

void std_mem_impl_my_free(void *state, void *ptr) {
  free(ptr);
}

bool std_sv_SV_is_empty(std_sv_SV *this) {
  return this->len==0;
}

std_sv_SV std_sv_SV_from_buf(std_buffer_Buffer buf) {
  return (std_sv_SV){.data=((char *)buf.data), .len=buf.size};
}

std_sv_SV std_sv_SV_from_str(char *s) {
  return (std_sv_SV){.data=s, .len=strlen(s)};
}

std_sv_SV std_sv_SV_get(std_sv_SV this, u32 n) {
  return (std_sv_SV){.data=this.data, .len=n};
}

char std_sv_SV_at(std_sv_SV this, u32 n) {
  if(!(n < this.len)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/sv.oc:30:12: Assertion failed: `n < .len`", "Index out of bounds in SV::at"); }
  return this.data[n];
}

std_sv_SV std_sv_SV_chop_left(std_sv_SV *this, u32 n) {
  std_sv_SV res = std_sv_SV_get((*this), n);
  this->data=(this->data + n);
  this->len-=n;
  return res;
}

std_sv_SV std_sv_SV_chop_right(std_sv_SV *this, u32 n) {
  std_sv_SV res = (std_sv_SV){.data=((this->data + this->len) - n), .len=n};
  this->len-=n;
  return res;
}

void std_sv_SV_trim_left(std_sv_SV *this) {
  while ((this->len > 0) && char_is_space(this->data[0])) {
    this->data=(this->data + 1);
    this->len-=1;
  }
}

void std_sv_SV_trim_right(std_sv_SV *this) {
  while ((this->len > 0) && char_is_space(this->data[(this->len - 1)])) {
    this->len-=1;
  }
}

void std_sv_SV_trim(std_sv_SV *this) {
  std_sv_SV_trim_left(this);
  std_sv_SV_trim_right(this);
}

i32 std_sv_SV_find_str(std_sv_SV *this, char *s) {
  for (u32 i = 0; i < (this->len - strlen(s)); i+=1) {
    if (memcmp((this->data + i), s, strlen(s))==0) {
      return ((i32)i);
    }
  }
  return -1;
}

std_sv_SV std_sv_SV_chop_by_delim(std_sv_SV *this, char delim) {
  u32 i = 0;
  while ((i < this->len) && (this->data[i] != delim)) {
    i+=1;
  }
  std_sv_SV res = std_sv_SV_get((*this), i);
  if (i==this->len) {
    this->data=(this->data + i);
    this->len-=i;
  } else {
    this->data=((this->data + i) + 1);
    this->len-=(i + 1);
  }
  return res;
}

std_sv_SV std_sv_SV_chop_by_sv(std_sv_SV *this, std_sv_SV delim) {
  for (u32 i = 0; i < ((this->len - delim.len) + 1); i++) {
    if (memcmp((this->data + i), delim.data, delim.len)==0) {
      std_sv_SV res = std_sv_SV_get((*this), i);
      this->data=((this->data + i) + delim.len);
      this->len-=(i + delim.len);
      return res;
    }
  }
  std_sv_SV res = std_sv_SV_get((*this), this->len);
  this->data=(this->data + this->len);
  this->len=0;
  return res;
}

std_sv_SV std_sv_SV_chop_by_str(std_sv_SV *this, char *delim) {
  return std_sv_SV_chop_by_sv(this, std_sv_SV_from_str(delim));
}

std_sv_SV std_sv_SV_chop_word(std_sv_SV *this) {
  std_sv_SV_trim_left(this);
  u32 i = 0;
  while ((i < this->len) && !char_is_space(this->data[i])) {
    i+=1;
  }
  std_sv_SV res = std_sv_SV_get((*this), i);
  if (i==this->len) {
    this->data=(this->data + i);
    this->len-=i;
  } else {
    this->data=((this->data + i) + 1);
    this->len-=(i + 1);
  }
  std_sv_SV_trim_left(this);
  return res;
}

u64 std_sv_SV_chop_unsigned(std_sv_SV *this, char *fn_name) {
  char *endptr = "";
  u64 res = strtoul(this->data, &endptr, 10);
  if(!(endptr != this->data)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/sv.oc:140:12: Assertion failed: `endptr != .data`", std_format("Failed to parse number in %s", fn_name)); }
  std_sv_SV_only_chop_left(this, ((u32)(endptr - this->data)));
  return res;
}

u64 std_sv_SV_chop_u64(std_sv_SV *this) {
  return ((u64)std_sv_SV_chop_unsigned(this, "SV::chop_u64"));
}

u32 std_sv_SV_chop_u32(std_sv_SV *this) {
  return ((u32)std_sv_SV_chop_unsigned(this, "SV::chop_u32"));
}

u16 std_sv_SV_chop_u16(std_sv_SV *this) {
  return ((u16)std_sv_SV_chop_unsigned(this, "SV::chop_u16"));
}

u8 std_sv_SV_chop_u8(std_sv_SV *this) {
  return ((u8)std_sv_SV_chop_unsigned(this, "SV::chop_u8"));
}

f32 std_sv_SV_chop_f32(std_sv_SV *this) {
  char *endptr = "";
  f64 res = strtod(this->data, &endptr);
  if(!(endptr != this->data)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/sv.oc:157:12: Assertion failed: `endptr != .data`", "Failed to parse number in SV::chop_f32"); }
  std_sv_SV_only_chop_left(this, ((u32)(endptr - this->data)));
  return ((f32)res);
}

i64 std_sv_SV_chop_signed(std_sv_SV *this, char *fn_name) {
  char *endptr = "";
  i64 res = strtoll(this->data, &endptr, 10);
  if(!(endptr != this->data)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/sv.oc:166:12: Assertion failed: `endptr != .data`", std_format("Failed to parse number in %s", fn_name)); }
  std_sv_SV_only_chop_left(this, ((u32)(endptr - this->data)));
  return res;
}

i64 std_sv_SV_chop_i64(std_sv_SV *this) {
  return ((i64)std_sv_SV_chop_signed(this, "SV::chop_i64"));
}

i32 std_sv_SV_chop_i32(std_sv_SV *this) {
  return ((i32)std_sv_SV_chop_signed(this, "SV::chop_i32"));
}

i16 std_sv_SV_chop_i16(std_sv_SV *this) {
  return ((i16)std_sv_SV_chop_signed(this, "SV::chop_i16"));
}

i8 std_sv_SV_chop_i8(std_sv_SV *this) {
  return ((i8)std_sv_SV_chop_signed(this, "SV::chop_i8"));
}

std_sv_SV std_sv_SV_chop_line(std_sv_SV *this) {
  return std_sv_SV_chop_by_delim(this, '\n');
}

void std_sv_SV_print(std_sv_SV *this, char *msg) {
  if (((bool)msg)) {
    printf("%s: '%.*s'""\n", msg, this->len, this->data);
  } else {
    printf("%.*s""\n", this->len, this->data);
  }
}

bool std_sv_SV_eq(std_sv_SV this, std_sv_SV other) {
  return this.len==other.len && memcmp(this.data, other.data, this.len)==0;
}

bool std_sv_SV_eq_str(std_sv_SV this, char *other) {
  return std_sv_SV_eq(this, std_sv_SV_from_str(other));
}

u32 std_sv_SV_hash(std_sv_SV this) {
  u32 hash = 5381;
  for (u32 i = 0; i < this.len; i+=1) {
    hash=((hash * 33) ^ ((u32)this.data[i]));
  }
  return hash;
}

void std_sv_SV_only_chop_left(std_sv_SV *this, u32 n) {
  this->data=(this->data + n);
  this->len-=n;
}

bool std_sv_SV_starts_with_sv(std_sv_SV this, std_sv_SV other) {
  if (this.len < other.len) {
    return false;
  }
  for (u32 i = 0; i < other.len; i+=1) {
    if (this.data[i] != other.data[i]) {
      return false;
    }
  }
  return true;
}

bool std_sv_SV_starts_with_str(std_sv_SV this, char *other) {
  return std_sv_SV_starts_with_sv(this, std_sv_SV_from_str(other));
}

bool std_sv_SV_ends_with_sv(std_sv_SV this, std_sv_SV other) {
  if (this.len < other.len) {
    return false;
  }
  for (u32 i = 0; i < other.len; i+=1) {
    if (this.data[((this.len - other.len) + i)] != other.data[i]) {
      return false;
    }
  }
  return true;
}

bool std_sv_SV_ends_with_str(std_sv_SV this, char *other) {
  return std_sv_SV_ends_with_sv(this, std_sv_SV_from_str(other));
}

std_sv_SV std_sv_SV_slice(std_sv_SV this, u32 off, u32 end) {
  if (end==0) {
    end=this.len;
  }
  if (end > this.len) {
    end=this.len;
  }
  if (off >= end) {
    off=end;
  }
  return (std_sv_SV){.data=(this.data + off), .len=(end - off)};
}

u64 std_sv_SV_count(std_sv_SV this, char c) {
  u64 total = ((u64)0);
  for (u32 i = 0; i < this.len; i++) {
    if (this.data[i]==c) {
      total+=((u64)1);
    }
  }
  return total;
}

bool std_sv_SV_contains(std_sv_SV this, char c) {
  return std_sv_SV_count(this, c) > ((u64)0);
}

std_CharIterator std_sv_SV_chars(std_sv_SV this, u32 start) {
  return (std_CharIterator){.data=this.data, .len=this.len, .pos=start};
}

std_sv_SVLineIterator std_sv_SV_lines(std_sv_SV this) {
  return (std_sv_SVLineIterator){.sv=this};
}

std_sv_SVSplitIterator std_sv_SV_split_sv(std_sv_SV this, std_sv_SV delim) {
  return (std_sv_SVSplitIterator){.sv=this, .delim=delim};
}

std_sv_SVSplitIterator std_sv_SV_split_str(std_sv_SV this, char *delim) {
  return (std_sv_SVSplitIterator){.sv=this, .delim=std_sv_SV_from_str(delim)};
}

std_sv_SVWordIterator std_sv_SV_split_words(std_sv_SV this) {
  return (std_sv_SVWordIterator){.sv=this};
}

std_sv_SVNumberIterator__0 std_sv_SV_iter_nums(std_sv_SV this) {
  return std_sv_SVNumberIterator__0_make(this);
}

std_sv_SVNumberIterator__0 std_sv_SV_iter_u64(std_sv_SV this) {
  return std_sv_SVNumberIterator__0_make(this);
}

std_sv_SVNumberIterator__1 std_sv_SV_iter_i64(std_sv_SV this) {
  return std_sv_SVNumberIterator__1_make(this);
}

std_sv_SVPredicateIterator std_sv_SV_iter_pred(std_sv_SV this, u32 (*pred)(std_sv_SV)) {
  return std_sv_SVPredicateIterator_make(this, pred);
}

std_buffer_BytesReader std_sv_SV_reader(std_sv_SV this) {
  return std_buffer_BytesReader_make(((u8 *)this.data), this.len);
}

char *std_sv_SV_copy_data_to_cstr(std_sv_SV this) {
  char *buf = std_mem_alloc__1((this.len + 1));
  memcpy(buf, this.data, this.len);
  buf[this.len]='\0';
  return buf;
}

bool std_sv_SVLineIterator_has_value(std_sv_SVLineIterator *this) {
  return !std_sv_SV_is_empty(&this->sv);
}

std_sv_SV std_sv_SVLineIterator_cur(std_sv_SVLineIterator *this) {
  return std_sv_SV_chop_line(&this->sv);
}

void std_sv_SVLineIterator_next(std_sv_SVLineIterator *this) {
}

bool std_sv_SVSplitIterator_has_value(std_sv_SVSplitIterator *this) {
  return !std_sv_SV_is_empty(&this->sv);
}

std_sv_SV std_sv_SVSplitIterator_cur(std_sv_SVSplitIterator *this) {
  return std_sv_SV_chop_by_sv(&this->sv, this->delim);
}

void std_sv_SVSplitIterator_next(std_sv_SVSplitIterator *this) {
}

bool std_sv_SVWordIterator_has_value(std_sv_SVWordIterator *this) {
  return !std_sv_SV_is_empty(&this->sv);
}

std_sv_SV std_sv_SVWordIterator_cur(std_sv_SVWordIterator *this) {
  return std_sv_SV_chop_word(&this->sv);
}

void std_sv_SVWordIterator_next(std_sv_SVWordIterator *this) {
}

std_sv_SVPredicateIterator std_sv_SVPredicateIterator_make(std_sv_SV sv, u32 (*pred)(std_sv_SV)) {
  std_sv_SVPredicateIterator it = (std_sv_SVPredicateIterator){.sv=sv, .pred=pred, .found=false, .res=std_sv_SV_from_str("")};
  std_sv_SVPredicateIterator_next(&it);
  return it;
}

bool std_sv_SVPredicateIterator_has_value(std_sv_SVPredicateIterator *this) {
  return this->found;
}

std_sv_SV std_sv_SVPredicateIterator_cur(std_sv_SVPredicateIterator *this) {
  return this->res;
}

void std_sv_SVPredicateIterator_next(std_sv_SVPredicateIterator *this) {
  this->found=false;
  while (this->sv.len > 0) {
    u32 res_len = this->pred(this->sv);
    if (res_len > 0) {
      this->found=true;
      this->res=std_sv_SV_chop_left(&this->sv, res_len);
      break;
    } else {
      std_sv_SV_chop_left(&this->sv, 1);
    }
  }
}

u64 std_sv_SVNumberIterator__0_cur(std_sv_SVNumberIterator__0 *this) {
  return this->cur_num;
}

void std_sv_SVNumberIterator__0_next(std_sv_SVNumberIterator__0 *this) {
  this->found=false;
  while (this->sv.len > 0) {
    char *endptr = "";
    u64 res = strtoul(this->sv.data, &endptr, 10);
    if (str_eq(endptr, this->sv.data)) {
      std_sv_SV_chop_left(&this->sv, 1);
    } else {
      this->cur_num=((u64)res);
      std_sv_SV_only_chop_left(&this->sv, ((u32)(endptr - this->sv.data)));
      this->found=true;
      break;
    }
  }
}

bool std_sv_SVNumberIterator__0_has_value(std_sv_SVNumberIterator__0 *this) {
  return this->found;
}

std_sv_SVNumberIterator__0 std_sv_SVNumberIterator__0_make(std_sv_SV sv) {
  std_sv_SVNumberIterator__0 it = {0};
  it.sv=sv;
  it.found=false;
  std_sv_SVNumberIterator__0_next(&it);
  return it;
}

i64 std_sv_SVNumberIterator__1_cur(std_sv_SVNumberIterator__1 *this) {
  return this->cur_num;
}

void std_sv_SVNumberIterator__1_next(std_sv_SVNumberIterator__1 *this) {
  this->found=false;
  while (this->sv.len > 0) {
    char *endptr = "";
    u64 res = strtoul(this->sv.data, &endptr, 10);
    if (str_eq(endptr, this->sv.data)) {
      std_sv_SV_chop_left(&this->sv, 1);
    } else {
      this->cur_num=((i64)res);
      std_sv_SV_only_chop_left(&this->sv, ((u32)(endptr - this->sv.data)));
      this->found=true;
      break;
    }
  }
}

bool std_sv_SVNumberIterator__1_has_value(std_sv_SVNumberIterator__1 *this) {
  return this->found;
}

std_sv_SVNumberIterator__1 std_sv_SVNumberIterator__1_make(std_sv_SV sv) {
  std_sv_SVNumberIterator__1 it = {0};
  it.sv=sv;
  it.found=false;
  std_sv_SVNumberIterator__1_next(&it);
  return it;
}

char *std_fs_realpath(char *path) {
  return realpath(path, NULL);
}

FILE *std_fs_File_open(char *path, char *mode) {
  FILE *file = fopen(path, mode);
  if (!((bool)file)) {
    std_panic(std_format("failed to open file: '%s': %s", path, std_libc_get_err()));
  }
  return file;
}

i32 std_fs_File_read(FILE *this, void *buf, u32 size) {
  return fread(buf, 1, size, this);
}

i32 std_fs_File_write(FILE *this, void *buf, u32 size) {
  return fwrite(buf, 1, size, this);
}

u32 std_fs_File_size(FILE *this) {
  i64 pos = ftell(this);
  fseek(this, ((i64)0), SEEK_END);
  u32 size = ((u32)ftell(this));
  fseek(this, pos, SEEK_SET);
  return size;
}

void std_fs_File_puts(FILE *this, char *s) {
  fwrite(s, 1, strlen(s), this);
}

void std_fs_File_truncate(FILE *this, i64 size) {
  i32 fd = fileno(this);
  i32 res = ftruncate(fd, size);
  if (res != 0) {
    std_panic(std_format("Failed to truncate file: %s", std_libc_get_err()));
  }
}

void std_fs_File_seek(FILE *this, i64 offset, i32 mode) {
  i32 res = fseek(this, offset, mode);
  if (res != 0) {
    std_panic(std_format("Failed to seek in file: %s", std_libc_get_err()));
  }
}

std_fs_FileMetadata std_fs_file_info(char *path) {
  struct stat s = {0};
  i32 res = stat(path, &s);
  if (res != 0) {
    std_panic(std_format("failed to get file info: '%s': %s", path, std_libc_get_err()));
  }
  return (std_fs_FileMetadata){.size=s.st_size, .atime=s.st_atime, .mtime=s.st_mtime, .ctime=s.st_ctime, .uid=s.st_uid, .gid=s.st_gid, .dev=s.st_dev, .mode=s.st_mode};
}

bool std_fs_file_exists(char *path) {
  FILE *file = fopen(path, "r");
  if (!((bool)file)) {
    return false;
  }
  fclose(file);
  return true;
}

void std_fs_write_file_bytes(char *path, void *data, u32 size) {
  FILE *file = fopen(path, "wb");
  if (!((bool)file)) {
    std_panic(std_format("[-] Failed to open file: %s", path));
  }
  i32 written = fwrite(data, 1, size, file);
  if (((u32)written) != size) {
    std_panic(std_format("[-] Failed to write to file: %s", path));
  }
  fclose(file);
}

void std_fs_write_file_str(char *path, char *data) {
  std_fs_write_file_bytes(path, data, strlen(data));
}

void std_fs_write_file(char *path, std_buffer_Buffer data) {
  std_fs_write_file_bytes(path, data.data, data.size);
}

std_buffer_Buffer std_fs_read_file(char *path) {
  FILE *file = fopen(path, "r");
  if (!((bool)file)) {
    std_panic(std_format("[-] Failed to open file: %s: %s", path, std_libc_get_err()));
  }
  fseek(file, ((i64)0), SEEK_END);
  u32 size = ((u32)ftell(file));
  fseek(file, ((i64)0), SEEK_SET);
  std_buffer_Buffer data = std_buffer_Buffer_make((size + 1));
  i32 read = fread(data.data, 1, size, file);
  if (((u32)read) != size) {
    std_panic(std_format("[-] Failed to read from file: %s", path));
  }
  fclose(file);
  data.size=((u32)size);
  data.data[size]=((u8)0);
  return data;
}

bool std_fs_directory_exists(char *path) {
  DIR *dir = opendir(path);
  if (dir==NULL) {
    return false;
  }
  closedir(dir);
  return true;
}

void std_fs_create_directory(char *path, bool exists_ok) {
  if (exists_ok && std_fs_directory_exists(path)) {
    return;
  }
  i32 res = mkdir(path, 0777);
  if (res != 0) {
    std_panic(std_format("[-] Failed to create directory: %s", path));
  }
}

std_fs_DirectoryIterator std_fs_iterate_directory(char *path, bool skip_self_and_parent) {
  DIR *dir = opendir(path);
  if (dir==NULL) {
    std_panic(std_format("[-] Failed to open directory: %s", path));
  }
  std_fs_DirectoryIterator iter = {0};
  iter.dir=dir;
  iter.skip_self_and_parent=skip_self_and_parent;
  std_fs_DirectoryIterator_next(&iter);
  return iter;
}

bool std_fs_DirectoryIterator_has_value(std_fs_DirectoryIterator *this) {
  return ((bool)this->dp);
}

void std_fs_DirectoryIterator_next(std_fs_DirectoryIterator *this) {
  this->dp=readdir(this->dir);
  while (((bool)this->dp) && this->skip_self_and_parent) {
    {
      char *__match_var_0 = this->dp->d_name;
      if (str_eq(__match_var_0, ".") || str_eq(__match_var_0, "..")) {
        this->dp=readdir(this->dir);
      } else  {
        break;
      }
    }
  }
}

std_fs_DirectoryEntry std_fs_DirectoryIterator_cur(std_fs_DirectoryIterator *this) {
  char *name = this->dp->d_name;
  std_fs_EntryType type = ({ std_fs_EntryType __yield_0;
    switch (this->dp->d_type) {
      case DT_DIR: {
        __yield_0 = std_fs_EntryType_Directory;
      } break;
      case DT_REG: {
        __yield_0 = std_fs_EntryType_File;
      } break;
      case DT_LNK: {
        __yield_0 = std_fs_EntryType_SymbolicLink;
      } break;
      default: {
        __yield_0 = std_fs_EntryType_Unknown;
      } break;
    }
;__yield_0; });
  return (std_fs_DirectoryEntry){.type=type, .name=name};
}

char *std_libc_get_err(void) {
  return ((char *)strerror(errno));
}

std_image_Image *std_image_Image_new(u32 width, u32 height) {
  std_image_Image *img = std_mem_alloc__2(1);
  (*img)=(std_image_Image){.width=width, .height=height, .data=std_mem_alloc__3((width * height))};
  return img;
}

void std_image_Image_free(std_image_Image *this) {
  std_mem_free(this->data);
  std_mem_free(this);
}

std_image_Color std_image_Image_get(std_image_Image *this, u32 x, u32 y) {
  return this->data[((y * this->width) + x)];
}

void std_image_Image_set(std_image_Image *this, u32 x, u32 y, std_image_Color col) {
  this->data[((y * this->width) + x)]=col;
}

void std_image_Image_save(std_image_Image *this, char *filename) {
  std_buffer_Buffer buffer = std_buffer_Buffer_make(16);
  if (str_ends_with(filename, ".ppm")) {
    std_image_ppm_encode(this, &buffer);
  } else if (str_ends_with(filename, ".qoi")) {
    std_image_qoi_encode(this, &buffer);
  } else if (str_ends_with(filename, ".png")) {
    std_image_png_encode(this, &buffer);
  } else {
    if(!(false)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/mod.oc:49:16: Assertion failed: `false`", std_format("Unknown file format for image: $%s", filename)); exit(1); }
  }
  std_fs_write_file(filename, buffer);
  std_buffer_Buffer_free(&buffer);
}

std_image_Image *std_image_Image_load(char *filename) {
  std_buffer_Buffer data = std_fs_read_file(filename);
  std_image_Image *image = ((std_image_Image *)NULL);
  if (str_ends_with(filename, ".ppm")) {
    image=std_image_ppm_decode(&data);
  } else if (str_ends_with(filename, ".qoi")) {
    image=std_image_qoi_decode(&data);
  } else if (str_ends_with(filename, ".png")) {
    image=std_image_png_decode(&data);
  } else {
    if(!(false)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/mod.oc:71:16: Assertion failed: `false`", std_format("Unknown file format for image: $%s", filename)); exit(1); }
  }
  if(!(((bool)image))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/mod.oc:74:12: Assertion failed: `image?`", std_format("Failed to load image: $%s", filename)); }
  std_buffer_Buffer_free(&data);
  return image;
}

f32 std_image_interp(f32 t, f32 a, f32 b) {
  return (a * (1.0f - t)) + (b * t);
}

f32 std_image_bilinear_interp(f32 t0, f32 t1, f32 a, f32 b, f32 c, f32 d) {
  return std_image_interp(t1, std_image_interp(t0, a, b), std_image_interp(t0, c, d));
}

std_image_Color std_image_Image_get_interpolated(std_image_Image *this, f32 xf, f32 yf) {
  if(!((0.0f <= xf) && (xf < ((f32)this->width)))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/mod.oc:88:12: Assertion failed: `0.0 <= xf < (.width as f32)`", NULL); }
  if(!((0.0f <= yf) && (yf < ((f32)this->height)))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/mod.oc:89:12: Assertion failed: `0.0 <= yf < (.height as f32)`", NULL); }
  u32 x_fl = ((u32)xf);
  u32 y_fl = ((u32)yf);
  u32 x_cl = ((xf != ((f32)x_fl)) ? (x_fl + 1) : x_fl);
  u32 y_cl = ((yf != ((f32)y_fl)) ? (y_fl + 1) : y_fl);
  std_image_Color a = std_image_Image_get(this, x_fl, y_fl);
  std_image_Color b = std_image_Image_get(this, x_cl, y_fl);
  std_image_Color c = std_image_Image_get(this, x_fl, y_cl);
  std_image_Color d = std_image_Image_get(this, x_cl, y_cl);
  f32 x_ratio = (xf - ((f32)x_fl));
  f32 y_ratio = (yf - ((f32)y_fl));
  std_image_Color col = (std_image_Color){.r=((u8)std_image_bilinear_interp(x_ratio, y_ratio, ((f32)a.r), ((f32)b.r), ((f32)c.r), ((f32)d.r))), .g=((u8)std_image_bilinear_interp(x_ratio, y_ratio, ((f32)a.g), ((f32)b.g), ((f32)c.g), ((f32)d.g))), .b=((u8)std_image_bilinear_interp(x_ratio, y_ratio, ((f32)a.b), ((f32)b.b), ((f32)c.b), ((f32)d.b)))};
  return col;
}

std_image_Image *std_image_Image_resize(std_image_Image *this, u32 new_width, u32 new_height) {
  std_image_Image *resized = std_image_Image_new(new_width, new_height);
  f32 x_ratio = (((f32)this->width) / ((f32)new_width));
  f32 y_ratio = (((f32)this->height) / ((f32)new_height));
  for (u32 ny = 0; ny < new_height; ++ny) {
    for (u32 nx = 0; nx < new_width; ++nx) {
      f32 x = (((f32)nx) * x_ratio);
      f32 y = (((f32)ny) * y_ratio);
      std_image_Image_set(resized, nx, ny, std_image_Image_get_interpolated(this, x, y));
    }
  }
  return resized;
}

std_image_Image *std_image_Image_rotate(std_image_Image *this, std_image_RotationDirection dir) {
  std_image_Image *rotated = std_image_Image_new(this->height, this->width);
  for (u32 y = 0; y < this->height; ++y) {
    for (u32 x = 0; x < this->width; ++x) {
      std_image_Color col = ({ std_image_Color __yield_0;
        if (dir==std_image_RotationDirection_CounterClockwise) {
          __yield_0 = std_image_Image_get(this, ((this->width - x) - 1), y);
        } else {
          __yield_0 = std_image_Image_get(this, x, ((this->height - y) - 1));
        }
;__yield_0; });
      std_image_Image_set(rotated, y, x, col);
    }
  }
  return rotated;
}

void std_image_Image_clear(std_image_Image *this) {
  memset(this->data, ((u8)0), ((this->width * this->height) * ((u32)sizeof(std_image_Color))));
}

std_image_Image *std_image_Image_copy(std_image_Image *this) {
  std_image_Image *copy = std_image_Image_new(this->width, this->height);
  memcpy(copy->data, this->data, ((this->width * this->height) * ((u32)sizeof(std_image_Color))));
  return copy;
}

void std_image_ppm_encode(std_image_Image *img, std_buffer_Buffer *data) {
  if(!(((u32)sizeof(std_image_Color))==3)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/ppm.oc:5:12: Assertion failed: `sizeof(Color) == 3`", "Color size must be 3 bytes for PPM encoding"); }
  std_buffer_Buffer_write_str_f(data, std_format("P6 %u %u 255\n", img->width, img->height));
  std_buffer_Buffer_write_bytes(data, img->data, ((img->width * img->height) * 3));
}

std_image_Image *std_image_ppm_decode(std_buffer_Buffer *data) {
  std_sv_SV data_sv = std_buffer_Buffer_sv((*data));
  if(!(std_sv_SV_eq_str(std_sv_SV_chop_word(&data_sv), "P6"))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/ppm.oc:12:12: Assertion failed: `data_sv.chop_word() == \"P6\"`", "Unsupported PPM format"); }
  u32 width = std_sv_SV_chop_u32(&data_sv);
  u32 height = std_sv_SV_chop_u32(&data_sv);
  if(!(std_sv_SV_chop_u32(&data_sv)==255)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/ppm.oc:15:12: Assertion failed: `data_sv.chop_u32() == 255`", "Unsupported PPM Color depth"); }
  std_image_Image *img = std_image_Image_new(width, height);
  std_buffer_BytesReader io = std_sv_SV_reader(data_sv);
  for (u32 i = 0; i < (width * height); i++) {
    u8 r = std_buffer_BytesReader_read_u8(&io);
    u8 g = std_buffer_BytesReader_read_u8(&io);
    u8 b = std_buffer_BytesReader_read_u8(&io);
    img->data[i]=(std_image_Color){.r=r, .g=g, .b=b};
  }
  return img;
}

u32 std_image_qoi_RGBA_hash(std_image_qoi_RGBA this) {
  u32 r = ((u32)this.r);
  u32 g = ((u32)this.g);
  u32 b = ((u32)this.b);
  u32 a = ((u32)this.a);
  return ((((r * 3) + (g * 5)) + (b * 7)) + (a * 11)) % 64;
}

bool std_image_qoi_RGBA_eq(std_image_qoi_RGBA this, std_image_qoi_RGBA other) {
  return ((this.r==other.r && this.g==other.g) && this.b==other.b) && this.a==other.a;
}

std_image_qoi_QOIDecoder std_image_qoi_QOIDecoder_make(std_buffer_Buffer *data) {
  std_image_qoi_QOIDecoder decoder = {0};
  memset(&decoder, ((u8)0), ((u32)sizeof(std_image_qoi_QOIDecoder)));
  decoder.io=std_buffer_Buffer_reader((*data));
  decoder.prev=(std_image_qoi_RGBA){.r=((u8)0), .g=((u8)0), .b=((u8)0), .a=((u8)255)};
  return decoder;
}

std_image_Image *std_image_qoi_QOIDecoder_decode(std_image_qoi_QOIDecoder *this) {
  u32 magic = std_buffer_BytesReader_read_u32(&this->io, std_Endian_Little);
  if(!(magic==0x716f6966)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/qoi.oc:49:12: Assertion failed: `magic == 0x716f6966`", "Invalid magic number"); }
  this->width=std_buffer_BytesReader_read_u32(&this->io, std_Endian_Little);
  this->height=std_buffer_BytesReader_read_u32(&this->io, std_Endian_Little);
  this->channels=std_buffer_BytesReader_read_u8(&this->io);
  this->colorspace=std_buffer_BytesReader_read_u8(&this->io);
  if (this->channels==((u8)4)) {
    printf("WARNING: QOI decoder does not support alpha. Ignoring alpha channel.\n");
  }
  this->img=std_image_Image_new(this->width, this->height);
  std_image_qoi_QOIDecoder_decode_pixels(this);
  return this->img;
}

void std_image_qoi_QOIDecoder_handle_pixel(std_image_qoi_QOIDecoder *this, std_image_qoi_RGBA pix) {
  u32 hash = std_image_qoi_RGBA_hash(pix);
  std_image_qoi_RGBA seen = this->seen[hash];
  this->seen[hash]=pix;
  this->prev=pix;
  this->img->data[this->pixels_done]=(std_image_Color){.r=pix.r, .g=pix.g, .b=pix.b};
  this->pixels_done+=1;
}

void std_image_qoi_QOIDecoder_decode_pixels(std_image_qoi_QOIDecoder *this) {
  while (this->pixels_done < (this->width * this->height)) {
    u8 b = std_buffer_BytesReader_read_u8(&this->io);
    if (b==((u8)0b11111110)) {
      std_image_qoi_QOIDecoder_decode_op_rgb(this);
    } else if (b==((u8)0b11111111)) {
      std_image_qoi_QOIDecoder_decode_op_rgba(this);
    } else if ((b >> ((u8)6))==((u8)0b00)) {
      std_image_qoi_QOIDecoder_decode_op_index(this, b);
    } else if ((b >> ((u8)6))==((u8)0b01)) {
      std_image_qoi_QOIDecoder_decode_op_diff(this, b);
    } else if ((b >> ((u8)6))==((u8)0b10)) {
      std_image_qoi_QOIDecoder_decode_op_luma(this, b);
    } else if ((b >> ((u8)6))==((u8)0b11)) {
      std_image_qoi_QOIDecoder_decode_op_run(this, b);
    } else {
      if(!(false)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/qoi.oc:99:20: Assertion failed: `false`", "Invalid opcode"); exit(1); }
    }
  }
}

void std_image_qoi_QOIDecoder_decode_op_rgb(std_image_qoi_QOIDecoder *this) {
  u8 r = std_buffer_BytesReader_read_u8(&this->io);
  u8 g = std_buffer_BytesReader_read_u8(&this->io);
  u8 b = std_buffer_BytesReader_read_u8(&this->io);
  std_image_qoi_RGBA pixel = (std_image_qoi_RGBA){.r=r, .g=g, .b=b, .a=((u8)255)};
  std_image_qoi_QOIDecoder_handle_pixel(this, pixel);
}

void std_image_qoi_QOIDecoder_decode_op_rgba(std_image_qoi_QOIDecoder *this) {
  printf("QOI_OP_RGBA""\n");
  u8 r = std_buffer_BytesReader_read_u8(&this->io);
  u8 g = std_buffer_BytesReader_read_u8(&this->io);
  u8 b = std_buffer_BytesReader_read_u8(&this->io);
  u8 a = std_buffer_BytesReader_read_u8(&this->io);
  std_image_qoi_RGBA pixel = (std_image_qoi_RGBA){.r=r, .g=g, .b=b, .a=a};
  std_image_qoi_QOIDecoder_handle_pixel(this, pixel);
}

void std_image_qoi_QOIDecoder_decode_op_luma(std_image_qoi_QOIDecoder *this, u8 first) {
  u8 second = std_buffer_BytesReader_read_u8(&this->io);
  u8 dg = ((u8)((first & ((u8)0b00111111)) - ((u8)32)));
  u8 dr_dg = ((u8)((second >> ((u8)4)) - ((u8)8)));
  u8 db_dg = ((u8)((second & ((u8)0b1111)) - ((u8)8)));
  u8 dr = (dg + dr_dg);
  u8 db = (dg + db_dg);
  std_image_qoi_RGBA pixel = (std_image_qoi_RGBA){.r=(this->prev.r + dr), .g=(this->prev.g + dg), .b=(this->prev.b + db), .a=this->prev.a};
  std_image_qoi_QOIDecoder_handle_pixel(this, pixel);
}

void std_image_qoi_QOIDecoder_decode_op_index(std_image_qoi_QOIDecoder *this, u8 first) {
  u8 index = (first & ((u8)0b00111111));
  std_image_qoi_RGBA pixel = this->seen[index];
  std_image_qoi_QOIDecoder_handle_pixel(this, pixel);
}

void std_image_qoi_QOIDecoder_decode_op_diff(std_image_qoi_QOIDecoder *this, u8 first) {
  u8 r = (((first >> ((u8)4)) & ((u8)0b11)) - ((u8)2));
  u8 g = (((first >> ((u8)2)) & ((u8)0b11)) - ((u8)2));
  u8 b = (((first >> ((u8)0)) & ((u8)0b11)) - ((u8)2));
  std_image_qoi_RGBA pixel = (std_image_qoi_RGBA){.r=(this->prev.r + ((u8)r)), .g=(this->prev.g + ((u8)g)), .b=(this->prev.b + ((u8)b)), .a=this->prev.a};
  std_image_qoi_QOIDecoder_handle_pixel(this, pixel);
}

void std_image_qoi_QOIDecoder_decode_op_run(std_image_qoi_QOIDecoder *this, u8 first) {
  u32 run = (((u32)(first & ((u8)0b00111111))) + 1);
  for (u32 i = 0; i < run; i++) {
    std_image_qoi_QOIDecoder_handle_pixel(this, this->prev);
  }
}

std_image_qoi_QOIEncoder std_image_qoi_QOIEncoder_make(std_image_Image *img, std_buffer_Buffer *buffer) {
  std_image_qoi_QOIEncoder encoder = {0};
  memset(&encoder, ((u8)0), ((u32)sizeof(std_image_qoi_QOIEncoder)));
  encoder.img=img;
  encoder.buf=buffer;
  encoder.prev=(std_image_qoi_RGBA){.r=((u8)0), .g=((u8)0), .b=((u8)0), .a=((u8)255)};
  return encoder;
}

void std_image_qoi_QOIEncoder_encode_header(std_image_qoi_QOIEncoder *this) {
  std_buffer_Buffer_write_str(this->buf, "qoif");
  std_buffer_Buffer_write_u32(this->buf, this->img->width, std_Endian_Big);
  std_buffer_Buffer_write_u32(this->buf, this->img->height, std_Endian_Big);
  std_buffer_Buffer_write_u8(this->buf, ((u8)3));
  std_buffer_Buffer_write_u8(this->buf, ((u8)0));
}

std_image_qoi_RGBA std_image_qoi_QOIEncoder_img_pix(std_image_qoi_QOIEncoder *this, u32 idx) {
  std_image_Color col = this->img->data[idx];
  return (std_image_qoi_RGBA){.r=col.r, .g=col.g, .b=col.b, .a=((u8)255)};
}

void std_image_qoi_QOIEncoder_handle_pixel(std_image_qoi_QOIEncoder *this, std_image_qoi_RGBA pix, u32 num) {
  u32 hash = std_image_qoi_RGBA_hash(pix);
  this->seen[hash]=pix;
  this->prev=pix;
  this->pixels_done+=num;
}

void std_image_qoi_QOIEncoder_encode_pixel(std_image_qoi_QOIEncoder *this, std_image_qoi_RGBA pix) {
  if (std_image_qoi_RGBA_eq(this->prev, pix)) {
    u32 run = 1;
    while (((this->pixels_done + run) < (this->img->width * this->img->height)) && (run < 62)) {
      if (!std_image_qoi_RGBA_eq(this->prev, std_image_qoi_QOIEncoder_img_pix(this, (this->pixels_done + run)))) {
        break;
      }
      run+=1;
    }
    if(!((1 <= run) && (run <= 62))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/qoi.oc:217:16: Assertion failed: `1 <= run <= 62`", "Run length must be between 1 and 62"); }
    std_buffer_Buffer_write_u8(this->buf, ((((u8)0b11) << ((u8)6)) | (((u8)run) - ((u8)1))));
    std_image_qoi_QOIEncoder_handle_pixel(this, pix, run);
    return;
  }
  {
    u32 hash = std_image_qoi_RGBA_hash(pix);
    std_image_qoi_RGBA seen_pix = this->seen[hash];
    if (std_image_qoi_RGBA_eq(seen_pix, pix)) {
      std_buffer_Buffer_write_u8(this->buf, ((((u8)0b00) << ((u8)6)) | ((u8)hash)));
      std_image_qoi_QOIEncoder_handle_pixel(this, pix, 1);
      return;
    }
  }
  {
    i32 dr = (((i32)pix.r) - ((i32)this->prev.r));
    i32 dg = (((i32)pix.g) - ((i32)this->prev.g));
    i32 db = (((i32)pix.b) - ((i32)this->prev.b));
    if ((((-2 <= dr) && (dr < 2)) && ((-2 <= dg) && (dg < 2))) && ((-2 <= db) && (db < 2))) {
      u8 dr_u = ((u8)(dr + 2));
      u8 dg_u = ((u8)(dg + 2));
      u8 db_u = ((u8)(db + 2));
      std_buffer_Buffer_write_u8(this->buf, ((((((u8)0b01) << ((u8)6)) | (dr_u << ((u8)4))) | (dg_u << ((u8)2))) | db_u));
      std_image_qoi_QOIEncoder_handle_pixel(this, pix, 1);
      return;
    }
    i32 dr_dg = (dr - dg);
    i32 db_dg = (db - dg);
    if ((((-32 <= dg) && (dg < 32)) && ((-8 <= dr_dg) && (dr_dg < 8))) && ((-8 <= db_dg) && (db_dg < 8))) {
      u8 dg_u = ((u8)(dg + 32));
      u8 dr_dg_u = ((u8)(dr_dg + 8));
      u8 db_dg_u = ((u8)(db_dg + 8));
      std_buffer_Buffer_write_u8(this->buf, ((((u8)0b10) << ((u8)6)) | dg_u));
      std_buffer_Buffer_write_u8(this->buf, ((dr_dg_u << ((u8)4)) | db_dg_u));
      std_image_qoi_QOIEncoder_handle_pixel(this, pix, 1);
      return;
    }
  }
  std_buffer_Buffer_write_u8(this->buf, ((u8)0b11111110));
  std_buffer_Buffer_write_u8(this->buf, pix.r);
  std_buffer_Buffer_write_u8(this->buf, pix.g);
  std_buffer_Buffer_write_u8(this->buf, pix.b);
  std_image_qoi_QOIEncoder_handle_pixel(this, pix, 1);
}

void std_image_qoi_QOIEncoder_encode_pixels(std_image_qoi_QOIEncoder *this) {
  while (this->pixels_done < (this->img->width * this->img->height)) {
    std_image_qoi_QOIEncoder_encode_pixel(this, std_image_qoi_QOIEncoder_img_pix(this, this->pixels_done));
  }
}

void std_image_qoi_QOIEncoder_encode_end(std_image_qoi_QOIEncoder *this) {
  for (u32 i = 0; i < 7; i++) {
    std_buffer_Buffer_write_u8(this->buf, ((u8)0));
  }
  std_buffer_Buffer_write_u8(this->buf, ((u8)1));
}

void std_image_qoi_QOIEncoder_encode(std_image_qoi_QOIEncoder *this) {
  std_image_qoi_QOIEncoder_encode_header(this);
  std_image_qoi_QOIEncoder_encode_pixels(this);
  std_image_qoi_QOIEncoder_encode_end(this);
}

void std_image_qoi_encode(std_image_Image *img, std_buffer_Buffer *data) {
  std_image_qoi_QOIEncoder encoder = std_image_qoi_QOIEncoder_make(img, data);
  std_image_qoi_QOIEncoder_encode(&encoder);
}

std_image_Image *std_image_qoi_decode(std_buffer_Buffer *data) {
  std_image_qoi_QOIDecoder decoder = std_image_qoi_QOIDecoder_make(data);
  return std_image_qoi_QOIDecoder_decode(&decoder);
}

void std_image_png_make_crc_table(void) {
  for (u32 n = 0; n < 256; n++) {
    u32 c = n;
    for (u32 k = 0; k < 8; k++) {
      if ((c & 1) > 0) {
        c=(0xedb88320 ^ (c >> 1));
      } else {
        c=(c >> 1);
      }
    }
    std_image_png_crc_table[n]=c;
  }
  std_image_png_crc_table_computed=true;
}

u32 std_image_png_get_crc(u8 *buf, u32 len, u32 init) {
  if (!std_image_png_crc_table_computed) {
    std_image_png_make_crc_table();
  }
  u32 c = init;
  for (u32 n = 0; n < len; n++) {
    c=(std_image_png_crc_table[((c ^ ((u32)buf[n])) & 0xff)] ^ (c >> 8));
  }
  return c ^ 0xffffffff;
}

std_image_png_Chunk std_image_png_Chunk_parse(std_buffer_BytesReader *io) {
  u32 length = std_buffer_BytesReader_read_u32(io, std_Endian_Big);
  std_sv_SV type = std_buffer_BytesReader_read_bytes_sv(io, 4);
  std_sv_SV data_sv = std_buffer_BytesReader_read_bytes_sv(io, length);
  u32 crc = std_buffer_BytesReader_read_u32(io, std_Endian_Big);
  return (std_image_png_Chunk){.type=type, .data=std_buffer_Buffer_from_sized_str(data_sv.data, data_sv.len), .is_critical=char_is_upper(std_sv_SV_at(type, 0)), .is_private=char_is_upper(std_sv_SV_at(type, 1))};
}

void std_image_png_Chunk_encode(std_image_png_Chunk *this, std_buffer_Buffer *out) {
  std_buffer_Buffer chunk_buf = std_buffer_Buffer_make(16);
  std_buffer_Buffer_write_u32(&chunk_buf, this->data.size, std_Endian_Big);
  std_buffer_Buffer_write_bytes(&chunk_buf, this->type.data, this->type.len);
  if (this->data.size > 0) {
    std_buffer_Buffer_write_buf(&chunk_buf, &this->data);
  }
  u32 crc = std_image_png_get_crc((chunk_buf.data + 4), (chunk_buf.size - 4), 0xffffffff);
  std_buffer_Buffer_write_u32(&chunk_buf, crc, std_Endian_Big);
  std_buffer_Buffer_write_buf_f(out, &chunk_buf);
}

u8 std_image_png_paeth_predictor(u8 ua, u8 ub, u8 uc) {
  i32 a = ((i32)((u32)ua));
  i32 b = ((i32)((u32)ub));
  i32 c = ((i32)((u32)uc));
  i32 p = ((a + b) - c);
  i32 pa = i32_abs((p - a));
  i32 pb = i32_abs((p - b));
  i32 pc = i32_abs((p - c));
  return ({ u8 __yield_0;
    if ((pa <= pb) && (pa <= pc)) {
      __yield_0 = ua;
    } else if (pb <= pc) {
      __yield_0 = ub;
    } else {
      __yield_0 = uc;
    }
;__yield_0; });
}

void std_image_png_PNG_decode_scanline(std_image_png_PNG *this, std_buffer_Buffer *out, u32 scanline, std_sv_SV sv) {
  if(!(sv.len==((this->width * this->S) + 1))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/png.oc:114:12: Assertion failed: `sv.len == .width * .S + 1`", "Invalid scanline length"); }
  u32 filter = ((u32)std_sv_SV_at(std_sv_SV_chop_left(&sv, 1), 0));
  u8 *line = ((u8 *)sv.data);
  u32 size = ((u32)sv.len);
  u32 cur_scanline_idx = out->size;
  u32 prev_scanline_idx = (cur_scanline_idx - (this->width * this->S));
  switch (filter) {
    case 0: {
      std_buffer_Buffer_write_bytes(out, line, size);
    } break;
    case 1: {
      for (u32 i = 0; i < sv.len; i++) {
        u8 x = line[i];
        u8 a = ((i >= this->S) ? out->data[((cur_scanline_idx + i) - this->S)] : ((u8)0));
        std_buffer_Buffer_write_u8(out, (x + a));
      }
    } break;
    case 2: {
      for (u32 i = 0; i < sv.len; i++) {
        u8 x = line[i];
        u8 b = ((scanline > 0) ? out->data[(prev_scanline_idx + i)] : ((u8)0));
        std_buffer_Buffer_write_u8(out, (x + b));
      }
    } break;
    case 3: {
      for (u32 i = 0; i < sv.len; i++) {
        u8 x = line[i];
        u32 a = ((i >= this->S) ? ((u32)out->data[((cur_scanline_idx + i) - this->S)]) : 0);
        u32 b = ((scanline > 0) ? ((u32)out->data[(prev_scanline_idx + i)]) : 0);
        std_buffer_Buffer_write_u8(out, (x + ((u8)((a + b) / 2))));
      }
    } break;
    case 4: {
      for (u32 i = 0; i < sv.len; i++) {
        u8 x = line[i];
        u8 a = ((i >= this->S) ? out->data[((cur_scanline_idx + i) - this->S)] : ((u8)0));
        u8 b = ((scanline > 0) ? out->data[(prev_scanline_idx + i)] : ((u8)0));
        u8 c = (((scanline > 0) && (i >= this->S)) ? out->data[((prev_scanline_idx + i) - this->S)] : ((u8)0));
        std_buffer_Buffer_write_u8(out, (x + std_image_png_paeth_predictor(a, b, c)));
      }
    } break;
    default: std_panic(std_format("Unsupported filter %u", filter)); break;
  }
}

void std_image_png_encode(std_image_Image *img, std_buffer_Buffer *data) {
  std_buffer_Buffer_write_str(data, "\x89PNG\r\n\x1a\n");
  {
    std_buffer_Buffer buf = std_buffer_Buffer_make(16);
    std_buffer_Buffer_write_u32(&buf, img->width, std_Endian_Big);
    std_buffer_Buffer_write_u32(&buf, img->height, std_Endian_Big);
    std_buffer_Buffer_write_u8(&buf, ((u8)8));
    std_buffer_Buffer_write_u8(&buf, ((u8)2));
    std_buffer_Buffer_write_u8(&buf, ((u8)0));
    std_buffer_Buffer_write_u8(&buf, ((u8)0));
    std_buffer_Buffer_write_u8(&buf, ((u8)0));
    std_image_png_Chunk chunk = (std_image_png_Chunk){.type=std_sv_SV_from_str("IHDR"), .data=buf, .is_critical=true, .is_private=false};
    std_image_png_Chunk_encode(&chunk, data);
    std_buffer_Buffer_free(&buf);
  }
  {
    std_buffer_Buffer chunk_data = std_buffer_Buffer_make(16);
    for (u32 row = 0; row < img->height; row++) {
      std_buffer_Buffer_write_u8(&chunk_data, ((u8)0));
      std_buffer_Buffer_write_bytes(&chunk_data, ((u8 *)&img->data[(row * img->width)]), (img->width * 3));
    }
    std_buffer_Buffer compressed = std_zlib_compress(chunk_data);
    std_buffer_Buffer_free(&chunk_data);
    std_image_png_Chunk chunk = (std_image_png_Chunk){.type=std_sv_SV_from_str("IDAT"), .data=compressed, .is_critical=true, .is_private=false};
    std_image_png_Chunk_encode(&chunk, data);
    std_buffer_Buffer_free(&compressed);
  }
  {
    std_image_png_Chunk chunk = (std_image_png_Chunk){.type=std_sv_SV_from_str("IEND"), .data=std_buffer_Buffer_make(16), .is_critical=true, .is_private=false};
    std_image_png_Chunk_encode(&chunk, data);
    std_buffer_Buffer_free(&chunk.data);
  }
}

std_image_Image *std_image_png_decode(std_buffer_Buffer *data) {
  std_buffer_BytesReader io = std_buffer_Buffer_reader((*data));
  std_sv_SV header = std_buffer_BytesReader_read_bytes_sv(&io, 8);
  if (!std_sv_SV_eq_str(header, "\x89PNG\r\n\x1a\n")) {
    printf("Not a PNG file""\n");
    exit(1);
  }
  std_image_png_PNG png = {0};
  std_buffer_Buffer datastream = std_buffer_Buffer_make(16);
  std_buffer_Buffer out = std_buffer_Buffer_make(16);
  while (!std_buffer_BytesReader_is_empty(&io)) {
    std_image_png_Chunk chunk = std_image_png_Chunk_parse(&io);
    {
      std_sv_SV __match_var_1 = chunk.type;
      if (std_sv_SV_eq_str(__match_var_1, "IHDR")) {
        std_buffer_BytesReader io = std_buffer_Buffer_reader(chunk.data);
        png.width=std_buffer_BytesReader_read_u32(&io, std_Endian_Big);
        png.height=std_buffer_BytesReader_read_u32(&io, std_Endian_Big);
        png.bit_depth=std_buffer_BytesReader_read_u8(&io);
        png.color_type=((int)std_buffer_BytesReader_read_u8(&io));
        u8 compression_method = std_buffer_BytesReader_read_u8(&io);
        u8 filter_method = std_buffer_BytesReader_read_u8(&io);
        if(!(compression_method==((u8)0))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/png.oc:245:24: Assertion failed: `compression_method == 0`", "Unsupported compression method"); }
        if(!(filter_method==((u8)0))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/png.oc:246:24: Assertion failed: `filter_method == 0`", "Unsupported filter method"); }
        png.interlace_method=((int)std_buffer_BytesReader_read_u8(&io));
        if(!(png.bit_depth==((u8)8))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/png.oc:251:24: Assertion failed: `png.bit_depth == 8`", "Unsupported bit depth"); }
        if(!(png.interlace_method==0)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/image/png.oc:252:24: Assertion failed: `png.interlace_method == None`", "Unsupported interlace method"); }
        png.S=({ u32 __yield_0;
          switch (png.color_type) {
            case 2: {
              __yield_0 = 3;
            } break;
            case 6: {
              __yield_0 = 4;
            } break;
            default: std_panic(std_format("Unsupported color type %s", std_image_png_ColorType_dbg(png.color_type))); break;
          }
;__yield_0; });
      } else if (std_sv_SV_eq_str(__match_var_1, "IDAT")) {
        std_buffer_Buffer_write_buf(&datastream, &chunk.data);
      } else if (std_sv_SV_eq_str(__match_var_1, "IEND")) {
        std_buffer_Buffer decompressed = std_zlib_decompress(datastream, NULL);
        std_buffer_Buffer_free(&datastream);
        std_buffer_Buffer_resize_if_necessary(&out, (((png.width * png.height) * png.S) + 1));
        std_buffer_BytesReader io = std_buffer_Buffer_reader(decompressed);
        for (u32 h = 0; h < png.height; h++) {
          std_sv_SV scanline = std_buffer_BytesReader_read_bytes_sv(&io, ((png.width * png.S) + 1));
          std_image_png_PNG_decode_scanline(&png, &out, h, scanline);
        }
        std_buffer_Buffer_free(&decompressed);
      } else  {
        if (!chunk.is_critical) {
          continue;
        }
        std_panic(std_format("Unsupported critical chunk %.*s", (chunk.type).len, (chunk.type).data));
      }
    }
  }
  std_image_Image *img = std_image_Image_new(png.width, png.height);
  for (u32 i = 0; i < (png.width * png.height); i++) {
    u32 idx = (i * png.S);
    u8 r = std_buffer_Buffer_get_byte_at(out, (idx + 0));
    u8 g = std_buffer_Buffer_get_byte_at(out, (idx + 1));
    u8 b = std_buffer_Buffer_get_byte_at(out, (idx + 2));
    img->data[i]=(std_image_Color){.r=r, .g=g, .b=b};
  }
  std_buffer_Buffer_free(&out);
  return img;
}

std_vec_Vec2__0 std_vec_Vec2__0_subrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_sub(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__0_normalized(std_vec_Vec2__0 this) {
  f32 len = std_vec_Vec2__0_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__1 std_vec_Vec2__1_subrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_sub(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_normalized(std_vec_Vec2__1 this) {
  f32 len = std_vec_Vec2__1_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__1 std_vec_Vec2__1_multrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_add(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x + other.x), .y=(this.y + other.y)};
}

f32 std_vec_Vec2__1_length_sq(std_vec_Vec2__1 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__1_to_f64(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_subrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_sub(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__2_normalized(std_vec_Vec2__2 this) {
  f32 len = std_vec_Vec2__2_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__2 std_vec_Vec2__2_multrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_add(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x + other.x), .y=(this.y + other.y)};
}

f64 std_vec_Vec2__2_length_sq(std_vec_Vec2__2 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__2_to_f64(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_mult(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_addrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__2_to_f32(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__2_to_i32(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_subrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_sub(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__3_normalized(std_vec_Vec2__3 this) {
  f32 len = std_vec_Vec2__3_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__3 std_vec_Vec2__3_multrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_add(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x + other.x), .y=(this.y + other.y)};
}

i32 std_vec_Vec2__3_length_sq(std_vec_Vec2__3 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__3_to_f64(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_mult(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_addrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__3_to_f32(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_to_i32(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__3_to_i64(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_subrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_sub(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__4_normalized(std_vec_Vec2__4 this) {
  f32 len = std_vec_Vec2__4_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__4 std_vec_Vec2__4_multrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_add(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x + other.x), .y=(this.y + other.y)};
}

i64 std_vec_Vec2__4_length_sq(std_vec_Vec2__4 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__4_to_f64(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_mult(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_addrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__4_to_f32(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__4_to_i32(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_to_i64(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_reflect(std_vec_Vec2__4 this, std_vec_Vec2__4 normal) {
  return std_vec_Vec2__4_sub(this, std_vec_Vec2__4_mults(normal, (((i64)2) * std_vec_Vec2__4_dot(this, normal))));
}

std_vec_Vec2__4 std_vec_Vec2__4_divs(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__4_eq(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__4 std_vec_Vec2__4_subs(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__4_to_u32(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_subrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val - this.x), .y=(val - this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_sub(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x - other.x), .y=(this.y - other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__5_normalized(std_vec_Vec2__5 this) {
  f32 len = std_vec_Vec2__5_length(this);
  return (std_vec_Vec2__1){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len)};
}

std_vec_Vec2__5 std_vec_Vec2__5_multrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_add(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x + other.x), .y=(this.y + other.y)};
}

u32 std_vec_Vec2__5_length_sq(std_vec_Vec2__5 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__5_to_f64(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_mult(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_addrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__5_to_f32(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__5_to_i32(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__5_to_i64(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_reflect(std_vec_Vec2__5 this, std_vec_Vec2__5 normal) {
  return std_vec_Vec2__5_sub(this, std_vec_Vec2__5_mults(normal, (((u32)2) * std_vec_Vec2__5_dot(this, normal))));
}

std_vec_Vec2__5 std_vec_Vec2__5_divs(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__5_eq(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__5 std_vec_Vec2__5_subs(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__5_to_u32(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__5_to_u64(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_mults(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__5_hash(std_vec_Vec2__5 this) {
  return std_traits_hash_pair_hash(u32_hash(this.x), u32_hash(this.y));
}

std_vec_Vec2__5 std_vec_Vec2__5_adds(std_vec_Vec2__5 this, u32 val) {
  return (std_vec_Vec2__5){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__5 std_vec_Vec2__5_negate(std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__5_length(std_vec_Vec2__5 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

u32 std_vec_Vec2__5_dot(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (this.x * other.x) + (this.y * other.y);
}

u32 std_vec_Vec2__5_cross(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__5_rotate(std_vec_Vec2__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__5 std_vec_Vec2__5_div(std_vec_Vec2__5 this, std_vec_Vec2__5 other) {
  return (std_vec_Vec2__5){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__5 std_vec_Vec2__5_divrs(u32 val, std_vec_Vec2__5 this) {
  return (std_vec_Vec2__5){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__4_to_u64(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_mults(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__4_hash(std_vec_Vec2__4 this) {
  return std_traits_hash_pair_hash(i64_hash(this.x), i64_hash(this.y));
}

std_vec_Vec2__4 std_vec_Vec2__4_adds(std_vec_Vec2__4 this, i64 val) {
  return (std_vec_Vec2__4){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__4 std_vec_Vec2__4_negate(std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__4_length(std_vec_Vec2__4 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

i64 std_vec_Vec2__4_dot(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (this.x * other.x) + (this.y * other.y);
}

i64 std_vec_Vec2__4_cross(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__4_rotate(std_vec_Vec2__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__4 std_vec_Vec2__4_div(std_vec_Vec2__4 this, std_vec_Vec2__4 other) {
  return (std_vec_Vec2__4){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__4 std_vec_Vec2__4_divrs(i64 val, std_vec_Vec2__4 this) {
  return (std_vec_Vec2__4){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_reflect(std_vec_Vec2__3 this, std_vec_Vec2__3 normal) {
  return std_vec_Vec2__3_sub(this, std_vec_Vec2__3_mults(normal, (((i32)2) * std_vec_Vec2__3_dot(this, normal))));
}

std_vec_Vec2__3 std_vec_Vec2__3_divs(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__3_eq(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__3 std_vec_Vec2__3_subs(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__3_to_u32(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__3_to_u64(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_mults(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__3_hash(std_vec_Vec2__3 this) {
  return std_traits_hash_pair_hash(i32_hash(this.x), i32_hash(this.y));
}

std_vec_Vec2__3 std_vec_Vec2__3_adds(std_vec_Vec2__3 this, i32 val) {
  return (std_vec_Vec2__3){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__3 std_vec_Vec2__3_negate(std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__3_length(std_vec_Vec2__3 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

i32 std_vec_Vec2__3_dot(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (this.x * other.x) + (this.y * other.y);
}

i32 std_vec_Vec2__3_cross(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__3_rotate(std_vec_Vec2__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__3 std_vec_Vec2__3_div(std_vec_Vec2__3 this, std_vec_Vec2__3 other) {
  return (std_vec_Vec2__3){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__3 std_vec_Vec2__3_divrs(i32 val, std_vec_Vec2__3 this) {
  return (std_vec_Vec2__3){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__2_to_i64(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_reflect(std_vec_Vec2__2 this, std_vec_Vec2__2 normal) {
  return std_vec_Vec2__2_sub(this, std_vec_Vec2__2_mults(normal, (((f64)2) * std_vec_Vec2__2_dot(this, normal))));
}

std_vec_Vec2__2 std_vec_Vec2__2_divs(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__2_eq(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__2 std_vec_Vec2__2_subs(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__2_to_u32(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__2_to_u64(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_mults(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__2_hash(std_vec_Vec2__2 this) {
  return std_traits_hash_pair_hash(f64_hash(this.x), f64_hash(this.y));
}

std_vec_Vec2__2 std_vec_Vec2__2_adds(std_vec_Vec2__2 this, f64 val) {
  return (std_vec_Vec2__2){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__2 std_vec_Vec2__2_negate(std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__2_length(std_vec_Vec2__2 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

f64 std_vec_Vec2__2_dot(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (this.x * other.x) + (this.y * other.y);
}

f64 std_vec_Vec2__2_cross(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__2_rotate(std_vec_Vec2__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__2 std_vec_Vec2__2_div(std_vec_Vec2__2 this, std_vec_Vec2__2 other) {
  return (std_vec_Vec2__2){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__2 std_vec_Vec2__2_divrs(f64 val, std_vec_Vec2__2 this) {
  return (std_vec_Vec2__2){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_mult(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_addrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_to_f32(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__1_to_i32(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__1_to_i64(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_reflect(std_vec_Vec2__1 this, std_vec_Vec2__1 normal) {
  return std_vec_Vec2__1_sub(this, std_vec_Vec2__1_mults(normal, (((f32)2) * std_vec_Vec2__1_dot(this, normal))));
}

std_vec_Vec2__1 std_vec_Vec2__1_divs(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__1_eq(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__1 std_vec_Vec2__1_subs(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__1_to_u32(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__1_to_u64(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_mults(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__1_hash(std_vec_Vec2__1 this) {
  return std_traits_hash_pair_hash(f32_hash(this.x), f32_hash(this.y));
}

std_vec_Vec2__1 std_vec_Vec2__1_adds(std_vec_Vec2__1 this, f32 val) {
  return (std_vec_Vec2__1){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__1 std_vec_Vec2__1_negate(std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__1_length(std_vec_Vec2__1 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

f32 std_vec_Vec2__1_dot(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (this.x * other.x) + (this.y * other.y);
}

f32 std_vec_Vec2__1_cross(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__1_rotate(std_vec_Vec2__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__1 std_vec_Vec2__1_div(std_vec_Vec2__1 this, std_vec_Vec2__1 other) {
  return (std_vec_Vec2__1){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__1 std_vec_Vec2__1_divrs(f32 val, std_vec_Vec2__1 this) {
  return (std_vec_Vec2__1){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_multrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val * this.x), .y=(val * this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_add(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x + other.x), .y=(this.y + other.y)};
}

u64 std_vec_Vec2__0_length_sq(std_vec_Vec2__0 this) {
  return (this.x * this.x) + (this.y * this.y);
}

std_vec_Vec2__2 std_vec_Vec2__0_to_f64(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__2){.x=((f64)this.x), .y=((f64)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_mult(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x * other.x), .y=(this.y * other.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_addrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val + this.x), .y=(val + this.y)};
}

std_vec_Vec2__1 std_vec_Vec2__0_to_f32(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__1){.x=((f32)this.x), .y=((f32)this.y)};
}

std_vec_Vec2__3 std_vec_Vec2__0_to_i32(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__3){.x=((i32)this.x), .y=((i32)this.y)};
}

std_vec_Vec2__4 std_vec_Vec2__0_to_i64(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__4){.x=((i64)this.x), .y=((i64)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_reflect(std_vec_Vec2__0 this, std_vec_Vec2__0 normal) {
  return std_vec_Vec2__0_sub(this, std_vec_Vec2__0_mults(normal, (((u64)2) * std_vec_Vec2__0_dot(this, normal))));
}

std_vec_Vec2__0 std_vec_Vec2__0_divs(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x / val), .y=(this.y / val)};
}

bool std_vec_Vec2__0_eq(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return this.x==other.x && this.y==other.y;
}

std_vec_Vec2__0 std_vec_Vec2__0_subs(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x - val), .y=(this.y - val)};
}

std_vec_Vec2__5 std_vec_Vec2__0_to_u32(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__5){.x=((u32)this.x), .y=((u32)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_to_u64(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=((u64)this.x), .y=((u64)this.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_mults(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x * val), .y=(this.y * val)};
}

u32 std_vec_Vec2__0_hash(std_vec_Vec2__0 this) {
  return std_traits_hash_pair_hash(u64_hash(this.x), u64_hash(this.y));
}

std_vec_Vec2__0 std_vec_Vec2__0_adds(std_vec_Vec2__0 this, u64 val) {
  return (std_vec_Vec2__0){.x=(this.x + val), .y=(this.y + val)};
}

std_vec_Vec2__0 std_vec_Vec2__0_negate(std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=-this.x, .y=-this.y};
}

f32 std_vec_Vec2__0_length(std_vec_Vec2__0 this) {
  return sqrtf(((f32)((this.x * this.x) + (this.y * this.y))));
}

u64 std_vec_Vec2__0_dot(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (this.x * other.x) + (this.y * other.y);
}

u64 std_vec_Vec2__0_cross(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (this.x * other.y) - (this.y * other.x);
}

std_vec_Vec2__1 std_vec_Vec2__0_rotate(std_vec_Vec2__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  return (std_vec_Vec2__1){.x=((((f32)this->x) * c) - (((f32)this->y) * s)), .y=((((f32)this->x) * s) + (((f32)this->y) * c))};
}

std_vec_Vec2__0 std_vec_Vec2__0_div(std_vec_Vec2__0 this, std_vec_Vec2__0 other) {
  return (std_vec_Vec2__0){.x=(this.x / other.x), .y=(this.y / other.y)};
}

std_vec_Vec2__0 std_vec_Vec2__0_divrs(u64 val, std_vec_Vec2__0 this) {
  return (std_vec_Vec2__0){.x=(val / this.x), .y=(val / this.y)};
}

std_vec_Vec3__0 std_vec_Vec3__0_subrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_sub(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_rotateX(std_vec_Vec3__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__0_normalized(std_vec_Vec3__0 this) {
  f32 len = std_vec_Vec3__0_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__0 std_vec_Vec3__0_multrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_add(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

f32 std_vec_Vec3__0_length_sq(std_vec_Vec3__0 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__0_to_f64(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_subrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_sub(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_rotateX(std_vec_Vec3__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__1_normalized(std_vec_Vec3__1 this) {
  f32 len = std_vec_Vec3__1_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__1 std_vec_Vec3__1_multrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_add(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

f64 std_vec_Vec3__1_length_sq(std_vec_Vec3__1 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__1_to_f64(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_rotateY(std_vec_Vec3__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__1 std_vec_Vec3__1_mult(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_addrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_to_f32(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__1_rotateZ(std_vec_Vec3__1 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__1_to_i32(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_subrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_sub(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_rotateX(std_vec_Vec3__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__2_normalized(std_vec_Vec3__2 this) {
  f32 len = std_vec_Vec3__2_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__2 std_vec_Vec3__2_multrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_add(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

i32 std_vec_Vec3__2_length_sq(std_vec_Vec3__2 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__2_to_f64(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_rotateY(std_vec_Vec3__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__2 std_vec_Vec3__2_mult(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_addrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_to_f32(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__2_rotateZ(std_vec_Vec3__2 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_to_i32(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__2_to_i64(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_subrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_sub(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_rotateX(std_vec_Vec3__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__3_normalized(std_vec_Vec3__3 this) {
  f32 len = std_vec_Vec3__3_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__3 std_vec_Vec3__3_multrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_add(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

i64 std_vec_Vec3__3_length_sq(std_vec_Vec3__3 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__3_to_f64(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_rotateY(std_vec_Vec3__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__3 std_vec_Vec3__3_mult(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_addrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_to_f32(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__3_rotateZ(std_vec_Vec3__3 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__3_to_i32(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_to_i64(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_reflect(std_vec_Vec3__3 this, std_vec_Vec3__3 normal) {
  return std_vec_Vec3__3_sub(this, std_vec_Vec3__3_mults(normal, (((i64)2) * std_vec_Vec3__3_dot(this, normal))));
}

std_vec_Vec3__3 std_vec_Vec3__3_divs(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__3_eq(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__3 std_vec_Vec3__3_subs(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__3_to_u32(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_subrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_sub(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_rotateX(std_vec_Vec3__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__4_normalized(std_vec_Vec3__4 this) {
  f32 len = std_vec_Vec3__4_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__4 std_vec_Vec3__4_multrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_add(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

u32 std_vec_Vec3__4_length_sq(std_vec_Vec3__4 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__4_to_f64(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_rotateY(std_vec_Vec3__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__4 std_vec_Vec3__4_mult(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_addrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_to_f32(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__4_rotateZ(std_vec_Vec3__4 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__4_to_i32(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__4_to_i64(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_reflect(std_vec_Vec3__4 this, std_vec_Vec3__4 normal) {
  return std_vec_Vec3__4_sub(this, std_vec_Vec3__4_mults(normal, (((u32)2) * std_vec_Vec3__4_dot(this, normal))));
}

std_vec_Vec3__4 std_vec_Vec3__4_divs(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__4_eq(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__4 std_vec_Vec3__4_subs(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__4_to_u32(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__4_to_u64(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_subrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val - this.x), .y=(val - this.y), .z=(val - this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_sub(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x - other.x), .y=(this.y - other.y), .z=(this.z - other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_rotateX(std_vec_Vec3__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 y = ((((f32)this->y) * c) - (((f32)this->z) * s));
  f32 z = ((((f32)this->y) * s) + (((f32)this->z) * c));
  return (std_vec_Vec3__0){.x=((f32)this->x), .y=y, .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__5_normalized(std_vec_Vec3__5 this) {
  f32 len = std_vec_Vec3__5_length(this);
  return (std_vec_Vec3__0){.x=(((f32)this.x) / len), .y=(((f32)this.y) / len), .z=(((f32)this.z) / len)};
}

std_vec_Vec3__5 std_vec_Vec3__5_multrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val * this.x), .y=(val * this.y), .z=(val * this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_add(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x + other.x), .y=(this.y + other.y), .z=(this.z + other.z)};
}

u64 std_vec_Vec3__5_length_sq(std_vec_Vec3__5 this) {
  return ((this.x * this.x) + (this.y * this.y)) + (this.z * this.z);
}

std_vec_Vec3__1 std_vec_Vec3__5_to_f64(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__1){.x=((f64)this.x), .y=((f64)this.y), .z=((f64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_rotateY(std_vec_Vec3__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__5 std_vec_Vec3__5_mult(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_addrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_to_f32(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__5_rotateZ(std_vec_Vec3__5 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__5_to_i32(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__5_to_i64(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_reflect(std_vec_Vec3__5 this, std_vec_Vec3__5 normal) {
  return std_vec_Vec3__5_sub(this, std_vec_Vec3__5_mults(normal, (((u64)2) * std_vec_Vec3__5_dot(this, normal))));
}

std_vec_Vec3__5 std_vec_Vec3__5_divs(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__5_eq(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__5 std_vec_Vec3__5_subs(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__5_to_u32(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_to_u64(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_mults(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__5_hash(std_vec_Vec3__5 this) {
  return std_traits_hash_pair_hash(u64_hash(this.x), std_traits_hash_pair_hash(u64_hash(this.y), u64_hash(this.z)));
}

std_vec_Vec3__5 std_vec_Vec3__5_adds(std_vec_Vec3__5 this, u64 val) {
  return (std_vec_Vec3__5){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__5 std_vec_Vec3__5_negate(std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__5_length(std_vec_Vec3__5 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

u64 std_vec_Vec3__5_dot(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

u64 std_vec_Vec3__5_index(std_vec_Vec3__5 this, u32 idx) {
  return ((u64 *)&this)[idx];
}

std_vec_Vec3__5 std_vec_Vec3__5_cross(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__5 std_vec_Vec3__5_div(std_vec_Vec3__5 this, std_vec_Vec3__5 other) {
  return (std_vec_Vec3__5){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__5 std_vec_Vec3__5_divrs(u64 val, std_vec_Vec3__5 this) {
  return (std_vec_Vec3__5){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_mults(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__4_hash(std_vec_Vec3__4 this) {
  return std_traits_hash_pair_hash(u32_hash(this.x), std_traits_hash_pair_hash(u32_hash(this.y), u32_hash(this.z)));
}

std_vec_Vec3__4 std_vec_Vec3__4_adds(std_vec_Vec3__4 this, u32 val) {
  return (std_vec_Vec3__4){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__4 std_vec_Vec3__4_negate(std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__4_length(std_vec_Vec3__4 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

u32 std_vec_Vec3__4_dot(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

u32 std_vec_Vec3__4_index(std_vec_Vec3__4 this, u32 idx) {
  return ((u32 *)&this)[idx];
}

std_vec_Vec3__4 std_vec_Vec3__4_cross(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__4 std_vec_Vec3__4_div(std_vec_Vec3__4 this, std_vec_Vec3__4 other) {
  return (std_vec_Vec3__4){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__4 std_vec_Vec3__4_divrs(u32 val, std_vec_Vec3__4 this) {
  return (std_vec_Vec3__4){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__3_to_u64(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_mults(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__3_hash(std_vec_Vec3__3 this) {
  return std_traits_hash_pair_hash(i64_hash(this.x), std_traits_hash_pair_hash(i64_hash(this.y), i64_hash(this.z)));
}

std_vec_Vec3__3 std_vec_Vec3__3_adds(std_vec_Vec3__3 this, i64 val) {
  return (std_vec_Vec3__3){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__3 std_vec_Vec3__3_negate(std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__3_length(std_vec_Vec3__3 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

i64 std_vec_Vec3__3_dot(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

i64 std_vec_Vec3__3_index(std_vec_Vec3__3 this, u32 idx) {
  return ((i64 *)&this)[idx];
}

std_vec_Vec3__3 std_vec_Vec3__3_cross(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__3 std_vec_Vec3__3_div(std_vec_Vec3__3 this, std_vec_Vec3__3 other) {
  return (std_vec_Vec3__3){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__3 std_vec_Vec3__3_divrs(i64 val, std_vec_Vec3__3 this) {
  return (std_vec_Vec3__3){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_reflect(std_vec_Vec3__2 this, std_vec_Vec3__2 normal) {
  return std_vec_Vec3__2_sub(this, std_vec_Vec3__2_mults(normal, (((i32)2) * std_vec_Vec3__2_dot(this, normal))));
}

std_vec_Vec3__2 std_vec_Vec3__2_divs(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__2_eq(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__2 std_vec_Vec3__2_subs(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__2_to_u32(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__2_to_u64(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_mults(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__2_hash(std_vec_Vec3__2 this) {
  return std_traits_hash_pair_hash(i32_hash(this.x), std_traits_hash_pair_hash(i32_hash(this.y), i32_hash(this.z)));
}

std_vec_Vec3__2 std_vec_Vec3__2_adds(std_vec_Vec3__2 this, i32 val) {
  return (std_vec_Vec3__2){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__2 std_vec_Vec3__2_negate(std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__2_length(std_vec_Vec3__2 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

i32 std_vec_Vec3__2_dot(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

i32 std_vec_Vec3__2_index(std_vec_Vec3__2 this, u32 idx) {
  return ((i32 *)&this)[idx];
}

std_vec_Vec3__2 std_vec_Vec3__2_cross(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__2 std_vec_Vec3__2_div(std_vec_Vec3__2 this, std_vec_Vec3__2 other) {
  return (std_vec_Vec3__2){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__2 std_vec_Vec3__2_divrs(i32 val, std_vec_Vec3__2 this) {
  return (std_vec_Vec3__2){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__1_to_i64(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_reflect(std_vec_Vec3__1 this, std_vec_Vec3__1 normal) {
  return std_vec_Vec3__1_sub(this, std_vec_Vec3__1_mults(normal, (((f64)2) * std_vec_Vec3__1_dot(this, normal))));
}

std_vec_Vec3__1 std_vec_Vec3__1_divs(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__1_eq(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__1 std_vec_Vec3__1_subs(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__1_to_u32(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__1_to_u64(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_mults(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__1_hash(std_vec_Vec3__1 this) {
  return std_traits_hash_pair_hash(f64_hash(this.x), std_traits_hash_pair_hash(f64_hash(this.y), f64_hash(this.z)));
}

std_vec_Vec3__1 std_vec_Vec3__1_adds(std_vec_Vec3__1 this, f64 val) {
  return (std_vec_Vec3__1){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__1 std_vec_Vec3__1_negate(std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__1_length(std_vec_Vec3__1 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

f64 std_vec_Vec3__1_dot(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

f64 std_vec_Vec3__1_index(std_vec_Vec3__1 this, u32 idx) {
  return ((f64 *)&this)[idx];
}

std_vec_Vec3__1 std_vec_Vec3__1_cross(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__1 std_vec_Vec3__1_div(std_vec_Vec3__1 this, std_vec_Vec3__1 other) {
  return (std_vec_Vec3__1){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__1 std_vec_Vec3__1_divrs(f64 val, std_vec_Vec3__1 this) {
  return (std_vec_Vec3__1){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_rotateY(std_vec_Vec3__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 z = ((((f32)this->z) * c) - (((f32)this->x) * s));
  f32 x = ((((f32)this->z) * s) + (((f32)this->x) * c));
  return (std_vec_Vec3__0){.x=x, .y=((f32)this->y), .z=z};
}

std_vec_Vec3__0 std_vec_Vec3__0_mult(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x * other.x), .y=(this.y * other.y), .z=(this.z * other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_addrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val + this.x), .y=(val + this.y), .z=(val + this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_to_f32(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=((f32)this.x), .y=((f32)this.y), .z=((f32)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_rotateZ(std_vec_Vec3__0 *this, f32 angle) {
  f32 c = cosf(angle);
  f32 s = sinf(angle);
  f32 x = ((((f32)this->x) * c) - (((f32)this->y) * s));
  f32 y = ((((f32)this->x) * s) + (((f32)this->y) * c));
  return (std_vec_Vec3__0){.x=x, .y=y, .z=((f32)this->z)};
}

std_vec_Vec3__2 std_vec_Vec3__0_to_i32(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__2){.x=((i32)this.x), .y=((i32)this.y), .z=((i32)this.z)};
}

std_vec_Vec3__3 std_vec_Vec3__0_to_i64(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__3){.x=((i64)this.x), .y=((i64)this.y), .z=((i64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_reflect(std_vec_Vec3__0 this, std_vec_Vec3__0 normal) {
  return std_vec_Vec3__0_sub(this, std_vec_Vec3__0_mults(normal, (((f32)2) * std_vec_Vec3__0_dot(this, normal))));
}

std_vec_Vec3__0 std_vec_Vec3__0_divs(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x / val), .y=(this.y / val), .z=(this.z / val)};
}

bool std_vec_Vec3__0_eq(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (this.x==other.x && this.y==other.y) && this.z==other.z;
}

std_vec_Vec3__0 std_vec_Vec3__0_subs(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x - val), .y=(this.y - val), .z=(this.z - val)};
}

std_vec_Vec3__4 std_vec_Vec3__0_to_u32(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__4){.x=((u32)this.x), .y=((u32)this.y), .z=((u32)this.z)};
}

std_vec_Vec3__5 std_vec_Vec3__0_to_u64(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__5){.x=((u64)this.x), .y=((u64)this.y), .z=((u64)this.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_mults(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x * val), .y=(this.y * val), .z=(this.z * val)};
}

u32 std_vec_Vec3__0_hash(std_vec_Vec3__0 this) {
  return std_traits_hash_pair_hash(f32_hash(this.x), std_traits_hash_pair_hash(f32_hash(this.y), f32_hash(this.z)));
}

std_vec_Vec3__0 std_vec_Vec3__0_adds(std_vec_Vec3__0 this, f32 val) {
  return (std_vec_Vec3__0){.x=(this.x + val), .y=(this.y + val), .z=(this.z + val)};
}

std_vec_Vec3__0 std_vec_Vec3__0_negate(std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=-this.x, .y=-this.y, .z=-this.z};
}

f32 std_vec_Vec3__0_length(std_vec_Vec3__0 this) {
  return sqrtf(((f32)(((this.x * this.x) + (this.y * this.y)) + (this.z * this.z))));
}

f32 std_vec_Vec3__0_dot(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return ((this.x * other.x) + (this.y * other.y)) + (this.z * other.z);
}

f32 std_vec_Vec3__0_index(std_vec_Vec3__0 this, u32 idx) {
  return ((f32 *)&this)[idx];
}

std_vec_Vec3__0 std_vec_Vec3__0_cross(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=((this.y * other.z) - (this.z * other.y)), .y=((this.z * other.x) - (this.x * other.z)), .z=((this.x * other.y) - (this.y * other.x))};
}

std_vec_Vec3__0 std_vec_Vec3__0_div(std_vec_Vec3__0 this, std_vec_Vec3__0 other) {
  return (std_vec_Vec3__0){.x=(this.x / other.x), .y=(this.y / other.y), .z=(this.z / other.z)};
}

std_vec_Vec3__0 std_vec_Vec3__0_divrs(f32 val, std_vec_Vec3__0 this) {
  return (std_vec_Vec3__0){.x=(val / this.x), .y=(val / this.y), .z=(val / this.z)};
}

f32 f32_inf(void) {
  return ((f32)INFINITY);
}

f64 f64_inf(void) {
  return INFINITY;
}

f32 f32_min(f32 this, f32 b) {
  return ((this < b) ? this : b);
}

f32 f32_max(f32 this, f32 b) {
  return ((this > b) ? this : b);
}

f64 f64_min(f64 this, f64 b) {
  return ((this < b) ? this : b);
}

f64 f64_max(f64 this, f64 b) {
  return ((this > b) ? this : b);
}

f32 f32_clamp(f32 this, f32 min, f32 max) {
  return f32_max(f32_min(this, max), min);
}

f32 f32_clamp01(f32 this) {
  return f32_clamp(this, 0.0f, 1.0f);
}

f64 f64_clamp(f64 this, f64 min, f64 max) {
  return f64_max(f64_min(this, max), min);
}

f64 f64_clamp01(f64 this) {
  return f64_clamp(this, 0.0, 1.0);
}

bool f32_is_close(f32 this, f32 other, f32 thresh) {
  return fabsf((this - other)) < thresh;
}

bool f64_is_close(f64 this, f64 other, f64 thresh) {
  return fabs((this - other)) < thresh;
}

i8 i8_abs(i8 this) {
  return ((this < ((i8)0)) ? -this : this);
}

i16 i16_abs(i16 this) {
  return ((this < ((i16)0)) ? -this : this);
}

i32 i32_abs(i32 this) {
  return ((this < 0) ? -this : this);
}

i64 i64_abs(i64 this) {
  return ((this < ((i64)0)) ? -this : this);
}

f32 std_math_degrees(f32 radians) {
  return (radians * 180.0f) / M_PI;
}

f32 std_math_radians(f32 degrees) {
  return (degrees * M_PI) / 180.0f;
}

std_buffer_Buffer std_buffer_Buffer_make(u32 capacity) {
  return (std_buffer_Buffer){.data=std_mem_alloc__4(capacity), .size=0, .capacity=capacity};
}

std_buffer_Buffer std_buffer_Buffer_from_str(char *s) {
  return (std_buffer_Buffer){.data=((u8 *)s), .size=((u32)strlen(s)), .capacity=((u32)strlen(s))};
}

std_buffer_Buffer std_buffer_Buffer_from_sv(std_sv_SV sv) {
  u8 *data = std_mem_alloc__4((sv.len + 1));
  memcpy(data, sv.data, sv.len);
  data[sv.len]=((u8)'\0');
  return (std_buffer_Buffer){.data=data, .size=sv.len, .capacity=sv.len};
}

std_buffer_Buffer std_buffer_Buffer_from_sized_str(char *s, u32 size) {
  return (std_buffer_Buffer){.data=((u8 *)s), .size=((u32)size), .capacity=((u32)size)};
}

void std_buffer_Buffer_resize_if_necessary(std_buffer_Buffer *this, u32 new_size) {
  if ((new_size + 1) >= this->capacity) {
    u32 new_capacity = u32_max(((this->capacity * 3) / 2), (new_size + 1));
    this->data=std_mem_realloc__0(this->data, this->capacity, new_capacity);
    memset((this->data + this->capacity), ((u8)0), (new_capacity - this->capacity));
    this->capacity=((u32)new_capacity);
    if(!(((bool)this->data))) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/buffer.oc:65:16: Assertion failed: `.data?`", "Out of memory!"); }
  }
}

void std_buffer_hex_dump(u8 *data, u32 size) {
  printf("(%d bytes): ", size);
  for (u32 i = 0; i < size; i+=1) {
    if ((i % 4)==0 && (i > 0)) {
      printf("_");
    }
    printf("%02x", data[i]);
  }
  printf("""\n");
}

void std_buffer_Buffer_hex_dump(std_buffer_Buffer this) {
  std_buffer_hex_dump(this.data, this.size);
}

u8 std_buffer_Buffer_get_byte_at(std_buffer_Buffer this, u32 index) {
  if(!(index < this.size)) { ae_assert_fail("/home/runner/work/raycasting/raycasting/ocen/std/buffer.oc:82:12: Assertion failed: `index < .size`", "Index out of bounds"); }
  return this.data[index];
}

void std_buffer_Buffer_write_buf(std_buffer_Buffer *this, std_buffer_Buffer *buf) {
  std_buffer_Buffer_resize_if_necessary(this, (this->size + buf->size));
  memcpy((this->data + this->size), buf->data, buf->size);
  this->size+=buf->size;
}

void std_buffer_Buffer_write_buf_f(std_buffer_Buffer *this, std_buffer_Buffer *buf) {
  std_buffer_Buffer_write_buf(this, buf);
  std_buffer_Buffer_free(buf);
}

void std_buffer_Buffer_write_str(std_buffer_Buffer *this, char *s) {
  u32 len = ((u32)strlen(s));
  std_buffer_Buffer_resize_if_necessary(this, (this->size + len));
  memcpy((this->data + this->size), s, len);
  this->size+=len;
}

void std_buffer_Buffer_write_str_f(std_buffer_Buffer *this, char *s) {
  std_buffer_Buffer_write_str(this, s);
  std_mem_free(s);
}

void std_buffer_Buffer_write_char(std_buffer_Buffer *this, char c) {
  std_buffer_Buffer_write_u8(this, ((u8)c));
}

void std_buffer_Buffer_write_bytes(std_buffer_Buffer *this, void *bytes, u32 size) {
  std_buffer_Buffer_resize_if_necessary(this, (this->size + size));
  memcpy((this->data + this->size), bytes, size);
  this->size+=size;
}

void std_buffer_Buffer_write_sv(std_buffer_Buffer *this, std_sv_SV sv) {
  std_buffer_Buffer_write_bytes(this, ((u8 *)sv.data), sv.len);
}

void std_buffer_Buffer_write_i64(std_buffer_Buffer *this, i64 value, std_Endian endian) {
  std_buffer_Buffer_write_u64(this, ((u64)value), endian);
}

void std_buffer_Buffer_write_u64(std_buffer_Buffer *this, u64 value, std_Endian endian) {
  std_buffer_Buffer_resize_if_necessary(this, (this->size + 8));
  for (u32 i = 0; i < 8; i+=1) {
    u32 shift = (endian==std_Endian_Big ? i : (7 - i));
    this->data[(this->size + i)]=((u8)((value >> (((u64)56) - (((u64)shift) * ((u64)8)))) & ((u64)0xff)));
  }
  this->size+=8;
}

void std_buffer_Buffer_write_i32(std_buffer_Buffer *this, i32 value, std_Endian endian) {
  std_buffer_Buffer_write_u32(this, ((u32)value), endian);
}

void std_buffer_Buffer_write_u32(std_buffer_Buffer *this, u32 value, std_Endian endian) {
  std_buffer_Buffer_resize_if_necessary(this, (this->size + 4));
  for (u32 i = 0; i < 4; i+=1) {
    u32 shift = (endian==std_Endian_Big ? i : (3 - i));
    this->data[(this->size + i)]=((u8)((value >> (24 - (shift * 8))) & 0xff));
  }
  this->size+=4;
}

void std_buffer_Buffer_write_i16(std_buffer_Buffer *this, i16 value, std_Endian endian) {
  std_buffer_Buffer_write_u16(this, ((u16)value), endian);
}

void std_buffer_Buffer_write_u16(std_buffer_Buffer *this, u16 value, std_Endian endian) {
  std_buffer_Buffer_resize_if_necessary(this, (this->size + 2));
  for (u32 i = 0; i < 2; i+=1) {
    u32 shift = (endian==std_Endian_Big ? i : (1 - i));
    this->data[(this->size + i)]=((u8)((value >> (((u16)8) - (((u16)shift) * ((u16)8)))) & ((u16)0xff)));
  }
  this->size+=2;
}

void std_buffer_Buffer_write_i8(std_buffer_Buffer *this, i8 value) {
  std_buffer_Buffer_write_u8(this, ((u8)value));
}

void std_buffer_Buffer_write_u8(std_buffer_Buffer *this, u8 value) {
  std_buffer_Buffer_resize_if_necessary(this, (this->size + 1));
  this->data[this->size]=value;
  this->size+=1;
}

std_buffer_BytesReader std_buffer_Buffer_reader(std_buffer_Buffer this) {
  return std_buffer_BytesReader_make(this.data, this.size);
}

char *std_buffer_Buffer_new_str(std_buffer_Buffer this) {
  return strdup(((char *)this.data));
}

char *std_buffer_Buffer_str(std_buffer_Buffer this) {
  return ((char *)this.data);
}

std_sv_SV std_buffer_Buffer_sv(std_buffer_Buffer this) {
  return (std_sv_SV){.data=((char *)this.data), .len=this.size};
}

std_buffer_Buffer std_buffer_Buffer_copy(std_buffer_Buffer *this) {
  u8 *new_data = std_mem_alloc__4(this->capacity);
  memcpy(new_data, this->data, this->size);
  return (std_buffer_Buffer){.data=new_data, .size=this->size, .capacity=this->capacity};
}

void std_buffer_Buffer_clear(std_buffer_Buffer *this) {
  this->size=0;
  memset(this->data, ((u8)0), this->capacity);
}

void std_buffer_Buffer_free(std_buffer_Buffer *this) {
  std_mem_free(this->data);
}

std_buffer_BytesReader std_buffer_BytesReader_make(u8 *data, u32 size) {
  return (std_buffer_BytesReader){.data=data, .size=size, .index=0};
}

i64 std_buffer_BytesReader_read_i64(std_buffer_BytesReader *this, std_Endian endian) {
  return ((i64)std_buffer_BytesReader_read_u64(this, endian));
}

u64 std_buffer_BytesReader_read_u64(std_buffer_BytesReader *this, std_Endian endian) {
  u64 value = ((u64)0);
  for (u32 i = 0; i < 8; i+=1) {
    u32 shift = (endian==std_Endian_Big ? i : (7 - i));
    value=(value | (((u64)this->data[(this->index + i)]) << (((u64)56) - (((u64)shift) * ((u64)8)))));
  }
  this->index+=8;
  return ((u64)value);
}

i32 std_buffer_BytesReader_read_i32(std_buffer_BytesReader *this, std_Endian endian) {
  return ((i32)std_buffer_BytesReader_read_u32(this, endian));
}

u32 std_buffer_BytesReader_read_u32(std_buffer_BytesReader *this, std_Endian endian) {
  u32 value = 0;
  for (u32 i = 0; i < 4; i+=1) {
    u32 shift = (endian==std_Endian_Big ? i : (3 - i));
    value=(value | (((u32)this->data[(this->index + i)]) << (24 - (shift * 8))));
  }
  this->index+=4;
  return value;
}

i16 std_buffer_BytesReader_read_i16(std_buffer_BytesReader *this, std_Endian endian) {
  return ((i16)std_buffer_BytesReader_read_u16(this, endian));
}

u16 std_buffer_BytesReader_read_u16(std_buffer_BytesReader *this, std_Endian endian) {
  u16 value = ((u16)0);
  for (u32 i = 0; i < 2; i+=1) {
    u32 shift = (endian==std_Endian_Big ? i : (1 - i));
    value=(value | (((u16)this->data[(this->index + i)]) << (((u16)8) - (((u16)shift) * ((u16)8)))));
  }
  this->index+=2;
  return ((u16)value);
}

i8 std_buffer_BytesReader_read_i8(std_buffer_BytesReader *this) {
  return ((i8)std_buffer_BytesReader_read_u8(this));
}

u8 std_buffer_BytesReader_read_u8(std_buffer_BytesReader *this) {
  u8 value = this->data[this->index];
  this->index+=1;
  return value;
}

void std_buffer_BytesReader_read_bytes(std_buffer_BytesReader *this, void *_dst, u32 count) {
  u8 *dst = ((u8 *)_dst);
  if (((bool)dst)) {
    for (u32 i = 0; i < count; i+=1) {
      dst[i]=this->data[(((u32)this->index) + i)];
    }
  }
  this->index+=count;
}

std_sv_SV std_buffer_BytesReader_read_bytes_sv(std_buffer_BytesReader *this, u32 count) {
  std_sv_SV sv = (std_sv_SV){.data=((char *)(this->data + this->index)), .len=count};
  this->index+=count;
  return sv;
}

bool std_buffer_BytesReader_is_empty(std_buffer_BytesReader *this) {
  return this->index >= this->size;
}

std_buffer_Buffer std_zlib_compress(std_buffer_Buffer data) {
  uLongf capacity = compressBound(((u64)data.size));
  std_buffer_Buffer dest = std_buffer_Buffer_make(((u32)capacity));
  uLongf dest_len = capacity;
  i32 result = compress(dest.data, &dest_len, data.data, ((u64)data.size));
  if (result != Z_OK) {
    std_panic("zlib: failed to compress data");
  }
  dest.size=((u32)dest_len);
  return dest;
}

std_buffer_Buffer std_zlib_compress_str(char *s) {
  return std_zlib_compress(std_buffer_Buffer_from_str(s));
}

std_buffer_Buffer std_zlib_decompress_bytes(u8 *data, u32 size, u32 *bytes_consumed) {
  z_stream stream = {0};
  stream.avail_in=size;
  stream.next_in=data;
  if (inflateInit(&stream) != Z_OK) {
    std_panic("Failed to initialize zlib stream");
  }
  std_buffer_Buffer res = std_buffer_Buffer_make((size * 2));
  stream.avail_out=res.capacity;
  stream.next_out=res.data;
  bool done = false;
  while (!done) {
    if (stream.avail_out==0) {
      u32 new_size = (res.capacity * 2);
      std_buffer_Buffer_resize_if_necessary(&res, new_size);
      stream.avail_out=(new_size - stream.total_out);
      stream.next_out=(res.data + stream.total_out);
    }
    i32 result = inflate(&stream, Z_NO_FLUSH);
    switch (result) {
      case Z_OK: {
      } break;
      case Z_STREAM_END: {
        res.size=stream.total_out;
        done=true;
      } break;
      default: std_panic("zlib: failed to decompress data"); break;
    }
  }
  if (((bool)bytes_consumed)) {
    (*bytes_consumed)=((u32)(stream.next_in - data));
  } else {
    if (((u32)(stream.next_in - data)) != size) {
      std_panic("zlib: not all bytes were consumed, use `bytes_consumed` to get the number of bytes consumed");
    }
  }
  inflateEnd(&stream);
  return res;
}

std_buffer_Buffer std_zlib_decompress(std_buffer_Buffer data, u32 *bytes_consumed) {
  return std_zlib_decompress_bytes(data.data, data.size, bytes_consumed);
}

u32 std_traits_hash_hash_bytes(u8 *data, u32 len) {
  u32 hash = 5381;
  for (u32 i = 0; i < len; i+=1) {
    hash=((hash * 33) ^ ((u32)data[i]));
  }
  return hash;
}

u32 std_traits_hash_pair_hash(u32 a, u32 b) {
  return (a * 33) ^ b;
}

u32 std_traits_hash_ptr_hash(void *ptr) {
  return u64_hash(((u64)ptr));
}

u32 str_hash(char *this) {
  return std_traits_hash_hash_bytes(((u8 *)this), strlen(this));
}

u32 u8_hash(u8 this) {
  return ((u32)this) * 7817;
}

u32 i8_hash(i8 this) {
  return ((u32)this) * 7817;
}

u32 u16_hash(u16 this) {
  return ((u32)this) * 7817;
}

u32 i16_hash(i16 this) {
  return ((u32)this) * 7817;
}

u32 u32_hash(u32 this) {
  return ((u32)this) * 7817;
}

u32 i32_hash(i32 this) {
  return ((u32)this) * 7817;
}

u32 u64_hash(u64 this) {
  return std_traits_hash_pair_hash(u32_hash(((u32)this)), u32_hash(((u32)(this >> ((u64)32)))));
}

u32 i64_hash(i64 this) {
  return std_traits_hash_pair_hash(u32_hash(((u32)this)), u32_hash(((u32)(this >> ((i64)32)))));
}

u32 char_hash(char this) {
  return ((u32)this) * 7817;
}

u32 f32_hash(f32 this) {
  return ((u32)this) * 7817;
}

u32 f64_hash(f64 this) {
  return u64_hash(((u64)this));
}

std_og_Rect std_og_Rect_from_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size) {
  return (std_og_Rect){.x=pos.x, .y=pos.y, .w=size.x, .h=size.y};
}

bool std_og_Rect_contains(std_og_Rect this, std_vec_Vec2__3 pos) {
  if (!((this.x <= pos.x) && (pos.x <= (this.x + this.w)))) {
    return false;
  }
  if (!((this.y <= pos.y) && (pos.y <= (this.y + this.h)))) {
    return false;
  }
  return true;
}

u32 std_og_get_ticks(void) {
  return ((u32)SDL_GetTicks());
}

void std_og_init(u32 width, u32 height, char *title, bool fullscreen) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_CreateWindowAndRenderer(((i32)width), ((i32)height), SDL_WINDOW_RESIZABLE, &std_og_state_window, &std_og_state_renderer);
  if (fullscreen) {
    SDL_SetWindowFullscreen(std_og_state_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  std_og_state_title=title;
  SDL_SetWindowTitle(std_og_state_window, title);
  std_og_state_prev_time=std_og_get_ticks();
  std_og_state_update_window_size();
}

void std_og_toggle_fullscreen(void) {
  i32 flags = SDL_GetWindowFlags(std_og_state_window);
  if ((flags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0) {
    SDL_SetWindowFullscreen(std_og_state_window, 0);
  } else {
    SDL_SetWindowFullscreen(std_og_state_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
}

bool std_og_is_running(void) {
  SDL_RenderPresent(std_og_state_renderer);
  std_og_state_frame_num+=((u64)1);
  u32 cur_time = ((u32)std_og_get_ticks());
  std_og_state_frame_time=(((f32)(cur_time - std_og_state_prev_time)) / 1000.0f);
  std_og_state_prev_time=cur_time;
  std_og_state_update_window_size();
  std_og_state_add_frametime(std_og_state_frame_time);
  if ((std_og_state_frame_num % ((u64)30))==((u64)0)) {
    f32 avg = std_og_state_get_avg_frametime();
    sprintf(std_og_state_window_title_fps_buf, "%s | FPS: %.0f", std_og_state_title, (1.0f / avg));
    SDL_SetWindowTitle(std_og_state_window, std_og_state_window_title_fps_buf);
  }
  std_og_utils_handle_sdl_events();
  return std_og_state_running;
}

void std_og_clear(SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_RenderClear(std_og_state_renderer);
}

void std_og_draw_rect(i32 x, i32 y, i32 w, i32 h, SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_Rect rect = (SDL_Rect){.x=x, .y=y, .w=w, .h=h};
  SDL_RenderFillRect(std_og_state_renderer, &rect);
}

void std_og_draw_rect_v(std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col) {
  std_og_draw_rect(pos.x, pos.y, size.x, size.y, col);
}

void std_og_draw_line(i32 x1, i32 y1, i32 x2, i32 y2, SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_RenderDrawLine(std_og_state_renderer, x1, y1, x2, y2);
}

void std_og_draw_line_v(std_vec_Vec2__3 p1, std_vec_Vec2__3 p2, SDL_Color col) {
  std_og_draw_line(p1.x, p1.y, p2.x, p2.y, col);
}

void std_og_draw_pixel(i32 x, i32 y, SDL_Color col) {
  SDL_SetRenderDrawColor(std_og_state_renderer, col.r, col.g, col.b, col.a);
  SDL_RenderDrawPoint(std_og_state_renderer, x, y);
}

void std_og_draw_pixel_v(std_vec_Vec2__3 pos, SDL_Color col) {
  std_og_draw_pixel(pos.x, pos.y, col);
}

f32 std_og_get_frame_time(void) {
  return std_og_state_frame_time;
}

void std_og_quit(void) {
  SDL_Quit();
}

bool std_og_is_key_pressed(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_prev_keys, k)==false && std_og_state_KeyState_get(&std_og_state_keys, k)==true;
}

bool std_og_is_key_released(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_prev_keys, k)==true && std_og_state_KeyState_get(&std_og_state_keys, k)==false;
}

bool std_og_is_key_down(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_keys, k)==true;
}

bool std_og_is_key_up(SDL_Scancode k) {
  return std_og_state_KeyState_get(&std_og_state_keys, k)==false;
}

bool std_og_is_mouse_pressed(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_prev_mouse, b)==false && std_og_state_MouseState_get_button(&std_og_state_mouse, b)==true;
}

bool std_og_is_mouse_released(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_prev_mouse, b)==true && std_og_state_MouseState_get_button(&std_og_state_mouse, b)==false;
}

bool std_og_is_mouse_down(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_mouse, b)==true;
}

bool std_og_is_mouse_up(u8 b) {
  return std_og_state_MouseState_get_button(&std_og_state_mouse, b)==false;
}

std_vec_Vec2__3 std_og_get_window_size(void) {
  return std_og_state_window_size;
}

bool std_og_is_scroll(std_og_state_MouseWheel w) {
  return std_og_state_MouseState_get_wheel(&std_og_state_mouse, w);
}

std_vec_Vec2__3 std_og_get_mouse_pos(void) {
  return std_og_state_mouse.pos;
}

std_vec_Vec2__3 std_og_get_mouse_delta(void) {
  return std_og_state_mouse.vec;
}

void std_og_capture_mouse(bool capture) {
  SDL_CaptureMouse(capture);
}

void std_og_grab_input(bool grab) {
  SDL_SetRelativeMouseMode(grab);
}

void std_og_show_cursor(bool show) {
  SDL_ShowCursor(show);
}

void std_og_set_cursor(SDL_SystemCursor c) {
  if (c==std_og_state_cursor_type) {
    return;
  }
  if (((bool)std_og_state_cursor)) {
    SDL_FreeCursor(std_og_state_cursor);
  }
  std_og_state_cursor=SDL_CreateSystemCursor(c);
  std_og_state_cursor_type=c;
  SDL_SetCursor(std_og_state_cursor);
}

void std_og_Texture_draw_pixel(std_og_Texture this, i32 x, i32 y, SDL_Color col) {
  if (((0 <= x) && (x < this.w)) && ((0 <= y) && (y < this.h))) {
    u32 offset = ((u32)((y * this.pitch) + (x * 4)));
    this.pixels[(offset + 0)]=((u8)col.r);
    this.pixels[(offset + 1)]=((u8)col.g);
    this.pixels[(offset + 2)]=((u8)col.b);
    this.pixels[(offset + 3)]=((u8)col.a);
  }
}

void std_og_Texture_draw_rect(std_og_Texture this, i32 x, i32 y, i32 w, i32 h, SDL_Color col) {
  x=i32_max(x, 0);
  y=i32_max(y, 0);
  w=i32_min(w, (this.w - x));
  h=i32_min(h, (this.h - y));
  for (i32 i = 0; i < h; i+=1) {
    i32 py = (y + i);
    u8 *view = (this.pixels + ((u32)((py * this.pitch) + (x * 4))));
    for (i32 j = 0; j < w; j+=1) {
      (*(view + 0))=((u8)col.r);
      (*(view + 1))=((u8)col.g);
      (*(view + 2))=((u8)col.b);
      (*(view + 3))=((u8)col.a);
      view=(view + 4);
    }
  }
}

void std_og_Texture_draw_rect_v(std_og_Texture this, std_vec_Vec2__3 pos, std_vec_Vec2__3 size, SDL_Color col) {
  std_og_Texture_draw_rect(this, pos.x, pos.y, size.x, size.y, col);
}

void std_og_Texture_draw_pixel_v(std_og_Texture this, std_vec_Vec2__3 pos, SDL_Color col) {
  std_og_Texture_draw_pixel(this, pos.x, pos.y, col);
}

std_og_Texture *std_og_get_buffer(void) {
  bool matches = true;
  if (!((bool)std_og_state_buffer.tx)) {
    matches=false;
  } else if ((std_og_state_buffer.w != std_og_state_window_size.x) || (std_og_state_buffer.h != std_og_state_window_size.y)) {
    SDL_DestroyTexture(std_og_state_buffer.tx);
    matches=false;
  }
  if (!matches) {
    std_og_Texture buf = {0};
    buf.tx=SDL_CreateTexture(std_og_state_renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, std_og_state_window_size.x, std_og_state_window_size.y);
    buf.w=std_og_state_window_size.x;
    buf.h=std_og_state_window_size.y;
    std_og_state_buffer=buf;
  }
  SDL_LockTexture(std_og_state_buffer.tx, NULL, ((void **)&std_og_state_buffer.pixels), &std_og_state_buffer.pitch);
  return &std_og_state_buffer;
}

void std_og_draw_buffer(void) {
  SDL_UnlockTexture(std_og_state_buffer.tx);
  SDL_RenderCopy(std_og_state_renderer, std_og_state_buffer.tx, NULL, NULL);
}

void std_og_set_main_loop(bool (*callback)(void)) {
  std_og_internal_user_main_fn=callback;
  c_set_main_loop(std_og_internal_og_main_fn);
}

bool std_og_state_KeyState_get(std_og_state_KeyState *this, SDL_Scancode k) {
  return this->keys[((u32)k)];
}

bool std_og_state_MouseState_get_button(std_og_state_MouseState *this, u8 b) {
  return this->buttons[((u32)b)];
}

bool std_og_state_MouseState_get_wheel(std_og_state_MouseState *this, std_og_state_MouseWheel w) {
  return this->wheel_dirs[((u32)w)];
}

void std_og_state_update_window_size(void) {
  SDL_GetWindowSize(std_og_state_window, &std_og_state_window_size.x, &std_og_state_window_size.y);
}

void std_og_state_add_frametime(f32 frametime) {
  std_og_state_frametime_total+=(frametime - std_og_state_frametimes[std_og_state_frametime_index]);
  std_og_state_frametimes[std_og_state_frametime_index]=frametime;
  std_og_state_frametime_index=((std_og_state_frametime_index + 1) % std_og_state_FRAMETIMES_COUNT);
}

f32 std_og_state_get_avg_frametime(void) {
  return std_og_state_frametime_total / ((f32)std_og_state_FRAMETIMES_COUNT);
}

void std_og_utils_handle_sdl_events(void) {
  std_og_state_prev_mouse=std_og_state_mouse;
  std_og_state_prev_keys=std_og_state_keys;
  SDL_Event event = {0};
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_QUIT: {
        std_og_state_running=false;
      } break;
      case SDL_KEYDOWN: {
        SDL_Scancode k = event.key.keysym.scancode;
        if (((i32)k) < 1024) {
          std_og_state_keys.keys[((i32)k)]=true;
        }
      } break;
      case SDL_KEYUP: {
        SDL_Scancode k = event.key.keysym.scancode;
        if (((i32)k) < 1024) {
          std_og_state_keys.keys[((i32)k)]=false;
        }
      } break;
      case SDL_MOUSEBUTTONDOWN: {
        u8 b = event.button.button;
        std_og_state_mouse.buttons[((i32)b)]=true;
      } break;
      case SDL_MOUSEBUTTONUP: {
        u8 b = event.button.button;
        std_og_state_mouse.buttons[((i32)b)]=false;
      } break;
      case SDL_MOUSEWHEEL: {
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Right)]=(event.wheel.x > 0);
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Left)]=(event.wheel.x < 0);
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Down)]=(event.wheel.y > 0);
        std_og_state_mouse.wheel_dirs[((i32)std_og_state_MouseWheel_Up)]=(event.wheel.y < 0);
      } break;
      default: {
      } break;
    }
  }
  SDL_GetMouseState(&std_og_state_mouse.pos.x, &std_og_state_mouse.pos.y);
  SDL_GetRelativeMouseState(&std_og_state_mouse.vec.x, &std_og_state_mouse.vec.y);
}

bool std_og_internal_og_main_fn(void) {
  if (!std_og_is_running()) {
    return false;
  }
  if (std_og_internal_user_main_fn != NULL) {
    return std_og_internal_user_main_fn();
  }
  return true;
}

