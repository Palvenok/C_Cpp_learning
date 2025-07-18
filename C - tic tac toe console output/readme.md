Этот код создаёт **игровое поле 3×3** (например, для крестиков-ноликов), заполняет его значениями, введёнными пользователем, и выводит его в символьном формате.  

### **Что делает код?**  
1. **Создание игрового поля**  
   - Объявляется двумерный массив `char pole[3][3]` (размер `SIZE × SIZE`, где `SIZE = 3`), инициализированный нулями.  

2. **Ввод данных**  
   - Пользователь вводит числа (`0`, `1` или `2`), которые записываются в массив.  
   - Ввод продолжается, пока:  
     - Не будет заполнено всё поле (9 чисел),  
     - Не встретится нечисловой ввод (например, буква или конец файла).  

3. **Вывод поля**  
   - Функция `print_array()` отображает поле, заменяя числа на символы:  
     - `0` → `#` (пустая клетка),  
     - `1` → `x` (крестик),  
     - `2` → `o` (нолик).  
   - Элементы разделяются пробелами, строки — переводами строк.  

### **Пример работы**  
**Входные данные:**  
```
1 0 2
0 1 0
2 0 1
```  
**Вывод:**  
```
x # o  
# x #  
o # x  
```  

### **Особенности**  
- Если введено меньше 9 чисел, оставшиеся клетки останутся `#` (т.к. массив инициализирован нулями).  
- Если введено число, отличное от `0`, `1`, `2`, оно всё равно запишется в массив, но при выводе может отобразиться некорректно (т.к. `char` интерпретируется напрямую).  

### **Применение**  
Этот код можно использовать как основу для игры в **крестики-нолики** или других пошаговых игр на поле 3×3.