#include "excecao.h"
//***********************************************************
QString Excecao::mensagens[QTD_ERROS][2]={
{"ERR_NULO", " "},
{"ERR_GLOBAL_OBJBADALOC", QObject::tr("Espaço em memória insuficiente para alocar o objeto!")},
{"ERR_PGMODELER_ATRPSDTIPOCOL", QObject::tr("Atribuição de um pseudo-tipo ao tipo da coluna!")},
{"ERR_PGMODELER_ATRCOMPRIMZERO", QObject::tr("Atribuição de comprimento igual a zero!")},
{"ERR_PGMODELER_ATRPRECISAOINV", QObject::tr("Atribuição de precisão maior do que o comprimento do tipo!")},
{"ERR_PGMODELER_ATRPRECTIPOTIMEINV", QObject::tr("Atribuição de precisão inválida a um tipo time, timestamp ou interval. A precisão neste caso deve ser igual ou inferior a 6!")},
{"ERR_PGMODELER_ATRCOLNAOALOC", QObject::tr("Atribuição de coluna não alocada ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_REFCOLIDXINV", QObject::tr("Referência a uma coluna com índice fora da capacidade da lista de colunas!")},
{"ERR_PGMODELER_ATROBJNAOALOC", QObject::tr("Atribuição de um objeto não alocado!")},
{"ERR_PGMODELER_ATRESQNAOALOC", QObject::tr("Atribuição de um esquema não alocado ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_ATROBJDEFSQLINV", QObject::tr("O objeto '%1' (%2) possui configuração inconsistente (definição SQL/XML inválida)!")},
{"ERR_PGMODELER_ATROBJDUPLIC", QObject::tr("O objeto '%1' (%2) já existente na lista de elementos desse mesmo tipo no objeto '%3' (%4)!")},
{"ERR_PGMODELER_ATROBJDUPLICCONT", QObject::tr("O objeto '%1' (%2) não pode ser atribuído pois já existe no container de objetos '%3'!")},
{"ERR_PGMODELER_ATROBJTIPOINV", QObject::tr("Atribuição de objeto de tipo inválido!")},
{"ERR_PGMODELER_REMOBJTIPOINV", QObject::tr("Remoção de objeto de tipo inválido!")},
{"ERR_PGMODELER_OBTOBJTIPOINV", QObject::tr("Obtenção de objeto de tipo inválido!")},
{"ERR_PGMODELER_ATRNOMEVAZIOTPRET", QObject::tr("Atribuição de nome vazio ao tipo de retorno de tabela!")},
{"ERR_PGMODELER_INSPARAMDUPLIC", QObject::tr("A inserção do parâmetro '%1' não será possível pois já existe outro parâmetro com mesmo nome na função '%2'!")},
{"ERR_PGMODELER_INSTPRETDUPLIC", QObject::tr("A inserção do tipo de retorno de tabela '%1' não será possível pois já existe outro tipo de retorno com mesmo nome na função '%2'!")},
{"ERR_PGMODELER_REFPARAMIDXINV", QObject::tr("Referência a um parâmetro com índice fora da capacidade da lista de parâmetros!")},
{"ERR_PGMODELER_REFEVENTOGATINV", QObject::tr("Referência a um evento não pertecente ao gatilho!")},
{"ERR_PGMODELER_ATRCOLINVGATILHO",QObject::tr("A coluna '%1' não pode ser atribuída ao gatilho '%2' pois a mesma não pertence à tabela pai do gatilho!")},
{"ERR_PGMODELER_ATRFUNCNAOALOC", QObject::tr("Atribuição de função não alocada ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCGATINV", QObject::tr("Atribuição de função com tipo de retorno diferente de 'trigger'!")},
{"ERR_PGMODELER_ATRFUNCNUMPARAMINV", QObject::tr("Atribuição de função com número de parâmetros inválido ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCLINGINV", QObject::tr("Atribuição de função com linguagem inválida!")},
{"ERR_PGMODELER_ATRTABNAOALOC", QObject::tr("Atribuição de tabela não alocada ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_REFARGIDXINV", QObject::tr("Referência a um argumento com índice fora da capacidade da lista de argumentos!")},
{"ERR_PGMODELER_INSCOLDUPLIC", QObject::tr("Inserção de coluna com nome já registrado em outra coluna do objeto!")},
//{"ERR_PGMODELER_ATRTAMGRADEINV", QObject::tr("Atribuição de tamanho de grade inválido!")},
//{"ERR_PGMODELER_DESSELOBJNAOALOC", QObject::tr("Desenho de seleção de objeto não alocado!")},
//{"ERR_PGMODELER_TIPOOBJINV", QObject::tr("Tipo de objeto inválido!")},
{"ERR_PGMODELER_ATRNOMEOBJVAZIO", QObject::tr("Atribuição de nome vazio a um objeto!")},
{"ERR_PGMODELER_ATRNOMEOBJINV", QObject::tr("Atribuição de nome inválido a um objeto!")},
{"ERR_PGMODELER_ATRESQTIPOINV", QObject::tr("Atribuição de um objeto esquema com tipo inválido!")},
{"ERR_PGMODELER_ATRESPTABTIPOINV", QObject::tr("Atribuição de um objeto espaço de tabela com tipo inválido!")},
{"ERR_PGMODELER_ATRESPTABOBJINV", QObject::tr("Atribuição de um espaço de tabela a um objeto de tipo inválido!")},
{"ERR_PGMODELER_ATRESPTABRESTTIPOINV", QObject::tr("Atribuição de um objeto espaço de tabela a uma restrição de tipo inválido! A restrição deve ser uma chave-primária ou única para pode pertencer a um espaço de tabela!")},
{"ERR_PGMODELER_ATRPAPELTIPOINV", QObject::tr("Atribuição de um objeto papel com tipo inválido!")},
{"ERR_PGMODELER_ATRDONOOBJINV", QObject::tr("Atribuição de um dono a um objeto de tipo inválido!")},
{"ERR_PGMODELER_REFFUNCTIPOINV", QObject::tr("Referência a uma função com tipo inválido!")},
{"ERR_PGMODELER_REFARGOPTIPOINV", QObject::tr("Referência a um argumento do operador com tipo inválido!")},
{"ERR_PGMODELER_REFOPTIPOINV", QObject::tr("Referência a um operador com tipo inválido!")},
{"ERR_PGMODELER_ATROPPAPELTIPOINV", QObject::tr("Atribuição de valor a uma opção de tipo inválido no papel!")},
{"ERR_PGMODELER_REFPAPELINV", QObject::tr("Referência a um tipo de papel inválido!")},
{"ERR_PGMODELER_INSITEMPAPELDUPLIC", QObject::tr("A inserção do papel '%1' não será possível pois este já está sendo referenciado pelo papel '%2'!")},
{"ERR_PGMODELER_REFREDUNDANTEPAPEIS",QObject::tr("Referência redundante detectada ao fazer com que o papel '%1' referencie o papel '%2'!")},
{"ERR_PGMODELER_ATRMEBROPROPPAPEL", QObject::tr("O papel '%1' não pode ser relacionado com um membro dele mesmo!")},
{"ERR_PGMODELER_REFPAPELIDXINV", QObject::tr("Referência a um papel com índice fora da capacidade da lista de papéis!")},
{"ERR_PGMODELER_INSCMDNULO", QObject::tr("Inserção de comando nulo à regra!")},
{"ERR_PGMODELER_REFCMDIDXINV", QObject::tr("Referência a um comando com índice fora da capacidade da lista de comandos!")},
{"ERR_PGMODELER_HERANCATABINV", QObject::tr("Não é possível criar um auto-relacionamento de generalização ou dependência! A tabela não pode herdar ou copiar seus próprios atributos!")},
{"ERR_PGMODELER_ATROBJRELAC", QObject::tr("Atribuição de um objeto o qual já pertence a outra tabela!")},
{"ERR_PGMODELER_ATRESQDIFSEQ", QObject::tr("Atribuição de esquema à seqüência o qual difere do esquema da tabela possuidora!")},
{"ERR_PGMODELER_ATRESQVALORINV", QObject::tr("Atribuição de valor inválido a um dos atributos da seqüência!")},
{"ERR_PGMODELER_ATRESQVALORMININV", QObject::tr("Atribuição de valor mínimo da seqüência maior do que o valor máximo!")},
{"ERR_PGMODELER_ATRESQVALORINIINV", QObject::tr("Atribuição de valor de início da seqüência extrapolando o intervalo definido pelos valores mínimo e máximo!")},
{"ERR_PGMODELER_ATRESQINCNULO", QObject::tr("Atribuição de incremento de seqüência com valor nulo!")},
{"ERR_PGMODELER_ATRESQCACHENULO", QObject::tr("Atribuição de cache de seqüência com valor nulo!")},
{"ERR_PGMODELER_ATRESQDIFTAB", QObject::tr("Atribuição de tabela possuidora a qual não faz parte do mesmo esquema da seqüência '%1'!")},
{"ERR_PGMODELER_ATRDONODIFTAB", QObject::tr("Atribuição de tabela possuidora a qual não pertence ao mesmo dono da seqüência  '%1'!")},
{"ERR_PGMODELER_ATRCOLPOSINDEF", QObject::tr("Atribuição de coluna possuidora inexistente à seqüência '%1'!")},
{"ERR_PGMODELER_ATRCOLPOSNAORELAC", QObject::tr("Atribuição de coluna possuidora a sequência '%1' não relacionada a uma tabela!")},
{"ERR_PGMODELER_REFROTIDXINV", QObject::tr("Referência a um rótulo com índice fora da capacidade da lista de rótulos!")},
{"ERR_PGMODELER_ALOCOBJTIPOINV", QObject::tr("Alocação de objeto com tipo inválido!")},
//{"ERR_PGMODELER_REDOBJNAOALOC", QObject::tr("Redimensionamento de objeto não alocado!")},
{"ERR_PGMODELER_ATRFUNCRETINV", QObject::tr("Atribuição de função com tipo de retorno inválido ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_ATRFUNCPARAMINV", QObject::tr("Atribuição de função com tipo de parâmetro(s) inválido(s) ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_ATRLINGNAOALOC", QObject::tr("Atribuição de linguagem não alocada!")},
{"ERR_PGMODELER_ATRLINGTIPOINV", QObject::tr("Atribuição de linguagem com tipo inválido!")},
{"ERR_PGMODELER_REFTIPOIDXINV", QObject::tr("Referência a tipo de dado com índice fora da capacidade da lista de tipos de dados!")},
{"ERR_PGMODELER_ATRTIPONULO", QObject::tr("Atribuição de tipo nulo ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_ATRTIPOINVOBJ", QObject::tr("Atribuição de tipo inválido ao objeto!")},
{"ERR_PGMODELER_ATRNOMEDIRVAZIO", QObject::tr("Atribuição de diretório vazio ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_OBTQTDTIPOINV", QObject::tr("Obtenção de tipos com quantidade inválida!")},
{"ERR_PGMODELER_INSATRIBTIPODUPLIC", QObject::tr("Inserção de item já existente na lista de atibutos do tipo!")},
{"ERR_PGMODELER_INSATRIBTIPOINV", QObject::tr("Inserção de item inválido na lista de atibutos do tipo!")},
{"ERR_PGMODELER_INSENUMTIPODUPLIC", QObject::tr("Inserção de item já existente na lista de enumerações do tipo!")},
{"ERR_PGMODELER_INSENUMTIPOINV", QObject::tr("Inserção de item inválido na lista de emumerações do tipo!")},
{"ERR_PGMODELER_REFATRIBTIPOIDXINV", QObject::tr("Referência a um atributo com índice fora da capacidade da lista de atributos!")},
{"ERR_PGMODELER_REFENUMTIPOIDXINV", QObject::tr("Referência a uma enumeração com índice fora da capacidade da lista de enumerações!")},
{"ERR_PGMODELER_ATRCONFTIPOINV", QObject::tr("Atribuição de configuração inválida ao tipo!")},
{"ERR_PGMODELER_INSTIPODUPLIC", QObject::tr("O tipo de dado '%1' não pode ser atribuído pois o mesmo já existente na lista de tipos da função de agregação '%2'!")},
{"ERR_PGMODELER_ATROPFUNCAGRARGINV", QObject::tr("Atribuição de operador a uma função de agregação com quantidade de tipos de entrada inválida!")},
{"ERR_PGMODELER_ATROPFUNCAGTIPOSINV", QObject::tr("Atribuição de operador com argumentos de tipos inválidos!")},
{"ERR_PGMODELER_ATRNOMERESERV", QObject::tr("Atribuição de nome reservado pelo sistema ao objeto '%1' (%2)!")},
{"ERR_PGMODELER_FUNCCONFIGINV", QObject::tr("Uma função de configuração inválida está sendo usada pelo objeto '%1' (%2)!")},
{"ERR_PGMODELER_ATRIDUSUARIOINV", QObject::tr("Atribuição de id de usuário inválido!")},
{"ERR_PGMODELER_ATRVALORINVNUMEST", QObject::tr("Atribuição de valor inválido ao número de estratégia/suporte do elemento de classe de operadores!")},
{"ERR_PGMODELER_INSELEMDUPLIC", QObject::tr("Inserção de elemento já existente na lista de elementos!")},
{"ERR_PGMODELER_REFELEMIDXINV", QObject::tr("Referência a um elemento com índice fora da capacidade da lista de elementos!")},
{"ERR_PGMODELER_REFOBJIDXINV", QObject::tr("Referência a um objeto com índice fora da capacidade da lista de objetos!")},
{"ERR_PGMODELER_REMOBJNAOALOC", QObject::tr("Remoção de objeto não alocado!")},
{"ERR_PGMODELER_REMOBJREFERDIR", QObject::tr("O objeto '%1' (%2) não pode ser removido pois está sendo referenciado pelo objeto '%3' (%4)!")},
{"ERR_PGMODELER_REMOBJREFERIND", QObject::tr("O objeto '%1' (%2) não pode ser removido pois está sendo referenciado pelo objeto '%3' (%4) pertecente à '%5' (%6)!")},
{"ERR_PGMODELER_OPROBJTIPOINV", QObject::tr("Operação com objeto(s) de tipo(s) inválido(s)!")},
{"ERR_PGMODELER_REFOBJTIPOINV", QObject::tr("Referência a um objeto com tipo inválido!")},
{"ERR_PGMODELER_OPROBJNAOALOC", QObject::tr("Operação com objeto não alocado!")},
{"ERR_PGMODELER_RELTABSEMPK", QObject::tr("A criação do relacionamento '%1' entre as tabela '%2' e '%3' não pode ser feito pois uma delas não possui uma chave primária. Caso o relacionamento seja do tipo n-n ambas as tabelas devem possuir chaves primárias!")},
{"ERR_PGMODELER_TIPORELNAOIMPL", QObject::tr("O relacionamento do tipo 1-1 onde ambas as tabelas são de participação obrigatória não é implementado por necessitar de fusão entre tabelas o que quebra a modelagem feita pelo usuário!")},
{"ERR_PGMODELER_ATREXPRINV", QObject::tr("Atribuição de expressão inválida ao objeto!")},
//{"ERR_PGMODELER_REFCOROBJINV", QObject::tr("Referência às cores de um objeto de tipo inválido!")},
{"ERR_PGMODELER_ATRPKEXISTETAB", QObject::tr("Atribuição de chave primária à uma tabela a qual já possui uma!")},
{"ERR_PGMODELER_RELIDENTINV", QObject::tr("Relacionamento identificador não pode ser criado para um autorelacionamentos, relacionamentos do tipo n-n, dependência ou generalização!")},
{"ERR_PGMODELER_RELINVCOLDUPLIC", QObject::tr("Impossível criar o relacionamento de dependência pois a coluna '%1' da tabela '%2' já existe na tabela '%3'!")},
{"ERR_PGMODELER_RELINVCOLSINCOMP", QObject::tr("Impossível criar o relacionamento de generalização pois a coluna '%1' da tabela '%2' não pode ser fundida com a coluna '%3' da tabela '%4' por serem de tipos incompatíveis!")},
{"ERR_PGMODELER_ATROBJRELINV", QObject::tr("Um atributo não pode ser adicionado a um relacionamento de generalização ou dependência!")},
{"ERR_PGMODELER_ATRFKRELAC", QObject::tr("Uma chave estrangeira não pode ser adicionado a um relacionamento pois esta é criada automaticamente no momento da ligação do mesmo!")},
{"ERR_PGMODELER_REFOBJINEXISTE", QObject::tr("O objeto '%1' (%2) está referenciando o objeto '%3' (%4) o qual não foi encontrado na lista de objetos de mesmo tipo no modelo!")},
{"ERR_PGMODELER_REFTIPOUSRINV", QObject::tr("Referência a tipo de dado definido pelo usuário o qual não existe no modelo!")},
{"ERR_PGMODELER_ATRTAMMAXINVLISTA", QObject::tr("Atribuição de tamanho máximo inválido à lista de operações!")},
{"ERR_PGMODELER_ARQNAOGRAVADO", QObject::tr("Não foi possível gravar o modelo no arquivo %1! Verifique se o diretório existe, ou se o usuário possui permissões de acesso sobre o mesmo!")},
//{"ERR_PGMODELER_ARQNAOCARREGADO", QObject::tr("Não foi possível carregar o arquivo %1! Verifique se o diretório existe, ou se o usuário possui permissões de acesso sobre o mesmo!")},
{"ERR_PGMODELER_ARQNAOGRAVADODEFINV", QObject::tr("Não foi possível gravar o modelo no arquivo %1 devido a um ou mais erros na geração da definição do mesmo!")},
{"ERR_PGMODELER_RELEXISTEMODELO", QObject::tr("Já existe um relacionamento entre a %1 %2 e a %3 %4 no modelo!")},
{"ERR_PGMODELER_INSRELGERAREDUNDANCIA", QObject::tr("A configuração do relacionamento '%1' gera uma redundância entre os relacionamentos '%2'. Redundância de relacionamentos identificadores ou de generalização/dependência não são aceitos pois resultam em propagação incorreta de colunas tornando o modelo inconsiste!")},
{"ERR_PGMODELER_REFCOLUNAINVTABELA", QObject::tr("Um ou mais objetos foram invalidados e automaticamente removidos pois os mesmos referenciavam colunas de tabelas as quais foram incluídas através de relacionamentos e que deixaram de existir devido a desconexão ou exclusão dos relacionamentos geradores de tais colunas!")},
{"ERR_PGMODELER_ALOCPKFORMAINVALIDA", QObject::tr("A chave-primária '%1' só pode ser alocada caso a mesma esteja declarada dentro de um bloco de código que define uma tabela ou relacionamento!")},
{"ERR_PGMODELER_REFPRIVLEGIOIDXINV",QObject::tr("Referência a um tipo de privilégio inválido!")},
{"ERR_PGMODELER_INSPAPELDUPLICPERM", QObject::tr("Inserção de um papel já existente na lista de papéis da permissão!")},
{"ERR_PGMODELER_ATRPRIVILEGIOOBJINV", QObject::tr("Atribuição de privilégio incompatível com o tipo do objeto referenciado pela permissão!")},
{"ERR_PGMODELER_ATRPERMISSAODUPLIC", QObject::tr("Já existe uma permissão sobre o objeto '%1' (%2) a qual possui um ou mais papéis iguais aos presentes na permissão a ser atribuída ao objeto!")},
{"ERR_PGMODELER_PERMREFOBJINEXISTE", QObject::tr("Uma permissão está referenciando o objeto '%1' (%2) o qual não foi encontrado no modelo!")},
{"ERR_PGMODELER_OBJSEMESQUEMA", QObject::tr("O objeto '%1' (%2) não pode ser criado por não estar atribuído a nenhum esquema!")},
{"ERR_PGMODELER_ATRESPTABDIRDUPLIC", QObject::tr("O espaço de tabela '%1' não pode ser inserido no modelo pois o mesmo aponta para o mesmo diretório que o espaço de tabela '%2'!")},
{"ERR_PGMODELER_ATRDIMENSAOINVDOMINIO", QObject::tr("Não é possível criar arrays de domínios (dimensão >= 1)! O PostgreSQL ainda não implementa esta funcionalidade!")},
{"ERR_PGMODELER_ATRCORPOFUNCLINGC", QObject::tr("A função '%1' não pode receber um código-fonte como definição pois sua linguagem está definida como C. Os atributos símbolo e biblioteca dinâmica é que devem ser utilizados!")},
{"ERR_PGMODELER_ATRBIBLIOFUNCLINGDIFC", QObject::tr("A função '%1' só pode ter os atributos de símbolo e biblioteca configurados caso sua linguagem esteja definida como C. Para os demais casos é necessário especificar um código-fonte que a define o SGBD!")},
{"ERR_PGMODELER_ATROPERCOMUTINV", QObject::tr("O operador '%1' não pode ser atribuído como comutador do operador '%2' pois possui configuração incompatível!")},
{"ERR_PGMODELER_ATROPERNEGINV", QObject::tr("O operador '%1' não pode ser atribuído como negador do operador '%2' pois possui configuração incompatível!")},
{"ERR_PGMODELER_TIPOUSRAUTOREF", QObject::tr("O tipo '%1' não pode se referenciar nos atributos 'elemento' ou 'tipo cópia' ou ser usado como tipo de dado de um atributo na configuração de tipo composto!")},
{"ERR_PGMODELER_ATRELEMENTOINVTIPO", QObject::tr("Atribuição de elemento inválido ao tipo '%1'!")},
{"ERR_PGMODELER_ATRALINHAMENTOINVTIPO",QObject::tr("Atribuição de alinhamento inválido ao tipo '%1'!")},
{"ERR_PGMODELER_ATRNOMEINVTABRELNN",  QObject::tr("Atribuição de nome inválido à tabela gerada por relacionamento N-N!")},
{"ERR_PGMODELER_PKESPTIPORELINV", QObject::tr("O relacionamento '%1' não pode fazer uso da chave-primária especial. Apenas relacionamentos de generalização/dependência têm acesso a este recurso!")},
{"ERR_PGMODELERUI_OPROBJINCRELACAO", QObject::tr("O objeto '%1' (%2) não pode ser editado ou excluído por ter sido incluído automaticamente através de relacionamento! Para manipular o objeto é necessário fazê-lo a partir do formulário de edição do relacionamento que o detém.")},
{"ERR_PGMODELERUI_REMOBJPROTEGIDO", QObject::tr("O objeto '%1' (%2) não pode ser excluído por estar protegido!")},
{"ERR_PGMODELERUI_REDECLGRUPODESTAQUE", QObject::tr("O grupo '%1' já foi declarado anteriormente!")},
{"ERR_PGMODELERUI_DEFGRUPOLOCALINV", QObject::tr("O grupo '%1' não pode ser construído no bloco de declaração de grupos ('%2')!")},
{"ERR_PGMODELERUI_DEFGRUPONAODECL", QObject::tr("O grupo '%1' foi construído porém não declarado no bloco de declaração de grupos ('%2')!")},
{"ERR_PGMODELERUI_DEFGRUPOSEMELEM", QObject::tr("O grupo '%1' não pode ser construído sem possuir elementos filhos!")},
{"ERR_PGMODELERUI_DEFGRUPODUPLICADA", QObject::tr("O grupo '%1' não pode ser construído novamente pois já foi construído em blocos anteriores!")},
{"ERR_PGMODELERUI_GRUPODECLNAODEF", QObject::tr("O grupo '%1' foi declarado porém não construído!")},
{"ERR_PGMODELERUI_REFCOLTABOBJIDXINV", QObject::tr("Referência a uma coluna da tabela de objetos com índice inválido!")},
{"ERR_PGMODELERUI_REFLINTABOBJIDXINV", QObject::tr("Referência a uma linha da tabela de objetos com índice inválido!")},
{"ERR_PGMODELERUI_OPROBJRESERVADO", QObject::tr("O esquema 'public' e as linguagens 'plpgsql', 'c' e 'sql' não podem ser manipuladas por serem reservados ao PostgreSQL! Os mesmos estão presentes no modelo de banco de dados apenas como referência!")},
{"ERR_PGMODELERUI_CONFFUNCINVALIDAOBJ", QObject::tr("A nova configuração da função invalida o objeto '%1' (%2) o qual a referencia! Neste caso é necessário desfazer a relação entre o objeto afetado e a função para que a nova configuração desta última seja efetivada!")},
{"ERR_PGMODELERUI_TIPOSQLINDEFREFVISAO", QObject::tr("Uma referência de visão deve possuir pelo menos uma aplicação SQL: SELECT-FROM, FROM-WHERE ou Após WHERE!")},
{"ERR_PGMODELERUI_RESTPKFKSEMCOLUNAS", QObject::tr("Restrições do tipo chave-primária, chave-estrangeira ou única devem possuir pelo menos uma coluna relacionada às mesmas! Para chaves-estrangeira devem ser selecionadas, adicionalmente, as colunas referenciadas!")},
{"ERR_PGMODELERUI_CONFNAOCARREGADA", QObject::tr("Não foi possível carregar um ou mais arquivos de configuração! Por favor, cheque se os arquivos existem na pasta de configurações e se o mesmos não estão corrompidos para que este erro não se repita na próxima inicialização!")},
{"ERR_PGMODELERUI_CONFPADRAONAORESTAURADA", QObject::tr("Não foi possível encontrar o arquivo de configurações padrão '%1'! Para restaurar as configurações padrão cheque a existência do referido arquivo e tente novamente!")},
{"ERR_PGMODELERUI_FALHAEXPORTACAO", QObject::tr("O processo de exportação falhou devido a um erro disparado pelo servidor PostgreSQL na tentativa de execução de um comando SQL. Para obter mais detalhes sobre erro cheque a pilha de exceções!\n\n** Comando SQL executado: **\n\n%1")},
{"ERR_PARSERS_SINTAXEINV", QObject::tr("Sintaxe inválida no arquivo %1, linha %2, coluna %3!")},
{"ERR_PARSERS_CONDICIONALINV", QObject::tr("Instrução condicional '%1' inválida no arquivo %2, linha %3, coluna %4!")},
{"ERR_PARSERS_ATRIBUTODESC", QObject::tr("Atributo '%1' desconhecido no arquivo %2, linha %3, coluna %4!")},
{"ERR_PARSERS_METACHARINV", QObject::tr("Metacaractere '%1' inválido no arquivo %2, linha %3, coluna %4!")},
{"ERR_PARSERS_ATRIBVALORNULO", QObject::tr("Atributo '%1' com valor indefinido no arquivo %2, linha %3, coluna %4!")},
{"ERR_PARSERS_ATRIBBUFXMLVAZIO", QObject::tr("Atribuição de buffer de código XML vazio ao parser!")},
{"ERR_PARSERS_ARQDIRNAOCARREGADO", QObject::tr("Não foi possível acessar o arquivo ou diretório %1! Verifique se ele existe ou se o usuário possui permissões de acesso sobre o mesmo!")},
{"ERR_PARSERS_ATRIBARQDTDVAZIO", QObject::tr("Atribuição de nome de arquivo DTD vazio!")},
{"ERR_PARSERS_ATRIBNOMEDTDVAZIO", QObject::tr("Atribuição de nome vazio à declaração DTD!")},
{"ERR_PARSERS_LIBXMLERR", QObject::tr("Erro ao interpretar buffer XML na linha %1 coluna %2.\n Mensagem gerada pelo parser: '%3'. %4")},
{"ERR_PARSERS_OPRARVELEMNAOALOC", QObject::tr("Operação sobre árvore de elementos não alocada! É necessário carregar o buffer XML do parser e interpretá-lo para que a árvore seja gerada!")},
{"ERR_PARSERS_ARQMODELOINV", QObject::tr("Não foi possível carregar o arquivo %1. O mesmo parece estar inconsistente ou uma de suas dependências (arquivos DTD) possui erros ou está ausente!")},
{"ERR_PARSERS_OPRELEMNAOALOC", QObject::tr("Operação com elemento de árvore de elementos não alocado!")},
{"ERR_PARSERS_OPRELEMINVARVDOC", QObject::tr("Operação com elemento o qual não faz parte da árvore de elementos carregada atualmente!")},
{"ERR_CONEXBD_ATRPARAMINVCONEX", QObject::tr("Atribuição de valor a um parâmetro de conexão inválido!")},
{"ERR_CONEXBD_OPRCONEXNAOALOC", QObject::tr("Operação sobre conexão não estabelecida!")},
{"ERR_CONEXBD_CONEXSEMPARAMCONFIG", QObject::tr("Tentativa de conexão sem parâmetros de configuração definidos!")},
{"ERR_CONEXBD_CONEXNAOESTABELECIDA", QObject::tr("Não foi possível estabelecer conexão com o banco de dados.\nMensagem retornada: '%1'")},
{"ERR_CONEXBD_ATRRESSQLNAOALOC", QObject::tr("Atribuição de resultado de comando SQL não alocado!")},
{"ERR_CONEXBD_SGBDRESPINCOMP", QObject::tr("Não foi possível alocar o resultado do comando SQL pois a resposta do SGBD não foi compreendida pelo cliente!")},
{"ERR_CONEXBD_SGBDERROFATAL", QObject::tr("Não foi possível alocar o resultado do comando SQL pois o servidor gerou um erro fatal!\nMensagem retornada pelo SGBD: %1")},
{"ERR_CONEXBD_COMANDOSQLVAZIO", QObject::tr("Não foi possível alocar o resultado do comando SQL pois o comando passado foi uma SQL vazia!")},
{"ERR_CONEXBD_REFCOLTUPLAIDXINV", QObject::tr("Referência à uma coluna da tupla com índice inválido!")},
{"ERR_CONEXBD_REFCOLTUPLANOMEINV", QObject::tr("Referência à uma coluna da tupla com nome inválido!")},
{"ERR_CONEXBD_REFTUPLANAOEXISTE", QObject::tr("Referência à uma tupla com índice inválido ou o resultado está vazio (sem tuplas)!")},
{"ERR_CONEXBD_CMDSQLNAOEXECUTADO", "Não foi possível executar o comando SQL.\n Mensagem retornada: %1"},
};
//-----------------------------------------------------------
Excecao::Excecao(void)
{
 configurarExcecao("",ERR_NULO,"","",-1,"");
}
//-----------------------------------------------------------
Excecao::Excecao(const QString &msg, const QString &local, const QString &arquivo, int linha, Excecao *excecao, const QString &info_adicional)
{
 configurarExcecao(msg,tipo_erro, local, arquivo, linha, info_adicional);
 if(excecao) adicionarExecao(*excecao);
}
//-----------------------------------------------------------
Excecao::Excecao(TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, Excecao *excecao, const QString &info_adicional)
{
 configurarExcecao(mensagens[tipo_erro][MENSAGEM_ERRO],tipo_erro, local, arquivo, linha, info_adicional);
 if(excecao) adicionarExecao(*excecao);
}
//-----------------------------------------------------------
Excecao::Excecao(const QString &msg, TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, Excecao *excecao, const QString &info_adicional)
{
 configurarExcecao(msg,tipo_erro, local, arquivo, linha, info_adicional);
 if(excecao) adicionarExecao(*excecao);
}
//-----------------------------------------------------------
Excecao::Excecao(TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, vector<Excecao> &excecoes, const QString &info_adicional)
{
 vector<Excecao>::iterator itr, itr_end;

 configurarExcecao(mensagens[tipo_erro][MENSAGEM_ERRO],tipo_erro, local, arquivo, linha, info_adicional);

 itr=excecoes.begin();
 itr_end=excecoes.end();
 while(itr!=itr_end)
 {
  adicionarExecao((*itr));
  itr++;
 }
}
//-----------------------------------------------------------
void Excecao::configurarExcecao(const QString &msg, TipoErro tipo_erro, const QString &local, const QString &arquivo, int linha, const QString &info_adicional)
{
 this->tipo_erro=tipo_erro;
 this->msg_erro=msg;
 this->local=local;
 this->arquivo=arquivo;
 this->linha=linha;
 this->info_adicional=info_adicional;
}
//-----------------------------------------------------------
QString Excecao::obterMensagemErro(void)
{
 return(msg_erro);
}
//-----------------------------------------------------------
QString Excecao::obterMensagemErro(TipoErro tipo_erro)
{
 if(tipo_erro < QTD_ERROS)
   return(QObject::tr(mensagens[tipo_erro][MENSAGEM_ERRO]));
 else
  return("");
}
//-----------------------------------------------------------
QString Excecao::obterNomeErro(TipoErro tipo_erro)
{
 if(tipo_erro < QTD_ERROS)
   return(mensagens[tipo_erro][CODIGO_ERRO]);
 else
  return("");
}
//-----------------------------------------------------------
QString Excecao::obterLocal(void)
{
 return(local);
}
//-----------------------------------------------------------
QString Excecao::obterArquivo(void)
{
 return(arquivo);
}
//-----------------------------------------------------------
QString Excecao::obterLinha(void)
{
 return(QString("%1").arg(linha));
}
//-----------------------------------------------------------
TipoErro Excecao::obterTipoErro(void)
{
 return(tipo_erro);
}
//-----------------------------------------------------------
QString Excecao::obterInfoAdicional(void)
{
 return(info_adicional);
}
//-----------------------------------------------------------
void Excecao::adicionarExecao(Excecao &excecao)
{
 deque<Excecao>::iterator itr, itr_end;

 itr=excecao.excecoes.begin();
 itr_end=excecao.excecoes.end();

 while(itr!=itr_end)
 {
  this->excecoes.push_front(Excecao(itr->msg_erro,itr->tipo_erro,
                                    itr->local,itr->arquivo,itr->linha,NULL,itr->info_adicional));
  itr++;
 }
 excecao.excecoes.clear();
 this->excecoes.push_front(Excecao(excecao.msg_erro,excecao.tipo_erro,
                                   excecao.local,excecao.arquivo,excecao.linha,NULL,excecao.info_adicional));
}
//-----------------------------------------------------------
void Excecao::obterListaExcecoes(deque<Excecao> &lista)
{
 lista.assign(this->excecoes.begin(), this->excecoes.end());
 lista.push_front(Excecao(this->msg_erro,this->tipo_erro,
                          this->local,this->arquivo,this->linha,NULL,this->info_adicional));
}
//***********************************************************
