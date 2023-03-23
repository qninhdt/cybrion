#pragma once

#include "core/bit_storage.hpp"
#include "util/math.hpp"

namespace cybrion
{
    template <u32 VALUE_SIZE, u32 SIZE>
    class LinearPalette
    {
    public:
        LinearPalette() : m_storage(nullptr), m_diffValues(1), m_valueToId{0}, m_bitExp(240404)
        {
            m_valueToId[0] = 1;
            m_idToValue.push_back(0);
        }

        ~LinearPalette()
        {
            if (m_storage)
                delete m_storage;
        }

        u32 get(const u32& index) const
        {
            if (m_storage == nullptr)
                return 0;

            return m_idToValue[m_storage->get(index)];
        }

        void set(const u32& index, const u32& value)
        {
            if (value == 0 && m_storage == nullptr)
                return;

            assert(value < VALUE_SIZE);
            auto& id = m_valueToId[value];

            if (id)
                m_storage->set(index, id - 1);
            else
            {
                u32 id = addValue(value);
                m_storage->set(index, id - 1);
            }
        }

        void reset()
        {
            m_idToValue.clear();
            delete m_storage;
            m_storage = nullptr;
        }

        BitStorage* getStorage()
        {
            return m_storage;
        }

        u32 getBitExp() const
        {
            return m_bitExp;
        }

        void fromJBT(const jbt::tag& tag)
        {
            auto& data = tag.get_byte_array("data");
            auto& palette = tag.get_tag("palette");

            if (data.data == nullptr)
                return;

            m_idToValue.clear();
            for (i32 i = 0; i < palette.size(); ++i)
                m_idToValue.push_back(palette.get_uint(i));

            if (m_storage)
            {
                delete m_storage;
                m_storage = nullptr;
            }

            u32 bit = std::round(std::log2(8 * data.size / SIZE));
            m_storage = BitStorage::create<SIZE>(bit);

            m_storage->fromJBT(data);
            m_diffValues = palette.size();
            m_bitExp = bit;
        }

        jbt::tag toJBT()
        {
            jbt::tag tag(jbt::tag_type::OBJECT);

            tag.set_byte_array("data", m_storage ? m_storage->toJBT() : jbt::byte_array_t{ nullptr, 0, false });
            
            jbt::tag palette(jbt::tag_type::LIST);
            for (u32 i : m_idToValue)
                palette.add_uint(i);

            tag.set_tag("palette", palette);

            return tag;
        }

    private:
        u32 addValue(const u32& value)
        {
            if (m_storage == nullptr)
            {
                m_storage = BitStorage::create<SIZE>(0);
            }
            else if (m_diffValues == m_storage->getMaxValue() + 1)
            {
                u32 bitExp = util::ceilLog2(util::ceilLog2(m_diffValues)) + 1;
                auto temp = m_storage;
                m_bitExp = bitExp;
                m_storage = BitStorage::create<SIZE>(bitExp);
                m_storage->copyFrom(*temp);
                delete temp;
            }

            m_diffValues += 1;
            m_idToValue.push_back(value);
            m_valueToId[value] = m_diffValues;

            return m_diffValues;
        }

        u32 m_bitExp;
        u32 m_diffValues;
        u32 m_valueToId[VALUE_SIZE];
        vector<u32> m_idToValue;
        BitStorage* m_storage;
    };
}