#ifndef _GL_IMAGE_HPP_
#define _GL_IMAGE_HPP_
#include <FL/Fl_Shared_Image.H>
#include <GL/glu.h>

/**Converts an FLTK shared image Fl_Image into an openGL texture,
   parameters and behavior are equivalent to glTexImage2D, which means that         the user must create the object names, proxys, etc as in normal opengl. 
   (it rounds image sizes to nearest power of 2 to comply with opengl spec.).
 */
void gl_image(Fl_Shared_Image *img, GLenum target=GL_TEXTURE_2D,int level=0,
              int border=0,int internalFormat=4,int w=0,int h=0);

/**Converts an FLTK shared image into an openGL texture.
   it looks for the image by it's filename on the set of FLTK loaded images,
   if it hasn't been loaded it loads it and adds it to the set of shared images.    Parameters and behavior are equivalent to glTexImage2D, which means that         the user must create the object names, proxys, etc as in normal opengl. 
   (it rounds image sizes to nearest power of 2 to comply with opengl spec.).
 */
void gl_image(char *fname, GLenum target=GL_TEXTURE_2D,int level=0,int border=0,
              int internalFormat=4,int w=0,int h=0);
#endif
