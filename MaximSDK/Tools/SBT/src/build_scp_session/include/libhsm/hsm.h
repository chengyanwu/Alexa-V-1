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

#ifndef __LIB_HSM_H__
#define __LIB_HSM_H__

#include <stdint.h>

#define STRINGIZER(arg)     #arg
#define STR_VALUE(arg)      STRINGIZER(arg)

#define LIBHSM_VERS_MAJOR		2
#define LIBHSM_VERS_MINOR		1
#define LIBHSM_VERS_PATCH		0
#define LIBHSM_VERSION			STR_VALUE(LIBHSM_VERS_MAJOR)"."STR_VALUE(LIBHSM_VERS_MINOR)"."STR_VALUE(LIBHSM_VERS_PATCH)


#define LIBHSM_MAX_SLOTS		10

#include <pkcs11/cryptoki.h> // For CK_OBJECT_HANDLE
typedef CK_OBJECT_HANDLE         hsm_object_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ucl_ec_curve_e{
	UCL_EC_CURVE_SECP192R1,
	UCL_EC_CURVE_SECP224R1,
	UCL_EC_CURVE_SECP256R1,
	UCL_EC_CURVE_SECP384R1,
	UCL_EC_CURVE_SECP521R1,
	UCL_EC_CURVE_SECP192K1,
	UCL_EC_CURVE_SECP224K1,
	UCL_EC_CURVE_SECP256K1,
	UCL_EC_CURVE_BP256R1,
	UCL_EC_CURVE_BP384R1,
	UCL_EC_CURVE_BP512R1,
	UCL_EC_CURVE_MAX,
	UCL_EC_CURVE_UNKNOWN = UCL_EC_CURVE_MAX,
}ucl_ec_curve_t;

typedef enum hsm_keytype_t{
	HSM_KEYTYPE_RSA,
	HSM_KEYTYPE_ECDSA,
	HSM_KEYTYPE_AES,
	HSM_KEYTYPE_DES3,
	HSM_KEYTYPE_GENERIC_SECRET,
}hsm_keytype_t;

#include "cryptoop.h"
#include "keyman.h"

/**
 * Get the version of the library
 * @return the version of the library
 */
char * hsm_get_version(void);


/**
 * Get the date when the library was built
 * @return the date when the library was built
 */
char *  hsm_get_build_date(void);


/**
 * Get the version of the pkcs dll
 */
void  hsm_get_dll_info(void);


/**
  * Perform Token login in order to open a session
  * @param slotID ID of the smartcard slot used to insert OCS card, can be determined with ListSlot
  * @retval rv
  */
uint_fast32_t  hsm_login(uint32_t slotID);


/**
  * Initialise DLL, giving the path of Thales cknfast DLL
  * @param path path of Thales cknfast DLL
  * @retval rv
  */
uint_fast32_t hsm_init_dll(char * path);


/**
  * Close the HSM Session
  * @retval rv
  */
uint_fast32_t  hsm_close(void);


/**
  * List All Slot Available on stdout
  * @retval rv
  */
uint_fast32_t hsm_slot_list(void);


/**
  * Print a Human explicit meaning of error code
  * @param rv
  * @retval rv
  */
uint_fast32_t  HSM_pError(uint_fast32_t rv);




#ifdef __cplusplus
}
#endif

#endif /* __LIB_HSM_H__ */
