/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2015, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following condition is met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */
#ifndef _SAM_INCLUDED_
#define _SAM_INCLUDED_

#define part_is_defined(part) (defined(__ ## part ## __))

/*
 * ----------------------------------------------------------------------------
 * SAMG family
 * ----------------------------------------------------------------------------
 */

/* SAMG55 series */
#define SAMG55J1 ( \
    part_is_defined( SAMG55J19 ) )

#define SAMG55G1 ( \
    part_is_defined( SAMG55G19 ) )

/* Entire SAMG55 series */
#define SAMG55_SERIES (SAMG55J1 || SAMG55G1)

/* SAMG54 series */
#define SAMG54N1 ( \
    part_is_defined( SAMG54N19 ) )

#define SAMG54J1 ( \
    part_is_defined( SAMG54J19 ) )

#define SAMG54G1 ( \
    part_is_defined( SAMG54G19 ) )

/* Entire SAMG54 series */
#define SAMG54_SERIES (SAMG54N1 || SAMG54J1 || SAMG54G1)



/* Entire SAMG family */
#define SAMG_SERIES ( SAMG55_SERIES || SAMG54_SERIES)

/*
 * ----------------------------------------------------------------------------
 * SAMD family
 * ----------------------------------------------------------------------------
 */

/* SAMD10 series */
#define SAMD10C14 ( \
    part_is_defined( SAMD10C14A ) )

#define SAMD10C13 ( \
    part_is_defined( SAMD10C13A ) )

#define SAMD10D13A ( \
    part_is_defined( SAMD10D13AS ) || \
    part_is_defined( SAMD10D13AM ) )

#define SAMD10D14A ( \
    part_is_defined( SAMD10D14AS ) || \
    part_is_defined( SAMD10D14AM ) )

/* Entire SAMD10 series */
#define SAMD10_SERIES (SAMD10C14 || SAMD10C13 || SAMD10D13A || SAMD10D14A)

/* SAMD11 series */
#define SAMD11C14 ( \
    part_is_defined( SAMD11C14A ) )

#define SAMD11D14A ( \
    part_is_defined( SAMD11D14AS ) || \
    part_is_defined( SAMD11D14AM ) )

/* Entire SAMD11 series */
#define SAMD11_SERIES (SAMD11C14 || SAMD11D14A)

/* SAMD21 series */
#define SAMD21J17 ( \
    part_is_defined( SAMD21J17A ) )

#define SAMD21G17A ( \
    part_is_defined( SAMD21G17AU ) )

#define SAMD21E15B ( \
    part_is_defined( SAMD21E15BU ) )

#define SAMD21E15 ( \
    part_is_defined( SAMD21E15A ) || \
    part_is_defined( SAMD21E15B ) || \
    part_is_defined( SAMD21E15L ) )

#define SAMD21E16 ( \
    part_is_defined( SAMD21E16A ) || \
    part_is_defined( SAMD21E16B ) || \
    part_is_defined( SAMD21E16L ) )

#define SAMD21E17 ( \
    part_is_defined( SAMD21E17A ) )

#define SAMD21E18 ( \
    part_is_defined( SAMD21E18A ) )

#define SAMD21E16B ( \
    part_is_defined( SAMD21E16BU ) )

#define SAMD21J15 ( \
    part_is_defined( SAMD21J15A ) || \
    part_is_defined( SAMD21J15B ) )

#define SAMD21J16 ( \
    part_is_defined( SAMD21J16A ) || \
    part_is_defined( SAMD21J16B ) )

#define SAMD21G16 ( \
    part_is_defined( SAMD21G16A ) || \
    part_is_defined( SAMD21G16B ) )

#define SAMD21G17 ( \
    part_is_defined( SAMD21G17A ) )

#define SAMD21G18A ( \
    part_is_defined( SAMD21G18AU ) )

#define SAMD21G15 ( \
    part_is_defined( SAMD21G15A ) || \
    part_is_defined( SAMD21G15B ) )

#define SAMD21G18 ( \
    part_is_defined( SAMD21G18A ) )

#define SAMD21J18 ( \
    part_is_defined( SAMD21J18A ) )

/* Entire SAMD21 series */
#define SAMD21_SERIES (SAMD21J17 || SAMD21G17A || SAMD21E15B || SAMD21E15 || SAMD21E16 || SAMD21E17 || SAMD21E18 || SAMD21E16B || SAMD21J15 || SAMD21J16 || SAMD21G16 || SAMD21G17 || SAMD21G18A || SAMD21G15 || SAMD21G18 || SAMD21J18)

/* Entire SAMD family */
#define SAMD_SERIES (SAMD10_SERIES || SAMD11_SERIES || SAMD21_SERIES)

/*
 * ----------------------------------------------------------------------------
 * SAML family
 * ----------------------------------------------------------------------------
 */

/* SAML21 series */
#define SAML21E18A ( \
    part_is_defined( SAML21E18A ) )
#define SAML21G18A ( \
    part_is_defined( SAML21G18A ) )
#define SAML21J18A ( \
    part_is_defined( SAML21J18A ) )

#define SAML21E15B ( \
    part_is_defined( SAML21E15B ) )
#define SAML21E16B ( \
    part_is_defined( SAML21E16B ) )
#define SAML21E17B ( \
    part_is_defined( SAML21E17B ) )
#define SAML21E18B ( \
    part_is_defined( SAML21E18B ) )
#define SAML21G16B ( \
    part_is_defined( SAML21G16B ) )
#define SAML21G17B ( \
    part_is_defined( SAML21G17B ) )
#define SAML21G18B ( \
    part_is_defined( SAML21G18B ) )
#define SAML21J16B ( \
    part_is_defined( SAML21J16B ) )
#define SAML21J17B ( \
    part_is_defined( SAML21J17B ) )
#define SAML21J18B ( \
    part_is_defined( SAML21J18B ) )
#define SAML21J18BU ( \
    part_is_defined( SAML21J18BU ) )

/* Entire SAML21A series */
#define SAML21_SERIES (SAML21E18A || SAML21G18A || SAML21J18A)

/* Entire SAML21B series */
#define SAML21B_SERIES (SAML21E15B || SAML21E16B || SAML21E17B || SAML21E18B || SAML21G16B || SAML21G17B || SAML21G18B || SAML21J16B || SAML21J17B || SAML21J18B || SAML21J18BU)

/* Entire SAMD family */
#define SAML_SERIES (SAML21A_SERIES || SAML21B_SERIES)

/*
 * ----------------------------------------------------------------------------
 * SAMS family
 * ----------------------------------------------------------------------------
 */

/* SAMS70 series */
#define SAMS70Q2 ( \
    part_is_defined( SAMS70Q20 ) || \
    part_is_defined( SAMS70Q21 ) )

#define SAMS70Q1 ( \
    part_is_defined( SAMS70Q19 ) )

#define SAMS70N2 ( \
    part_is_defined( SAMS70N20 ) || \
    part_is_defined( SAMS70N21 ) )

#define SAMS70N1 ( \
    part_is_defined( SAMS70N19 ) )

#define SAMS70J2 ( \
    part_is_defined( SAMS70J20 ) || \
    part_is_defined( SAMS70J21 ) )

#define SAMS70J1 ( \
    part_is_defined( SAMS70J19 ) )

/* Entire SAMS70 series */
#define SAMS70_SERIES (SAMS70Q2 || SAMS70Q1 || SAMS70N2 || SAMS70N1 || SAMS70J2 || SAMS70J1)

/* Entire SAMS family */
#define SAMS_SERIES (SAMS70_SERIES)

/*
 * ----------------------------------------------------------------------------
 * SAME family
 * ----------------------------------------------------------------------------
 */

/* SAME70 series */
#define SAME70N1 ( \
    part_is_defined( SAME70N19 ) )

#define SAME70N2 ( \
    part_is_defined( SAME70N20 ) || \
    part_is_defined( SAME70N21 ) )

#define SAME70J1 ( \
    part_is_defined( SAME70J19 ) )

#define SAME70J2 ( \
    part_is_defined( SAME70J20 ) || \
    part_is_defined( SAME70J21 ) )

#define SAME70Q1 ( \
    part_is_defined( SAME70Q19 ) )

#define SAME70Q2 ( \
    part_is_defined( SAME70Q20 ) || \
    part_is_defined( SAME70Q21 ) )

/* Entire SAME70 series */
#define SAME70_SERIES (SAME70N1 || SAME70N2 || SAME70J1 || SAME70J2 || SAME70Q1 || SAME70Q2)

/* Entire SAME family */
#define SAME_SERIES (SAME70_SERIES)

/*
 * ----------------------------------------------------------------------------
 * SAM3 family
 * ----------------------------------------------------------------------------
 */

/* SAM3U series */
#define SAM3U1 ( \
    part_is_defined( SAM3U1E ) || \
    part_is_defined( SAM3U1C ) )

#define SAM3U2 ( \
    part_is_defined( SAM3U2E ) || \
    part_is_defined( SAM3U2C ) )

#define SAM3U4 ( \
    part_is_defined( SAM3U4E ) || \
    part_is_defined( SAM3U4C ) )

/* Entire SAM3U series */
#define SAM3U_SERIES (SAM3U1 || SAM3U2 || SAM3U4)

/* SAM3XA series */
#define SAM3A4 ( \
    part_is_defined( SAM3A4C ) )

#define SAM3X4 ( \
    part_is_defined( SAM3X4C ) || \
    part_is_defined( SAM3X4E ) )

#define SAM3A8 ( \
    part_is_defined( SAM3A8C ) )

#define SAM3X8 ( \
    part_is_defined( SAM3X8C ) || \
    part_is_defined( SAM3X8E ) || \
    part_is_defined( SAM3X8H ) )

/* Entire SAM3XA series */
#define SAM3XA_SERIES (SAM3A4 || SAM3X4 || SAM3A8 || SAM3X8)

/* Entire SAM3 family */
#define SAM3_SERIES (SAM3U_SERIES || SAM3XA_SERIES)

/*
 * ----------------------------------------------------------------------------
 * SAMR family
 * ----------------------------------------------------------------------------
 */

/* SAMR21 series */
#define SAMR21E16 ( \
    part_is_defined( SAMR21E16A ) )

#define SAMR21E18 ( \
    part_is_defined( SAMR21E18A ) )

#define SAMR21G16 ( \
    part_is_defined( SAMR21G16A ) )

#define SAMR21G17 ( \
    part_is_defined( SAMR21G17A ) )

#define SAMR21G18 ( \
    part_is_defined( SAMR21G18A ) )

#define SAMR21E17 ( \
    part_is_defined( SAMR21E17A ) )

/* Entire SAMR21 series */
#define SAMR21_SERIES (SAMR21E16 || SAMR21E18 || SAMR21G16 || SAMR21G17 || SAMR21G18 || SAMR21E17)

/* Entire SAMR family */
#define SAMR_SERIES (SAMR21_SERIES)

/*
 * ----------------------------------------------------------------------------
 * SAM4 family
 * ----------------------------------------------------------------------------
 */

/* SAM4N series */
#define SAM4N8 ( \
    part_is_defined( SAM4N8A ) || \
    part_is_defined( SAM4N8B ) || \
    part_is_defined( SAM4N8C ) )

#define SAM4N16 ( \
    part_is_defined( SAM4N16B ) || \
    part_is_defined( SAM4N16C ) )

/* Entire SAM4N series */
#define SAM4N_SERIES (SAM4N8 || SAM4N16)

/* SAM4S series */
#define SAM4S8 ( \
    part_is_defined( SAM4S8C ) || \
    part_is_defined( SAM4S8B ) )

#define SAM4S2 ( \
    part_is_defined( SAM4S2A ) || \
    part_is_defined( SAM4S2B ) || \
    part_is_defined( SAM4S2C ) )

#define SAM4S4 ( \
    part_is_defined( SAM4S4A ) || \
    part_is_defined( SAM4S4B ) || \
    part_is_defined( SAM4S4C ) )

#define SAM4SD16 ( \
    part_is_defined( SAM4SD16B ) || \
    part_is_defined( SAM4SD16C ) )

#define SAM4S16 ( \
    part_is_defined( SAM4S16C ) || \
    part_is_defined( SAM4S16B ) )

#define SAM4SD32 ( \
    part_is_defined( SAM4SD32B ) || \
    part_is_defined( SAM4SD32C ) )

#define SAM4SA16 ( \
    part_is_defined( SAM4SA16B ) || \
    part_is_defined( SAM4SA16C ) )

/* Entire SAM4S series */
#define SAM4S_SERIES (SAM4S8 || SAM4S2 || SAM4S4 || SAM4SD16 || SAM4S16 || SAM4SD32 || SAM4SA16)

/* SAM4E series */
#define SAM4E16 ( \
    part_is_defined( SAM4E16E ) || \
    part_is_defined( SAM4E16C ) )

#define SAM4E8 ( \
    part_is_defined( SAM4E8E ) || \
    part_is_defined( SAM4E8C ) )

/* Entire SAM4E series */
#define SAM4E_SERIES (SAM4E16 || SAM4E8)

/* SAM4C series */
#define SAM4C4C_ ( \
    part_is_defined( SAM4C4C_0 ) || \
    part_is_defined( SAM4C4C_1 ) )

#define SAM4C8C_ ( \
    part_is_defined( SAM4C8C_0 ) || \
    part_is_defined( SAM4C8C_1 ) )

#define SAM4C16C_ ( \
    part_is_defined( SAM4C16C_0 ) || \
    part_is_defined( SAM4C16C_1 ) )

#define SAM4C32C_ ( \
    part_is_defined( SAM4C32C_0 ) || \
    part_is_defined( SAM4C32C_1 ) )

#define SAM4C32E_ ( \
    part_is_defined( SAM4C32E_0 ) || \
    part_is_defined( SAM4C32E_1 ) )

/* Entire SAM4C series */
#define SAM4C_SERIES (SAM4C4C_ || SAM4C8C_ || SAM4C16C_ || SAM4C32C_ || SAM4C32E_)

/* Entire SAM4 family */
#define SAM4_SERIES (SAM4N_SERIES || SAM4S_SERIES || SAM4E_SERIES || SAM4C_SERIES)

/*
 * ----------------------------------------------------------------------------
 * Whole SAM product line
 * ----------------------------------------------------------------------------
 */

#define SAM (SAM4_SERIES || SAMR_SERIES || SAM3_SERIES || SAMS_SERIES || SAME_SERIES || SAMD_SERIES || SAML_SERIES || SAMG_SERIES)

/*
 * ----------------------------------------------------------------------------
 * Header inclusion
 * ----------------------------------------------------------------------------
 */

#if SAMG_SERIES
#include "samg.h"
#endif /* SAMG_SERIES */

#if SAME_SERIES
#include "same.h"
#endif /* SAME_SERIES */

#if SAMD_SERIES
#include "samd.h"
#endif /* SAMD_SERIES */

#if SAML_SERIES
#include "saml.h"
#endif /* SAML_SERIES */

#if SAMS_SERIES
#include "sams.h"
#endif /* SAMS_SERIES */

#if SAM3_SERIES
#include "sam3.h"
#endif /* SAM3_SERIES */

#if SAMR_SERIES
#include "samr.h"
#endif /* SAMR_SERIES */

#if SAM4_SERIES
#include "sam4.h"
#endif /* SAM4_SERIES */

#endif
