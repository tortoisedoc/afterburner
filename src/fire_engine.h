#pragma once

//Add logging flag to cmake for logging
#ifdef LOGGING
#define LOG(cmd)\
    std::cout << cmd
#else
#define LOG(cmd)
#endif

class FireEngine {
private:
  int m_width;
  int m_height;
public:
  //Public for easyness of demo
  /**
    initialize with a given (width, height) pair for conveniency
  */
  FireEngine(int width, int height);
  ~FireEngine();
  /**
    feeds the fire with new combustion at the bottom
  */
  void Ignite(unsigned int *pixels, int verticalOffset,
      unsigned char chRed, unsigned char chGreen, unsigned char chBlue, unsigned char intensity);
  /**
    makes the fire "burn"
  */
  void Burn(unsigned int *pixels, unsigned int *pixelsOut, int nrPasses, int fading);
  /**
    postprocess the fire
  */
  void PostProcess(unsigned int *pixels, unsigned int *pixel);
  /**
    grows the flames
  */
  void Scroll(unsigned int *pixels, int verticalOffset);
};
