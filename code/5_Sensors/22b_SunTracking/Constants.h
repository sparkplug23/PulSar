// //
// // Constants.h
// // Version timestamp: 9-26-2018, 10:45 PM
// //
// // Attribution : Copyright (c) 2018 Northern_Loki (sha256::6F290BF833967127BE26C92C8F6B1C1A3949C55A7EABCEF3ECC785CD2D38D30D)
// // License is granted under the Creative Commons Attribution-ShareAlike 4.0 International.  https://creativecommons.org/licenses/by-sa/4.0/
// //
// # pragma once

// /* Euler's constant */
// # define EULER 2.7182818284590452353602875

// /* Avagadros Constant SI unit (1/mol) */
// # define NA (6.022140758 * pow(10,23))

// /* Boltzmann's constant (J/K) */
// # define BOLTZMANN (1.38064852 * (pow(10,−23)))


// /* Stefan-Boltzmann's constant SI units (W/(m^2.K^4)) */
// # define STEFAN_BOLTZMANN (5.67036713 * (pow(10,-8)))

// /* Latent heat of vaporization 1ATM and 0 degrees C SI units (J/g) */
// # define L0 2500

// /* Latent heat of vaporization 1ATM and 20 degrees C SI units (J/g) */
// # define L20 2450

// /* Latent heat of vaporization 1ATM and 100 degrees C SI units (J/g) */
// # define L100 2260

// /* Density of dry air 1ATM and 0 degrees C SI units (kg/m^3) */
// # define p_air0 1.2922

// /* Isobaric heat capacity of dry air 1ATM and 0 degrees C SI units (J/kg K) */
// # define Cp_air0 1010

// /* Molecular weight of water (g/mol) */
// # define M_H20 18.0


// /**************/
// /* Conversion */
// /**************/
// # define TO_DEGREE(radians) ((radians) * 57.295779513082320876798154814105)
// # define TO_RADIAN(degrees) ((degrees) * 0.017453292519943295769236907684886)


// /*********/
// /* Time  */
// /********/
// /* Julian J2000 */
// # define J2000 (2451545.0)

// /* Julian UNIX epoch 0h Jan 1, 1970 */
// # define UNIX_JULIAN_EPOCH (2440587.5)

// /* Julian days per year */
// # define JULIAN_DAYS_YEAR (365.25)

// /* Julian days per century */
// # define JULIAN_DAYS_CENTURY  (JULIAN_DAYS_YEAR) * 100.0

// /* Solar days per century */
// # define SOLAR_CENTURY (36524.22)

// /* Hours per day*/
// # define HOURS_DAY (24.0)

// /* Minutes per hour */
// # define MINUTES_HOUR (60.0)

// /* Minutes per day */
// # define MINUTES_DAY (MINUTES_HOUR) * (HOURS_DAY)

// /* Seconds per minute*/
// # define SECONDS_MINUTE (60.0)

// /* Seconds per hour (3600) */
// # define SECONDS_HOUR ((SECONDS_MINUTE) * (MINUTES_HOUR))

// /* Seconds per day (86400) */
// # define SECONDS_DAY ((HOURS_DAY) * (SECONDS_HOUR))

// /* Mean tropical days per year */
// # define MEAN_TROPICAL_DAYS_YEAR (365.242189)

// /* Mean sidereal days per year */
// # define SIDEREAL_DAYS_YEAR (365.25636)


// /**************/
// /* Distance   */
// /**************/

// /* Astronomical units. Kilometers per AU */ 
// # define AU_KM (149597870.700) 


// /**************/
// /*   Solar   */
// /*************/

// /* Solar constant W/m^2 (averaged of a year) Page, 1986 */
// # define SOLAR_CONSTANT 1366.9444

// /* Planetary Atmospheric Albedo Mean */
// # define ALBEDO_MEAN_Rp 0.3

// /* Sun Blackbody Temperature in Kelvin */
// # define SUN_BLACKBODY_TEMP 5777

// /**************/
// /* Pressure   */
// /*************/

// /* Average atmospheric pressure at sea level (Pascals) */
// # define ATM1_AVE 101325.0
	
// /* Pascals to millibars */
// # define TO_MILLIBAR(pascal) (pascal / 100.0)
	
// /* Millibars to pascals */
// # define TO_PASCAL(millibars) (millibars * 100.0)