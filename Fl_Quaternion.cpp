#include "FL/Fl_Quaternion.H"
#include <math.h>


#define FLQERR  1e-6

#define FLQVECMAG(x,y,z) sqrt(((x)*(x))+((y)*(y))+((z)*(z))) 

static void FLQVECNORM(float *v)
{
 float mg=FLQVECMAG(v[0],v[1],v[2]);
 if(mg!=0){
  v[0]/=mg;
  v[1]/=mg;
  v[2]/=mg;
 }
}
static void FLQVECCOPY(const float *v, float *v1)
{
 v1[0]=v[0];
 v1[1]=v[1];
 v1[2]=v[2];
}

static void FLQVECCROSS(const float *v1,const float *v2, float *vr)
{
 float t[3];
 t[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
 t[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
 t[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
 vr[0]=t[0]; 
 vr[1]=t[1];
 vr[2]=t[2];  
}

static float  FLQVECDOT(const float *v,const float *v1)
{
  return v[0]*v1[0]+v[1]*v1[1]+v[2]*v1[2];
}

static void FLQVECSCALE(float *v, float s)
{
 v[0] *= s;
 v[1] *= s;
 v[2] *= s;
}

static void FLQVECADD(float *v, float *v1,float vf=1.0)
{
 v[0]+=v1[0]*vf;
 v[1]+=v1[1]*vf;
 v[2]+=v1[2]*vf;
}

static float fl_tb_project_to_sphere(float r, float x, float y)
{
    float d, t, z;
    d = sqrt(x*x + y*y);
    if (d < r * 0.70710678118654752440) {    /* Inside sphere */
        z = sqrt(r*r - d*d);
    } else {           /* On hyperbola */
        t = r / 1.41421356237309504880;
        z = t*t / d;
    }
    return z;
}


void Fl_Quaternion::copy(Fl_Quaternion &nq)
{ 
 s=nq.getVal();
 FLQVECCOPY(nq.getVector(),v);
}

void Fl_Quaternion::add(Fl_Quaternion &nq) 
{
 Fl_Quaternion t1, t2,tf;
 t1.copy(*this);
 FLQVECSCALE(t1.getVector(),nq.getVal());
 t2.copy(nq);
 FLQVECSCALE(t2.getVector(),getVal());
 FLQVECCROSS(nq.getVector(),getVector(),tf.getVector());
 FLQVECADD(t1.getVector(),t2.getVector());
 FLQVECADD(tf.getVector(),t1.getVector());
 s=s*nq.getVal() - FLQVECDOT(getVector(),nq.getVector());
 FLQVECCOPY(tf.getVector(),v); 
 normalize();
}

void Fl_Quaternion::multiply(Fl_Quaternion &nq)
{
 Fl_Quaternion t1,t2,t3,tf;
 tf.copy(*this);
 FLQVECCROSS(tf.getVector(),nq.getVector(),tf.getVector()); 
 t1.copy(*this);
 FLQVECSCALE(t1.getVector(),nq.getVal());
 t2.copy(nq);
 FLQVECSCALE(t2.getVector(),getVal());
 FLQVECADD(tf.getVector(),t1.getVector());
 FLQVECADD(tf.getVector(),t2.getVector());
 s=s*nq.getVal() - FLQVECDOT(getVector(),nq.getVector());
 FLQVECCOPY(tf.getVector(),v); 
 normalize();
}

float Fl_Quaternion::normalize()
{
 int i;
 float mg=sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + s*s);
 if(mg>FLQERR) {
   for (i = 0; i < 3; i++)
      v[i] /= mg;
   s/=mg;
 } else {
   for (i = 0; i < 3; i++) 
      v[i] = 0.0;
   s=1.0;
 }
 return mg;
}

void Fl_Quaternion::fromAxis(float phi,float a[3])
{
 FLQVECCOPY(a,v);
 FLQVECNORM(v); 
 FLQVECSCALE(v,sin(phi/2.0));
 s=cos(phi/2.0);
}
void Fl_Quaternion::fromAxis(float phi,float x,float y, float z)
{
 float a[3]; 
 a[0]=x; 
 a[1]=y;
 a[2]=z;
 fromAxis(phi,a);
}

void Fl_Quaternion::fromEuler(float x,float y, float z)
{
 float a[3]; 
 a[0]=x; 
 a[1]=y;
 a[2]=z;
 fromEuler(a);
}
void Fl_Quaternion::fromEuler(float p[3])
{
 int i;
 Fl_Quaternion qt,q1;
 float a[3]={ 1.0, 0.0, 0.0 };
 qt.fromAxis(p[0],a);

 a[0]=0.0; a[1]=1.0; a[2]=0.0;
 q1.fromAxis(p[1],a);
 q1.multiply(qt);
 
 a[0]=0.0; a[1]=0.0; a[2]=1.0; 
 fromAxis(p[2],a);
 multiply(q1);
}

void Fl_Quaternion::fromRotMatrix(float m[4][4])
{
 // t= matrix trace
 float t=m[0][0]+m[1][1]+m[2][2]+m[3][3];
 float ss;
 float *mat=(float*)m;
 if(t>FLQERR) {
   ss=sqrt(t)*2.0;
   v[0]=(mat[9]-mat[6])/ss;
   v[1]=(mat[2]-mat[8])/ss;
   v[2]=(mat[4]-mat[1])/ss;
   s=0.25 * ss;
 } else {
    if ( mat[0] > mat[5] && mat[0] > mat[10] )  {	// Column 0: 
        ss= sqrt( 1.0 + mat[0] - mat[5] - mat[10] ) * 2.0;
        v[0]=0.25*ss;
        v[1]=(mat[4]+mat[1])/ ss;
        v[2]=(mat[2]+mat[8])/ ss;
        s=(mat[9]-mat[6])/ ss;
    } else if ( mat[5] > mat[10] ) {			// Column 1: 
        ss=sqrt(1.0+mat[5] - mat[0] - mat[10] ) * 2.0;
        v[0]=(mat[4]+mat[1])/ss;
        v[1]=0.25*ss;
        v[2]=(mat[9]+mat[6])/ss;
        s=(mat[2]-mat[8])/ss;
    } else {						// Column 2:
        ss=sqrt( 1.0 + mat[10] - mat[0] - mat[5] ) * 2.0;
        v[0]=(mat[2]+mat[8])/ss;
        v[1]=(mat[9]+mat[6])/ss;
        v[2]=0.25*ss;
        s=(mat[4]-mat[1])/ss;
    }
 }
}

void Fl_Quaternion::toAxis(float &phi,float &x,float &y, float &z)
{
 float a[3];
 toAxis(phi,a);
 x=a[0];
 y=a[1];
 z=a[2];
}

void Fl_Quaternion::toAxis(float &phi,float a[3])
{
 Fl_Quaternion tf;
 float cos_angle,sin_angle,angle;

 FLQVECCOPY(v,tf.getVector());
 tf.setVal(s);
 tf.normalize();

 cos_angle  = tf.getVal();
 angle      = acos( cos_angle ) * 2;
 sin_angle  = sqrt( 1.0 - cos_angle * cos_angle );

 if ( fabs( sin_angle ) < 0.0005 )
     sin_angle = 1;

 a[0] =tf.getVector()[0]/ sin_angle;
 a[1] =tf.getVector()[1]/ sin_angle;
 a[2] =tf.getVector()[2]/ sin_angle;
 phi=angle;
}


void Fl_Quaternion::toEuler(float &x, float &y, float &z)
{
 float a[3];
 toEuler(a);
 x=a[0];
 y=a[1];
 z=a[2];
}

void Fl_Quaternion::toEuler(float a[3])
{
 float m[4][4];
 toRotMatrix(m);
 fl_rotMatrixToEuler(m,a);
}

void Fl_Quaternion::toRotMatrix(float m[4][4])
{
 m[0][0] = 1.0 - 2.0 * (v[1] * v[1] + v[2] * v[2]);
 m[0][1] = 2.0 * (v[0] * v[1] - v[2] * s);
 m[0][2] = 2.0 * (v[2] * v[0] + v[1] * s);
 m[0][3] = 0.0; 

 m[1][0] = 2.0 * (v[0] * v[1] + v[2] * s);
 m[1][1]= 1.0 - 2.0 * (v[2] * v[2] + v[0] * v[0]);
 m[1][2] = 2.0 * (v[1] * v[2] - v[0] * s);
 m[1][3] = 0.0;

 m[2][0] = 2.0 * (v[2] * v[0] - v[1] * s);
 m[2][1] = 2.0 * (v[1] * v[2] + v[0] * s);
 m[2][2] = 1.0 - 2.0 * (v[1] * v[1] + v[0] * v[0]);
 m[2][3] = 0.0;

 m[3][0] = 0.0;
 m[3][1] = 0.0;
 m[3][2] = 0.0;
 m[3][3] = 1.0;
}


void fl_rotMatrixToEuler(float m[4][4],float a[3])
{
 float *mat=(float *)m;
 float tx,ty;
 float C,D;
 a[1] = D = -asin(mat[2]);        /* Calculate Y-axis angle */
 C  =  cos(a[1]);
 if ( fabs(C)>0.005 ) {	      /* Gimball lock? */
   tx=mat[10]/C;	      /* No, so get X-axis angle */
   ty=-mat[6]/C;
   a[0]=atan2(ty,tx);
   tx=mat[0]/C;	      /* Get Z-axis angle */
   ty=-mat[1]/C;
   a[2]=atan2(ty,tx);
 } else	{			      /* Gimball lock has occurred */
   a[0]  = 0;		      /* Set X-axis angle to zero */
   tx      = mat[5];		      /* And calculate Z-axis angle */
   ty      = mat[4];
   a[2]  = atan2(ty,tx);
 }
}

void fl_rotMatrixToEuler(float m[4][4],float &x,float &y,float &z)
{
 float a[3];
 a[0]=x; 
 a[1]=y;
 a[2]=z;
 fl_rotMatrixToEuler(m,a);
}

void fl_trackball(Fl_Quaternion &q, float p1x, float p1y, float p2x, float p2y, float tballsize)
{
 float a[3]; /* Axis of rotation */
 float phi;  /* how much to rotate about axis */
 float p1[3], p2[3];
 float t;
 if (p1x == p2x && p1y == p2y) {
     /* Zero rotation */
     q.getVector()[0]=0;
     q.getVector()[1]=0;
     q.getVector()[2]=0;
     q.setVal(1.0);
     return;
 }
 // First, figure out z-coordinates for projection of P1 and P2 to
 p1[0]=p1x; p1[1]=p1y; p1[2]=fl_tb_project_to_sphere(tballsize,p1x,p1y);
 p2[0]=p2x; p2[1]=p2y; p2[2]=fl_tb_project_to_sphere(tballsize,p2x,p2y);

 //  Now, we want the cross product of P1 and P2
 FLQVECCROSS(p2,p1,a); 

 // Figure out how much to rotate around that axis.
 FLQVECADD(p1,p2,-1);

 t = FLQVECMAG(p1[0],p1[1],p1[2]) / (2.0*tballsize);

 //Avoid problems with out-of-control values...
 if (t > 1.0) t = 1.0;
 if (t < -1.0) t = -1.0;
 phi = 2.0 * asin(t);

 q.fromAxis(phi,a);
}
