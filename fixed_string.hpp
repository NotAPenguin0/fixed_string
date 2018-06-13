/*
* This file is subject to the terms and conditions defined in
* file 'LICENSE.txt', which is part of this source code package.
* Licensed by Michiel Vande Ginste
*/

#ifndef FIXED_STRING_HPP_
#define FIXED_STRING_HPP_

#pragma region Includes

#include <iostream>
#include <string>
#include <exception>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

#pragma endregion
#undef min
#define min(a, b) (b < a ? b : a)

#pragma region Class Declaration

/*String class with compile-time fixed size.
 *All functions that return a size DO NOT COUNT THE NULL TERMINATOR*/
template<unsigned Size, typename CharT>
class basic_fixed_string
{
public:
	using value_type = CharT;
	using size_type = unsigned;
	using reference = value_type & ;
	using const_reference = value_type const&;
	using pointer = value_type* ;
	using const_pointer = value_type const*;
	using iterator = pointer;
	using const_iterator = const_pointer;

	static constexpr size_type npos = -1;

	//Fills the string with null characters
	basic_fixed_string();
	//buf should be null terminated
	basic_fixed_string(const CharT* buffer);
//	basic_fixed_string(const CharT (&str)[Size]);
	basic_fixed_string(std::string const& str);
	basic_fixed_string(basic_fixed_string const& other);
	basic_fixed_string(basic_fixed_string&& other);
	template<unsigned N>
	basic_fixed_string(basic_fixed_string<N, CharT> const& other);
	

	basic_fixed_string& operator=(basic_fixed_string const& other);
	basic_fixed_string& operator=(basic_fixed_string&& other);
//	basic_fixed_string& operator=(const CharT (&str)[Size]);
	basic_fixed_string& operator=(std::string const& str);
	basic_fixed_string& operator=(const CharT* buffer);
	template<unsigned N>
	basic_fixed_string& operator=(basic_fixed_string<N, CharT> const& other);

	pointer data();
	const_pointer data() const;

	const_pointer c_str() const;

	reference operator[](size_type idx);
	const_reference operator[](size_type idx) const;

	reference at(size_type idx);
	const_reference at(size_type idx) const;

	reference front();
	const_reference front() const;

	reference back();
	const_reference back() const;

	iterator begin();
	iterator end();

	const_iterator cbegin() const;
	const_iterator cend() const;


	constexpr size_type size() const;
	constexpr size_type length() const;
	constexpr size_type max_size() const;
	bool empty() const;

	void clear();

	void fill(CharT c);
	void fill(size_type pos, CharT c);

	//Replaces count characters, starting from pos with a given character c
	basic_fixed_string& replace(size_type pos, size_type count, CharT c);
	basic_fixed_string& replace(iterator first, iterator last, CharT c);
	//Throws if str.length() < count
	basic_fixed_string& replace(size_type pos, size_type count, std::string const& str);
	basic_fixed_string& replace(iterator first, iterator last, std::string const& str);

	std::string substr(size_type pos = 0, size_type count = npos);

	//returns the number of characters copied
	size_type copy(CharT* dest, size_type count, size_type pos = 0) const;

	void swap(basic_fixed_string& other);

	//Finds first occurrence
	size_type find(std::string const& str, size_type pos = 0) const;
	size_type find(CharT ch, size_type pos = 0) const;

	bool operator==(basic_fixed_string const& other) const;
	bool operator==(const CharT (&other)[Size]) const;
	bool operator!=(basic_fixed_string const& other) const;
	bool operator!=(const CharT(&other)[Size]) const;

	operator std::string() const;
	explicit operator const char*() const;
	explicit operator char*();

	template<unsigned Sz, typename ChT>
	friend std::istream& operator>>(std::istream& in, basic_fixed_string<Sz, ChT>& str);
	template<unsigned Sz, typename ChT>
	friend std::ostream& operator<<(std::ostream& out, basic_fixed_string<Sz, ChT>& str);


private:
	CharT buf[Size];
};

#pragma endregion


template<unsigned N, typename CharT1, unsigned M, typename CharT2>
basic_fixed_string<N + M, std::common_type_t<CharT1, CharT2>> operator+(basic_fixed_string<N, CharT1> const& first, basic_fixed_string<M, CharT2> const& second);

template<unsigned N, typename CharT1, unsigned M, typename CharT2>
basic_fixed_string<N + M, std::common_type_t<CharT1, CharT2>> operator+(basic_fixed_string<N, CharT1> const& first, const CharT2(&second)[M]);

#pragma region Typedefs

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

#pragma endregion

#pragma region Constructor Implementations

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::basic_fixed_string()
{
	for (size_type idx = 0; idx < Size; ++idx)
	{
		buf[idx] = '\0';
	}
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::basic_fixed_string(const CharT* buffer)
{
	for (size_type idx = 0; idx < Size; ++idx)
	{
		buf[idx] = buffer[idx];
		if (buffer[idx] == '\0') //do not read past null terminator
			break;
	}
}
	
/*
template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::basic_fixed_string(const CharT (&str)[Size])
{
	for (size_type idx = 0; idx < Size; ++idx)
	{
		buf[idx] = str[idx];
	}
}
*/

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::basic_fixed_string(std::string const& str)
{
	for (size_type idx = 0; idx < min(size(), str.size()); ++idx)
	{
		buf[idx] = str[idx];
		if (idx == str.size() - 1)
			buf[min(Size - 1, idx + 1)] = '\0'; //make sure to null terminate
	}
	buf[Size - 1] = '\0';
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::basic_fixed_string(basic_fixed_string const& other)
{
	for (size_type idx = 0; idx < Size; ++idx)
	{
		buf[idx] = other.buf[idx];
	}
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::basic_fixed_string(basic_fixed_string&& other)
{
	for (size_type idx = 0; idx < Size; ++idx)
	{
		buf[idx] = other.buf[idx];
		other.buf[idx] = '\0';
	}
}

template<unsigned Size, typename CharT>
template<unsigned N>
basic_fixed_string<Size, CharT>::basic_fixed_string(basic_fixed_string<N, CharT> const& other)
{
	size_type sz = min(Size, N);
	for (size_type idx = 0; idx < sz; ++idx)
	{
		buf[idx] = other[idx];
	}
	//Add null terminator
	buf[min(N, Size) - 1] = '\0';
}

#pragma endregion

#pragma region Assignment Operator Implementations

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::operator=(basic_fixed_string const& other)
{
	if (this != &other)
	{
		for (size_type idx = 0; idx < Size; ++idx)
		{
			buf[idx] = other.buf[idx];
		}
	}
	return *this;
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::operator=(basic_fixed_string&& other)
{
	if (this != &other)
	{
		for (size_type idx = 0; idx < Size; ++idx)
		{
			buf[idx] = other.buf[idx];
			other.buf[idx] = '\0';
		}
	}
	return *this;
}

/*
template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::operator=(const CharT (&str)[Size])
{
	for (size_type idx = 0; idx < Size; ++idx)
	{
		buf[idx] = str[idx];
	}
}
*/

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::operator=(std::string const& str)
{
	for (size_type idx = 0; idx < min(size(), str.size()); ++idx)
	{
		buf[idx] = str[idx];
		if (idx == str.size() - 1)
			buf[min(Size - 1, idx + 1)] = '\0'; //make sure to null terminate
	}
	buf[Size - 1] = '\0';

	return *this;
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::operator=(const CharT* buffer)
{
	for (size_type idx = 0; idx < Size; ++idx)
	{
		buf[idx] = buffer[idx];
		if (buffer[idx] == '\0') //do not read past null terminator
			break;
	}
	return *this;
}

template<unsigned Size, typename CharT>
template<unsigned N>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::operator=(basic_fixed_string<N, CharT> const& other)
{
	size_type sz = min(Size, N);
	for (size_type idx = 0; idx < sz; ++idx)
	{
		buf[idx] = other[idx];
	}
	//Add null terminator
	buf[min(N, Size) - 1] = '\0';

	return *this;
}

#pragma endregion

#pragma region Element Access Function Implementations

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::pointer basic_fixed_string<Size, CharT>::data()
{
	return buf;
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_pointer basic_fixed_string<Size, CharT>::data() const
{
	return buf;
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_pointer basic_fixed_string<Size, CharT>::c_str() const
{
	return buf;
}


template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::reference basic_fixed_string<Size, CharT>::operator[](size_type idx)
{
	return buf[idx];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_reference basic_fixed_string<Size, CharT>::operator[](size_type idx) const
{
	return buf[idx];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::reference basic_fixed_string<Size, CharT>::at(size_type idx)
{
	if (idx >= size())
	{
		throw std::out_of_range("fixed_string iterator out of range");
	}

	return buf[idx];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_reference basic_fixed_string<Size, CharT>::at(size_type idx) const
{
	if (idx >= size())
	{
		throw std::out_of_range("fixed_string iterator out of range");
	}

	return buf[idx];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::reference basic_fixed_string<Size, CharT>::front()
{
	return buf[0];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_reference basic_fixed_string<Size, CharT>::front() const
{
	return buf[0];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::reference basic_fixed_string<Size, CharT>::back()
{
	return buf[size() - 1]; //don't count null terminator
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_reference basic_fixed_string<Size, CharT>::back() const
{
	return buf[size() - 1];
}

#pragma endregion

#pragma region Iterator Implementations

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::pointer basic_fixed_string<Size, CharT>::begin()
{
	return &buf[0];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_pointer basic_fixed_string<Size, CharT>::cbegin() const
{
	return &buf[0];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::pointer basic_fixed_string<Size, CharT>::end()
{
	return &buf[size()];
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::const_pointer basic_fixed_string<Size, CharT>::cend() const
{
	return &buf[size()];
}

#pragma endregion

#pragma region Size Access Implementations

template<unsigned Size, typename CharT>
constexpr typename basic_fixed_string<Size, CharT>::size_type basic_fixed_string<Size, CharT>::size() const
{
	return Size - 1; //Don't count null terminator
}

template<unsigned Size, typename CharT>
constexpr typename basic_fixed_string<Size, CharT>::size_type basic_fixed_string<Size, CharT>::length() const
{
	return Size - 1;
}

template<unsigned Size, typename CharT>
constexpr typename basic_fixed_string<Size, CharT>::size_type basic_fixed_string<Size, CharT>::max_size() const
{
	return Size - 1;
}

template<unsigned Size, typename CharT>
bool basic_fixed_string<Size, CharT>::empty() const
{
	static basic_fixed_string<Size, CharT> empty_string;

	return *this == empty_string;
}

#pragma endregion

#pragma region Utility Function Implementations

template<unsigned Size, typename CharT>
void basic_fixed_string<Size, CharT>::clear()
{
	fill('\0');
}

template<unsigned Size, typename CharT>
void basic_fixed_string<Size, CharT>::fill(CharT c)
{
	for (size_type idx = 0; idx < size(); ++idx) //go through all characters EXCEPT the null terminator
	{
		buf[idx] = c;
	}
}

template<unsigned Size, typename CharT>
void basic_fixed_string<Size, CharT>::fill(size_type pos, CharT c)
{
	if (pos >= size()) throw std::out_of_range("fixed_string iterator out of range");

	for (size_type idx = pos; idx < size(); ++idx)
	{
		buf[idx] = c;
	}
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::replace(size_type pos, size_type count, CharT c)
{
	if (pos + count > size()) //if this is false, pos < size()
	{
		throw std::out_of_range("fixed_string iterator out of range");
	}
	for (size_type idx = pos; idx < pos + count; ++idx)
	{
		buf[idx] = c;
	}

	return *this;
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::replace(iterator first, iterator last, CharT c)
{
	if (first < begin() || first >= end() || last < begin() || last > end() || last < first)
		throw std::out_of_range("fixed_string iterator out of range");

	for (iterator it = first; it != last; std::advance(it, 1))
	{
		*it = c;
	}

	return *this;
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::replace(size_type pos, size_type count, std::string const& str)
{
	if (str.length() < count) throw std::length_error("str is too small");
	if (pos + count >= size()) throw std::out_of_range("fixed_string iterator out of range");

	for (size_type idx = pos; idx < pos + count; ++idx)
	{
		buf[idx] = str[idx - pos];
	}

	return *this;
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>& basic_fixed_string<Size, CharT>::replace(iterator first, iterator last, std::string const& str)
{
	if (str.length() < static_cast<unsigned>(std::distance(first, last))) 
		throw std::length_error("str is too small");
	if (first < begin() || first >= end() || last < begin() || last > end() || last < first)
		throw std::range_error("fixed_string iterator out of range");

	for (iterator it = first; it != last; std::advance(it, 1))
	{
		*it = str[std::distance(first, it)];
	}
	
	return *this;
}

template<unsigned Size, typename CharT>
std::string basic_fixed_string<Size, CharT>::substr(size_type pos, size_type count)
{
	if (pos + count > size())
		throw std::out_of_range("fixed_string iterator out of range");

	std::string result;
	for (size_type idx = pos; idx < pos + count; ++idx)
	{
		result += buf[idx];
	}

	return std::move(result);
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::size_type basic_fixed_string<Size, CharT>::copy(CharT* dest, size_type count, size_type pos) const
{
	if (pos > size())
		throw std::out_of_range("fixed_string iterator out of range");

	if (count == npos || pos + count > size())
		count = size() - pos - 1;

	for(size_type idx = pos; idx < pos + count; ++idx)
	{
		dest[idx - pos] = buf[idx];
	}
	dest[count] = '\0'; //add a null terminator
	
	return count;
}

template<unsigned Size, typename CharT>
void basic_fixed_string<Size, CharT>::swap(basic_fixed_string& other)
{
	for (size_type idx = 0; idx < size(); ++idx)
	{
		std::swap(buf[idx], other.buf[idx]);
	}
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::size_type basic_fixed_string<Size, CharT>::find(std::string const& str, size_type pos) const
{
	/*Stealing algorithms lol*/
	std::string s(buf);
	return s.find(str, pos);
}

template<unsigned Size, typename CharT>
typename basic_fixed_string<Size, CharT>::size_type basic_fixed_string<Size, CharT>::find(CharT ch, size_type pos) const
{
	if (pos >= size())
		throw std::out_of_range("fixed_string iterator out of range");
	for (size_type idx = pos; idx < size(); ++idx)
	{
		if (buf[idx] == ch) return idx;
	}
	return npos;
}

#pragma endregion

#pragma region Comparison Operator Implementation

template<unsigned Size, typename CharT>
bool basic_fixed_string<Size, CharT>::operator==(basic_fixed_string const& other) const
{
	for (size_type idx = 0; idx < size(); ++idx)
	{
		if (buf[idx] != other.buf[idx])
			return false;
		if (buf[idx] == '\0') return true;
	}
	return true;
}

template<unsigned Size, typename CharT>
bool basic_fixed_string<Size, CharT>::operator==(const CharT(&other)[Size]) const
{
	for (size_type idx = 0; idx < size(); ++idx)
	{
		if (buf[idx] != other[idx])
			return false;
		if (buf[idx] == '\0') return true;
	}
	return true;
}

template<unsigned Size, typename CharT>
bool basic_fixed_string<Size, CharT>::operator!=(const CharT(&other)[Size]) const
{
	return !(*this == other);
}

template<unsigned Size, typename CharT>
bool basic_fixed_string<Size, CharT>::operator!=(basic_fixed_string const& other) const
{
	return !(*this == other);
}

#pragma endregion

#pragma region Conversion Implementations

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::operator std::string() const
{
	return std::string(buf);
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::operator const char*() const
{
	return c_str();
}

template<unsigned Size, typename CharT>
basic_fixed_string<Size, CharT>::operator char*()
{
	return data();
}

#pragma endregion

#pragma region IO Operators Implementations

template<unsigned Sz, typename ChT>
std::istream& operator>>(std::istream& in, basic_fixed_string<Sz, ChT>& str)
{
	std::string s;
	in >> s;

	for (typename basic_fixed_string<Sz, ChT>::size_type idx = 0; idx < min(s.size(), str.size()); ++idx)
	{
		str.buf[idx] = s[idx];
	}

	return in;
}

template<unsigned Sz, typename ChT>
std::ostream& operator<<(std::ostream& out, basic_fixed_string<Sz, ChT>& str)
{
	out << str.c_str();
	return out;
}

#pragma endregion

template<unsigned N, typename CharT1, unsigned M, typename CharT2>
basic_fixed_string<N + M, std::common_type_t<CharT1, CharT2>> 
operator+(basic_fixed_string<N, CharT1> const& first, basic_fixed_string<M, CharT2> const& second)
{
	return basic_fixed_string<N + M, std::common_type_t<CharT1, CharT2>>(std::string(first.c_str()) + second.c_str());
}

template<unsigned N, typename CharT1, unsigned M, typename CharT2>
basic_fixed_string<N + M, std::common_type_t<CharT1, CharT2>> 
operator+(basic_fixed_string<N, CharT1> const& first, const CharT2(&second)[M])
{
	return basic_fixed_string<N + M, std::common_type_t<CharT1, CharT2>>(std::string(first.c_str()) + second);
}

#undef min

#endif
