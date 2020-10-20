#ifndef IMAGE2D_H
#define IMAGE2D_H

#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/ml.hpp"
#include <stdio.h>
#include "point2D.h"


template <class T> const T& min(const T& a, const T& b) {
    return (b > a) ? a : b;     // or: return !comp(b,a)?a:b; for version (2)
}

template <class T> const T& max(const T& a, const T& b) {
    return (a > b) ? a : b;     // or: return !comp(b,a)?a:b; for version (2)
}

class image2D
{
public:
    /** Constructeurs */
    image2D(void);
    //image2D(const int width, const int height ,const double dx, const double dy, const int val);
    image2D(const int width, const int height);
    image2D(const char* path);


    /** Destructeur */
    ~image2D();

    /** Fonctions accesseurs\mutateurs  */
    int getwidth() const;
    int getheight() const;
    double getdx();
    double getdy();
    int getsz();
    int getval(const int i, const int j, const int k) const;

    point2D getneighbor(float x, float y);                // token const pour permettre l'utilisation dans la redefinition de l'opérateur "<<"

    void setwidth(const int width);
    void setheight(const int height);
    void setdx(const double dx);
    void setdy(const double dy);
    void setsz(const int sz);
    void setval(const int i, const int j, const int k, const int val);
    float sd(float data[], int n);

    bool imcontains(const int x, const int y);

    void esperance_max(const int k, const char* filename);
    void imtranslate(float tx, float ty, char* fname);
    void add_saltandpeper(int percent_noise, char* fname);

    void imtresh(int val, const char* fname);
    void mask(char* fname, char* fdest);
    void save(const char* fdest);
    void imhist(double* h);
    void imotsu(double* hist, const char* fname);

    friend int my_rand(int N);
    friend std::ostream& operator<<(std::ostream& strm, const image2D& a);

protected:
    int*** img= NULL;                              //img pointeur de pointeur d'int
    /** Fonctions gestion mémoire */
    void imfree(void);
    void imalloc(void);


    //   void imalloc(const int val);

private:
    int width=0, height=0;  //? Pq ne pas déclarer les variables "const" (ds le sujet), comme elle ne sont pas sujet à modification
    double dx=0, dy=0;
    int sz=0;
};

/** Fonctions "inline" (Opérateurs & constructeurs) */
inline image2D::image2D(void) {
    width = 128; height = 128; dx = 1; dy = 1;
    (*this).imalloc();
}

inline image2D::image2D(const int width, const int height) {
    (*this).width = width; (*this).height = height; dx = 1; dy = 1;
    imalloc(); //pas utilisé
}

/*
inline image2D::image2D(const double dx, const double dy, char *fpath){

    FILE* f=fopen(fpath,"rb");
    unsigned char header[54];

    fread(header,sizeof(*header),54,f);

    (*this).width=*(int*)&header[18];
    (*this).height=*(int*)&header[22];
    (*this).sz=3*(*this).width*(*this).height;
    (*this).dx=dx;(*this).dy=dy;
    //img=imalloc(img,width,height);

    (*this).imalloc();
    int szrow = ((*this).width*3 + 3) & (~3);
    unsigned char* data = new unsigned char[szrow];

    for(int i = 0; i < height; i++){
        fread(data, sizeof(data), szrow, f);
        for(int j = 0; j < width*3; j += 3){
            (*this).img[(int)j/3][i]=(data[j]+data[j+2]+data[j+3])/3;
        }
    }
    fclose(f);
}
*/

/** OS Operator */

inline std::ostream& operator<<(std::ostream& strm, const image2D& a) {
    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < a.getwidth(); i++) {
            for (int j = 0; j < a.getheight(); j++) {
                strm << a.getval(i, j, k) << " ";
            }
            strm << std::endl;
        }
        strm << std::endl << std::endl;
    }
    return strm;
}


#endif // IMAGE2D_H
inline int  my_rand(int N)
{
    static int first = 0;

    if (first == 0)
    {
        srand(time(NULL));
        first = 1;
    }
    return (rand() % N);
}