Initial CFG grammar. ```TODO:``` ```remove left recursion```


* ```path -> path/.. | path/. | path/regexp | path//regexp | regexp```
* ```regexp -> node | node, regexp | regexp, iteration | (regexp)```
* ```iteration -> ? | + | * | {number} | {number, number} | {number, ε} | {ε, number}```
* ```node -> [selection] | [^selection] | single```
* ```selection -> range, selection | range```
* ```range -> digit-digit | letter-letter | single```
* ```single -> digit | letter | special```
* ```number -> digit | digit, number```
* ```digit -> 0 | 1 | ... | 9```
* ```letter -> a | b | ... | z | A | B | ... | Z```
* ```special -> . | \n | ...```
