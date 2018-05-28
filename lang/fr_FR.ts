<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr" sourcelanguage="en_US">
<context>
    <name>AboutWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="35"/>
        <source>About pgModeler</source>
        <translation>À propos de pgModeler</translation>
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
        <translation>Modeleur de base de données PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="362"/>
        <source>Open source data modeling tool designed for PostgreSQL. No more DDL commands written by hand, let pgModeler do the job for you! This software reunites the concepts of entity-relationship diagrams and the features that PostgreSQL implements as extensions of SQL standards. </source>
        <translation>Outil open source de modélisation de bases de données PostgreSQL. Plus aucun ordre LDD à saisir à la main, laissez pgModeler le faire pour vous ! Ce logiciel fusionne le concept de modélisation entité-association et toutes les fonctionnalités étendues que propose PostgreSQL en plus des standards SQL. </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="486"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Copyright 2006-2018 - Raphael Araújo e Silva &amp;lt;&lt;a href=&quot;mailto:raphael@pgmodeler.com.br&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;raphael@pgmodeler.io&lt;/span&gt;&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="461"/>
        <source>pgModeler is proudly a brazilian software!</source>
        <translation>pgModeler est un logiciel brésilien et fier de l&apos;être !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="445"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;a href=&quot;http://pgmodeler.com.br&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#2980b9;&quot;&gt;https://pgmodeler.io&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="312"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="244"/>
        <source>(BUILD_NUM)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="384"/>
        <source>License</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="309"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
</context>
<context>
    <name>AggregateWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="38"/>
        <source>Final Function:</source>
        <translation>Fonction finale :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="45"/>
        <source>Sort Operator:</source>
        <translation>Opérateur de tri :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="52"/>
        <source>Initial Condition:</source>
        <translation>Condition initiale :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="75"/>
        <source>Funtion Inputs</source>
        <translation>Entrées de la fonction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="80"/>
        <source>Function State</source>
        <translation>État de la fonction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="88"/>
        <source>Transition Func.:</source>
        <translation>Fonction de transition :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="37"/>
        <source>Input Data Type</source>
        <translation>Type de donnée en entrée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="38"/>
        <source>State Data Type</source>
        <translation>Type de donnée d&apos;état</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="65"/>
        <source>An aggregate function that accepts the types &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; and &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; as input types and which type of state is &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt;&lt;/em&gt;, must obey the following rules: &lt;br/&gt;&lt;br/&gt; &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Final Function:&lt;/strong&gt; &lt;em&gt;void final_function(&lt;strong&gt;state_type&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Transition Function:&lt;/strong&gt; &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt; transition_function(&lt;strong&gt;state_type&lt;/strong&gt;, &lt;strong&gt;typeA&lt;/strong&gt;, &lt;strong&gt;typeB&lt;/strong&gt;)&lt;/em&gt;</source>
        <translation>Une fonction d&apos;agrégat qui accepte les types &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; et &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; en types d&apos;entrée et dont le type d&apos;état est &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt;&lt;/em&gt;, vous devez respecter les règles suivantes : &lt;br/&gt; &lt;strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;• Fonction finale : &lt;/strong&gt;&lt;em&gt;void final_function (&lt;strong&gt;state_type&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;• Fonction de transition: &lt;/strong&gt;&lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt; transition_function(&lt;strong&gt;state_type&lt;/strong&gt;,&lt;strong&gt;typeA&lt;/strong&gt;, &lt;strong&gt;typeB&lt;/strong&gt;)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>AppearanceConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="59"/>
        <source>Element:</source>
        <translation>Élément :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="67"/>
        <source>Global: Font style</source>
        <translation>Global : Style de police</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="72"/>
        <source>Global: Constraints descriptor</source>
        <translation>Global : Marqueurs de contraintes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="77"/>
        <source>Global: Object selection</source>
        <translation>Global : Sélection d&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="82"/>
        <source>Global: Position hint text</source>
        <translation>Global : Texte de la boite de coordonnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="87"/>
        <source>Global: Position hint box</source>
        <translation>Global : Fond de la boite de coordonnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="92"/>
        <source>Global: Objects type</source>
        <translation>Global : Type de colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="97"/>
        <source>Global: Lock arc</source>
        <translation>Global : Cadenas (objet verrouillé, couleur de l&apos;arceau)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="102"/>
        <source>Global: Lock body</source>
        <translation>Global : Cadenas (objet verrouillé, couleur du corps)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="107"/>
        <source>Table: Schema name</source>
        <translation>Table : Nom du schéma</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="112"/>
        <source>Table: Table name</source>
        <translation>Table : Nom de la table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="117"/>
        <source>Table: Columns box</source>
        <translation>Table : Zone des colonnes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="122"/>
        <source>Table: Extended attributes box</source>
        <translation>Table : Zone d&apos;attributs étendus</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="127"/>
        <source>Table: Title box</source>
        <translation>Table : Zone de titre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="132"/>
        <source>Rule: Name</source>
        <translation>Règle : Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="137"/>
        <source>Rule: Descriptor</source>
        <translation>Règle : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="142"/>
        <source>Index: Name</source>
        <translation>Index : Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="147"/>
        <source>Index: Descriptor</source>
        <translation>Index : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="152"/>
        <source>Trigger: Name</source>
        <translation>Déclencheur : Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="157"/>
        <source>Trigger: Descriptor</source>
        <translation>Déclencheur : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="172"/>
        <source>View: Schema name</source>
        <translation>Vue : Nom du schéma</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="177"/>
        <source>View: View name</source>
        <translation>Vue : Nom de la vue</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="182"/>
        <source>View: References box</source>
        <translation>Vue : Couleur d&apos;arrière plan</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="187"/>
        <source>View: Extended attributes box</source>
        <translation>Vue : Dialogue des attributs étendus</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="192"/>
        <source>View: Title box</source>
        <translation>Vue :  Couleur d&apos;arrière plan de la barre de titre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="197"/>
        <source>View: Table / columns alias</source>
        <translation>Vue : Alias de table / colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="202"/>
        <source>View: Referenced column</source>
        <translation>Vue : Colonne référencée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="207"/>
        <source>View: Referenced table</source>
        <translation>Vue : Table référencée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="212"/>
        <source>View: Reference descriptor</source>
        <translation>Vue : Couleur de l&apos;icône des références</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="217"/>
        <source>Textbox: Body</source>
        <translation>Zone de texte : Corps</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="222"/>
        <source>Column: Column name</source>
        <translation>Colonne : Nom de la colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="227"/>
        <source>Column: Descriptor</source>
        <translation>Colonne : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="232"/>
        <source>Column: Included / Inherited by relationship</source>
        <translation>Colonne : Incluses / Héritées par associations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="237"/>
        <source>Column: Protected</source>
        <translation>Colonne : Protégée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="242"/>
        <source>Column (pk): Column name</source>
        <translation>Colonne (pk) : Nom de la colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="247"/>
        <source>Column (pk): Descriptor</source>
        <translation>Colonne (pk) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="252"/>
        <source>Column (fk): Column name</source>
        <translation>Colonne (fk) : Nom de la colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="257"/>
        <source>Column (fk): Descriptor</source>
        <translation>Colonne (fk) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="262"/>
        <source>Column (uq): Column name</source>
        <translation>Colonne (uq) : Nom de la colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="267"/>
        <source>Column (uq): Descriptor</source>
        <translation>Colonne (uq) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="272"/>
        <source>Column (nn): Column name</source>
        <translation>Colonne (nn) : Nom de la colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="277"/>
        <source>Column (nn): Descriptor</source>
        <translation>Colonne (nn) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="282"/>
        <source>Relationship: Descriptor</source>
        <translation>Associations : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="287"/>
        <source>Relationship: Label text</source>
        <translation>Associations : Texte de l&apos;étiquette</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="292"/>
        <source>Relationship: Label box</source>
        <translation>Relations : Arrière plan de l&apos;étiquette</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="297"/>
        <source>Relationship: Attribute text</source>
        <translation>Relations : Texte des attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="302"/>
        <source>Relationship: Attribute descriptor</source>
        <translation>Relations : Couleur de l&apos;icône des attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="307"/>
        <source>Tag: Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="312"/>
        <source>Tag: Body</source>
        <translation>Corps</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="331"/>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="385"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="438"/>
        <source>Bold</source>
        <translation>Gras</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="419"/>
        <source>Italic</source>
        <translation>Italique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="344"/>
        <source>Colors:</source>
        <translation>Couleur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="400"/>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="317"/>
        <source>Placeholder: Body</source>
        <translation>Placeholder : Corps</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="162"/>
        <source>Constraint: Name</source>
        <translation>Contrainte : Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="167"/>
        <source>Constraint: Descriptor</source>
        <translation>Contrainte : Descripteur</translation>
    </message>
</context>
<context>
    <name>Application</name>
    <message>
        <location filename="../main/src/application.cpp" line="141"/>
        <source>Unknown exception caught!</source>
        <translation>Exception inconnue générée !</translation>
    </message>
    <message>
        <location filename="../main/src/application.cpp" line="161"/>
        <source>Failed to create initial configuration in `%1&apos;! Check if the current user has write permission over that path and at least read permission over `%2&apos;.</source>
        <translation>Impossible de créer les fichiers de configuration initiaux dans `%1&apos; ! Vérifiez que le dossier existe que vous avez la permission d&apos;écriture dans `%2&apos;.</translation>
    </message>
</context>
<context>
    <name>BaseConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/baseconfigwidget.cpp" line="140"/>
        <source>A backup of the previous settings was saved into &lt;strong&gt;%1&lt;/strong&gt;!</source>
        <translation>Une sauvegarde des paramètres précédents a été enregistrée dans &lt;strong&gt;%1&lt;/strong&gt; !</translation>
    </message>
</context>
<context>
    <name>BaseForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="23"/>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="141"/>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="31"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="36"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="188"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="130"/>
        <source>%1 properties</source>
        <translation>%1 propriétés</translation>
    </message>
</context>
<context>
    <name>BaseObject</name>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="41"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="41"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="41"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="42"/>
        <source>Trigger</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="42"/>
        <source>Index</source>
        <translation>Index</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="42"/>
        <source>Rule</source>
        <translation>Règle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="43"/>
        <source>Table</source>
        <translation>Table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="43"/>
        <source>View</source>
        <translation>Vue</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="43"/>
        <source>Domain</source>
        <translation>Domaine</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="44"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="44"/>
        <source>Aggregate</source>
        <translation>Fonction d&apos;agrégat</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="44"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="45"/>
        <source>Sequence</source>
        <translation>Séquence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="45"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="45"/>
        <source>Conversion</source>
        <translation>Convertion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Cast</source>
        <translation>Convertion de type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Language</source>
        <translation>Langage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="47"/>
        <source>Operator Family</source>
        <translation>Famille d&apos;opérateurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="47"/>
        <source>Operator Class</source>
        <translation>Classe d&apos;opérateurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="48"/>
        <source>Database</source>
        <translation>Base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="48"/>
        <source>Extension</source>
        <translation>Extension</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="49"/>
        <source>Relationship</source>
        <translation>Relation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="48"/>
        <source>Collation</source>
        <translation>Collation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Textbox</source>
        <translation>Boite de texte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="49"/>
        <source>Event Trigger</source>
        <translation>Déclencheur sur évènement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="49"/>
        <source>Policy</source>
        <translation>Politique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Permission</source>
        <translation>Permission</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Parameter</source>
        <translation>Paramètre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="50"/>
        <source>Type Attribute</source>
        <translation>Attribut de type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="51"/>
        <source>Basic Relationship</source>
        <translation>Relation simple</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="51"/>
        <source>Tag</source>
        <translation>Balise</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="51"/>
        <source>Generic SQL</source>
        <translation>SQL générique</translation>
    </message>
    <message numerus="yes">
        <location filename="../libpgmodeler/src/baseobject.cpp" line="97"/>
        <source>new_object</source>
        <translation>
            <numerusform>nouvel_objet</numerusform>
            <numerusform>nouveaux_objets</numerusform>
        </translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="46"/>
        <source>Tablespace</source>
        <translation>Espace de stockage</translation>
    </message>
</context>
<context>
    <name>BaseObjectView</name>
    <message>
        <location filename="../libobjrenderer/src/baseobjectview.cpp" line="486"/>
        <source>SQL off</source>
        <translation>SQL désactivé</translation>
    </message>
</context>
<context>
    <name>BaseObjectWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="47"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="440"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="471"/>
        <source>icone</source>
        <translation>icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="109"/>
        <source>Comment:</source>
        <translation>Commentaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="128"/>
        <source>Tablespace:</source>
        <translation>Espace de stockage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="181"/>
        <source>Edit object&apos;s permissions</source>
        <translation>Modifier les permissions de l&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="305"/>
        <source>Disables the generated SQL code using comment tokens (--).
This will disable the code of all child and referrer objects.</source>
        <translation>Désactivez le code SQL généré en utilisant les marqueurs de commentaire (--).
Ceci désactivera également le code SQL pour ses objets enfants.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="362"/>
        <source>Append or prepend a set of SQL commands to the object&apos;s definition.</source>
        <translation>Ajouter des commandes SQL avant ou après la définition de cet objet.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="365"/>
        <source>Custom SQL</source>
        <translation>Code SQL Libre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="394"/>
        <source>Owner:</source>
        <translation>Propriétaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="147"/>
        <source>Schema:</source>
        <translation>Schéma :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="309"/>
        <source>Disable SQL code</source>
        <translation>Désactiver le code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="328"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="184"/>
        <source>Edit permissions</source>
        <translation>Modifier les permissions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="277"/>
        <source>This object is protected thus no change in form will be applied to it.</source>
        <translation>L&apos;objet est verrouillé, par conséquent aucun changement dans le formulaire ne lui sera appliqué.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="170"/>
        <source>Required field. Leaving this empty will raise errors!</source>
        <translation>Champ requis. Laisser ce champ vide génèrera une erreur !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="570"/>
        <source>Value(s)</source>
        <translation>Valeur(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="583"/>
        <source>Version</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="630"/>
        <source>The &lt;em style=&apos;color: %1&apos;&gt;&lt;strong&gt;highlighted&lt;/strong&gt;&lt;/em&gt; fields in the form or one of their values are available only on specific PostgreSQL versions. 							Generating SQL code for versions other than those specified in the fields&apos; tooltips may create incompatible code.</source>
        <translation>Les champs &lt;em style=&apos;color:%1&apos;&gt;&lt;strong&gt;en surbrillance&lt;/strong&gt;&lt;/em&gt; dans le formulaire ou de l&apos;une des valeurs ne sont disponibles que sur des versions spécifiques de PostgreSQL. 							La génération de code SQL pour des versions autres que celles spécifiées dans les info-bulles des champs peut créer un code incompatible.</translation>
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
        <translation>(Dés)activer l&apos;affichage des attributs étendus</translation>
    </message>
    <message>
        <location filename="../libobjrenderer/src/basetableview.cpp" line="445"/>
        <source>Connected rels: %1</source>
        <translation type="unfinished">Rels connectés : %1</translation>
    </message>
</context>
<context>
    <name>BugReportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="35"/>
        <source>Bug Report</source>
        <translation>Signaler un bug</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="107"/>
        <source>Bug report</source>
        <translation>Signaler un bug</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="203"/>
        <source>Create</source>
        <translation>Créer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="146"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="250"/>
        <source>Use the form below to generate a complete bug report. Please, try to be as clear as possible when describing the actions that can reproduce the bug. Additionally, it&apos;s important to attach a sample database model so that the bug can be quickly discovered and fixed!</source>
        <translation>Utilisez le formulaire ci-dessous pour générer un rapport d&apos;erreur complet. Merci d&apos;être le plus clair possible dans la description de vos actions afin que le bug puisse être reproduit. De plus, il est important de joindre un modèle de base dans lequel le bug puisse être rapidement découvert, et par la même occasion rapidement corrigé !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="285"/>
        <source>Report</source>
        <translation>Rapport</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="303"/>
        <source>Issue details</source>
        <translation>Détails du problème</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="348"/>
        <source>Output:</source>
        <translation>Sortie :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="358"/>
        <source>Select the report&apos;s output folder</source>
        <translation>Sélectionnez un dossier de sortie pour écrire le rapport d&apos;erreur</translation>
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
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Il vous est également possible de signaler un bug à tout moment via le dépôt du projet pgModeler sur &lt;a href=&quot;http ://github.com/pgmodeler/pgmodeler/issues&quot;&gt;&lt;span style=&quot; text-decoration : underline; color :#0057ae;&quot;&gt;GitHub&lt;/span&gt;&lt;/a&gt;. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="472"/>
        <source>Database Model</source>
        <translation>Modèle de base</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="511"/>
        <source>Attach the below database model file to be debugged.</source>
        <translation>Joindre le fichier modèle ci-dessous pour débogage.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="521"/>
        <source>Attach a different database model</source>
        <translation>Joindre un autre modèle de base</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="120"/>
        <source>Bug report successfuly generated! Please, send the file &lt;strong&gt;%1&lt;/strong&gt; to &lt;em&gt;%2&lt;/em&gt; in order be analyzed. Thank you for the collaboration!</source>
        <translation>Le rapport de bug a été généré avec succès ! S&apos;il vous plait, veuillez envoyer le fichier &lt;strong&gt;%1&lt;/strong&gt; à &lt;em&gt;%2&lt;/em&gt; pour expertise. Merci de votre collaboration !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="132"/>
        <source>Load model</source>
        <translation>Charger un modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="133"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modèle de base de données (*.dbm);; Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="164"/>
        <source>Select report output folder</source>
        <translation>Sélectionner le dossier de sortie du rapport</translation>
    </message>
</context>
<context>
    <name>BulkDataEditWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/bulkdataeditwidget.ui" line="20"/>
        <source>Bulk data edit</source>
        <translation>Modification de données en masse</translation>
    </message>
</context>
<context>
    <name>CastWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="122"/>
        <source>Conversion Func.:</source>
        <translation>Fonction de conversion :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="147"/>
        <source>Cast Type:</source>
        <translation>Conversion de type :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="84"/>
        <source>Assignment</source>
        <translation>Affectation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="97"/>
        <source>Input / Output</source>
        <translation>Entrée / Sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="31"/>
        <source>Source data type</source>
        <translation>Type de donnée source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="32"/>
        <source>Target data type</source>
        <translation>Type de donnée cible</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="46"/>
        <source>The function to be assigned to a cast from &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; to &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; must have the following signature: &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; function(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</source>
        <translation>La fonction convertissant de &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; vers &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; doit avoir la signature suivante : &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; function(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="56"/>
        <source>I&amp;mplicit</source>
        <translation>I&amp;mplicite</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="172"/>
        <source>E&amp;xplicit</source>
        <translation>E&amp;xplicite</translation>
    </message>
</context>
<context>
    <name>CodeCompletionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="41"/>
        <source>Make &amp;persistent</source>
        <translation>Rendre &amp;persistant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="42"/>
        <source>Makes the widget closable only by ESC key or mouse click on other controls.</source>
        <translation>Rendre ce widget refermable uniquement par la touche Échap ou par un clic sur les autres contrôles.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="453"/>
        <source>SQL Keyword</source>
        <translation>Mot-clé SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="484"/>
        <source>(no items found.)</source>
        <translation>(aucun élément trouvé.)</translation>
    </message>
</context>
<context>
    <name>CollationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="50"/>
        <source>Locale:</source>
        <translation>Locale :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="85"/>
        <source>Encoding:</source>
        <translation>Encodage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="130"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="175"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="30"/>
        <source>The fields &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; are mutually exclusive, so you have to set only one of them in order to properly handle a collation.</source>
        <translation>Les champs &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt; et &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; sont mutuellement exclusifs, vous ne devez en définir qu&apos;un seul de manière à gérer correctement la collation.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="39"/>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="51"/>
        <source>Not defined</source>
        <translation>Non défini</translation>
    </message>
</context>
<context>
    <name>ColorPickerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/colorpickerwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/colorpickerwidget.ui" line="50"/>
        <source>Generate random color(s)</source>
        <translation>Génération aléatoire de couleur(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/colorpickerwidget.ui" line="66"/>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/colorpickerwidget.cpp" line="148"/>
        <source>Select color</source>
        <translation>Sélectionner une couleur</translation>
    </message>
</context>
<context>
    <name>ColumnWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="32"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="77"/>
        <source>E&amp;xpression:</source>
        <translation>E&amp;xpression :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="148"/>
        <source>&amp;NOT NULL</source>
        <translation>&amp;NON NULL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="87"/>
        <source>Se&amp;quence:</source>
        <translation>Sé&amp;quence :</translation>
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
        <translation>Identité :</translation>
    </message>
</context>
<context>
    <name>ConfigurationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="29"/>
        <source>pgModeler Configuration</source>
        <translation>Paramètres de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="202"/>
        <source>Relationships</source>
        <translation>Associations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="214"/>
        <source>Appearance</source>
        <translation>Apparence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="229"/>
        <source>Connections</source>
        <translation>Connexions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="244"/>
        <source>Snippets</source>
        <translation>Extraits de code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="388"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="426"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="347"/>
        <source>Defaults</source>
        <translation>Défauts</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="187"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="256"/>
        <source>Plug-ins</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="126"/>
        <source>In some cases restore the default settings related to it may solve the problem. Would like to do that?</source>
        <translation>Dans certains cas, restaurer les paramètres liés par défaut peut résoudre le problème. Voulez-vous le faire ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="127"/>
        <source>Restore</source>
        <translation>Restaurer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="139"/>
        <source>Any modification made until now in the current section will be lost! Do you really want to restore default settings?</source>
        <translation>Toutes les modifications réalisées jusqu&apos;à maintenant dans la section actuelle seront perdues ! Souhaitez-vous réellement restaurer les paramètres par défaut ?</translation>
    </message>
</context>
<context>
    <name>ConnectionsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="65"/>
        <source>Connections:</source>
        <translation>Connexions :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="102"/>
        <source>Create new connection</source>
        <translation>Créer une nouvelle connexion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="125"/>
        <source>Cancel edition</source>
        <translation>Annuler l&apos;édition</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="148"/>
        <source>Duplicate the selected connection</source>
        <translation>Dupliquer la connexion sélectionnée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="171"/>
        <source>Edit selected connection</source>
        <translation>Modifier la connexion sélectionnée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="194"/>
        <source>Delete selected connection</source>
        <translation>Supprimer la connexion sélectionnée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="492"/>
        <source>Connection Alias:</source>
        <translation>Alias de la connexion :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="515"/>
        <source>Connection DB:</source>
        <translation>Nom de la base :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="479"/>
        <source>Host/Port:</source>
        <translation>Hôte/Port :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="466"/>
        <source>User:</source>
        <translation>Utilisateur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="388"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="244"/>
        <source>Timeout:</source>
        <translation>Délai d&apos;expiration :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="583"/>
        <source>second(s)</source>
        <translation>seconde(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="880"/>
        <source>SSL Mode:</source>
        <translation>Mode SSL :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="794"/>
        <source>Disable</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="799"/>
        <source>Allow</source>
        <translation>Autorisé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="804"/>
        <source>Require</source>
        <translation>Requis</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="809"/>
        <source>AC verification</source>
        <translation>Vérification des AC</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="814"/>
        <source>Full verification</source>
        <translation>Vérification complète</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="986"/>
        <source>Client Certificate:</source>
        <translation>Certificat client :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="775"/>
        <source>~/.postgresql/postgresql.crt</source>
        <translation>~/.postgresql/postgresql.crt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="690"/>
        <source>Automatically browses the named database when using this connection to manage databases on &lt;strong&gt;Manage&lt;/strong&gt; view.</source>
        <translation>Parcourt automatiquement la base de données nommée lors de l&apos;utilisation de cette connexion pour gérer les bases de données dans la vue &lt;strong&gt;Gérer&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="693"/>
        <source>Auto browse</source>
        <translation>Toujours montrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="896"/>
        <source>Client Key:</source>
        <translation>Clé client :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="967"/>
        <source>~/.postgresql/postgresql.key</source>
        <translation>~/.postgresql/postgresql.key</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="941"/>
        <source>Root Certificate:</source>
        <translation>Certificat racine :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="840"/>
        <source>~/.postgresql/root.crt</source>
        <translation>~/.postgresql/root.crt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="957"/>
        <source>Revoked Certs.:</source>
        <translation>Liste de révocations :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="906"/>
        <source>~/.postgresql/root.crl</source>
        <translation>~/.postgresql/root.crl</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="865"/>
        <source>Force GSSAPI</source>
        <translation>Forcer GSSAPI</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="325"/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="363"/>
        <source>Update</source>
        <translation>Mettre à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="287"/>
        <source>Test</source>
        <translation>Tester</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="437"/>
        <source>Success</source>
        <translation>Succès</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="14"/>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="602"/>
        <source>Edit database connections</source>
        <translation>Modifier les connexions de la base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="220"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="528"/>
        <source>Other params:</source>
        <translation>Autres paramètres :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="537"/>
        <source>Specify additional connection parameters in the form [param]=[value]. These parameters are described in the &lt;strong&gt;libpq&lt;/strong&gt; chapter at PostgreSQL docs.</source>
        <translation>Spécifier des paramètres de connexion supplémentaires sous la forme [param]=[valeur]. Ces paramètres sont décrits dans le chapitre &lt;strong&gt;libpq&lt;/strong&gt; de la doc de PostgreSQL.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="434"/>
        <source>Default for:</source>
        <translation>Par défaut pour :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="607"/>
        <source>Diff</source>
        <translation>Différencier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="614"/>
        <source>Export</source>
        <translation>Exporter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="621"/>
        <source>Import</source>
        <translation>Importer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="628"/>
        <source>Validation</source>
        <translation>Validation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="754"/>
        <source>Security</source>
        <translation>Sécurité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="925"/>
        <source>Kerberos Server:</source>
        <translation>Serveur Kerberos :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="38"/>
        <source>Indicates in which operations (diff, export, import or validation) the connection is used if none is explicitly specified by the user.</source>
        <translation>Indique dans quelles opérations (differencier, exporter, importer ou validation) la connexion est utilisée si aucune n&apos;est explicitement spécifiée par l&apos;utilisateur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="438"/>
        <source>Connection successfully established!

Server details:

PID: `%1&apos;
Protocol: `%2&apos;
Version: `%3&apos;</source>
        <translation>Connexion établie avec succès !

Détails du serveur :

PID : `%1&apos;
Protocole : `%2&apos;
Version : `%3&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="485"/>
        <source>There is a connection being created or edited! Do you want to save it?</source>
        <translation>Une connexion est en cours de création ou d&apos;édition ! Voulez-vous la sauvegarder ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="572"/>
        <source>Found %1 connection(s)</source>
        <translation>%1 connexion(s) trouvée(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="574"/>
        <source>No connections found</source>
        <translation>Aucune connexion trouvée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="586"/>
        <source>Edit connections</source>
        <translation>Modifier les connexions</translation>
    </message>
</context>
<context>
    <name>ConstraintWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="94"/>
        <source>Constraint Type:</source>
        <translation>Type de contrainte :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="544"/>
        <source>Fill Factor:</source>
        <translation>Taux de remplissage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="56"/>
        <source>Match:</source>
        <translation>Comparaison :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="107"/>
        <source>This attribute cannot be changed once the object is created.</source>
        <translation>Cet attribut ne peut pas être modifié une fois l&apos;objet créé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="126"/>
        <source>Expression:</source>
        <translation>Expression :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="167"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="208"/>
        <source>Deferral:</source>
        <translation>Report :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="262"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="287"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="353"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="380"/>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="494"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="426"/>
        <source>Referenced Columns</source>
        <translation>Colonnes référencées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="507"/>
        <source>Table:</source>
        <translation>Table :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="524"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="330"/>
        <source>Exclude Elements</source>
        <translation>Éléments exclus</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="310"/>
        <source>No inherit:</source>
        <translation>Pas d&apos;héritage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="54"/>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="61"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="56"/>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="63"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="88"/>
        <source>Columns which were included by relationship can not be added / removed manually from the primary key. If done such changes they can raise errors. To create primary key using columns included by relationship use the following options: identifier field, attributes &amp; constraints tab or primary key tab on the relationship form.</source>
        <translation>Les colonnes inclues dans la relation ne peuvent être ajoutées/retirées manuellement de la clé primaire sous peine d’erreur. Pour créer une clé primaire utilisant des colonnes contenues dans une relation, veuillez utiliser une des options suivantes: champ identifiant, l&apos;onglet attributs et contraintes ou l&apos;onglet clé primaire du formulaire de la relation.</translation>
    </message>
</context>
<context>
    <name>ConversionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="56"/>
        <source>Source Encoding:</source>
        <translation>Encodage de la source :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="100"/>
        <source>Target Encoding:</source>
        <translation>Encodage de la destination :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="147"/>
        <source>Conversion Func.:</source>
        <translation>Fonction de conversion :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="185"/>
        <source>Default Conversion:</source>
        <translation>Conversion par défaut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/conversionwidget.cpp" line="40"/>
        <source>The function to be assigned to an encoding conversion must have the following signature: &lt;em&gt;void function(integer, integer, cstring, internal, integer)&lt;/em&gt;.</source>
        <translation>La fonction de convertion d&apos;encodage doit avoir la signature suivante : &lt;em&gt;void function(integer, integer, cstring, internal, integer)&lt;/em&gt;.</translation>
    </message>
</context>
<context>
    <name>CrashHandlerForm</name>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="32"/>
        <source>Crash Handler</source>
        <translation>Gestionnaire d&apos;incident</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="44"/>
        <source>Stack trace</source>
        <translation>Pile d&apos;exécution</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="76"/>
        <source>Input:</source>
        <translation>Entrée :</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="89"/>
        <source>Load report file for analysis</source>
        <translation>Charger un rapport d&apos;erreur pour analyse</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="97"/>
        <source>Save the attached model file on the filesystem</source>
        <translation>Enregistrer le modèle joint sur le système de fichiers</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="174"/>
        <source>pgModeler bug report (*.bug);;All files (*.*)</source>
        <translation>Rapport de bug pgModeler (*.bug);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="175"/>
        <source>Load report</source>
        <translation>Charger un rapport</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="199"/>
        <source>Save model</source>
        <translation>Enregistrer le modèle</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="200"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modèle de base de données (*.dbm);; Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="240"/>
        <source>Crash handler</source>
        <translation>Gestionnaire d&apos;incident</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="241"/>
        <source>Bug report analysis mode activated.</source>
        <translation>Mode d&apos;analyse de rapport d&apos;erreur activé.</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="245"/>
        <source>Oops! pgModeler just crashed!</source>
        <translation>Oups ! pgModeler a planté !</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="246"/>
        <source>We apologize for what happened! It is clear that a nasty bug caused that. Please fill out the form below describing your actions before pgModeler quit unexpectedly. This will help on bug extermination and improve the software.</source>
        <translation>Veuillez nous excuser pour ce qui vient de se passer ! Il est clair qu&apos;un vilain bug est à l&apos;origine de tout cela. Veuillez renseigner le formulaire ci-dessous en décrivant les actions que vous avez faites avant que pgModeler ne sombre de manière inopinée. Cela nous aidera à exterminer ce bug et ainsi, à améliorer le logiciel.</translation>
    </message>
</context>
<context>
    <name>CsvLoadWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="44"/>
        <source>Load CSV</source>
        <translation>Charger un fichier CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="62"/>
        <source>CSV File:</source>
        <translation>Fichier CSV :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="81"/>
        <source>Select output file</source>
        <translation>Sélectionner un fichier de sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="84"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="101"/>
        <source>Separator: </source>
        <translation>Séparateur : </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="129"/>
        <source>Use the first row as column names in the CSV file. By unchecking this option the first row is used as data.</source>
        <translation>Utiliser la 1ère ligne comme noms de colonne pour le fichier CSV. En décochant cette option, la 1ère ligne est utilisée comme données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="132"/>
        <source>Columns in the first row</source>
        <translation>Colonnes dans la 1ère ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="200"/>
        <source>Load</source>
        <translation>Charger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="241"/>
        <source>Semicolon (;)</source>
        <translation>Point virgule (;)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="246"/>
        <source>Comma (,)</source>
        <translation>Virgule (,)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="251"/>
        <source>Space</source>
        <translation>Espace</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="256"/>
        <source>Tabulation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="261"/>
        <source>Other</source>
        <translation>Autre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="287"/>
        <source>;</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="307"/>
        <source>Text delimiter:</source>
        <translation>Délimiteur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="338"/>
        <source>&quot;</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/csvloadwidget.cpp" line="68"/>
        <source>Load CSV file</source>
        <translation>Charger un fichier CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/csvloadwidget.cpp" line="70"/>
        <source>Comma-separted values (*.csv);;All files (*.*)</source>
        <translation>Valeurs séparées par une virgule (*.csv);;Tous les fichiers (*.*)</translation>
    </message>
</context>
<context>
    <name>CustomSQLWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="44"/>
        <source>SQL code</source>
        <translation>Code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="88"/>
        <source>Puts an SELECT command template at current cursor position.</source>
        <translation>Insère une commande SELECT à la position courante du curseur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="91"/>
        <source>&amp;SELECT</source>
        <translation>&amp;SELECT</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="125"/>
        <source>Puts an INSERT command template at current cursor position.</source>
        <translation>Insère une commande INSERT à la position courante du curseur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="128"/>
        <source>&amp;INSERT</source>
        <translation>&amp;INSERT</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="162"/>
        <source>Puts an UPDATE command template at current cursor position.</source>
        <translation>Insère une commande UPDATE à la position courante du curseur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="199"/>
        <source>Puts an DELETE command template at current cursor position.</source>
        <translation>Insère une commande DELETE à la position courante du curseur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="202"/>
        <source>&amp;DELETE</source>
        <translation>&amp;DELETE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="233"/>
        <source>&amp;Clear</source>
        <translation>E&amp;ffacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="272"/>
        <source>Append SQL</source>
        <translation>Code SQL suffixé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="290"/>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="323"/>
        <source>Append the SQL code at the very end of model definition.
Unchecking this will cause the SQL to be appended at the end of CREATE DATABASE command.</source>
        <translation>Ajouter le code SQL à la toute fin de la définition du modèle
Sinon (décoché) pgModeler ajoutera ce code après la commande SQL CREATE DATABASE.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="294"/>
        <source>Append at end of model definition.</source>
        <translation>Suffixer à la fin de la définition du modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="305"/>
        <source>Prepend SQL</source>
        <translation>Code SQL préfixé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="327"/>
        <source>Prepend at beginning of model definition.</source>
        <translation>Préfixer au tout début de la définition du modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="411"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Use custom commands with extreme caution because you can change the semantics of the entire model when running SQL validation or export processes. Additionally, depending on the amount of commands, those processes can have their performance sensibly degradated.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Utilisez cet espace pour y inscrire vos commandes SQL personnalisées avec précaution. Vous pourriez créer des incohérences qui peuvent interférer avec les fonctionnalités de validation et d&apos;export du modèle. De plus, selon la nature des commandes entrées ces fonctionnalités peuvent être ralenties.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="48"/>
        <source>Generic INSERT</source>
        <translation>INSERT générique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="50"/>
        <source>Include serial columns</source>
        <translation type="unfinished">Inclure les colonnes en série</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="52"/>
        <source>Exclude serial columns</source>
        <translation type="unfinished">Exclure les colonnes en série</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="54"/>
        <source>Generic SELECT</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="56"/>
        <source>Table SELECT</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="58"/>
        <source>Generic UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="60"/>
        <source>Table UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="62"/>
        <source>Generic DELETE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="64"/>
        <source>Table DELETE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="165"/>
        <source>&amp;UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="26"/>
        <source>Add custom SQL code</source>
        <translation>Ajouter un code SQL personnalisé</translation>
    </message>
</context>
<context>
    <name>DataManipulationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="20"/>
        <source>Data Manipulation</source>
        <translation>Édition des données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="114"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="334"/>
        <source>Refresh listing</source>
        <translation>Actualiser la liste</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="347"/>
        <source>F5</source>
        <translation>F5</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="371"/>
        <source>Save changes</source>
        <translation>Enregistrer les changements</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="384"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="485"/>
        <source>Export results to CSV file</source>
        <translation>Exporter les résultats vers un fichier CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="498"/>
        <source>Ctrl+X</source>
        <translation>Ctrl+X</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="575"/>
        <source>Undo modifications</source>
        <translation>Annuler les modifications</translation>
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
        <translation>Marquer les lignes sélectionner pour suppression</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="662"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1314"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1352"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1390"/>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1428"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="811"/>
        <source>Filter the result set</source>
        <translation>Filtrer les résultats</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="842"/>
        <source>Table:</source>
        <translation>Table :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="855"/>
        <source>Schema:</source>
        <translation>Schéma :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="933"/>
        <source>in </source>
        <translation>dans </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="989"/>
        <source>Hide views</source>
        <translation>Masquer les vues</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1167"/>
        <source>Filter expression</source>
        <translation>Expression pour filtrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1202"/>
        <source>Order &amp;&amp; Limit</source>
        <translation>Ordre &amp;&amp; Limite</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1478"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1498"/>
        <source>Limit in:</source>
        <translation>Limite :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1260"/>
        <source>Add Item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1298"/>
        <source>Remove Item</source>
        <translation>Supprimer un élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1336"/>
        <source>Clear the order by columns list</source>
        <translation>Effacer la liste</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1374"/>
        <source>Move selected item up</source>
        <translation>Déplacer vers le haut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1412"/>
        <source>Move selected item down</source>
        <translation>Déplacer vers le bas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="54"/>
        <source>Copy as CSV</source>
        <translation>Copier en CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="62"/>
        <source>Copy as text</source>
        <translation>Copier en texte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="148"/>
        <source>Copy items</source>
        <translation>Copier les éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="151"/>
        <source>Pase items</source>
        <translation>Coller les éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="159"/>
        <source>Browse tables</source>
        <translation>Navigateur de tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="164"/>
        <source>Duplicate row(s)</source>
        <translation>Dupliquer ligne(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="167"/>
        <source>Delete row(s)</source>
        <translation>Effacer ligne(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="170"/>
        <source>Edit cell(s)</source>
        <translation>Modifier cellule(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="353"/>
        <source>&lt;em&gt;(Limit: &lt;strong&gt;%1&lt;/strong&gt;)&lt;/em&gt;</source>
        <translation>&lt;em&gt;(Limite : &lt;strong&gt;%1&lt;/strong&gt;)&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="382"/>
        <source>Column</source>
        <translation type="unfinished">Colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="646"/>
        <source>No objects found</source>
        <translation>Aucun objet trouvé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="648"/>
        <source>Found %1 object(s)</source>
        <translation>%1 objet(s) trouvé(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="678"/>
        <source>Views can&apos;t have their data handled through this grid, this way, all operations are disabled.</source>
        <translation>Les données des vues ne sont pas modifiables via cette grille, de ce fait, toutes les opérations sont bloquées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="689"/>
        <source>The selected table doesn&apos;t owns a primary key! Updates and deletes will be performed by considering all columns as primary key. &lt;strong&gt;WARNING:&lt;/strong&gt; those operations can affect more than one row.</source>
        <translation>La table sélectionnée n&apos;a pas sa propre clé primaire ! Les mises à jour et les suppressions seront réalisées en considérant que l&apos;ensemble des colonnes constitue une clé primaire. &lt;strong&gt;ATTENTION :&lt;/strong&gt; ces opérations peuvent affecter plus d&apos;une ligne.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="761"/>
        <source>Referenced tables</source>
        <translation>Tables référencées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="764"/>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="813"/>
        <source>(none)</source>
        <translation>(aucun)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="810"/>
        <source>Referrer tables</source>
        <translation>Tables de référence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="861"/>
        <source>This row is marked to be %1</source>
        <translation>Cette ligne est marquée pour être %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="866"/>
        <source>deleted</source>
        <translation>supprimé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="868"/>
        <source>updated</source>
        <translation>mise à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="870"/>
        <source>inserted</source>
        <translation>inséré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="995"/>
        <source>[binary data]</source>
        <translation>[données binaires]</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1211"/>
        <source>&lt;strong&gt;WARNING:&lt;/strong&gt; Once commited its not possible to undo the changes! Proceed with saving?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Une fois validés, il ne sera plus possible d&apos;annuler les changements ! Sauvegarder les changements ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1242"/>
        <source>delete</source>
        <translation>effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1243"/>
        <source>update</source>
        <translation>mettre à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="1244"/>
        <source>insert</source>
        <translation>inserer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1526"/>
        <source>ASC</source>
        <translation>ASC</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1542"/>
        <source>DESC</source>
        <translation>DESC</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="612"/>
        <source>Add empty rows</source>
        <translation>Ajouter des lignes vides</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="723"/>
        <source>Duplicate the selected rows</source>
        <translation>Dupliquer les lignes sélectionnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="736"/>
        <source>Ctrl+D</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="1491"/>
        <source>results (Use &lt;strong&gt;0&lt;/strong&gt; for no limit)</source>
        <translation>résultats (Utiliser &lt;strong&gt;0&lt;/strong&gt; pour ne pas limiter)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="304"/>
        <source>&lt;strong&gt;WARNING: &lt;/strong&gt; There are some changed rows waiting the commit! Do you really want to discard them and retrieve the data now?</source>
        <translation>&lt;strong&gt;ALERTE :&lt;/strong&gt; Des lignes modifiées attendent d&apos;être soumises ! Voulez-vous vraiment les jeter et récupérer les données maintenant ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="352"/>
        <source>Rows returned: &lt;strong&gt;%1&lt;/strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;</source>
        <translation>Lignes retournées : &lt;strong&gt;%1&lt;/strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="353"/>
        <source>none</source>
        <translation>aucun</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="258"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Empty values are assumed as &lt;span style=&quot; font-weight:600;&quot;&gt;DEFAULT&lt;/span&gt;. To use special values like &lt;span style=&quot; font-weight:600;&quot;&gt;NULL&lt;/span&gt;, a function call like &lt;span style=&quot; font-weight:600;&quot;&gt;now()&lt;/span&gt; or a specific data escaping, enclose values in two slashes, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;/value/&lt;/span&gt;. To use a slash as part of the value prepend the backslash character, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;\/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Les valeurs vides sont supposées être &lt;span style=&quot;font-weight :600;&quot;&gt;DEFAULT&lt;/span&gt;. Pour utiliser des valeurs spéciales comme &lt;span style=&quot;font-weight :600;&quot;&gt;NULL&lt;/span&gt;, un appel de fonction comme &lt;span style=&quot;font-weight :600;&quot;&gt;now()&lt;/span&gt; ou un échappement des données spécifiques, entourer les valeurs de deux barres obliques, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;/valeur/&lt;/span&gt;. Pour utiliser une barre oblique dans le cadre de la valeur, ajouter le caractère barre oblique inverse, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="408"/>
        <source>Copy items on the grid</source>
        <translation>Copier les éléments sur la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="445"/>
        <source>Paste items on the grid</source>
        <translation>Copier les éléments sur la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="458"/>
        <source>Ctrl+V</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="522"/>
        <source>Browse referenced tables</source>
        <translation>Parcourir les tables référencées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="771"/>
        <source>Add new rows from a CSV file</source>
        <translation>Ajouter de nouvelles lignes depuis un fichier CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="686"/>
        <source>Change the values of all selected cells at once</source>
        <translation>Modifier d&apos;un coup les valeurs de toutes les cellules sélectionnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="699"/>
        <source>Ctrl+E</source>
        <translation>Ctrl+E</translation>
    </message>
</context>
<context>
    <name>DatabaseExplorerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="51"/>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="128"/>
        <source>Data &amp;Grid</source>
        <translation>&amp;Grille des données</translation>
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
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="379"/>
        <source>Expands all items</source>
        <translation>Ouvre tous les nœuds de l&apos;arborescence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="411"/>
        <source>Collapses all items</source>
        <translation>Ferme tous les nœuds de l&apos;arborescence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="463"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="480"/>
        <source>By OID</source>
        <translation>Par OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="603"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="38"/>
        <source>Attribute</source>
        <translation>Attribut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="608"/>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="30"/>
        <source>(not found, OID: %1)</source>
        <translation>(OID: %1 non trouvé)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="38"/>
        <source>By value</source>
        <translation>Par valeur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="39"/>
        <source>Collatable</source>
        <translation>Assemblable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="41"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="76"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="43"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="43"/>
        <source>Definition</source>
        <translation>Définition</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="45"/>
        <source>Element</source>
        <translation>Élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="46"/>
        <source>Encrypted</source>
        <translation>Chiffré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="46"/>
        <source>Enumerations</source>
        <translation>Énumérations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="48"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="53"/>
        <source>Language</source>
        <translation>Langage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="54"/>
        <source>Length</source>
        <translation>Longueur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="55"/>
        <source>Library</source>
        <translation>Bibliothèque</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="55"/>
        <source>Materialized</source>
        <translation>Matérialisée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="56"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="61"/>
        <source>Precision</source>
        <translation>Précision</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="62"/>
        <source>Preferred</source>
        <translation>Préféré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="65"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="67"/>
        <source>Storage</source>
        <translation>Stockage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="68"/>
        <source>Superuser</source>
        <translation>Super utilisateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="68"/>
        <source>Tablespace</source>
        <translation>Espace de stockage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="71"/>
        <source>Unlogged</source>
        <translation>Déconnecté</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="71"/>
        <source>Validity</source>
        <translation>Validité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="75"/>
        <source>Subtype</source>
        <translation>Sous-type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="80"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="85"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="86"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="81"/>
        <source>Event</source>
        <translation>Évènement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="88"/>
        <source>Client encoding</source>
        <translation>Codage du client</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="88"/>
        <source>Configuration file</source>
        <translation>Fichier de configuration</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="88"/>
        <source>Data directory</source>
        <translation>Dossier de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="89"/>
        <source>Dynamic library path</source>
        <translation>Chemin de la bibliothèque dynamique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="89"/>
        <source>Dynamic shared memory</source>
        <translation>Mémoire partagée dynamique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="89"/>
        <source>Hba file</source>
        <translation>Fichier hba</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="90"/>
        <source>Listen addresses</source>
        <translation>Écouter des adresses</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="90"/>
        <source>Max. connections</source>
        <translation>Connexions max.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="90"/>
        <source>Listen port</source>
        <translation>Écouter port</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="91"/>
        <source>Server encoding</source>
        <translation>Encodage serveur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="91"/>
        <source>SSL ca file</source>
        <translation>Fichier ca SSL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="92"/>
        <source>SSL cert file</source>
        <translation>Fichier cert SSL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="92"/>
        <source>SSL crl file</source>
        <translation>Fichier crl SSL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="92"/>
        <source>SSL key file</source>
        <translation>Fichier clé SSL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="93"/>
        <source>Server version</source>
        <translation>Version serveur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="93"/>
        <source>Ident file</source>
        <translation>Fichier d&apos;identité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="93"/>
        <source>Password encryption</source>
        <translation>Mot de passe de chiffrement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="94"/>
        <source>Connection ID</source>
        <translation>ID connexion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="94"/>
        <source>Server PID</source>
        <translation>PID serveur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="94"/>
        <source>Server protocol</source>
        <translation>Protocole serveur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="95"/>
        <source>Identity</source>
        <translation>Identité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="95"/>
        <source>Command</source>
        <translation>Commande</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="96"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="118"/>
        <source>Show objects filter</source>
        <translation>Afficher les objets filtrés</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="124"/>
        <source>Show system objects</source>
        <translation>Afficher les objets système</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="128"/>
        <source>Show extension objects</source>
        <translation>Afficher les objets d&apos;extension</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="134"/>
        <source>Snippets</source>
        <translation>Bouts de code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="137"/>
        <source>Drop object</source>
        <translation>Supprimer l&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="140"/>
        <source>Drop cascade</source>
        <translation>Supprimer en cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="143"/>
        <source>Truncate</source>
        <translation>Tronquer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="144"/>
        <source>Trunc. cascade</source>
        <translation>Tronquer en cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="146"/>
        <source>Show data</source>
        <translation>Afficher les données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="148"/>
        <source>Reload properties</source>
        <translation>Recharger les propriétés</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="150"/>
        <source>Update</source>
        <translation>Mettre à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="945"/>
        <source>-- Source code unavailable for this kind of object --</source>
        <translation>-- Code source indisponible pour ce type d&apos;objet --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1193"/>
        <source>Do you really want to drop the object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;?</source>
        <translation>Voulez-vous vraiment supprimer l&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1267"/>
        <source>Do you really want to truncate the table &lt;strong&gt;%1&lt;/strong&gt;?</source>
        <translation>Voulez-vous vraiment tronquer la table &lt;strong&gt;%1&lt;/strong&gt; ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1269"/>
        <source>Do you really want to &lt;strong&gt;cascade&lt;/strong&gt; truncate the table &lt;strong&gt;%1&lt;/strong&gt;? This action will truncate all the tables that depends on it?</source>
        <translation>Voulez-vous vraiment tronquer en &lt;strong&gt;cascade&lt;/strong&gt; la table &lt;strong&gt;%1&lt;/strong&gt; ? Cette action tronquera toutes les tables qui en dépendent !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="35"/>
        <source>Admin. roles</source>
        <translation>Rôles admin.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="35"/>
        <source>Alignment</source>
        <translation>Alignement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="35"/>
        <source>Analyze func.</source>
        <translation>Fonc. d&apos;analyse</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="36"/>
        <source>Arg. count</source>
        <translation>Nombre d&apos;arg.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="36"/>
        <source>Arg. default count</source>
        <translation>Nombre d&apos;arg. par défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="36"/>
        <source>Arg. defaults</source>
        <translation>Arg. par défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="37"/>
        <source>Arg. modes</source>
        <translation>Modes des arg.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="37"/>
        <source>Arg. names</source>
        <translation>Noms des arg.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="37"/>
        <source>Arg. types</source>
        <translation>Types des arg.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="38"/>
        <source>Behavior type</source>
        <translation>Type de comportement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="39"/>
        <source>Cast type</source>
        <translation>Type de conversion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="39"/>
        <source>Category</source>
        <translation>Catégorie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="40"/>
        <source>Comment</source>
        <translation>Commentaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="40"/>
        <source>Commutator Op.</source>
        <translation>Commutateur Op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="40"/>
        <source>Collation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="41"/>
        <source>Conn. limit</source>
        <translation>Limite conn.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="41"/>
        <source>Configuration</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="42"/>
        <source>Create DB</source>
        <translation>Créer DB</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="42"/>
        <source>Create role</source>
        <translation>Créer rôle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="42"/>
        <source>Curr. version</source>
        <translation>Version courante</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="43"/>
        <source>Default value</source>
        <translation>Valeur par défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="44"/>
        <source>Delimiter</source>
        <translation>Délimiteur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="44"/>
        <source>Dest. type</source>
        <translation>Type de dest.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="44"/>
        <source>Dimension</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="45"/>
        <source>Directory</source>
        <translation>Répertoire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="45"/>
        <source>Dest. encoding</source>
        <translation>Encodage de dest.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="46"/>
        <source>Encoding</source>
        <translation>Encodage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="47"/>
        <source>Exec. cost</source>
        <translation>Exec. du coût</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="47"/>
        <source>Op. family</source>
        <translation>Famille d&apos;op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="47"/>
        <source>Expression</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="48"/>
        <source>Final func.</source>
        <translation>Fonc. finale</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="48"/>
        <source>Func. type</source>
        <translation>Type de fonc.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="49"/>
        <source>Handler func.</source>
        <translation>Gestionnaire de fonc.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="49"/>
        <source>Handles type</source>
        <translation>Type de poignées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="49"/>
        <source>Hashes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="50"/>
        <source>Index type</source>
        <translation>Type d&apos;index</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="50"/>
        <source>Inherit</source>
        <translation>Hérité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="50"/>
        <source>Ini. condition</source>
        <translation>Condition ini.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="51"/>
        <source>Inline func.</source>
        <translation>Func. en ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="51"/>
        <source>Input func.</source>
        <translation>Fonc. d&apos;entrée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="51"/>
        <source>Internal length</source>
        <translation>Longueur interne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="52"/>
        <source>Interval type</source>
        <translation>Type d&apos;interval</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="52"/>
        <source>I/O cast</source>
        <translation>Conversion E/S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="52"/>
        <source>Join func.</source>
        <translation>Fonc. de jointure</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="53"/>
        <source>LC COLLATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="53"/>
        <source>LC CTYPE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="54"/>
        <source>Leak proof</source>
        <translation>Étanche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="54"/>
        <source>Left type</source>
        <translation>Type gauche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="55"/>
        <source>Can login</source>
        <translation>Peut se connecter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="56"/>
        <source>Member roles</source>
        <translation>Rôles des membres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="56"/>
        <source>Merges</source>
        <translation>Fusionne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="57"/>
        <source>Negator op.</source>
        <translation>Négateur op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="57"/>
        <source>Not null</source>
        <translation>Non nul</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="57"/>
        <source>Object type</source>
        <translation>Type d&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="58"/>
        <source>With OIDs</source>
        <translation>Avec OIDs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="58"/>
        <source>Old version</source>
        <translation>Ancienne version</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="58"/>
        <source>OID</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="59"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="59"/>
        <source>Operator func.</source>
        <translation>Fonc. de l&apos;opérateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="59"/>
        <source>Output func.</source>
        <translation>Func. de sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="60"/>
        <source>Owner</source>
        <translation>Propriétaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="60"/>
        <source>Owner column</source>
        <translation>Colonne propriétaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="60"/>
        <source>Parents</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="61"/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="61"/>
        <source>Permissions</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="62"/>
        <source>Range attributes</source>
        <translation>Attributs de plage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="62"/>
        <source>Receive func.</source>
        <translation>Fonc. de réception</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="63"/>
        <source>Ref. roles</source>
        <translation>Rôles de ref.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="63"/>
        <source>Replication</source>
        <translation>Reproduction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="63"/>
        <source>Restriction func.</source>
        <translation>Fonc. de restriction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="64"/>
        <source>Return type</source>
        <translation>Type de retour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="64"/>
        <source>Returns SETOF</source>
        <translation>Retours SETOF</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="64"/>
        <source>Right type</source>
        <translation>Type droit</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="65"/>
        <source>Rows amount</source>
        <translation>Montant des lignes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="65"/>
        <source>Security type</source>
        <translation>Type de sécurité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="66"/>
        <source>Send func.</source>
        <translation>Fonc. d&apos;envoi</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="66"/>
        <source>Sort op.</source>
        <translation>Op. de tri</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="66"/>
        <source>Source type</source>
        <translation>Type de source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="67"/>
        <source>Src. encoding</source>
        <translation>Encodage de la src.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="67"/>
        <source>State type</source>
        <translation>Type d&apos;état</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="68"/>
        <source>Type mod. in func.</source>
        <translation>Type de mod. dans la fonc.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="69"/>
        <source>Type mod. out func.</source>
        <translation>Type de mod. hors fonc.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="69"/>
        <source>Transition func.</source>
        <translation>Fonc. de transition</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="69"/>
        <source>Trusted</source>
        <translation>Fiable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="70"/>
        <source>Type attribute</source>
        <translation>Attribut de type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="70"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="70"/>
        <source>Types</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="71"/>
        <source>Validator func.</source>
        <translation>Fonc. de validateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="72"/>
        <source>Windows func.</source>
        <translation>Fonc. de fenêtre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="72"/>
        <source>false</source>
        <translation>faux</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="72"/>
        <source>true</source>
        <translation>vrai</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="73"/>
        <source>Cache value</source>
        <translation>Valeur du cache</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="73"/>
        <source>Increment</source>
        <translation>Incrément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="73"/>
        <source>Cycle</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="74"/>
        <source>Max. value</source>
        <translation>Valeur max</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="74"/>
        <source>Min. value</source>
        <translation>Valeur min</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="74"/>
        <source>Start value</source>
        <translation>Valeur de départ</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="75"/>
        <source>Last value</source>
        <translation>Dernière valeure</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="75"/>
        <source>Op. class</source>
        <translation>Classe d&apos;op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="76"/>
        <source>Canonical func.</source>
        <translation>Fonc. de Canonical</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="76"/>
        <source>Subtype diff func.</source>
        <translation>Fonc de comparaison de sous-type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="77"/>
        <source>Deferrable</source>
        <translation>Reportable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="77"/>
        <source>For each row</source>
        <translation>Pour chaque ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="77"/>
        <source>Firing</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="78"/>
        <source>On insert</source>
        <translation>À l&apos;insertion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="78"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="83"/>
        <source>On delete</source>
        <translation>À la suppression</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="78"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="86"/>
        <source>On update</source>
        <translation>À la mise à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="79"/>
        <source>On truncate</source>
        <translation>Au tronquage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="79"/>
        <source>Arguments</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="79"/>
        <source>Table</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="80"/>
        <source>Trigger func.</source>
        <translation>Fonc. de déclenchement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="80"/>
        <source>Condition</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="81"/>
        <source>Deferment</source>
        <translation>Ajournement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="81"/>
        <source>Execution mode</source>
        <translation>Mode d&apos;exécution</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="82"/>
        <source>Commands</source>
        <translation>Commandes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="82"/>
        <source>Comparison type</source>
        <translation>Type de comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="82"/>
        <source>Position</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="83"/>
        <source>Ref. columns</source>
        <translation>Colonnes de ref.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="83"/>
        <source>Expressions</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="84"/>
        <source>Fill factor</source>
        <translation>Facteur de remplissage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="84"/>
        <source>No inherit</source>
        <translation>Non hérité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="84"/>
        <source>Op. classes</source>
        <translation>Classes op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="85"/>
        <source>Operators</source>
        <translation>Opérateurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="85"/>
        <source>Ref. table</source>
        <translation>Table de ref.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="86"/>
        <source>Unique</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="87"/>
        <source>Predicate</source>
        <translation>Prédicat</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="87"/>
        <source>Inherited</source>
        <translation>Hérité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="87"/>
        <source>Collations</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="91"/>
        <source>SSL</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="95"/>
        <source>Referrers</source>
        <translation>Référents</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="96"/>
        <source>USING expr.</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="96"/>
        <source>CHECK expr.</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
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
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1271"/>
        <source>Also restart sequences</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1896"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1919"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1897"/>
        <source>You&apos;re running a demonstration version! The data manipulation feature is available only in the full version!</source>
        <translation>Vous utilisez une version de démonstration ! Cette fonctionnalité de manipulation de données est uniquement disponible dans la version complète !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1920"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; You are about to drop the entire database &lt;strong&gt;%1&lt;/strong&gt;! All data will be completely wiped out. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Vous êtes sur le point de supprimer l&apos;intégralité de la base de données &lt;strong&gt;%1&lt;/strong&gt; ! Toutes les données seront définitivement détruites. Voulez-vous vraiment le faire ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="125"/>
        <source>Open the grid to visualize or edit data</source>
        <translation>Ouvrir la grille pour visualiser ou modifier les données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="154"/>
        <source>Open a new SQL execution pane</source>
        <translation>Ouvrir un nouveau panneau d&apos;exécution de code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="170"/>
        <source>Ctrl+F6</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="180"/>
        <source>Update the objects tree</source>
        <translation>Mettre à jour l&apos;arborescence des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="203"/>
        <source>Drop this database</source>
        <translation>Supprimer cette base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="460"/>
        <source>Filters the currently loaded items in the tree by using a pattern and matching their names. If &lt;strong&gt;By OID&lt;/strong&gt; is checked the pattern is interpreted as an integer value that represents the object id (OID). &lt;br&gt;&lt;br/&gt;&lt;strong&gt;HINT:&lt;/strong&gt; if you need to search the entire database use the full refresh (&lt;strong&gt;Ctrl+F5&lt;/strong&gt;) prior the filtering.</source>
        <translation>Filtre les éléments actuellement chargés dans l&apos;arborescence en utilisant un motif et en faisant correspondre leurs noms. Si &lt;strong&gt;Par OID&lt;/strong&gt; est coché, le motif est interprété comme une valeur entière qui représente l&apos;identifiant de l&apos;objet (OID).&lt;br&gt;&lt;br/&gt;&lt;strong&gt;ASTUCE :&lt;/strong&gt; si vous devez effectuer une recherche dans toute la base de données, utilisez l&apos;actualisation complète (&lt;strong&gt;Ctrl+F5&lt;/strong&gt;) avant le filtrage.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="616"/>
        <source>Show raw attributes</source>
        <translation>Afficher les attributs brutes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="32"/>
        <source>-- Source code not generated! Hit F7 or middle-click the item to load it. --</source>
        <translation>-- Code source non généré ! Appuyer sur F7 ou faire un clic molette sur l&apos;élément pour le charger. --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="153"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="156"/>
        <source>Source code</source>
        <translation>Code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="208"/>
        <source>Quick refresh</source>
        <translation>Rafraîchissement rapide</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="211"/>
        <source>Full refresh</source>
        <translation>Rafraîchissement complet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1196"/>
        <source>Do you really want to &lt;strong&gt;cascade&lt;/strong&gt; drop the object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;? This action will drop all the other objects that depends on it.</source>
        <translation>Voulez-vous vraiment supprimer en &lt;strong&gt;cascade&lt;/strong&gt; l&apos;objet &lt;strong&gt;%1&lt;/strong&gt;&lt;em&gt; (%2)&lt;/em&gt; ? Cette action supprimera tous les autres objets qui en dépendent.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1557"/>
        <source>Src. table: %1
Src. column(s): %2</source>
        <translation>Table src. : %1
Colonne(s) src. : %2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1568"/>
        <source>Ref. table: %1
Ref. column(s): %2</source>
        <translation>Table de ref. : %1
Colonne(s) de ref. : %2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1768"/>
        <source>-- Source code genaration for buil-in and base types currently unavailable --</source>
        <translation>-- Génération de code source pour les types intégrés et les type de base actuellement indisponibles --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="1822"/>
        <source>-- Source code unavailable for the object %1 (%2). --</source>
        <translation>-- Code source indisponible pour l&apos;objet %1 (%2). --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="48"/>
        <source>Toggle the display of filter widget as well the system/extension objects.</source>
        <translation>(Dés)activer l&apos;affichage du widget de filtre ainsi que les objets système/extension.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="85"/>
        <source>Sort items alphabetically. When unchecked, items are sorted by OID.</source>
        <translation>Trier les éléments par ordre alphabétique. Lorsque ce n&apos;est pas coché, les éléments sont triés par OID.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="88"/>
        <source>Sort alphabetically</source>
        <translation>Trier par ordre alphabétique</translation>
    </message>
</context>
<context>
    <name>DatabaseImportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="1141"/>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="1179"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="170"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="399"/>
        <source>Enables the import of objects created by extensions. Generally there is no need to check this option but if there are objects in the database that directly references this category of objects this mode must be enabled.</source>
        <translation>Active l&apos;import des objets créés par les extensions. Généralement il n&apos;y a pas lieu d&apos;activer cette option sauf si des objets en base référencent directement ce type d&apos;objet.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="405"/>
        <source>Import extension objects</source>
        <translation>Importer les objets des extensions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="194"/>
        <source>Connection:</source>
        <translation>Connexion :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="450"/>
        <source>Ignore import errors</source>
        <translation>Ignorer les erreurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="444"/>
        <source>pgModeler ignores import errors and will try to create as many as possible objects. By checking this option the import operation will be not aborted but an incomplete model will be constructed. This option generates a log file on pgModeler&apos;s temp directory.</source>
        <translation>pgModeler ignorera les erreurs qui pourront survenir lors de l&apos;import et essayera de créer autant d&apos;objets qui lui sera possible. En activant cette fonction le processus d&apos;import ne sera pas interrompu, toutefois vous pourrez vous retrouver avec un modèle incomplet. Cette option active la génération d&apos;un fichier log dans le dossier temporaire de pgModeler.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="354"/>
        <source>Enables the import of system built-in objects. It&apos;s recommend to select only those objects that are directly referenced by the ones to be imported. WARNING: Try to import a huge set of system objects can bloat the resultant model or even crash pgModeler due to memory/cpu overuse.</source>
        <translation>Active l&apos;importation d&apos;objets intégrés au système. Il est recommandé de sélectionner uniquement les objets directement référencés par ceux à importer. AVERTISSEMENT : Essayer d&apos;importer un grand nombre d&apos;objets système peut gonfler le modèle résultant ou même planter pgModeler en raison de la surutilisation de la mémoire/cpu.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="360"/>
        <source>Import system objects</source>
        <translation>Importer les objets système</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="215"/>
        <source>Automatically resolve dependencies</source>
        <translation>Résoudre automatiquement les dépendances</translation>
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
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="276"/>
        <source>Debug mode</source>
        <translation>Activer le mode debug</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="139"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="506"/>
        <source>Random colors will be assigned to imported relationships facilitating the identification of links between tables mainly in large models.</source>
        <translation>Colorer les associations de manière aléatoire afin de faciliter l&apos;identification des liens entre les tables, cela peut être utile sur de grands modèles.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="509"/>
        <source>Random colors for relationships</source>
        <translation>Colorer les associations de manière aléatoire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="553"/>
        <source>Database</source>
        <translation>Base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="762"/>
        <source>Select all objects</source>
        <translation>Tout cocher</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="800"/>
        <source>Clear object selection</source>
        <translation>Tout décocher</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="838"/>
        <source>Expands all items</source>
        <translation>Développer tous les éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="876"/>
        <source>Collapses all items</source>
        <translation>Replier tous les éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="702"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="722"/>
        <source>Filter object by it&apos;s OID</source>
        <translation>Filtrer un objet par son OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="725"/>
        <source>By OID</source>
        <translation>Par OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="920"/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="989"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="999"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="644"/>
        <source>Retrieving objects from database...</source>
        <translation>Récupération des objets depuis la base de données...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="397"/>
        <source>Importing process aborted!</source>
        <translation>Processus d&apos;import abandonnée !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="508"/>
        <source>Importing process canceled by user!</source>
        <translation>Processus d&apos;import interrompu par l&apos;utilisateur !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="532"/>
        <source>Importing process sucessfuly ended!</source>
        <translation>Processus d&apos;import terminé avec succès !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="596"/>
        <source>No databases found</source>
        <translation>Aucune base de données trouvée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="616"/>
        <source>Found %1 database(s)</source>
        <translation>%1 base(s) de données trouvée(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="646"/>
        <source>Retrieving cluster level objects...</source>
        <translation>Récupération des objets du cluster...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="693"/>
        <source>Retrieving objects of schema `%1&apos;...</source>
        <translation>Récupération des objets du schéma `%1&apos;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="707"/>
        <source>Retrieving objects of `%1&apos; (%2)...</source>
        <translation>Réccupération des objets de `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="833"/>
        <source>This is a PostgreSQL built-in data type and cannot be imported.</source>
        <translation>C&apos;est un type de données nativement intégré à PostgreSQL. Il ne peut être importé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="845"/>
        <source>This is a pgModeler&apos;s built-in object. It will be ignored if checked by user.</source>
        <translation>C&apos;est un objet nativement intégré à pgModeler. Il sera ignoré s&apos;il est coché par l&apos;utilisateur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="309"/>
        <source>Create all imported objects in the current working model instead of create a new one.</source>
        <translation>Créer tous les objets importés dans le modèle de travail actuel au lieu d&apos;en créer un nouveau.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="312"/>
        <source>Import objects to the working model</source>
        <translation>Importer les objets dans le modèle de travail en cours</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="194"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; You are about to import objects to the current working model! This action will cause irreversible changes to it even in case of critical errors during the process. Do you want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Vous êtes sur le point d&apos;importer des objets dans le modèle de travail actuel ! Cette action entraînera des modifications irréversibles même en cas d&apos;erreurs critiques au cours du processus. Voulez-vous poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="209"/>
        <source>Resolve some of the object&apos;s dependencies by querying the catalog when a needed object does not exists on the loaded set. In some cases it&apos;s necessary to combine this option with others below. This option does not applies to database level objects like role, tablespace and language as well for data types, extensions.</source>
        <translation>Résoudre certaines dépendances de l&apos;objet en interrogeant le catalogue. Dans certains cas, il est nécessaire de combiner cette option avec les autres ci-dessous. Cette option ne s&apos;applique pas aux objets du niveau de la base de données, tels que le rôle, l&apos;espace de stockage et la langue, pour les types de données et les extensions.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="270"/>
        <source>All catalog queries as well the created objects&apos; source code are printed to standard output (stdout).</source>
        <translation>Toutes les requêtes de catalogue ainsi que le code source des objets créés sont imprimés sur la sortie standard (stdout).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="35"/>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="122"/>
        <source>Import database</source>
        <translation>Importer une base de données</translation>
    </message>
</context>
<context>
    <name>DatabaseImportHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="183"/>
        <source>Retrieving system objects... `%1&apos;</source>
        <translation>Récupération des objets système... `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="233"/>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="257"/>
        <source>Retrieving objects... `%1&apos;</source>
        <translation>Récupération des objets... `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="2246"/>
        <source>Creating table inheritances...</source>
        <translation>Création des tables héritées...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="428"/>
        <source>Creating object `%1&apos; (%2)...</source>
        <translation>Création de l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="475"/>
        <source>Creating columns permissions...</source>
        <translation>Création des permissions sur les champs...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="2271"/>
        <source>Destroying unused detached columns...</source>
        <translation>Destruction des colonnes détachées inutilisées...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="591"/>
        <source>The database import ended but some errors were generated and saved into the log file `%1&apos;. This file will last until pgModeler quit.</source>
        <translation>L&apos;import de la base de données est términée mais des erreurs ont été générées et sauvegardées dans le journal `%1&apos;. Ce fichier sera disponible jusqu&apos;à la fermeture de pgModeler.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="314"/>
        <source>Creating object `%1&apos; (%2), oid `%3&apos;...</source>
        <translation>Création de l&apos;objet `%1&apos; (%2), oid `%3&apos;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="364"/>
        <source>Trying to recreate object `%1&apos; (%2), oid `%3&apos;...</source>
        <translation>Tentative de création de l&apos;objet `%1&apos; (%2), oid `%3&apos;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="388"/>
        <source>Import failed to recreate some objects in `%1&apos; tries.</source>
        <translation>L&apos;import a échoué à recréer certains objets à `%1&apos; reprise(s).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="457"/>
        <source>Creating permissions for object `%1&apos; (%2)...</source>
        <translation>Création des autorisations pour l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="526"/>
        <source>Updating relationships of `%1&apos; (%2)...</source>
        <translation>Mise à jour des relations de `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="563"/>
        <source>Validating relationships...</source>
        <translation>Validation des relations...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="2308"/>
        <source>Assigning sequences to columns...</source>
        <translation>Affectation des séquences aux colonnes...</translation>
    </message>
</context>
<context>
    <name>DatabaseModel</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="375"/>
        <source>The demonstration version can create only `%1&apos; instances of each object type! You&apos;ve reach this limit for the type: `%2&apos;</source>
        <translation>Cette version de démonstration peut seulement créer `%1&apos; instances pour chaque type d&apos;objet ! Cette limite a été atteinte pour les objets de type : `%2&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3006"/>
        <source>Loading: `%1&apos; (%2)</source>
        <translation>Chargement : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3053"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3065"/>
        <source>Validating relationships...</source>
        <translation>Validation des relations...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="6572"/>
        <source>Generating %1 code: `%2&apos; (%3)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9333"/>
        <source>Saving object `%1&apos; (%2)</source>
        <translation>Enregistrement de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9488"/>
        <source>Saving metadata of the object `%1&apos; (%2)</source>
        <translation>Enregistrement des métadonnées de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9512"/>
        <source>Metadata file successfully saved!</source>
        <translation>Fichier de métadonnées enregistré avec succès !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9515"/>
        <source>Process successfully ended but no metadata was saved!</source>
        <translation>Le processus s&apos;est terminé avec succès mais aucune métadonnée n&apos;a été enregistrée !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9597"/>
        <source>Creating object `%1&apos; (%2)</source>
        <translation>Création de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9603"/>
        <source>Object `%1&apos; (%2) already exists. Ignoring.</source>
        <translation>L&apos;objet `%1&apos; (%2) existe déjà et a été ignoré.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9655"/>
        <source>Loading metadata for object `%1&apos; (%2)</source>
        <translation>Chargement des métadonnées de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9781"/>
        <source>Object `%1&apos; (%2) not found. Ignoring metadata.</source>
        <translation>Objet`%1&apos; (%2) introuvable. Métadonnée ignorée.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9793"/>
        <source>Metadata file successfully loaded!</source>
        <translation>Fichier de métadonnées chargé avec succès !</translation>
    </message>
</context>
<context>
    <name>DatabaseWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="39"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="69"/>
        <source>Template DB:</source>
        <translation>Modèle de BDD :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="92"/>
        <source>Model Author:</source>
        <translation>Auteur du modèle :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="121"/>
        <source>Encoding:</source>
        <translation>Encodage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="344"/>
        <source>Default Objects</source>
        <translation>Attributs par défaut des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="362"/>
        <source>Tablespace:</source>
        <translation>Espace de stockage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="369"/>
        <source>Schema:</source>
        <translation>Schéma :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="376"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="383"/>
        <source>Owner:</source>
        <translation>Propriétaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="157"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="199"/>
        <source>Connections:</source>
        <translation>Connexions :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="244"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="37"/>
        <source>The fields &lt;strong&gt;LC_COLLATE&lt;/strong&gt; and &lt;strong&gt;LC_CTYPE&lt;/strong&gt; have pre-configured values based upon the running system. You can freely modify those values if you intend to export the model to another host.</source>
        <translation>Les champs &lt;strong&gt;LC_COLLATE&lt;/strong&gt; et &lt;strong&gt;LC_CTYPE&lt;/strong&gt; ont déjà des valeurs pré-configurées basées sur le système. Vous pouvez librement les modifier si vous devez exporter le modèle vers une autre machine hôte.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="42"/>
        <source>Use the above fields to specify the default attributes assigned to new objects created on the database model. Leaving a field empty will cause PostgreSQL to use the default values when exporting the model.</source>
        <translation>Utilisez les champs ci-dessus pour spécifier les attributs par défaut à assigner aux nouveaux objets créés dans le modèle. Laisser un champ vide fera que PostgreSQL utilisera ses valeurs par défaut lors de l&apos;export du modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="55"/>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="67"/>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="135"/>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="140"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="286"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="295"/>
        <source>Allow connections</source>
        <translation>Autoriser les connexions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="305"/>
        <source>Is template</source>
        <translation>Est le modèle</translation>
    </message>
</context>
<context>
    <name>DomainWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="91"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="51"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="33"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="63"/>
        <source>Not null</source>
        <translation>Non nulle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="73"/>
        <source>Check constraints</source>
        <translation>Vérifier les contraintes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="101"/>
        <source>Expression:</source>
        <translation>Expression :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/domainwidget.cpp" line="42"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/domainwidget.cpp" line="45"/>
        <source>Expression</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>DonateWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="71"/>
        <source>Donate to pgModeler</source>
        <translation>Faire un don à pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="96"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="99"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="133"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;pgModeler is brought to you thanks to a &lt;span style=&quot; font-style:italic;&quot;&gt;great effort to create and distribute a quality product&lt;/span&gt;. This project is reaching out levels of maturity never imagined. All this is the result of a joint work between its author and the &lt;span style=&quot; font-weight:600;&quot;&gt;Open Source community&lt;/span&gt;. &lt;br/&gt;&lt;br/&gt;This software has a long way to go yet and with your help we&apos;ll keep maintaining the good job and bringing new improvements on each release. If you did like pgModeler and thinks it deserves a contribution please make a donation!&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;pgModeler vous est offert grâce à un &lt;span style=&quot; font-style :italic;&quot;&gt;gros effort pour créer et distribuer un produit de qualité &lt;/span&gt;. Ce projet atteint un niveau de maturité jamais imaginé. Tout ceci est le résultat d&apos;un travail conjoint entre son auteur et la &lt;span style=&quot;font-weight :600;&quot;&gt;Communauté Open Source&lt;/span&gt;.&lt;br/&gt;&lt;br/&gt;Ce logiciel a encore un long chemin à parcourir mais avec votre aide, nous continuerons à faire du bon travail et à apporter de nouvelles améliorations à chaque version. Si vous avez aimé pgModeler et pensez qu&apos;il mérite une contribution, faites un don ! &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="182"/>
        <source>I want to help!</source>
        <translation>Je veux aider !</translation>
    </message>
</context>
<context>
    <name>ElementsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="180"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="224"/>
        <source>Expression:</source>
        <translation>Expression :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="243"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="53"/>
        <source>Operator Class:</source>
        <translation>Classe d&apos;opérateurs :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="199"/>
        <source>Operator:</source>
        <translation>Opérateur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="106"/>
        <source>Sorting:</source>
        <translation>Tri :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="124"/>
        <source>Ascending</source>
        <translation>Ascendant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="140"/>
        <source>Descending</source>
        <translation>Descendant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="153"/>
        <source>Nulls first</source>
        <translation>Null en premier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="40"/>
        <source>Element</source>
        <translation>Élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="42"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="44"/>
        <source>Operator Class</source>
        <translation>Classe d&apos;opérateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="46"/>
        <source>Sorting</source>
        <translation>Tri</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="47"/>
        <source>Nulls First</source>
        <translation>Vides en premier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="132"/>
        <source>Collation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="151"/>
        <source>Operator</source>
        <translation>Operateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="248"/>
        <source>Expression</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="269"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="271"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>EventTriggerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="29"/>
        <source>Event:</source>
        <translation>Évènement :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="56"/>
        <source>Function:</source>
        <translation>Fonction :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="69"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="89"/>
        <source>Tag:</source>
        <translation>Étiquette :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/eventtriggerwidget.cpp" line="37"/>
        <source>Tag command</source>
        <translation>Balise de la commande</translation>
    </message>
</context>
<context>
    <name>Exception</name>
    <message>
        <location filename="../libutils/src/exception.cpp" line="23"/>
        <source>Assignment of a pseudo-type to the type of the column!</source>
        <translation>Utilisation d&apos;un pseudo-type comme type de la colonne !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="24"/>
        <source>Assignment of a precision greater than the length of the type!</source>
        <translation>Utilisation d&apos;une précision est plus grande que la longueur du type !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="25"/>
        <source>Assignment of an invalid precision to type time, timestamp or interval. The precision in this case must be equal to or less than 6!</source>
        <translation>Utilisation d&apos;une précision de type time, timestamp ou interval invalide. La précision de ces types doit être inférieure ou égale à 6 !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="27"/>
        <source>Reference to a column which index is out of the capacity of the column list!</source>
        <translation>Référence à une colonne dont l&apos;index dépasse la capacité de la liste des colonnes !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="28"/>
        <source>Assignment of not allocated object!</source>
        <translation>Utilisation d&apos;un objet non référencé !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="33"/>
        <source>Assigning object of an invalid type!</source>
        <translation>Assignation d&apos;un objet dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="34"/>
        <source>Removing an object of an invalid type!</source>
        <translation>Suppression d&apos;un objet dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="35"/>
        <source>Obtaining an object of an invalid type!</source>
        <translation>L&apos;objet obtenu est de type invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="36"/>
        <source>Assignment of empty name to table return type!</source>
        <translation>Affectation d&apos;un nom vide au type retourné par la table !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="40"/>
        <source>Reference to an event which does not belongs to trigger!</source>
        <translation>Référence à un évènement qui n&apos;appartient pas au déclencheur !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="45"/>
        <source>Assignment of a function which language is invalid!</source>
        <translation>Définition d&apos;une fonction dont le langage est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="49"/>
        <source>Assignment of empty name to an object!</source>
        <translation>Affectation d&apos;un nom vide à l&apos;objet !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="52"/>
        <source>Assignment of schema object which type is invalid!</source>
        <translation>Affectation d&apos;un schema dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="53"/>
        <source>Assignment of tablespace object with invalid type!</source>
        <translation>Utilisation d&apos;un espace logique dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="54"/>
        <source>Assignment of tablespace to an invalid object!</source>
        <translation>Affectation d&apos;un espace logique à un objet invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="55"/>
        <source>Assignment of tablespace to a constraint which type is invalid! To belong to a tablespace the constraint must be a primary key or unique!</source>
        <translation>Affectation d&apos;un espace logique à une contrainte dont le type est invalide ! Pour appartenir à un espace logique une contrainte doit être une clé primaire ou unique !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="56"/>
        <source>Assignment of owner object which type is invalid!</source>
        <translation>Affectation d&apos;un propiétaire dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="57"/>
        <source>Assignment of owner to an invalid object!</source>
        <translation>Affectation d&apos;un propriétaire à un objet invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="59"/>
        <source>Reference to a function with invalid type!</source>
        <translation>Référence à une fonction dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="60"/>
        <source>Reference to an argument of the operator with invalid type!</source>
        <translation>Référence à un argument de l&apos;opérateur dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="61"/>
        <source>Reference to an operator with invalid type!</source>
        <translation>Référence à un opérateur dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="62"/>
        <source>Assignment of value to an invalid option type on role!</source>
        <translation>Utilisation d&apos;un attribut invalide pour un rôle !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="63"/>
        <source>Reference to an invalid role type!</source>
        <translation>Référence à un type de rôle invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="68"/>
        <source>Insertion of empty command to the rule!</source>
        <translation>Insertion d&apos;une commande vide dans cette règle !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="70"/>
        <source>Is not possible to create a self generalization/copy relationship! The table can not inherit or copy their own attributes!</source>
        <translation>Impossible de créer une association de généralisation/copie de relation avec elle même ! Une table ne peut hériter ou copier ses propres attributs !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="71"/>
        <source>Assignment of an object that already belongs to another table!</source>
        <translation>Utilisation d&apos;un objet appartenant déjà à une autre table !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="72"/>
        <source>Assignment of a schema to the sequence which differs from the schema of the owner table!</source>
        <translation>Affectation d&apos;un schéma à la séquence diffèrent du schéma de la table !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="73"/>
        <source>Assignment of an invalid value to one of the sequence attributes!</source>
        <translation>Utilisation d&apos;une valeur invalide comme attribut de séquence !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="74"/>
        <source>Assignment of a minimum value to the sequence which is greater than the maximum value!</source>
        <translation>La valeur minimale de la séquence est supérieure à la valeur maximale !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="76"/>
        <source>Assignment of a null increment value to the sequence!</source>
        <translation>La valeur de l&apos;incrément de la séquence est nulle !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="77"/>
        <source>Assignment of null cache value to the sequence!</source>
        <translation>La valeur cache de la séquence est nulle !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="83"/>
        <source>Allocation of object with invalid type!</source>
        <translation>Le type de l&apos;objet alloué est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="86"/>
        <source>Assignment of not allocated language!</source>
        <translation>Utilisation d&apos;un langage inconnu !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="87"/>
        <source>Assignment of language object which type is invalid!</source>
        <translation>Définition d&apos;un objet langage dont le type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="88"/>
        <source>Reference to data type with an index outside the capacity of data types list!</source>
        <translation>Référence à un type de donnée dont l&apos;index est en dehors de la capacité de la liste des types de données !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="90"/>
        <source>Assignment of invalid type to the object!</source>
        <translation>Affectation d&apos;un type invalide à l&apos;objet !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="92"/>
        <source>Obtaining types with invalid quantity!</source>
        <translation>Utilisation d&apos;une quantité de types invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="93"/>
        <source>Insertion of item which already exists in the attributes list of the type!</source>
        <translation>Insertion d&apos;un attribut déjà présent dans la définition de ce type !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="94"/>
        <source>Insertion of invalid item in the attributes list of the type!</source>
        <translation>Insertion d&apos;un attribut invalide à la liste des attributs de ce type !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="95"/>
        <source>Insertion of item which already exists in the enumarations list of the type!</source>
        <translation>Insertion d&apos;un élément déjà existant dans l&apos;énumeration !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="96"/>
        <source>Insertion of invalid item in the enumerations list of the type!</source>
        <translation>Insertion d&apos;un élément invlaide dans l&apos;énumeration !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="99"/>
        <source>Assignment of invalid configuration to the type!</source>
        <translation>La configuration assigné au type est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="100"/>
        <source>Assignment of an operator which input type count is invalid to aggregate function!</source>
        <translation>Utilisation d&apos;un opérateur dont le nombre d&apos;argument est invalide avec une fonction d&apos;agrégation !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="101"/>
        <source>Assignment of an operator which types of arguments is invalid!</source>
        <translation>Utilisation d&apos;un opérateur dont le type des arguments est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="129"/>
        <source>There is already a relationship between `%1&apos; (%2) and `%3&apos; (%4) in the model! When using relationships of the type generalization, copy and one-to-one there can&apos;t be other relationships linked to the pair of tables.</source>
        <translation>Il y a déjà une relation entre `%1 &apos;(%2) et `%3&apos; (%4) dans le modèle ! Lors de l&apos;utilisation de relations de type généralisation, copie et un-à-un, il ne peut y avoir d&apos;autres relations liées à la paire de tables.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="162"/>
        <source>A view reference must be used in at least one these SQL scopes: View Definition, SELECT-FROM, FROM-WHERE or After WHERE!</source>
        <translation>Lors de la définition d&apos;une vue, une référence doit utiliser au moins un des champs SQL : SELECT-FROM, FROM-WHERE ou après WHERE !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="164"/>
        <source>Unable to load the configuration file `%1&apos;! Please check if file exists in its folder and/or if it is not corrupted!</source>
        <translation>Impossible de charger le fichier de configuration `%1&apos;! Merci de vérifier que le fichier existe dans son dossier et / ou s&apos;il n&apos;est pas corrompu !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="165"/>
        <source>Could not find the default settings file `%1&apos;! To restore default settings check the existence of the file and try again!</source>
        <translation>Impossible de trouver le fichier des préférences par défaut `%1&apos; ! Pour restaurer les préférences par défaut veuillez vérifier l&apos;existance de ce fichier et réessayez !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="167"/>
        <source>Could not load the plugin `%1&apos; from the library `%2&apos;! Message returned by plugin manager: `%3&apos;</source>
        <translation>Impossible de charger l&apos;extension `%1&apos; depuis la bibliothèque`%2&apos; ! Message retourné par le gestionnaire d&apos;extension : `%3&apos;</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="250"/>
        <source>Failed to drop the database `%1&apos; because it is defined as the default database for the connection `%2&apos;!</source>
        <translation>Impossible de supprimer la base de données `%1 &apos;car elle est définie comme base de données par défaut pour la connexion`%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="251"/>
        <source>The column `%1&apos; must be `NOT NULL&apos; because it composes the primary key of the table `%2&apos;. You need to remove the column from the mentioned contraint in order to disable the `NOT NULL&apos; on it!</source>
        <translation>La colonne `%1&apos; doit être `NOT NULL&apos; car elle compose la clé primaire de la table `%2&apos;. Vous devez supprimer la colonne de la contrainte mentionnée afin de désactiver le `NOT NULL&apos; dessus !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="252"/>
        <source>The identity column `%1&apos; has an invalid data type! The data type must be `smallint&apos;, `integer&apos; or `bigint&apos;.</source>
        <translation>La colonne d&apos;identité `%1&apos; a un type de données invalide ! Le type de données doit être `smallint&apos;, `integer&apos; ou `bigint&apos;.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="253"/>
        <source>Reference to an invalid affected command in policy `%1&apos;!</source>
        <translation>Référence à une commande affectée invalide dans la règle `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="254"/>
        <source>Reference to an invalid special role in policy `%1&apos;!</source>
        <translation>Référence à un rôle spécial invalide dans la politique `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="199"/>
        <source>Assignment of a second definition expression to the view!</source>
        <translation>Assignation d&apos;une seconde expression à la vue !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="201"/>
        <source>Assignment of collation object which type is invalid!</source>
        <translation>Utilisation d&apos;un objet collation invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="203"/>
        <source>Collations must be created at least with attributes LC_COLLATE and LC_CTYPE defined!</source>
        <translation>Les collations doivent être définies avec les attributs LC_COLLATE et LC_CTYPE renseignés !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="206"/>
        <source>Reference to an invalid copy table option!</source>
        <translation>Référence à une option invalide de copie de table !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="208"/>
        <source>The INSTEAD OF mode cannot be used on triggers that belongs to tables! This is available only for view triggers!</source>
        <translation>Le mode AU LIEU DE (INSTEAD OF) ne peut être utilisé comme déclencheur de table ! Ce n&apos;est valable que pour les déclencheurs de vue !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="209"/>
        <source>The TRUNCATE event can only be used when the trigger executes for each statement and belongs to a table!</source>
        <translation>L&apos;événement TRUNCATE ne peut être utilisé que lors d&apos;un déclencheur sur chaque ligne et que s&apos;il appartient à une table !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="210"/>
        <source>The INSTEAD OF mode cannot be used on view triggers that executes for each statement!</source>
        <translation>Le mode AU LIEU DE (INSTEAD OF) ne peut être utilisé avec des déclencheurs de vues qui s’exécutent à chaque requête !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="211"/>
        <source>Constraint triggers can only be executed on AFTER events and for each row!</source>
        <translation>Les déclencheurs de contraintes ne peuvent être exécutés qu&apos;APRÈS les événements et pour chaque ligne !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="212"/>
        <source>A view trigger cannot be AFTER/BEFORE when it executes for each row!</source>
        <translation>Les déclencheurs de vues ne peuvent être AFTER/BEFORE quand il sont configurés pour une exécution sur chaque ligne !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="213"/>
        <source>A trigger cannot make reference to columns when using INSTEAD OF mode and UPDATE event!</source>
        <translation>Un déclencheur ne peut faire référence aux colonnes quand il est utilisé en mode INSTEAD OF sur un événement de type UPDATE !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="215"/>
        <source>Only constraint triggers can be deferrable or reference another table!</source>
        <translation>Seuls les déclencheurs de contraintes peuvent être déférés ou référencer une autre table !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="219"/>
        <source>The validation process failed due to an error triggered by the validation helper. For more details about the error check the exception stack!</source>
        <translation>La procédure de validation a échouée à cause d&apos;une erreur levée par l&apos;assistant de validation. Pour de plus d&apos;informations, referez-vous à la pile des exceptions !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="104"/>
        <source>Assignment of an invalid strategy/support number to an operator class element!</source>
        <translation>Affectation d&apos;un numéro de stratégie/support invalide à un élément de la classe &apos;operator&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="105"/>
        <source>Insertion of element which already exists in the element list!</source>
        <translation>Insertion d&apos;un élément déjà existant dans la liste des éléments !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="39"/>
        <source>Reference to a parameter which index is out of the parameter list bounds!</source>
        <translation>Référence un paramètre dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="48"/>
        <source>Reference to an argument which index is out of argument list bounds!</source>
        <translation>Référence un argument dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="50"/>
        <source>Assignment of a name which contains invalid characters!</source>
        <translation>Utilisation de caractères invalides dans le nom !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="51"/>
        <source>Assignment of a name which length exceeds the maximum of 63 characters!</source>
        <translation>Utilisation de plus de 63 caractères dans le nom !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="67"/>
        <source>Reference to a role which index is out of role list bounds!</source>
        <translation>Référence un rôle dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="69"/>
        <source>Reference to a command which index is out of the command list bounds!</source>
        <translation>Référence une commande dont l&apos;index est hors des limites de la liste de ces dernières !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="75"/>
        <source>Assignment of a start value to the sequence which is extrapolating the range defined by minimum and maximum values!</source>
        <translation>Affectation de la valeur de départ de la séquence en dehors de la plage de cette dernière définie par les valeurs min et max !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="82"/>
        <source>Reference to a label which index is out of labels list bounds!</source>
        <translation>Référence un label dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="97"/>
        <source>Reference to an attribute which index is out of the attributes list bounds!</source>
        <translation>Référence un attribut dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="98"/>
        <source>Reference to an enumeration which index is out of the enumerations list bounds!</source>
        <translation>Référence une énumération dont l&apos;index est hors des limites de la liste de ces dernières !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="106"/>
        <source>Reference to an element which index is out of element list bounds!</source>
        <translation>Référence un élément dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="107"/>
        <source>Reference to an object which index is out of object list bounds!</source>
        <translation>Référence un objet dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="108"/>
        <source>Removal of an object not allocated!</source>
        <translation>Suppression d&apos;un objet non alloué !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="160"/>
        <source>The object `%1&apos; (%2) can not be manipulated because it is reserved to PostgreSQL! This object is present in the database model only as a reference!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut être modifié car il est est réservé à PostgreSQL ! Cet objet apparaît dans le modèle uniquement comme référence !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="111"/>
        <source>Operation with object(s) which type(s) is invalid!</source>
        <translation>Opérations sur un ou des objets dont les types sont invalides !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="112"/>
        <source>Reference to object with invalid type!</source>
        <translation>Référence à un objet ayant un type invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="113"/>
        <source>Operation with object not allocated!</source>
        <translation>Opération sur des objets non alloués !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="115"/>
        <source>The relationship of the type 1-1 where both tables are mandatory participation is not implemented because it requires fusion between the tables that breaks the modeling done by the user!</source>
        <translation>Une association de type 1-1, quand les deux tables sont obligatoires, n&apos;est pas implémentée car cela requiert la fusion des tables, ce qui casse le modèle réalisé par l&apos;utilisateur !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="116"/>
        <source>Assignment of an invalid expression to the object!</source>
        <translation>Affectation d&apos;une expression invalide à l&apos;objet !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="117"/>
        <source>Assignment of a primary key to a table which already has one!</source>
        <translation>Impossible de définir une clé primaire à une table qui en possède déjà une !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="118"/>
        <source>Identifier relationship can not be created for a self relationship, relationships of the type n-n, copy or generalization!</source>
        <translation>La relation d&apos;identifiant ne peut pas être créée pour une relation vers elle même, pour les relations de type n-n, pour la copie ou pour la généralisation!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="122"/>
        <source>An attribute can not be added to a copy or generalization relationship!</source>
        <translation>Un attribut ne peut être ajouté à une relation copie ou généralisation !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="123"/>
        <source>A foreign key can not be added to a relationship because is created automatically when this is connected!</source>
        <translation>Une clé étrangère ne peut être ajoutée à une relation car elle est créée automatiquement lors de la mise en relation !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="125"/>
        <source>Reference to an user-defined data type that not exists in the model!</source>
        <translation>Référence à un type de donné défini par l&apos;utilisateur qui n&apos;existe pas dans le modèle !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="126"/>
        <source>Assignment of invalid maximum size to operation list!</source>
        <translation>La taille maximum affectée à la liste d&apos;opérations est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="131"/>
        <source>One or more objects were invalidated and automatically removed because they were referencing table columns which were included through relationships and which no longer exists due to disconnection of relationships or exclusion of such generated columns!</source>
        <translation>Un ou plusieurs objets ont été invalidés et automatiquement supprimés car ils référençaient des colonnes qui étaient incluses dans des associations et qui n&apos;existaient plus du fait de la suppression de l&apos;associations ou de l&apos;exclusion de ces colonnes !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="133"/>
        <source>Reference to an invalid privilege type!</source>
        <translation>Référence à un type de privilège invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="134"/>
        <source>Insertion of a role which already exists in the role list of the permission!</source>
        <translation>Insertion d&apos;un rôle déjà existant dans la liste de cette permission !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="135"/>
        <source>Assignment of privilege incompatible with the type of object referenced by permission!</source>
        <translation>Utilisation d&apos;un privilège incompatible avec le type d&apos;objet référencé par la permission !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="140"/>
        <source>It is not possible to create arrays of domains or sequences (dimension &gt;= 1)! PostgreSQL does not yet implement this feature!</source>
        <translation>Il est impossible de créer des tableaux de domaines ou de séquences (dimension &gt;= 1) ! PostgreSQL n&apos;inplémente pas encore cette fonctionnalité !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="148"/>
        <source>Assignment of invalid name to the table generated from N-N relationship!</source>
        <translation>Le nom de la table générée par la relation N-N est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="158"/>
        <source>Reference to a column of the objects table with invalid index!</source>
        <translation>Référence à une colonne de la table ayant un index invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="159"/>
        <source>Reference to a row of the objects table with invalid index!</source>
        <translation>Référence à une ligne de la table ayant un index invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="163"/>
        <source>Constraints like primary key, foreign key or unique must have at least one column related to them! For foreign keys must be selected, in addition, the referenced columns!</source>
        <translation>Les contraintes telles que clé primaire, clé étrangère ou unicité doivent avoir au moins une colonne associée ! Pour les clés étrangères, les colonnes référencées doivent également être sélectionnées !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="166"/>
        <source>The export process failed due to an error triggered by the PostgreSQL server in an attempt to execute a SQL command. For more details about the error check the exception stack!

** Executed SQL command: **

%1</source>
        <translation>Le process d&apos;exportat a échoué suite à une erreur déclenchée par le serveur PostgreSQL lors de la tentative d&apos;exécution d&apos;une commande SQL. Pour plus de détails concernant l&apos;erreur vérifiez la pile d&apos;exceptions !

** Commande SQL exécutée: **

%1</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="168"/>
        <source>One or more plugins were not activated due to errors during the loading process! Check the exception stack for more details.</source>
        <translation>Une ou plusieurs extensions n&apos;ont pas été activées suite à la survenue d&apos;erreurs lors du chargement. Pour plus de détails, consultez la pile d&apos;exceptions.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="176"/>
        <source>Assignment of empty XML buffer to parser!</source>
        <translation>Passage d&apos;un tampon XML vide à l&apos;analyseur !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="178"/>
        <source>Assignment of empty DTD file name!</source>
        <translation>Le nom du fichier DTD est vide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="179"/>
        <source>Assignment of empty name to the DTD declaration!</source>
        <translation>Le nom de la déclaration DTD est vide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="181"/>
        <source>Operation on unallocated element tree! It is necessary to load the XML parser buffer and interpret it so that the tree is generated!</source>
        <translation>Opération sur l&apos;arbre d&apos;élément non alloué ! Il est nécessaire de charger le tampon d&apos;analyseur XML et de l&apos;interpréter pour que l&apos;arbre soit généré !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="183"/>
        <source>Operation with unallocated tree element!</source>
        <translation>Opération avec un élément de l&apos;arbre non alloué !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="184"/>
        <source>Operation with element which does not exists in the element tree currently loaded!</source>
        <translation>Opération avec un élément inexistant dans l&apos;arbre des éléments actullement chargé !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="185"/>
        <source>Assignment of a value to an invalid connection parameter!</source>
        <translation>Affectation d&apos;une valeur à un paramètre de connexion invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="186"/>
        <source>Operation on connection not established!</source>
        <translation>Opération sur une connexion non établie !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="187"/>
        <source>Attempt to connect without define configuration parameters!</source>
        <translation>Tentative de connexion sans paramètre de configuration défini !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="190"/>
        <source>Assignment of not allocated SQL command result!</source>
        <translation>Affectation d&apos;un résultat de commande SQL non alloué !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="191"/>
        <source>Unable to allocate the result of the SQL command because the response from the DBMS was not understood by the client!</source>
        <translation>Impossible de retourner le résultat de la commande SQL car la réponse du SGBD n&apos;a pas été comprise par le client !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="193"/>
        <source>Reference to a column of tuple with invalid index!</source>
        <translation>Référence à une colonne de tuple dont l&apos;index est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="194"/>
        <source>Reference to a column of tuple with invalid name!</source>
        <translation>Référence à une colonne de tuple dont le nom est invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="26"/>
        <source>Assignment of a not allocated column to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une colonne non allouée à l&apos;objet `%1 &apos;(%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="29"/>
        <source>Assignment of a not allocated schema to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un schéma non allouée à l&apos;objet `%1 &apos;(%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="30"/>
        <source>The object `%1&apos; (%2) has inconsistent SQL or XML definition!</source>
        <translation>L&apos;objet `%1 &apos;(%2) a une définition SQL ou XML incohérente !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="31"/>
        <source>The object `%1&apos; (%2) already exists on `%3&apos; (%4)!</source>
        <translation>L&apos;objet `%1 &apos;(%2) existe déjà sur `%3&apos; (%4) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="32"/>
        <source>The object `%1&apos; (%2) cannot be assigned because there is already exists in the container object `%3&apos;!</source>
        <translation>L&apos;objet `%1 &apos;(%2) ne peut être assigné car il existe déjà dans l&apos;objet conteneur `%3&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="37"/>
        <source>The insertion of the parameter `%1&apos; will not be possible because there is another parameter with same name in the function `%2&apos;!</source>
        <translation>L&apos;insertion du paramètre `%1&apos; ne sera pas possible car il existe un autre paramètre avec le même nom dans la fonction `%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="38"/>
        <source>The insertion of the table return type `%1&apos; will not be possible because there is another return type with the same name in the `%2&apos;!</source>
        <translation>L&apos;insertion du type de retour de la table `%1&apos; ne sera pas possible car il existe un autre type de retour avec le même nom dans le`%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="41"/>
        <source>The column `%1&apos; cannot be assigned to the trigger `%2&apos; because they belongs to different parent tables!</source>
        <translation>La colonne `%1&apos; ne peut pas être affectée au déclencheur `%2&apos; car ils appartiennent à des tables parentes différentes !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="42"/>
        <source>Assignment of a not allocated function to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction non allouée à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="43"/>
        <source>Assignment of a function which return type is different from `%1&apos;!</source>
        <translation>Affectation d&apos;une fonction dont le type de retour est différent de `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="44"/>
        <source>Assignment of a function which parameter count is invalid to the object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction dont le nombre de paramètres n&apos;est pas valide pour l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="46"/>
        <source>Event trigger function must be coded in any language other than SQL!</source>
        <translation>La fonction de déclenchement d&apos;événement doit être codée dans n&apos;importe quelle langue autre que SQL !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="47"/>
        <source>Assignment of not allocated table to object `%1&apos; (%2)!</source>
        <translation>Affectation de la table non allouée à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="58"/>
        <source>Assignment of appended or prepended SQL to an invalid object!</source>
        <translation>Affectation de code SQL ajouté ou préfixé à un objet invalide !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="64"/>
        <source>The insertion of the role `%1&apos; is not possible because this is already being referenced by role `%2&apos;!</source>
        <translation>L&apos;insertion du rôle `%1 n&apos;est pas possible car il est déjà référencé par le rôle `%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="65"/>
        <source>Reference redundancy detected by having the role `%1&apos; referencing the role `%2&apos;!</source>
        <translation>Redondance de références détectée en ayant le rôle `%1&apos; référençant le rôle `%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="66"/>
        <source>The role `%1&apos; can not be listed as a member of itself!</source>
        <translation>Le rôle `%1&apos; ne peut pas être listé comme un membre de lui-même !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="78"/>
        <source>Assignment of owner table which is not in the same schema as the sequence `%1&apos;!</source>
        <translation>Affectation de la table propriétaire qui n&apos;est pas dans le même schéma que la séquence `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="79"/>
        <source>Assignment of owner table which does not belong to the same owner of the sequence `%1&apos;!</source>
        <translation>Affectation de la table propriétaire qui n&apos;appartient pas au même propriétaire de la séquence `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="80"/>
        <source>Assignment of a nonexistent owner column to the sequence `%1&apos;!</source>
        <translation>Affectation d&apos;une colonne propriétaire inexistante à la séquence `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="81"/>
        <source>Assignment of an owner column to the sequence `%1&apos; that is not related to any table!</source>
        <translation>Affectation d&apos;une colonne propriétaire à la séquence `%1&apos; qui n&apos;est liée à aucune table !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="84"/>
        <source>Assignment of a function with invalid return type to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction avec un type de retour invalide à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="85"/>
        <source>Assignment of a function with invalid parameter(s) type(s) to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction avec un (des) paramètre(s) non valide(s) à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="91"/>
        <source>Assignment of an empty directory to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un répertoire vide à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="102"/>
        <source>Assignment of system reserved name to the object `%1&apos; (%2)!</source>
        <translation>Affectation du nom réservé au système à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="103"/>
        <source>One function with invalid configuration is been used by the object `%1&apos; (%2)!</source>
        <translation>Une fonction avec une configuration invalide est utilisée par l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="109"/>
        <source>The object `%1&apos; (%2) can not be removed because it is being referenced by object `%3&apos; (%4)!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être supprimé car il est référencé par l&apos;objet `%3&apos; (%4) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="110"/>
        <source>The object `%1&apos; (%2) can not be removed because it is being referenced by object `%3&apos; (%4) that belongs to `%5&apos; (%6)!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être supprimé car il est référencé par l&apos;objet `%3&apos; (%4) appartenant à `%5&apos; (%6) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="114"/>
        <source>The creation of the relationship `%1&apos; between the table `%2&apos; and `%3&apos; can not be done because one does not have a primary key. If the relationship is of the type n-n both tables must have primary keys!</source>
        <translation>La création de la relation `%1&apos; entre la table `%2&apos; et `%3&apos; ne peut pas être effectuée car il n&apos;y a pas de clé primaire. Si la relation est du type n-n, les deux tables doivent avoir des clés primaires !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="119"/>
        <source>Unable to create a copy relationship because the column `%1&apos; in table `%2&apos; already exists in table `%3&apos;!</source>
        <translation>Impossible de créer une relation de copie car la colonne `%1&apos; de la table `%2&apos; existe déjà dans la table `%3&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="120"/>
        <source>Unable to create the generalization relationship because the column `%1&apos; in table `%2&apos; can not be merged with the column `%3&apos; of table `%4&apos; because they have incompatible types!</source>
        <translation>Impossible de créer la relation de généralisation car la colonne `%1&apos; de la table `%2&apos; ne peut pas être fusionnée avec la colonne `%3&apos; de la table `%4&apos; car ils ont des types incompatibles !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="124"/>
        <source>The object `%1&apos; (%2) is referencing the object `%3&apos; (%4) which was not found in the model!</source>
        <translation>L&apos;objet `%1&apos; (%2) fait référence à l&apos;objet `%3&apos; (%4) qui n&apos;a pas été trouvé dans le modèle !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="127"/>
        <source>Unable to write the file or directory `%1&apos;! Make sure the output directory exists, or if the user has write permissions over it!</source>
        <translation>Impossible d&apos;écrire le fichier ou le répertoire `%1&apos; ! Assurez-vous que le répertoire de sortie existe et que l&apos;utilisateur a les droits d&apos;écriture dedans !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="132"/>
        <source>The primary key `%1&apos; can only be allocated if declared within a block of code that defines a table or relationship!</source>
        <translation>La clé primaire `%1&apos; ne peut être allouée que si elle est déclarée dans un bloc de code qui définit une table ou une relation !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="136"/>
        <source>There is already a permission on object `%1&apos; (%2) which has one or more equal roles from those present on permission to be assigned to the object!</source>
        <translation>Il y a déjà une permission sur l&apos;objet `%1&apos; (%2) qui a un ou plusieurs rôles égaux à ceux présents dans l&apos;autorisation à assigner à l&apos;objet !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="137"/>
        <source>A permission is referencing the object `%1&apos; (%2) which was not found in the model!</source>
        <translation>Une permission fait référence à l&apos;objet `%1&apos; (%2) qui n&apos;a pas été trouvé dans le modèle !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="138"/>
        <source>The object `%1&apos; (%2) can not be created because its not being assigned to any schema!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être créé car il n&apos;est affecté à aucun schéma !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="139"/>
        <source>The tablespace `%1&apos; can not be inserted into the model because it points to the same directory as the tablespace `%2&apos;!</source>
        <translation>L&apos;espace de stockage `%1&apos; ne peut pas être inséré dans le modèle car il pointe sur le même répertoire que l&apos;espace de table `%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="141"/>
        <source>The function `%1&apos; can not get a source code as a definition because its language is set to C. Use the attributes symbol and dynamic library instead!</source>
        <translation>La fonction `%1&apos; ne peut pas obtenir un code source en tant que définition car sa langue est définie sur C. Utilisez plutôt le symbole d&apos;attributs et la bibliothèque dynamique !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="142"/>
        <source>The function `%1&apos; can have the attributes symbol and dynamic library configured only if the language is set to C. For all other cases you must specify a source code that defines it in the DBMS!</source>
        <translation>La fonction `%1&apos; peut avoir le symbole des attributs et la bibliothèque dynamique configurés uniquement si la langue est définie sur C. Dans tous les autres cas, il faut spécifier un code source qui le définit dans le SGBD !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="143"/>
        <source>The operator `%1&apos; can not be assigned as a comutator of operator `%2&apos; because it has incompatible settings!</source>
        <translation>L&apos;opérateur `%1&apos; ne peut pas être affecté en tant que commutateur de l&apos;opérateur `%2&apos;car il comporte des paramètres incompatibles !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="144"/>
        <source>The operator `%1&apos; can not be assigned as negator of operator `%2&apos; because it has incompatible settings!</source>
        <translation>L&apos;opérateur `%1&apos; ne peut pas être affecté en tant que négateur de l&apos;opérateur `%2&apos; car il a des paramètres incompatibles !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="145"/>
        <source>The type `%1&apos; can not self refer in the attributes `element&apos; or `copy type&apos; or be used as a data type of an attribute in the configuration of a composite type!</source>
        <translation>Le type `%1&apos; ne peut pas se référer à lui-même dans les attributs `element&apos; ou `copy type&apos; ou être utilisé comme type de données d&apos;un attribut dans la configuration d&apos;un type composite !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="146"/>
        <source>Assignment of invalid element to type `%1&apos;!</source>
        <translation>Affectation d&apos;un élément invalide de type `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="147"/>
        <source>Assignment of invalid alignment to type `%1&apos;!</source>
        <translation>Affectation de l&apos;alignement invalide de type `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="149"/>
        <source>The relationship `%1&apos; can not make use of the special primary key because it is marked as identifier or it is a self relationship!</source>
        <translation>La relation `%1&apos; ne peut pas utiliser la clé primaire spéciale car elle est marquée comme identifiant ou c&apos;est une association d&apos;elle-même !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="150"/>
        <source>The object `%1&apos; (%2) can not be edited or deleted because it was automatically included through a relationship! If the object is an attribute or constraint the modifications must be done on the relationship editing form.</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être modifié ou supprimé car il a été automatiquement inclus dans une association ! Si l&apos;objet est un attribut ou une contrainte, les modifications doivent être effectuées sur le formulaire d&apos;édition de la relation.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="151"/>
        <source>The object `%1&apos; (%2) can not be deleted because it is protected!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être supprimé car il est protégé !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="152"/>
        <source>The group `%1&apos; has already been declared earlier!</source>
        <translation>Le groupe `%1&apos; a déjà été déclaré !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="153"/>
        <source>The group `%1&apos; can not be built in the groups declaration block (%2)!</source>
        <translation>Le groupe `%1&apos; ne peut pas être construit dans le bloc de déclaration de groupe (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="154"/>
        <source>The group `%1&apos; was built but not declared in the groups declaration block (%2)!</source>
        <translation>Le groupe `%1&apos; a été construit mais pas déclaré dans le bloc de déclaration de groupe (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="155"/>
        <source>The group `%1&apos; can not be built without possessing child elements!</source>
        <translation>Le groupe `%1&apos; ne peut pas être construit sans posséder d&apos;éléments enfants !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="156"/>
        <source>The group `%1&apos; can not be built once more because this was done in previous blocks!</source>
        <translation>Le groupe `%1&apos; ne peut pas être construit une fois de plus car cela a été fait dans les blocs précédents !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="157"/>
        <source>The group `%1&apos; has been declared but not built!</source>
        <translation>Le groupe `%1&apos; a été déclaré mais pas construit !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="161"/>
        <source>The new configuration of the function invalidates the object `%1&apos; (%2)! In this case it is needed to undo the relationship between the affected object and function in order to the new configuration to take effect!</source>
        <translation>La nouvelle configuration de la fonction invalide l&apos;objet `%1&apos; (%2) ! Dans ce cas, il est nécessaire d&apos;annuler la relation entre l&apos;objet affecté et la fonction afin que la nouvelle configuration prenne effet !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="180"/>
        <source>Error while interpreting XML buffer at line %1 column %2.
Message generated by the parser: %3. %4</source>
        <translation>Erreur lors de l&apos;interprétation du tampon XML à la ligne %1, colonne %2.
Message généré par l&apos;analyseur : %3. %4</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="188"/>
        <source>Attempt to start a connection already stablished!</source>
        <translation>Tentative de lancement d&apos;une connexion déjà établie !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="189"/>
        <source>Could not connect to the database.
Message returned: `%1&apos;</source>
        <translation>Impossible de se connecter à la base de données.
Message renvoyé : `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="192"/>
        <source>Unable to allocate command result for the SQL because the server has generated a fatal error!
Message returned by the DBMS: `%1&apos;</source>
        <translation>Impossible d&apos;allouer le résultat de la commande SQL car le serveur a généré une erreur fatale !
Message renvoyé par le SGBD : `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="196"/>
        <source>Reference to a column of a tuple which was not yet initialized (tuple navigation not started)!</source>
        <translation>Référence à une colonne d&apos;un tuple qui n&apos;a pas encore été initialisé (navigation en tuple non démarrée) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="197"/>
        <source>Could not execute the SQL command.
 Message returned: `%1&apos;</source>
        <translation>Impossible d&apos;exécuter la commande SQL.
 Message renvoyé : `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="200"/>
        <source>It is not possible mix ordinary references (SELECT-FROM, FROM-WHERE, After WHERE) with references used as view SQL definition!</source>
        <translation>Il n&apos;est pas possible de mélanger des références ordinaires (SELECT-FROM, FROM-WHERE, Après WHERE) avec des références utilisées comme vue de définition SQL !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="204"/>
        <source>The object `%1&apos; (%2) cannot reference itself! This operation is not permitted for this kind of object!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas se référencer lui-même ! Cette opération n&apos;est pas autorisée pour ce genre d&apos;objet !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="205"/>
        <source>Only operator families which uses `btree&apos; as indexing method are accepted by operator class elements!</source>
        <translation>Seules les familles d&apos;opérateurs qui utilisent `btree &apos;comme méthode d&apos;indexation sont acceptées par les éléments de classe d&apos;opérateur !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="207"/>
        <source>Copy relationship between tables `%1&apos; and `%2&apos; cannot be done because the first one already copies attributes from `%3&apos;! Tables can have only one copy table!</source>
        <translation>L&apos;association de copie entre les tables `%1&apos; et `%2&apos; ne peut pas être effectuée car la première copie déjà les attributs de `%3&apos; ! Les tables ne peuvent avoir qu&apos;une seule table de copie !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="214"/>
        <source>Assignment of a column which has no parent table to the object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une colonne qui n&apos;a pas de table parent à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="217"/>
        <source>The operator class assigned to the object `%1&apos; (%2) must use `btree&apos; as indexing method!</source>
        <translation>La classe d&apos;opérateur affectée à l&apos;objet `%1&apos; (%2) doit utiliser` btree&apos; comme méthode d&apos;indexation !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="220"/>
        <source>The extension `%1&apos; is registered as a data type and cannot have the attribute `handles datatype&apos; modified!</source>
        <translation>L&apos;extension `%1&apos; est enregistrée en tant que type de données et ne peut pas avoir l&apos;attribut `handle datatype&apos; modifié !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="221"/>
        <source>The fk relationship `%1&apos; cannot be created because the foreign-key that represents it was not created on table `%2&apos;!</source>
        <translation>L&apos;association clé étrangère `%1&apos; ne peut pas être créée car la clé étrangère qui la représente n&apos;a pas été créée sur la table `%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="222"/>
        <source>Assignement of an invalid object name pattern to the relationship `%1&apos;!</source>
        <translation>Affectation d&apos;un modèle de nom d&apos;objet invalide à la relation `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="223"/>
        <source>Reference to an invalid object name pattern id on the relationship `%1&apos;!</source>
        <translation>Référence à un identifiant de modèle de nom d&apos;objet invalide sur la relation `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="225"/>
        <source>Mixing incompatibles DBMS export modes: `ignore object duplications&apos;, `drop database&apos; or `drop objects&apos; cannot be used with `simulate export&apos;!</source>
        <translation>Mélange incompatibles des modes d&apos;export du SGBD : `ignore object duplications&apos;, `drop database&apos; ou `drop objects&apos; ne peuvent être utilisés avec `simulate export&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="226"/>
        <source>Mixing incompatibles DROP options: `drop database&apos; and `drop objects&apos; cannot be used at the same time!</source>
        <translation>Mixage incompatible des options DROP : `drop database&apos; et `drop objects&apos; ne peuvent pas être utilisés en même temps !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="227"/>
        <source>Invalid object id swapping operation! The objects involved are the same!</source>
        <translation>Opération d&apos;échange d&apos;identifiant d&apos;objet invalide ! Les objets impliqués sont les mêmes !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="228"/>
        <source>Invalid object id swapping operation! The database itself, tablespaces or roles cannot have the ids swapped!</source>
        <translation>Opération d&apos;échange d&apos;identifiant d&apos;objet invalide ! La base de données elle-même, les espaces de stockage ou les rôles ne peuvent pas échanger leurs identifiants !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="229"/>
        <source>The widget already has a parent and cannot be assigned to a different object!</source>
        <translation>Le widget a déjà un parent et ne peut pas être assigné à un objet différent !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="231"/>
        <source>Could not load the database model file `%1&apos;. Check the error stack to see details. Try to run `pgmodeler-cli --fix-model&apos; in order to correct the structure of the file if that is the case.</source>
        <translation>Impossible de charger le fichier de modèle de base de données `%1&apos;. Vérifiez la pile d&apos;erreurs pour plus de détails. Essayez d&apos;exécuter `pgmodeler-cli --fix-model&apos; afin de corriger la structure du fichier si besoin.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="232"/>
        <source>The column `%1&apos; cannot reference it&apos;s parent table `%2&apos; as data type!</source>
        <translation>La colonne `%1&apos; ne peut pas référencer sa table parent `%2&apos; comme type de données !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="233"/>
        <source>Operation with an invalid element id `%1&apos;!</source>
        <translation>Opération avec un identifiant d&apos;élément invalide `%1&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="234"/>
        <source>Reference to an invalid color id `%1&apos; for element `%2&apos;!</source>
        <translation>Référence à un identifiant de couleur invalide `%1&apos; pour l&apos;élément `%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="236"/>
        <source>The sequence `%1&apos; can&apos;t be assigned to the column `%2&apos; because the data type of the latter is incompatible. The type used must be an integer one!</source>
        <translation>La séquence `%1&apos; ne peut pas être affectée à la colonne `%2&apos; car le type de données de cette dernière est incompatible. Le type utilisé doit être un entier !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="237"/>
        <source>The option to generate temporary object names can only be used in simulation mode!</source>
        <translation>L&apos;option de génération de noms d&apos;objets temporaires ne peut être utilisée qu&apos;en mode simulation !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="239"/>
        <source>Could not assign the variable `%1&apos; to event trigger&apos;s filter. Currently, PostgreSQL supports only the `TAG&apos; variable!</source>
        <translation>Impossible d&apos;affecter la variable `%1 &apos;au filtre du déclencheur d&apos;événement. Actuellement, PostgreSQL supporte uniquement la variable `TAG&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="241"/>
        <source>Malformed unescaped value on row `%1&apos; column `%2&apos;!</source>
        <translation>Valeur non echappée malformée à la ligne `%1&apos;, colonne `%2&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="243"/>
        <source>The object `%1&apos; (%2) can&apos;t be handled because some needed fields are not set! Please, make sure to fill at least the requires fields in order to properly create or update the object.</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être manipulé car certains champs nécessaires ne sont pas définis ! Merci de vous assurer de remplir au moins les champs requis afin de créer ou de mettre à jour correctement l&apos;objet.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="244"/>
        <source>A relationship can only be swapped by other object of the same kind!</source>
        <translation>Une association ne peut être permutée que par un autre objet du même genre !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="89"/>
        <source>Assignment of a null type to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un type nul à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="121"/>
        <source>Unable to create the generalization relationship because the constraint `%1&apos; in table `%2&apos; can not be merged with the constraint `%3&apos; of table `%4&apos; due to their incompatible composition!</source>
        <translation>Impossible de créer l&apos;association de généralisation car la contrainte `%1&apos; de la table `%2&apos; ne peut pas être fusionnée avec la contrainte `%3&apos; de la table `%4&apos; en raison de leurs compositions incompatibles !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="128"/>
        <source>Unable to write the file `%1&apos; due to one or more errors in the definition generation process!</source>
        <translation>Impossible d&apos;écrire le fichier `%1&apos; en raison d&apos;une ou de plusieurs erreurs dans le processus de génération de définition !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="130"/>
        <source>The configuration of the relationship `%1&apos; generates a redundancy between the relationships `%2&apos;. Redundancy on identifier or generalization/copy relationships are not accepted since they result in incorrect column spreading making the model inconsistent!</source>
        <translation>La configuration de la relation `%1&apos; génère une redondance entre les associations `%2 . La redondance sur l&apos;identificateur ou les relations de généralisation/copie de l&apos;association ne sont pas acceptées car elles entraînent une répartition incorrecte des colonnes, ce qui rend le modèle incohérent !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="169"/>
        <source>Invalid syntax in file `%1&apos;, line %2, column %3!</source>
        <translation>Syntaxe non valide dans le fichier `%1&apos;, ligne %2, colonne %3!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="170"/>
        <source>Invalid instruction `%1&apos; on file `%2&apos;, line %3, column %4!</source>
        <translation>Instruction invalide `%1&apos; dans le fichier `%2&apos;, ligne `%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="171"/>
        <source>Unknown attribute `%1&apos; in file `%2&apos;, line %3, column %4!</source>
        <translation>Attribut inconnu `%1&apos; dans le fichier `%2&apos;, ligne `%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="172"/>
        <source>Invalid metacharacter `%1&apos; in file `%2&apos;, line %3, column %4!</source>
        <translation>Métacaractère invalide `%1&apos; dans le fichier `%2&apos;, ligne `%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="173"/>
        <source>Invalid operator `%1&apos; in comparison expression, file `%2&apos;, line %3, column %4!</source>
        <translation>Opérateur invalide `%1&apos; dans l&apos;expression de comparaison, fichier `%2&apos;, ligne`%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="174"/>
        <source>Attribute `%1&apos; with an undefined value in file `%2&apos;, line %3, column %4!</source>
        <translation>L&apos;attribut `%1&apos; a une valeur non définie dans le fichier `%2&apos;, ligne`%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="175"/>
        <source>Attribute `%1&apos; with an invalid name in file `%2&apos;, line %3, column %4!</source>
        <translation>L&apos;attribut `%1&apos; a un nom invalide dans le fichier `%2&apos;, ligne`%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="177"/>
        <source>Could not access the file or directory `%1&apos;! Make sure that it exists or if the user has access permissions on it!</source>
        <translation>Impossible d&apos;acceder au fichier ou fossier `%1&apos; ! Vérifiez qu&apos;il existe et que l&apos;utilisateur à les droits d&apos;écritures !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="182"/>
        <source>Could not load file `%1&apos;. The same appears to be inconsistent or one of its dependencies (DTD files) has errors or is missing!</source>
        <translation>Impossible de charger le fichier `%1&apos;. La même chose semble être incohérente ou l&apos;une de ses dépendances (fichiers DTD) a des erreurs ou est manquante !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="195"/>
        <source>Reference to a tuple with an invalid index or the result is empty (no tuples)!</source>
        <translation>Référence à un tuple avec un index invalide ou le résultat est vide (pas de tuple) !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="198"/>
        <source>Invalid use of a view reference as whole SQL definition! The assigned reference must be an expression!</source>
        <translation>Utilisation invalide d&apos;une référence de vue comme définition SQL entière ! La référence affectée doit être une expression !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="202"/>
        <source>At the moment pgModeler does not support the creation of primary keys which some columns were generated by relationship connection. To create primary keys with this feature you can use the field `Identifier&apos; or the tab `Primary key&apos; on relationship editing form!</source>
        <translation>Actuellement pgModeler ne supporte pas la création de clés primaires dont certaines colonnes ont été générées par la connection d&apos;associations. Pour créer des clés primaires avec cette fonctionnalité, vous pouvez utiliser le champ `Identifier&apos; ou l&apos;onglet `Primary key&apos; sur le formulaire d&apos;édition de relation !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="216"/>
        <source>Reference to a function id which is incompatible with the user define type configuration!</source>
        <translation>Référence à un identifiant de fonction incompatible avec la configuration du type de définition de l&apos;utilisateur !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="218"/>
        <source>Unsupported PostgreSQL version (%1) detected! Valid versions are between %2 and %3.</source>
        <translation>Version PostgreSQL non prise en charge (%1) détectée!  Les versions valides sont comprises entre %2 et %3.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="224"/>
        <source>Invalid use of variadic parameter mode! This mode can be used only with an array or &quot;any&quot; data type!</source>
        <translation>Utilisation invalide du mode paramètre variadique ! Ce mode ne peut être utilisé qu&apos;avec un tableau ou un type de données &quot;any&quot; !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="230"/>
        <source>The object `%1&apos; (%2), oid `%3&apos;, could not be imported due to one or more errors! Check the exception stack for more details. `HINT:&apos; if the object somehow references objects in `pg_catalog&apos; or `information_schema&apos; consider enable the importing of system objects.</source>
        <translation>L&apos;objet `%1 &apos;(%2), oid `%3&apos;, n&apos;a pu être importé en raison d&apos;au moins une erreur ! Vérifiez la pile d&apos;exceptions pour plus de détails. `REMARQUE : &apos;Si l&apos;objet référence d&apos;une manière ou d&apos;une autre les objets dans` pg_catalog&apos; ou `information_schema &apos;, envisagez d&apos;importer les objets système.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="235"/>
        <source>Assignment of an invalid object to `%1&apos; (%2)! The assigned object must be of type `%3&apos;.</source>
        <translation>Affectation d&apos;un objet invalide à `%1&apos; (%2) ! L&apos;objet affecté doit être de type `%3&apos;.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="238"/>
        <source>It&apos;s not possible convert the type of the column `%1&apos; to serial! It must have an `integer&apos; based type and its default value must be a call to `nextval(seq_name::regclass)&apos; function or a sequence object must be directly assigned to the column!</source>
        <translation>Il n&apos;est pas possible de convertir le type de la colonne `%1&apos; en série ! Il doit avoir un type basé sur `integer&apos; et sa valeur par défaut doit être un appel à la fonction `nextval (seq_name::regclass)&apos; ou un objet séquence doit être directement assigné à la colonne !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="240"/>
        <source>Could not perform the `%1&apos; operation on `%2&apos; using the data on row `%3&apos;! All changes were rolled back. 

 ** Returned error ** 

%4</source>
        <translation>Impossible d&apos;exécuter l&apos;opération `%1 &apos;sur `%2&apos; en utilisant les données de la ligne `%3 &apos;! Tous les changements ont été annulés.

  ** Erreur retournée **

%4</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="242"/>
        <source>Trying to undo/redo an invalid operation over an object that does not exists anymore or can&apos;t be handled! The operation history will be cleaned up.</source>
        <translation>Tentative d&apos;annulation/de rétablissement d&apos;une opération invalide sur un objet qui n&apos;existe plus ou ne peut plus être géré ! L&apos;historique des opérations sera nettoyé.</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="245"/>
        <source>A parent table of `%1&apos; which OID is `%2&apos; was not found in the set of imported objects!</source>
        <translation>Une table parente de `%1&apos; dont l&apos;OID est `%2&apos; n&apos;a pas été trouvée dans l&apos;ensemble des objets importés !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="246"/>
        <source>The enumeration `%1&apos; can&apos;t be assigned to the type `%2&apos; because contains invalid characters!</source>
        <translation>L&apos;énumération `%1&apos; ne peut pas être affectée au type `%2&apos; car elle contient des caractères non valides !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="247"/>
        <source>The enumeration `%1&apos; can&apos;t be assigned to the type `%2&apos; because is too long!</source>
        <translation>L&apos;énumération `%1&apos; ne peut pas être affectée au type `%2&apos; car elle est trop longue !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="248"/>
        <source>The connection was idle for too long and was automatically closed!</source>
        <translation>La connexion était inactive depuis trop longtemps et a été automatiquement fermée !</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="249"/>
        <source>The connection was unexpectedly closed by the database server `%1&apos; at port `%2&apos;!</source>
        <translation>La connexion a été fermée de manière inattendue par le serveur de la base de données `%1&apos; sur le port `%2&apos; !</translation>
    </message>
</context>
<context>
    <name>ExtensionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="38"/>
        <source>Version:</source>
        <translation>Version :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="67"/>
        <source>Old Version:</source>
        <translation>Ancienne version :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="96"/>
        <source>This attribute cannot be changed once the object is created.</source>
        <translation>Cet attribut ne peut être modifié une fois l&apos;objet créé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="99"/>
        <source>Handles data type</source>
        <translation>Gérer les types de données</translation>
    </message>
</context>
<context>
    <name>FindReplaceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="188"/>
        <source>Hide this widget</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="191"/>
        <source>...</source>
        <translation type="unfinished">...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="249"/>
        <source>Replace one occurrence</source>
        <translation>Remplace une occurrence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="252"/>
        <source>Replace</source>
        <translation>Remplacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="271"/>
        <source>Replace all occurrences</source>
        <translation>Remplace toutes les occurrences</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="274"/>
        <source>Replace All</source>
        <translation>Remplacer tout</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="293"/>
        <source>Replace the selection and find the next one</source>
        <translation>Remplacer l&apos;occurrence courante et trouver le suivant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="296"/>
        <source>Replace &amp;&amp; Find</source>
        <translation>Remplacer &amp;&amp; Suivant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="213"/>
        <source>Replace:</source>
        <translation>Remplacer par :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="44"/>
        <source>Find:</source>
        <translation>Recherche :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="64"/>
        <source>Find previous</source>
        <translation>Précédent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="80"/>
        <source>Shift+F3</source>
        <translation>Maj+F3</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="98"/>
        <source>Find next</source>
        <translation>Suivant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="114"/>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="130"/>
        <source>Case sensitive</source>
        <translation>Respecter la casse</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="143"/>
        <source>Regular expression</source>
        <translation>Expression régulière</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="156"/>
        <source>Whole words</source>
        <translation>Mots entier</translation>
    </message>
</context>
<context>
    <name>FunctionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="60"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="129"/>
        <source>Function Type:</source>
        <translation>Type de fonction :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="384"/>
        <source>Execution Cost:</source>
        <translation>Coût d&apos;exécution :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="187"/>
        <source>Rows Returned:</source>
        <translation>Lignes retournées :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="281"/>
        <source>Behavior:</source>
        <translation>Comportement :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="97"/>
        <source>Security:</source>
        <translation>Sécurité :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="262"/>
        <source>Return Method:</source>
        <translation>Type de retour :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="325"/>
        <source>Set</source>
        <translation>Multiple</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="359"/>
        <source>Return Table</source>
        <translation>Table de retour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="400"/>
        <source>Windown Func.</source>
        <translation>Fonct. fenêtrage.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="439"/>
        <source>Leakproof</source>
        <translation>Leakproof</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="447"/>
        <source>Parameters</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="452"/>
        <source>Definition</source>
        <translation>Définition</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="503"/>
        <source>Dynamic Library:</source>
        <translation>Bibliothèque dynamique :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="532"/>
        <source>Symbol:</source>
        <translation>Symbole :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="561"/>
        <source>Library:</source>
        <translation>Bibliothèque :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="627"/>
        <source>Source code:</source>
        <translation>Code source :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="53"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="55"/>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="63"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="61"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="65"/>
        <source>Mode</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="66"/>
        <source>Default Value</source>
        <translation>Valeur par défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="306"/>
        <source>Si&amp;mple</source>
        <translation>Si&amp;mple</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="341"/>
        <source>Tab&amp;le</source>
        <translation>Tab&amp;le</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="215"/>
        <source>Language:</source>
        <translation>Langage :</translation>
    </message>
</context>
<context>
    <name>GeneralConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2076"/>
        <source>Milimeters</source>
        <translation>Millimètres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2081"/>
        <source>Pixels</source>
        <translation>Pixels</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2086"/>
        <source>Inches</source>
        <translation>Pouces</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2091"/>
        <source>Centimeter</source>
        <translation>Centimètres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1651"/>
        <source>A0 (841 x 1189 mm)</source>
        <translation>A0 (841 x 1189 mm)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="48"/>
        <source>General &amp;&amp; Design</source>
        <translation>Général &amp;&amp; modélisation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1250"/>
        <source>Check if there is a new version on server</source>
        <translation>Vérifier la présence d&apos;une nouvelle version au lancement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="66"/>
        <source>Design</source>
        <translation>Modélisation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="766"/>
        <source>Graphical objects (table, views and textboxes) will be created in a single step without the need to click on canvas </source>
        <translation>Les objets graphiques (tables, vues, zones de textes) seront créés en une seule étape sans avoir besoin de cliquer sur le canevas.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="769"/>
        <source>Simplify creation of graphical objects</source>
        <translation>Créer les nouveaux objets en un clic</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="327"/>
        <source>After loading the model the last zoom and position on canvas will be restored</source>
        <translation>Après le chargement du modèle, le dernier zoom et la position du canevas seront restaurés.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="330"/>
        <source>Save and restore last position and zoom</source>
        <translation>Sauvegarder la position et le zoom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="724"/>
        <source>Disable render smoothness</source>
        <translation>Désactiver l&apos;anticrénelage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="904"/>
        <source>Hide the object that represents the relationship name</source>
        <translation>Masque l&apos;étiquette indiquant le nom de chaque association.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="590"/>
        <source>Validate before save, export or diff</source>
        <translation>Validation du modèle toujours requise</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="465"/>
        <source>Hide the object which represents the tag assigned to the table</source>
        <translation>Masquer l&apos;objet qui représente la balise affectée à la table.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="420"/>
        <source>Hide the portion of table which represent triggers, indexes and rules</source>
        <translation>Masque les règles, les déclencheurs et les index dans l&apos;affichage des tables.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="817"/>
        <source>Enable SQL code completion</source>
        <translation>Activer la complétion du code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1343"/>
        <source>Printing &amp;&amp; Code</source>
        <translation>Impression &amp;&amp; édition</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1385"/>
        <source>Size:</source>
        <translation>Taille :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1498"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1398"/>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="907"/>
        <source>Hide relationship name</source>
        <translation>Masquer le nom des associations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="423"/>
        <source>Hide table extended attributes</source>
        <translation>Masquer les attributs étendus des tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1628"/>
        <source>Paper:</source>
        <translation>Papier :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1815"/>
        <source>Orientation:</source>
        <translation>Orientation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="999"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="513"/>
        <source>Operation history:</source>
        <translation>Historique des opérations :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="468"/>
        <source>Hide table tags</source>
        <translation>Masquer les balises des tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1565"/>
        <source>Printing</source>
        <translation>Impression</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1424"/>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1583"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1596"/>
        <source>Print grid</source>
        <translation>Imprimer la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1656"/>
        <source>A1 (594 x 841 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1661"/>
        <source>A2 (420 x 594 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1666"/>
        <source>A3 (297 x 420 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1671"/>
        <source>A4 (210 x 297 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1676"/>
        <source>A5 (148 x 210 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1681"/>
        <source>A6 (105 x 148 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1686"/>
        <source>A7 (74 x 105 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1691"/>
        <source>A8 (52 x 74 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1696"/>
        <source>A9 (37 x 52 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1701"/>
        <source>B0 (1030 x 1456 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1706"/>
        <source>B1 (728 x 1030 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1711"/>
        <source>B10 (32 x 45 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1716"/>
        <source>B2 (515 x 728 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1721"/>
        <source>B3 (364 x 515 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1726"/>
        <source>B4 (257 x 364 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1731"/>
        <source>B5 (182 x 257 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1736"/>
        <source>B6 (128 x 182 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1741"/>
        <source>B7 (91 x 128 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1746"/>
        <source>B8 (64 x 91 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1751"/>
        <source>B9 (45 x 64 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1756"/>
        <source>C5E (163 x 229 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1761"/>
        <source>Comm10E (105 x 241 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1766"/>
        <source>DLE (110 x 220 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1771"/>
        <source>Executive (191 x 254 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1776"/>
        <source>Folio (210 x 330 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1781"/>
        <source>Ledger (432 x 279 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1786"/>
        <source>Legal (216 x 356 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1791"/>
        <source>Letter (216 x 279 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1796"/>
        <source>Tabloid (279 x 432 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1801"/>
        <source>Custom</source>
        <translation>Personnalisé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1851"/>
        <source>Unity:</source>
        <translation>Unité :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1858"/>
        <source>Custom Size:</source>
        <translation>Taille papier :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1873"/>
        <source>Width:</source>
        <translation>Largeur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1902"/>
        <source>Height:</source>
        <translation>Hauteur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1927"/>
        <source>Page Margins:</source>
        <translation>Marges :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1942"/>
        <source>Left:</source>
        <translation>Gauche :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1961"/>
        <source>Left margin</source>
        <translation>Marge de gauche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1971"/>
        <source>Top:</source>
        <translation>Haut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1990"/>
        <source>Top margin</source>
        <translation>Marge du haut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2006"/>
        <source>Right:</source>
        <translation>Droite :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2047"/>
        <source>Right margin</source>
        <translation>Marge de droite</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2057"/>
        <source>Bottom:</source>
        <translation>Bas :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="2025"/>
        <source>Bottom margin</source>
        <translation>Marge du bas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1828"/>
        <source>Landscape</source>
        <translation>Paysage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1844"/>
        <source>Portrait</source>
        <translation>Portrait</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1612"/>
        <source>Print page numbers</source>
        <translation>Imprimer les numéros de page</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="279"/>
        <source>Start move the canvas when the cursor is on the canvas edges</source>
        <translation>Commencer à déplacer le canevas lorsque le curseur se trouve sur les bords de celui-ci.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="282"/>
        <source>Move canvas by keep mouse on corners</source>
        <translation>Déplacer le canevas en gardant la souris sur les coins</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="721"/>
        <source>Disable antialiasing for lines and texts improving performance when handling huge models.</source>
        <translation>Désactiver l&apos;antialiasing pour les lignes et les textes améliore les performances lors de la manipulation d&apos;énormes modèles.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="587"/>
        <source>Triggers a dialog asking the user to validate the model before a save, export or diff operation.</source>
        <translation>Déclenche une boîte de dialogue demandant à l&apos;utilisateur de valider le modèle avant une opération de sauvegarde, d&apos;export ou de comparaison.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="859"/>
        <source>When enabled this option creates a placeholder object at the previous table&apos;s position when starting to move it. This will cause graphical updates on relationship lines to be performed only when the drag &amp; drop action is done improving the performance. Disabling placeholders will cause those updates to be executed every time the table&apos;s position changes a single pixel (classical behavior).</source>
        <translation>Lorsqu&apos;elle est activée, cette option crée un objet réservé à la position de la table précédente lorsque vous commencez à la déplacer. Afin d&apos;améliorer les performances, la mise à jour graphique sur les lignes associées ne se fera uniquement qu&apos;une fois l&apos;action de glisser-déposer términée. La désactivation des espaces réservés entraîne l&apos;exécution de ces mises à jour chaque fois que la position de la table change d&apos;un seul pixel (comportement classique).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="862"/>
        <source>Use placeholders when moving tables</source>
        <translation>Utiliser des espaces réservés lors du déplacement de tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="814"/>
        <source>Toggles the code completion in all fields that accepts the input of SQL commands.</source>
        <translation>(Dés)active l&apos;achèvement du code dans tous les champs qui acceptent les commandes SQL en entrée.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1361"/>
        <source>Code style</source>
        <translation>Style du code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1411"/>
        <source>Colors:</source>
        <translation>Couleurs :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1433"/>
        <source>Display line numbers</source>
        <translation>Afficher les numéros de ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1449"/>
        <source>Highlight lines at cursor&apos;s position</source>
        <translation>Surligner les lignes à la position du curseur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1521"/>
        <source>Custom tab width:</source>
        <translation>Largeur d&apos;onglet personnalisé :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="44"/>
        <source>Line numbers&apos; font color</source>
        <translation>Couleur de la police des numéros de ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="47"/>
        <source>Line numbers&apos; background color</source>
        <translation>Couleur d&apos;arrière-plan des numéros de ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="50"/>
        <source>Highlighted line color</source>
        <translation>Couleur de la ligne en surbrillance</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="54"/>
        <source>The little brown fox jumps over the lazy dog</source>
        <translation>Le petit renard brun saute par-dessus le chien paresseux</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="241"/>
        <source>System default</source>
        <translation>Défaillance du système</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="636"/>
        <source>All files (*.*)</source>
        <translation>Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="638"/>
        <source>Load file</source>
        <translation>Charger un fichier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="101"/>
        <source>Minimum object opacity (%): </source>
        <translation>Opacité min. des objets (%) : </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="120"/>
        <source>Defines the minimum opacity percentage applied to the objects when using the fade out feature. A zero opacity causes the object to be completely hidden not being possible to interact with it in the canvas area.</source>
        <translation>Définit le pourcentage d&apos;opacité minimum appliqué aux objets lorsqu&apos;ils sont estompés. Une opacité nulle cache complètement l&apos;objet sans qu&apos;il soit possible d&apos;interagir avec lui dans la zone de canevas.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="193"/>
        <source>Canvas grid size:</source>
        <translation>Taille de grille du canevas :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="212"/>
        <source>Defines the vertical and horizontal grid size. This value affects the spacing of objects when using object grid alignment feature.</source>
        <translation>Définit la taille de la grille verticale et horizontale. Cette valeur affecte l&apos;espacement des objets lors de l&apos;utilisation de la fonction d&apos;alignement des objets sur la grille.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="372"/>
        <source>By default the range selection is triggered with Shift + left click. By checking this option range selection will be activated only with a single click and move.</source>
        <translation>Par défaut, la sélection de plage est déclenchée avec Maj + clic gauche. En cochant cette option, la sélection de la gamme sera activée qu&apos;en un seul clic et se déplacera.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="375"/>
        <source>Trigger range selection with a single click</source>
        <translation>Déclenchement de la sélection en un clic</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="532"/>
        <source>Defines the maximum amount of elements held in the operation history. Once reached the maximum number the history is automatically cleaned.</source>
        <translation>Définit le nombre maximal d&apos;éléments contenus dans l&apos;historique des opérations. Une fois le nombre maximum atteint, l&apos;historique est automatiquement nettoyé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="641"/>
        <source>Defines the period when the opened models will be saved automatically.</source>
        <translation>Définit la période (en minute) d&apos;enregistrement automatique des modèles ouverts.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="644"/>
        <source>Autosave interval (minutes):</source>
        <translation>Intervalle sauv. auto :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="949"/>
        <source>Replaces any straight line in relationship by curved ones in order to improve the model&apos;s visualization.</source>
        <translation>Remplace les lignes droites des relations par des courbes pour améliorer la lisibilité du modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="952"/>
        <source>Use curved lines for relationships</source>
        <translation>Utiliser des lignes courbes pour les associations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1233"/>
        <source>SQL history max. length:</source>
        <translation>Longueur max. de l&apos;historique SQL :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1325"/>
        <source>Souce code editor:</source>
        <translation>Éditeur de code source :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1072"/>
        <source>lines</source>
        <translation>lignes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1116"/>
        <source>Clear the entire SQL comand history.</source>
        <translation>Nettoyer complètement l&apos;historique des commandes SQL.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1119"/>
        <source>Clear history</source>
        <translation>Nettoyer l&apos;historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1332"/>
        <source>Configurations directory:</source>
        <translation>Dossier de configuration :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1305"/>
        <source>Browse the source code editor application</source>
        <translation>Rechercher l&apos;éditeur de code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1194"/>
        <source>Open in file manager</source>
        <translation>Ouvrir dans le gestionnaire de fichier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1253"/>
        <source>Check updates at startup</source>
        <translation>Vérifier MAJ au démarrage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1017"/>
        <source>Souce code editor args:</source>
        <translation>Arg. pour l&apos;éditeur de code source :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1280"/>
        <source>User interface language:</source>
        <translation>Langue de l&apos;interface graphique :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="1214"/>
        <source>Overrides the default user interface language defined by the system. Requires restarting the program. &lt;strong&gt;NOTE:&lt;/strong&gt; UI translations are third party collaborations thus any typo or mistake should be reported directly to their respective maintainers.</source>
        <translation>Remplace la langue de l&apos;interface utilisateur définie par défaut par le système. Nécessite de redémarrer le programme. &lt;strong&gt;REMARQUE :&lt;/strong&gt; les traductions de l&apos;interface utilisateur sont des collaborations avec des tiers; par conséquent, toute faute de frappe ou erreur doit être signalée directement à leurs responsables respectifs.</translation>
    </message>
</context>
<context>
    <name>GenericSQLWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/genericsqlwidget.ui" line="32"/>
        <source>SQL code</source>
        <translation>Code SQL</translation>
    </message>
</context>
<context>
    <name>HintTextWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/hinttextwidget.ui" line="44"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
</context>
<context>
    <name>IndexWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="42"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="88"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="133"/>
        <source>Fill Factor:</source>
        <translation>Taux de remplissage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="192"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="222"/>
        <source>Concurrent</source>
        <translation>Concurrent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="247"/>
        <source>Unique</source>
        <translation>Unique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="272"/>
        <source>Fast update</source>
        <translation>Mise à jour rapide</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="357"/>
        <source>Elements</source>
        <translation>Éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="285"/>
        <source>Buffering</source>
        <translation>Mémoire tampon</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="312"/>
        <source>Predicate:</source>
        <translation>Prédicat :</translation>
    </message>
</context>
<context>
    <name>LanguageWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="63"/>
        <source>Trusted:</source>
        <translation>De confiance :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/languagewidget.cpp" line="42"/>
        <source>The functions to be assigned to the language should have, respectively, the following signatures:&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Handler Function:&lt;/strong&gt; &lt;em&gt;language_handler function()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Validator Function:&lt;/strong&gt; &lt;em&gt;void function(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Inline Function:&lt;/strong&gt; &lt;em&gt;void function(internal)&lt;/em&gt;</source>
        <translation>Les fonctions attribuées au langage doivent avoir respectivement les signatures suivantes :&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Fonction de gestion :&lt;/strong&gt; &lt;em&gt;language_handler function()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Fonction de validation :&lt;/strong&gt; &lt;em&gt;void function(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Fonction sur une ligne :&lt;/strong&gt; &lt;em&gt;void function(internal)&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="82"/>
        <source>Validator Func.:</source>
        <translation>Fonc. de validation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="101"/>
        <source>Handler Func.:</source>
        <translation>Fonc. gestionnaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="120"/>
        <source>Inline Func.:</source>
        <translation>Fonc. en ligne :</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="29"/>
        <source>pgModeler - PostgreSQL Database Modeler</source>
        <translation>pgModeler – Modeleur de bases de données PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="485"/>
        <source>&amp;Validation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="498"/>
        <source>Alt+V</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="533"/>
        <source>Ctrl+F</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="581"/>
        <source>Alt+O</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="632"/>
        <source>Alt+B</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="692"/>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="717"/>
        <source>&amp;Edit</source>
        <translation>Édit&amp;ion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="737"/>
        <source>&amp;Show</source>
        <translation>&amp;Affichage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1270"/>
        <source>Plugins</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1258"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="832"/>
        <source>Controls</source>
        <translation>Contrôles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="771"/>
        <source>General</source>
        <translation>Général</translation>
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
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="953"/>
        <source>Ctrl+-</source>
        <translation>Ctrl+-</translation>
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
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1082"/>
        <source>Export the current opened model in different modes</source>
        <translation>Exporter le modèle courant sous différentes formes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1124"/>
        <source>Close current model</source>
        <translation>Fermer le modèle courant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1166"/>
        <source>Ctrl+H</source>
        <translation>Ctrl+H</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1202"/>
        <source>Edit pgModeler settings</source>
        <translation>Configurer pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1234"/>
        <source>F10</source>
        <translation>F10</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1273"/>
        <source>Access the list of loaded plugins</source>
        <translation>Accéder à la liste des extensions chargées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1285"/>
        <source>Load recently opened model</source>
        <translation>Charger des modèles ouverts récemment</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1300"/>
        <source>Import existing database to new model (reverse engineering)</source>
        <translation>Importer une base de données existante vers un modèle (reverse engineering)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1303"/>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Maj+I</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1333"/>
        <source>New version found!</source>
        <translation>Nouvelle version disponible !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1336"/>
        <source>Update for the current version is available on project&apos;s site</source>
        <translation>Une mise à jour pour votre version est disponible sur le site de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1357"/>
        <source>Main menu</source>
        <translation>Menu principal</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1362"/>
        <source>Show expanded</source>
        <translation>Afficher la barre de menus</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1373"/>
        <source>Hide main menu</source>
        <translation>Cacher la barre de menus</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1376"/>
        <source>Hides the main menu bar and put the action on a separated action</source>
        <translation>Masque la barre de menu (le menu reste accessible via un bouton dans la barre d&apos;outils)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1379"/>
        <source>Ctrl+Shift+H</source>
        <translation>Ctrl+Maj+H</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1412"/>
        <source>Welcome</source>
        <translation>Accueil</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1415"/>
        <source>Welcome screen</source>
        <translation>Écran d&apos;accueil</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1433"/>
        <source>Design</source>
        <translation>Modéliser</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1451"/>
        <source>Manage</source>
        <translation>Administrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1454"/>
        <source>Manage existent databases</source>
        <translation>Administrer des bases existantes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="619"/>
        <source>O&amp;bjects</source>
        <translation>O&amp;bjets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="568"/>
        <source>&amp;Operations</source>
        <translation>&amp;Opérations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="520"/>
        <source>Find Object</source>
        <translation>Chercher un objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1368"/>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Maj+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1106"/>
        <source>Show grid</source>
        <translation>Afficher la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1163"/>
        <source>Align objects position to grid</source>
        <translation>Aligner les objets sur la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1187"/>
        <source>Show the page delimiters</source>
        <translation>Afficher les délimiteurs de page</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1213"/>
        <source>Save all</source>
        <translation>Tout enregistrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1231"/>
        <source>Show the model overview</source>
        <translation>Afficher la vue d&apos;ensemble du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="772"/>
        <source>Clear Menu</source>
        <translation>Vider le menu</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="817"/>
        <source>The demonstration version can create only `one&apos; instance of database model!</source>
        <translation>La version de démonstration ne peut gérer qu&apos;une seule instance de modèle de base de données !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="905"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1174"/>
        <source>Save model</source>
        <translation>Enregistrer le modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1293"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1831"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1344"/>
        <source>Save &apos;%1&apos; as...</source>
        <translation>Enregistrer &apos;%1&apos; sous...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1345"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1530"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modèle de base de données (*.dbm);; Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1481"/>
        <source>Database model printing</source>
        <translation>Impression du modèle de base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1311"/>
        <source>Save anyway</source>
        <translation>Sauvegarder quand même</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1311"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1389"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1443"/>
        <source>Validate</source>
        <translation>Valider</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1509"/>
        <source>Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click &apos;No&apos; or &apos;Cancel&apos; to abort printing.</source>
        <translation>Des modifications ont été détectées dans les définitions du papier ou des marges du modèle ce qui pourrait causer une mauvaise impression des objets. Souhaitez-vous continuer l&apos;impression avec les nouveaux paramètres ? Pour utiliser les paramètres par défaut cliquez sur &apos;Non&apos; ou sur &apos;Annuler&apos; pour interrompre l&apos;impression.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1532"/>
        <source>Load model</source>
        <translation>Charger un modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1577"/>
        <source>Could not load the database model file `%1&apos;. Check the error stack to see details. You can try to fix it in order to make it loadable again.</source>
        <translation>Impossible de charger le ficher modèle de base de données `%1&apos;. Consultez la pile d&apos;appel pour plus de détails. Essayez de réparer le modèle pour tenter de l&apos;ouvrir à nouveau.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1579"/>
        <source>Fix model</source>
        <translation>Réparer un modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1579"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1670"/>
        <source>This action will open a web browser window! Want to proceed?</source>
        <translation>Cette action ouvrira votre navigateur web ! Souhaitez vous continuer ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="616"/>
        <source>Toggle the model objects widget</source>
        <translation>(Dés)activer le widget des objets du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="482"/>
        <source>Toogle the model validation widgets</source>
        <translation>(Dés)activer le widget de validation du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="565"/>
        <source>Toggle the operation history widget</source>
        <translation>(Dés)activer le widget de l&apos;historique des opérations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="517"/>
        <source>Toggle the object finder</source>
        <translation>(Dés)activer le chercheur d&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1354"/>
        <source>action_main_menu</source>
        <translation>action_menu_principal</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1365"/>
        <source>Expands the main menu bar in classical mode</source>
        <translation>Développe la barre de menus principale en mode classique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="440"/>
        <source>Saving temp. models</source>
        <translation>Sauvegarde temp. des modèles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="49"/>
        <source>Grid</source>
        <translation>Grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="50"/>
        <source>Hierarchical</source>
        <translation>Hiérarchique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="51"/>
        <source>Scattered</source>
        <translation>Dispersé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="233"/>
        <source> (Demo)</source>
        <translation> (Démo)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1294"/>
        <source>You&apos;re running a demonstration version! The model saving feature is available only in the full version!</source>
        <translation>Vous utilisez une version de démonstration ! La fonction d&apos;enregistrement du modèle est uniquement disponible dans la version complète !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1308"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1386"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1440"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1309"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! It&apos;s recommended to validate it before save in order to create a consistent model otherwise the generated file will be broken demanding manual fixes to be loadable again!</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; Le modèle &lt;strong&gt;%1&lt;/strong&gt; n&apos;a pas été validé ! Il est recommandé de le valider avant de l&apos;enregistrer afin de créer un modèle cohérent, sinon il peut en résulter un fichier cassé nécessitant une réparation manuelle pour être chargé à nouveau chargeable !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1387"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! Before run the export process it&apos;s recommended to validate in order to correctly create the objects on database server!</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; Le modèle &lt;strong&gt;%1&lt;/strong&gt; n&apos;a pas été validé ! Avant de procéder à un export il est recommandé de le valider afin de s&apos;assurer de correctement créer les objets sur le serveur de base de données !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1389"/>
        <source>Export anyway</source>
        <translation>Exporter quand même</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1441"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! Before run the diff process it&apos;s recommended to validate in order to correctly analyze and generate the difference between the model and a database!</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; Le modèle &lt;strong&gt;%1&lt;/strong&gt; n&apos;a pas été validé ! Avant de procéder à une comparaison il est recommandé de le valider afin de s&apos;assurer que l&apos;analyse entre celui-ci de la base de donnée se fasse correctement !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1443"/>
        <source>Diff anyway</source>
        <translation>Comparer quand même</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1764"/>
        <source>(no samples found)</source>
        <translation>(aucun exemple trouvé)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1844"/>
        <source>save</source>
        <translation>sauvegarde</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1845"/>
        <source>export</source>
        <translation>exporter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1845"/>
        <source>diff</source>
        <translation>comparer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1848"/>
        <source>Executing pending &lt;strong&gt;%1&lt;/strong&gt; operation...</source>
        <translation>Exécution de l&apos;opération &lt;strong&gt;%1&lt;/strong&gt; en attente...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1953"/>
        <source>Rearrange objects over the canvas is an irreversible operation! Would like to proceed?</source>
        <translation>Réorganiser les objets sur le canvas est une opération irréversible ! Voulez-vous poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="727"/>
        <source>He&amp;lp</source>
        <translation>Ai&amp;de</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="754"/>
        <source>Pl&amp;ugins</source>
        <translation>Mod&amp;ules</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="884"/>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="887"/>
        <source>New model</source>
        <translation>Nouveau modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="902"/>
        <source>&amp;Save</source>
        <translation>E&amp;nregistrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="920"/>
        <source>&amp;Zoom in</source>
        <translation>&amp;Zoomer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="944"/>
        <source>Zoo&amp;m out</source>
        <translation>Dézoo&amp;mer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="962"/>
        <source>&amp;Load</source>
        <translation>C&amp;harger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="977"/>
        <source>Sa&amp;ve as</source>
        <translation>Enre&amp;gistrer sous</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="986"/>
        <source>E&amp;xit</source>
        <translation>Qui&amp;tter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="989"/>
        <source>Exit pgModeler</source>
        <translation>Quitter pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1007"/>
        <source>&amp;About pgModeler</source>
        <translation>&amp;À propos de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1010"/>
        <source>F4</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1025"/>
        <source>&amp;Print</source>
        <translation>Im&amp;primer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1028"/>
        <source>Print model</source>
        <translation>Imprimer le modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1031"/>
        <source>Ctrl+P</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1043"/>
        <source>&amp;Undo</source>
        <translation>Ann&amp;uler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1046"/>
        <source>Undo operation</source>
        <translation>Annuler l&apos;opération</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1049"/>
        <source>Ctrl+Z</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1061"/>
        <source>&amp;Redo</source>
        <translation>&amp;Rétablir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1064"/>
        <source>Redo operation</source>
        <translation>Rétablir l&apos;opération</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1067"/>
        <source>Ctrl+Y</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1079"/>
        <source>&amp;Export</source>
        <translation>&amp;Exporter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1085"/>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Maj+E</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1103"/>
        <source>&amp;Show grid</source>
        <translation>A&amp;fficher la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1109"/>
        <source>Ctrl+G</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1121"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1127"/>
        <source>Ctrl+W</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1139"/>
        <source>&amp;Normal zoom</source>
        <translation>Zoom &amp;normal</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1142"/>
        <source>Ctrl+0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1160"/>
        <source>&amp;Align to grid</source>
        <translation>&amp;Aligner sur la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1184"/>
        <source>Show &amp;delimiters</source>
        <translation>Afficher les &amp;délimiteurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1190"/>
        <source>Ctrl+L</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1199"/>
        <source>&amp;Settings</source>
        <translation>Paramètre&amp;s</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1205"/>
        <source>F12</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1228"/>
        <source>&amp;Overview</source>
        <translation>&amp;Vue d&apos;ensemble</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1243"/>
        <source>&amp;Support</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1246"/>
        <source>Access the support page</source>
        <translation>Accéder à la page du support</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1249"/>
        <source>F1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1261"/>
        <source>New object</source>
        <translation>Nouvel objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1282"/>
        <source>&amp;Recent Models</source>
        <translation>Modèles &amp;récents</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1297"/>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1312"/>
        <source>Rest&amp;ore Session</source>
        <translation>Restaurer la sessi&amp;on</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1321"/>
        <source>&amp;Fix a model</source>
        <translation>Réparer u&amp;n modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1345"/>
        <source>&amp;Check for update</source>
        <translation>Re&amp;chercher une mise à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1391"/>
        <source>&amp;Diff</source>
        <translation>&amp;Comparer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1397"/>
        <source>Ctrl+Shift+D</source>
        <translation>Ctrl+Maj+D</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1418"/>
        <source>Shift+W</source>
        <translation>Maj+W</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1436"/>
        <source>Design database models</source>
        <translation>Conception de modèles de base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1439"/>
        <source>Shift+D</source>
        <translation>Maj+D</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1457"/>
        <source>Shift+M</source>
        <translation>Maj+M</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1466"/>
        <source>&amp;Bug report</source>
        <translation>Rapport de &amp;bug</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1469"/>
        <source>Report a bug</source>
        <translation>Rapporter un bug</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1481"/>
        <source>Donate</source>
        <translation>Faire un don</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1484"/>
        <source>Help pgModeler by donating!</source>
        <translation>Aider pgModeler en faisant un don !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1496"/>
        <source>Objects me&amp;tadata</source>
        <translation>Mé&amp;tadonnées des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1499"/>
        <source>Objects metadata</source>
        <translation>Métadonnées des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="599"/>
        <source>Save modified model(s)</source>
        <translation>Enregistrer le(s) modèle(s) modifié(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="600"/>
        <source>The following models were modified but not saved: %1. Do you really want to quit pgModeler?</source>
        <translation>Les modèles suivants ont été modifiés mais non enregistrés : %1. Voulez-vous vraiment quitter pgModeler ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1175"/>
        <source>The model &lt;strong&gt;%1&lt;/strong&gt; was modified! Do you really want to close without save it?</source>
        <translation>Le modèle &lt;strong&gt;%1&lt;/strong&gt; a été modifié ! Êtes-vous certain de vouloir quitter sans l&apos;enregistrer ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1669"/>
        <source>Access support page</source>
        <translation>Accéder à la page de support</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="1832"/>
        <source>You&apos;re running a demonstration version! Note that you&apos;ll be able to create only &lt;strong&gt;%1&lt;/strong&gt; instances 						of each type of object and some key features will be disabled or limited!&lt;br/&gt;&lt;br/&gt;You can purchase a full binary copy or get the source code at &lt;a href=&apos;http://pgmodeler.com.br&apos;&gt;pgmodeler.com.br&lt;/a&gt;.						&lt;strong&gt;NOTE:&lt;/strong&gt; pgModeler is an open source software, but purchasing binary copies or providing some donations will support the project and cover all development costs.&lt;br/&gt;&lt;br/&gt;						&lt;strong&gt;HINT:&lt;/strong&gt; in order to test all features it&apos;s recommended to use the &lt;strong&gt;demo.dbm&lt;/strong&gt; model located in &lt;/strong&gt;Sample models&lt;/strong&gt; at &lt;strong&gt;Welcome&lt;/strong&gt; view.&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;</source>
        <translation>Vous utilisez une version de démonstration ! Notez que vous ne pourrez créer que &lt;strong&gt;%1&lt;/strong&gt; instances 						de chaque type d&apos;objet et que certaines fonctionnalités clés seront désactivées ou limitées !&lt;br/&gt;&lt;br/&gt;Vous pouvez acheter une copie binaire complète ou obtenir le code source sur &lt;a href=&apos;http://pgmodeler.com.br&apos;&gt; pgmodeler.com.br &lt;/a&gt;.						&lt;strong&gt; REMARQUE :&lt;/strong&gt; pgModeler est un logiciel open source, mais l&apos;achat de copies binaires ou la fourniture de dons soutiendra le projet et couvrira tous les coûts de développement.&lt;br/&gt;&lt;br/&gt;						&lt;strong&gt;CONSEIL :&lt;/strong&gt; Pour tester toutes les fonctionnalités, il est recommandé d&apos;utiliser le modèle &lt;strong&gt;demo.dbm&lt;/strong&gt; situé dans &lt;strong&gt;exemples de modèles&lt;/strong&gt; dans la vue &lt;strong&gt;Bienvenue&lt;/strong&gt;.&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1394"/>
        <source>Determine the changes between model/database and another database</source>
        <translation>Déterminer les changements entre le modèle/la base de données et une autre base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1511"/>
        <source>Arrange objects</source>
        <translation>Organiser des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="1514"/>
        <source>Rearrange objects over the canvas</source>
        <translation>Réorganiser les objets sur le canevas</translation>
    </message>
</context>
<context>
    <name>Messagebox</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="29"/>
        <source>Dialog</source>
        <translation>Boîte de dialogue</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="176"/>
        <source>msg</source>
        <translation>message</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="309"/>
        <source>Exceptions</source>
        <translation>Exceptions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="421"/>
        <source>Show/hide exceptions stack.</source>
        <translation>Afficher/cacher la pile d&apos;exceptions.</translation>
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
        <translation>&amp;Oui</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="526"/>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="139"/>
        <source>&amp;No</source>
        <translation>&amp;Non</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="161"/>
        <source>Information</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="165"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="551"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="153"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="157"/>
        <source>Alert</source>
        <translation>Alerte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="135"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="142"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="392"/>
        <source>Show raw text errors or information.</source>
        <translation>Afficher le texte brute des erreurs ou des informations.</translation>
    </message>
</context>
<context>
    <name>MetadataHandlingForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="14"/>
        <source>Handle metadata</source>
        <translation>Gérer les métadonnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="97"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="135"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="215"/>
        <source>Handle objects metadata</source>
        <translation>Gérer les métadonnées d&apos;objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="234"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="252"/>
        <source>Extract from:</source>
        <translation>Extraction depuis :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="336"/>
        <source>Loading a metadata file to the current model is an irreversible operation so be sure to specify a backup file before proceed.</source>
        <translation>Le chargement d&apos;un fichier de métadonnées dans le modèle actuel est une opération irréversible, assurez-vous de spécifier un fichier de sauvegarde avant de continuer.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="589"/>
        <source>Handles the following database model attributes in the metadata file: author, zoom factor, last position and default objects.</source>
        <translation>Gère, dans le fichier de métadonnées, les attributs de modèle de base de données suivants : auteur, facteur de zoom, dernière position et objets par défaut.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="592"/>
        <source>Database model metadata</source>
        <translation>Métadonnées du modèle de base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="384"/>
        <source>Handles the objects&apos; positioning in the metadata file.</source>
        <translation>Gère le positionnement des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="355"/>
        <source>Options</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="387"/>
        <source>Objects&apos; positioning</source>
        <translation>Positionnement des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="445"/>
        <source>Handles the objects&apos; custom colors in the metadata file. Currently available only for relationships and schemas.</source>
        <translation>Gère les couleurs personnalisées des objets dans le fichier de métadonnées. Actuellement disponible uniquement pour les associations et les schémas.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="448"/>
        <source>Custom object&apos;s colors</source>
        <translation>Couleurs de l&apos;objet personnalisé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="493"/>
        <source>Handles the objects&apos; protection status in the metadata file.</source>
        <translation>Gère l&apos;état de protection des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="496"/>
        <source>Objects&apos; protection status</source>
        <translation>Statut de protection des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="541"/>
        <source>Handles the objects&apos; SQL disabled status in the metadata file.</source>
        <translation>Gère le statut désactivé du code SQL des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="544"/>
        <source>Objects&apos; SQL disabled status</source>
        <translation>Statut désactivé du code SQL des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="781"/>
        <source>Handles the objects&apos; custom SQL commands in the metadata file.</source>
        <translation>Gère les commandes SQL personnalisées des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="784"/>
        <source>Custom SQL commands</source>
        <translation>Commandes SQL personnalisées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="736"/>
        <source>Textbox objects</source>
        <translation>Objets de zone de texte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="688"/>
        <source>Tag objects</source>
        <translation>Balises des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="917"/>
        <source>Backup file:</source>
        <translation>Fichier de sauvegarde :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="932"/>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="264"/>
        <source>Select file</source>
        <translation>Sélectionner un fichier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1133"/>
        <source>Apply to:</source>
        <translation>Appliquer à :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="957"/>
        <source>Operation:</source>
        <translation>Opération :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1148"/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1237"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="115"/>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="126"/>
        <source>model not saved yet</source>
        <translation>modèle pas encore enregistré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="135"/>
        <source>The backup file cannot be the same as the input model!</source>
        <translation>Le fichier de sauvegarde ne peut être le même que le modèle d&apos;entrée !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="185"/>
        <source>Extracting metadata to file `%1&apos;</source>
        <translation>Extraction des métadonnées vers le fichier `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="194"/>
        <source>Saving backup metadata to file `%1&apos;</source>
        <translation>Enregistrement des métadonnées de sauvegarde vers le fichier `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="211"/>
        <source>Applying metadata from file `%1&apos;</source>
        <translation>Application des métadonnées depuis le fichier `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="244"/>
        <source>Metadata processing aborted!</source>
        <translation>Traitement des métadonnées abandonné !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="263"/>
        <source>Objects metadata file (*.omf);;All files (*.*)</source>
        <translation>Fichiers de métadonnées d&apos;objets (*.omf);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="637"/>
        <source>Handles the objects&apos; fade out status in the metadata file.</source>
        <translation>Gère l&apos;estompement des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="640"/>
        <source>Objects&apos; fade out status</source>
        <translation>Effet d&apos;estompement des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="685"/>
        <source>Save tags to the output file when extracting metadata. When loading the file, the tags are recreated and duplicated ones are ignored.</source>
        <translation>Enregistrer les étiquettes dans le fichier de sortie lors de l&apos;extraction des métadonnées. Lors du chargement du fichier, les balises sont recréées et les doublons ignorés.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="733"/>
        <source>Save textboxes to the output file when extracting metadata. When loading the file, the textboxes are recreated and duplicated ones are ignored.</source>
        <translation>Enregistrer les zones de texte dans le fichier de sortie lors de l&apos;extraction des métadonnées. Lors du chargement du fichier, les zones de texte sont recréées et les zones dupliquées ignorées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="829"/>
        <source>Handles the tables&apos; and views&apos; extended attributes display status in the metadata file.</source>
        <translation>Gère l&apos;état d&apos;affichage des attributs étendus des tables et des vues dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="832"/>
        <source>Tables&apos; extended attributes display</source>
        <translation>Affichage étendu des attributs des tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="877"/>
        <source>Save generic SQL objects to the output file when extracting metadata. When loading the file, the objects are recreated and duplicated ones are ignored.</source>
        <translation>Enregistrer les objets SQL génériques dans le fichier de sortie lors de l&apos;extraction des métadonnées. Lors du chargement du fichier, les objets sont recréés et les doublons ignorés.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="880"/>
        <source>Generic SQL objects</source>
        <translation>Objets SQL génériques</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="935"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="972"/>
        <source>Extracts the objects&apos; metadata from the loaded models and apply to the current focused model. A backup file can be specified to where the focused model&apos;s current metadata will be saved.</source>
        <translation>Extrait les métadonnées des objets des modèles chargés et s&apos;applique au modèle ciblé actuel. L&apos;emplacement du fichier de sauvegarde dans lequel les métadonnées actuelles du modèle ciblé seront enregistrées peut-être spécifié.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="975"/>
        <source>&amp;Extract and restore</source>
        <translation>&amp;Extraction et restauration</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1029"/>
        <source>Extracts the objects metadata from one of the loaded models saving the info to a backup file.</source>
        <translation>Extrait les métadonnées d&apos;objets de l&apos;un des modèles chargés et enregistre les informations dans un fichier.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1032"/>
        <source>Extract &amp;only</source>
        <translation>Extracti&amp;on seule</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1086"/>
        <source>Reads the objects&apos; metadata from a previously saved backup file and apply to the current model.</source>
        <translation>Lit les métadonnées des objets à partir d&apos;un fichier de sauvegarde précédemment enregistré et s&apos;applique au modèle actuel.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="1089"/>
        <source>&amp;Restore a backup file</source>
        <translation>&amp;Restaurer une sauvegarde</translation>
    </message>
</context>
<context>
    <name>ModelDatabaseDiffForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="152"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="302"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="405"/>
        <source>Connection:</source>
        <translation>Connexion :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1441"/>
        <source>Ignore import errors</source>
        <translation>Ignorer les erreurs d&apos;import</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1351"/>
        <source>Import system objects</source>
        <translation>Importer les objets système</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1396"/>
        <source>Import extension objects</source>
        <translation>Importer les objets des extensions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="661"/>
        <source>File:</source>
        <translation>Fichier :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="677"/>
        <source>Select output file</source>
        <translation>Sélectionner un fichier de sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1776"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1844"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2338"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="333"/>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="335"/>
        <source>Waiting process to start...</source>
        <translation>En attente du démarrage de la tâche...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="881"/>
        <source>SQL code (*.sql);;All files (*.*)</source>
        <translation>Code SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="271"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="337"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="440"/>
        <source>Database:</source>
        <translation>Base de données :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1055"/>
        <source>For DROP command, the objects that depends on an object to be dropped will be deleted as well. For TRUNCATE command, tables that are linked to a table to be truncated will be truncate too. &lt;strong&gt;NOTE:&lt;/strong&gt; this option can affect more objects than listed in the output or diff preview.</source>
        <translation>Pour la commande DROP, les objets qui dépendent d&apos;un objet à supprimer seront également supprimés. Pour la commande TRUNCATE, les tables liées à une table à tronquer seront également tronquées. &lt;strong&gt;REMARQUE :&lt;/strong&gt; cette option peut affecter plus d&apos;objets que ceux listés dans l&apos;aperçu de sortie ou de comparaison.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1058"/>
        <source>Drop or truncate in cascade mode</source>
        <translation>Supprimer ou tronquer en mode cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="869"/>
        <source>Permissions already set on database objects will be kept.The ones configured on the model will be applied to the database.</source>
        <translation>Les autorisations déjà définies sur les objets de la base de données seront conservées. Les données configurées sur le modèle seront appliquées à la base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="872"/>
        <source>Keep object&apos;s permissions</source>
        <translation>Conserver les permissions des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="827"/>
        <source>Database cluster level objects like roles and tablespaces will not be dropped.</source>
        <translation>Les objets de niveau de cluster de base de données tels que les rôles et les espaces de stockage ne seront pas supprimés.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="830"/>
        <source>Keep cluster objects</source>
        <translation>Conserver les objets de cluster</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1193"/>
        <source>Recreate only unmodifiable objects</source>
        <translation>Recréer uniquement des objets non modifiables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1250"/>
        <source>Force recreation of objects</source>
        <translation>Forcer la recréation d&apos;objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1504"/>
        <source>Ignores errors generated by duplicated objects when exporting the diff to database.</source>
        <translation>Ignore les erreurs générées par les objets dupliqués lors de l&apos;export de la comparaison vers la base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1507"/>
        <source>Ignore duplicity errors</source>
        <translation>Ignorer les erreurs de duplicité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1142"/>
        <source>Serial columns are converted to integer and having the default value changed  to &lt;strong&gt;nextval(sequence)&lt;/strong&gt; function call. By default, a new sequence is created for each serial column but checking this option sequences matching the name on column&apos;s default value will be reused and will not be dropped.</source>
        <translation>Les colonnes de séries sont converties en nombre entier et la valeur par défaut est changée en appel de fonction &lt;strong&gt;nextval(sequence)&lt;/strong&gt;. Par défaut, une nouvelle séquence est créée pour chaque colonne série, mais en cochant cette option, les séquences correspondantes au nom de la valeur par défaut de la colonne seront réutilisées et ne seront pas supprimées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1145"/>
        <source>Reuse sequences on serial columns</source>
        <translation>Réutiliser des séquences sur des colonnes de séries</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="481"/>
        <source>Diff mode</source>
        <translation>Mode comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="501"/>
        <source>Override the PostgreSQL version when generating the diff. The default is to use the same version as the input database.</source>
        <translation>Outrepasser la version de PostgreSQL lors de la génération de la comparaison. Par défaut, la valeur est celle de la version de la base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="504"/>
        <source>Use PostgreSQL:</source>
        <translation>Utiliser PostgreSQL :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="572"/>
        <source>Compares the model and the input database storing the diff in a SQL file for later usage.</source>
        <translation>Compare le modèle avec la base de données d&apos;entrée et stock le résultat dans un fichier SQL pour une utilisation ultérieure.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="680"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="730"/>
        <source>Compares the model and the input database generating a diff and applying it directly to the latter. &lt;strong&gt;WARNING:&lt;/strong&gt; this mode causes irreversible changes on the database and in case of failure the original structure is not restored, so make sure to have a backup before proceed.</source>
        <translation>Compare le modèle et la base de données d&apos;entrée générant un résultat qui est appliqué directement à cette dernière. &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; ce mode entraîne des modifications irréversibles sur la base de données et, en cas d&apos;échec, la structure d&apos;origine n&apos;est pas restaurée. Assurez-vous donc d&apos;avoir une sauvegarde avant de continuer.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1615"/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1699"/>
        <source>Changes:</source>
        <translation>Modifications :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1857"/>
        <source>Step label...</source>
        <translation>Étiquette étape...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1938"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Objects marked with an &lt;span style=&quot; font-weight:600;&quot;&gt;ALTER&lt;/span&gt; may not be effectively changed unless that the differences detected are in attributes that can be modified through ALTER commands otherwise no operationwill be performed or, if the force recreation is checked, the object will be dropped and created again.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Les objets marqués d&apos;un &lt;span style=&quot;font-weight : 600;&quot;&gt;ALTER&lt;/span&gt; ne peuvent être modifiés efficacement que si les différences détectées sont dans les attributs qui peuvent être modifié à l&apos;aide des commandes ALTER. Dans le cas contraire, aucune opération ne sera effectuée ou, si la recréation de force est vérifiée, l&apos;objet sera supprimé et créé à nouveau.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1990"/>
        <source>Objects to be created</source>
        <translation>Objets à créer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1993"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2034"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2075"/>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2116"/>
        <source>0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2031"/>
        <source>Objects to be dropped</source>
        <translation>Objets à supprimer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2072"/>
        <source>Possible objects to be changed</source>
        <translation>Objets possiblement modifiables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2113"/>
        <source>Ignored objects (system ones or with sql disabled)</source>
        <translation>Objets ignorés (ceux du système ou avec code SQL désactivé)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2159"/>
        <source>Diff Preview</source>
        <translation>Aperçu de la comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2205"/>
        <source>&amp;Apply diff</source>
        <translation>&amp;Appliquer la comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="2284"/>
        <source>&amp;Generate</source>
        <translation>&amp;Générer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="542"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The generated diff is ready to be exported! Once started this process will cause irreversible changes on the database. Do you really want to proceed?</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; La comparaison générée est prête à être exportée ! Une fois démarré, ce processus entraînera des changements irréversibles dans la base de données. Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="544"/>
        <source>Apply diff</source>
        <translation>Appliquer la comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="544"/>
        <source>Preview diff</source>
        <translation>Aperçu de la comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="165"/>
        <source>model not saved yet</source>
        <translation>modèle pas encore enregistré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="169"/>
        <source>(none)</source>
        <translation>(aucun)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="448"/>
        <source>Step %1/%2: Importing database &lt;strong&gt;%3&lt;/strong&gt;...</source>
        <translation>Étape %1/%2 : Import de la base de données &lt;strong&gt;%3&lt;/strong&gt;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="501"/>
        <source>Step %1/%2: Comparing &lt;strong&gt;%3&lt;/strong&gt; and &lt;strong&gt;%4&lt;/strong&gt;...</source>
        <translation>Étape %1/%2 : Comparaison de &lt;strong&gt;%3&lt;/strong&gt; et de &lt;strong&gt;%4&lt;/strong&gt;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="541"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="552"/>
        <source>Step %1/%2: Exporting diff to database &lt;strong&gt;%3&lt;/strong&gt;...</source>
        <translation>Étape %1/%2 : Export de la comparaison de la base de données &lt;strong&gt;%3&lt;/strong&gt;...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="580"/>
        <source>Diff process paused. Waiting user action...</source>
        <translation>Comparaison mise en pause. En attente d&apos;une action utilisateur...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="615"/>
        <source>Saving diff to file &lt;strong&gt;%1&lt;/strong&gt;</source>
        <translation>Enregistrement de la comparaison vers le fichier &lt;strong&gt;%1&lt;/strong&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="638"/>
        <source>Diff process sucessfully ended!</source>
        <translation>Comparaison términée avec succès !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="639"/>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="654"/>
        <source>No operations left.</source>
        <translation>Aucune opération restante.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="653"/>
        <source>Operation cancelled by the user.</source>
        <translation>Opération annulée par l&apos;utilisateur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="695"/>
        <source>Process aborted due to errors!</source>
        <translation>Processus abandonné en raison d&apos;erreurs !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="754"/>
        <source>-- No differences were detected between model and database. --</source>
        <translation>-- Aucune différence détectée entre le modèle et la base de données. --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="769"/>
        <source>Error code &lt;strong&gt;%1&lt;/strong&gt; found and ignored. Proceeding with export.</source>
        <translation>Code erreur &lt;strong&gt;%1&lt;/strong&gt; reçu et ignoré. Poursuite de l&apos;export.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="877"/>
        <source>Save diff as...</source>
        <translation>Enregistrer la comparaison sous...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1438"/>
        <source>Ignores as many as possible errors on import step. This option generates an incomplete diff.</source>
        <translation>Ignore autant d&apos;erreurs que possible sur l&apos;étape de l&apos;import. Cette option génère une comparaison incomplète.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1100"/>
        <source>Clears the data of all tables which will have columns modified. This is useful to avoid errors related to type casting. &lt;strong&gt;WARNING:&lt;/strong&gt; DO NOT use this option on production servers and always make a backup before use it.</source>
        <translation>Efface les données de toutes les tables qui auront des colonnes modifiées. Ceci est utile pour éviter les erreurs liées à la conversion de type. &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; N&apos;utilisez PAS cette option sur les serveurs de production et faites toujours une sauvegarde avant de l&apos;utiliser.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1348"/>
        <source>Import system (built-in) objects. Use this if the import step is returning errors related to missing objects.</source>
        <translation>Importer des objets système (intégrés). Utiliser cette option si l&apos;étape d&apos;import renvoie des erreurs liées à des objets manquants.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1393"/>
        <source>Import objects created by extensions. Use this if the import step is returning errors even importing built in ones.</source>
        <translation>Importer des objets créés par des extensions. Utiliser cette option si l&apos;étape d&apos;import renvoie des erreurs, même si celles-ci sont importées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1247"/>
        <source>Instead of use an ALTER command to modify certain kind of objects a DROP and CREATE will be used in order to do a full modification. This option does not affects the database object.</source>
        <translation>Au lieu d&apos;utiliser une commande ALTER pour modifier certains types d&apos;objets, un DROP et un CREATE seront utilisés pour effectuer une modification complète. Cette option n&apos;affecte pas l&apos;objet de base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1010"/>
        <source>No command to rename the destination database will be generated even the model&apos;s name differ from database name.</source>
        <translation>Aucune commande permettant de renommer la base de données de destination ne sera générée même si le nom du modèle diffère du nom de la base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1013"/>
        <source>Preserve database name</source>
        <translation>Conserver le nom de la base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="911"/>
        <source>Avoid the generation of DROP commands for objects that exists in database but not in the model. This is useful when diff a partial model against the complete database.</source>
        <translation>Éviter la génération de commandes DROP pour les objets qui existent dans la base de données mais pas dans le modèle. Ceci est utile lorsque un modèle partiel a été comparé à la base de données complète.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="914"/>
        <source>Do not drop missing objects</source>
        <translation>Ne pas supprimer les objets manquants</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="575"/>
        <source>Store in S&amp;QL file</source>
        <translation>Stocker dans un fichier S&amp;QL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="733"/>
        <source>Appl&amp;y on server</source>
        <translation>Appliquer sur le serveur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="738"/>
        <source>

-- SQL code purposely truncated at this point in demo version!</source>
        <translation>

-- Le code SQL est volontairement tronqué à ce stade dans la version démo !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="35"/>
        <source>Diff tool</source>
        <translation>Outil de comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="133"/>
        <source>Generate diff code</source>
        <translation>Générer un code de comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="195"/>
        <source>Source database</source>
        <translation>Base de données source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="230"/>
        <source>Current model:</source>
        <translation>Modèle courant :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="253"/>
        <source>(model)</source>
        <translation>(modèle)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="387"/>
        <source>Compare to</source>
        <translation>Comparer à</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="804"/>
        <source>Diff</source>
        <translation>Comparaison</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1301"/>
        <source>Import &amp;&amp; Export</source>
        <translation>Import &amp;&amp; Export</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1319"/>
        <source>Import</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1475"/>
        <source>Export</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1571"/>
        <source>This advanced option causes pgModeler to ignore extra errors by their numeric codes. These errors must be informed in the input below and separeted by space. For the complete list of error codes check the PostgreSQL docs, section &lt;strong&gt; Appendix A. PostgreSQL Error Codes&lt;/strong&gt;. &lt;strong&gt;WARNING:&lt;/strong&gt; use this option with extreme care since it can interfere in final export result.</source>
        <translation>Cette option avancée oblige pgModeler à ignorer les erreurs supplémentaires par leurs codes numériques. Ces erreurs doivent être renseignées dans l&apos;entrée ci-dessous et séparées par des espaces. Pour la liste complète des codes d&apos;erreur, consulter les documents PostgreSQL, section &lt;strong&gt;Annexe A. Codes d&apos;erreur PostgreSQL&lt;/strong&gt;. &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; utiliser cette option avec un soin extrême, car elle peut interférer dans le résultat de l&apos;export finale.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1574"/>
        <source>Ignore error codes</source>
        <translation>Ignorer les codes d&apos;erreur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="968"/>
        <source>Froce the generation of DROP commands for columns and constraints that exist in database but not in the model. This is useful when diff a partial model against the complete database and the user needs to drop columns and constraint but preserve the rest of the objects.</source>
        <translation>Forcer la génération de commandes DROP pour les colonnes et les contraintes qui existent dans la base de données mais pas dans le modèle. Ceci est utile lorsqu&apos;un modèle partiel est comparé à la base de données complète et que l&apos;utilisateur doit supprimer les colonnes ainsi que les contraintes mais conserver les autres objets.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="971"/>
        <source>Drop missing columns and constraints</source>
        <translation>Supprimer colonnes/contraintes manquantes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="1103"/>
        <source>Truncate tables before alter columns</source>
        <translation>Tronquer les tables avant de modifier les colonnes</translation>
    </message>
</context>
<context>
    <name>ModelExportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="724"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1067"/>
        <source>File:</source>
        <translation>Fichier :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="755"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1098"/>
        <source>Select target file</source>
        <translation>Sélectionner le fichier cible</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="758"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1101"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1140"/>
        <source>Zoom:</source>
        <translation>Zoom :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="35"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="133"/>
        <source>Export model</source>
        <translation>Export d&apos;un modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="152"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="176"/>
        <source>Database server</source>
        <translation>Serveur de base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="349"/>
        <source>pgModeler ignores errors generated by duplicated objects and creates only that ones which does not exists in the database. This option may be used when an object was created after a previous model export.</source>
        <translation>pgModeler ignorera les erreurs générées en raison d&apos;objets dupliqués et ne créera uniquement ceux qui n&apos;existent pas déjà en base. Cette option peut être utile lorsqu&apos;un objet est créé après un précédent export.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="392"/>
        <source>PostgreSQL version in which the SQL code should be generated. It is recommended to select this option only when the version of the DBMS, somehow, is not identifiable or if you need to generate a specific version of SQL code for test purposes.</source>
        <translation>Version de PostgreSQL pour laquelle le code SQL sera généré. Il est recommandé d&apos;activer cette option uniquement si la version du SGBD n&apos;est pas identifiable ou pour une raison spécifique par exemple pour des essais.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="547"/>
        <source>If &lt;strong&gt;DB&lt;/strong&gt; is checked pgModeler will destroy the database if already exists on the server. When &lt;strong&gt;Objects&lt;/strong&gt; is checked pgModeler will execute the DROP command attached to SQL-enabled objects. &lt;strong&gt;WARNING:&lt;/strong&gt; this option leads to data loss so make sure to have a backup first.</source>
        <translation>Si &lt;strong&gt;BDD&lt;/strong&gt; est coché, pgModeler procédera à la suppression complète de la base de données si elle existe déjà. Lorsque &lt;strong&gt;Objets&lt;/strong&gt; est coché pgModeler effectuera la commande DROP rattachée à l&apos;objet (si la génération du code SQL pour cet objet n&apos;a pas été désactivée dans ses propriétés). &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; cette option engendre la perte de données, pensez à faire une sauvegarde avant.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="550"/>
        <source>Drop:</source>
        <translation>Suppr. :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="560"/>
        <source>DB</source>
        <translation>BDD</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1187"/>
        <source>Show delimiters</source>
        <translation>Afficher les délimiteurs de page</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1203"/>
        <source>Exporting the model page by page will generate files with a &lt;strong&gt;_p[n]&lt;/strong&gt; suffix where &lt;strong&gt;n&lt;/strong&gt; is the page id. Check if the current user has write permission on output folder.</source>
        <translation>Exporter le modèle page par page générera des fichiers avec un suffixe de type &lt;strong&gt;_p[n]&lt;/strong&gt; où &lt;strong&gt;n&lt;/strong&gt; est le numéro de la page. Vérifiez que vous avez bien le droit d&apos;écriture dans le dossier de sortie.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1206"/>
        <source>Page by page</source>
        <translation>Une image par page</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="395"/>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="794"/>
        <source>PostgreSQL:</source>
        <translation>PostgreSQL :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="818"/>
        <source>PostgreSQL version in which the SQL code should be generated</source>
        <translation>Version de PostgreSQL pour laquelle le code SQL doit être généré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1375"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="598"/>
        <source>pgModeler will destroy the database if already exists on the server. Make sure to have a backup before use this option because all data will be lost.</source>
        <translation>pgModeler détruira la base de données si elle existe déjà sur le serveur. Assurez-vous d&apos;avoir une sauvegarde si besoin avant car les données seront définitivement perdues.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1171"/>
        <source>Show grid</source>
        <translation>Afficher la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="306"/>
        <source>Connection:</source>
        <translation>Connexion :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="352"/>
        <source>Ignore object duplicity</source>
        <translation>Ignorer les objets dupliqués</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="648"/>
        <source>SQL file</source>
        <translation>Fichier texte SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1029"/>
        <source>I&amp;mage (PNG)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1289"/>
        <source>Output</source>
        <translation>Progression</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1401"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1515"/>
        <source>&amp;Export</source>
        <translation>&amp;Exporter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1553"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="191"/>
        <source>Initializing model export...</source>
        <translation>Initialisation de l&apos;export du modèle...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="196"/>
        <source>Saving file &apos;%1&apos;</source>
        <translation>Enregistrement du fichier &apos;%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="225"/>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="303"/>
        <source>Exporting process aborted!</source>
        <translation>Export abandonnée !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="317"/>
        <source>Exporting process canceled by user!</source>
        <translation>Export annulée par l&apos;utilisateur !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="327"/>
        <source>Exporting process sucessfuly ended!</source>
        <translation>Export terminée avec succès !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="254"/>
        <source>Export model as...</source>
        <translation>Exporter le modèle sous...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="126"/>
        <source>Error code &lt;strong&gt;%1&lt;/strong&gt; found and ignored. Proceeding with export.</source>
        <translation>Code erreur &lt;strong&gt;%1&lt;/strong&gt; reçu et ignoré. Poursuite de l&apos;export.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="573"/>
        <source>Ob&amp;jects</source>
        <translation>Ob&amp;jets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="889"/>
        <source>Graphics file</source>
        <translation>Fichier graphique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1022"/>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="262"/>
        <source>SQL script (*.sql);;All files (*.*)</source>
        <translation>Script SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="269"/>
        <source>Portable Network Graphics (*.png);;All files (*.*)</source>
        <translation>Portable Network Graphics (*.png);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="274"/>
        <source>Scalable Vector Graphics (*.svg);;All files (*.*)</source>
        <translation>Scalable Vector Graphics (*.svg);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="514"/>
        <source>This advanced option causes pgModeler to ignore extra errors by their numeric codes. These errors must be informed in the input below and separeted by space. For the complete list of error codes check the PostgreSQL docs, section &lt;strong&gt; Appendix A. PostgreSQL Error Codes&lt;/strong&gt;. &lt;strong&gt;WARNING:&lt;/strong&gt; use this option with extreme care since it can interfere in final export result.</source>
        <translation>Cette option avancée oblige pgModeler à ignorer les erreurs supplémentaires par leurs codes numériques. Ces erreurs doivent être renseignées dans l&apos;entrée ci-dessous et séparées par des espaces. Pour la liste complète des codes d&apos;erreur, consulter les documents PostgreSQL, section &lt;strong&gt;Annexe A. Codes d&apos;erreur PostgreSQL&lt;/strong&gt;. &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; utiliser cette option avec un soin extrême, car elle peut interférer dans le résultat de l&apos;export finale.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="517"/>
        <source>Ignore error codes</source>
        <translation>Ignorer les codes d&apos;erreur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="1045"/>
        <source>&amp;Vectorial (SVG)</source>
        <translation>&amp;Vectoriel (SVG)</translation>
    </message>
</context>
<context>
    <name>ModelExportHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="74"/>
        <source>Generating SQL code for PostgreSQL `%1&apos;</source>
        <translation>Génération du code SQL pour PostgreSQL `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="79"/>
        <source>Output SQL file `%1&apos; successfully written.</source>
        <translation>Fichier SQL de sortie `%1&apos; écrit avec succès.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="201"/>
        <source>Rendering objects to page %1/%2.</source>
        <translation>Rendu des objets de la page %1/%2.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="228"/>
        <source>Output image `%1&apos; successfully written.</source>
        <translation>Fichier image de sortie `%1&apos; écrit avec succès.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="347"/>
        <source>Starting export to DBMS.</source>
        <translation>Lancement de l&apos;export vers le SGBD.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="353"/>
        <source>PostgreSQL version detection overridden. Using version `%1&apos;.</source>
        <translation>Détection de version dépassée de PostgreSQL. Utilisation de la version `%1&apos;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="358"/>
        <source>PostgreSQL `%1&apos; server detected.</source>
        <translation>Serveur PostgreSQL `%1&apos; détecté.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="363"/>
        <source>Generating temporary names for database, roles and tablespaces.</source>
        <translation>Génération de noms, de rôles et d&apos;espace de stockage temporaires pour la base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="374"/>
        <source>Enabling the SQL code for database `%1&apos; to avoid errors.</source>
        <translation>Activation du code SQL de la base de données `%1&apos; pour éviter les erreurs.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="380"/>
        <source>Ignoring object duplication errors.</source>
        <translation>Omission des erreurs de duplication d&apos;objet.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="387"/>
        <source>Ignoring the following error code(s): `%1&apos;.</source>
        <translation>Omission des codes erreur suivants : `%1&apos;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="391"/>
        <source>Trying to drop database `%1&apos;.</source>
        <translation>Tentative de suppression de la base de données `%1&apos;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="407"/>
        <source>Simulation mode activated.</source>
        <translation>Mode simulation activé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="511"/>
        <source>Generating SQL for `%1&apos; objects...</source>
        <translation>Génération de code SQL pour `%1&apos; objets...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="640"/>
        <source>Destroying objects created on the server.</source>
        <translation>Destruction des objets créés sur le server.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="666"/>
        <source>Restoring original names of database, roles and tablespaces.</source>
        <translation>Restauration des noms, des rôles et des espaces de stockage originaux de la base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="425"/>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="876"/>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="957"/>
        <source>Creating object `%1&apos; (%2)</source>
        <translation>Création de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="874"/>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="959"/>
        <source>Dropping object `%1&apos; (%2)</source>
        <translation>Suppression de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="961"/>
        <source>Changing object `%1&apos; (%2)</source>
        <translation>Modification de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="974"/>
        <source>Running auxiliary command.</source>
        <translation>Exécution de la commande auxiliaire.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="261"/>
        <source>Exporting model to SVG file.</source>
        <translation>Export du modèle vers un fichier SVG.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="264"/>
        <source>SVG representation of database model</source>
        <translation>Représentation SVG d&apos;un modèle de base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="265"/>
        <source>SVG file generated by pgModeler</source>
        <translation>Fichier SVG généré par pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="307"/>
        <source>Output file `%1&apos; successfully written.</source>
        <translation>Fichier de sortie `%1&apos; écrit avec succès.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="470"/>
        <source>Creating database `%1&apos;</source>
        <translation>Création de la base de données `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="504"/>
        <source>Connecting to database `%1&apos;</source>
        <translation>Connexion à la base de données `%1&apos;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="716"/>
        <source>Renaming `%1&apos; (%2) to `%3&apos;</source>
        <translation>Renommage de `%1&apos; (%2) en `%3&apos;</translation>
    </message>
</context>
<context>
    <name>ModelFixForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="35"/>
        <source>Model file fix</source>
        <translation>Réparer un fichier modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="727"/>
        <source>&amp;Fix</source>
        <translation>&amp;Réparer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="765"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="124"/>
        <source>Fix model file</source>
        <translation>Réparer un modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="226"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;[pgmodeler-cli not found error]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;[pgmodeler-cli not found error]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="245"/>
        <source>pgmodeler-cli:</source>
        <translation>pgmodeler-cli :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="263"/>
        <source>Browse for pgmodeler-cli tool</source>
        <translation>Rechercher l&apos;outil pgmodeler-cli</translation>
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
        <translation>Ce fichier n&apos;est pas l&apos;outil en ligne de commande de pgModeler (pgmodeler-cli).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="340"/>
        <source>Input file:</source>
        <translation>Fichier source :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="353"/>
        <source>Output file:</source>
        <translation>Fichier cible :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="360"/>
        <source>Fix tries:</source>
        <translation>Tentatives :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="500"/>
        <source>Select input file</source>
        <translation>Sélectionner le modèle à réparer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="530"/>
        <source>Select output file</source>
        <translation>Sélectionner un fichier de sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="570"/>
        <source>Load fixed model when finish</source>
        <translation>Charger le modèle réparé une fois l&apos;opération terminée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="659"/>
        <source>In some cases the fix process will fail to restore all objects within the model demanding manual fixes by changing the file on a text editor. &lt;strong&gt;NOTE:&lt;/strong&gt; relationships may lost their graphical configuration like custom points and line color.</source>
        <translation>Dans certains cas, le processus de réparation peut échouer à restaurer et récupérer tous les objets du modèle, ce qui peut réclamer des modifications manuelles dans le fichier à partir d&apos;un éditeur de texte. &lt;strong&gt;REMARQUE :&lt;/strong&gt; les associations peuvent perdre leur configuration graphique tels que des points placés manuellement par l&apos;utilisateur ou la couleur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelfixform.cpp" line="69"/>
        <source>Waiting process to start...</source>
        <translation>Processus en attente de lancement...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelfixform.cpp" line="80"/>
        <source>Could not locate &lt;strong&gt;%1&lt;/strong&gt; tool on &lt;strong&gt;%2&lt;/strong&gt;. The fix process can&apos;t continue! Please check pgModeler installation or try to manually specify the command below.</source>
        <translation>Impossible de localiser l&apos;outil &lt;strong&gt;%1&lt;/strong&gt; sur &lt;strong&gt;%2&lt;/strong&gt;. Le processus de réparation ne peut continuer ! Vérifiez votre installation de pgModeler ou spécifiez manuellement la commande ci-dessous.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelfixform.cpp" line="151"/>
        <source>pgModeler command line tool (%1)</source>
        <translation>Outil en ligne de commande de pgModeler (%1)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="475"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Monospace&apos;; font-size:10pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:9pt;&quot;&gt;Waiting process to start...&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt; !DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http ://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Monospace&apos;; font-size:10pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:9pt;&quot;&gt;En attente du lancement du processus...&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>ModelNavigationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="23"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="59"/>
        <source>Previous model</source>
        <translation>Modèle précédent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="176"/>
        <source>Alt+C</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="100"/>
        <source>Next model</source>
        <translation>Modèle suivant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="160"/>
        <source>Close model</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="163"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelnavigationwidget.cpp" line="73"/>
        <source>(model not saved yet)</source>
        <translation>(modèle pas encore enregistré)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="75"/>
        <source>Ctrl+Left</source>
        <translation>Ctrl+Gauche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="116"/>
        <source>Ctrl+Right</source>
        <translation>Ctrl+Droite</translation>
    </message>
</context>
<context>
    <name>ModelObjectsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="35"/>
        <source>Model Objects</source>
        <translation>Objets du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="567"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="226"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="323"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="335"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="347"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="359"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="461"/>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="474"/>
        <source>Clear All</source>
        <translation>Tout effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="674"/>
        <source>Select</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="690"/>
        <source>Return</source>
        <translation>Retour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="709"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="725"/>
        <source>Esc</source>
        <translation>Échap</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="820"/>
        <source>Objects view configuration</source>
        <translation>Filtres d&apos;affichage des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="858"/>
        <source>Expands all items</source>
        <translation>Développer tous les éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="893"/>
        <source>Collapses all items</source>
        <translation>Replier tous les éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="617"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
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
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="744"/>
        <source>Tree view</source>
        <translation>Vue en arbre</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="785"/>
        <source>List view</source>
        <translation>Vue en liste</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelobjectswidget.cpp" line="166"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="311"/>
        <source>ID</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="634"/>
        <source>By ID</source>
        <translation>Par identifiant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="388"/>
        <source>Visible object types</source>
        <translation>Types d&apos;objets visibles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="535"/>
        <source>Model objects</source>
        <translation>Objets de modèle</translation>
    </message>
</context>
<context>
    <name>ModelOverviewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeloverviewwidget.ui" line="41"/>
        <source>Model overview</source>
        <translation>Vue générale du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeloverviewwidget.cpp" line="126"/>
        <source>Failed to generate the overview image.
The requested size %1 x %2 was too big and there was not enough memory to allocate!</source>
        <translation>Impossible de générer l&apos;aperçu de l&apos;image.
La taille demandée%1 x%2 était trop grande et il n&apos;y avait pas assez de mémoire à allouer !</translation>
    </message>
</context>
<context>
    <name>ModelRestorationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="35"/>
        <source>Model restoration</source>
        <translation>Restauration de modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="78"/>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="362"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="214"/>
        <source>pgModeler was not closed properly in a previous execution and some models were still being edited. Click &lt;strong&gt;Restore&lt;/strong&gt; to reopen the models or &lt;strong&gt;Cancel&lt;/strong&gt; to abort the restoration.</source>
        <translation>pgModeler n&apos;a pas été fermé correctement lors d&apos;une précédente exécution et certains modèles étaient encore en cours d&apos;édition. Cliquez sur &lt;strong&gt;Restaurer&lt;/strong&gt; pour rouvrir les modèles ou sur &lt;strong&gt;Annuler&lt;/strong&gt; pour abandonner la restauration.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="238"/>
        <source>pgModeler will try to recover the selected models but will not destroy them in case of loading failure. This option serves as a last resort in order to try to recover the database model. Temporary models will last until the application is closed so the user must try to manually recover the files before exit pgModeler.</source>
        <translation>pgModeler va essayer de récupérer les modèles sélectionnés mais ne les détruira pas en cas d&apos;échec de chargement. Cette option sert de dernier recours pour essayer de récupérer le modèle de base de données. Les modèles temporaires resteront jusqu&apos;à ce que l&apos;application soit fermée. L&apos;utilisateur doit donc essayer de récupérer manuellement les fichiers avant de quitter pgModeler.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="241"/>
        <source>Keep temporary models in case of restoration failure</source>
        <translation>Conserver les modèles temporaires en cas d&apos;échec de la restauration</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="294"/>
        <source>Database</source>
        <translation>Base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="306"/>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="318"/>
        <source>Modified</source>
        <translation>Modifié</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="330"/>
        <source>Size</source>
        <translation>Taille</translation>
    </message>
</context>
<context>
    <name>ModelValidationHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationhelper.cpp" line="586"/>
        <source>There are pending errors! SQL validation will not be executed.</source>
        <translation>Il y a des erreurs en attente ! La validation SQL ne sera pas exécutée.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationhelper.cpp" line="670"/>
        <source>Operation canceled by the user.</source>
        <translation>Opération annulée par l&apos;utilisateur.</translation>
    </message>
</context>
<context>
    <name>ModelValidationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="32"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="523"/>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="611"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="97"/>
        <source>Try to apply a fix on the selected validation info.</source>
        <translation>Essayer d&apos;appliquer le correctif sur l’élément sélectionné.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="132"/>
        <source>Clear validation results</source>
        <translation>Effacer le résultat de la validation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="62"/>
        <source>Va&amp;lidate</source>
        <translation>Va&amp;lider</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="498"/>
        <source>Warnings: does not prevents model to be saved.</source>
        <translation>Avertissements : n&apos;empêche de pouvoir sauvegarder le modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="583"/>
        <source>Errors: model will not be saved while there are validation errors.</source>
        <translation>Erreurs : le modèle ne peut être sauvegardé tant que des erreurs de validation subsistent.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="235"/>
        <source>SQL Validation:</source>
        <translation>Validation SQL :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="332"/>
        <source>PostgreSQL version</source>
        <translation>Version de PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="718"/>
        <source>Try to resolve the reported issues.</source>
        <translation>Essayer de résoudre les problèmes rencontrés.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="734"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="135"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="100"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="376"/>
        <source>Cancel the SQL validation in progress.</source>
        <translation>Annuler le processus de validation SQL en cours.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="379"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="392"/>
        <source>Esc</source>
        <translation>Échap</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="721"/>
        <source>Apply fixes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="753"/>
        <source>Change the creation order for two objects by swapping their ids</source>
        <translation>Modifie l&apos;ordre de création de deux objets en échangeant leurs numéros d&apos;identification</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="756"/>
        <source>Swap ids</source>
        <translation type="unfinished">Échanger identifiants</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="686"/>
        <source>Hide this widget</source>
        <translation>Fermer ce widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="689"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="223"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] is being referenced by &lt;strong&gt;%4&lt;/strong&gt; object(s) before its creation.</source>
        <translation>L&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] est référencé par &lt;strong&gt;%4&lt;/strong&gt; objet(s) avant sa création.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="32"/>
        <source>Autodetect</source>
        <translation>Auto détection</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="238"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3]%4 is referencing columns created by &lt;strong&gt;%5&lt;/strong&gt; relationship(s) but is created before them.</source>
        <translation>L&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id : %3]%4 référence des colonnes créées par &lt;strong&gt;%5&lt;/strong&gt; association(s) mais a été créé avant eux.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="257"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; has a name that conflicts with &lt;strong&gt;%3&lt;/strong&gt; object&apos;s name(s).</source>
        <translation>Le nom de l&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; entre en conflit avec celui de l&apos;objet&lt;strong&gt;%3&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="484"/>
        <source>Processing object: %1</source>
        <translation>Traitement de l&apos;objet : %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="360"/>
        <source>Conflicting object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;.</source>
        <translation>Objet en conflit : &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="372"/>
        <source>Referrer object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</source>
        <translation>Objet réferent : &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="268"/>
        <source>SQL validation failed due to error(s) below. &lt;strong&gt;NOTE:&lt;/strong&gt;&lt;em&gt; These errors does not invalidates the model but may affect operations like &lt;strong&gt;export&lt;/strong&gt; and &lt;strong&gt;diff&lt;/strong&gt;.&lt;/em&gt;</source>
        <translation>La validation SQL a échoué en raison des erreurs ci-dessous. &lt;strong&gt;REMARQUE :&lt;/strong&gt;&lt;em&gt; Ces erreurs n&apos;invalident pas le modèle mais peuvent affecter des opérations telles que &lt;strong&gt;l&apos;export&lt;/strong&gt; et &lt;strong&gt;la comparaison&lt;/strong&gt;. &lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="273"/>
        <source>The column &lt;strong&gt;%1&lt;/strong&gt; on &lt;strong&gt;%2&lt;/strong&gt; &lt;em&gt;(%3)&lt;/em&gt; is referencing the geospatial data type &lt;strong&gt;%4&lt;/strong&gt; but the &lt;strong&gt;postgis&lt;/strong&gt; extension is not present in the model!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="321"/>
        <source>&lt;strong&gt;HINT:&lt;/strong&gt; Create the extension in the model or let it be created by applying the needed fixes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="354"/>
        <source>&lt;em&gt;The above object was created by a relationship. Change the name pattern on it&apos;s generator relationship. Fix will not be applied!&lt;/em&gt;</source>
        <translation>&lt;em&gt;L&apos;objet ci-dessus a été créé par une relation. Changer le motif du nom sur sa relation de générateur. La correction ne sera pas appliquée !&lt;/Em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="367"/>
        <source>Relationship: &lt;strong&gt;%1&lt;/strong&gt; [id: %2].</source>
        <translation>Relation : &lt;strong&gt;%1&lt;/strong&gt; [id : %2].</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="472"/>
        <source>Running SQL commands on server...</source>
        <translation>Exécution des commandes SQL sur le serveur...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="232"/>
        <source>Enables the validation of SQL code in DBMS. This process requires the use of a pre-configured connection. SQL validation will occur only in the last step (when all objects were validated) or when there are no warnings.</source>
        <translation>Active la validation du code SQL dans la SGBD. Ce processus nécessite l&apos;utilisation d&apos;une connexion pré-configurée. La validation SQL ne se produira qu&apos;à la dernière étape (lorsque tous les objets ont été validés) ou lorsqu&apos;il n&apos;y a pas d&apos;avertissement.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="266"/>
        <source>Connection to be used in the SQL validation</source>
        <translation>Connexion à utiliser dans la validation SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="354"/>
        <source>pgModeler will generate unique and temporary names for database, role and tablespace objects. This option avoids object duplication errors when running the SQL validation.</source>
        <translation>pgModeler générera des noms uniques et temporaires pour les objets base de données, rôle et espace de stockage. Cette option évite les erreurs de duplication d&apos;objet lors de l&apos;exécution de la validation SQL.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="357"/>
        <source>Use unique temporary names for cluster level objects</source>
        <translation>Utiliser des noms temporaires uniques pour les objets de niveau cluster</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="264"/>
        <source>The relationship &lt;strong&gt;%1&lt;/strong&gt; [id: %2] is in a permanent invalidation state and needs to be relocated.</source>
        <translation>La relation &lt;strong&gt;%1&lt;/strong&gt; [id : %2] est dans un état d&apos;invalidation permanent et a besoin d&apos;être déplacé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="316"/>
        <source>&lt;strong&gt;HINT:&lt;/strong&gt; try to swap the relationship by another ones that somehow are linked to it through generated columns or constraints to solve this issue. Note that other objects may be lost in the swap process.</source>
        <translation>&lt;strong&gt;CONSEIL :&lt;/strong&gt; essayer d&apos;échanger la relation par une autre qui est liée à celle-ci via des colonnes générées ou des contraintes pour résoudre ce problème. Noter que d&apos;autres objets peuvent être perdus dans le processus d&apos;échange.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="439"/>
        <source>SQL validation not executed! No connection defined.</source>
        <translation>La validation SQL n&apos;est pas exécutée ! Aucune connexion définie.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="446"/>
        <source>Database model successfully validated.</source>
        <translation>Modèle de base de données validé avec succès.</translation>
    </message>
</context>
<context>
    <name>ModelWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="84"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="241"/>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="239"/>
        <source>Convert</source>
        <translation>Convertir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="255"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="258"/>
        <source>Quick</source>
        <translation>Actions rapides</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3869"/>
        <source>Source code</source>
        <translation>Code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="217"/>
        <source>Show object source code</source>
        <translation>Afficher le code source de l&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="219"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3862"/>
        <source>Properties</source>
        <translation>Propriétés</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="220"/>
        <source>Space</source>
        <translation>Espace</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="221"/>
        <source>Edit the object properties</source>
        <translation>Modifier les propriétés de l&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="223"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3891"/>
        <source>Protect</source>
        <translation>Vérrouiller</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="224"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3886"/>
        <source>Unprotect</source>
        <translation>Déverrouiller</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="225"/>
        <source>Protects object(s) from modifications</source>
        <translation>Empêcher la modification du ou des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="227"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3898"/>
        <source>Delete</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="232"/>
        <source>Shift+Del</source>
        <translation>Shift+Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="235"/>
        <source>Select all</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="236"/>
        <source>Selects all the graphical objects in the model</source>
        <translation>Sélectionner tous les objets graphiques du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="245"/>
        <source>Paste</source>
        <translation>Coller</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="249"/>
        <source>Cut</source>
        <translation>Couper</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="253"/>
        <source>Deps &amp;&amp; Referrers</source>
        <translation>Dépendances &amp;&amp; Rèférences</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="256"/>
        <source>Add a new object in the model</source>
        <translation>Ajouter un nouvel objet dans le modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="262"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="264"/>
        <source>Quick rename the object</source>
        <translation>Renommage rapide de l&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="266"/>
        <source>Move to schema</source>
        <translation>Déplacer vers le schema</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="272"/>
        <source>Edit permissions</source>
        <translation>Modifier les permissions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="275"/>
        <source>Change owner</source>
        <translation>Changer le propriétaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="278"/>
        <source>Select children</source>
        <translation>Sélectionner les enfants</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="228"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="284"/>
        <source>Custom SQL</source>
        <translation>Code SQL personnalisé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="1355"/>
        <source>Loading database model</source>
        <translation>Ouverture du modèle de base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="1572"/>
        <source>Saving database model</source>
        <translation>Enregistrement du modèle de base de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2229"/>
        <source>Also copy all dependencies of selected objects? This minimizes the breakdown of references when copied objects are pasted into another model.</source>
        <translation>Copier également les dépendences des objets sélectionnés ? Cela minimise la casse des références lorsque les objets copiés sont collés dans un autre modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2333"/>
        <source>Pasting objects...</source>
        <translation>Collage des objets...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2591"/>
        <source>Not all objects were pasted to the model due to errors returned during the process! Refer to error stack for more details!</source>
        <translation>Tous les objets n&apos;ont pas été collés dans le modèle car des erreurs ont été retournées durant le processus ! Se référer à la pile d&apos;erreurs pour plus de détails !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2727"/>
        <source>Do you really want to delete the selected object?</source>
        <translation>Souhaitez-vous réellement effacer l&apos;objet sélectionné ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3149"/>
        <source>(no objects)</source>
        <translation>(aucun objet)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3156"/>
        <source>None</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3917"/>
        <source>Constraints</source>
        <translation>Contraintes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="83"/>
        <source>One to One (1-1)</source>
        <translation>Un à un (1-1)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="83"/>
        <source>One to Many (1-n)</source>
        <translation>Un à plusieurs (1-n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="84"/>
        <source>Many to Many (n-n)</source>
        <translation>Plusieurs à plusieurs (n-n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="138"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; The database model is protected! Operations that could modify it are disabled!</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Le modèle de base de données est protégé ! Les opérations qui pourraient le modifier sont désactivées !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="213"/>
        <source>Edit data</source>
        <translation type="unfinished">Modifier données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="215"/>
        <source>Source</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="216"/>
        <source>Alt+S</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="231"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3905"/>
        <source>Del. cascade</source>
        <translation>Supprimer en cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="242"/>
        <source>Ctrl+C</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="246"/>
        <source>Ctrl+V</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="250"/>
        <source>Ctrl+X</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="259"/>
        <source>Quick action for the selected object</source>
        <translation>Action rapide pour l&apos;objet sélectionné</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="263"/>
        <source>F2</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="269"/>
        <source>Set tag</source>
        <translation>Affecter une balise</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="273"/>
        <source>Ctrl+E</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="279"/>
        <source>Select tagged</source>
        <translation>Séléction balisée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="281"/>
        <source>Select</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="282"/>
        <source>Open relationship</source>
        <translation>Ouvrir les relations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="285"/>
        <source>Alt+Q</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="287"/>
        <source>Convert to sequence</source>
        <translation>Convertir en séquence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="288"/>
        <source>Convert to serial</source>
        <translation>Convertir en série</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="290"/>
        <source>Break line</source>
        <translation type="unfinished">Ligne d&apos;interruption</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="292"/>
        <source>Remove points</source>
        <translation>Supprimer des points</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="294"/>
        <source>Enable SQL</source>
        <translation>Activer le code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="295"/>
        <source>Disable SQL</source>
        <translation>Désactiver le code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="297"/>
        <source>Duplicate</source>
        <translation>Dupliquer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="298"/>
        <source>Ctrl+D</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="301"/>
        <source>Extended attributes</source>
        <translation>Attributs étendus</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="302"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="313"/>
        <source>Show</source>
        <translation>Afficher</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="303"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="314"/>
        <source>Hide</source>
        <translation>Masquer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="305"/>
        <source>Jump to table</source>
        <translation>Sauter à la table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="312"/>
        <source>Schemas rectangles</source>
        <translation type="unfinished">Rectangles de schémas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="319"/>
        <source>Fade in/out</source>
        <translation>Effets de fondu</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="320"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="323"/>
        <source>Fade in</source>
        <translation>Accentuer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="321"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="324"/>
        <source>Fade out</source>
        <translation>Estomper</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="329"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Relationships</source>
        <translation>Associations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="336"/>
        <source>Swap ids</source>
        <translation>Échanger identifiants</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="337"/>
        <source>Edit the objects creation order by swapping their ids</source>
        <translation>Modifier l&apos;ordre de création des objets en échangeant leurs identifiants</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="340"/>
        <source>90° (vertical)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="345"/>
        <source>90° (horizontal)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="350"/>
        <source>90° + 90° (vertical)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="355"/>
        <source>90° + 90° (horizontal)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3290"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3296"/>
        <source>All objects</source>
        <translation>Tous les objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Schemas</source>
        <translation>Schémas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Views</source>
        <translation>Vues</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Textboxes</source>
        <translation>Boites de textes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="393"/>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="3270"/>
        <source>Tables</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="401"/>
        <source>Ctrl+A</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="741"/>
        <source>Zoom: %1%</source>
        <translation>Zoom : %1%</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="1135"/>
        <source>Do you really want to convert the relationship into an intermediate table?</source>
        <translation>Voulez-vous vraiment convertir les relations en une table intermédiaire ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2356"/>
        <source>Validating object: `%1&apos; (%2)</source>
        <translation>Validation de l&apos;objet : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2458"/>
        <source>Generating XML for: `%1&apos; (%2)</source>
        <translation>Génération du code XML pour : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2536"/>
        <source>Pasting object: `%1&apos; (%2)</source>
        <translation>Collage de l&apos;objet : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2714"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; You are about to delete objects in cascade mode which means more objects than the selected will be dropped too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Vous êtes sur le point de supprimer des objets en mode cascade, ce qui signifie que des objets non sélectionnés seront également supprimés. Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2718"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; Remove multiple objects at once can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Supprimer plusieurs objets à la fois peut entraîner des invalidations irréversibles sur d&apos;autres objets du modèle, entraînant également la suppression de ces objets invalides. Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2724"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; Remove a relationship can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Supprimer une relation peut entraîner des invalidations irréversibles d&apos;autres objets dans le modèle, entraînant également la suppression de ces objets invalides. Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2993"/>
        <source>The cascade deletion found some problems when running! Some objects could not be deleted or registered in the operation&apos;s history! Please, refer to error stack for more details.</source>
        <translation>La suppression en cascade a rencontré des problèmes lors de son exécution ! Certains objets n&apos;ont pas pu être supprimés ou enregistrés dans l&apos;historique des opérations ! Veuillez vous reporter à la pile d&apos;erreurs pour plus de détails.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="84"/>
        <source>Inheritance</source>
        <translation>Héritage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2116"/>
        <source>Do you want to %1 the selected schema&apos;s children too?</source>
        <translation>Voulez-vous %1 les enfants du schèma séléctionné aussi ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2116"/>
        <source>protect</source>
        <translation>protéger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="2116"/>
        <source>unprotect</source>
        <translation>déprotéger</translation>
    </message>
</context>
<context>
    <name>ModelsDiffHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="249"/>
        <source>Processing object `%1&apos; (%2)...</source>
        <translation>Traitement de l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="362"/>
        <source>Skipping object `%1&apos; (%2)...</source>
        <translation>Omission de l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="626"/>
        <source>Processing diff infos...</source>
        <translation>Comparaison des informations...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="654"/>
        <source>Processing `%1&apos; info for object `%2&apos; (%3)...</source>
        <translation>Traitement de l&apos;info `%1&apos; pour l&apos;objet `%2&apos; (%3)...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="905"/>
        <source>No differences between the model and database.</source>
        <translation>Aucune différence entre le modèle et la base de données.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="907"/>
        <source>Preparing diff code...</source>
        <translation>Préparation du code de comparaison...</translation>
    </message>
</context>
<context>
    <name>NewObjectOverlayWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="28"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="268"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="488"/>
        <source>Tag</source>
        <translation>Tag</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="221"/>
        <source>Cast</source>
        <translation>Conversion de type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="400"/>
        <source>Language</source>
        <translation>Langage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="532"/>
        <source>Textbox</source>
        <translation>Zone de texte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="356"/>
        <source>Event Trigger</source>
        <translation>Déclencheur sur évènement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="177"/>
        <source>Tablespace</source>
        <translation>Espace de stockage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="133"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="779"/>
        <source>Domain</source>
        <translation>Domaine</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="647"/>
        <source>Conversion</source>
        <translation>Conversion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="312"/>
        <source>Extension</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="603"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="691"/>
        <source>Aggregate</source>
        <translation>Agrégat</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="823"/>
        <source>Collation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="867"/>
        <source>Sequence</source>
        <translation>Séquence</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="955"/>
        <source>Table</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="999"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1043"/>
        <source>View</source>
        <translation>Vue</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1131"/>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1290"/>
        <source>Permissions</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1334"/>
        <source>Index</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1378"/>
        <source>Rule</source>
        <translation>Règle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1246"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1202"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1422"/>
        <source>Trigger</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1713"/>
        <source>Copy</source>
        <translation>Copie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="234"/>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="32"/>
        <source>A</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="33"/>
        <source>G</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="34"/>
        <source>K</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="35"/>
        <source>H</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="36"/>
        <source>J</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="37"/>
        <source>D</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="38"/>
        <source>E</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="39"/>
        <source>F</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="40"/>
        <source>L</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="41"/>
        <source>O</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="42"/>
        <source>U</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="43"/>
        <source>I</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="44"/>
        <source>R</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="45"/>
        <source>S</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="46"/>
        <source>Q</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="47"/>
        <source>T</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="48"/>
        <source>P</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="49"/>
        <source>M</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="50"/>
        <source>Y</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="51"/>
        <source>W</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="52"/>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="58"/>
        <source>9</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="53"/>
        <source>Z</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="54"/>
        <source>X</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="55"/>
        <source>C</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="56"/>
        <source>V</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="57"/>
        <source>B</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="59"/>
        <source>8</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="61"/>
        <source>1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="62"/>
        <source>2</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="63"/>
        <source>3</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="64"/>
        <source>5</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="65"/>
        <source>4</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="95"/>
        <source>0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="911"/>
        <source>Op. Family</source>
        <translation>Famille d&apos;op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1087"/>
        <source>Op. Class</source>
        <translation>Classe d&apos;op.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="735"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1537"/>
        <source>Many-to-many</source>
        <translation>Plusieurs-à-plusieurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1581"/>
        <source>One-to-many</source>
        <translation>Un-à-plusieurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1625"/>
        <source>One-to-one</source>
        <translation>Un-à-un</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1669"/>
        <source>Inheritance</source>
        <translation>Héritage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="444"/>
        <source>Generic SQL</source>
        <translation>SQL générique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="1466"/>
        <source>Policy</source>
        <translation>Politique</translation>
    </message>
</context>
<context>
    <name>NumberedTextEditor</name>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="83"/>
        <source>Load</source>
        <translation>Charger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="84"/>
        <source>Load the object&apos;s source code from an external file</source>
        <translation>Charger le code source de l&apos;objet depuis un fichier externe</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="94"/>
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="95"/>
        <source>Edit the source code in the preferred external editor</source>
        <translation>Modifier le code source avec son éditeur préféré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="105"/>
        <source>Clear</source>
        <translation>Nettoyer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="220"/>
        <source>Upper case</source>
        <translation>Majuscule</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="223"/>
        <source>Lower case</source>
        <translation>Minuscule</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="228"/>
        <source>Ident right</source>
        <translation type="unfinished">Ident droit</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="231"/>
        <source>Ident left</source>
        <translation type="unfinished">Ident gauche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="333"/>
        <source>SQL file (*.sql);;All files (*.*)</source>
        <translation>Fichier SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="335"/>
        <source>Load file</source>
        <translation>Charger un fichier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="385"/>
        <source>The source editor `%1&apos; is running on `pid: %2&apos;.</source>
        <translation>L&apos;éditeur de source `%1&apos; s&apos;exécute sur pid : `%2&apos;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="418"/>
        <source>Could not start the source code editor application `%1&apos;! Make to sure that the source editor path defined in the general settings points to a valid executable and the current user has permission to run the application. Error message returned: `%2&apos;</source>
        <translation>Impossible de démarrer l&apos;application de l&apos;éditeur de code source `%1&apos; ! Assurez-vous que le chemin de l&apos;éditeur source défini dans les paramètres généraux pointe vers un exécutable valide et que l&apos;utilisateur actuel a l&apos;autorisation d&apos;exécuter l&apos;application. Message d&apos;erreur renvoyé : `%2&apos;</translation>
    </message>
</context>
<context>
    <name>ObjectDepsRefsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="49"/>
        <source>Dependencies</source>
        <translation>Dépendances</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="140"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="293"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="152"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="305"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="164"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="317"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="176"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="329"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="202"/>
        <source>References</source>
        <translation>Références</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="128"/>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="281"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="191"/>
        <source>Exclude indirect dependencies</source>
        <translation>Exclure les dépendances indirectes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="344"/>
        <source>Include indirect references</source>
        <translation>Inclure les références indirectes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="425"/>
        <source>This object does not exists anymore. The dependencies and references listing are disabled.</source>
        <translation>Cet objet n&apos;existe plus. L&apos;affichage de ses dépendances et références est désactivé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="14"/>
        <source>Object&apos;s dependencies &amp; references</source>
        <translation>Dépendances et références d&apos;un objet</translation>
    </message>
</context>
<context>
    <name>ObjectFinderWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="126"/>
        <source>Pattern:</source>
        <translation>Motif :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="145"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="50"/>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="148"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="640"/>
        <source>Find</source>
        <translation>Chercher</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="91"/>
        <source>Clears the search results</source>
        <translation>Effacer les résultats</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="94"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="234"/>
        <source>(Un)selects the graphical objects in the results grid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="315"/>
        <source>Regular Expression</source>
        <translation>Expression régulières</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="328"/>
        <source>Exact Match</source>
        <translation>Correspondance exacte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="383"/>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="411"/>
        <source>Clear All</source>
        <translation>Tout effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="482"/>
        <source>Case Sensitive</source>
        <translation>Sensible à la casse</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="576"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="588"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="600"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="612"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="675"/>
        <source>Fades outs all the graphical objects in the results grid (or those not listed). The current fade in/out state of all objects is modified.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="193"/>
        <source>Defines the search filter</source>
        <translation>Défini le filtre de recherche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="196"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
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
        <translation>&lt;strong&gt;%1&lt;/strong&gt; objet(s) trouvé(s).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="245"/>
        <source>No objects found.</source>
        <translation>Aucun objet trouvé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="564"/>
        <source>ID</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="237"/>
        <source>Select</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="678"/>
        <source>Fade out</source>
        <translation>Estomper</translation>
    </message>
</context>
<context>
    <name>ObjectRenameWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="158"/>
        <source>....</source>
        <translation>....</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="176"/>
        <source>to:</source>
        <translation>en :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="194"/>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="243"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="269"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
</context>
<context>
    <name>ObjectSelectorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="35"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="74"/>
        <source>Clear field</source>
        <translation>Effacer le champ</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="109"/>
        <source>Select Object</source>
        <translation>Sélectionner un objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectselectorwidget.cpp" line="189"/>
        <source>Select %1</source>
        <translation>Sélectionner %1</translation>
    </message>
</context>
<context>
    <name>ObjectsTableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="58"/>
        <source>Add Item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="74"/>
        <source>Ins</source>
        <translation>Insérer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="96"/>
        <source>Remove Item</source>
        <translation>Supprimer un élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="112"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="134"/>
        <source>Update Item</source>
        <translation>Actualiser l&apos;élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="150"/>
        <source>Alt+R</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="172"/>
        <source>Remove All</source>
        <translation>Tout supprimer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="188"/>
        <source>Shift+Del</source>
        <translation>Maj+Suppr.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="210"/>
        <source>Duplicate item</source>
        <translation>Dupliquer l&apos;élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="226"/>
        <source>Ctrl+D</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="248"/>
        <source>Edit Item</source>
        <translation>Modifier l&apos;élément</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="264"/>
        <source>Space</source>
        <translation>Espace</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="286"/>
        <source>Move Up</source>
        <translation>Déplacer vers le haut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="302"/>
        <source>Ctrl+Up</source>
        <translation>Ctrl+Haut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="324"/>
        <source>Move Down</source>
        <translation>Déplacer vers le bas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="340"/>
        <source>Ctrl+Down</source>
        <translation>Ctrl+Bas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="362"/>
        <source>Move to start</source>
        <translation>Déplacer au début</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="378"/>
        <source>Ctrl+Home</source>
        <translation>Ctrl+début</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="400"/>
        <source>Move to end</source>
        <translation>Déplacer à la fin</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="416"/>
        <source>Ctrl+End, Ctrl+S</source>
        <translation>Ctrl+Fin, Ctrl+S</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="388"/>
        <source>Do you really want to remove the selected item?</source>
        <translation>Souhaitez-vous réellement supprimer l&apos;élément sélectionné ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="388"/>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="437"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="437"/>
        <source>Do you really want to remove all the items?</source>
        <translation>Souhaitez-vous réellement supprimer tous les éléments ?</translation>
    </message>
</context>
<context>
    <name>OperationList</name>
    <message>
        <location filename="../libpgmodeler/src/operationlist.cpp" line="520"/>
        <source>(invalid object)</source>
        <translation>(objet invalide)</translation>
    </message>
</context>
<context>
    <name>OperationListWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="35"/>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="92"/>
        <source>Executed Operations</source>
        <translation>Opérations exécutées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="124"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
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
        <translation>Opérations :</translation>
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
        <translation>Position :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="318"/>
        <source>Delete operation history</source>
        <translation>Supprimer l&apos;historique des opérations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="344"/>
        <source>Undo</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="370"/>
        <source>Redo</source>
        <translation>Rétablir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="104"/>
        <source>Object: %1</source>
        <translation>Objet : %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="109"/>
        <source>Name: %1</source>
        <translation>Nom : %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="114"/>
        <source>created</source>
        <translation>créé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="119"/>
        <source>removed</source>
        <translation>supprimé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="124"/>
        <source>modified</source>
        <translation>modifié</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="129"/>
        <source>moved</source>
        <translation>déplacé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="135"/>
        <source>Operation: %1</source>
        <translation>Opération : %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="207"/>
        <source>Operation history exclusion</source>
        <translation>Exclure l&apos;historique des opérations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="208"/>
        <source>Delete the executed operations history is an irreversible action, do you want to continue?</source>
        <translation>Effacer l&apos;historique des opérations exécutées est une action irréversible, souhaitez-vous continuer ?</translation>
    </message>
</context>
<context>
    <name>OperatorClassWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="54"/>
        <source>Default Class:</source>
        <translation>Classe par défaut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="79"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="102"/>
        <source>Elements</source>
        <translation>Éléments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="123"/>
        <source>Element Type:</source>
        <translation>Type d&apos;élément :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="137"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="142"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="147"/>
        <source>Storage</source>
        <translation>Stockage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="168"/>
        <source>Function:</source>
        <translation>Fonction :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="175"/>
        <source>Operator:</source>
        <translation>Opérateur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="182"/>
        <source>Support/Strategy:</source>
        <translation>Support/Stratégie :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="196"/>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="218"/>
        <source>Op. Family:</source>
        <translation>Famille d&apos;opérateurs :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="38"/>
        <source>Storage Type</source>
        <translation>Type de Stockage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="42"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="45"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="48"/>
        <source>Support/Strategy</source>
        <translation>Support/Stratégie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="49"/>
        <source>Operator Family</source>
        <translation>Famille d&apos;opérateurs</translation>
    </message>
</context>
<context>
    <name>OperatorFamilyWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorfamilywidget.ui" line="50"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
</context>
<context>
    <name>OperatorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="140"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
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
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="36"/>
        <source>Arguments</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="41"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="69"/>
        <source>Restrict:</source>
        <translation>Restriction :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="76"/>
        <source>Commutator:</source>
        <translation>Commutateur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="83"/>
        <source>Negator:</source>
        <translation>Négateur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="120"/>
        <source>Operator Func.:</source>
        <translation>Fonction de l&apos;opérateur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="62"/>
        <source>Join:</source>
        <translation>Jointure :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="34"/>
        <source>Right Argument Type</source>
        <translation type="unfinished">Type argument droit</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="32"/>
        <source>Left Argument Type</source>
        <translation type="unfinished">Type d&apos;argument gauche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="44"/>
        <source>To create a unary operator it is necessary to specify as &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; one of its arguments. Additionally, the function that defines the operator must have only one parameter and this, in turn, must have the same data type of the the argument of unary operator.</source>
        <translation>Pour créer un opérateur unitaire, il est nécessaire de spécifier &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; comme l&apos;un de ses arguments. De plus, la fonction qui définit l&apos;opérateur doit avoir seulement un paramètre et celui-ci doit avoir le même type de donnée que l&apos;argument de l&apos;opérateur unitaire.</translation>
    </message>
</context>
<context>
    <name>ParameterWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="41"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="70"/>
        <source>Mode:</source>
        <translation>Mode :</translation>
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
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="152"/>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="66"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="90"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="51"/>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="395"/>
        <source>Permissions</source>
        <translation>Permissions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="283"/>
        <source>Add Permission</source>
        <translation>Ajouter permission</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="312"/>
        <source>Update Permission</source>
        <translation>Actualiser permission</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="341"/>
        <source>Cancel Operation</source>
        <translation>Annuler l&apos;opération</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="175"/>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="68"/>
        <source>Privileges</source>
        <translation>Privilèges</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="121"/>
        <source>Disable SQL code</source>
        <translation>Désactiver le code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="219"/>
        <source>Cascade</source>
        <translation>Cascade</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="242"/>
        <source>Privilege</source>
        <translation>Privilège</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="247"/>
        <source>GRANT OPTION</source>
        <translation>GRANT OPTION</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="404"/>
        <source>Code Preview</source>
        <translation>Prévisualisation du code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="52"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="64"/>
        <source>Id</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="91"/>
        <source>Leave the &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; grid empty in order to create a %1 applicable to &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</source>
        <translation>Laissez la grille &lt;em&gt;&lt;strong&gt;Rôles&lt;/strong&gt;&lt;/em&gt; vide afin de créer un %1 applicable à &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="526"/>
        <source>-- No permissions defined for the specified object!</source>
        <translation>-- Aucune permission définie pour cet objet !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="534"/>
        <source>/* Could not generate the SQL code preview for permissions!</source>
        <translation>/* Impossible de générer la prévisualisation du code SQL pour les permissions !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="26"/>
        <source>Edit permissions</source>
        <translation>Modifier les permissions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="199"/>
        <source>&amp;Grant</source>
        <translation>Acco&amp;rder</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="209"/>
        <source>Re&amp;voke</source>
        <translation>Ré&amp;voquer</translation>
    </message>
</context>
<context>
    <name>PgModelerCLI</name>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="125"/>
        <source>Unrecognized option &apos;%1&apos;.</source>
        <translation>Option &apos;%1&apos; non reconnue.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="129"/>
        <source>Value not specified for option &apos;%1&apos;.</source>
        <translation>Valeur pour l&apos;option &apos;%1&apos; manquante.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="131"/>
        <source>Option &apos;%1&apos; does not accept values.</source>
        <translation>L&apos;option &apos;%1&apos; n&apos;accepte pas de valeur.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="373"/>
        <source>Usage: pgmodeler-cli [OPTIONS]</source>
        <translation>Utilisation : pgmodeler-cli [OPTIONS]</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="369"/>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="580"/>
        <source> command line interface.</source>
        <translation> interface en ligne de commande.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="376"/>
        <source>General options: </source>
        <translation>Options générales : </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="407"/>
        <source>DBMS export options: </source>
        <translation>Options d&apos;export directe dans un serveur : </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="533"/>
        <source>Input file must be different from output!</source>
        <translation>Le fichier source doit être différent du fichier de sortie !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="537"/>
        <source>Incomplete connection information!</source>
        <translation>Les informations de connexion sont incomplètes !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1191"/>
        <source>Starting model export...</source>
        <translation>Début de l&apos;export du modèle...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="439"/>
        <source>Miscellaneous options: </source>
        <translation>Options diverses : </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="476"/>
        <source>There are no connections configured.</source>
        <translation>Il n&apos;y a aucune connexion configurée.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="540"/>
        <source>Invalid zoom specified!</source>
        <translation>Zoom spécifié invalide !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="543"/>
        <source>Invalid action specified to update mime option!</source>
        <translation>Action spécifiée invalide pour mettre à jour l&apos;option mime !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1177"/>
        <source>Starting model fixing...</source>
        <translation>Lancement de la fixation du modèle...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1450"/>
        <source>Starting mime update...</source>
        <translation>Lancement de la mise à jour du mime...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1186"/>
        <source>Model successfully fixed!</source>
        <translation>Modèle fixé avec succès !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="692"/>
        <source>Extracting objects&apos; XML...</source>
        <translation>Extraction des objets XML...</translation>
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
        <translation type="unfinished">  %1, %2=[CODES] Ignore les erreurs supplémentaires par leurs codes. Une liste de codes alphanumériques séparés par des virgules doit être fournie. {1,?} {2 ?}</translation>
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
        <location filename="../main-cli/src/pgmodelercli.cpp" line="708"/>
        <source>Invalid input file! It seems that is not a pgModeler generated model or the file is corrupted!</source>
        <translation>Fichier d&apos;entrée invalide ! Il semble que ce ne soit pas un modèle généré par pgModeler ou que le fichier soit corrompu !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="830"/>
        <source>Recreating objects...</source>
        <translation>Recréation des objets...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="950"/>
        <source>
** Object(s) that couldn&apos;t fixed: </source>
        <translation>
** Ces objets ne peuvent être corrigés : </translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="961"/>
        <source>WARNING: There are objects that maybe can&apos;t be fixed. Trying again... (tries %1/%2)</source>
        <translation>AVERTISSEMENT : Certains objets ne peuvent peut-être pas être corrigés. Nouvel essai... (%1/%2)</translation>
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
        <translation>Les fichiers de modèle de bases de données (.dbm) sont déjà associés à pgModeler !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1517"/>
        <source>There is no file association related to pgModeler and .dbm files!</source>
        <translation>Il n&apos;y a pas d&apos;association entre pgModeler et .les fichiers dbm !</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1519"/>
        <source>Mime database operation: %1</source>
        <translation>Opération de base de données mime : %1</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1578"/>
        <source>Can&apos;t erase the file %1! Check if the current user has permissions to delete it and if the file exists.</source>
        <translation>Impossible d&apos;effacer le fichier %1 ! Vérifiez si l&apos;utilisateur actuel dispose des autorisations pour le supprimer et si le fichier existe.</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="1653"/>
        <source>Running update-mime-database command...</source>
        <translation>Exécution de la commande update-mime-database...</translation>
    </message>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="401"/>
        <source>PNG and SVG export options: </source>
        <translation>Options d&apos;export en PNG et SVG : </translation>
    </message>
</context>
<context>
    <name>PgModelerPlugin</name>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="32"/>
        <source>Plugin Information</source>
        <translation>Information d&apos;extension</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="86"/>
        <source>Version: %1</source>
        <translation>Version : %1</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="87"/>
        <source>Author: %1</source>
        <translation>Auteur : %1</translation>
    </message>
</context>
<context>
    <name>PgModelerUiNS</name>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodeleruins.cpp" line="98"/>
        <source>Do you want to apply the &lt;strong&gt;SQL %1 status&lt;/strong&gt; to the object&apos;s references too? This will avoid problems when exporting or validating the model.</source>
        <translation>Souhaitez-vous appliquer &lt;strong&gt;statut SQL %1 &lt;/strong&gt; aux références de l&apos;objet également ? Cela permet d&apos;éviter des problèmes lors de l&apos;export ou de la validation du modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodeleruins.cpp" line="98"/>
        <source>disabling</source>
        <translation>désactivation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodeleruins.cpp" line="98"/>
        <source>enabling</source>
        <translation>activation</translation>
    </message>
</context>
<context>
    <name>PgSQLTypeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="32"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="68"/>
        <source>Data Type</source>
        <translation>Type de donnée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="143"/>
        <source>SRID:</source>
        <translation>SRID :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="189"/>
        <source>Variation:</source>
        <translation>Variation :</translation>
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
        <translation>Précision</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="287"/>
        <source>Spatial:</source>
        <translation>Spatial :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="312"/>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="609"/>
        <source>Dimension</source>
        <translation>Dimension</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="349"/>
        <source>Format:</source>
        <translation>Format :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="381"/>
        <source>Timezone:</source>
        <translation>Fuseau horaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="434"/>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="470"/>
        <source>P:</source>
        <translation>P :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="495"/>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="537"/>
        <source>Length</source>
        <translation>Longueur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="543"/>
        <source>L:</source>
        <translation>L :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="579"/>
        <source>Interval:</source>
        <translation>Intervalle :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="615"/>
        <source>[ ]:</source>
        <translation>[ ] :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgsqltypewidget.cpp" line="47"/>
        <source>NONE</source>
        <translation>AUCUN</translation>
    </message>
</context>
<context>
    <name>PluginsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="106"/>
        <source>Plug-ins root directory:</source>
        <translation>Répertoire racine des extensions :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="80"/>
        <source>Open in file manager</source>
        <translation>Ouvrir dans le gestionnaire de fichier</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="121"/>
        <source>Loaded plug-ins</source>
        <translation>Extensions chargées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="32"/>
        <source>Plugin</source>
        <translation>Extension</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="34"/>
        <source>Version</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="35"/>
        <source>Library</source>
        <translation>Bibliothèque</translation>
    </message>
</context>
<context>
    <name>PolicyWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="38"/>
        <source>Basics</source>
        <translation>Notions de base</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="56"/>
        <source>Command:</source>
        <translation>Commande :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="63"/>
        <source>Permissive</source>
        <translation>Permissif</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="112"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="117"/>
        <source>Expressions</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="135"/>
        <source>USING:</source>
        <translation>UTILISANT :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="152"/>
        <source>CHECK:</source>
        <translation>VÉRIFIER :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/policywidget.cpp" line="43"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/policywidget.cpp" line="49"/>
        <source>Leave the &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; grid empty in order to create a %1 applicable to &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</source>
        <translation>Laissez la grille &lt;em&gt;&lt;strong&gt;Rôles&lt;/strong&gt;&lt;/em&gt; vide afin de créer un %1 applicable à &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="34"/>
        <source>new_database</source>
        <translation>nouvelle_base_de_données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3016"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3075"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="3302"/>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="9801"/>
        <source>%1 (line: %2)</source>
        <translation>%1 (ligne : %2)</translation>
    </message>
</context>
<context>
    <name>Relationship</name>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="85"/>
        <source>%1_has_one_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="87"/>
        <source>%1_has_many_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="89"/>
        <source>many_%1_has_many_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="91"/>
        <source>%1_inherits_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="93"/>
        <source>%1_copies_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RelationshipConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="304"/>
        <source>Connect FK to PK columns</source>
        <translation>Relier les colonnes associant clés étrangères/clés primaires</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="406"/>
        <source>Connect tables&apos; center points</source>
        <translation>Relier les associations aux tables en un point central</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="539"/>
        <source>Foreign key settings</source>
        <translation>Clés étrangères</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="587"/>
        <source>Deferral:</source>
        <translation>Report :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="612"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="672"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="685"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="706"/>
        <source>Name patterns</source>
        <translation>Règles de nommage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="737"/>
        <source>Relationship type:</source>
        <translation>Type d&apos;association :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="796"/>
        <source>Foreign Key (Target):</source>
        <translation>Clé étrangère (Cible) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="806"/>
        <source>Unique Key Name:</source>
        <translation>Nom contrainte unique :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="816"/>
        <source>Primary Key Name:</source>
        <translation>Nom contrainte clé primaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="826"/>
        <source>Primary Key Column:</source>
        <translation>Nom colonne clé primaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="747"/>
        <source>Column (Target):</source>
        <translation>Colonne (Cible) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="730"/>
        <source>Foreign Key (Source):</source>
        <translation>Clé étrangère (Source) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="786"/>
        <source>Column (Source):</source>
        <translation>Colonne (Source) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="755"/>
        <source>One to one (1:1)</source>
        <translation>Un à un (1 :1)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="45"/>
        <source>Connection Mode</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="515"/>
        <source>FK Settings &amp;&amp; Patterns</source>
        <translation>Clés étrangères &amp;&amp; nommage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="760"/>
        <source>One to many (1:n)</source>
        <translation>Un à plusieurs (1 :n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="765"/>
        <source>Many to many (n:n)</source>
        <translation>Plusieurs à plusieurs (n :n)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="770"/>
        <source>Generalization</source>
        <translation>Généralisation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="775"/>
        <source>Copy</source>
        <translation>Copie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipconfigwidget.cpp" line="63"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="727"/>
        <source>Pattern for foreign key generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="744"/>
        <source>Pattern for columns generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="783"/>
        <source>Pattern for columns generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="793"/>
        <source>Pattern for foreign key generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur une clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="803"/>
        <source>Pattern for unique key generated by the relationship.</source>
        <translation>Motif pour les noms de clés uniques qui sont générées par une association.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="813"/>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="823"/>
        <source>Pattern for primary key generated by identifier relationship.</source>
        <translation>Motif pour les noms de clés primaires qui sont générées par une association.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="91"/>
        <source>This mode renders the relationships in crow&apos;s foot notation which has a better semantics and readability. It also determines the optimal point where the relationship is connected on the tables&apos; edges taking their position into account.</source>
        <translation>Ce mode affiche les associations en pied d&apos;oiseau ce qui améliore le sémantique et la lisibilité. Il détermine également le point optimal où la relation est connectée sur les bords des tables en tenant compte de leur position.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="94"/>
        <source>Crow&apos;s foot notation</source>
        <translation>Affichage en pied d&apos;oiseau</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="196"/>
        <source>This mode determines the optimal point where the relationship is connected on the tables&apos; edges taking their position into account. It implies the usage of the classical ER notation.</source>
        <translation>Ce mode détermine le point optimal où la relation est connectée sur les bords des tables en tenant compte de leur position. Cela implique l&apos;utilisation de la notation ER classique.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="199"/>
        <source>Connect tables&apos; edges</source>
        <translation>Connecter les bords des tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="301"/>
        <source>This mode is available only for &lt;strong&gt;one-to-one&lt;/strong&gt;, &lt;strong&gt;one-to-many&lt;/strong&gt; and &lt;strong&gt;fk relationships&lt;/strong&gt; but provides a better semantics when linking tables by placing the lines on the exact point where the relationship occurs. It implies the usage of the classical ER notation.</source>
        <translation>Ce mode d&apos;affichage est disponible uniquement pour les associations de type &lt;strong&gt;un-à-un&lt;/strong&gt;, &lt;strong&gt;un-à-plusieurs&lt;/strong&gt; et &lt;strong&gt;les associations aux clés étrangères&lt;/strong&gt; mais fournit une meilleure sémantique lors de l&apos;association de tables en plaçant les lignes sur le point exact où l&apos;association se produit. Cela implique l&apos;utilisation de la notation ER classique.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="403"/>
        <source>This mode is the classical one. It connects the relationship to tables through their central points. It implies the usage of the classical ER notation.</source>
        <translation>Ce mode est le classique. Il relie les associations de tables depuis leurs points centraux. Cela implique l&apos;utilisation de la notation ER classique.</translation>
    </message>
</context>
<context>
    <name>RelationshipWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="60"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="535"/>
        <source>One to one relationship</source>
        <translation>Association un à un</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="571"/>
        <source>One to many relationship</source>
        <translation>Association un à plusieurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="574"/>
        <source>1-n</source>
        <translation>1-n</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="607"/>
        <source>Many to many relationship</source>
        <translation>Association plusieurs à plusieurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="610"/>
        <source>n-n</source>
        <translation>n-n</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="643"/>
        <source>Generalization relationship (inheritance)</source>
        <translation>Association de généralisation (héritage)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="679"/>
        <source>Dependency / Copy relationship</source>
        <translation>Relation de dépendance / copie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="682"/>
        <source>dep</source>
        <translation>dep</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="709"/>
        <source>Relationship generated via foreign key</source>
        <translation>Relation générée par une clé étrangère</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="712"/>
        <source>fk</source>
        <translation>fk</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="762"/>
        <source>Identifier</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="215"/>
        <source>Rel. Type:</source>
        <translation>Type de relation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="99"/>
        <source>Table 1:</source>
        <translation>Table n°1 :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="262"/>
        <source>Table 2:</source>
        <translation>Table n°2 :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="305"/>
        <source>Foreign key Settings</source>
        <translation>Propriétés par défaut des clés étrangères</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="335"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1140"/>
        <source>Use the values defined on settings dialogs for the fields below</source>
        <translation>Utiliser les paramètres de pgModeler pour les champs ci-dessous</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="338"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1143"/>
        <source>Use global settings for these fields</source>
        <translation>Utiliser les paramètres globaux de pgModeler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="481"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="491"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="538"/>
        <source>&amp;1-1</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="646"/>
        <source>&amp;gen</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1119"/>
        <source>Name Patterns</source>
        <translation>Règles de nommage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1174"/>
        <source>Column (Source):</source>
        <translation>Colonne (Source) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1184"/>
        <source>Column (Target):</source>
        <translation>Colonne (Cible) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1194"/>
        <source>Foreign Key (Source):</source>
        <translation>Clé étrangère (Source) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1204"/>
        <source>Foreign Key (Target):</source>
        <translation>Clé étrangère (Cible) :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="146"/>
        <source>Cardinality:</source>
        <translation>Cardinalité :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="277"/>
        <source>[SRC] is required</source>
        <translation>[SRC] est requis</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="290"/>
        <source>[DST] is required</source>
        <translation>[DST] est requis</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="171"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1013"/>
        <source>Name of the table generated from many to many relationship</source>
        <translation>Nom de la table générée par une relation plusieurs à plusieurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="393"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="446"/>
        <source>Deferral:</source>
        <translation>Report :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="871"/>
        <source>Copy Options</source>
        <translation>Options de copie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="892"/>
        <source>INDEXES</source>
        <translation>INDEX</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="902"/>
        <source>COMMENTS</source>
        <translation>COMMENTAIRES</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="909"/>
        <source>INCLUDING</source>
        <translation>INCLURE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="919"/>
        <source>DEFAULTS</source>
        <translation>DEFAUTS</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="942"/>
        <source>CONSTRAINTS</source>
        <translation>CONTRAINTES</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="955"/>
        <source>Use defaults</source>
        <translation>Utiliser les valeurs par défauts</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="968"/>
        <source>ALL</source>
        <translation>TOUT</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="978"/>
        <source>STORAGE</source>
        <translation>STOCKAGE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="174"/>
        <source>Gen. Table Name:</source>
        <translation>Nom nouvelle table :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1444"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1449"/>
        <source>Constraints</source>
        <translation>Contraintes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1454"/>
        <source>Primary key</source>
        <translation>Clé primaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1498"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="79"/>
        <source>Attribute</source>
        <translation>Attribut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="81"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="87"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="93"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="85"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="91"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="119"/>
        <source>This advanced tab shows the objects (columns or table) auto created by the relationship&apos;s connection as well the foreign keys that represents the link between the participant tables.</source>
        <translation>Cet onglet avancé montre les objets (colonnes ou tables) générés automatiquement par la relation ainsi que les clés étrangères qui forment le(s) lien(s) entre les tables.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="301"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="302"/>
        <source> is required</source>
        <translation> est requis</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="280"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="291"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="293"/>
        <source>Reference Table:</source>
        <translation>Table référente :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="147"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="283"/>
        <source>Receiver Table:</source>
        <translation>Table de destination :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1171"/>
        <source>Pattern for columns generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1181"/>
        <source>Pattern for columns generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1191"/>
        <source>Pattern for foreign key generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1201"/>
        <source>Pattern for foreign key generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur une clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1211"/>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1231"/>
        <source>Pattern for primary key generated by identifier relationship.</source>
        <translation>Motif pour les noms de clés primaires qui sont générées par une association.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1214"/>
        <source>Primary Key Name:</source>
        <translation>Nom de clé primaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1221"/>
        <source>Pattern for unique key generated by the relationship.</source>
        <translation>Motif pour les noms de clés uniques qui sont générées par une association.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1224"/>
        <source>Unique Key Name:</source>
        <translation>Nom clé unique :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="1234"/>
        <source>Primay Key Column:</source>
        <translation>Colonne de clé primaire :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="759"/>
        <source>The receiver&apos;s primary key will be composed by the generated foreign key columns.</source>
        <translation>La clé primaire du destinataire sera composée des colonnes de clé étrangère générées.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="821"/>
        <source>Instead of create a multi-valued primary key with the generated foreign keys columns a single column is created and used as primary key.</source>
        <translation>Au lieu de créer une clé primaire à valeurs multiples avec les colonnes de clés étrangères générées, une seule colonne est créée et utilisée comme clé primaire.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="824"/>
        <source>Single PK column</source>
        <translation>Colonne clé primaire simple</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="858"/>
        <source>Custom Color:</source>
        <translation>Couleur personnalisée :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="109"/>
        <source>Use the special primary key if you want to include a primary key containing generated columns to the receiver table. &lt;strong&gt;Important:&lt;/strong&gt; if this is a new relationship there is a need to finish its creation and reopen this dialog to create the special primary key.</source>
        <translation>Utilisez la clé primaire spéciale si vous souhaitez inclure une clé primaire contenant des colonnes générées dans la table de destination. &lt;strong&gt;Important :&lt;/strong&gt; s&apos;il s&apos;agit d&apos;une nouvelle relation, il est nécessaire de terminer sa création et de réouvrir cette boîte de dialogue pour créer la clé primaire spéciale.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="260"/>
        <source>Referer View:</source>
        <translation>Vue référente :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="261"/>
        <source>Referer view references one or more columns of a table to construct it&apos;s own columns.</source>
        <translation>La vue référente référence une ou plusieurs colonnes d&apos;une table pour construire ses propres colonnes.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="262"/>
        <source>Referenced table has its columns referenced by a view in order to construct the columns of this latter.</source>
        <translation>La table référencée a ses colonnes référencées par une vue afin de construire les colonnes de cette dernière.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="266"/>
        <source>Referer Table:</source>
        <translation>Table référente :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="267"/>
        <source>Referer table references one or more columns of a table through foreign keys. This is the (n) side of relationship.</source>
        <translation>La table référente référence une ou plusieurs colonnes d&apos;une table via des clés étrangères. C&apos;est le côté (n) de la relation.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="268"/>
        <source>Referenced table has its columns referenced by a table&apos;s foreign key. This is the (1) side of relationship.</source>
        <translation>La table référencée a ses colonnes référencées par la clé étrangère d&apos;une table. C&apos;est le côté (1) de la relation.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="271"/>
        <source>Referenced Table:</source>
        <translation>Table référencée :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="281"/>
        <source>Reference table has the columns from its primary key will copied to the receiver table in order to represent the linking between them. This is the (1) side of relationship.</source>
        <translation>La table de référence a les colonnes de sa clé primaire copiées dans la table de réception afin de représenter la liaison entre elles. C&apos;est le côté (1) de la relation.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="284"/>
        <source>Receiver (or referer) table will receive the generated columns and the foreign key in order to represent the linking between them. This is the (n) side of relationship.</source>
        <translation>La table référente recevra les colonnes générées et la clé étrangère afin de représenter la liaison entre elles. C&apos;est le côté (n) de la relation.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="292"/>
        <source>In many-to-many relationships both tables are used as reference to generate the table that represents the linking. Columns from both tables are copied to the resultant table and two foreign keys are created as well in order to reference each participant table.</source>
        <translation>Dans les relations plusieurs-à-plusieurs, les deux tables sont utilisées comme référence pour générer la table qui représente la liaison. Les colonnes des deux tables sont copiées dans la table résultante et deux clés étrangères sont également créées afin de référencer chaque table participante.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="932"/>
        <source>E&amp;XCLUDING</source>
        <translation>E&amp;XCLUSION</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="134"/>
        <source>Available tokens to define name patterns:&lt;br/&gt;					&lt;strong&gt;%1&lt;/strong&gt; = Reference (source) primary key column name. &lt;em&gt;(Ignored on constraint patterns)&lt;/em&gt;&lt;br/&gt; 					&lt;strong&gt;%2&lt;/strong&gt; = Reference (source) table name.&lt;br/&gt; 					&lt;strong&gt;%3&lt;/strong&gt; = Receiver (destination) table name.&lt;br/&gt; 					&lt;strong&gt;%4&lt;/strong&gt; = Generated table name. &lt;em&gt;(Only for n:n relationships)&lt;/em&gt;</source>
        <translation>Formes disponibles pour définir les modèles de noms : &lt;br/&gt;					&lt;strong&gt;%1&lt;/strong&gt; = Nom de la colonne de clé primaire de référence (source). &lt;em&gt;(Ignoré sur les modèles de contrainte)&lt;/em&gt;&lt;br/&gt; 					&lt;strong&gt;%2&lt;/strong&gt; = Nom de la table de référence (source).&lt;br/&gt; 					&lt;strong&gt;%3&lt;/strong&gt; = Nom de la table référencée (ou de déstination). &lt;br/&gt; 				&lt;strong&gt;%4&lt;/strong&gt; = Nom de la table générée. &lt;em&gt;(Seulement pour les relations n:n)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>ResultSetModel</name>
    <message>
        <location filename="../libpgmodeler_ui/src/resultsetmodel.cpp" line="51"/>
        <source>[binary data]</source>
        <translation type="unfinished">[données binaires]</translation>
    </message>
</context>
<context>
    <name>RoleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="204"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="102"/>
        <source>Connections:</source>
        <translation>Connexions :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="109"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="130"/>
        <source>Superuser</source>
        <translation>Super utilisateur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="137"/>
        <source>Inherit permissions</source>
        <translation>Hériter des permissions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="144"/>
        <source>Can create database</source>
        <translation>Peut créer des bases de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="158"/>
        <source>Can use replication</source>
        <translation>Peut utiliser la réplication</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="165"/>
        <source>Can login</source>
        <translation>Peut se loguer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="227"/>
        <source>Encrypted</source>
        <translation>Encrypté</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="186"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="68"/>
        <source>Members</source>
        <translation>Membres</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="191"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="65"/>
        <source>Member of</source>
        <translation>Membre de</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="196"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="71"/>
        <source>Members (Admin.)</source>
        <translation>Membres (Admin.)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="59"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="88"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="62"/>
        <source>Validity</source>
        <translation>Validité</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="50"/>
        <source>yyyy-MMM-dd hh:mm:ss</source>
        <translation>aaaa-MMM-jj hh :mm :ss</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="35"/>
        <source>Assigning &lt;strong&gt;&lt;em&gt;-1&lt;/em&gt;&lt;/strong&gt; to &lt;strong&gt;&lt;em&gt;Connections&lt;/em&gt;&lt;/strong&gt; creates a role without connection limit.&lt;br/&gt;										  Unchecking &lt;strong&gt;&lt;em&gt;Validity&lt;/em&gt;&lt;/strong&gt; creates an role that never expires.</source>
        <translation>L&apos;affectation de &lt;strong&gt;&lt;em&gt;-1&lt;/em&gt;&lt;/strong&gt; à &lt;strong&gt;&lt;em&gt;Connexions&lt;/em&gt;&lt;/strong&gt; crée un rôle sans limite de connexion.&lt;br/&gt;										Décocher &lt;strong&gt;&lt;em&gt;Validité&lt;/em&gt;&lt;/strong&gt; crée un rôle qui n&apos;expire jamais.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="151"/>
        <source>Bypass RLS</source>
        <translation>Contournement RLS</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="172"/>
        <source>Can create role</source>
        <translation>Peut créer des rôles</translation>
    </message>
</context>
<context>
    <name>RuleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="50"/>
        <source>Event:</source>
        <translation>Évènement :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="102"/>
        <source>Execution Type:</source>
        <translation>Type d&apos;exécution :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="151"/>
        <source>Conditional Expr.:</source>
        <translation>Expr. conditionnelle :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="158"/>
        <source>Commands</source>
        <translation>Commandes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="191"/>
        <source>SQL Command:</source>
        <translation>Commandes SQL :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rulewidget.cpp" line="38"/>
        <source>SQL command</source>
        <translation>Commande SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rulewidget.cpp" line="42"/>
        <source>To create a rule that does not perform any action (&lt;strong&gt;DO NOTHING&lt;/strong&gt;) simply do not specify commands in the SQL commands table.</source>
        <translation>Pour créer une règle qui n&apos;éxécute aucune action (&lt;strong&gt;DO NOTHING&lt;/strong&gt;), ne spécifiez aucune commandes dans le champs des commandes SQL.</translation>
    </message>
</context>
<context>
    <name>SQLExecutionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="525"/>
        <source>Save SQL commands</source>
        <translation>Sauvegarder commandes SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="158"/>
        <source>Alt+F</source>
        <translation>Alt+F</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="233"/>
        <source>Run the specified SQL command</source>
        <translation>Exécuter la commande SQL entrée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="236"/>
        <source>Run SQL</source>
        <translation>Exécuter SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="249"/>
        <source>F6</source>
        <translation>F6</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="268"/>
        <source>Clear sql input field and results</source>
        <translation>Effacer le code SQL et la grille de résultat</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="271"/>
        <source>Clear All</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="300"/>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="344"/>
        <source>Export results to a CSV file</source>
        <translation>Exporter les résultats dans un fichier CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="303"/>
        <source>Snippe&amp;ts</source>
        <translation>Extrai&amp;ts</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="316"/>
        <source>Alt+T</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="347"/>
        <source>E&amp;xport</source>
        <translation>E&amp;xporter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="74"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="68"/>
        <source>SQL file (*.sql);;All files (*.*)</source>
        <translation>Fichier SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="89"/>
        <source>Load</source>
        <translation>Charger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="90"/>
        <source>Save</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="91"/>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="315"/>
        <source>[binary data]</source>
        <translation>[données binaires]</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="371"/>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="505"/>
        <source>Messages (%1)</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="498"/>
        <source>[%1]: SQL command successfully executed in &lt;em&gt;&lt;strong&gt;%2&lt;/strong&gt;&lt;/em&gt;. &lt;em&gt;%3 &lt;strong&gt;%4&lt;/strong&gt;&lt;/em&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="586"/>
        <source>Save CSV file</source>
        <translation>Sauvegarder fichier CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="587"/>
        <source>Comma-separated values file (*.csv);;All files (*.*)</source>
        <translation>Fichiers CSV avec champs séparés par des virgules (*.csv);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="623"/>
        <source>The SQL input field and the results grid will be cleared! Want to proceed?</source>
        <translation>Le champ d&apos;entrée SQL ainsi que la grille des résultats vont être purgés ! Voulez-vous poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="936"/>
        <source>Save history</source>
        <translation>Enregistrer l&apos;historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="937"/>
        <source>Reload history</source>
        <translation>Recharger l&apos;historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="942"/>
        <source>Find in history</source>
        <translation>Rechercher dans l&apos;historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="944"/>
        <source>Hide find tool</source>
        <translation>Masquer l&apos;outil de recherche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="959"/>
        <source>This action will wipe out all the SQL commands history for the current connection! Do you really want to proceed?</source>
        <translation>Cette action effacera tout l&apos;historique des commandes SQL pour la connexion en cours ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="554"/>
        <source>Load SQL commands</source>
        <translation>Charger commandes SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="718"/>
        <source>Copy selection</source>
        <translation>Copier la sélection</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="186"/>
        <source>Search in SQL code</source>
        <translation>Rechercher dans le code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="385"/>
        <source>Toggles the output pane</source>
        <translation>(Dés)active le volet de sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="388"/>
        <source>&amp;Output</source>
        <translation>S&amp;ortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="401"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="569"/>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="370"/>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="481"/>
        <source>Results</source>
        <translation>Résultats</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="646"/>
        <source>Messages</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="693"/>
        <source>History</source>
        <translation>Historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="362"/>
        <source>No results retrieved or changes done due to the error above.</source>
        <translation>Aucun résultat n&apos;a été récupéré ou des modifications ont été effectuées en raison de l&apos;erreur ci-dessus.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="476"/>
        <source>Results (%1)</source>
        <translation>Resultats (%1)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="501"/>
        <source>Rows affected</source>
        <translation>Lignes affectées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="501"/>
        <source>Rows retrieved</source>
        <translation>Lignes récupérées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="719"/>
        <source>Plain format</source>
        <translation>Format simple</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="720"/>
        <source>CVS format</source>
        <translation>Format CVS</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="888"/>
        <source>This action will wipe out all the SQL commands history for all connections! Do you really want to proceed?</source>
        <translation>Cette action effacera tout l&apos;historique des commandes SQL pour toutes les connexions ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="935"/>
        <source>Clear history</source>
        <translation>Nettoyer l&apos;historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="71"/>
        <source>Close the current SQL script</source>
        <translation>Fermer le script SQL actuel</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="100"/>
        <source>SQL script currently handled</source>
        <translation>Script SQL actuellement géré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="121"/>
        <source>(not saved)</source>
        <translation>(non enregistré)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="142"/>
        <source>Handle external SQL script</source>
        <translation>Gérer le script SQL externe</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="145"/>
        <source>&amp;Script</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="189"/>
        <source>Fi&amp;nd</source>
        <translation>&amp;Rechercher</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="360"/>
        <source>Alt+X</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="475"/>
        <source>Current working database</source>
        <translation>Base de données de travail actuelle</translation>
    </message>
</context>
<context>
    <name>SQLToolWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="87"/>
        <source>Database explorer</source>
        <translation>Explorateur de bases de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="387"/>
        <source>SQL execution</source>
        <translation>Exécuteur de code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="150"/>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="211"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqltoolwidget.cpp" line="187"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="147"/>
        <source>Disconnect from all databases</source>
        <translation>Se déconnecter de toutes les bases de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="208"/>
        <source>Update the database list</source>
        <translation>Mettre à jour la liste des bases de données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="282"/>
        <source>Toggle the object&apos;s attributes grid</source>
        <translation>(Dés)active la grille des attributs de l&apos;objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="288"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="301"/>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="323"/>
        <source>Toggle the display of source code pane</source>
        <translation>(Dés)active l&apos;affichage du volet du code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="329"/>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="434"/>
        <source>Source code</source>
        <translation>Code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqltoolwidget.cpp" line="188"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; Disconnect from all databases will close any opened tab in this view! Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Se déconnecter de toutes les bases de données fermera tout onglet ouvert dans cette vue ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
</context>
<context>
    <name>SceneInfoWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="64"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="104"/>
        <source>Current position of the mouse in the canvas</source>
        <translation>Position actuelle de la souris dans le canevas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="107"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="182"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="257"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="332"/>
        <source>-</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="139"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="179"/>
        <source>Current zoom factor</source>
        <translation>Facteur de zoom actuel</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="214"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="254"/>
        <source>Currently selected object(s)</source>
        <translation>Objet(s) actuellement sélectionné(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="289"/>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="329"/>
        <source>Dimensions of the selected object(s)</source>
        <translation>Dimensions de(s) l&apos;objet(s) sélectionné(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sceneinfowidget.cpp" line="42"/>
        <source>No selection</source>
        <translation>Aucune séléction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sceneinfowidget.cpp" line="43"/>
        <source>N/A</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sceneinfowidget.cpp" line="69"/>
        <source>Sel. objects: %1</source>
        <translation>Objets select. : %1</translation>
    </message>
</context>
<context>
    <name>SchemaWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/schemawidget.ui" line="48"/>
        <source>Show rectangle</source>
        <translation>Afficher le rectangle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/schemawidget.ui" line="29"/>
        <source>Fill color:</source>
        <translation>Couleur de remplissage :</translation>
    </message>
</context>
<context>
    <name>SequenceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="54"/>
        <source>Cyclic:</source>
        <translation>Cyclique :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="68"/>
        <source>Start:</source>
        <translation>Début :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="152"/>
        <source>Maximum:</source>
        <translation>Maximum :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="124"/>
        <source>Minimum:</source>
        <translation>Minimum :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="44"/>
        <source>Defualt values:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="96"/>
        <source>Increment:</source>
        <translation>Incrément :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="180"/>
        <source>Cache:</source>
        <translation>Cache :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="208"/>
        <source>Owner Col.:</source>
        <translation>Proprio. Col. :</translation>
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
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="180"/>
        <source>Create new connection</source>
        <translation>Créer une nouvelle connexion</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="209"/>
        <source>Cancel edition</source>
        <translation>Annuler l&apos;édition</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="238"/>
        <source>Edit selected connection</source>
        <translation>Modifier la connexion sélectionnée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="267"/>
        <source>Delete selected connection</source>
        <translation>Supprimer la connexion sélectionnée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="302"/>
        <source>Remove All</source>
        <translation>Tout supprimer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="318"/>
        <source>Shift+Del</source>
        <translation>Maj+Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="625"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="137"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="417"/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="455"/>
        <source>Update</source>
        <translation>Mettre à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="534"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="95"/>
        <source>Label:</source>
        <translation>Étiquette :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="124"/>
        <source>Applies to:</source>
        <translation>S&apos;applique à :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="515"/>
        <source>Parsable or dynamic snippets are written in the &lt;strong&gt;schema micro language&lt;/strong&gt; syntax. When using a parsable snippet the attributes surrounded in &lt;strong&gt;{}&lt;/strong&gt; will be replaced by the selected object&apos;s matching attributes.</source>
        <translation>Les extraits de code analysables ou dynamiques sont écrits dans la syntaxe &lt;strong&gt;schema micro language&lt;/strong&gt;. Lorsque vous utilisez un extrait de code analysé, les attributs entourés par des &lt;strong&gt;{}&lt;/strong&gt; seront remplacés par les attributs correspondants de l&apos;objet sélectionné.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="518"/>
        <source>Parsable</source>
        <translation>Analysable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="572"/>
        <source>When handling parsable snippets empty attributes will be replaced by a value in the format &lt;strong&gt;{attribute}&lt;/strong&gt;. Note that this option can affect the semantics of the resulting snippet.</source>
        <translation>Lors de la manipulation d&apos;extraits de code analysables, les attributs vides seront remplacés par une valeur au format &lt;strong&gt;{attribut}&lt;/strong&gt;. Noter que cette option peut affecter la sémantique de l&apos;extrait résultant.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="575"/>
        <source>Placeholders</source>
        <translation>Élément de substitution</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="376"/>
        <source>Parse the snippet in order to check if there are syntax errors.</source>
        <translation>Analyser l&apos;extrait de code afin de vérifier s&apos;il existe des erreurs de syntaxe.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="379"/>
        <source>Parse</source>
        <translation>Analyser</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="333"/>
        <source>Snippets:</source>
        <translation>Extraits :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="31"/>
        <source>General purpose</source>
        <translation>Usage général</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="53"/>
        <source>All snippets</source>
        <translation>Tous les extraits</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="204"/>
        <source>/* Error parsing the snippet &apos;%1&apos;:

 %2 */</source>
        <translation>/* Erreur lors de l&apos;analyse de l&apos;extrait &apos;%1&apos;:

 %2 */</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="226"/>
        <source>Duplicated snippet id &lt;strong&gt;%1&lt;/strong&gt; detected. Please, specify a different one!</source>
        <translation>Identifiant &lt;strong&gt;%1&lt;/strong&gt; d&apos;extrait dupliqué détécté. Merci d&apos;en choisir un autre !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="228"/>
        <source>Invalid ID pattern detected &lt;strong&gt;%1&lt;/strong&gt;. This one must start with at leat one letter and be composed by letters, numbers and/or underscore!</source>
        <translation>Motif d&apos;identification &lt;strong&gt;%1&lt;/strong&gt; incorrect détecté. Celui-ci doit commencer par au moins une lettre et être composé de lettres, de chiffres et/ou de tirets bas _ !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="230"/>
        <source>Empty label for snippet &lt;strong&gt;%1&lt;/strong&gt;. Please, specify a value for it!</source>
        <translation>Label vide pour l&apos;extrait &lt;strong&gt;%1&lt;/strong&gt;. Merci de lui spécifier une valeur !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="232"/>
        <source>Empty code for snippet &lt;strong&gt;%1&lt;/strong&gt;. Please, specify a value for it!</source>
        <translation>Code vide pour l&apos;extrait &lt;strong&gt;%1&lt;/strong&gt;. Merci de lui spécifier une valeur !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="248"/>
        <source>The dynamic snippet contains syntax error(s). Additional info: &lt;br/&gt;&lt;em&gt;%1&lt;/em&gt;</source>
        <translation>L&apos;extrait dynamique contient des erreurs de syntax. Plus d&apos;informations : &lt;br/&gt;&lt;em&gt;%1&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="366"/>
        <source>Do you really want to remove all snippets?</source>
        <translation>Voulez-vous vraiment supprimer tous les extraits ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="436"/>
        <source>No syntax errors found in the snippet.</source>
        <translation>Aucune erreur de syntax retrouvée dans l&apos;extrait.</translation>
    </message>
</context>
<context>
    <name>SourceCodeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="49"/>
        <source>Version:</source>
        <translation>Version :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="95"/>
        <source>PostgreSQL</source>
        <translation>PostgreSQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="146"/>
        <source>iconecodigo</source>
        <translation>icône</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="162"/>
        <source>SQL</source>
        <translation>SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="202"/>
        <source>Original</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="291"/>
        <source>XML</source>
        <translation>XML</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="14"/>
        <source>Source code visualization</source>
        <translation>Visualisation du code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="99"/>
        <source>SQL code (*.sql);;All files (*.*)</source>
        <translation>Code SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="143"/>
        <source>Generating source code...</source>
        <translation>Génération du code source...</translation>
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
        <translation>-- NOTE: le code ci-dessous contient le code SQL de l&apos;objet sélectionné
-- ainsi que de ses dépendances et enfants (le cas échéant).
--
-- Cette fonctionnalité n&apos;est qu&apos;une commodité vous permettant de tester
-- la définition SQL de l&apos;objet entier.
--
-- Lors de l&apos;export ou de la génération du code SQL pour l&apos;ensemble du modèle de base de données,
-- tous les objets seront placés à leurs positions d&apos;origine.


</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="190"/>
        <source>-- SQL code unavailable for this type of object --</source>
        <translation>-- Code SQL non disponible pour ce type d&apos;objet. --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="185"/>
        <source>Code display:</source>
        <translation>Affichage du code :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="207"/>
        <source>Original + depedencies&apos; SQL</source>
        <translation>Original + dépendances de SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="212"/>
        <source>Original + children&apos;s SQL</source>
        <translation>Original + enfants de SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="255"/>
        <source>Save the SQL code to a file.</source>
        <translation>Enregistrer le code SQL dans un fichier.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="258"/>
        <source>Save SQL</source>
        <translation>Enregistrer le code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="94"/>
        <source>Save SQL code as...</source>
        <translation>Enregistrer le code SQL sous...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="194"/>
        <source>&lt;!-- XML code preview disabled in demonstration version --&gt;</source>
        <translation>&lt; !-- Aperçu du code XML désactivé dans la version de démonstration --&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="46"/>
        <source>&lt;strong&gt;Original:&lt;/strong&gt; displays only the original object&apos;s SQL code.&lt;br/&gt;&lt;br/&gt;						   &lt;strong&gt;Dependencies:&lt;/strong&gt; displays the original code including all dependencies needed to properly create the selected object.&lt;br/&gt;&lt;br/&gt;						   &lt;strong&gt;Children:&lt;/strong&gt; displays the original code including all object&apos;s children SQL code. This option is used only by schemas, tables and views.</source>
        <translation>&lt;strong&gt;Original :&lt;/strong&gt; affiche uniquement le code SQL de l&apos;objet d&apos;origine.&lt;br/&gt;&lt;br/&gt;						   &lt;strong&gt;Dépendances :&lt;/strong&gt; affiche le code d&apos;origine ainsi que toutes les dépendances nécessaires pour créer correctement l&apos;objet sélectionné.&lt;br/&gt;&lt;br/&gt;						   &lt;strong&gt;Enfants :&lt;/strong&gt; affiche le code original ainsi que le code SQL de tous les enfants. Cette option est utilisée uniquement par les schémas, les tables et les vues.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="181"/>
        <source>

-- SQL code purposely truncated at this point in demo version!</source>
        <translation>

-- Le code SQL est volontairement tronqué à ce stade dans la version démo !</translation>
    </message>
</context>
<context>
    <name>SwapObjectsIdsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="38"/>
        <source>Create:</source>
        <translation>Créer :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="63"/>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="138"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="113"/>
        <source>Before:</source>
        <translation>Avant :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="307"/>
        <source>Swap the values of the fields</source>
        <translation>Procède à l&apos;échange</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="310"/>
        <source>Swap values</source>
        <translation>Échanger</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="20"/>
        <source>Change objects creation order</source>
        <translation>Changer l&apos;ordre de création des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="264"/>
        <source>Change the objects creation order is an irreversible operation and cause the operations history to be automatically erased. Note that the creation order configured in this form is not definitive and may change after a model validation.</source>
        <translation>Modifier l&apos;ordre de création des objets est une opération irréversible qui entraîne l&apos;effacement automatique de l&apos;historique des opérations. Noter que l&apos;ordre de création configuré dans ce formulaire n&apos;est pas définitif et peut changer après la validation d&apos;un modèle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="360"/>
        <source>Swap the object ids changing their creation order</source>
        <translation>Modifie l&apos;ordre de création de deux objets en échangeant leurs numéros d&apos;identification</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="363"/>
        <source>Swap ids</source>
        <translation>Échanger identifiants</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="413"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="502"/>
        <source>ID</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="514"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="526"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="538"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="550"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
</context>
<context>
    <name>Table</name>
    <message>
        <location filename="../libpgmodeler/src/table.cpp" line="46"/>
        <source>new_table</source>
        <translation>nouvelle_table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler/src/table.cpp" line="343"/>
        <source>In demonstration version tables can have only `%1&apos; instances of each child object type or ancestor tables! You&apos;ve reach this limit for the type: `%2&apos;</source>
        <translation>Dans la version de démonstration, les tables ne peuvent contenir que `%1&apos; instance(s) de chaque type d&apos;objet enfant ou table ancêtre ! Vous avez atteint cette limite pour le type : `%2&apos;</translation>
    </message>
</context>
<context>
    <name>TableDataWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="14"/>
        <source>Edit table data</source>
        <translation>Modifier les données de la table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="110"/>
        <source>Copy items on the grid</source>
        <translation type="unfinished">Copier les éléments sur la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="113"/>
        <source>Copy</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="145"/>
        <source>Add empty rows</source>
        <translation>Ajouter des lignes vides</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="148"/>
        <source>Add row</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="161"/>
        <source>Ins</source>
        <translation>Insérer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="433"/>
        <source>Ctrl+D</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="382"/>
        <source>Add an empty column</source>
        <translation>Ajouter une colonne vide</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="582"/>
        <source>Delete rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="703"/>
        <source>Remove all rows from the grid preserving columns</source>
        <translation>Supprimer toutes les lignes des colonnes de préservation de la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="719"/>
        <source>Shift+Del</source>
        <translation>Maj+Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="579"/>
        <source>Delete the selected rows</source>
        <translation>Supprimer les lignes sélectionnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="196"/>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="595"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="417"/>
        <source>Duplicate the selected rows</source>
        <translation>Dupliquer les lignes sélectionnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="180"/>
        <source>Delete the selected columns</source>
        <translation>Supprimer les colonnes sélectionnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="183"/>
        <source>Delete column</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="220"/>
        <source>Paste items on the grid</source>
        <translation type="unfinished">Copier les éléments sur la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="223"/>
        <source>Paste</source>
        <translation type="unfinished">Coller</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="236"/>
        <source>Ctrl+V</source>
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
        <translation type="unfinished">Modifier d&apos;un coup les valeurs de toutes les cellules sélectionnées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="473"/>
        <source>Bulk data edit</source>
        <translation type="unfinished">Modification de données en masse</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="486"/>
        <source>Ctrl+E</source>
        <translation type="unfinished">Ctrl+E</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="544"/>
        <source>Remove all columns (and rows) from the grid</source>
        <translation>Supprimer toutes les colonnes (et lignes) de la grille</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="547"/>
        <source>Delete all columns</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="560"/>
        <source>Ctrl+Shift+Del</source>
        <translation>Ctrl+Maj+Suppr</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="706"/>
        <source>Delete all rows</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="175"/>
        <source>Delete columns is an irreversible action! Do you really want to proceed?</source>
        <translation>Effacer des colonnes est une action irréversible ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="210"/>
        <source>Remove all rows is an irreversible action! Do you really want to proceed?</source>
        <translation>Supprimer toutes les lignes est une action irréversible ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="225"/>
        <source>Remove all columns is an irreversible action! Do you really want to proceed?</source>
        <translation>Supprimer toutes les colonnes est une action irréversible ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="262"/>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="377"/>
        <source>Unknown column</source>
        <translation>Colonne inconnue</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="379"/>
        <source>Duplicated column</source>
        <translation>Colonne dupliquée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="258"/>
        <source>Fills the grid using a CSV file</source>
        <translation>Remplit la grille en utilisant un fichier CSV</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="681"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Empty values are assumed as &lt;span style=&quot; font-weight:600;&quot;&gt;DEFAULT&lt;/span&gt;. To use special values like &lt;span style=&quot; font-weight:600;&quot;&gt;NULL&lt;/span&gt;, a function call like &lt;span style=&quot; font-weight:600;&quot;&gt;now()&lt;/span&gt; or a specific data escaping, enclose values in two slashes, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;/value/&lt;/span&gt;. To use a slash as part of the value prepend the backslash character, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;\/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Les valeurs vides sont supposées être &lt;span style=&quot;font-weight :600;&quot;&gt;DEFAULT&lt;/span&gt;. Pour utiliser des valeurs spéciales comme &lt;span style=&quot;font-weight :600;&quot;&gt;NULL&lt;/span&gt;, un appel de fonction comme &lt;span style=&quot;font-weight :600;&quot;&gt;now()&lt;/span&gt; ou un échappement des données spécifiques, entourer les valeurs de deux barres obliques, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;/valeur/&lt;/span&gt;. Pour utiliser une barre oblique dans le cadre de la valeur, ajouter le caractère barre oblique inverse, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>TableObjectView</name>
    <message>
        <location filename="../libobjrenderer/src/tableobjectview.cpp" line="208"/>
        <source>
Relationship: %1</source>
        <translation>
Relations : %1</translation>
    </message>
</context>
<context>
    <name>TableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="38"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="91"/>
        <source>Tag:</source>
        <translation>Balise :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="56"/>
        <source>With OID</source>
        <translation>Avec OID</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="133"/>
        <source>Generate ALTER for columns/constraints</source>
        <translation>Utiliser ALTER pour les colonnes/contraintes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="72"/>
        <source>Unlogged</source>
        <translation>Unlogged</translation>
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
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="62"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="64"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="100"/>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="123"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="97"/>
        <source>PK</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished">CP</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="102"/>
        <source>Default Value</source>
        <translation>Valeur par défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="103"/>
        <source>Attribute(s)</source>
        <translation>Attribut(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="114"/>
        <source>It is not possible to mark a column as primary key when the table already has a primary key which was created by a relationship! This action should be done in the section &lt;strong&gt;Primary key&lt;/strong&gt; of the relationship&apos;s editing form.</source>
        <translation>Il n&apos;est pas possible de marquer une colonne comme clé primaire quand la table a déjà une clé primaire qui a été créée par une relation ! Cette action doit être effectuée dans la section &lt;strong&gt;Clé primaire&lt;/strong&gt; du formulaire d&apos;édition de la relation.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="116"/>
        <source>It is not possible to mark a column created by a relationship as primary key! This action should be done in the section &lt;strong&gt;Primary key&lt;/strong&gt; of the relationship&apos;s editing form.</source>
        <translation>Il n&apos;est pas possible de marquer une colonne créée par une relation comme clé primaire ! Cette action doit être effectuée dans la section &lt;strong&gt;Clé primaire&lt;/strong&gt; du formulaire d&apos;édition de la relation.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="125"/>
        <source>ON DELETE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>À la suppression</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="126"/>
        <source>ON UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>À la mise à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="131"/>
        <source>Refer. Table</source>
        <translation>Table de référ.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="133"/>
        <source>Firing</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="135"/>
        <source>Events</source>
        <translation>Evènements</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="140"/>
        <source>Execution</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="141"/>
        <source>Event</source>
        <translation>Evènement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="146"/>
        <source>Indexing</source>
        <translation>Indexation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="151"/>
        <source>Command</source>
        <translation>Commande</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="153"/>
        <source>Permissive</source>
        <translation>Permissif</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="154"/>
        <source>USING expression</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="155"/>
        <source>CHECK expression</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="156"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="249"/>
        <source>Parent</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="258"/>
        <source>Copy</source>
        <translation>Copie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="511"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="511"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="147"/>
        <source>&amp;Columns</source>
        <translation>&amp;Colonnes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="152"/>
        <source>Co&amp;nstraints</source>
        <translation>Co&amp;ntraintes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="157"/>
        <source>Tri&amp;ggers</source>
        <translation>Décl&amp;encheurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="162"/>
        <source>&amp;Rules</source>
        <translation>&amp;Régles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="167"/>
        <source>&amp;Indexes</source>
        <translation>&amp;Index</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="177"/>
        <source>&amp;Tables</source>
        <translation>&amp;Tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="43"/>
        <source>Edit data</source>
        <translation>Modifier données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="44"/>
        <source>Define initial data for the table</source>
        <translation>Définir les données initiales de la table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="104"/>
        <source>Enable row level security</source>
        <translation>Activer la sécurité au niveau de la ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="120"/>
        <source>Force RLS for owner</source>
        <translation>Forcer RLS pour le propriétaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="172"/>
        <source>&amp;Policies</source>
        <translation>&amp;Politiques</translation>
    </message>
</context>
<context>
    <name>TablespaceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablespacewidget.ui" line="20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablespacewidget.ui" line="38"/>
        <source>Directory:</source>
        <translation>Dossier :</translation>
    </message>
</context>
<context>
    <name>TagWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="29"/>
        <source>Colors</source>
        <translation>Couleurs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="75"/>
        <source>Extended body:</source>
        <translation>Corps étendu :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="47"/>
        <source>Body:</source>
        <translation>Corps :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="54"/>
        <source>Title:</source>
        <translation>Titre :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="61"/>
        <source>Schema name:</source>
        <translation>Nom du schéma :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="68"/>
        <source>Table name:</source>
        <translation>Nom de la table :</translation>
    </message>
</context>
<context>
    <name>TaskProgressWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/taskprogresswidget.ui" line="35"/>
        <source>Executing tasks</source>
        <translation>Exécution des tâches</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/taskprogresswidget.ui" line="137"/>
        <location filename="../libpgmodeler_ui/src/taskprogresswidget.cpp" line="52"/>
        <source>Waiting task to start...</source>
        <translation>En attente du démarrage de la tâche ...</translation>
    </message>
</context>
<context>
    <name>TextboxWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="35"/>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="42"/>
        <source>Text</source>
        <translation>Texte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="93"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="102"/>
        <source>Color:</source>
        <translation>Couleur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="200"/>
        <source>Bold</source>
        <translation>Gras</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="207"/>
        <source>Italic</source>
        <translation>Italique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="220"/>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/textboxwidget.cpp" line="55"/>
        <source>Select text color</source>
        <translation>Sélection de la couleur du texte</translation>
    </message>
</context>
<context>
    <name>TriggerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="196"/>
        <source>Event:</source>
        <translation>Évènement :</translation>
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
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="74"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="409"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="442"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="367"/>
        <source>Arguments</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="46"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="169"/>
        <source>FOR EACH ROW</source>
        <translation>Pour chaque ligne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="331"/>
        <source>Refer. Table:</source>
        <translation>Table référ. :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="356"/>
        <source>Condition:</source>
        <translation>Condition :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="388"/>
        <source>Argument:</source>
        <translation>Argument :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="496"/>
        <source>Function:</source>
        <translation>Fonction :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="524"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="552"/>
        <source>Excution:</source>
        <translation>Exécution :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/triggerwidget.cpp" line="47"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/triggerwidget.cpp" line="49"/>
        <source>Type</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>TypeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="324"/>
        <source>Configuration:</source>
        <translation>Configuration :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="337"/>
        <source>Base Type</source>
        <translation>Type de base</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="353"/>
        <source>Enumeration</source>
        <translation>Énumération</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="391"/>
        <source>Enumerations</source>
        <translation>Énumérations</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="412"/>
        <source>Enumeration:</source>
        <translation>Énumération :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="126"/>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="437"/>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="490"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="147"/>
        <source>Internal Length:</source>
        <translation>Long. interne :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="170"/>
        <source>Storage:</source>
        <translation>Stockage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="224"/>
        <source>Category:</source>
        <translation>Catégorie :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="234"/>
        <source>Delimiter:</source>
        <translation>Délimiteur :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="258"/>
        <source>Alignment:</source>
        <translation>Alignement :</translation>
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
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="278"/>
        <source>integer</source>
        <translation>integer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="293"/>
        <source>double precision</source>
        <translation>double précision</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="248"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="44"/>
        <source>Range</source>
        <translation>Intervalle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="180"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="195"/>
        <source>By value</source>
        <translation>Par valeur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="208"/>
        <source>Preferred</source>
        <translation>Préféré</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="215"/>
        <source>Collatable</source>
        <translation>Assemblable</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="55"/>
        <source>Functions</source>
        <translation>Fonctions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="76"/>
        <source>INPUT:</source>
        <translation>INPUT :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="83"/>
        <source>OUTPUT:</source>
        <translation>OUTPUT :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="90"/>
        <source>RECV:</source>
        <translation>RECV :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="97"/>
        <source>SEND:</source>
        <translation>SEND :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="104"/>
        <source>TPMOD_IN:</source>
        <translation>TPMOD_IN :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="111"/>
        <source>TPMOD_OUT:</source>
        <translation>TPMOD_OUT :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="118"/>
        <source>ANALYZE:</source>
        <translation>ANALYZE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="458"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="474"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="511"/>
        <source>Subtype Diff Func.:</source>
        <translation>Fonction de différence du sous-type :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="518"/>
        <source>Operator Class:</source>
        <translation>Classe d&apos;opérateurs :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="525"/>
        <source>Canonical Func.:</source>
        <translation>Fonction canonique :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="33"/>
        <source>Like Type</source>
        <translation type="unfinished">Type de Like</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="34"/>
        <source>Element Type</source>
        <translation>Type d&apos;élement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="35"/>
        <source>Subtype</source>
        <translation>Sous-type</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="58"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="60"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="62"/>
        <source>Collation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="77"/>
        <source>The functions to be assigned to a type should be written in C language and possess, respectively, the following signatures:&lt;br/&gt;  &lt;table&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;INPUT:&lt;/strong&gt; &lt;em&gt;any function(cstring, oid, integer)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;OUTPUT:&lt;/strong&gt; &lt;em&gt;cstring function(any)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;SEND:&lt;/strong&gt; &lt;em&gt;byta function(any)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;RECV:&lt;/strong&gt; &lt;em&gt;any function(internal, oid, integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_IN:&lt;/strong&gt; &lt;em&gt;integer function(cstring[])&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_OUT:&lt;/strong&gt; &lt;em&gt;cstring function(integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;ANALYZE:&lt;/strong&gt; &lt;em&gt;boolean function(internal)&lt;/em&gt;&lt;/td&gt;    &lt;tr&gt;  &lt;/table&gt;</source>
        <translation>Les fonctions assignées à un type devraient être écrites en langage C et possèder, les signatures suivantes :&lt;br/&gt;  &lt;table&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;INPUT:&lt;/strong&gt; &lt;em&gt;any function(cstring, oid, integer)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;OUTPUT:&lt;/strong&gt; &lt;em&gt;cstring function(any)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;SEND:&lt;/strong&gt; &lt;em&gt;byta function(any)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;RECV:&lt;/strong&gt; &lt;em&gt;any function(internal, oid, integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_IN:&lt;/strong&gt; &lt;em&gt;integer function(cstring[])&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_OUT:&lt;/strong&gt; &lt;em&gt;cstring function(integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;ANALYZE:&lt;/strong&gt; &lt;em&gt;boolean function(internal)&lt;/em&gt;&lt;/td&gt;    &lt;tr&gt;  &lt;/table&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="95"/>
        <source>The functions to be assigned to a range type should have the following signatures:&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonical:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</source>
        <translation>Les fonction assignables au type plage (RANGE) doivent avoir la signature suivante :&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonical:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="372"/>
        <source>Co&amp;mposite</source>
        <translation>Co&amp;mposite</translation>
    </message>
</context>
<context>
    <name>UpdateNotifierWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="26"/>
        <source>Update Notifier</source>
        <translation>Vérificateur de mise à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="131"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="134"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="168"/>
        <source>Released in:</source>
        <translation>Date de sortie :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="188"/>
        <source>mmm dd, yyyy</source>
        <translation>jj mmm aaaa</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="236"/>
        <source>0.0.0</source>
        <translation>0.0.0</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="245"/>
        <source>Changelog</source>
        <translation>Journal des modifications</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="309"/>
        <source>Redirects to purchase page.</source>
        <translation>Redirige vers la page web pour acheter.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="312"/>
        <source>Get binary package</source>
        <translation>Télécharger l&apos;application compilée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="344"/>
        <source>Redirects to GitHub source repository.</source>
        <translation>Redirige vers la page web du dépôt GitHub.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="347"/>
        <source>Get source code</source>
        <translation>Télécharger le code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="115"/>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="162"/>
        <source>Failed to check updates</source>
        <translation>Impossible de vérifier la présence de mise à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="116"/>
        <source>The update notifier failed to check for new versions! Please, verify your internet connectivity and try again! Connection error returned: &lt;em&gt;%1&lt;/em&gt; - &lt;strong&gt;%2&lt;/strong&gt;.</source>
        <translation>Le notificateur de mise à jour n&apos;a pas pu vérifier les nouvelles versions ! Merci de vérifier votre connexion internet et  de réessayer ! Erreur de connexion renvoyée : &lt;em&gt;%1&lt;/em&gt; - &lt;strong&gt;%2&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="153"/>
        <source>No updates found</source>
        <translation>Aucune mise à jour trouvée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="154"/>
        <source>You are running the most recent pgModeler version! No update needed.</source>
        <translation>Vous utilisez déjà la version de pgModeler la plus récente ! Aucune mise à jour requise.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="163"/>
        <source>The update notifier failed to check for new versions! A HTTP status code was returned: &lt;strong&gt;%1&lt;/strong&gt;</source>
        <translation>Le vérificateur de mise à jour n&apos;a pas pu vérifier la présence de nouvelles versions ! Code retour HTTP reçu : &lt;strong&gt;%1&lt;/strong&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="81"/>
        <source>Update found!</source>
        <translation>Mise à jour trouvée !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="217"/>
        <source>New version:</source>
        <translation>Nouvelle version :</translation>
    </message>
</context>
<context>
    <name>ViewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="33"/>
        <source>References</source>
        <translation>Références</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="68"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="73"/>
        <source>Expression</source>
        <translation>Expression</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="81"/>
        <source>Used in:</source>
        <translation>Utilisé dans :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="60"/>
        <source>Reference Type:</source>
        <translation>Type de référence :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="256"/>
        <source>View Definition</source>
        <translation>Définition de la vue</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="323"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="347"/>
        <source>Tag:</source>
        <translation>Balise :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="360"/>
        <source>Mode:</source>
        <translation>Mode :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="369"/>
        <source>Ordinary</source>
        <translation>Ordinaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="405"/>
        <source>With no data</source>
        <translation>Sans données</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="88"/>
        <source>Table:</source>
        <translation>Table :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="101"/>
        <source>Table Alias:</source>
        <translation>Alias table :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="118"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="131"/>
        <source>Column Alias:</source>
        <translation>Alias colonne :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="148"/>
        <source>Expression:</source>
        <translation>Expression :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="161"/>
        <source>Expression Alias:</source>
        <translation>Alias expression :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="202"/>
        <source>SELECT ...</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="218"/>
        <source>WHERE ...</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="240"/>
        <source>FROM ...</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="295"/>
        <source>Triggers</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="300"/>
        <source>Rules</source>
        <translation>Règles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="305"/>
        <source>Indexes</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="310"/>
        <source>Table Expression</source>
        <translation>CTE</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="315"/>
        <source>Code Preview</source>
        <translation>Prévisualisation du code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="71"/>
        <source>Alias Col.</source>
        <translation>Alias col.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="69"/>
        <source>Col./Expr.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="70"/>
        <source>Alias</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="72"/>
        <source>Flags: SF FW AW EX VD</source>
        <translation>Marques : SF FW AW EX VD</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="77"/>
        <source>To reference all columns in a table (*) just do not fill the field &lt;strong&gt;Column&lt;/strong&gt;, this is the same as write &lt;em&gt;&lt;strong&gt;[schema].[table].*&lt;/strong&gt;&lt;/em&gt;</source>
        <translation>Pour référencer toutes les colonnes d&apos;une table (*), ne pas remplir le champ &lt;strong&gt;Colonne&lt;/strong&gt; revient à écrire &lt;em&gt;&lt;strong&gt;[schema].[table].*&lt;/Strong&gt;&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="105"/>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="114"/>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="119"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="107"/>
        <source>Refer. Table</source>
        <translation>Table de référ.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="109"/>
        <source>Firing</source>
        <translation>Déclenche</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="111"/>
        <source>Events</source>
        <translation>Evènements</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="116"/>
        <source>Indexing</source>
        <translation>Indexation</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="121"/>
        <source>Execution</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="122"/>
        <source>Event</source>
        <translation>Evènement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="710"/>
        <source>/* Could not generate the SQL code. Make sure all attributes are correctly filled! </source>
        <translation>/* Le code SQL ne peut être généré. Vérifiez que les champs sont correctement remplis ! </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="379"/>
        <source>Recursi&amp;ve</source>
        <translation>Récursi&amp;f</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="389"/>
        <source>&amp;Materialized</source>
        <translation>&amp;Matérialiser</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="199"/>
        <source>The element will be used as part of the SELECT statement to retrieve columns or expressions that will compose the view&apos;s columns</source>
        <translation>L&apos;élément sera utilisé dans le cadre de l&apos;instruction SELECT pour extraire des colonnes ou des expressions qui composeront les colonnes de la vue.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="215"/>
        <source>The element will be used as part of the WHERE clause in form of conditional expression</source>
        <translation>L&apos;élément sera utilisé dans le cadre de la clause WHERE sous forme d&apos;expression conditionnelle.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="237"/>
        <source>The element is used in the FROM portion of the command in order to reference tables or construct JOIN statements</source>
        <translation>L&apos;élément est utilisé dans la partie FROM de la commande afin de référencer des tables ou de construire des instructions JOIN.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="253"/>
        <source>The element&apos;s expression is used exclusively as the view&apos;s definition</source>
        <translation>L&apos;expression de l&apos;élément est utilisée exclusivement comme définition de la vue.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="269"/>
        <source>The element will be appended to the very end of the view&apos;s definition. This is useful when using GROUP BY/HAVING statements</source>
        <translation>L&apos;élément sera ajouté à la fin de la définition de la vue. Ceci est utile lors de l&apos;utilisation des instructions GROUP BY/HAVING.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="272"/>
        <source>End expression</source>
        <translation>Expression finale</translation>
    </message>
</context>
<context>
    <name>WelcomeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="23"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="97"/>
        <source>New model</source>
        <translation>Nouveau modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="135"/>
        <source>Open model</source>
        <translation>Ouvrir modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="176"/>
        <source>Sample models</source>
        <translation>Modèles de démo</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="220"/>
        <source>Recent models</source>
        <translation>Modèles récents</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="265"/>
        <source>Last session</source>
        <translation>Dernière session</translation>
    </message>
</context>
</TS>
