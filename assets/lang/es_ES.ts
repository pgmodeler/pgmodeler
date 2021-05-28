<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="es_ES">
<context>
    <name>AboutWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="35"/>
        <source>About pgModeler</source>
        <translation>Acerca de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="161"/>
        <source>0.0.0</source>
        <translation type="unfinished">0.0.0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="184"/>
        <source>build:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="282"/>
        <source>PostgreSQL Database Modeler</source>
        <translation>Modelador para Bases de Datos PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="362"/>
        <source>Open source data modeling tool designed for PostgreSQL. No more DDL commands written by hand, let pgModeler do the job for you! This software reunites the concepts of entity-relationship diagrams and the features that PostgreSQL implements as extensions of SQL standards. </source>
        <translation>Herramienta de modelado de datos de código abierto diseñada para PostgreSQL. ¡No más comandos de DDL escritos a mano, deje a pgModeler hacer el trabajo para usted! Este software reúne los conceptos de diagramas entidad-relación y las características que PostgreSQL implementa como extensiones de estándares SQL.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="445"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;a href=&quot;http://pgmodeler.com.br&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#2980b9;&quot;&gt;https://pgmodeler.io&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="486"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Copyright 2006-2018 - Raphael Araújo e Silva &amp;lt;&lt;a href=&quot;mailto:raphael@pgmodeler.com.br&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;raphael@pgmodeler.io&lt;/span&gt;&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="461"/>
        <source>pgModeler is proudly a brazilian software!</source>
        <translation>pgModeler es orgullosamente un programa brasileño!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="309"/>
        <source>Hide this widget</source>
        <translation>Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="312"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="244"/>
        <source>(BUILD_NUM)</source>
        <translation>(NUM_CONSTRUIDO)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="384"/>
        <source>License</source>
        <translation>Licencia</translation>
    </message>
</context>
<context>
    <name>AggregateWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="38"/>
        <source>Final Function:</source>
        <translation>Función final:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="45"/>
        <source>Sort Operator:</source>
        <translation>Operador de Ordenación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="52"/>
        <source>Initial Condition:</source>
        <translation>Condition Initial:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="75"/>
        <source>Funtion Inputs</source>
        <translation>Entradas de Funciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="80"/>
        <source>Function State</source>
        <translation>Estado de Función</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="88"/>
        <source>Transition Func.:</source>
        <translation>Función de transición:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="37"/>
        <source>Input Data Type</source>
        <translation>Tipo de Datos de Entrada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="38"/>
        <source>State Data Type</source>
        <translation>Tipo de Dato de Estado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="65"/>
        <source>An aggregate function that accepts the types &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; and &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; as input types and which type of state is &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt;&lt;/em&gt;, must obey the following rules: &lt;br/&gt;&lt;br/&gt; &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Final Function:&lt;/strong&gt; &lt;em&gt;void final_function(&lt;strong&gt;state_type&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Transition Function:&lt;/strong&gt; &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt; transition_function(&lt;strong&gt;state_type&lt;/strong&gt;, &lt;strong&gt;typeA&lt;/strong&gt;, &lt;strong&gt;typeB&lt;/strong&gt;)&lt;/em&gt;</source>
        <translation>Una función de agregación que acepta los tipos &lt;em&gt;&lt;strong&gt;tipoA&lt;/strong&gt;&lt;/em&gt; y &lt;em&gt;&lt;strong&gt;tipoB&lt;/strong&gt;&lt;/em&gt;                                    como entrada y cuyo tipo de estado sea &lt;em&gt;&lt;strong&gt;tipo_estado&lt;/strong&gt;&lt;/em&gt;, debe obedecer a la seguintes reglas:&lt;br/&gt;&lt;br/&gt;                                     &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Función Final:&lt;/strong&gt; &lt;em&gt;void funcao_final(&lt;strong&gt;tipo_estado&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Función Transición:&lt;/strong&gt; &lt;em&gt;&lt;strong&gt;tipo_estado&lt;/strong&gt; funcao_transicao(&lt;strong&gt;tipo_estado&lt;/strong&gt;, &lt;strong&gt;tipoA&lt;/strong&gt;, &lt;strong&gt;tipoB&lt;/strong&gt;)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>AppearanceConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="59"/>
        <source>Element:</source>
        <translation>Elemento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="67"/>
        <source>Global: Font style</source>
        <translation>Global: Estilo de fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="72"/>
        <source>Global: Constraints descriptor</source>
        <translation>Global: Descritor de restricciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="77"/>
        <source>Global: Object selection</source>
        <translation>Global: Selección de objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="82"/>
        <source>Global: Position hint text</source>
        <translation>Global: Texto informativo de posición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="87"/>
        <source>Global: Position hint box</source>
        <translation>Global: Cuadro de informativo de la Posición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="92"/>
        <source>Global: Objects type</source>
        <translation>Global: Tipo de objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="97"/>
        <source>Global: Lock arc</source>
        <translation>Global: Arco de bloqueo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="102"/>
        <source>Global: Lock body</source>
        <translation>Global: Cuerpo del bloqueo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="107"/>
        <source>Table: Schema name</source>
        <translation>Tabla: nombre de esquema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="112"/>
        <source>Table: Table name</source>
        <translation>Tabla: Nombre de tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="117"/>
        <source>Table: Columns box</source>
        <translation>Tabla: Cuadro de columnas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="122"/>
        <source>Table: Extended attributes box</source>
        <translation>Tabla: Cuadro de atributos extendidos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="127"/>
        <source>Table: Title box</source>
        <translation>Tabla: Cuadro de título</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="132"/>
        <source>Rule: Name</source>
        <translation>Regra: Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="137"/>
        <source>Rule: Descriptor</source>
        <translation>Regra: Descriptor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="142"/>
        <source>Index: Name</source>
        <translation>Índice: Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="147"/>
        <source>Index: Descriptor</source>
        <translation>Índice: Descriptor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="152"/>
        <source>Trigger: Name</source>
        <translation>Trigger: Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="157"/>
        <source>Trigger: Descriptor</source>
        <translation>Trigger: Descriptor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="162"/>
        <source>Constraint: Name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="167"/>
        <source>Constraint: Descriptor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="172"/>
        <source>View: Schema name</source>
        <translation>Vista: Nombre de esquema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="177"/>
        <source>View: View name</source>
        <translation>Vista: Nombre de vista</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="182"/>
        <source>View: References box</source>
        <translation>Vista: Cuadro de referéncias</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="187"/>
        <source>View: Extended attributes box</source>
        <translation>Vista: Cuadro de atributos extendidos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="192"/>
        <source>View: Title box</source>
        <translation>Vista: Cuadro de título</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="197"/>
        <source>View: Table / columns alias</source>
        <translation>Vista: Tabla / columna alias</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="202"/>
        <source>View: Referenced column</source>
        <translation>Vista: Columna referenciada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="207"/>
        <source>View: Referenced table</source>
        <translation>Vista: Tabla referenciada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="212"/>
        <source>View: Reference descriptor</source>
        <translation>Vista: Descritor de Referencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="217"/>
        <source>Textbox: Body</source>
        <translation>Cuadro de Texto: Cuerpo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="222"/>
        <source>Column: Column name</source>
        <translation>Columna: Nombre de columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="227"/>
        <source>Column: Descriptor</source>
        <translation>Columna: Descritor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="232"/>
        <source>Column: Included / Inherited by relationship</source>
        <translation>Columna: Incluída / Herendado por relación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="237"/>
        <source>Column: Protected</source>
        <translation>Columna: Protegida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="242"/>
        <source>Column (pk): Column name</source>
        <translation>Columna (pk): Nombre de columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="247"/>
        <source>Column (pk): Descriptor</source>
        <translation>Columna (fk): Descritor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="252"/>
        <source>Column (fk): Column name</source>
        <translation>Columna (fk): Nombre de columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="257"/>
        <source>Column (fk): Descriptor</source>
        <translation>Columna (fk): Descritor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="262"/>
        <source>Column (uq): Column name</source>
        <translation>Columna (uq): Nombre de columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="267"/>
        <source>Column (uq): Descriptor</source>
        <translation>Columna (uq): Descritor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="272"/>
        <source>Column (nn): Column name</source>
        <translation>Columna (nn): Nombre de columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="277"/>
        <source>Column (nn): Descriptor</source>
        <translation>Columna (nn): Descritor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="282"/>
        <source>Relationship: Descriptor</source>
        <translation>Relación: Descritor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="287"/>
        <source>Relationship: Label text</source>
        <translation>Relación: Texto etiqueta</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="292"/>
        <source>Relationship: Label box</source>
        <translation>Relación: Cuadro etiqueta</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="297"/>
        <source>Relationship: Attribute text</source>
        <translation>Relación: Texto del atributo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="302"/>
        <source>Relationship: Attribute descriptor</source>
        <translation>Relación: Descritor del atributo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="307"/>
        <source>Tag: Name</source>
        <translation>Tag: Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="312"/>
        <source>Tag: Body</source>
        <translation>Tag: Cuerpo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="317"/>
        <source>Placeholder: Body</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="331"/>
        <source>Font:</source>
        <translation>Fuente:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="344"/>
        <source>Colors:</source>
        <translation>Colores:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="385"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="400"/>
        <source>Underline</source>
        <translation>Subrayar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="419"/>
        <source>Italic</source>
        <translation>Itálico</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="438"/>
        <source>Bold</source>
        <translation>Negrito</translation>
    </message>
</context>
<context>
    <name>Application</name>
    <message>
        <location filename="../main/src/application.cpp" line="141"/>
        <source>Unknown exception caught!</source>
        <translation>Excepción desconocida capturada!</translation>
    </message>
    <message>
        <location filename="../main/src/application.cpp" line="161"/>
        <source>Failed to create initial configuration in `%1&apos;! Check if the current user has write permission over that path and at least read permission over `%2&apos;.</source>
        <translation>No se pudo crear la configuración inicial en `%1&apos;! Compruebe si el usuario actual tiene permiso de escritura sobre ese camino y al menos permiso de lectura sobre `%2&apos;.</translation>
    </message>
</context>
<context>
    <name>BaseConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/baseconfigwidget.cpp" line="140"/>
        <source>A backup of the previous settings was saved into &lt;strong&gt;%1&lt;/strong&gt;!</source>
        <translation>Se guardó una copia de seguridad de la configuración anterior en &lt;strong&gt;%1&lt;/strong&gt;!</translation>
    </message>
</context>
<context>
    <name>BaseForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="23"/>
        <source>Dialog</source>
        <translation>Dialogo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="141"/>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="31"/>
        <source>&amp;Apply</source>
        <translation>&amp;Aplicar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="188"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="36"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="130"/>
        <source>%1 properties</source>
        <translation>%1 propiedades</translation>
    </message>
</context>
<context>
    <name>BaseObject</name>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="41"/>
        <source>Column</source>
        <translation>Columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="41"/>
        <source>Constraint</source>
        <translation>Restricción</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="41"/>
        <source>Function</source>
        <translation>Función</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="42"/>
        <source>Trigger</source>
        <translation>Trigger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="42"/>
        <source>Index</source>
        <translation>Índice</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="42"/>
        <source>Rule</source>
        <translation>Regla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="43"/>
        <source>Table</source>
        <translation>Tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="43"/>
        <source>View</source>
        <translation>Vista</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="43"/>
        <source>Domain</source>
        <translation>Domínio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="44"/>
        <source>Schema</source>
        <translation>Esquema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="44"/>
        <source>Aggregate</source>
        <translation>Agregado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="44"/>
        <source>Operator</source>
        <translation>Operador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="45"/>
        <source>Sequence</source>
        <translation>Secuencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="45"/>
        <source>Role</source>
        <translation>Rol</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="45"/>
        <source>Conversion</source>
        <translation>Conversión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Cast</source>
        <translation>Cast</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Language</source>
        <translation>Lenguaje</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Tablespace</source>
        <translation>Tablespace</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="47"/>
        <source>Operator Family</source>
        <translation>Família de Operadores</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="47"/>
        <source>Operator Class</source>
        <translation>Operador de Clases</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="48"/>
        <source>Database</source>
        <translation>Base de Dados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="48"/>
        <source>Collation</source>
        <translation>Colación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="48"/>
        <source>Extension</source>
        <translation>Extensión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="49"/>
        <source>Event Trigger</source>
        <translation>Evento Trigger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="49"/>
        <source>Relationship</source>
        <translation>Relación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="49"/>
        <source>Policy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Textbox</source>
        <translation>Cuadro de Texto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Permission</source>
        <translation>Permisos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Parameter</source>
        <translation>Parámetro</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Type Attribute</source>
        <translation>Tipo atributo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="51"/>
        <source>Tag</source>
        <translation>Tag</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="51"/>
        <source>Basic Relationship</source>
        <translation>Relación básica</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="51"/>
        <source>Generic SQL</source>
        <translation type="unfinished"></translation>
    </message>
    <message numerus="yes">
        <location filename="../libpgmodeler/src/baseobject.cpp" line="97"/>
        <source>new_object</source>
        <translation>
            <numerusform>nuevo_objecto</numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BaseObjectView</name>
    <message>
        <location filename="../libobjrenderer/src/baseobjectview.cpp" line="486"/>
        <source>SQL off</source>
        <translation>SQL off</translation>
    </message>
</context>
<context>
    <name>BaseObjectWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="47"/>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="109"/>
        <source>Comment:</source>
        <translation>Comentario:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="128"/>
        <source>Tablespace:</source>
        <translation>Tablespace:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="147"/>
        <source>Schema:</source>
        <translation>Esquema:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="181"/>
        <source>Edit object&apos;s permissions</source>
        <translation>Editar permisos del objeto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="184"/>
        <source>Edit permissions</source>
        <translation>Editar permisos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="277"/>
        <source>This object is protected thus no change in form will be applied to it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="305"/>
        <source>Disables the generated SQL code using comment tokens (--).
This will disable the code of all child and referrer objects.</source>
        <translation>Desactiva el código SQL generado utilizando fichas comentario (-).
Esto desactivará el código de todos los objetos secundarios y referenciales.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="309"/>
        <source>Disable SQL code</source>
        <translation>Desactivar Código SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="328"/>
        <source>Collation:</source>
        <translation>Colación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="362"/>
        <source>Append or prepend a set of SQL commands to the object&apos;s definition.</source>
        <translation>Anexar o anteponer un conjunto de comandos SQL para la definicióno bjeto&apos;s.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="365"/>
        <source>Custom SQL</source>
        <translation>SQL personalizado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="394"/>
        <source>Owner:</source>
        <translation>Propietario:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="440"/>
        <source>ID:</source>
        <translation>ID:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="471"/>
        <source>icone</source>
        <translation>icone</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="170"/>
        <source>Required field. Leaving this empty will raise errors!</source>
        <translation>Campo requerido. Dejando este vacío elevará errores!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="570"/>
        <source>Value(s)</source>
        <translation>Valor(es)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="583"/>
        <source>Version</source>
        <translation>Versión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="630"/>
        <source>The &lt;em style=&apos;color: %1&apos;&gt;&lt;strong&gt;highlighted&lt;/strong&gt;&lt;/em&gt; fields in the form or one of their values are available only on specific PostgreSQL versions. 							Generating SQL code for versions other than those specified in the fields&apos; tooltips may create incompatible code.</source>
        <translation>Los campos &lt;em style =&apos;color: %1&apos;&gt;&lt;strong&gt;resaltados&lt;/strong&gt;&lt;/em&gt; en el formulario o uno de sus valores sólo están disponibles en versiones específicas de PostgreSQL.
					 Generar código SQL para versiones distintas a las especificadas en las sugerencias de los campos puede crear código incompatible.</translation>
    </message>
</context>
<context>
    <name>BaseRelationship</name>
    <message>
        <location filename="../libpgmodeler/src/baserelationship.cpp" line="62"/>
        <source>rel_%1_%2</source>
        <translation>rel_%1_%2</translation>
    </message>
</context>
<context>
    <name>BaseTableView</name>
    <message>
        <location filename="../libobjrenderer/src/basetableview.cpp" line="232"/>
        <source>Toggles the extended attributes display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libobjrenderer/src/basetableview.cpp" line="445"/>
        <source>Connected rels: %1</source>
        <translation type="unfinished">Rels conectados: %1</translation>
    </message>
</context>
<context>
    <name>BugReportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="35"/>
        <source>Bug Report</source>
        <translation>Informe Bug</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="107"/>
        <source>Bug report</source>
        <translation>Informe bug</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="203"/>
        <source>Create</source>
        <translation>Crear</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="146"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancel</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="250"/>
        <source>Use the form below to generate a complete bug report. Please, try to be as clear as possible when describing the actions that can reproduce the bug. Additionally, it&apos;s important to attach a sample database model so that the bug can be quickly discovered and fixed!</source>
        <translation>Utilice el formulario de abajo para generar un informe completo error. Por favor, trate de ser lo más claro posible al describir las acciones que pueden reproducir el error. Adicionalmente, es importante adjuntar un modelo de base de datos de muestra para que el error puede ser descubierto y se fija rápidamente!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="285"/>
        <source>Report</source>
        <translation>Reporte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="303"/>
        <source>Issue details</source>
        <translation>Detalles del problema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="348"/>
        <source>Output:</source>
        <translation>Salida:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="358"/>
        <source>Select the report&apos;s output folder</source>
        <translation>Seleccione la carpeta de destino del informes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="361"/>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="524"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="452"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;If you prefer it&apos;s possible to report this issue anytime on pgModeler&apos;s project repository at &lt;a href=&quot;http://github.com/pgmodeler/pgmodeler/issues&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;GitHub&lt;/span&gt;&lt;/a&gt;. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Si lo prefiere, se pueden informar de este tema en cualquier momento en repositorio del proyecto de pgModeler en &lt;a href=&quot;http://github.com/pgmodeler/pgmodeler/issues&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;GitHub&lt;/span&gt;&lt;/a&gt;. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="472"/>
        <source>Database Model</source>
        <translation>Base de datos del modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="511"/>
        <source>Attach the below database model file to be debugged.</source>
        <translation>Adjunte el archivo de modelo de base de datos de abajo para depurar.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="521"/>
        <source>Attach a different database model</source>
        <translation>Adjunte un modelo de base de datos diferente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="120"/>
        <source>Bug report successfuly generated! Please, send the file &lt;strong&gt;%1&lt;/strong&gt; to &lt;em&gt;%2&lt;/em&gt; in order be analyzed. Thank you for the collaboration!</source>
        <translation>Informe de error generado exitosamente! Por favor, enviar el archivo &lt;strong&gt;%1&lt;/strong&gt; para &lt;em&gt;%2&lt;/em&gt; con el fin de analizar. Gracias por la colaboración!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="132"/>
        <source>Load model</source>
        <translation>cargar modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="133"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modelo de base de datos (*.dbm);;Todos los Archivos (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="164"/>
        <source>Select report output folder</source>
        <translation>Seleccione carpeta de destino del informe</translation>
    </message>
</context>
<context>
    <name>BulkDataEditWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/bulkdataeditwidget.ui" line="20"/>
        <source>Bulk data edit</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CastWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="56"/>
        <source>I&amp;mplicit</source>
        <translation type="unfinished">I&amp;mplícito</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="84"/>
        <source>Assignment</source>
        <translation>Asignación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="97"/>
        <source>Input / Output</source>
        <translation>Entrada / Salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="122"/>
        <source>Conversion Func.:</source>
        <translation>Func. de conversión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="147"/>
        <source>Cast Type:</source>
        <translation>Tipo de Conversión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="172"/>
        <source>E&amp;xplicit</source>
        <translation>E&amp;xplícito</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="31"/>
        <source>Source data type</source>
        <translation>Tipo de datos origen</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="32"/>
        <source>Target data type</source>
        <translation>Tipo de datos destino</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="46"/>
        <source>The function to be assigned to a cast from &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; to &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; must have the following signature: &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; function(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</source>
        <translation>La función que se asignará a una conversión de &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; a &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; debe tener la siguiente firma: &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; function(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</translation>
    </message>
</context>
<context>
    <name>CodeCompletionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="41"/>
        <source>Make &amp;persistent</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="42"/>
        <source>Makes the widget closable only by ESC key or mouse click on other controls.</source>
        <translation>Hace que el widget se puede cerrar solamente por tecla ESC o clic del ratón sobre otros controles.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="453"/>
        <source>SQL Keyword</source>
        <translation>Palabra clave SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="484"/>
        <source>(no items found.)</source>
        <translatorcomment>(No se encontraron items.)</translatorcomment>
        <translation>(No se encontró ninguna.)</translation>
    </message>
</context>
<context>
    <name>CollationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="50"/>
        <source>Locale:</source>
        <translation>Local:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="85"/>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="130"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="175"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="30"/>
        <source>The fields &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; are mutually exclusive, so you have to set only one of them in order to properly handle a collation.</source>
        <translation>Los campos &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; son mutuamente excluyentes, así que tienes que configurar sólo uno de ellos con el fin de manejar adecuadamente una colación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="39"/>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="51"/>
        <source>Not defined</source>
        <translation>No definido</translation>
    </message>
</context>
<context>
    <name>ColorPickerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/colorpickerwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/colorpickerwidget.ui" line="50"/>
        <source>Generate random color(s)</source>
        <translation>Generar color(es) al azar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/colorpickerwidget.ui" line="66"/>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/colorpickerwidget.cpp" line="148"/>
        <source>Select color</source>
        <translation>Seleccionar colores</translation>
    </message>
</context>
<context>
    <name>ColumnWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="32"/>
        <source>Default Value:</source>
        <translation>Valor por Defecto:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="56"/>
        <source>Edit the underlying sequence&apos;s attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="59"/>
        <source>Edit sequence</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="70"/>
        <source>Identity:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="77"/>
        <source>E&amp;xpression:</source>
        <translation type="unfinished">E&amp;xpresión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="148"/>
        <source>&amp;NOT NULL</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="87"/>
        <source>Se&amp;quence:</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ConfigurationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="29"/>
        <source>pgModeler Configuration</source>
        <translation>Configuración del pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="187"/>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="202"/>
        <source>Relationships</source>
        <translation>Relaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="214"/>
        <source>Appearance</source>
        <translation>Apariencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="229"/>
        <source>Connections</source>
        <translation>Conexiones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="244"/>
        <source>Snippets</source>
        <translation>Snippets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="256"/>
        <source>Plug-ins</source>
        <translation>Plug-ins</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="347"/>
        <source>Defaults</source>
        <translation>Valores por Defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="388"/>
        <source>&amp;Apply</source>
        <translation>&amp;Aplicar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="426"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="126"/>
        <source>In some cases restore the default settings related to it may solve the problem. Would like to do that?</source>
        <translation>En algunos casos, restaurar la configuración predeterminada relacionada con él puede resolver el problema. ¿Le gustaría hacer eso?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="127"/>
        <source>Restore</source>
        <translation>Restaurar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="139"/>
        <source>Any modification made until now in the current section will be lost! Do you really want to restore default settings?</source>
        <translation>Cualquier modificación realizada hasta ahora en la sección actual se perderá! ¿Realmente desea restaurar la configuración predeterminada?</translation>
    </message>
</context>
<context>
    <name>ConnectionsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="388"/>
        <source>Password:</source>
        <translation>Contraseña:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="492"/>
        <source>Connection Alias:</source>
        <translation>Alias de Conexión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="515"/>
        <source>Connection DB:</source>
        <translation>BD Conexión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="479"/>
        <source>Host/Port:</source>
        <translation>Host/Puerto:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="466"/>
        <source>User:</source>
        <translation type="unfinished">Usuario:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="244"/>
        <source>Timeout:</source>
        <translation>Timeout:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="880"/>
        <source>SSL Mode:</source>
        <translation>Modo SSL:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="794"/>
        <source>Disable</source>
        <translation>Desactivado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="607"/>
        <source>Diff</source>
        <translation type="unfinished">Diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="614"/>
        <source>Export</source>
        <translation>Exportar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="621"/>
        <source>Import</source>
        <translation>Importar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="628"/>
        <source>Validation</source>
        <translation>Validación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="754"/>
        <source>Security</source>
        <translation>Seguridad</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="799"/>
        <source>Allow</source>
        <translation>Permitir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="804"/>
        <source>Require</source>
        <translation>Requerido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="809"/>
        <source>AC verification</source>
        <translation>Verificación de AC</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="814"/>
        <source>Full verification</source>
        <translation>Verificación Completa</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="925"/>
        <source>Kerberos Server:</source>
        <translation>Servidor Kerberos:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="986"/>
        <source>Client Certificate:</source>
        <translation>Certificado Cliente:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="957"/>
        <source>Revoked Certs.:</source>
        <translation>Cert. Revocados:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="896"/>
        <source>Client Key:</source>
        <translation>Clave Cliente:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="941"/>
        <source>Root Certificate:</source>
        <translation>Certificado Root:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="840"/>
        <source>~/.postgresql/root.crt</source>
        <translation>~/.postgresql/root.crt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="65"/>
        <source>Connections:</source>
        <translation>Conexiones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="583"/>
        <source>second(s)</source>
        <translation>segundo(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="865"/>
        <source>Force GSSAPI</source>
        <translation>Forzar a GSSAPI</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="775"/>
        <source>~/.postgresql/postgresql.crt</source>
        <translation>~/.postgresql/postgresql.crt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="325"/>
        <source>Add</source>
        <translation>Añadir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="363"/>
        <source>Update</source>
        <translation>Actualizar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="287"/>
        <source>Test</source>
        <translation>Testear</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="906"/>
        <source>~/.postgresql/root.crl</source>
        <translation>~/.postgresql/root.crl</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="967"/>
        <source>~/.postgresql/postgresql.key</source>
        <translation>~/.postgresql/postgresql.key</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="102"/>
        <source>Create new connection</source>
        <translation>Crear nueva conexión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="14"/>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="602"/>
        <source>Edit database connections</source>
        <translation>Editar conexiones de base de datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="125"/>
        <source>Cancel edition</source>
        <translation>Cancelar edición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="148"/>
        <source>Duplicate the selected connection</source>
        <translation>Duplicar la conexión seleccionada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="171"/>
        <source>Edit selected connection</source>
        <translation>Editar conexión seleccionada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="194"/>
        <source>Delete selected connection</source>
        <translation>Eliminar conexión seleccionada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="220"/>
        <source>General</source>
        <translation type="unfinished">General</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="528"/>
        <source>Other params:</source>
        <translation>Otros parámetros:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="537"/>
        <source>Specify additional connection parameters in the form [param]=[value]. These parameters are described in the &lt;strong&gt;libpq&lt;/strong&gt; chapter at PostgreSQL docs.</source>
        <translation>Especifique los parámetros de conexión adicionales en la forma [param]=[valor]. Estos parámetros se describen en el capítulo &lt;strong&gt; libpq &lt;/strong&gt; en la documentación de PostgreSQL.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="434"/>
        <source>Default for:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="690"/>
        <source>Automatically browses the named database when using this connection to manage databases on &lt;strong&gt;Manage&lt;/strong&gt; view.</source>
        <translation>Busca automáticamente la base de datos llamada cuando se utiliza esta conexión para administrar bases de datos en la vista &lt;strong&gt;Manage&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="693"/>
        <source>Auto browse</source>
        <translation>Auto explorar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="38"/>
        <source>Indicates in which operations (diff, export, import or validation) the connection is used if none is explicitly specified by the user.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="437"/>
        <source>Success</source>
        <translation>Éxito</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="438"/>
        <source>Connection successfully established!

Server details:

PID: `%1&apos;
Protocol: `%2&apos;
Version: `%3&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="485"/>
        <source>There is a connection being created or edited! Do you want to save it?</source>
        <translation>¡Hay una conexión que se está creando o editando! ¿Desea guardarla?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="572"/>
        <source>Found %1 connection(s)</source>
        <translation>Se encontró %1 conexión(es)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="574"/>
        <source>No connections found</source>
        <translation>No se encontraron conexiones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="586"/>
        <source>Edit connections</source>
        <translation>Editar conexiones</translation>
    </message>
</context>
<context>
    <name>ConstraintWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="56"/>
        <source>Match:</source>
        <translation>Partida:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="94"/>
        <source>Constraint Type:</source>
        <translation>Tipo de Restricción:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="107"/>
        <source>This attribute cannot be changed once the object is created.</source>
        <translation>Este atributo no se puede cambiar una vez que se crea el objeto.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="126"/>
        <source>Expression:</source>
        <translation>Expresión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="167"/>
        <source>Deferrable:</source>
        <translation>Diferible:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="208"/>
        <source>Deferral:</source>
        <translation>Diferimiento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="262"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="287"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="310"/>
        <source>No inherit:</source>
        <translation>No se hereda:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="330"/>
        <source>Exclude Elements</source>
        <translation>Excluir Elementos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="353"/>
        <source>Columns</source>
        <translation>Columnas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="380"/>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="494"/>
        <source>Column:</source>
        <translation>Columna:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="426"/>
        <source>Referenced Columns</source>
        <translation>Columnas Referenciadas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="507"/>
        <source>Table:</source>
        <translation>Tabla:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="524"/>
        <source>Indexing:</source>
        <translation>Indexación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="544"/>
        <source>Fill Factor:</source>
        <translation>Factor Rellen.:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="54"/>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="61"/>
        <source>Column</source>
        <translation>Column</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="56"/>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="63"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="88"/>
        <source>Columns which were included by relationship can not be added / removed manually from the primary key. If done such changes they can raise errors. To create primary key using columns included by relationship use the following options: identifier field, attributes &amp; constraints tab or primary key tab on the relationship form.</source>
        <translation>Las columnas que se incluyeron por la relación no se pueden añadir / retirado manualmente de la clave principal. Si se hace este tipo de cambios que pueden elevar los errores. Para crear la clave primaria utilizando columnas incluidas por relación utilizar las siguientes opciones: campo de identificador, atributos &amp; limitaciones ficha o pestaña clave principal en el formulario de la relación.</translation>
    </message>
</context>
<context>
    <name>ConversionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="56"/>
        <source>Source Encoding:</source>
        <translation>Codificación Fuente:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="100"/>
        <source>Target Encoding:</source>
        <translation>Codificación Destino:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="147"/>
        <source>Conversion Func.:</source>
        <translation>Func. Conversión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="185"/>
        <source>Default Conversion:</source>
        <translation>Conversión por Defecto:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/conversionwidget.cpp" line="40"/>
        <source>The function to be assigned to an encoding conversion must have the following signature: &lt;em&gt;void function(integer, integer, cstring, internal, integer)&lt;/em&gt;.</source>
        <translation>La función que se asigna a una conversión de codificación debe tener la siguiente firma:                                    &lt;em&gt;void función(integer, integer, cstring, internal, integer)&lt;/em&gt;.</translation>
    </message>
</context>
<context>
    <name>CrashHandlerForm</name>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="32"/>
        <source>Crash Handler</source>
        <translation>Accidente del Handler</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="44"/>
        <source>Stack trace</source>
        <translation>Pila de traza</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="76"/>
        <source>Input:</source>
        <translation>Entrada:</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="89"/>
        <source>Load report file for analysis</source>
        <translation>Cargando archivos de reporte para el análisis</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="97"/>
        <source>Save the attached model file on the filesystem</source>
        <translation>Guarde el archivo de modelo que se adjunta en el sistema de archivos</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="174"/>
        <source>pgModeler bug report (*.bug);;All files (*.*)</source>
        <translation>Reporte de Bog del pgModeler (*.bug);;Todos los archivos (*.*)</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="175"/>
        <source>Load report</source>
        <translation>Cargando reporte</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="199"/>
        <source>Save model</source>
        <translation>Salvar modelo</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="200"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modelo de Bases de Datos (*.dbm);;Todos los archivo (*.*)</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="240"/>
        <source>Crash handler</source>
        <translation>Accidente del Handler</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="241"/>
        <source>Bug report analysis mode activated.</source>
        <translation>Modo de análisis para informar de fallos activada.</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="245"/>
        <source>Oops! pgModeler just crashed!</source>
        <translation>¡Huy! pgModeler simplemente cayó!</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="246"/>
        <source>We apologize for what happened! It is clear that a nasty bug caused that. Please fill out the form below describing your actions before pgModeler quit unexpectedly. This will help on bug extermination and improve the software.</source>
        <translation>¡Pedimos disculpas por lo ocurrido! Es evidente que un error desagradable ha causado eso. Rellene el siguiente formulario para describir sus acciones antes de que pgModeler se cierre inesperadamente. Esto ayudará en la exterminación de errores y mejorará el software.</translation>
    </message>
</context>
<context>
    <name>CsvLoadWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="44"/>
        <source>Load CSV</source>
        <translation>Cargar CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="62"/>
        <source>CSV File:</source>
        <translation>Archivo CSV:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="81"/>
        <source>Select output file</source>
        <translation>Seleccionar archivo de salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="84"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="101"/>
        <source>Separator: </source>
        <translation>Separador: </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="129"/>
        <source>Use the first row as column names in the CSV file. By unchecking this option the first row is used as data.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="132"/>
        <source>Columns in the first row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="200"/>
        <source>Load</source>
        <translation type="unfinished">Cargar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="241"/>
        <source>Semicolon (;)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="246"/>
        <source>Comma (,)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="251"/>
        <source>Space</source>
        <translation type="unfinished">Espacio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="256"/>
        <source>Tabulation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="261"/>
        <source>Other</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="287"/>
        <source>;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="307"/>
        <source>Text delimiter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="338"/>
        <source>&quot;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/csvloadwidget.cpp" line="68"/>
        <source>Load CSV file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/csvloadwidget.cpp" line="70"/>
        <source>Comma-separted values (*.csv);;All files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>CustomSQLWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="26"/>
        <source>Add custom SQL code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="44"/>
        <source>SQL code</source>
        <translation>Código SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="88"/>
        <source>Puts an SELECT command template at current cursor position.</source>
        <translation>Pone un comando SELECT en una plantilla en la posición actual de cursor.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="91"/>
        <source>&amp;SELECT</source>
        <translation>&amp;SELECT</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="125"/>
        <source>Puts an INSERT command template at current cursor position.</source>
        <translation>Pone un comando INSERT en una plantilla en la posición actual de cursor.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="128"/>
        <source>&amp;INSERT</source>
        <translation>&amp;INSERT</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="162"/>
        <source>Puts an UPDATE command template at current cursor position.</source>
        <translation>Pone un comando UPDATE en una plantilla en la posición actual de cursor.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="165"/>
        <source>&amp;UPDATE</source>
        <translation>&amp;UPDATE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="199"/>
        <source>Puts an DELETE command template at current cursor position.</source>
        <translation>Pone un comando DELETE en una plantilla en la posición actual de cursor.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="202"/>
        <source>&amp;DELETE</source>
        <translation>&amp;DELETE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="233"/>
        <source>&amp;Clear</source>
        <translation>&amp;Borrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="272"/>
        <source>Append SQL</source>
        <translation>Adjuntar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="290"/>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="323"/>
        <source>Append the SQL code at the very end of model definition.
Unchecking this will cause the SQL to be appended at the end of CREATE DATABASE command.</source>
        <translation>Anexe el código SQL en el final de la determinación del modelo.
Desmarcando esto hará que el SQL que se adjunta al final del mandato CREATE DATABASE.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="294"/>
        <source>Append at end of model definition.</source>
        <translation>Adjuntar al final de la definición del modelo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="305"/>
        <source>Prepend SQL</source>
        <translation>Prefijar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="327"/>
        <source>Prepend at beginning of model definition.</source>
        <translation>Prefijar al comienzo de la determinación del modelo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="411"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Use custom commands with extreme caution because you can change the semantics of the entire model when running SQL validation or export processes. Additionally, depending on the amount of commands, those processes can have their performance sensibly degradated.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Utilice comandos personalizados con extrema precaución ya que puede cambiar la semántica de todo el modelo cuando se ejecutan los procesos de validación de SQL o de exportación. Además, dependiendo de la cantidad de comandos, esos procesos pueden tener su rendimiento sensiblemente degrada.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="48"/>
        <source>Generic INSERT</source>
        <translation>INSERT genérico</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="50"/>
        <source>Include serial columns</source>
        <translation>Incluir columnas serial</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="52"/>
        <source>Exclude serial columns</source>
        <translation>Excluir columnas serial</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="54"/>
        <source>Generic SELECT</source>
        <translation>SELECT genérico</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="56"/>
        <source>Table SELECT</source>
        <translation>SELECT tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="58"/>
        <source>Generic UPDATE</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="60"/>
        <source>Table UPDATE</source>
        <translation>UPDATE genérico</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="62"/>
        <source>Generic DELETE</source>
        <translation>DELETE genérico</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="64"/>
        <source>Table DELETE</source>
        <translation>DELETE tabla</translation>
    </message>
</context>
<context>
    <name>DataManipulationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="20"/>
        <source>Data Manipulation</source>
        <translation>Manipulación de Datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="114"/>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="334"/>
        <source>Refresh listing</source>
        <translation>Refrescar listado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="347"/>
        <source>F5</source>
        <translation>F5</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="371"/>
        <source>Save changes</source>
        <translation>Salvar cambios</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="384"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="485"/>
        <source>Export results to CSV file</source>
        <translation>Exportar resultados a archivo CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="498"/>
        <source>Ctrl+X</source>
        <translation>Ctrl+X</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="575"/>
        <source>Undo modifications</source>
        <translation>Deshacer modificaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="588"/>
        <source>Ctrl+Z</source>
        <translation>Ctrl+Z</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="625"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1276"/>
        <source>Ins</source>
        <translation>Ins</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="649"/>
        <source>Mark the selected rows to be deleted</source>
        <translation>Marque las filas seleccionadas para ser borradas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="662"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1314"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1352"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1390"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1428"/>
        <source>Del</source>
        <translation>Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="686"/>
        <source>Change the values of all selected cells at once</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="699"/>
        <source>Ctrl+E</source>
        <translation type="unfinished">Ctrl+E</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="723"/>
        <source>Duplicate the selected rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="736"/>
        <source>Ctrl+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="771"/>
        <source>Add new rows from a CSV file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="811"/>
        <source>Filter the result set</source>
        <translation>Filtrar el conjunto de resultados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="842"/>
        <source>Table:</source>
        <translation>Tabla:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="855"/>
        <source>Schema:</source>
        <translation>Esquema:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="933"/>
        <source>in </source>
        <translation>in </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="989"/>
        <source>Hide views</source>
        <translation>Ocultar vista</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1167"/>
        <source>Filter expression</source>
        <translation>Filtrar expresión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1202"/>
        <source>Order &amp;&amp; Limit</source>
        <translation>Order &amp;&amp; Límite</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1491"/>
        <source>results (Use &lt;strong&gt;0&lt;/strong&gt; for no limit)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1478"/>
        <source>Column:</source>
        <translation>Columna:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="258"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Empty values are assumed as &lt;span style=&quot; font-weight:600;&quot;&gt;DEFAULT&lt;/span&gt;. To use special values like &lt;span style=&quot; font-weight:600;&quot;&gt;NULL&lt;/span&gt;, a function call like &lt;span style=&quot; font-weight:600;&quot;&gt;now()&lt;/span&gt; or a specific data escaping, enclose values in two slashes, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;/value/&lt;/span&gt;. To use a slash as part of the value prepend the backslash character, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;\/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="408"/>
        <source>Copy items on the grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="445"/>
        <source>Paste items on the grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="458"/>
        <source>Ctrl+V</source>
        <translation type="unfinished">Ctrl+V</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="522"/>
        <source>Browse referenced tables</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1526"/>
        <source>ASC</source>
        <translation>ASC</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="612"/>
        <source>Add empty rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1542"/>
        <source>DESC</source>
        <translation>DESC</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1498"/>
        <source>Limit in:</source>
        <translation>Límite en:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1260"/>
        <source>Add Item</source>
        <translation>Añadir Item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1298"/>
        <source>Remove Item</source>
        <translation>Eliminar Item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1336"/>
        <source>Clear the order by columns list</source>
        <translation>Desactive el orden de lista columnas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1374"/>
        <source>Move selected item up</source>
        <translation>Mover arriba el item seleccionado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1412"/>
        <source>Move selected item down</source>
        <translation>Mover abajo el item seleccionado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="54"/>
        <source>Copy as CSV</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="62"/>
        <source>Copy as text</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="148"/>
        <source>Copy items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="151"/>
        <source>Pase items</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="159"/>
        <source>Browse tables</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="164"/>
        <source>Duplicate row(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="167"/>
        <source>Delete row(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="170"/>
        <source>Edit cell(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="304"/>
        <source>&lt;strong&gt;WARNING: &lt;/strong&gt; There are some changed rows waiting the commit! Do you really want to discard them and retrieve the data now?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="352"/>
        <source>Rows returned: &lt;strong&gt;%1&lt;/strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="353"/>
        <source>&lt;em&gt;(Limit: &lt;strong&gt;%1&lt;/strong&gt;)&lt;/em&gt;</source>
        <translation>&lt;em&gt;(Límite: &lt;strong&gt;%1&lt;/strong&gt;)&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="353"/>
        <source>none</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="382"/>
        <source>Column</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="646"/>
        <source>No objects found</source>
        <translation>Objects no encontrado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="648"/>
        <source>Found %1 object(s)</source>
        <translation>Encontrado %1 objecto(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="678"/>
        <source>Views can&apos;t have their data handled through this grid, this way, all operations are disabled.</source>
        <translation>Las vistas no pueden tener sus datos manejados a través de esta red, de esta manera, todas las operaciones están desactivadas.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="689"/>
        <source>The selected table doesn&apos;t owns a primary key! Updates and deletes will be performed by considering all columns as primary key. &lt;strong&gt;WARNING:&lt;/strong&gt; those operations can affect more than one row.</source>
        <translation>La tabla seleccionada no posee una clave primaria! Las actualizaciones y eliminaciones se realizarán teniendo en cuenta todas las columnas como clave primaria. &lt;strong&gt;WARNING:&lt;/strong&gt; estas operaciones pueden afectar a más de una fila.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="761"/>
        <source>Referenced tables</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="764"/>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="813"/>
        <source>(none)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="810"/>
        <source>Referrer tables</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="861"/>
        <source>This row is marked to be %1</source>
        <translation>Esta fila se marca para ser %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="866"/>
        <source>deleted</source>
        <translation>eliminado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="868"/>
        <source>updated</source>
        <translation>actualizado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="870"/>
        <source>inserted</source>
        <translation>insertado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="995"/>
        <source>[binary data]</source>
        <translation>[datos binarios]</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1211"/>
        <source>&lt;strong&gt;WARNING:&lt;/strong&gt; Once commited its not possible to undo the changes! Proceed with saving?</source>
        <translation>&lt;strong&gt;WARNING:&lt;/strong&gt; Una vez que su cometido no es posible deshacer los cambios! Procedo con salvar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1242"/>
        <source>delete</source>
        <translation>eliminar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1243"/>
        <source>update</source>
        <translation>actualizar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1244"/>
        <source>insert</source>
        <translation>insertar</translation>
    </message>
</context>
<context>
    <name>DatabaseExplorerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="51"/>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="128"/>
        <source>Data &amp;Grid</source>
        <translation>Datos &amp;Cuadrícula</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="64"/>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="141"/>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="157"/>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="183"/>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="206"/>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="382"/>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="414"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="125"/>
        <source>Open the grid to visualize or edit data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="154"/>
        <source>Open a new SQL execution pane</source>
        <translation>Abrir un nuevo panel de ejecución de SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="170"/>
        <source>Ctrl+F6</source>
        <translation type="unfinished">Ctrl+F6</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="180"/>
        <source>Update the objects tree</source>
        <translation>Actualizar el árbol de objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="48"/>
        <source>Toggle the display of filter widget as well the system/extension objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="85"/>
        <source>Sort items alphabetically. When unchecked, items are sorted by OID.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="88"/>
        <source>Sort alphabetically</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="203"/>
        <source>Drop this database</source>
        <translation>Eliminar esta base de datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="379"/>
        <source>Expands all items</source>
        <translation>Expande todos los item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="411"/>
        <source>Collapses all items</source>
        <translation>Contrae todos los item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="460"/>
        <source>Filters the currently loaded items in the tree by using a pattern and matching their names. If &lt;strong&gt;By OID&lt;/strong&gt; is checked the pattern is interpreted as an integer value that represents the object id (OID). &lt;br&gt;&lt;br/&gt;&lt;strong&gt;HINT:&lt;/strong&gt; if you need to search the entire database use the full refresh (&lt;strong&gt;Ctrl+F5&lt;/strong&gt;) prior the filtering.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="463"/>
        <source>Filter:</source>
        <translation>Filtro:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="480"/>
        <source>By OID</source>
        <translation>Por OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="603"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="38"/>
        <source>Attribute</source>
        <translation>Atributo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="608"/>
        <source>Value</source>
        <translation>Valor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="616"/>
        <source>Show raw attributes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="30"/>
        <source>(not found, OID: %1)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="32"/>
        <source>-- Source code not generated! Hit F7 or middle-click the item to load it. --</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="35"/>
        <source>Admin. roles</source>
        <translation>Admin. roles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="35"/>
        <source>Alignment</source>
        <translation>Alineación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="35"/>
        <source>Analyze func.</source>
        <translation>Analizar func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="36"/>
        <source>Arg. count</source>
        <translation>Arg. Contar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="36"/>
        <source>Arg. default count</source>
        <translation>Arg. contar por defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="36"/>
        <source>Arg. defaults</source>
        <translation>Arg. por defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="37"/>
        <source>Arg. modes</source>
        <translation>Arg. modos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="37"/>
        <source>Arg. names</source>
        <translation>Arg. nombres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="37"/>
        <source>Arg. types</source>
        <translation>Arg. tipos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="38"/>
        <source>Behavior type</source>
        <translation>Tipo de comportamiento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="38"/>
        <source>By value</source>
        <translation>Por valor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="39"/>
        <source>Cast type</source>
        <translation>Tipo conversión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="39"/>
        <source>Category</source>
        <translation>Categoria</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="39"/>
        <source>Collatable</source>
        <translation>Intercalable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="40"/>
        <source>Collation</source>
        <translation>Intercalación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="40"/>
        <source>Comment</source>
        <translation>Comentario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="40"/>
        <source>Commutator Op.</source>
        <translation>Commutador Op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="41"/>
        <source>Configuration</source>
        <translation>Configuración</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="41"/>
        <source>Conn. limit</source>
        <translation>Conn. límite</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="41"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="76"/>
        <source>Constraint</source>
        <translation>Restricción</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="42"/>
        <source>Create DB</source>
        <translation>Crear BD</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="42"/>
        <source>Create role</source>
        <translation>Crear rol</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="42"/>
        <source>Curr. version</source>
        <translation>Corr. versión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="43"/>
        <source>Default</source>
        <translation>Por defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="43"/>
        <source>Default value</source>
        <translation>Valor por defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="43"/>
        <source>Definition</source>
        <translation>Definición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="44"/>
        <source>Delimiter</source>
        <translation>Delimitador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="44"/>
        <source>Dest. type</source>
        <translation>Dest. tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="44"/>
        <source>Dimension</source>
        <translation>Dimensión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="45"/>
        <source>Directory</source>
        <translation>Directorio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="45"/>
        <source>Dest. encoding</source>
        <translation>Dest. codificación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="45"/>
        <source>Element</source>
        <translation>Elemento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="46"/>
        <source>Encoding</source>
        <translation>Codificación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="46"/>
        <source>Encrypted</source>
        <translation>Encriptado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="46"/>
        <source>Enumerations</source>
        <translation>Enumeraciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="47"/>
        <source>Exec. cost</source>
        <translation>Costo ejec</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="47"/>
        <source>Expression</source>
        <translation>Expresión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="47"/>
        <source>Op. family</source>
        <translation>Op. familia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="48"/>
        <source>Final func.</source>
        <translation>Final func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="48"/>
        <source>Function</source>
        <translation>Función</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="48"/>
        <source>Func. type</source>
        <translation>Func. tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="49"/>
        <source>Handler func.</source>
        <translation>Handler func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="49"/>
        <source>Handles type</source>
        <translation>Handles tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="49"/>
        <source>Hashes</source>
        <translation>Hashes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="50"/>
        <source>Index type</source>
        <translation>Indixe tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="50"/>
        <source>Inherit</source>
        <translation>Heredar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="50"/>
        <source>Ini. condition</source>
        <translation>Ini. condición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="51"/>
        <source>Inline func.</source>
        <translation>En linea func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="51"/>
        <source>Input func.</source>
        <translation>Entrada func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="51"/>
        <source>Internal length</source>
        <translation>Longitud interna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="52"/>
        <source>Interval type</source>
        <translation>Intervalo tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="52"/>
        <source>I/O cast</source>
        <translation>I/O cast</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="52"/>
        <source>Join func.</source>
        <translation>Join func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="53"/>
        <source>Language</source>
        <translation>Languaje</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="53"/>
        <source>LC COLLATE</source>
        <translation>LC COLLATE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="53"/>
        <source>LC CTYPE</source>
        <translation>LC CTYPE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="54"/>
        <source>Leak proof</source>
        <translation>Prueba de fugas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="54"/>
        <source>Left type</source>
        <translation>Tipo izquierda</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="54"/>
        <source>Length</source>
        <translation>Longitud</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="55"/>
        <source>Library</source>
        <translation>Biblioteca</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="55"/>
        <source>Can login</source>
        <translation>Puede login</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="55"/>
        <source>Materialized</source>
        <translation>Materializada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="56"/>
        <source>Member roles</source>
        <translation>Roles miembros</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="56"/>
        <source>Merges</source>
        <translation>Unir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="56"/>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="57"/>
        <source>Negator op.</source>
        <translation>Op negador.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="57"/>
        <source>Not null</source>
        <translation>No null</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="57"/>
        <source>Object type</source>
        <translation>Tipo object</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="58"/>
        <source>OID</source>
        <translation>OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="58"/>
        <source>With OIDs</source>
        <translation>Con OIDs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="58"/>
        <source>Old version</source>
        <translation>Vieja version</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="59"/>
        <source>Operator</source>
        <translation>Operador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="59"/>
        <source>Operator func.</source>
        <translation>Operador func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="59"/>
        <source>Output func.</source>
        <translation>Salida func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="60"/>
        <source>Owner</source>
        <translation>Dueño</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="60"/>
        <source>Owner column</source>
        <translation>Dueño columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="60"/>
        <source>Parents</source>
        <translation>Padres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="61"/>
        <source>Password</source>
        <translation>Contraseña</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="61"/>
        <source>Permissions</source>
        <translation>Permisos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="61"/>
        <source>Precision</source>
        <translation>Precisión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="62"/>
        <source>Preferred</source>
        <translation>Preferido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="62"/>
        <source>Range attributes</source>
        <translation>Rango atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="62"/>
        <source>Receive func.</source>
        <translation>Recibe func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="63"/>
        <source>Ref. roles</source>
        <translation>Ref. roles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="63"/>
        <source>Replication</source>
        <translation>Replicación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="63"/>
        <source>Restriction func.</source>
        <translation>Restricción func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="64"/>
        <source>Return type</source>
        <translation>Retornar tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="64"/>
        <source>Returns SETOF</source>
        <translation>Retornar SETOF</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="64"/>
        <source>Right type</source>
        <translation>Tipo a la derecha</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="65"/>
        <source>Rows amount</source>
        <translation>Cantidad filas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="65"/>
        <source>Schema</source>
        <translation>Esquema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="65"/>
        <source>Security type</source>
        <translation>Tipo de seguridad</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="66"/>
        <source>Send func.</source>
        <translation>Envio func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="66"/>
        <source>Sort op.</source>
        <translation>Op ordenación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="66"/>
        <source>Source type</source>
        <translation>Tipo de fuentes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="67"/>
        <source>Src. encoding</source>
        <translation>Fnt. codificación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="67"/>
        <source>State type</source>
        <translation>Tipo estado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="67"/>
        <source>Storage</source>
        <translation>Almacenamiento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="68"/>
        <source>Superuser</source>
        <translation>Superusuario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="68"/>
        <source>Tablespace</source>
        <translation>Tablespace</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="68"/>
        <source>Type mod. in func.</source>
        <translation>Tipo mod. entrada func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="69"/>
        <source>Type mod. out func.</source>
        <translation>Tipo mod. salida func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="69"/>
        <source>Transition func.</source>
        <translation>Transición func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="69"/>
        <source>Trusted</source>
        <translation>Verdadero</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="70"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="70"/>
        <source>Type attribute</source>
        <translation>Tipo atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="70"/>
        <source>Types</source>
        <translation>Tipos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="71"/>
        <source>Unlogged</source>
        <translation>Invitados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="71"/>
        <source>Validator func.</source>
        <translation>Validador func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="71"/>
        <source>Validity</source>
        <translation>Validez</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="72"/>
        <source>Windows func.</source>
        <translation>Func. ventana.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="72"/>
        <source>false</source>
        <translation>falso</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="72"/>
        <source>true</source>
        <translation>verdadero</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="73"/>
        <source>Cache value</source>
        <translation>Valor en cache</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="73"/>
        <source>Cycle</source>
        <translation>Ciclo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="73"/>
        <source>Increment</source>
        <translation>Incremneto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="74"/>
        <source>Max. value</source>
        <translation>Max. valor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="74"/>
        <source>Min. value</source>
        <translation>Min valor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="74"/>
        <source>Start value</source>
        <translation>Valor inicial</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="75"/>
        <source>Last value</source>
        <translation>Próximo valor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="75"/>
        <source>Subtype</source>
        <translation>Subtipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="75"/>
        <source>Op. class</source>
        <translation>Op. clases</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="76"/>
        <source>Canonical func.</source>
        <translation>func. canonica.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="76"/>
        <source>Subtype diff func.</source>
        <translation>Subtipo diff func.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="77"/>
        <source>Deferrable</source>
        <translation>Diferible</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="77"/>
        <source>For each row</source>
        <translation>For each row</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="77"/>
        <source>Firing</source>
        <translation>Disparadores</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="78"/>
        <source>On insert</source>
        <translation>On insert</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="78"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="83"/>
        <source>On delete</source>
        <translation>On delete</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="78"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="86"/>
        <source>On update</source>
        <translation>On update</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="79"/>
        <source>On truncate</source>
        <translation>On truncate</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="79"/>
        <source>Arguments</source>
        <translation>Argumentos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="79"/>
        <source>Table</source>
        <translation>Tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="80"/>
        <source>Trigger func.</source>
        <translation>func. Trigger.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="80"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="85"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="86"/>
        <source>Columns</source>
        <translation>Columnas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="80"/>
        <source>Condition</source>
        <translation>Condición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="81"/>
        <source>Deferment</source>
        <translation>Diferimiento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="81"/>
        <source>Event</source>
        <translation>Evento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="81"/>
        <source>Execution mode</source>
        <translation>Modo Ejecución</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="82"/>
        <source>Commands</source>
        <translation>Comandos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="82"/>
        <source>Position</source>
        <translation>Posición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="82"/>
        <source>Comparison type</source>
        <translation>Tipo comparación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="83"/>
        <source>Ref. columns</source>
        <translation>Ref. columnas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="83"/>
        <source>Expressions</source>
        <translation>Expresiones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="84"/>
        <source>Fill factor</source>
        <translation>Factor de relleno</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="84"/>
        <source>No inherit</source>
        <translation>No hereda</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="84"/>
        <source>Op. classes</source>
        <translation>Op. clases</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="85"/>
        <source>Operators</source>
        <translation>Operador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="85"/>
        <source>Ref. table</source>
        <translation>Tabla Ref</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="86"/>
        <source>Unique</source>
        <translation>Unique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="87"/>
        <source>Predicate</source>
        <translation>Predicado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="87"/>
        <source>Collations</source>
        <translation>Intercalaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="87"/>
        <source>Inherited</source>
        <translation>Heredado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="88"/>
        <source>Client encoding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="88"/>
        <source>Configuration file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="88"/>
        <source>Data directory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="89"/>
        <source>Dynamic library path</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="89"/>
        <source>Dynamic shared memory</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="89"/>
        <source>Hba file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="90"/>
        <source>Listen addresses</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="90"/>
        <source>Max. connections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="90"/>
        <source>Listen port</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="91"/>
        <source>Server encoding</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="91"/>
        <source>SSL</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="91"/>
        <source>SSL ca file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="92"/>
        <source>SSL cert file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="92"/>
        <source>SSL crl file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="92"/>
        <source>SSL key file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="93"/>
        <source>Server version</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="93"/>
        <source>Ident file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="93"/>
        <source>Password encryption</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="94"/>
        <source>Connection ID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="94"/>
        <source>Server PID</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="94"/>
        <source>Server protocol</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="95"/>
        <source>Referrers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="95"/>
        <source>Identity</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="95"/>
        <source>Command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="96"/>
        <source>USING expr.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="96"/>
        <source>CHECK expr.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="96"/>
        <source>Roles</source>
        <translation type="unfinished">Roles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="97"/>
        <source>RLS enabled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="97"/>
        <source>RLS forced</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="118"/>
        <source>Show objects filter</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="124"/>
        <source>Show system objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="128"/>
        <source>Show extension objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="134"/>
        <source>Snippets</source>
        <translation>Snippets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="137"/>
        <source>Drop object</source>
        <translation>Drop object</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="140"/>
        <source>Drop cascade</source>
        <translation>Drop cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="143"/>
        <source>Truncate</source>
        <translation>Truncate</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="144"/>
        <source>Trunc. cascade</source>
        <translation>Trunc. cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="146"/>
        <source>Show data</source>
        <translation>Mostrar dato</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="148"/>
        <source>Reload properties</source>
        <translation>Recargar propiedades</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="150"/>
        <source>Update</source>
        <translation>Actualizar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="153"/>
        <source>Rename</source>
        <translation type="unfinished">Renombrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="156"/>
        <source>Source code</source>
        <translation type="unfinished">Código fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="208"/>
        <source>Quick refresh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="211"/>
        <source>Full refresh</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="945"/>
        <source>-- Source code unavailable for this kind of object --</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1193"/>
        <source>Do you really want to drop the object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;?</source>
        <translation>¿De verdad quiere borrar el objeto &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1196"/>
        <source>Do you really want to &lt;strong&gt;cascade&lt;/strong&gt; drop the object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;? This action will drop all the other objects that depends on it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1271"/>
        <source>Also restart sequences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1557"/>
        <source>Src. table: %1
Src. column(s): %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1568"/>
        <source>Ref. table: %1
Ref. column(s): %2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1768"/>
        <source>-- Source code genaration for buil-in and base types currently unavailable --</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1822"/>
        <source>-- Source code unavailable for the object %1 (%2). --</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1896"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1919"/>
        <source>Warning</source>
        <translation type="unfinished">Advertencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1897"/>
        <source>You&apos;re running a demonstration version! The data manipulation feature is available only in the full version!</source>
        <translation type="unfinished">Usted está ejecutando una versión de demostración! La función de manipulación de datos sólo está disponible en la versión completa!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1920"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; You are about to drop the entire database &lt;strong&gt;%1&lt;/strong&gt;! All data will be completely wiped out. Do you really want to proceed?</source>
        <translation type="unfinished">&lt;strong&gt;ATENCION:&lt;/strong&gt; Vas a dejar toda la base de datos &lt;strong&gt;%1&lt;/strong&gt;! Todos los datos serán completamente eliminados. ¿Realmente desea continuar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1267"/>
        <source>Do you really want to truncate the table &lt;strong&gt;%1&lt;/strong&gt;?</source>
        <translation>¿De verdad quiere truncar la tabla &lt;strong&gt;%1&lt;/strong&gt;?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1269"/>
        <source>Do you really want to &lt;strong&gt;cascade&lt;/strong&gt; truncate the table &lt;strong&gt;%1&lt;/strong&gt;? This action will truncate all the tables that depends on it?</source>
        <translation>¿De verdad quieres truncar en &lt;strong&gt;cascade&lt;/strong&gt; la tabla &lt;strong&gt;%1&lt;/strong&gt;? ¿Esta acción truncar todas las tablas que depende de ella?</translation>
    </message>
</context>
<context>
    <name>DatabaseImportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="139"/>
        <source>Settings</source>
        <translation>Configuración</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="170"/>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="194"/>
        <source>Connection:</source>
        <translation>Conexión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="215"/>
        <source>Automatically resolve dependencies</source>
        <translation>Resolver dependencias automáticamente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="765"/>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="803"/>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="841"/>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="879"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="506"/>
        <source>Random colors will be assigned to imported relationships facilitating the identification of links between tables mainly in large models.</source>
        <translation>Colores al azar serán asignados a las relaciones importados que faciliten la identificación de los vínculos entre las tablas, principalmente en modelos de gran tamaño.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="209"/>
        <source>Resolve some of the object&apos;s dependencies by querying the catalog when a needed object does not exists on the loaded set. In some cases it&apos;s necessary to combine this option with others below. This option does not applies to database level objects like role, tablespace and language as well for data types, extensions.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="509"/>
        <source>Random colors for relationships</source>
        <translation>Colores aleatorios para las relaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="354"/>
        <source>Enables the import of system built-in objects. It&apos;s recommend to select only those objects that are directly referenced by the ones to be imported. WARNING: Try to import a huge set of system objects can bloat the resultant model or even crash pgModeler due to memory/cpu overuse.</source>
        <translation>Permite la importación de sistema de objetos integrados. Se recomienda seleccionar sólo aquellos objetos que se hace referencia directamente por las que ser importados. ADVERTENCIA: Trate de importar un enorme conjunto de objetos del sistema puede inflar el modelo resultante o incluso estrellarse pgModeler debido a la memoria / CPU uso excesivo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="360"/>
        <source>Import system objects</source>
        <translation>Importa objeto del sistema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="399"/>
        <source>Enables the import of objects created by extensions. Generally there is no need to check this option but if there are objects in the database that directly references this category of objects this mode must be enabled.</source>
        <translation>Permite la importación de objetos creados por las extensiones. Generalmente no hay necesidad de revisar esta opción, pero si hay objetos en la base de datos que hace referencia directa a esta categoría de objetos de este modo debe estar habilitado.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="405"/>
        <source>Import extension objects</source>
        <translation>Importar extensiones de objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="444"/>
        <source>pgModeler ignores import errors and will try to create as many as possible objects. By checking this option the import operation will be not aborted but an incomplete model will be constructed. This option generates a log file on pgModeler&apos;s temp directory.</source>
        <translation>pgModeler ignora los errores de importación y tratará de crear el mayor número posible de objetos. Al marcar esta opción, la operación de importación no se concluye, pero un modelo incompleto se construirá. Esta opción genera un archivo de registro en el directorio temporal de pgModeler.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="450"/>
        <source>Ignore import errors</source>
        <translation>Ignorar errores de importación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="270"/>
        <source>All catalog queries as well the created objects&apos; source code are printed to standard output (stdout).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="35"/>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="122"/>
        <source>Import database</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="276"/>
        <source>Debug mode</source>
        <translation>Modo depurarión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="309"/>
        <source>Create all imported objects in the current working model instead of create a new one.</source>
        <translation>Crear todos los objetos importados en el modelo de trabajo actual en lugar de crear uno nuevo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="312"/>
        <source>Import objects to the working model</source>
        <translation>Importar objetos al modelo de trabajo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="553"/>
        <source>Database</source>
        <translation>Base de datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="702"/>
        <source>Filter:</source>
        <translation>Filtrar:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="722"/>
        <source>Filter object by it&apos;s OID</source>
        <translation>Filtrar objeto por OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="725"/>
        <source>By OID</source>
        <translation>Por OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="762"/>
        <source>Select all objects</source>
        <translation>Seleccionar todos los objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="800"/>
        <source>Clear object selection</source>
        <translation>Limpiar los objetos seleccionados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="838"/>
        <source>Expands all items</source>
        <translation>Expandir todos los item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="876"/>
        <source>Collapses all items</source>
        <translation>Contrae todos los item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="920"/>
        <source>Output</source>
        <translation>Salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="989"/>
        <source>Progress label...</source>
        <translation>Etiqueta de Progreso...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="999"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="1141"/>
        <source>&amp;Import</source>
        <translation>&amp;Importar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="1179"/>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="194"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; You are about to import objects to the current working model! This action will cause irreversible changes to it even in case of critical errors during the process. Do you want to proceed?</source>
        <translation>&lt;strong&gt;ATENCIÓN:&lt;/strong&gt; Estás a punto de importar objetos al modelo actual de trabajo! Esta acción provocará cambios irreversibles a él, incluso en caso de errores críticos durante el proceso. ¿Quieres proceder?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="397"/>
        <source>Importing process aborted!</source>
        <translation>Proceso de Importación abortado!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="508"/>
        <source>Importing process canceled by user!</source>
        <translation>Proceso de Importación cancelado por usuario!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="532"/>
        <source>Importing process sucessfuly ended!</source>
        <translation>Proceso de Importación terminado satisfactoriamente!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="596"/>
        <source>No databases found</source>
        <translation>Base de datos no encontrada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="616"/>
        <source>Found %1 database(s)</source>
        <translation>Encontrada %1 bases de dato(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="644"/>
        <source>Retrieving objects from database...</source>
        <translation>Recuperando objetos de base de datos...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="646"/>
        <source>Retrieving cluster level objects...</source>
        <translation>Recuperando objetos a nivel de clúster...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="693"/>
        <source>Retrieving objects of schema `%1&apos;...</source>
        <translation>Recuperando objetos del esquema `%1&apos;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="707"/>
        <source>Retrieving objects of `%1&apos; (%2)...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="833"/>
        <source>This is a PostgreSQL built-in data type and cannot be imported.</source>
        <translation>Esta es una tipo de datos incorporado PostgreSQL y no se puede importar.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="845"/>
        <source>This is a pgModeler&apos;s built-in object. It will be ignored if checked by user.</source>
        <translation>Esta es objeto incorporado del pgModeler. Se ignora si se activa por el usuario.</translation>
    </message>
</context>
<context>
    <name>DatabaseImportHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="183"/>
        <source>Retrieving system objects... `%1&apos;</source>
        <translation>Recuperando objetos del sistema... `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="233"/>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="257"/>
        <source>Retrieving objects... `%1&apos;</source>
        <translation>Recuperando objetos... `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="314"/>
        <source>Creating object `%1&apos; (%2), oid `%3&apos;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="364"/>
        <source>Trying to recreate object `%1&apos; (%2), oid `%3&apos;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="428"/>
        <source>Creating object `%1&apos; (%2)...</source>
        <translation>Creando objecto `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="388"/>
        <source>Import failed to recreate some objects in `%1&apos; tries.</source>
        <translation>Importación no pudo recrear algunos objetos en `%1&apos; intento.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="457"/>
        <source>Creating permissions for object `%1&apos; (%2)...</source>
        <translation>Creación permisos por objecto `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="475"/>
        <source>Creating columns permissions...</source>
        <translation>Creación permisos de columnas...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="526"/>
        <source>Updating relationships of `%1&apos; (%2)...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="563"/>
        <source>Validating relationships...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="2308"/>
        <source>Assigning sequences to columns...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="591"/>
        <source>The database import ended but some errors were generated and saved into the log file `%1&apos;. This file will last until pgModeler quit.</source>
        <translation>La importación de base de datos terminó pero algunos errores se genera y se guarda en el archivo de registro `%1&apos;. Este archivo se prolongará hasta salga de pgModeler.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="2271"/>
        <source>Destroying unused detached columns...</source>
        <translation>Destruyendo columnas separadas no utilizados...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="2246"/>
        <source>Creating table inheritances...</source>
        <translation>Creación de herencias de tabla...</translation>
    </message>
</context>
<context>
    <name>DatabaseModel</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="375"/>
        <source>The demonstration version can create only `%1&apos; instances of each object type! You&apos;ve reach this limit for the type: `%2&apos;</source>
        <translation>La versión de demostración sólo puede crear `%1&apos; instancias de cada tipo de objeto! Tienes que llegar a este límite para el tipo: `%2&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3006"/>
        <source>Loading: `%1&apos; (%2)</source>
        <translation>Cargando: `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3053"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3065"/>
        <source>Validating relationships...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="6572"/>
        <source>Generating %1 code: `%2&apos; (%3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9333"/>
        <source>Saving object `%1&apos; (%2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9488"/>
        <source>Saving metadata of the object `%1&apos; (%2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9512"/>
        <source>Metadata file successfully saved!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9515"/>
        <source>Process successfully ended but no metadata was saved!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9597"/>
        <source>Creating object `%1&apos; (%2)</source>
        <translation type="unfinished">Creando objecto `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9603"/>
        <source>Object `%1&apos; (%2) already exists. Ignoring.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9655"/>
        <source>Loading metadata for object `%1&apos; (%2)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9781"/>
        <source>Object `%1&apos; (%2) not found. Ignoring metadata.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9793"/>
        <source>Metadata file successfully loaded!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>DatabaseWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="39"/>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="157"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="244"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="69"/>
        <source>Template DB:</source>
        <translation>DB Modelo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="92"/>
        <source>Model Author:</source>
        <translation>Autor del Modelo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="121"/>
        <source>Encoding:</source>
        <translation>Codificación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="199"/>
        <source>Connections:</source>
        <translation>Conexiones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="286"/>
        <source>Options:</source>
        <translation type="unfinished">Opciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="295"/>
        <source>Allow connections</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="305"/>
        <source>Is template</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="344"/>
        <source>Default Objects</source>
        <translation>Objectos por Defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="362"/>
        <source>Tablespace:</source>
        <translation>Tablespace:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="369"/>
        <source>Schema:</source>
        <translation>Esquema:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="376"/>
        <source>Collation:</source>
        <translation>Colación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="383"/>
        <source>Owner:</source>
        <translation>Propietario:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="37"/>
        <source>The fields &lt;strong&gt;LC_COLLATE&lt;/strong&gt; and &lt;strong&gt;LC_CTYPE&lt;/strong&gt; have pre-configured values based upon the running system. You can freely modify those values if you intend to export the model to another host.</source>
        <translation>Los campos &lt;strong&gt;LC_COLLATE&lt;/strong&gt; y &lt;strong&gt;LC_CTYPE&lt;/strong&gt; valores basados en el sistema en funcionamiento han preconfigurado. Usted puede modificar libremente estos valores si tiene intención de exportar el modelo a otro host.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="42"/>
        <source>Use the above fields to specify the default attributes assigned to new objects created on the database model. Leaving a field empty will cause PostgreSQL to use the default values when exporting the model.</source>
        <translation>Utilice los campos de arriba para especificar los atributos predeterminados asignados a los nuevos objetos creados en el modelo de base de datos. Dejando a un campo vacío hará que PostgreSQL para utilizar los valores predeterminados al exportar el modelo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="55"/>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="67"/>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="135"/>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="140"/>
        <source>Default</source>
        <translation>Por Defecto</translation>
    </message>
</context>
<context>
    <name>DomainWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="33"/>
        <source>Attributes</source>
        <translation type="unfinished">Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="51"/>
        <source>Default Value:</source>
        <translation>Valor Por Defecto:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="63"/>
        <source>Not null</source>
        <translation type="unfinished">No null</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="73"/>
        <source>Check constraints</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="101"/>
        <source>Expression:</source>
        <translation type="unfinished">Expresión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="91"/>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/domainwidget.cpp" line="42"/>
        <source>Name</source>
        <translation type="unfinished">Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/domainwidget.cpp" line="45"/>
        <source>Expression</source>
        <translation type="unfinished">Expresión</translation>
    </message>
</context>
<context>
    <name>DonateWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="26"/>
        <source>Form</source>
        <translation type="unfinished">Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="71"/>
        <source>Donate to pgModeler</source>
        <translation>Hacer una donación a pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="96"/>
        <source>Hide this widget</source>
        <translation>Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="99"/>
        <source>...</source>
        <translation type="unfinished">...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="133"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;pgModeler is brought to you thanks to a &lt;span style=&quot; font-style:italic;&quot;&gt;great effort to create and distribute a quality product&lt;/span&gt;. This project is reaching out levels of maturity never imagined. All this is the result of a joint work between its author and the &lt;span style=&quot; font-weight:600;&quot;&gt;Open Source community&lt;/span&gt;. &lt;br/&gt;&lt;br/&gt;This software has a long way to go yet and with your help we&apos;ll keep maintaining the good job and bringing new improvements on each release. If you did like pgModeler and thinks it deserves a contribution please make a donation!&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;pgModeler es traído a usted gracias a un &lt;span style=&quot; font-style:italic;&quot;&gt;gran esfuerzo para crear y distribuir un producto de calidad&lt;/span&gt;. Este proyecto está alcanzando niveles de madurez nunca imaginados. Todo esto es el resultado de un trabajo conjunto entre su autor y la &lt;span style=&quot; font-weight:600;&quot;&gt;comunidad Open Source &lt;/span&gt;. &lt;br/&gt;&lt;br/&gt;Este software tiene un largo camino por recorrer y con su ayuda seguiremos manteniendo el buen trabajo y trayendo nuevas mejoras en cada versión. Si te gustó pgModeler y cree que merece una contribución, haz una donación!&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="182"/>
        <source>I want to help!</source>
        <translation>¡Quiero ayudar!</translation>
    </message>
</context>
<context>
    <name>ElementsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="180"/>
        <source>Column:</source>
        <translation>Columna:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="224"/>
        <source>Expression:</source>
        <translation>Expresión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="243"/>
        <source>Collation:</source>
        <translation>Colación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="53"/>
        <source>Operator Class:</source>
        <translation>Operador de Clases:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="199"/>
        <source>Operator:</source>
        <translation>Operador:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="106"/>
        <source>Sorting:</source>
        <translation>Ordenación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="124"/>
        <source>Ascending</source>
        <translation>Ascendente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="140"/>
        <source>Descending</source>
        <translation>Descendente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="153"/>
        <source>Nulls first</source>
        <translation>Nulos primeros</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="40"/>
        <source>Element</source>
        <translation>Elemento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="42"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="44"/>
        <source>Operator Class</source>
        <translation>Operador de Clases</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="46"/>
        <source>Sorting</source>
        <translation>Ordenación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="47"/>
        <source>Nulls First</source>
        <translation>Nulos Primero</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="132"/>
        <source>Collation</source>
        <translation>Colación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="151"/>
        <source>Operator</source>
        <translation>Operador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="248"/>
        <source>Expression</source>
        <translation>Expresión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="269"/>
        <source>Yes</source>
        <translation>Si</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="271"/>
        <source>No</source>
        <translation>No</translation>
    </message>
</context>
<context>
    <name>EventTriggerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="29"/>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="56"/>
        <source>Function:</source>
        <translation>Función:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="69"/>
        <source>Filter</source>
        <translation>Filtro</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="89"/>
        <source>Tag:</source>
        <translation>Tag:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/eventtriggerwidget.cpp" line="37"/>
        <source>Tag command</source>
        <translation>Comando tag</translation>
    </message>
</context>
<context>
    <name>Exception</name>
    <message>
        <location filename="../libutils/src/exception.cpp" line="23"/>
        <source>Assignment of a pseudo-type to the type of the column!</source>
        <translation>Asignación de un tipo de pseudo con el tipo de la columna!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="24"/>
        <source>Assignment of a precision greater than the length of the type!</source>
        <translation>Asignación de una precisión mayor que la longitud del tipo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="25"/>
        <source>Assignment of an invalid precision to type time, timestamp or interval. The precision in this case must be equal to or less than 6!</source>
        <translation>Asignación de una precisión no válido para el tipo de tiempo, marca de tiempo o intervalo. La precisión en este caso debe ser igual o inferior a 6!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="26"/>
        <source>Assignment of a not allocated column to object `%1&apos; (%2)!</source>
        <translation>Asignación de una columna no asignados a objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="27"/>
        <source>Reference to a column which index is out of the capacity of the column list!</source>
        <translation>La referencia a una columna que índice está fuera de la capacidad de la lista de columnas!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="28"/>
        <source>Assignment of not allocated object!</source>
        <translation>Asignación de objeto no asignado!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="29"/>
        <source>Assignment of a not allocated schema to object `%1&apos; (%2)!</source>
        <translation>Asignación de un esquema no asignado al objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="30"/>
        <source>The object `%1&apos; (%2) has inconsistent SQL or XML definition!</source>
        <translation>El objeto `%1&apos; (%2) tiene la configuración inconsistente (definición SQL / XML no válido)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="31"/>
        <source>The object `%1&apos; (%2) already exists on `%3&apos; (%4)!</source>
        <translation>El objeto `%1&apos; (%2) ya existe en `%3&apos; (%4)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="32"/>
        <source>The object `%1&apos; (%2) cannot be assigned because there is already exists in the container object `%3&apos;!</source>
        <translation>El objeto `%1&apos; (%2) no puede ser asignado porque ya se existe en el objeto contenedor `%3&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="33"/>
        <source>Assigning object of an invalid type!</source>
        <translation>Asignación de objeto de un tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="34"/>
        <source>Removing an object of an invalid type!</source>
        <translation>Eliminación de un objeto de un tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="35"/>
        <source>Obtaining an object of an invalid type!</source>
        <translation>Obtención de un objeto de un tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="36"/>
        <source>Assignment of empty name to table return type!</source>
        <translation>Asignación de nombre vacío de tipo de tabla de retorno!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="37"/>
        <source>The insertion of the parameter `%1&apos; will not be possible because there is another parameter with same name in the function `%2&apos;!</source>
        <translation>La inserción del parámetro `%1&apos; no será posible porque no hay otro parámetro con el mismo nombre en la función `%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="38"/>
        <source>The insertion of the table return type `%1&apos; will not be possible because there is another return type with the same name in the `%2&apos;!</source>
        <translation>La inserción del tipo de tabla de retorno `%1&apos; no será posible porque hay otro tipo de retorno con el mismo nombre en el `%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="39"/>
        <source>Reference to a parameter which index is out of the parameter list bounds!</source>
        <translation>La referencia a un evento que no pertenece a disparar!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="40"/>
        <source>Reference to an event which does not belongs to trigger!</source>
        <translation>La referencia a un evento que no pertenece al trigger!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="41"/>
        <source>The column `%1&apos; cannot be assigned to the trigger `%2&apos; because they belongs to different parent tables!</source>
        <translation>La columna `%1&apos; no puede ser asignada a un Trigger `%2&apos; porque no pertenece a la tabla padre!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="42"/>
        <source>Assignment of a not allocated function to object `%1&apos; (%2)!</source>
        <translation>Asignación de una función no asignada a objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="43"/>
        <source>Assignment of a function which return type is different from `%1&apos;!</source>
        <translation>Asignación de una función que tipo de retorno es diferente de `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="44"/>
        <source>Assignment of a function which parameter count is invalid to the object `%1&apos; (%2)!</source>
        <translation>Asignación de una función que cuente parámetro no es válido para el objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="45"/>
        <source>Assignment of a function which language is invalid!</source>
        <translation>Asignación de una función que el lenguaje no es válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="46"/>
        <source>Event trigger function must be coded in any language other than SQL!</source>
        <translation>Función de disparo de eventos debe estar codificado en cualquier idioma que no sea SQL!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="47"/>
        <source>Assignment of not allocated table to object `%1&apos; (%2)!</source>
        <translation>Asignación de tabla no asignado a objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="48"/>
        <source>Reference to an argument which index is out of argument list bounds!</source>
        <translation>La referencia a un argumento que el índice está fuera de la capacidad de la lista de argumentos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="49"/>
        <source>Assignment of empty name to an object!</source>
        <translation>Asignación de nombre vacío a un objeto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="50"/>
        <source>Assignment of a name which contains invalid characters!</source>
        <translation>Asignación de nombre no válido a un objeto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="51"/>
        <source>Assignment of a name which length exceeds the maximum of 63 characters!</source>
        <translation>Asignación de un nombre cuya longitud supera el máximo de 63 caracteres!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="52"/>
        <source>Assignment of schema object which type is invalid!</source>
        <translation>Asignación de objeto que tipo de esquema no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="53"/>
        <source>Assignment of tablespace object with invalid type!</source>
        <translation>Asignación de objeto de tablespace con tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="54"/>
        <source>Assignment of tablespace to an invalid object!</source>
        <translation>Asignación de tablespace a un objeto no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="55"/>
        <source>Assignment of tablespace to a constraint which type is invalid! To belong to a tablespace the constraint must be a primary key or unique!</source>
        <translation>Asignación de espacio de tabla a una restricción de qué tipo es válido! Para pertenecer a un espacio de tabla la restricción debe ser una clave primaria o único!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="56"/>
        <source>Assignment of owner object which type is invalid!</source>
        <translation>Asignación de objeto que tipo de propietario no es válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="57"/>
        <source>Assignment of owner to an invalid object!</source>
        <translation>Asignación de propietario para un objeto no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="58"/>
        <source>Assignment of appended or prepended SQL to an invalid object!</source>
        <translation>Asignación de SQL añadido o preañadido a un objeto no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="59"/>
        <source>Reference to a function with invalid type!</source>
        <translation>La referencia a una función con tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="60"/>
        <source>Reference to an argument of the operator with invalid type!</source>
        <translation>La referencia a un argumento del operador con el tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="61"/>
        <source>Reference to an operator with invalid type!</source>
        <translation>La referencia a un operador con tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="62"/>
        <source>Assignment of value to an invalid option type on role!</source>
        <translation>Asignación de valor a un tipo de opción no válida en el rol!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="63"/>
        <source>Reference to an invalid role type!</source>
        <translation>La referencia a un tipo de rol no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="64"/>
        <source>The insertion of the role `%1&apos; is not possible because this is already being referenced by role `%2&apos;!</source>
        <translation>La inserción del rol `%1&apos; no es posible porque esto ya está siendo referenciado por rol `%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="65"/>
        <source>Reference redundancy detected by having the role `%1&apos; referencing the role `%2&apos;!</source>
        <translation>Referencia redundancia detecta por tener el rol `%1&apos; referencia al rol `%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="66"/>
        <source>The role `%1&apos; can not be listed as a member of itself!</source>
        <translation>El rol `%1&apos; no puede ser listado como un miembro de sí mismo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="67"/>
        <source>Reference to a role which index is out of role list bounds!</source>
        <translation>La referencia a un rol el índice del cual está fuera de la lista de rol límites!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="68"/>
        <source>Insertion of empty command to the rule!</source>
        <translation>Inserción de comando nulo en la regla!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="69"/>
        <source>Reference to a command which index is out of the command list bounds!</source>
        <translation>La referencia a un comando que el índice está fuera de la capacidad de la lista de comandos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="70"/>
        <source>Is not possible to create a self generalization/copy relationship! The table can not inherit or copy their own attributes!</source>
        <translation>No es posible crear una relación de auto generalización / dependecy! La tabla no puede heredar o copiar sus propios atributos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="71"/>
        <source>Assignment of an object that already belongs to another table!</source>
        <translation>Asignación de un objeto que ya pertenece a otra tabla!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="72"/>
        <source>Assignment of a schema to the sequence which differs from the schema of the owner table!</source>
        <translation>Asignación de un esquema de la secuencia que se diferencia del esquema de la tabla dueño!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="73"/>
        <source>Assignment of an invalid value to one of the sequence attributes!</source>
        <translation>Asignación de un valor no válido para uno de los atributos de secuencia!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="74"/>
        <source>Assignment of a minimum value to the sequence which is greater than the maximum value!</source>
        <translation>Asignación de un valor mínimo a la secuencia que es mayor que el valor máximo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="75"/>
        <source>Assignment of a start value to the sequence which is extrapolating the range defined by minimum and maximum values!</source>
        <translation>Asignación de un valor de inicio a la secuencia que se extrapolando el rango definido por valores mínimo y máximo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="76"/>
        <source>Assignment of a null increment value to the sequence!</source>
        <translation>Asignación de un valor nulo de incremento a la secuencia!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="77"/>
        <source>Assignment of null cache value to the sequence!</source>
        <translation>Asignación de cache de secuencia con valor nulo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="78"/>
        <source>Assignment of owner table which is not in the same schema as the sequence `%1&apos;!</source>
        <translation>Asignación de dueño de tabla que no está en el mismo esquema que la secuencia `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="79"/>
        <source>Assignment of owner table which does not belong to the same owner of the sequence `%1&apos;!</source>
        <translation>Asignación de dueño de tabla que no pertenece al mismo dueño de la secuencia `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="80"/>
        <source>Assignment of a nonexistent owner column to the sequence `%1&apos;!</source>
        <translation>Asignación de una dueño inexistente a una columna para secuencia `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="81"/>
        <source>Assignment of an owner column to the sequence `%1&apos; that is not related to any table!</source>
        <translation>Asignación de una dueño a una columna para la secuencia `%1&apos; que no está relacionada a ninguna tabla!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="82"/>
        <source>Reference to a label which index is out of labels list bounds!</source>
        <translation>Referencia a una etiqueta que el índice está fuera de límites de la lista de etiquetas!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="83"/>
        <source>Allocation of object with invalid type!</source>
        <translation>Asignación del objeto con tipo no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="84"/>
        <source>Assignment of a function with invalid return type to object `%1&apos; (%2)!</source>
        <translation>Asignación de función con tipo de retorno inválido a objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="85"/>
        <source>Assignment of a function with invalid parameter(s) type(s) to object `%1&apos; (%2)!</source>
        <translation>Asignación de función con tipo de parâmetro(s) inválido(s) a objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="86"/>
        <source>Assignment of not allocated language!</source>
        <translation>Asignación de el lenguaje no asignado!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="87"/>
        <source>Assignment of language object which type is invalid!</source>
        <translation>Asignación de objeto de lenguaje que el tipo no es válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="88"/>
        <source>Reference to data type with an index outside the capacity of data types list!</source>
        <translation>Referencia al tipo de datos con un índice fuera de la capacidad de la lista actualizada tipos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="89"/>
        <source>Assignment of a null type to object `%1&apos; (%2)!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="128"/>
        <source>Unable to write the file `%1&apos; due to one or more errors in the definition generation process!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="129"/>
        <source>There is already a relationship between `%1&apos; (%2) and `%3&apos; (%4) in the model! When using relationships of the type generalization, copy and one-to-one there can&apos;t be other relationships linked to the pair of tables.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="130"/>
        <source>The configuration of the relationship `%1&apos; generates a redundancy between the relationships `%2&apos;. Redundancy on identifier or generalization/copy relationships are not accepted since they result in incorrect column spreading making the model inconsistent!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="164"/>
        <source>Unable to load the configuration file `%1&apos;! Please check if file exists in its folder and/or if it is not corrupted!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="169"/>
        <source>Invalid syntax in file `%1&apos;, line %2, column %3!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="170"/>
        <source>Invalid instruction `%1&apos; on file `%2&apos;, line %3, column %4!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="171"/>
        <source>Unknown attribute `%1&apos; in file `%2&apos;, line %3, column %4!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="172"/>
        <source>Invalid metacharacter `%1&apos; in file `%2&apos;, line %3, column %4!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="173"/>
        <source>Invalid operator `%1&apos; in comparison expression, file `%2&apos;, line %3, column %4!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="174"/>
        <source>Attribute `%1&apos; with an undefined value in file `%2&apos;, line %3, column %4!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="175"/>
        <source>Attribute `%1&apos; with an invalid name in file `%2&apos;, line %3, column %4!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="177"/>
        <source>Could not access the file or directory `%1&apos;! Make sure that it exists or if the user has access permissions on it!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="182"/>
        <source>Could not load file `%1&apos;. The same appears to be inconsistent or one of its dependencies (DTD files) has errors or is missing!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="195"/>
        <source>Reference to a tuple with an invalid index or the result is empty (no tuples)!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="198"/>
        <source>Invalid use of a view reference as whole SQL definition! The assigned reference must be an expression!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="202"/>
        <source>At the moment pgModeler does not support the creation of primary keys which some columns were generated by relationship connection. To create primary keys with this feature you can use the field `Identifier&apos; or the tab `Primary key&apos; on relationship editing form!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="216"/>
        <source>Reference to a function id which is incompatible with the user define type configuration!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="218"/>
        <source>Unsupported PostgreSQL version (%1) detected! Valid versions are between %2 and %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="224"/>
        <source>Invalid use of variadic parameter mode! This mode can be used only with an array or &quot;any&quot; data type!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="230"/>
        <source>The object `%1&apos; (%2), oid `%3&apos;, could not be imported due to one or more errors! Check the exception stack for more details. `HINT:&apos; if the object somehow references objects in `pg_catalog&apos; or `information_schema&apos; consider enable the importing of system objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="235"/>
        <source>Assignment of an invalid object to `%1&apos; (%2)! The assigned object must be of type `%3&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="238"/>
        <source>It&apos;s not possible convert the type of the column `%1&apos; to serial! It must have an `integer&apos; based type and its default value must be a call to `nextval(seq_name::regclass)&apos; function or a sequence object must be directly assigned to the column!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="242"/>
        <source>Trying to undo/redo an invalid operation over an object that does not exists anymore or can&apos;t be handled! The operation history will be cleaned up.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="245"/>
        <source>A parent table of `%1&apos; which OID is `%2&apos; was not found in the set of imported objects!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="246"/>
        <source>The enumeration `%1&apos; can&apos;t be assigned to the type `%2&apos; because contains invalid characters!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="247"/>
        <source>The enumeration `%1&apos; can&apos;t be assigned to the type `%2&apos; because is too long!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="248"/>
        <source>The connection was idle for too long and was automatically closed!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="249"/>
        <source>The connection was unexpectedly closed by the database server `%1&apos; at port `%2&apos;!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="250"/>
        <source>Failed to drop the database `%1&apos; because it is defined as the default database for the connection `%2&apos;!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="251"/>
        <source>The column `%1&apos; must be `NOT NULL&apos; because it composes the primary key of the table `%2&apos;. You need to remove the column from the mentioned contraint in order to disable the `NOT NULL&apos; on it!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="252"/>
        <source>The identity column `%1&apos; has an invalid data type! The data type must be `smallint&apos;, `integer&apos; or `bigint&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="253"/>
        <source>Reference to an invalid affected command in policy `%1&apos;!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="254"/>
        <source>Reference to an invalid special role in policy `%1&apos;!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="90"/>
        <source>Assignment of invalid type to the object!</source>
        <translation>Asignación de un tipo inválido a objeto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="91"/>
        <source>Assignment of an empty directory to object `%1&apos; (%2)!</source>
        <translation>Asignación de un diretório vacio a objeto: `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="92"/>
        <source>Obtaining types with invalid quantity!</source>
        <translation>Obtención de tipos con una cantidad no válida!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="93"/>
        <source>Insertion of item which already exists in the attributes list of the type!</source>
        <translation>Inserción de objeto que ya existe en la lista del tipo de atributos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="94"/>
        <source>Insertion of invalid item in the attributes list of the type!</source>
        <translation>Inserción de item inválido en la lista del tipo de atibutos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="95"/>
        <source>Insertion of item which already exists in the enumarations list of the type!</source>
        <translation>Inserción de item ya existente en la lista del tipo de enumeraciones!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="96"/>
        <source>Insertion of invalid item in the enumerations list of the type!</source>
        <translation>Inserción de item inválido en laa lista del tipo de enumeraciones!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="97"/>
        <source>Reference to an attribute which index is out of the attributes list bounds!</source>
        <translation>Referencia a un atributo que el índice está fuera de la capacidad de la lista de atributos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="98"/>
        <source>Reference to an enumeration which index is out of the enumerations list bounds!</source>
        <translation>Referencia a una enumeración con índice fuera de la capacidad de la lista de enumeraciones!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="99"/>
        <source>Assignment of invalid configuration to the type!</source>
        <translation>Asignación de configuración no válida para el tipo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="100"/>
        <source>Assignment of an operator which input type count is invalid to aggregate function!</source>
        <translation>Asignación de operador a una función de agregacíon con cantidad de tipos de entrada inválida!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="101"/>
        <source>Assignment of an operator which types of arguments is invalid!</source>
        <translation>Asignación de operador con argumentos de tipos inválidos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="102"/>
        <source>Assignment of system reserved name to the object `%1&apos; (%2)!</source>
        <translation>Asignación de nombre reservado por el sistema a objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="103"/>
        <source>One function with invalid configuration is been used by the object `%1&apos; (%2)!</source>
        <translation>Una función de configuración inválido está siendo usada por el objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="104"/>
        <source>Assignment of an invalid strategy/support number to an operator class element!</source>
        <translation>Asignación de un número inválido de estratégia/suporte del elemento de clase de operadores!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="105"/>
        <source>Insertion of element which already exists in the element list!</source>
        <translation>Inserción de elemento ya existente en la lista de elementos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="106"/>
        <source>Reference to an element which index is out of element list bounds!</source>
        <translation>Referencia a un elemento con índice fuera de límites de la lista de elementos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="107"/>
        <source>Reference to an object which index is out of object list bounds!</source>
        <translation>La referencia a un objeto que el índice está fuera de límites de la lista de objetos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="108"/>
        <source>Removal of an object not allocated!</source>
        <translation>Eliminación de un objeto no asignado!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="109"/>
        <source>The object `%1&apos; (%2) can not be removed because it is being referenced by object `%3&apos; (%4)!</source>
        <translation>El objeto `%1&apos; (%2) no puede ser removido pues está siendo referenciado por el objeto `%3&apos; (%4)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="110"/>
        <source>The object `%1&apos; (%2) can not be removed because it is being referenced by object `%3&apos; (%4) that belongs to `%5&apos; (%6)!</source>
        <translation>El objeto `%1&apos; (%2) no pode ser removido pues está siendo referenciado por el objeto `%3&apos; (%4) perteneciente a `%5&apos; (%6)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="111"/>
        <source>Operation with object(s) which type(s) is invalid!</source>
        <translation>Operación con objeto(s) de tipo(s) inválido(s)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="112"/>
        <source>Reference to object with invalid type!</source>
        <translation>Referencia a un objeto con tipo inválido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="113"/>
        <source>Operation with object not allocated!</source>
        <translation>Operación con objeto no asignado!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="114"/>
        <source>The creation of the relationship `%1&apos; between the table `%2&apos; and `%3&apos; can not be done because one does not have a primary key. If the relationship is of the type n-n both tables must have primary keys!</source>
        <translation>La criación de la Relación `%1&apos; entre las tabla `%2&apos; y `%3&apos; no puede ser hecha pues una de ellas no posee una llave primária. Si la relación es del tipo n-n ambas tablas deben tener claves principales!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="115"/>
        <source>The relationship of the type 1-1 where both tables are mandatory participation is not implemented because it requires fusion between the tables that breaks the modeling done by the user!</source>
        <translation>La relación del tipo 1-1, donde ambas tablas son la participación obligatoria no se aplica, ya que requiere la fusión entre las tablas que rompe el modelado realizado por el usuario!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="116"/>
        <source>Assignment of an invalid expression to the object!</source>
        <translation>Asignación de expresión inválida a objeto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="117"/>
        <source>Assignment of a primary key to a table which already has one!</source>
        <translation>Asignación de llave primaria a una tabla la cual ya poseia una!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="118"/>
        <source>Identifier relationship can not be created for a self relationship, relationships of the type n-n, copy or generalization!</source>
        <translation>Identificador relación no se puede crear de una relación autorelaciones del tipo n-n, dependencia o la generalización!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="119"/>
        <source>Unable to create a copy relationship because the column `%1&apos; in table `%2&apos; already exists in table `%3&apos;!</source>
        <translation>Imposible crear una Relación de dependencia pues la columna `%1&apos; de la tabla `%2&apos; ya existe en la tabla `%3&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="120"/>
        <source>Unable to create the generalization relationship because the column `%1&apos; in table `%2&apos; can not be merged with the column `%3&apos; of table `%4&apos; because they have incompatible types!</source>
        <translation>Imposible crear una Relación de generalización pues la columna `%1&apos; de la tabla `%2&apos; no puede ser funcionada con la columna `%3&apos; de la tabla `%4&apos; por ser de tipos incompatibles!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="121"/>
        <source>Unable to create the generalization relationship because the constraint `%1&apos; in table `%2&apos; can not be merged with the constraint `%3&apos; of table `%4&apos; due to their incompatible composition!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="122"/>
        <source>An attribute can not be added to a copy or generalization relationship!</source>
        <translation>Un atributo no puede ser adicionado a una Relación de generalización o dependencia!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="123"/>
        <source>A foreign key can not be added to a relationship because is created automatically when this is connected!</source>
        <translation>Una llave estrangera no puede ser adicionado a um Relación pues esta es creada automaticamente en el momento de la conexión de la mismo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="124"/>
        <source>The object `%1&apos; (%2) is referencing the object `%3&apos; (%4) which was not found in the model!</source>
        <translation>El objeto `%1&apos; (%2) está referenciando el objeto `%3&apos; (%4) el cual no fue encontrado en la lista de objetos del mismo tipo en el modelo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="125"/>
        <source>Reference to an user-defined data type that not exists in the model!</source>
        <translation>Referencia un tipo de dato definido por el usuário el cual no existe en el modelo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="126"/>
        <source>Assignment of invalid maximum size to operation list!</source>
        <translation>Asignación de tamaño máximo inválido a lista de operaciones!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="127"/>
        <source>Unable to write the file or directory `%1&apos;! Make sure the output directory exists, or if the user has write permissions over it!</source>
        <translation>Imposible escribir en el archivo `%1&apos;! Asegurase de que el diretório existe o si el usuário tiene permisos de aceso al mismo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="131"/>
        <source>One or more objects were invalidated and automatically removed because they were referencing table columns which were included through relationships and which no longer exists due to disconnection of relationships or exclusion of such generated columns!</source>
        <translation>Uno o más objetos fueron invalidadas y se eliminan de forma automática, ya que se hacer referencia a columnas de tabla que se incluyeron a través de relaciones y que ya no existe debido a la desconexión de las relaciones o la exclusión de tales columnas generadas!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="132"/>
        <source>The primary key `%1&apos; can only be allocated if declared within a block of code that defines a table or relationship!</source>
        <translation>La llave primaria `%1&apos; sólo puede ser asignada si es declarada dentro de un bloque de código que define una tabla o Relación!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="133"/>
        <source>Reference to an invalid privilege type!</source>
        <translation>Referencia a un tipo de privilégio inválido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="134"/>
        <source>Insertion of a role which already exists in the role list of the permission!</source>
        <translation>La inserción de un rol que ya existe en la lista de roles de los permisos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="135"/>
        <source>Assignment of privilege incompatible with the type of object referenced by permission!</source>
        <translation>Asignación de privilegios incompatibles con el tipo de objeto al que hace referencia el permiso!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="136"/>
        <source>There is already a permission on object `%1&apos; (%2) which has one or more equal roles from those present on permission to be assigned to the object!</source>
        <translation>Ya existe una permiso sobre el objeto `%1&apos; (%2) que tiene uno o más iguales roles de de aquellos presentes en el permiso para ser asignado al objeto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="137"/>
        <source>A permission is referencing the object `%1&apos; (%2) which was not found in the model!</source>
        <translation>Una permiso está referenciando al objeto `%1&apos; (%2) el cual no fue encontrado en el modelo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="138"/>
        <source>The object `%1&apos; (%2) can not be created because its not being assigned to any schema!</source>
        <translation>El objeto `%1&apos; (%2) no puede ser creado por no estar asignado a ningun esquema!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="139"/>
        <source>The tablespace `%1&apos; can not be inserted into the model because it points to the same directory as the tablespace `%2&apos;!</source>
        <translation>El tablespace `%1&apos; no puede ser insertado en el modelo pues apunta para el mismo diretório que el tablespace &apos;%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="140"/>
        <source>It is not possible to create arrays of domains or sequences (dimension &gt;= 1)! PostgreSQL does not yet implement this feature!</source>
        <translation>No es posíble crear arreglos de domínios o secuencias (dimención &gt;= 1)! PostgreSQL todavía no implementa esta funcionalidad!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="141"/>
        <source>The function `%1&apos; can not get a source code as a definition because its language is set to C. Use the attributes symbol and dynamic library instead!</source>
        <translation>La función `%1&apos; no puede obtener un código fuente como una definición, porque su lenguaje se establece en C. Use el símbolo de atributos y la biblioteca dinámica en su lugar!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="142"/>
        <source>The function `%1&apos; can have the attributes symbol and dynamic library configured only if the language is set to C. For all other cases you must specify a source code that defines it in the DBMS!</source>
        <translation>La función %1&apos; puede tener el símbolo de atributos y la biblioteca dinámica configurada sólo si el idioma se establece en C. Para todos los demás casos se debe especificar un código fuente que lo define en el SGBD!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="143"/>
        <source>The operator `%1&apos; can not be assigned as a comutator of operator `%2&apos; because it has incompatible settings!</source>
        <translation>El operador %1&apos;; no puede ser asignado como comutador de operador `%2&apos; porque tiene configuración incompatibles!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="144"/>
        <source>The operator `%1&apos; can not be assigned as negator of operator `%2&apos; because it has incompatible settings!</source>
        <translation>El operador `%1&apos; no puede ser asignado como negador del operador `%2&apos; porque tiene configuración incompatibles!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="145"/>
        <source>The type `%1&apos; can not self refer in the attributes `element&apos; or `copy type&apos; or be used as a data type of an attribute in the configuration of a composite type!</source>
        <translation>El tipo `%1&apos; no puede ser referenciado en los atributos `elementos&apos; o `tipo cópia&apos; o ser usado como tipo de dato de un atributo en la configuración de tipo compuesto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="146"/>
        <source>Assignment of invalid element to type `%1&apos;!</source>
        <translation>Asignación de elemento no válido con el tipo `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="147"/>
        <source>Assignment of invalid alignment to type `%1&apos;!</source>
        <translation>Asignación de elemento no válido con el tipo `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="148"/>
        <source>Assignment of invalid name to the table generated from N-N relationship!</source>
        <translation>Asignación de nombre no válido a la tabla genera a partir de la relación N-N!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="149"/>
        <source>The relationship `%1&apos; can not make use of the special primary key because it is marked as identifier or it is a self relationship!</source>
        <translation>La Relación `%1&apos; no puede hacer uso de la clave primaria especial. Sólo las relaciones de generalización / dependencia tienen acceso a este recurso!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="150"/>
        <source>The object `%1&apos; (%2) can not be edited or deleted because it was automatically included through a relationship! If the object is an attribute or constraint the modifications must be done on the relationship editing form.</source>
        <translation>El objeto `%1&apos; (%2) no puede ser editado o eliminado porque estaba incluido automáticamente a través de una relación! Para manipular el objeto es necesario hacerlo desde el formulario de edición de la relación que mantiene.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="151"/>
        <source>The object `%1&apos; (%2) can not be deleted because it is protected!</source>
        <translation>El objeto `%1&apos; (%2) no puede ser excluído por estar protegido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="152"/>
        <source>The group `%1&apos; has already been declared earlier!</source>
        <translation>El grupo `%1&apos; ya fue declarado anteriormente!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="153"/>
        <source>The group `%1&apos; can not be built in the groups declaration block (%2)!</source>
        <translation>El grupo `%1&apos; no puede ser construído en el bloque de declaración de grupos (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="154"/>
        <source>The group `%1&apos; was built but not declared in the groups declaration block (%2)!</source>
        <translation>El grupo `%1&apos; fue construído pero no declarado en el bloque de declaración de grupos (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="155"/>
        <source>The group `%1&apos; can not be built without possessing child elements!</source>
        <translation>El grupo `%1&apos; no se puede construir sin poseer elementos hijos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="156"/>
        <source>The group `%1&apos; can not be built once more because this was done in previous blocks!</source>
        <translation>El grupo `%1&apos; no puede ser construído nuevamente debido a que ya fue construído en el bloque anteriores!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="157"/>
        <source>The group `%1&apos; has been declared but not built!</source>
        <translation>El grupo `%1&apos; fue declarado pero no construído!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="158"/>
        <source>Reference to a column of the objects table with invalid index!</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="159"/>
        <source>Reference to a row of the objects table with invalid index!</source>
        <translation>Referencia a uaa columna de la tabla de objetos con índice inválido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="160"/>
        <source>The object `%1&apos; (%2) can not be manipulated because it is reserved to PostgreSQL! This object is present in the database model only as a reference!</source>
        <translation>El objeto `%1&apos; (%2) no puede ser manipulado porque está reservado para PostgreSQL! Este objeto está presente en el modelo de base de datos sólo como una referencia!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="161"/>
        <source>The new configuration of the function invalidates the object `%1&apos; (%2)! In this case it is needed to undo the relationship between the affected object and function in order to the new configuration to take effect!</source>
        <translation>La nueva configuración de la función invalida El objeto `%1&apos; (%2)! En este caso se necesita para deshacer la relación entre el objeto afectado y la función de forma que la nueva configuración de este último tenga efecto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="162"/>
        <source>A view reference must be used in at least one these SQL scopes: View Definition, SELECT-FROM, FROM-WHERE or After WHERE!</source>
        <translation>Una Referencia de Vista debe tener por lo menos una aplicación SQL: SELECT-FROM, FROM-WHERE o después WHERE!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="163"/>
        <source>Constraints like primary key, foreign key or unique must have at least one column related to them! For foreign keys must be selected, in addition, the referenced columns!</source>
        <translation>Restricciones como clave primaria, clave externa o único deben tener al menos una columna relacionada con ellos! Para las claves externas se deben seleccionar, además, el columnas referenciadas!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="165"/>
        <source>Could not find the default settings file `%1&apos;! To restore default settings check the existence of the file and try again!</source>
        <translation>No fue posíble encontrar el archivo de configuración por defecto `%1&apos;! Para restaurar la configuración predeterminada comprobar la existencia del archivo y vuelva a intentarlo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="166"/>
        <source>The export process failed due to an error triggered by the PostgreSQL server in an attempt to execute a SQL command. For more details about the error check the exception stack!

** Executed SQL command: **

%1</source>
        <translation>El proceso de exportación falló debido a un error provocado por el servidor PostgreSQL en un intento de ejecutar un comando SQL. Para más detalles sobre el error verificar la excepción pila!

** Comando SQL ejecutado: **

%1</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="167"/>
        <source>Could not load the plugin `%1&apos; from the library `%2&apos;! Message returned by plugin manager: `%3&apos;</source>
        <translation>No fue posible cargar el plugin `%1&apos; a partir de la biblioteca `%2&apos;! Mensaje devuelto por el administrador de plugins: `%3&apos;</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="168"/>
        <source>One or more plugins were not activated due to errors during the loading process! Check the exception stack for more details.</source>
        <translation>Uno o más plugins no se activan debido a errores durante el proceso de carga! Compruebe la pila de la excepción para obtener más detalles.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="176"/>
        <source>Assignment of empty XML buffer to parser!</source>
        <translation>Asignación de buffer de código XML vacio o parser!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="178"/>
        <source>Assignment of empty DTD file name!</source>
        <translation>Asignación de nombre de archivo DTD vacio!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="179"/>
        <source>Assignment of empty name to the DTD declaration!</source>
        <translation>Asignación de nombre vacio para la declaración DTD!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="180"/>
        <source>Error while interpreting XML buffer at line %1 column %2.
Message generated by the parser: %3. %4</source>
        <translation>Error mientras se interpretar búfer XML en línea %1 columna %2.
 Mensage generada por el parser:  %3 %4</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="181"/>
        <source>Operation on unallocated element tree! It is necessary to load the XML parser buffer and interpret it so that the tree is generated!</source>
        <translation>Operación de árbol de elementos sin asignar! Es necesario para cargar el tampón parser XML e interpretar de manera que se genera el árbol!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="183"/>
        <source>Operation with unallocated tree element!</source>
        <translation>Operación con elemento de árbol sin asignar!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="184"/>
        <source>Operation with element which does not exists in the element tree currently loaded!</source>
        <translation>Existe Funcionamiento con elemento que no lo hace en el árbol de elementos cargados actualmente!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="185"/>
        <source>Assignment of a value to an invalid connection parameter!</source>
        <translation>Asignación de un valor a un parámetro de conexión no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="186"/>
        <source>Operation on connection not established!</source>
        <translation>Operación en la conexión no se ha establecido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="187"/>
        <source>Attempt to connect without define configuration parameters!</source>
        <translation>Intenta conectarse sin definir los parámetros de configuración!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="188"/>
        <source>Attempt to start a connection already stablished!</source>
        <translation>Intenta iniciar una conexión ya establecida!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="189"/>
        <source>Could not connect to the database.
Message returned: `%1&apos;</source>
        <translation>No se pudo conectar a la base de datos.
Mensage retornada: `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="190"/>
        <source>Assignment of not allocated SQL command result!</source>
        <translation>Asignación de no asignados resultado de comandos SQL!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="191"/>
        <source>Unable to allocate the result of the SQL command because the response from the DBMS was not understood by the client!</source>
        <translation>No se puede asignar el resultado del comando SQL porque la respuesta de los DBMS no fue entendido por el cliente!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="192"/>
        <source>Unable to allocate command result for the SQL because the server has generated a fatal error!
Message returned by the DBMS: `%1&apos;</source>
        <translation>No se puede asignar resultado de comandos para el SQL porque el servidor ha generado un error fatal!
Mensage retornada por el SGBD: `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="193"/>
        <source>Reference to a column of tuple with invalid index!</source>
        <translation>Referencia a una columna de tupla con índice no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="194"/>
        <source>Reference to a column of tuple with invalid name!</source>
        <translation>Referencia a una columna de tupla con nombre no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="196"/>
        <source>Reference to a column of a tuple which was not yet initialized (tuple navigation not started)!</source>
        <translation>Referencia a una columna de una tupla que aún no se ha inicializado (navegación de tupla no iniciado)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="197"/>
        <source>Could not execute the SQL command.
 Message returned: `%1&apos;</source>
        <translation>No se pudo ejecutar el comando SQL.
 Mensage retornada: %1</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="199"/>
        <source>Assignment of a second definition expression to the view!</source>
        <translation>Asignación de una segunda expresión definición a la vista!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="200"/>
        <source>It is not possible mix ordinary references (SELECT-FROM, FROM-WHERE, After WHERE) with references used as view SQL definition!</source>
        <translation>No es posible la mezcla referencias ordinarias (SELECT-FROM, FROM-WHERE, After WHERE) con referencias utilizadas como definición SQL de la vista!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="201"/>
        <source>Assignment of collation object which type is invalid!</source>
        <translation>Asignación de objeto que tipo de colación no es válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="203"/>
        <source>Collations must be created at least with attributes LC_COLLATE and LC_CTYPE defined!</source>
        <translation>Colaciones deben crear al menos con los atributos definidos LC_COLLATE y LC_CTYPE!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="204"/>
        <source>The object `%1&apos; (%2) cannot reference itself! This operation is not permitted for this kind of object!</source>
        <translation>El objecto `%1&apos; (%2) no se puede hacer referencia a sí mismo! No se permite esta operación para este tipo de objeto!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="205"/>
        <source>Only operator families which uses `btree&apos; as indexing method are accepted by operator class elements!</source>
        <translation>Sólo las familias de operadores que utiliza ``btree&apos; como método de indexación son aceptados por elementos de clase operador!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="206"/>
        <source>Reference to an invalid copy table option!</source>
        <translation>Referencia a una opción de tabla de copia no válida!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="207"/>
        <source>Copy relationship between tables `%1&apos; and `%2&apos; cannot be done because the first one already copies attributes from `%3&apos;! Tables can have only one copy table!</source>
        <translation>Copia relación entre las tablas %1&apos; y `%2&apos; no se puede hacer porque el primero ya copias de los atributos `%3&apos;! Las tablas pueden tener sólo una mesa de copia!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="208"/>
        <source>The INSTEAD OF mode cannot be used on triggers that belongs to tables! This is available only for view triggers!</source>
        <translation>El modo INSTEAD OF no se puede utilizar en los triggers que pertenece a tabla! Este servicio está disponible sólo para ver triggers!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="209"/>
        <source>The TRUNCATE event can only be used when the trigger executes for each statement and belongs to a table!</source>
        <translation>El evento TRUNCATE sólo se puede utilizar cuando el disparador se ejecuta para cada declaración y pertenece a una mesa!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="210"/>
        <source>The INSTEAD OF mode cannot be used on view triggers that executes for each statement!</source>
        <translation>El modo INSTEAD OF no puede ser utilizado en vista desencadenantes que ejecuta para cada declaración!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="211"/>
        <source>Constraint triggers can only be executed on AFTER events and for each row!</source>
        <translation>Restricciones de triggers de  sólo pueden ejecutarse en eventos AFTER y para cada fila!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="212"/>
        <source>A view trigger cannot be AFTER/BEFORE when it executes for each row!</source>
        <translation>Un trigger de vista no puede ser AFTER/BEFORE cuando se ejecuta para cada fila!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="213"/>
        <source>A trigger cannot make reference to columns when using INSTEAD OF mode and UPDATE event!</source>
        <translation>Un trigger no puede hacer referencia a las columnas cuando se utiliza el modo INSTEAD OF y el evento UPDATE!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="214"/>
        <source>Assignment of a column which has no parent table to the object `%1&apos; (%2)!</source>
        <translation>Asignación de una columna que no tiene ninguna tabla padre al objeto `%1&apos; (%2)!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="215"/>
        <source>Only constraint triggers can be deferrable or reference another table!</source>
        <translation>Solo las restricción de triggers pueden ser prorrogable o hacer referencia a otra tabla!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="217"/>
        <source>The operator class assigned to the object `%1&apos; (%2) must use `btree&apos; as indexing method!</source>
        <translation>El operador de clase asignado al objeto `%1&apos; (%2) puede usar `btree&apos; como metodo de indexado!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="219"/>
        <source>The validation process failed due to an error triggered by the validation helper. For more details about the error check the exception stack!</source>
        <translation>El proceso de validación ha fallado debido a un error provocado por el ayudante de validación. Para más detalles sobre el error verificar la excepción pila!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="220"/>
        <source>The extension `%1&apos; is registered as a data type and cannot have the attribute `handles datatype&apos; modified!</source>
        <translation>La extensión `%1&apos; se registra como un tipo de datos y no se puede tener el atributo `handles datatype&apos; modificado!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="221"/>
        <source>The fk relationship `%1&apos; cannot be created because the foreign-key that represents it was not created on table `%2&apos;!</source>
        <translation>La relación fk `%1&apos; no se puede crear porque la clave externa que representa no se ha creado en la tabla `%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="222"/>
        <source>Assignement of an invalid object name pattern to the relationship `%1&apos;!</source>
        <translation>Asignacion de un patrón de nombre de objeto no válido a la relación `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="223"/>
        <source>Reference to an invalid object name pattern id on the relationship `%1&apos;!</source>
        <translation>Referencia a un inválido nombre del objeto patrón Identificación de la relación `%1&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="225"/>
        <source>Mixing incompatibles DBMS export modes: `ignore object duplications&apos;, `drop database&apos; or `drop objects&apos; cannot be used with `simulate export&apos;!</source>
        <translation>Mezcla incompatibles del SGDB en modos de exportación: `ignore object duplications&apos;, `drop database&apos; or `drop objects&apos; no se puede utilizar con `simulate export&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="226"/>
        <source>Mixing incompatibles DROP options: `drop database&apos; and `drop objects&apos; cannot be used at the same time!</source>
        <translation>Mezcla incompatibles en la opción DROP: `drop database&apos; y `drop objects&apos; no puede ser utilizado al mismo tiempo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="227"/>
        <source>Invalid object id swapping operation! The objects involved are the same!</source>
        <translation>Invalida operación de intercambio del objeto id! Los objetos involucrados son lo mismo!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="228"/>
        <source>Invalid object id swapping operation! The database itself, tablespaces or roles cannot have the ids swapped!</source>
        <translation>Invalida operación de intercambio del objeto id! La propia base de datos, tablespaces o roles no pueden tener los ids de intercambiar!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="229"/>
        <source>The widget already has a parent and cannot be assigned to a different object!</source>
        <translation>El widget de ya cuenta con uno de los padres y no puede ser asignado a un objeto diferente!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="231"/>
        <source>Could not load the database model file `%1&apos;. Check the error stack to see details. Try to run `pgmodeler-cli --fix-model&apos; in order to correct the structure of the file if that is the case.</source>
        <translation>No se pudo cargar el archivo de modelo de base de datos `%1&apos;. Compruebe la pila de errores para ver los detalles. Intente ejecutar `pgmodeler-cli --fix-model&apos; con el fin de corregir la estructura del archivo si ese es el caso.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="232"/>
        <source>The column `%1&apos; cannot reference it&apos;s parent table `%2&apos; as data type!</source>
        <translation>La columna `%1&apos; no puede hacer referencia es la tabla padre `%2&apos; como tipo de datos!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="233"/>
        <source>Operation with an invalid element id `%1&apos;!</source>
        <translation>Operación con un elemento id `%1&apos; no válido!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="234"/>
        <source>Reference to an invalid color id `%1&apos; for element `%2&apos;!</source>
        <translation>La referencia a un id `%1&apos; de color no válido para el elemento `%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="236"/>
        <source>The sequence `%1&apos; can&apos;t be assigned to the column `%2&apos; because the data type of the latter is incompatible. The type used must be an integer one!</source>
        <translation>La secuencia `%1&apos; no puede ser asignada a la columna `%2&apos; debido a que el tipo de datos de este último es incompatible. El tipo que se utiliza debe ser un número entero!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="237"/>
        <source>The option to generate temporary object names can only be used in simulation mode!</source>
        <translation>La opción para generar nombres de objeto temporal sólo se puede utilizar en el modo de simulación!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="239"/>
        <source>Could not assign the variable `%1&apos; to event trigger&apos;s filter. Currently, PostgreSQL supports only the `TAG&apos; variable!</source>
        <translation>No se pudo asignar la variable `%1&apos; al filtrar el evento trigger&apos;s. Actualmente, PostgreSQL soporta sólo la variable `TAG&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="240"/>
        <source>Could not perform the `%1&apos; operation on `%2&apos; using the data on row `%3&apos;! All changes were rolled back. 

 ** Returned error ** 

%4</source>
        <translation>No se pudo realizar la operación `%1&apos; en `%2&apos; usando el dato en la fila `%3&apos;! Todos los cambios fueron deshacen. 

 ** Error retornado ** 

%4</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="241"/>
        <source>Malformed unescaped value on row `%1&apos; column `%2&apos;!</source>
        <translation>Valor sin escapar con formato incorrecto en la fila `%1&apos; columna `%2&apos;!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="243"/>
        <source>The object `%1&apos; (%2) can&apos;t be handled because some needed fields are not set! Please, make sure to fill at least the requires fields in order to properly create or update the object.</source>
        <translation>El objeto `%1&apos; (%2) no se pueden manejar debido a que algunos campos necesarios no están ajustados! Por favor, asegúrese de llenar al menos la requiere campos con el fin de crear o actualizar correctamente el objeto.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="244"/>
        <source>A relationship can only be swapped by other object of the same kind!</source>
        <translation>Una relación sólo puede ser intercambiado por otro objeto del mismo tipo!</translation>
    </message>
</context>
<context>
    <name>ExtensionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="38"/>
        <source>Version:</source>
        <translation>Versión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="67"/>
        <source>Old Version:</source>
        <translation>Versión Antigua:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="96"/>
        <source>This attribute cannot be changed once the object is created.</source>
        <translation>Este atributo no se puede cambiar una vez que se crea el objeto.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="99"/>
        <source>Handles data type</source>
        <translation>Tipo de datos Handles</translation>
    </message>
</context>
<context>
    <name>FindReplaceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="188"/>
        <source>Hide this widget</source>
        <translation type="unfinished">Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="191"/>
        <source>...</source>
        <translation type="unfinished">...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="249"/>
        <source>Replace one occurrence</source>
        <translation>Reemplazar una ocurrencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="252"/>
        <source>Replace</source>
        <translation>Reemplazar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="271"/>
        <source>Replace all occurrences</source>
        <translation>Reemplazar todas las ocurrencias</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="274"/>
        <source>Replace All</source>
        <translation>Reemplazar todas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="293"/>
        <source>Replace the selection and find the next one</source>
        <translation>Reemplace la selección y buscar la siguiente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="296"/>
        <source>Replace &amp;&amp; Find</source>
        <translation>Reemplazar &amp;&amp; Buscar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="213"/>
        <source>Replace:</source>
        <translation>Reemplazar:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="44"/>
        <source>Find:</source>
        <translation>Buscar:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="64"/>
        <source>Find previous</source>
        <translation>Buscar anterior</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="80"/>
        <source>Shift+F3</source>
        <translation>Shift+F3</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="98"/>
        <source>Find next</source>
        <translation>Buscar siguiente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="114"/>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="130"/>
        <source>Case sensitive</source>
        <translation>Caso sensible</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="143"/>
        <source>Regular expression</source>
        <translation>Expresón regular</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="156"/>
        <source>Whole words</source>
        <translation>Palabras completas</translation>
    </message>
</context>
<context>
    <name>FunctionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="60"/>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="97"/>
        <source>Security:</source>
        <translation>Seguridad:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="129"/>
        <source>Function Type:</source>
        <translation>Tipo de Función:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="187"/>
        <source>Rows Returned:</source>
        <translation>Filas Retornadas:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="262"/>
        <source>Return Method:</source>
        <translation>Método de Retorno:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="281"/>
        <source>Behavior:</source>
        <translation>Comportamiento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="325"/>
        <source>Set</source>
        <translation>Conjunto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="215"/>
        <source>Language:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="306"/>
        <source>Si&amp;mple</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="341"/>
        <source>Tab&amp;le</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="359"/>
        <source>Return Table</source>
        <translation>Tabla de retorno</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="384"/>
        <source>Execution Cost:</source>
        <translation>Costo ejecución:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="400"/>
        <source>Windown Func.</source>
        <translation>Func. Ventana.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="439"/>
        <source>Leakproof</source>
        <translation>Prueba de fugas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="447"/>
        <source>Parameters</source>
        <translation>Parámetros</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="452"/>
        <source>Definition</source>
        <translation>Definición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="503"/>
        <source>Dynamic Library:</source>
        <translation>Biblioteca Dinámica:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="532"/>
        <source>Symbol:</source>
        <translation>Símbolo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="561"/>
        <source>Library:</source>
        <translation>Biblioteca:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="627"/>
        <source>Source code:</source>
        <translation>Código fuente:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="53"/>
        <source>Column</source>
        <translation>Columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="55"/>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="63"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="61"/>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="65"/>
        <source>Mode</source>
        <translation>Modo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="66"/>
        <source>Default Value</source>
        <translation>Valor por defecto</translation>
    </message>
</context>
<context>
    <name>GeneralConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="48"/>
        <source>General &amp;&amp; Design</source>
        <translation>Diseño &amp;&amp; General</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="999"/>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="513"/>
        <source>Operation history:</source>
        <translation type="unfinished">Histórico de operaciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1250"/>
        <source>Check if there is a new version on server</source>
        <translation>Compruebe si hay una nueva versión en el servidor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="66"/>
        <source>Design</source>
        <translation>Diseño</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="766"/>
        <source>Graphical objects (table, views and textboxes) will be created in a single step without the need to click on canvas </source>
        <translation>Los objetos gráficos (mesa, puntos de vista y los cuadros de texto) se crearán en un solo paso sin la necesidad de hacer clic sobre lienzo </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="769"/>
        <source>Simplify creation of graphical objects</source>
        <translation>Simplificar la creación de objetos gráficos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="327"/>
        <source>After loading the model the last zoom and position on canvas will be restored</source>
        <translation>Después de cargar el modelo se restaurará el último zoom y la posición en la lona</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="330"/>
        <source>Save and restore last position and zoom</source>
        <translation>Salvar y restaurar la última posición y el zoom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="724"/>
        <source>Disable render smoothness</source>
        <translatorcomment>Deshabilitar renderizado suave</translatorcomment>
        <translation>Deshabilitar render suave</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="904"/>
        <source>Hide the object that represents the relationship name</source>
        <translation>Ocultar el objeto que representa el nombre de relación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="907"/>
        <source>Hide relationship name</source>
        <translation>Ocultar nombre de la relación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="590"/>
        <source>Validate before save, export or diff</source>
        <translation>Validar antes de guardar, exportar o diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="465"/>
        <source>Hide the object which represents the tag assigned to the table</source>
        <translation>Ocultar el objeto que representa la etiqueta asignada a la tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="468"/>
        <source>Hide table tags</source>
        <translation>Ocultar etiquetas de tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="279"/>
        <source>Start move the canvas when the cursor is on the canvas edges</source>
        <translation>Iniciar mover el lienzo cuando el cursor está en los bordes de la lona</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="282"/>
        <source>Move canvas by keep mouse on corners</source>
        <translation>Mueva la lona por el ratón torreón en las esquinas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="420"/>
        <source>Hide the portion of table which represent triggers, indexes and rules</source>
        <translation>Ocultar la parte de la tabla que representan triggers, índices y reglas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="423"/>
        <source>Hide table extended attributes</source>
        <translation>Ocultar atributos de tabla extendida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="817"/>
        <source>Enable SQL code completion</source>
        <translation>Activar autocompletado de código SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1343"/>
        <source>Printing &amp;&amp; Code</source>
        <translation>Imprimir &amp;&amp; Cógigo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1498"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1385"/>
        <source>Size:</source>
        <translation>Tamaño:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1398"/>
        <source>Font:</source>
        <translation>Fuente:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1565"/>
        <source>Printing</source>
        <translation>Imprimiendo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1424"/>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1583"/>
        <source>Options:</source>
        <translation>Opciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="721"/>
        <source>Disable antialiasing for lines and texts improving performance when handling huge models.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="587"/>
        <source>Triggers a dialog asking the user to validate the model before a save, export or diff operation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="859"/>
        <source>When enabled this option creates a placeholder object at the previous table&apos;s position when starting to move it. This will cause graphical updates on relationship lines to be performed only when the drag &amp; drop action is done improving the performance. Disabling placeholders will cause those updates to be executed every time the table&apos;s position changes a single pixel (classical behavior).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="862"/>
        <source>Use placeholders when moving tables</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="814"/>
        <source>Toggles the code completion in all fields that accepts the input of SQL commands.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="101"/>
        <source>Minimum object opacity (%): </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="120"/>
        <source>Defines the minimum opacity percentage applied to the objects when using the fade out feature. A zero opacity causes the object to be completely hidden not being possible to interact with it in the canvas area.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="193"/>
        <source>Canvas grid size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="212"/>
        <source>Defines the vertical and horizontal grid size. This value affects the spacing of objects when using object grid alignment feature.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="372"/>
        <source>By default the range selection is triggered with Shift + left click. By checking this option range selection will be activated only with a single click and move.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="375"/>
        <source>Trigger range selection with a single click</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="532"/>
        <source>Defines the maximum amount of elements held in the operation history. Once reached the maximum number the history is automatically cleaned.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="641"/>
        <source>Defines the period when the opened models will be saved automatically.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="644"/>
        <source>Autosave interval (minutes):</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="949"/>
        <source>Replaces any straight line in relationship by curved ones in order to improve the model&apos;s visualization.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="952"/>
        <source>Use curved lines for relationships</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1233"/>
        <source>SQL history max. length:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1325"/>
        <source>Souce code editor:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1072"/>
        <source>lines</source>
        <translation>lineas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1116"/>
        <source>Clear the entire SQL comand history.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1119"/>
        <source>Clear history</source>
        <translation>Limpiar historial</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1332"/>
        <source>Configurations directory:</source>
        <translation>Directorio de configuraciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1305"/>
        <source>Browse the source code editor application</source>
        <translation>Examinar la aplicación del editor de código fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1194"/>
        <source>Open in file manager</source>
        <translation type="unfinished">Abrir en el administrador de archivos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1253"/>
        <source>Check updates at startup</source>
        <translation>Comprobar actualizaciones al inicio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1017"/>
        <source>Souce code editor args:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1280"/>
        <source>User interface language:</source>
        <translation>Idioma de la interfaz de usuario:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1214"/>
        <source>Overrides the default user interface language defined by the system. Requires restarting the program. &lt;strong&gt;NOTE:&lt;/strong&gt; UI translations are third party collaborations thus any typo or mistake should be reported directly to their respective maintainers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1361"/>
        <source>Code style</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1411"/>
        <source>Colors:</source>
        <translation type="unfinished">Colores:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1433"/>
        <source>Display line numbers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1449"/>
        <source>Highlight lines at cursor&apos;s position</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1521"/>
        <source>Custom tab width:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1596"/>
        <source>Print grid</source>
        <translation>Imprimir cuadrícula</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1612"/>
        <source>Print page numbers</source>
        <translation>Imprimir número de página</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1628"/>
        <source>Paper:</source>
        <translation>Papel:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1651"/>
        <source>A0 (841 x 1189 mm)</source>
        <translation>A0 (841 x 1189 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1656"/>
        <source>A1 (594 x 841 mm)</source>
        <translation>A1 (594 x 841 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1661"/>
        <source>A2 (420 x 594 mm)</source>
        <translation>A2 (420 x 594 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1666"/>
        <source>A3 (297 x 420 mm)</source>
        <translation>A3 (297 x 420 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1671"/>
        <source>A4 (210 x 297 mm)</source>
        <translation>A4 (210 x 297 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1676"/>
        <source>A5 (148 x 210 mm)</source>
        <translation>A5 (148 x 210 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1681"/>
        <source>A6 (105 x 148 mm)</source>
        <translation>A6 (105 x 148 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1686"/>
        <source>A7 (74 x 105 mm)</source>
        <translation>A7 (74 x 105 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1691"/>
        <source>A8 (52 x 74 mm)</source>
        <translation>A8 (52 x 74 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1696"/>
        <source>A9 (37 x 52 mm)</source>
        <translation>A9 (37 x 52 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1701"/>
        <source>B0 (1030 x 1456 mm)</source>
        <translation>B0 (1030 x 1456 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1706"/>
        <source>B1 (728 x 1030 mm)</source>
        <translation>B1 (728 x 1030 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1711"/>
        <source>B10 (32 x 45 mm)</source>
        <translation>B10 (32 x 45 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1716"/>
        <source>B2 (515 x 728 mm)</source>
        <translation>B2 (515 x 728 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1721"/>
        <source>B3 (364 x 515 mm)</source>
        <translation>B3 (364 x 515 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1726"/>
        <source>B4 (257 x 364 mm)</source>
        <translation>B4 (257 x 364 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1731"/>
        <source>B5 (182 x 257 mm)</source>
        <translation>B5 (182 x 257 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1736"/>
        <source>B6 (128 x 182 mm)</source>
        <translation>B6 (128 x 182 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1741"/>
        <source>B7 (91 x 128 mm)</source>
        <translation>B7 (91 x 128 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1746"/>
        <source>B8 (64 x 91 mm)</source>
        <translation>B8 (64 x 91 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1751"/>
        <source>B9 (45 x 64 mm)</source>
        <translation>B9 (45 x 64 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1756"/>
        <source>C5E (163 x 229 mm)</source>
        <translation>C5E (163 x 229 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1761"/>
        <source>Comm10E (105 x 241 mm)</source>
        <translation>Comm10E (105 x 241 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1766"/>
        <source>DLE (110 x 220 mm)</source>
        <translation>DLE (110 x 220 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1771"/>
        <source>Executive (191 x 254 mm)</source>
        <translation>Ejecutivo (191 x 254 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1776"/>
        <source>Folio (210 x 330 mm)</source>
        <translation>Folio (210 x 330 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1781"/>
        <source>Ledger (432 x 279 mm)</source>
        <translation>Ledger (432 x 279 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1786"/>
        <source>Legal (216 x 356 mm)</source>
        <translation>Legal (216 x 356 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1791"/>
        <source>Letter (216 x 279 mm)</source>
        <translation>Carta (216 x 279 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1796"/>
        <source>Tabloid (279 x 432 mm)</source>
        <translation>Tabloid (279 x 432 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1801"/>
        <source>Custom</source>
        <translation>Personalizar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1815"/>
        <source>Orientation:</source>
        <translation>Orientación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1828"/>
        <source>Landscape</source>
        <translation>Landscape</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1844"/>
        <source>Portrait</source>
        <translation>Portrait</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1851"/>
        <source>Unity:</source>
        <translation>Unidad:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1858"/>
        <source>Custom Size:</source>
        <translation>Tamaño Personalizado:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1873"/>
        <source>Width:</source>
        <translation>Ancho:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1902"/>
        <source>Height:</source>
        <translation>Altura:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1927"/>
        <source>Page Margins:</source>
        <translation>Márgenes de página:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1942"/>
        <source>Left:</source>
        <translation>Izquierdo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1961"/>
        <source>Left margin</source>
        <translation>Margen izquierdo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1971"/>
        <source>Top:</source>
        <translation>Superior:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1990"/>
        <source>Top margin</source>
        <translation>Margen superior</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2006"/>
        <source>Right:</source>
        <translation>Derecho:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2047"/>
        <source>Right margin</source>
        <translation>Margen derecho</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2057"/>
        <source>Bottom:</source>
        <translation>Inferior:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2025"/>
        <source>Bottom margin</source>
        <translation>Margen inferior</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2076"/>
        <source>Milimeters</source>
        <translation>Milímetros</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2081"/>
        <source>Pixels</source>
        <translation>Pixeles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2086"/>
        <source>Inches</source>
        <translation>Pulgadas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2091"/>
        <source>Centimeter</source>
        <translation>Centímetros</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="44"/>
        <source>Line numbers&apos; font color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="47"/>
        <source>Line numbers&apos; background color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="50"/>
        <source>Highlighted line color</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="54"/>
        <source>The little brown fox jumps over the lazy dog</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="241"/>
        <source>System default</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="636"/>
        <source>All files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="638"/>
        <source>Load file</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>GenericSQLWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/genericsqlwidget.ui" line="32"/>
        <source>SQL code</source>
        <translation type="unfinished">Código SQL</translation>
    </message>
</context>
<context>
    <name>HintTextWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/hinttextwidget.ui" line="44"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
</context>
<context>
    <name>IndexWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="42"/>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="88"/>
        <source>Indexing:</source>
        <translation>Indexación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="133"/>
        <source>Fill Factor:</source>
        <translation>Factor Relleno:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="192"/>
        <source>Options:</source>
        <translation>Opciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="222"/>
        <source>Concurrent</source>
        <translation>Concurrente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="247"/>
        <source>Unique</source>
        <translation>Unico</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="272"/>
        <source>Fast update</source>
        <translation>Actual. Rápida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="285"/>
        <source>Buffering</source>
        <translation>Buffering</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="312"/>
        <source>Predicate:</source>
        <translation>Predicado:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="357"/>
        <source>Elements</source>
        <translation>Elementos</translation>
    </message>
</context>
<context>
    <name>LanguageWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="63"/>
        <source>Trusted:</source>
        <translation>Confiable:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="82"/>
        <source>Validator Func.:</source>
        <translation>Func. Validador:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="101"/>
        <source>Handler Func.:</source>
        <translation>Func. Handler:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="120"/>
        <source>Inline Func.:</source>
        <translation>Func. en linea:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/languagewidget.cpp" line="42"/>
        <source>The functions to be assigned to the language should have, respectively, the following signatures:&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Handler Function:&lt;/strong&gt; &lt;em&gt;language_handler function()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Validator Function:&lt;/strong&gt; &lt;em&gt;void function(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Inline Function:&lt;/strong&gt; &lt;em&gt;void function(internal)&lt;/em&gt;</source>
        <translation>Las funciones que se asignará a el lenguaje debe tener, respectivamente, las siguientes firmas:&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Función Handler:&lt;/strong&gt; &lt;em&gt;language_handler funcao()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Función Validador:&lt;/strong&gt; &lt;em&gt;void funcao(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Función en línea:&lt;/strong&gt; &lt;em&gt;void funcao(internal)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="29"/>
        <source>pgModeler - PostgreSQL Database Modeler</source>
        <translation>pgModeler - Modelador de Base de datos PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="616"/>
        <source>Toggle the model objects widget</source>
        <translation>Cambiar el modelo de objetos del widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="619"/>
        <source>O&amp;bjects</source>
        <translation>O&amp;bjectos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="632"/>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="482"/>
        <source>Toogle the model validation widgets</source>
        <translation>Cambiar el modelo de validación del widgets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="485"/>
        <source>&amp;Validation</source>
        <translation>&amp;Validación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="498"/>
        <source>Alt+V</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="565"/>
        <source>Toggle the operation history widget</source>
        <translation>Cambiar el historial de operaciones del widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="568"/>
        <source>&amp;Operations</source>
        <translation>&amp;Operaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="581"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="517"/>
        <source>Toggle the object finder</source>
        <translation>Cambiar el buscador objeto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="520"/>
        <source>Find Object</source>
        <translation>Buscar Objeto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="533"/>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="692"/>
        <source>&amp;File</source>
        <translation>&amp;Archivo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="717"/>
        <source>&amp;Edit</source>
        <translation>&amp;Editar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="737"/>
        <source>&amp;Show</source>
        <translation>&amp;Mostrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="771"/>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="832"/>
        <source>Controls</source>
        <translation>Controles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="962"/>
        <source>&amp;Load</source>
        <translation type="unfinished">&amp;Cargar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="977"/>
        <source>Sa&amp;ve as</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="986"/>
        <source>E&amp;xit</source>
        <translation>&amp;Salir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="989"/>
        <source>Exit pgModeler</source>
        <translation>Salir de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1007"/>
        <source>&amp;About pgModeler</source>
        <translation>&amp;Acerca de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1025"/>
        <source>&amp;Print</source>
        <translation>Im&amp;primir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1028"/>
        <source>Print model</source>
        <translation>Imprimir modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1043"/>
        <source>&amp;Undo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1046"/>
        <source>Undo operation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1061"/>
        <source>&amp;Redo</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1064"/>
        <source>Redo operation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1079"/>
        <source>&amp;Export</source>
        <translation type="unfinished">&amp;Exportar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1085"/>
        <source>Ctrl+Shift+E</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1103"/>
        <source>&amp;Show grid</source>
        <translation>Mostrar &amp;grid</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1121"/>
        <source>&amp;Close</source>
        <translation type="unfinished">&amp;Cerrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1139"/>
        <source>&amp;Normal zoom</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1160"/>
        <source>&amp;Align to grid</source>
        <translation>&amp;Alinear a la cuadrícula</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1184"/>
        <source>Show &amp;delimiters</source>
        <translation>Mostrar &amp;delimitadores</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1199"/>
        <source>&amp;Settings</source>
        <translation>&amp;Configuración</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1205"/>
        <source>F12</source>
        <translation type="unfinished">F12</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1228"/>
        <source>&amp;Overview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1243"/>
        <source>&amp;Support</source>
        <translation>&amp;Soporte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1246"/>
        <source>Access the support page</source>
        <translation>Acceder a la página de soporte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1258"/>
        <source>New</source>
        <translation>Nuevo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1391"/>
        <source>&amp;Diff</source>
        <translation>&amp;Diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1394"/>
        <source>Determine the changes between model/database and another database</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1436"/>
        <source>Design database models</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1439"/>
        <source>Shift+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1457"/>
        <source>Shift+M</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1466"/>
        <source>&amp;Bug report</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1469"/>
        <source>Report a bug</source>
        <translation>Reportar un error</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1481"/>
        <source>Donate</source>
        <translation>Donar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1484"/>
        <source>Help pgModeler by donating!</source>
        <translation>¡Ayuda a pgModeler donando!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1496"/>
        <source>Objects me&amp;tadata</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1499"/>
        <source>Objects metadata</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1511"/>
        <source>Arrange objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1514"/>
        <source>Rearrange objects over the canvas</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="890"/>
        <source>Ctrl+N</source>
        <translation>Ctrl+N</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="908"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="923"/>
        <source>Zoom in</source>
        <translation>Zoom +</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="926"/>
        <source>Ctrl+=</source>
        <translation>Ctrl+=</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="950"/>
        <source>Zoom out</source>
        <translation>Zoom -</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="947"/>
        <source>Zoom -</source>
        <translation>Zoom -</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="727"/>
        <source>He&amp;lp</source>
        <translation>A&amp;yuda</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="754"/>
        <source>Pl&amp;ugins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="884"/>
        <source>&amp;New</source>
        <translation>&amp;Nuevo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="887"/>
        <source>New model</source>
        <translation type="unfinished">Nuevo modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="902"/>
        <source>&amp;Save</source>
        <translation type="unfinished">&amp;Guardar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="920"/>
        <source>&amp;Zoom in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="944"/>
        <source>Zoo&amp;m out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="953"/>
        <source>Ctrl+-</source>
        <translation>&gt;Ctrl+-</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="965"/>
        <source>Ctrl+O</source>
        <translation>Ctrl+O</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="992"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1010"/>
        <source>F4</source>
        <translation>F4</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1031"/>
        <source>Ctrl+P</source>
        <translation>Ctrl+P</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1049"/>
        <source>Ctrl+Z</source>
        <translation>Ctrl+Z</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1067"/>
        <source>Ctrl+Y</source>
        <translation>Ctrl+Y</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1082"/>
        <source>Export the current opened model in different modes</source>
        <translation>Exportar el modelo abierto actual en diferentes modos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1282"/>
        <source>&amp;Recent Models</source>
        <translation>&amp;Modelos Recientes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1297"/>
        <source>&amp;Import</source>
        <translation type="unfinished">&amp;Importar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1312"/>
        <source>Rest&amp;ore Session</source>
        <translation>&amp;Restaurar Sesión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1321"/>
        <source>&amp;Fix a model</source>
        <translation>Arreglar un modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1345"/>
        <source>&amp;Check for update</source>
        <translation>&amp;Buscar actualizaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1368"/>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1106"/>
        <source>Show grid</source>
        <translation>Mostrar cuadrícula</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1109"/>
        <source>Ctrl+G</source>
        <translation>Ctrl+G</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1124"/>
        <source>Close current model</source>
        <translation>Cerrar modelo actual</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1127"/>
        <source>Ctrl+W</source>
        <translation>Ctrl+W</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1142"/>
        <source>Ctrl+0</source>
        <translation>Ctrl+0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1163"/>
        <source>Align objects position to grid</source>
        <translation>Alinear la posición de los objetos con la cuadrícula</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1166"/>
        <source>Ctrl+H</source>
        <translation>Ctrl+H</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1187"/>
        <source>Show the page delimiters</source>
        <translation>Mostrar los delimitadores de la página</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1190"/>
        <source>Ctrl+L</source>
        <translation>Ctrl+L</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1202"/>
        <source>Edit pgModeler settings</source>
        <translation>Editar configuraciones de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1213"/>
        <source>Save all</source>
        <translation>Guardar todo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1231"/>
        <source>Show the model overview</source>
        <translation>Mostrar visión general del modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1234"/>
        <source>F10</source>
        <translation>F10</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1249"/>
        <source>F1</source>
        <translation>F1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1261"/>
        <source>New object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1270"/>
        <source>Plugins</source>
        <translation>Plugins</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1273"/>
        <source>Access the list of loaded plugins</source>
        <translation>Acceda a la lista de plugins cargados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1285"/>
        <source>Load recently opened model</source>
        <translation>Cargar modelos abiertos recientemente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1300"/>
        <source>Import existing database to new model (reverse engineering)</source>
        <translation>Importación de base de datos existente para nuevo modelo (ingeniería inversa)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1303"/>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Shift+I</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1333"/>
        <source>New version found!</source>
        <translation>Nueva versión encontrado!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1336"/>
        <source>Update for the current version is available on project&apos;s site</source>
        <translation>Actualización para la versión actual está disponible en el sitio del proyecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1354"/>
        <source>action_main_menu</source>
        <translation>action_main_menu</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1357"/>
        <source>Main menu</source>
        <translation>Menú principal</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1362"/>
        <source>Show expanded</source>
        <translation>Mostrar expandido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1365"/>
        <source>Expands the main menu bar in classical mode</source>
        <translation>Expande la barra de menú principal en el modo clásico</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1373"/>
        <source>Hide main menu</source>
        <translation>Ocultar menú principal</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1376"/>
        <source>Hides the main menu bar and put the action on a separated action</source>
        <translation>Oculta la barra de menú principal y poner la acción en una acción separada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1379"/>
        <source>Ctrl+Shift+H</source>
        <translation>Ctrl+Shift+H</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1397"/>
        <source>Ctrl+Shift+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1412"/>
        <source>Welcome</source>
        <translation>Bienvenidos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1415"/>
        <source>Welcome screen</source>
        <translation>Pantalla de bienvenida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1418"/>
        <source>Shift+W</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1433"/>
        <source>Design</source>
        <translation>Diseño</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1451"/>
        <source>Manage</source>
        <translation>Administrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1454"/>
        <source>Manage existent databases</source>
        <translation>Administrar base de datos existentes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="233"/>
        <source> (Demo)</source>
        <translation> (Demo)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="440"/>
        <source>Saving temp. models</source>
        <translation>Guardando modelos temp</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="772"/>
        <source>Clear Menu</source>
        <translation>Limpiar Menú</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="817"/>
        <source>The demonstration version can create only `one&apos; instance of database model!</source>
        <translation>La versión de demostración sólo puede crear `una&apos; instancia de modelo de base de datos!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="905"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1174"/>
        <source>Save model</source>
        <translation>Guardar Modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="599"/>
        <source>Save modified model(s)</source>
        <translation>Guardar modelo(s) modificado(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="600"/>
        <source>The following models were modified but not saved: %1. Do you really want to quit pgModeler?</source>
        <translation>Los siguientes modelos fueron modificados pero no guardados: %1. ¿De verdad quieres salir de pgModeler?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1175"/>
        <source>The model &lt;strong&gt;%1&lt;/strong&gt; was modified! Do you really want to close without save it?</source>
        <translation>Se ha modificado el modelo &lt;strong&gt;%1&lt;/strong&gt;. ¿Realmente quieres cerrar sin guardarlo?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1293"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1831"/>
        <source>Warning</source>
        <translation>Advertencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1294"/>
        <source>You&apos;re running a demonstration version! The model saving feature is available only in the full version!</source>
        <translation>Usted está ejecutando una versión de demostración! La función de ahorro de modelo está disponible sólo en la versión completa!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1308"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1386"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1440"/>
        <source>Confirmation</source>
        <translation>Confirmación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1309"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! It&apos;s recommended to validate it before save in order to create a consistent model otherwise the generated file will be broken demanding manual fixes to be loadable again!</source>
        <translation>&lt;strong&gt;ADVERTENCIA:&lt;/strong&gt; El modelo &lt;strong&gt;%1&lt;/strong&gt; está invalidado! Se recomienda para validarlo antes de guardar con el fin de crear un modelo coherente de lo contrario el archivo generado se romperá exigiendo correcciones manuales que sea cargable de nuevo!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1311"/>
        <source>Save anyway</source>
        <translation>Guardar de todos modos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1311"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1389"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1443"/>
        <source>Validate</source>
        <translation>Validar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1344"/>
        <source>Save &apos;%1&apos; as...</source>
        <translation>Guardar &apos;%1&apos; como...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1345"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1530"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modelo de base de datos (*.dbm);; Todos los arquivos (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1669"/>
        <source>Access support page</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1832"/>
        <source>You&apos;re running a demonstration version! Note that you&apos;ll be able to create only &lt;strong&gt;%1&lt;/strong&gt; instances 						of each type of object and some key features will be disabled or limited!&lt;br/&gt;&lt;br/&gt;You can purchase a full binary copy or get the source code at &lt;a href=&apos;http://pgmodeler.com.br&apos;&gt;pgmodeler.com.br&lt;/a&gt;.						&lt;strong&gt;NOTE:&lt;/strong&gt; pgModeler is an open source software, but purchasing binary copies or providing some donations will support the project and cover all development costs.&lt;br/&gt;&lt;br/&gt;						&lt;strong&gt;HINT:&lt;/strong&gt; in order to test all features it&apos;s recommended to use the &lt;strong&gt;demo.dbm&lt;/strong&gt; model located in &lt;/strong&gt;Sample models&lt;/strong&gt; at &lt;strong&gt;Welcome&lt;/strong&gt; view.&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1953"/>
        <source>Rearrange objects over the canvas is an irreversible operation! Would like to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1387"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! Before run the export process it&apos;s recommended to validate in order to correctly create the objects on database server!</source>
        <translation>&lt;strong&gt;ADVERTENCIA:&lt;/strong&gt; El modelo &lt;strong&gt;%1&lt;/strong&gt; está invalidado. Antes de ejecutar el proceso de exportación se recomienda validar para crear correctamente los objetos en el servidor de base de datos!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="49"/>
        <source>Grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="50"/>
        <source>Hierarchical</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="51"/>
        <source>Scattered</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1389"/>
        <source>Export anyway</source>
        <translation>Exportar de todas modos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1441"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! Before run the diff process it&apos;s recommended to validate in order to correctly analyze and generate the difference between the model and a database!</source>
        <translation>&lt;strong&gt;ADVERTENCIA:&lt;/strong&gt; El modelo &lt;strong&gt;%1&lt;/strong&gt; está invalidado! Antes ejecutar el proceso diff se recomienda para validar el fin de analizar correctamente y generar la diferencia entre el modelo y una base de datos!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1443"/>
        <source>Diff anyway</source>
        <translation>Diff de todas modo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1481"/>
        <source>Database model printing</source>
        <translation>Imprimir modelo de Base de datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1509"/>
        <source>Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click &apos;No&apos; or &apos;Cancel&apos; to abort printing.</source>
        <translation>Se detectaron cambios en las definiciones de papel / margen del modelo que pueden causar la impresión incorrecta de los objetos. ¿Quieres continuar con la impresión utilizando la nueva configuración? Para utilizar la configuración predeterminada, haga clic &apos;No&apos; o &apos;Cancel&apos; para abortar la impresión.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1532"/>
        <source>Load model</source>
        <translation>Cargar modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1577"/>
        <source>Could not load the database model file `%1&apos;. Check the error stack to see details. You can try to fix it in order to make it loadable again.</source>
        <translation>No se pudo cargar el archivo de modelo de base de datos `%1&apos;. Compruebe la pila de errores para ver los detalles. Usted puede tratar de arreglarlo con el fin de hacer que se puede cargar de nuevo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1579"/>
        <source>Fix model</source>
        <translation>Fijar modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1579"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1670"/>
        <source>This action will open a web browser window! Want to proceed?</source>
        <translation>Esta acción abrirá una ventana del navegador web! ¿Quieres continuar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1764"/>
        <source>(no samples found)</source>
        <translation>(no hay ejemplos encontrados)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1844"/>
        <source>save</source>
        <translation>guardar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1845"/>
        <source>export</source>
        <translation>exportar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1845"/>
        <source>diff</source>
        <translation>diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1848"/>
        <source>Executing pending &lt;strong&gt;%1&lt;/strong&gt; operation...</source>
        <translation>Ejecución de operarienes pendientes &lt;strong&gt;%1&lt;/strong&gt;...</translation>
    </message>
</context>
<context>
    <name>Messagebox</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="29"/>
        <source>Dialog</source>
        <translation>Diálogo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="176"/>
        <source>msg</source>
        <translation>msg</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="309"/>
        <source>Exceptions</source>
        <translation>Excepciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="392"/>
        <source>Show raw text errors or information.</source>
        <translation>Mostrar errores de texto crudos o información.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="421"/>
        <source>Show/hide exceptions stack.</source>
        <translation>Mostrar/ocultar pila excepciones.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="424"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="495"/>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="135"/>
        <source>&amp;Yes</source>
        <translation>&amp;Si</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="526"/>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="139"/>
        <source>&amp;No</source>
        <translation>&amp;No</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="551"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="135"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="142"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="153"/>
        <source>Error</source>
        <translation>Error</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="157"/>
        <source>Alert</source>
        <translation>Alerta</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="161"/>
        <source>Information</source>
        <translation>Información</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="165"/>
        <source>Confirmation</source>
        <translation>Confirmación</translation>
    </message>
</context>
<context>
    <name>MetadataHandlingForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="14"/>
        <source>Handle metadata</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="97"/>
        <source>&amp;Apply</source>
        <translation type="unfinished">&amp;Aplicar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="135"/>
        <source>&amp;Cancel</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="215"/>
        <source>Handle objects metadata</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="234"/>
        <source>Settings</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="252"/>
        <source>Extract from:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="336"/>
        <source>Loading a metadata file to the current model is an irreversible operation so be sure to specify a backup file before proceed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="355"/>
        <source>Options</source>
        <translation type="unfinished">Opciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="589"/>
        <source>Handles the following database model attributes in the metadata file: author, zoom factor, last position and default objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="592"/>
        <source>Database model metadata</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="384"/>
        <source>Handles the objects&apos; positioning in the metadata file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="387"/>
        <source>Objects&apos; positioning</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="445"/>
        <source>Handles the objects&apos; custom colors in the metadata file. Currently available only for relationships and schemas.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="448"/>
        <source>Custom object&apos;s colors</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="493"/>
        <source>Handles the objects&apos; protection status in the metadata file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="496"/>
        <source>Objects&apos; protection status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="541"/>
        <source>Handles the objects&apos; SQL disabled status in the metadata file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="544"/>
        <source>Objects&apos; SQL disabled status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="637"/>
        <source>Handles the objects&apos; fade out status in the metadata file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="640"/>
        <source>Objects&apos; fade out status</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="685"/>
        <source>Save tags to the output file when extracting metadata. When loading the file, the tags are recreated and duplicated ones are ignored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="733"/>
        <source>Save textboxes to the output file when extracting metadata. When loading the file, the textboxes are recreated and duplicated ones are ignored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="781"/>
        <source>Handles the objects&apos; custom SQL commands in the metadata file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="784"/>
        <source>Custom SQL commands</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="736"/>
        <source>Textbox objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="688"/>
        <source>Tag objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="829"/>
        <source>Handles the tables&apos; and views&apos; extended attributes display status in the metadata file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="832"/>
        <source>Tables&apos; extended attributes display</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="877"/>
        <source>Save generic SQL objects to the output file when extracting metadata. When loading the file, the objects are recreated and duplicated ones are ignored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="880"/>
        <source>Generic SQL objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="917"/>
        <source>Backup file:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="932"/>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="264"/>
        <source>Select file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="935"/>
        <source>...</source>
        <translation type="unfinished">...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="972"/>
        <source>Extracts the objects&apos; metadata from the loaded models and apply to the current focused model. A backup file can be specified to where the focused model&apos;s current metadata will be saved.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="975"/>
        <source>&amp;Extract and restore</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1029"/>
        <source>Extracts the objects metadata from one of the loaded models saving the info to a backup file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1032"/>
        <source>Extract &amp;only</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1086"/>
        <source>Reads the objects&apos; metadata from a previously saved backup file and apply to the current model.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1089"/>
        <source>&amp;Restore a backup file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1133"/>
        <source>Apply to:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="957"/>
        <source>Operation:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1148"/>
        <source>Output</source>
        <translation type="unfinished">Salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1237"/>
        <source>Progress label...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="115"/>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="126"/>
        <source>model not saved yet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="135"/>
        <source>The backup file cannot be the same as the input model!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="185"/>
        <source>Extracting metadata to file `%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="194"/>
        <source>Saving backup metadata to file `%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="211"/>
        <source>Applying metadata from file `%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="244"/>
        <source>Metadata processing aborted!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="263"/>
        <source>Objects metadata file (*.omf);;All files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ModelDatabaseDiffForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="152"/>
        <source>Settings</source>
        <translation>Configuraciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="302"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="405"/>
        <source>Connection:</source>
        <translation>Conexión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="680"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="271"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="337"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="440"/>
        <source>Database:</source>
        <translation>bases de datos:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1441"/>
        <source>Ignore import errors</source>
        <translation>Ignorar errores de importación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1351"/>
        <source>Import system objects</source>
        <translation>Importar objectos del sistema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1396"/>
        <source>Import extension objects</source>
        <translation>Importar extensión de objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1055"/>
        <source>For DROP command, the objects that depends on an object to be dropped will be deleted as well. For TRUNCATE command, tables that are linked to a table to be truncated will be truncate too. &lt;strong&gt;NOTE:&lt;/strong&gt; this option can affect more objects than listed in the output or diff preview.</source>
        <translation>Para comando DROP, los objetos que depende de un objeto que se retiren serán eliminados también. Para comando TRUNCATE, tablas que están vinculadas a una mesa para truncar se truncará también. &lt;strong&gt;NOTA:&lt;/strong&gt; esta opción puede afectar a varios objetos que se enumeran en la salida o diff vista previa.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1058"/>
        <source>Drop or truncate in cascade mode</source>
        <translation>Drop o truncate en cascada mode</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="869"/>
        <source>Permissions already set on database objects will be kept.The ones configured on the model will be applied to the database.</source>
        <translation>Permisos ya establecidos en los objetos de base de datos serán los kept.The configuradas en el modelo se puede aplicar a la base de datos.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="872"/>
        <source>Keep object&apos;s permissions</source>
        <translation>Mantenga los permisos del objeto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="827"/>
        <source>Database cluster level objects like roles and tablespaces will not be dropped.</source>
        <translation>No se cayeron objetos de base de datos a nivel de grupo como los roles y espacios de tabla.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="830"/>
        <source>Keep cluster objects</source>
        <translation>Mantenga objetos clúster</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1193"/>
        <source>Recreate only unmodifiable objects</source>
        <translation>Recrear objetos sólo no modificables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1250"/>
        <source>Force recreation of objects</source>
        <translation>Forzar recreación de objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1504"/>
        <source>Ignores errors generated by duplicated objects when exporting the diff to database.</source>
        <translation>Ignora errores generados por objetos duplicados cuando se exporta el diff de base de datos.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1507"/>
        <source>Ignore duplicity errors</source>
        <translation>Ignorar duplicidad de errores</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1142"/>
        <source>Serial columns are converted to integer and having the default value changed  to &lt;strong&gt;nextval(sequence)&lt;/strong&gt; function call. By default, a new sequence is created for each serial column but checking this option sequences matching the name on column&apos;s default value will be reused and will not be dropped.</source>
        <translation>Columnas de serie se convierten a entero y haber cambiado el valor predeterminado para llamada a la función &lt;strong&gt;nextval(sequence)&lt;/strong&gt;. De forma predeterminada, se crea una nueva secuencia para cada columna de serie, pero marcando esta opción secuencias que coinciden con el nombre en valor por defecto de la columna se volverá a utilizar y no se borró.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1145"/>
        <source>Reuse sequences on serial columns</source>
        <translation>Reciclar secuencias sobre columnas de serial</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="481"/>
        <source>Diff mode</source>
        <translation>Modo diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="501"/>
        <source>Override the PostgreSQL version when generating the diff. The default is to use the same version as the input database.</source>
        <translation>Anular la versión de PostgreSQL al generar el diff. El valor por defecto es usar la misma versión que la base de datos de entrada.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="504"/>
        <source>Use PostgreSQL:</source>
        <translation>Use PostgreSQL:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="572"/>
        <source>Compares the model and the input database storing the diff in a SQL file for later usage.</source>
        <translation>Compara el modelo y la base de datos de entrada almacenar el diff en un archivo SQL para un uso posterior.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="661"/>
        <source>File:</source>
        <translation>Archivo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="677"/>
        <source>Select output file</source>
        <translation>Seleccionar archivo de salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="730"/>
        <source>Compares the model and the input database generating a diff and applying it directly to the latter. &lt;strong&gt;WARNING:&lt;/strong&gt; this mode causes irreversible changes on the database and in case of failure the original structure is not restored, so make sure to have a backup before proceed.</source>
        <translation>Compara el modelo y la base de datos de entrada generar un diff y aplicándolo directamente a éste. &lt; strong&gt;ADVERTENCIA:&lt;/strong&gt; este modo provoca cambios irreversibles en la base de datos y en caso de fallo de la estructura original no se restablece, así que asegúrese de tener una copia de seguridad antes de proceder.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1438"/>
        <source>Ignores as many as possible errors on import step. This option generates an incomplete diff.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1100"/>
        <source>Clears the data of all tables which will have columns modified. This is useful to avoid errors related to type casting. &lt;strong&gt;WARNING:&lt;/strong&gt; DO NOT use this option on production servers and always make a backup before use it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1348"/>
        <source>Import system (built-in) objects. Use this if the import step is returning errors related to missing objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1393"/>
        <source>Import objects created by extensions. Use this if the import step is returning errors even importing built in ones.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1247"/>
        <source>Instead of use an ALTER command to modify certain kind of objects a DROP and CREATE will be used in order to do a full modification. This option does not affects the database object.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1010"/>
        <source>No command to rename the destination database will be generated even the model&apos;s name differ from database name.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1013"/>
        <source>Preserve database name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="911"/>
        <source>Avoid the generation of DROP commands for objects that exists in database but not in the model. This is useful when diff a partial model against the complete database.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="914"/>
        <source>Do not drop missing objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="575"/>
        <source>Store in S&amp;QL file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="35"/>
        <source>Diff tool</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="133"/>
        <source>Generate diff code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="195"/>
        <source>Source database</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="230"/>
        <source>Current model:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="253"/>
        <source>(model)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="387"/>
        <source>Compare to</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="733"/>
        <source>Appl&amp;y on server</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="804"/>
        <source>Diff</source>
        <translation type="unfinished">Diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="968"/>
        <source>Froce the generation of DROP commands for columns and constraints that exist in database but not in the model. This is useful when diff a partial model against the complete database and the user needs to drop columns and constraint but preserve the rest of the objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="971"/>
        <source>Drop missing columns and constraints</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1103"/>
        <source>Truncate tables before alter columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1301"/>
        <source>Import &amp;&amp; Export</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1319"/>
        <source>Import</source>
        <translation type="unfinished">Importar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1475"/>
        <source>Export</source>
        <translation type="unfinished">Exportar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1571"/>
        <source>This advanced option causes pgModeler to ignore extra errors by their numeric codes. These errors must be informed in the input below and separeted by space. For the complete list of error codes check the PostgreSQL docs, section &lt;strong&gt; Appendix A. PostgreSQL Error Codes&lt;/strong&gt;. &lt;strong&gt;WARNING:&lt;/strong&gt; use this option with extreme care since it can interfere in final export result.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1574"/>
        <source>Ignore error codes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1615"/>
        <source>Output</source>
        <translation>Salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1699"/>
        <source>Changes:</source>
        <translation>Cambios:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1776"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1844"/>
        <source>Progress label...</source>
        <translation>Etiqueta de progreso...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1857"/>
        <source>Step label...</source>
        <translation>Etiqueta de pasos...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1938"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Objects marked with an &lt;span style=&quot; font-weight:600;&quot;&gt;ALTER&lt;/span&gt; may not be effectively changed unless that the differences detected are in attributes that can be modified through ALTER commands otherwise no operationwill be performed or, if the force recreation is checked, the object will be dropped and created again.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Objetos marcados con un &lt;span style=&quot; font-weight:600;&quot;&gt;ALTER&lt;/span&gt; No se puede cambiar de manera efectiva a menos que las diferencias detectadas son en los atributos que se pueden modificar a través de comandos ALTER de lo contrario no se realizará ninguna operationwill o, si la fuerza de la recreación está marcada, el objeto será dado de baja y creó de nuevo.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1990"/>
        <source>Objects to be created</source>
        <translation>Objetos que se creen</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1993"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2034"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2075"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2116"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2031"/>
        <source>Objects to be dropped</source>
        <translation>Objetos que se borran</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2072"/>
        <source>Possible objects to be changed</source>
        <translation>Objetos posibles de ser cambiados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2113"/>
        <source>Ignored objects (system ones or with sql disabled)</source>
        <translation>Objetos ignorados (los del sistema o con sql disabilitado)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2159"/>
        <source>Diff Preview</source>
        <translation>Vista previa Diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2205"/>
        <source>&amp;Apply diff</source>
        <translation>&amp;Aplicar diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2284"/>
        <source>&amp;Generate</source>
        <translation>&amp;Generar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2338"/>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="333"/>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="335"/>
        <source>Waiting process to start...</source>
        <translation>Proceso en espera para iniciar...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="541"/>
        <source>Confirmation</source>
        <translation>Confirmación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="542"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The generated diff is ready to be exported! Once started this process will cause irreversible changes on the database. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ADVERTENCIA:&lt;/strong&gt; El diff generada está listo para ser exportado! Una vez iniciado este proceso provocará cambios irreversibles en la base de datos. ¿Realmente desea continuar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="544"/>
        <source>Apply diff</source>
        <translation>Aplicar diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="544"/>
        <source>Preview diff</source>
        <translation>Vista previa Diff</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="165"/>
        <source>model not saved yet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="169"/>
        <source>(none)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="448"/>
        <source>Step %1/%2: Importing database &lt;strong&gt;%3&lt;/strong&gt;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="501"/>
        <source>Step %1/%2: Comparing &lt;strong&gt;%3&lt;/strong&gt; and &lt;strong&gt;%4&lt;/strong&gt;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="552"/>
        <source>Step %1/%2: Exporting diff to database &lt;strong&gt;%3&lt;/strong&gt;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="580"/>
        <source>Diff process paused. Waiting user action...</source>
        <translation>Proceso Diff en pausa. Esperando acción del usuario...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="615"/>
        <source>Saving diff to file &lt;strong&gt;%1&lt;/strong&gt;</source>
        <translation>Guardando diff a archivo &lt;strong&gt;%1&lt;/strong&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="638"/>
        <source>Diff process sucessfully ended!</source>
        <translation>Proceso Diff finalizado satisfactoriamente!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="639"/>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="654"/>
        <source>No operations left.</source>
        <translation>No hay operaciones a la izquierda.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="653"/>
        <source>Operation cancelled by the user.</source>
        <translation>Operación cancelada por el usuario.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="695"/>
        <source>Process aborted due to errors!</source>
        <translation>Proceso abortado debido a errores!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="738"/>
        <source>

-- SQL code purposely truncated at this point in demo version!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="754"/>
        <source>-- No differences were detected between model and database. --</source>
        <translation>--  No se detectaron diferencias entre el modelo y la base de datos. --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="769"/>
        <source>Error code &lt;strong&gt;%1&lt;/strong&gt; found and ignored. Proceeding with export.</source>
        <translation>Error de código encontrado &lt;strong&gt;%1&lt;/strong&gt; y ignorado. Procediendo con la exportación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="877"/>
        <source>Save diff as...</source>
        <translation>Salvar diff como...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="881"/>
        <source>SQL code (*.sql);;All files (*.*)</source>
        <translation>Código SQL (*.sql);;Todos los archivos (*.*)</translation>
    </message>
</context>
<context>
    <name>ModelExportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="35"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="133"/>
        <source>Export model</source>
        <translation>Exportar Modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="152"/>
        <source>Settings</source>
        <translation>Configuración</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="176"/>
        <source>Database server</source>
        <translation>Servidor de Bases de datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="349"/>
        <source>pgModeler ignores errors generated by duplicated objects and creates only that ones which does not exists in the database. This option may be used when an object was created after a previous model export.</source>
        <translation>El pgModeler ignora los errores generados por objetos duplicados y sólo crea objetos del modelo que no existen en la base de datos. Esta opción se puede usar cuando un objeto fue creado después de una exportación de modelo posterior.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="352"/>
        <source>Ignore object duplicity</source>
        <translation>Ignorar duplicidad de objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="306"/>
        <source>Connection:</source>
        <translation>Conexión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="392"/>
        <source>PostgreSQL version in which the SQL code should be generated. It is recommended to select this option only when the version of the DBMS, somehow, is not identifiable or if you need to generate a specific version of SQL code for test purposes.</source>
        <translation>PostgreSQL versión en la que se debe generar el código SQL. Se recomienda seleccionar esta opción sólo cuando la versión del SGDB, de alguna manera, no es identificable o si usted necesita para generar una versión específica de código SQL con fines de prueba.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="395"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="794"/>
        <source>PostgreSQL:</source>
        <translation>PostgreSQL:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="514"/>
        <source>This advanced option causes pgModeler to ignore extra errors by their numeric codes. These errors must be informed in the input below and separeted by space. For the complete list of error codes check the PostgreSQL docs, section &lt;strong&gt; Appendix A. PostgreSQL Error Codes&lt;/strong&gt;. &lt;strong&gt;WARNING:&lt;/strong&gt; use this option with extreme care since it can interfere in final export result.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="517"/>
        <source>Ignore error codes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="547"/>
        <source>If &lt;strong&gt;DB&lt;/strong&gt; is checked pgModeler will destroy the database if already exists on the server. When &lt;strong&gt;Objects&lt;/strong&gt; is checked pgModeler will execute the DROP command attached to SQL-enabled objects. &lt;strong&gt;WARNING:&lt;/strong&gt; this option leads to data loss so make sure to have a backup first.</source>
        <translation>Si &lt;strong&gt;BD&lt;/strong&gt; se comprueba pgModeler destruirá la base de datos si ya existe en el servidor. Cuando &lt;strong&gt;Objetos&lt;/strong&gt; se comprueba pgModeler ejecutará el comando DROP unido a objetos de SQL-habilitado. &lt;strong&gt;ADVERTENCIA:&lt;/strong&gt; esta opción conduce a la pérdida de datos, así que asegúrese de tener una copia de seguridad primero.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="550"/>
        <source>Drop:</source>
        <translation>Borrar:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="560"/>
        <source>DB</source>
        <translation>BD</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="598"/>
        <source>pgModeler will destroy the database if already exists on the server. Make sure to have a backup before use this option because all data will be lost.</source>
        <translation>pgModeler destruirá la base de datos si ya existe en el servidor. Asegúrese de tener una copia de seguridad antes de usar esta opción porque se perderán todos los datos.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1140"/>
        <source>Zoom:</source>
        <translation>Zoom:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1171"/>
        <source>Show grid</source>
        <translation>Mostrar cuadrícula</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1187"/>
        <source>Show delimiters</source>
        <translation>Mostrar delimitadores</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1203"/>
        <source>Exporting the model page by page will generate files with a &lt;strong&gt;_p[n]&lt;/strong&gt; suffix where &lt;strong&gt;n&lt;/strong&gt; is the page id. Check if the current user has write permission on output folder.</source>
        <translation>Exportación de la página modelo por página generará archivos con un &lt;strong&gt;_p[n]&lt;/strong&gt; sufijo donde &lt;strong&gt;n&lt;/strong&gt; es el identificador de página. Compruebe si el usuario actual tiene permiso de escritura en la carpeta de salida.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1206"/>
        <source>Page by page</source>
        <translation>Página por página</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="755"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1098"/>
        <source>Select target file</source>
        <translation>Seleccionar archivo de destino</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="758"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1101"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="648"/>
        <source>SQL file</source>
        <translation>Archivo SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="724"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1067"/>
        <source>File:</source>
        <translation>Archivo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="573"/>
        <source>Ob&amp;jects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="889"/>
        <source>Graphics file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1022"/>
        <source>Type:</source>
        <translation type="unfinished">Tipo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="818"/>
        <source>PostgreSQL version in which the SQL code should be generated</source>
        <translation>PostgreSQL versión en la que se debe generar el código SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1029"/>
        <source>I&amp;mage (PNG)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1045"/>
        <source>&amp;Vectorial (SVG)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1289"/>
        <source>Output</source>
        <translation>Salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1375"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1401"/>
        <source>Progress label...</source>
        <translation>Etiqueta de progreso...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1515"/>
        <source>&amp;Export</source>
        <translation>&amp;Exportar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1553"/>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="126"/>
        <source>Error code &lt;strong&gt;%1&lt;/strong&gt; found and ignored. Proceeding with export.</source>
        <translation>Error de código encontrado en &lt;strong&gt;%1&lt;/strong&gt; y ignorado. Procediendo con la exportación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="191"/>
        <source>Initializing model export...</source>
        <translation>Inicializando exportación del modelo...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="196"/>
        <source>Saving file &apos;%1&apos;</source>
        <translation>Guardar archivo &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="225"/>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="303"/>
        <source>Exporting process aborted!</source>
        <translation>Porceso de exportación abortado!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="254"/>
        <source>Export model as...</source>
        <translation>Exportar modelo como...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="262"/>
        <source>SQL script (*.sql);;All files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="269"/>
        <source>Portable Network Graphics (*.png);;All files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="274"/>
        <source>Scalable Vector Graphics (*.svg);;All files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="317"/>
        <source>Exporting process canceled by user!</source>
        <translation>Proceso de exportación cancelada por el usuario!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="327"/>
        <source>Exporting process sucessfuly ended!</source>
        <translation>Proceso de exportación terminó satisfactoriamente! </translation>
    </message>
</context>
<context>
    <name>ModelExportHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="74"/>
        <source>Generating SQL code for PostgreSQL `%1&apos;</source>
        <translation>Generación de código SQL para PostgreSQL `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="79"/>
        <source>Output SQL file `%1&apos; successfully written.</source>
        <translation>Archivo SQL de salida `%1&apos; escrito satisfactoriamente.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="201"/>
        <source>Rendering objects to page %1/%2.</source>
        <translation>Renderiando objetos a la página %1/%2.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="228"/>
        <source>Output image `%1&apos; successfully written.</source>
        <translation>Imagen de salida `%1&apos; escrito satisfactoriamente.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="261"/>
        <source>Exporting model to SVG file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="264"/>
        <source>SVG representation of database model</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="265"/>
        <source>SVG file generated by pgModeler</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="307"/>
        <source>Output file `%1&apos; successfully written.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="347"/>
        <source>Starting export to DBMS.</source>
        <translation>Comenzando exportación a SGDB.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="353"/>
        <source>PostgreSQL version detection overridden. Using version `%1&apos;.</source>
        <translation>Versión detección PostgreSQL anulado. Al usar la versión. Usar la versión `%1&apos;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="358"/>
        <source>PostgreSQL `%1&apos; server detected.</source>
        <translation>Servidor PostgreSQL `%1&apos; detectado.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="363"/>
        <source>Generating temporary names for database, roles and tablespaces.</source>
        <translation>Generar nombres temporales para bases de datos, roles y tablespaces.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="374"/>
        <source>Enabling the SQL code for database `%1&apos; to avoid errors.</source>
        <translation>Habilitar el código SQL para la base de datos `%1&apos; para evitar errores.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="380"/>
        <source>Ignoring object duplication errors.</source>
        <translation>Ignorando los errores de duplicación de objetos.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="387"/>
        <source>Ignoring the following error code(s): `%1&apos;.</source>
        <translation>Ignorando el código de error siguiente (s): `%1&apos;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="391"/>
        <source>Trying to drop database `%1&apos;.</source>
        <translation>Tratando de borrarr la base de datos `%1&apos;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="407"/>
        <source>Simulation mode activated.</source>
        <translation>Modo de simulación activado.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="511"/>
        <source>Generating SQL for `%1&apos; objects...</source>
        <translation>Generación de SQL para los objetos `%1&apos;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="640"/>
        <source>Destroying objects created on the server.</source>
        <translation>Destruyendo los objetos creados en el servidor.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="666"/>
        <source>Restoring original names of database, roles and tablespaces.</source>
        <translation>Restauración de los nombres originales de base de datos, roles y tablespaces.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="425"/>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="876"/>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="957"/>
        <source>Creating object `%1&apos; (%2)</source>
        <translation>Creando objecto `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="470"/>
        <source>Creating database `%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="504"/>
        <source>Connecting to database `%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="716"/>
        <source>Renaming `%1&apos; (%2) to `%3&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="874"/>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="959"/>
        <source>Dropping object `%1&apos; (%2)</source>
        <translation>Borrando objetos `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="961"/>
        <source>Changing object `%1&apos; (%2)</source>
        <translation>Cambiando objecto `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="974"/>
        <source>Running auxiliary command.</source>
        <translation>Ejecución de comandos auxiliar.</translation>
    </message>
</context>
<context>
    <name>ModelFixForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="35"/>
        <source>Model file fix</source>
        <translation>Fijar archivo Modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="475"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Monospace&apos;; font-size:10pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:9pt;&quot;&gt;Waiting process to start...&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="727"/>
        <source>&amp;Fix</source>
        <translation>&amp;Fijar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="765"/>
        <source>&amp;Close</source>
        <translation>&amp;Cerrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="124"/>
        <source>Fix model file</source>
        <translation>Fijar archivo Modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="226"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;[pgmodeler-cli not found error]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;[pgmodeler-error no encontrado]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="245"/>
        <source>pgmodeler-cli:</source>
        <translation>pgmodeler-cli:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="263"/>
        <source>Browse for pgmodeler-cli tool</source>
        <translation>Navegue por herramienta pgmodeler-cli</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="266"/>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="503"/>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="533"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="325"/>
        <source>The specified file is not the pgModeler command line tool (pgmodeler-cli).</source>
        <translation>El archivo especificado no es la herramienta de línea de comandos pgModeler (pgmodeler-cli).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="340"/>
        <source>Input file:</source>
        <translation>Archivo entrada:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="353"/>
        <source>Output file:</source>
        <translation>Archivo salida:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="360"/>
        <source>Fix tries:</source>
        <translation>Fijar intentos:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="500"/>
        <source>Select input file</source>
        <translation>Seleccionar archivo entrada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="530"/>
        <source>Select output file</source>
        <translation>Seleccionar archivo salida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="570"/>
        <source>Load fixed model when finish</source>
        <translation>Modelo fijo de carga cuando acabado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="659"/>
        <source>In some cases the fix process will fail to restore all objects within the model demanding manual fixes by changing the file on a text editor. &lt;strong&gt;NOTE:&lt;/strong&gt; relationships may lost their graphical configuration like custom points and line color.</source>
        <translation>En algunos casos, el Proceso Fijar no podrá restaurar todos los objetos dentro del modelo exigir correcciones manuales cambiando el archivo en un editor de texto. &lt;strong&gt;NOTA:&lt;/strong&gt; relaciones pueden perdido su configuración gráfica como puntos de medida y color de la línea.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelfixform.cpp" line="69"/>
        <source>Waiting process to start...</source>
        <translation>Proceso esperandi para iniciar...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelfixform.cpp" line="80"/>
        <source>Could not locate &lt;strong&gt;%1&lt;/strong&gt; tool on &lt;strong&gt;%2&lt;/strong&gt;. The fix process can&apos;t continue! Please check pgModeler installation or try to manually specify the command below.</source>
        <translation>No se pudo encontrar &lt;strong&gt;%1&lt;/strong&gt; herramienta en &lt;strong&gt;%2&lt;/strong&gt;. El proceso de corrección no puede continuar! Por favor verifique la instalación pgModeler o tratar de especificar manualmente el comando a continuación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelfixform.cpp" line="151"/>
        <source>pgModeler command line tool (%1)</source>
        <translation>pgModeler herramienta de línea de comandos (%1)</translation>
    </message>
</context>
<context>
    <name>ModelNavigationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="23"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="59"/>
        <source>Previous model</source>
        <translation>Modelo anterior</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="75"/>
        <source>Ctrl+Left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="116"/>
        <source>Ctrl+Right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="176"/>
        <source>Alt+C</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="100"/>
        <source>Next model</source>
        <translation>Siguiente modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="160"/>
        <source>Close model</source>
        <translation>Cerrar modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="163"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelnavigationwidget.cpp" line="73"/>
        <source>(model not saved yet)</source>
        <translation>(modelo no guarda aún)</translation>
    </message>
</context>
<context>
    <name>ModelObjectsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="35"/>
        <source>Model Objects</source>
        <translation>Modelo Objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="388"/>
        <source>Visible object types</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="567"/>
        <source>Hide this widget</source>
        <translation>Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="570"/>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="750"/>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="788"/>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="823"/>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="861"/>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="896"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="226"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="311"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="323"/>
        <source>Object</source>
        <translation>Objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="335"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="347"/>
        <source>Parent Object</source>
        <translation>Objeto Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="359"/>
        <source>Parent Type</source>
        <translation>Tipo del Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="461"/>
        <source>Select All</source>
        <translation>Marcar Todos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="474"/>
        <source>Clear All</source>
        <translation>Desmarcar Todos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="535"/>
        <source>Model objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="674"/>
        <source>Select</source>
        <translation>Selecionar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="690"/>
        <source>Return</source>
        <translation>Retorno</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="709"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="725"/>
        <source>Esc</source>
        <translation>Esc</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="744"/>
        <source>Tree view</source>
        <translation>Vista en árbol</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="785"/>
        <source>List view</source>
        <translation>Vista en lista</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="820"/>
        <source>Objects view configuration</source>
        <translation>Configuración de la Vista de objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="858"/>
        <source>Expands all items</source>
        <translation>Expande todos los items</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="893"/>
        <source>Collapses all items</source>
        <translation>Contrae todos los items</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="617"/>
        <source>Filter:</source>
        <translation>Filtrar:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="634"/>
        <source>By ID</source>
        <translation>Por ID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelobjectswidget.cpp" line="166"/>
        <source>New</source>
        <translation>Nuevo</translation>
    </message>
</context>
<context>
    <name>ModelOverviewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeloverviewwidget.ui" line="41"/>
        <source>Model overview</source>
        <translation>Vista general del modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeloverviewwidget.cpp" line="126"/>
        <source>Failed to generate the overview image.
The requested size %1 x %2 was too big and there was not enough memory to allocate!</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ModelRestorationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="35"/>
        <source>Model restoration</source>
        <translation>Restauración de modelos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="214"/>
        <source>pgModeler was not closed properly in a previous execution and some models were still being edited. Click &lt;strong&gt;Restore&lt;/strong&gt; to reopen the models or &lt;strong&gt;Cancel&lt;/strong&gt; to abort the restoration.</source>
        <translation>pgModeler no se cerró correctamente en una ejecución anterior y todavía se está editando algunos modelos. Haga clic en &apos;Restaurar&apos; para reabrir los modelos o &apos;Cancelar&apos; para abortar la restauración.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="238"/>
        <source>pgModeler will try to recover the selected models but will not destroy them in case of loading failure. This option serves as a last resort in order to try to recover the database model. Temporary models will last until the application is closed so the user must try to manually recover the files before exit pgModeler.</source>
        <translation>pgModeler tratará de recuperar los modelos seleccionados, pero no destruirlos en caso de fallo de carga. Esta opción sirve como último recurso con el fin de tratar de recuperar el modelo de base de datos. Modelos temporales durarán hasta que la aplicación está cerrada por lo que el usuario debe intentar recuperar manualmente los archivos antes de la salida pgModeler.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="241"/>
        <source>Keep temporary models in case of restoration failure</source>
        <translation>Mantenga modelos temporales en caso de fallo de restauración</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="294"/>
        <source>Database</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="306"/>
        <source>File</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="318"/>
        <source>Modified</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="330"/>
        <source>Size</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="362"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="78"/>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurar</translation>
    </message>
</context>
<context>
    <name>ModelValidationHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationhelper.cpp" line="586"/>
        <source>There are pending errors! SQL validation will not be executed.</source>
        <translation>Hay errores pendientes! No se ejecutará la validación de SQL.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationhelper.cpp" line="670"/>
        <source>Operation canceled by the user.</source>
        <translation>Operación cancelada por el usuario.</translation>
    </message>
</context>
<context>
    <name>ModelValidationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="32"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="232"/>
        <source>Enables the validation of SQL code in DBMS. This process requires the use of a pre-configured connection. SQL validation will occur only in the last step (when all objects were validated) or when there are no warnings.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="266"/>
        <source>Connection to be used in the SQL validation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="354"/>
        <source>pgModeler will generate unique and temporary names for database, role and tablespace objects. This option avoids object duplication errors when running the SQL validation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="357"/>
        <source>Use unique temporary names for cluster level objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="498"/>
        <source>Warnings: does not prevents model to be saved.</source>
        <translation>Advertencias: no previene modelo para ser salvos.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="523"/>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="611"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="583"/>
        <source>Errors: model will not be saved while there are validation errors.</source>
        <translation>Errores: modelo no se guardará mientras que hay errores de validación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="686"/>
        <source>Hide this widget</source>
        <translation>Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="689"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="756"/>
        <source>Swap ids</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="235"/>
        <source>SQL Validation:</source>
        <translation>Validación de SQL:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="332"/>
        <source>PostgreSQL version</source>
        <translation>Versión PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="718"/>
        <source>Try to resolve the reported issues.</source>
        <translation>Trate de resolver los problemas comunicados.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="734"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="132"/>
        <source>Clear validation results</source>
        <translation>Limpiar resultados de la validación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="135"/>
        <source>Clear</source>
        <translation>Limpiar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="97"/>
        <source>Try to apply a fix on the selected validation info.</source>
        <translation>Trate de aplicar una solución en la información de validación seleccionado.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="100"/>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="376"/>
        <source>Cancel the SQL validation in progress.</source>
        <translation>Cancelar la validación de SQL en curso.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="379"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="392"/>
        <source>Esc</source>
        <translation>Esc</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="721"/>
        <source>Apply fixes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="753"/>
        <source>Change the creation order for two objects by swapping their ids</source>
        <translation>Cambiar el orden de la creación de dos objetos mediante el canje de sus ids</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="62"/>
        <source>Va&amp;lidate</source>
        <translation>Va&amp;lidar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="32"/>
        <source>Autodetect</source>
        <translation>Autodetectar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="223"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] is being referenced by &lt;strong&gt;%4&lt;/strong&gt; object(s) before its creation.</source>
        <translation>El objeto &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id:% 3] está siendo referenciado por &lt;strong&gt;%4&lt;/strong&gt; objeto(s) antes de su creación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="238"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3]%4 is referencing columns created by &lt;strong&gt;%5&lt;/strong&gt; relationship(s) but is created before them.</source>
        <translation>El objeto &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; se hace referencia a las columnas creadas por &lt;strong&gt;%5&lt;/strong&gt; relación (es), pero se crea ante ellos.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="257"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; has a name that conflicts with &lt;strong&gt;%3&lt;/strong&gt; object&apos;s name(s).</source>
        <translation>El objeto &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; tiene un nombre que entre en conflicto con &lt;strong&gt;%3&lt;/strong&gt; nombre(s) de objteto(s).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="264"/>
        <source>The relationship &lt;strong&gt;%1&lt;/strong&gt; [id: %2] is in a permanent invalidation state and needs to be relocated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="273"/>
        <source>The column &lt;strong&gt;%1&lt;/strong&gt; on &lt;strong&gt;%2&lt;/strong&gt; &lt;em&gt;(%3)&lt;/em&gt; is referencing the geospatial data type &lt;strong&gt;%4&lt;/strong&gt; but the &lt;strong&gt;postgis&lt;/strong&gt; extension is not present in the model!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="316"/>
        <source>&lt;strong&gt;HINT:&lt;/strong&gt; try to swap the relationship by another ones that somehow are linked to it through generated columns or constraints to solve this issue. Note that other objects may be lost in the swap process.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="321"/>
        <source>&lt;strong&gt;HINT:&lt;/strong&gt; Create the extension in the model or let it be created by applying the needed fixes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="439"/>
        <source>SQL validation not executed! No connection defined.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="446"/>
        <source>Database model successfully validated.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="268"/>
        <source>SQL validation failed due to error(s) below. &lt;strong&gt;NOTE:&lt;/strong&gt;&lt;em&gt; These errors does not invalidates the model but may affect operations like &lt;strong&gt;export&lt;/strong&gt; and &lt;strong&gt;diff&lt;/strong&gt;.&lt;/em&gt;</source>
        <translation>Validación de SQL ha fallado debido a un error (s) a continuación. &lt;strong&gt;NOTA:&lt;/strong&gt;&lt;em&gt; Estos errores no invalida el modelo, pero pueden afectar a las operaciones como &lt;strong&gt;export&lt;/strong&gt; and &lt;strong&gt;diff&lt;/strong&gt;.&lt;/em&gt; </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="354"/>
        <source>&lt;em&gt;The above object was created by a relationship. Change the name pattern on it&apos;s generator relationship. Fix will not be applied!&lt;/em&gt;</source>
        <translation>&lt;em&gt;El objetivo anterior fue creado por una relación. Cambiar el patrón de nombre en él la relación de generador. Fijar no se aplicará!&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="360"/>
        <source>Conflicting object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;.</source>
        <translation>Objeto en conflicto: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="367"/>
        <source>Relationship: &lt;strong&gt;%1&lt;/strong&gt; [id: %2].</source>
        <translation>Relación: &lt;strong&gt;%1&lt;/strong&gt; [id: %2].</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="372"/>
        <source>Referrer object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</source>
        <translation>Objeto referente:: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="472"/>
        <source>Running SQL commands on server...</source>
        <translation>Ejecución de comandos SQL en el servidor...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="484"/>
        <source>Processing object: %1</source>
        <translation>Procesando object: %1</translation>
    </message>
</context>
<context>
    <name>ModelWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="83"/>
        <source>One to One (1-1)</source>
        <translation>Uno a Uno (1-1)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="83"/>
        <source>One to Many (1-n)</source>
        <translation>Uno a Muchos (1-n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="84"/>
        <source>Many to Many (n-n)</source>
        <translation>Mucho a Mucho (n-n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="84"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="241"/>
        <source>Copy</source>
        <translation>Copiar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="84"/>
        <source>Inheritance</source>
        <translation type="unfinished">Herencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="138"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; The database model is protected! Operations that could modify it are disabled!</source>
        <translation>&lt;strong&gt;ATENCIÓN:&lt;/strong&gt; El modelo de base de datos está protegida! Operaciones que podrían modificar la misma son habilitado!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="215"/>
        <source>Source</source>
        <translation>Fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="216"/>
        <source>Alt+S</source>
        <translation>Alt+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="217"/>
        <source>Show object source code</source>
        <translation>Exibir el código-Fuente del objeto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="219"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3862"/>
        <source>Properties</source>
        <translation>Propiedades</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="220"/>
        <source>Space</source>
        <translation>Espacio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="221"/>
        <source>Edit the object properties</source>
        <translation>Edita las propriedades del objeto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="223"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3891"/>
        <source>Protect</source>
        <translation>Proteger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="224"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3886"/>
        <source>Unprotect</source>
        <translation>Desproteger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="225"/>
        <source>Protects object(s) from modifications</source>
        <translation>Protege objeto(s) a partir de las modificaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="227"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3898"/>
        <source>Delete</source>
        <translation>Eliminar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="228"/>
        <source>Del</source>
        <translation>Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="231"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3905"/>
        <source>Del. cascade</source>
        <translation>Del. cascada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="232"/>
        <source>Shift+Del</source>
        <translation>Shift+Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="235"/>
        <source>Select all</source>
        <translation>Seleccionar todos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="281"/>
        <source>Select</source>
        <translation type="unfinished">Selecionar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="401"/>
        <source>Ctrl+A</source>
        <translation>Ctrl+A</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="236"/>
        <source>Selects all the graphical objects in the model</source>
        <translation>Selecciona todos los objetos gráficos en el modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="213"/>
        <source>Edit data</source>
        <translation type="unfinished">Editar datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="239"/>
        <source>Convert</source>
        <translation>Convertir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="242"/>
        <source>Ctrl+C</source>
        <translation>Ctrl+C</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="245"/>
        <source>Paste</source>
        <translation>Pegar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="246"/>
        <source>Ctrl+V</source>
        <translation>Ctrl+V</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="249"/>
        <source>Cut</source>
        <translation>Cortar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="250"/>
        <source>Ctrl+X</source>
        <translation>Ctrl+X</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="253"/>
        <source>Deps &amp;&amp; Referrers</source>
        <translation>Deps &amp;&amp; Referreridos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="255"/>
        <source>New</source>
        <translation>Nuevo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="256"/>
        <source>Add a new object in the model</source>
        <translation>Adicionar un nuevo objeto en el modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="258"/>
        <source>Quick</source>
        <translation>Rápido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="259"/>
        <source>Quick action for the selected object</source>
        <translation>Acciones rápidas para la selección de objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="262"/>
        <source>Rename</source>
        <translation>Renombrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="263"/>
        <source>F2</source>
        <translation>F2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="264"/>
        <source>Quick rename the object</source>
        <translation>Renombrar rápida del objeto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="266"/>
        <source>Move to schema</source>
        <translation>Mover al esquema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="269"/>
        <source>Set tag</source>
        <translation>Configurar tag</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="272"/>
        <source>Edit permissions</source>
        <translation>Editar permisos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="273"/>
        <source>Ctrl+E</source>
        <translation>Ctrl+E</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="275"/>
        <source>Change owner</source>
        <translation>Cambiar propietario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="278"/>
        <source>Select children</source>
        <translation>Seleccione los hijos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="279"/>
        <source>Select tagged</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="282"/>
        <source>Open relationship</source>
        <translation>Abrir relación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="284"/>
        <source>Custom SQL</source>
        <translation>Personalizar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="285"/>
        <source>Alt+Q</source>
        <translation>Alt+Q</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="287"/>
        <source>Convert to sequence</source>
        <translation>Converir a secuencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="288"/>
        <source>Convert to serial</source>
        <translation>Converir a serial</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="290"/>
        <source>Break line</source>
        <translation>Saltos de líena</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="292"/>
        <source>Remove points</source>
        <translation>Remover puntos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="294"/>
        <source>Enable SQL</source>
        <translation>Habilitar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="295"/>
        <source>Disable SQL</source>
        <translation>Deshabilitar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="297"/>
        <source>Duplicate</source>
        <translation>Duplicar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="298"/>
        <source>Ctrl+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="301"/>
        <source>Extended attributes</source>
        <translation>Atributos extendidos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="302"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="313"/>
        <source>Show</source>
        <translation>Mostrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="303"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="314"/>
        <source>Hide</source>
        <translation>Ocultar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="305"/>
        <source>Jump to table</source>
        <translation>Ir a tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="312"/>
        <source>Schemas rectangles</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="319"/>
        <source>Fade in/out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="320"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="323"/>
        <source>Fade in</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="321"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="324"/>
        <source>Fade out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="329"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Relationships</source>
        <translation type="unfinished">Relaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="336"/>
        <source>Swap ids</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="337"/>
        <source>Edit the objects creation order by swapping their ids</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="340"/>
        <source>90° (vertical)</source>
        <translation>90° (vertical)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="345"/>
        <source>90° (horizontal)</source>
        <translation>90° (horizontal)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="350"/>
        <source>90° + 90° (vertical)</source>
        <translation>90° + 90° (vertical)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="355"/>
        <source>90° + 90° (horizontal)</source>
        <translation>90° + 90° (horizontal)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3290"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3296"/>
        <source>All objects</source>
        <translation>Todos los objetos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Schemas</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Tables</source>
        <translation>Tablas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Views</source>
        <translation>Vistas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Textboxes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="741"/>
        <source>Zoom: %1%</source>
        <translation>Zoom: %1%</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="1135"/>
        <source>Do you really want to convert the relationship into an intermediate table?</source>
        <translation>¿Realmente desea convertir la relación en una tabla intermedia?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="1355"/>
        <source>Loading database model</source>
        <translation>Cargando modelo de base de datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="1572"/>
        <source>Saving database model</source>
        <translation>Guardando modelo de base de datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2116"/>
        <source>Do you want to %1 the selected schema&apos;s children too?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2116"/>
        <source>protect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2116"/>
        <source>unprotect</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2229"/>
        <source>Also copy all dependencies of selected objects? This minimizes the breakdown of references when copied objects are pasted into another model.</source>
        <translation>También copiar todas las dependencias de los objetos seleccionados? Esto minimiza la ruptura de referencias cuando los objetos copiados se pegan en otro modelo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2333"/>
        <source>Pasting objects...</source>
        <translation>Pegar Objetos...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2356"/>
        <source>Validating object: `%1&apos; (%2)</source>
        <translation>Validando El objeto: `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2458"/>
        <source>Generating XML for: `%1&apos; (%2)</source>
        <translation>Generando código XML del objeto: `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2536"/>
        <source>Pasting object: `%1&apos; (%2)</source>
        <translation>Pegar Objetos: `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2591"/>
        <source>Not all objects were pasted to the model due to errors returned during the process! Refer to error stack for more details!</source>
        <translation>No todos los objetos se pegan a la modelo debido a errores devueltos durante el proceso! Consulte a un error de pila para más detalles!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2714"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; You are about to delete objects in cascade mode which means more objects than the selected will be dropped too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;PRECAUCIÓN:&lt;/strong&gt; Estás a punto de eliminar objetos en modo de cascada que significa más objetos que el seleccionado se redujo también. ¿Realmente desea continuar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2718"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; Remove multiple objects at once can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;PRECAUCIÓN:&lt;/strong&gt; Eliminar varios objetos a la vez puede causar invalidaciones irreversibles a otros objetos en el modelo que causa este tipo de objetos no válidos para ser borrados también. ¿Realmente desea continuar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2724"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; Remove a relationship can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;PRECAUCIÓN:&lt;/strong&gt; Eliminar una relación puede causar invalidaciones irreversibles a otros objetos en el modelo que causa este tipo de objetos no válidos para ser borrados también. ¿Realmente desea continuar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2727"/>
        <source>Do you really want to delete the selected object?</source>
        <translation>¿Realmente desea eliminar el objeto seleccionado?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2993"/>
        <source>The cascade deletion found some problems when running! Some objects could not be deleted or registered in the operation&apos;s history! Please, refer to error stack for more details.</source>
        <translation>La eliminación en cascada encontró algunos problemas al ejecutar! Algunos objetos no podían ser borrados o registrados en la historia de la operación! Por favor, consulte a un error de la pila para obtener más detalles.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3149"/>
        <source>(no objects)</source>
        <translation>(no hay objetos)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3156"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3869"/>
        <source>Source code</source>
        <translation>Código fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3917"/>
        <source>Constraints</source>
        <translation>Restricciones</translation>
    </message>
</context>
<context>
    <name>ModelsDiffHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="249"/>
        <source>Processing object `%1&apos; (%2)...</source>
        <translation>Procesando objecto `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="362"/>
        <source>Skipping object `%1&apos; (%2)...</source>
        <translation>Omitiendo objeto `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="626"/>
        <source>Processing diff infos...</source>
        <translation>Procesando diff infos...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="654"/>
        <source>Processing `%1&apos; info for object `%2&apos; (%3)...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="905"/>
        <source>No differences between the model and database.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="907"/>
        <source>Preparing diff code...</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>NewObjectOverlayWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="28"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="268"/>
        <source>Role</source>
        <translation>Rol</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="488"/>
        <source>Tag</source>
        <translation>Tag</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="221"/>
        <source>Cast</source>
        <translation>Cast</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="234"/>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="32"/>
        <source>A</source>
        <translation>A</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="400"/>
        <source>Language</source>
        <translation>Lenguaje</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="532"/>
        <source>Textbox</source>
        <translation>Textbox</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="356"/>
        <source>Event Trigger</source>
        <translation>Evento Trigger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="177"/>
        <source>Tablespace</source>
        <translation>Tablespace</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="133"/>
        <source>Schema</source>
        <translation>Esquema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="779"/>
        <source>Domain</source>
        <translation>Dominio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="647"/>
        <source>Conversion</source>
        <translation>Conversión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="691"/>
        <source>Aggregate</source>
        <translation>Agregado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="823"/>
        <source>Collation</source>
        <translation>Collation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="955"/>
        <source>Table</source>
        <translation>Tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="603"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="911"/>
        <source>Op. Family</source>
        <translation>Op. Familia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="867"/>
        <source>Sequence</source>
        <translation>Sequencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="312"/>
        <source>Extension</source>
        <translation>Extensión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="999"/>
        <source>Function</source>
        <translation>Función</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1087"/>
        <source>Op. Class</source>
        <translation>Op. Clase</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="735"/>
        <source>Operator</source>
        <translation>Operador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1043"/>
        <source>View</source>
        <translation>Vista</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1131"/>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1290"/>
        <source>Permissions</source>
        <translation>Permisos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1378"/>
        <source>Rule</source>
        <translation>Regla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1334"/>
        <source>Index</source>
        <translation>Indice</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1246"/>
        <source>Column</source>
        <translation>Columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1202"/>
        <source>Constraint</source>
        <translation>Restricción</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="444"/>
        <source>Generic SQL</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1422"/>
        <source>Trigger</source>
        <translation>Trigger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1466"/>
        <source>Policy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1537"/>
        <source>Many-to-many</source>
        <translation>Muchos-a-muchos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1581"/>
        <source>One-to-many</source>
        <translation>Uno-a-muchos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1625"/>
        <source>One-to-one</source>
        <translation>Uno-a-uno</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1669"/>
        <source>Inheritance</source>
        <translation>Herencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1713"/>
        <source>Copy</source>
        <translation>Copia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="33"/>
        <source>G</source>
        <translation>G</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="34"/>
        <source>K</source>
        <translation>K</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="35"/>
        <source>H</source>
        <translation>H</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="36"/>
        <source>J</source>
        <translation>J</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="37"/>
        <source>D</source>
        <translation>D</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="38"/>
        <source>E</source>
        <translation>E</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="39"/>
        <source>F</source>
        <translation>F</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="40"/>
        <source>L</source>
        <translation>L</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="41"/>
        <source>O</source>
        <translation>O</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="42"/>
        <source>U</source>
        <translation>U</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="43"/>
        <source>I</source>
        <translation>I</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="44"/>
        <source>R</source>
        <translation>R</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="45"/>
        <source>S</source>
        <translation>S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="46"/>
        <source>Q</source>
        <translation>Q</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="47"/>
        <source>T</source>
        <translation>T</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="48"/>
        <source>P</source>
        <translation>P</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="49"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="50"/>
        <source>Y</source>
        <translation>Y</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="51"/>
        <source>W</source>
        <translation>W</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="52"/>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="58"/>
        <source>9</source>
        <translation>9</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="53"/>
        <source>Z</source>
        <translation>Z</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="54"/>
        <source>X</source>
        <translation>X</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="55"/>
        <source>C</source>
        <translation>C</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="56"/>
        <source>V</source>
        <translation>V</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="57"/>
        <source>B</source>
        <translation>B</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="59"/>
        <source>8</source>
        <translation type="unfinished">8</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="61"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="62"/>
        <source>2</source>
        <translation>2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="63"/>
        <source>3</source>
        <translation>3</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="64"/>
        <source>5</source>
        <translation>5</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="65"/>
        <source>4</source>
        <translation>4</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="95"/>
        <source>0</source>
        <translation>0</translation>
    </message>
</context>
<context>
    <name>NumberedTextEditor</name>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="335"/>
        <source>Load file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="84"/>
        <source>Load the object&apos;s source code from an external file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="83"/>
        <source>Load</source>
        <translation type="unfinished">Cargar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="94"/>
        <source>Edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="95"/>
        <source>Edit the source code in the preferred external editor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="105"/>
        <source>Clear</source>
        <translation type="unfinished">Limpiar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="220"/>
        <source>Upper case</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="223"/>
        <source>Lower case</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="228"/>
        <source>Ident right</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="231"/>
        <source>Ident left</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="333"/>
        <source>SQL file (*.sql);;All files (*.*)</source>
        <translation type="unfinished">Archivo SQL (*.sql);;Todos los archivos (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="385"/>
        <source>The source editor `%1&apos; is running on `pid: %2&apos;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="418"/>
        <source>Could not start the source code editor application `%1&apos;! Make to sure that the source editor path defined in the general settings points to a valid executable and the current user has permission to run the application. Error message returned: `%2&apos;</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ObjectDepsRefsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="49"/>
        <source>Dependencies</source>
        <translation>Dependencias</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="128"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="281"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="140"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="293"/>
        <source>Object</source>
        <translation>Objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="152"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="305"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="164"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="317"/>
        <source>Parent Object</source>
        <translation>Objeto Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="176"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="329"/>
        <source>Parent Type</source>
        <translation>Tipo del Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="191"/>
        <source>Exclude indirect dependencies</source>
        <translation>Excluir dependencias indirectas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="202"/>
        <source>References</source>
        <translation>Referencias</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="344"/>
        <source>Include indirect references</source>
        <translation>Incluir referencias indirectas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="425"/>
        <source>This object does not exists anymore. The dependencies and references listing are disabled.</source>
        <translation>Este objeto no existe más. Las dependencias y referencias listadas se deshabilitan.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="14"/>
        <source>Object&apos;s dependencies &amp; references</source>
        <translation>Dependencias de objeto &amp; referencias</translation>
    </message>
</context>
<context>
    <name>ObjectFinderWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="26"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="126"/>
        <source>Pattern:</source>
        <translation>Patrón:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="640"/>
        <source>Find</source>
        <translation>Buscar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="193"/>
        <source>Defines the search filter</source>
        <translation>Define el filtro de búsqueda</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="196"/>
        <source>Filter</source>
        <translation>Filtro</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="91"/>
        <source>Clears the search results</source>
        <translation>Limpiar los resultados de la búsqueda</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="94"/>
        <source>Clear</source>
        <translation>Limpiar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="50"/>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="148"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="145"/>
        <source>Hide this widget</source>
        <translation type="unfinished">Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="234"/>
        <source>(Un)selects the graphical objects in the results grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="237"/>
        <source>Select</source>
        <translation type="unfinished">Selecionar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="315"/>
        <source>Regular Expression</source>
        <translation>Expresión Regular</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="328"/>
        <source>Exact Match</source>
        <translation>Resultado Exacto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="383"/>
        <source>Select All</source>
        <translation>Selecionar todos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="411"/>
        <source>Clear All</source>
        <translation>Limpiar Todos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="482"/>
        <source>Case Sensitive</source>
        <translation>Sensible Case</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="564"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="576"/>
        <source>Object</source>
        <translation>Objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="588"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="600"/>
        <source>Parent Object</source>
        <translation>Objecto Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="612"/>
        <source>Parent Type</source>
        <translation>Tipo del Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="675"/>
        <source>Fades outs all the graphical objects in the results grid (or those not listed). The current fade in/out state of all objects is modified.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="678"/>
        <source>Fade out</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="30"/>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="34"/>
        <source>Listed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="31"/>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="35"/>
        <source>Not listed</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="240"/>
        <source>Found &lt;strong&gt;%1&lt;/strong&gt; object(s).</source>
        <translation>Encontrado objecto(s) &lt;strong&gt;%1&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="245"/>
        <source>No objects found.</source>
        <translation>Objetos no encontrados.</translation>
    </message>
</context>
<context>
    <name>ObjectRenameWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="26"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="158"/>
        <source>....</source>
        <translation>....</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="176"/>
        <source>to:</source>
        <translation>a:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="194"/>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="243"/>
        <source>Rename</source>
        <translation>Renombrar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="269"/>
        <source>Cancel</source>
        <translation>Cancelar</translation>
    </message>
</context>
<context>
    <name>ObjectSelectorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="35"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="74"/>
        <source>Clear field</source>
        <translation>Limpiar campo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="109"/>
        <source>Select Object</source>
        <translation>Seleccionar Objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectselectorwidget.cpp" line="189"/>
        <source>Select %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ObjectsTableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="20"/>
        <source>Form</source>
        <translation type="unfinished">Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="58"/>
        <source>Add Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="74"/>
        <source>Ins</source>
        <translation type="unfinished">Ins</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="96"/>
        <source>Remove Item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="112"/>
        <source>Del</source>
        <translation type="unfinished">Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="134"/>
        <source>Update Item</source>
        <translation type="unfinished">Actualizar Item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="150"/>
        <source>Alt+R</source>
        <translation type="unfinished">Alt+R</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="172"/>
        <source>Remove All</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="188"/>
        <source>Shift+Del</source>
        <translation type="unfinished">Shift+Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="210"/>
        <source>Duplicate item</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="226"/>
        <source>Ctrl+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="248"/>
        <source>Edit Item</source>
        <translation type="unfinished">Editar Item</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="264"/>
        <source>Space</source>
        <translation type="unfinished">Espacio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="286"/>
        <source>Move Up</source>
        <translation type="unfinished">Mover Arriba</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="302"/>
        <source>Ctrl+Up</source>
        <translation type="unfinished">Ctrl+Up</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="324"/>
        <source>Move Down</source>
        <translation type="unfinished">Mover Abajo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="340"/>
        <source>Ctrl+Down</source>
        <translation type="unfinished">Ctrl+Down</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="362"/>
        <source>Move to start</source>
        <translation type="unfinished">Mover al inicio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="378"/>
        <source>Ctrl+Home</source>
        <translation type="unfinished">Ctrl+Home</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="400"/>
        <source>Move to end</source>
        <translation type="unfinished">Mover al final</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="416"/>
        <source>Ctrl+End, Ctrl+S</source>
        <translation type="unfinished">Ctrl+End, Ctrl+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="388"/>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="437"/>
        <source>Confirmation</source>
        <translation type="unfinished">Confirmación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="388"/>
        <source>Do you really want to remove the selected item?</source>
        <translation type="unfinished">¿Realmente desea eliminar el elemento seleccionado?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="437"/>
        <source>Do you really want to remove all the items?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>OperationList</name>
    <message>
        <location filename="../libpgmodeler/src/operationlist.cpp" line="520"/>
        <source>(invalid object)</source>
        <translation>(objeto no válido)</translation>
    </message>
</context>
<context>
    <name>OperationListWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="35"/>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="92"/>
        <source>Executed Operations</source>
        <translation>Operaciones Ejecutadas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="124"/>
        <source>Hide this widget</source>
        <translation>Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="127"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="248"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="261"/>
        <source>Operations:</source>
        <translation>Operaciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="271"/>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="291"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="281"/>
        <source>Position:</source>
        <translation>Posición:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="318"/>
        <source>Delete operation history</source>
        <translation>Borrar histórico de operaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="344"/>
        <source>Undo</source>
        <translation>Deshacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="370"/>
        <source>Redo</source>
        <translation>Rehacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="104"/>
        <source>Object: %1</source>
        <translation>Objecto: %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="109"/>
        <source>Name: %1</source>
        <translation>Nambre: %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="114"/>
        <source>created</source>
        <translation>creado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="119"/>
        <source>removed</source>
        <translation>removido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="124"/>
        <source>modified</source>
        <translation>modificado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="129"/>
        <source>moved</source>
        <translation>movido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="135"/>
        <source>Operation: %1</source>
        <translation>Operación: %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="207"/>
        <source>Operation history exclusion</source>
        <translation>Exclusión del historia de Operación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="208"/>
        <source>Delete the executed operations history is an irreversible action, do you want to continue?</source>
        <translation>Eliminar el historial de operaciones ejecutadas es una acción irreversible, es lo que quieres continuar?</translation>
    </message>
</context>
<context>
    <name>OperatorClassWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="54"/>
        <source>Default Class:</source>
        <translation>Clase por Defecto:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="79"/>
        <source>Indexing:</source>
        <translation>Indexación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="102"/>
        <source>Elements</source>
        <translation>Elementos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="123"/>
        <source>Element Type:</source>
        <translation>Tipo de Elemento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="137"/>
        <source>Operator</source>
        <translation>Operador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="142"/>
        <source>Function</source>
        <translation>Función</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="147"/>
        <source>Storage</source>
        <translation>Almacenamiento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="168"/>
        <source>Function:</source>
        <translation>Función:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="175"/>
        <source>Operator:</source>
        <translation>Operador:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="182"/>
        <source>Support/Strategy:</source>
        <translation>Suporte/Estrategía:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="196"/>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="218"/>
        <source>Op. Family:</source>
        <translation>Op. Familia:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="38"/>
        <source>Storage Type</source>
        <translation>Tipo de Almacenamiento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="42"/>
        <source>Object</source>
        <translation>Objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="45"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="48"/>
        <source>Support/Strategy</source>
        <translation>Suporte/Estrategía</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="49"/>
        <source>Operator Family</source>
        <translation>Operador Familiar</translation>
    </message>
</context>
<context>
    <name>OperatorFamilyWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorfamilywidget.ui" line="50"/>
        <source>Indexing:</source>
        <translation>Indexación:</translation>
    </message>
</context>
<context>
    <name>OperatorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="147"/>
        <source>MERGES</source>
        <translation>MERGES</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="127"/>
        <source>HASHES</source>
        <translation>HASHES</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="140"/>
        <source>Options:</source>
        <translation>Opciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="36"/>
        <source>Arguments</source>
        <translation>Algumentos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="41"/>
        <source>Advanced</source>
        <translation>Avanzado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="62"/>
        <source>Join:</source>
        <translation>Join:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="69"/>
        <source>Restrict:</source>
        <translation>Restricción:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="76"/>
        <source>Commutator:</source>
        <translation>Conmutador:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="83"/>
        <source>Negator:</source>
        <translation>Negación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="120"/>
        <source>Operator Func.:</source>
        <translation>Operador Func .:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="32"/>
        <source>Left Argument Type</source>
        <translation>Tipo Algumento Izquierdo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="34"/>
        <source>Right Argument Type</source>
        <translation>Tipo Argumento Direcho</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="44"/>
        <source>To create a unary operator it is necessary to specify as &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; one of its arguments. Additionally, the function that defines the operator must have only one parameter and this, in turn, must have the same data type of the the argument of unary operator.</source>
        <translation>Para crear un operador unario es necesario especificar como &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; uno de sus argumentos. Además, la función que define el operador debe tener un solo parámetro y esto, a su vez, debe tener el mismo tipo de datos del argumento del operador unitario.</translation>
    </message>
</context>
<context>
    <name>ParameterWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="41"/>
        <source>Default Value:</source>
        <translation>Valor por Defecto:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="70"/>
        <source>Mode:</source>
        <translation>Modo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="89"/>
        <source>IN</source>
        <translation>IN</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="96"/>
        <source>OUT</source>
        <translation>OUT</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="109"/>
        <source>VARIADIC</source>
        <translation>VARIADIC</translation>
    </message>
</context>
<context>
    <name>PermissionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="51"/>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="395"/>
        <source>Permissions</source>
        <translation>Permisos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="90"/>
        <source>ID:</source>
        <translation>ID:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="121"/>
        <source>Disable SQL code</source>
        <translation>Desactivar Código SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="152"/>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="66"/>
        <source>Roles</source>
        <translation>Roles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="175"/>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="68"/>
        <source>Privileges</source>
        <translation>Privilégios</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="26"/>
        <source>Edit permissions</source>
        <translation type="unfinished">Editar permisos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="199"/>
        <source>&amp;Grant</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="209"/>
        <source>Re&amp;voke</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="219"/>
        <source>Cascade</source>
        <translation>Cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="242"/>
        <source>Privilege</source>
        <translation>Privilégio</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="247"/>
        <source>GRANT OPTION</source>
        <translation>GRANT OPTION</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="283"/>
        <source>Add Permission</source>
        <translation>Adicionar Permiso</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="312"/>
        <source>Update Permission</source>
        <translation>Actualizar Permiso</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="341"/>
        <source>Cancel Operation</source>
        <translation>Cancelar Operación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="404"/>
        <source>Code Preview</source>
        <translation>Vista previa Código</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="64"/>
        <source>Id</source>
        <translation>Id</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="52"/>
        <source>Name</source>
        <translation type="unfinished">Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="91"/>
        <source>Leave the &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; grid empty in order to create a %1 applicable to &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="526"/>
        <source>-- No permissions defined for the specified object!</source>
        <translation>-- No hay permisos definidos para el objeto especificado!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="534"/>
        <source>/* Could not generate the SQL code preview for permissions!</source>
        <translation>/ * No se pudo generar el código SQL para previsualización permisos!</translation>
    </message>
</context>
<context>
    <name>PgModelerCLI</name>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="125"/>
        <source>Unrecognized option &apos;%1&apos;.</source>
        <translation>Opción no reconocido &apos;%1&apos;.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="129"/>
        <source>Value not specified for option &apos;%1&apos;.</source>
        <translation>El valor no se especifica para la opción &apos;%1&apos;.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="131"/>
        <source>Option &apos;%1&apos; does not accept values.</source>
        <translation>Opción &apos;%1&apos; no acepta valores.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="373"/>
        <source>Usage: pgmodeler-cli [OPTIONS]</source>
        <translation>El uso: pgmodeler-cli [OPTIONS]</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="369"/>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="580"/>
        <source> command line interface.</source>
        <translation>interfaz de línea de comandos.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="376"/>
        <source>General options: </source>
        <translation>Opciones Generales: </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="407"/>
        <source>DBMS export options: </source>
        <translation>Opciones de exportación del SGDB: </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="439"/>
        <source>Miscellaneous options: </source>
        <translation> Otros Opciones: </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="476"/>
        <source>There are no connections configured.</source>
        <translation>No hay conexiones configuradas.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="533"/>
        <source>Input file must be different from output!</source>
        <translation>Archivo de entrada debe ser diferente de la salida!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="537"/>
        <source>Incomplete connection information!</source>
        <translation>Información de conexión incompleto!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="540"/>
        <source>Invalid zoom specified!</source>
        <translation>Inválido Zoom especificado!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="543"/>
        <source>Invalid action specified to update mime option!</source>
        <translation>Acción inválida para actualizar opción mimo!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1177"/>
        <source>Starting model fixing...</source>
        <translation>Iniciar fijación modelo...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1450"/>
        <source>Starting mime update...</source>
        <translation>Iniciar actualización de mime...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1191"/>
        <source>Starting model export...</source>
        <translation>Comenzando del modelo de exportación ...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1186"/>
        <source>Model successfully fixed!</source>
        <translation>Modelo fijado correctamente!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="401"/>
        <source>PNG and SVG export options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="220"/>
        <source>Connection aliased as &apos;%1&apos; was not found in the configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="370"/>
        <source>PostgreSQL Database Modeler Project - pgmodeler.io</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="371"/>
        <source>Copyright 2006-2018 Raphael A. Silva &lt;raphael@pgmodeler.io&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="374"/>
        <source>This CLI tool provides several operations over models and databases without the need to perform them
in pgModeler&apos;s graphical interface. All available options are described below.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="377"/>
        <source>  %1, %2 [FILE]		    Input model file (.dbm). This is mandatory for fix, export operations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="378"/>
        <source>  %1, %2 [DBNAME]	    Input database name. This is mandatory for import operation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="379"/>
        <source>  %1, %2 [FILE]		    Output file. This is mandatory for fixing model or exporting to file, png or svg.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="380"/>
        <source>  %1, %2		    Try to fix the structure of the input model file in order to make it loadable again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="381"/>
        <source>  %1, %2 [NUMBER]	    Model fix tries. When reaching the maximum count the invalid objects will be discarded.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="382"/>
        <source>  %1, %2		    Export the input model to a sql script file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="383"/>
        <source>  %1, %2		    Export the input model to a png image.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="384"/>
        <source>  %1, %2		    Export the input model to a svg file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="385"/>
        <source>  %1, %2		    Export the input model directly to a PostgreSQL server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="386"/>
        <source>  %1, %2		    Import a database to an output file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="387"/>
        <source>  %1, %2			    Compares a model and a database or two databases generating the SQL script to synch the latter in relation to the first.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="388"/>
        <source>  %1, %2		    Force the PostgreSQL version of generated SQL code.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="389"/>
        <source>  %1, %2			    Silent execution. Only critical messages and errors are shown during process.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="390"/>
        <source>  %1, %2			    Show this help menu.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="392"/>
        <source>Connection options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="393"/>
        <source>  %1, %2		    List available connections in file %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="394"/>
        <source>  %1, %2 [ALIAS]	    Connection configuration alias to be used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="395"/>
        <source>  %1, %2 [HOST]		    PostgreSQL host in which a task will operate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="396"/>
        <source>  %1, %2 [PORT]		    PostgreSQL host listening port.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="397"/>
        <source>  %1, %2 [USER]		    PostgreSQL username.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="398"/>
        <source>  %1, %2 [PASSWORD]	    PostgreSQL user password.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="399"/>
        <source>  %1, %2 [DBNAME]	    Connection&apos;s initial database.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="402"/>
        <source>  %1, %2		    Draws the grid in the exported image.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="403"/>
        <source>  %1, %2	    Draws the page delimiters in the exported image.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="404"/>
        <source>  %1, %2		    Each page will be exported in a separated png image. (Only for PNG images)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="405"/>
        <source>  %1, %2 [FACTOR]		    Applies a zoom (in percent) before export to png image. Accepted zoom interval: %3-%4 (Only for PNG images)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="408"/>
        <source>  %1, %2	    Ignores errors related to duplicated objects that eventually exist in the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="409"/>
        <source>  %1, %2 [CODES] Ignores additional errors by their codes. A comma-separated list of alphanumeric codes should be provided.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="410"/>
        <source>  %1, %2		    Drop the database before execute a export process.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="411"/>
        <source>  %1, %2		    Runs the DROP commands attached to SQL-enabled objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="412"/>
        <source>  %1, %2		    Simulates an export process by executing all steps but undoing any modification in the end.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="413"/>
        <source>  %1, %2		    Generates temporary names for database, roles and tablespaces when in simulation mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="415"/>
        <source>Database import options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="416"/>
        <source>  %1, %2		    Ignore all errors and try to create as many as possible objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="417"/>
        <source>  %1, %2	    Import system built-in objects. This option causes the model bloating due to the importing of unneeded objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="418"/>
        <source>  %1, %2	    Import extension objects. This option causes the model bloating due to the importing of unneeded objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="419"/>
        <source>  %1, %2		    Run import in debug mode printing all queries executed in the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="421"/>
        <source>Diff options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="422"/>
        <source>  %1, %2 [DBNAME]	    The database used in the comparison. All the SQL code generated is applied to it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="423"/>
        <source>  %1, %2		    Save the generated diff code to output file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="424"/>
        <source>  %1, %2		    Apply the generated diff code on the database server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="425"/>
        <source>  %1, %2	    Don&apos;t preview the generated diff code when applying it to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="426"/>
        <source>  %1, %2	    Drop cluster level objects like roles and tablespaces.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="427"/>
        <source>  %1, %2		    Revoke permissions already set on the database. New permissions configured in the input model are still applied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="428"/>
        <source>  %1, %2		    Drop missing objects. Generates DROP commands for objects that are present in the input model but not in the compared database.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="429"/>
        <source>  %1, %2	    Force the drop of missing columns and constraints. Causes only columns and constraints to be dropped, other missing objects aren&apos;t removed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="430"/>
        <source>  %1, %2		    Rename the destination database when the names of the involved databases are different.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="431"/>
        <source>  %1, %2		    Don&apos;t drop or truncate objects in cascade mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="432"/>
        <source>  %1, %2	    Truncate tables prior to alter columns. Avoids errors related to type casting when the new type of a column isn&apos;t compatible to the old one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="433"/>
        <source>  %1, %2	    Don&apos;t reuse sequences on serial columns. Drop the old sequence assigned to a serial column and creates a new one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="434"/>
        <source>  %1, %2	    Don&apos;t force the recreation of objects. Avoids the usage of a DROP and CREATE commands to create a new version of the objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="435"/>
        <source>  %1, %2	    Don&apos;t recreate the unmodifiable objects. These objects are the ones which can&apos;t be changed via ALTER command.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="440"/>
        <source>  %1, %2 [ACTION]	    Handles the file association to .dbm files. The ACTION can be [%3 | %4].</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="444"/>
        <source>** The diff process allows the usage of the following options related to import and export operations: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="445"/>
        <source>* Export: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="445"/>
        <source>* Import: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="447"/>
        <source>** When running the diff using two databases (%1 and %2) there&apos;s the need to specify two connections/aliases. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="448"/>
        <source>   If only one connection is set it will be used to import the input database as well to retrieve database used in the comparison.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="449"/>
        <source>   A second connection can be specified by appending a 1 on any connection configuration parameter listed above.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="481"/>
        <source>Available connections (alias : connection string)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="513"/>
        <source>No operation mode was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="516"/>
        <source>Export, fix model, import database, diff and update mime operations can&apos;t be used at the same time!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="519"/>
        <source>Multiple export mode was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="522"/>
        <source>No input file was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="525"/>
        <source>No input database was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="528"/>
        <source>No output file was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="548"/>
        <source>No input file or database was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="551"/>
        <source>The input file and database can&apos;t be used at the same time!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="554"/>
        <source>No database to be compared was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="557"/>
        <source>No diff action (save or apply) was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="560"/>
        <source>No output file for the diff code was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="613"/>
        <source>** Error code `%1&apos; found and ignored. Proceeding with export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="614"/>
        <source>** Command: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="692"/>
        <source>Extracting objects&apos; XML...</source>
        <translation>Exportando objetos XML...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="708"/>
        <source>Invalid input file! It seems that is not a pgModeler generated model or the file is corrupted!</source>
        <translation>Archivo de entrada no es válido! Parece que no es un modelo pgModeler generada o el archivo está dañado!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="830"/>
        <source>Recreating objects...</source>
        <translation>Recreando objetos...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="950"/>
        <source>
** Object(s) that couldn&apos;t fixed: </source>
        <translation>
** Objeto(s) que no podían ser fijado: </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="961"/>
        <source>WARNING: There are objects that maybe can&apos;t be fixed. Trying again... (tries %1/%2)</source>
        <translation>ADVERTENCIA: Hay objetos que tal vez no se pueden arreglar. Tratando de nuevo ... (Trata %1/%2)</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1178"/>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1192"/>
        <source>Loading input file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1179"/>
        <source>Fixed model file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1203"/>
        <source>Export to PNG image: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1213"/>
        <source>Export to SVG file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1222"/>
        <source>Export to SQL script file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1229"/>
        <source>Export to DBMS: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1242"/>
        <source>Export successfully ended!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1247"/>
        <source>Starting database import...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1248"/>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1315"/>
        <source>Input database: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1255"/>
        <source>Saving the imported database to file...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1259"/>
        <source>Import successfully ended!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1310"/>
        <source>Starting diff process...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1313"/>
        <source>Input model: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1318"/>
        <source>Compare to: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1322"/>
        <source>Loading input model...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1328"/>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1332"/>
        <source>Importing the database `%1&apos;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1356"/>
        <source>Comparing the generated models...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1360"/>
        <source>No differences were detected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1367"/>
        <source>Saving diff to file `%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1410"/>
        <source>** WARNING: You are about to apply the generated diff code to the server. Data can be lost in the process!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1414"/>
        <source>** Proceed with the diff applying? (yes/no) &gt; </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1420"/>
        <source>yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1420"/>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1422"/>
        <source>no</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1425"/>
        <source>Diff code not applied to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1431"/>
        <source>Applying diff to the database `%1&apos;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1444"/>
        <source>Diff successfully ended!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1454"/>
        <source>Mime database successfully updated!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1516"/>
        <source>Database model files (.dbm) are already associated to pgModeler!</source>
        <translation>Archivos de modelo de base de datos (.dbm) ya están asociados a pgModeler!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1517"/>
        <source>There is no file association related to pgModeler and .dbm files!</source>
        <translation>No hay ninguna asociación de archivos relacionados con pgModeler y archivos .dbm!</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1519"/>
        <source>Mime database operation: %1</source>
        <translation>Operación Mime de base de datos: %1</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1578"/>
        <source>Can&apos;t erase the file %1! Check if the current user has permissions to delete it and if the file exists.</source>
        <translation>No se puede borrar el archivo% 1! Compruebe si el usuario actual tiene permisos para eliminarlo y si existe el archivo.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1653"/>
        <source>Running update-mime-database command...</source>
        <translation>Ejecución de comando update-mime-database...</translation>
    </message>
</context>
<context>
    <name>PgModelerPlugin</name>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="32"/>
        <source>Plugin Information</source>
        <translation>Información del plugin</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="86"/>
        <source>Version: %1</source>
        <translation>Versión: %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="87"/>
        <source>Author: %1</source>
        <translation>Autor: %1</translation>
    </message>
</context>
<context>
    <name>PgModelerUiNS</name>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodeleruins.cpp" line="98"/>
        <source>Do you want to apply the &lt;strong&gt;SQL %1 status&lt;/strong&gt; to the object&apos;s references too? This will avoid problems when exporting or validating the model.</source>
        <translation>¿Desea aplicar el &lt;strong&gt;SQL %1 estatus&lt;/strong&gt; a las referencias del objeto también? Esto evitará problemas al exportar o validar el modelo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodeleruins.cpp" line="98"/>
        <source>disabling</source>
        <translation>deshabilitando</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodeleruins.cpp" line="98"/>
        <source>enabling</source>
        <translation>habilitando</translation>
    </message>
</context>
<context>
    <name>PgSQLTypeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="32"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="68"/>
        <source>Data Type</source>
        <translation>Tipo de Dato</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="143"/>
        <source>SRID:</source>
        <translation>SRID:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="189"/>
        <source>Variation:</source>
        <translation>Variación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="196"/>
        <source>Z</source>
        <translation>Z</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="209"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="236"/>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="464"/>
        <source>Precision</source>
        <translation>Precisión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="287"/>
        <source>Spatial:</source>
        <translation>Espacial:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="312"/>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="609"/>
        <source>Dimension</source>
        <translation>Dimensión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="349"/>
        <source>Format:</source>
        <translation>Formato:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="381"/>
        <source>Timezone:</source>
        <translation>Zona Horaria:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="434"/>
        <source>Type:</source>
        <translation>Tipo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="470"/>
        <source>P:</source>
        <translation>P:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="495"/>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="537"/>
        <source>Length</source>
        <translation>Longitud</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="543"/>
        <source>L:</source>
        <translation>L:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="579"/>
        <source>Interval:</source>
        <translation>Intervalo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="615"/>
        <source>[ ]:</source>
        <translation>[ ]:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgsqltypewidget.cpp" line="47"/>
        <source>NONE</source>
        <translation>NONE</translation>
    </message>
</context>
<context>
    <name>PluginsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="80"/>
        <source>Open in file manager</source>
        <translation>Abrir en el administrador de archivos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="106"/>
        <source>Plug-ins root directory:</source>
        <translation>Plug-ins de directorio raíz:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="121"/>
        <source>Loaded plug-ins</source>
        <translation>Cargando plug-ins</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="32"/>
        <source>Plugin</source>
        <translation>Plugin</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="34"/>
        <source>Version</source>
        <translation>Versión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="35"/>
        <source>Library</source>
        <translation>Biblioteca</translation>
    </message>
</context>
<context>
    <name>PolicyWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="38"/>
        <source>Basics</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="56"/>
        <source>Command:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="63"/>
        <source>Permissive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="112"/>
        <source>Roles</source>
        <translation type="unfinished">Roles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="117"/>
        <source>Expressions</source>
        <translation type="unfinished">Expresiones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="135"/>
        <source>USING:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="152"/>
        <source>CHECK:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/policywidget.cpp" line="43"/>
        <source>Name</source>
        <translation type="unfinished">Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/policywidget.cpp" line="49"/>
        <source>Leave the &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; grid empty in order to create a %1 applicable to &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="34"/>
        <source>new_database</source>
        <translation>new_database</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3016"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3075"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3302"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9801"/>
        <source>%1 (line: %2)</source>
        <translation>%1 (line: %2)</translation>
    </message>
</context>
<context>
    <name>Relationship</name>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="85"/>
        <source>%1_has_one_%2</source>
        <translation>%1_tiene_uno_%2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="87"/>
        <source>%1_has_many_%2</source>
        <translation>%1_tiene_muchos_%2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="89"/>
        <source>many_%1_has_many_%2</source>
        <translation>muchos_%1_tiene_muchos_%2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="91"/>
        <source>%1_inherits_%2</source>
        <translation>%1_herencia_%2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="93"/>
        <source>%1_copies_%2</source>
        <translation>%1_copias_%2</translation>
    </message>
</context>
<context>
    <name>RelationshipConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="45"/>
        <source>Connection Mode</source>
        <translation>Modo de Conexión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="304"/>
        <source>Connect FK to PK columns</source>
        <translation>Conecte FK a columnas PK</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="91"/>
        <source>This mode renders the relationships in crow&apos;s foot notation which has a better semantics and readability. It also determines the optimal point where the relationship is connected on the tables&apos; edges taking their position into account.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="94"/>
        <source>Crow&apos;s foot notation</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="196"/>
        <source>This mode determines the optimal point where the relationship is connected on the tables&apos; edges taking their position into account. It implies the usage of the classical ER notation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="199"/>
        <source>Connect tables&apos; edges</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="301"/>
        <source>This mode is available only for &lt;strong&gt;one-to-one&lt;/strong&gt;, &lt;strong&gt;one-to-many&lt;/strong&gt; and &lt;strong&gt;fk relationships&lt;/strong&gt; but provides a better semantics when linking tables by placing the lines on the exact point where the relationship occurs. It implies the usage of the classical ER notation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="403"/>
        <source>This mode is the classical one. It connects the relationship to tables through their central points. It implies the usage of the classical ER notation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="406"/>
        <source>Connect tables&apos; center points</source>
        <translation>Conecte tabla a puntos central</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="515"/>
        <source>FK Settings &amp;&amp; Patterns</source>
        <translation>Configurar FK &amp;&amp; Patrones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="539"/>
        <source>Foreign key settings</source>
        <translation>Configurar llave foránea</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="587"/>
        <source>Deferral:</source>
        <translation>Diferimiento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="612"/>
        <source>Deferrable:</source>
        <translation>Diferible:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="672"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="685"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="706"/>
        <source>Name patterns</source>
        <translation>Nombre patrones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="727"/>
        <source>Pattern for foreign key generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Patrón de llave foránea generada basa en pk de las tablas de referencia (1-1 y 1-n) o basada en pk (nn) de las tablas de origen.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="730"/>
        <source>Foreign Key (Source):</source>
        <translation>Llave Foránea (Fuente):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="737"/>
        <source>Relationship type:</source>
        <translation>Tipo de relación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="744"/>
        <source>Pattern for columns generated based upon target table&apos;s pk (n-n).</source>
        <translation>Patrón de columnas generadas basa en pk (nn) de las tablas de destino.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="747"/>
        <source>Column (Target):</source>
        <translation>Columna (Target):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="755"/>
        <source>One to one (1:1)</source>
        <translation>Uno a uno (1:1)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="760"/>
        <source>One to many (1:n)</source>
        <translation>Uno a muchos (1:n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="765"/>
        <source>Many to many (n:n)</source>
        <translation>muchos a muchos (n:n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="770"/>
        <source>Generalization</source>
        <translation>Generalización</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="775"/>
        <source>Copy</source>
        <translation>Copia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="783"/>
        <source>Pattern for columns generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Patrón de columnas generadas basa en pk de las tablas de referencia (1-1 y 1-n) o basada en pk (nn) de  las tablas de origen.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="786"/>
        <source>Column (Source):</source>
        <translation>Column (Fuente):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="793"/>
        <source>Pattern for foreign key generated based upon target table&apos;s pk (n-n).</source>
        <translation>Patrón de llave foránea generada basa en pk (nn) de las tablaa de destino.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="796"/>
        <source>Foreign Key (Target):</source>
        <translation>Lave Foránea (Destino):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="803"/>
        <source>Pattern for unique key generated by the relationship.</source>
        <translation>Patrón de llave única generada por la relación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="806"/>
        <source>Unique Key Name:</source>
        <translation>Nombre Llave Única:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="813"/>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="823"/>
        <source>Pattern for primary key generated by identifier relationship.</source>
        <translation>Patrón para la llave primaria generada por la relación identificador.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="816"/>
        <source>Primary Key Name:</source>
        <translation>Nombre Llave Primaria:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="826"/>
        <source>Primary Key Column:</source>
        <translation>Columna Llave Primaria:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipconfigwidget.cpp" line="63"/>
        <source>Default</source>
        <translation>por Defecto</translation>
    </message>
</context>
<context>
    <name>RelationshipWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="60"/>
        <source>General</source>
        <translation>General</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="99"/>
        <source>Table 1:</source>
        <translation>Tabla 1:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1119"/>
        <source>Name Patterns</source>
        <translation>Nombre Patrones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="335"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1140"/>
        <source>Use the values defined on settings dialogs for the fields below</source>
        <translation>Utilice los valores definidos en la configuración de los cuadros de diálogo de los campos siguientes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="338"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1143"/>
        <source>Use global settings for these fields</source>
        <translation>Utilice la configuración global de estos campos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1171"/>
        <source>Pattern for columns generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Patrón de columnas generadas basa en pk de las tablas de referencia (1-1 y 1-n) o basada en pk (nn) de  las tablas de origen.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1174"/>
        <source>Column (Source):</source>
        <translation>Columna (Fuente):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1181"/>
        <source>Pattern for columns generated based upon target table&apos;s pk (n-n).</source>
        <translation>Patrón de columnas generadas basa en pk (nn) de las tablas de destino.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1184"/>
        <source>Column (Target):</source>
        <translation type="unfinished">Columna (Target):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1191"/>
        <source>Pattern for foreign key generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Patrón de llave foránea generada basa en pk de las tablas de referencia (1-1 y 1-n) o basada en pk (nn) de las tablas de origen.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1194"/>
        <source>Foreign Key (Source):</source>
        <translation>Llave Foránea (Fuente):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1201"/>
        <source>Pattern for foreign key generated based upon target table&apos;s pk (n-n).</source>
        <translation>Patrón de llave foránea generada basa en pk (nn) de tabla de destino.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1204"/>
        <source>Foreign Key (Target):</source>
        <translation>Llave Foránea (Destino):</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1211"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1231"/>
        <source>Pattern for primary key generated by identifier relationship.</source>
        <translation>Patrón para la llave primaria generada por la relación identificador.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1214"/>
        <source>Primary Key Name:</source>
        <translation>Nombre Llave Primaria:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1221"/>
        <source>Pattern for unique key generated by the relationship.</source>
        <translation>Patrón de llave única generada por la relación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1224"/>
        <source>Unique Key Name:</source>
        <translation>Nombra Llave Única:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1234"/>
        <source>Primay Key Column:</source>
        <translation>Columna Llave Primaria:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="146"/>
        <source>Cardinality:</source>
        <translation>Cardinalidad:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="171"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1013"/>
        <source>Name of the table generated from many to many relationship</source>
        <translation>Nombre de la tabla generada a partir de relación muchos a muchos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="174"/>
        <source>Gen. Table Name:</source>
        <translation>Nombre tabla Gen.:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="215"/>
        <source>Rel. Type:</source>
        <translation>Tipo Rel.:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="262"/>
        <source>Table 2:</source>
        <translation>Tabla 2:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="277"/>
        <source>[SRC] is required</source>
        <translation>[SRC] es requirido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="290"/>
        <source>[DST] is required</source>
        <translation>[DST] es requirido</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="305"/>
        <source>Foreign key Settings</source>
        <translation>Configuración de Llave Foránea</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="393"/>
        <source>Deferrable:</source>
        <translation>Diferible:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="446"/>
        <source>Deferral:</source>
        <translation>Diferimiento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="481"/>
        <source>ON DELETE:</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="491"/>
        <source>ON UPDATE:</source>
        <translation>ON DELETE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="535"/>
        <source>One to one relationship</source>
        <translation>Relación uno a uno</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="538"/>
        <source>&amp;1-1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="646"/>
        <source>&amp;gen</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="932"/>
        <source>E&amp;XCLUDING</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="571"/>
        <source>One to many relationship</source>
        <translation>Relación de uno a muchos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="574"/>
        <source>1-n</source>
        <translation>1-n</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="607"/>
        <source>Many to many relationship</source>
        <translation>Relación de muchos a muchos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="610"/>
        <source>n-n</source>
        <translation>n-n</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="643"/>
        <source>Generalization relationship (inheritance)</source>
        <translation>Relación de generalización (herencia)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="679"/>
        <source>Dependency / Copy relationship</source>
        <translation>Relación de Dependencia / Copia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="682"/>
        <source>dep</source>
        <translation>dep</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="709"/>
        <source>Relationship generated via foreign key</source>
        <translation>Relación generada a través de clave externa</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="712"/>
        <source>fk</source>
        <translation>fk</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="759"/>
        <source>The receiver&apos;s primary key will be composed by the generated foreign key columns.</source>
        <translation>Clave principal del receptor estará compuesta por las columnas llave foránea generadas.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="762"/>
        <source>Identifier</source>
        <translation>Identificador</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="821"/>
        <source>Instead of create a multi-valued primary key with the generated foreign keys columns a single column is created and used as primary key.</source>
        <translation>En lugar de crear una clave principal de varios valores con las teclas extranjero generada columnas se crea una sola columna y se utiliza como clave principal.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="824"/>
        <source>Single PK column</source>
        <translation>Una sola columna PK</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="858"/>
        <source>Custom Color:</source>
        <translation>Color personalizado:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="871"/>
        <source>Copy Options</source>
        <translation>Opciones de copia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="892"/>
        <source>INDEXES</source>
        <translation>INDEXES</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="902"/>
        <source>COMMENTS</source>
        <translation>COMMENTS</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="909"/>
        <source>INCLUDING</source>
        <translation>INCLUDING</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="919"/>
        <source>DEFAULTS</source>
        <translation>DEFAULTS</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="942"/>
        <source>CONSTRAINTS</source>
        <translation>CONSTRAINTS</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="955"/>
        <source>Use defaults</source>
        <translation>Usar valores predeterminados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="968"/>
        <source>ALL</source>
        <translation>ALL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="978"/>
        <source>STORAGE</source>
        <translation>STORAGE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1444"/>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1449"/>
        <source>Constraints</source>
        <translation>Resticciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1454"/>
        <source>Primary key</source>
        <translation>Llave Primaria</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1498"/>
        <source>Advanced</source>
        <translation>Avanzado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="79"/>
        <source>Attribute</source>
        <translation>Atributo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="81"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="87"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="93"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="85"/>
        <source>Constraint</source>
        <translation>Resticción</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="91"/>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="109"/>
        <source>Use the special primary key if you want to include a primary key containing generated columns to the receiver table. &lt;strong&gt;Important:&lt;/strong&gt; if this is a new relationship there is a need to finish its creation and reopen this dialog to create the special primary key.</source>
        <translation>Utilice la clave primaria especial si usted desea incluir una clave principal que contiene las columnas generadas de la tabla receptor. &lt;strong&gt;Importante:&lt;/strong&gt; si se trata de una nueva relación es necesario para terminar su creación y vuelva a abrir este cuadro de diálogo para crear la clave primaria especial.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="119"/>
        <source>This advanced tab shows the objects (columns or table) auto created by the relationship&apos;s connection as well the foreign keys that represents the link between the participant tables.</source>
        <translation>Esta pestaña avanzado muestra los objetos (columnas o tabla) de automóviles creados por la conexión de la relación, así las claves externas que representa el enlace entre las mesas de los participantes.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="134"/>
        <source>Available tokens to define name patterns:&lt;br/&gt;					&lt;strong&gt;%1&lt;/strong&gt; = Reference (source) primary key column name. &lt;em&gt;(Ignored on constraint patterns)&lt;/em&gt;&lt;br/&gt; 					&lt;strong&gt;%2&lt;/strong&gt; = Reference (source) table name.&lt;br/&gt; 					&lt;strong&gt;%3&lt;/strong&gt; = Receiver (destination) table name.&lt;br/&gt; 					&lt;strong&gt;%4&lt;/strong&gt; = Generated table name. &lt;em&gt;(Only for n:n relationships)&lt;/em&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="147"/>
        <source>Default</source>
        <translation>por Defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="260"/>
        <source>Referer View:</source>
        <translation>Vista Intermedia:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="261"/>
        <source>Referer view references one or more columns of a table to construct it&apos;s own columns.</source>
        <translation>Vista Intermedia hace referencia a una o más columnas de una tabla de construir es propias columnas.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="262"/>
        <source>Referenced table has its columns referenced by a view in order to construct the columns of this latter.</source>
        <translation>Tabla referenciada tiene sus columnas referenciadas por una vista para construir las columnas de este último.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="266"/>
        <source>Referer Table:</source>
        <translation>Tabla Intermedia:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="267"/>
        <source>Referer table references one or more columns of a table through foreign keys. This is the (n) side of relationship.</source>
        <translation>Tabla intermedia referencia a una o más columnas de una tabla a través de claves externas. Este es el lado (n) de relación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="268"/>
        <source>Referenced table has its columns referenced by a table&apos;s foreign key. This is the (1) side of relationship.</source>
        <translation>Tabla referenciada tiene sus columnas referenciadas por una mesa y apos; s clave externa. Esta es la (1) lado de relación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="271"/>
        <source>Referenced Table:</source>
        <translation>Tabla Referenciada:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="280"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="291"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="293"/>
        <source>Reference Table:</source>
        <translation>Tabla Referencia:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="281"/>
        <source>Reference table has the columns from its primary key will copied to the receiver table in order to represent the linking between them. This is the (1) side of relationship.</source>
        <translation>Tabla de referencia tiene las columnas de su clave primaria se copian en la tabla del receptor con el fin de representar a la vinculación entre ellos. Esta es la (1) lado de relación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="283"/>
        <source>Receiver Table:</source>
        <translation>Tabla Receptora:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="284"/>
        <source>Receiver (or referer) table will receive the generated columns and the foreign key in order to represent the linking between them. This is the (n) side of relationship.</source>
        <translation>Receptor tabla (o intermedia) recibirá las columnas generadas y la clave externa con el fin de representar a la vinculación entre ellos. Este es el lado (n) de relación.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="292"/>
        <source>In many-to-many relationships both tables are used as reference to generate the table that represents the linking. Columns from both tables are copied to the resultant table and two foreign keys are created as well in order to reference each participant table.</source>
        <translation>En muchos-a-muchos relación ambas tablas se utilizan como referencia para generar la tabla que representa la vinculación. Las columnas de ambas tablas se copian en la tabla resultante y dos claves externas se crean así con el fin de hacer referencia a cada mesa participante.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="301"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="302"/>
        <source> is required</source>
        <translation> es requerido</translation>
    </message>
</context>
<context>
    <name>ResultSetModel</name>
    <message>
        <location filename="../libpgmodeler_ui/src/resultsetmodel.cpp" line="51"/>
        <source>[binary data]</source>
        <translation type="unfinished">[datos binarios]</translation>
    </message>
</context>
<context>
    <name>RoleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="50"/>
        <source>yyyy-MMM-dd hh:mm:ss</source>
        <translation>yyyy-MMM-dd hh:mm:ss</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="88"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="62"/>
        <source>Validity</source>
        <translation>Validez</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="102"/>
        <source>Connections:</source>
        <translation>Conexión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="109"/>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="130"/>
        <source>Superuser</source>
        <translation>Superusuário</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="137"/>
        <source>Inherit permissions</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="144"/>
        <source>Can create database</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="151"/>
        <source>Bypass RLS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="158"/>
        <source>Can use replication</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="165"/>
        <source>Can login</source>
        <translation type="unfinished">Puede login</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="172"/>
        <source>Can create role</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="186"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="68"/>
        <source>Members</source>
        <translation>Miembros</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="191"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="65"/>
        <source>Member of</source>
        <translation>Miembro de</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="196"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="71"/>
        <source>Members (Admin.)</source>
        <translation>Miembros (Admin.)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="204"/>
        <source>Password:</source>
        <translation>Contraseña:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="227"/>
        <source>Encrypted</source>
        <translation>Encriptada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="35"/>
        <source>Assigning &lt;strong&gt;&lt;em&gt;-1&lt;/em&gt;&lt;/strong&gt; to &lt;strong&gt;&lt;em&gt;Connections&lt;/em&gt;&lt;/strong&gt; creates a role without connection limit.&lt;br/&gt;										  Unchecking &lt;strong&gt;&lt;em&gt;Validity&lt;/em&gt;&lt;/strong&gt; creates an role that never expires.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="59"/>
        <source>Role</source>
        <translation>Rol</translation>
    </message>
</context>
<context>
    <name>RuleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="50"/>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="102"/>
        <source>Execution Type:</source>
        <translation>Tipo Ejecución:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="151"/>
        <source>Conditional Expr.:</source>
        <translation>Expr. Condicional:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="158"/>
        <source>Commands</source>
        <translation>Comandos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="191"/>
        <source>SQL Command:</source>
        <translation>Comando SQL:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rulewidget.cpp" line="38"/>
        <source>SQL command</source>
        <translation>Comando SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rulewidget.cpp" line="42"/>
        <source>To create a rule that does not perform any action (&lt;strong&gt;DO NOTHING&lt;/strong&gt;) simply do not specify commands in the SQL commands table.</source>
        <translation>Para crear una regla que no realiza ninguna acción (&lt;strong&gt;DO NOTHING&lt;/strong&gt;) basta não especificar Comandos na tabla de Comandos SQL.</translation>
    </message>
</context>
<context>
    <name>SQLExecutionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="525"/>
        <source>Save SQL commands</source>
        <translation>Guardar comandos SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="186"/>
        <source>Search in SQL code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="71"/>
        <source>Close the current SQL script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="100"/>
        <source>SQL script currently handled</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="121"/>
        <source>(not saved)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="142"/>
        <source>Handle external SQL script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="145"/>
        <source>&amp;Script</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="158"/>
        <source>Alt+F</source>
        <translation>Alt+F</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="189"/>
        <source>Fi&amp;nd</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="233"/>
        <source>Run the specified SQL command</source>
        <translation>Ejecutar un comando SQL especificada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="236"/>
        <source>Run SQL</source>
        <translation>Ejecutar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="249"/>
        <source>F6</source>
        <translation>F6</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="268"/>
        <source>Clear sql input field and results</source>
        <translation>Limpiar campo de entrada de SQL y resultados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="271"/>
        <source>Clear All</source>
        <translation>Limpiar Todos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="300"/>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="344"/>
        <source>Export results to a CSV file</source>
        <translation>Exportar resultados a un acchivo CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="303"/>
        <source>Snippe&amp;ts</source>
        <translation>Snippe&amp;ts</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="316"/>
        <source>Alt+T</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="347"/>
        <source>E&amp;xport</source>
        <translation>E&amp;xportar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="360"/>
        <source>Alt+X</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="385"/>
        <source>Toggles the output pane</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="388"/>
        <source>&amp;Output</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="401"/>
        <source>Alt+O</source>
        <translation type="unfinished">Alt+O</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="475"/>
        <source>Current working database</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="569"/>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="370"/>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="481"/>
        <source>Results</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="646"/>
        <source>Messages</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="693"/>
        <source>History</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="74"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="68"/>
        <source>SQL file (*.sql);;All files (*.*)</source>
        <translation>Archivo SQL (*.sql);;Todos los archivos (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="89"/>
        <source>Load</source>
        <translation type="unfinished">Cargar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="90"/>
        <source>Save</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="91"/>
        <source>Save as</source>
        <translation>Guardar como</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="315"/>
        <source>[binary data]</source>
        <translation>[datos binarios]</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="362"/>
        <source>No results retrieved or changes done due to the error above.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="371"/>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="505"/>
        <source>Messages (%1)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="476"/>
        <source>Results (%1)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="498"/>
        <source>[%1]: SQL command successfully executed in &lt;em&gt;&lt;strong&gt;%2&lt;/strong&gt;&lt;/em&gt;. &lt;em&gt;%3 &lt;strong&gt;%4&lt;/strong&gt;&lt;/em&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="501"/>
        <source>Rows affected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="501"/>
        <source>Rows retrieved</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="554"/>
        <source>Load SQL commands</source>
        <translation>Cargar comandos SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="586"/>
        <source>Save CSV file</source>
        <translation>Guardar archivos CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="587"/>
        <source>Comma-separated values file (*.csv);;All files (*.*)</source>
        <translation>Archivo de valores separados por comas (*.csv);;Todos los archivos (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="623"/>
        <source>The SQL input field and the results grid will be cleared! Want to proceed?</source>
        <translation>El campo de entrada de SQL y la cuadrícula de resultados se borrará! ¿Quieres continuar?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="718"/>
        <source>Copy selection</source>
        <translation>Copiar la selección</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="719"/>
        <source>Plain format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="720"/>
        <source>CVS format</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="888"/>
        <source>This action will wipe out all the SQL commands history for all connections! Do you really want to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="935"/>
        <source>Clear history</source>
        <translation type="unfinished">Limpiar historial</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="936"/>
        <source>Save history</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="937"/>
        <source>Reload history</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="942"/>
        <source>Find in history</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="944"/>
        <source>Hide find tool</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="959"/>
        <source>This action will wipe out all the SQL commands history for the current connection! Do you really want to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SQLToolWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="87"/>
        <source>Database explorer</source>
        <translation>Explorar Base de Datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="147"/>
        <source>Disconnect from all databases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="208"/>
        <source>Update the database list</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="282"/>
        <source>Toggle the object&apos;s attributes grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="288"/>
        <source>Attributes</source>
        <translation type="unfinished">Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="301"/>
        <source>Alt+R</source>
        <translation type="unfinished">Alt+R</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="323"/>
        <source>Toggle the display of source code pane</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="329"/>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="434"/>
        <source>Source code</source>
        <translation type="unfinished">Código fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="150"/>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="211"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="387"/>
        <source>SQL execution</source>
        <translation>Ejecutar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqltoolwidget.cpp" line="187"/>
        <source>Warning</source>
        <translation>Advertencia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqltoolwidget.cpp" line="188"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; Disconnect from all databases will close any opened tab in this view! Do you really want to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SceneInfoWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="20"/>
        <source>Form</source>
        <translation type="unfinished">Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="64"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="104"/>
        <source>Current position of the mouse in the canvas</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="107"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="182"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="257"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="332"/>
        <source>-</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="214"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="254"/>
        <source>Currently selected object(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="289"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="329"/>
        <source>Dimensions of the selected object(s)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="139"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="179"/>
        <source>Current zoom factor</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sceneinfowidget.cpp" line="42"/>
        <source>No selection</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sceneinfowidget.cpp" line="43"/>
        <source>N/A</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sceneinfowidget.cpp" line="69"/>
        <source>Sel. objects: %1</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SchemaWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/schemawidget.ui" line="29"/>
        <source>Fill color:</source>
        <translation>Color de relleno:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/schemawidget.ui" line="48"/>
        <source>Show rectangle</source>
        <translation>Mostrar rectángulo</translation>
    </message>
</context>
<context>
    <name>SequenceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="54"/>
        <source>Cyclic:</source>
        <translation>Cíclico:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="68"/>
        <source>Start:</source>
        <translation>Inicio:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="152"/>
        <source>Maximum:</source>
        <translation>Maximo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="124"/>
        <source>Minimum:</source>
        <translation>Minimo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="44"/>
        <source>Defualt values:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="96"/>
        <source>Increment:</source>
        <translation>Incremento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="180"/>
        <source>Cache:</source>
        <translation>Cache:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="208"/>
        <source>Owner Col.:</source>
        <translation>Dueño Col.:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sequencewidget.cpp" line="35"/>
        <source>User defined</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>SnippetsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="180"/>
        <source>Create new connection</source>
        <translation>Crear nueva conexión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="209"/>
        <source>Cancel edition</source>
        <translation>Cancelar edición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="238"/>
        <source>Edit selected connection</source>
        <translation>Editar conexión seleccionada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="267"/>
        <source>Delete selected connection</source>
        <translation>Eliminar conexión seleccionada</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="302"/>
        <source>Remove All</source>
        <translation>Borrar Todos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="318"/>
        <source>Shift+Del</source>
        <translation>Shift+Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="625"/>
        <source>Filter:</source>
        <translation>Filtrar:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="95"/>
        <source>Label:</source>
        <translation>Etiqueta:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="124"/>
        <source>Applies to:</source>
        <translation>Aplicar a:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="137"/>
        <source>ID:</source>
        <translation>ID:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="515"/>
        <source>Parsable or dynamic snippets are written in the &lt;strong&gt;schema micro language&lt;/strong&gt; syntax. When using a parsable snippet the attributes surrounded in &lt;strong&gt;{}&lt;/strong&gt; will be replaced by the selected object&apos;s matching attributes.</source>
        <translation>Fragmentos analizable o dinámicas están escritos en el sintaxis &lt;strong&gt;esquema micro lenguaje&lt;/strong&gt;. Cuando se utiliza un fragmento parsable los atributos envueltos en &lt;strong&gt;{}&lt;/strong&gt; será reemplazado por atributos coincidentes del objeto seleccionado.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="518"/>
        <source>Parsable</source>
        <translation>Parseable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="572"/>
        <source>When handling parsable snippets empty attributes will be replaced by a value in the format &lt;strong&gt;{attribute}&lt;/strong&gt;. Note that this option can affect the semantics of the resulting snippet.</source>
        <translation>When handling parsable snippets empty attributes will be replaced by a value in the format &lt;strong&gt;{atributo}&lt;/strong&gt;. Tenga en cuenta que esta opción puede afectar a la semántica del fragmento resultante.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="575"/>
        <source>Placeholders</source>
        <translation>Marcadores de posición</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="376"/>
        <source>Parse the snippet in order to check if there are syntax errors.</source>
        <translation>Analizar el fragmento con el fin de comprobar si hay errores de sintaxis.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="379"/>
        <source>Parse</source>
        <translation>Analizar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="417"/>
        <source>Add</source>
        <translation>Adicionar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="455"/>
        <source>Update</source>
        <translation>Actualizar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="333"/>
        <source>Snippets:</source>
        <translation>Fragmentos:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="31"/>
        <source>General purpose</source>
        <translation>Propósito general</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="53"/>
        <source>All snippets</source>
        <translation>Todos los fragmentos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="204"/>
        <source>/* Error parsing the snippet &apos;%1&apos;:

 %2 */</source>
        <translation>/ * Error al analizar el fragmento &apos;%1&apos;:

 %2 */</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="226"/>
        <source>Duplicated snippet id &lt;strong&gt;%1&lt;/strong&gt; detected. Please, specify a different one!</source>
        <translation>Duplicado Identificación del fragmento &lt;strong&gt;%1&lt;/strong&gt; detectado. Por favor, especifique uno diferente!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="228"/>
        <source>Invalid ID pattern detected &lt;strong&gt;%1&lt;/strong&gt;. This one must start with at leat one letter and be composed by letters, numbers and/or underscore!</source>
        <translation>Patrón de ID no válido detectado &lt;strong&gt;%1&lt;/strong&gt;. Éste debe comenzar con una letra en del acueducto y estar compuesta por letras, números y/o destacar!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="230"/>
        <source>Empty label for snippet &lt;strong&gt;%1&lt;/strong&gt;. Please, specify a value for it!</source>
        <translation>Etiqueta vacía para el fragmento &lt;strong&gt;%1&lt;/strong&gt;. Por favor, especifique un valor para él!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="232"/>
        <source>Empty code for snippet &lt;strong&gt;%1&lt;/strong&gt;. Please, specify a value for it!</source>
        <translation>Código vacío para el fragmento &lt;strong&gt;%1&lt;/strong&gt;. Por favor, especifique un valor para él!</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="248"/>
        <source>The dynamic snippet contains syntax error(s). Additional info: &lt;br/&gt;&lt;em&gt;%1&lt;/em&gt;</source>
        <translation>El fragmento de código dinámico contiene error de sintaxis (s). Información adicional: &lt;br/&gt;&lt;em&gt;%1&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="366"/>
        <source>Do you really want to remove all snippets?</source>
        <translation>¿De verdad quiere eliminar todos los fragmentos?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="436"/>
        <source>No syntax errors found in the snippet.</source>
        <translation>No hay errores de sintaxis encontrados en el fragmento.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="534"/>
        <source>General</source>
        <translation>General</translation>
    </message>
</context>
<context>
    <name>SourceCodeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="49"/>
        <source>Version:</source>
        <translation>Versión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="95"/>
        <source>PostgreSQL</source>
        <translation>PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="146"/>
        <source>iconecodigo</source>
        <translation>iconecodigo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="162"/>
        <source>SQL</source>
        <translation>SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="185"/>
        <source>Code display:</source>
        <translation>Visualización de Código:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="202"/>
        <source>Original</source>
        <translation>Originales</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="207"/>
        <source>Original + depedencies&apos; SQL</source>
        <translation>Originales + SQL dependientes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="212"/>
        <source>Original + children&apos;s SQL</source>
        <translation>Originales + SQL hijos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="255"/>
        <source>Save the SQL code to a file.</source>
        <translation>Guardar el código SQL code para un archivo.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="258"/>
        <source>Save SQL</source>
        <translation>Guardar SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="291"/>
        <source>XML</source>
        <translation>XML</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="14"/>
        <source>Source code visualization</source>
        <translation>Visualización del código fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="46"/>
        <source>&lt;strong&gt;Original:&lt;/strong&gt; displays only the original object&apos;s SQL code.&lt;br/&gt;&lt;br/&gt;						   &lt;strong&gt;Dependencies:&lt;/strong&gt; displays the original code including all dependencies needed to properly create the selected object.&lt;br/&gt;&lt;br/&gt;						   &lt;strong&gt;Children:&lt;/strong&gt; displays the original code including all object&apos;s children SQL code. This option is used only by schemas, tables and views.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="94"/>
        <source>Save SQL code as...</source>
        <translation>Guardar código SQL como...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="99"/>
        <source>SQL code (*.sql);;All files (*.*)</source>
        <translation>Código SQL (*.sql);;Todos los archivos (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="143"/>
        <source>Generating source code...</source>
        <translation>Generación de código fuente...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="162"/>
        <source>-- NOTE: the code below contains the SQL for the selected object
-- as well for its dependencies and children (if applicable).
-- 
-- This feature is only a convinience in order to permit you to test
-- the whole object&apos;s SQL definition at once.
-- 
-- When exporting or generating the SQL for the whole database model
-- all objects will be placed at their original positions.


</source>
        <translation>-- NOTA: el código de abajo contiene el SQL para el objeto seleccionado
-- así como para sus dependencias y los hijos (en su caso).
--
-- Esta función sólo una conveniencia a fin de permitir que probar
-- Definición SQL el conjunto de objetos a la vez.
--
-- Al exportar o generar el SQL para todo el modelo de base de datos
-- Todos los objetos serán colocados en sus posiciones originales.


</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="181"/>
        <source>

-- SQL code purposely truncated at this point in demo version!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="190"/>
        <source>-- SQL code unavailable for this type of object --</source>
        <translation>-- Código SQL disponible para este tipo de objeto --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="194"/>
        <source>&lt;!-- XML code preview disabled in demonstration version --&gt;</source>
        <translation>&lt;!-- Código XML previsualización desactivado en la versión de demostración --&gt;</translation>
    </message>
</context>
<context>
    <name>SwapObjectsIdsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="20"/>
        <source>Change objects creation order</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="38"/>
        <source>Create:</source>
        <translation>Crear:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="63"/>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="138"/>
        <source>ID:</source>
        <translation>ID:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="113"/>
        <source>Before:</source>
        <translation>Anterior:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="264"/>
        <source>Change the objects creation order is an irreversible operation and cause the operations history to be automatically erased. Note that the creation order configured in this form is not definitive and may change after a model validation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="360"/>
        <source>Swap the object ids changing their creation order</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="363"/>
        <source>Swap ids</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="413"/>
        <source>Filter:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="502"/>
        <source>ID</source>
        <translation type="unfinished">ID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="514"/>
        <source>Object</source>
        <translation type="unfinished">Objecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="526"/>
        <source>Type</source>
        <translation type="unfinished">Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="538"/>
        <source>Parent Object</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="550"/>
        <source>Parent Type</source>
        <translation type="unfinished">Tipo del Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="307"/>
        <source>Swap the values of the fields</source>
        <translation>Cambie los valores de los campos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="310"/>
        <source>Swap values</source>
        <translation>Cambie los valores</translation>
    </message>
</context>
<context>
    <name>Table</name>
    <message>
        <location filename="../libpgmodeler/src/table.cpp" line="46"/>
        <source>new_table</source>
        <translation>nueva_tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/table.cpp" line="343"/>
        <source>In demonstration version tables can have only `%1&apos; instances of each child object type or ancestor tables! You&apos;ve reach this limit for the type: `%2&apos;</source>
        <translation>En la demostración mesas versión sólo pueden tener `%1&apos; instancias de cada tipo de objeto hijo o antepasado de la tabla! Tienes que llegar a este límite para el tipo: `%2&apos;</translation>
    </message>
</context>
<context>
    <name>TableDataWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="14"/>
        <source>Edit table data</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="110"/>
        <source>Copy items on the grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="113"/>
        <source>Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="145"/>
        <source>Add empty rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="148"/>
        <source>Add row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="161"/>
        <source>Ins</source>
        <translation type="unfinished">Ins</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="183"/>
        <source>Delete column</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="220"/>
        <source>Paste items on the grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="223"/>
        <source>Paste</source>
        <translation type="unfinished">Pegar</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="236"/>
        <source>Ctrl+V</source>
        <translation type="unfinished">Ctrl+V</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="258"/>
        <source>Fills the grid using a CSV file</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="360"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Some invalid or duplicated columns were detected. In order to solve this issue double-click the header of the highlighted ones in order to define the correct name in which the data belongs to or delete the entire column. Note that these columns are completely ignored when generating the &lt;span style=&quot; font-weight:600;&quot;&gt;INSERT&lt;/span&gt; commands.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="385"/>
        <source>Add column</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="420"/>
        <source>Duplicate rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="470"/>
        <source>Change the values of all selected cells at once</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="473"/>
        <source>Bulk data edit</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="486"/>
        <source>Ctrl+E</source>
        <translation type="unfinished">Ctrl+E</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="547"/>
        <source>Delete all columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="582"/>
        <source>Delete rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="681"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Empty values are assumed as &lt;span style=&quot; font-weight:600;&quot;&gt;DEFAULT&lt;/span&gt;. To use special values like &lt;span style=&quot; font-weight:600;&quot;&gt;NULL&lt;/span&gt;, a function call like &lt;span style=&quot; font-weight:600;&quot;&gt;now()&lt;/span&gt; or a specific data escaping, enclose values in two slashes, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;/value/&lt;/span&gt;. To use a slash as part of the value prepend the backslash character, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;\/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="382"/>
        <source>Add an empty column</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="703"/>
        <source>Remove all rows from the grid preserving columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="706"/>
        <source>Delete all rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="719"/>
        <source>Shift+Del</source>
        <translation type="unfinished">Shift+Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="579"/>
        <source>Delete the selected rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="196"/>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="595"/>
        <source>Del</source>
        <translation type="unfinished">Del</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="417"/>
        <source>Duplicate the selected rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="433"/>
        <source>Ctrl+D</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="180"/>
        <source>Delete the selected columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="544"/>
        <source>Remove all columns (and rows) from the grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="560"/>
        <source>Ctrl+Shift+Del</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="175"/>
        <source>Delete columns is an irreversible action! Do you really want to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="210"/>
        <source>Remove all rows is an irreversible action! Do you really want to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="225"/>
        <source>Remove all columns is an irreversible action! Do you really want to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="262"/>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="377"/>
        <source>Unknown column</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="379"/>
        <source>Duplicated column</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>TableObjectView</name>
    <message>
        <location filename="../libobjrenderer/src/tableobjectview.cpp" line="208"/>
        <source>
Relationship: %1</source>
        <translation>
Relación:  %1</translation>
    </message>
</context>
<context>
    <name>TableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="38"/>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="91"/>
        <source>Tag:</source>
        <translation>Tag:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="56"/>
        <source>With OID</source>
        <translation>Con OIDs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="133"/>
        <source>Generate ALTER for columns/constraints</source>
        <translation>Generar ALTER para columnas/restricciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="72"/>
        <source>Unlogged</source>
        <translation>Invitados</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="104"/>
        <source>Enable row level security</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="120"/>
        <source>Force RLS for owner</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="147"/>
        <source>&amp;Columns</source>
        <translation>&amp;Columnas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="152"/>
        <source>Co&amp;nstraints</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="157"/>
        <source>Tri&amp;ggers</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="162"/>
        <source>&amp;Rules</source>
        <translation>&amp;Reglas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="167"/>
        <source>&amp;Indexes</source>
        <translation>&amp;Indíces</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="172"/>
        <source>&amp;Policies</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="177"/>
        <source>&amp;Tables</source>
        <translation>&amp;Tablas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="43"/>
        <source>Edit data</source>
        <translation>Editar datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="44"/>
        <source>Define initial data for the table</source>
        <translation>Definir los datos iniciales de la tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="60"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="98"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="121"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="129"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="138"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="144"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="149"/>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="62"/>
        <source>Schema</source>
        <translation>Esquema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="64"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="100"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="123"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="97"/>
        <source>PK</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="102"/>
        <source>Default Value</source>
        <translation>Valor por defecto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="103"/>
        <source>Attribute(s)</source>
        <translation>Atributo(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="114"/>
        <source>It is not possible to mark a column as primary key when the table already has a primary key which was created by a relationship! This action should be done in the section &lt;strong&gt;Primary key&lt;/strong&gt; of the relationship&apos;s editing form.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="116"/>
        <source>It is not possible to mark a column created by a relationship as primary key! This action should be done in the section &lt;strong&gt;Primary key&lt;/strong&gt; of the relationship&apos;s editing form.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="125"/>
        <source>ON DELETE</source>
        <translation>ON DELETE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="126"/>
        <source>ON UPDATE</source>
        <translation>ON UPDATE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="131"/>
        <source>Refer. Table</source>
        <translation>Tabla Refer </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="133"/>
        <source>Firing</source>
        <translation>Firing</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="135"/>
        <source>Events</source>
        <translation>Eventos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="140"/>
        <source>Execution</source>
        <translation>Ejecución</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="141"/>
        <source>Event</source>
        <translation>Evento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="146"/>
        <source>Indexing</source>
        <translation>Indezado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="151"/>
        <source>Command</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="153"/>
        <source>Permissive</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="154"/>
        <source>USING expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="155"/>
        <source>CHECK expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="156"/>
        <source>Roles</source>
        <translation type="unfinished">Roles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="249"/>
        <source>Parent</source>
        <translation>Padre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="258"/>
        <source>Copy</source>
        <translation>Copia</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="511"/>
        <source>Yes</source>
        <translation type="unfinished">Si</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="511"/>
        <source>No</source>
        <translation type="unfinished">No</translation>
    </message>
</context>
<context>
    <name>TablespaceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablespacewidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablespacewidget.ui" line="38"/>
        <source>Directory:</source>
        <translation>Directorio:</translation>
    </message>
</context>
<context>
    <name>TagWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="29"/>
        <source>Colors</source>
        <translation>Colores</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="75"/>
        <source>Extended body:</source>
        <translation>Cuerpo extendido:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="47"/>
        <source>Body:</source>
        <translation>Cuerpo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="54"/>
        <source>Title:</source>
        <translation>Título:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="61"/>
        <source>Schema name:</source>
        <translation>Nombre esquema:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="68"/>
        <source>Table name:</source>
        <translation>Nombre tabla:</translation>
    </message>
</context>
<context>
    <name>TaskProgressWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/taskprogresswidget.ui" line="35"/>
        <source>Executing tasks</source>
        <translation>Ejecutando tareas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/taskprogresswidget.ui" line="137"/>
        <location filename="../libpgmodeler_ui/src/taskprogresswidget.cpp" line="52"/>
        <source>Waiting task to start...</source>
        <translation>Esperando tarea para empezar...</translation>
    </message>
</context>
<context>
    <name>TextboxWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="35"/>
        <source>Font:</source>
        <translation>Fuente:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="42"/>
        <source>Text</source>
        <translation>Texto</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="93"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="102"/>
        <source>Color:</source>
        <translation>Color:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="200"/>
        <source>Bold</source>
        <translation>Negrita</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="207"/>
        <source>Italic</source>
        <translation>Itálica</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="220"/>
        <source>Underline</source>
        <translation>Subrayado</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/textboxwidget.cpp" line="55"/>
        <source>Select text color</source>
        <translation>Seleccionar color del texto</translation>
    </message>
</context>
<context>
    <name>TriggerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="46"/>
        <source>Constraint</source>
        <translation>Restricción</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="74"/>
        <source>Deferrable:</source>
        <translation>Diferible:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="169"/>
        <source>FOR EACH ROW</source>
        <translation>FOR EACH ROW</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="196"/>
        <source>Event:</source>
        <translation>Evento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="226"/>
        <source>INSERT</source>
        <translation>INSERT</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="251"/>
        <source>DELETE</source>
        <translation>DELETE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="276"/>
        <source>UPDATE</source>
        <translation>UPDATE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="301"/>
        <source>TRUNCATE</source>
        <translation>TRUNCATE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="331"/>
        <source>Refer. Table:</source>
        <translation>Tabla Refer.:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="356"/>
        <source>Condition:</source>
        <translation>Condición:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="367"/>
        <source>Arguments</source>
        <translation>Argumentos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="388"/>
        <source>Argument:</source>
        <translation>Argumento:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="409"/>
        <source>Columns</source>
        <translation>Columnas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="442"/>
        <source>Column:</source>
        <translation>Columna:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="496"/>
        <source>Function:</source>
        <translation>Función:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="524"/>
        <source>Options:</source>
        <translation>Opciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="552"/>
        <source>Excution:</source>
        <translation>Ejecución:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/triggerwidget.cpp" line="47"/>
        <source>Column</source>
        <translation>Columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/triggerwidget.cpp" line="49"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
</context>
<context>
    <name>TypeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="44"/>
        <source>Range</source>
        <translation>Rango</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="55"/>
        <source>Functions</source>
        <translation>Funciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="76"/>
        <source>INPUT:</source>
        <translation>INPUT:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="83"/>
        <source>OUTPUT:</source>
        <translation>OUTPUT:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="90"/>
        <source>RECV:</source>
        <translation>RECV:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="97"/>
        <source>SEND:</source>
        <translation>SEND:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="104"/>
        <source>TPMOD_IN:</source>
        <translation>TPMOD_IN:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="111"/>
        <source>TPMOD_OUT:</source>
        <translation>TPMOD_OUT:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="118"/>
        <source>ANALYZE:</source>
        <translation>ANALYZE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="126"/>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="437"/>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="490"/>
        <source>Attributes</source>
        <translation>Atributos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="147"/>
        <source>Internal Length:</source>
        <translation>Longtud Interna:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="170"/>
        <source>Storage:</source>
        <translation>Almacenar:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="180"/>
        <source>Options:</source>
        <translation>Opciones:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="195"/>
        <source>By value</source>
        <translation>Por valor</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="208"/>
        <source>Preferred</source>
        <translation>Preferida</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="215"/>
        <source>Collatable</source>
        <translation>Collatable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="224"/>
        <source>Category:</source>
        <translation>Categoría:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="234"/>
        <source>Delimiter:</source>
        <translation>Delimitador:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="248"/>
        <source>Default Value:</source>
        <translation>Valor por Defecto:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="258"/>
        <source>Alignment:</source>
        <translation>Aliniación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="278"/>
        <source>integer</source>
        <translation>integer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="283"/>
        <source>char</source>
        <translation>char</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="288"/>
        <source>smallint</source>
        <translation>smallint</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="293"/>
        <source>double precision</source>
        <translation>double precision</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="324"/>
        <source>Configuration:</source>
        <translation>Configuración:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="337"/>
        <source>Base Type</source>
        <translation>Tipo Base</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="353"/>
        <source>Enumeration</source>
        <translation>Enumeración</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="372"/>
        <source>Co&amp;mposite</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="391"/>
        <source>Enumerations</source>
        <translation>Enumeraciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="412"/>
        <source>Enumeration:</source>
        <translation>Enumeration:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="458"/>
        <source>Name:</source>
        <translation>Nombre:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="474"/>
        <source>Collation:</source>
        <translation>Colación:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="511"/>
        <source>Subtype Diff Func.:</source>
        <translation>Subtipo Diff Func .:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="518"/>
        <source>Operator Class:</source>
        <translation>Operador de Clase:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="525"/>
        <source>Canonical Func.:</source>
        <translation>Func. Canónica:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="33"/>
        <source>Like Type</source>
        <translation>Como Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="34"/>
        <source>Element Type</source>
        <translation>Elemento Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="35"/>
        <source>Subtype</source>
        <translation>Subtipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="58"/>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="60"/>
        <source>Type</source>
        <translation>Tipo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="62"/>
        <source>Collation</source>
        <translation>Colación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="77"/>
        <source>The functions to be assigned to a type should be written in C language and possess, respectively, the following signatures:&lt;br/&gt;  &lt;table&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;INPUT:&lt;/strong&gt; &lt;em&gt;any function(cstring, oid, integer)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;OUTPUT:&lt;/strong&gt; &lt;em&gt;cstring function(any)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;SEND:&lt;/strong&gt; &lt;em&gt;byta function(any)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;RECV:&lt;/strong&gt; &lt;em&gt;any function(internal, oid, integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_IN:&lt;/strong&gt; &lt;em&gt;integer function(cstring[])&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_OUT:&lt;/strong&gt; &lt;em&gt;cstring function(integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;ANALYZE:&lt;/strong&gt; &lt;em&gt;boolean function(internal)&lt;/em&gt;&lt;/td&gt;    &lt;tr&gt;  &lt;/table&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="95"/>
        <source>The functions to be assigned to a range type should have the following signatures:&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonical:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</source>
        <translation>Las funciones que se asigna a un tipo de rango deben tener las siguientes firmas:&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonical:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>UpdateNotifierWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="26"/>
        <source>Update Notifier</source>
        <translation>Notificación de Actualizaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="131"/>
        <source>Hide this widget</source>
        <translation>Ocultar este widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="134"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="168"/>
        <source>Released in:</source>
        <translation>Liberado en:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="188"/>
        <source>mmm dd, yyyy</source>
        <translation>mmm dd, yyyy</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="81"/>
        <source>Update found!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="217"/>
        <source>New version:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="236"/>
        <source>0.0.0</source>
        <translation>0.0.0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="245"/>
        <source>Changelog</source>
        <translation>Historial de cambios</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="309"/>
        <source>Redirects to purchase page.</source>
        <translation>Redirige a página de comprar.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="312"/>
        <source>Get binary package</source>
        <translation>Obtener paquete binario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="344"/>
        <source>Redirects to GitHub source repository.</source>
        <translation>Redirige al repositorio de fuentes GitHub.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="347"/>
        <source>Get source code</source>
        <translation>Obtener el código fuente</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="115"/>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="162"/>
        <source>Failed to check updates</source>
        <translation>No se ha podido comprobar las actualizaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="116"/>
        <source>The update notifier failed to check for new versions! Please, verify your internet connectivity and try again! Connection error returned: &lt;em&gt;%1&lt;/em&gt; - &lt;strong&gt;%2&lt;/strong&gt;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="153"/>
        <source>No updates found</source>
        <translation>No se encontraron actualizaciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="154"/>
        <source>You are running the most recent pgModeler version! No update needed.</source>
        <translation>Está ejecutando la versión más reciente pgModeler! Ninguna actualización necesaria.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="163"/>
        <source>The update notifier failed to check for new versions! A HTTP status code was returned: &lt;strong&gt;%1&lt;/strong&gt;</source>
        <translation>El notificador de actualización no pudo comprobar si hay nuevas versiones! Un código de estado HTTP fue devuelto: &lt;strong&gt;%1&lt;/strong&gt;</translation>
    </message>
</context>
<context>
    <name>ViewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="33"/>
        <source>References</source>
        <translation>Referencias</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="161"/>
        <source>Expression Alias:</source>
        <translation>Alias Expresión: </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="118"/>
        <source>Column:</source>
        <translation>Columna:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="88"/>
        <source>Table:</source>
        <translation>Tabla:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="101"/>
        <source>Table Alias:</source>
        <translation>Alias Tabla:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="81"/>
        <source>Used in:</source>
        <translation>Usado en:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="68"/>
        <source>Column</source>
        <translation>Columna</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="73"/>
        <source>Expression</source>
        <translation>Expresión</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="60"/>
        <source>Reference Type:</source>
        <translation>Tipo de Referencia:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="256"/>
        <source>View Definition</source>
        <translation>Definición de Vista</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="148"/>
        <source>Expression:</source>
        <translation>Expresión:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="131"/>
        <source>Column Alias:</source>
        <translation>Alias de Columna:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="199"/>
        <source>The element will be used as part of the SELECT statement to retrieve columns or expressions that will compose the view&apos;s columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="202"/>
        <source>SELECT ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="215"/>
        <source>The element will be used as part of the WHERE clause in form of conditional expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="218"/>
        <source>WHERE ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="237"/>
        <source>The element is used in the FROM portion of the command in order to reference tables or construct JOIN statements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="240"/>
        <source>FROM ...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="253"/>
        <source>The element&apos;s expression is used exclusively as the view&apos;s definition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="269"/>
        <source>The element will be appended to the very end of the view&apos;s definition. This is useful when using GROUP BY/HAVING statements</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="272"/>
        <source>End expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="295"/>
        <source>Triggers</source>
        <translation>Triggers</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="300"/>
        <source>Rules</source>
        <translation>Reglas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="305"/>
        <source>Indexes</source>
        <translation type="unfinished">Índices</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="310"/>
        <source>Table Expression</source>
        <translation>Expresión de tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="315"/>
        <source>Code Preview</source>
        <translation>Vista previa código</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="323"/>
        <source>Options</source>
        <translation>Opciones</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="347"/>
        <source>Tag:</source>
        <translation>Tag:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="360"/>
        <source>Mode:</source>
        <translation>Modo:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="369"/>
        <source>Ordinary</source>
        <translation>Ordinario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="379"/>
        <source>Recursi&amp;ve</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="389"/>
        <source>&amp;Materialized</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="405"/>
        <source>With no data</source>
        <translation>Sin datos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="69"/>
        <source>Col./Expr.</source>
        <translation>Col./Expr.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="70"/>
        <source>Alias</source>
        <translation>Alias</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="71"/>
        <source>Alias Col.</source>
        <translation>Alias Col.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="72"/>
        <source>Flags: SF FW AW EX VD</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="77"/>
        <source>To reference all columns in a table (*) just do not fill the field &lt;strong&gt;Column&lt;/strong&gt;, this is the same as write &lt;em&gt;&lt;strong&gt;[schema].[table].*&lt;/strong&gt;&lt;/em&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="105"/>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="114"/>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="119"/>
        <source>Name</source>
        <translation>Nombre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="107"/>
        <source>Refer. Table</source>
        <translation>Refer. Tabla</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="109"/>
        <source>Firing</source>
        <translation>Firing</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="111"/>
        <source>Events</source>
        <translation>Eventos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="116"/>
        <source>Indexing</source>
        <translation>Indexación</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="121"/>
        <source>Execution</source>
        <translation>Ejecución</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="122"/>
        <source>Event</source>
        <translation>Evento</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="710"/>
        <source>/* Could not generate the SQL code. Make sure all attributes are correctly filled! </source>
        <translation>/ * No se pudo generar el código SQL. Asegúrese de que todos los atributos se rellenan correctamente! </translation>
    </message>
</context>
<context>
    <name>WelcomeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="23"/>
        <source>Form</source>
        <translation type="unfinished">Formulario</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="97"/>
        <source>New model</source>
        <translation type="unfinished">Nuevo modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="135"/>
        <source>Open model</source>
        <translation type="unfinished">Abrir modelo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="176"/>
        <source>Sample models</source>
        <translation>Ejemplo de modelos</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="220"/>
        <source>Recent models</source>
        <translation>Modelos recientes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="265"/>
        <source>Last session</source>
        <translation>Última sesión</translation>
    </message>
</context>
</TS>
