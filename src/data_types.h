#pragma once

#include <stdint.h>

#define NULL_ID 0xFFFFFFFF
#define NONE -1

#define HALO_TWO_PI 6.28318530718f
#define HALO_PI 3.14159265359f
#define HALO_HALF_PI 1.570796326795f
#define HALO_QUARTER_PI 0.7853981633974f
#define HALO_FLOAT_EPSILON 0.0001f

#define DEGREES_TO_RADIANS(deg) (deg * 0.01745329251994f)
#define RADIANS_TO_DEGREES(rad) (rad * 57.29577951308f)
#define FLOOR(n, floor) ((n)<(floor)?(floor):(n))
#define CEILING(n, ceiling) ((n)>(ceiling)?(ceiling):(n))
#define PIN(n, floor, ceiling) ((n)<(floor) ? (floor) : CEILING(n, ceiling))
#define fast_ftol_floor(x) fast_ftol(floor(x))
#define FLAG(b) (1<<(b))
#define TEST_FLAG(f, b) (((f)&FLAG(b))!=0)
#define SET_FLAG(f, b, v) ((v) ? ((f)|=FLAG(b)) : ((f)&=~FLAG(b)))

typedef uint32_t Pointer32;
typedef uint32_t Pixel32;

typedef union HaloID {
    uint32_t whole_id;
    struct {
        uint16_t index;
        uint16_t id;
    };
} HaloID;
static_assert(sizeof(HaloID) == 4);

typedef HaloID TagID;

typedef struct String32 {
    char string[32];
} String32;
static_assert(sizeof(String32) == 32);

typedef struct int16_bounds {
    int16_t lower;
    int16_t upper;
} int16_bounds;
static_assert(sizeof(int16_bounds) == 4);

typedef struct vector2d {
    int16_t x;
    int16_t y;
} vector2d;
static_assert(sizeof(vector2d) == 4);

typedef struct vector3d {
    int16_t x;
    int16_t y;
    int16_t z;
} vector3d;
static_assert(sizeof(vector3d) == 6);

typedef vector2d point2d;
typedef vector3d point3d;

typedef struct rectangle2d {
    int16_t y0;
    int16_t x0;
    int16_t y1;
    int16_t x1;
} rectangle2d;
static_assert(sizeof(rectangle2d) == 8);

typedef struct float_rectangle2d {
    float x0;
    float x1;
    float y0;
    float y1;
} float_rectangle2d;
static_assert(sizeof(float_rectangle2d) == 16);

typedef struct float_rectangle3d {
    float x0;
    float x1;
    float y0;
    float y1;
    float z0;
    float z1;
} float_rectangle3d;
static_assert(sizeof(float_rectangle3d) == 24);

typedef struct float_bounds {
    float lower;
    float upper;
} float_bounds;
static_assert(sizeof(float_bounds) == 8);

typedef struct float_rgb_color {
    float red;
    float green;
    float blue;
} float_rgb_color;
static_assert(sizeof(float_rgb_color) == 12);

typedef struct float_rgb_color_bounds {
    float_rgb_color lower;
    float_rgb_color upper;
} float_rgb_color_bounds;
static_assert(sizeof(float_rgb_color_bounds) == 24);

typedef struct float_argb_color {
    float alpha;
    union {
        float_rgb_color rgb;
        struct {
            float red;
            float green;
            float blue;
        };
    };
} float_argb_color;
static_assert(sizeof(float_argb_color) == 16);

typedef struct float_argb_color_bounds {
    float_argb_color lower;
    float_argb_color upper;
} float_argb_color_bounds;
static_assert(sizeof(float_argb_color_bounds) == 32);

typedef struct float_vector2d {
    float x;
    float y;
} float_vector2d;
static_assert(sizeof(float_vector2d) == 8);

typedef struct float_vector3d {
    float x;
    float y;
    float z;
} float_vector3d;
static_assert(sizeof(float_vector3d) == 12);

typedef float_vector2d float_point2d;
typedef float_vector3d float_point3d;

// Replicates Halo's x87 ASM. round ties even
static inline signed long fast_ftol(float float_to_round) {
    long rounded_down = (long)float_to_round;

    long a = rounded_down;
    long b = rounded_down + 1;

    long low = float_to_round - ((float)a);
    long high = ((float)b) - float_to_round;

    if (low < high) {
        return a;
    }
    else {
        return b;
    }
}
