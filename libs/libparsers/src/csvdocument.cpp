#include "csvdocument.h"

CsvDocument::CsvDocument()
{

}

int CsvDocument::getRowCount()
{
	return values.size();
}

int CsvDocument::getColumnCount()
{
	if(!columns.isEmpty())
		return columns.size();

	if(!values.isEmpty())
		return values.at(0).size();

	return 0;
}

bool CsvDocument::isEmpty()
{
	return !values.isEmpty();
}

QStringList CsvDocument::getColumnNames()
{
	return columns;
}

void CsvDocument::addValues(const QStringList &values)
{
	/* Validate the document here.
	 * Check if the number of columns of the provided values is the same
	 * of the number of columns (first row or columns list) */

	this->values.append(values);
}

void CsvDocument::setValue(int row, int col, const QString &value)
{

}

QString CsvDocument::getValue(int row, const QString &col_name)
{

}

QString CsvDocument::getValue(int row, int col)
{
	//if(row >= values.size())
		//trhow error!

	//if(col >= getColumnCount())
		//trhow error!

	return values.at(row).at(col);
}
