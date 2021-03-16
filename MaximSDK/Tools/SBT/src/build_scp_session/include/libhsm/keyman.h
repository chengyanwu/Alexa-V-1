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


#ifndef __KEYMAN_H__
#define __KEYMAN_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <libhsm/hsm.h>

/**
  * @brief Find a Key object according to the LABEL
  * @param keyname name of the key to find
  * @param obj object handler for the found key
  * @retval status
  */
uint_fast32_t hsm_key_find(const char * keyName, uint8_t priv , hsm_object_t * obj);


/**
 * Get the type of the provided key
 * @param obj key to get the type
 * @param type key type
 * @return status
 */
uint_fast32_t hsm_key_gettype(hsm_object_t obj, hsm_keytype_t * type);


/**
 * List all available key
 * @return
 */
uint_fast32_t hsm_key_list(void);



/**
 * HMAC
 */

/**
  * @brief Generate a HMAC SHA256 key inside the HSM ( Cannot be exported )
  * @param keyname name of the key to generate
  * @retval rv
  *
  */
uint_fast32_t hsm_hmac_sha256_genkey(const char * keyName);


/**
  * @brief Import a HMAC SHA256 key inside the HSM ( need to modify CKNFAST_OVERRIDE_SECURITY_ASSURANCES=import only during this step )
  * @param keyvalue the key
  * @param label name of the key to import
  * @param objkey return an object to the imported key
  * @retval rv
  *
  */
uint_fast32_t hsm_hmac_sha256_import_key(uint8_t * keyvalue, const char * label, hsm_object_t * objkey);


/**
 * DES3
 */

/**
 * Generate a Triple DES key
 * @param keyName Name of the  key that will be generated
 * @return status
 */
uint_fast32_t hsm_des3_genkey(const char * keyName);



/**
 * Wrap a key using triple DES
 * @param hWrappingKey Triple DES key that will be used to wrap
 * @param hKey key to be wrapped
 * @param pWrappedKey output buffer for the wrapped key
 * @param pulWrappedKeyLen length of the wrapped key
 * @return status
 */
uint_fast32_t hsm_des3_wrap_key(hsm_object_t  hWrappingKey, hsm_object_t  hKey, uint8_t * pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen );


/**
 * Export an EC public key
 * @param obj key to export
 * @param Qx X coordinate
 * @param Qy Y coordinate
 * @param Q_len coordinate length
 * @param curve_id id of the curve
 * @return status
 */
uint_fast32_t hsm_ec_export_publickey(hsm_object_t obj,	uint8_t * Qx, uint8_t * Qy, size_t * Q_len, ucl_ec_curve_t * curve_id);



/**
 * Import an EC public key
 * @param ecparams	Curve parameters ( OID )
 * @param ecparams_len length of the curve parameters
 * @param ecpoint EC point
 * @param ecpoint_len length of the EC point
 * @param label name for the imported key
 * @param objkey handle for the newly imported key
 * @return status
 */
uint_fast32_t hsm_ec_import_publickey(uint8_t * ecparams, size_t ecparams_len,
														uint8_t * ecpoint, size_t ecpoint_len,
														const char * label,
														hsm_object_t * objkey);

/**
 * Import an EC private key
 * @param ecparams	Curve parameters ( OID )
 * @param ecparams_len length of the curve parameters
 * @param ecvalue d
 * @param ecvalue_len length of d
 * @param label name for the imported key
 * @param objkey handle for the newly imported key
 * @return status
 */
uint_fast32_t hsm_ec_import_privatekey(uint8_t * ecparams, size_t ecparams_len,
														uint8_t * ecvalue, size_t ecvalue_len,
														const char * label,
														hsm_object_t * objkey);

/**
 * Generate an EC keypair
 * @param keyName name for the generated key
 * @param curve_id EC curve
 * @return status
 */
uint_fast32_t hsm_ec_genkey(const char * keyName, ucl_ec_curve_t curve_id);


/**
 * RSA
 */

/**
 * Get the public part of a RSA key
 * @param obj Key to get the public part
 * @param pModulus buffer for the modulus
 * @param pModulus_len length of the modulus
 * @param pExponent buffer for the public exponent
 * @param pExponent_len length of the public exponent
 * @return
 */
uint_fast32_t hsm_rsa_get_publickey(hsm_object_t obj,
		uint8_t * pModulus,
		size_t * pModulus_len,
		uint8_t * pExponent, size_t * pExponent_len);

/**
 * Get the length of the modulus of a RSA key
 * @param obj Key to get the length
 * @return the modulus length
 */
uint_fast32_t hsm_rsa_get_modulus_len(hsm_object_t obj);


/**
 * Import an RSA key
 * @param objkey return an handler to the imported key
 * @param modulusvalue buffer with the modulus
 * @param modulus_len length of the modulus
 * @param public_exponent buffer with the public exponent
 * @param pub_exp_len length of the public exponent
 * @param label Name of the key that will be imported
 * @return
 */
uint_fast32_t hsm_rsa_import_key(hsm_object_t * objkey,
		uint8_t * modulusvalue, size_t modulus_len,
		uint8_t * public_exponent, size_t pub_exp_len, const char * label);


/**
 *	Generate an RSA key
 * @param keyName Name of the key that will be generated
 * @param key_size SIze of the key
 * @return status
 */
uint_fast32_t hsm_rsa_genkey(const char * keyName, uint32_t key_size);



/**
 * Unwrap a Key using rsa oaep algorithm
 * @param hWrappingKey key used to unwrap
 * @param hKey return an handler to the unwrapped key
 * @param pWrappedKey Wrapped key material
 * @param pulWrappedKeyLen length of the Wrapped key material
 * @param label Name of the will that will be unwrapped
 * @return status
 */
uint_fast32_t hsm_rsa_oaep_unwrap_key(hsm_object_t  hWrappingKey,
							hsm_object_t *  hKey,
							uint8_t * pWrappedKey,
							size_t pulWrappedKeyLen,
							const char * label);



/**
 * Wrap a Key using rsa oaep algorithm
 * @param hWrappingKey key used to wrap
 * @param hKey key to be wrapped
 * @param pWrappedKey output buffer for the wrapped key
 * @param pulWrappedKeyLen length of the wrapped key
 * @return status
 */
uint_fast32_t hsm_rsa_oaep_wrap_key(hsm_object_t  hWrappingKey,
		hsm_object_t  hKey,
		uint8_t * pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen );
//		uint8_t * pWrappedKey, size_t * pulWrappedKeyLen );



/**
 * AES
 */

/**
 * UnWrap a Key using AES CBC algorithm
 * @param hWrappingKey key used to wrap
 * @param pIV buffer for the AES CBC initialization vector
 * @param hKey return an handler to the unwrapped key
 * @param pWrappedKey input buffer for the wrapped key
 * @param pulWrappedKeyLen length of the wrapped key
 * @param label Name of the will that will be unwrapped
 * @return status
 */
uint_fast32_t hsm_aes_unwrap_key(hsm_object_t  hWrappingKey,
							uint8_t * pIV,
							hsm_object_t * hKey,
							uint8_t * pWrappedKey,
							size_t pulWrappedKeyLen,
							const char * label);

/**
 *
 * @param keyvalue
 * @param key_length
 * @param label
 * @return
 */
uint_fast32_t hsm_aes_import_key(uint8_t * keyvalue,size_t key_length, const char * label);


/**
 * Wrap a Key using AES CBC algorithm
 * @param hWrappingKey key used to wrap
 * @param pIv buffer for the AES CBC initialization vector
 * @param hKey key to be wrapped
 * @param pWrappedKey output buffer for the wrapped key
 * @param pulWrappedKeyLen length of the wrapped key
 * @return status
 */
uint_fast32_t hsm_aes_wrap_key(hsm_object_t  hWrappingKey,
		uint8_t * pIv,
		hsm_object_t  hKey,
		uint8_t * pWrappedKey, CK_ULONG_PTR pulWrappedKeyLen );


/**
 * Generate an AES key
 * @param keyName name of the key that will be generated
 * @param key_len length of the key to generate ( 128, 192, 256 )
 * @return status
 */
uint_fast32_t hsm_aes_genkey(const char * keyName, uint32_t key_len);



#ifdef __cplusplus
}
#endif

#endif /* __KEYMAN_H__ */
