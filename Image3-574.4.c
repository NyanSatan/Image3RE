#include "Image3Format.h"

int image3ValidateSignature(Image3InternalState *handle, char validationOptions) {
  Image3InternalState *image_handle; // r5@1
  char options; // r8@1
  unsigned int flags; // r3@2
  int new_flags; // r3@3
  Image3ObjectHeader *image; // r10@4
  unsigned int image_sig_check_area; // r1@4
  unsigned int image_size_no_pack; // r12@4
  char *image_buffer; // r11@5
  Image3TagHeader *shsh_tag; // r6@5
  unsigned int shsh_tag_full_size; // r3@7
  int cert_tag_cursor; // r2@7
  Image3TagHeader *cert_tag; // r4@9
  unsigned int cert_tag_end; // r0@9
  int is_memset_needed; // r8@16
  int result; // r4@16
  Image3TagHeader *shsh_tag_data; // [sp-44h] [bp-44h]@14
  int actual_hash; // [sp-3Ch] [bp-3Ch]@13
  int nested_image_length; // [sp-28h] [bp-28h]@16
  char *nested_image; // [sp-24h] [bp-24h]@16
  int result_; // [sp+4h] [bp+4h]@0

  image_handle = handle;
  options = validationOptions;
  if ((validationOptions & 3) != 3) {
    flags = handle->flags;

    if (!(flags & kImage3ImageWasValidated)) {
      new_flags = flags | kImage3ImageWasValidated;
      handle->flags = new_flags;

      if (new_flags & kImage3ImageIsSigned) {
        image = handle->image;
        image_sig_check_area = handle->image->ihSignedLength;
        image_size_no_pack = handle->image->ihBufferLength;

        if (image_sig_check_area + sizeof(Image3TagHeader) <= image_size_no_pack) {
          image_buffer = (char *)image->ihBuffer;
          shsh_tag = (Image3TagHeader *)((char *)image->ihBuffer + image_sig_check_area);
        
          if (image_size_no_pack >= shsh_tag->itBufferLength + image_sig_check_area + sizeof(Image3TagHeader)
            && *(unsigned int *)image_buffer[image_sig_check_area] == 'SHSH') {
            shsh_tag_full_size = shsh_tag->itSkipDistance;
            cert_tag_cursor = image_sig_check_area + shsh_tag_full_size;

            if (!__CFADD__(image_sig_check_area, shsh_tag_full_size) && image_size_no_pack >= cert_tag_cursor + sizeof(Image3TagHeader)) {
              cert_tag = (Image3TagHeader *)&image_buffer[cert_tag_cursor];
              cert_tag_end = cert_tag->itBufferLength + sizeof(Image3TagHeader) + cert_tag_cursor;

              if (image_size_no_pack >= cert_tag_end && *(unsigned int *)&image_buffer[cert_tag_cursor] == 'CERT') {
                if (image_size_no_pack != cert_tag_end)
                  image->ihBufferLength = cert_tag_end;

                image3SHA1Generate(&image->ihSignedLength, image_sig_check_area + 8, &actual_hash);

                if (options & kImage3ValidateLocalStorage) {
                  shsh_tag_data = shsh_tag->itBuffer;
                  image3AESDecryptUsingLocalKey(shsh_tag->itBuffer, shsh_tag->itBufferLength);
                } else {
                  shsh_tag_data = shsh_tag->itBuffer;
                }

                nested_image = NULL;
                nested_image_length = 0;
                is_memset_needed = options & 2;
                result = image3PKIVerifyHash(
                           &actual_hash,
                           20,
                           shsh_tag_data,
                           shsh_tag->itBufferLength,
                           &cert_tag[1],
                           cert_tag->itBufferLength,
                           &nested_image,
                           &nested_image_length);

                if (!is_memset_needed)
                  memset(shsh_tag_data, 0, shsh_tag->itBufferLength);
                if (!result) {
                  if (nested_image) {
                    if (image3InstantiateFromBuffer(&image_handle->nestedImage) )
                      goto out;
                    image_handle->nestedImage->flags |= kImage3ImageIsSigned | kImage3ImageWasValidated | kImage3ImageIsTrusted;
                  }

                  image_handle->flags |= kImage3ImageIsTrusted;

                  if (!is_memset_needed) {
                    memset(
                      &image_buffer[image->ihSignedLength],
                      0,
                      (char *)image + image_handle->allocSize - &image_buffer[image->ihSignedLength]);
                    image->ihBufferLength = image->ihSignedLength;
                  }

                }
              }
            }
          }
        }
      }
    }
  }
out:
  return result;
}