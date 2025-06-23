#include <iostream>
#include <string>
#include <concepts>
#include <type_traits>

// Концепт ComplexConcept
template<typename T>
concept ComplexConcept =
    requires(T a) {
        // Метод hash() возвращает тип, конвертируемый в long
        { a.hash() } -> std::convertible_to<long>;

        // Метод toString() возвращает std::string
        { a.toString() } -> std::same_as<std::string>;
    } &&
    // У типа не должен быть виртуальный деструктор
    (!std::has_virtual_destructor_v<T>);

// Класс, удовлетворяющий концепту
struct GoodType {
    long hash() const { return 123456; }
    std::string toString() const { return "GoodType"; }
};

// Класс, нарушающий концепт (есть виртуальный деструктор)
struct BadType1 {
    virtual ~BadType1() = default;
    long hash() const { return 0; }
    std::string toString() const { return "BadType1"; }
};

// Класс, нарушающий концепт (toString возвращает const char*)
struct BadType2 {
    long hash() const { return 0; }
    const char* toString() const { return "BadType2"; }
};

// Функция, использующая концепт
template<ComplexConcept T>
void process(const T& obj) {
    std::cout << "Hash: " << obj.hash() << "\n";
    std::cout << "String: " << obj.toString() << "\n";
}

int main() {
    GoodType good;
    process(good); // Работает

    // Следующие строки не скомпилируются, если раскомментировать:
    // BadType1 bad1;
    // process(bad1); // Ошибка: есть виртуальный деструктор

    // BadType2 bad2;
    // process(bad2); // Ошибка: toString не возвращает std::string

    return 0;
}
