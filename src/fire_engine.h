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
     makes the fire "burn"
  */
  void Burn(unsigned int *pixels, unsigned int *pixelsOut, int nrPasses, int fading);
  /**
    grows the flames
  */
  void Scroll(unsigned int *pixels, int verticalOffset);
  /**
    feeds the fire with new combustion at the bottom
  */
  void Ignite(unsigned int *pixels, int verticalOffset);
};
