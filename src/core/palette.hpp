#pragma once

#include "core/bit_storage.hpp"
#include "util/math.hpp"

namespace cybrion
{
    template <typename T, u32 SIZE>
    class Palette
    {
    public:
        Palette() : m_storage(nullptr)
        {
        }

        ~Palette()
        {
            if (m_storage)
                delete m_storage;
        }

        T get(const u32 &index) const
        {
            assert(m_storage != nullptr);
            return m_idToValue[m_storage->get(index)];
        }

        void set(const u32 &index, const T &value)
        {
            auto it = m_valueToId.find(value);

            if (it != m_valueToId.end())
            {
                m_storage->set(index, it->second);
            }
            else
            {
                u32 id = addValue(value);
                m_storage->set(index, id);
            }
        }

        void reset()
        {
            m_idToValue.clear();
            m_valueToId.clear();
            delete m_storage;
            m_storage = nullptr;
        }

        BitStorage *getStorage()
        {
            return m_storage;
        }

    private:
        u32 addValue(const T &value)
        {
            u32 size = m_valueToId.size();

            if (m_storage == nullptr)
            {
                m_storage = BitStorage::create<SIZE>(0);
            }
            else if (size == m_storage->getMaxValue() + 1)
            {
                u32 bitExp = util::CeilLog2(util::CeilLog2(size)) + 1;
                auto temp = m_storage;
                m_storage = BitStorage::create<SIZE>(bitExp);
                m_storage->copyFrom(*temp);
                delete temp;
            }

            m_idToValue.push_back(value);
            m_valueToId[value] = m_valueToId.size();

            return size;
        }

        umap<T, u32> m_valueToId;
        vector<T> m_idToValue;
        BitStorage *m_storage;
    };
} // namespace nith
