//-----------------------------------------------------------------------------
// Apps/krull/SqliteBackEnd.cc
//
// Implementation of the SqliteBackEnd class
//-----------------------------------------------------------------------------

#include "Krull.h"
#include "SqliteBackEnd.h"
extern "C"
{
#include "..\Libs\Sqlite\sqlite3.h"
};
#include "Data.h"
#include "Filename.h"
#include "Value.h"

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

SqliteBackEnd::SqliteBackEnd ()
	: mSqlite(0)
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
		// Generate tables
		for (const Data* data = project.FirstData(); data != 0; data = project.NextData())
		{
			string sql;
			const KTable& table = data->GetTable();

			//-----------------------------------------------------------------------------
			// Create table (and sub-tables)
			//-----------------------------------------------------------------------------
			
			sql = "CREATE TABLE main.";
			sql += table.GetName();
			sql += " ( krull_id INTEGER PRIMARY KEY ASC";

			// Cycle through all the columns

			for (unsigned int i = 0; i < table.GetNumFields(); ++i)
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
					buildResult = compiler.Error(0, "Invalid type passed to the back-end.  This should never happen, please contact the developers");
				}

				if (!buildResult) break;

				sql += ", " + fieldName + " " + sqlType;
			}

			sql += ");";

			Execute(sql);

			//-----------------------------------------------------------------------------
			// Insert the data rows
			//-----------------------------------------------------------------------------

			Execute("BEGIN TRANSACTION ;");

			for (unsigned int row = 0; row < data->GetNumEntries(); ++row)
			{
				sql = "INSERT INTO main.";
				sql += table.GetName();

				//
				// Column names
				//
				sql += "( ";
				for (unsigned field = 0; field < table.GetNumFields(); ++field)
				{
					if (field != 0)
					{
						sql += ", ";
					}
					sql += table.GetFieldName(field);
				}

				//
				// Values
				//
				sql += " ) VALUES (";

				for (unsigned field = 0; field < table.GetNumFields(); ++field)
				{
					Value value = data->GetField(row, field);

					if (field != 0)
					{
						sql += ", ";
					}

					switch(table.GetFieldType(field).GetType())
					{
					case TypeValue_Integer:
						{
							char number [32];
							sprintf_s(number, "%d", value.GetInteger());
							sql += number;
						}
						break;

					case TypeValue_Bool:
						{
							sql += value.GetBool() ? "1" : "0";
						}
						break;

					case TypeValue_Float:
						{
							char number [64];
							sprintf_s(number, "%g", value.GetFloat());
							sql += number;
						}
						break;

					case TypeValue_String:
						{
							sql += "\"";
							sql += value.GetString();
							sql += "\"";
						}
						break;

					case TypeValue_DataRef:
						{
							char number [32];
							sprintf_s(number, "%d", value.GetDataRef()+1);
							sql += number;
						}
						break;

					case TypeValue_DataRefList:
						{
							sql += "0";
						}
						break;

					default:
						buildResult = compiler.Error(0, "Invalid type passed to the back-end.  This should never happen, please contact the developers");
					}

					if (!buildResult) break;
				}

				sql += " ) ;";
				Execute(sql);
			}

			Execute("END TRANSACTION ;");
		}
	}

	// Clean up
	sqlite3_close(mSqlite);

	if (!buildResult)
	{
		FileName::Delete(fileName);
	}

	return buildResult;
}

//-----------------------------------------------------------------------------
// SQL execution
//-----------------------------------------------------------------------------

bool SqliteBackEnd::Execute (const string& sql)
{
	sqlite3_stmt* dbCode = 0;
	int result = sqlite3_prepare_v2(mSqlite, sql.c_str(), sql.length(), &dbCode, 0);
	if (result)
	{
		K_ASSERT(0);
		return false;
	}
	while ((result = sqlite3_step(dbCode)) != SQLITE_DONE)
	{
		K_ASSERT(0);
		return false;
	}

	sqlite3_finalize(dbCode);
	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
