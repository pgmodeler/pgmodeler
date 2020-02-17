/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libpgmodeler/pgsqltypes
\class SpatialType
\brief Implements the operations to manipulate geospatial (postgis) data types.
**/

#ifndef SPATIAL_TYPE
#define SPATIAL_TYPE

#include "templatetype.h"

class SpatialType: public TemplateType<SpatialType>{
	private:
		unsigned variation;

		/*! \brief Used in conjunction with spatial_type, and denotes the SRID value
	 for the spatial type. This value goes from -1 to n. */
		int srid;

	public:
		static constexpr unsigned NoVar = 0,
		VarZ = 1,
		VarM = 2,
		VarZm = 3;

		static constexpr unsigned Point = 1,
		LineString = 2,
		Polygon = 3,
		MultiPoint = 4,
		MultiLineString = 5,
		MultiPolygon = 6,
		Geometry = 7,
		GeometryCollection = 8;

		SpatialType(const QString &type_name, int srid, unsigned variation_id=SpatialType::NoVar);
		SpatialType(unsigned type_id, int srid, unsigned var_id=SpatialType::NoVar);
		SpatialType();

		void setVariation(unsigned var);
		unsigned getVariation();

		void setSRID(int srid);
		int getSRID();

		QString operator * ();
};

#endif 
