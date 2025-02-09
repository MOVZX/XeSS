/*******************************************************************************
 * Copyright (C) 2021 Intel Corporation
 * 
 * This software and the related documents are Intel copyrighted materials, and
 * your use of them is governed by the express license under which they were
 * provided to you ("License"). Unless the License provides otherwise, you may
 * not use, modify, copy, publish, distribute, disclose or transmit this
 * software or the related documents without Intel's prior written permission.
 * 
 * This software and the related documents are provided as is, with no express
 * or implied warranties, other than those that are expressly stated in the
 * License.
 ******************************************************************************/

#ifndef XESS_H
#define XESS_H

#ifdef XESS_SHARED_LIB
#ifdef _WIN32
#ifdef XESS_EXPORT_API
#define XESS_API __declspec(dllexport)
#else
#define XESS_API __declspec(dllimport)
#endif
#else
#define XESS_API __attribute__((visibility("default")))
#endif
#else
#define XESS_API
#endif

#if !defined _MSC_VER || (_MSC_VER >= 1928)
#define XESS_PRAGMA(x) _Pragma(#x)
#else
#define XESS_PRAGMA(x) __pragma(x)
#endif
#define XESS_PACK_B_X(x) XESS_PRAGMA(pack(push, x))
#define XESS_PACK_E() XESS_PRAGMA(pack(pop))
#define XESS_PACK_B() XESS_PACK_B_X(8)

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _xess_context_handle_t* xess_context_handle_t;

XESS_PACK_B()
/**
 * @brief X<sup>e</sup>SS version.
 * 
 * X<sup>e</sup>SS uses major.minor.patch version format and Numeric 90+ scheme for development stage builds.
 */
typedef struct _xess_version_t
{
    /** A major version increment indicates a new API and potentially a
     * break in functionality. */
    uint16_t major;
    /** A minor version increment indicates incremental changes such as
     * optional inputs or flags. This does not break existing functionality. */
    uint16_t minor;
    /** A patch version increment may include performance or quality tweaks or fixes for known issues.
     * There's no change in the interfaces.
     * Versions beyond 90 used for development builds to change the interface for the next release.
     */
    uint16_t patch;
    /** Reserved for future use. */
    uint16_t reserved;
} xess_version_t;
XESS_PACK_E()

XESS_PACK_B()
/**
 * @brief 2D variable.
 */
typedef struct _xess_2d_t
{
    uint32_t x;
    uint32_t y;
} xess_2d_t;
XESS_PACK_E()

/**
 * @brief 2D coordinates.
 */
typedef xess_2d_t xess_coord_t;

/**
 * @brief X<sup>e</sup>SS quality settings.
 */
typedef enum _xess_quality_settings_t
{
    XESS_QUALITY_SETTING_PERFORMANCE = 101,
    XESS_QUALITY_SETTING_BALANCED = 102,
    XESS_QUALITY_SETTING_QUALITY = 103,
    XESS_QUALITY_SETTING_ULTRA_QUALITY = 104,
} xess_quality_settings_t;

/**
 * @brief X<sup>e</sup>SS initialization flags.
 */
typedef enum _xess_init_flags_t
{
    XESS_INIT_FLAG_NONE = 0,
    /** Use motion vectors at target resolution. */
    XESS_INIT_FLAG_HIGH_RES_MV = 1 << 0,
    /** Use inverted (increased precision) depth encoding */
    XESS_INIT_FLAG_INVERTED_DEPTH = 1 << 1,
    /** Use exposure texture to scale input color. */
    XESS_INIT_FLAG_EXPOSURE_SCALE_TEXTURE = 1 << 2,
    /** Use responsive pixel mask texture. */
    XESS_INIT_FLAG_RESPONSIVE_PIXEL_MASK = 1 << 3,
    /** Use velocity in NDC */
    XESS_INIT_FLAG_USE_NDC_VELOCITY = 1 << 4,
    /** Use external descriptor heap */
    XESS_INIT_FLAG_EXTERNAL_DESCRIPTOR_HEAP = 1 << 5,
    /** Disable tonemapping for input and output */
    XESS_INIT_FLAG_LDR_INPUT_COLOR = 1 << 6,
    /** Remove jitter from input velocity*/
    XESS_INIT_FLAG_JITTERED_MV = 1 << 7,
} xess_init_flags_t;

XESS_PACK_B()
/**
 * @brief Properties for internal X<sup>e</sup>SS resources.
 */
typedef struct _xess_properties_t
{
    /** Required amount of descriptors for X<sup>e</sup>SS */
    uint32_t requiredDescriptorCount;
    /** The heap size required by X<sup>e</sup>SS for temporary buffer storage. */
    uint64_t tempBufferHeapSize;
    /** The heap size required by X<sup>e</sup>SS for temporary texture storage. */
    uint64_t tempTextureHeapSize;
} xess_properties_t;
XESS_PACK_E()

/**
 * @brief  X<sup>e</sup>SS return codes.
 */
typedef enum _xess_result_t
{
    /** Warning. Folder to store dump data doesn't exist. Write operation skipped.*/
    XESS_RESULT_WARNING_NONEXISTING_FOLDER = 1,
    /** X<sup>e</sup>SS operation was successful. */
    XESS_RESULT_SUCCESS = 0,
    /** X<sup>e</sup>SS not supported on the GPU. An SM 6.4 capable GPU is required. */
    XESS_RESULT_ERROR_UNSUPPORTED_DEVICE = -1,
    /** An outdated driver. */
    XESS_RESULT_ERROR_UNSUPPORTED_DRIVER = -2,
    /** Execute called without initialization. */
    XESS_RESULT_ERROR_UNINITIALIZED = -3,
    /** Invalid argument such as descriptor handles. */
    XESS_RESULT_ERROR_INVALID_ARGUMENT = -4,
    /** Not enough available GPU memory. */
    XESS_RESULT_ERROR_DEVICE_OUT_OF_MEMORY = -5,
    /** Device function such as resource or descriptor creation. */
    XESS_RESULT_ERROR_DEVICE = -6,
    /** The function is not implemented */
    XESS_RESULT_ERROR_NOT_IMPLEMENTED = -7,
    /** Invalid context. */
    XESS_RESULT_ERROR_INVALID_CONTEXT = -8,
    /** Operation not finished yet. */
    XESS_RESULT_ERROR_OPERATION_IN_PROGRESS = -9,
    /** Operation not supported in current configuration. */
    XESS_RESULT_ERROR_UNSUPPORTED = -10,
    /** The library cannot be loaded. */
    XESS_RESULT_ERROR_CANT_LOAD_LIBRARY = -11,
    /** Unknown internal failure */
    XESS_RESULT_ERROR_UNKNOWN = -1000,
} xess_result_t;

/**
 * @brief X<sup>e</sup>SS logging level
 */
typedef enum _xess_logging_level_t
{
    XESS_LOGGING_LEVEL_DEBUG = 0,
    XESS_LOGGING_LEVEL_INFO = 1,
    XESS_LOGGING_LEVEL_WARNING = 2,
    XESS_LOGGING_LEVEL_ERROR = 3
} xess_logging_level_t;

/**
 * A logging callback provided by the application. This callback can be called from other threads.
 * Message pointer are only valid inside function and may be invalid right after return call.
 * Message is a null-terminated utf-8 string
 */
 typedef void (*xess_app_log_callback_t)(const char *message, xess_logging_level_t loging_level);

#ifndef XESS_TYPES_ONLY

/** @addtogroup xess XeSS API exports
 * @{
 */
  
/**
 * @brief Gets the X<sup>e</sup>SS version. This is baked into the X<sup>e</sup>SS SDK release.
 * @param[out] pVersion Returned X<sup>e</sup>SS version.
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessGetVersion(xess_version_t* pVersion);

/**
 * @brief Gets the version of the loaded Intel XeFX library. When running on Intel platforms 
 * this function will return the version of the loaded Intel XeFX library, for other 
 * platforms 0.0.0 will be returned.
 * @param hContext The X<sup>e</sup>SS context handle.
 * @param[out] pVersion Returned Intel XeFX library version.
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessGetIntelXeFXVersion(xess_context_handle_t hContext, 
    xess_version_t* pVersion);

/**
 * @brief Gets X<sup>e</sup>SS internal resources properties.
 * @param hContext The X<sup>e</sup>SS context handle.
 * @param pOutputResolution Output resolution to calculate properties for.
 * @param[out] pBindingPropeties Retured properties.
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessGetProperties(xess_context_handle_t hContext,
    const xess_2d_t* pOutputResolution, xess_properties_t* pBindingProperties);

/**
 * @brief Get the input resolution for a specified output resolution for a given quality setting.
 * X<sup>e</sup>SS expects all the input buffers except motion vectors to be in the returned resolution.
 * Motion vectors can be either in output resolution (XESS_INIT_FLAG_HIGH_RES_MV) or
 * returned resolution (default).
 *
 * @param hContext: The X<sup>e</sup>SS context handle.
 * @param pOutputResolution Output resolution to calculate input resolution for.
 * @param qualitySettings Desired quality setting.
 * @param[out] pInputResolution Required input resolution.
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessGetInputResolution(xess_context_handle_t hContext,
    const xess_2d_t* pOutputResolution, xess_quality_settings_t qualitySettings,
    xess_2d_t* pInputResolution);

/**
 * @brief Destroys the X<sup>e</sup>SS context.
 * The user must ensure that any pending command lists are completed before destroying the context.
 * @param hContext: The X<sup>e</sup>SS context handle.
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessDestroyContext(xess_context_handle_t hContext);

/**
 * Sets jitter scale value
 * @param hContext The X<sup>e</sup>SS context handle.
 * @param x scale for the X axis
 * @param y scale for the Y axis
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessSetJitterScale(xess_context_handle_t hContext, float x, float y);

/**
 * Sets velocity scale value
 * @param hContext The X<sup>e</sup>SS context handle.
 * @param x scale for the X axis
 * @param y scale for the Y axis
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessSetVelocityScale(xess_context_handle_t hContext, float x, float y);

/**
 * Sets logging callback
 *
 * @param hContext The X<sup>e</sup>SS context handle.
 * @param loggingLevel Minimum logging level for logging callback.
 * @param loggingCallback Logging callback
 * @return X<sup>e</sup>SS return status code.
 */
XESS_API xess_result_t xessSetLoggingCallback(xess_context_handle_t hContext,
    xess_logging_level_t loggingLevel, xess_app_log_callback_t loggingCallback);

/** @}*/
  
#endif

// Enum size checks. All enums must be 4 bytes
typedef char sizecheck__LINE__[ (sizeof(xess_quality_settings_t) == 4) ? 1 : -1];
typedef char sizecheck__LINE__[ (sizeof(xess_init_flags_t) == 4) ? 1 : -1];
typedef char sizecheck__LINE__[ (sizeof(xess_result_t) == 4) ? 1 : -1];
typedef char sizecheck__LINE__[ (sizeof(xess_logging_level_t) == 4) ? 1 : -1];


#ifdef __cplusplus
}
#endif

#endif
