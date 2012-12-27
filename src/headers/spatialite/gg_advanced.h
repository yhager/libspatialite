/*
 gg_advanced.h -- Gaia common support for geometries: advanced
  
 version 4.0, 2012 August 6

 Author: Sandro Furieri a.furieri@lqt.it

 ------------------------------------------------------------------------------
 
 Version: MPL 1.1/GPL 2.0/LGPL 2.1
 
 The contents of this file are subject to the Mozilla Public License Version
 1.1 (the "License"); you may not use this file except in compliance with
 the License. You may obtain a copy of the License at
 http://www.mozilla.org/MPL/
 
Software distributed under the License is distributed on an "AS IS" basis,
WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
for the specific language governing rights and limitations under the
License.

The Original Code is the SpatiaLite library

The Initial Developer of the Original Code is Alessandro Furieri
 
Portions created by the Initial Developer are Copyright (C) 2008-2012
the Initial Developer. All Rights Reserved.

Contributor(s):


Alternatively, the contents of this file may be used under the terms of
either the GNU General Public License Version 2 or later (the "GPL"), or
the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
in which case the provisions of the GPL or the LGPL are applicable instead
of those above. If you wish to allow use of your version of this file only
under the terms of either the GPL or the LGPL, and not to allow others to
use your version of this file under the terms of the MPL, indicate your
decision by deleting the provisions above and replace them with the notice
and other provisions required by the GPL or the LGPL. If you do not delete
the provisions above, a recipient may use your version of this file under
the terms of any one of the MPL, the GPL or the LGPL.
 
*/

/*
 
CREDITS:

this module has been partly funded by:
Regione Toscana - Settore Sistema Informativo Territoriale ed Ambientale
(wrapping liblwgeom APIs) 

*/


/**
 \file gg_advanced.h

 Geometry handling functions: advanced
 */

#ifndef _GG_ADVANCED_H
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#define _GG_ADVANCED_H
#endif

/** Gaia-to-GEOS: all geometries */
#define GAIA2GEOS_ALL			0

/** Gaia-to-GEOS: only geometries of the Point type */
#define GAIA2GEOS_ONLY_POINTS		1

/** Gaia-to-GEOS: only geometries of the Linestring type */
#define GAIA2GEOS_ONLY_LINESTRINGS	2

/** Gaia-to-GEOS: only geometries of the Polygon type */
#define GAIA2GEOS_ONLY_POLYGONS		3

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef OMIT_PROJ		/* including PROJ.4 */
#endif

/**
 Converts and angle from Radians into Degrees
 \param rads the angle measured in Radians.
 
 \return the angle measured in Degrees.

 \sa gaiaDegsToRads

 \remark \b PROJ.4 support required
 */
    GAIAGEO_DECLARE double gaiaRadsToDegs (double rads);

/**
 Converts and angle from Degrees into Radians
 \param degs the angle measured in Degrees.

 \return the angle measured in Radians.

 \sa gaiaRadsToDegs

 \remark \b PROJ.4 support required
 */
    GAIAGEO_DECLARE double gaiaDegsToRads (double degs);

/**
 Tansforms a Geometry object into a different Reference System
 [aka Reprojection]
 \param org pointer to input Geometry object.
 \param proj_from geodetic parameters string [EPSG format] qualifying the
 input Reference System
 \param proj_to geodetic parameters string [EPSG format] qualifying the
 output Reference System

 \return the pointer to newly created Geometry object: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,  this including any Geometry returned by gaiaGeometryTransform()

 \remark \b PROJ.4 support required
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaTransform (gaiaGeomCollPtr org,
						   char *proj_from,
						   char *proj_to);

#endif				/* end including PROJ.4 */

#ifndef OMIT_GEOS		/* including GEOS */

/**
 Resets the GEOS error and warning messages to an empty state

 \sa gaiaGetGeosErrorMsg, gaiaGetGeosWarningMsg, gaiaSetGeosErrorMsg,
 gaiaSetGeosWarningMsg

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE void gaiaResetGeosMsg (void);

/**
 Return the latest GEOS error message (if any)

 \return the latest GEOS error message: an empty string if no error was
 previoysly found.

 \sa gaiaResetGeosMsg, gaiaGetGeosWarningMsg, gaiaSetGeosErrorMsg,
 gaiaSetGeosWarningMsg

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE const char *gaiaGetGeosErrorMsg (void);

/**
 Return the latest GEOS warning message (if any)

 \return the latest GEOS warning message: an empty string if no warning was 
 previoysly found.

 \sa gaiaResetGeosMsg, gaiaGetGeosErrorMsg, gaiaSetGeosErrorMsg,
 gaiaSetGeosWarningMsg

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE const char *gaiaGetGeosWarningMsg (void);

/**
 Set the current GEOS error message

 \param msg the error message to be set.

 \sa gaiaResetGeosMsg, gaiaGetGeosErrorMsg, gaiaGetGeosWarningMsg,
 gaiaSetGeosWarningMsg

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE void gaiaSetGeosErrorMsg (const char *msg);

/**
 Set the current GEOS warning message

 \param msg the warning message to be set.

 \sa gaiaResetGeosMsg, gaiaGetGeosErrorMsg, gaiaGetGeosWarningMsg,
 gaiaSetGeosErrorMsg

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE void gaiaSetGeosWarningMsg (const char *msg);

/**
 Converts a Geometry object into a GEOS Geometry

 \param gaia pointer to Geometry object

 \return handle to GEOS Geometry
 
 \sa gaiaFromGeos_XY, gaiaFromGeos_XYZ, gaiaFromGeos_XYM, gaiaFromGeos_XYZM,
  gaiaToGeosSelective

 \note convenience method, simply defaulting to gaiaToGeos(geom, GAIA2GEOS_ALL)

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE void *gaiaToGeos (const gaiaGeomCollPtr gaia);

/**
 Converts a Geometry object into a GEOS Geometry

 \param gaia pointer to Geometry object
 \param mode one of GAIA2GEOS_ALL, GAIA2GEOS_ONLY_POINTS,
  GAIA2GEOS_ONLY_LINESTRINGS or GAIA2GEOS_ONLY_POLYGONS

 \return handle to GEOS Geometry
 
 \sa gaiaFromGeos_XY, gaiaFromGeos_XYZ, gaiaFromGeos_XYM, gaiaFromGeos_XYZM

 \note if the mode argument is not GAIA2GEOS_ALL only elementary geometries
  of the selected type will be passed to GEOS, ignoring any other.

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE void *gaiaToGeosSelective (const gaiaGeomCollPtr gaia,
					       int mode);

/**
 Converts a GEOS Geometry into a Geometry object [XY dims]

 \param geos handle to GEOS Geometry

 \return the pointer to the newly created Geometry object

 \sa gaiaToGeos, gaiaFromGeos_XYZ, gaiaFromGeos_XYM, gaiaFromGeos_XYZM

 \note you are responsible to destroy (before or after) any allocated 
 Geometry, this including any Geometry returned by gaiaFromGeos_XY()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaFromGeos_XY (const void *geos);

/**
 Converts a GEOS Geometry into a Geometry object [XYZ dims]

 \param geos handle to GEOS Geometry
    
 \return the pointer to the newly created Geometry object

 \sa gaiaToGeos, gaiaFromGeos_XY, gaiaFromGeos_XYM, gaiaFromGeos_XYZM
 
 \note you are responsible to destroy (before or after) any allocated 
 Geometry, this including any Geometry returned by gaiaFromGeos_XYZ()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaFromGeos_XYZ (const void *geos);

/**
 Converts a GEOS Geometry into a Geometry object [XYM dims]

 \param geos handle to GEOS Geometry
    
 \return the pointer to the newly created Geometry object

 \sa gaiaToGeos, gaiaFromGeos_XY, gaiaFromGeos_XYZ, gaiaFromGeos_XYZM
 
 \note you are responsible to destroy (before or after) any allocated 
 Geometry, this including any Geometry returned by gaiaFromGeos_XYM()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaFromGeos_XYM (const void *geos);

/**
 Converts a GEOS Geometry into a Geometry object [XYZM dims]

 \param geos handle to GEOS Geometry
    
 \return the pointer to the newly created Geometry object

 \sa gaiaToGeos, gaiaFromGeos_XY, gaiaFromGeos_XYZ, gaiaFromGeos_XYM
 
 \note you are responsible to destroy (before or after) any allocated 
 Geometry, this including any Geometry returned by gaiaFromGeos_XYZM()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaFromGeos_XYZM (const void *geos);

/**
 Checks if a Geometry object represents an OGC Simple Geometry

 \param geom pointer to Geometry object.

 \return 0 if false; any other value if true

 \sa gaiaIsClosed, gaiaIsRing, gaiaIsValid

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaIsSimple (gaiaGeomCollPtr geom);

/**
 Checks if a Linestring object represents an OGC Closed Geometry
 
 This function only works on a single linestring - if you pass in a multi-line
 linestring geometry, it will return 0 (false). See gaiaIsClosedGeom for an
 alternative.

 \param line pointer to Linestring object.

 \return 0 if false; any other value if true

 \sa gaiaIsSimple, gaiaIsRing, gaiaIsValid, gaiaIsClosedGeom

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaIsClosed (gaiaLinestringPtr line);

/**
 Checks if a Geometry object represents an OGC Closed Linestring

 \param geom pointer to Geometry object.

 \return 0 if false; any other value if true

 \sa gaiaIsSimple, gaiaIsRing, gaiaIsValid, gaiaIsClosed

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaIsClosedGeom (gaiaGeomCollPtr geom);

/**
 Checks if a Linestring object represents an OGC Ring Geometry

 \param line pointer to Geometry object.

 \return 0 if false; any other value if true

 \sa gaiaIsSimple, gaiaIsClosed, gaiaIsValid

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaIsRing (gaiaLinestringPtr line);

/**
 Checks if a Geometry object represents an OGC Valid Geometry

 \param geom pointer to Geometry object.

 \return 0 if false; any other value if true

 \sa gaiaIsSimple, gaiaIsClosed, gaiaIsRing

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaIsValid (gaiaGeomCollPtr geom);

/**
 Measures the total Length for a Geometry object

 \param geom pointer to Geometry object
 \param length on completion this variable will contain the measured length

 \return 0 on failure: any other value on success

 \sa gaiaGeomCollArea, gaiaMeasureLength, gaiaGeomCollLengthOrPerimeter

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollLength (gaiaGeomCollPtr geom,
					    double *length);

/**
 Measures the total Length or Perimeter for a Geometry object

 \param geom pointer to Geometry object
 \param perimeter if TRUE only Polygons will be considered, ignoring any Linesting
 \n the opposite if FALSE (considering only Linestrings and ignoring any Polygon)
 \param length on completion this variable will contain the measured length
  or perimeter

 \return 0 on failure: any other value on success

 \sa gaiaGeomCollArea, gaiaMeasureLength, gaiaGeomCollLength

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollLengthOrPerimeter (gaiaGeomCollPtr geom,
						       int perimeter,
						       double *length);
/**
 Measures the total Area for a Geometry object

 \param geom pointer to Geometry object
 \param area on completion this variable will contain the measured area

 \return 0 on failure: any other value on success

 \sa gaiaGeomCollLength, gaiaMeasureArea

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollArea (gaiaGeomCollPtr geom, double *area);


/**
 Attempts to rearrange a generic Geometry object into a Polygon or MultiPolygon

 \param geom the input Geometry object
 \param force_multi if not set to 0, then an eventual Polygon will be 
 returned casted to MultiPolygon

 \return the pointer to newly created Geometry object representing a
 Polygon or MultiPolygon Geometry: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaPolygonize()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaPolygonize (gaiaGeomCollPtr geom,
						    int force_multi);
/**
 Spatial relationship evalution: Equals
 
 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollDisjoint, gaiaGeomCollIntersects, gaiaGeomCollOverlaps,
 gaiaGeomCollCrosses, gaiaGeomCollContains, gaiaGeomCollWithin,
 gaiaGeomCollTouches, gaiaGeomCollRelate, gaiaGeomCollPreparedDisjoint

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollEquals (gaiaGeomCollPtr geom1,
					    gaiaGeomCollPtr geom2);

/**
 Spatial relationship evalution: Disjoint

 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollIntersects, gaiaGeomCollOverlaps,
 gaiaGeomCollCrosses, gaiaGeomCollContains, gaiaGeomCollWithin,
 gaiaGeomCollTouches, gaiaGeomCollRelate
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollDisjoint (gaiaGeomCollPtr geom1,
					      gaiaGeomCollPtr geom2);

/**
 Spatial relationship evalution: Disjoint (GEOSPreparedGeometry)

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 the first Geometry object to be evaluated
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 the second Geometry object to be evaluated
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false: any other value if true

 \sa gaiaGeomCollDisjoint
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedDisjoint (void *p_cache,
						      gaiaGeomCollPtr geom1,
						      unsigned char *blob1,
						      int size1,
						      gaiaGeomCollPtr geom2,
						      unsigned char *blob2,
						      int size2);

/**
 Spatial relationship evalution: Intesects

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollDisjoint, gaiaGeomCollOverlaps,
 gaiaGeomCollCrosses, gaiaGeomCollContains, gaiaGeomCollWithin,
 gaiaGeomCollTouches, gaiaGeomCollRelate, gaiaGeomCollPreparedIntersects
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollIntersects (gaiaGeomCollPtr geom1,
						gaiaGeomCollPtr geom2);
/**
 Spatial relationship evalution: Intesects (GEOSPreparedGeometry)

 \param geom1 the first Geometry object to be evaluated
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 the second Geometry object to be evaluated
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false: any other value if true

 \sa gaiaGeomCollIntersects
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedIntersects (void *p_cache,
							gaiaGeomCollPtr geom1,
							unsigned char *blob1,
							int size1,
							gaiaGeomCollPtr geom2,
							unsigned char *blob2,
							int size2);

/**
 Spatial relationship evalution: Overlaps

 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollDisjoint, gaiaGeomCollIntersects, 
 gaiaGeomCollCrosses, gaiaGeomCollContains, gaiaGeomCollWithin,
 gaiaGeomCollTouches, gaiaGeomCollRelate, gaiaGeomCollPreparedOverlaps
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollOverlaps (gaiaGeomCollPtr geom1,
					      gaiaGeomCollPtr geom2);

/**
 Spatial relationship evalution: Overlaps (GEOSPreparedGeometry)

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 the first Geometry object to be evaluated
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 the second Geometry object to be evaluated
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false: any other value if true

 \sa gaiaGeomCollOverlaps
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedOverlaps (void *p_cache,
						      gaiaGeomCollPtr geom1,
						      unsigned char *blob1,
						      int size1,
						      gaiaGeomCollPtr geom2,
						      unsigned char *blob2,
						      int size2);

/**
 Spatial relationship evalution: Crosses

 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollDisjoint, gaiaGeomCollIntersects, 
 gaiaGeomCollOverlaps, gaiaGeomCollContains, gaiaGeomCollWithin,
 gaiaGeomCollTouches, gaiaGeomCollRelate, gaiaGeomCollCrosses
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollCrosses (gaiaGeomCollPtr geom1,
					     gaiaGeomCollPtr geom2);

/**
 Spatial relationship evalution: Crosses (GEOSPreparedGeometry)

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 the first Geometry object to be evaluated
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 the second Geometry object to be evaluated
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false: any other value if true

 \sa gaiaGeomCollCrosses
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedCrosses (void *p_cache,
						     gaiaGeomCollPtr geom1,
						     unsigned char *blob1,
						     int size1,
						     gaiaGeomCollPtr geom2,
						     unsigned char *blob2,
						     int size2);

/**
 Spatial relationship evalution: Contains

 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollDisjoint, gaiaGeomCollIntersects, 
 gaiaGeomCollOverlaps, gaiaGeomCollCrosses, gaiaGeomCollWithin,
 gaiaGeomCollTouches, gaiaGeomCollRelate, gaiaGeomCollPreparedContains
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollContains (gaiaGeomCollPtr geom1,
					      gaiaGeomCollPtr geom2);

/**
 Spatial relationship evalution: Contains (GEOSPreparedGeometry)

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 the first Geometry object to be evaluated
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 the second Geometry object to be evaluated
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false: any other value if true

 \sa gaiaGeomCollContains
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedContains (void *p_cache,
						      gaiaGeomCollPtr geom1,
						      unsigned char *blob1,
						      int size1,
						      gaiaGeomCollPtr geom2,
						      unsigned char *blob2,
						      int size2);

/**
 Spatial relationship evalution: Within

 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollDisjoint, gaiaGeomCollIntersects, 
 gaiaGeomCollOverlaps, gaiaGeomCollCrosses, gaiaGeomCollContains, 
 gaiaGeomCollTouches, gaiaGeomCollRelate, gaiaGeomCollWithin
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollWithin (gaiaGeomCollPtr geom1,
					    gaiaGeomCollPtr geom2);

/**
 Spatial relationship evalution: Within (GEOSPreparedGeometry)

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 the first Geometry object to be evaluated
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 the second Geometry object to be evaluated
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false: any other value if true

 \sa gaiaGeomCollPrepared
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedWithin (void *p_cache,
						    gaiaGeomCollPtr geom1,
						    unsigned char *blob1,
						    int size1,
						    gaiaGeomCollPtr geom2,
						    unsigned char *blob2,
						    int size2);

/**
 Spatial relationship evalution: Touches

 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollDisjoint, gaiaGeomCollIntersects, 
 gaiaGeomCollOverlaps, gaiaGeomCollCrosses, gaiaGeomCollContains, 
 gaiaGeomCollWithin, gaiaGeomCollRelate, gaiaGeomCollPreparedTouches
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollTouches (gaiaGeomCollPtr geom1,
					     gaiaGeomCollPtr geom2);

/**
 Spatial relationship evalution: Touches (GEOSPreparedGeometry)

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 the first Geometry object to be evaluated
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 the second Geometry object to be evaluated
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false: any other value if true

 \sa gaiaGeomCollTouches
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedTouches (void *p_cache,
						     gaiaGeomCollPtr geom1,
						     unsigned char *blob1,
						     int size1,
						     gaiaGeomCollPtr geom2,
						     unsigned char *blob2,
						     int size2);

/**
 Spatial relationship evalution: Relate

 \param geom1 the first Geometry object to be evaluated
 \param geom2 the second Geometry object to be evaluated
 \param pattern intersection matrix pattern [DE-9IM]

 \return 0 if false: any other value if true

 \sa gaiaGeomCollEquals, gaiaGeomCollDisjoint, gaiaGeomCollIntersects, 
 gaiaGeomCollOverlaps, gaiaGeomCollCrosses, gaiaGeomCollContains, 
 gaiaGeomCollWithin, gaiaGeomCollTouches
 
 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollRelate (gaiaGeomCollPtr geom1,
					    gaiaGeomCollPtr geom2,
					    const char *pattern);

/**
 Calculates the maximum distance intercurring between two Geometry objects

 \param geom1 the first Geometry object 
 \param geom2 the second Geometry object 
 \param dist on completion this variable will contain the calculated distance

 \return 0 on failure: any other value on success.

 \sa gaia3DDistance, gaiaMaxDistance, gaia3DMaxDistance

 \note this function always computes the 2D cartesian distance.

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollDistance (gaiaGeomCollPtr geom1,
					      gaiaGeomCollPtr geom2,
					      double *dist);

/**
 Spatial operator: Intersection
                                              
 \param geom1 the first Geometry object 
 \param geom2 the second Geometry object 

 \return the pointer to newly created Geometry object representing the
 geometry Intersection of both input Geometries: NULL on failure.

 \sa gaiaFreeGeomColl, gaiaGeometryUnion, gaiaGeometryDifference,
 gaiaGeometrySymDifference, gaiaBoundary

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaGeometryIntersection()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaGeometryIntersection (gaiaGeomCollPtr
							      geom1,
							      gaiaGeomCollPtr
							      geom2);

/**
 Spatial operator: Union

 \param geom1 the first Geometry object
 \param geom2 the second Geometry object

 \return the pointer to newly created Geometry object representing the
 geometry Union of both input Geometries: NULL on failure.

 \sa gaiaFreeGeomColl, gaiaUnaryUnion, gaiaUnionCascaded

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaGeometryUnion()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaGeometryUnion (gaiaGeomCollPtr geom1,
						       gaiaGeomCollPtr geom2);

/**
 Spatial operator: Union Cascaded

 \param geom the input Geometry object.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function is similar to gaiaUnaryUnion, but it only accepts Polygons and 
 MultiPolygons and it's now deprecated; anyway it's supported on older GEOS versions.
 NULL on failure.

 \sa gaiaFreeGeomColl, gaiaGeometryUnion, gaiaUnionUnion

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaUnionCascaded()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaUnionCascaded (gaiaGeomCollPtr geom);

/**
 Spatial operator: Difference

 \param geom1 the first Geometry object
 \param geom2 the second Geometry object

 \return the pointer to newly created Geometry object representing the
 geometry Difference of both input Geometries: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaGeometryDifference()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaGeometryDifference (gaiaGeomCollPtr
							    geom1,
							    gaiaGeomCollPtr
							    geom2);

/**
 Spatial operator: SymDifference

 \param geom1 the first Geometry object
 \param geom2 the second Geometry object

 \return the pointer to newly created Geometry object representing the
 geometry SymDifference of both input Geometries: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaGeometrySymDifference()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaGeometrySymDifference (gaiaGeomCollPtr
							       geom1,
							       gaiaGeomCollPtr
							       geom2);

/**
 Spatial operator: Boundary

 \param geom the Geometry object to be evaluated

 \return the pointer to newly created Geometry object representing the
 geometry Boundary of the input Geometry: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaBoundary()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaBoundary (gaiaGeomCollPtr geom);

/**
 Spatial operator: Centroid

 \param geom pointer to Geometry object.
 \param x on completion this variable will contain the centroid X coordinate 
 \param y on completion this variable will contain the centroid Y coordinate 
 
 \return 0 on failure: any other value on success

 \sa gaiaRingCentroid

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollCentroid (gaiaGeomCollPtr geom, double *x,
					      double *y);

/**
 Spatial operator: PointOnSurface

 \param geom pointer to Geometry object.
 \param x on completion this variable will contain the Point X coordinate  
 \param y on completion this variable will contain the Point Y coordinate
 
 \return 0 on failure: any other value on success

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE int gaiaGetPointOnSurface (gaiaGeomCollPtr geom, double *x,
					       double *y);

/**
 Spatial operator: Simplify

 \param geom the input Geometry object
 \param tolerance approximation threshold

 \return the pointer to newly created Geometry object representing the
 simplified Geometry [applying the Douglas-Peucker algorithm]: NULL on failure.

 \sa gaiaFreeGeomColl, gaiaGeomCollSimplifyPreserveTopology

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaGeomCollSimplify()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaGeomCollSimplify (gaiaGeomCollPtr geom,
							  double tolerance);

/**
 Spatial operator: Simplify [preserving topology]

 \param geom the input Geometry object
 \param tolerance approximation threshold

 \return the pointer to newly created Geometry object representing the
 simplified Geometry [applying the Douglas-Peucker algorithm]: NULL on failure.

 \sa gaiaFreeGeomColl, gaiaGeomCollSimplify

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaGeomCollSimplify()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr
	gaiaGeomCollSimplifyPreserveTopology (gaiaGeomCollPtr geom,
					      double tolerance);

/**
 Spatial operator: ConvexHull

 \param geom the input Geometry object

 \return the pointer to newly created Geometry object representing the
 ConvexHull of input Geometry: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaConvexHull()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaConvexHull (gaiaGeomCollPtr geom);

/** 
 Spatial operator: Buffer

 \param geom the input Geometry object
 \param radius the buffer's radius
 \param points number of points (aka vertices) to be used in order to 
 approximate a circular arc.

 \return the pointer to newly created Geometry object representing the
 Buffer of input Geometry: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaGeomCollBuffer()

 \remark \b GEOS support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaGeomCollBuffer (gaiaGeomCollPtr geom,
							double radius,
							int points);

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
#ifdef GEOS_ADVANCED
#endif

/**
 Calculates the Hausdorff distance intercurring between two Geometry objects

 \param geom1 pointer to first Geometry object
 \param geom2 pointer to second Geometry object
 \param dist on completion this variable will contain the calculated Hausdorff
 distance 

 \return 0 on failure: any other value on success.

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE int gaiaHausdorffDistance (gaiaGeomCollPtr geom1,
					       gaiaGeomCollPtr geom2,
					       double *dist);

/**
 Spatial operator: Offset Curve

 \param geom the input Geometry object
 \param radius the buffer's radius
 \param points number of points (aka vertices) to be used in order to 
 approximate a circular arc.
 \param left_right if set to 1 the left-sided OffsetCurve will be returned;
 otherwise the right-sided one.

 \return the pointer to newly created Geometry object representing the
 OffsetCurve of input Geometry: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaOffsetCurve()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaOffsetCurve (gaiaGeomCollPtr geom,
						     double radius, int points,
						     int left_right);

/**
 Spatial operator: Single Sided Buffer

 \param geom the input Geometry object
 \param radius the buffer's radius
 \param points number of points (aka vertices) to be used in order to
 approximate a circular arc.
 \param left_right if set to 1 the left-sided Buffer will be returned;
 otherwise the right-sided one.

 \return the pointer to newly created Geometry object representing the
 single-sided Buffer of input Geometry: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSingleSidedBuffer()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSingleSidedBuffer (gaiaGeomCollPtr geom,
							   double radius,
							   int points,
							   int left_right);

/**
 Spatial operator: Shared Paths

 \param geom1 pointer to first Geometry object
 \param geom2 pointer to second Geometry object

 \return the pointer to newly created Geometry object representing any
 Share Path common to both input geometries: NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSharedPaths()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSharedPaths (gaiaGeomCollPtr geom1,
						     gaiaGeomCollPtr geom2);

/**
 Spatial operator: Line Interpolate Point

 \param ln_geom the input Geometry object [expected to be of lineal type]
 \param fraction total length fraction [in the range 0.0 / 1.0]

 \return the pointer to newly created Geometry object representing a Point
 laying on the input Geometry and positioned at the given length fraction:
 NULL on failure.

 \sa gaiaLineInterpolateEquidistantPoints
 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaLineInterpolatePoint()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaLineInterpolatePoint (gaiaGeomCollPtr
							      ln_geom,
							      double fraction);

/**
 Spatial operator: Line Interpolate Equidistant Points

 \param ln_geom the input Geometry object [expected to be of lineal type]
 \param distance regular distance between interpolated points

 \return the pointer to newly created Geometry object representing a MultiPoint;
 such MultiPoint always supports the M coordinate (the corresponding value
 representing the progressive distance for each interpolated Point).
 individual Points will be regularly spaced by the given distance:
 NULL on failure.

 \sa gaiaLineInterpolatePoint
 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaLineInterpolateEquidistantPoints()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr
	gaiaLineInterpolateEquidistantPoints (gaiaGeomCollPtr ln_geom,
					      double distance);

/**
 Spatial operator: Line Substring

 \param ln_geom the input Geometry object [expected to be of lineal type]
 \param start_fraction substring start, expressed as total length fraction
 [in the range 0.0 / 1.0]
 \param end_fraction substring end, expressed as total length fraction

 \return the pointer to newly created Geometry object representing a Linestring
 laying on the input Geometry.
 \n this Linestring will begin (and stop) at given total length fractions. 
 NULL on failure.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaLineSubstring()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaLineSubstring (gaiaGeomCollPtr ln_geom,
						       double start_fraction,
						       double end_fraction);

/**
 Spatial operator: Shortest Line

 \param geom1 pointer to the first Geometry object.
 \param geom2 pointer to the second Geometry object.

 \return the pointer to newly created Geometry object representing a Linestring;
 NULL on failure.
 \n the returned Linestring graphically represents the minimum distance 
 intercurrinng between both input geometries.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaShortestLine()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaShortestLine (gaiaGeomCollPtr geom1,
						      gaiaGeomCollPtr geom2);

/**
 Spatial operator: Shortest Line

 \param geom1 pointer to the first Geometry object.
 \param geom2 pointer to the second Geometry object.
 \param tolerance approximation factor

 \return the pointer to newly created Geometry object; NULL on failure.
 \n the returned Geometry represents the first input Geometry (nicely
 \e snapped to the second input Geometry, whenever is possible).

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaShortestLine()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSnap (gaiaGeomCollPtr geom1,
					      gaiaGeomCollPtr geom2,
					      double tolerance);

/**
 Spatial operator: Line Merge

 \param geom pointer to input Geometry object.

 \return the pointer to newly created Geometry object; NULL on failure.
 \n if possible, this representing a reassembled Linestring or MultiLinestring.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaLineMerge()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaLineMerge (gaiaGeomCollPtr geom);

/**
 Spatial operator: Line Cut At Nodes

 \param geom1 pointer to input Geometry object [Linestring or MultiLinestring].
 \param geom2 pointer to input Geometry object [Point or MultiPoint].

 \return the pointer to newly created Geometry object; NULL on failure.
 \n if possible, any input Linestring will be split accordingly to given Node(s):
 no point will be interpolated, existing Linestring Vertices will be evaluated.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaLinesCutAtNodes()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaLinesCutAtNodes (gaiaGeomCollPtr geom1,
							 gaiaGeomCollPtr geom2);

/**
 Spatial operator: Unary Union

 \param geom the input Geometry object.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function is the same as gaiaGeometryUnion, except in that this
 works internally to the input Geometry itself.
 NULL on failure.

 \sa gaiaFreeGeomColl, gaiaGeometryUnion, gaiaUnionCascaded

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaUnaryUnion()

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaUnaryUnion (gaiaGeomCollPtr geom);

/**
 Determines the location of the closest Point on Linestring to the given Point

 \param ln_geom pointer to first input Geometry object [expected to be of
 the lineal type].
 \param pt_geom pointer to second input Geometry object [expected to be a
 Point].

 \return the fraction [in the range 0.0 / 1.0] of ln_geom total length
 where the closest Point to pt_geom lays.

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE double gaiaLineLocatePoint (gaiaGeomCollPtr ln_geom,
						gaiaGeomCollPtr pt_geom);

/** 
 Topology check: test if a Geometry covers another one

 \param geom1 pointer to first input Geometry object.
 \param geom2 pointer to second input Geometry object.

 \return 0 if false; any other value if geom1 \e spatially \e covers geom2.

 \sa gaiaGeomCollCoveredBy

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollCovers (gaiaGeomCollPtr geom1,
					    gaiaGeomCollPtr geom2);

/** 
 Topology check: test if a Geometry covers another one (GEOSPreparedGeometry)

 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 pointer to first input Geometry object.
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 pointer to second input Geometry object.
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB

 \return 0 if false; any other value if geom1 \e spatially \e covers geom2.

 \sa gaiaGeomCollCovers

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedCovers (void *p_cache,
						    gaiaGeomCollPtr geom1,
						    unsigned char *blob1,
						    int size1,
						    gaiaGeomCollPtr geom2,
						    unsigned char *blob2,
						    int size2);

/**
 Topology check: test if a Geometry is covered by another one
                                            
 \param geom1 pointer to first input Geometry object.
 \param geom2 pointer to second input Geometry object.
                                               
 \return 0 if false; any other value if geom2 is \e spatially \e covered \e by
 geom1.

 \sa gaiaGeomCollCovers

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollCoveredBy (gaiaGeomCollPtr geom1,
					       gaiaGeomCollPtr geom2);

/**
 Topology check: test if a Geometry is covered by another one (GEOSPreparedGeometry)
                                            
 \param p_cache a memory pointer returned by spatialite_alloc_connection()
 \param geom1 pointer to first input Geometry object.
 \param blob1 the BLOB corresponding to the first Geometry
 \param size1 the size (in bytes) of the first BLOB
 \param geom2 pointer to second input Geometry object.
 \param blob2 the BLOB corresponding to the second Geometry
 \param size2 the size (in bytes) of the second BLOB
                                               
 \return 0 if false; any other value if geom2 is \e spatially \e covered \e by
 geom1.

 \sa gaiaGeomCollCovers

 \remark \b GEOS-ADVANCED support required.
 */
    GAIAGEO_DECLARE int gaiaGeomCollPreparedCoveredBy (void *p_cache,
						       gaiaGeomCollPtr geom1,
						       unsigned char *blob1,
						       int size1,
						       gaiaGeomCollPtr geom2,
						       unsigned char *blob2,
						       int size2);

/**
 Utility function: SquareGrid

 \param geom the Geometry to be covered by the Grid.
 \param origin_x the X ccordinate identifying the Grid Origin.
 \param origin_y the Y coordinate identifiying the Grid Origin.
 \param size the Grid cell-side size.
 \param only_edges if non-zero will return a MULTILINESTRING, otherwise it will
  return a MULTIPOLYGON containing square POLYGONs.
 
 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function will always return a MultiPolygon 
 \n NULL will be returned if any argument is invalid.

 \sa gaiaFreeGeomColl, gaiaTriangularGrid, gaiaHexagonalGrid

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSquareGrid()

 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSquareGrid (gaiaGeomCollPtr geom,
						    double origin_x,
						    double origin_y,
						    double size,
						    int only_edges);

/**
 Utility function: TriangularGrid

 \param geom the Geometry to be covered by the Grid.
 \param origin_x the X ccordinate identifying the Grid Origin.
 \param origin_y the Y coordinate identifiying the Grid Origin.
 \param size the Grid cell-side size.
 \param only_edges if non-zero will return a MULTILINESTRING, otherwise it will
  return a MULTIPOLYGON containing triangular POLYGONs.
 
 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function will always return a MultiPolygon 
 \n NULL will be returned if any argument is invalid.

 \sa gaiaFreeGeomColl, gaiaSquareGrid, gaiaHexagonalGrid

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaTriangularGrid()

 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaTriangularGrid (gaiaGeomCollPtr geom,
							double origin_x,
							double origin_y,
							double size,
							int only_edges);

/**
 Utility function: HexagonalGrid

 \param geom the Geometry to be covered by the Grid.
 \param origin_x the X ccordinate identifying the Grid Origin.
 \param origin_y the Y coordinate identifiying the Grid Origin.
 \param size the Grid cell-side size.
 \param only_edges if non-zero will return a MULTILINESTRING, otherwise it will
  return a MULTIPOLYGON containing hexagonal POLYGONs.
 
 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function will always return a MultiPolygon 
 \n NULL will be returned if any argument is invalid.

 \sa gaiaFreeGeomColl, gaiaSquareGrid, gaiaTriangularGrid

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaHexagonalGrid()

 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaHexagonalGrid (gaiaGeomCollPtr geom,
						       double origin_x,
						       double origin_y,
						       double size,
						       int only_edges);

#endif				/* end GEOS advanced features */

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
#ifdef GEOS_TRUNK
#endif

/**
 Delaunay Triangulation
                                            
 \param geom pointer to input Geometry object.
 \param tolerance optional snapping tolerance.
 \param only_edges if non-zero will return a MULTILINESTRING, otherwise it will
  return a MULTIPOLYGON containing triangular POLYGONs.
 
 \return the pointer to newly created Geometry object: NULL on failure.
 \n NULL will be returned if any argument is invalid.

 \sa gaiaFreeGeomColl, gaiaVoronojDiagram, gaiaConcaveHull

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaDelaunayTriangulation()

 \remark \b GEOS-TRUNK support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaDelaunayTriangulation (gaiaGeomCollPtr
							       geom,
							       double tolerance,
							       int only_edges);

/**
 Voronoj Diagram
                                            
 \param geom pointer to input Geometry object.
 \param extra_frame_size percent factor expanding the BBOX of input Geometry
 \param tolerance optional snapping tolerance.
 \param only_edges if non-zero will return a MULTILINESTRING, otherwise it will
  return a MULTIPOLYGON.
 
 \return the pointer to newly created Geometry object: NULL on failure.
 \n NULL will be returned if any argument is invalid.

 \sa gaiaFreeGeomColl, gaiaDelaunayTriangulation

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaVoronojDiagram()

 \remark \b GEOS-TRUNK support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaVoronojDiagram (gaiaGeomCollPtr geom,
							double extra_frame_size,
							double tolerance,
							int only_edges);

/**
 Concave Hull
                                            
 \param geom pointer to input Geometry object.
 \param factor multiplier used for filtering Delaunay triangles: please read the note.
 \param tolerance optional snapping tolerance.
 \param allow_holes if FALSE any interior hole will be suppressed.
 
 \return the pointer to newly created Geometry object (always of the Polygon type): 
  NULL on failure.
 \n NULL will be returned if any argument is invalid.

 \sa gaiaFreeGeomColl, gaiaDelaunayTriangulation

 \note This function will first create the Delauany Triangulation corresponding
  to input Geometry, determining at the same time the \b standard \b deviation
  for all edge's lengths.
 \n Then in a second pass all Delaunay's triangles will be filtered, and all
 triangles presenting at least one edge longer than \b standard \b deviation
 \b * \b factor will be discarded. 
 \n All filtered triangles will then be merged altogether so to create the Concave Hull.

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaConcaveHull()

 \remark \b GEOS-TRUNK support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaConcaveHull (gaiaGeomCollPtr geom,
						     double factor,
						     double tolerance,
						     int allow_holes);

#endif				/* end GEOS experimental features */

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
#ifdef ENABLE_LWGEOM
#endif

/**
 Utility function: MakeValid

 \param geom the input Geometry object.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function will attempt to create a valid representation of a given 
 invalid geometry without loosing any of the input vertices. 
 \n Already-valid geometries are returned without further intervention. 
 \n NULL will be returned if the passed argument is invalid.

 \sa gaiaFreeGeomColl, gaiaMakeValidDiscarded

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaMakeValid()

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaMakeValid (gaiaGeomCollPtr geom);

/**
 Utility function: MakeValidDiscarded

 \param geom the input Geometry object.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function will attempt to collect any invalid item (offending
 geometries) discarded by gaiaMakeValid while building a valid Geometry.
 \n Saving any discarded item could be useful for a finer (manual) adjustment.
 \n NULL will be returned if gaiaMakeValid hasn't identified any offending item 
 to be discarded during the validation.

 \sa gaiaFreeGeomColl, gaiaMakeValid

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaMakeValidDiscarded()

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaMakeValidDiscarded (gaiaGeomCollPtr
							    geom);

/**
 Utility function: Segmentize

 \param geom the input Geometry object.
 \param dist the meximum segment length.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function will return a modified geometry having no segment longer than the given distance. 
 \n Distance computation is performed in 2d only.
 \n all Points or segments shorter than 'dist' will be returned without further intervention. 
 \n NULL will be returned if the passed argument is invalid.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSegmentize()

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSegmentize (gaiaGeomCollPtr geom,
						    double dist);

/**
 Utility function: Azimuth

 \param xa the X ccordinate of PointA.
 \param ya the Y coordinate of PointA.
 \param xb the X ccordinate of PointB.
 \param yb the Y coordinate of PointB.
 \param azimuth on completion this variable will contain the angle in radians from 
  the horizontal of the vector defined by pointA and pointB. 
 \n Angle is computed clockwise from down-to-up: on the clock: 12=0; 3=PI/2; 6=PI; 9=3PI/2.

 \return 0 on failure: any other value on success

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE int gaiaAzimuth (double xa, double ya, double xb,
				     double yb, double *azimuth);

/**
 Utility function: GeoHash

 \param geom the input geometry.
 \param precision the expected precision: if <= 0 will be automatically determined.

 \return NULL on failure: a null-terminated text string on success

 \note you are responsible to free (before or after) any text string returned
  by gaiaGeoHash()

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE char *gaiaGeoHash (gaiaGeomCollPtr geom, int precision);

/**
 Utility function: AsX3D

 \param geom the input geometry.
 \param srs the WKT SRS definition.
 \param precision the expected precision (coord decimal digits).
 \param options 
 \param refid the X3D namespace

 \return NULL on failure: a null-terminated text string on success

 \note you are responsible to free (before or after) any text string returned
  by gaiaAsX3D()

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE char *gaiaAsX3D (gaiaGeomCollPtr geom, const char *srs,
				     int precision, int options,
				     const char *refid);

/**
 Calculates the minimum 3D distance intercurring between two Geometry objects

 \param geom1 the first Geometry object 
 \param geom2 the second Geometry object 
 \param dist on completion this variable will contain the calculated distance

 \return 0 on failure: any other value on success.

 \sa gaiaGeomCollDistance, gaiaMaxDistance, gaia3DMaxDisance

 \note this function computes the 3D cartesian distance (if Z is supported)

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE int gaia3DDistance (gaiaGeomCollPtr geom1,
					gaiaGeomCollPtr geom2, double *dist);

/**
 Calculates the maximum 2D distance intercurring between two Geometry objects

 \param geom1 the first Geometry object 
 \param geom2 the second Geometry object 
 \param dist on completion this variable will contain the calculated distance

 \return 0 on failure: any other value on success.

 \sa gaiaGeomCollDistance, gaia3DDistance, gaia3DMaxDistance

 \note this function computes the 2D maximum cartesian distance (Z is always ignored)

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE int gaiaMaxDistance (gaiaGeomCollPtr geom1,
					 gaiaGeomCollPtr geom2, double *dist);

/**
 Calculates the maximum 3D distance intercurring between two Geometry objects

 \param geom1 the first Geometry object 
 \param geom2 the second Geometry object 
 \param dist on completion this variable will contain the calculated distance

 \return 0 on failure: any other value on success.

 \sa gaiaGeomCollDistance, gaia3DDistance, gaiaMaxDistance

 \note this function computes the 3D maximum cartesian distance (if Z is supported)

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE int gaia3DMaxDistance (gaiaGeomCollPtr geom1,
					   gaiaGeomCollPtr geom2, double *dist);

/**
 Utility function: Split

 \param input the input Geometry object.
 \param blade the blade Geometry object.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n The function supports splitting a line by point, a line by line, a polygon by line.

 \sa gaiaFreeGeomColl, gaiaSplitLeft, gaiaSplitRight

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSplit()
 
 \note gaiaSplit will return both the \b left and the \b right split halves at the same time.

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSplit (gaiaGeomCollPtr input,
					       gaiaGeomCollPtr blade);

/**
 Utility function: SplitLeft

 \param input the input Geometry object.
 \param blade the blade Geometry object.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n The function supports splitting a line by point, a line by line, a polygon by line.

 \sa gaiaFreeGeomColl, gaiaSplit, gaiaSplitRight

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSplitLeft()
 
 \note gaiaSplitLeft will only return the \b left split half; NULL may be eventually
 returned if empty.

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSplitLeft (gaiaGeomCollPtr input,
						   gaiaGeomCollPtr blade);

/**
 Utility function: SplitRight

 \param input the input Geometry object.
 \param blade the blade Geometry object.

 \return the pointer to newly created Geometry object: NULL on failure.
 \n The function supports splitting a line by point, a line by line, a polygon by line.

 \sa gaiaFreeGeomColl, gaiaSplit, gaiaSplitLeft

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSplitRight()
 
 \note gaiaSplitLeft will only return the \b right split half; NULL may be eventually
 returned if empty.

 \remark \b LWGEOM support required.
 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSplitRight (gaiaGeomCollPtr input,
						    gaiaGeomCollPtr blade);


#endif				/* end LWGEOM support */

#endif				/* end including GEOS */

/**
 Utility function: SnapToGrid

 \param geom the input Geometry object.
 \param origin_x the X ccordinate identifying the Grid Origin.
 \param origin_y the Y coordinate identifiying the Grid Origin.
 \param origin_z the Z ccordinate identifying the Grid Origin.
 \param origin_m the M coordinate identifiying the Grid Origin.
 \param size_x Grid cell size (X axis).
 \param size_y Grid cell size (Y axis).
 \param size_z Grid cell size (Z axis).
 \param size_m Grid cell size (M axis).
 
 \return the pointer to newly created Geometry object: NULL on failure.
 \n this function will return a modified geometry having all points snapped to a regular Grid
 defined by its origin and cell size.
 \n Consecutive points falling on the same cell will be removed, eventually returning NULL if 
 \n output points are not enough to define a geometry of the given type.
 \n Collapsed geometries in a collection are stripped from it.
 \n Specify 0 as size for any dimension you don't want to snap to a grid. 
 \n NULL will be returned if the passed argument is invalid.

 \sa gaiaFreeGeomColl

 \note you are responsible to destroy (before or after) any allocated Geometry,
 this including any Geometry returned by gaiaSnapToGrid()

 */
    GAIAGEO_DECLARE gaiaGeomCollPtr gaiaSnapToGrid (gaiaGeomCollPtr geom,
						    double origin_x,
						    double origin_y,
						    double origin_z,
						    double origin_m,
						    double size_x,
						    double size_y,
						    double size_z,
						    double size_m);

#ifdef __cplusplus
}
#endif

#endif				/* _GG_ADVANCED_H */
