#ifndef FLTK_1_0_COMPAT
#define FLTK_1_0_COMPAT
#endif
#include <FL/Fl.H>
#include <FL/Fl_Multi_Value_Slider.H>
#include <FL/fl_draw.H>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double Fl_Multi_Value_Slider::normval(double v)
{
  double val;
  if (minimum() == maximum())
    val = 0.5;
  else {
    val = (v-minimum())/(maximum()-minimum());
    if (val > 1.0) val = 1.0;
    else if (val < 0.0) val = 0.0;
  }
  return val;
}

int Fl_Multi_Value_Slider::findseg(double v)
{
  int iseg;
  double val;
  if(segments) {
    val=normval(v);
    for(iseg=0; iseg<nsegs && 
               !(val>=segments[iseg].left && val<=segments[iseg].right) ; 
        iseg++);
    if(iseg>=nsegs ) {
       iseg=nsegs-1;
    } 
    return iseg;
  } 
  return FL_VALUESEG_INVALID;
}

int Fl_Multi_Value_Slider::splitseg(int iseg)
{
 int i;
 double mid;
 if(FL_VALUESEG_INVALID==iseg)
   iseg=currseg;

 if(iseg>=nsegs || iseg<0 )
   return FL_VALUESEG_INVALID;

 nsegs++;
 segments=(_Fl_Value_Segment*)realloc(segments,sizeof(_Fl_Value_Segment)*nsegs);
 for(i=nsegs-1;i>iseg; i--) {
   segments[i].left=segments[i-1].left;
   segments[i].right=segments[i-1].right;
   segments[i].middle=segments[i-1].middle;
  }

 if(segments[iseg].middle<0) {
   mid=(segments[iseg].left+segments[iseg].right)*0.5;
 } else {
   mid=segments[iseg].middle;
 }
 segments[iseg+1].left=mid;
 segments[iseg].right=mid;

 if(segments[iseg].middle>=0) {
  segments[iseg].middle=(segments[iseg].left+segments[iseg].right)*0.5;
  segments[iseg+1].middle=(segments[iseg+1].left+segments[iseg+1].right)*0.5;
 } 

 return iseg;
}

int Fl_Multi_Value_Slider::removeseg(int iseg)
{
 int i;
 double mid;
 if(FL_VALUESEG_INVALID==iseg)
   iseg=currseg;

 if(iseg>=nsegs || iseg<0 )
   return FL_VALUESEG_INVALID;

 if(nsegs<=1)
   return 1;

 if(iseg >0 && iseg<nsegs-1)
   mid=(segments[iseg].left+segments[iseg].right)*0.5;
 else if(iseg<=0)
   mid=0.0;
 else 
   mid=1.0;

 if(iseg>0) 
  segments[iseg-1].right=mid;

 if(iseg<nsegs-1)  
  segments[iseg+1].left=mid;

 nsegs--;
 for(i=iseg; i<nsegs; i++) {
   segments[i].left=segments[i+1].left;
   segments[i].right=segments[i+1].right;
   segments[i].middle=segments[i+1].middle;
 }
 segments=(_Fl_Value_Segment*)realloc(segments,sizeof(_Fl_Value_Segment)*nsegs);
 if(iseg>=nsegs) {
   iseg=nsegs-1;
 }

 if(currseg>=nsegs) {
  currseg=nsegs-1;
 }

 return iseg;
}

int Fl_Multi_Value_Slider::setsegments(int n, double *slist)
{
 int i;

 if(n<=0 || slist==NULL)
   return FL_VALUESEG_INVALID;

 nsegs=n;
 segments=(_Fl_Value_Segment*)realloc(segments,sizeof(_Fl_Value_Segment)*nsegs);
 for(i=0; i<n; i++) {
    segments[i].left=slist[i*3];
    segments[i].middle=slist[i*3+1];
    segments[i].right=slist[i*3+2];
 }
 if(currseg>=nsegs)
   currseg=nsegs-1;
 return nsegs;
}

double * Fl_Multi_Value_Slider::getsegments(int &n)
{
 int i;
 double *slist;

 n=nsegs;
 if(nsegs<=0); 
   return NULL;
 slist=(double *)malloc(sizeof(double)*nsegs);
 for(i=0; i<n; i++) {
    slist[i*3]=segments[i].left;
    slist[i*3+1]=segments[i].middle;
    slist[i*3+2]=segments[i].right;
 }
 return slist;
}

int Fl_Multi_Value_Slider::middlevals(int m)
{
 if(m!=-1) 
   mvals=(m)?1:0;
 return mvals;
}

Fl_Multi_Value_Slider::Fl_Multi_Value_Slider(int X, int Y, int W, int H, const char*l)
: Fl_Slider(X,Y,W,H,l) {
  step(0);

  textfont_ = FL_HELVETICA;
  textsize_ = 10;
  textcolor_ = FL_BLACK;

  slider_size(0);
  currseg=0;
  segments=(_Fl_Value_Segment*)malloc(sizeof(_Fl_Value_Segment));
  nsegs=1;
  mvals=1;
  segments[0].left=0.0;
  segments[0].middle=0.5;
  segments[0].right=1.0;
}

void Fl_Multi_Value_Slider::draw_bg(int X, int Y, int W, int H) {
  if (!(damage()&FL_DAMAGE_ALL)) { // not a complete redraw
    fl_push_clip(X, Y, W, H);
    draw_box();
    fl_pop_clip();
  } else {
    draw_box();
  }
  if(currseg>=0) {
    int ww; 
    ww = (horizontal() ? W : H);
    if(horizontal())
     draw_box(FL_FLAT_BOX,X+int(segments[currseg].left*ww)
                      ,Y
                      ,int(ww*(segments[currseg].right-segments[currseg].left))
                      ,H
,(textcolor_==FL_BLACK)?fl_lighter(color()):textcolor());
    else 
     draw_box(FL_FLAT_BOX,X
                      ,Y+int(segments[currseg].left*ww)
                      ,W
                      ,int(ww*(segments[currseg].right-segments[currseg].left))
,(textcolor_==FL_BLACK)?fl_lighter(color()):textcolor());
  }
  Fl_Color black = active_r() ? FL_FOREGROUND_COLOR : FL_INACTIVE_COLOR;
  if (type() == FL_VERT_NICE_SLIDER) {
    draw_box(FL_THIN_DOWN_BOX, X+W/2-2, Y, 2, H, black);
  } else if (type() == FL_HOR_NICE_SLIDER) {
    draw_box(FL_THIN_DOWN_BOX, X, Y+H/2-2, W, 2, black);
  }

}

void Fl_Multi_Value_Slider::draw_peg(int X,int Y,int W,int H,double val, int t)
{
 int xx, S;
 int xsl, ysl, wsl, hsl;
 int ww = (horizontal() ? W : H);
 
 S=(maximum()-minimum()<=1.0)?4:int(ww/(maximum()-minimum()));
 S=(2>=S)?4:S; 
 if(t) S/=2;
 S=(2>=S)?3:S; 
 
 xx = int(val*ww+.5);
 xx =(xx>=ww)?ww-1:xx;
 if (active_r()) {
     fl_color(selection_color());  
 } else {
     fl_color(selection_color() | 8);
 }    
 fl_push_clip(X, Y, W, H);
 if (horizontal()) {
    xsl = X+xx;
    wsl = S;
    ysl = Y;
    hsl = H;
    if(t)
      fl_loop(xsl,ysl, xsl+wsl, ysl+hsl/2 , xsl,ysl+hsl, xsl-wsl, ysl+hsl/2 );
    else if(!slider_size()) 
      fl_polygon(xsl,ysl, xsl+wsl, ysl+hsl, xsl-wsl, ysl+hsl);
    else 
      fl_polygon(xsl,ysl+hsl, xsl+wsl, ysl, xsl-wsl, ysl);
 } else {
    ysl = Y+xx;
    hsl = S;
    xsl = X;
    wsl = W;
    if(t)
     fl_loop(xsl,ysl, xsl+wsl/2, ysl-hsl,xsl+wsl, ysl , xsl+wsl/2, ysl+hsl);
    else if(!slider_size()) 
     fl_polygon(xsl,ysl, xsl+wsl, ysl-hsl, xsl+wsl, ysl+hsl);
    else 
     fl_polygon(xsl+wsl,ysl, xsl, ysl-hsl, xsl, ysl+hsl);
 }
 fl_pop_clip();
}

void Fl_Multi_Value_Slider::draw() 
{
 int sxx = x(), syy = y(), sww = w(), shh = h();
 int X,Y,W,H,iseg; 
 int ww; 
 X=sxx+Fl::box_dx(box());
 Y=syy+Fl::box_dy(box());
 W=sww-Fl::box_dw(box());
 H=shh-Fl::box_dh(box());
 ww = (horizontal() ? W : H);
 if (damage()&FL_DAMAGE_ALL) draw_box(box(),sxx,syy,sww,shh,color());
 draw_bg(X,Y,W,H);

 for(iseg=0;iseg<nsegs;iseg++){
   draw_peg( X,Y,W, H, segments[iseg].left);
   if(mvals && segments[iseg].middle>=0)
     draw_peg( X,Y,W, H, segments[iseg].middle,1);
 }
 draw_peg( X,Y,W, H, segments[iseg-1].right);
}

#define _M_ABS(x) (((x)>0)?(x):-(x))
int Fl_Multi_Value_Slider::handle(int event, int X, int Y, int W, int H)
{
 int mx,ww,iseg,l,r,m,S;
 int slider_resp; 
 double oval=value();
 double val,nval;
 X+=Fl::box_dx(box());
 Y+=Fl::box_dy(box());
 W-=Fl::box_dw(box());
 H-=Fl::box_dh(box());
 ww = (horizontal() ? W : H);
 mx = (horizontal() ? Fl::event_x()-X : Fl::event_y()-Y);
 nval =((double(mx)/double(ww))*double(maximum()-minimum()));
 S=(maximum()-minimum()<=1.0)?4:int(ww/(maximum()-minimum()));
 S=(2>=S)?4:S; 

 slider_resp=1;

 switch(event) {
   case FL_PUSH: 
     if (!Fl::event_inside(X, Y, W, H)) return 0;
     if (Fl::visible_focus()) Fl::focus(this);
     value(clamp(nval));
     iseg=findseg(nval);
     dragelem=8;
     l=int(segments[iseg].left*ww);
     r=int(segments[iseg].right*ww);
     m=int(segments[iseg].middle*ww);
     if(_M_ABS(mx-l)<=S) {
       dragelem=1;
       if(iseg==currseg+1) {
         iseg=currseg;
         dragelem=2;
      }
     }
     if(_M_ABS(mx-r)<=S) {
       dragelem=2;
        if(iseg==currseg-1) {
          iseg=currseg;
          dragelem=1;
        }
     }
     if(mvals && segments[iseg].middle>=0) {
       S/=2;
       S=(2>=S)?3:S; 
       if(_M_ABS(mx-m)<=S) 
         dragelem=4;
     }
     if(iseg!=currseg) 
          currseg=iseg;

     handle_push();
   case FL_DRAG:
     handle_drag(clamp(nval));
   case FL_RELEASE:
     value(clamp(nval));
     val=normval(nval);
     switch(dragelem){
     case 1: 
       if(currseg>=1 && 
        (val>=segments[currseg-1].left && val<=segments[currseg].right) && 
        ((mvals &&
        ((segments[currseg].middle>=0 && val<=segments[currseg].middle) 
          || (segments[currseg].middle<=0)) &&
        ((segments[currseg-1].middle>=0 && val>=segments[currseg-1].middle)
           || (segments[currseg-1].middle<=0)) ) || !mvals)
         ){
        segments[currseg].left=val;
        segments[currseg-1].right=val;
       }
       break;
     case 2: 
       if(currseg<=nsegs-2 && 
        (val<=segments[currseg+1].right && val>=segments[currseg].left) &&
        ((mvals &&
        ((segments[currseg].middle>=0 && val>=segments[currseg].middle) 
          || (segments[currseg].middle<=0)) &&
        ((segments[currseg+1].middle>=0 && val<=segments[currseg+1].middle) 
          || (segments[currseg].middle<=0))) || !mvals)  
        ) {
        segments[currseg].right=val;
        segments[currseg+1].left=val;
       }
       break;
     case 4: 
       if(mvals && val<=segments[currseg].right && val>=segments[currseg].left)
         segments[currseg].middle=val;
       break;
     case 8:
       break;
     default:
       break;
     }
     handle_release();
     slider_resp=1;
     break;
   case FL_KEYBOARD:
     switch (Fl::event_key()) {
       case FL_Insert:
          splitseg(); 
          redraw();
          slider_resp=1;
          break;
       case FL_Delete:
          removeseg();
          redraw();
          slider_resp=1;
          break;
       default:
        slider_resp=0;
     }
     break;
   case FL_FOCUS :
   case FL_UNFOCUS :
     if (Fl::visible_focus()) {
       redraw();
       slider_resp=1;
     } else slider_resp=0;
     break;
   case FL_ENTER :
   case FL_LEAVE :
     slider_resp=1;
     break;
   default:
     slider_resp=0;
 }
 return slider_resp;
}

int Fl_Multi_Value_Slider::handle(int event) 
{
  return handle(event,
		x()+Fl::box_dx(box()),
		y()+Fl::box_dy(box()),
		w()-Fl::box_dw(box()),
		h()-Fl::box_dh(box()));
}
