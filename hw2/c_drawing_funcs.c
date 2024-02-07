// C implementations of drawing functions (and helper functions)

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "drawing_funcs.h"

////////////////////////////////////////////////////////////////////////
// Helper functions
////////////////////////////////////////////////////////////////////////

// TODO: implement helper functions
int32_t in_bounds(struct Image *img, int32_t x, int32_t y){
  return clamp(x, 0, img->width) && (clamp(y, 0, img->height));
}
uint32_t compute_index(struct Image *img, int32_t x, int32_t y){
  return x * (img->width) + y;
}
int32_t clamp(int32_t val, int32_t min, int32_t max){
  return min < val && val < max;
}
uint8_t get_r(uint32_t color) {
  return color >> 24;
}
uint8_t get_g(uint32_t color) {
  color = color << 8;
  return color >> 24;
}
uint8_t get_b(uint32_t color) {
  color = color << 16;
  return color >> 24;
}
uint8_t get_a(uint32_t color) {
  color = color << 24;
  return color >> 24;
}
uint8_t blend_components(uint32_t fg, uint32_t bg, uint32_t alpha){
  return (alpha * fg + (255 - alpha) * bg) / 255;
}
uint32_t blend_colors(uint32_t fg, uint32_t bg){
  uint32_t alpha = get_a(fg);
  uint32_t red = blend_components(get_r(fg), get_r(bg), alpha);
  uint32_t green = blend_components(get_g(fg), get_g(bg), alpha);
  uint32_t blue = blend_components(get_b(fg), get_b(bg), alpha);
  return red << 24 + green << 16 + blue << 8 + 255;
}
void set_pixel(struct Image *img, uint32_t index, uint32_t color) {
  img->data[index] = color;
}
int64_t square(int64_t x) {
  return x*x;
}
int64_t square_dist(int64_t x1, int64_t y1, int64_t x2, int64_t y2) {
  return square(x1-x2) + square (y1-y2);
}

////////////////////////////////////////////////////////////////////////
// API functions
////////////////////////////////////////////////////////////////////////

//
// Draw a pixel.
//
// Parameters:
//   img   - pointer to struct Image
//   x     - x coordinate (pixel column)
//   y     - y coordinate (pixel row)
//   color - uint32_t color value
//
void draw_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color) {
  // TODO: implement
  uint32_t index = img->width*x +y;
  set_pixel(img, index, color);
}

//
// Draw a rectangle.
// The rectangle has rect->x,rect->y as its upper left corner,
// is rect->width pixels wide, and rect->height pixels high.
//
// Parameters:
//   img     - pointer to struct Image
//   rect    - pointer to struct Rect
//   color   - uint32_t color value
//
void draw_rect(struct Image *img,
               const struct Rect *rect,
               uint32_t color) {
  // TODO: implement
  uint32_t initial = rect->y*img->width + rect->x; 
  for (int a = 0; a < rect->height; a++) {
    for (int b = 0; b <rect->width; b++) {
      set_pixel(img, initial+b, color);
    }
    initial += img->width;
  }
}

//
// Draw a circle.
// The circle has x,y as its center and has r as its radius.
//
// Parameters:
//   img     - pointer to struct Image
//   x       - x coordinate of circle's center
//   y       - y coordinate of circle's center
//   r       - radius of circle
//   color   - uint32_t color value
//
void draw_circle(struct Image *img,
                 int32_t x, int32_t y, int32_t r,
                 uint32_t color) {
  // TODO: implement
  for (int a = 0; a < img->height; a++) {
    for (int b = 0; b < img->width; b++) {
      if (square_dist(x, y, a, b)) {
        draw_pixel(img, a, b, color);
      }
    }
  }
}

//
// Draw a tile by copying all pixels in the region
// enclosed by the tile parameter in the tilemap image
// to the specified x/y coordinates of the destination image.
// No blending of the tile pixel colors with the background
// colors should be done.
//
// Parameters:
//   img     - pointer to Image (dest image)
//   x       - x coordinate of location where tile should be copied
//   y       - y coordinate of location where tile should be copied
//   tilemap - pointer to Image (the tilemap)
//   tile    - pointer to Rect (the tile)
//
void draw_tile(struct Image *img,
               int32_t x, int32_t y,
               struct Image *tilemap,
               const struct Rect *tile) {
 // TODO: implement
}

//
// Draw a sprite by copying all pixels in the region
// enclosed by the sprite parameter in the spritemap image
// to the specified x/y coordinates of the destination image.
// The alpha values of the sprite pixels should be used to
// blend the sprite pixel colors with the background
// pixel colors.
//
// Parameters:
//   img       - pointer to Image (dest image)
//   x         - x coordinate of location where sprite should be copied
//   y         - y coordinate of location where sprite should be copied
//   spritemap - pointer to Image (the spritemap)
//   sprite    - pointer to Rect (the sprite)
//
void draw_sprite(struct Image *img,
                 int32_t x, int32_t y,
                 struct Image *spritemap,
                 const struct Rect *sprite) {
  // TODO: implement
}
