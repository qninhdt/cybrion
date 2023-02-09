#pragma once

namespace cybrion
{
    namespace detail
    {
        template <typename E>
        struct enum_registry
        {
            static vector<string> enumToStr;
            static umap<string, E> strToEnum;
        };

        template <typename E>
        vector<string> enum_registry<E>::enumToStr;

        template <typename E>
        umap<string, E> enum_registry<E>::strToEnum;
    }

    template <typename E>
    void RegisterEnum(E value, string str)
    {
        u32 id = (u32)value;
        auto& vec = detail::enum_registry<E>::enumToStr;

        if (vec.size() < id + 1)
            vec.resize(id + 1);

        vec[id] = str;
        detail::enum_registry<E>::strToEnum[str] = value;
    }

    template <typename E>
    string EnumToString(E value)
    {
        return detail::enum_registry<E>::enumToStr[u32(value)];
    }

    template <typename E>
    E StringToEnum(string str)
    {
        return detail::enum_registry<E>::strToEnum[str];
    }
};