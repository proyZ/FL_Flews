#ifndef __Fl_Spin__
#define __Fl_Spin__
#ifndef FLTK_1_0_COMPAT
#define FLTK_1_0_COMPAT
#endif

#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Valuator.H>
#include <math.h>

class Fl_Spin : public Fl_Valuator {
private:
  int ix, iy, drag, indrag;
  int delta,deltadir;
  char soft_;
  uchar mouseobj;
  static FL_EXPORT void repeat_callback(void *);
  FL_EXPORT void increment_cb(); 

#if FL_MAJOR_VERSION==1 && FL_MINOR_VERSION==3
// draw is now private on fltk 1.3
protected:
#else
// draw is public on 1.1.x
public:
#endif
   FL_EXPORT void draw();
  
public:
    FL_EXPORT int handle(int);
    FL_EXPORT Fl_Spin(int x,int y,int w,int h, const char *l = 0);

  void soft(char x) {soft_ = x;}
  char soft() const {return soft_;}

  ~Fl_Spin();
};

#endif
