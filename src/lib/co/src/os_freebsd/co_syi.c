/* 
 * Proview   $Id: co_syi.c,v 1.2 2008-06-24 07:03:42 claes Exp $
 * Copyright (C) 2005 SSAB Oxel�sund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* co_syi.c -- System information

   This module gives information about the system.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#include "pwr.h"
#include "co_errno.h"
#include "co_syi.h"
#include "co_syi_msg.h"


char *
syi_HostName (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{

  return syi_NodeName(status, ibuffer, isize);
}

char *
syi_NodeName (
  pwr_tStatus *status,
  char *ibuffer,
  int isize
)
{
  char* cp;
  pwr_dStatus(sts, status, SYI__SUCCESS);

  if (gethostname(ibuffer, isize) != 0) {
    if (errno == EINVAL) {
      *sts = SYI__TRUNCATED;
    } else { 
      *sts = errno_Status(errno);
      ibuffer = NULL;
    }
  }
  
  /* Remove domain */
  if ((cp = strchr(ibuffer, '.')))
    *cp = 0;

  return ibuffer;
}

char *
syi_Ethernet (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  pwr_dStatus(sts, status, SYI__NYI);

  return NULL;
}

char *
syi_NodeSpec (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  pwr_dStatus(sts, status, SYI__NYI);

  return NULL;
}

char *
syi_HostSpec (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{

  return syi_NodeSpec(status, ibuffer, size);
}

/* Return true if node is booted locally or
   false if booted from remote node via network.  */

pwr_tBoolean
syi_LocalBoot (
  pwr_tStatus *status
)
{

  return YES;
}

/* .  */

char*
syi_BootDisk (
  pwr_tStatus *status,
  char *ibuffer,
  int size
)
{
  pwr_dStatus(sts, status, SYI__NYI);

  return NULL;
}

pwr_tStatus syi_UserName( char *user, int len)
{
  char *p;
  
  p = getlogin();
  if ( !p)
    return 0;
  strncpy( user, p, len);
  return 1;
}

const char *syi_Hardware()
{
#if defined HW_X86_64
  static const char hw[] = "x86_64";
#else
  static const char hw[] = "x86";
#endif
  return hw;
}

const char *syi_OpSys()
{
#if defined OS_LINUX
  static const char opsys[] = "Linux";
#elif defined OS_MACOS
  static const char opsys[] = "MacOS";
#elif defined OS_FREEBSD
  static const char opsys[] = "FreeBSD";
#else
  static const char opsys[] = "Unknown";
#endif
  return opsys;
}

char *syi_ProcessId()
{
  static char pidstr[40];

  sprintf( pidstr, "%u", getpid());
  return pidstr;
}