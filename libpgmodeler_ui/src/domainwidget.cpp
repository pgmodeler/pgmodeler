#include "domainwidget.h"

DomainWidget::DomainWidget(QWidget *parent): BaseObjectWidget(parent, OBJ_DOMAIN)
{
	try
	{
		Ui_DomainWidget::setupUi(this);

		check_expr_hl=NULL;
		check_expr_hl=new SyntaxHighlighter(check_expr_txt, false);
		check_expr_hl->loadConfiguration(GlobalAttributes::CONFIGURATIONS_DIR +
																		 GlobalAttributes::DIR_SEPARATOR +
																		 GlobalAttributes::SQL_HIGHLIGHT_CONF +
																		 GlobalAttributes::CONFIGURATION_EXT);

		data_type=NULL;
		data_type=new PgSQLTypeWidget(this);
		domain_grid->addWidget(data_type,4,0,1,2);

		configureFormLayout(domain_grid, OBJ_DOMAIN);
		connect(parent_form->apply_ok_btn,SIGNAL(clicked(bool)), this, SLOT(applyConfiguration(void)));

		parent_form->setMinimumSize(530, 500);
		parent_form->setMaximumHeight(500);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

void DomainWidget::hideEvent(QHideEvent *event)
{
	def_value_edt->clear();
	check_expr_txt->clear();
	constr_name_edt->clear();
	not_null_chk->setChecked(false);
	BaseObjectWidget::hideEvent(event);
}

void DomainWidget::setAttributes(DatabaseModel *model, OperationList *op_list, Schema *schema, Domain *domain)
{
	PgSQLType type;

	BaseObjectWidget::setAttributes(model, op_list, domain, schema);

	if(domain)
	{
		type=domain->getType();
		def_value_edt->setText(Utf8String::create(domain->getDefaultValue()));
		check_expr_txt->setPlainText(Utf8String::create(domain->getExpression()));
		constr_name_edt->setText(Utf8String::create(domain->getConstraintName()));
		not_null_chk->setChecked(domain->isNotNull());
	}

	data_type->setAttributes(type, model);
}

void DomainWidget::applyConfiguration(void)
{
	try
	{
		Domain *domain=NULL;
		startConfiguration<Domain>();

		domain=dynamic_cast<Domain *>(this->object);
		domain->setType(data_type->getPgSQLType());
		domain->setDefaultValue(def_value_edt->text());
		domain->setExpression(check_expr_txt->toPlainText());

		if(!constr_name_edt->text().isEmpty())
			domain->setConstraintName(constr_name_edt->text());

		domain->setNotNull(not_null_chk->isChecked());

		BaseObjectWidget::applyConfiguration();
		finishConfiguration();
	}
	catch(Exception &e)
	{
		cancelConfiguration();
		throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
	}
}

