/******************************************************************************
*                                                                             *
*                      SOFTING Industrial Automation GmbH                     *
*                          Richard-Reitzner-Allee 6                           *
*                                D-85540 Haar                                 *
*                        Phone: (++49)-(0)89-45656-0                          *
*                        Fax:   (++49)-(0)89-45656-399                        *
*                                                                             *
*            Copyright (C) SOFTING Industrial Automation GmbH 1995-2012       *
*                              All Rights Reserved                            *
*                                                                             *
*******************************************************************************

FILE_NAME               FDLGDL.C

PROJECT_NAME            PROFIBUS

MODULE                  FDLGDL

COMPONENT_LIBRARY       PAPI Lib
                        PAPI DLL

AUTHOR                  SOFTING

VERSION                 5.23.0.00.release

DATE                    October-2000

STATUS                  finished

FUNCTIONAL_MODULE_DESCRIPTION

This modul contains the FDL(IF)-Service-Specific-Functions which return the
length
length of the Request- or Response-Datas.


RELATED_DOCUMENTS
=============================================================================*/
#include "keywords.h"

INCLUDES

#if defined(WIN16) || defined(WIN32)
#include <windows.h>
#endif

#include "pb_type.h"
#include "pb_if.h"
#include "pb_err.h"
#include "pb_fdl.h"

GLOBAL_DEFINES

LOCAL_DEFINES

EXPORT_TYPEDEFS

LOCAL_TYPEDEFS

FUNCTION_DECLARATIONS

EXPORT_DATA

IMPORT_DATA

LOCAL_DATA

#if defined(WIN32) || defined(_WIN32) || defined(WIN16) || defined(_WIN16)
#pragma check_stack(off)
#endif

FUNCTION PUBLIC INT16 fdlgdl_get_data_len(
    IN INT16 result,         /* Service-Result */
    IN USIGN8 service,       /* Service */
    IN USIGN8 primitive,     /* Service-Primitive */
    IN USIGN8 FAR* data_ptr, /* pointer to data */
    OUT INT16* data_len_ptr  /* length of data */
    )

/*-----------------------------------------------------------------------------
FUNCTIONAL_DESCRIPTION

this function is used to return the data length of FDL-SERVICES

possible return values:
- Data-length

-----------------------------------------------------------------------------*/
{
  LOCAL_VARIABLES

  FUNCTION_BODY

  *data_len_ptr = 0;
  result = E_OK;

  if (primitive == REQ)
  {
    switch (service)
    {
    case FDLIF_SDA:
    case FDLIF_SDN:
    case FDLIF_SRD:
    {
      T_FDLIF_SDN_SDA_SRD_REQ FAR* req = (T_FDLIF_SDN_SDA_SRD_REQ FAR*)data_ptr;
      *data_len_ptr = sizeof(T_FDLIF_SDN_SDA_SRD_REQ) + req->length;
    }
    break;

    case FDLIF_REPLY_UPDATE:
    case FDLIF_REPLY_UPDATE_MULTIPLE:
    {
      T_FDLIF_RUP_REQ FAR* req = (T_FDLIF_RUP_REQ FAR*)data_ptr;
      *data_len_ptr = sizeof(T_FDLIF_RUP_REQ) + req->length;
    }
    break;

    case FDLIF_SET_BUSPARAMETER:
      *data_len_ptr = sizeof(T_FDLIF_SET_BUSPARAMETER_REQ);
      break;

    case FDLIF_READ_BUSPARAMETER:
      *data_len_ptr = 0;
      break;

    case FDLIF_SAP_ACTIVATE:
      *data_len_ptr = sizeof(T_FDLIF_SAP_ACTIVATE_REQ);
      break;

    case FDLIF_RSAP_ACTIVATE:
      *data_len_ptr = sizeof(T_FDLIF_RSAP_ACTIVATE_REQ);
      break;

    case FDLIF_SAP_CHANGE_ACCESS:
      *data_len_ptr = sizeof(T_FDLIF_SAP_CHANGE_REQ);
      break;

    case FDLIF_SAP_DEACTIVATE:
      *data_len_ptr = sizeof(T_FDLIF_SAP_DEACTIVATE_REQ);
      break;

    case FDLIF_EXIT:
      *data_len_ptr = 0;
      break;

    default:
      *data_len_ptr = 0;
      result = E_IF_INVALID_SERVICE;
      break;
    }
  }
  else
  {
    result = E_IF_INVALID_PRIMITIVE;
  }

  return (result);
}

#if defined(WIN32) || defined(_WIN32) || defined(WIN16) || defined(_WIN16)
#pragma check_stack
#endif
