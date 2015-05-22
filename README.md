# EasyAlias
Quick and robust aliasing for command line.

# Installation
Clone this repository and run ```make```. </br>
Navigate to either ```~/.bash_profile``` or ```~/.bashrc```. </br> 
Paste in the following code: </br>
```
if [ -f ~/.bash_easyaliases ]; then
  . ~/.bash_easyaliases
fi
``` 
Finally, run ```source ~/.bashrc``` or ```source ~/.bash_profile```. <br/>
You're all set.

# Usage
* **Learn**: assign current working directory an alias. </br> ```learn <alias>``` </br> ```l <alias>```

* **Forget**: drop matching alias. </br> ```forget <alias>``` </br> ```f <alias>```

* **Enter**: navigate to directory stored as alias. </br> ```enter <alias>``` ```goto <alias>```  ```switch <alias>``` </br> ```e <alias>``` ```gt <alias>``` ```sw <alias>```

* **Show**: list all aliases currently in use. </br> ```show```



 
