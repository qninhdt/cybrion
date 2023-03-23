#pragma once

namespace cybrion
{
    class BitStorage
    {
    public:
        template <u32 SIZE>
        static BitStorage *create(const u32 &bitExp);

        virtual ~BitStorage(){};

        virtual void clear() = 0;
        virtual u32 get(const u32 &index) const = 0;
        virtual void set(const u32 &index, const u32 &value) = 0;
        virtual u32 getMaxValue() const = 0;
        virtual u32 getSize() const = 0;
        virtual void fromJBT(const jbt::byte_array_t& data) = 0;
        virtual jbt::byte_array_t toJBT() = 0;

        void copyFrom(const BitStorage &other)
        {
            u32 size = getSize();
            assert(size == other.getSize());

            for (u32 i = 0; i < size; ++i)
            {
                set(i, other.get(i));
            }
        }
    };

    template <u32 BIT_SIZE, u32 SIZE>
    class BitStorageImpl : public BitStorage
    {
    public:
        virtual ~BitStorageImpl() = default;

        void clear() override
        {
            std::memset(m_data, 0, sizeof(m_data));
        }

        u32 get(const u32 &index) const override
        {
            assert(index < SIZE);

            u32 intIndex = index >> FIVE_MINUS_BIT_SIZE;
            u32 bitOffset = (index & VALUES_PER_INT_MINUS_ONE) << BIT_SIZE;

            return (m_data[intIndex] >> bitOffset) & BIT_MASK;
        }

        void set(const u32 &index, const u32 &value) override
        {
            assert(index < SIZE);
            assert(value <= MAX_VALUE);

            u32 intIndex = index >> FIVE_MINUS_BIT_SIZE;
            u32 bitOffset = (index & VALUES_PER_INT_MINUS_ONE) << BIT_SIZE;
            u32 clearMask = ~(BIT_MASK << bitOffset);

            m_data[intIndex] = (m_data[intIndex] & clearMask) | (value << bitOffset);
        }

        u32 getSize() const override
        {
            return SIZE;
        }

        u32 getMaxValue() const override
        {
            return MAX_VALUE;
        }

        void fromJBT(const jbt::byte_array_t& data)
        {
            if (data.size)
                std::memcpy(m_data, data.data.get(), data.size);
        }

        jbt::byte_array_t toJBT()
        {
            u32 size = TOTAL_INTS * sizeof(u32);
            jbt::byte_array_t data{ std::make_shared<i8[]>(size), size, false};

            std::memcpy(data.data.get(), m_data, size);

            return data;
        }

    private:
        constexpr static u32 BITS_PER_VALUE = 1 << BIT_SIZE;
        constexpr static u32 FIVE_MINUS_BIT_SIZE = 5 - BIT_SIZE;
        constexpr static u32 VALUES_PER_INT = 32 >> BIT_SIZE;
        constexpr static u32 VALUES_PER_INT_MINUS_ONE = VALUES_PER_INT - 1;
        constexpr static u32 MAX_VALUE = 0xFFFFFFFF >> (32 - BITS_PER_VALUE);
        constexpr static u32 TOTAL_INTS = (SIZE * BITS_PER_VALUE + 31) >> 5;
        constexpr static u32 BIT_MASK = 0xFFFFFFFF >> (32 - BITS_PER_VALUE);

        u32 m_data[TOTAL_INTS];
    };

    template <u32 SIZE>
    BitStorage *BitStorage::create(const u32 &bitExp)
    {
        switch (bitExp)
        {
        case 0:
            return new BitStorageImpl<0, SIZE>();
        case 1:
            return new BitStorageImpl<1, SIZE>();
        case 2:
            return new BitStorageImpl<2, SIZE>();
        case 3:
            return new BitStorageImpl<3, SIZE>();
        case 4:
            return new BitStorageImpl<4, SIZE>();
        case 5:
            return new BitStorageImpl<5, SIZE>();
        default:
            assert(false);
            return nullptr;
        }
    }
} // namespace nith
