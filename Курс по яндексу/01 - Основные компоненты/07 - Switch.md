В коде с перечислением мы несколько раз используем `if`. Выглядит это громоздко:

Скопировать кодCPP

`enum class RequestType {
      ADD,
      REMOVE,
      NEGATE
};

void ProcessRequest(
            set<int>& numbers,
            RequestType request_type,
            int request_data) {
      if (request_type == RequestType::ADD) {
            numbers.insert(request_data);
      } else if (request_type == RequestType::REMOVE) {
            numbers.erase(request_data);
      } else if (request_type == RequestType::NEGATE) {
            if (numbers.count(request_data) == 1) {
                  numbers.erase(request_data);
                  numbers.insert(-request_data);
            }
      }
}

ProcessRequest(numbers, RequestType::ADD, 8);
ProcessRequest(numbers, RequestType::NEGATE, 8);
ProcessRequest(numbers, RequestType::REMOVE, -8);` 

Но нам повезло: в C++ есть компактная альтернатива цепочке условных операторов — оператор `switch`. В отличие от `if`, `switch` не может проверять произвольные логические выражения. Он позволяет сравнить заданную переменную или результат выражения с конкретными значениями и выполнить действия в зависимости от того, с каким значением произошло совпадение. Используем оператор `switch` и перепишем функцию `ProcessRequest`:

Скопировать кодCPP

`void ProcessRequest(
            set<int>& numbers,
            RequestType request_type,
            int request_data) {
      switch (request_type) {
      case RequestType::ADD:
            numbers.insert(request_data);
            break;
      case RequestType::REMOVE:
            numbers.erase(request_data);
            break;
      case RequestType::NEGATE:
            if (numbers.count(request_data) == 1) {
                  numbers.erase(request_data);
                  numbers.insert(-request_data);
            }
            break;
      }
}` 

Каждая ветка кончается оператором `break`. Фигурные скобки ей не нужны. Оператор `break` означает выход из оператора `switch` и не повлечёт за собой выход из объемлющего цикла `for` при его наличии.

Компактный аналог `else` для оператора `switch` — ветка `default`. Допустим, мы хотим перестраховаться от добавления новых типов запросов и добавить вывод предупреждающего сообщения для неизвестного запроса. `default`-ветка выполнится всегда, если не подошла ни одна `case`-ветка:

Скопировать кодCPP

`void ProcessRequest(
            set<int>& numbers,
            RequestType request_type,
            int request_data) {
      switch (request_type) {
      case RequestType::ADD:
            numbers.insert(request_data);
            break;
      case RequestType::REMOVE:
            numbers.erase(request_data);
            break;
      case RequestType::NEGATE:
            if (numbers.count(request_data) == 1) {
                  numbers.erase(request_data);
                  numbers.insert(-request_data);
            }
            break;
      default:
            cout << "Unknown request"s << endl;
      }
}` 

Если объявляем переменную в одной из веток оператора `switch`, всю ветку заключаем в блок из фигурных скобок:

Скопировать кодCPP

`void ProcessRequest(
            set<int>& numbers,
            RequestType request_type,
            int request_data) {
      switch (request_type) {
      case RequestType::ADD:
            numbers.insert(request_data);
            break;
      case RequestType::REMOVE:
            numbers.erase(request_data);
            break;
      case RequestType::NEGATE: {  // фигурные скобки обязательны
            bool contains = numbers.count(request_data) == 1;
            if (contains) {
              numbers.erase(request_data);
              numbers.insert(-request_data);
            }
            break;
      }
      default:
            cout << "Unknown request" << endl;
      }
}` 

В нашем случае нумерация перечислителей в наборе не имела значения. Но когда числовые значения элементов `enum` важны, их можно указать явно:

Скопировать кодCPP

`enum class RequestType {
    ADD = 9,
    REMOVE = 8,
    NEGATE = 7
};` 

Это удобно, когда типы запросов поступают на вход программы в виде чисел 9, 8 и 7. Тогда получить объект типа `RequestType` по его числовому коду можно будет простым оператором `static_cast`:

Скопировать кодCPP

`int request_code;
cin >> request_code;
auto request_type = static_cast<RequestType>(request_code);
// Если ввести request_code = 7,
// в переменной request_type окажется RequestType::NEGATE` 

Если преобразуемому числовому коду не будет соответствовать ни один элемент перечислимого типа, ошибки компиляции не произойдёт. Но дальнейшее поведение программы грозит стать непредсказуемым.