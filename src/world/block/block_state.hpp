#pragma once

#include "core/const_string.hpp"
#include "util/math.hpp"
#include "core/enum.hpp"

namespace cybrion
{
    namespace block
    {
        template <typename T, const_string pname, u32 pnum_values>
        struct state
        {
            using type = T;
            static constexpr const_string name = pname;
            static constexpr u32 num_values = pnum_values;
            static constexpr u32 num_bits = util::CeilLog2(num_values);
            static constexpr u32 name_length = name.length();

            T value = (T)0;

            bool runtime_has(const string& name)
            {
                return name == string((const char*)pname);
            }

            u32 runtime_get(const string& name)
            {
                return (u32)value;
            }

            string runtime_get_as_string(const string& name) const
            {
                return to_string();
            }

            string to_string() const
            {
                if constexpr (std::is_same_v<T, bool>)
                    return value ? "true" : "false";
                else if constexpr (std::is_enum_v<T>)
                    return EnumToString(value);
                else
                    return std::to_string(value);
            }
        };

        template <const_string name, u32 num_values>
        using int_state = block::state<u32, name, num_values>;

        template <const_string name>
        using bool_state = block::state<bool, name, 2>;

        template <typename... S>
        class state_holder : private S...
        {
        public:
            static constexpr u32 num_states()
            {
                if constexpr (sizeof...(S) == 0)
                    return 1;
                else
                    return num_states_impl<S...>();
            }
        protected:

            template <const_string name>
            auto& get() const
            {
                return get_impl<name, S...>();
            }

            template <const_string name>
            i32 set(const auto& value)
            {
                return set_impl<name, 1, S...>(value);
            }

            template <const_string name, i32 n, typename T, typename... R>
            i32 set_impl(const auto& value)
            {
                constexpr u32 m = num_states() / n / T::num_values;
                if constexpr (T::name == name)
                {
                    auto& self = *(T*)this;
                    return m * (i32(value) - i32(self.value));
                }
                else if (sizeof...(R) > 0)
                    return set_impl<name, n* T::num_values, R...>(value);
            }

            string to_string() const
            {
                if constexpr (sizeof...(S) == 0)
                    return "[]";
                else
                {
                    string str = ((S::name.to_string() + "=" + ((S&)*this).to_string() + ',') + ...);
                    str.erase(str.end() - 1);
                    return "[" + str + "]";
                }
            }

            template <typename H>
            static void each_possible_values(H& holder, auto callback)
            {
                if constexpr (sizeof...(S) == 0)
                    callback();
                else
                    each_possible_values_impl<S...>(holder, callback);
            }

            u32 runtime_get(const string& name)
            {
                u32 result = 0;
                ([&] {
                    S& self = *(S*)this;
                    if (self.runtime_has(name))
                    {
                        result = self.runtime_get(name);
                        return true;
                    }
                    return false;
                }() || ...);
                return result;
            }

            string runtime_get_as_string(const string& name)
            {
                string result = "";
                ([&] {
                    S& self = *(S*)this;
                    if (self.runtime_has(name))
                    {
                        result = self.runtime_get_as_string(name);
                        return true;
                    }
                    return false;
                }() || ...);
                return result;
            }

            bool runtime_has(const string& name)
            {
                return (((S&)*this).runtime_has(name) || ...);
            }

            bool check_query(const umap<string, string>& query) const
            {
                for (auto& [key, value] : query)
                    if (!check_query_impl(key, value))
                        return false;

                return true;
            }

        private:

            template <typename T, typename ...R>
            static constexpr u32 num_states_impl()
            {
                if constexpr (sizeof...(R) == 0)
                    return T::num_values;
                else
                    return T::num_values * num_states_impl<R...>();
            }

            bool check_query_impl(const string& key, const string& value) const
            {
                return ([&] {
                    S& self = *(S*)this;
                    if (self.runtime_has(key) && self.runtime_get_as_string(key) == value)
                        return true;
                    return false;
                }() || ...);
            }

            template <typename T, typename... R>
            static void each_possible_values_impl(auto& holder, auto callback)
            {
                for (u32 i = 0; i < T::num_values; ++i)
                {
                    *(u32*)&(holder.get<T::name>()) = i;

                    if constexpr (sizeof...(R) == 0)
                        callback();
                    else
                        each_possible_values_impl<R...>(holder, callback);
                }
            }

            template <const_string name, typename T, typename... R>
            auto& get_impl() const
            {
                if constexpr (T::name == name)
                    return ((T*)this)->value;
                else
                {
                    static_assert(sizeof...(R) > 0);
                    return get_impl<name, R...>();
                }
            }
        };
    }
}
