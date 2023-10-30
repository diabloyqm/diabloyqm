// #include <iostream>
// #include <algorithm> // For std::copy

// template <typename T>
// class MyVector {
// private:
//     T* array;       // Pointer to the dynamically allocated array
//     size_t size;     // Number of elements in the vector
//     size_t capacity; // Capacity of the allocated array

// public:
//     // Constructor
//     MyVector() : array(nullptr), size(0), capacity(0) {}

//     // Destructor
//     ~MyVector() {
//         delete[] array;
//     }

//     // Function to add an element to the end of the vector
//     void push_back(const T& value) {
//         if (size >= capacity) {
//             // If the array is full, double its capacity
//             capacity = (capacity == 0) ? 1 : capacity * 2;
//             T* newArray = new T[capacity];

//             // Copy existing elements to the new array
//             std::copy(array, array + size, newArray);

//             // Delete the old array
//             delete[] array;

//             // Update the pointer to the new array
//             array = newArray;
//         }

//         // Add the new element to the end
//         array[size] = value;
//         size++;
//     }

//     // Function to access an element by index
//     T& operator[](size_t index) {
//         if (index < size) {
//             return array[index];
//         } else {
//             // Handle out-of-bounds access (this is a simple example)
//             throw std::out_of_range("Nooooo");
//         }
//     }

//     // Function to get the size of the vector
//     size_t getSize() const {
//         return size;
//     }
// };

// int main() {
//     // Create an instance of the custom vector for integers
//     MyVector<int> myVector;

//     // Add elements to the vector
//     myVector.push_back(1);
//     myVector.push_back(2);
//     myVector.push_back(3);

//     // Accessing elements
//     std::cout << "Elements of the vector: ";
//     for (size_t i = 0; i < myVector.getSize(); ++i) {
//         std::cout << myVector[i] << " ";
//     }
//     std::cout << std::endl;

//     return 0;
// }

#include <iostream>
template <typename T>
class Singleton {
    public:
    Singleton() = default;
    ~Singleton() = default;
    void display_log(){
        std::cout<< "I am a singleton" << std::endl;
    }

    static T& getInstance() {
        static T instance;
        return instance;
    }
};
class Logger:public Singleton<Logger>
{
    friend class Singleton<Logger>;
private:
    Logger(){}; 
    ~Logger(){};
public:
    void display_log(std::string logtext){
        std::cout << logtext << std::endl;
        static_cast<Singleton<Logger> *>(this)->display_log();
    }
};

Logger * f1()
{
    Logger::getInstance().display_log("display log from f1");
    return &Logger::getInstance();
}

Logger * f2()
{
    Logger::getInstance().display_log("display log from f2");
    return &Logger::getInstance();
}

int main(void)
{ 
   
    Logger::getInstance().display_log("Try to make it work");      // use Logger
    std::cout << (f1()==f2()) << std::endl;
    return 0;
}