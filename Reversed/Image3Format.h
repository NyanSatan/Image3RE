struct image_info {
    unsigned   imageLength;
    unsigned   imageAllocation;
    unsigned   imageType;
    unsigned   imagePrivateMagic;
#define IMAGE_MEMORY_INFO_MAGIC     'Memz'
#define IMAGE2_IMAGE_INFO_MAGIC     'img2'
#define IMAGE3_IMAGE_INFO_MAGIC     'img3'
#define IMAGE4_IMAGE_INFO_MAGIC     'img4'
    unsigned   imageOptions;
    void       *imagePrivate;
};

typedef struct _Image3ObjectHeader {
    /* these fields are unsigned */
    unsigned    ihMagic;
#define kImage3HeaderMagic      'Img3'
    unsigned    ihSkipDistance;
    unsigned    ihBufferLength;
    
    /* fields below are signed */
    unsigned    ihSignedLength;
    unsigned    ihType;
    unsigned char   ihBuffer[];
} Image3ObjectHeader;

typedef struct _Image3InternalState {
    Image3ObjectHeader      *image;
    unsigned            flags;
#define kImage3ImageWasInstantiated (1<<0)
#define kImage3ImageWasValidated    (1<<1)  /* validation has been performed */
#define kImage3ImageIsTrusted       (1<<2)  /* validation indicates image is trusted */
#define kImage3ImageWasCreated      (1<<16)
#define kImage3ImageIsSigned        (1<<17) /* signature has been appended */
#define kImage3ImageWasAllocated    (1<<18)
    unsigned                allocSize;
    struct _Image3InternalState *nestedImage;
} Image3InternalState;


struct image3_info {
    void *prev;
    void *next;

    void     *bdev;      /* device the image is on */
    unsigned           devOffset;  /* where on the bdev it is */

    unsigned unk1;
    unsigned unk2;   /* if the image is loaded, its current handle */

    struct image_info   image_info; /* public image_info */
};

typedef struct {
    unsigned    itTag;
    unsigned    itSkipDistance;
    unsigned    itBufferLength;
    unsigned char   itBuffer[];
} Image3TagHeader;

#define kImage3ValidateLocalStorage (1<<0)
