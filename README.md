# XML Parser

Parses XML tags and supports the execution of basic XPath 2.0 queries.

## Supports operators:
* '/': A/B returns the data of all the B children of A;
* '\[\]': A/B\[0\] returns the first of the A/B;
* '\@': A[\@attribute] returns returns the attribute(s) of all A elements;
* '=': A[B = data]/C returns the C-data of all B children of A whose data matches data

## Contents
* [Main]()
* [Header]()
* [Source]()