#pragma once

#include "core/entity.hpp"
#include "world/block/block_state.hpp"
#include "world/block/block_defs.hpp"

namespace cybrion
{
    union packed_block
    {
        struct
        {
            u16 type;
            u16 state;
        };
        u32 data;

        bool operator==(const packed_block& other) const
        {
            return data == other.data;
        }
    };

    enum class BlockFace
    {
        EAST,
        UP,
        SOUTH,
        WEST,
        DOWN,
        NORTH
    };

    class Block
    {
    public:
        Block(const BlockType &type): m_type(type)
        {
        }

        u32 getId() const
        {
            return m_id;
        }

        BlockType getType() const
        {
            return m_type;
        }

        string getDisplayName() const
        {
            return m_displayName;
        }

        BlockShape getShape() const
        {
            return m_shape;
        }

        virtual string toString() const = 0;

    protected:
        virtual bool checkQuery(const umap<string, string>& query) const = 0;

    private:
        friend class BlockLoader;

        template <typename... B>
        friend class BlockRegistry;

        u32 m_id;

        BlockShape m_shape;
        BlockType m_type;
        string m_displayName;
        u32 m_northTexture;
        u32 m_southTexture;
        u32 m_westTexture;
        u32 m_eastTexture;
        u32 m_topTexture;
        u32 m_bottomTexture;
    };

    template <typename B, typename... S>
    class TBlock : public Block, public block::state_holder<S...>
    {
    public:
        TBlock(BlockType type) : Block(type)
        {
        }

        string toString() const override
        {
            return block::state_holder<S...>::to_string();
        }

        template <const_string name>
        auto& get() const
        {
            return block::state_holder<S...>::template get<name>();
        }

        template <const_string name>
        B& set(const auto& value);

    private:
        template <typename... B>
        friend class BlockRegistry;

        bool checkQuery(const umap<string, string>& query) const override
        {
            return block::state_holder<S...>::check_query(query);
        }
    };

    namespace detail
    {
        template <typename B>
        struct block_registry
        {
            static constexpr u32 num_states = B::num_states();
            B blocks[num_states];
        };
    }

    template <typename... B>
    class BlockRegistry : private detail::block_registry<B> ...
    {
    public:

        void init()
        {
            registerImpl<B...>(0);
        }

        void queryBlocks(BlockType type, const umap<string, string>& query, vector<Block*>& result)
        {
            auto& [from, to] = m_typeToRange[u32(type)];

            for (u32 i = from; i < to; ++i)
                if (m_idToBlock[i]->checkQuery(query))
                    result.push_back(m_idToBlock[i]);
        }

        Block& getBlock(BlockType type)
        {
            // return first block state of this type (all states are 0)
            return getBlock(m_typeToRange[u32(type)].first);
        }

        Block& getBlock(u32 id)
        {
            return *m_idToBlock[id];
        }

        static constexpr u32 BlockStateCount()
        {
            return BlockStateCountImpl<B...>();
        }

    private:

        template <typename T, typename ...R>
        static constexpr u32 BlockStateCountImpl()
        {
            if constexpr (sizeof...(R) == 0)
                return T::num_states();
            else
                return T::num_states() + BlockStateCountImpl<R...>();
        }

        template <typename T, typename... R>
        void registerImpl(u32 index)
        {
            auto& self = (detail::block_registry<T>&)*this;
            BlockType type = self.blocks[0].getType();
            
            T block;
            u32 idx = 0;
            T::each_possible_values(block, [&] {
                self.blocks[idx] = block;
                self.blocks[idx].m_id = idx + index;
                m_idToBlock[idx + index] = &self.blocks[idx];
                //std::cout << block.toString() << '\n';
                idx += 1;
            });

            m_typeToRange[u32(type)] = { index, index + idx };

            if constexpr (sizeof...(R) > 0)
                registerImpl<R...>(index + idx);
        }

        Block* m_idToBlock[BlockStateCount()];
        std::pair<u32, u32> m_typeToRange[u32(BlockType::ENUM_SIZE)];
    };
}

namespace std
{
    template <>
    struct hash<cybrion::packed_block>
    {
        size_t operator()(const cybrion::packed_block& block) const
        {
            return hash<cybrion::u32>()(block.data);
        }
    };
}