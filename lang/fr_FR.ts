<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="fr" sourcelanguage="en_US">
<defaultcodec>UTF-8</defaultcodec>
<context>
    <name>AboutForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aboutform.ui" line="+35"/>
        <source>About pgModeler</source>
        <translation>À propos de pgModeler</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location line="+83"/>
        <source>PostgreSQL Database Modeler</source>
        <translation>PostgreSQL Database Modeler</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Design, configure, deploy</source>
        <translation>Concevoir, configurer, déployer</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>0.0.0.0</source>
        <translation>0.0.0.0</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;a href=&quot;http://pgmodeler.com.br&quot;&gt;&lt;span style=&quot; font-size:11pt; font-style:italic; text-decoration: underline; color:#0057ae;&quot;&gt;http://pgmodeler.com.br&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;&lt;a href=&quot;http://pgmodeler.com.br&quot;&gt;&lt;span style=&quot; font-size:11pt; font-style:italic; text-decoration: underline; color:#0057ae;&quot;&gt;http://pgmodeler.com.br&lt;/span&gt;&lt;/a&gt;&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location line="+112"/>
        <source>Copyright 2006-2013 - Raphael Araújo e Silva &lt;rkhaotix@gmail.com&gt;

The pgModeler project is a CASE tool for modeling relational databases for PostgreSQL DBMS through the use of modeling techniques from entity-relationship diagrams as well merging concepts that PostgreSQL implements.</source>
        <translation>Le projet pgModeler est un outil de CASE (computer-aided software engineering ou génie logiciel assisté par ordinateur) dédié à la modélisation de bases de données relationnelles pour PostgreSQL utilisant les techniques de modélisation des diagrammes de relantion entre les entités, ainsi que les concepts implémentés dans PostgreSQL.</translation>
    </message>
    <message>
        <location line="-38"/>
        <source>This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation version 3.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

The complete text of GPLv3 is at LICENSE file on source code root directory. Also, you can get the complete GNU General Public License at &lt;http://www.gnu.org/licenses/&gt;</source>
        <translation>Ce programme est un logiciel libre : vous pouvez le redistribuer et/ou le modifier sous les termes de la licence &quot;GNU General Public License v3&quot; publié par la Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

The complete text of GPLv3 is at LICENSE file on source code root directory. Also, you can get the complete GNU General Public License at &lt;http://www.gnu.org/licenses/&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aboutform.cpp" line="+24"/>
        <source>Version %1</source>
        <translation>Version %1</translation>
    </message>
</context>
<context>
    <name>AggregateWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/aggregatewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Final Function:</source>
        <translation>Fonction finale:</translation>
    </message>
    <message>
        <location line="+75"/>
        <source>Transition Function:</source>
        <translation>Fonction de transition:</translation>
    </message>
    <message>
        <location line="-68"/>
        <source>Sort Operator:</source>
        <translation>Opérateur de tri:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Initial Condition:</source>
        <translation>Condition initiale:</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Funtion Inputs</source>
        <translation>Entrées de la fonction</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Function State</source>
        <translation>État de la fonction</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/aggregatewidget.cpp" line="+40"/>
        <source>Input Data Type</source>
        <translation>Type de donnée en entrée</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>State Data Type</source>
        <translation>Type de donnée d&apos;état</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>An aggregate function that accepts the types &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; and &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; as input types and which type of state is &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt;&lt;/em&gt;, must obey the following rules: &lt;br/&gt;&lt;br/&gt; &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Final Function:&lt;/strong&gt; &lt;em&gt;void final_function(&lt;strong&gt;state_type&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Transition Function:&lt;/strong&gt; &lt;em&gt;&lt;strong&gt;state_type&lt;/strong&gt; transition_function(&lt;strong&gt;state_type&lt;/strong&gt;, &lt;strong&gt;typeA&lt;/strong&gt;, &lt;strong&gt;typeB&lt;/strong&gt;)&lt;/em&gt;</source>
        <translation>Une fonction d&apos;agrégation qui accepte les types &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; et &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; comme types d&apos;entrées et dont le type d&apos;état est &lt;em&gt;&lt;strong&gt;type_etat&lt;/strong&gt;&lt;/em&gt;, doit obéir aux règles suivantes:&lt;br/&gt;&lt;br/&gt; &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Fonction Finale:&lt;/strong&gt; &lt;em&gt;void fonction_finale(&lt;strong&gt;type_etat&lt;/strong&gt;)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt; &amp;nbsp;&amp;nbsp;&amp;nbsp;• Fonction de Transition,:&lt;/strong&gt; &lt;em&gt;&lt;strong&gt;type_etat&lt;/strong&gt; fonction_transition(&lt;strong&gt;type_etat&lt;/strong&gt;, &lt;strong&gt;typeA&lt;/strong&gt;, &lt;strong&gt;typeB&lt;/strong&gt;)&lt;/em&gt;</translation>
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
        <location line="+18"/>
        <source>Element:</source>
        <translation>Élement:</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Global: Font style</source>
        <translation>Global: style de police</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Constraints descriptor</source>
        <translation>Global: Marqueurs de contraintes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Object selection</source>
        <translation>Global: Selection d&apos;objet</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Position hint text</source>
        <translation>Global: Texte de la boite de coordonées</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Position hint box</source>
        <translation>Global: Fond de la boite de coordonées</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Objects type</source>
        <translation>Global: Type de colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Lock arc</source>
        <translation>Global: Cadenas (objet vérouillé, couleur de l&apos;arceau)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Global: Lock body</source>
        <translation>Global: Cadenas (objet vérouillé, couleur du corps)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Schema name</source>
        <translation>Table: Nom du schema</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Table name</source>
        <translation>Table: Nom de la table</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Columns box</source>
        <translation>Table: Zone des colonnes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Extended attributes box</source>
        <translation>Table: Zone d&apos;attributs étendus</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Table: Title box</source>
        <translation>Table: Zone de titre</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rule: Name</source>
        <translation>Règle: Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rule: Descriptor</source>
        <translation>Règle: Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Index: Name</source>
        <translation>Index: Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Index: Descriptor</source>
        <translation>Index: Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Trigger: Name</source>
        <translation>Déclencheur: Nom</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Trigger: Descriptor</source>
        <translation>Déclencheur: Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Schema name</source>
        <translation>Vue: Nom du schema</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: View name</source>
        <translation>Vue: Nom de la vue</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: References box</source>
        <translation>Vue: Couleur d&apos;arriere plan</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Extended attributes box</source>
        <translation>Vue: Dialogue des attributs étendus</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Title box</source>
        <translation>Vue:  Couleur d&apos;arriere plan de la barre de titre</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Table / columns alias</source>
        <translation>Vue: Alias de table / colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Referenced column</source>
        <translation>Vue: Colonne référencée</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Referenced table</source>
        <translation>Vue: Table référencée</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>View: Reference descriptor</source>
        <translation>Vue: Couleur de l&apos;icône des références</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Textbox: Body</source>
        <translation>Zone de texte: Corps</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Column name</source>
        <translation>Colonne: Nom de la colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Descriptor</source>
        <translation>Colonne: Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Included / Inherited by relationship</source>
        <translation>Colonne: Incluses / Héritées par relations</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column: Protected</source>
        <translation>Colonne: Protégée</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (pk): Column name</source>
        <translation>Colonne (pk): Nom de la colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (pk): Descriptor</source>
        <translation>Colonne (pk): Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (fk): Column name</source>
        <translation>Colonne (fk): Nom de la colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (fk): Descriptor</source>
        <translation>Colonne (fk): Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (uq): Column name</source>
        <translation>Colonne (uq): Nom de la colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (uq): Descriptor</source>
        <translation>Colonne (uq): Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (nn): Column name</source>
        <translation>Colonne (nn): Nom de la colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Column (nn): Descriptor</source>
        <translation>Colnne (nn): Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Descriptor</source>
        <translation>Relations: Couleur de l&apos;icône</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Label text</source>
        <translation>Relations: Texte de l&apos;étiquette</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Label box</source>
        <translation>Relations: Arrière plan de l&apos;étiquette</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Attribute text</source>
        <translation>Relations: Texte des attributs</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Relationship: Attribute descriptor</source>
        <translation>Relations: Couleur de l&apos;icône des attributs</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Font:</source>
        <translation>Police:</translation>
    </message>
    <message>
        <location line="+41"/>
        <source> pt</source>
        <translation> pt</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Bold</source>
        <translation>Gras</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Italic</source>
        <translation>Italique</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Colors:</source>
        <translation>Couleur:</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Font color / Fill color 1</source>
        <translation>Couleur de police / Couleur de remplissage 1</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Fill color 2</source>
        <translation>Couleur de remplissage 2</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Border color</source>
        <translation>Couleur de la bordure</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
</context>
<context>
    <name>Application</name>
    <message>
        <location filename="../main/src/application.h" line="+56"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Unknown exception caught!</source>
        <translation>Exception inconnue générée!</translation>
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
        <location line="+125"/>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="+40"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseform.cpp" line="+5"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
</context>
<context>
    <name>BaseObject</name>
    <message>
        <location filename="../libpgmodeler/src/baseobject.cpp" line="+37"/>
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
        <translatorcomment>doute sur le contexte / traduction appropriée ?</translatorcomment>
        <translation type="unfinished">Agrégat</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Operator</source>
        <translation>Operateur</translation>
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
        <location line="+0"/>
        <source>Tablespace</source>
        <translatorcomment>traduciton inappropriée</translatorcomment>
        <translation>Tablespace</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operator Family</source>
        <translation>Famille d&apos;opérateurs</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Operator Class</source>
        <translation>Classe d&apos;Operateurs</translation>
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
        <source>Relationship</source>
        <translation>Relation</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Collation</source>
        <translation>Collation</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Textbox</source>
        <translation>Zone de texte</translation>
    </message>
    <message>
        <location line="+0"/>
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
        <location line="+0"/>
        <source>Basic Relationship</source>
        <translation>Relation simple</translation>
    </message>
    <message numerus="yes">
        <location line="+39"/>
        <source>new_object</source>
        <translation>
            <numerusform>nouvel_objet</numerusform>
            <numerusform></numerusform>
        </translation>
    </message>
</context>
<context>
    <name>BaseObjectWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/baseobjectwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Name:</source>
        <translation>Nom:</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>icone</source>
        <translation>icône</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Comment:</source>
        <translation>Commentaire:</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Tablespace:</source>
        <translation>Tablespace:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Owner:</source>
        <translation>Propriétaire:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Schema:</source>
        <translation>Schéma:</translation>
    </message>
    <message>
        <location line="+163"/>
        <source>Disables the generated SQL code using comment tokens (--)</source>
        <translation>Désactivez le SQL généré en utilisant les marqueurs de commentaire (--)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Disable SQL code</source>
        <translation>Désactiver le code SQL</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Collation:</source>
        <translation>Collation:</translation>
    </message>
    <message>
        <location line="-135"/>
        <source>Edit Permissions</source>
        <translation>Editer les permissions</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Edit permissions</source>
        <translation>Editer les permissions</translation>
    </message>
    <message>
        <location line="+85"/>
        <source>This object is protected thus no change in form will be applied to it.</source>
        <translation>L&apos;objet est vérouillé, par conséquent aucun changement dans le formulaire ne lui sera appliqué.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/baseobjectwidget.cpp" line="+56"/>
        <source>Create / Edit: </source>
        <translation>Créer / Éditer: </translation>
    </message>
    <message>
        <location line="+154"/>
        <source>Required field. Leaving this empty will raise errors!</source>
        <translation>Champrequis. Laisser ce champ vide génèrera une erreur!</translation>
    </message>
    <message>
        <location line="+300"/>
        <source>Value(s)</source>
        <translation>Valeur(s)</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>The &lt;em style=&apos;color: %1&apos;&gt;&lt;strong&gt;highlighted&lt;/strong&gt;&lt;/em&gt; fields on the form are available only on specific PostgreSQL versions. 													When generating SQL code for versions other than those specified on field&apos;s tooltips pgModeler will ignore it&apos;s values.</source>
        <translation>Les champs en &lt;em style=&apos;color: %1&apos;&gt;&lt;strong&gt;surbrillance&lt;/strong&gt;&lt;/em&gt; dans le formulaire sont disponnibles pour certaine version de PostgreSQL uniquement.													Ils seront ignorés s&apos;il est demandé un export SQL pour une version de PostgreSQL non spécifié dans l&apos;infobule du champ.</translation>
    </message>
</context>
<context>
    <name>BaseRelationship</name>
    <message>
        <location filename="../libpgmodeler/src/baserelationship.cpp" line="+57"/>
        <source>rel_%1_%2</source>
        <translation>rel_%1_%2</translation>
    </message>
</context>
<context>
    <name>CastWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/castwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Cast Type:</source>
        <translation>Conversion de type:</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Implicit</source>
        <translation>Implicite</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Assignment</source>
        <translation>Affectation</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Input / Output</source>
        <translation>Entrée / Sortie</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Conversion Function:</source>
        <translation>Fonction de conversion:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/castwidget.cpp" line="+31"/>
        <source>Source data type</source>
        <translation>Type de donnée source</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Target data type</source>
        <translation>Type de donnée cible</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>The function to be assigned to a cast from &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; to &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; must have the following signature: &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; function(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</source>
        <translation>La fonction à affecter à une convertion de &lt;em&gt;&lt;strong&gt;typeA&lt;/strong&gt;&lt;/em&gt; vers &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt;&lt;/em&gt; doit avoir la signature suivante: &lt;em&gt;&lt;strong&gt;typeB&lt;/strong&gt; fonction(&lt;strong&gt;typeA&lt;/strong&gt;, integer, boolean)&lt;/em&gt;.</translation>
    </message>
</context>
<context>
    <name>CollationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/collationwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Locale:</source>
        <translation>Locale:</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Encoding:</source>
        <translation>Encodage:</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE:</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/collationwidget.cpp" line="+28"/>
        <source>The fields &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; are mutually exclusive, so you have to set only one of them in order to properly handle a collation.</source>
        <translation>Les champs &lt;strong&gt;&lt;em&gt;Collation&lt;/em&gt;&lt;/strong&gt;, &lt;strong&gt;&lt;em&gt;Locale&lt;/em&gt;&lt;/strong&gt; et &lt;strong&gt;&lt;em&gt;LC_COLLATE &amp; LC_CTYPE&lt;/em&gt;&lt;/strong&gt; sont mutuellement exclusifs, vous ne devez en définir qu&apos;un de manière à gérer correctement la collation.</translation>
    </message>
    <message>
        <location line="+8"/>
        <location line="+12"/>
        <source>Not defined</source>
        <translation>Non défini</translation>
    </message>
</context>
<context>
    <name>ColumnWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/columnwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut:</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Not Null:</source>
        <translation>Non Null:</translation>
    </message>
</context>
<context>
    <name>ConfigurationForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/configurationform.ui" line="+29"/>
        <source>pgModeler Configuration</source>
        <translation>Configuration de pgModeler</translation>
    </message>
    <message>
        <location line="+49"/>
        <source>&amp;Apply</source>
        <translation>&amp;Appliquer</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Defaults</source>
        <translation>Défauts</translation>
    </message>
    <message>
        <location line="+153"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Style</source>
        <translation>Style</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Connetions</source>
        <translation>Connexions</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Plug-ins</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/configurationform.cpp" line="+103"/>
        <source>Confirmation</source>
        <translation>Confirmation</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Any modification made until now in the current section will be lost! Do you really want to restore default settings?</source>
        <translation>Toutes les modifications réalisées jusqu&apos;à présent dans la section actuelle seront perdues! Souhaitez vous réellement restaurer les paramètres par défaut?</translation>
    </message>
</context>
<context>
    <name>ConnectionsConfigWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/connectionsconfigwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Connections:</source>
        <translation>Connexions:</translation>
    </message>
    <message>
        <location line="+28"/>
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
        <source>Edit selected connection</source>
        <translation>Editer la connexion sélectionnée</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Delete selected connection</source>
        <translation>Supprimer la connexion sélectionnée</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Connection Alias:</source>
        <translation>Alias de la connexion:</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Connection DB:</source>
        <translation>Nom de la base:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Host/Port:</source>
        <translation>Hôte/Port:</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>User:</source>
        <translation>Utilisateur:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Password:</source>
        <translation>Mot de passe:</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Timeout:</source>
        <translation>Délai d&apos;expiration:</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>second(s)</source>
        <translation>seconde(s)</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>SSL Mode:</source>
        <translation>Mode SSL:</translation>
    </message>
    <message>
        <location line="+14"/>
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
        <location line="+30"/>
        <source>Client Certificate:</source>
        <translation>Certificat Client:</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>~/.postgresql/postgresql.crt</source>
        <translation>~/.postgresql/postgresql.crt</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Client Key:</source>
        <translation>Clef client:</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>~/.postgresql/postgresql.key</source>
        <translation>~/.postgresql/postgresql.key</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Root Certificate:</source>
        <translation>Certificat racine:</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>~/.postgresql/root.crt</source>
        <translation>~/.postgresql/root.crt</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Revoked Certs.:</source>
        <translation>Liste de révocations:</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>~/.postgresql/root.crl</source>
        <translation>~/.postgresql/root.crl</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Kerberus Server:</source>
        <translation>Serveur Kerberos:</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Force GSSAPI</source>
        <translation>Forcer GSSAPI</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Options:</source>
        <translation>Options:</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Add</source>
        <translation>Ajouter</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Update</source>
        <translation>Mettre à jour</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Test</source>
        <translation>Tester</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/connectionsconfigwidget.cpp" line="+320"/>
        <source>Success</source>
        <translation>Succès</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Connection successfuly stablished!</source>
        <translation>Connexion établie avec succès !</translation>
    </message>
</context>
<context>
    <name>ConstraintWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/constraintwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+68"/>
        <source>Constraint Type:</source>
        <translation>Type de contrainte:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Check Expr.:</source>
        <translation>Expression de vérification:</translation>
    </message>
    <message>
        <location line="+65"/>
        <source>Fill Factor:</source>
        <translation>Taux de remplissage:</translation>
    </message>
    <message>
        <location line="-132"/>
        <source>Match:</source>
        <translation>Comparaison:</translation>
    </message>
    <message>
        <location line="+167"/>
        <source>Deferrable:</source>
        <translation>Différable:</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Deferral:</source>
        <translation>Report:</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>ON DELETE:</source>
        <translation>ON DELETE:</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>ON UPDATE:</source>
        <translation>ON UPDATE:</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+96"/>
        <source>Column:</source>
        <translation>Colonne:</translation>
    </message>
    <message>
        <location line="-53"/>
        <source>Referenced Columns</source>
        <translation>Colonnes référencées</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Table:</source>
        <translation>Table:</translation>
    </message>
    <message>
        <location line="-150"/>
        <source>Exclude Elements</source>
        <translation>Élements exclus</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>No inherit:</source>
        <translation>Pas d&apos;héritage:</translation>
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
        <location line="+22"/>
        <source>Columns which were included by relationship can not be added / removed manually from the primary key. If done such changes they can raise errors. To create primary key using columns included by relationship use the following options: identifier field, attributes &amp; constraints tab or primary key tab on the relationship form.</source>
        <translation>Les colonnes inclues dans la relation de peuvent être ajoutées/retirés manuellement de la clef primaire sous peine d’erreur. Pour créer une clef primaire utilisant des colonnes contenues dans une relation, veuillez utiliser une des options suivantes: champ identifiant, l&apos;onglet attributs et contraintes ou l&apos;onglet clef primaire du formulaire de la relation.</translation>
    </message>
</context>
<context>
    <name>ConversionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/conversionwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Source Encoding:</source>
        <translation>Encodage de la source:</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Target Encoding:</source>
        <translation>Encodage de la destination:</translation>
    </message>
    <message>
        <location line="+47"/>
        <source>Conversion Func.:</source>
        <translation>Fonction de conversion:</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Default Conversion:</source>
        <translation>Convertion par défaut:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/conversionwidget.cpp" line="+40"/>
        <source>The function to be assigned to an encoding conversion must have the following signature: &lt;em&gt;void function(integer, integer, cstring, internal, integer)&lt;/em&gt;.</source>
        <translation>La fonction à affecter à une convertion d&apos;encodage doit avoir la signature suivante: &lt;em&gt;void function(integer, integer, cstring, internal, integer)&lt;/em&gt;.</translation>
    </message>
</context>
<context>
    <name>CrashHandler</name>
    <message>
        <location filename="../crashhandler/ui/crashhandler.ui" line="+35"/>
        <source>pgModeler Crash Handler</source>
        <translation>Gestionnaire des erreurs de pgModeler</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>Oops! pgModeler just crashed!</source>
        <translation>Oups! pgModeler s&apos;est brutalement et innopinément arrêté!</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Create</source>
        <translation>Créer</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>We apologize for what happened! It is clear that a nasty bug caused it. Please fill out the form below describing your actions before pgModeler quit unexpectedly. This will help on bug extermination and improve the software.</source>
        <translation>Veuillez nous excuser pour ce qu&apos;il vient d&apos;arriver! Il s&apos;agit probablement d&apos;un bug. Merci de remplir le formulaire ci dessous en décrivant ce que vous faisiez avant que pgModeler ne s&apos;arrête inopinément. Celà aidera à supprimer les bugs et améliorer le logiciel.</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Report</source>
        <translation>Rapport</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Actions made before the crash:</source>
        <translation>Actions faites avant le crash:</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Loaded Model</source>
        <translation>Modèle chargé</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Attach the below database model file that possibly generates the crash.</source>
        <translation>Joindre le fichier du modèle qui a potentiellement génére le crash.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Stack trace</source>
        <translation>Pile d&apos;exécution</translation>
    </message>
    <message>
        <location filename="../crashhandler/src/crashhandler.cpp" line="+89"/>
        <source>pgModeler crash file analysis</source>
        <translation>Fichier d&apos;analyse de crash de pgModeler</translation>
    </message>
    <message>
        <location line="+6"/>
        <location line="+58"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location line="-50"/>
        <source>File: %1
Size: %2 bytes

</source>
        <translation>Fichier: %1
Taille: %2 bytes

</translation>
    </message>
    <message>
        <location line="+70"/>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>Crash report successfuly generated! Please send the file &apos;%1&apos; to %2 in order be debugged. Thank you for the collaboration!</source>
        <translation>Rapport de crash généré avec succès! Merci d&apos;envoyer le fichier &apos;%1&apos; à %2 afin qu&apos;il soit analysé. Merci de votre collaboration!</translation>
    </message>
</context>
<context>
    <name>DatabaseModel</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="+2561"/>
        <source>Loading object: %1 (%2)</source>
        <translation>Chargement de l&apos;objet: %1 (%2)</translation>
    </message>
    <message>
        <location line="+3132"/>
        <source>Generating %1 of the object: %2 (%3)</source>
        <translation>Génération %1 de l&apos;objet : %2 (%3)</translation>
    </message>
</context>
<context>
    <name>DatabaseWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/databasewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Template DB:</source>
        <translation>Modèle de BDD:</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Model Author:</source>
        <translation>Auteur du modèle:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Encoding:</source>
        <translation>Encodage:</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>LC_COLLATE:</source>
        <translation>LC_COLLATE:</translation>
    </message>
    <message>
        <location line="+39"/>
        <source>Connections:</source>
        <translation>Connexions:</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>LC_CTYPE:</source>
        <translation>LC_CTYPE:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/databasewidget.cpp" line="+38"/>
        <location line="+12"/>
        <source>Default</source>
        <translation>Défaut</translation>
    </message>
</context>
<context>
    <name>DomainWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/domainwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Name:</source>
        <translation>Nom:</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Not Null:</source>
        <translation>Non Null:</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Check Expr.:</source>
        <translation>Expr. de vérif.:</translation>
    </message>
    <message>
        <location line="-40"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut:</translation>
    </message>
</context>
<context>
    <name>ElementsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/elementswidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Column:</source>
        <translation>Colonne:</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Expression:</source>
        <translation>Expression:</translation>
    </message>
    <message>
        <location line="+62"/>
        <source>Collation:</source>
        <translation>Collation:</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Operator Class:</source>
        <translation>Classe d&apos;opérateurs:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Operator:</source>
        <translation>Opérateur:</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Sorting:</source>
        <translation>Tri:</translation>
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
        <location filename="../libpgmodeler_ui/src/elementswidget.cpp" line="+44"/>
        <source>Element</source>
        <translation>Élement</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Operator Class</source>
        <translation>Classe d&apos;opérateur</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Sorting</source>
        <translation>Tri</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Nulls First</source>
        <translation>Null en premier</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>Collation</source>
        <translation>Collation</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Operator</source>
        <translation>Operateur</translation>
    </message>
    <message>
        <location line="+95"/>
        <source>Expression</source>
        <translation>Expression</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>Exception</name>
    <message>
        <source>Insufficient memory space to allocate the object!</source>
        <translation type="obsolete">Espace mémoire insuffisant pour allouer l&apos;objet!</translation>
    </message>
    <message>
        <location filename="../libutils/src/exception.cpp" line="+23"/>
        <source>Assignment of a pseudo-type to the type of the column!</source>
        <translation>Utilisation d&apos;un pseudo type comme type de la colonne!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Zero length assignment!</source>
        <translation>Affectation de longueur nulle!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a precision greater than the length of the type!</source>
        <translation>Utilisation d&apos;une précision est plus grande que la longueur du type!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an invalid precision to type time, timestamp or interval. The precision in this case must be equal to or less than 6!</source>
        <translation>Utilisation d&apos;une précision de type time, timestamp ou interval invalide. La précision de ces types doit être inférieure ou égale à 6!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a not allocated column to object &apos;%1&apos; (%2)!</source>
        <translation>Affectation à l&apos;objet &apos;%1&apos; (%2) d&apos;une colonne non allouée!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a column which index is out of the capacity of the column list!</source>
        <translation>Référence à une colonne dont l&apos;index dépasse la capacité de la liste des colonnes!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of not allocated object!</source>
        <translatorcomment>Indefini ?</translatorcomment>
        <translation type="unfinished">Utilisation d&apos;un objet non alloué!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a not allocated schema to object &apos;%1&apos; (%2)!</source>
        <translation>Affectation à l&apos;objet &apos;%1&apos; (%2) d&apos;un schéma non alloué!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object &apos;%1&apos; (%2) has inconsistent configuration (invalid SQL/XML definition)!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) a une configuration incohérente (définition SQL/XML invalide)!</translation>
    </message>
    <message>
        <source>The object &apos;%1&apos; (%2) already exists in the list of elements of the same type in the object &apos;%3&apos; (%4)!</source>
        <translation type="obsolete">L&apos;objet &apos;%1&apos; (%2) existe déjà dans la liste des éléments du même type dans l&apos;objet &apos;%3&apos; (%4)!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The object &apos;%1&apos; (%2) can&apos;t be assigned because there is already exists in the container object &apos;%3&apos;!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) ne peut pas être assigné car il existe déjà dans l&apos;objet conteneur &apos;%3&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assigning object of an invalid type!</source>
        <translation>Assignation d&apos;un objet dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Removing an object of an invalid type!</source>
        <translation>Suppression d&apos;un objet dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Obtaining an object of an invalid type!</source>
        <translation>L&apos;objet obtenu est de type invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of empty name to table return type!</source>
        <translation>Affectation d&apos;un nom vide au type retourné par la table!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The insertion of the parameter &apos;%1&apos; will not be possible because there is another parameter with same name in the function &apos;%2&apos;!</source>
        <translation>L&apos;ajout du paramètre &apos;%1&apos; n&apos;est pas possible car un paramètre porte le même nom dans la fonction &apos;%2&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The insertion of the table return type &apos;%1&apos; will not be possible because there is another return type with the same name in the &apos;%2&apos;!</source>
        <translation>L&apos;ajout du type retourné par table &apos;%1&apos; n&apos;est pas possible car un autre type retourné porte le même nom dans &apos;%2&apos;!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to an event which does not belongs to trigger!</source>
        <translatorcomment>Qui ne provient ?</translatorcomment>
        <translation>Référence à un évènement qui n&apos;appartient pas au déclencheur!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The column &apos;%1&apos; can&apos;t be assigned to the trigger &apos;%2&apos; because does not belongs to the trigger&apos;s parent table!</source>
        <translation>La colonne &apos;%1&apos; ne peut être assigné au déclencheur &apos;%2&apos; car elle n&apos;appartient pas à la table parente du déclencheur!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a not allocated function to object &apos;%1&apos; (%2)!</source>
        <translation>Affectation à l&apos;objet &apos;%1&apos; (%2) d&apos;une fonction non allouée!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function which return type is different from &apos;trigger&apos;!</source>
        <translation>Affectation d&apos;une fonction retournant un type autre que &apos;trigger&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function which parameter count is invalid to the object &apos;%1&apos; (%2)!</source>
        <translation>Affectation à l&apos;objet &apos;%1&apos; (%2) d&apos;une fonction ayant un nombre de paramètres invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function which language is invalid!</source>
        <translation>Définition d&apos;une fonction dont le langage est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of not allocated table to object &apos;%1&apos; (%2)!</source>
        <translation type="unfinished">Affectation à l&apos;objet &apos;%1&apos; (%2) d&apos;une table indéfinie!</translation>
    </message>
    <message>
        <source>Insertion of a column which name is already registered to another column of the object!</source>
        <translation type="obsolete">Ajout d&apos;une colonne dont le nom est déjà utilisé pour une autre colonne de l&apos;objet!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of empty name to an object!</source>
        <translation>Affectation d&apos;un nom vide à l&apos;objet!</translation>
    </message>
    <message>
        <source>Assignment of invalid name to an object!</source>
        <translation type="obsolete">Le nom associé à l&apos;objet est invalide!</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Assignment of schema object which type is invalid!</source>
        <translation>Affectation d&apos;un schema dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of tablespace object with invalid type!</source>
        <translation>Utilisation d&apos;un tablespace dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of tablespace to an invalid object!</source>
        <translation>Affectation d&apos;un tablespace à un objet invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of tablespace to a constraint which type is invalid! To belong to a tablespace the constraint must be a primary key or unique!</source>
        <translation>Affectation d&apos;un tablespace à une contrainte dont le type est invalide! Pour appartenir à un tablespace une contrainte doit être une clef primaire ou unique!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of owner object which type is invalid!</source>
        <translation>Affectation d&apos;un propiétaire dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of owner to an invalid object!</source>
        <translation>Affectation d&apos;un propriétaire à un objet invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a function with invalid type!</source>
        <translation>Référence à une fonction dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an argument of the operator with invalid type!</source>
        <translation>Référence à un argument de l&apos;opérateur dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an operator with invalid type!</source>
        <translation>Référence à un opérateur dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of value to an invalid option type on role!</source>
        <translation>Utilisation d&apos;un attribut invalide pour un rôle!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid role type!</source>
        <translation>Référence à un type de rôle invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The insertion of the role &apos;%1&apos; is not possible because this is already being referenced by role &apos;%2&apos;!</source>
        <translation>Impossible d&apos;ajouter le rôle &apos;%1&apos; car il est déjà référencé par le rôle &apos;%2&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference redundancy detected by having the role &apos;%1&apos; referencing the role &apos;%2&apos;!</source>
        <translation>Redondance détéctée, le rôle &apos;%1&apos; référençant le rôle &apos;%2&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The role &apos;%1&apos; can not be listed as a member of itself!</source>
        <translation>Le rôle &apos;%1&apos; ne peut pas être membre de lui-même !</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Insertion of empty command to the rule!</source>
        <translation>Ajout d&apos;une commande vide dans cette règle!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Is not possible to create a self generalization/copy relationship! The table can not inherit or copy their own attributes!</source>
        <translation>Impossible de créer une relation de généralisation/copie avec elle même! Une table ne peut hériter ou copier ses propres attributs!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an object that already belongs to another table!</source>
        <translation>Utilisation d&apos;un objet appartenant déjà à une autre table!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a schema to the sequence which differs from the schema of the owner table!</source>
        <translation>Affectation d&apos;un schéma à la séquence diffèrent du schéma de la table!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an invalid value to one of the sequence attributes!</source>
        <translation>Utilisation d&apos;une valeur invalide comme attribut de séquence!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a minimum value to the sequence which is greater than the maximum value!</source>
        <translation>La valeur minimale de la séquence est supérieure à la valeur maximale!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of a null increment value to the sequence!</source>
        <translation>La valeur de l&apos;incrément de la séquence est nulle!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of null cache value to the sequence!</source>
        <translation>La valeur cache de la séquence est nul!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of owner table which is not in the same schema as the sequence &apos;%1&apos;!</source>
        <translation>Utilisation d&apos;un schéma pour la table diffèrent du schéma de la séquence &apos;%1&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of owner table which does not belong to the same owner of the sequence &apos;%1&apos;!</source>
        <translation>Le propriétaire de la table n&apos;est pas le même que celui de la séquence &apos;%1&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a nonexistent owner column to the sequence &apos;%1&apos;!</source>
        <translation>Le propriétaire de la séquence &apos;%1&apos; n&apos;existe pas!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an owner column to the sequence &apos;%1&apos; that is not related to any table!</source>
        <translation>La colonne affilié à la séquence &apos;%1&apos; n&apos;est référencé dans aucune table!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Allocation of object with invalid type!</source>
        <translation>Le type de l&apos;objet alloué est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function with invalid return type to object &apos;%1&apos; (%2)!</source>
        <translation>Assignation à l&apos;objet &apos;%1&apos; (%2) d&apos;une fonction dont le type de retour est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a function with invalid parameter(s) type(s) to object &apos;%1&apos; (%2)!</source>
        <translation>Affectation à l&apos;objet &apos;%1&apos; (%2) d&apos;une fonction dont le(s) type(s) de paramètre(s) sont invalides!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of not allocated language!</source>
        <translatorcomment>Indéfini ?</translatorcomment>
        <translation type="unfinished">Utilisation d&apos;un langage non alloué!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of language object which type is invalid!</source>
        <translation>Définition d&apos;un objet langage dont le type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to data type with an index outside the capacity of data types list!</source>
        <translation>Référence à un type de donnée dont l&apos;index est en dehors de la capacité de la liste des types de données!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a null type to to object &apos;%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un type null à l&apos;objet &apos;%1&apos; (%2)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid type to the object!</source>
        <translation>Affectation d&apos;un type invalide à l&apos;objet!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an empty directory to object &apos;%1&apos; (%2)!</source>
        <translation>Affectation d&apos;un nom de dossier vide à l&apos;objet &apos;%1&apos; (%2)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Obtaining types with invalid quantity!</source>
        <translation>Utilisation d&apos;une quantité de types invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of item which already exists in the attributes list of the type!</source>
        <translation>Ajout d&apos;un attribut déjà présent dans la définition de ce type!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of invalid item in the attributes list of the type!</source>
        <translation>Ajout d&apos;un attribut invalide à la liste des attributs de ce type!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of item which already exists in the enumarations list of the type!</source>
        <translation>Ajout d&apos;un élément déjà existant dans l&apos;énumeration!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of invalid item in the enumerations list of the type!</source>
        <translation>Ajout d&apos;un élément invlaide dans l&apos;énumeration!</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Assignment of invalid configuration to the type!</source>
        <translation>La configuration assigné au type est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The data type &apos;%1&apos; can not be assigned because it already exists in the types list of the aggregate function &apos;%2&apos;!</source>
        <translation>Le type de donnée &apos;%1&apos; ne peut être assigné car il existe déjà dans la liste de types de la fonction d&apos;agrégation &apos;%2&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an operator which input type count is invalid to aggregate function!</source>
        <translation>Utilisation d&apos;un opérateur dont le nombre d&apos;argument est invalide avec une fonction d&apos;agrégation (un et seulement un)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an operator which types of arguments is invalid!</source>
        <translation>Utilisation d&apos;un opérateur dont le type des arguments est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of system reserved name to the object &apos;%1&apos; (%2)!</source>
        <translation>Utilisation d&apos;un mot clef réservé avec l&apos;objet &apos;%1&apos; (%2)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>One function with invalid configuration is been used by the object &apos;%1&apos; (%2)!</source>
        <translation>Une fonction dont laconfiguration n&apos;est pas valide est utilisé par l&apos;objet &apos;%1&apos; (%2)!</translation>
    </message>
    <message>
        <location line="+20"/>
        <source>The object &apos;%1&apos; (%2) is referencing the object &apos;%3&apos; (%4) which was not found in the model!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) référence à l&apos;objet &apos;%3&apos; (%4) qui n&apos;a pas été trouvé dans le modèle!</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>The object &apos;%1&apos; (%2) can not be edited or deleted because it was automatically included through a relationship! If the object is an attribute or constraint the modifications must be done on the relationship editing form.</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) ne peut être édité ou supprimé car il a été inclus automatiquement par une relation. Pour manipuler cet objet il est nécessaire de le faire depuis le formulaire d&apos;édition de ladite relation.</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>A view reference must be used in at least one these SQL scopes: View Definition, SELECT-FROM, FROM-WHERE or After WHERE!</source>
        <translation>Lors de la définition d&apos;une vue, une référence doit utiliser au moins une des options suivantes:SELECT-FROM, FROM-WHERE ou Après WHERE!</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Invalid use of a view referece as whole SQL definition! The assigned reference must be an expression!</source>
        <translatorcomment>À contrôler</translatorcomment>
        <translation type="unfinished">Utilisation invalide d&apos;une vue en tant que définition SQL! La référence doit être une expression!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a second definition expression to the view!</source>
        <translation>Assignation d&apos;une seconde expression à la vue!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>It&apos;s not possible mix ordinary references (SELECT-FROM, FROM-WHERE, After WHERE) with references used as view SQL definition!</source>
        <translation>Il n&apos;est pas possible de mélanger les défintion ordinaires (SELECT FROM, FROM WHERE, Après WHHERE) et les expressions SQL en tant que définition de la vue!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of collation object which type is invalid!</source>
        <translatorcomment>À contrôler</translatorcomment>
        <translation type="unfinished">Utilisation d&apos;un objet collation invlaide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>At the moment pgModeler doesn&apos;t support the creation of primary keys which some columns were generated by relationship connection. To create primary keys with this feature you can use the field &apos;Indentifier&apos; or the tab &apos;Primary key&apos; on relationship editing form!</source>
        <translation>Pour le moment, phModeler ne gère pas la création de clef primaires avec des colonnes générés par des relations. Pour cela, vous pouvez utiliser le champs &apos;Identifiant&apos; ou l&apos;onglet &apos;Clef primaire&apos; dans le formulaire d&apos;édition de la relation!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Collations must be created at least with attributes LC_COLLATE and LC_CTYPE defined!</source>
        <translation>Les collations doivent être définies avec les attributs LC_COLLATE et LC_CTYPE renseignés!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object &apos;%1&apos; (%2) cannot reference itself! This operation is not permitted for this kind of object!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) ne peut s&apos;auto-réferencer! Cette opération n&apos;est pas autorisé sur ce genre d&apos;objet!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only operator families which uses &apos;btree&apos; as indexing method is accepted by operator class elements!</source>
        <translatorcomment>utilisable comme OU utilisable par ?</translatorcomment>
        <translation type="unfinished">Seuls les opérateurs utilisant des indexes BTREE sont utilisable comme classe d&apos;opérateur!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid copy table option!</source>
        <translation type="unfinished">Référence à une option invalide de copie de table!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Copy relationship between tables &apos;%1&apos; and &apos;%2&apos; cannot be done because the first one already copies attributes from &apos;%3&apos;! Tables can have only one copy table!</source>
        <translation>La relation de copie entre les tables &apos;%1&apos; et &apos;%2&apos; car la première copie déjà des attributs depuis la table &apos;%3&apos;! Les tables ne peuvent avoir qu&apos;une table de copie!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The INSTEAD OF mode cannot be used on triggers that belongs to tables! This is available only for view triggers!</source>
        <translation>Le mode AU LIEU DE (INSTEAD OF) ne peut être utilisé comme trigger de table! Ce n&apos;est valable que pour les triggers de vue!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The TRUNCATE event can only be used when the trigger executes for each statement and belongs to a table!</source>
        <translation>L&apos;événement TRUNCATE ne peut être utilisé que lors d&apos;un trigger sur chaque ligne et qu&apos;il appartient à une table!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The INSTEAD OF mode cannot be used on view triggers that executes for each statement!</source>
        <translation>Le mode AU LIEU DE (INSTEAD OF) ne peut être utilisé avec triggeurs de vues qui s’exécutent à chaque requête!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Constraint triggers can only be executed on AFTER events and for each row!</source>
        <translation>Les triggeurs de contraintes ne peuvent être exécutés qu&apos;après (AFTER) et pour chaque lignes!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A view trigger cannot be AFTER/BEFORE when it executes for each row!</source>
        <translation>Les triggeurs de vues ne peuvent être AFTER/BEFORE quand il sont configuré pour une exécution sur chaque ligne!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A trigger cannot make reference to columns when using INSTEAD OF mode and UPDATE event!</source>
        <translation>Un triggeur ne peut faire référence aux colonnes quand il est utilisé en mode INSTEAD OF sur un événement de type UPDATE!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a column which has no parent table to the object &apos;%1&apos; (%2)!</source>
        <translation>Utilisation d&apos;une colonne qui n&apos; pas de table parente avec l&apos;objet &apos;%1&apos; (%2)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Only constraint triggers can be deferrable or reference another table!</source>
        <translation>Seuls les déclencheurs de contraintes peuvent être déférés ou référencer une autre table!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a function id which is incompatible whit the user define type configuration!</source>
        <translatorcomment>Doutes</translatorcomment>
        <translation type="unfinished">Référence faite à un identifiant de fonction incompatible avec le type configuré!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The operator class assigned to the object &apos;%1&apos; (%2) must use &apos;btree&apos; as indexing method!</source>
        <translation>La classe d&apos;opérateur assigné à l&apos;objet &apos;%1&apos; (%2) doit utiliser une indexation de type &apos;btree&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid PostgreSQL version &apos;%1&apos; assigned to the schema parser! Valid versions are: %2</source>
        <translation>La version &apos;%1&apos; de PostgreSQL défini pour le parseur est invalide. Les verions valides sont: %2</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The validation process failed due to an error triggered by the validation helper. For more details about the error check the exception stack!</source>
        <translation>La procédure de validation a échouée à cause d&apos;une erreur levée par l&apos;assistant de validation. Pour de plus amples informations, referez vous à la pile des exceptions!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The extension &apos;%1&apos; is registered as a data type and cannot have the attribute &apos;handles datatype&apos; modified!</source>
        <translatorcomment>singulier ? pluriel ?</translatorcomment>
        <translation type="unfinished">L’extension &apos;%1&apos; est enregistré en tant que type de données et ne peut voir son attribut &quot;gère des types de données&quot; modifié !</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The fk relationship &apos;%1&apos; cannot be created because the foreign-key that represents it wasn&apos;t created on table &apos;%2&apos;!</source>
        <translation>La relation &apos;%1&apos; ne peut être créée car la clef étrangère qui la représente ne peut être créer dans la table &apos;%2&apos;!</translation>
    </message>
    <message>
        <source>Assignment of invalid id to the user!</source>
        <translation type="obsolete">Affectation d&apos;un id invalide à l&apos;utilisateur!</translation>
    </message>
    <message>
        <location line="-114"/>
        <source>Assignment of an invalid strategy/support number to an operator class element!</source>
        <translatorcomment>Hum ... numéro de stratégie/support ?</translatorcomment>
        <translation type="unfinished">Affectation d&apos;un numéro de stratégie/support invalide à un élément de la classe &apos;operator&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of element which already exists in the element list!</source>
        <translation>Insertion d&apos;un élément déjà existant dans la liste des éléments!</translation>
    </message>
    <message>
        <location line="-65"/>
        <source>Reference to a parameter which index is out of the parameter list bounds!</source>
        <translation>Référence un paramètre dont l&apos;index est hors des limites de la liste de ces derniers!</translation>
    </message>
    <message>
        <location line="-8"/>
        <source>The object &apos;%1&apos; (%2) already exists on &apos;%3&apos; (%4)!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) existe déjà: &apos;%3&apos; (%4)!</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Reference to an argument which index is out of argument list bounds!</source>
        <translation>Référence un argument dont l&apos;index est hors des limites de la liste de ces derniers!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Assignment of a name which contains invalid characters!</source>
        <translation>Utilisation de caractères invalides dans le nom!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a name which length exceeds the maximum of 63 characters!</source>
        <translation>Utilisation de plus de 63 caractères dans le nom!</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Reference to a role which index is out of role list bounds!</source>
        <translation>Référence un rôle dont l&apos;index est hors des limites de la liste de ces derniers!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to a command which index is out of the command list bounds!</source>
        <translation>Référence une commande dont l&apos;index est hors des limites de la liste de ces dernières!</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Assignment of a start value to the sequence which is extrapolating the range defined by minimum and maximum values!</source>
        <translatorcomment>À contrôler.</translatorcomment>
        <translation>Affectation de la valeur de départ de la séquence en dehors des limites de cette dernière (min et max)!</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Reference to a label which index is out of labels list bounds!</source>
        <translation>Référence un label dont l&apos;index est hors des limites de la liste de ces derniers!</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Reference to an attribute which index is out of the attributes list bounds!</source>
        <translation>Référence un attribut dont l&apos;index est hors des limites de la liste de ces derniers!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an enumeration which index is out of the enumerations list bounds!</source>
        <translation>Référence une énumération dont l&apos;index est hors des limites de la liste de ces dernières!</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Reference to an element which index is out of element list bounds!</source>
        <translation>Référence un élément dont l&apos;index est hors des limites de la liste de ces derniers!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an object which index is out of object list bounds!</source>
        <translation>Référence un objet dont l&apos;index est hors des limites de la liste de ces derniers!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Removal of an object not allocated!</source>
        <translation>Suppression d&apos;un objet non alloué!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object &apos;%1&apos; (%2) can not be removed because it is being referenced by object &apos;%3&apos; (%4)!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) ne peut être supprimé car il est référencé par l&apos;objet &apos;%3&apos; (%4)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object &apos;%1&apos; (%2) can not be removed because it is being referenced by object &apos;%3&apos; (%4) that belongs to &apos;%5&apos; (%6)!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) ne peut être supprimé car il est référencé par l&apos;objet  &apos;%3&apos; (%4) qui appartient à  &apos;%5&apos; (%6)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation with object(s) which type(s) is invalid!</source>
        <translation>Opérations sur des objet(s) dont le(s) type(s) sont invalides!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to object with invalid type!</source>
        <translation>Référence à un objet ayant un type invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation with object not allocated!</source>
        <translation>Opération sur des objets non alloués!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The creation of the relationship &apos;%1&apos; between the table &apos;%2&apos; and &apos;%3&apos; can not be done because one does not have a primary key. If the relationship is of the type n-n both tables must have primary keys!</source>
        <translation>La création de la relation &apos;%1&apos; entre la table &apos;%2&apos; et &apos;%3&apos; ne peut être faite car une des tables ne contient pas de clef primaire. Si la relation est de type N-N, les deux tables doivent avoir une clef primaire!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The relationship of the type 1-1 where both tables are mandatory participation is not implemented because it requires fusion between the tables that breaks the modeling done by the user!</source>
        <translation>La relation de type 1-1, quand les deux tables sont obligatoires, n&apos;est pas implémentée car celà requiert la fusion des tables, ce qui casse le modèle réalisé par l&apos;utilisateur!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of an invalid expression to the object!</source>
        <translation>Affectation d&apos;une expression invalide à l&apos;objet!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a primary key to a table which already has one!</source>
        <translation>Impossible de définir une clef primaire à une table qui en possède déjà une!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Identifier relationship can not be created for a self relationship, relationships of the type n-n, copy or generalization!</source>
        <translatorcomment>Review needed ! Doutes ...</translatorcomment>
        <translation>Seuls des relations de type 1-1 ou 1-N peuvents être utilisé comme identifiant. En aucun cas les autorelations,relations N-N, généralisations ou copies ne le peuvent!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to create a copy relationship because the column &apos;%1&apos; in table &apos;%2&apos; already exists in table &apos;%3&apos;!</source>
        <translation>Impossible de créer une relation copie car la colonne &apos;%1&apos; de la table &apos;%2&apos; existe déjà dans la table &apos;%3&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to create the generalization relationship because the column &apos;%1&apos; in table &apos;%2&apos; can not be merged with the column &apos;%3&apos; of table &apos;%4&apos; because they have incompatible types!</source>
        <translation>Impossible de créer la relation de généralisation car la colonne &apos;%1&apos; de la table &apos;%2&apos; ne peut pas être fusionnée avec la colonne &apos;%3&apos; de la table &apos;%4&apos;, leurs types sont incompatibles!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>An attribute can not be added to a copy or generalization relationship!</source>
        <translation>Un attribut ne peut être ajouté à une relation copie ou généralisation!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A foreign key can not be added to a relationship because is created automatically when this is connected!</source>
        <translation>Une clef étrangère ne peut être ajoutée à une relation car elle est créée automatiquement lors de la mise en relation!</translation>
    </message>
    <message>
        <source>The object &apos;%1&apos; (%2) is referencing the object &apos;%3&apos; (%4) which was not found in the list of objects of the same type in the model!</source>
        <translation type="obsolete">L&apos;objet &apos;%1&apos; (%2) référence l&apos;objet &apos;%3&apos; (%4) qui n&apos;a pas été trouvé dans la liste des objets du même type dans le modèle!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Reference to an user-defined data type that not exists in the model!</source>
        <translation>Référence à un type de donné défini par l&apos;utilisateur qui n&apos;existe pas dans le modèle!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid maximum size to operation list!</source>
        <translation>La taille maximum affectée à la liste d&apos;opérations est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to write the file %1! Make sure the directory exists, or if the user has access permissions on it!</source>
        <translation>Impossible d&apos;écrire dans le fichier %1!! Assurez vous que le dossier existe et que l&apos;utilisateur a les permissions d&apos;écriture dessus!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to write the model in the file %1 due to one or more errors in the definition generation process!</source>
        <translation>Impossible d&apos;écrire le modèle dans le fichier %1 en raisons d&apos;erreurs dans le process de génération de définition!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>There is already a relationship between &apos;%1&apos; (%2) and &apos;%3&apos; (%4) in the model!</source>
        <translation>Il y existe déjà une relation entre &apos;%1&apos; (%2) et &apos;%3&apos; (%4) dans le modèle!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The configuration of the relationship &apos;%1&apos; generates a redundancy between the relationships &apos;%2&apos;. Redundancy on identifier or generalization/copy relationships are not accepted since they result in  incorrect column spreading making the model inconsistent!</source>
        <translation>La définition de la relation &apos;%1&apos; introduit une redondance avec la relation &apos;%2&apos;. Les redondances protant sur les identifiants ou les relations de généralisations/cope ne sont pas autorisés du fait qu&apos;il en résulte une dispertion des colonnes rendant le le modèle incohérent!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>One or more objects were invalidated and automatically removed because they were referencing table columns which were included through relationships and which no longer exists due to disconnection of relationships or exclusion of such generated columns!</source>
        <translation>Un ou plusieurs objets ont été invalidés et automatiquement supprimés car ils référençaient des colonnes qui étaient incluses dans des relations et qui n&apos;existaient plus du fait de la suppression de relation ou d&apos;exclusion de ces colonnes!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The primary key &apos;%1&apos; can only be allocated if declared within a block of code that defines a table or relationship!</source>
        <translation>La clef primaire &apos;%1&apos; ne peut être définie que si elle a été déclarée dans une portion de code qui défini une table ou une relation!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to an invalid privilege type!</source>
        <translation>Référence à un type de privilège invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Insertion of a role which already exists in the role list of the permission!</source>
        <translation>Ajout d&apos;un rôle déjà existant dans la liste de cette permission!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of privilege incompatible with the type of object referenced by permission!</source>
        <translation>Utilisation d&apos;un privilège incompatible avec le type d&apos;objet référencé par la permission!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>There is already a permission on object &apos;%1&apos; (%2) which has one or more equal roles from those present on permission to be assigned to the object!</source>
        <translation>Il y a déjà une permission sur l&apos;objet &apos;%1&apos; (%2) qui possède un ou plusieurs rôles identiques parmis ceux présents!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>A permission is referencing the object &apos;%1&apos; (%2) which was not found in the model!</source>
        <translation>Une autorisation référence l&apos;objet &apos;%1&apos; (%2) mais n&apos;a pas été trouvée dans le modèle!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The object &apos;%1&apos; (%2) can not be created because its not being assigned to any schema!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) ne peut être céer car il n&apos;est assigné à aucun schéma!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The tablespace &apos;%1&apos; can not be inserted into the model because it points to the same directory as the tablespace &apos;%2&apos;!</source>
        <translation>Le tablespace &apos;%1&apos; ne peut être inséré dans le modèle car il pointe sur le même dossier que le tablespace &apos;%2&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>It is not possible to create arrays of domains or sequences (dimension &gt;= 1)! PostgreSQL does not yet implement this feature!</source>
        <translation>Il est impossible de créer des tableaux de domaines ou séquences (dimension &gt;= 1)! PostgreSQL n&apos;inplémente pas encore cette fonctionnalité!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The function &apos;%1&apos; can not get a source code as a definition because its language is set to C. Use the attributes symbol and dynamic library instead!</source>
        <translation>La fonction &apos;%1&apos; ne peut avoir de code source car définie comme langage C. Utilisez plutôt le symbol des attributs et les librairies dynamiques!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The function &apos;%1&apos; can have the attributes symbol and dynamic library configured only if the language is set to C. For all other cases you must specify a source code that defines it in the DBMS!</source>
        <translation>La fonction &apos;%1&apos; ne peut avoir de symbole d&apos;attributs ou des librairies dynamiques configurés que si définit comme langage C. Pour tout autre cas vous devez spécifier un code source défini dans le SGBD!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The operator &apos;%1&apos; can not be assigned as a comutator of operator &apos;%2&apos; because it has incompatible settings!</source>
        <translation>L&apos;opérateur &apos;%1&apos; ne peut être assigné comme un commutateur de l&apos;opérateur &apos;%2&apos; car il a des paramètres incompatibles!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The operator &apos;%1&apos; can not be assigned as negator of operator &apos;%2&apos; because it has incompatible settings!</source>
        <translation>L&apos;opérateur &apos;%1&apos; ne peut être assigné comme une négation de l&apos;opérateur &apos;%2&apos; car il a des paramètres incompatibles!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The type &apos;%1&apos; can not self refer in the attributes &apos;element&apos; or &apos;copy type&apos; or be used as a data type of an attribute in the configuration of a composite type!</source>
        <translation>Le type &apos;%1&apos; ne peut s&apos;auto référencer ni dans les attributs &apos;element&apos;, ni &apos;copy type&apos;, ni même être utilisé comme un type de donnée d&apos;un attribut pour la définition d&apos;un type composite!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid element to type &apos;%1&apos;!</source>
        <translation>Utilisation d&apos;un élément invalide pour le type &apos;%1&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid alignment to type &apos;%1&apos;!</source>
        <translation>Utilisation d&apos;un alignement invalide pour le type &apos;%1&apos;!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid name to the table generated from N-N relationship!</source>
        <translation>Le nom de la table générée par la relation N-N est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The relationship &apos;%1&apos; can not make use of the special primary key. Only generalization/copy relationships have access to this resource!</source>
        <translation>La realtion &apos;%1&apos; ne peut pas utilier de clef primaire particulière. Seuls les relations de généralisation ou de copie peuvent!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of invalid suffix to the relationship &apos;%1&apos;!</source>
        <translation>Utilisation d&apos;un suffixe invalide dans la relation &apos;%1&apos;!</translation>
    </message>
    <message>
        <source>The object &apos;%1&apos; (%2) can not be edited or deleted because it was automatically included through a relationship! To manipulate the object is necessary to make it from the edit form of the relationship that holds it.</source>
        <translation type="obsolete">L&apos;objet &apos;%1&apos; (%2) ne peut être édité ou supprimé car il a été inclus automatiquement par une relation. Pour manipuler cet objet il est nécessaire de le faire depuis le formulaire d&apos;édition de ladite relation.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>The object &apos;%1&apos; (%2) can not be deleted because it is protected!</source>
        <translation>L&apos;objet &apos;%1&apos; (%2) ne peut être effacé car il est vérrouillé!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group &apos;%1&apos; has already been declared earlier!</source>
        <translation>Le groupe &apos;%1&apos; a déjà été déclaré précédemment!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group &apos;%1&apos; can not be built in the groups declaration block (&apos;%2&apos;)!</source>
        <translation>Le groupe &apos;%1&apos; ne peut être construit dans le bloc de déclaration des groupes (&apos;%2&apos;)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group &apos;%1&apos; was built but not declared in the groups declaration block (&apos;%2&apos;)!</source>
        <translation>Le groupe &apos;%1&apos; a été construit mais non déclaré dans le bloc de déclaration des groupes (&apos;%2&apos;)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group &apos;%1&apos; can not be built without possessing child elements!</source>
        <translation>Le groupe &apos;%1&apos; ne peut être construit s&apos;il ne possède pas les éléments enfants!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group &apos;%1&apos; can not be built once more because this was done in previous blocks!</source>
        <translation>Le groupe &apos;%1&apos; ne peut être construit une fois de plus car celà a été effectué dans le bloc précédent!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The group &apos;%1&apos; has been declared but not built!</source>
        <translation>Le groupe &apos;%1&apos; a été déclaré mais non construit!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a column of the objects table with invalid index!</source>
        <translation>Référence à une colonne de la table ayant un index invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a row of the objects table with invalid index!</source>
        <translation>Référence à une ligne de la table ayant un index invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The schema public and the languages plpgsql, c and sql can not be manipulated because they are reserved to PostgreSQL! They are present in the model database only as a reference!</source>
        <translation>Le schéma public et les langages plpgsql, c et sql ne peuvent être manipulés car ils sont réservés à PostgreSQL! Ils sont présents dans la BDD des modèles seulement en tant que référence!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The new configuration function invalidates the object &apos;%1&apos; (%2)! In this case it is needed to undo the relationship between the affected object and function so that the new configuration of the latter to take effect!</source>
        <translation>La nouvelle fonction de configuration invalide l&apos;objet &apos;%1&apos; (%2)! Dans ce cas il est nécessaire de supprimer la relation entre les objets concernés et la fonction de manière à ce que ce soit la configuration la plus récente qui soit prise en compte!</translation>
    </message>
    <message>
        <source>A vision reference must have at least one SQL scope: SELECT, FROM, FROM-WHERE or After WHERE!</source>
        <translation type="obsolete">Lors de la définition d&apos;une vue, une référence doit utiliser au moins une des options suivantes:SELECT-FROM, FROM-WHERE ou Après WHERE!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Constraints like primary key, foreign key or unique must have at least one column related to them! For foreign keys must be selected, in addition, the referenced columns!</source>
        <translation>Les contraintes telles que clef primaire, clef étrangère ou l&apos;unicité doivent avoir au moins une colonne associée! Pour les clefs étrangères, les colonnes référencées doivent également être sélectionnées!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to load one or more configuration files! Please check if files exists in the configuration folder and if they are not corrupted to preventing this error to occur again on the next startup!</source>
        <translation>Impossible de charger un ou plusieurs fichiers de configuration! Veuillez vérifier que les fichiers existent bien dans le dossier de configuration et qu&apos;ils ne soient pas corrompus pour éviter que l&apos;erreur ne se produise à nouveau au prochain lancement!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not find the default settings file &apos;%1&apos;! To restore default settings check the existence of the file and try again!</source>
        <translation>Impossible de trouver le fichier des paramètres par défaut &apos;%1&apos;! Pour restaurer les paramètres par défaut vérifiez que le fichier existe et essayez encore!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The export process failed due to an error triggered by the PostgreSQL server in an attempt to execute a SQL command. For more details about the error check the exception stack!

** Executed SQL command: **

%1</source>
        <translation>Le process d&apos;exportation a échoué suite à une erreur déclenchée par le serveur PostgreSQL lors de la tentative d&apos;exécution d&apos;une commande SQL. Pour plus de détails concernant l&apos;erreur vérifiez la pile d&apos;exceptions!

** Commande SQL exécutée: **

%1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not load the plugin &apos;%1&apos; from the library &apos;%2&apos;! Message returned by plugin manager: &apos;%3&apos;</source>
        <translation>Impossible de charger le greffon &apos;%1&apos; à partir de la librairie &apos;%2&apos;! Message retourné par le gestionnaire de greffons: &apos;%3&apos;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>One or more plugins were not activated due to errors during the loading process! Check the exception stack for more details.</source>
        <translation>Un ou plusieurs greffons n&apos;ont pas été activé(s) car des erreurs sont apparues lors du processus de chargment. Pour plus de détails, consultez la pile d&apos;exceptions.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid syntax in file %1, line %2, column %3!</source>
        <translation>Syntaxe invalide dans le fichier %1, ligne %2, colonne %3!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid conditional instruction &apos;%1&apos; on file %2, line %3, column %4!</source>
        <translation>Instruction conditionnelle invalide &apos;%1 dans le fichier %2, ligne %3, colonne %4!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unknown attribute &apos;%1&apos; in file %2, line %3, column %4!</source>
        <translation>Attribut inconnu &apos;%1&apos; dans le fichier %2, ligne %3, colonne %4!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Invalid metacharacter &apos;%1&apos; in file %2, line %3, column %4!</source>
        <translation>Metacaractère invalide &apos;%1 dans le fichier %2, ligne %3, colonne %4!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attribute &apos;%1&apos; with an undefined value in file %2, line %3, column %4!</source>
        <translation>Attribut &apos;%1&apos; ayant une valeur non définie dans le fichier %2, ligne %3, colonne %4!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of empty XML buffer to parser!</source>
        <translation>Passage d&apos;un tampon XML vide au parseur!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not access the file or directory %1! Make sure that it exists or if the user has access permissions on it!</source>
        <translation>Impossible d&apos;accéder au fichier ou dossier %1! Assurez vous qu&apos;il existe et que l&apos;utilisateur a le droit d&apos;y accéder!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of empty DTD file name!</source>
        <translation>Le nom du fichier DTD est vide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of empty name to the DTD declaration!</source>
        <translation>Le nom de la DTD est vide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Error while interpreting XML buffer at line %1 column %2.
Message generated by the parser: &apos;%3&apos;. %4</source>
        <translation>Erreur lors de l&apos;interprétation du tampon XML ligne %1 colonne %2.
Message généré par l&apos;analyseur: &apos;%3&apos;. %4</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation on unallocated element tree! It is necessary to load the XML parser buffer and interpret it so that the tree is generated!</source>
        <translation>Opération sur un élément de l&apos;arbre XML non initialisé! Il est nécessaire de remplir le tampon de l&apos;analyseur XML et de lancer l&apos;interprétation pour que l&apos;arbre soit généré!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not load file %1. The same appears to be inconsistent or one of its dependencies (DTD files) has errors or is missing!</source>
        <translation>Impossible de charger le fichier %1. Le fichier semble incohérent ou une de ses dépendances (fichiers DTD) contient des erreurs ou est manquantes!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation with unallocated tree element!</source>
        <translation>Opération sur un élément non alloué de l&apos;arbre!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation with element which does not exists in the element tree currently loaded!</source>
        <translation>Opération sur un élément inexistant dans l&apos;arbre des élément actullement chargé!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of a value to an invalid connection parameter!</source>
        <translation>Affectation d&apos;une valeur à un paramètre de connexion invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Operation on connection not established!</source>
        <translation>Opération sur une connexion non établie!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attempt to connect without define configuration parameters!</source>
        <translation>Tentative de connexion sans paramètre de configuration défini!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not connect to the database.
Message returned: &apos;%1&apos;</source>
        <translation>Connexion à la base impossible.
Message retourné: &apos;%1&apos;</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Assignment of not allocated SQL command result!</source>
        <translatorcomment>indéfini ?</translatorcomment>
        <translation type="unfinished">Affectation d&apos;un résultat de commande SQL non alloué!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to allocate the result of the SQL command because the response from the DBMS was not understood by the client!</source>
        <translation>Impossible de retourner le résultat de la commande SQL car la réponse du SGBD n&apos;a pas été comprise par le client!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to allocate command result for the SQL because the server has generated a fatal error!
Message returned by the DBMS: %1</source>
        <translation>Impossible de retourner le résultat de la commande SQL car le serveur a généré une erreur fatale!
Message retourné par le SGBD: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unable to allocate the result of the SQL command because the command passed was an empty SQL!</source>
        <translation>Impossible de retourner le résultat de la commande SQL car cette dernière était vide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a column of tuple with invalid index!</source>
        <translation>Référence à une colonne de tuple dont l&apos;index est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a column of tuple with invalid name!</source>
        <translation>Référence à une colonne de tuple dont le nom est invalide!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Reference to a tuple with index invalid or the result is empty (no tuples)!</source>
        <translation>Référence à un tuple avec un index invalide ou le résultat est vide (pas de tuple)!</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Could not execute the SQL command.
 Message returned: %1</source>
        <translation>Ne peut exécuter la commande SQL.
 Message retourné: %1</translation>
    </message>
</context>
<context>
    <name>ExtensionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/extensionwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Version:</source>
        <translation>Version:</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Old Version:</source>
        <translation>Ancienne version:</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Handles data type</source>
        <translation>Prednre en charge les types de donnée</translation>
    </message>
</context>
<context>
    <name>FunctionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/functionwidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+125"/>
        <source>Langague:</source>
        <translation>Langage:</translation>
    </message>
    <message>
        <location line="-68"/>
        <source>Function Type:</source>
        <translation>Type de fonction:</translation>
    </message>
    <message>
        <location line="+194"/>
        <source>Window Function:</source>
        <translation>Fonction window:</translation>
    </message>
    <message>
        <location line="+76"/>
        <source>Execution Cost:</source>
        <translation>Coût d&apos;exécution:</translation>
    </message>
    <message>
        <location line="-231"/>
        <source>Rows Returned:</source>
        <translation>Lignes retournées:</translation>
    </message>
    <message>
        <location line="+177"/>
        <source>Behavior:</source>
        <translation>Comportement:</translation>
    </message>
    <message>
        <location line="-248"/>
        <source>Security:</source>
        <translation>Sécurité:</translation>
    </message>
    <message>
        <location line="+135"/>
        <source>Return Method:</source>
        <translation>Type de retour:</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Simple</source>
        <translation>Simple</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Set</source>
        <translation>Multiple</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Table</source>
        <translation>Table</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Return Table</source>
        <translation>Table de retour</translation>
    </message>
    <message>
        <location line="+126"/>
        <source>Parameters</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Definition</source>
        <translation>Définition</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Dynamic Library:</source>
        <translation>Bibliotèque dynamique:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Symbol:</source>
        <translation>Symbole:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Library:</source>
        <translation>Bibliotèque:</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>Source code:</source>
        <translation>Code source:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/functionwidget.cpp" line="+45"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+8"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>IN/OUT</source>
        <translation>IN/OUT</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Default Value</source>
        <translation>Valeur par défaut</translation>
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
        <location line="+697"/>
        <source>Printing Options:</source>
        <translation>Options d&apos;impression:</translation>
    </message>
    <message>
        <location line="-290"/>
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
        <location line="+201"/>
        <source>Save current session</source>
        <translation>Enregistrer la session courante</translation>
    </message>
    <message>
        <location line="-384"/>
        <source>A0 (841 x 1189 mm)</source>
        <translation>A0 (841 x 1189 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A1 (594 x 841 mm)</source>
        <translation>A1 (594 x 841 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A2 (420 x 594 mm)</source>
        <translation>A2 (420 x 594 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A3 (297 x 420 mm)</source>
        <translation>A3 (297 x 420 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A4 (210 x 297 mm)</source>
        <translation>A4 (210 x 297 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A5 (148 x 210 mm)</source>
        <translation>A5 (148 x 210 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A6 (105 x 148 mm)</source>
        <translation>A6 (105 x 148 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A7 (74 x 105 mm)</source>
        <translation>A7 (74 x 105 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A8 (52 x 74 mm)</source>
        <translation>A8 (52 x 74 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>A9 (37 x 52 mm)</source>
        <translation>A9 (37 x 52 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B0 (1030 x 1456 mm)</source>
        <translation>B0 (1030 x 1456 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B1 (728 x 1030 mm)</source>
        <translation>B1 (728 x 1030 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B10 (32 x 45 mm)</source>
        <translation>B10 (32 x 45 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B2 (515 x 728 mm)</source>
        <translation>B2 (515 x 728 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B3 (364 x 515 mm)</source>
        <translation>B3 (364 x 515 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B4 (257 x 364 mm)</source>
        <translation>B4 (257 x 364 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B5 (182 x 257 mm)</source>
        <translation>B5 (182 x 257 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B6 (128 x 182 mm)</source>
        <translation>B6 (128 x 182 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B7 (91 x 128 mm)</source>
        <translation>B7 (91 x 128 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B8 (64 x 91 mm)</source>
        <translation>B8 (64 x 91 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>B9 (45 x 64 mm)</source>
        <translation>B9 (45 x 64 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>C5E (163 x 229 mm)</source>
        <translation>C5E (163 x 229 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Comm10E (105 x 241 mm)</source>
        <translation>Comm10E (105 x 241 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>DLE (110 x 220 mm)</source>
        <translation>DLE (110 x 220 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Executive (191 x 254 mm)</source>
        <translation>Executive (191 x 254 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Folio (210 x 330 mm)</source>
        <translation>Folio (210 x 330 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Ledger (432 x 279 mm)</source>
        <translation>Ledger (432 x 279 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Legal (216 x 356 mm)</source>
        <translation>Legal (216 x 356 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Letter (216 x 279 mm)</source>
        <translation>Letter (216 x 279 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Tabloid (279 x 432 mm)</source>
        <translation>Tabloid (279 x 432 mm)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Custom (Based on margins)</source>
        <translation>Personnalisé (Selon les marges)</translation>
    </message>
    <message>
        <location line="+334"/>
        <source>Hide relationship name</source>
        <translation>Cacher les noms des relations</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Hide table extended attributes</source>
        <translation>Cacher les attributs étendus des tables</translation>
    </message>
    <message>
        <location line="-527"/>
        <source>Paper:</source>
        <translation>Papier:</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Orientation:</source>
        <translation>Orientation:</translation>
    </message>
    <message>
        <location line="+356"/>
        <source>Margins:</source>
        <translation>Marges:</translation>
    </message>
    <message>
        <location line="-468"/>
        <source>Autosave interval:</source>
        <translation>Interval d&apos;auto-enregistrement:</translation>
    </message>
    <message>
        <location line="+612"/>
        <source>Grid size:</source>
        <translation>Taille de la grille:</translation>
    </message>
    <message>
        <location line="-112"/>
        <source>Operation history:</source>
        <translation>Historique des opérations:</translation>
    </message>
    <message>
        <location line="-487"/>
        <source>Print grid</source>
        <translation>Imprimer la grille</translation>
    </message>
    <message>
        <location line="+331"/>
        <source>Left:</source>
        <translation>Gauche:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Left margin</source>
        <translation>Marge gauche</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Top:</source>
        <translation>Haut:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Top margin</source>
        <translation>Marge haute</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Right:</source>
        <translation>Droite:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Right margin</source>
        <translation>Marge droite</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Bottom:</source>
        <translation>Bas:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Bottom margin</source>
        <translation>Marge basse</translation>
    </message>
    <message>
        <location line="-536"/>
        <source>Landscape</source>
        <translation>Paysage</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Portrait</source>
        <translation>Portrait</translation>
    </message>
    <message>
        <location line="+653"/>
        <source>Print page numbers</source>
        <translation>Imprimer les numéros de page</translation>
    </message>
    <message>
        <location line="-604"/>
        <source>minute(s)</source>
        <translation>minute(s)</translation>
    </message>
    <message>
        <location line="+589"/>
        <source>items</source>
        <translation>entrées</translation>
    </message>
    <message>
        <location line="-484"/>
        <source>pixels</source>
        <translation>pixels</translation>
    </message>
</context>
<context>
    <name>IndexWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/indexwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Indexing:</source>
        <translation>Indexation:</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Fill Factor:</source>
        <translation>Taux de remplissage:</translation>
    </message>
    <message>
        <location line="+59"/>
        <source>Options:</source>
        <translation>Options:</translation>
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
        <location line="+27"/>
        <source>Conditional Expr.:</source>
        <translation>Expr. conditionnelle:</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>Elements</source>
        <translation>Élements</translation>
    </message>
</context>
<context>
    <name>LanguageWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/languagewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Trusted:</source>
        <translation>De confiance:</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Handler Function:</source>
        <translation>Fonction de management:</translation>
    </message>
    <message>
        <location line="-19"/>
        <source>Validator Function:</source>
        <translation>Fonction de validation:</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Inline Function:</source>
        <translation>Fonction Inline:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/languagewidget.cpp" line="+45"/>
        <source>The functions to be assigned to the language should have, respectively, the following signatures:&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Handler Function:&lt;/strong&gt; &lt;em&gt;language_handler function()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Validator Function:&lt;/strong&gt; &lt;em&gt;void function(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Inline Function:&lt;/strong&gt; &lt;em&gt;void function(internal)&lt;/em&gt;</source>
        <translation>Les fonctions attribuées au langage doivent avoir espectivement les signatures suivantes:&lt;br/&gt;&lt;br/&gt;  &lt;strong&gt;Fonction de management:&lt;/strong&gt; &lt;em&gt;language_handler fonction()&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Fonction de validation:&lt;/strong&gt; &lt;em&gt;void fonction(oid)&lt;/em&gt;&lt;br/&gt;  &lt;strong&gt;Fonction Inline:&lt;/strong&gt; &lt;em&gt;void fonction(internal)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/mainwindow.ui" line="+26"/>
        <source>pgModeler - PostgreSQL Database Modeler</source>
        <translation>pgModeler - PostgreSQL Database Modeler</translation>
    </message>
    <message>
        <location line="+436"/>
        <source>&amp;File</source>
        <translation>&amp;Fichier</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Edit</source>
        <translation>Édit&amp;er</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>&amp;Help</source>
        <translation>A&amp;ide</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>&amp;Show</source>
        <translation>&amp;Afficher</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>&amp;Plugins</source>
        <translation>E&amp;xtensions</translation>
    </message>
    <message>
        <location line="+513"/>
        <source>Plugins</source>
        <translation>Extensions</translation>
    </message>
    <message>
        <location line="-397"/>
        <location line="+379"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <source>Objects</source>
        <translation type="obsolete">Objets</translation>
    </message>
    <message>
        <source>Validation</source>
        <translation type="obsolete">Validation</translation>
    </message>
    <message>
        <source>Operations</source>
        <translation type="obsolete">Opérations</translation>
    </message>
    <message>
        <location line="-472"/>
        <source>Controls</source>
        <translation>Contrôles</translation>
    </message>
    <message>
        <source>Model Editing</source>
        <translation type="obsolete">Édition de modèle</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="+43"/>
        <source>Ctrl+N</source>
        <translation>Ctrl+N</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Save</source>
        <translation>Enregistrer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+S</source>
        <translation>Ctrl+S</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+3"/>
        <source>Zoom in</source>
        <translation>Zoom +</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+=</source>
        <translation>Ctrl+=</translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+6"/>
        <source>Zoom out</source>
        <translation>Zoom -</translation>
    </message>
    <message>
        <location line="-3"/>
        <source>Zoom -</source>
        <translation>Zoom -</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Ctrl+-</source>
        <translation>Ctrl+-</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Load</source>
        <translation>Ouvrir</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+O</source>
        <translation>Ctrl+O</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+3"/>
        <source>Next</source>
        <translation>Suivant</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Right</source>
        <translation>Ctrl+Right</translation>
    </message>
    <message>
        <location line="+12"/>
        <location line="+3"/>
        <source>Previous</source>
        <translation>Précédent</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Left</source>
        <translation>Ctrl+Left</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Save as</source>
        <translation>Enregistrer sous</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Exit</source>
        <translation>Quitter</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Q</source>
        <translation>Ctrl+Q</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>About pgModeler</source>
        <translation>À propos de pgModeler</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>F3</source>
        <translation>F3</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>Close</source>
        <translation>Fermer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Close current model</source>
        <translation>Fermer le modèle courant</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Ctrl+H</source>
        <translation>Ctrl+H</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Settings</source>
        <translation>Paramètres</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>F10</source>
        <translation>F10</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Wiki</source>
        <translation>Wiki</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>CSS</source>
        <translation>CSS</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Access the list of loaded plugins</source>
        <translation>Accéder a la liste des extensions chargées</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Recent Models</source>
        <translation>Modèles récents</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Load recently opened model</source>
        <translation>Charger des modèles ouverts récemment</translation>
    </message>
    <message>
        <location line="-42"/>
        <source>F1</source>
        <translation>F1</translation>
    </message>
    <message>
        <location line="-885"/>
        <source>O&amp;bjects</source>
        <translation>O&amp;bjets</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+B</source>
        <translation>Alt+B</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Validation</source>
        <translation>&amp;Validation</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+V</source>
        <translation>Alt+V</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>&amp;Operations</source>
        <translation>&amp;Opérations</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Alt+O</source>
        <translation>Alt+O</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Find Object</source>
        <translation>Chercher un objet</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Ctrl+F</source>
        <translation>Ctrl+F</translation>
    </message>
    <message>
        <location line="+571"/>
        <source>Print</source>
        <translation>Imprimer</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+P</source>
        <translation>Ctrl+P</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Undo</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Z</source>
        <translation>Ctrl+Z</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Redo</source>
        <translation>Rétablir</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Y</source>
        <translation>Ctrl+Y</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Export</source>
        <translation>Exporter</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+Shift+S</source>
        <translation>Ctrl+Shift+S</translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+3"/>
        <source>Show grid</source>
        <translation>Afficher la grille</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+G</source>
        <translation>Ctrl+G</translation>
    </message>
    <message>
        <source>Close model</source>
        <translation type="obsolete">Fermer</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Ctrl+W</source>
        <translation>Ctrl+W</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Normal zoom</source>
        <translation>Zoom normal</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+0</source>
        <translation>Ctrl+0</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Align to grid</source>
        <translation>Aligner sur la grille</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Align objects position to grid</source>
        <translation>Aligner les objets sur la grille</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Show delimiters</source>
        <translation>Afficher les délimiteurs</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Show the page delimiters</source>
        <translation>Afficher les délimiteurs de page</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Ctrl+L</source>
        <translation>Ctrl+L</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Widgets</source>
        <translation>Widgets</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Save all</source>
        <translation>Tout enregistrer</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Overview</source>
        <translation>Vue d&apos;ensemble</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Show the model overview</source>
        <translation>Afficher la vue d&apos;ensemble du modèle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/mainwindow.cpp" line="+454"/>
        <source>Save all models</source>
        <translation>Enregistrer tous les modèles</translation>
    </message>
    <message>
        <source>Some models were modified! Do you want to save them before finish the pgModeler?</source>
        <translation type="obsolete">Certains modèles ont été modifiés! Voulez vous les sauvegarder avant de quitter pgModeler?</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Some models were modified! Do you want to save them before exit pgModeler?</source>
        <translation>Certains modèles ont été modifiés! Voulez vous les sauvegarder avant de sortir de pgModeler?</translation>
    </message>
    <message>
        <location line="+87"/>
        <source>Clear Menu</source>
        <translation>Netoyer le menu</translation>
    </message>
    <message>
        <location line="+256"/>
        <source>Save model</source>
        <translation>Enregistrer le modèle</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>The model were modified! Do you want to save it before close?</source>
        <translation>Le modèle a été modifié! Souhaitez vous le sauvegarder avant de le fermer?</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>Model validation recommended</source>
        <translation>La validation du modèle est recommandé</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>ATTENTION: The model &apos;%1&apos; is invalidated and its recommended that you revalidate it before save! Saving an invalidated model can generate a corrupted model file! Do you really want to proceed with the saving?</source>
        <translation>ATTENTION: Le modèle &apos;%1&apos; a été invalidé et il est recommandé de le revalider avant de le sauver. Sauvegarder un modèle non validé peut engendrer un fichier corrompu! Voulez vous vraiement continuer et sauvegarder?</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Save &apos;%1&apos; as...</source>
        <translation>Enregistrer &apos;%1&apos; sous...</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+99"/>
        <source>Database model (*.dbm);;All files (*.*)</source>
        <translation>Modèle de base de donnée (*.dbm);; Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="-55"/>
        <source>Database model printing</source>
        <translation>Impression du modèle de base de données</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Confirmation</source>
        <translation>Confirmation</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Changes were detected in the definitions of paper/margin of the model which may cause the incorrect print of the objects. Do you want to continue printing using the new settings? To use the default settings click &apos;No&apos; or &apos;Cancel&apos; to abort printing.</source>
        <translation>Des modifications ont été détectées dans les définitions du papier ou des marges du modèle ce qui pourrait causer une mauvaise impression des objets. Souhaitez vous continuer l&apos;impression aves les nouveaux paramètres? Pour utiliser les paramètres par défaut cliquez sur &apos;Non&apos; ou &apos;Annuler&apos; pour interrompre l&apos;impression.</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Load model</source>
        <translation>Charger un modèle</translation>
    </message>
    <message>
        <location line="+120"/>
        <source>Open Wiki pages</source>
        <translation>Accèder au Wiki</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>This action will open a web browser window! Want to proceed?</source>
        <translation>Cette action ouvrira votre navigateur web! Souhaitez vous continuer?</translation>
    </message>
</context>
<context>
    <name>MessageBox</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/messagebox.ui" line="+29"/>
        <source>Dialog</source>
        <translation>Dialogue</translation>
    </message>
    <message>
        <location line="+67"/>
        <source>Show excpetion stack in text format (useful to report errors).</source>
        <translation>Afficher la pile d&apos;exception au format texte (utile pour les rapports d&apos;erreur).</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Show/hide exceptions stack.</source>
        <translation>Afficher/cacher la pile d&apos;exceptions.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="+113"/>
        <source>msg</source>
        <translation>message</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>Exceptions</source>
        <translation>Exceptions</translation>
    </message>
    <message>
        <location line="+71"/>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="+187"/>
        <location line="+11"/>
        <source>&amp;Yes</source>
        <translation>&amp;Oui</translation>
    </message>
    <message>
        <location line="+35"/>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="-12"/>
        <location line="+11"/>
        <source>&amp;No</source>
        <translation>&amp;Non</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/messagebox.cpp" line="-42"/>
        <source>Error</source>
        <translation>Erreur</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Alert</source>
        <translation>Alerte</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Information</source>
        <translation>Information</translation>
    </message>
    <message>
        <location line="+28"/>
        <location line="+5"/>
        <source>&amp;Cancel</source>
        <translation>&amp;Annuler</translation>
    </message>
    <message>
        <location line="-4"/>
        <location line="+11"/>
        <source>&amp;Ok</source>
        <translation>&amp;Ok</translation>
    </message>
</context>
<context>
    <name>ModelExportForm</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelexportform.ui" line="+35"/>
        <source>Model Export</source>
        <translation>Export du modèle</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Select the export type:</source>
        <translation>Sélectionner le type d&apos;exportation:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Export to SQL file:</source>
        <translation>Exporter dans un fichier SQL:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>File:</source>
        <translation>Fichier:</translation>
    </message>
    <message>
        <location line="+25"/>
        <location line="+309"/>
        <source>Select target file</source>
        <translation>Sélectionner le fichier cible</translation>
    </message>
    <message>
        <location line="-306"/>
        <location line="+309"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-239"/>
        <location line="+508"/>
        <source>PostgreSQL:</source>
        <translation>PostgreSQL:</translation>
    </message>
    <message>
        <location line="-484"/>
        <location line="+518"/>
        <source>PostgreSQL version in which the SQL code should be generated</source>
        <translation>Version de PostgreSQL pour laquelle le code SQL doit être généré</translation>
    </message>
    <message>
        <location line="-412"/>
        <source>Check if the system user has write permission on target folder.</source>
        <translation>Vérifiez que vous avez les droits d&apos;écriture pour le dossier de destination.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Export to PNG image:</source>
        <translation>Exporter en une image PNG:</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Image:</source>
        <translation>Image:</translation>
    </message>
    <message>
        <location line="+79"/>
        <source>Options:</source>
        <translation>Options:</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>Show grid</source>
        <translation>Afficher la grille</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Show the page delimiters</source>
        <translation>Afficher les délimiteurs de page</translation>
    </message>
    <message>
        <location line="+96"/>
        <source>Check if the system user has write permission on image target folder.</source>
        <translation>Vérifiez que vous avez les droits d&apos;écriture pour le dossier de destination.</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>Connection:</source>
        <translation>Connexion:</translation>
    </message>
    <message>
        <location line="+204"/>
        <source>It is recommended to select this option only when the version of the DBMS, somehow, is not identifiable or if you need to generate a specific version of SQL code for testing.</source>
        <translation>Il est recoomandé de sélectionner cette option seulement lorsque la version du SGBD n&apos;est pas indentifiable ou si vous avez besoin de générer une version spécifique du code SQL à des fins de tests.</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Ignore object duplicity</source>
        <translation>Ignorer les objets dupliqués</translation>
    </message>
    <message>
        <location line="+74"/>
        <source>The pgModeler ignores errors generated by duplicate objects and creates only model objects that do not exist in the database. This option may be used when an object was created after a later model export.</source>
        <translation>pgModeler ignore les erreurs générées par les objets dupliqués et créé seulement les objets du modèle qui n&apos;existent pas dans la base de données. Cette option peut être utilisée quand un objet a été créé après un export du modèle.</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Export to DBMS:</source>
        <translation>Exporter vers le SGBD:</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Progress label...</source>
        <translation>Indicateur de progression...</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>ico</source>
        <translation></translation>
    </message>
    <message>
        <location line="+67"/>
        <source>&amp;Export</source>
        <translation>&amp;Exporter</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&amp;Close</source>
        <translation>&amp;Fermer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexportform.cpp" line="+113"/>
        <source>Initializing model export...</source>
        <translation>Initialisation de l&apos;exportation du modèle...</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Saving file &apos;%1&apos;</source>
        <translation>Enregistrement du fichier &apos;%1&apos;</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Exporting process sucessfuly ended!</source>
        <translation>Processus d&apos;exportation terminé avec succès!</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Error on export!</source>
        <translation>Erreur lors de l&apos;exportation!</translation>
    </message>
    <message>
        <location line="+58"/>
        <source>Export model as...</source>
        <translation>Exporter le modèle sous...</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>SQL code (*.sql);;All files (*.*)</source>
        <translation>Code SQL (*.sql);;Tous les fichiers (*.*)</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>PNG image (*.png);;All files (*.*)</source>
        <translation>Image PNG (*.png);;Tous les fichiers (*.*)</translation>
    </message>
</context>
<context>
    <name>ModelExportHelper</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelexporthelper.cpp" line="+21"/>
        <source>PostgreSQL %1 version code generation...</source>
        <translation>Génération de code pour PostgreSQL version %1...</translation>
    </message>
    <message>
        <location line="+106"/>
        <source>PostgreSQL version detection overrided. Using version %1...</source>
        <translation>Détection de la version de PostgreSQL surchargé. Utilisation de la version %1...</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>PostgreSQL %1 server detected...</source>
        <translation>Serveur PostgreSQL %1 détecté...</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Ignoring object duplication error...</source>
        <translation>Ignore les erreur d&apos;objets dupliqués...</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Creating object &apos;%1&apos; (%2)...</source>
        <translation>Création de l&apos;objet &apos;%1&apos; (%2)...</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Creating database &apos;%1&apos;...</source>
        <translation>Création de la base de données &apos;%1&apos;...</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Connecting to database &apos;%1&apos;...</source>
        <translation>Connexion à la base de données &apos;%1&apos;...</translation>
    </message>
    <message>
        <location line="+5"/>
        <location line="+39"/>
        <source>Creating objects on database &apos;%1&apos;...</source>
        <translation>Création des objets dans la base de données &apos;%1&apos;...</translation>
    </message>
</context>
<context>
    <name>ModelObjectsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelobjectswidget.ui" line="+35"/>
        <location line="+39"/>
        <source>Model Objects</source>
        <translation>Objets du modèle</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location line="+70"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Visible Object Types</source>
        <translation>Afficher les objets des types suivants</translation>
    </message>
    <message>
        <location line="+61"/>
        <source>Select All</source>
        <translation>Tout sélect.</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Clear All</source>
        <translation>Tout effacer</translation>
    </message>
    <message>
        <location line="+56"/>
        <source>Select</source>
        <translation>Sélectionner</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
    <message>
        <location line="+108"/>
        <source>Objects view configuration</source>
        <translation>Filtres d&apos;affichage des objets</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Expands all items</source>
        <translation>Ouvrir tout les éléments</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>Collapses all items</source>
        <translation>Fermer tout les éléments</translation>
    </message>
    <message>
        <location line="-619"/>
        <location line="+476"/>
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
        <location filename="../libpgmodeler_ui/src/modelobjectswidget.cpp" line="+129"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
</context>
<context>
    <name>ModelOverviewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modeloverviewwidget.ui" line="+41"/>
        <source>Model overview</source>
        <translation>Vue générale du modèle</translation>
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
        <location line="+55"/>
        <source>&amp;Restore</source>
        <translation>&amp;Restaurer</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>&amp;Cancel</source>
        <translation>A&amp;nnuler</translation>
    </message>
    <message>
        <location line="+107"/>
        <source>pgModeler was not closed properly in a previous execution and some models were still being edited. Click &apos;Restore&apos; to reopen the models or &apos;Cancel&apos; to abort the restoration.</source>
        <translation>pgModeler n&apos;a pas été fermé correctement lors de sa précédente exécution, certains modèles étaient toujours en cours d&apos;édition. Cliquez sur &apos;Restaurer&apos; pour réouvrir les modèles ou &apos;Annuler&apos; pour interrompre la restauration.</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelrestorationform.cpp" line="+49"/>
        <source>Modified: %1</source>
        <translation>Modifié: %1</translation>
    </message>
    <message>
        <location line="+0"/>
        <source>yyyy-MM-dd hh:mm:ss</source>
        <translation>yyyy-MMM-dd hh:mm</translation>
    </message>
</context>
<context>
    <name>ModelValidationWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/modelvalidationwidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+76"/>
        <source>Warnings</source>
        <translation>Avertissements</translation>
    </message>
    <message>
        <location line="+25"/>
        <location line="+66"/>
        <source>0</source>
        <translation>0</translation>
    </message>
    <message>
        <location line="-28"/>
        <location line="+25"/>
        <source>Errors</source>
        <translation>Erreurs</translation>
    </message>
    <message>
        <location line="+72"/>
        <source>Vali&amp;date</source>
        <translation>Vali&amp;der</translation>
    </message>
    <message>
        <location line="+31"/>
        <location line="+176"/>
        <source>Try to apply a fix on the selected validation info.</source>
        <translation>Essayer d&apos;appliquer le correctif sur l’élément sélectionné.</translation>
    </message>
    <message>
        <location line="-145"/>
        <source>Clear validation results</source>
        <translation>Effacer le résultat de la validation</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Clear</source>
        <translation>Éffacer</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Enable validation of SQL code on DBMS (requires connection)</source>
        <translation>Activer la validation SQL sur le serveur (requiert une connexion)</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>PostgreSQL:</source>
        <translation>PostgreSQL:</translation>
    </message>
    <message>
        <location line="+47"/>
        <source>SQL Validation</source>
        <translation>Validation SQL</translation>
    </message>
    <message>
        <source>SQL validation</source>
        <translation type="obsolete">Validation SQL</translation>
    </message>
    <message>
        <location line="-88"/>
        <source>Connection:</source>
        <translation>Connexion:</translation>
    </message>
    <message>
        <location line="-88"/>
        <source>Apply Fix</source>
        <translation>Appliquer le correctif</translation>
    </message>
    <message>
        <source>Validate</source>
        <translation type="obsolete">Valider</translation>
    </message>
    <message>
        <location line="-246"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/modelvalidationwidget.cpp" line="+95"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] is being referenced by &lt;strong&gt;%4&lt;/strong&gt; object(s) before its creation.</source>
        <translation>L&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] est référencé par &lt;strong&gt;%4&lt;/strong&gt; objet(s) avant sa création.</translation>
    </message>
    <message>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] has a name that conflicts with &lt;strong&gt;%4&lt;/strong&gt; object&apos;s name(s).</source>
        <translation type="obsolete">L&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3] a un nom en conflit avec &lt;strong&gt;%4&lt;/strong&gt; nom(s) d&apos;objet(s).</translation>
    </message>
    <message>
        <source>Validation failed to execute the DDL command below:</source>
        <translation type="obsolete">La validation n&apos;a pas pu exécuter la DDL suivante:</translation>
    </message>
    <message>
        <location line="-52"/>
        <source>Autodetect</source>
        <translation>Auto détection</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>The object &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; has a name that conflicts with &lt;strong&gt;%3&lt;/strong&gt; object&apos;s name(s).</source>
        <translation>Le nom de l&apos;objet &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; entre en conflit avec l&apos;objet&lt;strong&gt;%3&lt;/strong&gt;.</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Validation failed to execute the SQL command below.</source>
        <translation>La validation a échouée lors de l&apos;exécution de la commande SQL suivante.</translation>
    </message>
    <message>
        <location line="+45"/>
        <source>Conflicting object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt;.</source>
        <translation>Objet en conflit: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; </translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Referrer object: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</source>
        <translatorcomment>Seconde lecture ?</translatorcomment>
        <translation type="unfinished">Objet réferent: &lt;strong&gt;%1&lt;/strong&gt; &lt;em&gt;(%2)&lt;/em&gt; [id: %3].</translation>
    </message>
    <message>
        <location line="+99"/>
        <source>The validation process failed due to external errors! You can restart the validation by clicking &lt;strong&gt;Validate&lt;/strong&gt; again.</source>
        <translation>Le processus de validation a échoué a cause d&apos;une erreur extérieur! Vous pouvez relancer la validation en cliquant &lt;strong&gt;Valider&lt;/strong&gt; de nouveau.</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>The validation process ended up without any issues.</source>
        <translation>Le procéssus de validation s&apos;est terminé avec success.</translation>
    </message>
</context>
<context>
    <name>ModelWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/src/modelwidget.cpp" line="+97"/>
        <location line="+105"/>
        <source>Copy</source>
        <translation>Copie</translation>
    </message>
    <message>
        <location line="-105"/>
        <source>Generalization</source>
        <translation>Généralisation</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>&lt;strong&gt;CAUTION:&lt;/strong&gt; The object model is protected! New objects will be inserted only when the protection is removed!</source>
        <translation>&lt;strong&gt;ATTENTION:&lt;/strong&gt; Le modèle est verrouillé! Les nouveaux objets seront insérés lorsque le vérrou sera enlevé!</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Convert</source>
        <translation>Convertir</translation>
    </message>
    <message>
        <source>Depend</source>
        <translation type="obsolete">Dépendance(s)</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>New</source>
        <translation>Nouveau</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Quick</source>
        <translation>Act. rapides</translation>
    </message>
    <message>
        <location line="+2461"/>
        <source>Source code</source>
        <translation>Code source</translation>
    </message>
    <message>
        <location line="-2501"/>
        <source>Show object source code</source>
        <translation>Afficher le code source de l&apos;objet</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Source</source>
        <translation>Source</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+2492"/>
        <source>Properties</source>
        <translation>Propriétés</translation>
    </message>
    <message>
        <location line="-2490"/>
        <source>Edit the object properties</source>
        <translation>Editer les propriétés de l&apos;objet</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2517"/>
        <source>Protect</source>
        <translation>Vérrouiller</translation>
    </message>
    <message>
        <location line="-2516"/>
        <location line="+2511"/>
        <source>Unprotect</source>
        <translation>Déverrouiller</translation>
    </message>
    <message>
        <location line="-2510"/>
        <source>Protects object(s) from modifications</source>
        <translation>Empêcher l(es)&apos;objet(s) d&apos;être modifiés</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Unprotect objetc(s) from modifications</source>
        <translation>Libérer l(es)&apos;objet(s) afin d&apos;être modifiés</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+2519"/>
        <source>Delete</source>
        <translation>Supprimer</translation>
    </message>
    <message>
        <location line="-2517"/>
        <source>Delete the selected object(s)</source>
        <translation>Supprimer les objets sélectionnés</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Select all</source>
        <translation>Tout sélect.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Selects all the graphical objects in the model</source>
        <translation>Sélectionner tous les objets graphiques du modèle</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Converts the n-n relationship into table</source>
        <translation>Convertir la relation n-n en table</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Copy the selected objects(s)</source>
        <translation>Copier le(s) objet(s) sélectionné(s)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Paste</source>
        <translation>Coller</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Paste the previous copied object(s)</source>
        <translation>Coller l&apos;(es) objet(s) copié(s)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Cut</source>
        <translation>Couper</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Cut the selected object(s)</source>
        <translation>Couper l&apos;(es) objets(s) selectionnés(s)</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Deps &amp;&amp; Referrers</source>
        <translation>Dép. &amp;&amp; Rèf.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Displays objects that reference and that are dependent of the selected object</source>
        <translation>Afficher les objets qui référencent et/ou qui sont dépendants des objets sélectionnés</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Add a new object in the model</source>
        <translation>Ajouter un nouvel objet dans le modèle</translation>
    </message>
    <message>
        <location line="+5"/>
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
        <location line="+1"/>
        <source>Move the object to another schema</source>
        <translation>Déplacer l&apos;objet vers un autre schéma</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Edit permissions</source>
        <translation>Editer les permissions</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Edit object&apos;s permissions</source>
        <translation>Editer les permissions de l&apos;objet</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Change owner</source>
        <translation>Changer le propriétaire</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Change object&apos;s owner</source>
        <translation>Changer le propriétaire de l&apos;objet</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Select children</source>
        <translation>Sélectionnez les éléments</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Selects all the children graphical objects on the selected schema</source>
        <translation>Sélectionne tout les éléments graphiques du schéma actuellement sélectionné</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Highlight</source>
        <translation>Mettre en avant</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Clears the current selection and centers the model view on the selected object.</source>
        <translation>Efface la sélection courante et centre la vue du modèle sur l&apos;objet sélectionné</translation>
    </message>
    <message>
        <location line="+512"/>
        <location line="+956"/>
        <location line="+417"/>
        <location line="+7"/>
        <location line="+4"/>
        <source>Confirmation</source>
        <translation>Confirmation</translation>
    </message>
    <message>
        <location line="-1383"/>
        <source>Do you really want to convert the relationship?</source>
        <translation>Souhaitez vous réellement convertir la relation?</translation>
    </message>
    <message>
        <location line="+131"/>
        <source>Loading database model</source>
        <translation>Ouverture du modèle de base de données</translation>
    </message>
    <message>
        <location line="+231"/>
        <source>Saving database model</source>
        <translation>Enregistrement du modèle de base de données</translation>
    </message>
    <message>
        <location line="+594"/>
        <source>Also copy all dependencies of selected objects? This minimizes the breakdown of references when copied objects are pasted into another model.</source>
        <translation>Copier également les dépendences des objets sélectionnés? Ceci minimise la perte des références lorsque les objets copiés sont collés dans un autre modèle.</translation>
    </message>
    <message>
        <location line="+104"/>
        <source>Pasting objects...</source>
        <translation>Collage des objets...</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Validating object: %1 (%2)</source>
        <translation>Validation de l&apos;objet: %1 (%2)</translation>
    </message>
    <message>
        <location line="+105"/>
        <source>Generating XML code of object: %1 (%2)</source>
        <translation>Génération du code XML de l&apos;objet : %1 (%2)</translation>
    </message>
    <message>
        <location line="+73"/>
        <source>Pasting object: %1 (%2)</source>
        <translation>Collage de l&apos;objet: %1 (%2)</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Not all objects were pasted to the model due to errors returned during the process! Refer to error stack for more details!</source>
        <translation>Tous les objets n&apos;ont pas été collés dans le modèle à cause d&apos;erreurs retournés durant le processus! Se référer à la pile d&apos;erreurs pour plus de détails!</translation>
    </message>
    <message>
        <location line="+60"/>
        <source>CAUTION: Remove multiple objects at once can cause irreversible invalidations to other objects in the model. Such invalid objects will be deleted too. Do you really want to delete ALL selected objects?</source>
        <translation>ATTENTION: Supprimer plusieurs objets d&apos;un coup peut causer des invalidations irréversibles à d&apos;autres objets du modèle. Ces derniers seront aussi supprimés. Voulez vous vraiment supprimer TOUT les objets sélectionnés?</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>CAUTION: Remove a relationship can cause irreversible invalidations to other objects in the model. Such invalid objects will be deleted too. Do you really want to delete the relationship?</source>
        <translation>ATTENTION: Supprimer une relation peut causer des invalidations irréversibles à d&apos;autres objets du modèle. Ces derniers seront aussi supprimés. Voulez vous vraiment supprimer la relation sélectionnée?</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Do you really want to delete the selected object?</source>
        <translation>Souhaitez vous réellement supprimer l&apos;objet sélectionné?</translation>
    </message>
    <message>
        <location line="+263"/>
        <source>(no objects)</source>
        <translation>(aucun objet)</translation>
    </message>
    <message>
        <location line="+319"/>
        <source>Constraints</source>
        <translation>Contraintes</translation>
    </message>
</context>
<context>
    <name>ObjectDepsRefsWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectdepsrefswidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Dependencies</source>
        <translation>Dépendances</translation>
    </message>
    <message>
        <location line="+75"/>
        <location line="+109"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="-100"/>
        <location line="+109"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="-100"/>
        <location line="+109"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location line="-100"/>
        <location line="+109"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location line="-96"/>
        <source>References</source>
        <translation>Références</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectdepsrefswidget.cpp" line="+26"/>
        <source>Dependences / References of Object</source>
        <translation>Dépendances et références d&apos;objets</translation>
    </message>
</context>
<context>
    <name>ObjectFinderWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectfinderwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Pattern:</source>
        <translation>Motif:</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Hide this widget</source>
        <translation>Cacher ce widget</translation>
    </message>
    <message>
        <location line="+3"/>
        <location line="+70"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location line="-48"/>
        <source>Highlight graphical objects when selecting them on results</source>
        <translation>Mettre en évidence l’élément graphique lors de sa sélection dans les résultats de la recherche</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Highlight</source>
        <translation>Mettre en avant</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Find</source>
        <translation>Chercher</translation>
    </message>
    <message>
        <location line="+31"/>
        <source>Clears the search results</source>
        <translation>Éffacer les résultats</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Clear</source>
        <translation>Éffacer</translation>
    </message>
    <message>
        <location line="+48"/>
        <source>Filter:</source>
        <translation>Filtrer:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Regular Expression</source>
        <translation>Expression régulières</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Exact Match</source>
        <translation>Correspondance exacte</translation>
    </message>
    <message>
        <location line="+42"/>
        <source>Select All</source>
        <translation>Tout sélect.</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Clear All</source>
        <translation>Tout effacer</translation>
    </message>
    <message>
        <location line="+71"/>
        <source>Case Sensitive</source>
        <translation>Sensible à la casse</translation>
    </message>
    <message>
        <location line="+71"/>
        <source>Object</source>
        <translation>Objet</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Parent Object</source>
        <translation>Objet parent</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Parent Type</source>
        <translation>Type parent</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Defines the search filter</source>
        <translation>Défini le filtre de recherche</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Filter</source>
        <translation>Filtrer</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/objectfinderwidget.cpp" line="+119"/>
        <source>Found &lt;strong&gt;%1&lt;/strong&gt; object(s).</source>
        <translation>&lt;strong&gt;%1&lt;/strong&gt; objet(s) trouvé(s).</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>No objects found.</source>
        <translation>Aucun objet trouvé.</translation>
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
        <location line="+114"/>
        <source>....</source>
        <translation>....</translation>
    </message>
    <message>
        <location line="+18"/>
        <source>to:</source>
        <translation>en:</translation>
    </message>
    <message>
        <location line="+18"/>
        <location line="+55"/>
        <source>Rename</source>
        <translation>Renommer</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Cancel</source>
        <translation>Annuler</translation>
    </message>
</context>
<context>
    <name>ObjectSelectorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objectselectorwidget.ui" line="+32"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+72"/>
        <source>Clear field</source>
        <translation>Effeacer le champs</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Select Object</source>
        <translation>Sélectionner un objet</translation>
    </message>
</context>
<context>
    <name>ObjectTableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/objecttablewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Add Item</source>
        <translation>Ajouter un élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Ins</source>
        <translation>Ins</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Remove Item</source>
        <translation>Supprimer un élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Del</source>
        <translation>Suppr.</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Update Item</source>
        <translation>Actualiser les éléments</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Alt+R</source>
        <translation>Alt+R</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Remove All</source>
        <translation>Tout supprimer</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Shift+Del</source>
        <translation>Shift+Del</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>Edit Item</source>
        <translation>Editer l&apos;élément</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Space</source>
        <translation>Space</translation>
    </message>
    <message>
        <location line="+22"/>
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
        <location filename="../libpgmodeler_ui/src/objecttablewidget.cpp" line="+366"/>
        <location line="+25"/>
        <source>Confirmação</source>
        <translation>Confirmation</translation>
    </message>
    <message>
        <location line="-25"/>
        <source>Do you really want to remove the selected item?</source>
        <translation>Souhaitez vous réellement supprimer l&apos;élément sélectionné?</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Do you really want to remove the all items?</source>
        <translation>Souhaitez vous réellement supprimer tous les éléments?</translation>
    </message>
</context>
<context>
    <name>OperationList</name>
    <message>
        <location filename="../libpgmodeler/src/operationlist.cpp" line="+546"/>
        <source>Undoing operation on object: %1 (%2)</source>
        <translation>Défaire les opération sur l&apos;objet: %1 (%2)</translation>
    </message>
    <message>
        <location line="+63"/>
        <source>Redoing operation on object:: %1 (%2)</source>
        <translation>Refaire les opération sur l&apos;ojet: %1 (%2)</translation>
    </message>
</context>
<context>
    <name>OperationListWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operationlistwidget.ui" line="+35"/>
        <location line="+39"/>
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
        <location line="+112"/>
        <source>1</source>
        <translation>1</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Operations:</source>
        <translation>Opérations:</translation>
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
        <translation>Position:</translation>
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
        <location filename="../libpgmodeler_ui/src/operationlistwidget.cpp" line="+105"/>
        <source>Object: %1</source>
        <translation>Objet: %1</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Name: %1</source>
        <translation>Nom: %1</translation>
    </message>
    <message>
        <location line="+5"/>
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
        <location line="+6"/>
        <source>Operation: %1</source>
        <translation>Opération: %1</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Undoing operations...</source>
        <translation>Annulation des opérations...</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Redoing operations...</source>
        <translation>Rétablissement des opérations...</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Operation history exclusion</source>
        <translatorcomment>?? À controler dans le contexte ??</translatorcomment>
        <translation>Exclure l&apos;historique des opérations</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Delete the executed operations history is an irreversible action, do you want to continue?</source>
        <translation>Supprimer l&apos;historique des opérations exécutées est une action irréversible, souhaitez vous continuer?</translation>
    </message>
</context>
<context>
    <name>OperatorClassWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorclasswidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Default Class:</source>
        <translation>Classe par défaut:</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Indexing:</source>
        <translation>Indexation:</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Elements</source>
        <translation>Élements</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Element Type:</source>
        <translation>Type d&apos;élement:</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Operator</source>
        <translation>Operateur</translation>
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
        <translation>Fonction:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Operator:</source>
        <translation>Opérateur:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Support/Strategy:</source>
        <translation>Support/Stratégie:</translation>
    </message>
    <message>
        <location line="+14"/>
        <location line="+35"/>
        <source>Op. Family:</source>
        <translation>Famille d&apos;opérateurs:</translation>
    </message>
    <message>
        <location line="-22"/>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="+46"/>
        <source>FOR ORDER BY</source>
        <translation>FOR ORDER BY</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorclasswidget.cpp" line="-11"/>
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
        <translation>Type</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Support/Strategy</source>
        <translation>Support/Stratégie</translation>
    </message>
    <message>
        <location line="+118"/>
        <source>Yes</source>
        <translation>Oui</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>No</source>
        <translation>Non</translation>
    </message>
</context>
<context>
    <name>OperatorFamilyWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorfamilywidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Indexing:</source>
        <translation>Indexation:</translation>
    </message>
</context>
<context>
    <name>OperatorWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/operatorwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Options:</source>
        <translation>Options:</translation>
    </message>
    <message>
        <location line="-20"/>
        <source>MERGES</source>
        <translation>MERGES</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>HASHES</source>
        <translation>HASHES</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Arguments</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Restrict:</source>
        <translation>Restriction:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Commutator</source>
        <translation>Commutateur:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Negator:</source>
        <translation>Négateur:</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Operator Func.:</source>
        <translation>Fonction de l&apos;opérateur:</translation>
    </message>
    <message>
        <location line="-71"/>
        <source>Join:</source>
        <translation>Jointure:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/operatorwidget.cpp" line="+35"/>
        <source>Right Argument Type</source>
        <translation>Type argument à droite</translation>
    </message>
    <message>
        <location line="-2"/>
        <source>Left Argument Type</source>
        <translation>Type d&apos;argument à gauche</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>To create a unary operator it is necessary to specify as &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; one of its arguments. Additionally, the function that defines the operator must have only one parameter and this, in turn, must have the same data type of the the argument of unary operator.</source>
        <translation>Pour créer un opérateur unaire il est nécessaire de spécifier &lt;strong&gt;&lt;em&gt;&apos;any&apos;&lt;/em&gt;&lt;/strong&gt; comme l&apos;un de ses arguments. De plus, la fonction qui définit l&apos;opérateur doit avoir seulement un paramètre et celui-ci doit avoir le même type de donnée que l&apos;argument de l&apos;opérateur unaire.</translation>
    </message>
</context>
<context>
    <name>ParameterWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/parameterwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut:</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Mode:</source>
        <translation>Mode:</translation>
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
</context>
<context>
    <name>PermissionWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/permissionwidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+92"/>
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="+76"/>
        <source>Roles</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>ID:</source>
        <translation>Identifiant:</translation>
    </message>
    <message>
        <location line="+296"/>
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
        <location line="-157"/>
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
        <location line="+69"/>
        <source>Grant</source>
        <translation>Grant</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Revoke</source>
        <translation>Revoke</translation>
    </message>
    <message>
        <location line="+10"/>
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
        <location filename="../libpgmodeler_ui/src/permissionwidget.cpp" line="-30"/>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Id</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Leave the &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; empty to create a permission applicable to &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</source>
        <translation>Laissez le champ &lt;em&gt;&lt;strong&gt;Roles&lt;/strong&gt;&lt;/em&gt; vide pour créer une permission applicable à &lt;strong&gt;&lt;em&gt;PUBLIC&lt;/em&gt;&lt;/strong&gt;.</translation>
    </message>
</context>
<context>
    <name>PgModelerCLI</name>
    <message>
        <location filename="../main-cli/src/pgmodelercli.cpp" line="+81"/>
        <source>Unrecognized option &apos;%1&apos;.</source>
        <translation>Option &apos;%1&apos; non reconnue.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>Value not specified for option &apos;%1&apos;.</source>
        <translation>Valeur pour l&apos;option &apos;%1&apos; manquante.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Option &apos;%1&apos; does not accept values.</source>
        <translation>L&apos;option &apos;%1&apos; n&apos;accepte pas de valeur.</translation>
    </message>
    <message>
        <location line="+38"/>
        <source>Connection aliased as &apos;%1&apos; was not found on configuration file.</source>
        <translation>La connexion nommé &apos;%1&apos; n&apos;existe pas dans le fichier de configuration.</translation>
    </message>
    <message>
        <location line="+91"/>
        <source>Usage: pgmodeler-cli [OPTIONS]</source>
        <translation>Utilisation: pgmodeler-cli [OPTIONS]</translation>
    </message>
    <message>
        <location line="+1"/>
        <location line="+98"/>
        <source> command line interface.</source>
        <translation> interface en ligne de commande</translation>
    </message>
    <message>
        <location line="-97"/>
        <source>PostgreSQL Database Modeler Project - pgmodeler.com.br</source>
        <translation>PostgreSQL Database Modeler Project - pgmodeler.com.br</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Copyright 2006-2013 Raphael A. Silva &lt;rkhaotix@gmail.com&gt;</source>
        <translation>Copyright 2006-2013 Raphael A. Silva &lt;rkhaotix@gmail.com&gt;</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>This tool provides a way to export pgModeler&apos;s database models without
the need to load them on graphical interface. All available exporting
modes are described below.</source>
        <translation>Cet outil fournis un moyen d’exporter un modèle de base de donnée pgModeler sans utiliser l&apos;interface graphique. Les modes d&apos;exportation disponibles sont décris ci-dessous.</translation>
    </message>
    <message>
        <location line="+4"/>
        <source>General options: </source>
        <translation>Options générales:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[FILE]		 Input model file (.dbm).</source>
        <translation>   %1, %2=[FICHIER]		 Modèle source (.dbm).</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[FILE]		 Output file. Available only on export to file or png.</source>
        <translation>   %1, %2=[FICHIER]		 Fichier de sortie. Disponnible uniquement pour les exports SQL fichier et PNG.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2		 Export to a sql script file.</source>
        <translation>   %1, %2		 Export vers un fichier SQL.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2		 Export to a png image.</source>
        <translation>   %1, %2		 Export en image PNG.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2		 Export directly to a PostgreSQL server.</source>
        <translation>   %1, %2		 Exporter directement dans un serveur PostgreSQL.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2		 List available connections on %3 file.</source>
        <translation>   %1, %2		 Lister les connexions disponnibles dans le fichier %3.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2		 Version of generated SQL code. Only for file or dbms export.</source>
        <translation>   %1, %2		 Version de PostgreSQL pour laquelle doit être généré l&apos;export. Uniquement pour les exports fichier SQL et en direct dans le serveur.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2			 Silent execution. Only critical errors are shown during process.</source>
        <translation>   %1, %2			 Exécution silencieuse. Seuls les avertissements et erreurs seront affichés pendant le traitement.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2			 Show this help menu.</source>
        <translation>   %1, %2			 Affiche ce menu.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>PNG export options: </source>
        <translation>Option d&apos;export PNG: </translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2		 Draws the grid on the exported png image.</source>
        <translation>   %1, %2		 Dessiner la grille.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2	 Draws the page delimiters on the exported png image.</source>
        <translation>   %1, %2	 Dessiner les limites de pages.</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>DBMS export options: </source>
        <translation>Option d&apos;exportation directe dans un serveur:</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2	 Ignores errors related to duplicated objects that eventually exists on server side.</source>
        <translation>   %1, %2	 Ignorer les erreurs relatives à d&apos;éventuels objets dupliqués déjà existant sur le serveur.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[ALIAS]	 Connection configuration alias to be used.</source>
        <translation>   %1, %2=[ALIAS]	 Alias de connexion à utiliser.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[HOST]		 PostgreSQL host which export will operate.</source>
        <translation>   %1, %2=[HOST]		 Hôte du serveur PostgreSQL.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[PORT]		 PostgreSQL host listening port.</source>
        <translation>   %1, %2=[PORT]		 Port du serveur PostgreSQL.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[USER]		 PosrgreSQL username.</source>
        <translation>   %1, %2=[USER]		 Nom d&apos;utilisateur.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[PASSWORD]	 PosrgreSQL user password.</source>
        <translation>   %1, %2=[PASSWORD]	 Mot de passe.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>   %1, %2=[DBNAME]	 Connection&apos;s initial database.</source>
        <translation>   %1, %2=[DBNAME]	 Base de donnée initiale de connexion.</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>There is no connections configured.</source>
        <translation>Aucune connxion configurée.</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Available connections (alias : conn. string)</source>
        <translation>Connexions disponibles (alias : chaîne de conn.)</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>No export mode specified!</source>
        <translation>Pas de mode d&apos;export spécifié!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Multiple export mode especified!</source>
        <translation>Multiples mode d&apos;exportation spécifiés!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>No input file specified!</source>
        <translation>Pas de fichier source!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>No output file specified!</source>
        <translation>Pas de fichier de sortie!</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Input file must be different from output!</source>
        <translation>Le fichier source doit être différent du fichier de sortie!</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Incomplete connection information!</source>
        <translation>Les information de connexion sont incomplètes!</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Starting model export...</source>
        <translation>Début de l&apos;export du modèle...</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Loading input file: </source>
        <translation>Chargement du fichier source:</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Export to PNG image: </source>
        <translation>Export en image PNG:</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Export to SQL script file: </source>
        <translation>Export en fichier SQL:</translation>
    </message>
    <message>
        <location line="+8"/>
        <source>Export to DBMS: </source>
        <translation>Export directement en base:</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>Export successfully ended!</source>
        <translation>Export complété avec succès!</translation>
    </message>
</context>
<context>
    <name>PgModelerPlugin</name>
    <message>
        <location filename="../libpgmodeler_ui/src/pgmodelerplugin.cpp" line="+37"/>
        <source>Plugin Information</source>
        <translation>Information de l&apos;extension</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Version: %1</source>
        <translation>Version: %1</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Author: %1</source>
        <translation>Auteur: %1</translation>
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
        <location line="+33"/>
        <source>Data Type</source>
        <translation>Type de donnée</translation>
    </message>
    <message>
        <location line="+57"/>
        <source>SRID:</source>
        <translation>SRID:</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Variation:</source>
        <translation>Variation:</translation>
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
        <location line="+225"/>
        <source>Precision</source>
        <translation>Précision</translation>
    </message>
    <message>
        <location line="-174"/>
        <source>Spatial:</source>
        <translation>Spatial:</translation>
    </message>
    <message>
        <location line="+25"/>
        <location line="+328"/>
        <source>Dimension</source>
        <translation>Dimension</translation>
    </message>
    <message>
        <location line="-291"/>
        <source>Format:</source>
        <translation>Format:</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Timezone:</source>
        <translation>Fuseau horaire:</translation>
    </message>
    <message>
        <location line="+50"/>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>P:</source>
        <translation>P:</translation>
    </message>
    <message>
        <location line="+59"/>
        <location line="+42"/>
        <source>Length</source>
        <translatorcomment>Taille ?</translatorcomment>
        <translation>Longueur</translation>
    </message>
    <message>
        <location line="+6"/>
        <source>L:</source>
        <translation>L:</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>Interval:</source>
        <translation>Intervalle:</translation>
    </message>
    <message>
        <location line="+36"/>
        <source>[ ]:</source>
        <translation>[ ]:</translation>
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
        <location line="+20"/>
        <source>Plug-ins root directory:</source>
        <translation>Répertoire racine des extentions:</translation>
    </message>
    <message>
        <location line="+32"/>
        <source>Open in file manager</source>
        <translation>Ouvrir dans le getionnaire de fichier</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Loaded plug-ins</source>
        <translation>Extensions chargées</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Details about plugins development and installation can be found at &lt;a href=&quot;http://www.pgmodeler.com.br/wiki/doku.php?id=plugins&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;pgModeler Wiki&lt;/span&gt;&lt;/a&gt;. &lt;span style=&quot; font-weight:600;&quot;&gt;Note:&lt;/span&gt; plugin installation requires pgModeler to be restarted.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</source>
        <translation>&lt;html&gt;&lt;head/&gt;&lt;body&gt;&lt;p&gt;Les informations concernant le développement et l&apos;installation des extensions sont disponibles sur le  &lt;a href=&quot;http://www.pgmodeler.com.br/wiki/doku.php?id=plugins&quot;&gt;&lt;span style=&quot; text-decoration: underline; color:#0057ae;&quot;&gt;Wiki pgModeler&lt;/span&gt;&lt;/a&gt;. &lt;span style=&quot; font-weight:600;&quot;&gt;Note:&lt;/span&gt; l&apos;installation d&apos;extensions requière le redémarrage de pgModeler.&lt;/p&gt;&lt;/body&gt;&lt;/html&gt;</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/pluginsconfigwidget.cpp" line="+32"/>
        <source>Plugin</source>
        <translation>Extension</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Version</source>
        <translation>Version</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Library</source>
        <translation>Bibliothèque</translation>
    </message>
</context>
<context>
    <name>QObject</name>
    <message>
        <location filename="../libpgmodeler/src/databasemodel.cpp" line="-5664"/>
        <source>new_database</source>
        <translation>nouvelle_base</translation>
    </message>
    <message>
        <location line="+2543"/>
        <location line="+22"/>
        <location line="+308"/>
        <location line="+26"/>
        <location line="+27"/>
        <location line="+80"/>
        <location line="+143"/>
        <location line="+59"/>
        <location line="+64"/>
        <location line="+223"/>
        <location line="+70"/>
        <location line="+76"/>
        <location line="+62"/>
        <location line="+100"/>
        <location line="+124"/>
        <location line="+26"/>
        <location line="+74"/>
        <location line="+61"/>
        <location line="+48"/>
        <location line="+216"/>
        <location line="+213"/>
        <location line="+66"/>
        <location line="+168"/>
        <location line="+82"/>
        <location line="+164"/>
        <location line="+59"/>
        <location line="+29"/>
        <location line="+37"/>
        <location line="+216"/>
        <location line="+158"/>
        <source>%1 (line: %2)</source>
        <translation>%1 (ligne: %2)</translation>
    </message>
</context>
<context>
    <name>Relationship</name>
    <message>
        <location filename="../libpgmodeler/src/relationship.cpp" line="+72"/>
        <source>%1_has_one_%2</source>
        <translatorcomment>Traduction approprié ?</translatorcomment>
        <translation>%1_has_one_%2</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1_has_many_%2</source>
        <translatorcomment>Traduction approprié ?</translatorcomment>
        <translation>%1_has_many_%2</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>many_%1_has_many_%2</source>
        <translatorcomment>Traduction approprié ?</translatorcomment>
        <translation>many_%1_has_many_%2</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1_inherits_%2</source>
        <translatorcomment>Traduction approprié ?</translatorcomment>
        <translation>%1_inherits_%2</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>%1_copies_%2</source>
        <translatorcomment>Traduction approprié ?</translatorcomment>
        <translation>%1_copies_%2</translation>
    </message>
</context>
<context>
    <name>RelationshipWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/relationshipwidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>General</source>
        <translation>Général</translation>
    </message>
    <message>
        <location line="+37"/>
        <source>Relationship:</source>
        <translation>Relation:</translation>
    </message>
    <message>
        <location line="+35"/>
        <source>One to one relationship</source>
        <translation>Relation un à un</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>1-1</source>
        <translation>1-1</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>One to many relationship</source>
        <translation>Relation un à plusieurs</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>1-n</source>
        <translation>1-n</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Many to many relationship</source>
        <translation>Relation plusieurs à plusieurs</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>n-n</source>
        <translation>n-n</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Generalization relationship (inheritance)</source>
        <translation>Relation de généralisation (héritage)</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>gen</source>
        <translation>gen</translation>
    </message>
    <message>
        <location line="+33"/>
        <source>Dependency / Copy relationship</source>
        <translation>Relation de dépendance / copie</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>dep</source>
        <translation>dep</translation>
    </message>
    <message>
        <location line="+27"/>
        <source>Relationship generated via foreign key</source>
        <translation>Relation générée par une clef étrangère</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>fk</source>
        <translation>fk</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Identifier</source>
        <translation>Identifiant</translation>
    </message>
    <message>
        <location line="+25"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="+244"/>
        <source>Table 1:</source>
        <translation>Table 1:</translation>
    </message>
    <message>
        <location line="+77"/>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="+2"/>
        <source>Table 2:</source>
        <translation>Table 2:</translation>
    </message>
    <message>
        <location line="+64"/>
        <source>Cardinality:</source>
        <translation>Cardinalité:</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>[SRC] is required</source>
        <translation>[SRC] est requis</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>[DST] is required</source>
        <translation>[DST] est requis</translation>
    </message>
    <message>
        <location line="-446"/>
        <location line="+473"/>
        <source>Name of the table generated from many to many relationship</source>
        <translation>Nom de la table générée par une relation plusieurs à plusieurs</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Table Name:</source>
        <translation>Nom de table:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Source Suffix:</source>
        <translation>Suffixe source:</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Target Suffix:</source>
        <translation>Suffixe cible:</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Foreign key</source>
        <translation>Clef étrangère</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>Deferrable:</source>
        <translation>Différable:</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Deferral:</source>
        <translation>Report:</translation>
    </message>
    <message>
        <location line="+44"/>
        <source>Automatic Suffix</source>
        <translation>Suffixe automatique</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Copy Options</source>
        <translation>Options de copie</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>INDEXES</source>
        <translation>INDEXES</translation>
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
        <location line="+13"/>
        <source>EXCLUDING</source>
        <translation>EXCLURE</translation>
    </message>
    <message>
        <location line="+10"/>
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
        <location line="+21"/>
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
        <translation>Clef primaire</translation>
    </message>
    <message>
        <location line="+29"/>
        <source>Advanced</source>
        <translation>Avancé</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/relationshipwidget.cpp" line="-184"/>
        <source>Attribute</source>
        <translation>Attribut</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+6"/>
        <location line="+6"/>
        <source>Type</source>
        <translation>Type</translation>
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
        <location line="+18"/>
        <source>Editing attributes of an existing relationship is allowed, but must be done carefully because it may break references to columns and cause invalidation of objects such as triggers, indexes, constraints and sequences.</source>
        <translation>Éditer les attributs d&apos;une relation existante est autorisée, cependant cela doit être fait avec prudence car cela peut casser des références aux colonnes et causer l&apos;invalidation d&apos;objets tels que déclencheurs, index, contraintes et séquences.</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Use the special primary key if you want to include a primary key containing inherited / copied columns to the receiving table. This is a feature available only for generalization / copy relationships.</source>
        <translatorcomment>Doutes</translatorcomment>
        <translation type="unfinished">Utilisez une clef primaire spécifique si vous souhaitez inclure une clef primaire héritée ou copiée de la table de destination. Cette fonctionnalité n&apos;est disponnible que pour les relations généralisation / copie.</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>This advanced tab shows the objects (columns or table) auto created by the relationship&apos;s connection as well the foreign keys that represents the link between the participant tables.</source>
        <translation>Cet onglet avancé montre les objets (colonnes ou tables) générés automatiquement par la relation ainsi que les clefs étrangères qui forment le(s) lien(s) entre les tables.</translation>
    </message>
    <message>
        <location line="+144"/>
        <location line="+1"/>
        <source> is required</source>
        <translation> est requis</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>Reference Table:</source>
        <translation>Table de référence:</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Receiver Table:</source>
        <translation>Table de destination:</translation>
    </message>
    <message>
        <location line="+3"/>
        <source>Reference Suffix:</source>
        <translation>Suffix de référence:</translation>
    </message>
    <message>
        <location line="+4"/>
        <location line="+1"/>
        <source> Suffix:</source>
        <translation>Suffix:</translation>
    </message>
    <message>
        <location line="-1"/>
        <source>Table 1 </source>
        <translation>Table 1 </translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Table 2 </source>
        <translation>Table 2 </translation>
    </message>
</context>
<context>
    <name>RoleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+159"/>
        <source>Password:</source>
        <translation>Mot de passe:</translation>
    </message>
    <message>
        <location line="-135"/>
        <source>yyyy-MMM-dd hh:mm</source>
        <translation>yyyy-MMM-dd hh:mm</translation>
    </message>
    <message>
        <location line="+49"/>
        <source>Connections:</source>
        <translation>Connexions:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Superuser</source>
        <translation>Super utilisateur</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Inherit Permissions</source>
        <translation>Hériter des permissions</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Can create Database</source>
        <translation>Peut créer des bases de données</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Can create Role</source>
        <translation>Peut créer des rôles</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Can use Replication</source>
        <translation>Peut utiliser la réplication</translation>
    </message>
    <message>
        <location line="+55"/>
        <source>Encrypted</source>
        <translation>Encrypté</translation>
    </message>
    <message>
        <location line="-69"/>
        <source>Can Login</source>
        <translation>Peut s&apos;authentifier</translation>
    </message>
    <message>
        <location line="+28"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="+67"/>
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
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="-37"/>
        <source>Assigning &lt;strong&gt;&lt;em&gt;-1&lt;/em&gt;&lt;/strong&gt; to &lt;strong&gt;&lt;em&gt;Connections&lt;/em&gt;&lt;/strong&gt; creates a role without connection limit.&lt;br/&gt;																				 Unchecking &lt;strong&gt;&lt;em&gt;Validity&lt;/em&gt;&lt;/strong&gt; creates an role that never expires.</source>
        <translation>Definir &lt;strong&gt;&lt;em&gt;Connexions&lt;/em&gt;&lt;/strong&gt; à &lt;strong&gt;&lt;em&gt;-1&lt;/em&gt;&lt;/strong&gt; créé le rôle sans limite de connexion.&lt;br/&gt;																				 Décocher &lt;strong&gt;&lt;em&gt;Validité&lt;/em&gt;&lt;/strong&gt; créé un rôle qui n&apos;expire jamais.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Role</source>
        <translation>Rôle</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/ui/rolewidget.ui" line="-92"/>
        <location filename="../libpgmodeler_ui/src/rolewidget.cpp" line="+3"/>
        <source>Validity</source>
        <translation>Validité</translation>
    </message>
</context>
<context>
    <name>RuleWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/rulewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Event:</source>
        <translation>Evènement:</translation>
    </message>
    <message>
        <location line="+52"/>
        <source>Execution Type:</source>
        <translation>Type d&apos;exécution:</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>Conditional Expr.:</source>
        <translation>Expr. conditionnelle:</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Executed Commands</source>
        <translation>Commandes éxécutées</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>SQL Command:</source>
        <translation>Commandes SQL:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/rulewidget.cpp" line="+43"/>
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
    <name>SchemaWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/schemawidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+80"/>
        <source>Show rectangle</source>
        <translation>Afficher le rectangle</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Fill color:</source>
        <translation>Couleur de remplissage:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/schemawidget.cpp" line="+72"/>
        <source>Select fill color</source>
        <translation>Choisissez une couleur de remplissage</translation>
    </message>
</context>
<context>
    <name>SequenceWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sequencewidget.ui" line="+26"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Cyclic:</source>
        <translation>Cyclique:</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Start:</source>
        <translation>Début:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Maximum:</source>
        <translation>Maximum:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Minimum:</source>
        <translation>Minimum:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Increment:</source>
        <translation>Incrément:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Cache:</source>
        <translation>Cache:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Owner:</source>
        <translation>Propriétaire:</translation>
    </message>
</context>
<context>
    <name>SourceCodeWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/sourcecodewidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+26"/>
        <source>Version:</source>
        <translation>Version:</translation>
    </message>
    <message>
        <location line="+46"/>
        <source>PostgreSQL</source>
        <translation>PostgreSQL</translation>
    </message>
    <message>
        <location line="+51"/>
        <source>iconecodigo</source>
        <translation>icone</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>SQL</source>
        <translation>SQL</translation>
    </message>
    <message>
        <location line="+34"/>
        <source>XML</source>
        <translation>XML</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/sourcecodewidget.cpp" line="+32"/>
        <source>Type:</source>
        <translation>Type:</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>Source code visualization</source>
        <translation>Visualisation du code source</translation>
    </message>
    <message>
        <location line="+66"/>
        <source>Generating source code...</source>
        <translation>Génération du code source...</translation>
    </message>
    <message>
        <location line="+11"/>
        <source>-- SQL code unavailable for this type of object --</source>
        <translation>-- Code SQL non disponible pour ce type d&apos;objet. --</translation>
    </message>
</context>
<context>
    <name>Table</name>
    <message>
        <location filename="../libpgmodeler/src/table.cpp" line="+34"/>
        <source>new_table</source>
        <translation>nouvelle_table</translation>
    </message>
</context>
<context>
    <name>TableWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/tablewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+30"/>
        <source>With OIDs:</source>
        <translation>Avec OIDs:</translation>
    </message>
    <message>
        <location line="+21"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Constraints</source>
        <translation>Contraintes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Triggers</source>
        <translation>Déclencheur</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Rules</source>
        <translation>Rôles</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Indexes</source>
        <translation>Indexes</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Tables</source>
        <translation>Tables</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/tablewidget.cpp" line="+44"/>
        <location line="+33"/>
        <location line="+8"/>
        <location line="+8"/>
        <location line="+9"/>
        <location line="+6"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="-62"/>
        <source>Schema</source>
        <translation>Schéma</translation>
    </message>
    <message>
        <location line="+2"/>
        <location line="+31"/>
        <location line="+8"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="-6"/>
        <source>Default Value</source>
        <translation>Valeur par défaut</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Attribute</source>
        <translation>Attribut</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>ON DELETE</source>
        <translation>ON DELETE</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>ON UPDATE</source>
        <translation>ON UPDATE</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Refer. Table</source>
        <translation>Table référ.</translation>
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
        <location line="+5"/>
        <source>Execution</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Event</source>
        <translation>Evènement</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Indexing</source>
        <translation>Indexation</translation>
    </message>
    <message>
        <location line="+167"/>
        <source>Parent</source>
        <translation>Parent</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Copy</source>
        <translation>Copie</translation>
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
        <location line="+18"/>
        <source>Directory:</source>
        <translation>Dossier:</translation>
    </message>
</context>
<context>
    <name>TaskProgressWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/taskprogresswidget.ui" line="+29"/>
        <source>Executing tasks</source>
        <translation>Exécution des tâches</translation>
    </message>
    <message>
        <location line="+81"/>
        <source>Waiting task to start...</source>
        <translation>En attente du démarrage de la tâche ...</translation>
    </message>
</context>
<context>
    <name>TextboxWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/textboxwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+14"/>
        <source>Bold</source>
        <translation>Gras</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Italic</source>
        <translation>Italique</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Underline</source>
        <translation>Souligné</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/textboxwidget.cpp" line="+68"/>
        <source>Select text color</source>
        <translation>Sélection de la couleur de texte</translation>
    </message>
</context>
<context>
    <name>TriggerWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/triggerwidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+154"/>
        <source>Event:</source>
        <translation>Evènement:</translation>
    </message>
    <message>
        <location line="+30"/>
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
        <location line="-211"/>
        <source>Deferrable:</source>
        <translation>Différable:</translation>
    </message>
    <message>
        <location line="+356"/>
        <source>Columns</source>
        <translation>Colonnes</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Column:</source>
        <translation>Colonne:</translation>
    </message>
    <message>
        <location line="-57"/>
        <source>Arguments</source>
        <translation>Arguments</translation>
    </message>
    <message>
        <location line="-351"/>
        <source>Constraint</source>
        <translation>Contrainte</translation>
    </message>
    <message>
        <location line="+107"/>
        <source>FOR EACH ROW</source>
        <translation>Pour chaque ligne</translation>
    </message>
    <message>
        <location line="+162"/>
        <source>Refer. Table:</source>
        <translation>Table référ.</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Condition:</source>
        <translation>Condition:</translation>
    </message>
    <message>
        <location line="+69"/>
        <source>Argument:</source>
        <translation>Argument:</translation>
    </message>
    <message>
        <location line="+99"/>
        <source>Function:</source>
        <translation>Fonction:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Options:</source>
        <translation>Options:</translation>
    </message>
    <message>
        <location line="+28"/>
        <source>Excution:</source>
        <translation>Excution:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/triggerwidget.cpp" line="+49"/>
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
        <location filename="../libpgmodeler_ui/ui/typewidget.ui" line="+20"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+277"/>
        <source>Configuration:</source>
        <translation>Configuration:</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Base Type</source>
        <translation>Type de base</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Enumeration</source>
        <translation>Enumération</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Composite</source>
        <translation>Composite</translation>
    </message>
    <message>
        <location line="+19"/>
        <source>Enumerations</source>
        <translation>Énumérations</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>Enumeration:</source>
        <translation>Énumeration:</translation>
    </message>
    <message>
        <location line="-327"/>
        <location line="+352"/>
        <location line="+41"/>
        <source>Attributes</source>
        <translation>Attributs</translation>
    </message>
    <message>
        <location line="-384"/>
        <source>Internal Length:</source>
        <translation>Long. interne:</translation>
    </message>
    <message>
        <location line="+23"/>
        <source>Storage:</source>
        <translation>Stockage:</translation>
    </message>
    <message>
        <location line="+54"/>
        <source>Category:</source>
        <translation>Catégorie:</translation>
    </message>
    <message>
        <location line="+10"/>
        <source>Delimiter:</source>
        <translation>Délimiteur:</translation>
    </message>
    <message>
        <location line="+24"/>
        <source>Alignment:</source>
        <translation>Alignement:</translation>
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
        <translation>double precision</translation>
    </message>
    <message>
        <location line="-45"/>
        <source>Default Value:</source>
        <translation>Valeur par défaut:</translation>
    </message>
    <message>
        <location line="-121"/>
        <source>Range</source>
        <translation>Interval</translation>
    </message>
    <message>
        <location line="+53"/>
        <source>Options:</source>
        <translation>Options:</translation>
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
        <translation>Collatable</translation>
    </message>
    <message>
        <location line="+100"/>
        <source>Functions</source>
        <translation>Fonctions</translation>
    </message>
    <message>
        <location line="+12"/>
        <source>INPUT:</source>
        <translation>INPUT:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>OUTPUT:</source>
        <translation>OUTPUT:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>RECV:</source>
        <translation>RECV:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>SEND:</source>
        <translation>SEND:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>TPMOD_IN:</source>
        <translation>TPMOD_IN:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>TPMOD_OUT:</source>
        <translation>TPMOD_OUT:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>ANALYZE:</source>
        <translation>ANALYZE:</translation>
    </message>
    <message>
        <location line="+130"/>
        <source>Name:</source>
        <translation>Nom:</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>Collation:</source>
        <translation>Collation:</translation>
    </message>
    <message>
        <location line="+25"/>
        <source>Subtype Diff Func.:</source>
        <translation>Fonction de différence du sous-type:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Operator Class:</source>
        <translation>Classe d&apos;opérateurs:</translation>
    </message>
    <message>
        <location line="+7"/>
        <source>Canonical Func.:</source>
        <translation>Fonction canonique:</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/typewidget.cpp" line="+34"/>
        <source>Like Type</source>
        <translation>Type de Like</translation>
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
        <location line="+24"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Type</source>
        <translation>Type</translation>
    </message>
    <message>
        <location line="+2"/>
        <source>Collation</source>
        <translation>Collation</translation>
    </message>
    <message>
        <location line="+15"/>
        <source>The functions to be assigned to a type should be written in C language and possess, respectively, the following signatures:&lt;br/&gt;  &lt;table&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;INPUT:&lt;/strong&gt; &lt;em&gt;any function(cstring, oid, integer)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;OUTPUT:&lt;/strong&gt; &lt;em&gt;cstring function(any)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;SEND:&lt;/strong&gt; &lt;em&gt;byta function(any)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;RECV:&lt;/strong&gt; &lt;em&gt;any function(internal, oid, integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_IN:&lt;/strong&gt; &lt;em&gt;integer function(cstring[])&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_OUT:&lt;/strong&gt; &lt;em&gt;cstring function(integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;ANALYZE:&lt;/strong&gt; &lt;em&gt;boolean function(internal)&lt;/em&gt;&lt;/td&gt;    &lt;tr&gt;  &lt;/table&gt;</source>
        <translation>La fonction assignée à un type devrait être écrite en langage C et possèder, les signatures respective suivantes:&lt;br/&gt;  &lt;table&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;INPUT:&lt;/strong&gt; &lt;em&gt;any fonction(cstring, oid, integer)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;OUTPUT:&lt;/strong&gt; &lt;em&gt;cstring fonction(any)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;SEND:&lt;/strong&gt; &lt;em&gt;byta fonction(any)&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;RECV:&lt;/strong&gt; &lt;em&gt;any fonction(internal, oid, integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_IN:&lt;/strong&gt; &lt;em&gt;integer fonction(cstring[])&lt;/em&gt;&lt;/td&gt;    &lt;td&gt;&lt;strong&gt;TPMOD_OUT:&lt;/strong&gt; &lt;em&gt;cstring fonction(integer)&lt;/em&gt;&lt;/td&gt;   &lt;/tr&gt;   &lt;tr&gt;    &lt;td&gt;&lt;strong&gt;ANALYZE:&lt;/strong&gt; &lt;em&gt;boolean fonction(internal)&lt;/em&gt;&lt;/td&gt;    &lt;tr&gt;  &lt;/table&gt;</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>The functions to be assigned to a range type should have the following signatures:&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonical:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</source>
        <translation>Les fonction assignables au type interval (RANGE) doivent avoir la signature suivante:&lt;br/&gt;&lt;br/&gt;&lt;strong&gt;Canonique:&lt;/strong&gt; &lt;em&gt;any function(any)&lt;/em&gt; &lt;br/&gt;&lt;strong&gt;Subtype Diff:&lt;/strong&gt; &lt;em&gt;double precision function(subtype, subtype)&lt;/em&gt;</translation>
    </message>
</context>
<context>
    <name>ViewWidget</name>
    <message>
        <location filename="../libpgmodeler_ui/ui/viewwidget.ui" line="+14"/>
        <source>Form</source>
        <translation>Formulaire</translation>
    </message>
    <message>
        <location line="+16"/>
        <source>References</source>
        <translation>Références</translation>
    </message>
    <message>
        <location line="+210"/>
        <source>Column</source>
        <translation>Colonne</translation>
    </message>
    <message>
        <location line="+5"/>
        <source>Expression</source>
        <translation>Expression</translation>
    </message>
    <message>
        <location line="-177"/>
        <source>Used in:</source>
        <translation>Utilisé dans:</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>Reference Type:</source>
        <translation>Type de référence:</translation>
    </message>
    <message>
        <location line="+22"/>
        <source>SELECT-FROM</source>
        <translation>SELECT-FROM</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>FROM-WHERE</source>
        <translation>FROM-WHERE</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>After WHERE</source>
        <translation>Après WHERE</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>View Definition</source>
        <translation>Définition de la vue</translation>
    </message>
    <message>
        <location line="+9"/>
        <source>Table:</source>
        <translation>Table:</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Table Alias:</source>
        <translation>Alias table:</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Column:</source>
        <translation>Colonne:</translation>
    </message>
    <message>
        <location line="+13"/>
        <source>Column Alias:</source>
        <translation>Alias colonne:</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Expression:</source>
        <translation>Expression:</translation>
    </message>
    <message>
        <location line="+41"/>
        <source>Expression Alias:</source>
        <translation>Alias expression:</translation>
    </message>
    <message>
        <location line="+32"/>
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
        <source>Table Expression</source>
        <translation>CTE</translation>
    </message>
    <message>
        <location line="+17"/>
        <source>Code Preview</source>
        <translation>Prévisualisation du code</translation>
    </message>
    <message>
        <location filename="../libpgmodeler_ui/src/viewwidget.cpp" line="+61"/>
        <source>Col./Expr.</source>
        <translation>Col./Expr.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Alias</source>
        <translation>Alias</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Alias Col.</source>
        <translation>Alias col.</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Flags: SF FW AW VD</source>
        <translation>Flags: SF FW AW VD</translation>
    </message>
    <message>
        <location line="+29"/>
        <location line="+9"/>
        <source>Name</source>
        <translation>Nom</translation>
    </message>
    <message>
        <location line="-7"/>
        <source>Refer. Table</source>
        <translation>Table référ.</translation>
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
        <location line="+5"/>
        <source>Execution</source>
        <translation>Exécution</translation>
    </message>
    <message>
        <location line="+1"/>
        <source>Event</source>
        <translation>Evènement</translation>
    </message>
    <message>
        <location line="+534"/>
        <source>/* Could not generate the SQL code. Make sure all attributes are correctly filled! </source>
        <translation>/* Le code SQL ne peut être généré. Vérifiez que les champs sont correctement remplis! </translation>
    </message>
    <message>
        <location line="-573"/>
        <source>To reference all columns in a table (*) just do not fill the field &lt;strong&gt;Column&lt;/strong&gt;, this is the same as write &lt;em&gt;&lt;strong&gt;[schema].[tablel].*&lt;/strong&gt;&lt;/em&gt;</source>
        <translation>Pour référencer toutes les colonnes dans une table (*) ne remplissez pas le champs &lt;strong&gt;Colonne&lt;/strong&gt;, ceci revient à écrire &lt;em&gt;&lt;strong&gt;[schéma].[table].*&lt;/strong&gt;&lt;/em&gt;</translation>
    </message>
</context>
</TS>
