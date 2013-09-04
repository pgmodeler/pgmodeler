#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

PgSQLType parseString(const QString &str)
{
	QString type_str=str.toLower().simplified(), sptype, interv;
	bool with_tz=false;
	unsigned len=0, prec=-1, dim=0, srid=0;
	int start=-1, end=-1;
	QStringList value, intervals;
	PgSQLType type;

	//Checking if the string contains one of interval types
	IntervalType::getTypes(intervals);
	while(!intervals.isEmpty())
	{
		interv=intervals.back();
		intervals.pop_back();

		start=type_str.indexOf(interv.toLower());
		if(start>=0)
		{
			type_str.remove(start, interv.size());
			break;
		}
		else
			interv.clear();
	}

	//Check if the type contains "with timezone" descriptor
	with_tz=QRegExp("(.)*(with timezone)(.)*").exactMatch(type_str);

	//Removes the timezone descriptor
	type_str.remove(QRegExp("(with)(out)*( )(timezone)"));

	//Count the dimension of the type and removes the array descriptor
	dim=type_str.count("[]");
	type_str.remove("[]");

	//Check if the type is a variable length type, e.g varchar(200)
	if(QRegExp("(.)+\\(( )*[0-9]+( )*\\)").indexIn(type_str) >=0)
	{
		start=type_str.indexOf("(");
		end=type_str.indexOf(")", start);
		len=type_str.mid(start+1, end-start-1).toUInt();
	}
	//Check if the type is a numeric type, e.g, numeric(10,2)
	else if(QRegExp("(.)+\\(( )*[0-9]+( )*(,)( )*[0-9]+( )*\\)").indexIn(type_str) >=0)
	{
		start=type_str.indexOf("(");
		end=type_str.indexOf(")", start);
		value=type_str.mid(start+1, end-start-1).split(",");
		len=value[0].toUInt();
		prec=value[1].toUInt();
	}
	//Check if the type is a spatial type (PostGiS), e.g, geography(POINTZ, 4296)
	else if(QRegExp("(.)+\\(( )*[a-z]+( )*(,)( )*[0-9]+( )*\\)").indexIn(type_str) >=0)
	{
		start=type_str.indexOf("(");
		end=type_str.indexOf(")", start);
		value=type_str.mid(start+1, end-start-1).split(",");
		sptype=value[0].toUpper();
		srid=value[1].toUInt();
	}

	//If the string matches one of the regexp above remove the analyzed parts
	if(start >=0 && end>=0)
		type_str.remove(start, end-start+1);

	//The resultant string must be only the name of the type without [] and ()
	type_str=type_str.trimmed();

	//Creates the type based on the extracted values
	type=PgSQLType(type_str);
	type.setWithTimezone(with_tz);
	type.setDimension(dim);

	if(type.isNumericType())
	{
		type.setLength(len);
		type.setPrecision(prec);
	}
	else if(type.isDateTimeType() && len > 0)
		type.setPrecision(len);
	else if(type.hasVariableLength() && len > 0)
		type.setLength(len);

	if(!interv.isEmpty())
		type.setIntervalType(IntervalType(interv));
	else if(!sptype.isEmpty())
		type.setSpatialType(SpatialType(sptype, srid));

	return(type);
}

int main(int argc, char **argv)
{
	try
	{
		PgSQLType tp;
		QTextStream ts(stdout);

		tp=parseString("interval YEAR");
		ts << *tp << endl;

		tp=parseString("geography (POINTZ, 4396)");
		ts << *tp << endl;

		tp=parseString("numeric[] ( 10 , 3) [][]");
		ts << *tp << endl;

		tp=parseString("character    varying (255)");
		ts << *tp << endl;

		tp=parseString("timestamp    with     timezone");
		ts << *tp << endl;

		tp=parseString("timestamp without timezone[]");
		ts << *tp << endl;

		tp=parseString("varchar (255)");
		ts << *tp << endl;

		tp=parseString("integer [][]");
		ts << *tp << endl;


		/*cout << "\npgModeler reverse engineering test tool" << endl << endl;

		if(argc < 6)
		{
			cout << "** Insufficient parameters!" << endl;
			cout << "** The correct usage is: pgmodeler-rev {dbname dbuser dbpass dbhost dbport} [schema] [table]" << endl;
			cout << "** Process aborted!" << endl << endl;
			return(1);
		}
		else
		{
			Connection conn;
			Connection::setPrintSQL(true);
			conn.setConnectionParam(Connection::PARAM_DB_NAME, argv[1]);
			conn.setConnectionParam(Connection::PARAM_USER, argv[2]);
			conn.setConnectionParam(Connection::PARAM_PASSWORD, argv[3]);
			conn.setConnectionParam(Connection::PARAM_SERVER_FQDN, argv[4]);
			conn.setConnectionParam(Connection::PARAM_PORT, argv[5]);

			Catalog catalog;
			catalog.setConnection(conn);
			catalog.setFilter(Catalog::LIST_ONLY_SYS_OBJS);

			ObjectType types[]={ OBJ_DATABASE, OBJ_TABLESPACE , OBJ_ROLE, OBJ_SCHEMA,
													 OBJ_LANGUAGE, OBJ_EXTENSION , OBJ_FUNCTION, OBJ_AGGREGATE,
													 OBJ_OPERATOR, OBJ_OPCLASS, OBJ_OPFAMILY, OBJ_COLLATION,
													 OBJ_CONVERSION, OBJ_CAST , OBJ_VIEW, OBJ_SEQUENCE ,
													 OBJ_DOMAIN, OBJ_TYPE , OBJ_TABLE, OBJ_COLUMN, OBJ_CONSTRAINT,
													 OBJ_RULE,   OBJ_TRIGGER , OBJ_INDEX };

			unsigned i, cnt=sizeof(types)/sizeof(ObjectType);

			QString schema=(argc >= 7 ? argv[6] : ""),
							table=(argc >= 8 ? argv[7] : "");

			for(i=0; i < cnt; i++)
			{
				long time1=QDateTime::currentMSecsSinceEpoch();
				cout << "[object]: " << BaseObject::getTypeName(types[i]).toStdString() << endl;
				cout << "[count]: " << catalog.getObjectCount(types[i], schema , table) << endl;

				cout << "[list]: ";
				attribs_map v1=catalog.getObjectsNames(types[i], schema , table);
				attribs_map::iterator itr1=v1.begin();

				while(itr1!=v1.end())
				{
					cout << itr1->first.toStdString() <<  ":" <<itr1->second.toStdString() << " ";
					itr1++;
				}

				cout << endl;

				vector<attribs_map> v=catalog.getObjectsAttributes(types[i], schema , table);
				attribs_map::iterator itr;

				while(!v.empty())
				{
					cout << "[attribs]: ";
					for(itr=v.back().begin(); itr!=v.back().end(); itr++)
						cout << itr->first.toStdString() << "=" << itr->second.toStdString() << " ";

					cout << endl;
					v.pop_back();
				}
				long time2=QDateTime::currentMSecsSinceEpoch();
				cout << "[Execution]: " << time2 - time1 << " ms" << endl;

				cout << endl<< endl;
			}

			return(0);
		}*/
	}
	catch(Exception &e)
	{
		cout << e.getExceptionsText().toStdString() << endl;
		return(e.getErrorType());
	}
}
