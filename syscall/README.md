# syscall

Есть программа (`main`) и операционная система (`init`).
Операционная система расширяет машину специальной областью памяти (массив int'ов по адерсу `base`).
При чтении элемента из массива его значение должно содержвать значение выражения
```
100000 + (индекс элемента * 1000) + (количество обращений к любому элементу массива)
```

См. main.c и test/1.out
