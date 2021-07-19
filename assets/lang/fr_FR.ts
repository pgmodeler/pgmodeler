<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="fr" sourcelanguage="en_US">
<context>
    <name>AboutWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutwidget.ui" line="+35"/>
        <source>About pgModeler</source>
        <translation>À propos de pgModeler</translation>
    </message>
    <message>
        <location line="+126"/>
        <source>0.0.0</source>
        <translatorcomment>Version is replaced at runtime</translatorcomment>
        <translation>0.0.0</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>build:</source>
        <translatorcomment>Anglicisme accepté (jargon)</translatorcomment>
        <translation>build:</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>PostgreSQL Database Modeler</source>
        <translation>Modeleur de base de données PostgreSQL</translation>
    </message>
    <message>
        <location line="+80"/>
        <source>Open source data modeling tool designed for PostgreSQL. No more DDL commands written by hand, let pgModeler do the job for you! This software reunites the concepts of entity-relationship diagrams and the features that PostgreSQL implements as extensions of SQL standards. </source>
        <translation>Outil open source de modélisation de bases de données PostgreSQL. Plus aucun ordre LDD à saisir à la main, laissez pgModeler le faire pour vous ! Ce logiciel fusionne le concept de modélisation entité-association et toutes les fonctionnalités étendues que propose PostgreSQL en plus des standards SQL. </translation>
    </message>
    <message>
        <location line="+124"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Copyright 2006-2019 - Raphael Araújo e Silva &amp;lt;&lt;a href=&quot;mailto:raphael@pgmodeler.com.br&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;raphael@pgmodeler.io&lt;/span&gt;&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translatorcomment>Même chose</translatorcomment>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Copyright 2006-2019 - Raphael Araújo e Silva &amp;lt;&lt;a href=&quot;mailto:raphael@pgmodeler.com.br&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;raphael@pgmodeler.io&lt;/span&gt;&lt;/a&gt;&amp;gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="-25"/>
        <source>pgModeler is proudly a brazilian software!</source>
        <translation>pgModeler est un logiciel brésilien et fier de l&apos;être !</translation>
    </message>
    <message>
        <location line="-16"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;a href=&quot;http://pgmodeler.com.br&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#2980b9;&quot;&gt;https://pgmodeler.io&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation></translation>
    </message>
    <message>
        <location line="-133"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-68"/>
        <source>(BUILD_NUM)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+140"/>
        <source>License</source>
        <translation></translation>
    </message>
    <message>
        <location line="-75"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
</context>
<context>
    <name>AggregateWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="+38"/>
        <source>Final Function:</source>
        <translation>Fonction finale :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Sort Operator:</source>
        <translation>Opérateur de tri :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Initial Condition:</source>
        <translation>Condition initiale :</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Funtion Inputs</source>
        <translation>Entrées de la fonction</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Function State</source>
        <translation>État de la fonction</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Transition Func.:</source>
        <translation>Fonction de transition :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="+37"/>
        <source>Input Data Type</source>
        <translation>Type de donnée en entrée</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>State Data Type</source>
        <translation>Type de donnée d&apos;état</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>An aggregate function that accepts the types &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; and &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; as input types and which type of state is &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt;&lt;/em&gt;, must obey the following rules: &lt;br/&gt;&lt;br/&gt; &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Final Function:&lt;/strong&gt; &lt;em&gt;void final_function(&lt;strong&gt;state_type&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Transition Function:&lt;/strong&gt; &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt; transition_function(&lt;strong&gt;state_type&lt;/strong&gt;, &lt;strong&gt;typeA&lt;/strong&gt;, &lt;strong&gt;typeB&lt;/strong&gt;)&lt;/em&gt;</source>
        <translation>Une fonction d&apos;agrégat qui accepte les types &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; et &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; en types d&apos;entrée et dont le type d&apos;état est &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt;&lt;/em&gt;, vous devez respecter les règles suivantes : &lt;br/&gt; &lt;strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;• Fonction finale : &lt;/strong&gt;&lt;em&gt;void final_function (&lt;strong&gt;state_type&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;• Fonction de transition: &lt;/strong&gt;&lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt; transition_function(&lt;strong&gt;state_type&lt;/strong&gt;,&lt;strong&gt;typeA&lt;/strong&gt;, &lt;strong&gt;typeB&lt;/strong&gt;)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>AppearanceConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/appearanceconfigwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Element:</source>
        <translation>Élément :</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Global: Font style</source>
        <translation>Global : Style de police</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Constraints descriptor</source>
        <translation>Global : Marqueurs de contraintes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Object selection</source>
        <translation>Global : Sélection d&apos;objet</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Position hint text</source>
        <translation>Global : Texte de la boite de coordonnées</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Position hint box</source>
        <translation>Global : Fond de la boite de coordonnées</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Objects type</source>
        <translation>Global : Type de colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Lock arc</source>
        <translation>Global : Cadenas (objet verrouillé, couleur de l&apos;arceau)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Lock body</source>
        <translation>Global : Cadenas (objet verrouillé, couleur du corps)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Schema name</source>
        <translation>Table : Nom du schéma</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Table: Columns box</source>
        <translation>Table : Zone des colonnes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Extended attributes box</source>
        <translation>Table : Zone d&apos;attributs étendus</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Title box</source>
        <translation>Table : Zone de titre</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rule: Name</source>
        <translation>Règle : Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rule: Descriptor</source>
        <translation>Règle : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Index: Name</source>
        <translation>Index : Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Index: Descriptor</source>
        <translation>Index : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Trigger: Name</source>
        <translation>Déclencheur : Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Trigger: Descriptor</source>
        <translation>Déclencheur : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Policy: Name</source>
        <translation>Politique : Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Policy: Descriptor</source>
        <translation>Politique : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Schema name</source>
        <translation>Vue : Nom du schéma</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>View: References box</source>
        <translation>Vue : Couleur d&apos;arrière plan</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Extended attributes box</source>
        <translation>Vue : Dialogue des attributs étendus</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Title box</source>
        <translation>Vue : Couleur d&apos;arrière plan de la barre de titre</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Table / columns alias</source>
        <translation>Vue : Alias de table / colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Referenced column</source>
        <translation>Vue : Colonne référencée</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Referenced table</source>
        <translation>Vue : Table référencée</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Reference descriptor</source>
        <translation>Vue : Couleur de l&apos;icône des références</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Textbox: Body</source>
        <translation>Zone de texte : Corps</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Column name</source>
        <translation>Colonne : Nom de la colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Descriptor</source>
        <translation>Colonne : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Included / Inherited by relationship</source>
        <translation>Colonne : Incluses / Héritées par associations</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Protected</source>
        <translation>Colonne : Protégée</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (pk): Descriptor</source>
        <translation>Colonne (pk) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (fk): Descriptor</source>
        <translation>Colonne (fk) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (uq): Descriptor</source>
        <translation>Colonne (uq) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (nn): Descriptor</source>
        <translation>Colonne (nn) : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Descriptor</source>
        <translation>Associations : Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Label text</source>
        <translation>Associations : Texte de l&apos;étiquette</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Label box</source>
        <translation>Relations : Arrière plan de l&apos;étiquette</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Attribute text</source>
        <translation>Relations : Texte des attributs</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Attribute descriptor</source>
        <translation>Relations : Couleur de l&apos;icône des attributs</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Tag: Name</source>
        <translation>Tag : Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Tag: Body</source>
        <translation>Tag : Corps</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Foreign table: Schema name</source>
        <translation>Table distante: Nom du schéma</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Foreign table: Name</source>
        <translation>Table distante : Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Foreign table: Attributes box</source>
        <translation>Table distante : Dialogue des attributs</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Foreign table: Extended attributes box</source>
        <translation>Table distante : Dialogue des attributs étendus</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Foreign table: Title box</source>
        <translation>Table distante: Zone de titre</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <location line="+54"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Bold</source>
        <translation>Gras</translation>
    </message>
    <message>
        <location line="-19"/>
        <source>Italic</source>
        <translation>Italique</translation>
    </message>
    <message>
        <location line="-342"/>
        <source>Table: Name</source>
        <translation>Table : Nom</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>View: Name</source>
        <translation>Vue : Nom</translation>
    </message>
    <message>
        <location line="+65"/>
        <source>Column (pk): Name</source>
        <translation>Colonne (pk): Nom</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (fk): Name</source>
        <translation>Colonne (fk): Nom</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (uq): Name</source>
        <translation>Colonne (uq): Nom</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (nn): Name</source>
        <translation>Colonne (nn): Nom</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>Colors:</source>
        <translation>Couleurs :</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <location line="-108"/>
        <source>Placeholder: Body</source>
        <translation>Placeholder : Corps</translation>
    </message>
    <message>
        <location line="-165"/>
        <source>Constraint: Name</source>
        <translation>Contrainte : Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Constraint: Descriptor</source>
        <translation>Contrainte : Descripteur</translation>
    </message>
</context>
<context>
    <name>AttributesTogglerItem</name>
    <message>
        <location filename="../libobjrenderer/src/attributestoggleritem.cpp" line="+40"/>
        <source>Expands the currently collapsed section of the object</source>
        <translation>Développer</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Collapses the currently expanded section of the object</source>
        <translation>Replier</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Displays the next attributes page</source>
        <translation>Page suivante des attributs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Displays the previous attributes page</source>
        <translation>Page précédente des attributs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Displays the next extended attributes page</source>
        <translation>Page suivante des attributs étendus</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Displays the previous extended attributes page</source>
        <translation>Page précédente des attributs étendus</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Toggles the attributes pagination on the object</source>
        <translation>Activer/Désactiver la pagination des attributs</translation>
    </message>
</context>
<context>
    <name>BaseConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/baseconfigwidget.cpp" line="+127"/>
        <source>A backup of the previous settings was saved into &lt;strong&gt;%1&lt;/strong&gt;!</source>
        <translation>Une sauvegarde des paramètres précédents a été enregistrée dans &lt;strong&gt;%1&lt;/strong&gt; !</translation>
    </message>
</context>
<context>
    <name>BaseForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="+23"/>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <location line="+118"/>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="+31"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="+5"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseform.ui" line="+47"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="+95"/>
        <source>%1 properties</source>
        <translation>%1 propriétés</translation>
    </message>
</context>
<context>
    <name>BaseObject</name>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="+42"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Trigger</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Index</source>
        <translation>Index</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Rule</source>
        <translation>Règle</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Table</source>
        <translation>Table</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>View</source>
        <translation>Vue</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Domain</source>
        <translation>Domaine</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Aggregate</source>
        <translation>Fonction d&apos;agrégat</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Sequence</source>
        <translation>Séquence</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Conversion</source>
        <translation>Convertion</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cast</source>
        <translation>Convertion de type</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Language</source>
        <translation>Langage</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operator Family</source>
        <translation>Famille d&apos;opérateurs</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Operator Class</source>
        <translation>Classe d&apos;opérateurs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Database</source>
        <translation>Base de données</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Extension</source>
        <translation>Extension</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Foreign Data Wrapper</source>
        <translation>Wrapper de données distantes</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Foreign Server</source>
        <translation>Serveur distant</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Foreign Table</source>
        <translation>Table distante</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>User Mapping</source>
        <translation>Correspondance d&apos;utilisateurs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Relationship</source>
        <translation>Relation</translation>
    </message>
    <message>
        <location line="-3"/>
        <source>Collation</source>
        <translation>Collation</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Textbox</source>
        <translation>Boite de texte</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Event Trigger</source>
        <translation>Déclencheur sur évènement</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Policy</source>
        <translation>Politique</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Permission</source>
        <translation>Permission</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Parameter</source>
        <translation>Paramètre</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Type Attribute</source>
        <translation>Attribut de type</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Basic Relationship</source>
        <translation>Relation simple</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Tag</source>
        <translation>Balise</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Generic SQL</source>
        <translation>SQL générique</translation>
    </message>
    <message numerus="yes">
        <location line="+51"/>
        <source>new_object</source>
        <translation>
            <numerusform>nouvel_objet</numerusform>
            <numerusform>nouveaux_objets</numerusform>
        </translation>
    </message>
    <message>
        <location line="-58"/>
        <source>Tablespace</source>
        <translation>Tablespace</translation>
    </message>
</context>
<context>
    <name>BaseObjectView</name>
    <message>
        <location filename="../libobjrenderer/src/baseobjectview.cpp" line="+466"/>
        <source>SQL off</source>
        <translation>SQL désactivé</translation>
    </message>
</context>
<context>
    <name>BaseObjectWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="+47"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location line="+399"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>icone</source>
        <translation>icône</translation>
    </message>
    <message>
        <location line="-362"/>
        <source>Comment:</source>
        <translation>Commentaire :</translation>
    </message>
    <message>
        <location line="-37"/>
        <source>This is the name of the object in the PostgreSQL database.</source>
        <translation>Ceci est le nom de l&apos;objet dans la base de données PostgreSQL.</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>Tablespace:</source>
        <translation>Tablespace :</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Edit object&apos;s permissions</source>
        <translation>Modifier les permissions de l&apos;objet</translation>
    </message>
    <message>
        <location line="+124"/>
        <source>Disables the generated SQL code using comment tokens (--).
This will disable the code of all child and referrer objects.</source>
        <translation>Désactivez le code SQL généré en utilisant les marqueurs de commentaire (--).
Ceci désactivera également le code SQL pour ses objets enfants.</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Append or prepend a set of SQL commands to the object&apos;s definition.</source>
        <translation>Ajouter des commandes SQL avant ou après la définition de cet objet.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Custom SQL</source>
        <translation>Code SQL Libre</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Owner:</source>
        <translation>Propriétaire :</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>&lt;p&gt;This is a more friendly name for the object. When displaying the model in compact view this is the name shown for the object instead of its real name. If this field is empty the real name will be displayed anyway.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Entrez ici un nom plus convivial pour cet objet (optionnel). Ce nom remplace le nom réel lorsque le modèle est affiché en mode Vue compacte&quot; . Si ce champ est vide le nom réel est utilisé.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Alias:</source>
        <translation>Alias :</translation>
    </message>
    <message>
        <location line="-423"/>
        <source>Schema:</source>
        <translation>Schéma :</translation>
    </message>
    <message>
        <location line="+162"/>
        <source>Disable SQL code</source>
        <translation>Désactiver le code SQL</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location line="-144"/>
        <source>Edit permissions</source>
        <translation>Modifier les permissions</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>This object is protected thus no change in form will be applied to it.</source>
        <translation>L&apos;objet est verrouillé, par conséquent aucun changement dans cette fenêtre ne peut lui être apporté.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="+179"/>
        <source>Required field. Leaving this empty will raise errors!</source>
        <translation>Champ requis. Laisser ce champ vide génèrera une erreur !</translation>
    </message>
    <message>
        <location line="+419"/>
        <source>Value(s)</source>
        <translation>Valeur(s)</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Version </source>
        <translation>Version </translation>
    </message>
    <message>
        <location line="+46"/>
        <source>The &lt;em style=&apos;color: %1&apos;&gt;&lt;strong&gt;highlighted&lt;/strong&gt;&lt;/em&gt; fields in the form or one of their values are available only on specific PostgreSQL versions. 							Generating SQL code for versions other than those specified in the fields&apos; tooltips may create incompatible code.</source>
        <translation>Les champs &lt;em style=&apos;color:%1&apos;&gt;&lt;strong&gt;en surbrillance&lt;/strong&gt;&lt;/em&gt; dans le formulaire ou de l&apos;une des valeurs ne sont disponibles que sur des versions spécifiques de PostgreSQL. 							La génération de code SQL pour des versions autres que celles spécifiées dans les info-bulles des champs peut créer un code incompatible.</translation>
    </message>
</context>
<context>
    <name>BaseRelationship</name>
    <message>
        <location filename="../libpgmodeler/src/baserelationship.cpp" line="+62"/>
        <source>rel_%1_%2</source>
        <translation>rel_%1_%2</translation>
    </message>
    <message>
        <location line="+465"/>
        <source>One-to-one</source>
        <translation>Un-à-un</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>One-to-many</source>
        <translation>Un-à-plusieurs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Many-to-many</source>
        <translation>Plusieurs-à-plusieurs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inheritance</source>
        <translation>Héritage</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Partitioning</source>
        <translation>Partitionnement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>FK relationship</source>
        <translation>Association clé étrangère</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Dependency</source>
        <translation>Dépendance</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Copy</source>
        <translation>Copie</translation>
    </message>
</context>
<context>
    <name>BaseTableView</name>
    <message>
        <location filename="../libobjrenderer/src/basetableview.cpp" line="+439"/>
        <source>Connected rels: %1</source>
        <translation>Nombre d&apos;associations : %1</translation>
    </message>
</context>
<context>
    <name>BugReportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/bugreportform.ui" line="+35"/>
        <source>Bug Report</source>
        <translation>Signaler un bug</translation>
    </message>
    <message>
        <location line="+72"/>
        <source>Bug report</source>
        <translation>Signaler un bug</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>Create</source>
        <translation>Créer</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location line="+104"/>
        <source>Use the form below to generate a complete bug report. Please, try to be as clear as possible when describing the actions that can reproduce the bug. Additionally, it&apos;s important to attach a sample database model so that the bug can be quickly discovered and fixed!</source>
        <translation>Utilisez le formulaire ci-dessous pour générer un rapport d&apos;erreur complet. Merci d&apos;être le plus clair possible dans la description de vos actions afin que le bug puisse être reproduit. De plus, il est important de joindre un modèle de base dans lequel le bug puisse être rapidement découvert, et par la même occasion rapidement corrigé !</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Report</source>
        <translation>Rapport</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Issue details</source>
        <translation>Détails du problème</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Output:</source>
        <translation>Sortie :</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Select the report&apos;s output folder</source>
        <translation>Sélectionnez un dossier de sortie pour écrire le rapport d&apos;erreur</translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+163"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-72"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;If you prefer it&apos;s possible to report this issue anytime on pgModeler&apos;s project repository at &lt;a href=&quot;http://github.com/pgmodeler/pgmodeler/issues&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;GitHub&lt;/span&gt;&lt;/a&gt;. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Il vous est également possible de signaler un bug à tout moment via le dépôt du projet pgModeler sur &lt;a href=&quot;http ://github.com/pgmodeler/pgmodeler/issues&quot;&gt;&lt;span style=&quot; text-decoration : underline; color :#0057ae;&quot;&gt;GitHub&lt;/span&gt;&lt;/a&gt;. &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Database Model</source>
        <translation>Modèle de base</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Attach the below database model file to be debugged.</source>
        <translation>Joindre le fichier modèle ci-dessous pour débogage.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Attach a different database model</source>
        <translation>Joindre un autre modèle de base</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/bugreportform.cpp" line="+117"/>
        <source>Bug report successfuly generated! Please, send the file &lt;strong&gt;%1&lt;/strong&gt; to &lt;em&gt;%2&lt;/em&gt; in order be analyzed. Thank you for the collaboration!</source>
        <translation>Le rapport de bug a été généré avec succès ! S&apos;il vous plait, veuillez envoyer le fichier &lt;strong&gt;%1&lt;/strong&gt; à &lt;em&gt;%2&lt;/em&gt; pour expertise. Merci de votre collaboration !</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Load model</source>
        <translation>Charger un modèle</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modèle de base de données (*.dbm);; Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Select report output folder</source>
        <translation>Sélectionner le dossier de sortie du rapport</translation>
    </message>
</context>
<context>
    <name>BulkDataEditWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/bulkdataeditwidget.ui" line="+20"/>
        <source>Bulk data edit</source>
        <translation>Modification de données en masse</translation>
    </message>
</context>
<context>
    <name>CastWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="+122"/>
        <source>Conversion Func.:</source>
        <translation>Fonction de conversion :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Cast Type:</source>
        <translation>Conversion de type :</translation>
    </message>
    <message>
        <location line="-63"/>
        <source>Assignment</source>
        <translation>Affectation</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Input / Output</source>
        <translation>Entrée / Sortie</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="+30"/>
        <source>Source data type</source>
        <translation>Type de donnée source</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Target data type</source>
        <translation>Type de donnée cible</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The function to be assigned to a cast from &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; to &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; must have the following signature: &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; function(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</source>
        <translation>La fonction convertissant de &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; vers &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; doit avoir la signature suivante : &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; function(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="-41"/>
        <source>I&amp;mplicit</source>
        <translation>I&amp;mplicite</translation>
    </message>
    <message>
        <location line="+116"/>
        <source>E&amp;xplicit</source>
        <translation>E&amp;xplicite</translation>
    </message>
</context>
<context>
    <name>Catalog</name>
    <message numerus="yes">
        <location filename="../libpgconnector/src/catalog.cpp" line="+610"/>
        <location line="+14"/>
        <location line="+31"/>
        <source>Object type: %1</source>
        <translation>
            <numerusform>Type d&apos;objet : %1</numerusform>
            <numerusform>Types d&apos;objets : %1</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="-14"/>
        <source>The catalog query returned more than one OID!</source>
        <translation>
            <numerusform>La requête sur le catalogue retourne plusieurs OID !</numerusform>
            <numerusform>Les requêtes sur le catalogue retournent plusieurs OID !</numerusform>
        </translation>
    </message>
    <message numerus="yes">
        <location line="+53"/>
        <source>Object type: server</source>
        <translation>
            <numerusform>Type d&apos;objet : serveur</numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
</context>
<context>
    <name>CodeCompletionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/codecompletionwidget.cpp" line="+41"/>
        <source>Make &amp;persistent</source>
        <translation>Rendre &amp;persistant</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Makes the widget closable only by ESC key or mouse click on other controls.</source>
        <translation>Rendre ce widget refermable uniquement par la touche Échap ou par un clic sur les autres contrôles.</translation>
    </message>
    <message>
        <location line="+408"/>
        <source>SQL Keyword</source>
        <translation>Mot-clé SQL</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>(no items found.)</source>
        <translation>(aucun élément trouvé.)</translation>
    </message>
</context>
<context>
    <name>CollationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="+50"/>
        <source>Locale:</source>
        <translation>Locale :</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Encoding:</source>
        <translation>Encodage :</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE :</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="+30"/>
        <source>The fields &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; are mutually exclusive, so you have to set only one of them in order to properly handle a collation.</source>
        <translation>Les champs &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt; et &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; sont mutuellement exclusifs, vous ne devez en définir qu&apos;un seul de manière à gérer correctement la collation.</translation>
    </message>
    <message>
        <location line="+9"/>
        <location line="+12"/>
        <source>Not defined</source>
        <translation>Non défini</translation>
    </message>
</context>
<context>
    <name>ColorPickerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/colorpickerwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Generate random color(s)</source>
        <translation>Génération aléatoire de couleur(s)</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/colorpickerwidget.cpp" line="+148"/>
        <source>Select color</source>
        <translation>Sélectionner une couleur</translation>
    </message>
</context>
<context>
    <name>ColumnWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="+32"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>E&amp;xpression:</source>
        <translation>E&amp;xpression :</translation>
    </message>
    <message>
        <location line="+71"/>
        <source>&amp;NOT NULL</source>
        <translation>&amp;NON NULL</translation>
    </message>
    <message>
        <location line="-61"/>
        <source>Se&amp;quence:</source>
        <translation>Sé&amp;quence :</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>Edit the underlying sequence&apos;s attributes</source>
        <translation>Modifier les propriétés sous-jacentes de la séquence</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Edit sequence</source>
        <translation>Modifier la séquence</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Identity:</source>
        <translation>Identité :</translation>
    </message>
</context>
<context>
    <name>ConfigurationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="+29"/>
        <source>pgModeler Configuration</source>
        <translation>Paramètres de pgModeler</translation>
    </message>
    <message>
        <location line="+173"/>
        <source>Relationships</source>
        <translation>Associations</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Appearance</source>
        <translation>Apparence</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Connections</source>
        <translation>Connexions</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Snippets</source>
        <translation>Extraits de code</translation>
    </message>
    <message>
        <location line="+144"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location line="-79"/>
        <source>Defaults</source>
        <translation>Défauts</translation>
    </message>
    <message>
        <location line="-160"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Plug-ins</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="+131"/>
        <source>In some cases restore the default settings related to it may solve the problem. Would like to do that?</source>
        <translation>Dans certains cas, restaurer les paramètres liés par défaut peut résoudre le problème. Voulez-vous le faire ?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Restore</source>
        <translation>Restaurer</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Any modification made until now in the current section will be lost! Do you really want to restore default settings?</source>
        <translation>Toutes les modifications réalisées jusqu&apos;à maintenant dans la section actuelle seront perdues ! Souhaitez-vous réellement restaurer les paramètres par défaut ?</translation>
    </message>
</context>
<context>
    <name>Connection</name>
    <message>
        <location filename="../libpgconnector/src/connection.cpp" line="+228"/>
        <source>ERROR: trying to open an already stablished connection.</source>
        <translation>ERREUR : Tentative d&apos;ouverture d&apos; une connexion déjà établie.</translation>
    </message>
</context>
<context>
    <name>ConnectionsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="+65"/>
        <source>Connections:</source>
        <translation>Connexions :</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Create new connection</source>
        <translation>Créer une nouvelle connexion</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Cancel edition</source>
        <translation>Annuler l&apos;édition</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Duplicate the selected connection</source>
        <translation>Dupliquer la connexion sélectionnée</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Edit selected connection</source>
        <translation>Modifier la connexion sélectionnée</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Delete selected connection</source>
        <translation>Supprimer la connexion sélectionnée</translation>
    </message>
    <message>
        <location line="+243"/>
        <source>&lt;p&gt;Indicates in which operations (diff, export, import or validation) the connection is used if none is explicitly specified by the user.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Indique pour quelles opérations (diff, export, import ou validation) cette connexion sera utilisée par défaut si aucune autre n&apos;est choisie par l&apos;utilisateur.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Connection Alias:</source>
        <translation>Alias de la connexion :</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Connection DB:</source>
        <translation>Nom de la base :</translation>
    </message>
    <message>
        <location line="-36"/>
        <source>Host/Port:</source>
        <translation>Hôte/Port :</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>User:</source>
        <translation>Utilisateur :</translation>
    </message>
    <message>
        <location line="-84"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location line="-144"/>
        <source>Timeout:</source>
        <translation>Délai d&apos;expiration :</translation>
    </message>
    <message>
        <location line="+329"/>
        <source>second(s)</source>
        <translation>seconde(s)</translation>
    </message>
    <message>
        <location line="+274"/>
        <source>SSL Mode:</source>
        <translation>Mode SSL :</translation>
    </message>
    <message>
        <location line="-92"/>
        <source>Disable</source>
        <translation>Désactivé</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Allow</source>
        <translation>Autorisé</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Require</source>
        <translation>Requis</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>AC verification</source>
        <translation>Vérification des AC</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Full verification</source>
        <translation>Vérification complète</translation>
    </message>
    <message>
        <location line="+184"/>
        <source>Client Certificate:</source>
        <translation>Certificat client :</translation>
    </message>
    <message>
        <location line="-226"/>
        <source>~/.postgresql/postgresql.crt</source>
        <translation>~/.postgresql/postgresql.crt</translation>
    </message>
    <message>
        <location line="-66"/>
        <source>Automatically browses the named database when using this connection to manage databases on &lt;strong&gt;Manage&lt;/strong&gt; view.</source>
        <translation>Parcourt automatiquement la base de données nommée lors de l&apos;utilisation de cette connexion pour gérer les bases de données dans la vue &lt;strong&gt;Gérer&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Auto browse</source>
        <translation>Toujours montrer</translation>
    </message>
    <message>
        <location line="+190"/>
        <source>Client Key:</source>
        <translation>Clé client :</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>~/.postgresql/postgresql.key</source>
        <translation>~/.postgresql/postgresql.key</translation>
    </message>
    <message>
        <location line="-26"/>
        <source>Root Certificate:</source>
        <translation>Certificat racine :</translation>
    </message>
    <message>
        <location line="-110"/>
        <source>~/.postgresql/root.crt</source>
        <translation>~/.postgresql/root.crt</translation>
    </message>
    <message>
        <location line="+126"/>
        <source>Revoked Certs.:</source>
        <translation>Liste de révocations :</translation>
    </message>
    <message>
        <location line="-54"/>
        <source>~/.postgresql/root.crl</source>
        <translation>~/.postgresql/root.crl</translation>
    </message>
    <message>
        <location line="-41"/>
        <source>Force GSSAPI</source>
        <translation>Forcer GSSAPI</translation>
    </message>
    <message>
        <location line="-507"/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Update</source>
        <translation>Mettre à jour</translation>
    </message>
    <message>
        <location line="-76"/>
        <source>Test</source>
        <translation>Tester</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="+415"/>
        <source>Success</source>
        <translation>Succès</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="-273"/>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="+172"/>
        <source>Edit database connections</source>
        <translation>Modifier les connexions de la base de données</translation>
    </message>
    <message>
        <location line="+206"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="+314"/>
        <source>Other params:</source>
        <translation>Autres paramètres :</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Specify additional connection parameters in the form [param]=[value]. These parameters are described in the &lt;strong&gt;libpq&lt;/strong&gt; chapter at PostgreSQL docs.</source>
        <translation>Spécifier des paramètres de connexion supplémentaires sous la forme [param]=[valeur]. Ces paramètres sont décrits dans le chapitre &lt;strong&gt;libpq&lt;/strong&gt; de la doc de PostgreSQL.</translation>
    </message>
    <message>
        <location line="-103"/>
        <source>Default for:</source>
        <translation>Par défaut pour :</translation>
    </message>
    <message>
        <location line="+163"/>
        <source>Diff</source>
        <translation>Comparer</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Export</source>
        <translation>Exporter</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Import</source>
        <translation>Importer</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Validation</source>
        <translation>Validation</translation>
    </message>
    <message>
        <location line="+88"/>
        <source>Security</source>
        <translation>Sécurité</translation>
    </message>
    <message>
        <location line="+183"/>
        <source>Kerberos Server:</source>
        <translation>Serveur Kerberos :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="-171"/>
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
        <location line="+47"/>
        <source>There is a connection being created or edited! Do you want to save it?</source>
        <translation>Une connexion est en cours de création ou d&apos;édition ! Voulez-vous la sauvegarder ?</translation>
    </message>
    <message>
        <location line="+94"/>
        <source>Found %1 connection(s)</source>
        <translation>%1 connexion(s) trouvée(s)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>No connections found</source>
        <translation>Aucune connexion trouvée</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Edit connections</source>
        <translation>Modifier les connexions</translation>
    </message>
</context>
<context>
    <name>ConstraintWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="+94"/>
        <source>Constraint Type:</source>
        <translation>Type de contrainte :</translation>
    </message>
    <message>
        <location line="+450"/>
        <source>Fill Factor:</source>
        <translation>Taux de remplissage :</translation>
    </message>
    <message>
        <location line="-488"/>
        <source>Match:</source>
        <translation>Comparaison :</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>This attribute cannot be changed once the object is created.</source>
        <translation>Cet attribut ne peut pas être modifié une fois l&apos;objet créé.</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Expression:</source>
        <translation>Expression :</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Deferral:</source>
        <translation>Report :</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE :</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="+27"/>
        <location line="+114"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location line="-68"/>
        <source>Referenced Columns</source>
        <translation>Colonnes référencées</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Table:</source>
        <translation>Table :</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
    <message>
        <location line="-194"/>
        <source>Exclude Elements</source>
        <translation>Éléments exclus</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>No inherit:</source>
        <translation>Pas d&apos;héritage :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/constraintwidget.cpp" line="+54"/>
        <location line="+7"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="-5"/>
        <location line="+7"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Columns which were included by relationship can not be added / removed manually from the primary key. If done such changes they can raise errors. To create primary key using columns included by relationship use the following options: identifier field, attributes &amp; constraints tab or primary key tab on the relationship form.</source>
        <translation>Les colonnes inclues dans la relation ne peuvent être ajoutées/retirées manuellement de la clé primaire sous peine d’erreur. Pour créer une clé primaire utilisant des colonnes contenues dans une relation, veuillez utiliser une des options suivantes: champ identifiant, l&apos;onglet attributs et contraintes ou l&apos;onglet clé primaire du formulaire de la relation.</translation>
    </message>
</context>
<context>
    <name>ConversionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="+56"/>
        <source>Source Encoding:</source>
        <translation>Encodage de la source :</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Target Encoding:</source>
        <translation>Encodage de la destination :</translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Conversion Func.:</source>
        <translation>Fonction de conversion :</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Default Conversion:</source>
        <translation>Conversion par défaut :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/conversionwidget.cpp" line="+38"/>
        <source>The function to be assigned to an encoding conversion must have the following signature: &lt;em&gt;void function(integer, integer, cstring, internal, integer)&lt;/em&gt;.</source>
        <translation>La fonction de convertion d&apos;encodage doit avoir la signature suivante : &lt;em&gt;void function(integer, integer, cstring, internal, integer)&lt;/em&gt;.</translation>
    </message>
</context>
<context>
    <name>CrashHandlerForm</name>
    <message>
        <location filename="../crashhandler/src/crashhandlerform.cpp" line="+32"/>
        <source>Crash Handler</source>
        <translation>Gestionnaire d&apos;incident</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Stack trace</source>
        <translation>Pile d&apos;exécution</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Input:</source>
        <translation>Entrée :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Load report file for analysis</source>
        <translation>Charger un rapport d&apos;erreur pour analyse</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Save the attached model file on the filesystem</source>
        <translation>Enregistrer le modèle joint sur le système de fichiers</translation>
    </message>
    <message>
        <location line="+77"/>
        <source>pgModeler bug report (*.bug);;All files (*.*)</source>
        <translation>Rapport de bug pgModeler (*.bug);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Load report</source>
        <translation>Charger un rapport</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Save model</source>
        <translation>Enregistrer le modèle</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modèle de base de données (*.dbm);; Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>Crash handler</source>
        <translation>Gestionnaire d&apos;incident</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Bug report analysis mode activated.</source>
        <translation>Mode d&apos;analyse de rapport d&apos;erreur activé.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Oops! pgModeler just crashed!</source>
        <translation>Oups ! pgModeler a planté !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>We apologize for what happened! It is clear that a nasty bug caused that. Please fill out the form below describing your actions before pgModeler quit unexpectedly. This will help on bug extermination and improve the software.</source>
        <translation>Veuillez nous excuser pour ce qui vient de se passer ! Il est clair qu&apos;un vilain bug est à l&apos;origine de tout cela. Veuillez renseigner le formulaire ci-dessous en décrivant les actions que vous avez faites avant que pgModeler ne sombre de manière inopinée. Cela nous aidera à exterminer ce bug et ainsi, à améliorer le logiciel.</translation>
    </message>
</context>
<context>
    <name>CsvLoadWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/csvloadwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Load CSV</source>
        <translation>Charger un fichier CSV</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>CSV File:</source>
        <translation>Fichier CSV :</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Select output file</source>
        <translation>Sélectionner un fichier de sortie</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Separator: </source>
        <translation>Séparateur : </translation>
    </message>
    <message>
        <location line="+28"/>
        <source>&lt;p&gt;Use the first row as column names in the CSV file. By unchecking this option the first row is used as data.&lt;/p&gt;</source>
        <translation>&lt;p&gt;La première ligne du fichier CSV sera utilisée pour les noms des colonnes. En décochant cette option la première ligne sera utilisée comme source de données.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Columns in the first row</source>
        <translation>Nom des colonnes sur 1ère ligne</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Load</source>
        <translation>Charger</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Semicolon (;)</source>
        <translation>Point virgule (;)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Comma (,)</source>
        <translation>Virgule (,)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Space</source>
        <translation>Espace</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Tabulation</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Other</source>
        <translation>Autre</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>;</source>
        <translation>;</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Text delimiter:</source>
        <translation>Délimiteur :</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>&quot;</source>
        <translation>&quot;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/csvloadwidget.cpp" line="+62"/>
        <source>Load CSV file</source>
        <translation>Charger un fichier CSV</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Comma-separted values (*.csv);;All files (*.*)</source>
        <translation>Valeurs séparées par une virgule (*.csv);;Tous les fichiers (*.*)</translation>
    </message>
</context>
<context>
    <name>CustomSQLWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="+44"/>
        <source>SQL code</source>
        <translation>Code SQL</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Puts an SELECT command template at current cursor position.</source>
        <translation>Insère une commande SELECT à la position courante du curseur.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;SELECT</source>
        <translation>&amp;SELECT</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Puts an INSERT command template at current cursor position.</source>
        <translation>Insère une commande INSERT à la position courante du curseur.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;INSERT</source>
        <translation>&amp;INSERT</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Puts an UPDATE command template at current cursor position.</source>
        <translation>Insère une commande UPDATE à la position courante du curseur.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Puts an DELETE command template at current cursor position.</source>
        <translation>Insère une commande DELETE à la position courante du curseur.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;DELETE</source>
        <translation>&amp;DELETE</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>&amp;Clear</source>
        <translation>E&amp;ffacer</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Append SQL</source>
        <translation>Code SQL suffixé</translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+33"/>
        <source>Append the SQL code at the very end of model definition.
Unchecking this will cause the SQL to be appended at the end of CREATE DATABASE command.</source>
        <translation>Ajouter le code SQL à la toute fin de la définition du modèle
Sinon (décoché) pgModeler ajoutera ce code après la commande SQL CREATE DATABASE.</translation>
    </message>
    <message>
        <location line="-29"/>
        <source>Append at end of model definition.</source>
        <translation>Suffixer à la fin de la définition du modèle.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Prepend SQL</source>
        <translation>Code SQL préfixé</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Prepend at beginning of model definition.</source>
        <translation>Préfixer au tout début de la définition du modèle.</translation>
    </message>
    <message>
        <location line="+84"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Use custom commands with extreme caution because you can change the semantics of the entire model when running SQL validation or export processes. Additionally, depending on the amount of commands, those processes can have their performance sensibly degradated.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Utilisez cet espace pour y inscrire vos commandes SQL personnalisées avec précaution. Vous pourriez créer des incohérences qui peuvent interférer avec les fonctionnalités de validation et d&apos;export du modèle. De plus, selon la nature des commandes entrées ces fonctionnalités peuvent être ralenties.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/customsqlwidget.cpp" line="+48"/>
        <source>Generic INSERT</source>
        <translation>INSERT générique</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Include serial columns</source>
        <translation>Inclure les colonnes en série</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Exclude serial columns</source>
        <translation>Exclure les colonnes de type séries</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Generic SELECT</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>SELECT générique</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Table SELECT</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>SELECT table</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Generic UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>UPDATE générique</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Table UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>UPDATE table</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Generic DELETE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>DELETE générique</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Table DELETE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>DELETE table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/customsqlwidget.ui" line="-246"/>
        <source>&amp;UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>&amp;UPDATE</translation>
    </message>
    <message>
        <location line="-139"/>
        <source>Add custom SQL code</source>
        <translation>Ajouter un code SQL personnalisé</translation>
    </message>
</context>
<context>
    <name>DataManipulationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="+26"/>
        <source>Data Manipulation</source>
        <translation>Édition des données</translation>
    </message>
    <message>
        <location line="+1724"/>
        <source>Refresh</source>
        <translation>Actualiser</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Truncate the browsed table</source>
        <translation>Vide la table</translation>
    </message>
    <message>
        <location line="+3"/>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="+100"/>
        <source>Truncate</source>
        <translation>Vider</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location line="-145"/>
        <source>Refresh listing</source>
        <translation>Actualiser la liste</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>F5</source>
        <translation>F5</translation>
    </message>
    <message>
        <location line="-148"/>
        <source>Save changes</source>
        <translation>Enregistrer les changements</translation>
    </message>
    <message>
        <location line="-190"/>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Delete</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Open a new data manipulation window</source>
        <translation>Ouvre une nouvelle fenêtre d&apos;édition des données</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>New </source>
        <translation>Nouvelle fenêtre </translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+N</source>
        <translation>Ctrl+N</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Paste</source>
        <translation>Coller</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Save</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Undo</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Export results to CSV file</source>
        <translation>Exporter les résultats vers un fichier CSV</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Export</source>
        <translation>Exporter</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+X</source>
        <translation>Ctrl+X</translation>
    </message>
    <message>
        <location line="-60"/>
        <source>Undo modifications</source>
        <translation>Annuler les modifications</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+Z</source>
        <translation>Ctrl+Z</translation>
    </message>
    <message>
        <location line="-578"/>
        <source>Ins</source>
        <translation>Ins</translation>
    </message>
    <message>
        <location line="+359"/>
        <source>Mark the selected rows to be deleted</source>
        <translation>Marquer les lignes sélectionner pour suppression</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location line="-63"/>
        <source>Filter the result set</source>
        <translation>Filtrer les résultats</translation>
    </message>
    <message>
        <location line="-1273"/>
        <source>Table:</source>
        <translation>Table :</translation>
    </message>
    <message>
        <location line="-35"/>
        <source>Schema:</source>
        <translation>Schéma :</translation>
    </message>
    <message>
        <location line="+73"/>
        <source>Hide views</source>
        <translation>Masquer les vues</translation>
    </message>
    <message>
        <location line="+210"/>
        <source>Order &amp;&amp; Limit</source>
        <translation>Ordre &amp;&amp; Limite</translation>
    </message>
    <message>
        <location line="+261"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Limit in:</source>
        <translation>Limite :</translation>
    </message>
    <message>
        <location line="-223"/>
        <source>Add Item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location line="-118"/>
        <location line="+1088"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
    </message>
    <message>
        <location line="-1063"/>
        <source>Expression</source>
        <translation>Expression</translation>
    </message>
    <message>
        <location line="+128"/>
        <source>Remove Item</source>
        <translation>Supprimer un élément</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Clear the order by columns list</source>
        <translation>Effacer la liste</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Move selected item up</source>
        <translation>Déplacer vers le haut</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Move selected item down</source>
        <translation>Déplacer vers le bas</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="-31"/>
        <source>Copy as CSV</source>
        <translation>Copier au format CSV</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Copy as text</source>
        <translation>Copier au format texte simple</translation>
    </message>
    <message>
        <location line="+1518"/>
        <source>Copy items</source>
        <translation>Copier les valeurs</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Pase items</source>
        <translation>Coller les éléments</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Clear items</source>
        <translation>Effacer les valeurs</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Browse tables</source>
        <translation>Navigateur de tables</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Duplicate row(s)</source>
        <translation>Dupliquer ligne(s)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Delete row(s)</source>
        <translation>Effacer ligne(s)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Edit cell(s)</source>
        <translation>Modifier en masse la valeur des cellules sélectionnées dans la grille</translation>
    </message>
    <message>
        <location line="-1206"/>
        <source>&lt;em&gt;(Limit: &lt;strong&gt;%1&lt;/strong&gt;)&lt;/em&gt;</source>
        <translation>&lt;em&gt;(Limite : &lt;strong&gt;%1&lt;/strong&gt;)&lt;/em&gt;</translation>
    </message>
    <message>
        <location line="-329"/>
        <source>Paste as text</source>
        <translation>Coller depuis format texte simple</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Paste as CSV</source>
        <translation>Coller depuis format CSV</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Truncate cascade</source>
        <translation>Vide la table avec clause CASCADE</translation>
    </message>
    <message>
        <location line="+345"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="+312"/>
        <source>No objects found</source>
        <translation>Aucun objet trouvé</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Found %1 object(s)</source>
        <translation>%1 objet(s) trouvé(s)</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Views can&apos;t have their data handled through this grid, this way, all operations are disabled.</source>
        <translation>Les données des vues ne sont pas modifiables via cette grille, de ce fait, toutes les opérations sont désactivées.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>The selected table doesn&apos;t owns a primary key! Updates and deletes will be performed by considering all columns as primary key. &lt;strong&gt;WARNING:&lt;/strong&gt; those operations can affect more than one row.</source>
        <translation>La table sélectionnée n&apos;a pas sa propre clé primaire ! Les mises à jour et les suppressions seront réalisées en considérant que l&apos;ensemble des colonnes constitue une clé primaire. &lt;strong&gt;ATTENTION :&lt;/strong&gt; ces opérations peuvent affecter plus d&apos;une ligne.</translation>
    </message>
    <message>
        <location line="+72"/>
        <source>Referenced tables</source>
        <translation>Tables référencées</translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+49"/>
        <source>(none)</source>
        <translation>(aucun)</translation>
    </message>
    <message>
        <location line="-3"/>
        <source>Referrer tables</source>
        <translation>Tables de référence</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>This row is marked to be %1</source>
        <translation>Cette ligne est marquée pour être %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>deleted</source>
        <translation>supprimé</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>updated</source>
        <translation>mise à jour</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>inserted</source>
        <translation>inséré</translation>
    </message>
    <message>
        <location line="+125"/>
        <source>[binary data]</source>
        <translation>[données binaires]</translation>
    </message>
    <message>
        <location line="+215"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>You&apos;re running a demonstration version! The save feature of the data manipulation form is available only in the full version!</source>
        <translation>Vous utilisez une version de démonstration ! La fonctionnalité de sauvegarde des changements n&apos;est disponible que dans la version complète !</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>&lt;strong&gt;WARNING:&lt;/strong&gt; Once commited its not possible to undo the changes! Proceed with saving?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Une fois validés, il ne sera plus possible d&apos;annuler les changements ! Sauvegarder les changements ?</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>delete</source>
        <translation>effacer</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>update</source>
        <translation>mettre à jour</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>insert</source>
        <translation>inserer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="+111"/>
        <source>ASC</source>
        <translation>ASC</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>DESC</source>
        <translation>DESC</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Select all</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Clear all</source>
        <translation>Effacer tout</translation>
    </message>
    <message>
        <location line="+303"/>
        <source>Add empty rows</source>
        <translation>Ajouter des lignes vides</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Clears the items selected on the grid</source>
        <translation>Effacer les valeurs sélectionnées sur la grille</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+R</source>
        <translation>Ctrl+R</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Browse</source>
        <translation>Naviguer</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Load CSV</source>
        <translation>Charger un fichier CSV</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+L</source>
        <translation>Ctrl+L</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Duplicate the selected rows</source>
        <translation>Dupliquer les lignes sélectionnées</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Duplicate</source>
        <translation>Dupliquer</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+D</source>
        <translation>Ctrl+D</translation>
    </message>
    <message>
        <location line="-716"/>
        <source>results (Use &lt;strong&gt;0&lt;/strong&gt; for no limit)</source>
        <translation>résultats (Utiliser &lt;strong&gt;0&lt;/strong&gt; pour ne pas limiter)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/datamanipulationform.cpp" line="-1018"/>
        <source>&lt;strong&gt;WARNING: &lt;/strong&gt; There are some changed rows waiting the commit! Do you really want to discard them and retrieve the data now?</source>
        <translation>&lt;strong&gt;ALERTE :&lt;/strong&gt; Des lignes modifiées attendent d&apos;être soumises ! Voulez-vous vraiment les jeter et récupérer les données maintenant ?</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Rows returned: &lt;strong&gt;%1&lt;/strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;</source>
        <translation>Lignes retournées : &lt;strong&gt;%1&lt;/strong&gt;&amp;nbsp;&amp;nbsp;&amp;nbsp;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>none</source>
        <translation>aucun</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/datamanipulationform.ui" line="+223"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Empty values are assumed as &lt;span style=&quot; font-weight:600;&quot;&gt;DEFAULT&lt;/span&gt;. To use special values like &lt;span style=&quot; font-weight:600;&quot;&gt;NULL&lt;/span&gt;, a function call like &lt;span style=&quot; font-weight:600;&quot;&gt;now()&lt;/span&gt; or a specific data escaping, enclose values in two slashes, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;/value/&lt;/span&gt;. To use a slash as part of the value prepend the backslash character, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;\/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Les valeurs vides sont supposées être &lt;span style=&quot;font-weight :600;&quot;&gt;DEFAULT&lt;/span&gt;. Pour utiliser des valeurs spéciales comme &lt;span style=&quot;font-weight :600;&quot;&gt;NULL&lt;/span&gt;, un appel de fonction comme &lt;span style=&quot;font-weight :600;&quot;&gt;now()&lt;/span&gt; ou un échappement des données spécifiques, entourer les valeurs de deux barres obliques, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;/valeur/&lt;/span&gt;. Pour utiliser une barre oblique dans le cadre de la valeur, ajouter le caractère barre oblique inverse, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="+433"/>
        <source>Copy items on the grid</source>
        <translation>Copier les valeurs de la grille</translation>
    </message>
    <message>
        <location line="+257"/>
        <source>Paste items on the grid</source>
        <translation>Copier les éléments sur la grille</translation>
    </message>
    <message>
        <location line="-348"/>
        <source>Browse referenced tables</source>
        <translation>Parcourir les tables référencées</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Add new rows from a CSV file</source>
        <translation>Ajouter de nouvelles lignes depuis un fichier CSV</translation>
    </message>
    <message>
        <location line="-88"/>
        <source>Change the values of all selected cells at once</source>
        <translation>Modifier les valeurs de toutes les cellules sélectionnées</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+E</source>
        <translation>Ctrl+E</translation>
    </message>
</context>
<context>
    <name>DatabaseExplorerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
    </message>
    <message>
        <location line="+80"/>
        <source>Data &amp;Grid</source>
        <translation>&amp;Grille des données</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+G</source>
        <translation>Alt+G</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Add a new SQL execution tab for the current database</source>
        <translation>Nouvel onglet d&apos;exécution SQL pour la base courante</translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+29"/>
        <location line="+26"/>
        <location line="+179"/>
        <location line="+32"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-253"/>
        <source>Ctrl+T</source>
        <translation>Ctrl+T</translation>
    </message>
    <message>
        <location line="+218"/>
        <source>Expands all items</source>
        <translation>Développer tous les éléments</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Collapses all items</source>
        <translation>Ferme tous les nœuds de l&apos;arborescence</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>By OID</source>
        <translation>Par OID</translation>
    </message>
    <message>
        <location line="+101"/>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="+38"/>
        <source>Attribute</source>
        <translation>Attribut</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="-9"/>
        <source>(not found, OID: %1)</source>
        <translation>(OID: %1 non trouvé)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Source code not generated! Hit F7 or middle-click the item to load it.</source>
        <translation>Code source non généré ! Appuyez sur F7  ou faire un clic central sur l&apos;élément pour le charger.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>By value</source>
        <translation>Par valeur</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Collatable</source>
        <translation>Assemblable</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Definition</source>
        <translation>Définition</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Element</source>
        <translation>Élément</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Encrypted</source>
        <translation>Chiffré</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Enumerations</source>
        <translation>Énumérations</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Language</source>
        <translation>Langage</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Length</source>
        <translation>Longueur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Library</source>
        <translation>Bibliothèque</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Materialized</source>
        <translation>Matérialisée</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Precision</source>
        <translation>Précision</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Preferred</source>
        <translation>Préféré</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Storage</source>
        <translation>Stockage</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Superuser</source>
        <translation>Superutilisateur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tablespace</source>
        <translatorcomment>Jargon</translatorcomment>
        <translation>Tablespace</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Unlogged</source>
        <translatorcomment>Jargon</translatorcomment>
        <translation>Unlogged</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Validity</source>
        <translation>Validité</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Subtype</source>
        <translation>Sous-type</translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+8"/>
        <location line="+1"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>Event</source>
        <translation>Évènement</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Client encoding</source>
        <translation>Codage du client</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Configuration file</source>
        <translation>Fichier de configuration</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Data directory</source>
        <translation>Dossier de données</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Dynamic library path</source>
        <translation>Chemin des bibliothèques dynamiques</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Dynamic shared memory</source>
        <translation>Mémoire partagée dynamique</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Hba file</source>
        <translation>Fichier hba</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Listen addresses</source>
        <translation>Adresses d&apos;écoute</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Max. connections</source>
        <translation>Connexions max.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Listen port</source>
        <translation>Port d&apos;écoute</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Server encoding</source>
        <translation>Encodage serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>SSL ca file</source>
        <translation>Fichier ca SSL</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>SSL cert file</source>
        <translation>Fichier cert SSL</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>SSL crl file</source>
        <translation>Fichier crl SSL</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>SSL key file</source>
        <translation>Fichier clé SSL</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Server version</source>
        <translation>Version serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Ident file</source>
        <translation>Fichier d&apos;identité</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Password encryption</source>
        <translation>Mot de passe de chiffrement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Connection ID</source>
        <translation>ID connexion</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Server PID</source>
        <translation>PID serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Server protocol</source>
        <translation>Protocole serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Identity</source>
        <translation>Identité</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Command</source>
        <translation>Commande</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Last analyze</source>
        <translation>Denière analyse</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Last autovacuum</source>
        <translation>Dernier autovacuum</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Last vacuum</source>
        <translation>Dernier vacuum</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tuples deleted</source>
        <translation>Lignes supprimées</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Tuples updated</source>
        <translation>Lignes mises à jour</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Tuples inserted</source>
        <translation>Lignes insérées</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Partitioned</source>
        <translation>Partitionnée</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Partition of</source>
        <translation>Partition de</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Partition bound expr.</source>
        <translation>Expr. de bornage de la partition</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Dead rows amount</source>
        <translation>Quantité de lignes mortes</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Partition keys</source>
        <translation>Clés de partitionnement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Partitioning</source>
        <translation>Partionnement</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Options</source>
        <translation type="unfinished">Options</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Foreign data wrapper</source>
        <translation>Wrapper de données distantes</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Server</source>
        <translation>Serveur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Bypass RLS</source>
        <translation>Contournement RLS</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Show objects filter</source>
        <translation>Afficher le filtrage</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Show system objects</source>
        <translation>Afficher les objets système</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Show extension objects</source>
        <translation>Afficher les objets d&apos;extension</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Snippets</source>
        <translation>Bouts de code</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Drop object</source>
        <translation>Supprimer l&apos;objet</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Drop cascade</source>
        <translation>Supprimer en cascade</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Truncate</source>
        <translation>Vider</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Trunc. cascade</source>
        <translation>Vider en cascade</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Show data</source>
        <translation>Afficher les données</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reload properties</source>
        <translation>Recharger les propriétés</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Update</source>
        <translation>Mettre à jour</translation>
    </message>
    <message>
        <location line="+835"/>
        <source>-- Source code unavailable for this kind of object --</source>
        <translation>-- Code source indisponible pour ce type d&apos;objet --</translation>
    </message>
    <message>
        <location line="+256"/>
        <source>Do you really want to drop the object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;?</source>
        <translation>Voulez-vous vraiment supprimer l&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; ?</translation>
    </message>
    <message>
        <location line="+735"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; You are about to drop the entire database &lt;strong&gt;%1&lt;/strong&gt; from the server &lt;strong&gt;%2&lt;/strong&gt;! All data will be completely wiped out. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt;Vous êtes sur le point de supprimer entièrement la base de données &lt;strong&gt;%1&lt;/strong&gt; du serveur &lt;strong&gt;%2&lt;/strong&gt; ! Toutes les données seront supprimées. Souhaitez-vous continuer ?</translation>
    </message>
    <message>
        <location line="-1979"/>
        <source>Admin. roles</source>
        <translation>Rôles admin.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Alignment</source>
        <translation>Alignement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Analyze func.</source>
        <translation>Fonc. d&apos;analyse</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Arg. count</source>
        <translation>Nombre d&apos;arg.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Arg. default count</source>
        <translation>Nombre d&apos;arg. par défaut</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Arg. defaults</source>
        <translation>Arg. par défaut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Arg. modes</source>
        <translation>Modes des arg.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Arg. names</source>
        <translation>Noms des arg.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Arg. types</source>
        <translation>Types des arg.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Behavior type</source>
        <translation>Type de comportement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cast type</source>
        <translation>Type de conversion</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Category</source>
        <translation>Catégorie</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Comment</source>
        <translation>Commentaire</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Commutator Op.</source>
        <translation>Commutateur Op.</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Collation</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Conn. limit</source>
        <translation>Limite conn.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Configuration</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Create DB</source>
        <translation>Créer DB</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Create role</source>
        <translation>Créer rôle</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Curr. version</source>
        <translation>Version courante</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default value</source>
        <translation>Valeur par défaut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Delimiter</source>
        <translation>Délimiteur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Dest. type</source>
        <translation>Type de dest.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Dimension</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Directory</source>
        <translation>Répertoire</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Dest. encoding</source>
        <translation>Encodage de dest.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Encoding</source>
        <translation>Encodage</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Exec. cost</source>
        <translation>Coût d&apos;exécution</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Op. family</source>
        <translation>Famille d&apos;op.</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Expression</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Final func.</source>
        <translation>Fonc. finale</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Func. type</source>
        <translation>Type de fonc.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Handler func.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Handles type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Hashes</source>
        <translatorcomment>Jargon</translatorcomment>
        <translation>Hashes</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Index type</source>
        <translation>Type d&apos;index</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inherit</source>
        <translation>Hérité</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Ini. condition</source>
        <translation>Condition ini.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inline func.</source>
        <translation>Func. en ligne</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Input func.</source>
        <translation>Fonc. d&apos;entrée</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Internal length</source>
        <translation>Longueur interne</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Interval type</source>
        <translation>Type d&apos;interval</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>I/O cast</source>
        <translation>Conversion E/S</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Join func.</source>
        <translation>Fonc. de jointure</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>LC COLLATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>LC COLLATE</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>LC CTYPE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>LC CTYPE</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Leak proof</source>
        <translation>Étanche</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Left type</source>
        <translation>Type gauche</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Can login</source>
        <translation>Peut se connecter</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Member roles</source>
        <translation>Rôles des membres</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Merges</source>
        <translation>Fusionne</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Negator op.</source>
        <translation>Négateur op.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Not null</source>
        <translation>Non nul</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Object type</source>
        <translation>Type d&apos;objet</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>With OIDs</source>
        <translation>Avec OIDs</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Old version</source>
        <translation>Ancienne version</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>OID</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>OID</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Operator func.</source>
        <translation>Fonc. de l&apos;opérateur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Output func.</source>
        <translation>Func. de sortie</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Owner</source>
        <translation>Propriétaire</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Owner column</source>
        <translation>Colonne propriétaire</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Parents</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Password</source>
        <translation>Mot de passe</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Permissions</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Range attributes</source>
        <translation>Attributs de plage</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Receive func.</source>
        <translation>Fonc. de réception</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Ref. roles</source>
        <translation>Rôles de ref.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Replication</source>
        <translation>Reproduction</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Restriction func.</source>
        <translation>Fonc. de restriction</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Return type</source>
        <translation>Type de retour</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Returns SETOF</source>
        <translation>Retours SETOF</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Right type</source>
        <translation>Type droit</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Rows amount</source>
        <translation>Montant des lignes</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Security type</source>
        <translation>Type de sécurité</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Send func.</source>
        <translation>Fonc. d&apos;envoi</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Sort op.</source>
        <translation>Opér. de tri</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Source type</source>
        <translation>Type de la source</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Src. encoding</source>
        <translation>Encodage de la src.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>State type</source>
        <translation>Type d&apos;état</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Type mod. in func.</source>
        <translation>Type de mod. dans la fonc.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Type mod. out func.</source>
        <translation>Type de mod. hors fonc.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Transition func.</source>
        <translation>Fonc. de transition</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Trusted</source>
        <translatorcomment>Jargon</translatorcomment>
        <translation>Trusted</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Type attribute</source>
        <translation>Attribut de type</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Types</source>
        <translation>Types</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Validator func.</source>
        <translation>Fonct. de validation</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Windows func.</source>
        <translation>Fonct. de fenêtrage</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>false</source>
        <translation>faux</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>true</source>
        <translation>vrai</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Cache value</source>
        <translation>Valeur du cache</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Increment</source>
        <translation>Incrément</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Cycle</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Max. value</source>
        <translation>Valeur max</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Min. value</source>
        <translation>Valeur min</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Start value</source>
        <translation>Valeur de départ</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Last value</source>
        <translation>Dernière valeure</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Op. class</source>
        <translation>Classe d&apos;opérateur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Canonical func.</source>
        <translation>Fonct. canonique</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Subtype diff func.</source>
        <translation>Fonc de comparaison de sous-type</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Deferrable</source>
        <translation>Reportable</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>For each row</source>
        <translation>Pour chaque ligne</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Firing</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>On insert</source>
        <translatorcomment>Jargon SQL</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+7"/>
        <source>On delete</source>
        <translatorcomment>Jargon SQL</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location line="-6"/>
        <location line="+11"/>
        <source>On update</source>
        <translatorcomment>Jargon SQL</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location line="-11"/>
        <source>On truncate</source>
        <translatorcomment>Jargon SQL</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Arguments</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Table</source>
        <translation>Table</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Trigger func.</source>
        <translation>Fonc. de déclenchement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Condition</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Deferment</source>
        <translation>Ajournement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Execution mode</source>
        <translation>Mode d&apos;exécution</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Commands</source>
        <translation>Commandes</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Comparison type</source>
        <translation>Type de comparaison</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Position</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Ref. columns</source>
        <translation>Colonnes de ref.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Expressions</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Fill factor</source>
        <translation>Facteur de remplissage</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>No inherit</source>
        <translation>Non hérité</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Op. classes</source>
        <translation>Classes d&apos;opérateur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Operators</source>
        <translation>Opérateurs</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Ref. table</source>
        <translation>Table de ref.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unique</source>
        <translation>Unique</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Predicate</source>
        <translation>Prédicat</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Inherited</source>
        <translation>Hérité</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Collations</source>
        <translation></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>SSL</source>
        <translation>SSL</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Referrers</source>
        <translation>Référents</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>USING expr.</source>
        <translation>expr. USING</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>CHECK expr.</source>
        <translation>expr. CHECK</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>RLS enabled</source>
        <translation>RLS activé</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>RLS forced</source>
        <translation>RLS forcé</translation>
    </message>
    <message>
        <location line="+1219"/>
        <source>Do you really want to truncate the table &lt;strong&gt;%1.%2&lt;/strong&gt;?</source>
        <translation>Voulez-vous vraiment vider la table &lt;strong&gt;%1.%2&lt;/strong&gt; ?</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Do you really want to truncate in &lt;strong&gt;cascade&lt;/strong&gt; mode the table &lt;strong&gt;%1.%2&lt;/strong&gt;? This action will truncate all the tables that depends on it?</source>
        <translation>Souhaitez-vous réellement vider en mode &lt;strong&gt;CASCADE&lt;/strong&gt; la table &lt;strong&gt;%1.%2&lt;/strong&gt; ? Cette action poura entraîner des suppressions de lignes dans les tables dépendantes.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Also restart sequences</source>
        <translation>Remettre également les séquences à zéro ?</translation>
    </message>
    <message>
        <location line="+519"/>
        <source>Source code genaration for built-in and base types currently unavailable.</source>
        <translation>La génération du code source pour les objets intégrés et les types n&apos;est pas possible pour le moment.</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>Source code unavailable for the object %1 (%2).</source>
        <translation>Code source indisponible pour l&apos;objet %1 (%2).</translation>
    </message>
    <message>
        <location line="+89"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="-479"/>
        <source>Open the grid to visualize or edit data</source>
        <translation>Ouvrir la grille pour visualiser ou modifier les données</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Update the objects tree</source>
        <translation>Actualiser l&apos;arbre des objets</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Drop this database</source>
        <translation>Supprimer cette base de données</translation>
    </message>
    <message>
        <location line="+276"/>
        <source>Filters the currently loaded items in the tree by using a pattern and matching their names. If &lt;strong&gt;By OID&lt;/strong&gt; is checked the pattern is interpreted as an integer value that represents the object id (OID). &lt;br&gt;&lt;br/&gt;&lt;strong&gt;HINT:&lt;/strong&gt; if you need to search the entire database use the full refresh (&lt;strong&gt;Ctrl+F5&lt;/strong&gt;) prior the filtering.</source>
        <translation>Filtre les éléments actuellement chargés dans l&apos;arborescence en utilisant un motif et en faisant correspondre leurs noms. Si &lt;strong&gt;Par OID&lt;/strong&gt; est coché, le motif est interprété comme une valeur entière qui représente l&apos;identifiant de l&apos;objet (OID).&lt;br&gt;&lt;br/&gt;&lt;strong&gt;ASTUCE :&lt;/strong&gt; si vous devez effectuer une recherche dans toute la base de données, utilisez l&apos;actualisation complète (&lt;strong&gt;Ctrl+F5&lt;/strong&gt;) avant le filtrage.</translation>
    </message>
    <message>
        <location line="+124"/>
        <source>Show raw attributes</source>
        <translation>Noms bruts pour les attributs</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseexplorerwidget.cpp" line="-1822"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Source code</source>
        <translation>Code source</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Quick refresh</source>
        <translation>Actualisation rapide</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Full refresh</source>
        <translation>Actualisation complète</translation>
    </message>
    <message>
        <location line="+1033"/>
        <source>Do you really want to &lt;strong&gt;cascade&lt;/strong&gt; drop the object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;? This action will drop all the other objects that depends on it.</source>
        <translation>Voulez-vous vraiment supprimer en &lt;strong&gt;cascade&lt;/strong&gt; l&apos;objet &lt;strong&gt;%1&lt;/strong&gt;&lt;em&gt; (%2)&lt;/em&gt; ? Cette action supprimera tous les autres objets qui en dépendent.</translation>
    </message>
    <message>
        <location line="+375"/>
        <source>Src. table: %1
Src. column(s): %2</source>
        <translation>Table src. : %1
Colonne(s) src. : %2</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Ref. table: %1
Ref. column(s): %2</source>
        <translation>Table de ref. : %1
Colonne(s) de ref. : %2</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseexplorerwidget.ui" line="-567"/>
        <source>Toggle the display of filter widget as well the system/extension objects.</source>
        <translation>Afficher le widget de filtre ainsi que les objets système/extension.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Sort items alphabetically. When unchecked, items are sorted by OID.</source>
        <translation>Trier les éléments par ordre alphabétique. Lorsque ce n&apos;est pas coché, les éléments sont triés par OID.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Sort alphabetically</source>
        <translation>Trier par ordre alphabétique</translation>
    </message>
</context>
<context>
    <name>DatabaseImportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="+966"/>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location line="-876"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location line="+162"/>
        <source>Import extension objects</source>
        <translation>Importer les objets des extensions</translation>
    </message>
    <message>
        <location line="-138"/>
        <source>Connection:</source>
        <translation>Connexion :</translation>
    </message>
    <message>
        <location line="+164"/>
        <source>Ignore import errors</source>
        <translation>Ignorer les erreurs</translation>
    </message>
    <message>
        <location line="-52"/>
        <source>Import system objects</source>
        <translation>Importer les objets système</translation>
    </message>
    <message>
        <location line="-88"/>
        <source>Automatically resolve dependencies</source>
        <translation>Résoudre automatiquement les dépendances</translation>
    </message>
    <message>
        <location line="+414"/>
        <location line="+38"/>
        <location line="+38"/>
        <location line="+38"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-486"/>
        <source>Debug mode</source>
        <translation>Activer le mode debug</translation>
    </message>
    <message>
        <location line="-121"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location line="+259"/>
        <source>Random colors for relationships</source>
        <translation>Coloriser les relations de manière aléatoire</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Database</source>
        <translation>Base de données</translation>
    </message>
    <message>
        <location line="+209"/>
        <source>Select all objects</source>
        <translation>Tout cocher</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Clear object selection</source>
        <translation>Tout décocher</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Expands all items</source>
        <translation>Développer tous les éléments</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Collapses all items</source>
        <translation>Replier tous les éléments</translation>
    </message>
    <message>
        <location line="-174"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location line="-360"/>
        <source>&lt;p&gt;Resolve some of the object&apos;s dependencies by querying the catalog when a needed object does not exists on the loaded set. In some cases it&apos;s necessary to combine this option with others below. This option does not applies to database level objects like role, tablespace and language as well for data types, extensions.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Résoudre les dépendances de certains objets en intérrogeant le catalogue lorsqu&apos;un objet nécessaire n&apos;existe pas dans le jeu chargé. Dans certains cas il est nécessaire de combiner cette option avec d&apos;autres ci-dessous. Cette option ne s&apos;applique pas aux roles, aux tablespaces, aux langages aux extensions ainsi qu&apos;au types de données.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>&lt;p&gt;All catalog queries as well the created objects&apos; source code are printed to standard output (stdout).&lt;/p&gt;</source>
        <translation>&lt;p&gt;Toutes les requêtes sur le catalogue ainsi que les codes SQL des objets générés seront affichés sur la sortie standard (stdout).&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;Create all imported objects in the current working model instead of create a new one.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Créer les objets importés dans le modèle en cours plutôt que dans créer un nouveau.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&lt;p&gt;Enables the import of system built-in objects. It&apos;s recommend to select only those objects that are directly referenced by the ones to be imported. WARNING: Try to import a huge set of system objects can bloat the resultant model or even crash pgModeler due to memory/cpu overuse.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Active l&apos;importation des objets système. Il est recommandé de les sélectionner seulement ci ces objets sont directement référencés par ceux importés. ATTENTION : Importer un nombre important d&apos;objets système peut saturer le modèle voire même faire planter pgModeler en cas de surconsommation CPU ou mémoire.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&lt;p&gt;Enables the import of objects created by extensions. Generally there is no need to check this option but if there are objects in the database that directly references this category of objects this mode must be enabled.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Active l&apos;importation des objets créés par les extensions. Générallement il n&apos;y a pas lieu d&apos;activer cette option mais s&apos;il existe des objets dans la base de cette catégorie alors ce mode doit être activé.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&lt;p&gt; pgModeler ignores import errors and will try to create as many as possible objects. By checking this option the import operation will not be aborted but an incomplete model will be constructed. This option generates a log file on pgModeler&apos;s temp directory.&lt;/p&gt;</source>
        <translation>&lt;p&gt;pgModeler ignorera les erreurs lors de l&apos;importation et tentera de créer le maximum d&apos;objets possible. En activant cette option l&apos;opération d&apos;importation ne sera pas annulée mais pourra générer un modèle incomplet. Cette option génère un fichier journal dans le répertoire temporaire de pgModeler.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>&lt;p&gt;Random colors will be assigned to imported relationships facilitating the identification of links between tables mainly in large models.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Des couleurs aléatoires seront utilisées pour les relations afin de faciliter l&apos;identification des liens entre les tables pour les gros modèles.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+197"/>
        <source>Filter object by it&apos;s OID</source>
        <translation>Filtrer un objet par son OID</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>By OID</source>
        <translation>Par OID</translation>
    </message>
    <message>
        <location line="+195"/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="+638"/>
        <source>Retrieving objects from database...</source>
        <translation>Récupération des objets depuis la base de données...</translation>
    </message>
    <message>
        <location line="-248"/>
        <source>Importing process aborted!</source>
        <translation>Processus d&apos;import abandonnée !</translation>
    </message>
    <message>
        <location line="-195"/>
        <source>&lt;strong&gt;Low verbosity is set:&lt;/strong&gt; only key informations and errors will be displayed.</source>
        <translation>&lt;strong&gt;Le mode verbeux n&apos;est pas activé :&lt;/strong&gt; seules quelques informations clés et les erreurs seront affichées.</translation>
    </message>
    <message>
        <location line="+306"/>
        <source>Importing process canceled by user!</source>
        <translation>Processus d&apos;import interrompu par l&apos;utilisateur !</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Importing process sucessfuly ended!</source>
        <translation>Processus d&apos;import terminé avec succès !</translation>
    </message>
    <message>
        <location line="+64"/>
        <source>No databases found</source>
        <translation>Aucune base de données trouvée</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Found %1 database(s)</source>
        <translation>%1 base(s) de données trouvée(s)</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Retrieving cluster level objects...</source>
        <translation>Récupération des objets du cluster...</translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Retrieving objects of schema `%1&apos;...</source>
        <translation>Récupération des objets du schéma `%1&apos;...</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Retrieving objects of `%1&apos; (%2)...</source>
        <translation>Réccupération des objets de `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location line="+126"/>
        <source>This is a PostgreSQL built-in data type and cannot be imported.</source>
        <translation>C&apos;est un type de données nativement intégré à PostgreSQL. Il ne peut être importé.</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>This is a pgModeler&apos;s built-in object. It will be ignored if checked by user.</source>
        <translation>C&apos;est un objet nativement intégré à pgModeler. Il sera ignoré s&apos;il est coché par l&apos;utilisateur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="-589"/>
        <source>Import objects to the working model</source>
        <translation>Importer les objets dans le modèle en cours</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimportform.cpp" line="-656"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; You are about to import objects to the current working model! This action will cause irreversible changes to it even in case of critical errors during the process. Do you want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Vous êtes sur le point d&apos;importer des objets dans le modèle actuel ! Cette action entraînera des modifications irréversibles même en cas d&apos;erreurs critiques au cours du processus. Souhaitez-vous poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databaseimportform.ui" line="-200"/>
        <source>Import database</source>
        <translation>Importer une base de données</translation>
    </message>
</context>
<context>
    <name>DatabaseImportHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/databaseimporthelper.cpp" line="+184"/>
        <source>Retrieving system objects... `%1&apos;</source>
        <translation>Récupération des objets système... `%1&apos;</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Retrieving objects... `%1&apos;</source>
        <translation>Récupération des objets... `%1&apos;</translation>
    </message>
    <message>
        <location line="+2165"/>
        <source>Creating table inheritances...</source>
        <translation>Création des tables héritées...</translation>
    </message>
    <message>
        <location line="-1970"/>
        <source>Creating object `%1&apos; (%2)...</source>
        <translation>Création de l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Creating columns permissions...</source>
        <translation>Création des permissions sur les champs...</translation>
    </message>
    <message>
        <location line="+1990"/>
        <source>Destroying unused detached columns...</source>
        <translation>Destruction des colonnes détachées inutilisées...</translation>
    </message>
    <message>
        <location line="-1872"/>
        <source>The database import ended but some errors were generated and saved into the log file `%1&apos;. This file will last until pgModeler quit.</source>
        <translation>L&apos;import de la base de données est términée mais des erreurs ont été générées et sauvegardées dans le journal `%1&apos;. Ce fichier sera disponible jusqu&apos;à la fermeture de pgModeler.</translation>
    </message>
    <message>
        <location line="-334"/>
        <source>Retrieving columns of table `%1.%2&apos;, oid `%3&apos;...</source>
        <translation>Récupération des colonnes de la table `%1.%2&apos;, oid `%3&apos;...</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Creating object `%1&apos; (%2), oid `%3&apos;...</source>
        <translation>Création de l&apos;objet `%1&apos; (%2), oid `%3&apos;...</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Trying to recreate object `%1&apos; (%2), oid `%3&apos;...</source>
        <translation>Tentative de création de l&apos;objet `%1&apos; (%2), oid `%3&apos;...</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Import failed to recreate some objects in `%1&apos; tries.</source>
        <translation>L&apos;import a échoué à recréer certains objets à `%1&apos; reprise(s).</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Creating permissions for object `%1&apos; (%2)...</source>
        <translation>Création des autorisations pour l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Updating relationships of `%1&apos; (%2)...</source>
        <translation>Mise à jour des relations de `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Validating relationships...</source>
        <translation>Validation des relations...</translation>
    </message>
    <message>
        <location line="+1857"/>
        <source>Creating table partitionings...</source>
        <translation>Création des tables partitionnées...</translation>
    </message>
    <message>
        <location line="+219"/>
        <source>Assigning sequences to columns...</source>
        <translation>Affectation des séquences aux colonnes...</translation>
    </message>
</context>
<context>
    <name>DatabaseModel</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="+377"/>
        <source>The demonstration version can create only `%1&apos; instances of each object type! You&apos;ve reach this limit for the type: `%2&apos;</source>
        <translation>Cette version de démonstration peut seulement créer `%1&apos; instances pour chaque type d&apos;objet ! Cette limite a été atteinte pour les objets de type : `%2&apos;</translation>
    </message>
    <message>
        <location line="+2909"/>
        <source>Loading: `%1&apos; (%2)</source>
        <translation>Chargement : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+47"/>
        <location line="+7"/>
        <location line="+5"/>
        <source>Validating relationships...</source>
        <translation>Validation des relations...</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rendering database model...</source>
        <translation>Création du rendu du modèle...</translation>
    </message>
    <message>
        <location line="+3917"/>
        <source>Generating %1 code: `%2&apos; (%3)</source>
        <translation>Génération du code %1 : `%2&apos; (%3)</translation>
    </message>
    <message>
        <location line="+3108"/>
        <source>Saving object `%1&apos; (%2)</source>
        <translation>Enregistrement de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+163"/>
        <source>Saving metadata of the object `%1&apos; (%2)</source>
        <translation>Enregistrement des métadonnées de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Metadata file successfully saved!</source>
        <translation>Fichier de métadonnées enregistré avec succès !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Process successfully ended but no metadata was saved!</source>
        <translation>Le processus s&apos;est terminé avec succès mais aucune métadonnée n&apos;a été enregistrée !</translation>
    </message>
    <message>
        <location line="+82"/>
        <source>Creating object `%1&apos; (%2)</source>
        <translation>Création de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Object `%1&apos; (%2) already exists. Ignoring.</source>
        <translation>L&apos;objet `%1&apos; (%2) existe déjà et a été ignoré.</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Loading metadata for object `%1&apos; (%2)</source>
        <translation>Chargement des métadonnées de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+129"/>
        <source>Object `%1&apos; (%2) not found. Ignoring metadata.</source>
        <translation>Objet`%1&apos; (%2) introuvable. Métadonnée ignorée.</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Metadata file successfully loaded!</source>
        <translation>Fichier de métadonnées chargé avec succès !</translation>
    </message>
</context>
<context>
    <name>DatabaseWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="+39"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Template DB:</source>
        <translation>Modèle de BDD :</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Model Author:</source>
        <translation>Auteur du modèle :</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Encoding:</source>
        <translation>Encodage :</translation>
    </message>
    <message>
        <location line="+223"/>
        <source>Default Objects</source>
        <translation>Attributs par défaut des objets</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Tablespace:</source>
        <translation type="unfinished">Tablespace :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Schema:</source>
        <translation>Schéma :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Owner:</source>
        <translation>Propriétaire :</translation>
    </message>
    <message>
        <location line="-226"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE :</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Connections:</source>
        <translation>Connexions :</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="+37"/>
        <source>The fields &lt;strong&gt;LC_COLLATE&lt;/strong&gt; and &lt;strong&gt;LC_CTYPE&lt;/strong&gt; have pre-configured values based upon the running system. You can freely modify those values if you intend to export the model to another host.</source>
        <translation>Les champs &lt;strong&gt;LC_COLLATE&lt;/strong&gt; et &lt;strong&gt;LC_CTYPE&lt;/strong&gt; ont déjà des valeurs pré-configurées basées sur le système. Vous pouvez librement les modifier si vous devez exporter le modèle vers une autre machine hôte.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Use the above fields to specify the default attributes assigned to new objects created on the database model. Leaving a field empty will cause PostgreSQL to use the default values when exporting the model.</source>
        <translation>Utilisez les champs ci-dessus pour spécifier les attributs par défaut à assigner aux nouveaux objets créés dans le modèle. Laisser un champ vide fera que PostgreSQL utilisera ses valeurs par défaut lors de l&apos;export du modèle.</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+12"/>
        <location line="+68"/>
        <location line="+5"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="+42"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Allow connections</source>
        <translation>Autoriser les connexions</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Is template</source>
        <translation>Est le modèle</translation>
    </message>
</context>
<context>
    <name>DomainWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="+95"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location line="-18"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Not null</source>
        <translation>Non nulle</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Check constraints</source>
        <translation>Vérifier les contraintes</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Expression:</source>
        <translation>Expression :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/domainwidget.cpp" line="+42"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Expression</source>
        <translation></translation>
    </message>
</context>
<context>
    <name>DonateWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/donatewidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Donate to pgModeler</source>
        <translation>Faire un don à pgModeler</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;pgModeler is brought to you thanks to a &lt;span style=&quot; font-style:italic;&quot;&gt;great effort to create and distribute a quality product&lt;/span&gt;. This project is reaching out levels of maturity never imagined. All this is the result of a joint work between its author and the &lt;span style=&quot; font-weight:600;&quot;&gt;Open Source community&lt;/span&gt;. &lt;br/&gt;&lt;br/&gt;This software has a long way to go yet and with your help we&apos;ll keep maintaining the good job and bringing new improvements on each release. If you did like pgModeler and thinks it deserves a contribution please make a donation!&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;pgModeler vous est offert grâce à un &lt;span style=&quot; font-style :italic;&quot;&gt;gros effort pour créer et distribuer un produit de qualité &lt;/span&gt;. Ce projet atteint un niveau de maturité jamais imaginé. Tout ceci est le résultat d&apos;un travail conjoint entre son auteur et la &lt;span style=&quot;font-weight :600;&quot;&gt;Communauté Open Source&lt;/span&gt;.&lt;br/&gt;&lt;br/&gt;Ce logiciel a encore un long chemin à parcourir mais avec votre aide, nous continuerons à faire du bon travail et à apporter de nouvelles améliorations à chaque version. Si vous avez aimé pgModeler et pensez qu&apos;il mérite une contribution, faites un don ! &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="+49"/>
        <source>I want to help!</source>
        <translation>Je veux aider !</translation>
    </message>
</context>
<context>
    <name>ElementWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementwidget.ui" line="+20"/>
        <source>Element properties</source>
        <translation>Propriétés de l&apos;élément</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Operator Class:</source>
        <translation>Classe d&apos;opérateur :</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Sorting:</source>
        <translation>Tri :</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Ascending</source>
        <translation>Ascendant</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Descending</source>
        <translation>Descendant</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Nulls first</source>
        <translation>Null en premier</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Operator:</source>
        <translation>Opérateur :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Expression:</source>
        <translation>Expression :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/elementwidget.cpp" line="+162"/>
        <source>Index element properties</source>
        <translation>Propriétés de l&apos;élément de l&apos;index</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Exclude element properties</source>
        <translation>Propriétés de l&apos;élément d&apos;exclusion</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Partition key properties</source>
        <translation>Propriétés de clé de partition</translation>
    </message>
</context>
<context>
    <name>ElementsTableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/elementstablewidget.cpp" line="+40"/>
        <source>Element</source>
        <translation type="unfinished">Élément</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Type</source>
        <translation type="unfinished">Type</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Operator</source>
        <translation type="unfinished">Opérateur</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Operator Class</source>
        <translation type="unfinished">Classe d&apos;opérateurs</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Collation</source>
        <translation type="unfinished">Collation</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sorting</source>
        <translation type="unfinished">Tri</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Nulls First</source>
        <translation type="unfinished">Vides en premier</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Expression</source>
        <translation type="unfinished">Expression</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Ascending</source>
        <translation type="unfinished">Ascendant</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Descending</source>
        <translation type="unfinished">Descendant</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Yes</source>
        <translation type="unfinished">Oui</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>No</source>
        <translation type="unfinished">Non</translation>
    </message>
</context>
<context>
    <name>EventTriggerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/eventtriggerwidget.ui" line="+29"/>
        <source>Event:</source>
        <translation>Évènement :</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Function:</source>
        <translation>Fonction :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Tag:</source>
        <translation>Étiquette :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/eventtriggerwidget.cpp" line="+37"/>
        <source>Tag command</source>
        <translation>Balise de la commande</translation>
    </message>
</context>
<context>
    <name>Exception</name>
    <message>
        <location filename="../libutils/src/exception.cpp" line="+23"/>
        <source>Assignment of a pseudo-type to the type of the column!</source>
        <translation>Utilisation d&apos;un pseudo-type comme type de la colonne !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a precision greater than the length of the type!</source>
        <translation>Utilisation d&apos;une précision est plus grande que la longueur du type !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an invalid precision to type time, timestamp or interval. The precision in this case must be equal to or less than 6!</source>
        <translation>Utilisation d&apos;une précision de type time, timestamp ou interval invalide. La précision de ces types doit être inférieure ou égale à 6 !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to a column which index is out of the capacity of the column list!</source>
        <translation>Référence à une colonne dont l&apos;index dépasse la capacité de la liste des colonnes !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of not allocated object!</source>
        <translation>Utilisation d&apos;un objet non référencé !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Assigning object of an invalid type!</source>
        <translation>Assignation d&apos;un objet dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Removing an object of an invalid type!</source>
        <translation>Suppression d&apos;un objet dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Obtaining an object of an invalid type!</source>
        <translation>L&apos;objet obtenu est de type invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of empty name to table return type!</source>
        <translation>Affectation d&apos;un nom vide au type retourné par la table !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reference to an event which does not belongs to trigger!</source>
        <translation>Référence à un évènement qui n&apos;appartient pas au déclencheur !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Assignment of a function which language is invalid!</source>
        <translation>Définition d&apos;une fonction dont le langage est invalide !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Assignment of empty name to an object!</source>
        <translation>Affectation d&apos;un nom vide à l&apos;objet !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Assignment of schema object which type is invalid!</source>
        <translation>Affectation d&apos;un schema dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of tablespace object with invalid type!</source>
        <translation>Utilisation d&apos;un espace logique dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of tablespace to an invalid object!</source>
        <translation>Affectation d&apos;un espace logique à un objet invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of tablespace to a constraint which type is invalid! To belong to a tablespace the constraint must be a primary key or unique!</source>
        <translation>Affectation d&apos;un espace logique à une contrainte dont le type est invalide ! Pour appartenir à un espace logique une contrainte doit être une clé primaire ou unique !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of owner object which type is invalid!</source>
        <translation>Affectation d&apos;un propiétaire dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of owner to an invalid object!</source>
        <translation>Affectation d&apos;un propriétaire à un objet invalide !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to a function with invalid type!</source>
        <translation>Référence à une fonction dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an argument of the operator with invalid type!</source>
        <translation>Référence à un argument de l&apos;opérateur dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an operator with invalid type!</source>
        <translation>Référence à un opérateur dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of value to an invalid option type on role!</source>
        <translation>Utilisation d&apos;un attribut invalide pour un rôle !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid role type!</source>
        <translation>Référence à un type de rôle invalide !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Insertion of empty command to the rule!</source>
        <translation>Insertion d&apos;une commande vide dans cette règle !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Assignment of an object that already belongs to another table!</source>
        <translation>Utilisation d&apos;un objet appartenant déjà à une autre table !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a schema to the sequence which differs from the schema of the owner table!</source>
        <translation>Affectation d&apos;un schéma à la séquence diffèrent du schéma de la table !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an invalid value to one of the sequence attributes!</source>
        <translation>Utilisation d&apos;une valeur invalide comme attribut de séquence !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a minimum value to the sequence which is greater than the maximum value!</source>
        <translation>La valeur minimale de la séquence est supérieure à la valeur maximale !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of a null increment value to the sequence!</source>
        <translation>La valeur de l&apos;incrément de la séquence est nulle !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of null cache value to the sequence!</source>
        <translation>La valeur cache de la séquence est nulle !</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Assignment of not allocated language!</source>
        <translation>Utilisation d&apos;un langage inconnu !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of language object which type is invalid!</source>
        <translation>Définition d&apos;un objet langage dont le type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to data type with an index outside the capacity of data types list!</source>
        <translation>Référence à un type de donnée dont l&apos;index est en dehors de la capacité de la liste des types de données !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of invalid type to the object!</source>
        <translation>Affectation d&apos;un type invalide à l&apos;objet !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Obtaining types with invalid quantity!</source>
        <translation>Utilisation d&apos;une quantité de types invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of item which already exists in the attributes list of the type!</source>
        <translation>Insertion d&apos;un attribut déjà présent dans la définition de ce type !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of invalid item in the attributes list of the type!</source>
        <translation>Insertion d&apos;un attribut invalide à la liste des attributs de ce type !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of item which already exists in the enumarations list of the type!</source>
        <translation>Insertion d&apos;un élément déjà existant dans l&apos;énumeration !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of invalid item in the enumerations list of the type!</source>
        <translation>Insertion d&apos;un élément invlaide dans l&apos;énumeration !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Assignment of invalid configuration to the type!</source>
        <translation>La configuration assigné au type est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an operator which input type count is invalid to aggregate function!</source>
        <translation>Utilisation d&apos;un opérateur dont le nombre d&apos;argument est invalide avec une fonction d&apos;agrégation !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an operator which types of arguments is invalid!</source>
        <translation>Utilisation d&apos;un opérateur dont le type des arguments est invalide !</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>There is already a relationship between `%1&apos; (%2) and `%3&apos; (%4) in the model! When using relationships of the type generalization, copy and one-to-one there can&apos;t be other relationships linked to the pair of tables.</source>
        <translation>Il y a déjà une relation entre `%1 &apos;(%2) et `%3&apos; (%4) dans le modèle ! Lors de l&apos;utilisation de relations de type généralisation, copie et un-à-un, il ne peut y avoir d&apos;autres relations liées à la paire de tables.</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Unable to load the configuration file `%1&apos;! Please check if file exists in its folder and/or if it is not corrupted!</source>
        <translation>Impossible de charger le fichier de configuration `%1&apos;! Merci de vérifier que le fichier existe dans son dossier et / ou s&apos;il n&apos;est pas corrompu !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not find the default settings file `%1&apos;! To restore default settings check the existence of the file and try again!</source>
        <translation>Impossible de trouver le fichier des préférences par défaut `%1&apos; ! Pour restaurer les préférences par défaut veuillez vérifier l&apos;existance de ce fichier et réessayez !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Could not load the plugin `%1&apos; from the library `%2&apos;! Message returned by plugin manager: `%3&apos;</source>
        <translation>Impossible de charger l&apos;extension `%1&apos; depuis la bibliothèque`%2&apos; ! Message retourné par le gestionnaire d&apos;extension : `%3&apos;</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Failed to drop the database `%1&apos; because it is defined as the default database for the connection `%2&apos;!</source>
        <translation>Impossible de supprimer la base de données `%1 &apos;car elle est définie comme base de données par défaut pour la connexion`%2&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The column `%1&apos; must be `NOT NULL&apos; because it composes the primary key of the table `%2&apos;. You need to remove the column from the mentioned contraint in order to disable the `NOT NULL&apos; on it!</source>
        <translation>La colonne `%1&apos; doit être `NOT NULL&apos; car elle compose la clé primaire de la table `%2&apos;. Vous devez supprimer la colonne de la contrainte mentionnée afin de désactiver le `NOT NULL&apos; dessus !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The identity column `%1&apos; has an invalid data type! The data type must be `smallint&apos;, `integer&apos; or `bigint&apos;.</source>
        <translation>La colonne d&apos;identité `%1&apos; a un type de données invalide ! Le type de données doit être `smallint&apos;, `integer&apos; ou `bigint&apos;.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid affected command in policy `%1&apos;!</source>
        <translation>Référence à une commande affectée invalide dans la règle `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid special role in policy `%1&apos;!</source>
        <translation>Référence à un rôle spécial invalide dans la politique `%1&apos; !</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>Assignment of a second definition expression to the view!</source>
        <translation>Assignation d&apos;une seconde expression à la vue !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of collation object which type is invalid!</source>
        <translation>Utilisation d&apos;un objet collation invalide !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Collations must be created at least with attributes LC_COLLATE and LC_CTYPE defined!</source>
        <translation>Les collations doivent être définies avec les attributs LC_COLLATE et LC_CTYPE renseignés !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reference to an invalid copy table option!</source>
        <translation>Référence à une option invalide de copie de table !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The INSTEAD OF mode cannot be used on triggers that belongs to tables! This is available only for view triggers!</source>
        <translation>Le mode AU LIEU DE (INSTEAD OF) ne peut être utilisé comme déclencheur de table ! Ce n&apos;est valable que pour les déclencheurs de vue !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The TRUNCATE event can only be used when the trigger executes for each statement and belongs to a table!</source>
        <translation>L&apos;événement TRUNCATE ne peut être utilisé que lors d&apos;un déclencheur sur chaque ligne et que s&apos;il appartient à une table !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The INSTEAD OF mode cannot be used on view triggers that executes for each statement!</source>
        <translation>Le mode AU LIEU DE (INSTEAD OF) ne peut être utilisé avec des déclencheurs de vues qui s’exécutent à chaque requête !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Constraint triggers can only be executed on AFTER events and for each row!</source>
        <translation>Les déclencheurs de contraintes ne peuvent être exécutés qu&apos;APRÈS les événements et pour chaque ligne !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A view trigger cannot be AFTER/BEFORE when it executes for each row!</source>
        <translation>Les déclencheurs de vues ne peuvent être AFTER/BEFORE quand il sont configurés pour une exécution sur chaque ligne !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A trigger cannot make reference to columns when using INSTEAD OF mode and UPDATE event!</source>
        <translation>Un déclencheur ne peut faire référence aux colonnes quand il est utilisé en mode INSTEAD OF sur un événement de type UPDATE !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Only constraint triggers can be deferrable or reference another table!</source>
        <translation>Seuls les déclencheurs de contraintes peuvent être déférés ou référencer une autre table !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The validation process failed due to an error triggered by the validation helper. For more details about the error check the exception stack!</source>
        <translation>La procédure de validation a échouée à cause d&apos;une erreur levée par l&apos;assistant de validation. Pour de plus d&apos;informations, referez-vous à la pile des exceptions !</translation>
    </message>
    <message>
        <location line="-117"/>
        <source>Assignment of an invalid strategy/support number to an operator class element!</source>
        <translation>Affectation d&apos;un numéro de stratégie/support invalide à un élément de la classe &apos;operator&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of element which already exists in the element list!</source>
        <translation>Insertion d&apos;un élément déjà existant dans la liste des éléments !</translation>
    </message>
    <message>
        <location line="-66"/>
        <source>Reference to a parameter which index is out of the parameter list bounds!</source>
        <translation>Référence un paramètre dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Reference to an argument which index is out of argument list bounds!</source>
        <translation>Référence un argument dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of a name which contains invalid characters!</source>
        <translation>Utilisation de caractères invalides dans le nom !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a name which length exceeds the maximum of 63 characters!</source>
        <translation>Utilisation de plus de 63 caractères dans le nom !</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Reference to a role which index is out of role list bounds!</source>
        <translation>Référence un rôle dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to a command which index is out of the command list bounds!</source>
        <translation>Référence une commande dont l&apos;index est hors des limites de la liste de ces dernières !</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Assignment of a start value to the sequence which is extrapolating the range defined by minimum and maximum values!</source>
        <translation>Affectation de la valeur de départ de la séquence en dehors de la plage de cette dernière définie par les valeurs min et max !</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Reference to a label which index is out of labels list bounds!</source>
        <translation>Référence un label dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Reference to an attribute which index is out of the attributes list bounds!</source>
        <translation>Référence un attribut dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an enumeration which index is out of the enumerations list bounds!</source>
        <translation>Référence une énumération dont l&apos;index est hors des limites de la liste de ces dernières !</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Reference to an element which index is out of element list bounds!</source>
        <translation>Référence un élément dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an object which index is out of object list bounds!</source>
        <translation>Référence un objet dont l&apos;index est hors des limites de la liste de ces derniers !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Removal of an object not allocated!</source>
        <translation>Suppression d&apos;un objet non alloué !</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>The object `%1&apos; (%2) can not be manipulated because it is reserved to PostgreSQL! This object is present in the database model only as a reference!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut être modifié car il est est réservé à PostgreSQL ! Cet objet apparaît dans le modèle uniquement comme référence !</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Operation with object(s) which type(s) is invalid!</source>
        <translation>Opérations sur un ou des objets dont les types sont invalides !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to object with invalid type!</source>
        <translation>Référence à un objet ayant un type invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation with object not allocated!</source>
        <translation>Opération sur des objets non alloués !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The relationship of the type 1-1 where both tables are mandatory participation is not implemented because it requires fusion between the tables that breaks the modeling done by the user!</source>
        <translation>Une association de type 1-1, quand les deux tables sont obligatoires, n&apos;est pas implémentée car cela requiert la fusion des tables, ce qui casse le modèle réalisé par l&apos;utilisateur !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an invalid expression to the object!</source>
        <translation>Affectation d&apos;une expression invalide à l&apos;objet !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a primary key to a table which already has one!</source>
        <translation>Impossible de définir une clé primaire à une table qui en possède déjà une !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Identifier relationship can not be created for a self relationship, relationships of the type n-n, copy or generalization!</source>
        <translation>La relation d&apos;identifiant ne peut pas être créée pour une relation vers elle même, pour les relations de type n-n, pour la copie ou pour la généralisation!</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A foreign key can not be added to a relationship because is created automatically when this is connected!</source>
        <translation>Une clé étrangère ne peut être ajoutée à une relation car elle est créée automatiquement lors de la mise en relation !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to an user-defined data type that not exists in the model!</source>
        <translation>Référence à un type de donné défini par l&apos;utilisateur qui n&apos;existe pas dans le modèle !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid maximum size to operation list!</source>
        <translation>La taille maximum affectée à la liste d&apos;opérations est invalide !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>One or more objects were invalidated and automatically removed because they were referencing table columns which were included through relationships and which no longer exists due to disconnection of relationships or exclusion of such generated columns!</source>
        <translation>Un ou plusieurs objets ont été invalidés et automatiquement supprimés car ils référençaient des colonnes qui étaient incluses dans des associations et qui n&apos;existaient plus du fait de la suppression de l&apos;associations ou de l&apos;exclusion de ces colonnes !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to an invalid privilege type!</source>
        <translation>Référence à un type de privilège invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of a role which already exists in the role list of the permission!</source>
        <translation>Insertion d&apos;un rôle déjà existant dans la liste de cette permission !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of privilege incompatible with the type of object referenced by permission!</source>
        <translation>Utilisation d&apos;un privilège incompatible avec le type d&apos;objet référencé par la permission !</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Assignment of invalid name to the table generated from N-N relationship!</source>
        <translation>Le nom de la table générée par la relation N-N est invalide !</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Reference to a column of the objects table with invalid index!</source>
        <translation>Référence à une colonne de la table ayant un index invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a row of the objects table with invalid index!</source>
        <translation>Référence à une ligne de la table ayant un index invalide !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Constraints like primary key, foreign key or unique must have at least one column related to them! For foreign keys must be selected, in addition, the referenced columns!</source>
        <translation>Les contraintes telles que clé primaire, clé étrangère ou unicité doivent avoir au moins une colonne associée ! Pour les clés étrangères, les colonnes référencées doivent également être sélectionnées !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>The export process failed due to an error triggered by the PostgreSQL server in an attempt to execute a SQL command. For more details about the error check the exception stack!

** Executed SQL command: **

%1</source>
        <translation>Le process d&apos;exportat a échoué suite à une erreur déclenchée par le serveur PostgreSQL lors de la tentative d&apos;exécution d&apos;une commande SQL. Pour plus de détails concernant l&apos;erreur vérifiez la pile d&apos;exceptions !

** Commande SQL exécutée: **

%1</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>One or more plugins were not activated due to errors during the loading process! Check the exception stack for more details.</source>
        <translation>Une ou plusieurs extensions n&apos;ont pas été activées suite à la survenue d&apos;erreurs lors du chargement. Pour plus de détails, consultez la pile d&apos;exceptions.</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Assignment of empty XML buffer to parser!</source>
        <translation>Passage d&apos;un tampon XML vide à l&apos;analyseur !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of empty DTD file name!</source>
        <translation>Le nom du fichier DTD est vide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of empty name to the DTD declaration!</source>
        <translation>Le nom de la déclaration DTD est vide !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Operation on unallocated element tree! It is necessary to load the XML parser buffer and interpret it so that the tree is generated!</source>
        <translation>Opération sur l&apos;arbre d&apos;élément non alloué ! Il est nécessaire de charger le tampon d&apos;analyseur XML et de l&apos;interpréter pour que l&apos;arbre soit généré !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Operation with unallocated tree element!</source>
        <translation>Opération avec un élément de l&apos;arbre non alloué !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation with element which does not exists in the element tree currently loaded!</source>
        <translation>Opération avec un élément inexistant dans l&apos;arbre des éléments actullement chargé !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a value to an invalid connection parameter!</source>
        <translation>Affectation d&apos;une valeur à un paramètre de connexion invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation on connection not established!</source>
        <translation>Opération sur une connexion non établie !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attempt to connect without define configuration parameters!</source>
        <translation>Tentative de connexion sans paramètre de configuration défini !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Assignment of not allocated SQL command result!</source>
        <translation>Affectation d&apos;un résultat de commande SQL non alloué !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to allocate the result of the SQL command because the response from the DBMS was not understood by the client!</source>
        <translation>Impossible de retourner le résultat de la commande SQL car la réponse du SGBD n&apos;a pas été comprise par le client !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to a column of tuple with invalid index!</source>
        <translation>Référence à une colonne de tuple dont l&apos;index est invalide !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a column of tuple with invalid name!</source>
        <translation>Référence à une colonne de tuple dont le nom est invalide !</translation>
    </message>
    <message>
        <location line="-168"/>
        <source>Assignment of a not allocated column to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une colonne non allouée à l&apos;objet `%1 &apos;(%2) !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Assignment of a not allocated schema to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un schéma non allouée à l&apos;objet `%1 &apos;(%2) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) has inconsistent SQL or XML definition!</source>
        <translation>L&apos;objet `%1 &apos;(%2) a une définition SQL ou XML incohérente !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) already exists on `%3&apos; (%4)!</source>
        <translation>L&apos;objet `%1 &apos;(%2) existe déjà sur `%3&apos; (%4) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) cannot be assigned because there is already exists in the container object `%3&apos;!</source>
        <translation>L&apos;objet `%1 &apos;(%2) ne peut être assigné car il existe déjà dans l&apos;objet conteneur `%3&apos; !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The insertion of the parameter `%1&apos; will not be possible because there is another parameter with same name in the function `%2&apos;!</source>
        <translation>L&apos;insertion du paramètre `%1&apos; ne sera pas possible car il existe un autre paramètre avec le même nom dans la fonction `%2&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The insertion of the table return type `%1&apos; will not be possible because there is another return type with the same name in the `%2&apos;!</source>
        <translation>L&apos;insertion du type de retour de la table `%1&apos; ne sera pas possible car il existe un autre type de retour avec le même nom dans le`%2&apos; !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>The column `%1&apos; cannot be assigned to the trigger `%2&apos; because they belongs to different parent tables!</source>
        <translation>La colonne `%1&apos; ne peut pas être affectée au déclencheur `%2&apos; car ils appartiennent à des tables parentes différentes !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a not allocated function to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction non allouée à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function which return type is different from `%1&apos;!</source>
        <translation>Affectation d&apos;une fonction dont le type de retour est différent de `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function which parameter count is invalid to the object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction dont le nombre de paramètres n&apos;est pas valide pour l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Event trigger function must be coded in any language other than SQL!</source>
        <translation>La fonction de déclenchement d&apos;événement doit être codée dans n&apos;importe quelle langue autre que SQL !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of not allocated table to object `%1&apos; (%2)!</source>
        <translation>Affectation de la table non allouée à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Assignment of appended or prepended SQL to an invalid object!</source>
        <translation>Affectation de code SQL ajouté ou préfixé à un objet invalide !</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The insertion of the role `%1&apos; is not possible because this is already being referenced by role `%2&apos;!</source>
        <translation>L&apos;insertion du rôle `%1 n&apos;est pas possible car il est déjà référencé par le rôle `%2&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference redundancy detected by having the role `%1&apos; referencing the role `%2&apos;!</source>
        <translation>Redondance de références détectée en ayant le rôle `%1&apos; référençant le rôle `%2&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The role `%1&apos; can not be listed as a member of itself!</source>
        <translation>Le rôle `%1&apos; ne peut pas être listé comme un membre de lui-même !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>It&apos;s not possible to create a self generalization/copy/partition relationship! The table can not inherit or copy its own attributes or be a partition of itself!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Assignment of owner table which is not in the same schema as the sequence `%1&apos;!</source>
        <translation>Affectation de la table propriétaire qui n&apos;est pas dans le même schéma que la séquence `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of owner table which does not belong to the same owner of the sequence `%1&apos;!</source>
        <translation>Affectation de la table propriétaire qui n&apos;appartient pas au même propriétaire de la séquence `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a nonexistent owner column to the sequence `%1&apos;!</source>
        <translation>Affectation d&apos;une colonne propriétaire inexistante à la séquence `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an owner column to the sequence `%1&apos; that is not related to any table!</source>
        <translation>Affectation d&apos;une colonne propriétaire à la séquence `%1&apos; qui n&apos;est liée à aucune table !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allocation of an object with invalid type!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function with invalid return type to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction avec un type de retour invalide à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function with invalid parameter(s) type(s) to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une fonction avec un (des) paramètre(s) non valide(s) à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Assignment of an empty directory to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un répertoire vide à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Assignment of system reserved name to the object `%1&apos; (%2)!</source>
        <translation>Affectation du nom réservé au système à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>One function with invalid configuration is been used by the object `%1&apos; (%2)!</source>
        <translation>Une fonction avec une configuration invalide est utilisée par l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The object `%1&apos; (%2) can not be removed because it is being referenced by object `%3&apos; (%4)!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être supprimé car il est référencé par l&apos;objet `%3&apos; (%4) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) can not be removed because it is being referenced by object `%3&apos; (%4) that belongs to `%5&apos; (%6)!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être supprimé car il est référencé par l&apos;objet `%3&apos; (%4) appartenant à `%5&apos; (%6) !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The creation of the relationship `%1&apos; between the table `%2&apos; and `%3&apos; can not be done because one does not have a primary key. If the relationship is of the type n-n both tables must have primary keys!</source>
        <translation>La création de la relation `%1&apos; entre la table `%2&apos; et `%3&apos; ne peut pas être effectuée car il n&apos;y a pas de clé primaire. Si la relation est du type n-n, les deux tables doivent avoir des clés primaires !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Unable to create a copy relationship because the column `%1&apos; in table `%2&apos; already exists in table `%3&apos;!</source>
        <translation>Impossible de créer une relation de copie car la colonne `%1&apos; de la table `%2&apos; existe déjà dans la table `%3&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to create the generalization relationship because the column `%1&apos; in table `%2&apos; can not be merged with the column `%3&apos; of table `%4&apos; because they have incompatible types!</source>
        <translation>Impossible de créer la relation de généralisation car la colonne `%1&apos; de la table `%2&apos; ne peut pas être fusionnée avec la colonne `%3&apos; de la table `%4&apos; car ils ont des types incompatibles !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>An attribute can not be added to a copy, generalization or partitioning relationship!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The object `%1&apos; (%2) is referencing the object `%3&apos; (%4) which was not found in the model!</source>
        <translation>L&apos;objet `%1&apos; (%2) fait référence à l&apos;objet `%3&apos; (%4) qui n&apos;a pas été trouvé dans le modèle !</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>The primary key `%1&apos; can only be allocated if declared within a block of code that defines a table or relationship!</source>
        <translation>La clé primaire `%1&apos; ne peut être allouée que si elle est déclarée dans un bloc de code qui définit une table ou une relation !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>There is already a permission on object `%1&apos; (%2) which has one or more equal roles from those present on permission to be assigned to the object!</source>
        <translation>Il y a déjà une permission sur l&apos;objet `%1&apos; (%2) qui a un ou plusieurs rôles égaux à ceux présents dans l&apos;autorisation à assigner à l&apos;objet !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A permission is referencing the object `%1&apos; (%2) which was not found in the model!</source>
        <translation>Une permission fait référence à l&apos;objet `%1&apos; (%2) qui n&apos;a pas été trouvé dans le modèle !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) can not be created because its not being assigned to any schema!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être créé car il n&apos;est affecté à aucun schéma !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The tablespace `%1&apos; can not be inserted into the model because it points to the same directory as the tablespace `%2&apos;!</source>
        <translation>L&apos;espace de stockage `%1&apos; ne peut pas être inséré dans le modèle car il pointe sur le même répertoire que l&apos;espace de table `%2&apos; !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The function `%1&apos; can not get a source code as a definition because its language is set to C. Use the attributes symbol and dynamic library instead!</source>
        <translation>La fonction `%1&apos; ne peut pas obtenir un code source en tant que définition car sa langue est définie sur C. Utilisez plutôt le symbole d&apos;attributs et la bibliothèque dynamique !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The function `%1&apos; can have the attributes symbol and dynamic library configured only if the language is set to C. For all other cases you must specify a source code that defines it in the DBMS!</source>
        <translation>La fonction `%1&apos; peut avoir le symbole des attributs et la bibliothèque dynamique configurés uniquement si la langue est définie sur C. Dans tous les autres cas, il faut spécifier un code source qui le définit dans le SGBD !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The operator `%1&apos; can not be assigned as negator of operator `%2&apos; because it has incompatible settings!</source>
        <translation>L&apos;opérateur `%1&apos; ne peut pas être affecté en tant que négateur de l&apos;opérateur `%2&apos; car il a des paramètres incompatibles !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The type `%1&apos; can not self refer in the attributes `element&apos; or `copy type&apos; or be used as a data type of an attribute in the configuration of a composite type!</source>
        <translation>Le type `%1&apos; ne peut pas se référer à lui-même dans les attributs `element&apos; ou `copy type&apos; ou être utilisé comme type de données d&apos;un attribut dans la configuration d&apos;un type composite !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid element to type `%1&apos;!</source>
        <translation>Affectation d&apos;un élément invalide de type `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid alignment to type `%1&apos;!</source>
        <translation>Affectation de l&apos;alignement invalide de type `%1&apos; !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The relationship `%1&apos; can not make use of the special primary key because it is marked as identifier or it is a self relationship!</source>
        <translation>La relation `%1&apos; ne peut pas utiliser la clé primaire spéciale car elle est marquée comme identifiant ou c&apos;est une association d&apos;elle-même !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) can not be edited or deleted because it was automatically included through a relationship! If the object is an attribute or constraint the modifications must be done on the relationship editing form.</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être modifié ou supprimé car il a été automatiquement inclus dans une association ! Si l&apos;objet est un attribut ou une contrainte, les modifications doivent être effectuées sur le formulaire d&apos;édition de la relation.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) can not be deleted because it is protected!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être supprimé car il est protégé !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group `%1&apos; has already been declared earlier!</source>
        <translation>Le groupe `%1&apos; a déjà été déclaré !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group `%1&apos; can not be built in the groups declaration block (%2)!</source>
        <translation>Le groupe `%1&apos; ne peut pas être construit dans le bloc de déclaration de groupe (%2) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group `%1&apos; was built but not declared in the groups declaration block (%2)!</source>
        <translation>Le groupe `%1&apos; a été construit mais pas déclaré dans le bloc de déclaration de groupe (%2) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group `%1&apos; can not be built without possessing child elements!</source>
        <translation>Le groupe `%1&apos; ne peut pas être construit sans posséder d&apos;éléments enfants !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group `%1&apos; can not be built once more because this was done in previous blocks!</source>
        <translation>Le groupe `%1&apos; ne peut pas être construit une fois de plus car cela a été fait dans les blocs précédents !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group `%1&apos; has been declared but not built!</source>
        <translation>Le groupe `%1&apos; a été déclaré mais pas construit !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The new configuration of the function invalidates the object `%1&apos; (%2)! In this case it is needed to undo the relationship between the affected object and function in order to the new configuration to take effect!</source>
        <translation>La nouvelle configuration de la fonction invalide l&apos;objet `%1&apos; (%2) ! Dans ce cas, il est nécessaire d&apos;annuler la relation entre l&apos;objet affecté et la fonction afin que la nouvelle configuration prenne effet !</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Error while interpreting XML buffer at line %1 column %2.
Message generated by the parser: %3. %4</source>
        <translation>Erreur lors de l&apos;interprétation du tampon XML à la ligne %1, colonne %2.
Message généré par l&apos;analyseur : %3. %4</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Attempt to start a connection already stablished!</source>
        <translation>Tentative de lancement d&apos;une connexion déjà établie !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not connect to the database.
Message returned: `%1&apos;</source>
        <translation>Impossible de se connecter à la base de données.
Message renvoyé : `%1&apos;</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unable to allocate command result for the SQL because the server has generated a fatal error!
Message returned by the DBMS: `%1&apos;</source>
        <translation>Impossible d&apos;allouer le résultat de la commande SQL car le serveur a généré une erreur fatale !
Message renvoyé par le SGBD : `%1&apos;</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Reference to a column of a tuple which was not yet initialized (tuple navigation not started)!</source>
        <translation>Référence à une colonne d&apos;un tuple qui n&apos;a pas encore été initialisé (navigation en tuple non démarrée) !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not execute the SQL command.
 Message returned: `%1&apos;</source>
        <translation>Impossible d&apos;exécuter la commande SQL.
 Message renvoyé : `%1&apos;</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>It is not possible mix ordinary references (SELECT-FROM, FROM-WHERE, After WHERE) with references used as view SQL definition!</source>
        <translation>Il n&apos;est pas possible de mélanger des références ordinaires (SELECT-FROM, FROM-WHERE, Après WHERE) avec des références utilisées comme vue de définition SQL !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The object `%1&apos; (%2) cannot reference itself! This operation is not permitted for this kind of object!</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas se référencer lui-même ! Cette opération n&apos;est pas autorisée pour ce genre d&apos;objet !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only operator families which uses `btree&apos; as indexing method are accepted by operator class elements!</source>
        <translation>Seules les familles d&apos;opérateurs qui utilisent `btree &apos;comme méthode d&apos;indexation sont acceptées par les éléments de classe d&apos;opérateur !</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Assignment of a column which has no parent table to the object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;une colonne qui n&apos;a pas de table parent à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>The operator class assigned to the object `%1&apos; (%2) must use `btree&apos; as indexing method!</source>
        <translation>La classe d&apos;opérateur affectée à l&apos;objet `%1&apos; (%2) doit utiliser` btree&apos; comme méthode d&apos;indexation !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>The extension `%1&apos; is registered as a data type and cannot have the attribute `handles datatype&apos; modified!</source>
        <translation>L&apos;extension `%1&apos; est enregistrée en tant que type de données et ne peut pas avoir l&apos;attribut `handle datatype&apos; modifié !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The fk relationship `%1&apos; cannot be created because the foreign-key that represents it was not created on table `%2&apos;!</source>
        <translation>L&apos;association clé étrangère `%1&apos; ne peut pas être créée car la clé étrangère qui la représente n&apos;a pas été créée sur la table `%2&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignement of an invalid object name pattern to the relationship `%1&apos;!</source>
        <translation>Affectation d&apos;un modèle de nom d&apos;objet invalide à la relation `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid object name pattern id on the relationship `%1&apos;!</source>
        <translation>Référence à un identifiant de modèle de nom d&apos;objet invalide sur la relation `%1&apos; !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Mixing incompatibles DBMS export modes: `ignore object duplications&apos;, `drop database&apos; or `drop objects&apos; cannot be used with `simulate export&apos;!</source>
        <translation>Mélange incompatibles des modes d&apos;export du SGBD : `ignore object duplications&apos;, `drop database&apos; ou `drop objects&apos; ne peuvent être utilisés avec `simulate export&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Mixing incompatibles DROP options: `drop database&apos; and `drop objects&apos; cannot be used at the same time!</source>
        <translation>Mixage incompatible des options DROP : `drop database&apos; et `drop objects&apos; ne peuvent pas être utilisés en même temps !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid object id swapping operation! The objects involved are the same!</source>
        <translation>Opération d&apos;échange d&apos;identifiant d&apos;objet invalide ! Les objets impliqués sont les mêmes !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid object id swapping operation! The database itself, tablespaces or roles cannot have the ids swapped!</source>
        <translation>Opération d&apos;échange d&apos;identifiant d&apos;objet invalide ! La base de données elle-même, les espaces de stockage ou les rôles ne peuvent pas échanger leurs identifiants !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The widget already has a parent and cannot be assigned to a different object!</source>
        <translation>Le widget a déjà un parent et ne peut pas être assigné à un objet différent !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Could not load the database model file `%1&apos;. Check the error stack to see details. Try to run `pgmodeler-cli --fix-model&apos; in order to correct the structure of the file if that is the case.</source>
        <translation>Impossible de charger le fichier de modèle de base de données `%1&apos;. Vérifiez la pile d&apos;erreurs pour plus de détails. Essayez d&apos;exécuter `pgmodeler-cli --fix-model&apos; afin de corriger la structure du fichier si besoin.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The column `%1&apos; cannot reference it&apos;s parent table `%2&apos; as data type!</source>
        <translation>La colonne `%1&apos; ne peut pas référencer sa table parent `%2&apos; comme type de données !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation with an invalid element id `%1&apos;!</source>
        <translation>Opération avec un identifiant d&apos;élément invalide `%1&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid color id `%1&apos; for element `%2&apos;!</source>
        <translation>Référence à un identifiant de couleur invalide `%1&apos; pour l&apos;élément `%2&apos; !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The sequence `%1&apos; can&apos;t be assigned to the column `%2&apos; because the data type of the latter is incompatible. The type used must be an integer one!</source>
        <translation>La séquence `%1&apos; ne peut pas être affectée à la colonne `%2&apos; car le type de données de cette dernière est incompatible. Le type utilisé doit être un entier !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The option to generate temporary object names can only be used in simulation mode!</source>
        <translation>L&apos;option de génération de noms d&apos;objets temporaires ne peut être utilisée qu&apos;en mode simulation !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Could not assign the variable `%1&apos; to event trigger&apos;s filter. Currently, PostgreSQL supports only the `TAG&apos; variable!</source>
        <translation>Impossible d&apos;affecter la variable `%1 &apos;au filtre du déclencheur d&apos;événement. Actuellement, PostgreSQL supporte uniquement la variable `TAG&apos; !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Malformed unescaped value on row `%1&apos; column `%2&apos;!</source>
        <translation>Valeur non echappée malformée à la ligne `%1&apos;, colonne `%2&apos; !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The object `%1&apos; (%2) can&apos;t be handled because some needed fields are not set! Please, make sure to fill at least the requires fields in order to properly create or update the object.</source>
        <translation>L&apos;objet `%1&apos; (%2) ne peut pas être manipulé car certains champs nécessaires ne sont pas définis ! Merci de vous assurer de remplir au moins les champs requis afin de créer ou de mettre à jour correctement l&apos;objet.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A relationship can only be swapped by other object of the same kind!</source>
        <translation>Une association ne peut être permutée que par un autre objet du même genre !</translation>
    </message>
    <message>
        <location line="-157"/>
        <source>Assignment of a null type to object `%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un type nul à l&apos;objet `%1&apos; (%2) !</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Unable to create the generalization relationship because the constraint `%1&apos; in table `%2&apos; can not be merged with the constraint `%3&apos; of table `%4&apos; due to their incompatible composition!</source>
        <translation>Impossible de créer l&apos;association de généralisation car la contrainte `%1&apos; de la table `%2&apos; ne peut pas être fusionnée avec la contrainte `%3&apos; de la table `%4&apos; en raison de leurs compositions incompatibles !</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to write the file or directory `%1&apos;! Make sure that the path exists and the user has write permissions over it!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to write the file `%1&apos; due to one or more errors in the definition generation process!</source>
        <translation>Impossible d&apos;écrire le fichier `%1&apos; en raison d&apos;une ou de plusieurs erreurs dans le processus de génération de définition !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The configuration of the relationship `%1&apos; generates a redundancy between the relationships `%2&apos;. Redundancy on identifier or generalization/copy relationships are not accepted since they result in incorrect column spreading making the model inconsistent!</source>
        <translation>La configuration de la relation `%1&apos; génère une redondance entre les associations `%2 . La redondance sur l&apos;identificateur ou les relations de généralisation/copie de l&apos;association ne sont pas acceptées car elles entraînent une répartition incorrecte des colonnes, ce qui rend le modèle incohérent !</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>It is not possible to create arrays sequences (dimension &gt;= 1)! PostgreSQL does not yet implement this feature!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>The operator `%1&apos; can not be assigned as a commutator of operator `%2&apos; because it has incompatible settings!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>A view reference should be used in at least one of these SQL scopes: View Definition, SELECT, FROM, WHERE or GROUP/HAVING!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Invalid syntax in file `%1&apos;, line %2, column %3!</source>
        <translation>Syntaxe non valide dans le fichier `%1&apos;, ligne %2, colonne %3!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid instruction `%1&apos; on file `%2&apos;, line %3, column %4!</source>
        <translation>Instruction invalide `%1&apos; dans le fichier `%2&apos;, ligne `%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unknown attribute `%1&apos; in file `%2&apos;, line %3, column %4!</source>
        <translation>Attribut inconnu `%1&apos; dans le fichier `%2&apos;, ligne `%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid metacharacter `%1&apos; in file `%2&apos;, line %3, column %4!</source>
        <translation>Métacaractère invalide `%1&apos; dans le fichier `%2&apos;, ligne `%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid operator `%1&apos; in comparison expression, file `%2&apos;, line %3, column %4!</source>
        <translation>Opérateur invalide `%1&apos; dans l&apos;expression de comparaison, fichier `%2&apos;, ligne`%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attribute `%1&apos; with an undefined value in file `%2&apos;, line %3, column %4!</source>
        <translation>L&apos;attribut `%1&apos; a une valeur non définie dans le fichier `%2&apos;, ligne`%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attribute `%1&apos; with an invalid name in file `%2&apos;, line %3, column %4!</source>
        <translation>L&apos;attribut `%1&apos; a un nom invalide dans le fichier `%2&apos;, ligne`%3&apos;, colonne `%4&apos; !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Could not access the file or directory `%1&apos;! Make sure that it exists or if the user has access permissions on it!</source>
        <translation>Impossible d&apos;acceder au fichier ou fossier `%1&apos; ! Vérifiez qu&apos;il existe et que l&apos;utilisateur à les droits d&apos;écritures !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Could not load file `%1&apos;. The same appears to be inconsistent or one of its dependencies (DTD files) has errors or is missing!</source>
        <translation>Impossible de charger le fichier `%1&apos;. La même chose semble être incohérente ou l&apos;une de ses dépendances (fichiers DTD) a des erreurs ou est manquante !</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Reference to a tuple with an invalid index or the result is empty (no tuples)!</source>
        <translation>Référence à un tuple avec un index invalide ou le résultat est vide (pas de tuple) !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Invalid use of a view reference as whole SQL definition! The assigned reference must be an expression!</source>
        <translation>Utilisation invalide d&apos;une référence de vue comme définition SQL entière ! La référence affectée doit être une expression !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>At the moment pgModeler does not support the creation of primary keys which some columns were generated by relationship connection. To create primary keys with this feature you can use the field `Identifier&apos; or the tab `Primary key&apos; on relationship editing form!</source>
        <translation>Actuellement pgModeler ne supporte pas la création de clés primaires dont certaines colonnes ont été générées par la connection d&apos;associations. Pour créer des clés primaires avec cette fonctionnalité, vous pouvez utiliser le champ `Identifier&apos; ou l&apos;onglet `Primary key&apos; sur le formulaire d&apos;édition de relation !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>The copy relationship between the tables `%1&apos; and `%2&apos; cannot be done because the first one already copies attributes from `%3&apos;! Tables can have only one copy table!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The paritioning relationship between the tables `%1&apos; and `%2&apos; cannot be done because the first one is already a partition of the table `%3&apos;! Partition tables can be participating of only one partition hierarchy at a time!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The relationship between the tables `%1&apos; and `%2&apos; can&apos;t be created because one of the entities is part of a partitioning hierachy! The table `%3&apos; can&apos;t be used in `generalization&apos;, `copy&apos; and `one-to-one&apos; relationships. In `one-to-many&apos; and `many-to-many&apos; relationships the mentioned table can&apos;t be referenced by the generated foreign key(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Reference to a function id which is incompatible with the user define type configuration!</source>
        <translation>Référence à un identifiant de fonction incompatible avec la configuration du type de définition de l&apos;utilisateur !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Unsupported PostgreSQL version (%1) detected! Valid versions are between %2 and %3.</source>
        <translation>Version PostgreSQL non prise en charge (%1) détectée!  Les versions valides sont comprises entre %2 et %3.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Invalid use of variadic parameter mode! This mode can be used only with an array or &quot;any&quot; data type!</source>
        <translation>Utilisation invalide du mode paramètre variadique ! Ce mode ne peut être utilisé qu&apos;avec un tableau ou un type de données &quot;any&quot; !</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>The object `%1&apos; (%2), oid `%3&apos;, could not be imported due to one or more errors! Check the exception stack for more details. `HINT:&apos; if the object somehow references objects in `pg_catalog&apos; or `information_schema&apos; consider enable the importing of system objects.</source>
        <translation>L&apos;objet `%1 &apos;(%2), oid `%3&apos;, n&apos;a pu être importé en raison d&apos;au moins une erreur ! Vérifiez la pile d&apos;exceptions pour plus de détails. `REMARQUE : &apos;Si l&apos;objet référence d&apos;une manière ou d&apos;une autre les objets dans` pg_catalog&apos; ou `information_schema &apos;, envisagez d&apos;importer les objets système.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Assignment of an invalid object to `%1&apos; (%2)! The assigned object must be of type `%3&apos;.</source>
        <translation>Affectation d&apos;un objet invalide à `%1&apos; (%2) ! L&apos;objet affecté doit être de type `%3&apos;.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>It&apos;s not possible convert the type of the column `%1&apos; to serial! It must have an `integer&apos; based type and its default value must be a call to `nextval(seq_name::regclass)&apos; function or a sequence object must be directly assigned to the column!</source>
        <translation>Il n&apos;est pas possible de convertir le type de la colonne `%1&apos; en série ! Il doit avoir un type basé sur `integer&apos; et sa valeur par défaut doit être un appel à la fonction `nextval (seq_name::regclass)&apos; ou un objet séquence doit être directement assigné à la colonne !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Could not perform the `%1&apos; operation on `%2&apos; using the data on row `%3&apos;! All changes were rolled back. 

 ** Returned error ** 

%4</source>
        <translation>Impossible d&apos;exécuter l&apos;opération `%1 &apos;sur `%2&apos; en utilisant les données de la ligne `%3 &apos;! Tous les changements ont été annulés.

  ** Erreur retournée **

%4</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Trying to undo/redo an invalid operation over an object that does not exists anymore or can&apos;t be handled! The operation history will be cleaned up.</source>
        <translation>Tentative d&apos;annulation/de rétablissement d&apos;une opération invalide sur un objet qui n&apos;existe plus ou ne peut plus être géré ! L&apos;historique des opérations sera nettoyé.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>A parent table of `%1&apos; which OID is `%2&apos; was not found in the set of imported objects!</source>
        <translation>Une table parente de `%1&apos; dont l&apos;OID est `%2&apos; n&apos;a pas été trouvée dans l&apos;ensemble des objets importés !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The enumeration `%1&apos; can&apos;t be assigned to the type `%2&apos; because contains invalid characters!</source>
        <translation>L&apos;énumération `%1&apos; ne peut pas être affectée au type `%2&apos; car elle contient des caractères non valides !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The enumeration `%1&apos; can&apos;t be assigned to the type `%2&apos; because is too long!</source>
        <translation>L&apos;énumération `%1&apos; ne peut pas être affectée au type `%2&apos; car elle est trop longue !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The connection was idle for too long and was automatically closed!</source>
        <translation>La connexion était inactive depuis trop longtemps et a été automatiquement fermée !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The connection was unexpectedly closed by the database server `%1&apos; at port `%2&apos;!</source>
        <translation>La connexion a été fermée de manière inattendue par le serveur de la base de données `%1&apos; sur le port `%2&apos; !</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Unable to create a partition relationship because the partion table `%1&apos; is not empty or has columns that are not present on the partitioned table `%2&apos;!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to create a partition relationship between the tables `%1&apos; (partition) and `%2&apos; (partitioned) because no partitioning type is defined on the latter!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid amount of partition keys being assinged to the table `%1&apos;! Multiples partition keys are allowed only on `HASH&apos; and `RANGE&apos; partitioning strategies.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A partition key of the table `%1&apos; is referencing the object `%3&apos; (%4) which was not found in the model!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The column `%1&apos; can&apos;t be assigned to a partition key because it was created by a relatinship and this kind of operation is not yet supported! HINT: create the column manually on the table and then create the partition key using it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The column `%1&apos; on the table `%2&apos; can&apos;t be removed because it is being referenced by one or more patition keys!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an option to the object with an invalid name!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an invalid name to the object reference!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a not allocated object to the object reference!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object reference name `%1&apos; is already defined!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A zero-byte file was detected while saving to `%1&apos;. In order to avoid data loss the original contents of the file prior to the last saving was restored and a security copy kept on `%2&apos;. You can copy that backup file to a safe place as a last resort to avoid the complete data loss! Note that the backup file will be erased when the application is closed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object `%1&apos; (%2) can&apos;t be assigned to the foreign table `%3&apos; because it&apos;s unsupported! Foreign tables only accepts columns, check constraints and triggers.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The creation of the relationship `%1&apos; between the tables `%2&apos; and `%3&apos; can&apos;t be done because one of the entities is a foreign table. Foreign tables can only be part of a inheritance, copy or partitioning relationship!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The creation of the copy relationship `%1&apos; between the tables `%2&apos; and `%3&apos; can&apos;t be done because a foreign table is not allowed to copy table columns!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Failed to save the data dictionary into `%1&apos;! Make sure that the provided path points to a directory or if the user has write permissions over it!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+198"/>
        <source>** Another %1 error(s) were suppressed due to stacktrace size limits.

</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>ExtensionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="+38"/>
        <source>Version:</source>
        <translation>Version :</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Old Version:</source>
        <translation>Ancienne version :</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>This attribute cannot be changed once the object is created.</source>
        <translation>Cet attribut ne peut être modifié une fois l&apos;objet créé.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Handles data type</source>
        <translation>Gérer les types de données</translation>
    </message>
</context>
<context>
    <name>FindReplaceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/findreplacewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+172"/>
        <source>Hide this widget</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation type="unfinished">...</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>Replace one occurrence</source>
        <translation>Remplace une occurrence</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Replace</source>
        <translation>Remplacer</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Replace all occurrences</source>
        <translation>Remplace toutes les occurrences</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Replace All</source>
        <translation>Remplacer tout</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Replace the selection and find the next one</source>
        <translation>Remplacer l&apos;occurrence courante et trouver le suivant</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Replace &amp;&amp; Find</source>
        <translation>Remplacer &amp;&amp; Suivant</translation>
    </message>
    <message>
        <location line="-87"/>
        <source>Replace:</source>
        <translation>Remplacer par :</translation>
    </message>
    <message>
        <location line="-173"/>
        <source>Find:</source>
        <translation>Recherche :</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Find previous</source>
        <translation>Précédent</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Shift+F3</source>
        <translation>Maj+F3</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Find next</source>
        <translation>Suivant</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Case sensitive</source>
        <translation>Respecter la casse</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Regular expression</source>
        <translation>Expression régulière</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Whole words</source>
        <translation>Mots entier</translation>
    </message>
</context>
<context>
    <name>ForeignDataWrapperWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/foreigndatawrapperwidget.ui" line="+47"/>
        <source>Validator:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Handler:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Options</source>
        <translation type="unfinished">Options</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/foreigndatawrapperwidget.cpp" line="+32"/>
        <source>The handler function must have the following signature:  &lt;strong&gt;fdw_handler&lt;/strong&gt; &lt;em&gt;function_name&lt;/em&gt;()</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The validator function must have the following signature: &lt;em&gt;function_name&lt;/em&gt;(&lt;strong&gt;text[]&lt;/strong&gt;,&lt;strong&gt;oid&lt;/strong&gt;). The return type of ths function is ignored.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Option</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Value</source>
        <translation type="unfinished">Valeur</translation>
    </message>
</context>
<context>
    <name>ForeignServerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/foreignserverwidget.ui" line="+50"/>
        <source>Version:</source>
        <translation type="unfinished">Version :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>FDW:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Type:</source>
        <translation type="unfinished">Type :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Options</source>
        <translation type="unfinished">Options</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/foreignserverwidget.cpp" line="+41"/>
        <source>Option</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Value</source>
        <translation type="unfinished">Valeur</translation>
    </message>
</context>
<context>
    <name>ForeignTable</name>
    <message>
        <location filename="../libpgmodeler/src/foreigntable.cpp" line="+28"/>
        <source>new_foreign_table</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>FunctionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="+60"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Function Type:</source>
        <translation>Type de fonction :</translation>
    </message>
    <message>
        <location line="+255"/>
        <source>Execution Cost:</source>
        <translation>Coût d&apos;exécution :</translation>
    </message>
    <message>
        <location line="-197"/>
        <source>Rows Returned:</source>
        <translation>Lignes retournées :</translation>
    </message>
    <message>
        <location line="+94"/>
        <source>Behavior:</source>
        <translation>Comportement :</translation>
    </message>
    <message>
        <location line="-184"/>
        <source>Security:</source>
        <translation>Sécurité :</translation>
    </message>
    <message>
        <location line="+165"/>
        <source>Return Method:</source>
        <translation>Type de retour :</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Set</source>
        <translation>Multiple</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Return Table</source>
        <translation>Table de retour</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Windown Func.</source>
        <translation>Fonct. fenêtrage.</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Leakproof</source>
        <translation>Leakproof</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Parameters</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Definition</source>
        <translation>Définition</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>Dynamic Library:</source>
        <translation>Bibliothèque dynamique :</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Symbol:</source>
        <translation>Symbole :</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Library:</source>
        <translation>Bibliothèque :</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Source code:</source>
        <translation>Code source :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="+53"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+8"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Mode</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Value</source>
        <translation>Valeur par défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="-327"/>
        <source>Si&amp;mple</source>
        <translation>Si&amp;mple</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Tab&amp;le</source>
        <translation>Tab&amp;le</translation>
    </message>
    <message>
        <location line="-126"/>
        <source>Language:</source>
        <translation>Langage :</translation>
    </message>
</context>
<context>
    <name>GeneralConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+1969"/>
        <source>Milimeters</source>
        <translation>Millimètres</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Pixels</source>
        <translation>Pixels</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Inches</source>
        <translation>Pouces</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Centimeter</source>
        <translation>Centimètres</translation>
    </message>
    <message>
        <location line="-440"/>
        <source>A0 (841 x 1189 mm)</source>
        <translation>A0 (841 x 1189 mm)</translation>
    </message>
    <message>
        <location line="-1516"/>
        <source>General &amp;&amp; Design</source>
        <translation>Général &amp;&amp; modélisation</translation>
    </message>
    <message>
        <location line="+1151"/>
        <source>Check if there is a new version on server</source>
        <translation>Vérifier la présence d&apos;une nouvelle version au lancement</translation>
    </message>
    <message>
        <location line="-1120"/>
        <source>Design</source>
        <translation>Modélisation</translation>
    </message>
    <message>
        <location line="+478"/>
        <source>Save and restore last position and zoom</source>
        <translation>Sauvegarder la position et le zoom</translation>
    </message>
    <message>
        <location line="+207"/>
        <source>Disable render smoothness</source>
        <translation>Désactiver l&apos;anticrénelage</translation>
    </message>
    <message>
        <location line="-121"/>
        <source>Hide the object that represents the relationship name</source>
        <translation>Masque l&apos;étiquette indiquant le nom de chaque association.</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Validate before save, export or diff</source>
        <translation>Validation du modèle exigée pour sauvegarder, exporter ou comparer</translation>
    </message>
    <message>
        <location line="-46"/>
        <source>Hide the object which represents the tag assigned to the table</source>
        <translation>Masquer l&apos;objet qui représente la balise affectée à la table.</translation>
    </message>
    <message>
        <location line="+121"/>
        <source>Enable SQL code completion</source>
        <translation>Activer la complétion du code SQL</translation>
    </message>
    <message>
        <location line="+515"/>
        <source>Printing &amp;&amp; Code</source>
        <translation>Impression &amp;&amp; édition</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Size:</source>
        <translation>Taille :</translation>
    </message>
    <message>
        <location line="+113"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location line="-100"/>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <location line="-662"/>
        <source>Hide relationship name</source>
        <translation>Masquer le nom des associations</translation>
    </message>
    <message>
        <location line="-46"/>
        <source>Hide table extended attributes</source>
        <translation>Masquer les attributs étendus des tables</translation>
    </message>
    <message>
        <location line="+938"/>
        <source>Paper:</source>
        <translation>Papier :</translation>
    </message>
    <message>
        <location line="+187"/>
        <source>Orientation:</source>
        <translation>Orientation :</translation>
    </message>
    <message>
        <location line="-929"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="-173"/>
        <source>Hide table tags</source>
        <translation>Masquer les balises des tables</translation>
    </message>
    <message>
        <location line="+852"/>
        <source>Printing</source>
        <translation>Impression</translation>
    </message>
    <message>
        <location line="-141"/>
        <location line="+159"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location line="-1382"/>
        <source>Attributes per page:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>&lt;p&gt;These spinners define the minimum amount of attributes visible per page in each section of tables and views when the pagination is enabled on them. The first one controls the amount of columns per page. The second defines the amount of extended attributes (constraints, indexes, trigger, rules, policies) displayed per page.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+50"/>
        <source>&lt;p&gt;Defines the minimum amount of attributes visible per page on tables and view when the pagination is enabled on these objects.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+70"/>
        <source>&lt;p&gt;Defines the minimum opacity percentage applied to the objects when using the fade out feature. A zero opacity causes the object to be completely hidden not being possible to interact with it in the canvas area.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+51"/>
        <source>&lt;p&gt;Defines the period when the opened models will be saved automatically. The temporary models saving period will be defined as the half of the value defined here. If the autosave is disabled the temporary models saving will occur in a period of 5 minutes.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+67"/>
        <source>Operation history size:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>&lt;p&gt;Defines the maximum amount of elements held in the operation history. Once reached the maximum number the history is automatically cleaned.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+67"/>
        <source>&lt;p&gt;Defines the vertical and horizontal grid size. This value affects the spacing of objects when using the feature that aligns the objects on the grid. &lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+45"/>
        <source>&lt;p&gt;Graphical objects (table, views and textboxes) will be created in a single step without the need to click on canvas &lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Simplify the creation of graphical objects</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;When enabled this option creates a placeholder object at the previous table&apos;s position when starting to move it. This will cause graphical updates on relationship lines to be performed only when the drag &amp; drop action is done improving the performance. Disabling placeholders will cause those updates to be executed every time the table&apos;s position changes a single pixel (classical behavior).&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;After loading the model the last zoom and position on canvas will be restored&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;By default the range selection is triggered with Shift + left click. By checking this option range selection will be activated only with a single click and move.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Hide the portion of table which represents triggers, indexes and rules.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+60"/>
        <source>&lt;p&gt;Triggers a dialog asking the user to validate the model before a save, export or diff operation.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Move canvas by keeping the mouse on corners</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+66"/>
        <source>&lt;p&gt; Disable the antialiasing for lines and texts improving performance when handling huge models.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;Forces the escaping of line breaks and tabulations in objects&apos; comments during SQL code generation. This option produces a more compact SQL code since all comments will be presented in one line. Despite changing the presentation this option doesn&apos;t affect the final semantics of multilined comments.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Escape objects&apos; comments in SQL code</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Save/restore dialogs sizes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Reset the dialogs sizes and positions to their default values.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reset sizes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+345"/>
        <source>&lt;p&gt;Reduces the verbosity of the export, import and diff process causing only key info messages and errors to be displayed. This option causes less rendering operations to be performed when displaying progress messages and, as a consequence, it makes these processes run faster.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Low verbosity for the export, import and diff processes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+289"/>
        <source>Print grid</source>
        <translation>Imprimer la grille</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>A1 (594 x 841 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A2 (420 x 594 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A3 (297 x 420 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A4 (210 x 297 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A5 (148 x 210 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A6 (105 x 148 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A7 (74 x 105 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A8 (52 x 74 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A9 (37 x 52 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B0 (1030 x 1456 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B1 (728 x 1030 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B10 (32 x 45 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B2 (515 x 728 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B3 (364 x 515 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B4 (257 x 364 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B5 (182 x 257 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B6 (128 x 182 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B7 (91 x 128 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B8 (64 x 91 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B9 (45 x 64 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>C5E (163 x 229 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Comm10E (105 x 241 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>DLE (110 x 220 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Executive (191 x 254 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Folio (210 x 330 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Ledger (432 x 279 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Legal (216 x 356 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Letter (216 x 279 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Tabloid (279 x 432 mm)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Custom</source>
        <translation>Personnalisé</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Unity:</source>
        <translation>Unité :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Custom Size:</source>
        <translation>Taille papier :</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Width:</source>
        <translation>Largeur :</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Height:</source>
        <translation>Hauteur :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Page Margins:</source>
        <translation>Marges :</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Left:</source>
        <translation>Gauche :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Left margin</source>
        <translation>Marge de gauche</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Top:</source>
        <translation>Haut :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Top margin</source>
        <translation>Marge du haut</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Right:</source>
        <translation>Droite :</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Right margin</source>
        <translation>Marge de droite</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bottom:</source>
        <translation>Bas :</translation>
    </message>
    <message>
        <location line="-32"/>
        <source>Bottom margin</source>
        <translation>Marge du bas</translation>
    </message>
    <message>
        <location line="-197"/>
        <source>Landscape</source>
        <translation>Paysage</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Portrait</source>
        <translation>Portrait</translation>
    </message>
    <message>
        <location line="-232"/>
        <source>Print page numbers</source>
        <translation>Imprimer les numéros de page</translation>
    </message>
    <message>
        <location line="-839"/>
        <source>Start move the canvas when the cursor is on the canvas edges</source>
        <translation>Commencer à déplacer le canevas lorsque le curseur se trouve sur les bords de celui-ci.</translation>
    </message>
    <message>
        <location line="-152"/>
        <source>Use placeholders when moving tables</source>
        <translation>Utiliser des espaces réservés lors du déplacement de tables</translation>
    </message>
    <message>
        <location line="+201"/>
        <source>Toggles the code completion in all fields that accepts the input of SQL commands.</source>
        <translation>(Dés)active l&apos;achèvement du code dans tous les champs qui acceptent les commandes SQL en entrée.</translation>
    </message>
    <message>
        <location line="+539"/>
        <source>Code style</source>
        <translation>Style du code</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Colors:</source>
        <translation>Couleurs :</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Display line numbers</source>
        <translation>Afficher les numéros de ligne</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Highlight lines at cursor&apos;s position</source>
        <translation>Surligner les lignes à la position du curseur</translation>
    </message>
    <message>
        <location line="+72"/>
        <source>Custom tab width:</source>
        <translation>Largeur d&apos;onglet personnalisé :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/generalconfigwidget.cpp" line="+48"/>
        <source>Line numbers&apos; font color</source>
        <translation>Couleur de la police des numéros de ligne</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Line numbers&apos; background color</source>
        <translation>Couleur d&apos;arrière-plan des numéros de ligne</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Highlighted line color</source>
        <translation>Couleur de la ligne en surbrillance</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>The little brown fox jumps over the lazy dog</source>
        <translation>Le petit renard brun saute par-dessus le chien paresseux</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>System default</source>
        <translation>Défaillance du système</translation>
    </message>
    <message>
        <location line="+515"/>
        <source>All files (*.*)</source>
        <translation>Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Load file</source>
        <translation>Charger un fichier</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>This action will reset all dialogs to their default size and positions on the screen! Do you really want to proceed?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/generalconfigwidget.ui" line="-1200"/>
        <source>Minimum object opacity (%): </source>
        <translation>Opacité min. des objets (%) : </translation>
    </message>
    <message>
        <location line="+204"/>
        <source>Canvas grid size:</source>
        <translation>Taille de grille du canevas :</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>Trigger range selection with a single click</source>
        <translation>Déclenchement de la sélection en un clic</translation>
    </message>
    <message>
        <location line="-270"/>
        <source>Autosave interval (minutes):</source>
        <translation>Intervalle sauv. auto :</translation>
    </message>
    <message>
        <location line="+402"/>
        <source>Replaces any straight line in relationship by curved ones in order to improve the model&apos;s visualization.</source>
        <translation>Remplace les lignes droites des relations par des courbes pour améliorer la lisibilité du modèle.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Use curved lines for relationships</source>
        <translation>Utiliser des lignes courbes pour les associations</translation>
    </message>
    <message>
        <location line="+218"/>
        <source>SQL history max. length:</source>
        <translation>Longueur max. de l&apos;historique SQL :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Souce code editor:</source>
        <translation>Éditeur de code source :</translation>
    </message>
    <message>
        <location line="+131"/>
        <source>lines</source>
        <translation>lignes</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Clear the entire SQL comand history.</source>
        <translation>Nettoyer complètement l&apos;historique des commandes SQL.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Clear history</source>
        <translation>Nettoyer l&apos;historique</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Configurations directory:</source>
        <translation>Dossier de configuration :</translation>
    </message>
    <message>
        <location line="-153"/>
        <source>Browse the source code editor application</source>
        <translation>Rechercher l&apos;éditeur de code source</translation>
    </message>
    <message>
        <location line="-38"/>
        <source>Open in file manager</source>
        <translation>Ouvrir dans le gestionnaire de fichier</translation>
    </message>
    <message>
        <location line="+234"/>
        <source>Check updates at startup</source>
        <translation>Vérifier MAJ au démarrage</translation>
    </message>
    <message>
        <location line="-295"/>
        <source>Souce code editor args:</source>
        <translation>Arg. pour l&apos;éditeur de code source :</translation>
    </message>
    <message>
        <location line="+259"/>
        <source>User interface language:</source>
        <translation>Langue de l&apos;interface graphique :</translation>
    </message>
    <message>
        <location line="+76"/>
        <source>Overrides the default user interface language defined by the system. Requires restarting the program. &lt;strong&gt;NOTE:&lt;/strong&gt; UI translations are third party collaborations thus any typo or mistake should be reported directly to their respective maintainers.</source>
        <translation>Remplace la langue de l&apos;interface utilisateur définie par défaut par le système. Nécessite de redémarrer le programme. &lt;strong&gt;REMARQUE :&lt;/strong&gt; les traductions de l&apos;interface utilisateur sont des collaborations avec des tiers; par conséquent, toute faute de frappe ou erreur doit être signalée directement à leurs responsables respectifs.</translation>
    </message>
</context>
<context>
    <name>GenericSQLWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/genericsqlwidget.ui" line="+36"/>
        <source>SQL code</source>
        <translation>Code SQL</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>References</source>
        <translation>Référence</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Object:</source>
        <translation>Objet :</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Ref. name:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>&lt;p&gt;The name of the reference to an object. All occurences of the reference enclosed by &lt;strong&gt;{}&lt;/strong&gt; are replaced by the referenced object&apos;s name or signature in the generic SQL object&apos;s code.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>&lt;p&gt;Use the referenced object&apos;s signature instead of its name. For some objects like functions, casts, operators and some others the signature will include parameters types and some other information.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Use signature</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>&lt;p&gt;The referenced object&apos;s name or signature will be automatically quoted when special characters are found. Additionally, for schema qualified objects, the name of the parent schema is prepended to the referenced object&apos;s name or signature. This will avoid common SQL syntax errors or loss of semantics.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location filename="../libpgmodeler_ui/src/genericsqlwidget.cpp" line="+74"/>
        <source>Format name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Preview</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/genericsqlwidget.cpp" line="-10"/>
        <source>Ref. name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Signature</source>
        <translation>Signature</translation>
    </message>
    <message>
        <location line="+78"/>
        <location line="+1"/>
        <location line="+60"/>
        <location line="+1"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location line="-14"/>
        <source>No object name, SQL code or references defined! Preview unavailable.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+1"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>HintTextWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/hinttextwidget.ui" line="+44"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
</context>
<context>
    <name>IndexWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="+42"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Fill Factor:</source>
        <translation>Taux de remplissage :</translation>
    </message>
    <message>
        <location line="+59"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Concurrent</source>
        <translation>Concurrent</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Unique</source>
        <translation>Unique</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Fast update</source>
        <translation>Mise à jour rapide</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Elements</source>
        <translation>Éléments</translation>
    </message>
    <message>
        <location line="-72"/>
        <source>Buffering</source>
        <translation>Mémoire tampon</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Predicate:</source>
        <translation>Prédicat :</translation>
    </message>
</context>
<context>
    <name>LanguageWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="+63"/>
        <source>Trusted:</source>
        <translation>De confiance :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/languagewidget.cpp" line="+42"/>
        <source>The functions to be assigned to the language should have, respectively, the following signatures:&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Handler Function:&lt;/strong&gt; &lt;em&gt;language_handler function()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Validator Function:&lt;/strong&gt; &lt;em&gt;void function(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Inline Function:&lt;/strong&gt; &lt;em&gt;void function(internal)&lt;/em&gt;</source>
        <translation>Les fonctions attribuées au langage doivent avoir respectivement les signatures suivantes :&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Fonction de gestion :&lt;/strong&gt; &lt;em&gt;language_handler function()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Fonction de validation :&lt;/strong&gt; &lt;em&gt;void function(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Fonction sur une ligne :&lt;/strong&gt; &lt;em&gt;void function(internal)&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="+19"/>
        <source>Validator Func.:</source>
        <translation>Fonc. de validation :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Handler Func.:</source>
        <translation>Fonc. gestionnaire :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Inline Func.:</source>
        <translation>Fonc. en ligne :</translation>
    </message>
</context>
<context>
    <name>LayersWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/layerswidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Remove all layers</source>
        <translation>Effacer toutes les couches</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Delete all</source>
        <translation>Effacer tout</translation>
    </message>
    <message>
        <location line="+13"/>
        <location line="+140"/>
        <source>Shift+Del</source>
        <translation>Shift+Del</translation>
    </message>
    <message>
        <location line="-108"/>
        <source>Hide this widget</source>
        <translation>Fermer ce widget</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Add a new layer</source>
        <translation>Ajouter une nouvelle couche</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ins</source>
        <translation>Ins</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Remove the selected layer</source>
        <translation>Efface la couche sélectionnée</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Delete</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/layerswidget.cpp" line="+92"/>
        <source>This action will delete all layers (except the default one) and the objects in them will be moved to the default layer. Do you want to proceed?</source>
        <translation>Cette opération supprimera toutes les couches (à l&apos;exception de celle par défaut) et tous leurs objets seront déplacés dans celle-ci. Souhaitez-vous continuer ?</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Delete the selected layer will cause objects in it to be moved to the default layer. Do you want to proceed?</source>
        <translation>Supprimer la couche sélectionnée provoquera le déplacement de ses objets dans la couche par défaut. Souhaitez-vous continuer ?</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>New layer</source>
        <translation>Nouvelle couche</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="+29"/>
        <source>pgModeler - PostgreSQL Database Modeler</source>
        <translation>pgModeler – Modeleur de bases de données PostgreSQL</translation>
    </message>
    <message>
        <location line="+379"/>
        <source>&amp;Validation</source>
        <translation></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+V</source>
        <translation></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Ctrl+F</source>
        <translation></translation>
    </message>
    <message>
        <location line="+153"/>
        <source>Alt+O</source>
        <translation></translation>
    </message>
    <message>
        <location line="-54"/>
        <source>Alt+B</source>
        <translation></translation>
    </message>
    <message>
        <location line="+181"/>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>&amp;Edit</source>
        <translation>Édit&amp;ion</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&amp;Show</source>
        <translation>&amp;Affichage</translation>
    </message>
    <message>
        <location line="+541"/>
        <source>Plugins</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <location line="-12"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location line="-425"/>
        <source>Controls</source>
        <translation>Contrôles</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="+119"/>
        <source>Ctrl+N</source>
        <translation>Ctrl+N</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Zoom in</source>
        <translation>Zoom +</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+=</source>
        <translation>Ctrl+=</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Zoom out</source>
        <translation>Zoom -</translation>
    </message>
    <message>
        <location line="-3"/>
        <source>Zoom -</source>
        <translation>Zoom -</translation>
    </message>
    <message>
        <location line="-556"/>
        <source>Find objects</source>
        <translation>Rechercher des objets</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Layers</source>
        <translation>Couches</translation>
    </message>
    <message>
        <location line="+524"/>
        <source>Ctrl+-</source>
        <translation>Ctrl+-</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Ctrl+O</source>
        <translation>Ctrl+O</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <location line="+90"/>
        <source>Export the current opened model in different modes</source>
        <translation>Exporter le modèle courant sous différentes formes</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Close current model</source>
        <translation>Fermer le modèle courant</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Ctrl+H</source>
        <translation>Ctrl+H</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Edit pgModeler settings</source>
        <translation>Configurer pgModeler</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>F10</source>
        <translation>F10</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Access the list of loaded plugins</source>
        <translation>Accéder à la liste des extensions chargées</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Load recently opened model</source>
        <translation>Charger des modèles ouverts récemment</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Import existing database to new model (reverse engineering)</source>
        <translation>Importer une base de données existante vers un modèle (reverse engineering)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Shift+I</source>
        <translation>Ctrl+Maj+I</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Model&apos;s structure</source>
        <translation>Réparer un m&amp;odèle</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Performs the model structure fix routine</source>
        <translation>Routine de récupération des fichiers modèles corrompus</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>New version found!</source>
        <translation>Nouvelle version disponible !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Update for the current version is available on project&apos;s site</source>
        <translation>Une mise à jour pour votre version est disponible sur le site de pgModeler</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Main menu</source>
        <translation>Menu principal</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Show expanded</source>
        <translation>Afficher la barre de menus</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Hide main menu</source>
        <translation>Cacher la barre de menus</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Hides the main menu bar and put the action on a separated action</source>
        <translation>Masque la barre de menu (le menu reste accessible via un bouton dans la barre d&apos;outils)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Shift+H</source>
        <translation>Ctrl+Maj+H</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Welcome</source>
        <translation>Accueil</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Welcome screen</source>
        <translation>Écran d&apos;accueil</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Design</source>
        <translation>Modéliser</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Manage</source>
        <translation>Administrer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Manage existent databases</source>
        <translation>Administrer des bases existantes</translation>
    </message>
    <message>
        <location line="-967"/>
        <source>O&amp;bjects</source>
        <translation>O&amp;bjets</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>&amp;Operations</source>
        <translation>&amp;Opérations</translation>
    </message>
    <message>
        <location line="+827"/>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Maj+S</translation>
    </message>
    <message>
        <location line="-265"/>
        <source>Show grid</source>
        <translation>Afficher la grille</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Align objects position to grid</source>
        <translation>Aligner les objets sur la grille</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Show the page delimiters</source>
        <translation>Afficher les délimiteurs de page</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Save all</source>
        <translation>Tout enregistrer</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Show the model overview</source>
        <translation>Afficher la vue d&apos;ensemble du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="+864"/>
        <source>Clear Menu</source>
        <translation>Vider le menu</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>The demonstration version can create only `one&apos; instance of database model!</source>
        <translation>La version de démonstration ne peut gérer qu&apos;une seule instance de modèle de base de données !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="-326"/>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="+362"/>
        <source>Save model</source>
        <translation>Enregistrer le modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="+129"/>
        <location line="+536"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location line="-485"/>
        <source>Save &apos;%1&apos; as...</source>
        <translation>Enregistrer &apos;%1&apos; sous...</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+199"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modèle de base de données (*.dbm);; Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Database model printing</source>
        <translation>Impression du modèle de base de données</translation>
    </message>
    <message>
        <location line="-184"/>
        <source>Save anyway</source>
        <translation>Sauvegarder quand même</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+79"/>
        <location line="+60"/>
        <source>Validate</source>
        <translation>Valider</translation>
    </message>
    <message>
        <location line="+73"/>
        <source>Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click &apos;No&apos; or &apos;Cancel&apos; to abort printing.</source>
        <translation>Des modifications ont été détectées dans les définitions du papier ou des marges du modèle ce qui pourrait causer une mauvaise impression des objets. Souhaitez-vous continuer l&apos;impression avec les nouveaux paramètres ? Pour utiliser les paramètres par défaut cliquez sur &apos;Non&apos; ou sur &apos;Annuler&apos; pour interrompre l&apos;impression.</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Load model</source>
        <translation>Charger un modèle</translation>
    </message>
    <message>
        <location line="+49"/>
        <source>Could not load the database model file `%1&apos;. Check the error stack to see details. You can try to fix it in order to make it loadable again.</source>
        <translation>Impossible de charger le ficher modèle de base de données `%1&apos;. Consultez la pile d&apos;appel pour plus de détails. Essayez de réparer le modèle pour tenter de l&apos;ouvrir à nouveau.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Fix model</source>
        <translation>Réparer un modèle</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="-418"/>
        <source>Toggle the model objects widget</source>
        <translation>(Dés)activer le widget des objets du modèle</translation>
    </message>
    <message>
        <location line="-137"/>
        <source>Toogle the model validation widgets</source>
        <translation>(Dés)activer le widget de validation du modèle</translation>
    </message>
    <message>
        <location line="+191"/>
        <source>Toggle the operation history widget</source>
        <translation>(Dés)activer le widget de l&apos;historique des opérations</translation>
    </message>
    <message>
        <location line="-153"/>
        <location line="+38"/>
        <source>Toggle the object finder</source>
        <translation>(Dés)activer le chercheur d&apos;objet</translation>
    </message>
    <message>
        <location line="+931"/>
        <source>action_main_menu</source>
        <translation>action_menu_principal</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Expands the main menu bar in classical mode</source>
        <translation>Développe la barre de menus principale en mode classique</translation>
    </message>
    <message>
        <location line="-767"/>
        <source>Saving temp. models</source>
        <translation>Sauvegarde temp. des modèles</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="-1646"/>
        <source>Grid</source>
        <translation>Grille</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Hierarchical</source>
        <translation>Hiérarchique</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Scattered</source>
        <translation>Dispersé</translation>
    </message>
    <message>
        <location line="+205"/>
        <source> (Demo)</source>
        <translation> (Démo)</translation>
    </message>
    <message>
        <location line="+1136"/>
        <source>You&apos;re running a demonstration version! The model saving feature is available only in the full version!</source>
        <translation>Vous utilisez une version de démonstration ! La fonctionnalité d&apos;enregistrement du modèle est uniquement disponible dans la version complète !</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+79"/>
        <location line="+60"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location line="-138"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! It&apos;s recommended to validate it before save in order to create a consistent model otherwise the generated file will be broken demanding manual fixes to be loadable again!</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; Le modèle &lt;strong&gt;%1&lt;/strong&gt; n&apos;a pas été validé ! Il est recommandé de le valider avant de l&apos;enregistrer afin de créer un modèle cohérent, sinon il peut en résulter un fichier cassé nécessitant une réparation manuelle pour être chargé à nouveau chargeable !</translation>
    </message>
    <message>
        <location line="+79"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! Before run the export process it&apos;s recommended to validate in order to correctly create the objects on database server!</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; Le modèle &lt;strong&gt;%1&lt;/strong&gt; n&apos;a pas été validé ! Avant de procéder à un export il est recommandé de le valider afin de s&apos;assurer de correctement créer les objets sur le serveur de base de données !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Export anyway</source>
        <translation>Exporter quand même</translation>
    </message>
    <message>
        <location line="+58"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The model &lt;strong&gt;%1&lt;/strong&gt; is invalidated! Before run the diff process it&apos;s recommended to validate in order to correctly analyze and generate the difference between the model and a database!</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; Le modèle &lt;strong&gt;%1&lt;/strong&gt; n&apos;a pas été validé ! Avant de procéder à une comparaison il est recommandé de le valider afin de s&apos;assurer que l&apos;analyse entre celui-ci de la base de donnée se fasse correctement !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Diff anyway</source>
        <translation>Comparer quand même</translation>
    </message>
    <message>
        <location line="+312"/>
        <source>(no samples found)</source>
        <translation>(aucun exemple trouvé)</translation>
    </message>
    <message>
        <location line="+68"/>
        <source>You&apos;re running a demonstration version! Note that you&apos;ll be able to create only &lt;strong&gt;%1&lt;/strong&gt; instances 						of each type of object and some key features will be disabled or limited!&lt;br/&gt;&lt;br/&gt;You can purchase a full binary copy or get the source code at &lt;a href=&apos;https://pgmodeler.io&apos;&gt;https://pgmodeler.io&lt;/a&gt;.						&lt;strong&gt;NOTE:&lt;/strong&gt; pgModeler is an open source software, but purchasing binary copies or providing some donations will support the project and keep the development alive and at full speed!&lt;br/&gt;&lt;br/&gt;						&lt;strong&gt;HINT:&lt;/strong&gt; in order to test all features it&apos;s recommended to use the &lt;strong&gt;demo.dbm&lt;/strong&gt; model located in &lt;/strong&gt;Sample models&lt;/strong&gt; at &lt;strong&gt;Welcome&lt;/strong&gt; view.&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;</source>
        <translation>Vous utilisez une version de démonstration ! Vous ne pouvez créer uniquement que &lt;strong&gt;%1&lt;/strong&gt; instances 						de chaque type d&apos;objet et certaines fonctionnalités sont bridées ou désactivées !&lt;br/&gt;&lt;br/&gt;Vous pouvez acheter une copie exécutable de ce logiciel ou obtenir son code source sur &lt;a href=&apos;https://pgmodeler.io&apos;&gt;https://pgmodeler.io&lt;/a&gt;.						&lt;strong&gt;REMARQUE:&lt;/strong&gt; pgModeler est un logiciel libre, mais acheter une copie binaire ou faire un don permet de soutenir ce projet et de continuer son développement actif.&lt;br/&gt;&lt;br/&gt;						&lt;strong&gt;CONSEIL:&lt;/strong&gt;pour avoir un aperçu de toutes les fonctionnalités, ouvrez le modèle &lt;strong&gt;demo.dbm&lt;/strong&gt; situé dans le dossier &lt;/strong&gt;Modèles de démo&lt;/strong&gt; à l&apos;écran d&apos; &lt;strong&gt;Accueil&lt;/strong&gt; du logiciel.&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;&lt;br/&gt;</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>save</source>
        <translation>sauvegarde</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>export</source>
        <translation>exporter</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>diff</source>
        <translation>comparer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Executing pending &lt;strong&gt;%1&lt;/strong&gt; operation...</source>
        <translation>Exécution de l&apos;opération &lt;strong&gt;%1&lt;/strong&gt; en attente...</translation>
    </message>
    <message>
        <location line="+113"/>
        <source>Rearrange objects over the canvas is an irreversible operation! Would like to proceed?</source>
        <translation>Réorganiser les objets sur le canvas est une opération irréversible ! Voulez-vous poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="+118"/>
        <source>He&amp;lp</source>
        <translation>Ai&amp;de</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Pl&amp;ugins</source>
        <translation>Mod&amp;ules</translation>
    </message>
    <message>
        <location line="+136"/>
        <source>&amp;New</source>
        <translation>&amp;Nouveau</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>New model</source>
        <translation>Nouveau modèle</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>&amp;Save</source>
        <translation>E&amp;nregistrer</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Zoom in</source>
        <translation>&amp;Zoomer</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Zoo&amp;m out</source>
        <translation>Dézoo&amp;mer</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Load</source>
        <translation>C&amp;harger</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Sa&amp;ve as</source>
        <translation>Enre&amp;gistrer sous</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>E&amp;xit</source>
        <translation>Qui&amp;tter</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Exit pgModeler</source>
        <translation>Quitter pgModeler</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;About pgModeler</source>
        <translation>&amp;À propos de pgModeler</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>F4</source>
        <translation></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>&amp;Print</source>
        <translation>Im&amp;primer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Print model</source>
        <translation>Imprimer le modèle</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+P</source>
        <translation></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>&amp;Undo</source>
        <translation>Ann&amp;uler</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Undo operation</source>
        <translation>Annuler l&apos;opération</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Z</source>
        <translation></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>&amp;Redo</source>
        <translation>&amp;Rétablir</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Redo operation</source>
        <translation>Rétablir l&apos;opération</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Y</source>
        <translation></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>&amp;Export</source>
        <translation>&amp;Exporter</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ctrl+Shift+E</source>
        <translation>Ctrl+Maj+E</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Show grid</source>
        <translation>A&amp;fficher la grille</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ctrl+G</source>
        <translation></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ctrl+W</source>
        <translation></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>&amp;Normal zoom</source>
        <translation>Zoom &amp;normal</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+0</source>
        <translation></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Align to grid</source>
        <translation>&amp;Aligner sur la grille</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Show &amp;delimiters</source>
        <translation>Afficher les &amp;délimiteurs</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ctrl+L</source>
        <translation></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>&amp;Settings</source>
        <translation>Paramètre&amp;s</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>F12</source>
        <translation></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&amp;Overview</source>
        <translation>&amp;Vue d&apos;ensemble</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>&amp;Support</source>
        <translation></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Access the support page</source>
        <translation>Accéder à la page du support</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>F1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>New object</source>
        <translation>Nouvel objet</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>&amp;Recent Models</source>
        <translation>Modèles &amp;récents</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>&amp;Import</source>
        <translation>&amp;Importer</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Rest&amp;ore Session</source>
        <translation>Restaurer la sessi&amp;on</translation>
    </message>
    <message>
        <location line="+220"/>
        <source>Compact view</source>
        <translation>Vue compacte</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Toggle the compact view on the model(s)</source>
        <translation>Activer la vue compacte</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>More</source>
        <translation>Plus</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Addition action over the model</source>
        <translation>Plus d&apos;actions concernant le modèle</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Fix</source>
        <translation>Réparer</translation>
    </message>
    <message>
        <location line="-208"/>
        <source>&amp;Check for update</source>
        <translation>Re&amp;chercher une mise à jour</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>&amp;Diff</source>
        <translation>&amp;Comparer</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ctrl+Shift+D</source>
        <translation>Ctrl+Maj+D</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Shift+W</source>
        <translation>Maj+W</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Design database models</source>
        <translation>Conception de modèles de base de données</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Shift+D</source>
        <translation>Maj+D</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Shift+M</source>
        <translation>Maj+M</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>&amp;Bug report</source>
        <translation>Rapport de &amp;bug</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Report a bug</source>
        <translation>Rapporter un bug</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Donate</source>
        <translation>Faire un don</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Help pgModeler by donating!</source>
        <translation>Aider pgModeler en faisant un don !</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Objects me&amp;tadata</source>
        <translation>Mé&amp;tadonnées des objets</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="-1369"/>
        <source>Save modified model(s)</source>
        <translation>Enregistrer le(s) modèle(s) modifié(s)</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The following models were modified but not saved: %1. Do you really want to quit pgModeler?</source>
        <translation>Les modèles suivants ont été modifiés mais non enregistrés : %1. Voulez-vous vraiment quitter pgModeler ?</translation>
    </message>
    <message>
        <location line="+574"/>
        <source>The model &lt;strong&gt;%1&lt;/strong&gt; was modified! Do you really want to close without save it?</source>
        <translation>Le modèle &lt;strong&gt;%1&lt;/strong&gt; a été modifié ! Êtes-vous certain de vouloir quitter sans l&apos;enregistrer ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="-102"/>
        <source>Determine the changes between model/database and another database</source>
        <translation>Déterminer les changements entre le modèle/la base de données et une autre base de données</translation>
    </message>
    <message>
        <location line="+105"/>
        <source>Perform the objects metadata handling</source>
        <translation>Manipulation des métadonnées des objets</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Arrange objects</source>
        <translation>Organiser des objets</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Rearrange objects over the canvas</source>
        <translation>Réorganiser les objets sur le canevas</translation>
    </message>
</context>
<context>
    <name>Messagebox</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="+29"/>
        <source>Dialog</source>
        <translation>Boîte de dialogue</translation>
    </message>
    <message>
        <location line="+147"/>
        <source>msg</source>
        <translation>message</translation>
    </message>
    <message>
        <location line="+133"/>
        <source>Exceptions</source>
        <translation>Exceptions</translation>
    </message>
    <message>
        <location line="+112"/>
        <source>Show/hide exceptions stack.</source>
        <translation>Afficher/cacher la pile d&apos;exceptions.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+71"/>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="+135"/>
        <source>&amp;Yes</source>
        <translation>&amp;Oui</translation>
    </message>
    <message>
        <location line="+31"/>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="+4"/>
        <source>&amp;No</source>
        <translation>&amp;Non</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="+22"/>
        <source>Information</source>
        <translation></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="+25"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="-12"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Alert</source>
        <translation>Alerte</translation>
    </message>
    <message>
        <location line="-22"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="-159"/>
        <source>Show raw text errors or information.</source>
        <translation>Afficher le texte brute des erreurs ou des informations.</translation>
    </message>
</context>
<context>
    <name>MetadataHandlingForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="+14"/>
        <source>Handle metadata</source>
        <translation>Gérer les métadonnées</translation>
    </message>
    <message>
        <location line="+83"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Extract from:</source>
        <translation>Extraction depuis :</translation>
    </message>
    <message>
        <location line="+84"/>
        <source>Loading a metadata file to the current model is an irreversible operation so be sure to specify a backup file before proceed.</source>
        <translation>Le chargement d&apos;un fichier de métadonnées dans le modèle actuel est une opération irréversible, assurez-vous de spécifier un fichier de sauvegarde avant de continuer.</translation>
    </message>
    <message>
        <location line="+194"/>
        <source>Database model metadata</source>
        <translation>Métadonnées du modèle de base de données</translation>
    </message>
    <message>
        <location line="-123"/>
        <source>Handles the objects&apos; positioning in the metadata file.</source>
        <translation>Gère le positionnement des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location line="-52"/>
        <source>Options</source>
        <translation></translation>
    </message>
    <message>
        <location line="+58"/>
        <source>Objects&apos; positioning</source>
        <translation>Positionnement des objets</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Custom object&apos;s colors</source>
        <translation>Couleurs de l&apos;objet personnalisé</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Handles the objects&apos; protection status in the metadata file.</source>
        <translation>Gère l&apos;état de protection des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Objects&apos; protection status</source>
        <translation>Statut de protection des objets</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Handles the objects&apos; SQL disabled status in the metadata file.</source>
        <translation>Gère le statut désactivé du code SQL des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Objects&apos; SQL disabled status</source>
        <translation>Statut désactivé du code SQL des objets</translation>
    </message>
    <message>
        <location line="+124"/>
        <source>Handles the objects&apos; custom SQL commands in the metadata file.</source>
        <translation>Gère les commandes SQL personnalisées des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Custom SQL commands</source>
        <translation>Commandes SQL personnalisées</translation>
    </message>
    <message>
        <location line="-26"/>
        <source>Textbox objects</source>
        <translation>Objets de zone de texte</translation>
    </message>
    <message>
        <location line="-227"/>
        <source>&lt;p&gt;Handles the objects&apos; aliases  in the metadata file. Only for graphical objects and table&apos;s children objects.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Objects&apos; aliases</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+59"/>
        <source>&lt;p&gt;Handles the objects&apos; custom colors in the metadata file. Currently available only for relationships and schemas.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+78"/>
        <source>&lt;p&gt;Handles the following database model attributes in the metadata file: author, zoom factor, last position and default objects.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+52"/>
        <source>&lt;p&gt;Save tags to the output file when extracting metadata. When loading the file, the tags are recreated and duplicated ones are ignored.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Tag objects</source>
        <translation>Balises des objets</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;Save textboxes to the output file when extracting metadata. When loading the file, the textboxes are recreated and duplicated ones are ignored.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Handles the tables&apos; and views&apos; collapsing mode in the metadata file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Tables and views collpsing mode</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;Save generic SQL objects to the output file when extracting metadata. When loading the file, the objects are recreated and duplicated ones are ignored.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Select all</source>
        <translation type="unfinished">Tout sélectionner</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Clear all</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Backup file:</source>
        <translation>Fichier de sauvegarde :</translation>
    </message>
    <message>
        <location line="+19"/>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="+240"/>
        <source>Select file</source>
        <translation>Sélectionner un fichier</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>&lt;p&gt;Extracts the objects&apos; metadata from the loaded models and apply to the current focused model. A backup file can be specified to where the focused model&apos;s current metadata will be saved.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&lt;p&gt;Extracts the objects metadata from one of the loaded models saving the info to a backup file.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&lt;p&gt;Reads the objects&apos; metadata from a previously saved backup file and apply to the current model.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Apply to:</source>
        <translation>Appliquer à :</translation>
    </message>
    <message>
        <location line="-119"/>
        <source>Operation:</source>
        <translation>Opération :</translation>
    </message>
    <message>
        <location line="+137"/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location line="+89"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/metadatahandlingform.cpp" line="-151"/>
        <location line="+11"/>
        <source>model not saved yet</source>
        <translation>modèle pas encore enregistré</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The backup file cannot be the same as the input model!</source>
        <translation>Le fichier de sauvegarde ne peut être le même que le modèle d&apos;entrée !</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Extracting metadata to file `%1&apos;</source>
        <translation>Extraction des métadonnées vers le fichier `%1&apos;</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Saving backup metadata to file `%1&apos;</source>
        <translation>Enregistrement des métadonnées de sauvegarde vers le fichier `%1&apos;</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Applying metadata from file `%1&apos;</source>
        <translation>Application des métadonnées depuis le fichier `%1&apos;</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Metadata processing aborted!</source>
        <translation>Traitement des métadonnées abandonné !</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Objects metadata file (*.omf);;All files (*.*)</source>
        <translation>Fichiers de métadonnées d&apos;objets (*.omf);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/metadatahandlingform.ui" line="-500"/>
        <source>Handles the objects&apos; fade out status in the metadata file.</source>
        <translation>Gère l&apos;estompement des objets dans le fichier de métadonnées.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Objects&apos; fade out status</source>
        <translation>Effet d&apos;estompement des objets</translation>
    </message>
    <message>
        <location line="+130"/>
        <source>Generic SQL objects</source>
        <translation>Objets SQL génériques</translation>
    </message>
    <message>
        <location line="+116"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location line="+43"/>
        <source>&amp;Extract and restore</source>
        <translation>&amp;Extraction et restauration</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Extract &amp;only</source>
        <translation>Extracti&amp;on seule</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&amp;Restore a backup file</source>
        <translation>&amp;Restaurer une sauvegarde</translation>
    </message>
</context>
<context>
    <name>ModelDatabaseDiffForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="+100"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location line="+380"/>
        <location line="+106"/>
        <source>Connection:</source>
        <translation>Connexion :</translation>
    </message>
    <message>
        <location line="+733"/>
        <source>Ignore import errors</source>
        <translation>Ignorer les erreurs d&apos;import</translation>
    </message>
    <message>
        <location line="-46"/>
        <source>Import system objects</source>
        <translation>Importer les objets système</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Import extension objects</source>
        <translation>Importer les objets des extensions</translation>
    </message>
    <message>
        <location line="-492"/>
        <source>File:</source>
        <translation>Fichier :</translation>
    </message>
    <message>
        <location line="-661"/>
        <source>Presets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Add new preset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ins</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Edit preset name</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+E</source>
        <translation type="unfinished">Ctrl+E</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Save preset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+S</source>
        <translation type="unfinished">Ctrl+S</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Cancel operation on the preset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Esc</source>
        <translation type="unfinished">Échap</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Delete the selected preset</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Del</source>
        <translation type="unfinished">Suppr</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Restore the default presets</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+R</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Input</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+312"/>
        <source>&lt;p&gt;Override the PostgreSQL version when generating the diff code. The default is to use the same version as the input database (detected automatically).&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+52"/>
        <source>&lt;p&gt;Compares the model and the input database storing the diff in a SQL file for later usage.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+86"/>
        <source>Select output file</source>
        <translation>Sélectionner un fichier de sortie</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>&lt;p&gt;Compares the model and the input database generating a diff and applying it directly to the latter. &lt;strong&gt;WARNING:&lt;/strong&gt; this mode causes irreversible changes on the database and in case of failure the original structure is not restored, so make sure to have a backup before proceed.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+75"/>
        <source>&lt;p&gt;Database cluster level objects like roles and tablespaces will not be dropped.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;Permissions already set on database objects will be kept. The ones configured on the model will be applied to the database.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;Avoid the generation of DROP commands for objects that exists in database but not in the model. This is useful when diff a partial model against the complete database.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+41"/>
        <source>&lt;p&gt;Force the generation of DROP commands for columns and constraints that exist in database but not in the model. This is useful when diff a partial model against the complete database and the user needs to drop columns and constraint but preserve the rest of the objects.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;No command to rename the destination database will be generated even the model&apos;s name differ from database name.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;For DROP command, the objects that depends on an object to be dropped will be deleted as well. For TRUNCATE command, tables that are linked to a table to be truncated will be truncate too. &lt;strong&gt;NOTE:&lt;/strong&gt; this option can affect more objects than listed in the output or diff preview.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&lt;p&gt;Clears the data of all tables which will have columns modified. This is useful to avoid errors related to type casting. &lt;strong&gt;WARNING:&lt;/strong&gt; DO NOT use this option on production servers and always make a backup before use it.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;Serial columns are converted to integer and having the default value changed  to &lt;strong&gt;nextval(sequence)&lt;/strong&gt; function call. By default, a new sequence is created for each serial column but checking this option sequences matching the name on column&apos;s default value will be reused and will not be dropped.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;Instead of use an ALTER command to modify certain kind of objects a DROP and CREATE will be used in order to do a full modification. This option does not affects the database object.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Recreates only objects that can&apos;t be changed through ALTER commands according to pgModeler implementation not the PostgreSQL one. &lt;br/&gt;Currently, these objects are:&lt;br/&gt;&lt;br/&gt;aggregate, cast, constraint, collation, conversion, language, operator, operator class, operator family, rule, trigger and view.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+76"/>
        <source>&lt;p&gt;Import system (built-in) objects. Use this if the import step is returning errors related to missing objects.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;Import objects created by extensions. Use this if the import step is returning errors even importing built in ones.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;Ignores as many as possible errors on import step. This option generates an incomplete diff.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+44"/>
        <source>&lt;p&gt;Ignores errors generated by duplicated objects when exporting the diff to database.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+36"/>
        <source>&lt;p&gt;This advanced option causes pgModeler to ignore extra errors by their numeric codes. These errors must be informed in the input below and separeted by space. For the complete list of error codes check the PostgreSQL docs, section &lt;strong&gt; Appendix A. PostgreSQL Error Codes&lt;/strong&gt;. &lt;strong&gt;WARNING:&lt;/strong&gt; use this option with extreme care since it can interfere in final export result.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+198"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="+68"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location line="+428"/>
        <source>Loads the generated diff code in the destination server for manual applying.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Open in SQL Tool</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+136"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="+341"/>
        <location line="+2"/>
        <source>Waiting process to start...</source>
        <translation>En attente du démarrage de la tâche...</translation>
    </message>
    <message>
        <location line="+231"/>
        <source>Step %1/%2: Exporting diff to database &lt;strong&gt;%3@%4&lt;/strong&gt;...</source>
        <translation type="unfinished">Étape %1/%2 : Export de la comparaison de la base de données &lt;strong&gt;%3&lt;/strong&gt;... {1/%2:?} {3@%4&lt;?}</translation>
    </message>
    <message>
        <location line="+377"/>
        <source>SQL code (*.sql);;All files (*.*)</source>
        <translation>Code SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="-1780"/>
        <location line="+72"/>
        <location line="+103"/>
        <source>Database:</source>
        <translation>Base de données :</translation>
    </message>
    <message>
        <location line="+460"/>
        <source>Drop or truncate in cascade mode</source>
        <translation>Supprimer ou tronquer en mode cascade</translation>
    </message>
    <message>
        <location line="-104"/>
        <source>Keep object&apos;s permissions</source>
        <translation>Conserver les permissions des objets</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>Keep cluster objects</source>
        <translation>Conserver les objets de cluster</translation>
    </message>
    <message>
        <location line="+240"/>
        <source>Recreate only unmodifiable objects</source>
        <translation>Recréer uniquement des objets non modifiables</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Force recreation of objects</source>
        <translation>Forcer la recréation d&apos;objets</translation>
    </message>
    <message>
        <location line="+210"/>
        <source>Ignore duplicity errors</source>
        <translation>Ignorer les erreurs de duplicité</translation>
    </message>
    <message>
        <location line="-233"/>
        <source>Reuse sequences on serial columns</source>
        <translation>Réutiliser des séquences sur des colonnes de séries</translation>
    </message>
    <message>
        <location line="-468"/>
        <source>Diff mode</source>
        <translation>Mode comparaison</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Use PostgreSQL:</source>
        <translation>Utiliser PostgreSQL :</translation>
    </message>
    <message>
        <location line="+135"/>
        <source>...</source>
        <translation></translation>
    </message>
    <message>
        <location line="+607"/>
        <source>Output</source>
        <translation>Sortie</translation>
    </message>
    <message>
        <location line="+84"/>
        <source>Changes:</source>
        <translation>Modifications :</translation>
    </message>
    <message>
        <location line="+158"/>
        <source>Step label...</source>
        <translation>Étiquette étape...</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Objects marked with an &lt;span style=&quot; font-weight:600;&quot;&gt;ALTER&lt;/span&gt; may not be effectively changed unless that the differences detected are in attributes that can be modified through ALTER commands otherwise no operationwill be performed or, if the force recreation is checked, the object will be dropped and created again.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Les objets marqués d&apos;un &lt;span style=&quot;font-weight : 600;&quot;&gt;ALTER&lt;/span&gt; ne peuvent être modifiés efficacement que si les différences détectées sont dans les attributs qui peuvent être modifié à l&apos;aide des commandes ALTER. Dans le cas contraire, aucune opération ne sera effectuée ou, si la recréation de force est vérifiée, l&apos;objet sera supprimé et créé à nouveau.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Objects to be created</source>
        <translation>Objets à créer</translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+41"/>
        <location line="+41"/>
        <location line="+41"/>
        <source>0</source>
        <translation></translation>
    </message>
    <message>
        <location line="-85"/>
        <source>Objects to be dropped</source>
        <translation>Objets à supprimer</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Possible objects to be changed</source>
        <translation>Objets possiblement modifiables</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Ignored objects (system ones or with sql disabled)</source>
        <translation>Objets ignorés (ceux du système ou avec code SQL désactivé)</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Diff Preview</source>
        <translation>Aperçu de la comparaison</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>&amp;Apply diff</source>
        <translation>&amp;Appliquer la comparaison</translation>
    </message>
    <message>
        <location line="+146"/>
        <source>&amp;Generate</source>
        <translation>&amp;Générer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="-390"/>
        <source> &lt;strong&gt;WARNING:&lt;/strong&gt; The generated diff is ready to be exported! Once started this process will cause irreversible changes on the database. Do you really want to proceed?</source>
        <translation> &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; La comparaison générée est prête à être exportée ! Une fois démarré, ce processus entraînera des changements irréversibles dans la base de données. Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Apply diff</source>
        <translation>Appliquer la comparaison</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Preview diff</source>
        <translation>Aperçu de la comparaison</translation>
    </message>
    <message>
        <location line="-403"/>
        <source>model not saved yet</source>
        <translation>modèle pas encore enregistré</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>(none)</source>
        <translation>(aucun)</translation>
    </message>
    <message>
        <location line="+256"/>
        <source>&lt;strong&gt;Low verbosity is set:&lt;/strong&gt; only key informations and errors will be displayed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Step %1/%2: Importing database &lt;strong&gt;%3&lt;/strong&gt;...</source>
        <translation>Étape %1/%2 : Import de la base de données &lt;strong&gt;%3&lt;/strong&gt;...</translation>
    </message>
    <message>
        <location line="+49"/>
        <source>Step %1/%2: Comparing &lt;strong&gt;%3&lt;/strong&gt; and &lt;strong&gt;%4&lt;/strong&gt;...</source>
        <translation>Étape %1/%2 : Comparaison de &lt;strong&gt;%3&lt;/strong&gt; et de &lt;strong&gt;%4&lt;/strong&gt;...</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Diff process paused. Waiting user action...</source>
        <translation>Comparaison mise en pause. En attente d&apos;une action utilisateur...</translation>
    </message>
    <message>
        <location line="+71"/>
        <source>Saving diff to file &lt;strong&gt;%1&lt;/strong&gt;</source>
        <translation>Enregistrement de la comparaison vers le fichier &lt;strong&gt;%1&lt;/strong&gt;</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Diff process sucessfully ended!</source>
        <translation>Comparaison términée avec succès !</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+15"/>
        <source>No operations left.</source>
        <translation>Aucune opération restante.</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Operation cancelled by the user.</source>
        <translation>Opération annulée par l&apos;utilisateur.</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Process aborted due to errors!</source>
        <translation>Processus abandonné en raison d&apos;erreurs !</translation>
    </message>
    <message>
        <location line="+59"/>
        <source>-- No differences were detected between model and database. --</source>
        <translation>-- Aucune différence détectée entre le modèle et la base de données. --</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Error code &lt;strong&gt;%1&lt;/strong&gt; found and ignored. Proceeding with export.</source>
        <translation>Code erreur &lt;strong&gt;%1&lt;/strong&gt; reçu et ignoré. Poursuite de l&apos;export.</translation>
    </message>
    <message>
        <location line="+118"/>
        <source>Save diff as...</source>
        <translation>Enregistrer la comparaison sous...</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>In some cases restore the default settings related to it may solve the problem. Would like to do that?</source>
        <translation type="unfinished">Dans certains cas, restaurer les paramètres liés par défaut peut résoudre le problème. Voulez-vous le faire ?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Restore</source>
        <translation type="unfinished">Restaurer</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Do you really want to restore the default settings?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+119"/>
        <source>Are you sure do you want to remove the selected diff preset?</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="-1114"/>
        <source>Preserve database name</source>
        <translation>Conserver le nom de la base de données</translation>
    </message>
    <message>
        <location line="-61"/>
        <source>Do not drop missing objects</source>
        <translation>Ne pas supprimer les objets manquants</translation>
    </message>
    <message>
        <location line="-257"/>
        <source>Store in S&amp;QL file</source>
        <translation>Stocker dans un fichier S&amp;QL</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>Appl&amp;y on server</source>
        <translation>Appliquer sur le serveur</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeldatabasediffform.cpp" line="-354"/>
        <source>

-- SQL code purposely truncated at this point in demo version!</source>
        <translation>

-- Le code SQL est volontairement tronqué à ce stade dans la version démo !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeldatabasediffform.ui" line="-847"/>
        <source>Diff tool</source>
        <translation>Outil de comparaison</translation>
    </message>
    <message>
        <location line="+370"/>
        <source>Current model:</source>
        <translation>Modèle courant :</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>(model)</source>
        <translation>(modèle)</translation>
    </message>
    <message>
        <location line="+140"/>
        <source>Compare to</source>
        <translation>Comparer à</translation>
    </message>
    <message>
        <location line="+363"/>
        <source>Diff</source>
        <translation>Comparaison</translation>
    </message>
    <message>
        <location line="+292"/>
        <source>Import &amp;&amp; Export</source>
        <translation>Import &amp;&amp; Export</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Import</source>
        <translation></translation>
    </message>
    <message>
        <location line="+90"/>
        <source>Export</source>
        <translation></translation>
    </message>
    <message>
        <location line="+68"/>
        <source>Ignore error codes</source>
        <translation>Ignorer les codes d&apos;erreur</translation>
    </message>
    <message>
        <location line="-361"/>
        <source>Drop missing columns and constraints</source>
        <translation>Supprimer colonnes/contraintes manquantes</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Truncate tables before alter columns</source>
        <translation>Tronquer les tables avant de modifier les colonnes</translation>
    </message>
</context>
<context>
    <name>ModelExportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="+612"/>
        <location line="+346"/>
        <source>File:</source>
        <translation>Fichier :</translation>
    </message>
    <message>
        <location line="-312"/>
        <location line="+346"/>
        <location line="+252"/>
        <source>Select target file</source>
        <translation>Sélectionner le fichier cible</translation>
    </message>
    <message>
        <location line="-595"/>
        <location line="+346"/>
        <location line="+252"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-213"/>
        <source>Zoom:</source>
        <translation>Zoom :</translation>
    </message>
    <message>
        <location line="-999"/>
        <source>Export model</source>
        <translation>Export d&apos;un modèle</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Database server</source>
        <translation>Serveur de base de données</translation>
    </message>
    <message>
        <location line="+197"/>
        <source>PostgreSQL version in which the SQL code should be generated. It is recommended to select this option only when the version of the DBMS, somehow, is not identifiable or if you need to generate a specific version of SQL code for test purposes.</source>
        <translation>Version de PostgreSQL pour laquelle le code SQL sera généré. Il est recommandé d&apos;activer cette option uniquement si la version du SGBD n&apos;est pas identifiable ou pour une raison spécifique par exemple pour des essais.</translation>
    </message>
    <message>
        <location line="+142"/>
        <source>Drop:</source>
        <translation>Suppr. :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>DB</source>
        <translation>BDD</translation>
    </message>
    <message>
        <location line="+611"/>
        <source>Show delimiters</source>
        <translation>Afficher les délimiteurs de page</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Exporting the model page by page will generate files with a &lt;strong&gt;_p[n]&lt;/strong&gt; suffix where &lt;strong&gt;n&lt;/strong&gt; is the page id. Check if the current user has write permission on output folder.</source>
        <translation>Exporter le modèle page par page générera des fichiers avec un suffixe de type &lt;strong&gt;_p[n]&lt;/strong&gt; où &lt;strong&gt;n&lt;/strong&gt; est le numéro de la page. Vérifiez que vous avez bien le droit d&apos;écriture dans le dossier de sortie.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Page by page</source>
        <translation>Une image par page</translation>
    </message>
    <message>
        <location line="-782"/>
        <location line="+364"/>
        <source>PostgreSQL:</source>
        <translation>PostgreSQL :</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>PostgreSQL version in which the SQL code should be generated</source>
        <translation>Version de PostgreSQL pour laquelle le code SQL doit être généré</translation>
    </message>
    <message>
        <location line="+843"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="-487"/>
        <source>Show grid</source>
        <translation>Afficher la grille</translation>
    </message>
    <message>
        <location line="-814"/>
        <source>Connection:</source>
        <translation>Connexion :</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>&lt;p&gt;pgModeler ignores errors generated by duplicated objects and creates only that ones which does not exists in the database. This option may be used when an object was created after a previous model export.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ignore object duplicity</source>
        <translation>Ignorer les objets dupliqués</translation>
    </message>
    <message>
        <location line="+154"/>
        <source>If &lt;strong&gt;DB&lt;/strong&gt; is checked pgModeler will destroy the database if already exists on the server. When &lt;strong&gt;Objects&lt;/strong&gt; is checked pgModeler will execute the DROP command attached to SQL-enabled objects. &lt;strong&gt;WARNING:&lt;/strong&gt; this option leads to data loss so make sure to have a backup before exporting.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+82"/>
        <source>SQL file</source>
        <translation>Fichier texte SQL</translation>
    </message>
    <message>
        <location line="+384"/>
        <source>I&amp;mage (PNG)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+236"/>
        <source>Data dictionary</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+110"/>
        <source>&lt;strong&gt;Standalone:&lt;/strong&gt; pgModeler will generate a single HTML file containing the data dictionaries of all tables in the database model. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Standalone</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+54"/>
        <source>&lt;strong&gt;Splitted:&lt;/strong&gt; the data dictionaries are generated in separated files inside the selected directory. In this mode the files are named &lt;em&gt;schema.table.html&lt;/em&gt;.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Splitted</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Mode:</source>
        <translation type="unfinished">Mode :</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>pgModeler will generate an index to help navigate through the data dictionary. If splitted mode is set then a separated file named &lt;em&gt;index.html&lt;/em&gt; will be saved into the destination folder.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Include index</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Output:</source>
        <translation type="unfinished">Sortie :</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>Output</source>
        <translation>Progression</translation>
    </message>
    <message>
        <location line="+112"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location line="+114"/>
        <source>&amp;Export</source>
        <translation>&amp;Exporter</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="+188"/>
        <source>Initializing model export...</source>
        <translation>Initialisation de l&apos;export du modèle...</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;strong&gt;Low verbosity is set:&lt;/strong&gt; only key informations and errors will be displayed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Saving file &apos;%1&apos;</source>
        <translation>Enregistrement du fichier &apos;%1&apos;</translation>
    </message>
    <message>
        <location line="+34"/>
        <location line="+96"/>
        <source>Exporting process aborted!</source>
        <translation>Export abandonnée !</translation>
    </message>
    <message>
        <location line="-46"/>
        <source>HTML file (*.html);;All files (*.*)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Exporting process canceled by user!</source>
        <translation>Export annulée par l&apos;utilisateur !</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Exporting process sucessfully ended!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-90"/>
        <source>Export model as...</source>
        <translation>Exporter le modèle sous...</translation>
    </message>
    <message>
        <location line="-139"/>
        <source>Error code &lt;strong&gt;%1&lt;/strong&gt; found and ignored. Proceeding with export.</source>
        <translation>Code erreur &lt;strong&gt;%1&lt;/strong&gt; reçu et ignoré. Poursuite de l&apos;export.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="-1244"/>
        <source>Ob&amp;jects</source>
        <translation>Ob&amp;jets</translation>
    </message>
    <message>
        <location line="+294"/>
        <source>Graphics file</source>
        <translation>Fichier graphique</translation>
    </message>
    <message>
        <location line="+133"/>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="+147"/>
        <source>SQL script (*.sql);;All files (*.*)</source>
        <translation>Script SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Portable Network Graphics (*.png);;All files (*.*)</source>
        <translation>Portable Network Graphics (*.png);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Scalable Vector Graphics (*.svg);;All files (*.*)</source>
        <translation>Scalable Vector Graphics (*.svg);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="-485"/>
        <source>This advanced option causes pgModeler to ignore extra errors by their numeric codes. These errors must be informed in the input below and separeted by space. For the complete list of error codes check the PostgreSQL docs, section &lt;strong&gt; Appendix A. PostgreSQL Error Codes&lt;/strong&gt;. &lt;strong&gt;WARNING:&lt;/strong&gt; use this option with extreme care since it can interfere in final export result.</source>
        <translation>Cette option avancée oblige pgModeler à ignorer les erreurs supplémentaires par leurs codes numériques. Ces erreurs doivent être renseignées dans l&apos;entrée ci-dessous et séparées par des espaces. Pour la liste complète des codes d&apos;erreur, consulter les documents PostgreSQL, section &lt;strong&gt;Annexe A. Codes d&apos;erreur PostgreSQL&lt;/strong&gt;. &lt;strong&gt;AVERTISSEMENT :&lt;/strong&gt; utiliser cette option avec un soin extrême, car elle peut interférer dans le résultat de l&apos;export finale.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ignore error codes</source>
        <translation>Ignorer les codes d&apos;erreur</translation>
    </message>
    <message>
        <location line="+502"/>
        <source>&amp;Vectorial (SVG)</source>
        <translation>&amp;Vectoriel (SVG)</translation>
    </message>
</context>
<context>
    <name>ModelExportHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="+74"/>
        <source>Generating SQL code for PostgreSQL `%1&apos;</source>
        <translation>Génération du code SQL pour PostgreSQL `%1&apos;</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Output SQL file `%1&apos; successfully written.</source>
        <translation>Fichier SQL de sortie `%1&apos; écrit avec succès.</translation>
    </message>
    <message>
        <location line="+124"/>
        <source>Rendering objects to page %1/%2.</source>
        <translation>Rendu des objets de la page %1/%2.</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Output image `%1&apos; successfully written.</source>
        <translation>Fichier image de sortie `%1&apos; écrit avec succès.</translation>
    </message>
    <message>
        <location line="+121"/>
        <source>Starting export to DBMS.</source>
        <translation>Lancement de l&apos;export vers le SGBD.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>PostgreSQL version detection overridden. Using version `%1&apos;.</source>
        <translation>Détection de version dépassée de PostgreSQL. Utilisation de la version `%1&apos;.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>PostgreSQL `%1&apos; server detected.</source>
        <translation>Serveur PostgreSQL `%1&apos; détecté.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Generating temporary names for database, roles and tablespaces.</source>
        <translation>Génération de noms, de rôles et d&apos;espace de stockage temporaires pour la base de données.</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Enabling the SQL code for database `%1&apos; to avoid errors.</source>
        <translation>Activation du code SQL de la base de données `%1&apos; pour éviter les erreurs.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ignoring object duplication errors.</source>
        <translation>Omission des erreurs de duplication d&apos;objet.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Ignoring the following error code(s): `%1&apos;.</source>
        <translation>Omission des codes erreur suivants : `%1&apos;.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Trying to drop database `%1&apos;.</source>
        <translation>Tentative de suppression de la base de données `%1&apos;.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Simulation mode activated.</source>
        <translation>Mode simulation activé.</translation>
    </message>
    <message>
        <location line="+104"/>
        <source>Generating SQL for `%1&apos; objects...</source>
        <translation>Génération de code SQL pour `%1&apos; objets...</translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Starting data dictionary generation...</source>
        <translation>Début de la génération du dictionnaire des données...</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Data dictionary successfully saved into `%1&apos;.</source>
        <translation>Le dictionnaire des données à été sauvegardé avec succès dans `%1&apos;.</translation>
    </message>
    <message>
        <location line="+71"/>
        <source>Destroying objects created on the server.</source>
        <translation>Destruction des objets créés sur le server.</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Restoring original names of database, roles and tablespaces.</source>
        <translation>Restauration des noms, des rôles et des espaces de stockage originaux de la base de données.</translation>
    </message>
    <message>
        <location line="-267"/>
        <location line="+477"/>
        <location line="+86"/>
        <source>Creating object `%1&apos; (%2)</source>
        <translation>Création de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="-88"/>
        <location line="+90"/>
        <source>Dropping object `%1&apos; (%2)</source>
        <translation>Suppression de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Changing object `%1&apos; (%2)</source>
        <translation>Modification de l&apos;objet `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Running auxiliary `%1&apos; command...</source>
        <translation>Exécution de la commande auxiliaire `%1&apos;...</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Running auxiliary command.</source>
        <translation>Exécution de la commande auxiliaire.</translation>
    </message>
    <message>
        <location line="-759"/>
        <source>Exporting model to SVG file.</source>
        <translation>Export du modèle vers un fichier SVG.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>SVG representation of database model</source>
        <translation>Représentation SVG d&apos;un modèle de base de données</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>SVG file generated by pgModeler</source>
        <translation>Fichier SVG généré par pgModeler</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Output file `%1&apos; successfully written.</source>
        <translation>Fichier de sortie `%1&apos; écrit avec succès.</translation>
    </message>
    <message>
        <location line="+163"/>
        <source>Creating database `%1&apos;</source>
        <translation>Création de la base de données `%1&apos;</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Connecting to database `%1&apos;</source>
        <translation>Connexion à la base de données `%1&apos;</translation>
    </message>
    <message>
        <location line="+238"/>
        <source>Renaming `%1&apos; (%2) to `%3&apos;</source>
        <translation>Renommage de `%1&apos; (%2) en `%3&apos;</translation>
    </message>
</context>
<context>
    <name>ModelFixForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelfixform.ui" line="+35"/>
        <source>Model file fix</source>
        <translation>Réparer un fichier modèle</translation>
    </message>
    <message>
        <location line="+636"/>
        <source>&amp;Fix</source>
        <translation>&amp;Réparer</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location line="-349"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;[pgmodeler-cli not found error]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;[pgmodeler-cli not found error]&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>pgmodeler-cli:</source>
        <translation>pgmodeler-cli :</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Browse for pgmodeler-cli tool</source>
        <translation>Rechercher l&apos;outil pgmodeler-cli</translation>
    </message>
    <message>
        <location line="-221"/>
        <location line="+224"/>
        <location line="+103"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>The specified file is not the pgModeler command line tool (pgmodeler-cli).</source>
        <translation>Ce fichier n&apos;est pas l&apos;outil en ligne de commande de pgModeler (pgmodeler-cli).</translation>
    </message>
    <message>
        <location line="-460"/>
        <source>Input file:</source>
        <translation>Fichier source :</translation>
    </message>
    <message>
        <location line="+145"/>
        <source>&lt;!DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Monospace&apos;; font-size:11pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:9pt;&quot;&gt;Waiting process to start...&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation type="unfinished">&lt; !DOCTYPE HTML PUBLIC &quot;-//W3C//DTD HTML 4.0//EN&quot; &quot;http ://www.w3.org/TR/REC-html40/strict.dtd&quot;&gt;
&lt;html&gt;&lt;head&gt;&lt;meta name=&quot;qrichtext&quot; content=&quot;1&quot; /&gt;&lt;style type=&quot;text/css&quot;&gt;
p, li { white-space: pre-wrap; }
&lt;/style&gt;&lt;/head&gt;&lt;body style=&quot; font-family:&apos;Monospace&apos;; font-size:10pt; font-weight:400; font-style:normal;&quot;&gt;
&lt;p style=&quot; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;&quot;&gt;&lt;span style=&quot; font-size:9pt;&quot;&gt;En attente du lancement du processus...&lt;/span&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt; {3C?} {4.0/?} {3.?} {40/?} {1&quot;?} {11p?} {400;?} {0p?} {0p?} {0p?} {0p?} {0;?} {0p?} {9p?}</translation>
    </message>
    <message>
        <location line="+330"/>
        <source>Output file:</source>
        <translation>Fichier cible :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Fix tries:</source>
        <translation>Tentatives :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Select input file</source>
        <translation>Sélectionner le modèle à réparer</translation>
    </message>
    <message>
        <location line="-327"/>
        <source>Select output file</source>
        <translation>Sélectionner un fichier de sortie</translation>
    </message>
    <message>
        <location line="-149"/>
        <source>Load fixed model when finish</source>
        <translation>Charger le modèle réparé une fois l&apos;opération terminée</translation>
    </message>
    <message>
        <location line="+332"/>
        <source>In some cases the fix process will fail to restore all objects within the model demanding manual fixes by changing the file on a text editor. &lt;strong&gt;NOTE:&lt;/strong&gt; relationships may lost their graphical configuration like custom points and line color.</source>
        <translation>Dans certains cas, le processus de réparation peut échouer à restaurer et récupérer tous les objets du modèle, ce qui peut réclamer des modifications manuelles dans le fichier à partir d&apos;un éditeur de texte. &lt;strong&gt;REMARQUE :&lt;/strong&gt; les associations peuvent perdre leur configuration graphique tels que des points placés manuellement par l&apos;utilisateur ou la couleur.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelfixform.cpp" line="+69"/>
        <source>Waiting process to start...</source>
        <translation>Processus en attente de lancement...</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Could not locate &lt;strong&gt;%1&lt;/strong&gt; tool on &lt;strong&gt;%2&lt;/strong&gt;. The fix process can&apos;t continue! Please check pgModeler installation or try to manually specify the command below.</source>
        <translation>Impossible de localiser l&apos;outil &lt;strong&gt;%1&lt;/strong&gt; sur &lt;strong&gt;%2&lt;/strong&gt;. Le processus de réparation ne peut continuer ! Vérifiez votre installation de pgModeler ou spécifiez manuellement la commande ci-dessous.</translation>
    </message>
    <message>
        <location line="+71"/>
        <source>pgModeler command line tool (%1)</source>
        <translation>Outil en ligne de commande de pgModeler (%1)</translation>
    </message>
</context>
<context>
    <name>ModelNavigationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="+23"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Previous model</source>
        <translation>Modèle précédent</translation>
    </message>
    <message>
        <location line="+117"/>
        <source>Alt+C</source>
        <translation></translation>
    </message>
    <message>
        <location line="-76"/>
        <source>Next model</source>
        <translation>Modèle suivant</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Close model</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelnavigationwidget.cpp" line="+73"/>
        <source>(model not saved yet)</source>
        <translation>(modèle pas encore enregistré)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelnavigationwidget.ui" line="-88"/>
        <source>Ctrl+Left</source>
        <translation>Ctrl+Gauche</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Ctrl+Right</source>
        <translation>Ctrl+Droite</translation>
    </message>
</context>
<context>
    <name>ModelObjectsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="+35"/>
        <source>Model Objects</source>
        <translation>Objets du modèle</translation>
    </message>
    <message>
        <location line="+532"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="-341"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location line="+102"/>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Clear All</source>
        <translation>Tout effacer</translation>
    </message>
    <message>
        <location line="+200"/>
        <source>Select</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Return</source>
        <translation>Retour</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Esc</source>
        <translation>Échap</translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Objects view configuration</source>
        <translation>Filtres d&apos;affichage des objets</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Expands all items</source>
        <translation>Développer tous les éléments</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Collapses all items</source>
        <translation>Replier tous les éléments</translation>
    </message>
    <message>
        <location line="-276"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location line="-47"/>
        <location line="+180"/>
        <location line="+38"/>
        <location line="+35"/>
        <location line="+38"/>
        <location line="+35"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-152"/>
        <source>Tree view</source>
        <translation>Vue en arbre</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>List view</source>
        <translation>Vue en liste</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelobjectswidget.cpp" line="+184"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="-474"/>
        <source>ID</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location line="+323"/>
        <source>By ID</source>
        <translation>Par identifiant</translation>
    </message>
    <message>
        <location line="-246"/>
        <source>Visible object types</source>
        <translation>Types d&apos;objets visibles</translation>
    </message>
    <message>
        <location line="+147"/>
        <source>Model objects</source>
        <translation>Objets de modèle</translation>
    </message>
</context>
<context>
    <name>ModelOverviewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeloverviewwidget.ui" line="+41"/>
        <source>Model overview</source>
        <translation>Vue générale du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modeloverviewwidget.cpp" line="+141"/>
        <source>Failed to generate the overview image.
The requested size %1 x %2 was too big and there was not enough memory to allocate!</source>
        <translation>Impossible de générer l&apos;aperçu de l&apos;image.
La taille demandée%1 x%2 était trop grande et il n&apos;y avait pas assez de mémoire à allouer !</translation>
    </message>
</context>
<context>
    <name>ModelRestorationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelrestorationform.ui" line="+35"/>
        <source>Model restoration</source>
        <translation>Restauration de modèle</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurer</translation>
    </message>
    <message>
        <location line="+160"/>
        <source>&lt;p&gt;pgModeler will try to recover the selected models but will not destroy them in case of loading failure. This option serves as a last resort in order to try to recover the database model. Temporary models will last until the application is closed so the user must try to manually recover the files before exit pgModeler.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+105"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location line="-129"/>
        <source>pgModeler was not closed properly in a previous execution and some models were still being edited. Click &lt;strong&gt;Restore&lt;/strong&gt; to reopen the models or &lt;strong&gt;Cancel&lt;/strong&gt; to abort the restoration.</source>
        <translation>pgModeler n&apos;a pas été fermé correctement lors d&apos;une précédente exécution et certains modèles étaient encore en cours d&apos;édition. Cliquez sur &lt;strong&gt;Restaurer&lt;/strong&gt; pour rouvrir les modèles ou sur &lt;strong&gt;Annuler&lt;/strong&gt; pour abandonner la restauration.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Keep temporary models in case of restoration failure</source>
        <translation>Conserver les modèles temporaires en cas d&apos;échec de la restauration</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Database</source>
        <translation>Base de données</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>File</source>
        <translation>Fichier</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Modified</source>
        <translation>Modifié</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Size</source>
        <translation>Taille</translation>
    </message>
</context>
<context>
    <name>ModelValidationHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationhelper.cpp" line="+621"/>
        <source>There are pending errors! SQL validation will not be executed.</source>
        <translation>Il y a des erreurs en attente ! La validation SQL ne sera pas exécutée.</translation>
    </message>
    <message>
        <location line="+84"/>
        <source>Operation canceled by the user.</source>
        <translation>Opération annulée par l&apos;utilisateur.</translation>
    </message>
</context>
<context>
    <name>ModelValidationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="+32"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+453"/>
        <location line="+88"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location line="-476"/>
        <source>Try to apply a fix on the selected validation info.</source>
        <translation>Essayer d&apos;appliquer le correctif sur l’élément sélectionné.</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Clear validation results</source>
        <translation>Effacer le résultat de la validation</translation>
    </message>
    <message>
        <location line="-70"/>
        <source>Va&amp;lidate</source>
        <translation>Va&amp;lider</translation>
    </message>
    <message>
        <location line="+398"/>
        <source>Warnings: does not prevents model to be saved.</source>
        <translation>Avertissements : n&apos;empêche de pouvoir sauvegarder le modèle.</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>Errors: model will not be saved while there are validation errors.</source>
        <translation>Erreurs : le modèle ne peut être sauvegardé tant que des erreurs de validation subsistent.</translation>
    </message>
    <message>
        <location line="-326"/>
        <source>SQL Validation:</source>
        <translation>Validation SQL :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>PostgreSQL version</source>
        <translation>Version de PostgreSQL</translation>
    </message>
    <message>
        <location line="+436"/>
        <source>Try to resolve the reported issues.</source>
        <translation>Essayer de résoudre les problèmes rencontrés.</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location line="-561"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location line="-35"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location line="+94"/>
        <source>&lt;p&gt;pgModeler will generate unique and temporary names for database, role and tablespace objects. This option avoids object duplication errors when running the SQL validation.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+19"/>
        <source>&lt;p&gt;Enables the validation of SQL code in DBMS. This process requires the use of a pre-configured connection. SQL validation will occur only in the last step (when all objects were validated) or when there are no warnings.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+125"/>
        <source>Cancel the SQL validation in progress.</source>
        <translation>Annuler le processus de validation SQL en cours.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Esc</source>
        <translation>Échap</translation>
    </message>
    <message>
        <location line="+329"/>
        <source>Apply fixes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Change the creation order for two objects by swapping their ids</source>
        <translation>Modifie l&apos;ordre de création de deux objets en échangeant leurs numéros d&apos;identification</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Swap ids</source>
        <translation type="unfinished">Échanger identifiants</translation>
    </message>
    <message>
        <location line="-70"/>
        <source>Hide this widget</source>
        <translation>Fermer ce widget</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="+266"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] is being referenced by &lt;strong&gt;%4&lt;/strong&gt; object(s) before its creation.</source>
        <translation>L&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] est référencé par &lt;strong&gt;%4&lt;/strong&gt; objet(s) avant sa création.</translation>
    </message>
    <message>
        <location line="-234"/>
        <source>Autodetect</source>
        <translation>Auto détection</translation>
    </message>
    <message>
        <location line="+249"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3]%4 is referencing columns created by &lt;strong&gt;%5&lt;/strong&gt; relationship(s) but is created before them.</source>
        <translation>L&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id : %3]%4 référence des colonnes créées par &lt;strong&gt;%5&lt;/strong&gt; association(s) mais a été créé avant eux.</translation>
    </message>
    <message>
        <location line="+260"/>
        <source>Processing object: %1</source>
        <translation>Traitement de l&apos;objet : %1</translation>
    </message>
    <message>
        <location line="-131"/>
        <source>Conflicting object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;.</source>
        <translation>Objet en conflit : &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;.</translation>
    </message>
    <message>
        <location line="-110"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; has a name that conflicts with &lt;strong&gt;%3&lt;/strong&gt; object name(s).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+122"/>
        <source>Referrer object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</source>
        <translation>Objet réferent : &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</translation>
    </message>
    <message>
        <location line="-106"/>
        <source>The column &lt;strong&gt;%1&lt;/strong&gt; on &lt;strong&gt;%2&lt;/strong&gt; &lt;em&gt;(%3)&lt;/em&gt; is referencing the geospatial data type &lt;strong&gt;%4&lt;/strong&gt; but the &lt;strong&gt;postgis&lt;/strong&gt; extension is not present in the model!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+47"/>
        <source>&lt;strong&gt;HINT:&lt;/strong&gt; Create the extension in the model or let it be created by applying the needed fixes.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+41"/>
        <source>&lt;em&gt;The above object was created by a relationship. Change the name pattern on it&apos;s generator relationship. Fix will not be applied!&lt;/em&gt;</source>
        <translation>&lt;em&gt;L&apos;objet ci-dessus a été créé par une relation. Changer le motif du nom sur sa relation de générateur. La correction ne sera pas appliquée !&lt;/Em&gt;</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Relationship: &lt;strong&gt;%1&lt;/strong&gt; [id: %2].</source>
        <translation>Relation : &lt;strong&gt;%1&lt;/strong&gt; [id : %2].</translation>
    </message>
    <message>
        <location line="+112"/>
        <source>Running SQL commands on server...</source>
        <translation>Exécution des commandes SQL sur le serveur...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="-341"/>
        <source>Connection to be used in the SQL validation</source>
        <translation>Connexion à utiliser dans la validation SQL</translation>
    </message>
    <message>
        <location line="-110"/>
        <source>Use unique temporary names for cluster level objects</source>
        <translation>Utiliser des noms temporaires uniques pour les objets de niveau cluster</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="-222"/>
        <source>The relationship &lt;strong&gt;%1&lt;/strong&gt; [id: %2] is in a permanent invalidation state and needs to be relocated.</source>
        <translation>La relation &lt;strong&gt;%1&lt;/strong&gt; [id : %2] est dans un état d&apos;invalidation permanent et a besoin d&apos;être déplacé.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>SQL validation failed due to the error(s) below. &lt;strong&gt;NOTE:&lt;/strong&gt;&lt;em&gt; Errors during SQL validation don&apos;t invalidate the model but may affect operations like &lt;strong&gt;export&lt;/strong&gt; and &lt;strong&gt;diff&lt;/strong&gt;.&lt;/em&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+47"/>
        <source>&lt;strong&gt;HINT:&lt;/strong&gt; try to swap the relationship by another ones that somehow are linked to it through generated columns or constraints to solve this issue. Note that other objects may be lost in the swap process.</source>
        <translation>&lt;strong&gt;CONSEIL :&lt;/strong&gt; essayer d&apos;échanger la relation par une autre qui est liée à celle-ci via des colonnes générées ou des contraintes pour résoudre ce problème. Noter que d&apos;autres objets peuvent être perdus dans le processus d&apos;échange.</translation>
    </message>
    <message>
        <location line="+135"/>
        <source>SQL validation not executed! No connection defined.</source>
        <translation>La validation SQL n&apos;est pas exécutée ! Aucune connexion définie.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Database model successfully validated.</source>
        <translation>Modèle de base de données validé avec succès.</translation>
    </message>
</context>
<context>
    <name>ModelWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="+239"/>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Convert</source>
        <translation>Convertir</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Quick</source>
        <translation>Actions rapides</translation>
    </message>
    <message>
        <location line="+3670"/>
        <source>Source code</source>
        <translation>Code source</translation>
    </message>
    <message>
        <location line="-3711"/>
        <source>Show object source code</source>
        <translation>Afficher le code source de l&apos;objet</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+3702"/>
        <source>Properties</source>
        <translation>Propriétés</translation>
    </message>
    <message>
        <location line="-3701"/>
        <source>Space</source>
        <translation>Espace</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Edit the object properties</source>
        <translation>Modifier les propriétés de l&apos;objet</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+3727"/>
        <source>Protect</source>
        <translation>Vérrouiller</translation>
    </message>
    <message>
        <location line="-3726"/>
        <location line="+3721"/>
        <source>Unprotect</source>
        <translation>Déverrouiller</translation>
    </message>
    <message>
        <location line="-3720"/>
        <source>Protects object(s) from modifications</source>
        <translation>Empêcher la modification du ou des objets</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+3730"/>
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location line="-3725"/>
        <source>Shift+Del</source>
        <translation>Shift+Suppr</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Select all</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Selects all the graphical objects in the model</source>
        <translation>Sélectionner tous les objets graphiques du modèle</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Paste</source>
        <translation>Coller</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Cut</source>
        <translation>Couper</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Deps &amp;&amp; Referrers</source>
        <translation>Voir les dépendances &amp;&amp; les références</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Add a new object in the model</source>
        <translation>Ajouter un nouvel objet dans le modèle</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Quick rename the object</source>
        <translation>Renommage rapide de l&apos;objet</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Move to schema</source>
        <translation>Déplacer vers le schema</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Edit permissions</source>
        <translation>Modifier les permissions</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Change owner</source>
        <translation>Changer le propriétaire</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Select children</source>
        <translation>Sélectionner les enfants</translation>
    </message>
    <message>
        <location line="-53"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location line="+59"/>
        <source>Custom SQL</source>
        <translation>Code SQL personnalisé</translation>
    </message>
    <message>
        <location line="+165"/>
        <source>Stacking</source>
        <translation>Positionnement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Send to back</source>
        <translation>Envoyer à l&apos;arrière plan</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Bring to front</source>
        <translation>Ramener au premier plan</translation>
    </message>
    <message>
        <location line="+963"/>
        <source>Loading database model</source>
        <translation>Ouverture du modèle de base de données</translation>
    </message>
    <message>
        <location line="+237"/>
        <source>Saving database model</source>
        <translation>Enregistrement du modèle de base de données</translation>
    </message>
    <message>
        <location line="+647"/>
        <source>Do you want to %1 the children of the schema &lt;strong&gt;%2&lt;/strong&gt; too?</source>
        <translation>Souhaitez vous également %1 l&apos;objet enfant du schéma &lt;strong&gt;%2&lt;/strong&gt; ?</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Also copy all dependencies of selected objects? This minimizes the breakdown of references when copied objects are pasted into another model.</source>
        <translation>Copier également les dépendences des objets sélectionnés ? Cela minimise la casse des références lorsque les objets copiés sont collés dans un autre modèle.</translation>
    </message>
    <message>
        <location line="+106"/>
        <source>Pasting objects...</source>
        <translation>Collage des objets...</translation>
    </message>
    <message>
        <location line="+292"/>
        <source>Not all objects were pasted to the model due to errors returned during the process! Refer to error stack for more details!</source>
        <translation>Tous les objets n&apos;ont pas été collés dans le modèle car des erreurs ont été retournées durant le processus ! Se référer à la pile d&apos;erreurs pour plus de détails !</translation>
    </message>
    <message>
        <location line="+182"/>
        <source>Do you really want to delete the selected object?</source>
        <translation>Souhaitez-vous réellement supprimer l&apos;objet sélectionné ?</translation>
    </message>
    <message>
        <location line="+410"/>
        <source>(no objects)</source>
        <translation>(aucun objet)</translation>
    </message>
    <message>
        <location line="+7"/>
        <location line="+5"/>
        <source>None</source>
        <translation>Aucun</translation>
    </message>
    <message>
        <location line="+201"/>
        <source>Table &amp;&amp; Relationships</source>
        <translation>Table &amp;&amp; ses associations</translation>
    </message>
    <message>
        <location line="+397"/>
        <source>Constraints</source>
        <translation>Contraintes</translation>
    </message>
    <message>
        <location line="-3839"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; The database model is protected! Operations that could modify it are disabled!</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Le modèle de base de données est protégé ! Les opérations qui pourraient le modifier sont désactivées !</translation>
    </message>
    <message>
        <location line="+76"/>
        <source>Edit data</source>
        <translation>Modifier les données</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Source</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Alt+S</source>
        <translation></translation>
    </message>
    <message>
        <location line="+15"/>
        <location line="+3733"/>
        <source>Del. cascade</source>
        <translation>Supprimer en cascade</translation>
    </message>
    <message>
        <location line="-3722"/>
        <source>Ctrl+C</source>
        <translation></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Ctrl+V</source>
        <translation></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Ctrl+X</source>
        <translation></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Quick action for the selected object</source>
        <translation>Action rapide pour l&apos;objet sélectionné</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>F2</source>
        <translation></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Move to layer</source>
        <translation>Déplacer vers la couche</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Set tag</source>
        <translation>Affecter une balise</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Ctrl+E</source>
        <translation></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Select tagged</source>
        <translation>Séléction balisée</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Select</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Open relationship</source>
        <translation>Ouvrir les relations</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Alt+Q</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Convert to sequence</source>
        <translation>Convertir en séquence</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Convert to serial</source>
        <translation>Convertir en série</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Break line</source>
        <translation>Cassure de ligne</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Remove points</source>
        <translation>Supprimer des points</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Enable SQL</source>
        <translation>Activer le code SQL</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Disable SQL</source>
        <translation>Désactiver le code SQL</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Duplicate</source>
        <translation>Dupliquer</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Ctrl+D</source>
        <translation></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Pagination</source>
        <translation>Pagination des attributs</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Enable</source>
        <translation>Activer</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Disable</source>
        <translation>Désactiver</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Collapse</source>
        <translation>Replier</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Not collapsed</source>
        <translation>Aucun repliement (tout afficher)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Extended attributes</source>
        <translation>Attributs étendus uniquement</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>All attributes</source>
        <translation>Tous les attributs</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Show</source>
        <translation>Afficher</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Hide</source>
        <translation>Masquer</translation>
    </message>
    <message>
        <location line="-10"/>
        <source>Jump to table</source>
        <translation>Aller à la table</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Schemas rectangles</source>
        <translation>Rectangles des schémas</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Fade in/out</source>
        <translation>Transparence</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+3"/>
        <source>Fade in</source>
        <translation>Accentuer</translation>
    </message>
    <message>
        <location line="-2"/>
        <location line="+3"/>
        <source>Fade out</source>
        <translation>Estomper</translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+82"/>
        <location line="+3079"/>
        <source>Relationships</source>
        <translation>Associations</translation>
    </message>
    <message>
        <location line="-3154"/>
        <location line="+4408"/>
        <source>Swap ids</source>
        <translation>Échanger les identifiants</translation>
    </message>
    <message>
        <location line="-4407"/>
        <source>Edit the objects creation order by swapping their ids</source>
        <translation>Modifier l&apos;ordre de création des objets en échangeant leurs identifiants</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>90° (vertical)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>90° (horizontal)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>90° + 90° (vertical)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>90° + 90° (horizontal)</source>
        <translation></translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Database object</source>
        <translation>Objet de base de données</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Schema object</source>
        <translation>Objet de schéma</translation>
    </message>
    <message>
        <location line="+31"/>
        <location line="+3100"/>
        <location line="+6"/>
        <source>All objects</source>
        <translation>Tous les objets</translation>
    </message>
    <message>
        <location line="-3106"/>
        <location line="+3080"/>
        <source>Schemas</source>
        <translation>Schémas</translation>
    </message>
    <message>
        <location line="-3080"/>
        <source>Foreign Tables</source>
        <translation>Tables distantes</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+3079"/>
        <source>Views</source>
        <translation>Vues</translation>
    </message>
    <message>
        <location line="-3079"/>
        <location line="+3079"/>
        <source>Textboxes</source>
        <translation>Boîtes de textes</translation>
    </message>
    <message>
        <location line="-3080"/>
        <location line="+3080"/>
        <source>Tables</source>
        <translation>Tables</translation>
    </message>
    <message>
        <location line="-3071"/>
        <source>Ctrl+A</source>
        <translation></translation>
    </message>
    <message>
        <location line="+358"/>
        <source>Zoom: %1%</source>
        <translation>Zoom : %1%</translation>
    </message>
    <message>
        <location line="+400"/>
        <source>Do you really want to convert the relationship into an intermediate table?</source>
        <translation>Voulez-vous vraiment convertir les associations en une table intermédiaire ?</translation>
    </message>
    <message>
        <location line="+1309"/>
        <source>Validating object: `%1&apos; (%2)</source>
        <translation>Validation de l&apos;objet : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+102"/>
        <source>Generating XML for: `%1&apos; (%2)</source>
        <translation>Génération du code XML pour : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+105"/>
        <source>Pasting object: `%1&apos; (%2)</source>
        <translation>Collage de l&apos;objet : `%1&apos; (%2)</translation>
    </message>
    <message>
        <location line="+231"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; You are about to delete objects in cascade mode which means more objects than the selected will be dropped too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Vous êtes sur le point de supprimer des objets en mode cascade, ce qui signifie que des objets non sélectionnés seront également supprimés. Souhaitez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; Remove multiple objects at once can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Supprimer plusieurs objets à la fois peut entraîner des invalidations irréversibles sur d&apos;autres objets du modèle, entraînant également la suppression de ces objets invalides. Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; Remove a relationship can cause irreversible invalidations to other objects in the model causing such invalid objects to be deleted too. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Supprimer une association peut entraîner des invalidations irréversibles d&apos;autres objets dans le modèle, entraînant également la suppression de ces objets invalides. Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location line="+250"/>
        <source>The cascade deletion found some problems when running! Some objects could not be deleted or registered in the operation&apos;s history! Please, refer to error stack for more details.</source>
        <translation>La suppression en cascade a rencontré des problèmes lors de son exécution ! Certains objets n&apos;ont pas pu être supprimés ou enregistrés dans l&apos;historique des opérations ! Veuillez vous reporter à la pile d&apos;erreurs pour plus de détails.</translation>
    </message>
    <message>
        <location line="-901"/>
        <source>protect</source>
        <translation>protéger</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>unprotect</source>
        <translation>déprotéger</translation>
    </message>
</context>
<context>
    <name>ModelsDiffHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelsdiffhelper.cpp" line="+260"/>
        <source>Processing object `%1&apos; (%2)...</source>
        <translation>Traitement de l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location line="+120"/>
        <source>Skipping object `%1&apos; (%2)...</source>
        <translation>Omission de l&apos;objet `%1&apos; (%2)...</translation>
    </message>
    <message>
        <location line="+272"/>
        <source>Processing diff infos...</source>
        <translation>Comparaison des informations...</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Processing `%1&apos; info for object `%2&apos; (%3)...</source>
        <translation>Traitement de l&apos;info `%1&apos; pour l&apos;objet `%2&apos; (%3)...</translation>
    </message>
    <message>
        <location line="+253"/>
        <source>No differences between the model and database.</source>
        <translation>Aucune différence entre le modèle et la base de données.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Preparing diff code...</source>
        <translation>Préparation du code de comparaison...</translation>
    </message>
</context>
<context>
    <name>NewObjectOverlayWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="+28"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+299"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location line="+311"/>
        <source>Tag</source>
        <translation>Tag</translation>
    </message>
    <message>
        <location line="-91"/>
        <source>Cast</source>
        <translation>Conversion de type</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Language</source>
        <translation>Langage</translation>
    </message>
    <message>
        <location line="+179"/>
        <source>Textbox</source>
        <translation>Zone de texte</translation>
    </message>
    <message>
        <location line="-223"/>
        <source>Event Trigger</source>
        <translation>Déclencheur sur évènement</translation>
    </message>
    <message>
        <location line="+135"/>
        <source>Tablespace</source>
        <translation>Tablespace</translation>
    </message>
    <message>
        <location line="-179"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location line="+491"/>
        <source>Domain</source>
        <translation>Domaine</translation>
    </message>
    <message>
        <location line="-88"/>
        <source>Conversion</source>
        <translation>Conversion</translation>
    </message>
    <message>
        <location line="-623"/>
        <source>Extension</source>
        <translation></translation>
    </message>
    <message>
        <location line="+799"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Aggregate</source>
        <translation>Agrégat</translation>
    </message>
    <message>
        <location line="-88"/>
        <source>Collation</source>
        <translation></translation>
    </message>
    <message>
        <location line="+220"/>
        <source>Sequence</source>
        <translation>Séquence</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Table</source>
        <translation></translation>
    </message>
    <message>
        <location line="+308"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location line="-220"/>
        <source>View</source>
        <translation>Vue</translation>
    </message>
    <message>
        <location line="-1038"/>
        <source>Database objects</source>
        <translation>Objets de base de données</translation>
    </message>
    <message>
        <location line="+151"/>
        <source>Data Wrapper</source>
        <translation>Wrapper de données</translation>
    </message>
    <message>
        <location line="+44"/>
        <location line="+1243"/>
        <source>Permissions</source>
        <translation></translation>
    </message>
    <message>
        <location line="-1155"/>
        <source>Server</source>
        <translation>Serveur</translation>
    </message>
    <message>
        <location line="+355"/>
        <source>User mapping</source>
        <translation>Correspondance d&apos;utilisateur</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Schema objects</source>
        <translation>Objets de schéma</translation>
    </message>
    <message>
        <location line="+547"/>
        <source>Foreign Table</source>
        <translation>Table distante</translation>
    </message>
    <message>
        <location line="+73"/>
        <source>Table objects</source>
        <translation>Objets de table</translation>
    </message>
    <message>
        <location line="+195"/>
        <source>Index</source>
        <translation></translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Rule</source>
        <translation>Règle</translation>
    </message>
    <message>
        <location line="+400"/>
        <source>Partitioning</source>
        <translation>Partionnement</translation>
    </message>
    <message>
        <location line="-532"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location line="+220"/>
        <source>Trigger</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location line="+312"/>
        <source>Copy</source>
        <translation>Copie</translation>
    </message>
    <message>
        <location line="-1410"/>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="+32"/>
        <source>A</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/newobjectoverlaywidget.cpp" line="+1"/>
        <source>G</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>K</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>H</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>J</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>D</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>E</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>F</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>L</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>O</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>U</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>I</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>R</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>S</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Q</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>T</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>P</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>M</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Y</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>W</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+6"/>
        <source>9</source>
        <translation></translation>
    </message>
    <message>
        <location line="-5"/>
        <source>Z</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>X</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>C</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>V</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>B</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>8</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>7</source>
        <translation>7</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+10"/>
        <source>6</source>
        <translation>6</translation>
    </message>
    <message>
        <location line="-5"/>
        <source>1</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>2</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>3</source>
        <translation></translation>
    </message>
    <message>
        <location line="-6"/>
        <location line="+7"/>
        <source>5</source>
        <translation></translation>
    </message>
    <message>
        <location line="-6"/>
        <location line="+7"/>
        <source>4</source>
        <translation></translation>
    </message>
    <message>
        <location line="+31"/>
        <source>0</source>
        <translation></translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/newobjectoverlaywidget.ui" line="+610"/>
        <source>Op. Family</source>
        <translation>Famille d&apos;op.</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Op. Class</source>
        <translation>Classe d&apos;op.</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location line="+473"/>
        <source>Relationships</source>
        <translation>Associations</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Many-to-many</source>
        <translation>Plusieurs-à-plusieurs</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>One-to-many</source>
        <translation>Un-à-plusieurs</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>One-to-one</source>
        <translation>Un-à-un</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Inheritance</source>
        <translation>Héritage</translation>
    </message>
    <message>
        <location line="-1775"/>
        <source>Generic SQL</source>
        <translation>SQL générique</translation>
    </message>
    <message>
        <location line="+1551"/>
        <source>Policy</source>
        <translation>Politique</translation>
    </message>
</context>
<context>
    <name>NumberedTextEditor</name>
    <message>
        <location filename="../libpgmodeler_ui/src/numberedtexteditor.cpp" line="+84"/>
        <source>Load</source>
        <translation>Charger</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Load the object&apos;s source code from an external file</source>
        <translation>Charger le code source de l&apos;objet depuis un fichier externe</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Edit</source>
        <translation>Modifier</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Edit the source code in the preferred external editor</source>
        <translation>Modifier le code source avec son éditeur préféré</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location line="+124"/>
        <source>Upper case</source>
        <translation>Majuscule</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Lower case</source>
        <translation>Minuscule</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Ident right</source>
        <translation>Ident droit</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ident left</source>
        <translation>Ident gauche</translation>
    </message>
    <message>
        <location line="+102"/>
        <source>SQL file (*.sql);;All files (*.*)</source>
        <translation>Fichier SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Load file</source>
        <translation>Charger un fichier</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>The source editor `%1&apos; is running on `pid: %2&apos;.</source>
        <translation>L&apos;éditeur de source `%1&apos; s&apos;exécute sur pid : `%2&apos;.</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Failed to the source code editor &lt;strong&gt;%1&lt;/strong&gt;! Make to sure that the source editor path points to a valid executable and the current user has permission to run the application. Error message returned: &lt;strong&gt;%2&lt;/strong&gt;</source>
        <translation type="unfinished">Impossible </translation>
    </message>
</context>
<context>
    <name>ObjectDepsRefsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="+49"/>
        <source>Dependencies</source>
        <translation>Dépendances</translation>
    </message>
    <message>
        <location line="+91"/>
        <location line="+153"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="-141"/>
        <location line="+153"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="-141"/>
        <location line="+153"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location line="-141"/>
        <location line="+153"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location line="-127"/>
        <source>References</source>
        <translation>Références</translation>
    </message>
    <message>
        <location line="-74"/>
        <location line="+153"/>
        <source>ID</source>
        <translation>ID</translation>
    </message>
    <message>
        <location line="-90"/>
        <source>Exclude indirect dependencies</source>
        <translation>Exclure les dépendances indirectes</translation>
    </message>
    <message>
        <location line="+153"/>
        <source>Include indirect references</source>
        <translation>Inclure les références indirectes</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>This object does not exists anymore. The dependencies and references listing are disabled.</source>
        <translation>Cet objet n&apos;existe plus. L&apos;affichage de ses dépendances et références est désactivé.</translation>
    </message>
    <message>
        <location line="-411"/>
        <source>Object&apos;s dependencies &amp; references</source>
        <translation>Dépendances et références d&apos;un objet</translation>
    </message>
</context>
<context>
    <name>ObjectFinderWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+90"/>
        <source>Pattern:</source>
        <translation>Motif :</translation>
    </message>
    <message>
        <location line="+149"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="-92"/>
        <location line="+95"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-35"/>
        <source>Find</source>
        <translation>Chercher</translation>
    </message>
    <message>
        <location line="-35"/>
        <source>Clears the search results</source>
        <translation>Effacer les résultats</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Clear</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location line="-63"/>
        <source>(Un)selects the graphical objects in the results grid</source>
        <translation>(Dés)sélectionner les objets graphiques dans la grille des résultats</translation>
    </message>
    <message>
        <location line="+253"/>
        <source>Regular Expression</source>
        <translation>Expression régulières</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Exact Match</source>
        <translation>Correspondance exacte</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Select All</source>
        <translation>Tout sélectionner</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Clear All</source>
        <translation>Tout effacer</translation>
    </message>
    <message>
        <location line="+187"/>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="+28"/>
        <location line="+234"/>
        <source>Comment</source>
        <translation>Commentaire</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>The attribute of the objects in which the search will occur</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-311"/>
        <source>Case Sensitive</source>
        <translation>Sensible à la casse</translation>
    </message>
    <message>
        <location line="+235"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location line="-378"/>
        <source>Fades outs all the graphical objects in the results grid (or those not listed). The current fade in/out state of all objects is modified.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-238"/>
        <source>Defines the search filter</source>
        <translation>Défini le filtre de recherche</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="-234"/>
        <source>Name</source>
        <translation type="unfinished">Nom</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Signature</source>
        <translation>Signature</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Owner</source>
        <translation type="unfinished">Propriétaire</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Tablespace</source>
        <translatorcomment>Jargon</translatorcomment>
        <translation>Tablespace</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Data type</source>
        <translation>Type de donnée</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Return type</source>
        <translation>Type de retour</translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+4"/>
        <source>Listed</source>
        <translation>Listé</translation>
    </message>
    <message>
        <location line="-3"/>
        <location line="+4"/>
        <source>Not listed</source>
        <translation>Non listé</translation>
    </message>
    <message>
        <location line="+224"/>
        <source>Found &lt;strong&gt;%1&lt;/strong&gt; object(s).</source>
        <translation>&lt;strong&gt;%1&lt;/strong&gt; objet(s) trouvé(s).</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>No objects found.</source>
        <translation>Aucun objet trouvé.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="+565"/>
        <source>ID</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location line="-486"/>
        <source>Select</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>Fade out</source>
        <translation>Estomper</translation>
    </message>
</context>
<context>
    <name>ObjectRenameWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectrenamewidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+97"/>
        <source>....</source>
        <translation>....</translation>
    </message>
    <message>
        <location line="+119"/>
        <source>to:</source>
        <translation>en :</translation>
    </message>
    <message>
        <location line="-138"/>
        <location line="+84"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectrenamewidget.cpp" line="+69"/>
        <source>Rename &lt;strong&gt;%1&lt;/strong&gt; object(s) to:</source>
        <translation>Renommer &lt;strong&gt;%1&lt;/strong&gt; objet(s) en :</translation>
    </message>
    <message>
        <location line="+40"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; You&apos;re about to rename multiple objects at once! This operation may cause irreversible changes to other objects not necessarily selected. Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;strong&gt;Vous allez renommer plusieurs objets à la fois ! Cette opération peut entrâiner des changements irréversibles sur des objets non sélectionnés. Souhaitez-vous continuer ?</translation>
    </message>
</context>
<context>
    <name>ObjectSelectorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="+35"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Clear field</source>
        <translation>Effacer le champ</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Select Object</source>
        <translation>Sélectionner un objet</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectselectorwidget.cpp" line="+196"/>
        <source>Select %1</source>
        <translation>Sélectionner %1</translation>
    </message>
</context>
<context>
    <name>ObjectsScene</name>
    <message>
        <location filename="../libobjrenderer/src/objectsscene.cpp" line="+35"/>
        <source>Default layer</source>
        <translation>Couche par défaut</translation>
    </message>
</context>
<context>
    <name>ObjectsTableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectstablewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Add Item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ins</source>
        <translation>Insérer</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Remove Item</source>
        <translation>Supprimer un élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location line="+136"/>
        <source>Update Item</source>
        <translation>Actualiser l&apos;élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Alt+R</source>
        <translation></translation>
    </message>
    <message>
        <location line="-130"/>
        <source>Remove All</source>
        <translation>Tout supprimer</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Shift+Del</source>
        <translation>Maj+Suppr</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Duplicate item</source>
        <translation>Dupliquer l&apos;élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+D</source>
        <translation></translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Edit Item</source>
        <translation>Modifier l&apos;élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Space</source>
        <translation>Espace</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Move Up</source>
        <translation>Déplacer vers le haut</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+Up</source>
        <translation>Ctrl+Haut</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Move Down</source>
        <translation>Déplacer vers le bas</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+Down</source>
        <translation>Ctrl+Bas</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Move to start</source>
        <translation>Déplacer au début</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+Home</source>
        <translation>Ctrl+début</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Move to end</source>
        <translation>Déplacer à la fin</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ctrl+End, Ctrl+S</source>
        <translation>Ctrl+Fin, Ctrl+S</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Resize columns to fit contents</source>
        <translation>Retailler la largeur des colonnes à leur contenu</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectstablewidget.cpp" line="+403"/>
        <source>Do you really want to remove the selected item?</source>
        <translation>Souhaitez-vous réellement supprimer l&apos;élément sélectionné ?</translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+53"/>
        <source>Confirmation</source>
        <translation></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Do you really want to remove all the items?</source>
        <translation>Souhaitez-vous réellement supprimer tous les éléments ?</translation>
    </message>
</context>
<context>
    <name>OperationList</name>
    <message>
        <location filename="../libpgmodeler/src/operationlist.cpp" line="+522"/>
        <source>(invalid object)</source>
        <translation>(objet invalide)</translation>
    </message>
</context>
<context>
    <name>OperationListWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="+35"/>
        <location line="+57"/>
        <source>Executed Operations</source>
        <translation>Opérations exécutées</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+121"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Operations:</source>
        <translation>Opérations :</translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+20"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location line="-10"/>
        <source>Position:</source>
        <translation>Position :</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Delete operation history</source>
        <translation>Supprimer l&apos;historique des opérations</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Undo</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Redo</source>
        <translation>Rétablir</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="+110"/>
        <source>created</source>
        <translation>créé</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>removed</source>
        <translation>supprimé</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>modified</source>
        <translation>modifié</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>moved</source>
        <translation>déplacé</translation>
    </message>
    <message>
        <location line="+80"/>
        <source>Operation history exclusion</source>
        <translation>Exclure l&apos;historique des opérations</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Delete the executed operations history is an irreversible action, do you want to continue?</source>
        <translation>Effacer l&apos;historique des opérations exécutées est une action irréversible, souhaitez-vous continuer ?</translation>
    </message>
</context>
<context>
    <name>OperatorClassWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="+54"/>
        <source>Default Class:</source>
        <translation>Classe par défaut :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Elements</source>
        <translation>Éléments</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Element Type:</source>
        <translation>Type d&apos;élément :</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Operator</source>
        <translation>Opérateur</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Function</source>
        <translation>Fonction</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Storage</source>
        <translation>Stockage</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Function:</source>
        <translation>Fonction :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Operator:</source>
        <translation>Opérateur :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Support/Strategy:</source>
        <translation>Support/Stratégie :</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+22"/>
        <source>Op. Family:</source>
        <translation>Famille d&apos;opérateurs :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="+37"/>
        <source>Storage Type</source>
        <translation>Type de Stockage</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Support/Strategy</source>
        <translation>Support/Stratégie</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operator Family</source>
        <translation>Famille d&apos;opérateurs</translation>
    </message>
</context>
<context>
    <name>OperatorFamilyWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorfamilywidget.ui" line="+50"/>
        <source>Indexing:</source>
        <translation>Indexation :</translation>
    </message>
</context>
<context>
    <name>OperatorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="+140"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>MERGES</source>
        <translation>MERGES</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>HASHES</source>
        <translation>HASHES</translation>
    </message>
    <message>
        <location line="-91"/>
        <source>Arguments</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Restrict:</source>
        <translation>Restriction :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Commutator:</source>
        <translation>Commutateur :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Negator:</source>
        <translation>Négateur :</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Operator Func.:</source>
        <translation>Fonction de l&apos;opérateur :</translation>
    </message>
    <message>
        <location line="-58"/>
        <source>Join:</source>
        <translation>Jointure :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="+34"/>
        <source>Right Argument Type</source>
        <translation>Type d&apos;argument de droite</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Left Argument Type</source>
        <translation>Type d&apos;argument de gauche</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>To create a unary operator it is necessary to specify as &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; one of its arguments. Additionally, the function that defines the operator must have only one parameter and this, in turn, must have the same data type of the the argument of unary operator.</source>
        <translation>Pour créer un opérateur unitaire, il est nécessaire de spécifier &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; comme l&apos;un de ses arguments. De plus, la fonction qui définit l&apos;opérateur doit avoir seulement un paramètre et celui-ci doit avoir le même type de donnée que l&apos;argument de l&apos;opérateur unitaire.</translation>
    </message>
</context>
<context>
    <name>ParameterWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="+41"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Mode:</source>
        <translation>Mode :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>IN</source>
        <translation>IN</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>OUT</source>
        <translation>OUT</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>VARIADIC</source>
        <translation>VARIADIC</translation>
    </message>
</context>
<context>
    <name>PermissionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="+152"/>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="+62"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location line="-39"/>
        <location line="+344"/>
        <source>Permissions</source>
        <translation>Permissions</translation>
    </message>
    <message>
        <location line="-112"/>
        <source>Add Permission</source>
        <translation>Ajouter permission</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Update Permission</source>
        <translation>Actualiser permission</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Cancel Operation</source>
        <translation>Annuler l&apos;opération</translation>
    </message>
    <message>
        <location line="-166"/>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="+2"/>
        <source>Privileges</source>
        <translation>Privilèges</translation>
    </message>
    <message>
        <location line="-54"/>
        <source>Disable SQL code</source>
        <translation>Désactiver le code SQL</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>Cascade</source>
        <translation>Cascade</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Privilege</source>
        <translation>Privilège</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>GRANT OPTION</source>
        <translation>GRANT OPTION</translation>
    </message>
    <message>
        <location line="+157"/>
        <source>Code Preview</source>
        <translation>Prévisualisation du code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="-16"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Id</source>
        <translation></translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Leave the &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; grid empty in order to create a %1 applicable to &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</source>
        <translation>Laissez la grille &lt;em&gt;&lt;strong&gt;Rôles&lt;/strong&gt;&lt;/em&gt; vide afin de créer un %1 applicable à &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location line="+435"/>
        <source>-- No permissions defined for the specified object!</source>
        <translation>-- Aucune permission définie pour cet objet !</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>/* Could not generate the SQL code preview for permissions!</source>
        <translation>/* Impossible de générer la prévisualisation du code SQL pour les permissions !</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="-378"/>
        <source>Edit permissions</source>
        <translation>Modifier les permissions</translation>
    </message>
    <message>
        <location line="+173"/>
        <source>&amp;Grant</source>
        <translation>Acco&amp;rder</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Re&amp;voke</source>
        <translation>Ré&amp;voquer</translation>
    </message>
</context>
<context>
    <name>PgModelerApp</name>
    <message>
        <location filename="../main/src/pgmodelerapp.cpp" line="+137"/>
        <source>Unknown exception caught!</source>
        <translation>Exception inconnue rencontrée !</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Failed to create initial configuration in `%1&apos;! Check if the current user has write permission over that path and at least read permission over `%2&apos;.</source>
        <translation>Impossible de créer les fichiers de configuration initiaux dans `%1&apos; ! Vérifiez que le dossier existe et que vous ayez la permission d&apos;écrire dans `%2&apos;.</translation>
    </message>
</context>
<context>
    <name>PgModelerCliApp</name>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="+87"/>
        <source>Database model files (.dbm) are already associated to pgModeler!</source>
        <translation type="unfinished">Les fichiers de modèle de bases de données (.dbm) sont déjà associés à pgModeler !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>There is no file association related to pgModeler and .dbm files!</source>
        <translation type="unfinished">Il n&apos;y a pas d&apos;association entre pgModeler et .les fichiers dbm !</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>Unrecognized option &apos;%1&apos;.</source>
        <translation type="unfinished">Option &apos;%1&apos; non reconnue.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Value not specified for option &apos;%1&apos;.</source>
        <translation type="unfinished">Valeur pour l&apos;option &apos;%1&apos; manquante.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Option &apos;%1&apos; does not accept values.</source>
        <translation type="unfinished">L&apos;option &apos;%1&apos; n&apos;accepte pas de valeur.</translation>
    </message>
    <message>
        <location line="+109"/>
        <source>Connection aliased as &apos;%1&apos; was not found in the configuration file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+157"/>
        <source> command line interface.</source>
        <translation type="unfinished"> interface en ligne de commande.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>PostgreSQL Database Modeler Project - pgmodeler.io</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Copyright 2006-2019 Raphael A. Silva &lt;raphael@pgmodeler.io&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Usage: pgmodeler-cli [OPTIONS]</source>
        <translation type="unfinished">Utilisation : pgmodeler-cli [OPTIONS]</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This CLI tool provides several operations over models and databases without the need to perform them
in pgModeler&apos;s graphical interface. All available options are described below.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>General options: </source>
        <translation type="unfinished">Options générales : </translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [FILE]		    Input model file (.dbm). This is mandatory for fix, export operations.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [DBNAME]	    Input database name. This is mandatory for import operation.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [FILE]		    Output file. This is mandatory for fixing model or exporting to file, png or svg.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Try to fix the structure of the input model file in order to make it loadable again.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [NUMBER]	    Model fix tries. When reaching the maximum count the invalid objects will be discarded.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Export the input model to a sql script file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Export the input model to a png image.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Export the input model to a svg file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Export the input model directly to a PostgreSQL server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Export the input model to a data directory in HTML format.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Import a database to an output file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2			    Compares a model and a database or two databases generating the SQL script to synch the latter in relation to the first.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Force the PostgreSQL version of generated SQL code.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2			    Silent execution. Only critical messages and errors are shown during process.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2			    Show this help menu.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Connection options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    List available connections in file %3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [ALIAS]	    Connection configuration alias to be used.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [HOST]		    PostgreSQL host in which a task will operate.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [PORT]		    PostgreSQL host listening port.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [USER]		    PostgreSQL username.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [PASSWORD]	    PostgreSQL user password.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [DBNAME]	    Connection&apos;s initial database.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>PNG and SVG export options: </source>
        <translation type="unfinished">Options d&apos;export en PNG et SVG : </translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Draws the grid in the exported image.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Draws the page delimiters in the exported image.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Each page will be exported in a separated png image. (Only for PNG images)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [FACTOR]		    Applies a zoom (in percent) before export to png image. Accepted zoom interval: %3-%4 (Only for PNG images)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>DBMS export options: </source>
        <translation type="unfinished">Options d&apos;export directe dans un serveur : </translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Ignores errors related to duplicated objects that eventually exist in the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [CODES] Ignores additional errors by their codes. A comma-separated list of alphanumeric codes should be provided.</source>
        <translation type="unfinished">  %1, %2=[CODES] Ignore les erreurs supplémentaires par leurs codes. Une liste de codes alphanumériques séparés par des virgules doit être fournie. {1,?} {2 ?}</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Drop the database before execute a export process.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Runs the DROP commands attached to SQL-enabled objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Simulates an export process by executing all steps but undoing any modification in the end.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Generates temporary names for database, roles and tablespaces when in simulation mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Data dictionary export options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		   The data dictionaries are generated in separated files inside the selected output directory.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		   Avoids the generation of the index that is used to help navigating through the data dictionary.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Database import options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Ignore all errors and try to create as many as possible objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Import system built-in objects. This option causes the model bloating due to the importing of unneeded objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Import extension objects. This option causes the model bloating due to the importing of unneeded objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Run import in debug mode printing all queries executed in the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Diff options: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [DBNAME]	    The database used in the comparison. All the SQL code generated is applied to it.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Save the generated diff code to output file.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Apply the generated diff code on the database server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Don&apos;t preview the generated diff code when applying it to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Drop cluster level objects like roles and tablespaces.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Revoke permissions already set on the database. New permissions configured in the input model are still applied.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Drop missing objects. Generates DROP commands for objects that are present in the input model but not in the compared database.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Force the drop of missing columns and constraints. Causes only columns and constraints to be dropped, other missing objects aren&apos;t removed.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Rename the destination database when the names of the involved databases are different.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    Don&apos;t drop or truncate objects in cascade mode.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Truncate tables prior to alter columns. Avoids errors related to type casting when the new type of a column isn&apos;t compatible to the old one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Don&apos;t reuse sequences on serial columns. Drop the old sequence assigned to a serial column and creates a new one.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Don&apos;t force the recreation of objects. Avoids the usage of a DROP and CREATE commands to create a new version of the objects.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2	    Don&apos;t recreate the unmodifiable objects. These objects are the ones which can&apos;t be changed via ALTER command.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Miscellaneous options: </source>
        <translation type="unfinished">Options diverses : </translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2 [ACTION]	    Handles the file association to .dbm files. The ACTION can be [%3 | %4].</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>  %1, %2		    The file association to .dbm files will be applied in a system wide level instead of to the current user.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>** The diff process allows the usage of the following options related to import and export operations: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>* Export: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <source>* Import: </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>** When running the diff using two databases (%1 and %2) there&apos;s the need to specify two connections/aliases. </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   If only one connection is set it will be used to import the input database as well to retrieve database used in the comparison.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   A second connection can be specified by appending a 1 on any connection configuration parameter listed above.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+42"/>
        <source>There are no connections configured.</source>
        <translation type="unfinished">Il n&apos;y a aucune connexion configurée.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Available connections (alias : connection string)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+33"/>
        <source>No operation mode was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Export, fix model, import database, diff and update mime operations can&apos;t be used at the same time!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Multiple export mode was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>No input file was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>No input database was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>No output file was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Input file must be different from output!</source>
        <translation type="unfinished">Le fichier source doit être différent du fichier de sortie !</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Incomplete connection information!</source>
        <translation type="unfinished">Les informations de connexion sont incomplètes !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Invalid zoom specified!</source>
        <translation type="unfinished">Zoom spécifié invalide !</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Invalid action specified to update mime option!</source>
        <translation type="unfinished">Action spécifiée invalide pour mettre à jour l&apos;option mime !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>No input file or database was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>The input file and database can&apos;t be used at the same time!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>No database to be compared was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>No diff action (save or apply) was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>No output file for the diff code was specified!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>command line interface.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+33"/>
        <source>** Error code `%1&apos; found and ignored. Proceeding with export.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>** Command: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+80"/>
        <source>Extracting objects&apos; XML...</source>
        <translation type="unfinished">Extraction des objets XML...</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Invalid input file! It seems that is not a pgModeler generated model or the file is corrupted!</source>
        <translation type="unfinished">Fichier d&apos;entrée invalide ! Il semble que ce ne soit pas un modèle généré par pgModeler ou que le fichier soit corrompu !</translation>
    </message>
    <message>
        <location line="+156"/>
        <source>Recreating objects...</source>
        <translation type="unfinished">Recréation des objets...</translation>
    </message>
    <message>
        <location line="+119"/>
        <source>
** Object(s) that couldn&apos;t fixed: </source>
        <translation type="unfinished">
** Ces objets ne peuvent être corrigés : </translation>
    </message>
    <message>
        <location line="+11"/>
        <source>WARNING: There are objects that maybe can&apos;t be fixed. Trying again... (tries %1/%2)</source>
        <translation type="unfinished">AVERTISSEMENT : Certains objets ne peuvent peut-être pas être corrigés. Nouvel essai... (%1/%2)</translation>
    </message>
    <message>
        <location line="+245"/>
        <source>Starting model fixing...</source>
        <translation type="unfinished">Lancement de la fixation du modèle...</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+14"/>
        <source>Loading input file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-13"/>
        <source>Fixed model file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Model successfully fixed!</source>
        <translation type="unfinished">Modèle fixé avec succès !</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Starting model export...</source>
        <translation type="unfinished">Début de l&apos;export du modèle...</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Export to PNG image: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Export to SVG file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Export to SQL script file: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Export to data dictionary: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Export to DBMS: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Export successfully ended!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Starting database import...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+67"/>
        <source>Input database: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="-60"/>
        <source>Saving the imported database to file...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Import successfully ended!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+51"/>
        <source>Starting diff process...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Input model: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Compare to: %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Loading input model...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <location line="+4"/>
        <source>Importing the database `%1&apos;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Comparing the generated models...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>No differences were detected.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Saving diff to file `%1&apos;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+42"/>
        <source>** WARNING: You are about to apply the generated diff code to the server. Data can be lost in the process!</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+4"/>
        <source>** Proceed with the diff applying? (yes/no) &gt; </source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>yes</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+0"/>
        <location line="+2"/>
        <source>no</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Diff code not applied to the server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Applying diff to the database `%1&apos;...</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Diff successfully ended!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Starting mime update...</source>
        <translation type="unfinished">Lancement de la mise à jour du mime...</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Mime database successfully updated!
</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Mime database operation: %1</source>
        <translation type="unfinished">Opération de base de données mime : %1</translation>
    </message>
    <message>
        <location line="+64"/>
        <source>Can&apos;t erase the file %1! Check if the current user has permissions to delete it and if the file exists.</source>
        <translation type="unfinished">Impossible d&apos;effacer le fichier %1 ! Vérifiez si l&apos;utilisateur actuel dispose des autorisations pour le supprimer et si le fichier existe.</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Running update-mime-database command...</source>
        <translation type="unfinished">Exécution de la commande update-mime-database...</translation>
    </message>
</context>
<context>
    <name>PgModelerPlugin</name>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="+33"/>
        <source>Plugin Information</source>
        <translation>Information d&apos;extension</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Version: %1</source>
        <translation>Version : %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Author: %1</source>
        <translation>Auteur : %1</translation>
    </message>
</context>
<context>
    <name>PgModelerUiNs</name>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodeleruins.cpp" line="+125"/>
        <source>Do you want to apply the &lt;strong&gt;SQL %1 status&lt;/strong&gt; to the object&apos;s references too? This will avoid problems when exporting or validating the model.</source>
        <translation>Souhaitez-vous appliquer le statut SQL &lt;strong&gt;%1&lt;/strong&gt; aux références de l&apos;objet également ? Cela permet d&apos;éviter des problèmes lors de l&apos;export ou de la validation du modèle.</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>disabling</source>
        <translation>désactivé</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>enabling</source>
        <translation>activé</translation>
    </message>
    <message>
        <location line="+175"/>
        <source>Another %1 error(s) were suppressed due to stacktrace size limits.</source>
        <translation>%1 autre erreur(s) ont été supprimés en raison de la limite de taille de la pile d&apos;appels.</translation>
    </message>
</context>
<context>
    <name>PgSQLTypeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/pgsqltypewidget.ui" line="+32"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Data Type</source>
        <translation>Type de donnée</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>SRID:</source>
        <translation>SRID :</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Variation:</source>
        <translation>Variation :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Z</source>
        <translation>Z</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>M</source>
        <translation>M</translation>
    </message>
    <message>
        <location line="+27"/>
        <location line="+228"/>
        <source>Precision</source>
        <translation>Précision</translation>
    </message>
    <message>
        <location line="-177"/>
        <source>Spatial:</source>
        <translation>Spatial :</translation>
    </message>
    <message>
        <location line="+25"/>
        <location line="+297"/>
        <source>Dimension</source>
        <translation>Dimension</translation>
    </message>
    <message>
        <location line="-260"/>
        <source>Format:</source>
        <translation>Format :</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Timezone:</source>
        <translation>Fuseau horaire :</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>P:</source>
        <translation>P :</translation>
    </message>
    <message>
        <location line="+25"/>
        <location line="+42"/>
        <source>Length</source>
        <translation>Longueur</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>L:</source>
        <translation>L :</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Interval:</source>
        <translation>Intervalle :</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>[ ]:</source>
        <translation>[ ] :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pgsqltypewidget.cpp" line="+47"/>
        <source>NONE</source>
        <translation>AUCUN</translation>
    </message>
</context>
<context>
    <name>PhysicalTable</name>
    <message>
        <location filename="../libpgmodeler/src/physicaltable.cpp" line="+371"/>
        <source>In demonstration version tables can have only `%1&apos; instances of each child object type or ancestor tables! You&apos;ve reach this limit for the type: `%2&apos;</source>
        <translation>Dans la version de démonstration, les tables ne peuvent contenir que `%1&apos; instance(s) de chaque type d&apos;objet enfant ou table ancêtre ! Vous avez atteint cette limite pour le type : `%2&apos;</translation>
    </message>
</context>
<context>
    <name>PluginsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/pluginsconfigwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+92"/>
        <source>Plug-ins root directory:</source>
        <translation>Répertoire racine des extensions :</translation>
    </message>
    <message>
        <location line="-26"/>
        <source>Open in file manager</source>
        <translation>Ouvrir dans le gestionnaire de fichier</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Loaded plug-ins</source>
        <translation>Extensions chargées</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="+32"/>
        <source>Plugin</source>
        <translation>Extension</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Version</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Library</source>
        <translation>Bibliothèque</translation>
    </message>
</context>
<context>
    <name>PolicyWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/policywidget.ui" line="+38"/>
        <source>Basics</source>
        <translation>Notions de base</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Command:</source>
        <translation>Commande :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Permissive</source>
        <translation>Permissif</translation>
    </message>
    <message>
        <location line="+49"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Expressions</source>
        <translation></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>USING:</source>
        <translatorcomment>Anglicisme a conserver (SQL)</translatorcomment>
        <translation>USING :</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>CHECK:</source>
        <translatorcomment>Anglicisme a conserver (SQL)</translatorcomment>
        <translation>CHECK :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/policywidget.cpp" line="+45"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Leave the &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; grid empty in order to create a %1 applicable to &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</source>
        <translation>Laissez la grille &lt;em&gt;&lt;strong&gt;Rôles&lt;/strong&gt;&lt;/em&gt; vide afin de créer un %1 applicable à &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="-10823"/>
        <source>new_database</source>
        <translation>nouvelle_base_de_données</translation>
    </message>
    <message>
        <location line="+3260"/>
        <location line="+63"/>
        <location line="+251"/>
        <location line="+7257"/>
        <source>%1 (line: %2)</source>
        <translation>%1 (ligne : %2)</translation>
    </message>
</context>
<context>
    <name>ReferenceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/referencewidget.ui" line="+20"/>
        <source>Reference properties</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Properties</source>
        <translation type="unfinished">Propriétés</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Ref. type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>&lt;p&gt;The reference will be used as part of the WHERE clause in form of conditional expression. &lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>WHERE</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <source>&lt;p&gt;The reference is used in the FROM portion of the command in order to reference tables or construct JOIN statements.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>FROM</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&lt;p&gt;The reference will be appended to the very end of the view&apos;s definition. This is useful when using GROUP BY/HAVING statements.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>GROUP/HAVING</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>&lt;p&gt;The reference&apos;s expression is used exclusively as the view&apos;s definition.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>View Definition</source>
        <translation type="unfinished">Définition de la vue</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&lt;p&gt;The reference will be used as part of the SELECT statement to retrieve columns or expressions that will compose the view&apos;s columns.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>SELECT</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+11"/>
        <source>View&apos;s references can point to a table, column or expression.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+4"/>
        <source>Table / Column</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Expression</source>
        <translation type="unfinished">Expression</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Ref. alias:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>&lt;p&gt;This is a more friendly description for the reference. When displaying the model in compact view this is the text shown for the reference instead of its real description. If this field is empty the real description will be displayed anyway.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Used in:</source>
        <translation type="unfinished">Utilisé dans :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ref. object:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Table alias:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Column alias:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Expression:</source>
        <translation type="unfinished">Expression :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Expr. alias:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Columns</source>
        <translation type="unfinished">Colonnes</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>This is the name of the object in the PostgreSQL database.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Name:</source>
        <translation type="unfinished">Nom :</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>&lt;p&gt;This is a more friendly name for the column. When displaying the model in compact view this is the name shown for the column instead of its real name. If this field is empty the real name will be displayed anyway.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Alias:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Referenced tables</source>
        <translation type="unfinished">Tables référencées</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Table:</source>
        <translation type="unfinished">Table :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/referencewidget.cpp" line="+35"/>
        <source>To reference all columns of a table select only a table in the object selector, this is the same as write &lt;em&gt;&lt;strong&gt;[schema].[table].*&lt;/strong&gt;&lt;/em&gt;. In order to reference a only a single column of a table select a column object in the selector.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+19"/>
        <source>Name</source>
        <translation type="unfinished">Nom</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Type</source>
        <translation type="unfinished">Type</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Alias</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This tab can be used to inform the columns that the view owns. This is just a convenience to make the visualization of this kind of object more intuitive. If no column is specified here the columns of the view displayed in the canvas will be a fragment of the expression defined in the previous tab.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Schema</source>
        <translation type="unfinished">Schéma</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>This tab can be used to inform the tables that the view references. This is just a convenience to make the visualization of this kind of object more intuitive. If no table is specified here no relationship will be displayed in the canvas. Note that no validation will be done to check if the provided tables are really referenced by the view.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>Relationship</name>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="+139"/>
        <source>%1_has_one_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1_has_many_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>many_%1_has_many_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1_inherits_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1_is_partition_of_%2</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1_copies_%2</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>RelationshipConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+118"/>
        <source>&lt;p&gt;This mode renders the relationships in crow&apos;s foot notation which has a better semantics and readability. It also determines the optimal point where the relationship is connected on the tables&apos; edges taking their position into account.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Mode en patte de corbeau. La liaison s&apos;effectue entre les tables avec cette notation pour une meilleure lisibilité.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>&lt;p&gt;This mode determines the optimal point where the relationship is connected on the tables&apos; edges taking their position into account. It implies the usage of the classical ER notation.&lt;/p&gt;</source>
        <translation>&lt;p&gt;Mode optimisé. La liaison s&apos;effectue entre les tables en un point optimal sur les bords des tables. Ce mode utilise la notation classique Entité-Association.&lt;/p&gt;</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Connect FK to PK columns</source>
        <translation>Relier via les colonnes des clés étrangères et clés primaires</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>&lt;p&gt;This mode is the classical one. It connects the relationship to tables through their central points. It implies the usage of the classical ER notation.&lt;/p&gt;</source>
        <translatorcomment>&lt;p&gt;Mode classique. La liaison s&apos;effectue entre les tables en leur point central. Ce mode utilise la notation classique Entité-Association.&lt;/p&gt;</translatorcomment>
        <translation></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Connect tables&apos; center points</source>
        <translation>Relier en un point central</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Foreign key settings</source>
        <translation>Clés étrangères</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Deferral:</source>
        <translation>Report :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE :</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Name patterns</source>
        <translation>Règles de nommage</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Relationship type:</source>
        <translation>Type de relation :</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Foreign Key (Target):</source>
        <translation>Clé étrangère (Cible) :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Unique Key Name:</source>
        <translation>Nom contrainte unique :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Primary Key Name:</source>
        <translation>Nom contrainte clé primaire :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Primary Key Column:</source>
        <translation>Nom colonne clé primaire :</translation>
    </message>
    <message>
        <location line="-53"/>
        <source>Column (Target):</source>
        <translation>Colonne (Cible) :</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Foreign Key (Source):</source>
        <translation>Clé étrangère (Source) :</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Column (Source):</source>
        <translation>Colonne (Source) :</translation>
    </message>
    <message>
        <location line="-589"/>
        <source>Connection Mode</source>
        <translation>Affichage</translation>
    </message>
    <message>
        <location line="+344"/>
        <source>FK Settings &amp;&amp; Patterns</source>
        <translation>Clés étrangères &amp;&amp; nommage</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipconfigwidget.cpp" line="+50"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipconfigwidget.ui" line="+212"/>
        <source>Pattern for foreign key generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Pattern for columns generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Pattern for columns generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Pattern for foreign key generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur une clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Pattern for unique key generated by the relationship.</source>
        <translation>Motif pour les noms de clés uniques qui sont générées par une association.</translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+10"/>
        <source>Pattern for primary key generated by identifier relationship.</source>
        <translation>Motif pour les noms de clés primaires qui sont générées par une relation.</translation>
    </message>
    <message>
        <location line="-527"/>
        <source>Crow&apos;s foot notation</source>
        <translation>Relier en patte de corbeau (Crow&apos;s foot)</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Connect tables&apos; edges</source>
        <translation>Relier par les bords des tables</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>This mode is available only for &lt;strong&gt;one-to-one&lt;/strong&gt;, &lt;strong&gt;one-to-many&lt;/strong&gt; and &lt;strong&gt;fk relationships&lt;/strong&gt; but provides a better semantics when linking tables by placing the lines on the exact point where the relationship occurs. It implies the usage of the classical ER notation.</source>
        <translation>Ce mode d&apos;affichage est disponible uniquement pour les associations de type &lt;strong&gt;un-à-un&lt;/strong&gt;, &lt;strong&gt;un-à-plusieurs&lt;/strong&gt; et &lt;strong&gt;les associations aux clés étrangères&lt;/strong&gt; mais fournit une meilleure sémantique lors de l&apos;association de tables en plaçant les lignes sur le point exact où l&apos;association se produit. Cela implique l&apos;utilisation de la notation ER classique.</translation>
    </message>
</context>
<context>
    <name>RelationshipWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="+66"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="+259"/>
        <source>Identifier</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location line="-142"/>
        <source>Rel. Type:</source>
        <translation>Type de relation :</translation>
    </message>
    <message>
        <location line="-78"/>
        <source>Table 1:</source>
        <translation>Table n°1 :</translation>
    </message>
    <message>
        <location line="+103"/>
        <source>Table 2:</source>
        <translation>Table n°2 :</translation>
    </message>
    <message>
        <location line="+744"/>
        <source>Foreign key Settings</source>
        <translation>Propriétés par défaut des clés étrangères</translation>
    </message>
    <message>
        <location line="-309"/>
        <location line="+339"/>
        <source>Use the values defined on settings dialogs for the fields below</source>
        <translation>Utiliser les paramètres de pgModeler pour les champs ci-dessous</translation>
    </message>
    <message>
        <location line="-336"/>
        <location line="+339"/>
        <source>Use global settings for these fields</source>
        <translation>Utiliser les paramètres globaux de pgModeler</translation>
    </message>
    <message>
        <location line="+143"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE :</translation>
    </message>
    <message>
        <location line="-516"/>
        <source>Name Patterns</source>
        <translation>Règles de nommage</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Column (Source):</source>
        <translation>Colonne (Source) :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Column (Target):</source>
        <translation>Colonne (Cible) :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Foreign Key (Source):</source>
        <translation>Clé étrangère (Source) :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Foreign Key (Target):</source>
        <translation>Clé étrangère (Cible) :</translation>
    </message>
    <message>
        <location line="-577"/>
        <source>Cardinality:</source>
        <translation>Cardinalité :</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>[SRC] is required</source>
        <translation>[SRC] est requis</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>[DST] is required</source>
        <translation>[DST] est requis</translation>
    </message>
    <message>
        <location line="-81"/>
        <location line="+235"/>
        <source>Name of the table generated from many to many relationship</source>
        <translation>Nom de la table générée par une relation plusieurs à plusieurs</translation>
    </message>
    <message>
        <location line="-118"/>
        <source>rel_type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+47"/>
        <source>&lt;p&gt;The receiver&apos;s primary key will be composed by the generated foreign key columns.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+43"/>
        <source>&lt;p&gt;Instead of create a multi-valued primary key with the generated foreign keys columns a single column is created and used as primary key.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+120"/>
        <source>Bounding expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Partitioning type:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+13"/>
        <source>NONE</source>
        <translation type="unfinished">AUCUN</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>&lt;p&gt;Default partitions are only supported on &lt;strong&gt;PostgreSQL 11+&lt;/strong&gt;. Using this option and exporting the code to PostgreSQL 10 syntax errors will be raised.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Default partition</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Generate the partition bounding expression based upon the partitioning type in use.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Generate expression</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Settings</source>
        <translation type="unfinished">Paramètres</translation>
    </message>
    <message>
        <location line="+447"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Deferral:</source>
        <translation>Report :</translation>
    </message>
    <message>
        <location line="+67"/>
        <source>Copy Options</source>
        <translation>Options de copie</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>INDEXES</source>
        <translation>INDEX</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>COMMENTS</source>
        <translation>COMMENTAIRES</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>INCLUDING</source>
        <translation>INCLURE</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>DEFAULTS</source>
        <translation>DEFAUTS</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>CONSTRAINTS</source>
        <translation>CONTRAINTES</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Use defaults</source>
        <translation>Utiliser les valeurs par défauts</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>ALL</source>
        <translation>TOUT</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>STORAGE</source>
        <translation>STOCKAGE</translation>
    </message>
    <message>
        <location line="-1109"/>
        <source>Gen. Table Name:</source>
        <translation>Nom nouvelle table :</translation>
    </message>
    <message>
        <location line="+1141"/>
        <source>STATISTICS</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>IDENTITY</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Constraints</source>
        <translation>Contraintes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Primary key</source>
        <translation>Clé primaire</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="+66"/>
        <source>Attribute</source>
        <translation>Attribut</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+6"/>
        <location line="+6"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="-8"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>This advanced tab shows the objects (columns or table) auto created by the relationship&apos;s connection as well the foreign keys that represents the link between the participant tables.</source>
        <translation>Cet onglet avancé montre les objets (colonnes ou tables) générés automatiquement par la relation ainsi que les clés étrangères qui forment le(s) lien(s) entre les tables.</translation>
    </message>
    <message>
        <location line="+146"/>
        <source>&lt;p&gt;Referer view references one or more columns of a table to construct it&apos;s own columns.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;p&gt;Referenced table has its columns referenced by a view in order to construct the columns of this latter.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&lt;p&gt;Referer table references one or more columns of a table through foreign keys. This is the (n) side of relationship.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;p&gt;Referenced table has its columns referenced by a table&apos;s foreign key. This is the (1) side of relationship.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Partitioned Table:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;p&gt;Partitioned table is the one which is splitted into smaller pieces (partitions). This table is where the partitioning strategy or type is defined.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Partition Table:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>&lt;p&gt;Partition table is the one attached to a partitioned table in which operations over data will be routed (according to the paritionig rule) when trying to handle the partitioned table.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>&lt;p&gt;Reference table has the columns from its primary key will copied to the receiver table in order to represent the linking between them. This is the (1) side of relationship.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&lt;p&gt;Receiver (or referer) table will receive the generated columns and the foreign key in order to represent the linking between them. This is the (n) side of relationship.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+8"/>
        <source>&lt;p&gt;In many-to-many relationships both tables are used as reference to generate the table that represents the linking. Columns from both tables are copied to the resultant table and two foreign keys are created as well in order to reference each participant table.&lt;/p&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+1"/>
        <source> is required</source>
        <translation> est requis</translation>
    </message>
    <message>
        <location line="-31"/>
        <location line="+11"/>
        <location line="+2"/>
        <source>Reference Table:</source>
        <translation>Table référente :</translation>
    </message>
    <message>
        <location line="-162"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
    <message>
        <location line="+152"/>
        <source>Receiver Table:</source>
        <translation>Table de destination :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="-713"/>
        <source>Pattern for columns generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Pattern for columns generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de colonnes générées qui sont basées sur la clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Pattern for foreign key generated based upon reference table&apos;s pk (1-1 and 1-n) or based upon source table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur la clé primaire d&apos;une table référencée (1 :1 et 1 :n) ou sur la clé primaire de la table source (n :n).</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Pattern for foreign key generated based upon target table&apos;s pk (n-n).</source>
        <translation>Motif pour les noms de clés étrangères générées qui sont basées sur une clé primaire de la table cible (n :n).</translation>
    </message>
    <message>
        <location line="+10"/>
        <location line="+20"/>
        <source>Pattern for primary key generated by identifier relationship.</source>
        <translation>Motif pour les noms de clés primaires qui sont générées par une association.</translation>
    </message>
    <message>
        <location line="-17"/>
        <source>Primary Key Name:</source>
        <translation>Nom de clé primaire :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Pattern for unique key generated by the relationship.</source>
        <translation>Motif pour les noms de clés uniques qui sont générées par une association.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Unique Key Name:</source>
        <translation>Nom clé unique :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Primay Key Column:</source>
        <translation>Colonne de clé primaire :</translation>
    </message>
    <message>
        <location line="-369"/>
        <source>Single PK column</source>
        <translation>Colonne clé primaire simple</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Custom Color:</source>
        <translation>Couleur personnalisée :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="-189"/>
        <source>Use the special primary key if you want to include a primary key containing generated columns to the receiver table. &lt;strong&gt;Important:&lt;/strong&gt; if this is a new relationship there is a need to finish its creation and reopen this dialog to create the special primary key.</source>
        <translation>Utilisez la clé primaire spéciale si vous souhaitez inclure une clé primaire contenant des colonnes générées dans la table de destination. &lt;strong&gt;Important :&lt;/strong&gt; s&apos;il s&apos;agit d&apos;une nouvelle relation, il est nécessaire de terminer sa création et de réouvrir cette boîte de dialogue pour créer la clé primaire spéciale.</translation>
    </message>
    <message>
        <location line="+155"/>
        <source>Referer View:</source>
        <translation>Vue référente :</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Referer Table:</source>
        <translation>Table référente :</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Referenced Table:</source>
        <translation>Table référencée :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="+841"/>
        <source>E&amp;XCLUDING</source>
        <translation>E&amp;XCLUSION</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="-142"/>
        <source>Available tokens to define name patterns:&lt;br/&gt;					&lt;strong&gt;%1&lt;/strong&gt; = Reference (source) primary key column name. &lt;em&gt;(Ignored on constraint patterns)&lt;/em&gt;&lt;br/&gt; 					&lt;strong&gt;%2&lt;/strong&gt; = Reference (source) table name.&lt;br/&gt; 					&lt;strong&gt;%3&lt;/strong&gt; = Receiver (destination) table name.&lt;br/&gt; 					&lt;strong&gt;%4&lt;/strong&gt; = Generated table name. &lt;em&gt;(Only for n:n relationships)&lt;/em&gt;</source>
        <translation>Formes disponibles pour définir les modèles de noms : &lt;br/&gt;					&lt;strong&gt;%1&lt;/strong&gt; = Nom de la colonne de clé primaire de référence (source). &lt;em&gt;(Ignoré sur les modèles de contrainte)&lt;/em&gt;&lt;br/&gt; 					&lt;strong&gt;%2&lt;/strong&gt; = Nom de la table de référence (source).&lt;br/&gt; 					&lt;strong&gt;%3&lt;/strong&gt; = Nom de la table référencée (ou de déstination). &lt;br/&gt; 				&lt;strong&gt;%4&lt;/strong&gt; = Nom de la table générée. &lt;em&gt;(Seulement pour les relations n:n)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>ResultSetModel</name>
    <message>
        <location filename="../libpgmodeler_ui/src/resultsetmodel.cpp" line="+51"/>
        <location line="+102"/>
        <source>[binary data]</source>
        <translation>[données binaires]</translation>
    </message>
</context>
<context>
    <name>RoleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="+204"/>
        <source>Password:</source>
        <translation>Mot de passe :</translation>
    </message>
    <message>
        <location line="-102"/>
        <source>Connections:</source>
        <translation>Connexions :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Superuser</source>
        <translation>Super utilisateur</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Inherit permissions</source>
        <translation>Hériter des permissions</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Can create database</source>
        <translation>Peut créer des bases de données</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Can use replication</source>
        <translation>Peut utiliser la réplication</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Can login</source>
        <translation>Peut se loguer</translation>
    </message>
    <message>
        <location line="+65"/>
        <source>Encrypted</source>
        <translation>Encrypté</translation>
    </message>
    <message>
        <location line="-44"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="+68"/>
        <source>Members</source>
        <translation>Membres</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="-3"/>
        <source>Member of</source>
        <translation>Membre de</translation>
    </message>
    <message>
        <location line="+5"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="+6"/>
        <source>Members (Admin.)</source>
        <translation>Membres (Admin.)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="-12"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="-108"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="+3"/>
        <source>Validity</source>
        <translation>Validité</translation>
    </message>
    <message>
        <location line="-38"/>
        <source>yyyy-MMM-dd hh:mm:ss</source>
        <translation>aaaa-MMM-jj hh :mm :ss</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="-27"/>
        <source>Assigning &lt;strong&gt;&lt;em&gt;-1&lt;/em&gt;&lt;/strong&gt; to &lt;strong&gt;&lt;em&gt;Connections&lt;/em&gt;&lt;/strong&gt; creates a role without connection limit.&lt;br/&gt;										  Unchecking &lt;strong&gt;&lt;em&gt;Validity&lt;/em&gt;&lt;/strong&gt; creates an role that never expires.</source>
        <translation>L&apos;affectation de &lt;strong&gt;&lt;em&gt;-1&lt;/em&gt;&lt;/strong&gt; à &lt;strong&gt;&lt;em&gt;Connexions&lt;/em&gt;&lt;/strong&gt; crée un rôle sans limite de connexion.&lt;br/&gt;										Décocher &lt;strong&gt;&lt;em&gt;Validité&lt;/em&gt;&lt;/strong&gt; crée un rôle qui n&apos;expire jamais.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="+101"/>
        <source>Bypass RLS</source>
        <translation>Contournement RLS</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Can create role</source>
        <translation>Peut créer des rôles</translation>
    </message>
</context>
<context>
    <name>RuleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="+50"/>
        <source>Event:</source>
        <translation>Évènement :</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Execution Type:</source>
        <translation>Type d&apos;exécution :</translation>
    </message>
    <message>
        <location line="+49"/>
        <source>Conditional Expr.:</source>
        <translation>Expr. conditionnelle :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Commands</source>
        <translation>Commandes</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>SQL Command:</source>
        <translation>Commandes SQL :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rulewidget.cpp" line="+38"/>
        <source>SQL command</source>
        <translation>Commande SQL</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>To create a rule that does not perform any action (&lt;strong&gt;DO NOTHING&lt;/strong&gt;) simply do not specify commands in the SQL commands table.</source>
        <translation>Pour créer une règle qui n&apos;éxécute aucune action (&lt;strong&gt;DO NOTHING&lt;/strong&gt;), ne spécifiez aucune commandes dans le champs des commandes SQL.</translation>
    </message>
</context>
<context>
    <name>SQLExecutionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="+648"/>
        <source>Save SQL commands</source>
        <translation>Sauvegarder commandes SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="+229"/>
        <source>Alt+F</source>
        <translation>Alt+F</translation>
    </message>
    <message>
        <location line="-98"/>
        <source>Run the specified SQL command</source>
        <translation>Exécuter la commande SQL entrée</translation>
    </message>
    <message>
        <location line="+176"/>
        <source>Clear sql input field and results</source>
        <translation>Effacer le code SQL et la grille de résultat</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Clear All</source>
        <translation>Effacer</translation>
    </message>
    <message>
        <location line="+32"/>
        <location line="+47"/>
        <source>Export results to a CSV file</source>
        <translation>Exporter les résultats dans un fichier CSV</translation>
    </message>
    <message>
        <location line="-44"/>
        <source>Snippe&amp;ts</source>
        <translation>Extrai&amp;ts de code</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+T</source>
        <translation></translation>
    </message>
    <message>
        <location line="+34"/>
        <source>E&amp;xport</source>
        <translation>E&amp;xporter</translation>
    </message>
    <message>
        <location line="-338"/>
        <location line="+735"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="-577"/>
        <source>SQL file (*.sql);;All files (*.*)</source>
        <translation>Fichier SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Load</source>
        <translation>Charger</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Save</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <location line="+239"/>
        <source>[binary data]</source>
        <translation>[données binaires]</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>No results retrieved or changes done due to the error above! Run the command again.</source>
        <translation>Aucun resultat retourné ou aucun changement effectué en raison de l&apos;erreur ci-dessus. Relancer la commande.</translation>
    </message>
    <message>
        <location line="+11"/>
        <location line="+78"/>
        <source>Messages (%1)</source>
        <translation></translation>
    </message>
    <message>
        <location line="-7"/>
        <source>[%1]: SQL command successfully executed in &lt;em&gt;&lt;strong&gt;%2&lt;/strong&gt;&lt;/em&gt;. &lt;em&gt;%3 &lt;strong&gt;%4&lt;/strong&gt;&lt;/em&gt;</source>
        <translation>[%1]: Commande SQL exécutée avec succès en &lt;em&gt;&lt;strong&gt;%2&lt;/strong&gt;&lt;/em&gt;. &lt;em&gt;%3 &lt;strong&gt;%4&lt;/strong&gt;&lt;/em&gt;</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Executed at</source>
        <translation>Exécuté à</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Command failed</source>
        <translation>La commande a échoué</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Rows:</source>
        <translation>Lignes :</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>[%1]: SQL command is running...</source>
        <translation>[%1]: Commande SQL en cours d&apos;éxécution...</translation>
    </message>
    <message>
        <location line="+73"/>
        <source>Save CSV file</source>
        <translation>Sauvegarder fichier CSV</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Comma-separated values file (*.csv);;All files (*.*)</source>
        <translation>Fichiers CSV avec champs séparés par des virgules (*.csv);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>The SQL input field and the results grid will be cleared! Want to proceed?</source>
        <translation>Le champ d&apos;entrée SQL ainsi que la grille des résultats vont être purgés ! Voulez-vous poursuivre ?</translation>
    </message>
    <message>
        <location line="+322"/>
        <source>Save history</source>
        <translation>Enregistrer l&apos;historique</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reload history</source>
        <translation>Recharger l&apos;historique</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Find in history</source>
        <translation>Rechercher dans l&apos;historique</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Hide find tool</source>
        <translation>Masquer l&apos;outil de recherche</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>This action will wipe out all the SQL commands history for the current connection! Do you really want to proceed?</source>
        <translation>Cette action effacera tout l&apos;historique des commandes SQL pour la connexion en cours ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location line="-414"/>
        <source>Load SQL commands</source>
        <translation>Charger commandes SQL</translation>
    </message>
    <message>
        <location line="+196"/>
        <source>Copy selection</source>
        <translation>Copier la sélection</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="-529"/>
        <source>Search in SQL code</source>
        <translation>Rechercher dans le code SQL</translation>
    </message>
    <message>
        <location line="+217"/>
        <source>Toggles the output pane</source>
        <translation>(Dés)active le volet de sortie</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;Output</source>
        <translation>S&amp;ortie</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location line="+174"/>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="-480"/>
        <location line="+58"/>
        <location line="+182"/>
        <source>Results</source>
        <translation>Résultats</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Case sensitive</source>
        <translation>Respecter la casse</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Regular expression</source>
        <translation>Expression régulière</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Exact match</source>
        <translation>Correspondance exacte</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Hide this widget</source>
        <translation>Masquer ce widget</translation>
    </message>
    <message>
        <location line="+67"/>
        <source>Messages</source>
        <translation></translation>
    </message>
    <message>
        <location line="+47"/>
        <source>History</source>
        <translation>Historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqlexecutionwidget.cpp" line="-187"/>
        <source>Results (%1)</source>
        <translation>Resultats (%1)</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Rows affected</source>
        <translation>Lignes affectées</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Rows retrieved</source>
        <translation>Lignes récupérées</translation>
    </message>
    <message>
        <location line="+406"/>
        <source>Plain format</source>
        <translation>Format simple</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>CVS format</source>
        <translation>Format CVS</translation>
    </message>
    <message>
        <location line="+149"/>
        <source>This action will wipe out all the SQL commands history for all connections! Do you really want to proceed?</source>
        <translation>Cette action effacera tout l&apos;historique des commandes SQL pour toutes les connexions ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Clear history</source>
        <translation>Nettoyer l&apos;historique</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqlexecutionwidget.ui" line="-849"/>
        <source>Close the current SQL script</source>
        <translation>Fermer le script SQL actuel</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>SQL script currently handled</source>
        <translation>Script SQL actuellement géré</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>(not saved)</source>
        <translation>(non enregistré)</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Run</source>
        <translation>Lancer</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>F5</source>
        <translation>F5</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Cancel the execution of the current SQL command</source>
        <translation>Annuler l&apos;exécution de la commande SQL courante</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Stop</source>
        <translation>Arrêter</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Esc</source>
        <translation>Échap</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Handle external SQL script</source>
        <translation>Gérer le script SQL externe</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>&amp;Script</source>
        <translation></translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Fi&amp;nd</source>
        <translation>&amp;Rechercher</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <location line="+129"/>
        <source>Alt+X</source>
        <translation></translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Filter the retrived results</source>
        <translation>Filtrer parmis les résultats obtenus</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>F&amp;ilter</source>
        <translation>F&amp;iltrer</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+I</source>
        <translation>Alt+I</translation>
    </message>
    <message>
        <location line="+121"/>
        <source>Current working database</source>
        <translation>Base de données de travail actuelle</translation>
    </message>
</context>
<context>
    <name>SQLToolWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+73"/>
        <source>Database explorer</source>
        <translation>Explorateur de bases de données</translation>
    </message>
    <message>
        <location line="+300"/>
        <source>SQL execution</source>
        <translation>Exécuteur de code SQL</translation>
    </message>
    <message>
        <location line="-237"/>
        <location line="+61"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqltoolwidget.cpp" line="+36"/>
        <source>Add a new execution tab for the current database (%1)</source>
        <translation>Ouvrir un nouvel onglet pour la base courante (%1)</translation>
    </message>
    <message>
        <location line="+172"/>
        <location line="+167"/>
        <source>Warning</source>
        <translation>Avertissement</translation>
    </message>
    <message>
        <location line="-37"/>
        <source>Failed to load the file `%1&apos; in SQL tool because the connection ID `%2&apos; was not found!</source>
        <translation>Echec du chargement du fichier `%1&apos; dans l&apos;outil SQL car la connexion `%2 n&apos;existe pas !</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; Close the database being browsed will close any opened SQL execution pane related to it! Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION:&lt;/strong&gt; La fermeture de la connexion à la base de données en cours de consultation provoquera la fermeture de tous les onglets ouverts associés ! Souhaitez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sqltoolwidget.ui" line="-64"/>
        <source>Disconnect from all databases</source>
        <translation>Se déconnecter de toutes les bases de données</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Update the database list</source>
        <translation>Actualiser la liste des bases disponibles</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>Toggle the object&apos;s attributes grid</source>
        <translation>(Dés)active la grille des attributs de l&apos;objet</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Toggle the display of source code pane</source>
        <translation>(Dés)active l&apos;affichage du volet du code source</translation>
    </message>
    <message>
        <location line="+6"/>
        <location line="+105"/>
        <source>Source code</source>
        <translation>Code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sqltoolwidget.cpp" line="-167"/>
        <source>&lt;strong&gt;ATTENTION:&lt;/strong&gt; Disconnect from all databases will close any opened tab in this view! Do you really want to proceed?</source>
        <translation>&lt;strong&gt;ATTENTION :&lt;/strong&gt; Se déconnecter de toutes les bases de données fermera tout onglet ouvert dans cette vue ! Souhaitez-vous vraiment poursuivre ?</translation>
    </message>
</context>
<context>
    <name>SceneInfoWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sceneinfowidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+44"/>
        <location line="+40"/>
        <source>Current position of the mouse in the canvas</source>
        <translation>Position actuelle de la souris dans le canevas</translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+75"/>
        <location line="+75"/>
        <location line="+75"/>
        <source>-</source>
        <translation></translation>
    </message>
    <message>
        <location line="-193"/>
        <location line="+40"/>
        <source>Current zoom factor</source>
        <translation>Facteur de zoom actuel</translation>
    </message>
    <message>
        <location line="+35"/>
        <location line="+40"/>
        <source>Currently selected object(s)</source>
        <translation>Objet(s) actuellement sélectionné(s)</translation>
    </message>
    <message>
        <location line="+35"/>
        <location line="+40"/>
        <source>Dimensions of the selected object(s)</source>
        <translation>Dimensions de(s) l&apos;objet(s) sélectionné(s)</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sceneinfowidget.cpp" line="+42"/>
        <source>No selection</source>
        <translation>Aucune séléction</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>N/A</source>
        <translation>Inconnu</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Sel. objects: %1</source>
        <translation>Objets select. : %1</translation>
    </message>
</context>
<context>
    <name>SchemaWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/schemawidget.ui" line="+48"/>
        <source>Show rectangle</source>
        <translation>Afficher le rectangle</translation>
    </message>
    <message>
        <location line="-19"/>
        <source>Fill color:</source>
        <translation>Couleur de remplissage :</translation>
    </message>
</context>
<context>
    <name>SequenceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="+54"/>
        <source>Cyclic:</source>
        <translation>Cyclique :</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Start:</source>
        <translation>Début :</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>Maximum:</source>
        <translation>Maximum :</translation>
    </message>
    <message>
        <location line="-31"/>
        <source>Minimum:</source>
        <translation>Minimum :</translation>
    </message>
    <message>
        <location line="-86"/>
        <source>Defualt values:</source>
        <translation>Valeurs par défaut :</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Increment:</source>
        <translation>Incrément :</translation>
    </message>
    <message>
        <location line="+93"/>
        <source>Cache:</source>
        <translation>Cache :</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Owner Col.:</source>
        <translation>Proprio. Col. :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sequencewidget.cpp" line="+35"/>
        <source>User defined</source>
        <translation>Utilisateur défini</translation>
    </message>
</context>
<context>
    <name>SnippetsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+172"/>
        <source>Create new connection</source>
        <translation>Créer une nouvelle connexion</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Cancel edition</source>
        <translation>Annuler l&apos;édition</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Edit selected connection</source>
        <translation>Modifier la connexion sélectionnée</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Delete selected connection</source>
        <translation>Supprimer la connexion sélectionnée</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Remove All</source>
        <translation>Tout supprimer</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Shift+Del</source>
        <translation>Maj+Suppr</translation>
    </message>
    <message>
        <location line="+269"/>
        <source>Filter:</source>
        <translation>Filtrer :</translation>
    </message>
    <message>
        <location line="-453"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location line="+283"/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Update</source>
        <translation>Mettre à jour</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="+522"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/snippetsconfigwidget.ui" line="-366"/>
        <source>Label:</source>
        <translation>Étiquette :</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Applies to:</source>
        <translation>S&apos;applique à :</translation>
    </message>
    <message>
        <location line="+394"/>
        <source>Parsable or dynamic snippets are written in the &lt;strong&gt;schema micro language&lt;/strong&gt; syntax. When using a parsable snippet the attributes surrounded in &lt;strong&gt;{}&lt;/strong&gt; will be replaced by the selected object&apos;s matching attributes.</source>
        <translation>Les extraits de code analysables ou dynamiques sont écrits dans la syntaxe &lt;strong&gt;schema micro language&lt;/strong&gt;. Lorsque vous utilisez un extrait de code analysé, les attributs entourés par des &lt;strong&gt;{}&lt;/strong&gt; seront remplacés par les attributs correspondants de l&apos;objet sélectionné.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Parsable</source>
        <translation>Analysable</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>When handling parsable snippets empty attributes will be replaced by a value in the format &lt;strong&gt;{attribute}&lt;/strong&gt;. Note that this option can affect the semantics of the resulting snippet.</source>
        <translation>Lors de la manipulation d&apos;extraits de code analysables, les attributs vides seront remplacés par une valeur au format &lt;strong&gt;{attribut}&lt;/strong&gt;. Noter que cette option peut affecter la sémantique de l&apos;extrait résultant.</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Placeholders</source>
        <translation>Élément de substitution</translation>
    </message>
    <message>
        <location line="-183"/>
        <source>Parse the snippet in order to check if there are syntax errors.</source>
        <translation>Analyser l&apos;extrait de code afin de vérifier s&apos;il existe des erreurs de syntaxe.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Parse</source>
        <translation>Analyser</translation>
    </message>
    <message>
        <location line="-46"/>
        <source>Snippets:</source>
        <translation>Extraits :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/snippetsconfigwidget.cpp" line="-491"/>
        <source>General purpose</source>
        <translation>Usage général</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>All snippets</source>
        <translation>Tous les extraits</translation>
    </message>
    <message>
        <location line="+145"/>
        <source>/* Error parsing the snippet &apos;%1&apos;:

 %2 */</source>
        <translation>/* Erreur lors de l&apos;analyse de l&apos;extrait &apos;%1&apos;:

 %2 */</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Duplicated snippet id &lt;strong&gt;%1&lt;/strong&gt; detected. Please, specify a different one!</source>
        <translation>Identifiant &lt;strong&gt;%1&lt;/strong&gt; d&apos;extrait dupliqué détécté. Merci d&apos;en choisir un autre !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Invalid ID pattern detected &lt;strong&gt;%1&lt;/strong&gt;. This one must start with at leat one letter and be composed by letters, numbers and/or underscore!</source>
        <translation>Motif d&apos;identification &lt;strong&gt;%1&lt;/strong&gt; incorrect détecté. Celui-ci doit commencer par au moins une lettre et être composé de lettres, de chiffres et/ou de tirets bas _ !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Empty label for snippet &lt;strong&gt;%1&lt;/strong&gt;. Please, specify a value for it!</source>
        <translation>Label vide pour l&apos;extrait &lt;strong&gt;%1&lt;/strong&gt;. Merci de lui spécifier une valeur !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Empty code for snippet &lt;strong&gt;%1&lt;/strong&gt;. Please, specify a value for it!</source>
        <translation>Code vide pour l&apos;extrait &lt;strong&gt;%1&lt;/strong&gt;. Merci de lui spécifier une valeur !</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>The dynamic snippet contains syntax error(s). Additional info: &lt;br/&gt;&lt;em&gt;%1&lt;/em&gt;</source>
        <translation>L&apos;extrait dynamique contient des erreurs de syntax. Plus d&apos;informations : &lt;br/&gt;&lt;em&gt;%1&lt;/em&gt;</translation>
    </message>
    <message>
        <location line="+118"/>
        <source>Do you really want to remove all snippets?</source>
        <translation>Voulez-vous vraiment supprimer tous les extraits ?</translation>
    </message>
    <message>
        <location line="+70"/>
        <source>No syntax errors found in the snippet.</source>
        <translation>Aucune erreur de syntax retrouvée dans l&apos;extrait.</translation>
    </message>
</context>
<context>
    <name>SourceCodeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="+49"/>
        <source>Version:</source>
        <translation>Version :</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>PostgreSQL</source>
        <translation>PostgreSQL</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>iconecodigo</source>
        <translation>icône</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>SQL</source>
        <translation>SQL</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>&lt;strong&gt;Original:&lt;/strong&gt; displays only the original object&apos;s SQL code.&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Dependencies:&lt;/strong&gt; displays the original code including all dependencies needed to properly create the selected object.&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Children:&lt;/strong&gt; displays the original code including all object&apos;s children SQL code. This option is used only by schemas, tables and views.</source>
        <translation>&lt;strong&gt;Original :&lt;/strong&gt;restitue uniquement the code SQL de l&apos;objet.&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Dépendances :&lt;/strong&gt;restitue le code original de l&apos;objet en y incluant toutes les dépendances nécessaires à sa bonne création.&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Enfants :&lt;/strong&gt;restitue le code original de l&apos;objet en incluant tous les objets enfants. Cette option n&apos;est utilisée que par les schémas, les tables et les vues.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Original</source>
        <translation></translation>
    </message>
    <message>
        <location line="+67"/>
        <source>XML</source>
        <translation>XML</translation>
    </message>
    <message>
        <location line="-258"/>
        <source>Source code visualization</source>
        <translation>Visualisation du code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="+93"/>
        <source>SQL code (*.sql);;All files (*.*)</source>
        <translation>Code SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Generating source code...</source>
        <translation>Génération du code source...</translation>
    </message>
    <message>
        <location line="+19"/>
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
        <location line="+28"/>
        <source>-- SQL code unavailable for this type of object --</source>
        <translation>-- Code SQL non disponible pour ce type d&apos;objet. --</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="+171"/>
        <source>Code display:</source>
        <translation>Affichage du code :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Original + depedencies&apos; SQL</source>
        <translation>Original + dépendances SQL</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Original + children&apos;s SQL</source>
        <translation>Original + enfants SQL</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Save the SQL code to a file.</source>
        <translation>Enregistrer le code SQL dans un fichier.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Save SQL</source>
        <translation>Enregistrer le code SQL</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="-96"/>
        <source>Save SQL code as...</source>
        <translation>Enregistrer le code SQL sous...</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>&lt;!-- XML code preview disabled in demonstration version --&gt;</source>
        <translation>&lt; !-- L&apos;aperçu du code XML est désactivé dans la version de démonstration --&gt;</translation>
    </message>
    <message>
        <location line="-13"/>
        <source>

-- SQL code purposely truncated at this point in demo version!</source>
        <translation>

-- Le code SQL est volontairement tronqué à ce stade dans la version démo !</translation>
    </message>
</context>
<context>
    <name>SwapObjectsIdsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/swapobjectsidswidget.ui" line="+38"/>
        <source>Create:</source>
        <translation>Créer :</translation>
    </message>
    <message>
        <location line="+25"/>
        <location line="+81"/>
        <source>ID:</source>
        <translation>Identifiant :</translation>
    </message>
    <message>
        <location line="-25"/>
        <source>Before:</source>
        <translation>Avant :</translation>
    </message>
    <message>
        <location line="+194"/>
        <source>Swap the values of the fields</source>
        <translation>Procède à l&apos;échange</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Swap values</source>
        <translation>Échanger</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Filter by name</source>
        <translation>Filtrer par nom</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Hide system objects</source>
        <translation>Masquer les objets système</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Hide relationships</source>
        <translation>Masquer les relations</translation>
    </message>
    <message>
        <location line="+161"/>
        <source>Defines the search filter</source>
        <translation>Définir un filtre de recherche</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
    </message>
    <message>
        <location line="-542"/>
        <source>Change objects creation order</source>
        <translation>Changer l&apos;ordre de création des objets</translation>
    </message>
    <message>
        <location line="+250"/>
        <source>Change the objects creation order is an irreversible operation and cause the operations history to be automatically erased. Note that the creation order configured in this form is not definitive and may change after a model validation.</source>
        <translation>Modifier l&apos;ordre de création des objets est une opération irréversible qui entraîne l&apos;effacement automatique de l&apos;historique des opérations. Noter que l&apos;ordre de création configuré dans ce formulaire n&apos;est pas définitif et peut changer après la validation d&apos;un modèle.</translation>
    </message>
    <message>
        <location line="+205"/>
        <source>ID</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
</context>
<context>
    <name>Table</name>
    <message>
        <location filename="../libpgmodeler/src/table.cpp" line="+30"/>
        <source>new_table</source>
        <translation>nouvelle_table</translation>
    </message>
</context>
<context>
    <name>TableDataWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="+14"/>
        <source>Edit table data</source>
        <translation>Modifier les données de la table</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>Copy items on the grid</source>
        <translation>Copier les valeurs de la grille</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Copy</source>
        <translation>Copier</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Add empty rows</source>
        <translation>Ajouter des lignes vides</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Add row</source>
        <translation>Ajouter une ligne</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ins</source>
        <translation>Insérer</translation>
    </message>
    <message>
        <location line="+272"/>
        <source>Ctrl+D</source>
        <translation></translation>
    </message>
    <message>
        <location line="-51"/>
        <source>Add an empty column</source>
        <translation>Ajouter une colonne vide</translation>
    </message>
    <message>
        <location line="+200"/>
        <source>Delete rows</source>
        <translation>Supprimer des lignes</translation>
    </message>
    <message>
        <location line="+121"/>
        <source>Remove all rows from the grid preserving columns</source>
        <translation>Supprimer toutes les lignes en préservant les colonnes</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Shift+Del</source>
        <translation>Maj+Suppr</translation>
    </message>
    <message>
        <location line="-140"/>
        <source>Delete the selected rows</source>
        <translation>Supprimer les lignes sélectionnées</translation>
    </message>
    <message>
        <location line="-383"/>
        <location line="+399"/>
        <source>Del</source>
        <translation>Suppr</translation>
    </message>
    <message>
        <location line="-178"/>
        <source>Duplicate the selected rows</source>
        <translation>Dupliquer les lignes sélectionnées</translation>
    </message>
    <message>
        <location line="-237"/>
        <source>Delete the selected columns</source>
        <translation>Supprimer les colonnes sélectionnées</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Delete column</source>
        <translation>Supprimer une colonne</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Paste items on the grid</source>
        <translation>Coller les valeurs sur la grille</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Paste</source>
        <translation>Coller</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+V</source>
        <translation>Ctrl+V</translation>
    </message>
    <message>
        <location line="+124"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Some invalid or duplicated columns were detected. In order to solve this issue double-click the header of the highlighted ones in order to define the correct name in which the data belongs to or delete the entire column. Note that these columns are completely ignored when generating the &lt;span style=&quot; font-weight:600;&quot;&gt;INSERT&lt;/span&gt; commands.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Des colonnes invalides ou en double ont été détectées. Pour solutionner la chose vous pouvez double-cliquer sur l&apos;entête de chaque colonne mise en surbrillance pour lui associer un nom afin d&apos;y rattacher les données ou sinon vous pouvez simplement supprimer ces colonnes. Les colonnes invalides sont ignorées lors de la génération des commandes SQL de type &lt;span style=&quot; font-weight:600;&quot;&gt;INSERT&lt;/span&gt; &lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Add column</source>
        <translation>Ajouter une colonne</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Duplicate rows</source>
        <translation>Dupliquer des lignes</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Change the values of all selected cells at once</source>
        <translation>Modifier la valeur de toutes les cellules sélectionnées</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Bulk data edit</source>
        <translation>Modification de données en masse</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+E</source>
        <translation>Ctrl+E</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>Remove all columns (and rows) from the grid</source>
        <translation>Supprimer toutes les lignes et les colonnes également</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Delete all columns</source>
        <translation>Supprimer toutes les colonnes</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+Shift+Del</source>
        <translation>Ctrl+Maj+Suppr</translation>
    </message>
    <message>
        <location line="+146"/>
        <source>Delete all rows</source>
        <translation>Supprimer toutes les lignes</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tabledatawidget.cpp" line="+175"/>
        <source>Delete columns is an irreversible action! Do you really want to proceed?</source>
        <translation>Effacer des colonnes est une action irréversible ! Souhaitez-vous continuer ?</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Remove all rows is an irreversible action! Do you really want to proceed?</source>
        <translation>Supprimer toutes les lignes est une action irréversible ! Souhaitez-vous continuer ?</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Remove all columns is an irreversible action! Do you really want to proceed?</source>
        <translation>Supprimer toutes les colonnes est une action irréversible ! Voulez-vous vraiment poursuivre ?</translation>
    </message>
    <message>
        <location line="+37"/>
        <location line="+115"/>
        <source>Unknown column</source>
        <translation>Colonne inconnue</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Duplicated column</source>
        <translation>Colonne dupliquée</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tabledatawidget.ui" line="-448"/>
        <source>Fills the grid using a CSV file</source>
        <translation>Remplit la grille en utilisant un fichier CSV</translation>
    </message>
    <message>
        <location line="+423"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Empty values are assumed as &lt;span style=&quot; font-weight:600;&quot;&gt;DEFAULT&lt;/span&gt;. To use special values like &lt;span style=&quot; font-weight:600;&quot;&gt;NULL&lt;/span&gt;, a function call like &lt;span style=&quot; font-weight:600;&quot;&gt;now()&lt;/span&gt; or a specific data escaping, enclose values in two slashes, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;/value/&lt;/span&gt;. To use a slash as part of the value prepend the backslash character, e.g., &lt;span style=&quot; font-weight:600;&quot;&gt;\/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Les valeurs vides sont supposées être &lt;span style=&quot;font-weight :600;&quot;&gt;DEFAULT&lt;/span&gt;. Pour utiliser des valeurs spéciales comme &lt;span style=&quot;font-weight :600;&quot;&gt;NULL&lt;/span&gt;, un appel de fonction comme &lt;span style=&quot;font-weight :600;&quot;&gt;now()&lt;/span&gt; ou un échappement des données spécifiques, entourer les valeurs de barres obliques, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;/valeur/&lt;/span&gt;. Pour utiliser une barre oblique dans le cadre de la valeur, ajouter le caractère barre oblique inverse, par exemple &lt;span style=&quot;font-weight :600;&quot;&gt;\/&lt;/span&gt;.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
</context>
<context>
    <name>TableObjectView</name>
    <message>
        <location filename="../libobjrenderer/src/tableobjectview.cpp" line="+200"/>
        <source>
Relationship: %1</source>
        <translation>
Relations : %1</translation>
    </message>
</context>
<context>
    <name>TableView</name>
    <message>
        <location filename="../libobjrenderer/src/tableview.cpp" line="+246"/>
        <source>Partitioned</source>
        <translation>Partitionnée</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Partition</source>
        <translation>Partition</translation>
    </message>
</context>
<context>
    <name>TableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="+292"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location line="-143"/>
        <source>Tag:</source>
        <translation>Balise :</translation>
    </message>
    <message>
        <location line="-111"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>With OID</source>
        <translation>Avec OID</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Generate ALTER for columns/constraints</source>
        <translation>Utiliser ALTER pour les colonnes/contraintes</translation>
    </message>
    <message>
        <location line="-42"/>
        <source>Unlogged</source>
        <translation>Unlogged</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="+65"/>
        <location line="+60"/>
        <location line="+25"/>
        <location line="+10"/>
        <location line="+11"/>
        <location line="+8"/>
        <location line="+7"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="-119"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+58"/>
        <location line="+25"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="-68"/>
        <source>Option</source>
        <translation>Option</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>PK</source>
        <translatorcomment>Faut-il le traduire ? Non, jargon DB accepté</translatorcomment>
        <translation>PK</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Default Value</source>
        <translation>Valeur par défaut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attribute(s)</source>
        <translation>Attribut(s)</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+25"/>
        <location line="+11"/>
        <location line="+8"/>
        <location line="+7"/>
        <location line="+13"/>
        <source>Alias</source>
        <translation>Alias</translation>
    </message>
    <message>
        <location line="-63"/>
        <location line="+25"/>
        <location line="+11"/>
        <location line="+8"/>
        <location line="+7"/>
        <location line="+13"/>
        <source>Comment</source>
        <translation>Commentaire</translation>
    </message>
    <message>
        <location line="-53"/>
        <source>It is not possible to mark a column as primary key when the table already has a primary key which was created by a relationship! This action should be done in the section &lt;strong&gt;Primary key&lt;/strong&gt; of the relationship&apos;s editing form.</source>
        <translation>Il n&apos;est pas possible de marquer une colonne comme clé primaire quand la table a déjà une clé primaire qui a été créée par une relation ! Cette action doit être effectuée dans la section &lt;strong&gt;Clé primaire&lt;/strong&gt; du formulaire d&apos;édition de la relation.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>It is not possible to mark a column created by a relationship as primary key! This action should be done in the section &lt;strong&gt;Primary key&lt;/strong&gt; of the relationship&apos;s editing form.</source>
        <translation>Il n&apos;est pas possible de marquer une colonne créée par une relation comme clé primaire ! Cette action doit être effectuée dans la section &lt;strong&gt;Clé primaire&lt;/strong&gt; du formulaire d&apos;édition de la relation.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>ON DELETE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>À la suppression</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>ON UPDATE</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>À la mise à jour</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Refer. Table</source>
        <translation>Table référencée</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Firing</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Events</source>
        <translation>Evènements</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Execution</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Event</source>
        <translation>Evènement</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Indexing</source>
        <translation>Indexation</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Command</source>
        <translation>Commande</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Permissive</source>
        <translatorcomment>Jargon commande SQL</translatorcomment>
        <translation>PERMISSIVE</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>USING expression</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>expression USING</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>CHECK expression</source>
        <translatorcomment>Faut-il le traduire ?</translatorcomment>
        <translation>expression CHECK</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>None</source>
        <translation>Aucun</translation>
    </message>
    <message>
        <location line="+143"/>
        <source>Parent</source>
        <translation></translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Copy</source>
        <translation>Copie</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Partitioned</source>
        <translation>Partitionnée</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Partition</source>
        <translation>Partition</translation>
    </message>
    <message>
        <location line="+312"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="+131"/>
        <source>&amp;Columns</source>
        <translation>&amp;Colonnes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Co&amp;nstraints</source>
        <translation>Co&amp;ntraintes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Tri&amp;ggers</source>
        <translation>Décl&amp;encheurs</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&amp;Rules</source>
        <translation>&amp;Régles</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>&amp;Indexes</source>
        <translation>&amp;Index</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Partition &amp;keys</source>
        <translation>Clés de &amp;partition</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Partitioning:</source>
        <translation>Partitionnement :</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>&amp;Tables</source>
        <translation>&amp;Tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="-642"/>
        <source>Edit data</source>
        <translation>Modifier données</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Define initial data for the table</source>
        <translation>Définir les données initiales de la table</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="-191"/>
        <source>Enable row level security</source>
        <translation>Activer la sécurité au niveau de la ligne</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Force RLS for owner</source>
        <translation>Forcer RLS pour le propriétaire</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Server:</source>
        <translation>Serveur :</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>&amp;Policies</source>
        <translation>&amp;Politiques</translation>
    </message>
</context>
<context>
    <name>TablespaceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablespacewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Directory:</source>
        <translation>Dossier :</translation>
    </message>
</context>
<context>
    <name>TagWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tagwidget.ui" line="+29"/>
        <source>Colors</source>
        <translation>Couleurs</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Extended body:</source>
        <translation>Corps étendu :</translation>
    </message>
    <message>
        <location line="-28"/>
        <source>Body:</source>
        <translation>Corps :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Title:</source>
        <translation>Titre :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Schema name:</source>
        <translation>Nom du schéma :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Table name:</source>
        <translation>Nom de la table :</translation>
    </message>
</context>
<context>
    <name>TaskProgressWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/taskprogresswidget.ui" line="+35"/>
        <source>Executing tasks</source>
        <translation>Exécution des tâches</translation>
    </message>
    <message>
        <location line="+102"/>
        <location filename="../libpgmodeler_ui/src/taskprogresswidget.cpp" line="+52"/>
        <source>Waiting task to start...</source>
        <translation>En attente du démarrage de la tâche ...</translation>
    </message>
</context>
<context>
    <name>TextboxWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="+35"/>
        <source>Font:</source>
        <translation>Police :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Text</source>
        <translation>Texte</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>pt</source>
        <translation>pt</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Color:</source>
        <translation>Couleur :</translation>
    </message>
    <message>
        <location line="+98"/>
        <source>Bold</source>
        <translation>Gras</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Italic</source>
        <translation>Italique</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/textboxwidget.cpp" line="+55"/>
        <source>Select text color</source>
        <translation>Sélection de la couleur du texte</translation>
    </message>
</context>
<context>
    <name>TriggerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="+421"/>
        <source>INSERT</source>
        <translation>INSERT</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>DELETE</source>
        <translation>DELETE</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>UPDATE</source>
        <translation>UPDATE</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>TRUNCATE</source>
        <translation>TRUNCATE</translation>
    </message>
    <message>
        <location line="-242"/>
        <source>Deferrable:</source>
        <translation>Reportable :</translation>
    </message>
    <message>
        <location line="-201"/>
        <source>Events:</source>
        <translation>Evènements :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Type:</source>
        <translation>Type :</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Column:</source>
        <translation>Colonne :</translation>
    </message>
    <message>
        <location line="+492"/>
        <source>Transition tables names</source>
        <translation>Noms des tables de transition</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>OLD:</source>
        <translatorcomment>Pas à traduire</translatorcomment>
        <translation>OLD:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>NEW:</source>
        <translatorcomment>Pas à traduire</translatorcomment>
        <translation>NEW:</translation>
    </message>
    <message>
        <location line="-608"/>
        <source>Arguments</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location line="+108"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location line="+327"/>
        <source>Ordinary trigger</source>
        <translation>Déclencheur ordinaire</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Constraint trigger</source>
        <translation>Céclencheur contrainte</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>FOR EACH ROW</source>
        <translation>Pour chaque ligne</translation>
    </message>
    <message>
        <location line="-207"/>
        <source>Refer. Table:</source>
        <translation>Table référ. :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Condition:</source>
        <translation>Condition :</translation>
    </message>
    <message>
        <location line="-273"/>
        <source>Argument:</source>
        <translation>Argument :</translation>
    </message>
    <message>
        <location line="+536"/>
        <source>Function:</source>
        <translation>Fonction :</translation>
    </message>
    <message>
        <location line="-410"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location line="+382"/>
        <source>Excution:</source>
        <translation>Exécution :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/triggerwidget.cpp" line="+46"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
</context>
<context>
    <name>TypeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="+331"/>
        <source>Configuration:</source>
        <translation>Configuration :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Base Type</source>
        <translation>Type de base</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Enumeration</source>
        <translation>Énumération</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Enumerations</source>
        <translation>Énumérations</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Enumeration:</source>
        <translation>Énumération :</translation>
    </message>
    <message>
        <location line="-293"/>
        <location line="+322"/>
        <location line="+57"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="-358"/>
        <source>Internal Length:</source>
        <translation>Long. interne :</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Storage:</source>
        <translation>Stockage :</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>Category:</source>
        <translation>Catégorie :</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Delimiter:</source>
        <translation>Délimiteur :</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Alignment:</source>
        <translation>Alignement :</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>char</source>
        <translation>char</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>smallint</source>
        <translation>smallint</translation>
    </message>
    <message>
        <location line="-10"/>
        <source>integer</source>
        <translation>integer</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>double precision</source>
        <translation>double précision</translation>
    </message>
    <message>
        <location line="-49"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut :</translation>
    </message>
    <message>
        <location line="-207"/>
        <source>Range</source>
        <translation>Intervalle</translation>
    </message>
    <message>
        <location line="+136"/>
        <source>Options:</source>
        <translation>Options :</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>By value</source>
        <translation>Par valeur</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Preferred</source>
        <translation>Préféré</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Collatable</source>
        <translation>Assemblable</translation>
    </message>
    <message>
        <location line="-160"/>
        <source>Functions</source>
        <translation>Fonctions</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>INPUT:</source>
        <translation>INPUT :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>OUTPUT:</source>
        <translation>OUTPUT :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>RECV:</source>
        <translation>RECV :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>SEND:</source>
        <translation>SEND :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>TPMOD_IN:</source>
        <translation>TPMOD_IN :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>TPMOD_OUT:</source>
        <translation>TPMOD_OUT :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>ANALYZE:</source>
        <translation>ANALYZE :</translation>
    </message>
    <message>
        <location line="+351"/>
        <source>Name:</source>
        <translation>Nom :</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>Collation:</source>
        <translation>Collation :</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Subtype Diff Func.:</source>
        <translation>Fonction de différence du sous-type :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Operator Class:</source>
        <translation>Classe d&apos;opérateurs :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Canonical Func.:</source>
        <translation>Fonction canonique :</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="+32"/>
        <source>Like Type</source>
        <translation>Type de like</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Element Type</source>
        <translation>Type d&apos;élement</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Subtype</source>
        <translation>Sous-type</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Type</source>
        <translation></translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Collation</source>
        <translation></translation>
    </message>
    <message>
        <location line="+15"/>
        <source>The functions to be assigned to a type should be written in C language and possess, respectively, the following signatures:&lt;br/&gt;  &lt;table&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;INPUT:&lt;/strong&gt; &lt;em&gt;any function(cstring, oid, integer)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;OUTPUT:&lt;/strong&gt; &lt;em&gt;cstring function(any)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;SEND:&lt;/strong&gt; &lt;em&gt;byta function(any)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;RECV:&lt;/strong&gt; &lt;em&gt;any function(internal, oid, integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_IN:&lt;/strong&gt; &lt;em&gt;integer function(cstring[])&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_OUT:&lt;/strong&gt; &lt;em&gt;cstring function(integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;ANALYZE:&lt;/strong&gt; &lt;em&gt;boolean function(internal)&lt;/em&gt;&lt;/td&gt;    &lt;tr&gt;  &lt;/table&gt;</source>
        <translation>Les fonctions assignées à un type devraient être écrites en langage C et possèder, les signatures suivantes :&lt;br/&gt;  &lt;table&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;INPUT:&lt;/strong&gt; &lt;em&gt;any function(cstring, oid, integer)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;OUTPUT:&lt;/strong&gt; &lt;em&gt;cstring function(any)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;SEND:&lt;/strong&gt; &lt;em&gt;byta function(any)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;RECV:&lt;/strong&gt; &lt;em&gt;any function(internal, oid, integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_IN:&lt;/strong&gt; &lt;em&gt;integer function(cstring[])&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_OUT:&lt;/strong&gt; &lt;em&gt;cstring function(integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;ANALYZE:&lt;/strong&gt; &lt;em&gt;boolean function(internal)&lt;/em&gt;&lt;/td&gt;    &lt;tr&gt;  &lt;/table&gt;</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>The functions to be assigned to a range type should have the following signatures:&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonical:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</source>
        <translation>Les fonction assignables au type plage (RANGE) doivent avoir la signature suivante :&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonical:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="-161"/>
        <source>Co&amp;mposite</source>
        <translation>Co&amp;mposite</translation>
    </message>
</context>
<context>
    <name>UpdateNotifierWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="+26"/>
        <source>Update Notifier</source>
        <translation>Vérificateur de mise à jour</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Update available!</source>
        <translation>Mise à jour disponible !</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Released in:</source>
        <translation>Date de sortie :</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>mmm dd, yyyy</source>
        <translation>jj mmm aaaa</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>0.0.0</source>
        <translation>0.0.0</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Changelog</source>
        <translation>Journal des modifications</translation>
    </message>
    <message>
        <location line="+76"/>
        <source>Redirects to purchase page.</source>
        <translation>Redirige vers la page web pour acheter.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Get binary package</source>
        <translation>Télécharger l&apos;application compilée</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Redirects to GitHub source repository.</source>
        <translation>Redirige vers la page web du dépôt GitHub.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Get source code</source>
        <translation>Télécharger le code source</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/updatenotifierwidget.cpp" line="+115"/>
        <location line="+47"/>
        <source>Failed to check updates</source>
        <translation>Impossible de vérifier la présence de mise à jour</translation>
    </message>
    <message>
        <location line="-46"/>
        <source>The update notifier failed to check for new versions! Please, verify your internet connectivity and try again! Connection error returned: &lt;em&gt;%1&lt;/em&gt; - &lt;strong&gt;%2&lt;/strong&gt;.</source>
        <translation>Le notificateur de mise à jour n&apos;a pas pu vérifier les nouvelles versions ! Merci de vérifier votre connexion internet et  de réessayer ! Erreur de connexion renvoyée : &lt;em&gt;%1&lt;/em&gt; - &lt;strong&gt;%2&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>No updates found</source>
        <translation>Aucune mise à jour disponible</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>You are running the most recent pgModeler version! No update needed.</source>
        <translation>Vous utilisez déjà la version de pgModeler la plus récente ! Aucune mise à jour requise.</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>The update notifier failed to check for new versions! A HTTP status code was returned: &lt;strong&gt;%1&lt;/strong&gt;</source>
        <translation>Le vérificateur de mise à jour n&apos;a pas pu vérifier la présence de nouvelles versions ! Code retour HTTP reçu : &lt;strong&gt;%1&lt;/strong&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/updatenotifierwidget.ui" line="-142"/>
        <source>New version:</source>
        <translation>Nouvelle version :</translation>
    </message>
</context>
<context>
    <name>UserMappingWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/usermappingwidget.ui" line="+47"/>
        <source>Server:</source>
        <translation>Serveur :</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/usermappingwidget.cpp" line="+41"/>
        <source>Option</source>
        <translation>Option</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Value</source>
        <translation>Valeur</translation>
    </message>
</context>
<context>
    <name>ViewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="+33"/>
        <source>References</source>
        <translation>Références</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Options</source>
        <translation>Options</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Tag:</source>
        <translation>Balise :</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Mode:</source>
        <translation>Mode :</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Ordinary</source>
        <translation>Ordinaire</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>With no data</source>
        <translation>Sans données</translation>
    </message>
    <message>
        <location line="-110"/>
        <source>Triggers</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rules</source>
        <translation>Règles</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Indexes</source>
        <translation></translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table Expression</source>
        <translation>CTE</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Code Preview</source>
        <translation>Prévisualisation du code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="+60"/>
        <source>Col./Expr.</source>
        <translation></translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Table alias</source>
        <translation>Alias de table</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Column alias</source>
        <translation>Alias de colonne</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference alias</source>
        <translation>Alias de référence</translation>
    </message>
    <message>
        <location line="+36"/>
        <location line="+7"/>
        <location line="+8"/>
        <source>Alias</source>
        <translation></translation>
    </message>
    <message>
        <location line="-52"/>
        <source>Flags: SF FW AW EX VD</source>
        <translation>Marques : SF FW AW EX VD</translation>
    </message>
    <message>
        <location line="+30"/>
        <location line="+11"/>
        <location line="+7"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="-16"/>
        <source>Refer. Table</source>
        <translation>Table de référence</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Firing</source>
        <translation>Déclenche</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Events</source>
        <translation>Evènements</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+7"/>
        <location line="+8"/>
        <source>Comment</source>
        <translation>Commentaire</translation>
    </message>
    <message>
        <location line="-10"/>
        <source>Indexing</source>
        <translation>Indexation</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Execution</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Event</source>
        <translation>Evènement</translation>
    </message>
    <message>
        <location line="+511"/>
        <source>/* Could not generate the SQL code. Make sure all attributes are correctly filled! </source>
        <translation>/* Le code SQL ne peut être généré. Vérifiez que les champs sont correctement remplis ! </translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="+64"/>
        <source>Recursi&amp;ve</source>
        <translation>Récursi&amp;f</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>&amp;Materialized</source>
        <translation>&amp;Matérialiser</translation>
    </message>
</context>
<context>
    <name>WelcomeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/welcomewidget.ui" line="+23"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+128"/>
        <source>Support</source>
        <translation>Support</translation>
    </message>
    <message>
        <location line="+76"/>
        <source>New model</source>
        <translation>Nouveau modèle</translation>
    </message>
    <message>
        <location line="-38"/>
        <source>Open model</source>
        <translation>Ouvrir modèle</translation>
    </message>
    <message>
        <location line="+121"/>
        <source>Sample models</source>
        <translation>Modèles de démo</translation>
    </message>
    <message>
        <location line="-204"/>
        <source>Recent models</source>
        <translation>Modèles récents</translation>
    </message>
    <message>
        <location line="+163"/>
        <source>Last session</source>
        <translation>Dernière session</translation>
    </message>
</context>
</TS>
