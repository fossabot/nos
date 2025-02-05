#pragma once

#include <ncxx/algorithm/min.hpp>
#include <ncxx/basic-types.hpp>
#include <ncxx/range/data.hpp>
#include <ncxx/range/size.hpp>

namespace NOS {

template<typename TChar>
class BasicStringView
{
public:
    using CharType = TChar;
    using SizeType = size_t;

    static constexpr SizeType NPOS{static_cast<SizeType>(-1)};

    constexpr BasicStringView() = default;
    constexpr BasicStringView(const BasicStringView&) = default;
    constexpr BasicStringView(BasicStringView&&) = default;

    constexpr BasicStringView(nullptr_t) = delete;

    constexpr BasicStringView(const CharType* str, SizeType size);
    constexpr BasicStringView(const CharType* cstr);

    template<typename It, typename End>
    constexpr BasicStringView(It first, End end);

    constexpr BasicStringView& operator=(const BasicStringView&) = default;
    constexpr BasicStringView& operator=(BasicStringView&&) = default;

    constexpr bool operator==(const BasicStringView& other) const;

    constexpr CharType operator[](SizeType index) const;

    constexpr SizeType size() const;
    constexpr bool isEmpty() const;

    constexpr const CharType* data() const;

    constexpr auto begin() const;
    constexpr auto end() const;

    constexpr auto begin();
    constexpr auto end();

    constexpr SizeType findLastOf(CharType ch) const;

    constexpr BasicStringView substr(SizeType offset, SizeType size = NPOS) const;

private:
    const CharType* _data{nullptr};
    SizeType _size{0};
};

using StringView = BasicStringView<char>;

template<typename CharType>
constexpr BasicStringView<CharType>::BasicStringView(const CharType* str, SizeType size)
    : _data(str)
    , _size(size)
{
}

template<typename CharType>
constexpr BasicStringView<CharType>::BasicStringView(const CharType* cstr)
    : _data(cstr)
{
    while (*cstr++ != '\0') ++_size;
}

template<typename CharType>
template<typename It, typename End>
constexpr BasicStringView<CharType>::BasicStringView(It first, End end)
    : _data(first)
    , _size(end - first)
{
}

template<typename CharType>
constexpr bool BasicStringView<CharType>::operator==(const BasicStringView& other) const
{
    if (size() != other.size())
    {
        return false;
    }

    for (size_t i = 0; i < size(); ++i)
    {
        if (_data[i] != other._data[i])
        {
            return false;
        }
    }

    return true;
}

template<typename CharType>
constexpr CharType BasicStringView<CharType>::operator[](SizeType index) const
{
    return _data[index];
}

template<typename CharType>
constexpr BasicStringView<CharType>::SizeType BasicStringView<CharType>::size() const
{
    return _size;
}

template<typename CharType>
constexpr bool BasicStringView<CharType>::isEmpty() const
{
    return _size <= 0;
}

template<typename CharType>
constexpr const CharType* BasicStringView<CharType>::data() const
{
    return _data;
}

template<typename CharType>
constexpr auto BasicStringView<CharType>::begin() const
{
    return _data;
}

template<typename CharType>
constexpr auto BasicStringView<CharType>::end() const
{
    return _data + _size;
}

template<typename CharType>
constexpr auto BasicStringView<CharType>::begin()
{
    return _data;
}

template<typename CharType>
constexpr auto BasicStringView<CharType>::end()
{
    return _data + _size;
}

template<typename CharType>
constexpr BasicStringView<CharType>::SizeType BasicStringView<CharType>::findLastOf(CharType ch) const
{
    if (!isEmpty())
    {
        for (SizeType i = size() - 1; i--;)
        {
            if (_data[i] == ch)
            {
                return i;
            }
        }
    }

    return NPOS;
}

template<typename CharType>
constexpr BasicStringView<CharType> BasicStringView<CharType>::substr(SizeType offset, SizeType size) const
{
    size = min(size, _size - offset);
    return size == 0 ? BasicStringView<CharType>{} : BasicStringView<CharType>{_data + offset, size};
}

} // namespace NOS
