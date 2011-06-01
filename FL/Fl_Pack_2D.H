#ifndef _FL_PACK_2D_H_
#define _FL_PACK_2D_H_
#include <FL/Fl_Pack.H>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <string.h>
class Fl_Pack_2D : public Fl_Pack {
  int _nstrips;
public:
  Fl_Pack_2D(int x,int y,int w ,int h,const char *l = 0):Fl_Pack(x,y,w,h,l) {
      _nstrips=1;
  }

  void strips(int s) { _nstrips=(s<1)?1:s; }
  int strips(void) { return _nstrips; }
  void draw() {
  int tx = x()+Fl::box_dx(box());
  int ty = y()+Fl::box_dy(box());
  int tw = w()-Fl::box_dw(box());
  int th = h()-Fl::box_dh(box());
  int rw, rh;
  int current_position = horizontal() ? tx : ty;
  int maximum_position = current_position;

  uchar d = damage();
  Fl_Widget*const* a = array();

  if (horizontal()) {
    rw = -spacing();
    rh = (th-spacing()*children())/strips();

    for (int i = children(); i--;)
      if (child(i)->visible()) {
	if (child(i) != this->resizable()) rw += child(i)->w();
	rw += spacing();
      }
    if(label() ) { 
      if(label()[0]==' ') {
        current_position+=6;
        if(selection_color()!=labelcolor()) {
          fl_color(selection_color());
          fl_rectf(tx+2,ty+2,current_position-tx-2,th-4);
        }
      } else {
        current_position+=labelsize()*strlen(label())/2+4;
        if(selection_color()!=labelcolor()) {
          fl_color(selection_color());
          fl_rectf(tx,ty,current_position-tx,th);
        }
        draw_label();
      }
      current_position+=4;
    } 
  } else {
    rw = (tw-spacing()*children())/strips();
    rh = -spacing();

    for (int i = children(); i--;)
      if (child(i)->visible()) {
	if (child(i) != this->resizable()) rh += child(i)->h();
	rh += spacing();
      }
    if(label()) {
      current_position+=labelsize()+4;
      if(selection_color()!=labelcolor()) {
        fl_color(selection_color());
        fl_rectf(tx,ty,tw,current_position-ty);
      }
      draw_label();
      current_position+=2;
    }
  }
 
  for (int i = children(),j=0; i-- ;j++) {
    Fl_Widget* o = *a++;

    if (o->visible()) {
      int X,Y,W,H;
      if (horizontal()) {
        X = current_position;
        W = o->w();
      //depends of strip (if horizontal draw alternated vertical!).. 
        Y = ty+rh*(j%strips());
        H = rh;
      } else {
        X = tx+rw*(j%strips());
        W = rw;
        Y = current_position;
        H = o->h();
      }
      // last child, if resizable, takes all remaining room
      if(i==0 && o == this->resizable()) {
       if(horizontal())
         W = tw - rw;
       else
         H = th - rh;
      }
      if (spacing() && current_position>maximum_position &&
  	  (X != o->x() || Y != o->y() || d&FL_DAMAGE_ALL)) {
        fl_color(color());
        if (horizontal())
	  fl_rectf(maximum_position, ty, spacing(), th);
        else
	  fl_rectf(tx, maximum_position, tw, spacing());
      }
      if (X != o->x() || Y != o->y() || W != o->w() || H != o->h()) {
        o->resize(X,Y,W,H);
        o->clear_damage(FL_DAMAGE_ALL);
      }
      if (d&FL_DAMAGE_ALL) draw_child(*o); else update_child(*o);
      // child's draw() can change it's size, so use new size:
      current_position += ((j+1)%strips() && i) ? 0 : (horizontal() ? o->w() : o->h());
      if (current_position > maximum_position)
        maximum_position = current_position;
      current_position += spacing();
    }
  }

  if (horizontal()) {
    if (maximum_position < tx+tw) {
      fl_color(color());
      fl_rectf(maximum_position, ty, tx+tw-maximum_position, th);
    }
    tw = maximum_position-tx;
  } else {
    if (maximum_position < ty+th) {
      fl_color(color());
      fl_rectf(tx, maximum_position, tw, ty+th-maximum_position);
    }
    th = maximum_position-ty;
  }

  tw += Fl::box_dw(box()); if (tw <= 0) tw = 1;
  th += Fl::box_dh(box()); if (th <= 0) th = 1;
  if (tw != w() || th != h()) {
      Fl_Group::resize(x(),y(),tw,th); 
      d = FL_DAMAGE_ALL;
  }
  if (d&FL_DAMAGE_ALL) draw_box();
}

};
#endif
