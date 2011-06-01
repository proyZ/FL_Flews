#ifndef Fl_Multi_Value_Slider_H
#define Fl_Multi_Value_Slider_H

#include <FL/Fl_Slider.H>

typedef struct {    // segment values, normalized to 0-1
   double left;      // if any of this values is <0 then handle inexistant
   double right;
   double middle;
} _Fl_Value_Segment;
#define FL_VALUESEG_INVALID -1

class  Fl_Multi_Value_Slider : public Fl_Slider {
    _Fl_Value_Segment *segments;
    int nsegs;
    int currseg;
    char dragelem; //0 nop, 1 left, 2 rigth, 4 middle, 8 segment
    char mvals; //0 mvals off, 1 mvals on

    uchar textfont_, textsize_;
    unsigned textcolor_;
public:
    double normval(double v);
    int findseg(double v);
    int splitseg(int iseg=FL_VALUESEG_INVALID);
    int removeseg(int iseg=FL_VALUESEG_INVALID);
    int setsegments(int n, double *slist);
    double * getsegments(int &n);

    void draw_peg(int X,int Y,int W,int H,double val, int t=0);
    void draw_bg(int X, int Y, int W, int H);
    void draw();

    int handle(int event, int X, int Y, int W, int H);
    int handle(int);
    int middlevals(int m=-1);
    Fl_Multi_Value_Slider(int x,int y,int w,int h, const char *l = 0);

    Fl_Font textfont() const {return (Fl_Font)textfont_;}
    void textfont(uchar s) {textfont_ = s;}
    uchar textsize() const {return textsize_;}
    void textsize(uchar s) {textsize_ = s;}
    Fl_Color textcolor() const {return (Fl_Color)textcolor_;}
    void textcolor(unsigned s) {textcolor_ = s;}
 };


#endif
