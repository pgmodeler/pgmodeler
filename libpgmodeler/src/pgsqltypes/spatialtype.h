/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2019 - Raphael Araújo e Silva <raphael@pgmodeler.io>
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

#include "basetype.h"

class SpatialType: public BaseType{
	private:
		unsigned variation;
		static constexpr unsigned Offset=225;
		static constexpr unsigned TypesCount=16;

		/*! \brief Used in conjunction with spatial_type, and denotes the SRID value
	 for the spatial type. This value goes from -1 to n. */
		int srid;

	public:
		static constexpr unsigned NoVar=0;
		static constexpr unsigned VarZ=1;
		static constexpr unsigned VarM=2;
		static constexpr unsigned VarZm=3;

		static constexpr unsigned Point=Offset;
		static constexpr unsigned LineString=Offset+1;
		static constexpr unsigned Polygon=Offset+2;
		static constexpr unsigned MultiPoint=Offset+3;
		static constexpr unsigned MultiLineString=Offset+4;
		static constexpr unsigned MultiPolygon=Offset+5;
		static constexpr unsigned Geometry=Offset+6;
		static constexpr unsigned GeometryCollection=Offset+7;

		SpatialType(const QString &type_name, int srid, unsigned variation_id=SpatialType::NoVar);
		SpatialType(unsigned type_id, int srid, unsigned var_id=SpatialType::NoVar);
		SpatialType();

		void setVariation(unsigned var);
		unsigned getVariation();

		void setSRID(int srid);
		int getSRID();

		static void getTypes(QStringList &type_list);
		QString operator * ();
		virtual QString operator ~();
};

#endif 
