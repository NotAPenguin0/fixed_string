# fixed_string
compile-time size string class

# What is fixed_string

This class is an STL-like wrapper for C-style strings. It maintains a fixed-size character array
and provides general string utilities, like replacing characters, filling it, adding two strings,
finding characters. 

# Usage

The header provides the `basic_fixed_string` class. It also provides the following typedefs:

```
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

Examples will be added soon...

# Contribution

If you find any bugs, please report them as an issue so I can fix them, or create a pull request.

Any other feedback, like code improvements and new features are also greatly appreciated.

# Licensing

This code is licenced under the GNU General Public License v3.0
