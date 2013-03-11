#include "stdafx.h"
#include "OdbcRecordset.h"

using std::auto_ptr;

OdbcRecordset::OdbcRecordset(OdbcConnection * dbConnection) : _dbConnection(dbConnection)
{
	_dbCommand = _dbConnection->CreateCommand();
}

TCHAR * OdbcRecordset::Read(bool bAllowEmptyTable /*= false*/)
{
	// Build statement
	tstring szSQL = _T("SELECT ");
	szSQL += GetColumns();
	szSQL += _T("FROM ");
	szSQL += GetTableName();

	// Attempt to execute the statement.
	if (!_dbCommand->Execute(szSQL))
		return _dbCommand->GetError();

	// Does the table have any rows?
	// Make sure we allow for tables that can be empty.
	if (!_dbCommand->hasData())
	{
		if (bAllowEmptyTable)
			return NULL;

		return _T("Table is empty.");
	}

	do
	{
		Fetch();
	} while (_dbCommand->MoveNext());

	return NULL;
}

OdbcRecordset::~OdbcRecordset()
{
	_dbCommand->Close();
}