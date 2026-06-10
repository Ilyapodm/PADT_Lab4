# Лабораторная работа №4 по ПАТД

**Вариант 14:** умный генератор тестовых данных.

Проект реализует систему ленивых последовательностей, потоков ввода/вывода и генераторов тестовых данных. Главная идея архитектуры — разделить три разные ответственности:

```text
Generator<T>       -> порождает значения последовательно
Node<T>            -> маршрутизирует индексный доступ и операции над LazySequence
Stream<T>          -> выполняет последовательное чтение или запись
```

`LazySequence<T>` не является генератором и не является потоком. Это фасад над деревом `Node<T>`. Генераторы только создают значения, `SourceNode<T>` кэширует уже материализованные элементы, а операционные узлы (`ConcatNode`, `SubsequenceNode`, `AppendNode` и другие) лениво описывают преобразования последовательности.

---

## Содержание

- [Возможности проекта](#возможности-проекта)
- [Сборка и запуск](#сборка-и-запуск)
- [Структура проекта](#структура-проекта)
- [Архитектурная схема](#архитектурная-схема)
- [Ordinal](#ordinal)
- [Генераторы](#генераторы)
- [SmartDataGenerator](#smartdatagenerator)
- [Node-архитектура](#node-архитектура)
- [LazySequence](#lazysequence)
- [Streams](#streams)
- [Console UI](#console-ui)

---

## Возможности проекта

Реализованы:

- ограниченные ординалы вида `ω * k + n`;
- ленивые генераторы данных;
- `LazySequence<T>` поверх дерева `Node<T>`;
- операции над ленивыми последовательностями без полной материализации;
- потоки ввода и вывода для обычных `Sequence<T>`;
- специализированные потоки ввода и вывода для `LazySequence<T>`;
- умный генератор тестовых данных `SmartDataGenerator`;
- консольный UI для ручной проверки структур.

Основные операции `LazySequence<T>`:

```text
get
append
prepend
insert_at
set
remove_at
concat
get_subsequence
```

---

## Сборка и запуск

Проект собирается через CMake. Рекомендуемый генератор сборки — Ninja.

### 1. Конфигурация

```bash
cmake -S . -B build -G Ninja
```

### 2. Сборка

```bash
cmake --build build
```

### 3. Запуск консольного приложения

```bash
./build/src/lab_main
```

На Windows:

```powershell
.\build\src\lab_main.exe
```

### 4. Запуск тестов

Универсальный способ через CTest:

```bash
ctest --test-dir build --output-on-failure
```

macOS / Linux:

```bash
./build/tests/lab_tests
```

Windows:

```powershell
.\build\tests\lab_tests.exe
```

### 5. Полная пересборка

```bash
rm -rf build
cmake -S . -B build -G Ninja
cmake --build build
```

На Windows PowerShell:

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build -G Ninja
cmake --build build
```

---

## Структура проекта

Актуальная структура проекта логически делится на несколько независимых слоёв:

```text
.
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/
│   ├── adt_lab_2/
│   │
│   ├── core/
│   │   └── ordinal.hpp
│   │
│   ├── adt/
│   │   └── ring_buffer.hpp
│   │
│   ├── generators/
│   │   ├── generator.hpp
│   │   ├── empty_generator.hpp
│   │   ├── singleton_generator.hpp
│   │   ├── sequence_generator.hpp
│   │   ├── function_generator.hpp
│   │   └── recurrence_generator.hpp
│   │
│   ├── nodes/
│   │   ├── node.hpp
│   │   ├── source_node.hpp
│   │   ├── concat_node.hpp
│   │   ├── subsequence_node.hpp
│   │   ├── set_node.hpp
│   │   ├── insert_at_node.hpp
│   │   ├── remove_at_node.hpp
│   │   ├── append_node.hpp
│   │   └── prepend_node.hpp
│   │
│   ├── lazy_sequence/
│   │   ├── lazy_sequence.tpp
│   │   └── lazy_sequence.hpp
│   │
│   ├── streams/
│   │   ├── input_stream.hpp
│   │   ├── output_stream.hpp
│   │   ├── sequence_input_stream.hpp
│   │   ├── sequence_output_stream.hpp
│   │   ├── lazy_sequence_input_stream.hpp
│   │   └── lazy_sequence_output_stream.hpp
│   │
│   └── smart_data/
│       ├── distribution.hpp
│       ├── uniform_distribution.hpp
│       ├── normal_distribution.hpp
│       ├── smart_data_config.hpp
│       └── smart_data_generator.hpp
│
├── src/
│   ├── main.cpp
│   │
│   └── console/
│        ├── console_io.hpp
│        ├── ui_records.hpp
│        ├── console_ui.hpp
│        └── console_ui.cpp
│
└── tests/
```

---

## Архитектурная схема

### LazySequence

```text
Generator<T>
    ↓
SourceNode<T>
    ↓
Operation Node<T>
    ↓
LazySequence<T>
```

`Generator<T>` выдаёт значения только последовательно через `get_next()`.

`SourceNode<T>` превращает генератор в индексный источник и хранит плотный префикс-кэш уже вычисленных элементов.

Операционные узлы (`ConcatNode`, `SubsequenceNode`, `SetNode`, `InsertAtNode`, `RemoveAtNode`, `AppendNode`, `PrependNode`) не материализуют результат. Они только перенаправляют индекс в нужный дочерний узел.

`LazySequence<T>` — внешний фасад. Он владеет корневым узлом и делегирует ему доступ к данным.

### Streams

```text
Sequence<T> / LazySequence<T>
    ↓
SequenceInputStream<T>

Sequence<T>
    ↑
SequenceOutputStream<T>

LazySequence<T>
    ↓
LazySequenceInputStream<T>

LazySequence<T>
    ↑
LazySequenceOutputStream<T>
```

Обычные sequence-потоки используют конечные позиции `std::size_t`.

Lazy-sequence-потоки используют `Ordinal`, поэтому могут переходить к позициям вида `ω`, `ω + n`, `ω * k + n`.

---

## Ordinal

`Ordinal` хранит ограниченные ординалы вида:

```text
ω * k + n
```

где:

- `k` — количество бесконечных блоков;
- `n` — конечная часть.

Примеры:

```text
0
5
ω
ω + 3
ω * 2 + 4
```

### Сложение

Сложение ординалов некоммутативно:

```text
5 + ω = ω
ω + 5 != 5 + ω
ω + 5 + ω + 3 = ω * 2 + 3
```

Если справа добавляется бесконечная часть, конечный хвост слева поглощается.

### Сравнение

Сравнение выполняется лексикографически:

```text
сначала omega_coeff
потом finite_part
```

Например:

```text
7 < ω
ω + 3 < ω + 5
ω + 100 < ω * 2
```

### subtract_prefix

`subtract_prefix(prefix)` — это не обычное арифметическое вычитание, а вычисление локального индекса после префикса.

Он ищет такой `x`, что:

```text
prefix + x == current
```

Используется, например, в `ConcatNode`, чтобы перевести глобальный индекс правой последовательности в локальный индекс.

Примеры:

```text
(7).subtract_prefix(3) = 4
(ω + 5).subtract_prefix(ω) = 5
(ω + 7).subtract_prefix(ω + 5) = 2
(ω * 2 + 4).subtract_prefix(ω + 7) = ω + 4
```

Если `prefix` больше текущего ординала, выбрасывается исключение.

---

## Генераторы

### Generator

`Generator<T>` — базовая абстракция для ленивого порождения элементов.

Основные методы:

```text
has_next()  -> есть ли следующий элемент
get_next()  -> вернуть следующий элемент и сдвинуть состояние
length()    -> длина источника как Ordinal
clone()     -> копия генератора в текущем состоянии
```

`clone()` нужен потому, что производные ленивые последовательности должны быть независимыми от исходных.

### EmptyGenerator

`EmptyGenerator<T>` представляет пустой источник длины `0`.

Свойства:

- `has_next()` всегда возвращает `false`;
- `get_next()` выбрасывает исключение;
- `length()` возвращает `0`.

Используется как нулевой объект для пустой `LazySequence`.

### SingletonGenerator

`SingletonGenerator<T>` представляет источник из одного элемента.

Свойства:

- `length()` возвращает `1`;
- `get_next()` один раз возвращает сохранённое значение, затем выбрасывает исключение;
- `clone()` копирует значение и состояние чтения.

### SequenceGenerator

`SequenceGenerator<T>` адаптирует обычный конечный `Sequence<T>` к интерфейсу `Generator<T>`.

Он делает snapshot входной последовательности. После создания генератора исходная sequence больше не нужна.

Свойства:

- `get_next()` читает элементы snapshot-копии по порядку;
- `length()` равна размеру snapshot-копии;
- `clone()` копирует snapshot и текущий индекс чтения.

Важно: `SequenceGenerator` предназначен для обычных конечных `Sequence<T>`. Для бесконечной `LazySequence<T>` snapshot через `get_size()` невозможен.

### FunctionGenerator

`FunctionGenerator<T>` создаёт последовательность по правилу:

```text
a_i = f(i)
```

Особенности:

- хранит функцию, длину и текущий индекс чтения;
- `get_next()` вычисляет следующий элемент и сдвигает индекс;
- поддерживает конечную длину или ровно `ω`;
- не хранит полную историю значений.

Примеры:

```text
a_i = i
a_i = i * i
a_i = 2 * i + 1
```

### RecurrenceGenerator

`RecurrenceGenerator<T>` создаёт последовательность по рекуррентному правилу.

Он хранит:

- функцию перехода;
- окно последних `k` элементов;
- текущий индекс чтения;
- длину последовательности.

`get_next()` сначала возвращает начальные значения, затем вычисляет следующий элемент по окну и сдвигает окно через `RingBuffer`.

Генератор не хранит всю историю значений и не выполняет полную мемоизацию. Полный кэш материализованных элементов находится в `SourceNode<T>`.

### RingBuffer

`RingBuffer<T>` — фиксированный кольцевой буфер для `RecurrenceGenerator<T>`.

Он хранит окно последних `k` значений без физического сдвига элементов. При добавлении нового значения самый старый элемент перезаписывается, а индекс начала окна сдвигается по кругу.

Свойства:

- `get(0)` возвращает самый старый элемент окна;
- `get(size - 1)` возвращает самый новый элемент;
- обновление окна выполняется за `O(1)`;
- память фиксирована и равна `O(k)`.

---

## SmartDataGenerator

`SmartDataGenerator` — генератор тестовых данных для варианта 14.

Он реализован как обычный `Generator<int>` и поэтому может быть обёрнут в `LazySequence<int>` через конструктор:

```cpp
LazySequence(const Generator<T>& generator)
```

Общая схема:

```text
SmartDataConfig
    ↓
SmartDataGenerator
    ↓
Distribution<int>
    ↓
SourceNode<int>
    ↓
LazySequence<int>
```

### SmartDataConfig

`SmartDataConfig` — простая структура параметров. Она не хранит состояние генерации.

Основные поля:

```text
distribution_type       Uniform или Normal
length                  конечная длина или ω
seed                    начальное значение для random engine
min_value               нижняя граница результата
max_value               верхняя граница результата
mean                    среднее значение для normal distribution
standard_deviation      стандартное отклонение для normal distribution
```

Для генераторов допустима только длина:

```text
finite n
ω
```

Длины вида `ω + n`, `ω * 2`, `ω * k + n` создаются не генератором, а операциями над `LazySequence`, например через `concat` или `append`.

### SmartDataGenerator

`SmartDataGenerator` хранит:

```text
SmartDataConfig config_
std::mt19937 engine_
std::size_t index_
Distribution<int>* distribution_
```

`seed` и `engine_` обеспечивают воспроизводимость:

```text
один config + один seed -> одна и та же последовательность
```

`clone()` копирует:

- config;
- текущее состояние `std::mt19937`;
- текущий индекс;
- distribution вместе с её внутренним состоянием.

Это важно: если генератор уже выдал 10 элементов, clone должен продолжать с 11-го элемента, а не начинать заново.

### Distribution

`Distribution<int>` — полиморфный слой, который превращает состояние random engine в полезное значение типа `int`.

```text
std::mt19937 -> Distribution<int> -> int
```

Реализованы:

- `UniformDistribution`;
- `NormalDistribution`.

### UniformDistribution

`UniformDistribution` генерирует целые значения из диапазона `[min_value, max_value]` так, чтобы каждое значение было примерно равновероятным.

Для устранения modulo bias используется rejection sampling, а не простой `% range`.

### NormalDistribution

`NormalDistribution` генерирует значения около `mean` со стандартным отклонением `standard_deviation`.

Математическая модель:

```text
X = mean + standard_deviation * Z
Z ~ N(0, 1)
```

Стандартное нормальное значение `Z` получается через преобразование Box-Muller. Поскольку Box-Muller за один раз создаёт два значения, второе сохраняется внутри distribution как `spare`. Поэтому `clone()` distribution должен копировать не только параметры, но и внутреннее состояние.

Результат округляется до `int` и ограничивается диапазоном `[min_value, max_value]`. Поэтому это не чистое бесконечное normal distribution, а clamped normal distribution, что удобно для генерации тестовых массивов.

---

## Node-архитектура

### Node

`Node<T>` — абстрактный узел ленивой последовательности.

Он не является генератором и не выполняет последовательное чтение. `Node` отвечает за доступ к значению по ординальному индексу и за маршрутизацию индексов внутри операций.

Основная идея:

- `LazySequence` хранит корневой `Node`;
- операции создают новые узлы;
- каждый узел знает свою `Ordinal`-длину;
- `value_at(index)` возвращает значение по индексу;
- `clone()` создаёт независимую копию узла вместе с его кэшем.

### SourceNode

`SourceNode<T>` — базовый узел-источник для `LazySequence<T>`.

Он оборачивает `Generator<T>` и хранит собственный плотный кэш уже вычисленных значений.

Инвариант:

```text
число элементов в cache == число уже вызванных generator.get_next()
```

Если запрошенный элемент уже есть в кэше, он возвращается сразу. Если элемента нет, `SourceNode` доращивает кэш через `generator_->get_next()`.

### ConcatNode

`ConcatNode<T>` представляет конкатенацию двух узлов.

```text
length = left.length + right.length
```

Маршрутизация:

```text
if index < left.length:
    return left[index]
else:
    return right[index.subtract_prefix(left.length)]
```

Поддерживаемые случаи:

```text
finite + finite -> finite
finite + ω      -> ω
ω + finite      -> ω + finite
ω + ω           -> ω * 2
```

### SubsequenceNode

`SubsequenceNode<T>` представляет диапазон `[start, end_exclusive)` исходного узла.

```text
sub[i] = source[start + i]
```

Узел не материализует элементы и не хранит собственный плоский кэш.

### SetNode

`SetNode<T>` представляет ленивую замену одного элемента.

```text
set[i] = value,     если i == index
set[i] = source[i], иначе
```

Длина последовательности не меняется.

### InsertAtNode

`InsertAtNode<T>` представляет ленивую вставку одного значения.

```text
i < insert_index   -> source[i]
i == insert_index  -> inserted_value
i > insert_index   -> source[shifted_i]
```

Для конечного блока после вставки индекс сдвигается назад на `1`. Через `ω`-границы обычный finite-сдвиг не применяется.

### RemoveAtNode

`RemoveAtNode<T>` представляет ленивое удаление одного элемента.

```text
result[i] = source[i],     если i < removed_index
result[i] = source[i + 1], если i >= removed_index в том же блоке
```

Удаление конечного элемента из бесконечного блока не меняет длину `ω`. Удаление элемента из конечного хвоста уменьшает конечную часть на `1`.

### AppendNode

`AppendNode<T>` добавляет один элемент в конец.

```text
append_index = source.length
```

Для конечной последовательности длина увеличивается на `1`. Для бесконечной последовательности добавленный элемент находится на индексе `ω`, поэтому длина становится `ω + 1`.

### PrependNode

`PrependNode<T>` добавляет один элемент в начало.

```text
prepended[0] = value
prepended[i] = source[i - 1], если i конечный и i > 0
prepended[ω + n] = source[ω + n]
```

Для конечной последовательности длина увеличивается на `1`. Для бесконечной последовательности левая конечная добавка не меняет ординальную длину: `1 + ω = ω`.

---

## LazySequence

`LazySequence<T>` — фасад над деревом `Node<T>`.

`LazySequence<T>` хранит:

```text
Node<T>* root_node_
```

Делегируемые операции:

```text
get(int index)              -> root_node_->value_at(Ordinal(index))
get(Ordinal index)          -> root_node_->value_at(index)
get_size()                  -> finite size или exception
get_ordinal_length()        -> root_node_->length()
get_materialized_count()    -> число материализованных элементов в источниках
```

Поддерживаемые источники:

- пустая последовательность;
- массив `T*`;
- обычная конечная `Sequence<T>`;
- любой `Generator<T>`;
- `FunctionGenerator<T>`;
- `RecurrenceGenerator<T>` через `RingBuffer<T>`;
- `RecurrenceGenerator<T>` через массив последних значений;
- `SmartDataGenerator` через общий конструктор от `Generator<T>`.

Операции:

```text
append      -> AppendNode
prepend     -> PrependNode
set         -> SetNode
insert_at   -> InsertAtNode
remove_at   -> RemoveAtNode
concat      -> ConcatNode
subsequence -> SubsequenceNode
```

Операции не меняют исходную последовательность. Они возвращают новый `LazySequence<T>`.

Копирование `LazySequence<T>` выполняется через `Node::clone()`, поэтому производные последовательности независимы от исходных и не шарят кэш.

---

## Streams

В проекте есть две группы потоков:

```text
потоки для обычных Sequence<T>
потоки для LazySequence<T>
```

### InputStream

`InputStream<T>` — абстракция последовательного чтения.

Основные методы:

```text
open()
close()
reset()
is_open()
is_end_of_stream()
input()
get_position()
seek(index)
can_seek()
can_go_back()
```

### OutputStream

`OutputStream<T>` — абстракция последовательной записи.

Основные методы:

```text
open()
close()
is_open()
output(value)
get_position()
release_sequence()
```

### SequenceInputStream

`SequenceInputStream<T>` работает поверх `Sequence<T>` через `IEnumerator<T>`.

Он подходит для:

- `MutableArraySequence<T>`;
- `ImmutableArraySequence<T>`;
- `MutableListSequence<T>`;
- `ImmutableListSequence<T>`;
- конечного или начального finite-префикса `LazySequence<T>`.

Позиция потока — обычный `std::size_t`.

### SequenceOutputStream

`SequenceOutputStream<T>` пишет в обычную `Sequence<T>` через `append`.

Для mutable-последовательностей `append` изменяет объект на месте. Для immutable-последовательностей `append` создаёт новую версию, которой поток начинает владеть.

Для получения результата используется:

```text
release_sequence()
```

После `release_sequence()` поток больше не должен использоваться.

### LazySequenceInputStream

`LazySequenceInputStream<T>` работает напрямую поверх `LazySequence<T>`.

Главное отличие — позиция хранится как `Ordinal`, поэтому поток поддерживает переходы через ординальные границы.

Примеры:

```text
seek(5)       -> конечная позиция 5
seek(ω)       -> первый элемент после бесконечного блока
seek(ω + 3)   -> элемент с индексом ω + 3
```

Обычный `get_position()` работает только для конечной позиции. Для полной позиции используется:

```text
get_ordinal_position()
```

`input()` читает элемент через:

```text
source.get(position)
```

и сдвигает позицию на `1`.

### LazySequenceOutputStream

`LazySequenceOutputStream<T>` пишет в `LazySequence<T>` и хранит позицию как `Ordinal`.

При `open()` поток становится в конец последовательности:

```text
position = target.get_ordinal_length()
```

Поэтому поток корректно работает с длинами:

```text
finite n
ω
ω + n
ω * k + n
```

Для полной поддержки ординальных позиций используется:

```text
output_ordinal(value)
```

Обычный `output(value)` подходит только для конечной позиции, потому что возвращает `std::size_t`.

Как и остальные операции над `LazySequence<T>`, запись не изменяет исходную последовательность. Поток получает новую версию через `append` и продолжает работать с ней.

Итоговая версия забирается через:

```text
release_sequence()
```

После release поток отключается.

---

## Console UI

Консольный интерфейс предназначен для ручной проверки всех основных структур.

UI хранит объекты в отдельных registry, потому что разные типы имеют разные интерфейсы и разные правила владения памятью.

Основные registry:

```text
ordinary sequence registry
lazy sequence registry
smart data config registry
sequence input stream registry
lazy sequence input stream registry
sequence output stream registry
lazy sequence output stream registry
```

Для обычных sequence хранится metadata:

```text
Array или List
Mutable или Immutable
описание
состояние alive/deleted
```

Для lazy sequence хранится metadata происхождения:

```text
empty
array input
sequence snapshot
function preset
recurrence preset
smart data config
append/prepend/set/insert/remove/subsequence/concat
output stream release
```

Удаление выполняется через soft-delete:

```text
id не сдвигаются
удалённый объект остаётся как deleted record
новые объекты добавляются в конец registry
```

Нельзя удалить sequence или lazy sequence, пока на неё ссылается живой stream.

---