# XML Parser

Parses XML tags and supports the execution of basic XPath 2.0 queries.

## Supports operators:
* '/': A/B returns the data of all the B children of A;
* '\[\]': A/B\[0\] returns the first of the A/B;
* '\@': A[\@attribute] returns returns the attribute(s) of all A elements;
* '=': A[B = data]/C returns the C-data of all B children of A whose data matches data

## Contents
* [Main](https://github.com/vladimirggg/XMLParser/blob/main/Main.cpp)
* [Header](https://github.com/vladimirggg/XMLParser/blob/main/Parser.h)
* [Source](https://github.com/vladimirggg/XMLParser/blob/main/Parser.cpp)

### Example
<img width="539" alt="image" src="https://github.com/vladimirggg/XMLParser/assets/52763967/4bc90112-abf7-40a6-a5da-44df96d58e10">

- person/address --> ["Brooklyn, NY", ”Manhattan, NYC", "Bulgaria"]
- person/address[0] --> "Brooklyn, NY"
- person[\@id] --> "0", "1"
- person[address=Bulgaria]/name --> "Ivan Petrov" 
