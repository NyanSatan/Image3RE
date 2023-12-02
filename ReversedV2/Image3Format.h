#include <stdint.h>
#include <sys/types.h>

struct image_info {
    uint32_t   imageLength;
    uint32_t   imageAllocation;
    uint32_t   imageType;
    uint32_t   imagePrivateMagic;
#define IMAGE_MEMORY_INFO_MAGIC     'Memz'
#define IMAGE3_IMAGE_INFO_MAGIC     'img3'
    uint32_t   imageOptions;
    void       *imagePrivate;
};

typedef struct _Image3ObjectHeader {
    /* these fields are unsigned */
    uint32_t    ihMagic;
#define kImage3HeaderMagic      'Img3'
    uint32_t    ihSkipDistance;
    uint32_t    ihBufferLength;
    
    /* fields below are signed */
    uint32_t    ihSignedLength;
    uint32_t    ihType;
    uint8_t     ihBuffer[];
} Image3ObjectHeader;

typedef struct _Image3InternalState {
    Image3ObjectHeader      *image;
    uint32_t            flags;
#define kImage3ImageWasInstantiated (1<<0)
#define kImage3ImageWasValidated    (1<<1)  /* validation has been performed */
#define kImage3ImageIsTrusted       (1<<2)  /* validation indicates image is trusted */
#define kImage3ImageWasCreated      (1<<16)
#define kImage3ImageIsSigned        (1<<17) /* signature has been appended */
#define kImage3ImageWasAllocated    (1<<18)
    uint32_t                allocSize;
    struct _Image3InternalState *nestedImage;
} Image3InternalState;

typedef struct _Image3InternalState *Image3ObjectHandle;

struct image3_info {
    void *prev;
    void *next;

    void     *bdev;      /* device the image is on */
    off_t  devOffset;  /* where on the bdev it is */

    Image3ObjectHandle imageHandle;   /* if the image is loaded, its current handle */

    struct image_info   image_info; /* public image_info */
};

typedef struct {
    uint32_t    itTag;
    uint32_t    itSkipDistance;
    uint32_t    itBufferLength;
    uint8_t     itBuffer[];
} Image3TagHeader;

#define kImage3ValidateLocalStorage (1<<0)
