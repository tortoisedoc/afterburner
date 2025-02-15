#include "fire_engine.h"
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <math.h>

#include <opencv4/opencv2/opencv.hpp>
//#include <opencv2/core/mat.hpp>

#define WOBBLE_AMPLITUDE 10
#define RANDOM_IN_RANGE(_min,_max) \
    (rand() % (_max + 1 - _min) + _min)

#define INTENSITY_SCALED(_intensity,chValue) \
    ((float)_intensity / 255)*chValue)

#define MAX_ALPHA 255
#define MAX_SMOOTH_PIXELS 1000
#define SMOOTH_COLOR_MAX 64

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
unsigned char CLIP_ACCUMULATOR(short sumc, int countc, int intensity, int fadingc) {
    LOG(std::hex << sumc)
    //int angle = (int)RANDOM_IN_RANGE(0, 360);
    float sinValue = 0;
    if (sumc  != 0)
        sinValue = WOBBLE_AMPLITUDE*sin((128 - intensity)*M_PI/180);
    if (sinValue <0)
        sinValue = 0;
    sumc = (sumc / countc) + sinValue - fadingc;
    if (sumc < 0)
        sumc = 0;
    LOG("- " << std::hex <<sumc)
    return (unsigned char)(sumc & 0xFF);
}

void FireEngine::Burn(unsigned int * pixels, unsigned int * pixelsOut, int nrPasses, int intensity, int fading){
    if (!pixels || !pixelsOut) {
        LOG(" smooth : check parameters?");
        return;
    }

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

                unsigned int newValue = CLIP_ACCUMULATOR((short)sumr, count, intensity, fading)
                    | (CLIP_ACCUMULATOR ((short)sumg, count, intensity, fading) << 8)
                    | (CLIP_ACCUMULATOR ((short)sumb, count, intensity, fading) << 16);
                //Back to pixel it is!
                pixelsOut[x+y*m_width] = newValue | (pixels[x+y*m_width] & 0xFF000000);
            }
        }
    }
}

void FireEngine::Ignite(unsigned int * pixels, int verticalOffset,
    unsigned char chRed, unsigned char chGreen, unsigned char chBlue, unsigned char intensity) {
    //Generate random pixels at the bottom
    for (int y = verticalOffset; y < m_height; y++) {
        for (int x = 0; x < m_width; x ++) {
            pixels[x + y*m_width] = ((unsigned char) 0xFF) << 24
                | ((((unsigned char)RANDOM_IN_RANGE((unsigned char)(((float)intensity / 255)*chRed), chRed)) << 0))
                | ((((unsigned char)RANDOM_IN_RANGE((unsigned char)(((float)intensity / 255)*chGreen), chGreen)) << 8))
                | ((((unsigned char)RANDOM_IN_RANGE((unsigned char)(((float)intensity / 255)*chBlue), chBlue)) << 16));
        }
    }
}

void FireEngine::Scroll(unsigned int *pixels, int verticalOffset) {
    memcpy(pixels, pixels + verticalOffset*m_width, (m_height - verticalOffset)*m_width*4);
}

void FireEngine::PostProcess(unsigned int *pixels, unsigned int *newPixels) {

    cv::Mat map_x(m_height, m_width, CV_32FC1);
    cv::Mat map_y(map_x.size(), map_x.type());
    cv::Mat src (map_x.size(), CV_32FC1, pixels); //Not deallocated
    cv::Mat dst (src.size(), src.type());

    for( int y = 0; y < map_x.rows; y++ ) {
        for( int x = 0; x < map_x.cols; x++ ) {
            //Heads down for testing
            float val = (map_y.rows - y);
            map_x.at<float>(y, x) = (float)x;
            map_y.at<float>(y, x) =  (float)val;
//                printf("new val %d -> %f, cols %d rows %d\n", y, val, map_x.cols, map_x.rows);
        }
    }
    cv::remap(src, dst, map_x, map_y, cv::INTER_CUBIC, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0) );
    memcpy(newPixels, dst.data, m_width*m_height*4);
}

FireEngine::~FireEngine() {
}
