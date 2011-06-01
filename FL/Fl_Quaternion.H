#ifndef __FL_QUATERNION_H__
#define __FL_QUATERNION_H__

class Fl_Quaternion {
    float v[3];
    float s;
  public:
    Fl_Quaternion() { v[0]=v[1]=v[2]=0; s=0; }
    Fl_Quaternion(float x, float y, float z) 
                 { fromEuler(x,y,z); }
    Fl_Quaternion(float phi, float x, float y, float z) 
                 {  fromAxis(phi,x,y,z);  }
    Fl_Quaternion(float phi,float a[3]) 
                 { fromAxis(phi,a); }
    Fl_Quaternion(Fl_Quaternion &nq) 
                 { copy(nq); }

    float *getVector() { return v; }
    float getVal() { return s; }
    void setVal(float ss) { s=ss; }

    void copy(Fl_Quaternion &nq);

    void add(Fl_Quaternion &nq);
    void multiply(Fl_Quaternion &nq);
    float normalize();

    void fromAxis(float phi,float a[3]);
    void fromAxis(float phi,float x,float y, float z);
    void fromEuler(float x,float y, float z);
    void fromEuler(float p[3]);
    void fromRotMatrix(float m[4][4]);

    void toAxis(float &phi,float &x,float &y, float &z);
    void toAxis(float &phi,float a[3]);
    void toEuler(float &x, float &y, float &z);
    void toEuler(float a[3]);
    void toRotMatrix(float m[4][4]);
};

void fl_rotMatrixToEuler(float m[4][4],float a[3]);
void fl_rotMatrixToEuler(float m[4][4],float &x,float &y,float &z);
void fl_trackball(Fl_Quaternion &q, float p1x, float p1y, float p2x, float p2y, float tballsize=0.8f);

#endif
