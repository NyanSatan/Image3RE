#include "Image3.h"
#include "common.h"

#include <string.h>

#define SHA1_HASH_SIZE  0x20

// TODO: check these macroses
#define LETOH(x)    (((x << 24) & 0xff) | ((x << 16) & 0xff) | ((x << 8) & 0xff) | ((x) & 0xff))
#define HTOLE(x)    (((x >> 24) & 0xff) | ((x >> 16) & 0xff) | ((x >> 8) & 0xff) | ((x) & 0xff))

int image3InstantiateFromBuffer(
    Image3InternalState **newHandle, /* R0 */
    Image3ObjectHeader *buffer, /* R1 */
    uint32_t bufferSize, /* R2 */
    bool copyBuffer /* R3 */
) {
    /* ... */  
}

int image3ValidateSignature(Image3ObjectHandle handle /* R0 */, char validationOptions /* R1 */) {
    int                 result;
    uint32_t            flags;
    Image3ObjectHeader *image;
    uint32_t            imageBufferLength;
    uint32_t            signedLength;
    Image3TagHeader    *shshTag;
    uint32_t            certTagPosition;
    Image3TagHeader    *certTag;
    uint32_t            certTagContentEnd;
    uint8_t             hash[SHA1_HASH_SIZE];
    void               *shshTagBuffer;
    void               *nestedImage;
    uint32_t            nestedImageLength;

    /*
     * ROM:00004370                 PUSH            {R4-R7,LR}
     * ROM:00004372                 ADD             R7, SP, #0xC
     * ROM:00004374                 PUSH.W          {R8,R10,R11}
     * ROM:00004378                 SUB             SP, SP, #0x38
     *
     * Prologue, pushing callee-saved registers,
     * setting frame pointer,
     * allocating memory from stack (0x38 bytes)
     */

    /*
     * ROM:0000437A                 STR             R1, [SP,#0x14]
     *
     * Saving `validationOptions` variable to stack,
     * for fututre usage
     */

    /*
     * ROM:0000437C                 LDR             R1, =0x10060008
     *
     * Loading stack cookie address from DATA
     */

    /*
     * ROM:0000437E                 MOV             R8, R0
     *
     * Saving `handle` to R8, for future usage
     */

    /*
     * ROM:00004380                 LDR             R3, [R1]
     * ROM:00004382                 STR             R3, [SP,#0x34]
     *
     * Loading stack cookie itself,
     * and storing it at the end of the stack
     */

    /*
     * ROM:00004384                 LDR             R3, [R0,#0x4]
     * ROM:00004386                 ANDS.W          R11, R3, #2
     * ROM:0000438A                 BEQ             loc_4398
     * ROM:0000438C                 LSRS            R3, R3, #2
     * ROM:0000438E                 EOR.W           R0, R3, #1
     * ROM:00004392                 AND.W           R5, R0, #1
     * ROM:00004396                 B               out
     *
     * Loading handle flags and checking if an image
     * was already validated, if so returning result
     */

    flags = handle->flags;

    if (flags & kImage3ImageWasValidated) {
        result = (flags & kImage3ImageIsTrusted) ? 0 : EPERM;
        goto out;
    }

    /*
     * ROM:00004398 loc_4398                                ; CODE XREF: image3ValidateSignature+1A↑j
     * ROM:00004398                 ORR.W           R3, R3, #2
     * ROM:0000439C                 TST.W           R3, #0x20000
     * ROM:000043A0                 STR             R3, [R0,#4]
     * ROM:000043A2                 BNE             loc_43A8
     * ROM:000043A4                 MOVS            R5, #1
     * ROM:000043A6                 B               out
     *
     * Setting kImage3ImageWasValidated flag back to handle,
     * checking if image is signed, if not returning EPERM
     */

    flags |= kImage3ImageWasValidated;
    handle->flags = flags;

    if ((flags & kImage3ImageIsSigned) == 0) {
        result = EPERM;
        goto out;
    }

    /*
     * ROM:000043A8 loc_43A8                                ; CODE XREF: image3ValidateSignature+32↑j
     * ROM:000043A8                 LDR             R6, [R0]
     * ROM:000043AA                 LDRB            R3, [R6,#9]
     * ROM:000043AC                 LDRB            R2, [R6,#8]
     * ROM:000043AE                 ORR.W           R2, R2, R3,LSL#8
     * ROM:000043B2                 LDRB            R3, [R6,#0xA]
     * ROM:000043B4                 ORR.W           R2, R2, R3,LSL#16
     * ROM:000043B8                 LDRB            R3, [R6,#0xB]
     * ROM:000043BA                 ORR.W           R12, R2, R3,LSL#24
     * ROM:000043BE                 CMP.W           R12, #0xB
     * ROM:000043C2                 BLS.W           out_einval
     *
     * Loading image header pointer from the handle,
     * validating that image contains at least one tag
     * (ihBufferLength >= 12)
     *
     * Note the horrible way to extract LE values
     */

    image = handle->image;
    imageBufferLength = LETOH(image->ihBufferLength);

    if (imageBufferLength < sizeof(Image3TagHeader)) {
        goto out_einval;
    }

    /*
     * ROM:000043C6                 LDRB            R3, [R6,#0xD]
     * ROM:000043C8                 LDRB            R2, [R6,#0xC]
     * ROM:000043CA                 LDRB            R1, [R6,#0xF]
     * ROM:000043CC                 SUB.W           LR, R12, #0xC
     * ROM:000043D0                 ORR.W           R2, R2, R3,LSL#8
     * ROM:000043D4                 LDRB            R3, [R6,#0xE]
     * ROM:000043D6                 ORR.W           R3, R2, R3,LSL#16
     * ROM:000043DA                 ORR.W           R1, R3, R1,LSL#24
     * ROM:000043DE                 CMP             R1, LR
     * ROM:000043E0                 BHI.W           out_einval
     *
     * If signed length overlaps overall payload length, we fail
     */

    signedLength = LETOH(image->ihSignedLength);

    if (signedLength > imageBufferLength - sizeof(Image3TagHeader)) {
        goto out_einval;
    }

    /*
     * ROM:000043E4                 ADD.W           R10, R6, #0x14
     * ROM:000043E8                 ADD.W           R4, R10, R1
     * ROM:000043EC                 LDRB            R3, [R4,#9]
     * ROM:000043EE                 LDRB            R2, [R4,#8]
     * ROM:000043F0                 ORR.W           R2, R2, R3,LSL#8
     * ROM:000043F4                 LDRB            R3, [R4,#0xA]
     * ROM:000043F6                 ORR.W           R2, R2, R3,LSL#16
     * ROM:000043FA                 LDRB            R3, [R4,#0xB]
     * ROM:000043FC                 ORR.W           R2, R2, R3,LSL#24
     * ROM:00004400                 RSB.W           R3, R1, R12
     * ROM:00004404                 SUBS            R3, #0xC
     * ROM:00004406                 CMP             R2, R3
     * ROM:00004408                 BHI.W           out_einval
     *
     * Checking that SHSH tag's content doesn't overlap the end of the image
     */

    shshTag = (Image3TagHeader *)&image->ihBuffer[signedLength];

    if (LETOH(shshTag->itBufferLength) > imageBufferLength - signedLength - sizeof(Image3TagHeader)) {
        goto out_einval;
    }

    /*
     * ROM:0000440C                 LDRB            R3, [R4,#1]
     * ROM:0000440E                 LDRB.W          R2, [R10,R1]
     * ROM:00004412                 ORR.W           R2, R2, R3,LSL#8
     * ROM:00004416                 LDRB            R3, [R4,#2]
     * ROM:00004418                 ORR.W           R2, R2, R3,LSL#16
     * ROM:0000441C                 LDRB            R3, [R4,#3]
     * ROM:0000441E                 ORR.W           R3, R2, R3,LSL#24
     * ROM:00004422                 LDR             R2, ='SHSH'
     * ROM:00004424                 CMP             R3, R2
     * ROM:00004426                 BNE.W           out_einval
     *
     * Checking that SHSH tag's ident is really 'SHSH'
     */

    if (LETOH(shshTag->itTag) != 'SHSH') {
        goto out_einval;
    }

    /*
     * ROM:0000442A                 LDRB            R3, [R4,#5]
     * ROM:0000442C                 LDRB            R2, [R4,#4]
     * ROM:0000442E                 ORR.W           R2, R2, R3,LSL#8
     * ROM:00004432                 LDRB            R3, [R4,#6]
     * ROM:00004434                 ORR.W           R2, R2, R3,LSL#16
     * ROM:00004438                 LDRB            R3, [R4,#7]
     * ROM:0000443A                 ORR.W           R3, R2, R3,LSL#24
     * ROM:0000443E                 ADDS            R0, R3, R1
     * ROM:00004440                 BCS.W           out_einval
     *
     * Checking that SHSH tag start + its skipDistance do not overflow uint32_t
     */

    certTagPosition = signedLength + shshTag->itSkipDistance;

    if (certTagPosition < signedLength) {
        goto out_einval;
    }

    /*
     * ROM:00004444                 CMP             LR, R0
     * ROM:00004446                 BCC.W           out_einval
     *
     * Checking that CERT tag doesn't overlaps the image
     */

    if (certTagPosition > imageBufferLength - sizeof(Image3TagHeader)) {
        goto out_einval;
    }

    /*
     * ROM:0000444A                 ADD.W           R5, R10, R0
     * ROM:0000444E                 LDRB            R3, [R5,#9]
     * ROM:00004450                 LDRB            R2, [R5,#8]
     * ROM:00004452                 ORR.W           R2, R2, R3,LSL#8
     * ROM:00004456                 LDRB            R3, [R5,#0xA]
     * ROM:00004458                 ORR.W           R2, R2, R3,LSL#16
     * ROM:0000445C                 LDRB            R3, [R5,#0xB]
     * ROM:0000445E                 ORR.W           LR, R2, R3,LSL#24
     * ROM:00004462                 RSB.W           R3, R0, R12
     * ROM:00004466                 SUBS            R3, #0xC
     * ROM:00004468                 CMP             LR, R3
     * ROM:0000446A                 BHI.W           out_einval
     *
     * Casting CERT tag pointer and validating it's length lies within the image
     */

    certTag = (Image3TagHeader *)&image->ihBuffer[certTagPosition];

    if (LETOH(certTag->itBufferLength) > imageBufferLength - certTagPosition - sizeof(Image3TagHeader)) {
        goto out_einval;
    }

    /*
     * ROM:0000446E                 LDRB            R3, [R5,#1]
     * ROM:00004470                 LDRB.W          R2, [R10,R0]
     * ROM:00004474                 ORR.W           R2, R2, R3,LSL#8
     * ROM:00004478                 LDRB            R3, [R5,#2]
     * ROM:0000447A                 ORR.W           R2, R2, R3,LSL#16
     * ROM:0000447E                 LDRB            R3, [R5,#3]
     * ROM:00004480                 ORR.W           R3, R2, R3,LSL#24
     * ROM:00004484                 LDR.W           R2, ='CERT'
     * ROM:00004488                 CMP             R3, R2
     * ROM:0000448A                 BNE.W           out_einval
     *
     * Checking that CERT tag's ident really is 'CERT'
     */

    if (LETOH(certTag->itTag) != 'CERT') {
        goto out_einval;
    }

    /*
     * ROM:0000448E                 ADD.W           R3, LR, #0xC
     * ROM:00004492                 ADDS            R3, R3, R0
     * ROM:00004494                 CMP             R12, R3
     * ROM:00004496                 BEQ             loc_44A6
     * ROM:00004498                 LSRS            R2, R3, #8
     * ROM:0000449A                 STRB            R3, [R6,#8]
     * ROM:0000449C                 STRB            R2, [R6,#9]
     * ROM:0000449E                 LSRS            R2, R3, #0x10
     * ROM:000044A0                 LSRS            R3, R3, #0x18
     * ROM:000044A2                 STRB            R2, [R6,#0xA]
     * ROM:000044A4                 STRB            R3, [R6,#0xB]
     *
     * If the end of CERT tag' content doesn't match with the end of the image contents,
     * we trim it to match with CERT tag
     */

    certTagContentEnd = certTagPosition + sizeof(Image3TagHeader) + LETOH(certTag->itBufferLength);

    if (imageBufferLength != certTagContentEnd) {
        image->ihBufferLength = HTOLE(certTagContentEnd);
    }

    /*
     * ROM:000044A6 loc_44A6                                ; CODE XREF: image3ValidateSignature+126↑j
     * ROM:000044A6                 ADDS            R1, #8
     * ROM:000044A8                 ADD             R2, SP, #0x20
     * ROM:000044AA                 ADD.W           R0, R6, #0xC
     * ROM:000044AE                 BL              image3SHA1Generate
     *
     * Generating SHA1 hash of the signed area + ident and ihSignedLength itself
     */

    image3SHA1Generate(&image->ihSignedLength, signedLength + 8, hash);

    /*
     * ROM:000044B2                 LDR             R2, [SP,#0x14]
     * ROM:000044B4                 TST.W           R2, #1
     * ROM:000044B8                 BEQ             loc_44E2
     *
     * Testing if kImage3ValidateLocalStorage flag was set in options
     */

    if (flags & kImage3ValidateLocalStorage) {
        /*
         * ROM:000044BA                 LDRB            R3, [R4,#9]
         * ROM:000044BC                 LDRB            R2, [R4,#8]
         * ROM:000044BE                 LDRB            R1, [R4,#0xB]
         * ROM:000044C0                 ORR.W           R2, R2, R3,LSL#8
         * ROM:000044C4                 LDRB            R3, [R4,#0xA]
         * ROM:000044C6                 ORR.W           R3, R2, R3,LSL#16
         * ROM:000044CA                 TST.W           R3, #0xF
         * ROM:000044CE                 ORR.W           R1, R3, R1,LSL#24
         * ROM:000044D2                 BNE             out_einval
         *
         * Testing if SHSH tag's content length is dividable by 16 (AES block size),
         * otherwise failing
         */

        if (LETOH(shshTag->itBufferLength) % 16) {
            goto out_einval;
        }

        /*
         * ROM:000044D4                 ADD.W           R3, R4, #0xC
         * ROM:000044D8                 STR             R3, [SP,#0x10]
         * ROM:000044DA                 MOV             R0, R3
         * ROM:000044DC                 BL              image3AESDecryptUsingLocalKey
         * ROM:000044E0                 B               loc_44E8
         *
         * Decrypting SHSH tag's buffer with UID-based key
         */

        shshTagBuffer = shshTag->itBuffer;

        image3AESDecryptUsingLocalKey(shshTagBuffer, LETOH(shshTag->itBufferLength));

    } else {
        /*
         * ROM:000044E2 loc_44E2                                ; CODE XREF: image3ValidateSignature+148↑j
         * ROM:000044E2                 ADD.W           R1, R4, #0xC
         * ROM:000044E6                 STR             R1, [SP,#0x10]
         *
         * Setting shshTagBuffer for the case when the image is not from local storage
         */

        shshTagBuffer = shshTag->itBuffer;
    }

    /*
     * ROM:000044E8 loc_44E8                                ; CODE XREF: image3ValidateSignature+170↑j
     * ROM:000044E8                 STR.W           R11, [SP,#0x1C]
     * ROM:000044EC                 STR.W           R11, [SP,#0x18]
     *
     * Zeroing nestedImage and nestedImageLength
     */

    nestedImage = NULL;
    nestedImageLength = 0;

    /*
     * ROM:000044F0                 LDRB            R2, [R4,#9]
     * ROM:000044F2                 LDRB            R3, [R4,#8]
     * ROM:000044F4                 ADD             R0, SP, #0x20
     * ROM:000044F6                 ORR.W           R3, R3, R2,LSL#8
     * ROM:000044FA                 LDRB            R2, [R4,#0xA]
     * ROM:000044FC                 ORR.W           R3, R3, R2,LSL#16
     * ROM:00004500                 LDRB            R2, [R4,#0xB]
     * ROM:00004502                 ORR.W           R3, R3, R2,LSL#24
     * ROM:00004506                 ADD.W           R2, R5, #0xC
     * ROM:0000450A                 STR             R2, [SP]
     * ROM:0000450C                 LDRB            R2, [R5,#9]
     * ROM:0000450E                 LDRB            R1, [R5,#8]
     * ROM:00004510                 ORR.W           R1, R1, R2,LSL#8
     * ROM:00004514                 LDRB            R2, [R5,#0xA]
     * ROM:00004516                 ORR.W           R1, R1, R2,LSL#16
     * ROM:0000451A                 LDRB            R2, [R5,#0xB]
     * ROM:0000451C                 ORR.W           R2, R1, R2,LSL#24
     * ROM:00004520                 MOVS            R1, #0x14
     * ROM:00004522                 STR             R2, [SP,#0x4]
     * ROM:00004524                 ADD             R2, SP, #0x1C
     * ROM:00004526                 STR             R2, [SP,#0x8]
     * ROM:00004528                 ADD             R2, SP, #0x18
     * ROM:0000452A                 STR             R2, [SP,#0xC]
     * ROM:0000452C                 LDR             R2, [SP,#0x10]
     * ROM:0000452E                 BL              image3PKIVerifyHash
     *
     * Calling PKI verification function
     */

    result = image3PKIVerifyHash(
        hash,
        SHA1_HASH_SIZE,
        shshTagBuffer,
        LETOH(shshTag->itBufferLength),
        certTag->itBuffer,
        LETOH(certTag->itBufferLength),
        &nestedImage,
        &nestedImageLength
    );

    /*
     * ROM:00004532                 LDRB            R3, [R4,#9]
     * ROM:00004534                 LDRB            R2, [R4,#8]
     * ROM:00004536                 MOV             R1, R11
     * ROM:00004538                 ORR.W           R2, R2, R3,LSL#8
     * ROM:0000453C                 LDRB            R3, [R4,#0xA]
     * ROM:0000453E                 ORR.W           R2, R2, R3,LSL#16
     * ROM:00004542                 LDRB            R3, [R4,#0xB]
     * ROM:00004544                 ORR.W           R2, R2, R3,LSL#24
     * ROM:00004548                 MOV             R5, R0
     * ROM:0000454A                 LDR             R0, [SP,#0x10]
     * ROM:0000454C                 BLX             memset
     *
     * Clearing SHSH tag's contents
     */

    memset(shshTagBuffer, 0x0, LETOH(shshTag->itBufferLength));

    /*
     * ROM:00004550                 CMP             R5, #0
     * ROM:00004552                 BNE             out
     *
     * If PKI didn't succeed, returning its' error to the caller
     */

    if (result) {
        goto out;
    }

    /*
     * ROM:00004554                 LDR             R1, [SP,#0x1C]
     * ROM:00004556                 CBZ             R1, loc_4576
     *
     * Checking if PKI returned nested image
     */

    if (nestedImage) {
        /*
         * ROM:00004558                 ADD.W           R0, R8, #0xC
         * ROM:0000455C                 LDR             R2, [SP,#0x18]
         * ROM:0000455E                 MOVS            R3, #1
         * ROM:00004560                 BL              image3InstantiateFromBuffer
         *
         * Calling image3InstantiateFromBuffer() against the nested image
         */

        result = image3InstantiateFromBuffer(&handle->nestedImage, nestedImage, nestedImageLength, true);

        /*
         * ROM:00004564                 CBZ             R0, loc_456A
         * ROM:00004566                 MOV             R5, R0
         * ROM:00004568                 B               out
         *
         * If instantiation failed, return an error to the caller
         */

        if (result) {
            goto out;
        }

        /*
         * ROM:0000456A loc_456A                                ; CODE XREF: image3ValidateSignature+1F4↑j
         * ROM:0000456A                 LDR.W           R2, [R8,#0xC]
         * ROM:0000456E                 LDR             R1, =0x20006
         * ROM:00004570                 LDR             R3, [R2,#4]
         * ROM:00004572                 ORRS            R3, R1
         * ROM:00004574                 STR             R3, [R2,#4]
         *
         * Setting flags to the nested image handle
         */

        handle->nestedImage->flags |= (kImage3ImageWasValidated | kImage3ImageIsTrusted | kImage3ImageIsSigned);
    }

    /*
     * ROM:00004576 loc_4576                                ; CODE XREF: image3ValidateSignature+1E6↑j
     * ROM:00004576                 LDR.W           R3, [R8,#4]
     * ROM:0000457A                 LDR.W           R2, [R8,#8]
     * ROM:0000457E                 MOVS            R1, #0
     * ROM:00004580                 ORR.W           R3, R3, #4
     * ROM:00004584                 STR.W           R3, [R8,#4]
     * ROM:00004588                 LDRB            R3, [R6,#0xD]
     * ROM:0000458A                 LDRB            R0, [R6,#0xC]
     * ROM:0000458C                 ADD             R2, R6
     * ROM:0000458E                 ORR.W           R0, R0, R3,LSL#8
     * ROM:00004592                 LDRB            R3, [R6,#0xE]
     * ROM:00004594                 ORR.W           R0, R0, R3,LSL#16
     * ROM:00004598                 LDRB            R3, [R6,#0xF]
     * ROM:0000459A                 ORR.W           R0, R0, R3,LSL#24
     * ROM:0000459E                 ADD             R0, R10
     * ROM:000045A0                 SUBS            R2, R2, R0
     * ROM:000045A2                 BLX             memset
     *
     * Zeroing out the image footer (SHSH+CERT) and setting trusted flag to the handle
     */

    handle->flags |= kImage3ImageIsTrusted;

    memset((void *)shshTag, 0x0, handle->allocSize + (void *)image - (void *)shshTag);

    /*
     * ROM:000045A6                 LDRB            R3, [R6,#0xD]
     * ROM:000045A8                 LDRB            R2, [R6,#0xC]
     * ROM:000045AA                 ORR.W           R2, R2, R3,LSL#8
     * ROM:000045AE                 LDRB            R3, [R6,#0xE]
     * ROM:000045B0                 ORR.W           R2, R2, R3,LSL#16
     * ROM:000045B4                 LDRB            R3, [R6,#0xF]
     * ROM:000045B6                 STRB            R2, [R6,#8]
     * ROM:000045B8                 ORR.W           R3, R2, R3,LSL#24
     * ROM:000045BC                 LSRS            R2, R3, #8
     * ROM:000045BE                 STRB            R2, [R6,#9]
     * ROM:000045C0                 LSRS            R2, R3, #0x10
     * ROM:000045C2                 LSRS            R3, R3, #0x18
     * ROM:000045C4                 STRB            R2, [R6,#0xA]
     * ROM:000045C6                 STRB            R3, [R6,#0xB]
     * ROM:000045C8                 B               out
     *
     * Setting image's ihBufferLength to its' own iHSignedLength,
     * effectively cutting the footer (SHSH + CERT), and exiting
     */

    image->ihBufferLength = HTOLE(LETOH(image->ihSignedLength));

    goto out;

out_einval:
    /*
     * ROM:000045CA out_einval                              ; CODE XREF: image3ValidateSignature+52↑j
     * ROM:000045CA                                         ; image3ValidateSignature+70↑j ...
     * ROM:000045CA                 MOVS            R5, #0x16
     *
     * Setting return value to EINVAL
     */
    
    result = EINVAL;

out:
    /*
     * ROM:000045CC out                                     ; CODE XREF: image3ValidateSignature+26↑j
     * ROM:000045CC                                         ; image3ValidateSignature+36↑j ...
     * ROM:000045CC                 LDR.W           R1, =0x10060008
     * ROM:000045D0                 LDR             R2, [SP,#0x34]
     * ROM:000045D2                 MOV             R0, R5
     * ROM:000045D4                 LDR             R3, [R1]
     * ROM:000045D6                 CMP             R2, R3
     * ROM:000045D8                 BEQ             ret
     * ROM:000045DA                 BL              stack_chk_fail
     *
     * Checking the stack cookie against reference one,
     * panicking if check failed, handing result to R0
     */
ret:
    /*
     * ROM:000045DE ret                                     ; CODE XREF: image3ValidateSignature+268↑j
     * ROM:000045DE                 SUB.W           SP, R7, #0x18
     * ROM:000045E2                 POP.W           {R8,R10,R11}
     * ROM:000045E6                 POP             {R4-R7,PC}
     *
     * Setting stack pointer back, popping saved registers,
     * returning to a caller
     */

    return result;
}
