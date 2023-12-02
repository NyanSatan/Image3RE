#include "Image3Format.h"

#include <stdbool.h>

int image3InstantiateFromBuffer(
    Image3InternalState **newHandle,
    Image3ObjectHeader *buffer,
    uint32_t bufferSize,
    bool copyBuffer
);

int image3ValidateSignature(Image3InternalState *handle, char validationOptions);

void image3SHA1Generate(void *dataBuffer, uint32_t dataSize, void *hashBuffer);

int image3AESDecryptUsingLocalKey(void *buffer, uint32_t length);

int image3PKIVerifyHash(
    void *hashBuffer,
    uint32_t hashSize,
    void *signedHashBuffer,
    uint32_t signedHashSize,
    void *certBlobBuffer,
    uint32_t certBlobSize,
    void **certCustomData,
    uint32_t *certCustomDataSize
);
