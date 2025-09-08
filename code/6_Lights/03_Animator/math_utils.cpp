#include "6_Lights/03_Animator/mAnimatorLight.h"
#include <cmath> // For math constants like M_PI, fabs, sqrt

int16_t sin16_t(uint16_t theta) {
  int scale = 1;
  if (theta > 0x7FFF) {
    theta = 0xFFFF - theta;
    scale = -1;
  }
  uint32_t precal = theta * (0x7FFF - theta);
  uint64_t numerator = (uint64_t)precal * (4 * 0x7FFF);
  int32_t denominator = 1342095361 - precal;
  int16_t result = numerator / denominator;
  return result * scale;
}

int16_t cos16_t(uint16_t theta) {
  return sin16_t(theta + 0x4000);
}

uint8_t sin8_t(uint8_t theta) {
  int32_t sin16 = sin16_t((uint16_t)theta * 257);
  sin16 += 0x7FFF + 128;
  return min(sin16, int32_t(0xFFFF)) >> 8;
}

uint8_t cos8_t(uint8_t theta) {
  return sin8_t(theta + 64);
}

float sin_approx(float theta) {
  uint16_t scaled_theta = (int)(theta * (float)(0xFFFF / M_TWOPI));
  int32_t result = sin16_t(scaled_theta);
  return float(result) / 0x7FFF;
}

float cos_approx(float theta) {
  uint16_t scaled_theta = (int)(theta * (float)(0xFFFF / M_TWOPI));
  int32_t result = sin16_t(scaled_theta + 0x4000);
  return float(result) / 0x7FFF;
}

float tan_approx(float x) {
  float c = cos_approx(x);
  return c == 0.0f ? 0 : sin_approx(x) / c;
}

#define ATAN2_CONST_A 0.1963f
#define ATAN2_CONST_B 0.9817f

float atan2_t(float y, float x) {
  float abs_y = fabs(y);
  float abs_x = fabs(x);
  float r = (abs_x - abs_y) / (abs_y + abs_x + 1e-10f);
  float angle = x < 0 ? M_PI_2 + M_PI_4 : M_PI_2 - M_PI_4;
  angle += (ATAN2_CONST_A * (r * r) - ATAN2_CONST_B) * r;
  return y < 0 ? -angle : angle;
}

float acos_t(float x) {
  float negate = float(x < 0);
  float xabs = fabs(x);
  float ret = -0.0187293f * xabs;
  ret = (ret + 0.0742610f) * xabs;
  ret = (ret - 0.2121144f) * xabs;
  ret = (ret + M_PI_2) * sqrt(1.0f - xabs);
  ret = ret - 2 * negate * ret;
  return negate * M_PI + ret;
}

float asin_t(float x) {
  return M_PI_2 - acos_t(x);
}

float atan_t(float x) {
  bool neg = (x < 0);
  x = fabs(x);
  float res;
  if (x > 5.0f) {
    res = M_PI_2 - (1.0f / x);
  } else if (x > 1.0f) {
    float xx = x * x;
    res = (0.05375f * xx * xx) + (-0.326175f * xx * x) + (0.974207f * xx) + (0.089494f * x);
  } else {
    float xx = x * x;
    res = ((0.0776509570923569 * xx + -0.287434475393028) * xx + (M_PI_4 - 0.0776509570923569 - -0.287434475393028)) * x;
  }
  return neg ? -res : res;
}

float floor_t(float x) {
  return (x < 0) ? int(x) - 1 : int(x);
}

float fmod_t(float num, float denom) {
  int tquot = num / denom;
  return num - tquot * denom;
}


// /*
//  * Fixed point integer based Perlin noise functions by @dedehai
//  * Note: optimized for speed and to mimic fastled inoise functions, not for accuracy or best randomness
//  */
// #define PERLIN_SHIFT 1

// // calculate gradient for corner from hash value
// static inline __attribute__((always_inline)) int32_t hashToGradient(uint32_t h) {
//   // using more steps yields more "detailed" perlin noise but looks less like the original fastled version (adjust PERLIN_SHIFT to compensate, also changes range and needs proper adustment)
//   // return (h & 0xFF) - 128; // use PERLIN_SHIFT 7
//   // return (h & 0x0F) - 8; // use PERLIN_SHIFT 3
//   // return (h & 0x07) - 4; // use PERLIN_SHIFT 2
//   return (h & 0x03) - 2; // use PERLIN_SHIFT 1 -> closest to original fastled version
// }

// // Gradient functions for 1D, 2D and 3D Perlin noise  note: forcing inline produces smaller code and makes it 3x faster!
// static inline __attribute__((always_inline)) int32_t gradient1D(uint32_t x0, int32_t dx) {
//   uint32_t h = x0 * 0x27D4EB2D;
//   h ^= h >> 15;
//   h *= 0x92C3412B;
//   h ^= h >> 13;
//   h ^= h >> 7;
//   return (hashToGradient(h) * dx) >> PERLIN_SHIFT;
// }

// static inline __attribute__((always_inline)) int32_t gradient2D(uint32_t x0, int32_t dx, uint32_t y0, int32_t dy) {
//   uint32_t h = (x0 * 0x27D4EB2D) ^ (y0 * 0xB5297A4D);
//   h ^= h >> 15;
//   h *= 0x92C3412B;
//   h ^= h >> 13;
//   return (hashToGradient(h) * dx + hashToGradient(h>>PERLIN_SHIFT) * dy) >> (1 + PERLIN_SHIFT);
// }

// static inline __attribute__((always_inline)) int32_t gradient3D(uint32_t x0, int32_t dx, uint32_t y0, int32_t dy, uint32_t z0, int32_t dz) {
//   // fast and good entropy hash from corner coordinates
//   uint32_t h = (x0 * 0x27D4EB2D) ^ (y0 * 0xB5297A4D) ^ (z0 * 0x1B56C4E9);
//   h ^= h >> 15;
//   h *= 0x92C3412B;
//   h ^= h >> 13;
//   return ((hashToGradient(h) * dx + hashToGradient(h>>(1+PERLIN_SHIFT)) * dy + hashToGradient(h>>(1 + 2*PERLIN_SHIFT)) * dz) * 85) >> (8 + PERLIN_SHIFT); // scale to 16bit, x*85 >> 8 = x/3
// }

// // fast cubic smoothstep: t*(3 - 2t²), optimized for fixed point, scaled to avoid overflows
// static uint32_t smoothstep(const uint32_t t) {
//   uint32_t t_squared = (t * t) >> 16;
//   uint32_t factor = (3 << 16) - ((t << 1));
//   return (t_squared * factor) >> 18; // scale to avoid overflows and give best resolution
// }

// // simple linear interpolation for fixed-point values, scaled for perlin noise use
// static inline int32_t lerpPerlin(int32_t a, int32_t b, int32_t t) {
//     return a + (((b - a) * t) >> 14); // match scaling with smoothstep to yield 16.16bit values
// }

// // 1D Perlin noise function that returns a value in range of -24691 to 24689
// int32_t perlin1D_raw(uint32_t x, bool is16bit = false) {
//   // integer and fractional part coordinates
//   int32_t x0 = x >> 16;
//   int32_t x1 = x0 + 1;
//   if(is16bit) x1 = x1 & 0xFF; // wrap back to zero at 0xFF instead of 0xFFFF

//   int32_t dx0 = x & 0xFFFF;
//   int32_t dx1 = dx0 - 0x10000;
//   // gradient values for the two corners
//   int32_t g0 = gradient1D(x0, dx0);
//   int32_t g1 = gradient1D(x1, dx1);
//   // interpolate and smooth function
//   int32_t tx = smoothstep(dx0);
//   int32_t noise = lerpPerlin(g0, g1, tx);
//   return noise;
// }

// // 2D Perlin noise function that returns a value in range of -20633 to 20629
// int32_t perlin2D_raw(uint32_t x, uint32_t y, bool is16bit = false) {
//   int32_t x0 = x >> 16;
//   int32_t y0 = y >> 16;
//   int32_t x1 = x0 + 1;
//   int32_t y1 = y0 + 1;

//   if(is16bit) {
//     x1 = x1 & 0xFF; // wrap back to zero at 0xFF instead of 0xFFFF
//     y1 = y1 & 0xFF;
//   }

//   int32_t dx0 = x & 0xFFFF;
//   int32_t dy0 = y & 0xFFFF;
//   int32_t dx1 = dx0 - 0x10000;
//   int32_t dy1 = dy0 - 0x10000;

//   int32_t g00 = gradient2D(x0, dx0, y0, dy0);
//   int32_t g10 = gradient2D(x1, dx1, y0, dy0);
//   int32_t g01 = gradient2D(x0, dx0, y1, dy1);
//   int32_t g11 = gradient2D(x1, dx1, y1, dy1);

//   uint32_t tx = smoothstep(dx0);
//   uint32_t ty = smoothstep(dy0);

//   int32_t nx0 = lerpPerlin(g00, g10, tx);
//   int32_t nx1 = lerpPerlin(g01, g11, tx);

//   int32_t noise = lerpPerlin(nx0, nx1, ty);
//   return noise;
// }

// // 3D Perlin noise function that returns a value in range of -16788 to 16381
// int32_t perlin3D_raw(uint32_t x, uint32_t y, uint32_t z, bool is16bit = false) {
//   int32_t x0 = x >> 16;
//   int32_t y0 = y >> 16;
//   int32_t z0 = z >> 16;
//   int32_t x1 = x0 + 1;
//   int32_t y1 = y0 + 1;
//   int32_t z1 = z0 + 1;

//   if(is16bit) {
//     x1 = x1 & 0xFF; // wrap back to zero at 0xFF instead of 0xFFFF
//     y1 = y1 & 0xFF;
//     z1 = z1 & 0xFF;
//   }

//   int32_t dx0 = x & 0xFFFF;
//   int32_t dy0 = y & 0xFFFF;
//   int32_t dz0 = z & 0xFFFF;
//   int32_t dx1 = dx0 - 0x10000;
//   int32_t dy1 = dy0 - 0x10000;
//   int32_t dz1 = dz0 - 0x10000;

//   int32_t g000 = gradient3D(x0, dx0, y0, dy0, z0, dz0);
//   int32_t g001 = gradient3D(x0, dx0, y0, dy0, z1, dz1);
//   int32_t g010 = gradient3D(x0, dx0, y1, dy1, z0, dz0);
//   int32_t g011 = gradient3D(x0, dx0, y1, dy1, z1, dz1);
//   int32_t g100 = gradient3D(x1, dx1, y0, dy0, z0, dz0);
//   int32_t g101 = gradient3D(x1, dx1, y0, dy0, z1, dz1);
//   int32_t g110 = gradient3D(x1, dx1, y1, dy1, z0, dz0);
//   int32_t g111 = gradient3D(x1, dx1, y1, dy1, z1, dz1);

//   uint32_t tx = smoothstep(dx0);
//   uint32_t ty = smoothstep(dy0);
//   uint32_t tz = smoothstep(dz0);

//   int32_t nx0 = lerpPerlin(g000, g100, tx);
//   int32_t nx1 = lerpPerlin(g010, g110, tx);
//   int32_t nx2 = lerpPerlin(g001, g101, tx);
//   int32_t nx3 = lerpPerlin(g011, g111, tx);
//   int32_t ny0 = lerpPerlin(nx0, nx1, ty);
//   int32_t ny1 = lerpPerlin(nx2, nx3, ty);

//   int32_t noise = lerpPerlin(ny0, ny1, tz);
//   return noise;
// }

// // scaling functions for fastled replacement
// uint16_t perlin16(uint32_t x) {
//   return ((perlin1D_raw(x) * 1159) >> 10) + 32803; //scale to 16bit and offset (fastled range: about 4838 to 60766)
// }

// uint16_t perlin16(uint32_t x, uint32_t y) {
//  return ((perlin2D_raw(x, y) * 1537) >> 10) + 32725; //scale to 16bit and offset (fastled range: about 1748 to 63697)
// }

// uint16_t perlin16(uint32_t x, uint32_t y, uint32_t z) {
//   return ((perlin3D_raw(x, y, z) * 1731) >> 10) + 33147; //scale to 16bit and offset (fastled range: about 4766 to 60840)
// }

// uint8_t perlin8(uint16_t x) {
//   return (((perlin1D_raw((uint32_t)x << 8, true) * 1353) >> 10) + 32769) >> 8; //scale to 16 bit, offset, then scale to 8bit
// }

// uint8_t perlin8(uint16_t x, uint16_t y) {
//   return (((perlin2D_raw((uint32_t)x << 8, (uint32_t)y << 8, true) * 1620) >> 10) + 32771) >> 8; //scale to 16 bit, offset, then scale to 8bit
// }

// uint8_t perlin8(uint16_t x, uint16_t y, uint16_t z) {
//   return (((perlin3D_raw((uint32_t)x << 8, (uint32_t)y << 8, (uint32_t)z << 8, true) * 2015) >> 10) + 33168) >> 8; //scale to 16 bit, offset, then scale to 8bit
// }