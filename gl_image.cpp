#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <FL/Fl.H>
#include <FL/Fl_Shared_Image.H>
#include "FL/gl_image.hpp"

#define GL_CHECK_ERROR(str)                                        \
{                                                                  \
    GLenum error;                                                  \
    if(error = glGetError())                                       \
       fprintf(stderr,"GL Error: %s (%s)\n", gluErrorString(error), str);  \
}

unsigned clp2(unsigned x) 
{ 
 x = x - 1; 
 x = x | (x >> 1); 
 x = x | (x >> 2);
 x = x | (x >> 4); 
 x = x | (x >> 8); 
 x = x | (x >>16); 
 return x + 1; 
}

void gl_image(Fl_Shared_Image *img, GLenum target,int level,int border,
              int internalFormat,int w,int h)
{
 Fl_Image *temp;
 GLvoid *imgdata;
 GLubyte *iptr=NULL;
 GLenum imgformat;
 int datasize,ww,hh,ld,d,i,j; 
 double ss;

 if(!img) // spit error?
    return;
 if(!(img->count()))
   return; // image with no data
 
 // all ok generate texture..
 // scale image to ogl usable size
 ww=(w>0)?w:img->w();
 hh=(h>0)?h:img->h();
 
 // find nearest power of 2 >= ww
 ww=(int)clp2((unsigned)ww);
 // find nearest power of 2 >= hh
 hh=(int)clp2((unsigned)hh);
 
 // resize image
 temp = img->copy(ww,hh);
 ld=temp->ld();
 d=temp->d();

 if(1==temp->count() && d==0) {
     // image is a bitmap!! convert to one channel texture (luminance)
     unsigned char *bitdata=(unsigned char *)*(temp->data());
     imgdata= new GLubyte [ww*hh];
     datasize=(ww*hh)/8;
     for(i=0,iptr=(GLubyte*)imgdata; i<datasize; i++) {
           *iptr++=(bitdata[i]  & 1<<7)?255:0;
           *iptr++=(bitdata[i]  & 1<<6)?255:0;
           *iptr++=(bitdata[i]  & 1<<5)?255:0;
           *iptr++=(bitdata[i]  & 1<<4)?255:0;
           *iptr++=(bitdata[i]  & 1<<3)?255:0;
           *iptr++=(bitdata[i]  & 1<<2)?255:0;
           *iptr++=(bitdata[i]  & 1<<1)?255:0;
           *iptr++=(bitdata[i]  & 1)?255:0;
     }
     iptr=(GLubyte*)imgdata;
 } else if(temp->count()>=2) {
    // image is a pixmap convert to rgba 
   int tw,th,ncols,colw;
   unsigned char tcolor=0,c;
   imgdata= new GLubyte [ww*hh*4];
   sscanf(temp->data()[0],"%d %d %d %d",&tw,&th,&ncols,&colw);
   if(colw!=1) 
     return; // unhandled image format
   
   unsigned char colors[256][3],*cptr;
   // initialize colormap (fltk color ramp by default);
   for(i=0;i<256;i++) {
         Fl::get_color((Fl_Color)i, colors[i][0],colors[i][1],colors[i][2]);
   }
   
   if(ncols<0) { 
     // fltk colors in non standard xpm format (fltk compact)
     ncols=-ncols;
     tcolor=0;
     cptr=(unsigned char *)(temp->data()[1]);
     c=*cptr;
     i=0;
     colw=0;
     if(c==' ') {
       tcolor=c-' ';
       cptr++;
       colors[tcolor][0]=*cptr++;
       colors[tcolor][1]=*cptr++;
       colors[tcolor][2]=*cptr++;
       i++;
       colw=1;
     } 
     for(;i<ncols;i++) {
       c=(*cptr++)-' ';
       colors[c][0]=*cptr++;
       colors[c][1]=*cptr++;
       colors[c][2]=*cptr++;
     } 
     ncols=1;
   } else {
     // process colors in standard xpm pixmap format
     // uses standard fltk color map for the time being..
   }
 
   ncols++;
   for(i=ncols,iptr=(GLubyte*)imgdata; i<temp->count(); i++) {
     cptr=(unsigned char *)(temp->data()[i]);
     for(j=0; j<tw; j++) {
        c=(*cptr++)-' ';
        *iptr++=colors[c][0];
        *iptr++=colors[c][1];
        *iptr++=colors[c][2];
        if(colw && c==tcolor)
         *iptr++=0;
        else
         *iptr++=255;
    }
   }
   iptr=(GLubyte*)imgdata;
   d=4;
  } else {
   imgdata=(GLvoid*)*(temp->data());
   iptr=NULL;
 }
 switch(d) {
    case 0:
    case 1: imgformat=GL_LUMINANCE; break;
    case 2: imgformat=GL_LUMINANCE_ALPHA; break;
    case 3: imgformat=GL_RGB; break;
    case 4: imgformat=GL_RGBA; break;
    default: 
       imgformat=GL_RGB;
 }
    
 glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
/*
 if (!ld) ld = ww*temp->d();
 glPixelStorei(GL_UNPACK_ROW_LENGTH, ld/temp->d());
*/ 
 glTexImage2D(target,level,internalFormat,ww,hh,
                  border,imgformat,GL_UNSIGNED_BYTE, imgdata);
 GL_CHECK_ERROR("problem creating the texture");

 Fl_Shared_Image *im1=(Fl_Shared_Image *)temp;
 im1->release();

 if(iptr!=NULL)
   delete iptr;
}

void gl_image(char *fname, GLenum target,int level,int border,
              int internalFormat,int w,int h) 
{
 Fl_Shared_Image *img;

 if(fname==NULL) 
  return;
 img = Fl_Shared_Image::get(fname);
 gl_image(img,target,level,border,internalFormat,w,h);
}
