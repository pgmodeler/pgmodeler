#include <iostream>
#include "exception.h"
#include "mainwindow.h"
#include "catalog.h"

int main(int argc, char **argv)
{
	try
	{
		PgSQLType tp;
		QTextStream ts(stdout);

		tp=PgSQLType::parseString("interval MINUTE TO SECOND []");
		ts << *tp << endl;

		/*tp=PgSQLType::parseString("timestamp(0) without timezone");
		ts << *tp << endl;*/
/*
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
		ts << *tp << endl;*/


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
