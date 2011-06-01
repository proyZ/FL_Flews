#ifndef __Fl_Value_Input_Spin__
#define __Fl_Value_Input_Spin__
#ifndef FLTK_1_0_COMPAT
#define FLTK_1_0_COMPAT
#endif

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Valuator.H>
#include <math.h>

class Fl_Value_Input_Spin : public Fl_Valuator {
private:
  int ix, iy, drag, indrag,sldrag;
  int delta,deltadir;
  char soft_;
  uchar mouseobj;
  int butsize;
  static FL_EXPORT void input_cb(Fl_Widget*,void*);
  virtual FL_EXPORT void value_damage(); // cause damage() due to value() changing
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
    // FUGLY HACK: since fltk 1.3 made the draw method private on now this ugly hack
    // is needed to acces the input widget draw method, this is temporary until
    // I get time to rewrite these composite widgets using an Fl_Group 
    // (but then they wouldn't show nicely on fluid :-(
    class Fl_Value_Slider_Input_Input: public Fl_Input
    {
      public:
         Fl_Value_Slider_Input_Input(int x,int y,int w,int h,const char *l = 0):
	         Fl_Input(x,y,w,h,l) {}
	
         void doDraw() { draw(); }
    };

    Fl_Value_Slider_Input_Input input;
    

    FL_EXPORT int handle(int);
    FL_EXPORT void resize(int,int,int,int);
    FL_EXPORT Fl_Value_Input_Spin(int x,int y,int w,int h, const char *l = 0);

  void soft(char x) {soft_ = x;}
  char soft() const {return soft_;}

  Fl_Font textfont() const {return input.textfont();}
  void textfont(uchar s) { input.textfont(s);}
  uchar textsize() const {return input.textsize();}
  void textsize(uchar s) {input.textsize(s);}
  Fl_Color textcolor() const {return input.textcolor();}
  void textcolor(uchar n) {input.textcolor(n);}
  Fl_Color cursor_color() const {return input.cursor_color();}
  void cursor_color(uchar n) {input.cursor_color(n);}
  int buttonssize() const {return butsize;}
  void buttonssize(int s) { butsize=s;}
  ~Fl_Value_Input_Spin();
};

#endif
