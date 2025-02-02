#include "fire_engine.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <math.h>

#define RANDOM_IN_RANGE(_min,_max) \
    (rand() % (_max + 1 - _min) + _min)

#define RANDOM_RED_YELLOW_PIXEL(__min,__max) \
0xFF << 24 | ((((unsigned char)RANDOM_IN_RANGE(__min,__max)) << 0))
//  0xFF << 24 | ((((unsigned char)(rand() % 256 + 1)) << 16) | (((unsigned char)(rand() % 256 + 1)) << 8))

#define MAX_ALPHA 255
#define MAX_SMOOTH_PIXELS 1000
#define SMOOTH_COLOR_MAX 64

//Add logging flag to cmake for logging
#ifdef LOGGING
#define LOG(cmd)\
  std::cout << cmd
#else
#define LOG(cmd)
#endif

//Accumulates from 8-bit to 16-bits; can overflow as it should
#define ACCUMULATE_RGB(sumrm,sumgm,sumbm,pixel)\
  sumrm+=pixel & 0xFF;\
  sumgm+=(pixel >> 8) & 0xFF;\
  sumbm+=(pixel >> 16) & 0xFF;

// Ancient knowledge from the dawn of time lights the way
// Fire logic with added cooling map!
// https://web.archive.org/web/20160418004150/http://freespace.virgin.net/hugo.elias/models/m_fire.htm
// Warp map to make it wobble!
// https://web.archive.org/web/20160418004149/http://freespace.virgin.net/hugo.elias/graphics/x_warp.htm

FireEngine::FireEngine(int width, int height) {
  m_width = width;
  m_height = height;
}


//Clips a channel accumulator by calculating the average (out of the collected values) and applying an offset (fading)
unsigned char CLIP_ACCUMULATOR(short sumc, int countc, int fadingc) {
  LOG(std::hex << sumc)
  sumc = (sumc / countc) - fadingc;
  if (sumc < 0)
    sumc = 0;
  LOG("- " << std::hex <<sumc)
  return (unsigned char)(sumc & 0xFF);
}

void FireEngine::Burn(unsigned int * pixels, unsigned int * pixelsOut, int nrPasses, int fading){
  if (!pixels || !pixelsOut) {
    LOG(" smooth : check parameters?");
    return;
  }

  int sum = 0;
  int count = 0;

  for (int pass = 0; pass < nrPasses; pass++) {
    // If we proceed from bottom to top, we wont get the right intensity -
    //   we have have to go by line!
    for (int y = m_height - 1; y > 0; --y) {
      LOG("y :" << y);
      for (int x = 0; x < m_width; x ++) {
        unsigned short sumr = 0;
        unsigned short sumg = 0;
        unsigned short sumb = 0;
        unsigned short count = 0;

        if (x > 0) {
          ACCUMULATE_RGB(sumr, sumg, sumb, pixels[(x-1)+y*m_width])
          count ++;
          LOG(std::hex << sumr << "/" << sumg << "/" <<sumb << "\n");
        }

        if (x < m_width-1) {
          ACCUMULATE_RGB(sumr, sumg, sumb, pixels[(x+1)+y*m_width])
          count ++;
        }
        LOG(std::hex << sumr << "/" << sumg << "/" <<sumb << "\n");

/*
        We assume there's void only above the fire
        if (y > 0) {
          ACCUMULATE_RGB(sumr, sumg, sumb, pixels[x+(y-1)*m_width])
          count ++;
        }
        std::cout << std::hex << sumr << "/" << sumg << "/" <<sumb << "\n";
*/
        if (y < m_height - 1) {
          ACCUMULATE_RGB(sumr, sumg, sumb, pixels[x+(y+1)*m_width])
          count ++;
        }
        LOG(std::hex << sumr << "/" << sumg << "/" <<sumb << "\n");

        unsigned int newValue = CLIP_ACCUMULATOR((short)sumr, count, fading)
          | (CLIP_ACCUMULATOR ((short)sumg, count, fading) << 8)
          | (CLIP_ACCUMULATOR ((short)sumb, count, fading) << 16);

        //Back to pixel it is!
        pixelsOut[x+y*m_width] = newValue | 0xFF000000;
      }
    }
  }
}

void FireEngine::Ignite(unsigned int * pixels, int verticalOffset) {
  //Generate random pixels at the bottom
  for (int y = verticalOffset; y < m_height; y++) {
    for (int x = 0; x < m_width; x ++) {
      pixels[x + y*m_width] = RANDOM_RED_YELLOW_PIXEL(100, 256);
    }
  }
}

void FireEngine::Scroll(unsigned int *pixels, int verticalOffset) {
  memcpy(pixels, pixels + verticalOffset*m_width, (m_height - verticalOffset)*m_width*4);
}

FireEngine::~FireEngine() {
}
