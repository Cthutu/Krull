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

			sql = "CREATE TABLE main.";
			sql += data->GetTable().GetName();
			sql += " ( krull_id INTEGER PRIMARY KEY ASC";

			// Cycle through all the columns
			const KTable& table = data->GetTable();
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

			sqlite3_stmt* dbCode = 0;
			result = sqlite3_prepare_v2(mSqlite, sql.c_str(), sql.length(), &dbCode, 0);
			K_ASSERT(result == 0);

			while((result = sqlite3_step(dbCode)) != SQLITE_DONE)
			{
				K_ASSERT(0);
			}

			sqlite3_finalize(dbCode);
		}
	}

	// Clean up
	sqlite3_close(mSqlite);

	return buildResult;
}

//-----------------------------------------------------------------------------
// Table generation
//-----------------------------------------------------------------------------

bool SqliteBackEnd::GenerateTable (const Compiler& compiler, const Project& project, const Data& data)
{
	bool buildResult = true;

	return buildResult;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
