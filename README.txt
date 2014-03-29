Piglet - a simple data processing scripting language inspired by Apache Pig.

Piglet supports a small number of capabilities that were inspired by Apache Pig.
This includes a simple type system (BYTEARRAY, CHARARRAY, LONG, DOUBLE, BOOLEAN).
The following operators are supported:

  <relation> = LOAD <filename> DELIM <char>

  STORE <relation> INTO <filename> DELIM <char>

  <relation> = FOREACH <relation> GENERATE <expr>[, <expr>]

  <relation> = FILTER <relation> BY <expr>

  <relation> = SORT <relation> BY <field>

  DUMP <relation>

  DESCRIBE <relation>

An expression is a postfix expression that includes type and name information.
Fields within Piglet can be defined by name (as defined by the user) or using
positional notation ($0 is the first field).  The following expression uses
positional notation to add the first two fields of a relation:

  [$0 $1 +]:LONG:varname

For the FILTER operator, the result must be a BOOLEAN type.  The following 
expression tests the equality of the first two named fields:

  [field1 field2 ==]:BOOLEAN:test

A number of postfix operators are available for use, including:

  +, -, *, /        Arithmetic operations
  ==, >=, <=, <. >  Conditional operations
  &&, ||            Boolean operations
  STRLEN            length of a string (top object on stack)
  COUNT             Apply to a group to count members

