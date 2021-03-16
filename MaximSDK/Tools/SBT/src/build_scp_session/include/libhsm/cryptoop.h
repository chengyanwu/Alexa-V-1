/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
*
* This software is protected by copyright laws of the United States and
* of foreign countries. This material may also be protected by patent laws
* and technology transfer regulations of the United States and of foreign
* countries. This software is furnished under a license agreement and/or a
* nondisclosure agreement and may only be used or reproduced in accordance
* with the terms of those agreements. Dissemination of this information to
* any party or parties not specified in the license agreement and/or
* nondisclosure agreement is expressly prohibited.
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*******************************************************************************
*/

#ifndef __CRYPTOOP_H__
#define __CRYPTOOP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "hsm.h"

/**
  * @brief Sign Data with SHA-256 HMAC Algorithm
  * @param data  pointer to data to sign
  * @param data_len length of the data to sign
  * @param sig pointer of a buffer for the resulting signature
  * @param siglen length of the sig buffer, return the size of the signature, should be 32
  * @param objkey Object handle of the key to use
  * @retval rv
  *
  */
uint_fast32_t hsm_hmac_sha256_sign(uint8_t * data, CK_ULONG data_len, uint8_t * sig, CK_ULONG_PTR siglen, hsm_object_t objkey);

/**
 * Sign data with ECDSA Algorithm
 * @param data pointer to data to sign
 * @param data_len length of the data to sign
 * @param sig pointer of a buffer for the resulting signature
 * @param siglen length of the sig buffer, return the size of the signature
 * @param objkey Object handle of the key to use
 * @return return value
 */
uint_fast32_t hsm_ecdsa_sign(uint8_t * data, CK_ULONG data_len, uint8_t * sig, CK_ULONG_PTR siglen, hsm_object_t objkey);


/**
 * Verify the ECDSA Signature of the data
 * @param data buffer with the data to verify the signature
 * @param data_len length of the data
 * @param sig buffer with the signature to verify
 * @param siglen length of the signature
 * @param objkey EC key to verify the signature
 * @return result
 */
uint_fast32_t hsm_ecdsa_verify(uint8_t * data, CK_ULONG data_len, uint8_t * sig, CK_ULONG siglen, hsm_object_t objkey);


/**
 * Cipher data using RSA
 * @param data_in input buffer with plain data to cipher
 * @param data_in_len length of plain data
 * @param data_out output buffer for ciphered data
 * @param data_out_len length of ciphered data
 * @param objkey key used to cipher
 * @return status
 */
uint_fast32_t hsm_rsa_cipher(uint8_t * data_in, CK_ULONG data_in_len, uint8_t * data_out, CK_ULONG_PTR data_out_len, hsm_object_t objkey);

/**
 * Uncipher data using RSA
 * @param data_in input buffer with ciphered data
 * @param data_in_len length of ciphered data
 * @param data_out output buffer for plain data
 * @param data_out_len length of plain data
 * @param objkey key used to uncipher
 * @return status
 */
uint_fast32_t hsm_rsa_uncipher(uint8_t * data_in, CK_ULONG data_in_len, uint8_t * data_out, CK_ULONG_PTR data_out_len, hsm_object_t objkey);


#ifdef __cplusplus
}
#endif


#endif /* __CRYPTOOP_H__ */
