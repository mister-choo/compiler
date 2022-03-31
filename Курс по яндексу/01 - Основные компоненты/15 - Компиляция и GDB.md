# Флаги компиляции

Как вы, возможно, помните, отлавливать ошибки в С++ можно ещё на стадии компиляции. В этом уроке настроим компилятор так, чтобы найти и обезвредить максимум ошибок.

Компилятор — программа. Её задача — преобразовать текстовый файл с кодом в программу. Не пугайтесь рекурсии: всё есть программа. Как в «Матрице». Даже компилятор — это результат компиляции некоторого кода.

Если вы уже писали программы на C++ на своём компьютере, компилятор у вас есть. Если нет, вернитесь в начало первого спринта. На Linux и macOS компилятор стоит по умолчанию. Поставить и настроить его на Windows вы можете по инструкции в уроке [«Установка Eclipse на Windows, macOS и Linux»](https://praktikum.prestable.yandex.ru/trainer/cpp/lesson/34903d95-eb63-4ca2-af47-fda962200c8e/).

Мы рассмотрим работу с компилятором в консоли. Вывод приведён для g++ версии 10.0.1 на Ubuntu 20.04.1. На других системах или компиляторах он может незначительно отличаться.

Запустим g++ без параметров:

Скопировать кодBASH

`$ g++
g++-10: fatal error: no input files
compilation terminated.` 

Символ `$` вводить не нужно. В консоли он обычно используется как приглашение к вводу команды. Другой вариант приглашения — `>`. Просто введите `g++`, нажмите Enter — и получите вывод, похожий на тот, что показан выше после `$ g++`. Вывод не отделяется символом `$`, и это помогает отличить его от вводимой команды.

Напишем простую программу, суммирующую два числа:
```cpp

Скопировать кодCPP

`#include <iostream>

using namespace std;

int main() {
    int x, y;
    cin >> x >> y;
    cout << x + y << endl;
}` 

```

Сохраним её в файл example.cpp, напишем команду `cd`, чтобы прямо в консоли перейти в директорию с этим файлом. Скомпилируем файл из консоли:

Скопировать кодBASH

`$ g++ example.cpp -o example` 

Читать эту команду нужно так: «‎Скомпилируй файл example.cpp, а результат положи в исполняемый файл `example`». Компиляция прошла успешно, поэтому никакого вывода не последовало.

Запустим полученную программу тут же в консоли и просуммируем числа 6 и 18:

Скопировать кодBASH

`$ ./example 
6 18
24` 

На Windows вместо `./example` пишите `example.exe`.

Так вы можете компилировать код из консоли.

Компилятор, как и любая серьёзная программа, имеет различные настройки своего поведения. Их можно менять флагами. Некоторые флаги отвечают за предупреждения. Вспомним сравнение знакового и беззнакового целых чисел:

Скопировать кодCPP

`#include <iostream>
#include <vector>

using namespace std;

int main() {
    const vector<int> v = {6, 18, 2093};
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << endl;
    }
}` 

Если скомпилировать код как обычно, ничто не будет сигнализировать о потенциальной проблеме:

Скопировать кодBASH

`$ g++ unsigned_cmp.cpp -o unsigned_cmp` 

Если включим вывод предупреждений флагом `-Wall` (all warnings), увидим:

Скопировать кодBASH

`$ g++ unsigned_cmp.cpp -Wall -o unsigned_cmp
unsigned_cmp.cpp: In function ‘int main()’:
unsigned_cmp.cpp:8:23: warning: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<int>::size_type’ {aka ‘long unsigned int’} [-Wsign-compare]
    8 |     for (int i = 0; i < v.size(); ++i) {
      |                     ~~^~~~~~~~~~` 

При этом программа скомпилируется и будет работать. А предупреждение невнимательный разработчик не заметит. Чтобы обезопасить себя, добавьте флаг `-Werror` — treat warnings as errors. Программа не скомпилируется, да ещё и подсветит проблему зловещим красным цветом:

Скопировать кодBASH

`$ g++ unsigned_cmp.cpp -Wall -Werror -o unsigned_cmp
unsigned_cmp.cpp: In function ‘int main()’:
unsigned_cmp.cpp:8:23: error: comparison of integer expressions of different signedness: ‘int’ and ‘std::vector<int>::size_type’ {aka ‘long unsigned int’} [-Werror=sign-compare]
    8 |     for (int i = 0; i < v.size(); ++i) {
      |                     ~~^~~~~~~~~~
cc1plus: all warnings being treated as errors` 

Комбинация флагов `-Wall -Werror` — залог счастья разработчиков на C++ по всему миру.

Флагом можно настроить и стандарт языка. Разные версии компиляторов поддерживают разные стандарты C++. Стандарт — это формальный документ, описывающий возможности языка. Новый стандарт — новые возможности.

Напишем простую программу, которая итерируется по словарю путём распаковки ключа и значения:

Скопировать кодCPP

`#include <iostream>
#include <map>

using namespace std;

int main() {
    const map<string, int> cities_birth = {
        {"Moscow"s, 1147},
        {"Saint Petersburg"s, 1703},
        {"Petropavlovsk-Kamchatshiy"s, 1740},
    };
    for (const auto& [name, birth] : cities_birth) {
        cout << name << " is "s << 2020 - birth << " years old"s << endl;
    }
}` 

Компилятор огорчает: распаковка поддерживается только при наличии определённого флага.

Скопировать кодBASH

`$ g++ map.cpp -Wall -Werror -o map
map.cpp: In function ‘int main()’:
map.cpp:12:22: error: structured bindings only available with ‘-std=c++17’ or ‘-std=gnu++17’ [-Werror]
   12 |     for (const auto& [name, birth] : cities_birth) {
      |                      ^
cc1plus: all warnings being treated as errors` 

Добавляем флаг `-std=c++17`, и всё компилируется:

Скопировать кодBASH

`$ g++ map.cpp -Wall -Werror -std=c++17 -o map` 

Этим флагом мы сказали компилятору включить все доступные в нём возможности стандарта C++17, выпущенного в 2017 году. Также можно включить флаги `c++11`, `c++14` и `c++20`. В стандартах C++11 и C++14 ожидаемо меньше возможностей, чем в C++17.

Есть и флаги, запускающие только одну стадию компиляции. В этом случае результатом может быть предварительно обработанный текст программы. Или наоборот: на вход подаётся почти готовая программа, которую человек уже не прочитает, а на выход — привычный исполняемый файл.

# GDB

Компилятор — не единственный инструмент поиска ошибок в коде. Для максимального эффекта используйте и встроенный отладчик своей IDE. Как правило, такие отладчики основаны на консольном отладчике GDB. Научимся работать с ним на примере задачи разделения на слова:

Скопировать кодCPP

`#include <iostream>
#include <string>
#include <vector>
    
using namespace std;

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] == ' ') {
            continue;
        }
        const size_t space_pos = text.find(' ', i);
        if (space_pos == text.npos) {
            words.push_back(text.substr(i));
            break;
        } else {
            words.push_back(text.substr(i, space_pos - i));
            i = space_pos;
        }
    }
    return words;
}

int main() {
    const string query = "hello kitty"s;
    for (const auto& word : SplitIntoWords(query)) {
        cout << '[' << word << ']' << endl;
    }
}` 

Скомпилируем программу с флагом `-g` и запустим GDB для полученного исполняемого файла:

Скопировать кодBASH

`$ g++ gdb_example.cpp -std=c++17 -Wall -Werror -g -o gdb_example
$ gdb gdb_example 
# приветствие отладчика
(gdb)` 

Теперь можно вводить команды. Запустим программу: она выведет слова и завершит работу. Мы ничего не отладили.

Скопировать кодBASH

`(gdb) run
Starting program: /path/to/project/gdb_example 
[hello]
[kitty]
[Inferior 1 (process 18489) exited normally]` 

Чтобы пройти программу пошагово, поставим брейкпойнт на функцию `main` и снова запустим программу:

Скопировать кодBASH

`(gdb) break main
Breakpoint 1 at 0x55555555553a: file gdb_example.cpp, line 25.
(gdb) run
Starting program: /path/to/project/gdb_example 

Breakpoint 1, main () at gdb_example.cpp:25
25    int main() {` 

Выполнение программы прервалось в самом начале — при вызове функции `main`. Отладчик вывел строку, на которой это произошло. Чтобы понять, в каком месте кода мы находимся, выведем соседние строки:

Скопировать кодBASH

`(gdb) list
20            }
21        }
22        return words;
23    }
24    
25    int main() {
26        const string query = "hello kitty"s;
27        for (const auto& word : SplitIntoWords(query)) {
28            cout << '[' << word << ']' << endl;
29        }` 

Напишем команду `next` в консоли и перейдём к следующей строке:

Скопировать кодBASH

`(gdb) next
26        const string query = "hello kitty"s;` 

Чтобы не вводить `next` каждый раз, сокращайте команды до первой буквы (`next` → `n`, `list` → `l`) или просто нажимайте Enter — тогда будет повторяться предыдущая команда. Так можно довольно быстро дойти до конца программы:

Скопировать кодBASH

`(gdb) 
27        for (const auto& word : SplitIntoWords(query)) {
(gdb) 
28            cout << '[' << word << ']' << endl;
(gdb) 
[hello]
27        for (const auto& word : SplitIntoWords(query)) {
(gdb) 
28            cout << '[' << word << ']' << endl;
(gdb) 
[kitty]
27        for (const auto& word : SplitIntoWords(query)) {
(gdb) 
26        const string query = "hello kitty"s;
(gdb) 
30    }
(gdb) 
__libc_start_main (main=0x55555555553a <main()>, argc=1, argv=0x7fffffffdf48, init=<optimized out>, fini=<optimized out>, 
    rtld_fini=<optimized out>, stack_end=0x7fffffffdf38) at ../csu/libc-start.c:342
342    ../csu/libc-start.c: No such file or directory.
(gdb) 
[Inferior 1 (process 18669) exited normally]` 

Видно, что после каждой итерации программа выводила очередное слово.

Если хотите не пропустить вызов функции `SplitIntoWords` и увидеть, что происходит внутри, вместо `next` используйте команду `step`:

Скопировать кодBASH

`(gdb) r
Starting program: /path/to/project/gdb_example 

Breakpoint 1, main () at gdb_example.cpp:25
25    int main() {
(gdb) n
26        const string query = "hello kitty"s;
(gdb) 
27        for (const auto& word : SplitIntoWords(query)) {
(gdb) s
SplitIntoWords (text=<error: Cannot access memory at address 0x8d4c5741fa1e0ff3>) at gdb_example.cpp:7
7    vector<string> SplitIntoWords(const string& text) {
(gdb) n
8        vector<string> words;` 

Вы окажетесь в начале функции `SplitIntoWords`.

Распечатаем содержимое переменной `text`, дав команду `print`:

Скопировать кодBASH

`(gdb) p text
$2 = "hello kitty"` 

Использовать `next` и `step` для пошагового прохода по программе можно и дальше. Добавим брейкпойнт и перейдём напрямую к обработке символа `k`:

Скопировать кодBASH

`(gdb) b 10 if text[i] == 'k'
Breakpoint 2 at 0x5555555553fc: file gdb_example.cpp, line 10.
(gdb) c
Continuing.

Breakpoint 2, SplitIntoWords (text="hello kitty") at gdb_example.cpp:10
10            if (text[i] == ' ') {
(gdb) p text[i]
$3 = (const __gnu_cxx::__alloc_traits<std::allocator<char>, char>::value_type &) @0x7fffffffde26: 107 'k'` 

-   `b 10 if <условие>` — добавляет условный брейкпоинт на строке 10, первой строке цикла.
-   `Breakpoint 2` — вторая точка остановки. Первая стоит на вызове функции `main`.
-   Команда `c` — возобновляет выполнение программы. Брейкпойнт перехватил и приостановил его. Команда `continue` снова запускает программу.
-   `p text[i]` — выводит значение `text[i]`. Это ссылка на символ `'k'`, как и ожидалось.

Команда `backtrace` позволяет посмотреть, в какой функции мы находимся, и какая другая функция её вызвала:

Скопировать кодBASH

`(gdb) backtrace 
#0  SplitIntoWords (text="hello kitty") at gdb_example.cpp:10
#1  0x0000555555555581 in main () at gdb_example.cpp:27` 

Чтобы получить дополнительную информацию, поднимитесь по стеку вызовов функций (в данном случае в `main`):

Скопировать кодBASH

`(gdb) up
#1  0x0000555555555581 in main () at gdb_example.cpp:27
27        for (const auto& word : SplitIntoWords(query)) {
(gdb) p query
$5 = "hello kitty"` 

Этих команд вполне достаточно для полноценной отладки.

Пользователи Linux могут оценить ещё и визуальный режим:

![image](https://pictures.s3.yandex.net/resources/Screenshot_from_2020-08-09_13-10-41_1599727829.png)

Чтобы переключиться в него, используйте сочетание клавиш Ctrl+XA. Вводите XA последовательно при зажатом Ctrl.

Для выхода из отладчика введите команду `q`.