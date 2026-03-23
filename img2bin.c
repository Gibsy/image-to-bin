#define STB_IMAGE_IMPLEMENTATION
#ifdef __has_include
  #if __has_include("stb_image.h")
    #include "stb_image.h"
  #else
    #include <stb/stb_image.h>
  #endif
#else
  #include <stb/stb_image.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define OUT_W 128

#ifdef _WIN32
  #include <windows.h>
  #define PATH_SEP '\\'
  static void msgbox(const char*t,const char*m,int e){
      MessageBoxA(NULL,m,t,e?MB_ICONERROR:MB_ICONINFORMATION|MB_OK);
  }
#else
  #define PATH_SEP '/'
  static void msgbox(const char*t,const char*m,int e){
      fprintf(e?stderr:stdout,"[%s] %s\n",t,m);
  }
#endif

static uint8_t *resize_rgb(const uint8_t*src,int sw,int sh,int dw,int dh){
    uint8_t*dst=(uint8_t*)malloc(dw*dh*3);
    for(int y=0;y<dh;y++){
        float fy=(dh>1)?(float)y*(sh-1)/(dh-1):0;
        int y0=(int)fy,y1=y0+1; if(y1>=sh)y1=sh-1; float wy=fy-y0;
        for(int x=0;x<dw;x++){
            float fx=(dw>1)?(float)x*(sw-1)/(dw-1):0;
            int x0=(int)fx,x1=x0+1; if(x1>=sw)x1=sw-1; float wx=fx-x0;
            for(int c=0;c<3;c++){
                float v=(src[(y0*sw+x0)*3+c]*(1-wx)+src[(y0*sw+x1)*3+c]*wx)*(1-wy)
                       +(src[(y1*sw+x0)*3+c]*(1-wx)+src[(y1*sw+x1)*3+c]*wx)*wy;
                dst[(y*dw+x)*3+c]=(uint8_t)(v+0.5f);
            }
        }
    }
    return dst;
}

static void make_out(const char*in,char*out,size_t n){
    strncpy(out,in,n-1); out[n-1]=0;
    char*dot=strrchr(out,'.');
    char*sep=strrchr(out,PATH_SEP);
    if(dot&&dot>(sep?sep:out))*dot=0;
    strncat(out,".bin",n-strlen(out)-1);
}

static void process(const char*path){
    int w,h,ch;
    uint8_t*px=stbi_load(path,&w,&h,&ch,3);
    if(!px){
        char msg[512];
        snprintf(msg,sizeof(msg),"Cannot load:\n%s\n\n%s",path,stbi_failure_reason());
        msgbox("img2bin - Error",msg,1);
        return;
    }

    int dh=(int)((double)h*OUT_W/w); if(dh<1)dh=1;
    uint8_t*res=resize_rgb(px,w,h,OUT_W,dh);
    stbi_image_free(px);

    char out_path[4096];
    make_out(path,out_path,sizeof(out_path));
    FILE*fp=fopen(out_path,"wb");
    if(!fp){
        char msg[512]; snprintf(msg,sizeof(msg),"Cannot write:\n%s",out_path);
        msgbox("img2bin - Error",msg,1);
        free(res); return;
    }
    fwrite(res,1,OUT_W*dh*3,fp);
    fclose(fp);
    free(res);

    char msg[512];
    snprintf(msg,sizeof(msg),
        "Done!\n\nOutput: %s\nSize: %d x %d px (%d bytes)\n\nOpen in Binary Waterfall in RGB mode",
        out_path,OUT_W,dh,OUT_W*dh*3);
    msgbox("img2bin",msg,0);
}

int main(int argc,char**argv){
    if(argc<2){
        msgbox("img2bin",
            "\nSupported: JPG, PNG, BMP, TGA, PPM",0);
        return 0;
    }
    for(int i=1;i<argc;i++) process(argv[i]);
    return 0;
}