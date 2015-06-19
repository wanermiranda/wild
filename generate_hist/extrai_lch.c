#include "colordescriptorslib.h"
//#include "texturedescriptorslib.h"

void WriteFHist(Histogram *h,FILE *fp){
  int i;
  if (fp != NULL){
    for (i=0; i < h->n; i++){
        fprintf(fp,"%c",h->v[i]);
    }
    fprintf(fp,"\n");
  }
}

int main(int argc, char** argv)
{
    timer *tic, *toc;
    CImage *cimg=NULL;
    Image *mask=NULL;
    Histogram *hist=NULL;

    if (argc != 4) {
    fprintf(stderr,"usage: extrai_bic <image> <mask> <outputfile>\n");
    exit(-1);
    }

    cimg = ReadJPEGFile(argv[1]);
    mask = ReadImage(argv[2]);

    tic = Tic();
    hist = LCH(cimg, mask);
    toc = Toc();
    printf("LCH extracted in %f milliseconds\n\n",CTime(tic, toc));

    FILE *fp;
    fp = fopen(argv[3],"w");
    if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n",argv[3]);
        exit(-1);
    }

    WriteFHist(hist, fp);

    DestroyCImage(&cimg);
    DestroyHistogram(&hist);
    DestroyImage(&mask);
    fclose(fp);
    return(0);
}
