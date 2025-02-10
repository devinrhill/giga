# C++ Coding Style Guide
## 1. Naming Conventions
### Variable Names
    Use camelCase for local variables and instance variables.
    Use descriptive names that clarify the purpose of the variable.
    Examples:
        int userAge;
        float accountBalance;

### Constant Variables
    Use UPPER_CASE_WITH_UNDERSCORES for constants.
    Define constants using const or constexpr.
    Examples:
        const int MAX_USERS = 100;
        constexpr float PI = 3.14159;

    Use CamelCase for class-enum enumerations
    Example:
        enum class MyEnum {
            None = 0,
            Value1 = 1,
            Value2 = 2
        };

### Function Names
    Use camelCase for function names.
    Functions should be named as actions that describe their purpose.
    Examples: 
        void calculateTotal();
        int getUserInput();

### Class Names
    Use PascalCase for class names (each word starts with an uppercase letter).
    Class names should be nouns that represent the concept or entity.
    Examples:
        class UserProfile {};
        class AccountManager {};

### Class member separation
    Use this order:
        - public
        - protected
        - private

### Member Variables
    Prefix private member variables with an underscore (_).
    Examples:
        int _age;
        std::string _name;

    Try to prefix getters and setters with get and set respectively.
    Example:
        class MyClass {
        public:
            int getX() const noexcept;
            void setX(int x) noexcept;

        private:
            int _x;
        };

    Also, keep variables above function declarations.

### Namespaces
    Use lowercase for namespaces, with words separated by underscores.
    Example:
        namespace data_processing {};

    Try to have as little namespaces as possible or only use when necessary.

## 2. Formatting and Indentation
### Indentation
    Use 4 spaces per indentation level (no tabs).

### Line Length
    Keep line length to a maximum of 79 characters for readability and legacy terminal purposes.

### Braces
    Inline Braces: Opening brace should be on the same line as the control statement, and closing brace should be aligned with the start of the statement.
    The same rule applies for opening function braces.
    Example:
        if (condition) {
            // code block
        } else {
            // code block
        }
    
    For switch-statements, open a brace after the case semicolon
    Example:
        switch(someEnum) {
            case Enum::VALUE1: {
                break;
            } case Enum::VALUE2: {
                break;
            }
        }

### Spacing
    Use spaces around operators (e.g., +, -, =, ==).
    Place a space after commas in function calls and definitions.
    Example:
        int sum = a + b;

## 3. Comments
### Single-Line Comments
    Use // for single-line comments to explain code sections, especially for complex logic.
    Example:
        // Calculate the user's total balance

### Block Comments
    Use /* */ for multi-line comments only when necessary, and avoid using them within code blocks.

### Documentation Comments
    Use /// or /** */ for documenting functions, parameters, and return values.
    Example:
        /**
         * Calculates the area of a rectangle.
         * @param width Width of the rectangle.
         * @param height Height of the rectangle.
         * @return Area of the rectangle.
         */
        double calculateArea(double width, double height);

## 4. Code Organization
### Header Files
        Use header guards to prevent multiple inclusions.
        Keep class declarations in header files (.h) and function implementations in source files (.cpp).
        Example:
            #ifndef USER_PROFILE_H
            #define USER_PROFILE_H
            class UserProfile { /* class definition */ };
            #endif

### Order of Declarations
    Organize code logically:
        Includes (#include statements)
        Namespace declarations
        Class definitions
        Function definitions

## 5. Error Handling
### Use Exceptions for Error Handling
    Prefer throwing exceptions for handling errors instead of returning error codes.
    Example:
        void processUserData() {
            if (data.empty()) {
                throw std::runtime_error("Data cannot be empty");
            }
        }

    ALWAYS indicate to the end-user in some way, whether it be a message to stdout,
    a return-code, etc. the reason of error, and the conditions involved in producing the error.

    If the error is caused by the user, e.g. passing the name of a file which does not exist,
    prefer exceptions.

### Avoid Catching General Exceptions
    Catch specific exceptions whenever possible to avoid hiding errors.
    Example:
        try {
            // code that may throw
        } catch (const std::out_of_range& e) {
            // handle out-of-range error
        }

## 6. Function Practices
### Function Length
    Keep functions short, ideally under 40–50 lines. If a function is too long, consider splitting it into smaller helper functions.

### Parameter Passing
    Pass large objects (like structs, classes) by const reference to avoid copying.
    Example: void processUser(const User& user);

### Return Values
    Prefer returning meaningful values instead of using void, especially if the function’s success or output can be checked.

### Use auto for Type Inference Carefully
    Use auto only when the type is clear from context, as overusing it can reduce code readability.

## 7. Standard Library and Modern C++
### Use nullptr Instead of NULL or 0
    nullptr is type-safe and specific to pointers in C++.

### Use std::unique_ptr and std::shared_ptr for Memory Management
    Prefer smart pointers over raw pointers for managing dynamic memory.

### Range-Based Loops
    Use range-based for-loops when iterating over collections to improve readability and avoid errors.
    Example:
        for(const auto& item: collection) {
            // process item
        }

### Avoid Using using namespace std;
    It’s better to qualify standard library names explicitly to avoid naming conflicts.

## 8. File Naming
    Use snake_case for filenames, with lowercase letters and underscores between words.
    Header files should end with .h and and source files with .cpp.
    Example: user_profile.h, account_manager.cpp
