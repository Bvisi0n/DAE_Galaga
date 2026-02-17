# Naming Conventions

## Types & Functions
- **Class/Struct**: PascalCase
- **Function names**: PascalCase

## Variables
- **Function parameters**: camelCase
- **Member variables**: m_camelCase
- **Local variables**: snake_case
- **global variables**: g_camelCase
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
- **Variable initialization**: Uniform Initialization
```
int x{ 0 };
```
- **Class layout**: Public > Protected > Private
```
class MyClass
{
public:
    // public members
    // public functions
protected:
    // protected members
    // protected functions
private:
    // friends
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
