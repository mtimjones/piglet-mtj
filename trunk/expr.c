// Postfix expression parser

//To quickly get to the point of building expressions, build a postfix expr parser
//
//    x = FOREACH a GENERATE $0 + $1 AS f1:int
//
//Becomes
//
//    x = FOREACH a GENERATE f1:int:[$0 $1 *]
//
//A multi-field example
//
//    x = FOREACH b GENERATE f1:int:[ $0 $1 * $2 $3 * / ] f1:int[$4]
//
//
//This is parsed, and each expression stored as a string into a field expression array.
//
//As the relation is iterated, the field expression array is used to determine how to 
//project the relation.  Execution of the GENERATE is simply iterate the rows of the
//relation and apply the field expression array to determine what to produce.
//
//  Expression:
//
//    [<expr>]
//
//    [<expr>]:<type>:<name>
//
//


