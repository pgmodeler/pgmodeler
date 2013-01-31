#include "syntaxhighlighter.h"

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *parent, bool auto_rehighlight) : QSyntaxHighlighter(parent)
{
	this->auto_rehighlight=auto_rehighlight;
	configureAttributes();
}

SyntaxHighlighter::SyntaxHighlighter(QTextEdit *parent, bool auto_rehighlight) : QSyntaxHighlighter(parent)
{
	this->auto_rehighlight=auto_rehighlight;
	configureAttributes();
}

void SyntaxHighlighter::configureAttributes(void)
{
	conf_loaded=false;
	current_block=-1;
	curr_blk_info_count=0;

	if(auto_rehighlight)
	{
		connect(document(), SIGNAL(blockCountChanged(int)), this, SLOT(rehighlight(void)));
		connect(document(), SIGNAL(contentsChange(int,int,int)), this, SLOT(validateTextModification(int,int,int)));
	}
}

void SyntaxHighlighter::validateTextModification(int, int removed, int added)
{
	unsigned count;

	//Obtém a quantidade de informações de multilinha do bloco atual
	count=getMultiLineInfoCount(current_block);

	/* Caso a quantidade obtida seja diferente da quantidade capturada
		antes da entrada do texto pelo usuário, ou se não exista nenhuma
		informação de multilinha para o bloco atual porém foram inseridos
		ou removidos alguns caracteres, o documento passará por um redestaque
		completo pois os caracteres removidos ou inseridos podem interferir
		na forma de apresentação do destaque e por isso o documento como
		um todo precisa ser redestacado */
	if(count!=curr_blk_info_count ||
		 (/*qtd==0 && qtd_info_bloco_atual==qtd &&*/ (added > 0 || removed > 0)))
		rehighlight();
}

SyntaxHighlighter::MultiLineInfo *SyntaxHighlighter::getMultiLineInfo(int start_col, int end_col, int block)
{
	unsigned i, count;
	bool found=false;
	MultiLineInfo *info=NULL;

	/* Varre o vetor de informações de multilinha a fim de verificar se
		os parâmetros passados estão dentro de um bloco (informação) multilinha */
	count=multi_line_infos.size();
	for(i=0; i < count; i++)
	{
		//Obtém uma informação
		info=multi_line_infos[i];

		/* Primeiramente é preciso verificar se o bloco passado está dentro dos limites
		 estabelecidos pelos blocos inicial e final da informação de multilinha.
		 É importante salientar que quando um bloco multilinha está em aberto, por exemplo,
		 o usuário abriu um multilinha com '/ *' e não fechou com '* /' os atributos
		 bloco_fim e col_fim da informação de multilinha possuirão o valor -1 até
		 que o usuário feche o multilinha e o destacador identifique em que parte
		 do texto se encontra esse fechamento */
		if(block >= info->start_block && (info->end_block < 0 || block <= info->end_block))
		{
			/* A seguir várias condições são testadas a fim de verificar se os parâmetros
			passados estão dentro de um bloco multilinha.*/

			/* Condição 1: O bloco passado é o mesmo da informação atual e a informação
									atual trata de um multilinha porém aberto e fechado na mesma linha
									de texto (info->bloco_ini==info->bloco_fim), será verificado
									se o parâmetro col_ini e col_fim estão nos limites estabelecidos
									pelas colunas inicial e final do informação */
			if(block==info->start_block && info->start_block==info->end_block)
				found=(start_col >= info->start_col && end_col <= info->end_col);

			/* Condição 2: O bloco passado é o mesmo da informação atual e a informação
									atual trata de um multilinha em aberto. Testa apenas se
									a coluna inicial do parâmetro está após da coluna inicial da
									informação. Isso indica que o texto digitado está após a
									abertura do multilinha e consequentemente dentro do mesmo */
			else if(block == info->start_block)
				found=(start_col >= info->start_col);

			/* Condição 3: O bloco passado é o mesmo do bloco final da informação atual
									e a informação atual trata de um multilinha fechado. Testa
									apenas se a coluna final do parâmetro está antes da coluna
									final da informação multilinha atual indicando que o texto
									inserido está dentro do bloco multilinha */
			else if(info->end_block >=0 && block == info->end_block)
				found=(end_col <= info->end_col);

			/* Condição 4: A informação atual trata de um multilinha em aberto. Testa
									apenas se o bloco passado está no mesmo bloco inicial da informação
									ou após sem a necessidade de se testar as colunas e o bloco final.
									Isso é feito pois se o texto é inserido no meio do multilinha
									após o bloco de abertura, e como o bloco está em aberto, todo texto
									inserido após o bloco de aberto será considerado um multibloco */
			else if(info->end_block < 0)
				found=(block >= info->start_block);

			/* Condição 5: A informação atual trata de um multilinha fechado. Testa
								 apenas se o bloco passado está no meio do intervalo estabelecido
								 pelos blocos inicial e final da informação. Isso é feito
								 pois se o texto é inserido no meio do multilinha após o bloco de
								 abertura e antes do fechamento a linha do texto como um todo
								 é considerada um multibloco */
			else if(info->end_block >=0 && info->start_block!=info->end_block)
				found=(block >= info->start_block && block <= info->end_block);
		}
	}

	if(found)
		return(info);
	else
		return(NULL);
}

void SyntaxHighlighter::removeMultiLineInfo(int block)
{
	vector<MultiLineInfo *>::iterator itr, itr_end;

	itr=multi_line_infos.begin();
	itr_end=multi_line_infos.end();

	//Varre a lista de informações de multilinha
	while(itr!=itr_end)
	{
		//Caso a info atual tenha como bloco inicial o mesmo bloco passado
		if((*itr)->start_block==block)
		{
			//Remove a informação da memória
			delete(*itr);
			//Remove o elemento da lista e reinicia a varredura
			multi_line_infos.erase(itr);
			itr=multi_line_infos.begin();
			itr_end=multi_line_infos.end();
		}
		else
			itr++;
	}
}

unsigned SyntaxHighlighter::getMultiLineInfoCount(int block)
{
	vector<MultiLineInfo *>::iterator itr, itr_end;
	unsigned count=0;

	itr=multi_line_infos.begin();
	itr_end=multi_line_infos.end();

	/* Varre a lista de informação de multilinha e contabiliza
		aquelas as quais tem como bloco inicial o bloco passado */
	while(itr!=itr_end)
	{
		if((*itr)->start_block==block) count++;
		itr++;
	}

	return(count);
}

QString SyntaxHighlighter::identifyWordGroup(const QString &word, const QChar &lookahead_chr, int idx, int &match_idx, int &match_len)
{
	QRegExp expr;
	vector<QString>::iterator itr, itr_end;
	vector<QRegExp>::iterator itr_exp, itr_exp_end;
	vector<QRegExp> *vet_expr=NULL;
	QString group;
	bool match=false, part_mach=false;
	MultiLineInfo *info=NULL;

	//Tenta obter uma informação de multilinha do bloco atual
	info=getMultiLineInfo(idx, idx, current_block);

	/* Caso o destacador estiver no meio do destaque de um bloco
		multi linha, é executado um procedimento diferente que
		é verificar se a palavra em questão não combina com uma
		das expressões finais do grupo indicando que o detaque
		do grupo deve ser interrompido após a palavra em questão */
	if(info)
	{
		group=info->group;

		/* Varre as expresões finais exclusivamente do grupo atual
		 para verificar se a palavra em questão naõ é a de finalização
		 do destaque do grupo */
		itr_exp=final_exprs[group].begin();
		itr_exp_end=final_exprs[group].end();
		part_mach=partial_match[group];

		while(itr_exp!=itr_exp_end && !match)
		{
			//Obtém a expressão do iterador atual
			expr=(*itr_exp);

			//Caso a expressão esteja configurara para combinação parcial
			if(part_mach)
			{
				//Obtém o índice inicial na palavra onde a expressão combina
				match_idx=word.indexOf(expr);
				//Obtém o comprimento da combinação
				match_len=expr.matchedLength();
				/* Para se saber se a expressão combinou parcialmente com a palavra
				verifica se o indice da combinação é igual ou superior a zero */
				match=(match_idx >= 0);
			}
			else
			{
				/* Caso a expressão esteja com o tipo de padrão configurado como
			 como FixedString indica que a mesmo não precisa ser tratada como
			 uma expressão regular e sim como uma string comum, sendo assim
			 a comparação feita é de string com string o que acaba se tornando
			 mais rápido */
				if(expr.patternSyntax()==QRegExp::FixedString)
					match=((expr.pattern().compare(word, expr.caseSensitivity())==0));
				else
					//Combina a expressão regular com a palavra
					match=expr.exactMatch(word);

				if(match)
				{
					match_idx=0;
					match_len=word.length();
				}
			}

			if(match && lookahead_char.count(group) > 0 && lookahead_chr!=lookahead_char.at(group))
				match=false;

			itr_exp++;
		}

		/* Caso a palavra combina com uma das expressões finais do grupo
		 marca a informação multilinha obtida e configura a coluna final
		 e bloco final da informção de bloco multilinha. */
		if(match)
		{
			info->end_col=idx + match_idx + match_len-1;
			info->end_block=current_block;
		}
		/* Caso o destacador permaneça num bloco de multilinha o índice
		 de combinação e o comprimento da combinação serão, respectivamente,
		 zero e o comprimento da palavra para forçar o destaque da palavra
		 atual no grupo de multi linha atual indicando que o destacador ainda
		 se encontra no bloco deste tipo */
		else
		{
			match_idx=0;
			match_len=word.length();
		}

		return(group);
	}
	else
	{
		/* Obtém os iteradores do vetor de ordem de grupos
		 para que as expressões dos mesmos sejam aplicadas
			 palavra em questão com o intuito de verificar se
		 a mesma faz parte do grupo */
		itr=groups_order.begin();
		itr_end=groups_order.end();

		while(itr!=itr_end && !match)
		{
			//Obtém o grupo a partir do iterador
			group=(*itr);
			//Obtém o vetor de expressões iniciais do grupo
			vet_expr=&initial_exprs[group];
			itr++;

			//Varre a lista de expressões comparando com a palavra atual
			itr_exp=vet_expr->begin();
			itr_exp_end=vet_expr->end();
			part_mach=partial_match[group];

			while(itr_exp!=itr_exp_end && !match)
			{
				//Obtém a expressão a partir do iterador atual
				expr=(*itr_exp);

				//Caso a expressão esteja configurara para combinação parcial
				if(part_mach)
				{
					//Obtém o índice inicial na palavra onde a expressão combina
					match_idx=word.indexOf(expr);
					//Obtém o comprimento da combinação
					match_len=expr.matchedLength();
					/* Para se saber se a expressão combinou parcialmente com a palavra
				verifica se o indice da combinação é igual ou superior a zero */
					match=(match_idx >= 0);
				}
				else
				{
					/* Caso a expressão esteja com o tipo de padrão configurado como
					como FixedString indica que a mesmo não precisa ser tratada como
					uma expressão regular e sim como uma string comum, sendo assim
					a comparação feita é de string com string o que acaba se tornando
					mais rápido */
					if(expr.patternSyntax()==QRegExp::FixedString)
						match=((expr.pattern().compare(word, expr.caseSensitivity())==0));
					else
						//Combina a expressão regular com a palavra
						match=expr.exactMatch(word);

					if(match)
					{
						match_idx=0;
						match_len=word.length();
					}
				}

				if(match && lookahead_char.count(group) > 0 && lookahead_chr!=lookahead_char.at(group))
					match=false;

				itr_exp++;
			}

			/* Caso a palavra combine com uma das expressões do grupo
			verifica se o mesmo possui expressões finais o que indica
			que o grupo se trata de elementos de multi linha, ou seja,
			que o destaque do grupo se extende além da linha atual até
			um delimitador final do grupo ser encontrado. Desta
			forma aloca uma informção de multilinha com configurações iniciais */
			if(match && final_exprs.count(group))
			{
				if(!info)
				{
					info=new MultiLineInfo;
					info->group=group;
					info->start_col=idx + match_idx + match_len;
					info->start_block=current_block;
					multi_line_infos.push_back(info);
				}
			}
		}

		/* Caso a palavra não combine com nenhuma expressão de nenhum
		 grupo força o método a retornar um nome de grupo vazio
		 indicando que a palavra não combina com grupo algum */
		if(!match) group="";

		return(group);
	}
}

void SyntaxHighlighter::rehighlight(void)
{
	MultiLineInfo *info=NULL;

	/* Remove todas as informações de multilinha
	pois durante o redestaque as mesmas são obtidas
	novamente */
	while(!multi_line_infos.empty())
	{
		info=multi_line_infos.back();
		multi_line_infos.pop_back();
		delete(info);
	}

	QSyntaxHighlighter::rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString &txt)
{
	/* Caso a linha esteja vazia ou consita apenas de uma
		linha em branco não executa o destacamento para não
		gerar processamento desnecessário. */
	current_block=currentBlock().blockNumber();

	if(!txt.isEmpty())
	{
		QString word, group, text;
		unsigned i=0, len, idx=0, i1;
		int match_idx, match_len, aux_len, start_col;
		QChar chr_delim, chr_lookup;

		//Obtém o tamanho total do bloco de texto
		text=txt + '\n';
		len=text.length();

		/* Remove as informações de multilinha do bloco atual
		 para forçar a identifição de novas informações
		 de multilinha no bloco atual */
		removeMultiLineInfo(current_block);

		do
		{
			/* Remove os caracteres que constam na lista de caracteres
			ignorandos enquanto estes aparecerem no texto */
			while(i < len && ignored_chars.indexOf(text[i])>=0) i++;

			//Caso o fim do texto não tenha sido alcançado
			if(i < len)
			{
				/* Armazena a posição atual no texto pois é a partir dela que será
			 feito o destaque da palavra extraída nas iterações abaixo */
				idx=i;

				//Caso o caractere atual seja um caractere separador de palavras
				if(word_separators.indexOf(text[i])>=0)
				{
					/* Enquanto o caractere for um separado, o mesmo é concatenada junto
				com os demais separadores */
					while(i < len && word_separators.indexOf(text[i])>=0)
						word+=text[i++];
				}
				//Caso o caractere atual seja um delimitador de palavras
				else if(word_delimiters.indexOf(text[i])>=0)
				{
					//Armazena o caractere delimitador
					chr_delim=text[i++];
					//Adiciona-o na palavra que está sendo extraída
					word+=chr_delim;

					/* Extrai todos os próximos caracteres concatenando-o  palavra,
						idenpendente da categoria destes, enquanto o caractere final
				delimitador de palavra não seja encontrado ou o fim do texto
				seja alcançado. */
					while(i < len && chr_delim!=text[i])
						word+=text[i++];

					/* Caso o caractere delimitador final for encontrado concatena-o   palavra
				formando a palavra delimitada como um todo */
					if(i < len && text[i]==chr_delim)
					{
						word+=chr_delim;
						i++;
					}
				}
				/* Caso o caractere atual não se encaixe em nenhuma das categorias
			 acima, será a executada a iteração padrão, que é extrair
			 o caractere até alcançar um separadaor ou delimitador de palavra
			 ou um caractere a ser ignorado */
				else
				{
					while(i < len &&
								word_separators.indexOf(text[i]) < 0 &&
								word_delimiters.indexOf(text[i]) < 0 &&
								ignored_chars.indexOf(text[i]) < 0)
					{
						word+=text[i++];
					}
				}
			}

			/* Caso a palavra não esteja vazia, tenta localizar o grupo
			ao qual ela pertence */
			if(!word.isEmpty())
			{
				i1=i;
				while(i1 < len && ignored_chars.indexOf(text[i1])>=0) i1++;

				if(i1 < len)
					chr_lookup=text[i1];
				else
					chr_lookup='\0';

				//Obtém o grupo ao qual a palavra faz parte
				match_idx=-1;
				match_len=0;
				group=identifyWordGroup(word,chr_lookup, idx, match_idx, match_len);

				/* Caso o grupo foi identificado faz o destaque da palavra
			 usando a posição inicial da palavra com o comprimento
			 da mesma */
				if(!group.isEmpty())
				{
					start_col=idx + match_idx;
					setFormat(start_col, match_len, formats[group]);
				}

				aux_len=(match_idx + match_len);
				if(match_idx >=0 &&  aux_len != word.length())
					i-=word.length() - aux_len;

				//Limpa a palavra atual para obter uma nova
				word="";
			}
		}
		while(i < len);

		/* Armazena a quantidade de informação de multilinhas no bloco atual,
		 pois este atributo é usado para se saber se o documento passará
		 por um redestaque ou não */
		curr_blk_info_count=getMultiLineInfoCount(current_block);
	}
}

bool SyntaxHighlighter::isConfigurationLoaded(void)
{
	return(conf_loaded);
}

void SyntaxHighlighter::clearConfiguration(void)
{
	initial_exprs.clear();
	final_exprs.clear();
	formats.clear();
	partial_match.clear();
	groups_order.clear();
	word_separators.clear();
	word_delimiters.clear();
	ignored_chars.clear();
	lookahead_char.clear();

	configureAttributes();
}

void SyntaxHighlighter::loadConfiguration(const QString &filename)
{
	if(filename!="")
	{
		map<QString, QString> attribs;
		QString elem, expr_type, group;
		bool groups_decl=false, chr_sensitive=false,
				bold=false, italic=false,
				underline=false, partial_match=false;
		QTextCharFormat format;
		QRegExp regexp;
		QColor bg_color, fg_color;
		vector<QString>::iterator itr, itr_end;

		try
		{
			/* Caso o usuário tente carregar uma nova configuração na mesma instância,
			as configurações anteriores são descartadas */
			clearConfiguration();

			//Reinicia o parser XML para a leitura do arquivo
			XMLParser::restartParser();

			/* Montando o caminho padrão para localização do arquivo DTD que define a sintaxe
		 do arquivo xml de destaque de código fonte. */
			XMLParser::setDTDFile(GlobalAttributes::CONFIGURATIONS_DIR +
														GlobalAttributes::DIR_SEPARATOR +
														GlobalAttributes::OBJECT_DTD_DIR +
														GlobalAttributes::DIR_SEPARATOR +
														GlobalAttributes::CODE_HIGHLIGHT_CONF +
														GlobalAttributes::OBJECT_DTD_EXT,
														GlobalAttributes::CODE_HIGHLIGHT_CONF);

			//Carrega o arquivo validando-o de acordo com a DTD informada
			XMLParser::loadXMLFile(filename);

			//Acessa o elemento inicial do arquivo de destaque de código fonte
			if(XMLParser::accessElement(XMLParser::CHILD_ELEMENT))
			{
				do
				{
					if(XMLParser::getElementType()==XML_ELEMENT_NODE)
					{
						//Obtém o elemento atual
						elem=XMLParser::getElementName();

						//Obtém os separadores de palavras da linguagem
						if(elem==ParsersAttributes::WORD_SEPARATORS)
						{
							//Obtém os atributos do mesmo
							XMLParser::getElementAttributes(attribs);
							word_separators=attribs[ParsersAttributes::VALUE];
						}

						//Obtém os delimitadores de palavras da linguagem
						else if(elem==ParsersAttributes::WORD_DELIMITERS)
						{
							//Obtém os atributos do mesmo
							XMLParser::getElementAttributes(attribs);
							word_delimiters=attribs[ParsersAttributes::VALUE];
						}
						else if(elem==ParsersAttributes::IGNORED_CHARS)
						{
							//Obtém os atributos do mesmo
							XMLParser::getElementAttributes(attribs);
							ignored_chars=attribs[ParsersAttributes::VALUE];
						}

						/* Caso o elemento seja o que define a ordem de aplicação dos grupos
				 de destaque (highlight-order). É neste bloco que são declarados
				 os grupos usados para destacar o código-fonte. TODOS os grupos
				 precisam ser declarados neste bloco antes de serem construídos
				 caso contrário será disparado um erro. */
						else if(elem==ParsersAttributes::HIGHLIGHT_ORDER)
						{
							//Marca a flag indicando que os grupos estão sendo declarados
							groups_decl=true;
							//Salva a posição atual do parser xml
							XMLParser::savePosition();
							/* Acesso o primeiro elemento filho da tag de ordem de destaque que
					no caso é uma tag de declaração de grupo <group> */
							XMLParser::accessElement(XMLParser::CHILD_ELEMENT);
							//Obtém o nome do elemento, no caso <group>
							elem=XMLParser::getElementName();
						}

						//Caso o elemento atual seja de construção de um grupo '<group>'
						if(elem==ParsersAttributes::GROUP)
						{
							//Obtém os atributos do mesmo
							XMLParser::getElementAttributes(attribs);
							//Armazena seu nome em uma variável auxiliar
							group=attribs[ParsersAttributes::NAME];

							/* Caso o parser estiver no bloco de declaração de grupos e não no bloco
					de construção dos mesmos, algumas validações serão executadas. */
							if(groups_decl)
							{
								/* 1ª Validação: Verifica se o grupo já não foi declarando anteriormente,
												 para isso o mesmo é buscado na lista que armazena a ordem
												 de aplicação dos grupos (ordem_grupos). Caso o mesmo seja
												 encontrado um erro é disparado. Um grupo é dito como localizado
												 na lista quando a chamada a função find() retorna o iterador
												 diferente do iterador final da lista 'ordem_grupos.end()' */
								if(find(groups_order.begin(), groups_order.end(), group)!=groups_order.end())
								{
									//Dispara o erro indicado que o grupo está sendo redeclarado
									throw Exception(Exception::getErrorMessage(ERR_REDECL_HL_GROUP).arg(group),
																	ERR_REDECL_HL_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								/* 2ª Validação: Verifica se o grupo está sendo declarado e construído ao mesmo tempo no
												 bloco de declaração. Um grupo no bloco de declaração deve aparecer no
												 formato <group name='nome'/>, qualquer construção diferente da apresentada
												 seja com mais atributos ou elementos filhos é considerado que o grupo está
												 sendo construído em local inválido */
								else if(attribs.size() > 1 || XMLParser::hasElement(XMLParser::CHILD_ELEMENT))
								{
									throw Exception(Exception::getErrorMessage(ERR_DEF_INV_GROUP_DECL)
																	.arg(group).arg(ParsersAttributes::HIGHLIGHT_ORDER),
																	ERR_REDECL_HL_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}

								/* Caso nenhum erro for disparado o grupo é adicionad  lista de
					 ordem de aplicação dos grupos */
								groups_order.push_back(group);
							}
							/* Caso o parser estiver no bloco de construção de grupos e não no bloco
					de declaração dos mesmos, algumas validações iniciais serão executadas. */
							else
							{
								/* 1ª Validação: Verifica se o grupo está sendo construído pela segunda vez.
												 Para tal, verifica se o mapa de expressões do grupo foi criado
												 em alguma iteração anterior. Caso exista essa ocorrencia indica
												 que o grupo já foi construído anteriormente,
												 desta forma um erro será disparado ao usuário */
								if(initial_exprs.count(group)!=0)
								{
									//Dispara o erro ao usuário indicando construção duplicada
									throw Exception(Exception::getErrorMessage(ERR_DEF_DUPLIC_GROUP).arg(group),
																	ERR_DEF_DUPLIC_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								/* 2ª Validação: Verifica se o grupo está sendo construído sem ter sido declarado.
												 Para tal, verifica se grupo que está sendo construído não existe
												 na lista de ordem de aplicação de grupos. Um grupo é dito como
												 não localizado na lista quando a chamada a função find() retorna
												 o iterador final da lista 'ordem_grupos.end() */
								else if(find(groups_order.begin(), groups_order.end(), group)==groups_order.end())
								{
									//Dispara o erro indicando que o grupo foi construído e não declarado
									throw Exception(Exception::getErrorMessage(ERR_DEF_NOT_DECL_GROUP)
																	.arg(group).arg(ParsersAttributes::HIGHLIGHT_ORDER),
																	ERR_DEF_NOT_DECL_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}
								/* 3ª Validação: Verifica se o grupo possui elementos filhos. No bloco de construção
												 do grupo é necessário que ele possua pelo menos um filho '<element>'.
												 Caso ele não possua elementos deste tipo um erro é retornado ao usuário */
								else if(!XMLParser::hasElement(XMLParser::CHILD_ELEMENT))
								{
									throw Exception(Exception::getErrorMessage(ERR_DEF_EMPTY_GROUP).arg(group),
																	ERR_DEF_EMPTY_GROUP,__PRETTY_FUNCTION__,__FILE__,__LINE__);
								}

								//Obtém e armazena em variáveis os atributos do grupo que está sendo construído
								chr_sensitive=(attribs[ParsersAttributes::CASE_SENSITIVE]==ParsersAttributes::_TRUE_);
								italic=(attribs[ParsersAttributes::ITALIC]==ParsersAttributes::_TRUE_);
								bold=(attribs[ParsersAttributes::BOLD]==ParsersAttributes::_TRUE_);
								underline=(attribs[ParsersAttributes::UNDERLINE]==ParsersAttributes::_TRUE_);
								partial_match=(attribs[ParsersAttributes::PARTIAL_MATCH]==ParsersAttributes::_TRUE_);
								fg_color.setNamedColor(attribs[ParsersAttributes::FOREGROUND_COLOR]);
								bg_color.setNamedColor(attribs[ParsersAttributes::BACKGROUND_COLOR]);

								if(!attribs[ParsersAttributes::LOOKAHEAD_CHAR].isEmpty())
									lookahead_char[group]=attribs[ParsersAttributes::LOOKAHEAD_CHAR][0];

								//Configura a formatação do grupo de acordo com os atributos obtidos
								format.setFontItalic(italic);
								format.setFontUnderline(underline);

								if(bold)
									format.setFontWeight(QFont::Bold);
								else
									format.setFontWeight(QFont::Normal);

								format.setForeground(fg_color);
								format.setBackground(bg_color);
								formats[group]=format;

								//Salva a posição atual do parser e acesso os elementos filhos do grupo
								XMLParser::savePosition();
								XMLParser::accessElement(XMLParser::CHILD_ELEMENT);

								/* Configura a variável de expressão regular para ser sensível a
					 caracteres (case sensitive) de acordo com o mesmo atributo
					 obtido do xml */
								if(chr_sensitive)
									regexp.setCaseSensitivity(Qt::CaseSensitive);
								else
									regexp.setCaseSensitivity(Qt::CaseInsensitive);

								this->partial_match[group]=partial_match;

								do
								{
									if(XMLParser::getElementType()==XML_ELEMENT_NODE)
									{
										//Obtém os atributos do elemento filho do grupo
										XMLParser::getElementAttributes(attribs);
										//Obtém o tipo do elemento
										expr_type=attribs[ParsersAttributes::TYPE];

										//Configura a expressão regular com o valor presente no atributo 'value' do elemento
										regexp.setPattern(attribs[ParsersAttributes::VALUE]);

										if(attribs[ParsersAttributes::REGULAR_EXP]==ParsersAttributes::_TRUE_)
											regexp.setPatternSyntax(QRegExp::RegExp);
										else
											regexp.setPatternSyntax(QRegExp::FixedString);

										/* A expressão regular configura será inserida na lista de expressões
						 de acordo com o tipo do elemento */
										if(expr_type=="" ||
											 expr_type==ParsersAttributes::EXP_SIMPLES ||
											 expr_type==ParsersAttributes::INITIAL_EXP)
											initial_exprs[group].push_back(regexp);
										else
											final_exprs[group].push_back(regexp);
									}
								}
								while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));

								//Restaura a posição do parser para continuar a leitura dos próximos grupos
								XMLParser::restorePosition();
							}
						}
					}

					/* Após a inserção do grupo, verifica se existem outros grupos a serem
				declarados. Caso não existe, desmarca a flag de declaração de grupos
				e restaura a posição do parser para que o restante do arquivo possa
				ser lido */
					if(groups_decl && !XMLParser::hasElement(XMLParser::NEXT_ELEMENT))
					{
						groups_decl=false;
						XMLParser::restorePosition();
					}

				}
				while(XMLParser::accessElement(XMLParser::NEXT_ELEMENT));
			}

			/* Executa a validação final do carregamento do arquivo que consiste em
			verificar se algum grupo foi declarado porém não construído. Para
			isso, a lista de ordem de grupos é varrida e verifica-se se
			existem expressões para tal grupo. Caso não exista expressões
			para o grupo indica que o mesmo foi declarado e não foi construído */
			itr=groups_order.begin();
			itr_end=groups_order.end();

			while(itr!=itr_end)
			{
				group=(*itr);
				itr++;

				//Caso o número de expressões do grupo seja zero
				if(initial_exprs[group].size()==0)
				{
					//Dispara o erro indicando que o grupo foi declarado porém não construído
					throw Exception(Exception::getErrorMessage(ERR_GROUP_DECL_NOT_DEFINED).arg(group),
													ERR_GROUP_DECL_NOT_DEFINED,__PRETTY_FUNCTION__,__FILE__,__LINE__);
				}
			}

			//Marca a flag indicando que a configuração foi carregada com sucesso
			conf_loaded=true;
		}
		catch(Exception &e)
		{
			//Captura e redireciona erros das demais bibliotecas
			throw Exception(e.getErrorMessage(),e.getErrorType(),__PRETTY_FUNCTION__,__FILE__,__LINE__, &e);
		}
	}
}

