# fixed_string
compile-time size string class

# What is fixed_string

This class is an STL-like wrapper for C-style strings. It maintains a fixed-size character array
and provides general string utilities, like replacing characters, filling it, adding two strings,
finding characters. 

fixed_string is guaranteed to be null terminated at all times, unless the user modifies the value at the end() iterator, 
or accesses the last index of the array and mofidies it.
```cpp
fixed_string<16> buf;
buf[16] = 'X' //BAD! fixed_string is no longer null terminated
```

If you find a case where fixed_string is not null terminated, please open an issue so it can be fixed.

# Usage

The header provides the `basic_fixed_string` class. It also provides the following typedefs:

```cpp
template<unsigned N>
using fixed_string = basic_fixed_string<N, char>;
using fixed_string_16 = fixed_string<16>;
using fixed_string_32 = fixed_string<32>;
using fixed_string_64 = fixed_string<64>;
using fixed_string_128 = fixed_string<128>;
using fixed_string_256 = fixed_string<256>;

template<unsigned N>
using fixed_wstring = basic_fixed_string<N, wchar_t>;
using fixed_wstring_16 = fixed_wstring<16>;
using fixed_wstring_32 = fixed_wstring<32>;
using fixed_wstring_64 = fixed_wstring<64>;
using fixed_wstring_128 = fixed_wstring<128>;
using fixed_wstring_256 = fixed_wstring<256>;
```

# Examples

**Constructors**

```cpp
fixed_string<16> empty; //Fills the string with 16 '\0' characters
fixed_string<16> fromCharArray = "ThisIsAString";
fixed_string<16> fromStdString = std::string("Hello World");
fixed_string<16> copied = fromStdString;
fixed_string<32> copiedWithDifferentSize = copied;
fixed_string<32> copiedWithMove = std::move(copied);
```

**Element access**

```cpp
fixed_string<32> str = "This is a string";

//[] does no bounds checking, just like with std::string
std::cout << str[2];
str[3] = 'X';

try
{
  str.at(512) = 'X'; //out of range, at() throws an std::out_of_range
}
catch(std::out_of_range& e)
{
  std::cout << "error: " << e.what() << '\n';
}

str.front() = 'X'; //returns a reference to the first element of the string
str.back() = 'V'; //returns a reference to the last element of the string
```

**Size**

```cpp
fixed_string<32> str = "This is a string";

std::cout << str.size(); //prints '31'. This is important to note, all size access functions do not count the null terminator

//Note that a call to size() is the same as calling length() or max_size()

fixed_string<32> str;
if (str.empty())
{
  //empty() returns true if the string is filled with null characters
}
```

**Iterators**

```cpp
fixed_string<32> str = "This is a string";

//prints: 'This is a string'
for(auto& it = str.begin(); it != str.end(); ++it)
{
  std::cout << *it; 
}

//Because fixed_string has begin() and end() functions, it also supports range-based for loops!
for(auto const& it : str)
{
  std::cout << *it;
}
```

**Modifiers**

```cpp
fixed_string<32> str;

str.fill('P'); //str is now 32 'P' characters
str.fill(16, 'S'); //starting from index 16, to the end, str is filled with 'S' characters

str.clear(); //str is now 32 '\0' characters

str.replace(2, 5, 'Z'); //the range of indices [2, 5] is now replaced with 'Z' characters

//replace() can be chained, as it returns *this
str.replace(str.begin() + 6, str.begin() + 12, 'D').replace(0, 3, "Hiya");
//after this replace call, string will look like this:
"Hiya\0\0DDDDDD\0\0..."

fixed_string<32> otherStr;
otherStr.swap(str); //swaps contents
```

**Basic utilities**

```cpp
fixed_string<32> str;

fixed_string<32>::size_type pos = str.find("Hello");
if (pos != fixed_string<32>::npos)
{
  //do something
}

fixed_string<32>::size_type pos2 = str.find('=', 10); //start search at index 10

char buf[32];
str.copy(buf, 32);

//buf now contains the same data as str

char buf2[16];
str.copy(buf, 16, 15); //start copying at index 15

fixed_string<32>::size_type pos3 = str.substr("This_Is_A_Substring"); //find the first occurrence of specified substring
```

**Overloaded operators**

```cpp
fixed_string<32> first = "Hello there";
fixed_string<32> second = "I'm another string!";

if (first == second) //compares two strings
{
  std::cout << "huh?"
}

fixed_string<32> third = "Hello there";
if (first != third)
{
  std::cout << "huh?";
}

fixed_string<6> a = "aaaaa";
fixed_string<6> b = "bbbbb";

auto sum = a + b; //sum will be a fixed_string<12>
```

**I/O Operators**

```cpp
fixed_string<32> str;
std::cin >> str; //reads until next space or newline, just like std::string, or until str buffer is full

std::cout << str;
```

# Contribution

If you find any bugs, please report them as an issue so I can fix them, or create a pull request.

Any other feedback, like code improvements and new features are also greatly appreciated.

# Licensing

This code is licenced under the GNU General Public License v3.0
