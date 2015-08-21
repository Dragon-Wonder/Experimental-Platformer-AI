# Notation

This project will use the following naming notation for variables within its code; in an attempt to keep all code clear between different people.

There are expections to using these in cases where the variable is used in a for loop.

This method is slightly based on the [Hungarian Notation] (https://en.wikipedia.org/wiki/Hungarian_notation) but with our own tweaks.

## Scope

| Scope         | Prefix                       | Example            |
| ------------- |:----------------------------:|--------------------|
| Global        | In Global Namespace or g_    | Global::blnLogging |
| Local         | None                         | strMyString        |
| Constant      | k                            |  kblnMyConstant    |
| Define        | DEFINED_ (everything in caps)|  DEFINED_MAP_WIDTH |


## Types

| Type          | Prefix | Example            |
| ------------- |:------:|--------------------|
| Boolean       | bln    | blnLogging         |
| Char          | chr    | chrNull            |
| Float         | f      | fTempFitness       |
| Integer       | int/ i | iValue             |
| Unsigned Int  | uint/u | uintSeed           |
| Unsigned Char | uchr   | uchrValue          |
| Double        | db     | dbSomeValue        |
| String        | str    | strMyString        |
| Pointer       | p      | pstrStringPointer  |
| Long          | lg     | lgLong             |
| Unsigned Long | ulg    | ulgValue           |
| Long Long     | llg    | llgWhyDoWeNeedThis |

## Classes / Structs

| Type          | Prefix            | Example            |
| ------------- |:-----------------:|--------------------|
| Class         | cls               | clsTick            |
| Object        | abr of class name | tckTick            |
| Struct        | abr of struct name| plyPlayer          |

The abr that should be used should be written in a comment.

Also the objects that have been declared in the global namespace do not have to follow this rule.

