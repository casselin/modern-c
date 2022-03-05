/* Challenge 11 Image segmentation
 * 
 * In addition to the C standard library, there are many other support
 * libraries out there that provide very different features. Among those
 * are a lot that do image processing of some kind. Try to find a suitable
 * such image-processing library that is written in or interfaced to C and
 * that allows you to treat grayscale images as two-dimensional matrices
 * of base type unsigned char.
 *
 * The goal of this challenge is to perform a segmentation of such an
 * image: to group the pixels (the unsigned char elements of the matrix)
 * into connected regions that are ``similar'' in some sense or another.
 * Such a segmentation forms a partition of the set of pixels, much as we
 * saw in challenge 4. Therefore, you should use a Union-Find structure
 * to represent regions, one per pixel at the start.
 *
 * Can you implement a statistics function that computes a statistic for
 * all regions? This should be another array (the third array in the game)
 * that for each root holds the number of pixels and the sum of all values.
 *
 * Can you implement a merge criterion for regions? Test whether the mean
 * values of two regions are not too far apart: say no more than five gray
 * values.
 *
 * Can you implement a line-by-line merge strategy that, for each pixel on
 * a line of the image, tests whether its region should be merged to the
 * left and/or to the top?
 *
 * Can you iterate line by line until there are no more changes: that is,
 * such that the resulting regions/sets all test negatively with their
 * respective neighboring regions?
 *
 * Now that you have a complete function for image segmentation, try it
 * on images with assorted subjects and sizes, and also vary your merge
 * criterion with different values for the mean distance instead of five.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <tgmath.h>
#include <stdbool.h>
#include <assert.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define TEST_W 3
#define TEST_H 2

typedef struct stats stats;
struct stats {
  double sum;
  unsigned count;
};

size_t Find(size_t table[static 1], size_t i) {
  if (table[i] == SIZE_MAX) {
    return i;
  }

  return Find(table, table[i]);
}

void FindReplace(size_t table[static 1], size_t i, size_t x) {
  if (table[i] == SIZE_MAX) {
    table[i] = x;
  	return;
  }

  FindReplace(table, table[i], x);

  table[i] = x;

  if (table[i] == i) {
    table[i] = SIZE_MAX;
  }
}

void FindCompress(size_t table[static 1], size_t i) {
  size_t root = Find(table, i);

  while (i != root) {
    size_t temp = table[i];
  	table[i] = root;
  	i = temp;
  }
}

void Union(size_t table[static 1], size_t a, size_t b) {
  FindCompress(table, a);

  if (Find(table, a) == Find(table, b)) {
    return;
  }
  FindReplace(table, b, a);
}

void initialize(size_t table[static 1], size_t size) {
  for (size_t i = 0; i < size; ++i) {
    table[i] = SIZE_MAX;
  }
}

void init_stats(int width, int height,
    unsigned char pixels[height * width],
    stats s[height * width]) {
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int idx = i * width + j;
      s[idx].sum = pixels[idx];
      s[idx].count = 1;
    }
  }
}

bool check_criterion(int width, int height, size_t a, size_t b,
    size_t table[height * width],
    stats s[height * width]) {
  size_t i = Find(table, a);
  size_t j = Find(table, b);
  if (i == j) return false;
  return fabs(s[i].sum / s[i].count - s[j].sum / s[j].count) <= 5.0;
}

void merge(int width, int height, size_t a, size_t b,
    size_t table[height * width],
    stats s[height * width]) {
  size_t i = Find(table, a);
  size_t j = Find(table, b);
  if (i == j) return;
  s[i].sum += s[j].sum;
  s[i].count += s[j].count;
  Union(table, a, b);
}

bool line_seg(int width, int height, int i,
    size_t table[height * width],
    stats s[height * width]) {
  bool changed = false;
  for (unsigned j = 0; j < width; ++j) {
    bool west = false;
    if (j > 0) {
      west = check_criterion(width, height,
          i*width + j, i*width + (j-1), table, s);
    }
    bool north = false;
    if (i > 0) {
      north = check_criterion(width, height,
          i*width + j, (i-1)*width + j, table, s);
    }
    if (west)
      merge(width, height, i*width + j, i*width + (j-1), table, s);
    if (north)
      merge(width, height, i*width + j, (i-1)*width + j, table, s);
    changed = changed || west || north;
  }
  return changed;
}

void segment_image(int width, int height,
    size_t table[height * width],
    stats s[height * width]) {
  bool changed = false;
  do {
    changed = false;
    for (unsigned i = 0; i < height; ++i) {
      bool line_change = false;
      line_change = line_seg(width, height, i, table, s);
      changed = changed || line_change;
    }
  } while (changed);
}

void synthesize_image(int width, int height,
  size_t table[height * width],
  stats s[height * width],
  unsigned char out[height * width]) {
    for (int i = 0; i < height; ++i) {
      for (int j = 0; j < width; ++j) {
        int idx = i * width + j;
        size_t root = Find(table, idx);
        double mean = floor(s[root].sum / s[root].count);
        assert(mean <= 255.0);
        assert(mean >= 0.0);
        unsigned char pixel = (unsigned char) mean;
        out[idx] = pixel;
      }
    }
}

int main(int argc, char* argv[argc+1]) {

  if (argc < 2) {
    printf("Provide the name of the image to be segmented\n");
    return EXIT_SUCCESS;
  }

  int x, y, n;
  unsigned char *data = stbi_load(argv[1], &x, &y, &n, 1);
  if (!data) {
    return EXIT_FAILURE;
  }

  size_t seg_table[y * x];
  stats *s = malloc((sizeof(stats)) * y * x);

  if (!s) {
    return EXIT_FAILURE;
  }

  initialize(seg_table, y * x);
  init_stats(x, y, data, s);
  segment_image(x, y, seg_table, s);

  unsigned char output[y * x];
  
  synthesize_image(x, y, seg_table, s, output);

  stbi_write_png("output.png", x, y, 1, output, x * 1);

  stbi_image_free(data);
  free(s);

  return EXIT_SUCCESS;
}
