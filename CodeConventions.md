# Naming Conventions

## Types & Functions
- **Class / Struct / Enum**: PascalCase
- **Function()**: PascalCase

## Scope Variables
- **Local variables**: camelCase
- **Function parameters**: camelCase

## Member Variables
- **Member variables**: m_camelCase

## Static / Global / Constant Variables
- **Compile-time Constants (constexpr)**: c_PascalCase
- **Static Class Members**: s_camelCase
- **Global variables**: g_camelCase

## Namespaces
- **Namespaces**: snake_case


# Style & Formatting

## General
- **Braces**: Allman Style
```
if (condition)
{
    // code
}
else
{
    // code
}
```
- **Variable initialization**: Enforced Uniform Initialization where possible
```
int x{ 0 };
```
- **Class layout**: Public > Protected > Private
```
class MyClass
{
    // friend classes
public:
    // public members
    // public functions
protected:
    // protected members
    // protected functions
private:
    // private members
    // private functions
};
```

## Preprocessor
- **header guards**: Handmade & ALL_CAPS_H
```
#ifndef FILE_NAME_H
#define FILE_NAME_H
#endif
```
- **Preprocessor logic**: Indent unless it's a header guard
```
void LogSDLVersion(const std::string& message, int major, int minor, int patch)
{
    #if WIN32
        std::stringstream ss;
        ss << message << major << "." << minor << "." << patch << "\n";
        OutputDebugString(ss.str().c_str());
    #else
        std::cout << message << major << "." << minor << "." << patch << "\n";
    #endif
}
```
