#include "Image3Format.h"
#include "common.h"

int image3_load(struct image_info *image_info, unsigned int magic, void **addr, unsigned int *size) {
    /*
     * Look to IDB for this function.
     * At some point it starts to reuse stack vars,
     * and I'm too lazy to fix that, especially now
     */
}

int image3GetTagStruct(Image3InternalState *objectHandle, const unsigned int withTag, void **structPtr, unsigned int *structSize, int skipCount) {
  Image3ObjectHeader *image_buffer; // r0@1
  unsigned int tag_ident; // r4@1
  int skipCount_; // r1@1
  Image3TagHeader *tagPointer; // r12@1
  void **structPtr_; // r6@1
  unsigned int *structSize_; // r5@1
  int image_size_no_pack; // r0@1
  unsigned __int8 is_end_overflow; // cf@1
  char *end_of_tags; // r2@1
  void *tag_buffer; // r9@2
  int tag_data_length; // r3@3
  signed int is_tag_end_overflow; // r0@4
  int result; // r0@16
  unsigned int tag_full_length; // r0@18

  image_buffer = objectHandle->image;
  tag_ident = withTag;
  skipCount_ = skipCount;
  tagPointer = (Image3TagHeader *)&image_buffer->ihBuffer;
  structPtr_ = structPtr;
  structSize_ = structSize;
  image_size_no_pack = image_buffer->ihBufferLength;
  is_end_overflow = __CFADD__(image_size_no_pack, tagPointer);
  end_of_tags = (char *)tagPointer + image_size_no_pack;

  while (1) {

    if (is_end_overflow)
      return EINVAL;

    if (tagPointer >= (Image3TagHeader *)end_of_tags)
      return ENOENT;

    tag_buffer = &tagPointer->itBuffer;
    if (end_of_tags < (char *)&tagPointer->itBuffer)
      return EINVAL;

    tag_data_length = tagPointer->itBufferLength;
    is_tag_end_overflow = __CFADD__(tag_buffer, tag_data_length) ? 1 : 0;

    if ((char*)tag_buffer + tag_data_length > end_of_tags)
      is_tag_end_overflow |= 1u;

    if (is_tag_end_overflow)
      return EINVAL;

    if (tag_ident == -1 || tag_ident == tagPointer->itTag) 
      break;

next_tag:
    tag_full_length = tagPointer->itSkipDistance;

    if (tag_full_length < tag_data_length + sizeof(Image3TagHeader))
      return EINVAL;

    is_end_overflow = __CFADD__(tag_full_length, tagPointer);
    tagPointer = (Image3TagHeader *)((char *)tagPointer + tag_full_length);

  }

  if (skipCount_) {
    --skipCount_;
    goto next_tag;
  }

  if (!structSize_) {
out:
    result = 0;
    *structPtr_ = tag_buffer;
    return result;
  }

  if (!*structSize_ || tag_data_length == *structSize_) {
    *structSize_ = tag_data_length;
    goto out;
  }

  return EINVAL;

}

int image3PKIVerifyHash(void *hashBuffer, unsigned int hashSize, void *signedHashBuffer, unsigned int signedHashSize, void *certBlobBuffer, unsigned int certBlobSize, void **certCustomData, unsigned int *certCustomDataSize) {
  return verify_signature_with_chain(
           certBlobBuffer,
           certBlobSize,
           signedHashBuffer,
           signedHashSize,
           hashBuffer,
           hashSize,
           certCustomData,
           certCustomDataSize) != 0;
}

int image3AESDecryptUsingLocalKey(void *buffer, unsigned int length) {
  int result; // r0@3
  char output[16]; // [sp+Ch] [bp-30h]@1
  char dest[16]; // [sp+1Ch] [bp-20h]@1
  int stack_protector; // [sp+2Ch] [bp-10h]@1

  unsigned char derivedSeed[16] = {0xdb, 0x1f, 0x5b, 0x33, 0x60, 0x6c, 0x5f, 0x1c,
                       0x19, 0x34, 0xaa, 0x66, 0x58, 0x9c, 0x06, 0x61};


  stack_protector = 0x10060008;
  memmove(&dest, &derivedSeed, 0x10u);
  if ( aes_crypto_cmd(0x10u, &dest, &output, 0x10u, 0x100u, 0, 0)
    || aes_crypto_cmd(0x11u, buffer, buffer, length, 0, &output, 0)) {
    result = -1;
  }
  else {
    memset(&output, 0, 16);
    result = 0;
  }

  if (stack_protector != 0x10060008)
    stack_chk_fail(result);

  return result;
}

void image3SHA1Generate(void *dataBuffer, unsigned int dataSize, void *hashBuffer) {
  sha1_calculate(dataBuffer, dataSize, hashBuffer);
}

void* image3Malloc(unsigned int size) {
  return calloc(size, 1, 0);
}

void image3Free(void *buffer, unsigned int size) {
  free(buffer);
}

int image3InstantiateFromBuffer(Image3InternalState *newHandle, Image3ObjectHeader *buffer, unsigned int bufferSize, int copyBuffer) {
  unsigned int size_no_pack; // r1@3
  Image3InternalState *handle_buffer; // r0@6
  int image_total_length; // r0@10
  Image3ObjectHeader *newBuffer; // r0@10
  signed int result; // r0@12


  if ( bufferSize < 0x14
    || buffer->ihMagic != 'Img3'
    || size_no_pack = buffer->ihBufferLength, size_no_pack > bufferSize - 20
    || size_no_pack < buffer->ihSignedLength
    || size_no_pack + 20 > buffer->ihSkipDistance) {

      result = EINVAL;

  } else {

    handle_buffer = (Image3InternalState *)image3Malloc(16);

    if (!handle_buffer)
      return ENOMEM;

    handle_buffer->flags = kImage3ImageWasInstantiated;
    handle_buffer->nestedImage = NULL;

    if (buffer->ihSignedLength)
      handle_buffer->flags |= kImage3ImageIsSigned;

    if (copyBuffer) {
      image_total_length = buffer->ihBufferLength + 20;

      handle_buffer->allocSize = image_total_length;
      newBuffer = image3Malloc(image_total_length);
      handle_buffer->image = newBuffer;

      if (!newBuffer) {
        image3Free(handle_buffer, 16);
        return ENOMEM;
      }

      memmove(newBuffer, buffer, handle_buffer->allocSize);

      handle_buffer->flags |= kImage3ImageWasAllocated;

    } else {
      handle_buffer->image = buffer;
      handle_buffer->allocSize = bufferSize;
    }

    result = 0;
    newHandle = (Image3InternalState *)handle_buffer;

  }

  return result;

}

int image3ValidateSignature(Image3InternalState *handle, char validationOptions) {
  Image3InternalState *image_handle; // r8@1
  unsigned int flags; // r3@1
  int new_flags; // r3@3
  Image3ObjectHeader *image; // r6@5
  unsigned int image_size_no_pack; // r12@5
  unsigned int image_sig_check_area; // r1@6
  unsigned char *image_buffer; // r10@7
  Image3TagHeader *shsh_tag; // r4@7
  int shsh_tag_full_size; // r3@9
  unsigned int cert_tag_cursor; // r0@9
  Image3TagHeader *cert_tag; // r5@11
  unsigned int cert_tag_end; // r3@13
  int nested_image_init; // r0@21
  unsigned int image_full_size; // r2@24
  char *shsh_tag_ptr; // r0@24
  int image_sig_check_area_part_1; // r2@24
  int image_sig_check_area_part_2; // r3@24
  unsigned int image_sig_check_area_; // r3@24
  int result; // r0@26
  unsigned char *shsh_tag_data; // [sp+10h] [bp-40h]@17
  char options; // [sp+14h] [bp-3Ch]@1
  int nested_image_length; // [sp+18h] [bp-38h]@19
  unsigned char *nested_image; // [sp+1Ch] [bp-34h]@19
  char actual_hash; // [sp+20h] [bp-30h]@15
  int stack_protector; // [sp+34h] [bp-1Ch]@1

  options = validationOptions;
  image_handle = handle;
  flags = handle->flags;

  if (flags & kImage3ImageWasValidated) {
    result = ((flags >> 2) ^ 1) & 1;
    goto out;
  }

  new_flags = flags | kImage3ImageWasValidated;
  handle->flags = new_flags;
  if (!(new_flags & kImage3ImageIsSigned)) {
    result = EPERM;
    goto out;
  }

  image = handle->image;

  image_size_no_pack = handle->image->ihBufferLength;

  if (image_size_no_pack < sizeof(Image3TagHeader))
    goto out_einval;

  image_sig_check_area = handle->image->ihSignedLength;

  if (image_sig_check_area > handle->image->ihBufferLength - sizeof(Image3TagHeader))
    goto out_einval;

  image_buffer = (unsigned char*)&image->ihBuffer;

  shsh_tag = (Image3TagHeader *)((unsigned char *)&image->ihBuffer + image->ihSignedLength);

  if (shsh_tag->itBufferLength > handle->image->ihBufferLength - image->ihSignedLength - sizeof(Image3TagHeader))
    goto out_einval;

  if (shsh_tag->itTag != 'SHSH')
    goto out_einval;

  shsh_tag_full_size = shsh_tag->itSkipDistance;
  cert_tag_cursor = shsh_tag_full_size + image->ihSignedLength;

  if (__CFADD__(shsh_tag_full_size, image->ihSignedLength)) {
    goto out_einval;
  }

  if (image_size_no_pack - sizeof(Image3TagHeader) < cert_tag_cursor)
    goto out_einval;

  cert_tag = (Image3TagHeader *)&image_buffer[cert_tag_cursor];

  if (cert_tag->itBufferLength > image_size_no_pack - cert_tag_cursor - sizeof(Image3TagHeader) || cert_tag->itTag != 'CERT' ) {
    goto out_einval;
  }

  cert_tag_end = cert_tag->itBufferLength + sizeof(Image3TagHeader) + cert_tag_cursor;

  if (image_size_no_pack != cert_tag_end) {
    image->ihBufferLength = cert_tag_end;
  }

  image3SHA1Generate(&image->ihSignedLength, image_sig_check_area + 8, &actual_hash);
  if (!(options & kImage3ValidateLocalStorage)) {
    shsh_tag_data = shsh_tag->itBuffer;
    goto verify;
  }

  if (shsh_tag->itBufferLength & 0xF) {
out_einval:
    result = EINVAL;
    goto out;
  }

  shsh_tag_data = shsh_tag->itBuffer;
  image3AESDecryptUsingLocalKey(
    shsh_tag->itBuffer,
    shsh_tag->itBufferLength);

verify:
  nested_image = NULL;
  nested_image_length = 0;
  result = image3PKIVerifyHash(
              &actual_hash,
              20,
              shsh_tag_data,
              shsh_tag->itBufferLength,
              cert_tag->itBuffer,
              cert_tag->itBufferLength,
              &nested_image,
              &nested_image_length);
  memset(shsh_tag_data, 0, shsh_tag->itBufferLength);

  if (result)
    goto out;

  if (nested_image) {
    nested_image_init = image3InstantiateFromBuffer(&image_handle->nestedImage);
    if (nested_image_init) {
      result_ = nested_image_init;
      goto out;
    }

    image_handle->nestedImage->flags |= kImage3ImageIsSigned | kImage3ImageWasValidated | kImage3ImageIsTrusted;
  }

  image_full_size = image_handle->allocSize;
  image_handle->flags |= kImage3ImageIsTrusted;

  shsh_tag_ptr = (char *)&image->ihBuffer + image->ihSignedLength;
  memset(shsh_tag_ptr, 0, (char *)image + image_full_size - shsh_tag_ptr);

  image->ihBufferLength = image->ihSignedLength;

out:
  return result;

}
