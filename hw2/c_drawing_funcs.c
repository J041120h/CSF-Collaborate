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
int32_t in_bounds(struct Image *img, int32_t x, int32_t y){ //check if x, y is in the bounds
  return clamp(x, 0, img->width) && (clamp(y, 0, img->height)); //call clamp
}
uint32_t compute_index(struct Image *img, int32_t x, int32_t y){ 
  return y * (img->width) + x;
}
int32_t clamp(int32_t val, int32_t min, int32_t max){
  return min <= val && val < max; // needs to be <= because lower bound can be 0
}
uint8_t get_r(uint32_t color) {
  return color >> 24; //right shift 24
}
uint8_t get_g(uint32_t color) {
  color = color << 8;  //remove first 8
  return color >> 24;
}
uint8_t get_b(uint32_t color) {
  color = color << 16;  //remove first 16
  return color >> 24;
}
uint8_t get_a(uint32_t color) {
  color = color << 24; //remove first 24
  return color >> 24;
}
uint8_t blend_components(uint32_t fg, uint32_t bg, uint32_t alpha){
  return (alpha * fg + (255 - alpha) * bg) / 255; //using the formula
}
uint32_t blend_colors(uint32_t fg, uint32_t bg){
  uint32_t alpha = get_a(fg);
  uint32_t red = blend_components(get_r(fg), get_r(bg), alpha); //use uint32_t here to accomodate left-shift of bit
  uint32_t green = blend_components(get_g(fg), get_g(bg), alpha);
  uint32_t blue = blend_components(get_b(fg), get_b(bg), alpha);
  return (red << 24) + (green << 16) + (blue << 8) + 255;  //formulate the result
}
void set_pixel(struct Image *img, uint32_t index, uint32_t color) {
  img->data[index] = blend_colors(color, img->data[index]);  //set pixel automatically blend the color (in most circumstance blend is needed)
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
void draw_pixel(struct Image *img, int32_t x, int32_t y, uint32_t color) { //encapsulate set_pixel
  // TODO: implement 
  if (in_bounds(img, x, y)) {
    uint32_t index = compute_index(img, x, y);
    set_pixel(img, index, color);
  }
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
  for (int a = 0; a < rect->height; a++) {
    for (int b = 0; b <rect->width; b++) {
      draw_pixel(img, rect->x + b, rect->y + a, color); //get appropriate index
    }
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
  for (int a = 0; a < img->width; a++) {
    for (int b = 0; b < img->height; b++) {  //traverse all the points, for the points within the circle, print the color
      if (square_dist(x, y, a, b) <= square(r)) {
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
  for(int i = 0; i < tile->width; i++){
    for(int j = 0; j < tile->height; j++){
      if(!in_bounds(tilemap, tile->x + i, tile->y + j) || !in_bounds(img, x + i, y + j)){ //check if the tile is in range of both img and tilemap
        continue;
      }
      uint32_t copy_index = compute_index(tilemap, tile->x + i, tile->y + j); //get corresponding index
      uint32_t index = compute_index(img, x + i, y + j);
      img->data[index] = tilemap->data[copy_index]; //directly replicate the color
    }
 }
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
  for(int i = 0; i < sprite->width; i++){
    for(int j = 0; j < sprite->height; j++){
      if(!in_bounds(spritemap, sprite->x + i, sprite->y + j) || !in_bounds(img, x + i, y + j)){ //check if the sprite is in the range of both img and spritemap
        continue;
      }
      uint32_t copy_index = compute_index(spritemap, sprite->x + i, sprite->y + j); //compute the index in spritemap
      draw_pixel(img, x + i, y + j, spritemap->data[copy_index]);
    }
 }
}
