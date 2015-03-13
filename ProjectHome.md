A simple data processing scripting language inspired by Apache Pig.  This was written quickly as a monthly code project, but implements some interesting features of Pig for processing data sets.

From the README:

# Piglet #

Piglet is a simple data processing environment for processing and analyzing small data sets (inspired by Apache Pig).

Piglet supports a small number of capabilities that were inspired by Apache Pig.
This includes a simple type system (BYTEARRAY, CHARARRAY, LONG, DOUBLE, BOOLEAN).
The following operators are supported:

```
<relation> = LOAD <filename> DELIM <char>

STORE <relation> INTO <filename> DELIM <char>

<relation> = FOREACH <relation> GENERATE <expr>[, <expr>]

<relation> = FILTER <relation> BY <expr>

<relation> = GROUP <relation> BY <field>

<relation> = SORT <relation> BY <field> [ASCEND | DESCEND]

DUMP <relation>

DESCRIBE <relation>
```

An expression is a postfix expression that includes type and name information.
Fields within Piglet can be defined by name (as defined by the user) or using
positional notation ($0 is the first field). The following expression uses
positional notation to add the first two fields of a relation:

```
[$0 $1 +]:LONG:varname
```

For the FILTER operator, the result must be a BOOLEAN type. The following
expression tests the equality of the first two named fields:

```
[field1 field2 ==]:BOOLEAN:test
```

Or testing whether a string is more than < 2 characters:

```
[string1 STRLEN 2 <]:BOOLEAN:test
```

Or testing a string to be equal to a given sequence:

```
[state "CO" ==]:BOOLEAN:test
```

A number of postfix operators are available for use, including:

`+, -, *, /` Arithmetic operations
`==, >=, <=, <. >` Conditional operations
`&&, ||` Boolean operations
`STRLEN` length of a string (top object on stack)
`COUNT` Apply to a group to count members

See the ./tests subdirectory for script examples.

# Example #

The following example illustrates the use of Piglet to find states with large bank failures using an FDIC failed bank data set.  The data itself has the structure:

```
Bank Name,City,State,CERT #,Acquiring Institution,Closing Date,Updated Date
,,,,,,
Community Bank of the Ozarks,Sunrise Beach,MO,27331,Bank of Sullivan,14-Dec-12,19-Dec-12
Hometown Community Bank,Braselton,GA,57928,"CertusBank, National Association",16-Nov-12,29-Nov-12
Citizens First National Bank,Princeton,IL,3731,Heartland Bank and Trust Company,2-Nov-12,7-Nov-12
...
```

The following script illustrates this use case, including reading the data, cleansing it, sorting and grouping, and then counting and dumping/storing:

```
# Read in the banks data and build a relation with a schema (script1.pigl)
mydata = LOAD banks.txt DELIM ','
newdata = FOREACH mydata GENERATE [$0]:CHARARRAY:name, [$1]:CHARARRAY:city, [$2]:CHARARRAY:state, [$3]:LONG:cert

# Filter the data to those records with 2 character states
filtered = FILTER newdata BY [state STRLEN 2 ==]:BOOLEAN:test

# Sort and Group the data by state
sorted = SORT filtered BY state ASCEND
grouped = GROUP sorted BY state

# Count the number of tuples in the inner relation (group)
counted = FOREACH grouped GENERATE [state]:CHARARRAY:state, [group COUNT]:LONG:count

maxed = FILTER counted BY [count 50 > ]:BOOLEAN:test

sorted2 = SORT maxed BY count DESCEND

DUMP sorted2

STORE sorted2 INTO bigfail.txt DELIM ','
```

Executing this script with Piglet is demonstrated below:

```
mtj@mtj-desktop:~/work/piglet-mtj/tests$ ../piglet -f script1.pigl 
sorted2 = {
  {"GA",83},
  {"FL",63},
  {"IL",55}
}

mtj@mtj-desktop:~/work/piglet-mtj/tests$ more bigfail.txt 
GA,83
FL,63
IL,55
mtj@mtj-desktop:~/work/piglet-mtj/tests$ 
```