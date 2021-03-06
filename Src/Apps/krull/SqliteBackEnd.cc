//-----------------------------------------------------------------------------
// Apps/krull/SqliteBackEnd.cc
//
// Implementation of the SqliteBackEnd class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "SqliteBackEnd.h"
extern "C"
{
#include <Sqlite/sqlite3.h>
};
#include "Data.h"
#include "Filename.h"
#include "Value.h"

#include <iostream>

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

SqliteBackEnd::SqliteBackEnd (bool addSymbols)
	: mSqlite(0)
	, mAddSymbols(addSymbols)
{

}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

SqliteBackEnd::~SqliteBackEnd ()
{

}

//-----------------------------------------------------------------------------
// Builder
//-----------------------------------------------------------------------------

bool SqliteBackEnd::Build (const string& fileName, const Compiler& compiler, const Project& project)
{
	bool buildResult = true;

	// Create the Sqlite database file
	compiler.Status("[SQLITE]  Opening database '%s'", fileName.c_str());
	if (!FileName::Delete(fileName))
	{
		return compiler.Error(0, "Cannot delete the file '%s'", fileName.c_str());
		K_ASSERT(0);
	}
	int result = sqlite3_open(fileName.c_str(), &mSqlite);
	if (result)
	{
		buildResult = false;
		compiler.Error(0, "Failed to open database '%s'", fileName.c_str());
	}
	else
	{
		Execute("BEGIN TRANSACTION ;");

		// Generate tables
		for (const KTable* table = project.FirstTable(); table != 0; table = project.NextTable())
		{
			string sql;

			//-----------------------------------------------------------------------------
			// Generate type information for the tables
			//
			// SQL code to create a meta-table:
			//
			//		CREATE TABLE main.`@<table name>`
			//		(
			//			name	TEXT,
			//			type	TEXT
			//		);
			//
			// SQL code to insert an entry:
			//
			//		INSERT INTO main.`@<table name>`
			//		(
			//			name,
			//			type
			//		)
			//		VALUES
			//		(
			//			<field name>,
			//			<field type>
			//		);
			//
			//-----------------------------------------------------------------------------

			sql = "CREATE TABLE main.`_";
			sql += table->GetName();
			sql += "` ( name TEXT, type TEXT ) ;";
			Execute(sql);

			for (unsigned int i = 1; i < table->GetNumFields(); ++i)
			{
				sql = "INSERT INTO main.`_";
				sql += table->GetName();
				sql += "` (name, type) VALUES ( \"";

				string name = table->GetFieldName(i);
				Type type = table->GetFieldType(i);
				string ref;
				
				sql += name;
				sql += "\", \"";

				switch(type.GetType())
				{
				case TypeValue_Integer:		sql += "integer";			break;
				case TypeValue_Float:		sql += "float";				break;
				case TypeValue_Bool:		sql += "boolean";			break;
				case TypeValue_String:		sql += "string";			break;
				case TypeValue_DataRef:		sql += "ref ";
											sql += type.GetDataName();	break;
				case TypeValue_DataRefList:	sql += "list ";
											sql += type.GetDataName();	break;
				default:
					buildResult = compiler.Error(0, "Invalid type passed to the back-end.  This should never happen, please contact the developers [0]");
				}

				if (!buildResult) break;

				sql += "\" ) ;";

				Execute(sql);
			}

			if (!buildResult) break;
		}

		for (const Data* data = project.FirstData(); data != 0; data = project.NextData())
		{
			string sql;
			const KTable& table = data->GetTable();

			//-----------------------------------------------------------------------------
			// Create table (and sub-tables)
			//
			// SQL code to create a table:
			//
			//		CREATE TABLE main.<table name>
			//		(
			//			krull_id		TEXT PRIMARY KEY,     (only if mAddSymbols = true)
			//			<field name>	<field type>
			//			...
			//		);
			//
			// SQL code to create a sub table:
			//
			//		CREATE TABLE main.`<field name>_<table name>`
			//		(
			//			id				INTEGER,
			//			ref				INTEGER
			//		);
			//
			//-----------------------------------------------------------------------------
			
			sql = "CREATE TABLE main.";
			sql += data->GetName();
			if (mAddSymbols)
			{
				sql += " ( krull_id TEXT PRIMARY KEY";
			}
			else
			{
				sql += " ( ";
			}

			// Cycle through all the columns
			
			for (unsigned int i = 1; i < table.GetNumFields(); ++i)
			{
				string fieldName = table.GetFieldName(i);
				Type fieldType = table.GetFieldType(i);
				string sqlType;

				switch(fieldType.GetType())
				{
				case TypeValue_Integer:
				case TypeValue_Bool:
				case TypeValue_DataRef:
				case TypeValue_DataRefList:		sqlType = "INTEGER";		break;
				case TypeValue_Float:			sqlType = "REAL";			break;
				case TypeValue_String:			sqlType = "TEXT";			break;
				default:
					buildResult = compiler.Error(0, "Invalid type passed to the back-end.  This should never happen, please contact the developers [1]");
				}

				if (!buildResult) break;
				
				if (mAddSymbols || (i > 1)) sql += ", ";

				sql += fieldName + " " + sqlType;

				//
				// Handle sub-tables
				//
				if (fieldType.GetType() == TypeValue_DataRefList)
				{
					string sqlSubTable = "CREATE TABLE main.`";
					sqlSubTable += fieldName;
					sqlSubTable += "_";
					sqlSubTable += data->GetName();
					sqlSubTable += "` ( id INTEGER, ref INTEGER ) ;";
					Execute(sqlSubTable);
				}
			}

			sql += ");";

			Execute(sql);

			//-----------------------------------------------------------------------------
			// Insert the data rows
			//
			// SQL code for inserting rows:
			//
			//		INSERT INTO main.<table name>
			//		(
			//			<field name>,
			//			...
			//		)
			//		VALUES
			//		(
			//			<value>,
			//			...
			//		);
			//
			//-----------------------------------------------------------------------------

			//Execute("BEGIN TRANSACTION ;");

			string sqlSubTable;

			for (unsigned int row = 0; row < data->GetNumEntries(); ++row)
			{
				sql = "INSERT INTO main.";
				sql += data->GetName();

				//
				// Column names
				//
				unsigned initialField = mAddSymbols ? 0 : 1;
				sql += "( ";
				for (unsigned field = initialField; field < table.GetNumFields(); ++field)
				{
					if (field != initialField)
					{
						sql += ", ";
					}
					sql += table.GetFieldName(field);
				}

				//
				// Values
				//
				sql += " ) VALUES (";
				
				// Insert row name
				

				for (unsigned field = initialField; field < table.GetNumFields(); ++field)
				{
					Value value = data->GetField(row, field);

					if (field != initialField)
					{
						sql += ", ";
					}

					switch(table.GetFieldType(field).GetType())
					{
					case TypeValue_Integer:
						{
							sql += FromInt(value.GetInteger());
						}
						break;

					case TypeValue_Bool:
						{
							sql += value.GetBool() ? "1" : "0";
						}
						break;

					case TypeValue_Float:
						{
							sql += FromFloat(value.GetFloat());
						}
						break;

					case TypeValue_String:
					case TypeValue_RowName:
						{
							sql += "\"";
							sql += value.GetString();
							sql += "\"";
						}
						break;

					case TypeValue_DataRef:
						{
							sql += FromUInt(value.GetDataRef()+1);
						}
						break;

					case TypeValue_DataRefList:
						{
							vector<size_t>& refs = value.GetDataRefList();

							for (unsigned i = 0; i < refs.size(); ++i)
							{
								string sqlSubTable = "INSERT INTO main.`";
								sqlSubTable += table.GetFieldName(field);
								sqlSubTable += "_";
								sqlSubTable += data->GetName();
								sqlSubTable += "` (`id`, `ref`) VALUES ( ";
								sqlSubTable += FromUInt(row+1);
								sqlSubTable += ", ";
								sqlSubTable += FromUInt(refs[i]+1);
								sqlSubTable += " ) ;";

								buildResult = Execute(sqlSubTable);
								if (!buildResult) break;
							}

 							sql += FromUInt(row+1);

						}
						break;

					default:
						buildResult = compiler.Error(0, "Invalid type passed to the back-end.  This should never happen, please contact the developers [2]");
					}

					if (!buildResult) break;
				}

				sql += " ) ;";
				Execute(sql);
			}

			//Execute("END TRANSACTION ;");
		}

		//-----------------------------------------------------------------------------
		// Generate type information for all the tables
		//
		// SQL code for main type information table:
		//
		//		CREATE TABLE main.`__type`
		//		(
		//			id			INTEGER PRIMARY KEY ASC,
		//			dataName	TEXT,
		//			tableName	TEXT
		//		);
		//
		// SQL code for add rows to main type information table:
		//
		//		INSERT INTO main.`__type`
		//		(
		//			dataName,
		//			tableName,
		//		)
		//		VALUES
		//		(
		//			"<data name>",
		//			"<table name>"
		//		);
		//
		//-----------------------------------------------------------------------------

		Execute("CREATE TABLE main.`__type` (id INTEGER PRIMARY KEY ASC, dataName TEXT, tableName TEXT);");

		for (const Data* data = project.FirstData(); data != 0; data = project.NextData())
		{
			string sql = "INSERT INTO main.`__type` ( dataName, tableName ) VALUES ( \"";
			sql += data->GetName();
			sql += "\", \"";
			sql += data->GetTable().GetName();
			sql += "\" );";

			Execute(sql);
		}

		//-----------------------------------------------------------------------------
	}

	Execute("END TRANSACTION ;");

	// Clean up
	sqlite3_close(mSqlite);

	if (!buildResult)
	{
		//FileName::Delete(fileName);
	}

	return buildResult;
}

//-----------------------------------------------------------------------------
// SQL execution
//-----------------------------------------------------------------------------

bool SqliteBackEnd::Execute (const string& sql)
{
	sqlite3_stmt* dbCode = 0;
	int result = sqlite3_prepare_v2(mSqlite, sql.c_str(), (int)sql.length(), &dbCode, 0);

	bool buildResult = true;

	if (result)
	{
		cerr << "SQL ERROR: " << sqlite3_errmsg(mSqlite) << endl;
		buildResult = false;
	}
	else
	{
		while ((result = sqlite3_step(dbCode)) != SQLITE_DONE)
		{
			cerr << "SQL ERROR: " << sqlite3_errmsg(mSqlite) << endl;
			buildResult = false;
			break;
		}
	}

	sqlite3_finalize(dbCode);
	if (!buildResult)
	{
		cerr << "SQL: " << sql << endl << endl;
	}
	return buildResult;
}

//-----------------------------------------------------------------------------
// Numeric conversions
//-----------------------------------------------------------------------------

string SqliteBackEnd::FromInt (int value) const
{
	char number [32];
	sprintf_s(number, 32, "%d", value);
	return number;
}

string SqliteBackEnd::FromUInt (size_t value) const
{
	char number [32];
	sprintf_s(number, 32, "%zu", value);
	return number;
}

string SqliteBackEnd::FromFloat (float value) const
{
	char number [64];
	sprintf_s(number, 32, "%g", value);
	return number;
}



//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
