/*

 gg_wkt.c -- Gaia common support for WKT encoded geometries
  
 version 2.4, 2009 September 17

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
 
Portions created by the Initial Developer are Copyright (C) 2008
the Initial Developer. All Rights Reserved.

Contributor(s):
Klaus Foerster klaus.foerster@svg.cc

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

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef SPL_AMALGAMATION		/* spatialite-amalgamation */
#include <spatialite/sqlite3ext.h>
#else
#include <sqlite3ext.h>
#endif

#include <spatialite/gaiageo.h>

static void
gaiaOutClean (char *buffer)
{
/* cleans unneeded trailing zeros */
    int i;
    for (i = strlen (buffer) - 1; i > 0; i--)
      {
	  if (buffer[i] == '0')
	      buffer[i] = '\0';
	  else
	      break;
      }
    if (buffer[i] == '.')
	buffer[i] = '\0';
}

GAIAGEO_DECLARE void
gaiaOutBufferInitialize (gaiaOutBufferPtr buf)
{
/* initializing a dynamically growing output buffer */
    buf->Buffer = NULL;
    buf->WriteOffset = 0;
    buf->BufferSize = 0;
    buf->Error = 0;
}

GAIAGEO_DECLARE void
gaiaOutBufferReset (gaiaOutBufferPtr buf)
{
/* cleaning a dynamically growing output buffer */
    if (buf->Buffer)
	free (buf->Buffer);
    buf->Buffer = NULL;
    buf->WriteOffset = 0;
    buf->BufferSize = 0;
    buf->Error = 0;
}

GAIAGEO_DECLARE void
gaiaAppendToOutBuffer (gaiaOutBufferPtr buf, const char *text)
{
/* appending a text string */
    int len = strlen (text);
    int free_size = buf->BufferSize - buf->WriteOffset;
    if ((len + 1) > free_size)
      {
	  /* we must allocate a bigger buffer */
	  int new_size;
	  char *new_buf;
	  if (buf->BufferSize == 0)
	      new_size = 1024;
	  else if (buf->BufferSize <= 4196)
	      new_size = buf->BufferSize + (len + 1) + 4196;
	  else if (buf->BufferSize <= 65536)
	      new_size = buf->BufferSize + (len + 1) + 65536;
	  else
	      new_size = buf->BufferSize + (len + 1) + (1024 * 1024);
	  new_buf = malloc (new_size);
	  if (!new_buf)
	    {
		buf->Error = 1;
		return;
	    }
	  memcpy (new_buf, buf->Buffer, buf->WriteOffset);
	  if (buf->Buffer)
	      free (buf->Buffer);
	  buf->Buffer = new_buf;
	  buf->BufferSize = new_size;
      }
    strcpy (buf->Buffer + buf->WriteOffset, text);
    buf->WriteOffset += len;
}

static void
gaiaOutPoint (gaiaOutBufferPtr out_buf, gaiaPointPtr point)
{
/* formats a WKT POINT */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    sprintf (buf_x, "%1.6f", point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%1.6f", point->Y);
    gaiaOutClean (buf_y);
    sprintf (buf, "%s %s", buf_x, buf_y);
    gaiaAppendToOutBuffer (out_buf, buf);
}

GAIAGEO_DECLARE void
gaiaOutPointZ (gaiaOutBufferPtr out_buf, gaiaPointPtr point)
{
/* formats a WKT POINTZ */
    char buf_x[128];
    char buf_y[128];
    char buf_z[128];
    char buf[512];
    sprintf (buf_x, "%1.6f", point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%1.6f", point->Y);
    gaiaOutClean (buf_y);
    sprintf (buf_z, "%1.6f", point->Z);
    gaiaOutClean (buf_z);
    sprintf (buf, "%s %s %s", buf_x, buf_y, buf_z);
    gaiaAppendToOutBuffer (out_buf, buf);
}

static void
gaiaOutPointM (gaiaOutBufferPtr out_buf, gaiaPointPtr point)
{
/* formats a WKT POINTM */
    char buf_x[128];
    char buf_y[128];
    char buf_m[128];
    char buf[512];
    sprintf (buf_x, "%1.6f", point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%1.6f", point->Y);
    gaiaOutClean (buf_y);
    sprintf (buf_m, "%1.6f", point->M);
    gaiaOutClean (buf_m);
    sprintf (buf, "%s %s %s", buf_x, buf_y, buf_m);
    gaiaAppendToOutBuffer (out_buf, buf);
}

static void
gaiaOutPointZM (gaiaOutBufferPtr out_buf, gaiaPointPtr point)
{
/* formats a WKT POINTZM */
    char buf_x[128];
    char buf_y[128];
    char buf_z[128];
    char buf_m[128];
    char buf[1024];
    sprintf (buf_x, "%1.6f", point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%1.6f", point->Y);
    gaiaOutClean (buf_y);
    sprintf (buf_z, "%1.6f", point->Z);
    gaiaOutClean (buf_z);
    sprintf (buf_m, "%1.6f", point->M);
    gaiaOutClean (buf_m);
    sprintf (buf, "%s %s %s %s", buf_x, buf_y, buf_z, buf_m);
    gaiaAppendToOutBuffer (out_buf, buf);
}

static void
gaiaOutLinestring (gaiaOutBufferPtr out_buf, gaiaLinestringPtr line)
{
/* formats a WKT LINESTRING */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    double x;
    double y;
    int iv;
    for (iv = 0; iv < line->Points; iv++)
      {
	  gaiaGetPoint (line->Coords, iv, &x, &y);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  if (iv > 0)
	      sprintf (buf, ", %s %s", buf_x, buf_y);
	  else
	      sprintf (buf, "%s %s", buf_x, buf_y);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
}

GAIAGEO_DECLARE void
gaiaOutLinestringZ (gaiaOutBufferPtr out_buf, gaiaLinestringPtr line)
{
/* formats a WKT LINESTRINGZ */
    char buf_x[128];
    char buf_y[128];
    char buf_z[128];
    char buf[512];
    double x;
    double y;
    double z;
    int iv;
    for (iv = 0; iv < line->Points; iv++)
      {
	  gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  sprintf (buf_z, "%1.6f", z);
	  gaiaOutClean (buf_z);
	  if (iv > 0)
	      sprintf (buf, ", %s %s %s", buf_x, buf_y, buf_z);
	  else
	      sprintf (buf, "%s %s %s", buf_x, buf_y, buf_z);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
}

static void
gaiaOutLinestringM (gaiaOutBufferPtr out_buf, gaiaLinestringPtr line)
{
/* formats a WKT LINESTRINGM */
    char buf_x[128];
    char buf_y[128];
    char buf_m[128];
    char buf[512];
    double x;
    double y;
    double m;
    int iv;
    for (iv = 0; iv < line->Points; iv++)
      {
	  gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  sprintf (buf_m, "%1.6f", m);
	  gaiaOutClean (buf_m);
	  if (iv > 0)
	      sprintf (buf, ", %s %s %s", buf_x, buf_y, buf_m);
	  else
	      sprintf (buf, "%s %s %s", buf_x, buf_y, buf_m);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
}

static void
gaiaOutLinestringZM (gaiaOutBufferPtr out_buf, gaiaLinestringPtr line)
{
/* formats a WKT LINESTRINGZM */
    char buf_x[128];
    char buf_y[128];
    char buf_z[128];
    char buf_m[128];
    char buf[1024];
    double x;
    double y;
    double z;
    double m;
    int iv;
    for (iv = 0; iv < line->Points; iv++)
      {
	  gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  sprintf (buf_z, "%1.6f", z);
	  gaiaOutClean (buf_z);
	  sprintf (buf_m, "%1.6f", m);
	  gaiaOutClean (buf_m);
	  if (iv > 0)
	      sprintf (buf, ", %s %s %s %s", buf_x, buf_y, buf_z, buf_m);
	  else
	      sprintf (buf, "%s %s %s %s", buf_x, buf_y, buf_z, buf_m);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
}

static void
gaiaOutPolygon (gaiaOutBufferPtr out_buf, gaiaPolygonPtr polyg)
{
/* formats a WKT POLYGON */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    int ib;
    int iv;
    double x;
    double y;
    gaiaRingPtr ring = polyg->Exterior;
    for (iv = 0; iv < ring->Points; iv++)
      {
	  gaiaGetPoint (ring->Coords, iv, &x, &y);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  if (iv == 0)
	      sprintf (buf, "(%s %s", buf_x, buf_y);
	  else if (iv == (ring->Points - 1))
	      sprintf (buf, ", %s %s)", buf_x, buf_y);
	  else
	      sprintf (buf, ", %s %s", buf_x, buf_y);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    for (ib = 0; ib < polyg->NumInteriors; ib++)
      {
	  ring = polyg->Interiors + ib;
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		gaiaGetPoint (ring->Coords, iv, &x, &y);
		sprintf (buf_x, "%1.6f", x);
		gaiaOutClean (buf_x);
		sprintf (buf_y, "%1.6f", y);
		gaiaOutClean (buf_y);
		if (iv == 0)
		    sprintf (buf, ", (%s %s", buf_x, buf_y);
		else if (iv == (ring->Points - 1))
		    sprintf (buf, ", %s %s)", buf_x, buf_y);
		else
		    sprintf (buf, ", %s %s", buf_x, buf_y);
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
      }
}

GAIAGEO_DECLARE void
gaiaOutPolygonZ (gaiaOutBufferPtr out_buf, gaiaPolygonPtr polyg)
{
/* formats a WKT POLYGONZ */
    char buf_x[128];
    char buf_y[128];
    char buf_z[128];
    char buf[512];
    int ib;
    int iv;
    double x;
    double y;
    double z;
    gaiaRingPtr ring = polyg->Exterior;
    for (iv = 0; iv < ring->Points; iv++)
      {
	  gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  sprintf (buf_z, "%1.6f", z);
	  gaiaOutClean (buf_z);
	  if (iv == 0)
	      sprintf (buf, "(%s %s %s", buf_x, buf_y, buf_z);
	  else if (iv == (ring->Points - 1))
	      sprintf (buf, ", %s %s %s)", buf_x, buf_y, buf_z);
	  else
	      sprintf (buf, ", %s %s %s", buf_x, buf_y, buf_z);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    for (ib = 0; ib < polyg->NumInteriors; ib++)
      {
	  ring = polyg->Interiors + ib;
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
		sprintf (buf_x, "%1.6f", x);
		gaiaOutClean (buf_x);
		sprintf (buf_y, "%1.6f", y);
		gaiaOutClean (buf_y);
		sprintf (buf_z, "%1.6f", z);
		gaiaOutClean (buf_z);
		if (iv == 0)
		    sprintf (buf, ", (%s %s %s", buf_x, buf_y, buf_z);
		else if (iv == (ring->Points - 1))
		    sprintf (buf, ", %s %s %s)", buf_x, buf_y, buf_z);
		else
		    sprintf (buf, ", %s %s %s", buf_x, buf_y, buf_z);
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
      }
}

static void
gaiaOutPolygonM (gaiaOutBufferPtr out_buf, gaiaPolygonPtr polyg)
{
/* formats a WKT POLYGONM */
    char buf_x[128];
    char buf_y[128];
    char buf_m[128];
    char buf[512];
    int ib;
    int iv;
    double x;
    double y;
    double m;
    gaiaRingPtr ring = polyg->Exterior;
    for (iv = 0; iv < ring->Points; iv++)
      {
	  gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  sprintf (buf_m, "%1.6f", m);
	  gaiaOutClean (buf_m);
	  if (iv == 0)
	      sprintf (buf, "(%s %s %s", buf_x, buf_y, buf_m);
	  else if (iv == (ring->Points - 1))
	      sprintf (buf, ", %s %s %s)", buf_x, buf_y, buf_m);
	  else
	      sprintf (buf, ", %s %s %s", buf_x, buf_y, buf_m);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    for (ib = 0; ib < polyg->NumInteriors; ib++)
      {
	  ring = polyg->Interiors + ib;
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
		sprintf (buf_x, "%1.6f", x);
		gaiaOutClean (buf_x);
		sprintf (buf_y, "%1.6f", y);
		gaiaOutClean (buf_y);
		sprintf (buf_m, "%1.6f", m);
		gaiaOutClean (buf_m);
		if (iv == 0)
		    sprintf (buf, ", (%s %s %s", buf_x, buf_y, buf_m);
		else if (iv == (ring->Points - 1))
		    sprintf (buf, ", %s %s %s)", buf_x, buf_y, buf_m);
		else
		    sprintf (buf, ", %s %s %s", buf_x, buf_y, buf_m);
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
      }
}

static void
gaiaOutPolygonZM (gaiaOutBufferPtr out_buf, gaiaPolygonPtr polyg)
{
/* formats a WKT POLYGONZM */
    char buf_x[128];
    char buf_y[128];
    char buf_z[128];
    char buf_m[128];
    char buf[1024];
    int ib;
    int iv;
    double x;
    double y;
    double z;
    double m;
    gaiaRingPtr ring = polyg->Exterior;
    for (iv = 0; iv < ring->Points; iv++)
      {
	  gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
	  sprintf (buf_x, "%1.6f", x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%1.6f", y);
	  gaiaOutClean (buf_y);
	  sprintf (buf_z, "%1.6f", z);
	  gaiaOutClean (buf_z);
	  sprintf (buf_m, "%1.6f", m);
	  gaiaOutClean (buf_m);
	  if (iv == 0)
	      sprintf (buf, "(%s %s %s %s", buf_x, buf_y, buf_z, buf_m);
	  else if (iv == (ring->Points - 1))
	      sprintf (buf, ", %s %s %s %s)", buf_x, buf_y, buf_z, buf_m);
	  else
	      sprintf (buf, ", %s %s %s %s", buf_x, buf_y, buf_z, buf_m);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    for (ib = 0; ib < polyg->NumInteriors; ib++)
      {
	  ring = polyg->Interiors + ib;
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
		sprintf (buf_x, "%1.6f", x);
		gaiaOutClean (buf_x);
		sprintf (buf_y, "%1.6f", y);
		gaiaOutClean (buf_y);
		sprintf (buf_z, "%1.6f", z);
		gaiaOutClean (buf_z);
		sprintf (buf_m, "%1.6f", m);
		gaiaOutClean (buf_m);
		if (iv == 0)
		    sprintf (buf, ", (%s %s %s %s", buf_x, buf_y, buf_z, buf_m);
		else if (iv == (ring->Points - 1))
		    sprintf (buf, ", %s %s %s %s)", buf_x, buf_y, buf_z, buf_m);
		else
		    sprintf (buf, ", %s %s %s %s", buf_x, buf_y, buf_z, buf_m);
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
      }
}

GAIAGEO_DECLARE void
gaiaOutWkt (gaiaOutBufferPtr out_buf, gaiaGeomCollPtr geom)
{
/* prints the WKT representation of current geometry */
    int pts = 0;
    int lns = 0;
    int pgs = 0;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    if (!geom)
	return;
    point = geom->FirstPoint;
    while (point)
      {
	  /* counting how many POINTs are there */
	  pts++;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* counting how many LINESTRINGs are there */
	  lns++;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* counting how many POLYGONs are there */
	  pgs++;
	  polyg = polyg->Next;
      }
    if ((pts + lns + pgs) == 1
	&& (geom->DeclaredType == GAIA_POINT
	    || geom->DeclaredType == GAIA_LINESTRING
	    || geom->DeclaredType == GAIA_POLYGON))
      {
	  /* we have only one elementary geometry */
	  point = geom->FirstPoint;
	  while (point)
	    {
		if (point->DimensionModel == GAIA_XY_Z)
		  {
		      /* processing POINTZ */
		      gaiaAppendToOutBuffer (out_buf, "POINT Z(");
		      gaiaOutPointZ (out_buf, point);
		  }
		else if (point->DimensionModel == GAIA_XY_M)
		  {
		      /* processing POINTM */
		      gaiaAppendToOutBuffer (out_buf, "POINT M(");
		      gaiaOutPointM (out_buf, point);
		  }
		else if (point->DimensionModel == GAIA_XY_Z_M)
		  {
		      /* processing POINTZM */
		      gaiaAppendToOutBuffer (out_buf, "POINT ZM(");
		      gaiaOutPointZM (out_buf, point);
		  }
		else
		  {
		      /* processing POINT */
		      gaiaAppendToOutBuffer (out_buf, "POINT(");
		      gaiaOutPoint (out_buf, point);
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
		point = point->Next;
	    }
	  line = geom->FirstLinestring;
	  while (line)
	    {
		if (line->DimensionModel == GAIA_XY_Z)
		  {
		      /* processing LINESTRINGZ */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRING Z(");
		      gaiaOutLinestringZ (out_buf, line);
		  }
		else if (line->DimensionModel == GAIA_XY_M)
		  {
		      /* processing LINESTRINGM */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRING M(");
		      gaiaOutLinestringM (out_buf, line);
		  }
		else if (line->DimensionModel == GAIA_XY_Z_M)
		  {
		      /* processing LINESTRINGZM */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRING ZM(");
		      gaiaOutLinestringZM (out_buf, line);
		  }
		else
		  {
		      /* processing LINESTRING */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
		      gaiaOutLinestring (out_buf, line);
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
		line = line->Next;
	    }
	  polyg = geom->FirstPolygon;
	  while (polyg)
	    {
		if (polyg->DimensionModel == GAIA_XY_Z)
		  {
		      /* processing POLYGONZ */
		      gaiaAppendToOutBuffer (out_buf, "POLYGON Z(");
		      gaiaOutPolygonZ (out_buf, polyg);
		  }
		else if (polyg->DimensionModel == GAIA_XY_M)
		  {
		      /* processing POLYGONM */
		      gaiaAppendToOutBuffer (out_buf, "POLYGON M(");
		      gaiaOutPolygonM (out_buf, polyg);
		  }
		else if (polyg->DimensionModel == GAIA_XY_Z_M)
		  {
		      /* processing POLYGONZM */
		      gaiaAppendToOutBuffer (out_buf, "POLYGON ZM(");
		      gaiaOutPolygonZM (out_buf, polyg);
		  }
		else
		  {
		      /* processing POLYGON */
		      gaiaAppendToOutBuffer (out_buf, "POLYGON(");
		      gaiaOutPolygon (out_buf, polyg);
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
		polyg = polyg->Next;
	    }
      }
    else
      {
	  /* we have some kind of complex geometry */
	  if (pts > 0 && lns == 0 && pgs == 0
	      && geom->DeclaredType == GAIA_MULTIPOINT)
	    {
		/* some kind of MULTIPOINT */
		if (geom->DimensionModel == GAIA_XY_Z)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOINT Z(");
		else if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOINT M(");
		else if (geom->DimensionModel == GAIA_XY_Z_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOINT ZM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOINT(");
		point = geom->FirstPoint;
		while (point)
		  {
		      if (point->DimensionModel == GAIA_XY_Z)
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPointZ (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_M)
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPointM (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_Z_M)
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPointZM (out_buf, point);
			}
		      else
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPoint (out_buf, point);
			}
		      point = point->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
	  else if (pts == 0 && lns > 0 && pgs == 0
		   && geom->DeclaredType == GAIA_MULTILINESTRING)
	    {
		/* some kind of MULTILINESTRING */
		if (geom->DimensionModel == GAIA_XY_Z)
		    gaiaAppendToOutBuffer (out_buf, "MULTILINESTRING Z(");
		else if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTILINESTRING M(");
		else if (geom->DimensionModel == GAIA_XY_Z_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTILINESTRING ZM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "MULTILINESTRING(");
		line = geom->FirstLinestring;
		while (line)
		  {
		      if (line != geom->FirstLinestring)
			  gaiaAppendToOutBuffer (out_buf, ", (");
		      else
			  gaiaAppendToOutBuffer (out_buf, "(");
		      if (line->DimensionModel == GAIA_XY_Z)
			{
			    gaiaOutLinestringZ (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (line->DimensionModel == GAIA_XY_M)
			{
			    gaiaOutLinestringM (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (line->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaOutLinestringZM (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else
			{
			    gaiaOutLinestring (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      line = line->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
	  else if (pts == 0 && lns == 0 && pgs > 0
		   && geom->DeclaredType == GAIA_MULTIPOLYGON)
	    {
		/* some kind of MULTIPOLYGON */
		if (geom->DimensionModel == GAIA_XY_Z)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOLYGON Z(");
		else if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOLYGON M(");
		else if (geom->DimensionModel == GAIA_XY_Z_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOLYGON ZM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOLYGON(");
		polyg = geom->FirstPolygon;
		while (polyg)
		  {
		      if (polyg != geom->FirstPolygon)
			  gaiaAppendToOutBuffer (out_buf, ", (");
		      else
			  gaiaAppendToOutBuffer (out_buf, "(");
		      if (polyg->DimensionModel == GAIA_XY_Z)
			{
			    gaiaOutPolygonZ (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (polyg->DimensionModel == GAIA_XY_M)
			{
			    gaiaOutPolygonM (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (polyg->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaOutPolygonZM (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else
			{
			    gaiaOutPolygon (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      polyg = polyg->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
	  else
	    {
		/* some kind of GEOMETRYCOLLECTION */
		int ie = 0;
		if (geom->DimensionModel == GAIA_XY_Z)
		    gaiaAppendToOutBuffer (out_buf, "GEOMETRYCOLLECTION Z(");
		else if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "GEOMETRYCOLLECTION M(");
		else if (geom->DimensionModel == GAIA_XY_Z_M)
		    gaiaAppendToOutBuffer (out_buf, "GEOMETRYCOLLECTION ZM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "GEOMETRYCOLLECTION(");
		point = geom->FirstPoint;
		while (point)
		  {
		      /* processing POINTs */
		      if (ie > 0)
			  gaiaAppendToOutBuffer (out_buf, ", ");
		      ie++;
		      if (point->DimensionModel == GAIA_XY_Z)
			{
			    gaiaAppendToOutBuffer (out_buf, "POINT Z(");
			    gaiaOutPointZ (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POINT M(");
			    gaiaOutPointM (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POINT ZM(");
			    gaiaOutPointZM (out_buf, point);
			}
		      else
			{
			    gaiaAppendToOutBuffer (out_buf, "POINT(");
			    gaiaOutPoint (out_buf, point);
			}
		      gaiaAppendToOutBuffer (out_buf, ")");
		      point = point->Next;
		  }
		line = geom->FirstLinestring;
		while (line)
		  {
		      /* processing LINESTRINGs */
		      if (ie > 0)
			  gaiaAppendToOutBuffer (out_buf, ", ");
		      ie++;
		      if (line->DimensionModel == GAIA_XY_Z)
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRING Z(");
			    gaiaOutLinestringZ (out_buf, line);
			}
		      else if (line->DimensionModel == GAIA_XY_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRING M(");
			    gaiaOutLinestringM (out_buf, line);
			}
		      else if (line->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRING ZM(");
			    gaiaOutLinestringZM (out_buf, line);
			}
		      else
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
			    gaiaOutLinestring (out_buf, line);
			}
		      gaiaAppendToOutBuffer (out_buf, ")");
		      line = line->Next;
		  }
		polyg = geom->FirstPolygon;
		while (polyg)
		  {
		      /* processing POLYGONs */
		      if (ie > 0)
			  gaiaAppendToOutBuffer (out_buf, ", ");
		      ie++;
		      if (polyg->DimensionModel == GAIA_XY_Z)
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGON Z(");
			    gaiaOutPolygonZ (out_buf, polyg);
			}
		      else if (polyg->DimensionModel == GAIA_XY_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGON M(");
			    gaiaOutPolygonM (out_buf, polyg);
			}
		      else if (polyg->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGON ZM(");
			    gaiaOutPolygonZM (out_buf, polyg);
			}
		      else
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGON(");
			    gaiaOutPolygon (out_buf, polyg);
			}
		      gaiaAppendToOutBuffer (out_buf, ")");
		      polyg = polyg->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
      }
}

GAIAGEO_DECLARE void
gaiaToEWKT (gaiaOutBufferPtr out_buf, gaiaGeomCollPtr geom)
{
/* prints the EWKT representation of current geometry */
    char buf[128];
    int pts = 0;
    int lns = 0;
    int pgs = 0;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    if (!geom)
	return;
    sprintf (buf, "SRID=%d;", geom->Srid);
    gaiaAppendToOutBuffer (out_buf, buf);
    point = geom->FirstPoint;
    while (point)
      {
	  /* counting how many POINTs are there */
	  pts++;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* counting how many LINESTRINGs are there */
	  lns++;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* counting how many POLYGONs are there */
	  pgs++;
	  polyg = polyg->Next;
      }
    if ((pts + lns + pgs) == 1
	&& (geom->DeclaredType == GAIA_POINT
	    || geom->DeclaredType == GAIA_LINESTRING
	    || geom->DeclaredType == GAIA_POLYGON))
      {
	  /* we have only one elementary geometry */
	  point = geom->FirstPoint;
	  while (point)
	    {
		if (point->DimensionModel == GAIA_XY_Z)
		  {
		      /* processing POINTZ */
		      gaiaAppendToOutBuffer (out_buf, "POINT(");
		      gaiaOutPointZ (out_buf, point);
		  }
		else if (point->DimensionModel == GAIA_XY_M)
		  {
		      /* processing POINTM */
		      gaiaAppendToOutBuffer (out_buf, "POINTM(");
		      gaiaOutPointM (out_buf, point);
		  }
		else if (point->DimensionModel == GAIA_XY_Z_M)
		  {
		      /* processing POINTZM */
		      gaiaAppendToOutBuffer (out_buf, "POINT(");
		      gaiaOutPointZM (out_buf, point);
		  }
		else
		  {
		      /* processing POINT */
		      gaiaAppendToOutBuffer (out_buf, "POINT(");
		      gaiaOutPoint (out_buf, point);
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
		point = point->Next;
	    }
	  line = geom->FirstLinestring;
	  while (line)
	    {
		if (line->DimensionModel == GAIA_XY_Z)
		  {
		      /* processing LINESTRINGZ */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
		      gaiaOutLinestringZ (out_buf, line);
		  }
		else if (line->DimensionModel == GAIA_XY_M)
		  {
		      /* processing LINESTRINGM */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRINGM(");
		      gaiaOutLinestringM (out_buf, line);
		  }
		else if (line->DimensionModel == GAIA_XY_Z_M)
		  {
		      /* processing LINESTRINGZM */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
		      gaiaOutLinestringZM (out_buf, line);
		  }
		else
		  {
		      /* processing LINESTRING */
		      gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
		      gaiaOutLinestring (out_buf, line);
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
		line = line->Next;
	    }
	  polyg = geom->FirstPolygon;
	  while (polyg)
	    {
		if (polyg->DimensionModel == GAIA_XY_Z)
		  {
		      /* processing POLYGONZ */
		      gaiaAppendToOutBuffer (out_buf, "POLYGON(");
		      gaiaOutPolygonZ (out_buf, polyg);
		  }
		else if (polyg->DimensionModel == GAIA_XY_M)
		  {
		      /* processing POLYGONM */
		      gaiaAppendToOutBuffer (out_buf, "POLYGONM(");
		      gaiaOutPolygonM (out_buf, polyg);
		  }
		else if (polyg->DimensionModel == GAIA_XY_Z_M)
		  {
		      /* processing POLYGONZM */
		      gaiaAppendToOutBuffer (out_buf, "POLYGON(");
		      gaiaOutPolygonZM (out_buf, polyg);
		  }
		else
		  {
		      /* processing POLYGON */
		      gaiaAppendToOutBuffer (out_buf, "POLYGON(");
		      gaiaOutPolygon (out_buf, polyg);
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
		polyg = polyg->Next;
	    }
      }
    else
      {
	  /* we have some kind of complex geometry */
	  if (pts > 0 && lns == 0 && pgs == 0
	      && geom->DeclaredType == GAIA_MULTIPOINT)
	    {
		/* some kind of MULTIPOINT */
		if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOINTM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOINT(");
		point = geom->FirstPoint;
		while (point)
		  {
		      if (point->DimensionModel == GAIA_XY_Z)
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPointZ (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_M)
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPointM (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_Z_M)
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPointZM (out_buf, point);
			}
		      else
			{
			    if (point != geom->FirstPoint)
				gaiaAppendToOutBuffer (out_buf, ", ");
			    gaiaOutPoint (out_buf, point);
			}
		      point = point->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
	  else if (pts == 0 && lns > 0 && pgs == 0
		   && geom->DeclaredType == GAIA_MULTILINESTRING)
	    {
		/* some kind of MULTILINESTRING */
		if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTILINESTRINGM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "MULTILINESTRING(");
		line = geom->FirstLinestring;
		while (line)
		  {
		      if (line != geom->FirstLinestring)
			  gaiaAppendToOutBuffer (out_buf, ", (");
		      else
			  gaiaAppendToOutBuffer (out_buf, "(");
		      if (line->DimensionModel == GAIA_XY_Z)
			{
			    gaiaOutLinestringZ (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (line->DimensionModel == GAIA_XY_M)
			{
			    gaiaOutLinestringM (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (line->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaOutLinestringZM (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else
			{
			    gaiaOutLinestring (out_buf, line);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      line = line->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
	  else if (pts == 0 && lns == 0 && pgs > 0
		   && geom->DeclaredType == GAIA_MULTIPOLYGON)
	    {
		/* some kind of MULTIPOLYGON */
		if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOLYGONM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "MULTIPOLYGON(");
		polyg = geom->FirstPolygon;
		while (polyg)
		  {
		      if (polyg != geom->FirstPolygon)
			  gaiaAppendToOutBuffer (out_buf, ", (");
		      else
			  gaiaAppendToOutBuffer (out_buf, "(");
		      if (polyg->DimensionModel == GAIA_XY_Z)
			{
			    gaiaOutPolygonZ (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (polyg->DimensionModel == GAIA_XY_M)
			{
			    gaiaOutPolygonM (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else if (polyg->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaOutPolygonZM (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      else
			{
			    gaiaOutPolygon (out_buf, polyg);
			    gaiaAppendToOutBuffer (out_buf, ")");
			}
		      polyg = polyg->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
	  else
	    {
		/* some kind of GEOMETRYCOLLECTION */
		int ie = 0;
		if (geom->DimensionModel == GAIA_XY_M)
		    gaiaAppendToOutBuffer (out_buf, "GEOMETRYCOLLECTIONM(");
		else
		    gaiaAppendToOutBuffer (out_buf, "GEOMETRYCOLLECTION(");
		point = geom->FirstPoint;
		while (point)
		  {
		      /* processing POINTs */
		      if (ie > 0)
			  gaiaAppendToOutBuffer (out_buf, ", ");
		      ie++;
		      if (point->DimensionModel == GAIA_XY_Z)
			{
			    gaiaAppendToOutBuffer (out_buf, "POINT(");
			    gaiaOutPointZ (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POINTM(");
			    gaiaOutPointM (out_buf, point);
			}
		      else if (point->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POINT(");
			    gaiaOutPointZM (out_buf, point);
			}
		      else
			{
			    gaiaAppendToOutBuffer (out_buf, "POINT(");
			    gaiaOutPoint (out_buf, point);
			}
		      gaiaAppendToOutBuffer (out_buf, ")");
		      point = point->Next;
		  }
		line = geom->FirstLinestring;
		while (line)
		  {
		      /* processing LINESTRINGs */
		      if (ie > 0)
			  gaiaAppendToOutBuffer (out_buf, ", ");
		      ie++;
		      if (line->DimensionModel == GAIA_XY_Z)
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
			    gaiaOutLinestringZ (out_buf, line);
			}
		      else if (line->DimensionModel == GAIA_XY_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRINGM(");
			    gaiaOutLinestringM (out_buf, line);
			}
		      else if (line->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
			    gaiaOutLinestringZM (out_buf, line);
			}
		      else
			{
			    gaiaAppendToOutBuffer (out_buf, "LINESTRING(");
			    gaiaOutLinestring (out_buf, line);
			}
		      gaiaAppendToOutBuffer (out_buf, ")");
		      line = line->Next;
		  }
		polyg = geom->FirstPolygon;
		while (polyg)
		  {
		      /* processing POLYGONs */
		      if (ie > 0)
			  gaiaAppendToOutBuffer (out_buf, ", ");
		      ie++;
		      if (polyg->DimensionModel == GAIA_XY_Z)
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGON(");
			    gaiaOutPolygonZ (out_buf, polyg);
			}
		      else if (polyg->DimensionModel == GAIA_XY_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGONM(");
			    gaiaOutPolygonM (out_buf, polyg);
			}
		      else if (polyg->DimensionModel == GAIA_XY_Z_M)
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGON(");
			    gaiaOutPolygonZM (out_buf, polyg);
			}
		      else
			{
			    gaiaAppendToOutBuffer (out_buf, "POLYGON(");
			    gaiaOutPolygon (out_buf, polyg);
			}
		      gaiaAppendToOutBuffer (out_buf, ")");
		      polyg = polyg->Next;
		  }
		gaiaAppendToOutBuffer (out_buf, ")");
	    }
      }
}

/*
/
/  Gaia common support for SVG encoded geometries
/
////////////////////////////////////////////////////////////
/
/ Author: Klaus Foerster klaus.foerster@svg.cc
/ version 0.9. 2008 September 21
 /
 */

static void
SvgCoords (gaiaOutBufferPtr out_buf, gaiaPointPtr point, int precision)
{
/* formats POINT as SVG-attributes x,y */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    sprintf (buf_x, "%.*f", precision, point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%.*f", precision, point->Y * -1);
    gaiaOutClean (buf_y);
    sprintf (buf, "x=\"%s\" y=\"%s\"", buf_x, buf_y);
    gaiaAppendToOutBuffer (out_buf, buf);
}

static void
SvgCircle (gaiaOutBufferPtr out_buf, gaiaPointPtr point, int precision)
{
/* formats POINT as SVG-attributes cx,cy */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    sprintf (buf_x, "%.*f", precision, point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%.*f", precision, point->Y * -1);
    gaiaOutClean (buf_y);
    sprintf (buf, "cx=\"%s\" cy=\"%s\"", buf_x, buf_y);
    gaiaAppendToOutBuffer (out_buf, buf);
}

static void
SvgPathRelative (gaiaOutBufferPtr out_buf, int dims, int points, double *coords,
		 int precision, int closePath)
{
/* formats LINESTRING as SVG-path d-attribute with relative coordinate moves */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    double x;
    double y;
    double z;
    double m;
    double lastX = 0.0;
    double lastY = 0.0;
    int iv;
    for (iv = 0; iv < points; iv++)
      {
	  if (dims == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (coords, iv, &x, &y, &z);
	    }
	  else if (dims == GAIA_XY_M)
	    {
		gaiaGetPointXYM (coords, iv, &x, &y, &m);
	    }
	  else if (dims == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (coords, iv, &x, &y);
	    }
	  sprintf (buf_x, "%.*f", precision, x - lastX);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, (y - lastY) * -1);
	  gaiaOutClean (buf_y);
	  if (iv == 0)
	      sprintf (buf, "M %s %s l ", buf_x, buf_y);
	  else
	      sprintf (buf, "%s %s ", buf_x, buf_y);
	  lastX = x;
	  lastY = y;
	  if (iv == points - 1 && closePath == 1)
	      sprintf (buf, "z ");
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
}

static void
SvgPathAbsolute (gaiaOutBufferPtr out_buf, int dims, int points, double *coords,
		 int precision, int closePath)
{
/* formats LINESTRING as SVG-path d-attribute with relative coordinate moves */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    double x;
    double y;
    double z;
    double m;
    int iv;
    for (iv = 0; iv < points; iv++)
      {
	  if (dims == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (coords, iv, &x, &y, &z);
	    }
	  else if (dims == GAIA_XY_M)
	    {
		gaiaGetPointXYM (coords, iv, &x, &y, &m);
	    }
	  else if (dims == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (coords, iv, &x, &y);
	    }
	  sprintf (buf_x, "%.*f", precision, x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, y * -1);
	  gaiaOutClean (buf_y);
	  if (iv == 0)
	      sprintf (buf, "M %s %s L ", buf_x, buf_y);
	  else
	      sprintf (buf, "%s %s ", buf_x, buf_y);
	  if (iv == points - 1 && closePath == 1)
	      sprintf (buf, "z ");
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
}

GAIAGEO_DECLARE void
gaiaOutSvg (gaiaOutBufferPtr out_buf, gaiaGeomCollPtr geom, int relative,
	    int precision)
{
/* prints the SVG representation of current geometry */
    int pts = 0;
    int lns = 0;
    int pgs = 0;
    int ib;
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    gaiaRingPtr ring;
    if (!geom)
	return;
    point = geom->FirstPoint;
    while (point)
      {
	  /* counting how many POINTs are there */
	  pts++;
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* counting how many LINESTRINGs are there */
	  lns++;
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* counting how many POLYGONs are there */
	  pgs++;
	  polyg = polyg->Next;
      }

    if ((pts + lns + pgs) == 1)
      {
	  /* we have only one elementary geometry */
	  point = geom->FirstPoint;
	  while (point)
	    {
		/* processing POINT */
		if (relative == 1)
		    SvgCoords (out_buf, point, precision);
		else
		    SvgCircle (out_buf, point, precision);
		point = point->Next;
	    }
	  line = geom->FirstLinestring;
	  while (line)
	    {
		/* processing LINESTRING */
		if (relative == 1)
		    SvgPathRelative (out_buf, line->DimensionModel,
				     line->Points, line->Coords, precision, 0);
		else
		    SvgPathAbsolute (out_buf, line->DimensionModel,
				     line->Points, line->Coords, precision, 0);
		line = line->Next;
	    }
	  polyg = geom->FirstPolygon;
	  while (polyg)
	    {
		/* process exterior and interior rings */
		ring = polyg->Exterior;
		if (relative == 1)
		  {
		      SvgPathRelative (out_buf, ring->DimensionModel,
				       ring->Points, ring->Coords, precision,
				       1);
		      for (ib = 0; ib < polyg->NumInteriors; ib++)
			{
			    ring = polyg->Interiors + ib;
			    SvgPathRelative (out_buf, ring->DimensionModel,
					     ring->Points, ring->Coords,
					     precision, 1);
			}
		  }
		else
		  {
		      SvgPathAbsolute (out_buf, ring->DimensionModel,
				       ring->Points, ring->Coords, precision,
				       1);
		      for (ib = 0; ib < polyg->NumInteriors; ib++)
			{
			    ring = polyg->Interiors + ib;
			    SvgPathAbsolute (out_buf, ring->DimensionModel,
					     ring->Points, ring->Coords,
					     precision, 1);
			}
		  }
		polyg = polyg->Next;
	    }
      }
    else
      {
	  /* we have some kind of complex geometry */
	  if (pts > 0 && lns == 0 && pgs == 0)
	    {
		/* this one is a MULTIPOINT */
		point = geom->FirstPoint;
		while (point)
		  {
		      /* processing POINTs */
		      if (point != geom->FirstPoint)
			  gaiaAppendToOutBuffer (out_buf, ",");
		      if (relative == 1)
			  SvgCoords (out_buf, point, precision);
		      else
			  SvgCircle (out_buf, point, precision);
		      point = point->Next;
		  }
	    }
	  else if (pts == 0 && lns > 0 && pgs == 0)
	    {
		/* this one is a MULTILINESTRING */
		line = geom->FirstLinestring;
		while (line)
		  {
		      /* processing LINESTRINGs */
		      if (relative == 1)
			  SvgPathRelative (out_buf, line->DimensionModel,
					   line->Points, line->Coords,
					   precision, 0);
		      else
			  SvgPathAbsolute (out_buf, line->DimensionModel,
					   line->Points, line->Coords,
					   precision, 0);
		      line = line->Next;
		  }
	    }
	  else if (pts == 0 && lns == 0 && pgs > 0)
	    {
		/* this one is a MULTIPOLYGON */
		polyg = geom->FirstPolygon;
		while (polyg)
		  {
		      /* processing POLYGONs */
		      ring = polyg->Exterior;
		      if (relative == 1)
			{
			    SvgPathRelative (out_buf, ring->DimensionModel,
					     ring->Points, ring->Coords,
					     precision, 1);
			    for (ib = 0; ib < polyg->NumInteriors; ib++)
			      {
				  ring = polyg->Interiors + ib;
				  SvgPathRelative (out_buf,
						   ring->DimensionModel,
						   ring->Points, ring->Coords,
						   precision, 1);
			      }
			}
		      else
			{
			    SvgPathAbsolute (out_buf, ring->DimensionModel,
					     ring->Points, ring->Coords,
					     precision, 1);
			    for (ib = 0; ib < polyg->NumInteriors; ib++)
			      {
				  ring = polyg->Interiors + ib;
				  SvgPathAbsolute (out_buf,
						   ring->DimensionModel,
						   ring->Points, ring->Coords,
						   precision, 1);
			      }
			}
		      polyg = polyg->Next;
		  }
	    }
	  else
	    {
		/* this one is a GEOMETRYCOLLECTION */
		int ie = 0;
		point = geom->FirstPoint;
		while (point)
		  {
		      /* processing POINTs */
		      if (ie > 0)
			{
			    gaiaAppendToOutBuffer (out_buf, ";");
			}
		      ie++;
		      if (relative == 1)
			  SvgCoords (out_buf, point, precision);
		      else
			  SvgCircle (out_buf, point, precision);
		      point = point->Next;
		  }
		line = geom->FirstLinestring;
		while (line)
		  {
		      /* processing LINESTRINGs */
		      if (ie > 0)
			  gaiaAppendToOutBuffer (out_buf, ";");
		      ie++;
		      if (relative == 1)
			  SvgPathRelative (out_buf, line->DimensionModel,
					   line->Points, line->Coords,
					   precision, 0);
		      else
			  SvgPathAbsolute (out_buf, line->DimensionModel,
					   line->Points, line->Coords,
					   precision, 0);
		      line = line->Next;
		  }
		polyg = geom->FirstPolygon;
		while (polyg)
		  {
		      /* processing POLYGONs */
		      ie++;
		      /* process exterior and interior rings */
		      ring = polyg->Exterior;
		      if (relative == 1)
			{
			    SvgPathRelative (out_buf, ring->DimensionModel,
					     ring->Points, ring->Coords,
					     precision, 1);
			    for (ib = 0; ib < polyg->NumInteriors; ib++)
			      {
				  ring = polyg->Interiors + ib;
				  SvgPathRelative (out_buf,
						   ring->DimensionModel,
						   ring->Points, ring->Coords,
						   precision, 1);
			      }
			}
		      else
			{
			    SvgPathAbsolute (out_buf, ring->DimensionModel,
					     ring->Points, ring->Coords,
					     precision, 1);
			    for (ib = 0; ib < polyg->NumInteriors; ib++)
			      {
				  ring = polyg->Interiors + ib;
				  SvgPathAbsolute (out_buf,
						   ring->DimensionModel,
						   ring->Points, ring->Coords,
						   precision, 1);
			      }
			}
		      polyg = polyg->Next;
		  }
	    }
      }
}

/* END of Klaus Foerster SVG implementation */


static char *
XmlClean (const char *string)
{
/* well formatting a text string for XML */
    int ind;
    char *clean;
    char *p_out;
    int len = strlen (string);
    clean = malloc (len * 3);
    if (!clean)
	return NULL;
    p_out = clean;
    for (ind = 0; ind < len; ind++)
      {
	  /* masking XML special chars */
	  switch (string[ind])
	    {
	    case '&':
		*p_out++ = '&';
		*p_out++ = 'a';
		*p_out++ = 'm';
		*p_out++ = 'p';
		*p_out++ = ';';
		break;
	    case '<':
		*p_out++ = '&';
		*p_out++ = 'l';
		*p_out++ = 't';
		*p_out++ = ';';
		break;
	    case '>':
		*p_out++ = '&';
		*p_out++ = 'g';
		*p_out++ = 't';
		*p_out++ = ';';
		break;
	    case '"':
		*p_out++ = '&';
		*p_out++ = 'q';
		*p_out++ = 'u';
		*p_out++ = 'o';
		*p_out++ = 't';
		*p_out++ = ';';
		break;
	    default:
		*p_out++ = string[ind];
		break;
	    };
      }
    *p_out = '\0';
    return clean;
}

static void
out_bare_kml_point (gaiaOutBufferPtr out_buf, gaiaPointPtr point, int precision)
{
/* formats POINT as 'bare' KML [x,y] */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    sprintf (buf_x, "%.*f", precision, point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%.*f", precision, point->Y);
    gaiaOutClean (buf_y);
    gaiaAppendToOutBuffer (out_buf, "<Point><coordinates>");
    sprintf (buf, "%s,%s", buf_x, buf_y);
    gaiaAppendToOutBuffer (out_buf, buf);
    gaiaAppendToOutBuffer (out_buf, "</coordinates></Point>");
}

static void
out_full_kml_point (gaiaOutBufferPtr out_buf, const char *name,
		    const char *desc, gaiaPointPtr point, int precision)
{
/* formats POINT as 'full' KML [x,y] */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    char *xml_clean;
    sprintf (buf_x, "%.*f", precision, point->X);
    gaiaOutClean (buf_x);
    sprintf (buf_y, "%.*f", precision, point->Y);
    gaiaOutClean (buf_y);
    gaiaAppendToOutBuffer (out_buf, "<Placemark><name>");
    xml_clean = XmlClean (name);
    if (xml_clean)
      {
	  gaiaAppendToOutBuffer (out_buf, xml_clean);
	  free (xml_clean);
      }
    else
	gaiaAppendToOutBuffer (out_buf, " ");
    gaiaAppendToOutBuffer (out_buf, "</name><description>");
    xml_clean = XmlClean (desc);
    if (xml_clean)
      {
	  gaiaAppendToOutBuffer (out_buf, xml_clean);
	  free (xml_clean);
      }
    else
	gaiaAppendToOutBuffer (out_buf, " ");
    gaiaAppendToOutBuffer (out_buf, "</description><Point><coordinates>");
    sprintf (buf, "%s,%s", buf_x, buf_y);
    gaiaAppendToOutBuffer (out_buf, buf);
    gaiaAppendToOutBuffer (out_buf, "</coordinates></Point></Placemark>");
}

static void
out_bare_kml_linestring (gaiaOutBuffer * out_buf, int dims, int points,
			 double *coords, int precision)
{
/* formats LINESTRING as 'bare' KML [x,y] */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    int iv;
    double x;
    double y;
    double z;
    double m;
    gaiaAppendToOutBuffer (out_buf, "<LineString><coordinates>");
    for (iv = 0; iv < points; iv++)
      {
	  /* exporting vertices */
	  if (dims == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (coords, iv, &x, &y, &z);
	    }
	  else if (dims == GAIA_XY_M)
	    {
		gaiaGetPointXYM (coords, iv, &x, &y, &m);
	    }
	  else if (dims == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (coords, iv, &x, &y);
	    }
	  sprintf (buf_x, "%.*f", precision, x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, y);
	  gaiaOutClean (buf_y);
	  if (iv == 0)
	      sprintf (buf, "%s,%s", buf_x, buf_y);
	  else
	      sprintf (buf, " %s,%s", buf_x, buf_y);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    gaiaAppendToOutBuffer (out_buf, "</coordinates></LineString>");
}

static void
out_full_kml_linestring (gaiaOutBufferPtr out_buf, const char *name,
			 const char *desc, int dims, int points, double *coords,
			 int precision)
{
/* formats LINESTRING as 'full' KML [x,y] */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    char *xml_clean;
    int iv;
    double x;
    double y;
    double z;
    double m;
    gaiaAppendToOutBuffer (out_buf, "<Placemark><name>");
    xml_clean = XmlClean (name);
    if (xml_clean)
      {
	  gaiaAppendToOutBuffer (out_buf, xml_clean);
	  free (xml_clean);
      }
    else
	gaiaAppendToOutBuffer (out_buf, " ");
    gaiaAppendToOutBuffer (out_buf, "</name><description>");
    xml_clean = XmlClean (desc);
    if (xml_clean)
      {
	  gaiaAppendToOutBuffer (out_buf, xml_clean);
	  free (xml_clean);
      }
    else
	gaiaAppendToOutBuffer (out_buf, " ");
    gaiaAppendToOutBuffer (out_buf, "</description><LineString><coordinates>");
    for (iv = 0; iv < points; iv++)
      {
	  /* exporting vertices */
	  if (dims == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (coords, iv, &x, &y, &z);
	    }
	  else if (dims == GAIA_XY_M)
	    {
		gaiaGetPointXYM (coords, iv, &x, &y, &m);
	    }
	  else if (dims == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (coords, iv, &x, &y);
	    }
	  sprintf (buf_x, "%.*f", precision, x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, y);
	  gaiaOutClean (buf_y);
	  if (iv == 0)
	      sprintf (buf, "%s,%s", buf_x, buf_y);
	  else
	      sprintf (buf, " %s,%s", buf_x, buf_y);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    gaiaAppendToOutBuffer (out_buf, "</coordinates></LineString></Placemark>");
}

static void
out_bare_kml_polygon (gaiaOutBufferPtr out_buf, gaiaPolygonPtr polygon,
		      int precision)
{
/* formats POLYGON as 'bare' KML [x,y] */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    gaiaRingPtr ring;
    int iv;
    int ib;
    double x;
    double y;
    double z;
    double m;
    gaiaAppendToOutBuffer (out_buf, "<Polygon>");
    gaiaAppendToOutBuffer (out_buf,
			   "<outerBoundaryIs><LinearRing><coordinates>");
    ring = polygon->Exterior;
    for (iv = 0; iv < ring->Points; iv++)
      {
	  /* exporting vertices [Exterior Ring] */
	  if (ring->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
	    }
	  else if (ring->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
	    }
	  else if (ring->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (ring->Coords, iv, &x, &y);
	    }
	  sprintf (buf_x, "%.*f", precision, x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, y);
	  gaiaOutClean (buf_y);
	  if (iv == 0)
	      sprintf (buf, "%s,%s", buf_x, buf_y);
	  else
	      sprintf (buf, " %s,%s", buf_x, buf_y);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    gaiaAppendToOutBuffer (out_buf,
			   "</coordinates></LinearRing></outerBoundaryIs>");
    for (ib = 0; ib < polygon->NumInteriors; ib++)
      {
	  /* interior rings */
	  ring = polygon->Interiors + ib;
	  gaiaAppendToOutBuffer (out_buf,
				 "<innerBoundaryIs><LinearRing><coordinates>");
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		/* exporting vertices [Interior Ring] */
		if (ring->DimensionModel == GAIA_XY_Z)
		  {
		      gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
		  }
		else if (ring->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
		  }
		else if (ring->DimensionModel == GAIA_XY_Z_M)
		  {
		      gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (ring->Coords, iv, &x, &y);
		  }
		sprintf (buf_x, "%.*f", precision, x);
		gaiaOutClean (buf_x);
		sprintf (buf_y, "%.*f", precision, y);
		gaiaOutClean (buf_y);
		if (iv == 0)
		    sprintf (buf, "%s,%s", buf_x, buf_y);
		else
		    sprintf (buf, " %s,%s", buf_x, buf_y);
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  gaiaAppendToOutBuffer (out_buf,
				 "</coordinates></LinearRing></innerBoundaryIs>");
      }
    strcpy (buf, "</Polygon>");
    gaiaAppendToOutBuffer (out_buf, buf);
}

static void
out_full_kml_polygon (gaiaOutBufferPtr out_buf, const char *name,
		      const char *desc, gaiaPolygonPtr polygon, int precision)
{
/* formats POLYGON as 'full' KML [x,y] */
    char buf_x[128];
    char buf_y[128];
    char buf[256];
    char *xml_clean;
    gaiaRingPtr ring;
    int iv;
    int ib;
    double x;
    double y;
    double z;
    double m;
    gaiaAppendToOutBuffer (out_buf, "<Placemark><name>");
    xml_clean = XmlClean (name);
    if (xml_clean)
      {
	  gaiaAppendToOutBuffer (out_buf, xml_clean);
	  free (xml_clean);
      }
    else
	gaiaAppendToOutBuffer (out_buf, " ");
    gaiaAppendToOutBuffer (out_buf, "</name><description>");
    xml_clean = XmlClean (desc);
    if (xml_clean)
      {
	  gaiaAppendToOutBuffer (out_buf, xml_clean);
	  free (xml_clean);
      }
    else
	gaiaAppendToOutBuffer (out_buf, " ");
    gaiaAppendToOutBuffer (out_buf, "</description><Polygon>");
    gaiaAppendToOutBuffer (out_buf,
			   "<outerBoundaryIs><LinearRing><coordinates>");
    ring = polygon->Exterior;
    for (iv = 0; iv < ring->Points; iv++)
      {
	  /* exporting vertices [Exterior Ring] */
	  if (ring->DimensionModel == GAIA_XY_Z)
	    {
		gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
	    }
	  else if (ring->DimensionModel == GAIA_XY_M)
	    {
		gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
	    }
	  else if (ring->DimensionModel == GAIA_XY_Z_M)
	    {
		gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
	    }
	  else
	    {
		gaiaGetPoint (ring->Coords, iv, &x, &y);
	    }
	  sprintf (buf_x, "%.*f", precision, x);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, y);
	  gaiaOutClean (buf_y);
	  if (iv == 0)
	      sprintf (buf, "%s,%s", buf_x, buf_y);
	  else
	      sprintf (buf, " %s,%s", buf_x, buf_y);
	  gaiaAppendToOutBuffer (out_buf, buf);
      }
    gaiaAppendToOutBuffer (out_buf,
			   "</coordinates></LinearRing></outerBoundaryIs>");
    for (ib = 0; ib < polygon->NumInteriors; ib++)
      {
	  /* interior rings */
	  ring = polygon->Interiors + ib;
	  gaiaAppendToOutBuffer (out_buf,
				 "<innerBoundaryIs><LinearRing><coordinates>");
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		/* exporting vertices [Interior Ring] */
		if (ring->DimensionModel == GAIA_XY_Z)
		  {
		      gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
		  }
		else if (ring->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
		  }
		else if (ring->DimensionModel == GAIA_XY_Z_M)
		  {
		      gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (ring->Coords, iv, &x, &y);
		  }
		sprintf (buf_x, "%.*f", precision, x);
		gaiaOutClean (buf_x);
		sprintf (buf_y, "%.*f", precision, y);
		gaiaOutClean (buf_y);
		if (iv == 0)
		    sprintf (buf, "%s,%s", buf_x, buf_y);
		else
		    sprintf (buf, " %s,%s", buf_x, buf_y);
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  gaiaAppendToOutBuffer (out_buf,
				 "</coordinates></LinearRing></innerBoundaryIs>");
      }
    gaiaAppendToOutBuffer (out_buf, "</Polygon></Placemark>");
}

GAIAGEO_DECLARE void
gaiaOutFullKml (gaiaOutBufferPtr out_buf, const char *name, const char *desc,
		gaiaGeomCollPtr geom, int precision)
{
/* prints the 'full' KML representation of current geometry */
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    if (!geom)
	return;
    if (precision > 18)
	precision = 18;
    point = geom->FirstPoint;
    while (point)
      {
	  /* processing POINT */
	  out_full_kml_point (out_buf, name, desc, point, precision);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* processing LINESTRING */
	  out_full_kml_linestring (out_buf, name, desc, line->DimensionModel,
				   line->Points, line->Coords, precision);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* processing POLYGON */
	  out_full_kml_polygon (out_buf, name, desc, polyg, precision);
	  polyg = polyg->Next;
      }
}

GAIAGEO_DECLARE void
gaiaOutBareKml (gaiaOutBufferPtr out_buf, gaiaGeomCollPtr geom, int precision)
{
/* prints the 'bare' KML representation of current geometry */
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    if (!geom)
	return;
    if (precision > 18)
	precision = 18;
    point = geom->FirstPoint;
    while (point)
      {
	  /* processing POINT */
	  out_bare_kml_point (out_buf, point, precision);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* processing LINESTRING */
	  out_bare_kml_linestring (out_buf, line->DimensionModel, line->Points,
				   line->Coords, precision);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* processing POLYGON */
	  out_bare_kml_polygon (out_buf, polyg, precision);
	  polyg = polyg->Next;
      }
}

GAIAGEO_DECLARE void
gaiaOutGml (gaiaOutBufferPtr out_buf, int version, int precision,
	    gaiaGeomCollPtr geom)
{
/*
/ prints the GML representation of current geometry
/ *result* returns the encoded GML or NULL if any error is encountered
*/
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    gaiaRingPtr ring;
    int iv;
    int ib;
    double x;
    double y;
    double z;
    double m;
    int has_z;
    int is_multi = 1;
    char buf[1024];
    char buf_x[128];
    char buf_y[128];
    char buf_z[128];
    if (!geom)
	return;
    if (precision > 18)
	precision = 18;

    switch (geom->DeclaredType)
      {
      case GAIA_POINT:
      case GAIA_LINESTRING:
      case GAIA_POLYGON:
	  *buf = '\0';
	  is_multi = 0;
	  break;
      case GAIA_MULTIPOINT:
	  sprintf (buf, "<gml:MultiPoint SrsName=\"EPSG::%d\">", geom->Srid);
	  break;
      case GAIA_MULTILINESTRING:
	  if (version == 3)
	      sprintf (buf, "<gml:MultiCurve SrsName=\"EPSG::%d\">",
		       geom->Srid);
	  else
	      sprintf (buf,
		       "<gml:MultiLineString SrsName=\"EPSG::%d\">",
		       geom->Srid);
	  break;
      case GAIA_MULTIPOLYGON:
	  if (version == 3)
	      sprintf (buf, "<gml:MultiSurface SrsName=\"EPSG::%d\">",
		       geom->Srid);
	  else
	      sprintf (buf, "<gml:MultiPolygon SrsName=\"EPSG::%d\">",
		       geom->Srid);
	  break;
      default:
	  sprintf (buf, "<gml:MultiGeometry SrsName=\"EPSG::%d\">", geom->Srid);
	  break;
      };
    gaiaAppendToOutBuffer (out_buf, buf);
    point = geom->FirstPoint;
    while (point)
      {
	  /* processing POINT */
	  if (is_multi)
	    {
		strcpy (buf, "<gml:pointMember>");
		strcat (buf, "<gml:Point>");
	    }
	  else
	      sprintf (buf, "<gml:Point SrsName=\"EPSG::%d\">", geom->Srid);
	  if (version == 3)
	      strcat (buf, "<gml:pos>");
	  else
	      strcat (buf, "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
	  sprintf (buf_x, "%.*f", precision, point->X);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, point->Y);
	  gaiaOutClean (buf_y);
	  if (point->DimensionModel == GAIA_XY_Z
	      || point->DimensionModel == GAIA_XY_Z_M)
	    {
		sprintf (buf_z, "%.*f", precision, point->Z);
		gaiaOutClean (buf_z);
		if (version == 3)
		  {
		      strcat (buf, buf_x);
		      strcat (buf, " ");
		      strcat (buf, buf_y);
		      strcat (buf, " ");
		      strcat (buf, buf_z);
		  }
		else
		  {
		      strcat (buf, buf_x);
		      strcat (buf, ",");
		      strcat (buf, buf_y);
		      strcat (buf, ",");
		      strcat (buf, buf_z);
		  }
	    }
	  else
	    {
		if (version == 3)
		  {
		      strcat (buf, buf_x);
		      strcat (buf, " ");
		      strcat (buf, buf_y);
		  }
		else
		  {
		      strcat (buf, buf_x);
		      strcat (buf, ",");
		      strcat (buf, buf_y);
		  }
	    }
	  if (version == 3)
	      strcat (buf, "</gml:pos>");
	  else
	      strcat (buf, "</gml:coordinates>");
	  if (is_multi)
	    {
		strcat (buf, "</gml:Point>");
		strcat (buf, "</gml:pointMember>");
	    }
	  else
	      strcat (buf, "</gml:Point>");
	  gaiaAppendToOutBuffer (out_buf, buf);
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* processing LINESTRING */
	  if (is_multi)
	    {
		if (version == 3)
		  {
		      strcpy (buf, "<gml:curveMember>");
		      strcat (buf, "<gml:LineString>");
		      strcat (buf, "<gml:posList>");
		  }
		else
		  {
		      strcpy (buf, "<gml:lineStringMember>");
		      strcat (buf, "<gml:LineString>");
		      strcat (buf,
			      "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
		  }
	    }
	  else
	    {
		sprintf (buf, "<gml:LineString SrsName=\"EPSG::%d\">",
			 geom->Srid);
		if (version == 3)
		    strcat (buf, "<gml:posList>");
		else
		    strcat (buf,
			    "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
	    }
	  gaiaAppendToOutBuffer (out_buf, buf);
	  for (iv = 0; iv < line->Points; iv++)
	    {
		/* exporting vertices */
		has_z = 0;
		if (line->DimensionModel == GAIA_XY_Z)
		  {
		      has_z = 1;
		      gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
		  }
		else if (line->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
		  }
		else if (line->DimensionModel == GAIA_XY_Z_M)
		  {
		      has_z = 1;
		      gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (line->Coords, iv, &x, &y);
		  }
		if (iv == 0)
		    *buf = '\0';
		else
		    strcpy (buf, " ");
		if (has_z)
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      sprintf (buf_z, "%.*f", precision, z);
		      gaiaOutClean (buf_z);
		      if (version == 3)
			{
			    strcat (buf, buf_x);
			    strcat (buf, " ");
			    strcat (buf, buf_y);
			    strcat (buf, " ");
			    strcat (buf, buf_z);
			}
		      else
			{
			    strcat (buf, buf_x);
			    strcat (buf, ",");
			    strcat (buf, buf_y);
			    strcat (buf, ",");
			    strcat (buf, buf_z);
			}
		  }
		else
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      if (version == 3)
			{
			    strcat (buf, buf_x);
			    strcat (buf, " ");
			    strcat (buf, buf_y);
			}
		      else
			{
			    strcat (buf, buf_x);
			    strcat (buf, ",");
			    strcat (buf, buf_y);
			}
		  }
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  if (is_multi)
	    {
		if (version == 3)
		  {
		      strcpy (buf, "</gml:posList>");
		      strcat (buf, "</gml:LineString>");
		      strcat (buf, "</gml:curveMember>");
		  }
		else
		  {
		      strcpy (buf, "</gml:coordinates>");
		      strcat (buf, "</gml:LineString>");
		      strcat (buf, "</gml:lineStringMember>");
		  }
	    }
	  else
	    {
		if (version == 3)
		    strcpy (buf, "</gml:posList>");
		else
		    strcpy (buf, "</gml:coordinates>");
		strcat (buf, "</gml:LineString>");
	    }
	  gaiaAppendToOutBuffer (out_buf, buf);
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* processing POLYGON */
	  if (is_multi)
	    {
		if (version == 3)
		  {
		      strcpy (buf, "<gml:surfaceMember>");
		      strcat (buf, "<gml:Polygon>");
		      strcat (buf, "<gml:exterior>");
		      strcat (buf, "<gml:LinearRing>");
		      strcat (buf, "<gml:posList>");
		  }
		else
		  {
		      strcpy (buf, "<gml:polygonMember>");
		      strcat (buf, "<gml:Polygon>");
		      strcat (buf, "<gml:outerBoundaryIs>");
		      strcat (buf, "<gml:LinearRing>");
		      strcat (buf,
			      "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
		  }
	    }
	  else
	    {
		sprintf (buf, "<gml:Polygon SrsName=\"EPSG::%d\">", geom->Srid);
		if (version == 3)
		  {
		      strcat (buf, "<gml:exterior>");
		      strcat (buf, "<gml:LinearRing>");
		      strcat (buf, "<gml:posList>");
		  }
		else
		  {
		      strcat (buf, "<gml:outerBoundaryIs>");
		      strcat (buf, "<gml:LinearRing>");
		      strcat (buf,
			      "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
		  }
	    }
	  gaiaAppendToOutBuffer (out_buf, buf);
	  ring = polyg->Exterior;
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		/* exporting vertices [Interior Ring] */
		has_z = 0;
		if (ring->DimensionModel == GAIA_XY_Z)
		  {
		      has_z = 1;
		      gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
		  }
		else if (ring->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
		  }
		else if (ring->DimensionModel == GAIA_XY_Z_M)
		  {
		      has_z = 1;
		      gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (ring->Coords, iv, &x, &y);
		  }
		if (iv == 0)
		    *buf = '\0';
		else
		    strcpy (buf, " ");
		if (has_z)
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      sprintf (buf_z, "%.*f", precision, z);
		      gaiaOutClean (buf_z);
		      if (version == 3)
			{
			    strcat (buf, buf_x);
			    strcat (buf, " ");
			    strcat (buf, buf_y);
			    strcat (buf, " ");
			    strcat (buf, buf_z);
			}
		      else
			{
			    strcat (buf, buf_x);
			    strcat (buf, ",");
			    strcat (buf, buf_y);
			    strcat (buf, ",");
			    strcat (buf, buf_z);
			}
		  }
		else
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      if (version == 3)
			{
			    strcat (buf, buf_x);
			    strcat (buf, " ");
			    strcat (buf, buf_y);
			}
		      else
			{
			    strcat (buf, buf_x);
			    strcat (buf, ",");
			    strcat (buf, buf_y);
			}
		  }
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  /* closing the Exterior Ring */
	  if (is_multi)
	    {
		if (version == 3)
		  {
		      strcpy (buf, "</gml:posList>");
		      strcat (buf, "</gml:LinearRing>");
		      strcat (buf, "</gml:exterior>");
		  }
		else
		  {
		      strcpy (buf, "</gml:coordinates>");
		      strcat (buf, "</gml:LinearRing>");
		      strcat (buf, "</gml:outerBoundaryIs>");
		  }
	    }
	  else
	    {
		if (version == 3)
		  {
		      strcpy (buf, "</gml:posList>");
		      strcat (buf, "</gml:LinearRing>");
		      strcat (buf, "</gml:exterior>");
		  }
		else
		  {
		      strcpy (buf, "</gml:coordinates>");
		      strcat (buf, "</gml:LinearRing>");
		      strcat (buf, "</gml:outerBoundaryIs>");
		  }
	    }
	  gaiaAppendToOutBuffer (out_buf, buf);
	  for (ib = 0; ib < polyg->NumInteriors; ib++)
	    {
		/* interior rings */
		ring = polyg->Interiors + ib;
		if (is_multi)
		  {
		      if (version == 3)
			{
			    strcpy (buf, "<gml:interior>");
			    strcat (buf, "<gml:LinearRing>");
			    strcat (buf, "<gml:posList>");
			}
		      else
			{
			    strcpy (buf, "<gml:innerBoundaryIs>");
			    strcat (buf, "<gml:LinearRing>");
			    strcat (buf,
				    "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
			}
		  }
		else
		  {
		      if (version == 3)
			{
			    strcpy (buf, "<gml:interior>");
			    strcat (buf, "<gml:LinearRing>");
			    strcat (buf, "<gml:posList>");
			}
		      else
			{
			    strcpy (buf, "<gml:innerBoundaryIs>");
			    strcat (buf, "<gml:LinearRing>");
			    strcat (buf,
				    "<gml:coordinates decimal=\".\" cs=\",\" ts=\" \">");
			}
		  }
		gaiaAppendToOutBuffer (out_buf, buf);
		for (iv = 0; iv < ring->Points; iv++)
		  {
		      /* exporting vertices [Interior Ring] */
		      has_z = 0;
		      if (ring->DimensionModel == GAIA_XY_Z)
			{
			    has_z = 1;
			    gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
			}
		      else if (ring->DimensionModel == GAIA_XY_M)
			{
			    gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
			}
		      else if (ring->DimensionModel == GAIA_XY_Z_M)
			{
			    has_z = 1;
			    gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
			}
		      else
			{
			    gaiaGetPoint (ring->Coords, iv, &x, &y);
			}
		      if (iv == 0)
			  *buf = '\0';
		      else
			  strcpy (buf, " ");
		      if (has_z)
			{
			    sprintf (buf_x, "%.*f", precision, x);
			    gaiaOutClean (buf_x);
			    sprintf (buf_y, "%.*f", precision, y);
			    gaiaOutClean (buf_y);
			    sprintf (buf_z, "%.*f", precision, z);
			    gaiaOutClean (buf_z);
			    if (version == 3)
			      {
				  strcat (buf, buf_x);
				  strcat (buf, " ");
				  strcat (buf, buf_y);
				  strcat (buf, " ");
				  strcat (buf, buf_z);
			      }
			    else
			      {
				  strcat (buf, buf_x);
				  strcat (buf, ",");
				  strcat (buf, buf_y);
				  strcat (buf, ",");
				  strcat (buf, buf_z);
			      }
			}
		      else
			{
			    sprintf (buf_x, "%.*f", precision, x);
			    gaiaOutClean (buf_x);
			    sprintf (buf_y, "%.*f", precision, y);
			    gaiaOutClean (buf_y);
			    if (version == 3)
			      {
				  strcat (buf, buf_x);
				  strcat (buf, " ");
				  strcat (buf, buf_y);
			      }
			    else
			      {
				  strcat (buf, buf_x);
				  strcat (buf, ",");
				  strcat (buf, buf_y);
			      }
			}
		      gaiaAppendToOutBuffer (out_buf, buf);
		  }
		/* closing the Interior Ring */
		if (is_multi)
		  {
		      if (version == 3)
			{
			    strcpy (buf, "</gml:posList>");
			    strcat (buf, "</gml:LinearRing>");
			    strcat (buf, "</gml:interior>");
			}
		      else
			{
			    strcpy (buf, "</gml:coordinates>");
			    strcat (buf, "</gml:LinearRing>");
			    strcat (buf, "</gml:innerBoundaryIs>");
			}
		  }
		else
		  {
		      if (version == 3)
			{
			    strcpy (buf, "</gml:posList>");
			    strcat (buf, "</gml:LinearRing>");
			    strcat (buf, "</gml:interior>");
			}
		      else
			{
			    strcpy (buf, "</gml:coordinates>");
			    strcat (buf, "</gml:LinearRing>");
			    strcat (buf, "</gml:innerBoundaryIs>");
			}
		  }
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  /* closing the Polygon */
	  if (is_multi)
	    {
		if (version == 3)
		  {
		      strcpy (buf, "</gml:Polygon>");
		      strcat (buf, "</gml:curveMember>");
		  }
		else
		  {
		      strcpy (buf, "</gml:Polygon>");
		      strcat (buf, "</gml:polygonMember>");
		  }
	    }
	  else
	      strcpy (buf, "</gml:Polygon>");
	  gaiaAppendToOutBuffer (out_buf, buf);
	  polyg = polyg->Next;
      }
    switch (geom->DeclaredType)
      {
      case GAIA_POINT:
      case GAIA_LINESTRING:
      case GAIA_POLYGON:
	  *buf = '\0';
	  break;
      case GAIA_MULTIPOINT:
	  sprintf (buf, "</gml:MultiPoint>");
	  break;
      case GAIA_MULTILINESTRING:
	  if (version == 3)
	      sprintf (buf, "</gml:MultiCurve>");
	  else
	      sprintf (buf, "</gml:MultiLineString>");
	  break;
      case GAIA_MULTIPOLYGON:
	  if (version == 3)
	      sprintf (buf, "</gml:MultiSurface>");
	  else
	      sprintf (buf, "</gml:MultiPolygon>");
	  break;
      default:
	  sprintf (buf, "</gml:MultiGeometry>");
	  break;
      };
    gaiaAppendToOutBuffer (out_buf, buf);
}

GAIAGEO_DECLARE void
gaiaOutGeoJSON (gaiaOutBufferPtr out_buf, gaiaGeomCollPtr geom, int precision,
		int options)
{
/*
/ prints the GeoJSON representation of current geometry
/ *result* returns the encoded GeoJSON or NULL if any error is encountered
*/
    gaiaPointPtr point;
    gaiaLinestringPtr line;
    gaiaPolygonPtr polyg;
    gaiaRingPtr ring;
    int iv;
    int ib;
    double x;
    double y;
    double z;
    double m;
    int has_z;
    int is_multi = 0;
    int multi_count = 0;
    char bbox[1024];
    char crs[1024];
    char buf[2048];
    char buf_x[128];
    char buf_y[128];
    char buf_m[128];
    char buf_z[128];
    char endJson[16];
    if (!geom)
	return;
    if (precision > 18)
	precision = 18;

    if (options != 0)
      {
	  *bbox = '\0';
	  *crs = '\0';
	  if (geom->Srid != -1)
	    {
		if (options == 2 || options == 3)
		  {
		      // including short CRS
		      sprintf (crs,
			       ",\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"EPSG:%d\"}}",
			       geom->Srid);
		  }
		if (options == 4 || options == 5)
		  {
		      // including long CRS
		      sprintf (crs,
			       ",\"crs\":{\"type\":\"name\",\"properties\":{\"name\":\"urn:ogc:def:crs:EPSG:%d\"}}",
			       geom->Srid);
		  }
	    }
	  if (options == 1 || options == 3 || options == 5)
	    {
		// including BBOX
		gaiaMbrGeometry (geom);
		sprintf (buf_x, "%.*f", precision, geom->MinX);
		gaiaOutClean (buf_x);
		sprintf (buf_y, "%.*f", precision, geom->MinY);
		gaiaOutClean (buf_y);
		sprintf (buf_z, "%.*f", precision, geom->MaxX);
		gaiaOutClean (buf_z);
		sprintf (buf_m, "%.*f", precision, geom->MaxY);
		gaiaOutClean (buf_m);
		sprintf (bbox, ",\"bbox\":[%s,%s,%s,%s]", buf_x, buf_y, buf_z,
			 buf_m);
	    }
	  switch (geom->DeclaredType)
	    {
	    case GAIA_POINT:
		strcpy (buf, "{\"type\":\"Point\"");
		strcat (buf, crs);
		strcat (buf, bbox);
		strcat (buf, ",\"coordinates\":");
		strcpy (endJson, "}");
		break;
	    case GAIA_LINESTRING:
		strcpy (buf, "{\"type\":\"LineString\"");
		strcat (buf, crs);
		strcat (buf, bbox);
		strcat (buf, ",\"coordinates\":[");
		strcpy (endJson, "}");
		break;
	    case GAIA_POLYGON:
		strcpy (buf, "{\"type\":\"Polygon\"");
		strcat (buf, crs);
		strcat (buf, bbox);
		strcat (buf, ",\"coordinates\":[");
		strcpy (endJson, "}");
		break;
	    case GAIA_MULTIPOINT:
		strcpy (buf, "{\"type\":\"MultiPoint\"");
		strcat (buf, crs);
		strcat (buf, bbox);
		strcat (buf, ",\"coordinates\":[");
		strcpy (endJson, "]}");
		break;
	    case GAIA_MULTILINESTRING:
		strcpy (buf, "{\"type\":\"MultiLineString\"");
		strcat (buf, crs);
		strcat (buf, bbox);
		strcat (buf, ",\"coordinates\":[[");
		strcpy (endJson, "]}");
		break;
	    case GAIA_MULTIPOLYGON:
		strcpy (buf, "{\"type\":\"MultiPolygon\"");
		strcat (buf, crs);
		strcat (buf, bbox);
		strcat (buf, ",\"coordinates\":[[");
		strcpy (endJson, "]}");
		break;
	    default:
		strcpy (buf, "{\"type\":\"GeometryCollection\"");
		strcat (buf, crs);
		strcat (buf, bbox);
		strcat (buf, ",\"geometries\":[");
		strcpy (endJson, "]}");
		is_multi = 1;
		break;
	    };
      }
    else
      {
	  // omitting BBOX
	  switch (geom->DeclaredType)
	    {
	    case GAIA_POINT:
		strcpy (buf, "{\"type\":\"Point\",\"coordinates\":");
		strcpy (endJson, "}");
		break;
	    case GAIA_LINESTRING:
		strcpy (buf, "{\"type\":\"LineString\",\"coordinates\":[");
		strcpy (endJson, "}");
		break;
	    case GAIA_POLYGON:
		strcpy (buf, "{\"type\":\"Polygon\",\"coordinates\":[");
		strcpy (endJson, "}");
		break;
	    case GAIA_MULTIPOINT:
		strcpy (buf, "{\"type\":\"MultiPoint\",\"coordinates\":[");
		strcpy (endJson, "]}");
		break;
	    case GAIA_MULTILINESTRING:
		strcpy (buf,
			"{\"type\":\"MultiLineString\",\"coordinates\":[[");
		strcpy (endJson, "]}");
		break;
	    case GAIA_MULTIPOLYGON:
		strcpy (buf, "{\"type\":\"MultiPolygon\",\"coordinates\":[[");
		strcpy (endJson, "]}");
		break;
	    default:
		strcpy (buf,
			"{\"type\":\"GeometryCollection\",\"geometries\":[");
		strcpy (endJson, "]}");
		is_multi = 1;
		break;
	    };
      }
    gaiaAppendToOutBuffer (out_buf, buf);
    point = geom->FirstPoint;
    while (point)
      {
	  /* processing POINT */
	  if (is_multi)
	    {
		if (multi_count > 0)
		    strcpy (buf, ",{\"type\":\"Point\",\"coordinates\":");
		else
		    strcpy (buf, "{\"type\":\"Point\",\"coordinates\":");
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  else if (point != geom->FirstPoint)
	    {
		/* adding a further Point */
		strcpy (buf, ",");
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  sprintf (buf_x, "%.*f", precision, point->X);
	  gaiaOutClean (buf_x);
	  sprintf (buf_y, "%.*f", precision, point->Y);
	  gaiaOutClean (buf_y);
	  has_z = 0;
	  if (point->DimensionModel == GAIA_XY_Z
	      || point->DimensionModel == GAIA_XY_Z_M)
	    {
		sprintf (buf_z, "%.*f", precision, point->Z);
		gaiaOutClean (buf_z);
		has_z = 1;
	    }
	  if (has_z)
	      sprintf (buf, "[%s,%s,%s]", buf_x, buf_y, buf_z);
	  else
	      sprintf (buf, "[%s,%s]", buf_x, buf_y);
	  gaiaAppendToOutBuffer (out_buf, buf);
	  if (is_multi)
	    {
		strcpy (buf, "}");
		gaiaAppendToOutBuffer (out_buf, buf);
		multi_count++;
	    }
	  point = point->Next;
      }
    line = geom->FirstLinestring;
    while (line)
      {
	  /* processing LINESTRING */
	  if (is_multi)
	    {
		if (multi_count > 0)
		    strcpy (buf, ",{\"type\":\"LineString\",\"coordinates\":[");
		else
		    strcpy (buf, "{\"type\":\"LineString\",\"coordinates\":[");
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  else if (line != geom->FirstLinestring)
	    {
		/* opening a further LineString */
		strcpy (buf, ",[");
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  for (iv = 0; iv < line->Points; iv++)
	    {
		/* exporting vertices */
		has_z = 0;
		if (line->DimensionModel == GAIA_XY_Z)
		  {
		      has_z = 1;
		      gaiaGetPointXYZ (line->Coords, iv, &x, &y, &z);
		  }
		else if (line->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (line->Coords, iv, &x, &y, &m);
		  }
		else if (line->DimensionModel == GAIA_XY_Z_M)
		  {
		      has_z = 1;
		      gaiaGetPointXYZM (line->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (line->Coords, iv, &x, &y);
		  }
		if (has_z)
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      sprintf (buf_z, "%.*f", precision, z);
		      gaiaOutClean (buf_z);
		      if (iv == 0)
			  sprintf (buf, "[%s,%s,%s]", buf_x, buf_y, buf_z);
		      else
			  sprintf (buf, ",[%s,%s,%s]", buf_x, buf_y, buf_z);
		  }
		else
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      if (iv == 0)
			  sprintf (buf, "[%s,%s]", buf_x, buf_y);
		      else
			  sprintf (buf, ",[%s,%s]", buf_x, buf_y);
		  }
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  /* closing the LineString */
	  strcpy (buf, "]");
	  gaiaAppendToOutBuffer (out_buf, buf);
	  if (is_multi)
	    {
		strcpy (buf, "}");
		gaiaAppendToOutBuffer (out_buf, buf);
		multi_count++;
	    }
	  line = line->Next;
      }
    polyg = geom->FirstPolygon;
    while (polyg)
      {
	  /* processing POLYGON */
	  if (is_multi)
	    {
		if (multi_count > 0)
		    strcpy (buf, ",{\"type\":\"Polygon\",\"coordinates\":[");
		else
		    strcpy (buf, "{\"type\":\"Polygon\",\"coordinates\":[");
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  else if (polyg != geom->FirstPolygon)
	    {
		/* opening a further Polygon */
		strcpy (buf, ",[");
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  ring = polyg->Exterior;
	  for (iv = 0; iv < ring->Points; iv++)
	    {
		/* exporting vertices [Interior Ring] */
		has_z = 0;
		if (ring->DimensionModel == GAIA_XY_Z)
		  {
		      has_z = 1;
		      gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
		  }
		else if (ring->DimensionModel == GAIA_XY_M)
		  {
		      gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
		  }
		else if (ring->DimensionModel == GAIA_XY_Z_M)
		  {
		      has_z = 1;
		      gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
		  }
		else
		  {
		      gaiaGetPoint (ring->Coords, iv, &x, &y);
		  }
		if (has_z)
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      sprintf (buf_z, "%.*f", precision, z);
		      gaiaOutClean (buf_z);
		      if (iv == 0)
			  sprintf (buf, "[[%s,%s,%s]", buf_x, buf_y, buf_z);
		      else
			  sprintf (buf, ",[%s,%s,%s]", buf_x, buf_y, buf_z);
		  }
		else
		  {
		      sprintf (buf_x, "%.*f", precision, x);
		      gaiaOutClean (buf_x);
		      sprintf (buf_y, "%.*f", precision, y);
		      gaiaOutClean (buf_y);
		      if (iv == 0)
			  sprintf (buf, "[[%s,%s]", buf_x, buf_y);
		      else
			  sprintf (buf, ",[%s,%s]", buf_x, buf_y);
		  }
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  /* closing the Exterior Ring */
	  strcpy (buf, "]");
	  gaiaAppendToOutBuffer (out_buf, buf);
	  for (ib = 0; ib < polyg->NumInteriors; ib++)
	    {
		/* interior rings */
		ring = polyg->Interiors + ib;
		for (iv = 0; iv < ring->Points; iv++)
		  {
		      /* exporting vertices [Interior Ring] */
		      has_z = 0;
		      if (ring->DimensionModel == GAIA_XY_Z)
			{
			    has_z = 1;
			    gaiaGetPointXYZ (ring->Coords, iv, &x, &y, &z);
			}
		      else if (ring->DimensionModel == GAIA_XY_M)
			{
			    gaiaGetPointXYM (ring->Coords, iv, &x, &y, &m);
			}
		      else if (ring->DimensionModel == GAIA_XY_Z_M)
			{
			    has_z = 1;
			    gaiaGetPointXYZM (ring->Coords, iv, &x, &y, &z, &m);
			}
		      else
			{
			    gaiaGetPoint (ring->Coords, iv, &x, &y);
			}
		      if (has_z)
			{
			    sprintf (buf_x, "%.*f", precision, x);
			    gaiaOutClean (buf_x);
			    sprintf (buf_y, "%.*f", precision, y);
			    gaiaOutClean (buf_y);
			    sprintf (buf_z, "%.*f", precision, z);
			    gaiaOutClean (buf_z);
			    if (iv == 0)
				sprintf (buf, ",[[%s,%s,%s]", buf_x, buf_y,
					 buf_z);
			    else
				sprintf (buf, ",[%s,%s,%s]", buf_x, buf_y,
					 buf_z);
			}
		      else
			{
			    sprintf (buf_x, "%.*f", precision, x);
			    gaiaOutClean (buf_x);
			    sprintf (buf_y, "%.*f", precision, y);
			    gaiaOutClean (buf_y);
			    if (iv == 0)
				sprintf (buf, ",[[%s,%s]", buf_x, buf_y);
			    else
				sprintf (buf, ",[%s,%s]", buf_x, buf_y);
			}
		      gaiaAppendToOutBuffer (out_buf, buf);
		  }
		/* closing the Interior Ring */
		strcpy (buf, "]");
		gaiaAppendToOutBuffer (out_buf, buf);
	    }
	  /* closing the Polygon */
	  strcpy (buf, "]");
	  gaiaAppendToOutBuffer (out_buf, buf);
	  if (is_multi)
	    {
		strcpy (buf, "}");
		gaiaAppendToOutBuffer (out_buf, buf);
		multi_count++;
	    }
	  polyg = polyg->Next;
      }
    strcpy (buf, endJson);
    gaiaAppendToOutBuffer (out_buf, buf);
}
