#include "csvdocument.h"

const QChar CsvDocument::TextDelimiterChar = '"';
const QChar CsvDocument::SeparatorChar = ';';
const QChar CsvDocument::LineBreakChar = QChar::LineFeed;

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

void CsvDocument::setColumns(const QStringList &cols)
{
	columns = cols;
}

void CsvDocument::addValues(const QStringList &vals)
{
	if(vals.isEmpty())
		return;

	if(getColumnCount() > 0 && vals.size() != getColumnCount())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::MalformedCsvDocument)
										.arg(getColumnCount()).arg(values.size() + 1).arg(vals.size()),
										ErrorCode::MalformedCsvDocument, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	this->values.append(vals);
}

QString CsvDocument::getValue(int row, int col)
{
	if(row >= values.size() || col >= getColumnCount())
	{
		throw Exception(Exception::getErrorMessage(ErrorCode::RefInvCsvDocumentValue).arg(row).arg(col),
										ErrorCode::RefInvCsvDocumentValue, __PRETTY_FUNCTION__, __FILE__, __LINE__);
	}

	return values.at(row).at(col);
}
