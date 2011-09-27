

#ifdef __cplusplus
extern "C"{
#endif 


typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

const IID IID_IFractalEngine = {0x2a0e4f58,0x3908,0x101b,{0x89,0x63,0x00,0x00,0x0b,0x65,0xc7,0x5b}};

#ifdef __cplusplus
}
#endif

