#pragma once

#include "core/entity.hpp"
#include "world/block/block_state.hpp"
#include "world/block/block_defs.hpp"

namespace cybrion
{
    class Block
    {
    public:

        Block(const BlockType &type):
            m_shape(BlockShape::CUBE),
            m_type(type),
            m_displayName(""),
            m_display(BlockDisplay::OPAQUE),
            m_rotationX(BlockRotation::R0),
            m_rotationY(BlockRotation::R0),
            m_rotationZ(BlockRotation::R0),
            m_northTexture(0),
            m_southTexture(0),
            m_westTexture(0),
            m_eastTexture(0),
            m_topTexture(0),
            m_bottomTexture(0),
            m_toLocalFace{ 0, 1, 2, 3, 4, 5 },
            m_toWorldFace{ 0 },
            m_id(0)
        {
        }

        Block(const Block& block) = delete;

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

        BlockDisplay getDisplay() const
        {
            return m_display;
        }

        BlockShape getShape() const
        {
            return m_shape;
        }

        u32 getTexture(u32 index)
        {
            return m_textures[index];
        }

        BlockFace getWorldFace(BlockFace face) const
        {
            return (BlockFace)m_toWorldFace[(u32)face];
        }

        BlockRotation getRotationX() const
        {
            return m_rotationX;
        }

        BlockRotation getRotationY() const
        {
            return m_rotationY;
        }

        BlockRotation getRotationZ() const
        {
            return m_rotationZ;
        }

        bool operator== (const Block& other) const
        {
            return m_id == other.m_id;
        }

        virtual string toString() const = 0;

    protected:
        virtual bool checkQuery(const umap<string, string>& query) const = 0;

    private:
        friend class BlockLoader;

        template <typename... B>
        friend class BaseBlockRegistry;

        void rotateBlock()
        {
            auto& f = m_toLocalFace;
            cycleFace(f[1], f[2], f[4], f[5], (u32)m_rotationX);
            cycleFace(f[2], f[0], f[5], f[3], (u32)m_rotationY);
            cycleFace(f[0], f[1], f[3], f[4], (u32)m_rotationZ);
            
            for (u32 i = 0; i < 6; ++i)
                m_toWorldFace[m_toLocalFace[i]] = i;
        }

        void cycleFace(u32& f1, u32& f2, u32& f3, u32& f4, u32 n)
        {
            while (n--)
            {
                u32 temp = f4;
                f4 = f3;
                f3 = f2;
                f2 = f1;
                f1 = temp;
            }
        }

        u32 m_id;

        BlockDisplay m_display;
        BlockShape m_shape;
        BlockType m_type;
        string m_displayName;

        BlockRotation m_rotationX;
        BlockRotation m_rotationY;
        BlockRotation m_rotationZ;

        union
        {
            struct
            {
                u32 m_eastTexture;
                u32 m_topTexture;
                u32 m_southTexture;
                u32 m_westTexture;
                u32 m_bottomTexture;
                u32 m_northTexture;
            };
            u32 m_textures[6];
        };

        u32 m_toWorldFace[6];
        u32 m_toLocalFace[6];
        
    };

    template <typename B, BlockType type, typename... S>
    class TBlock : public Block, public block::state_holder<S...>
    {
    public:
        TBlock() : Block(type)
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
        friend class BaseBlockRegistry;

        static constexpr BlockType BlockType = type;

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
    class BaseBlockRegistry : private detail::block_registry<B> ...
    {
    public:
        void load()
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

        void computeRotation()
        {
            for (u32 i = 0; i < BlockStateCount(); ++i)
            {
                m_idToBlock[i]->rotateBlock();
            }
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
            auto& self = (detail::block_registry<T>&) * this;
            BlockType type = T::BlockType;

            T block;
            u32 idx = 0;
            T::each_possible_values(block, [&] {
                self.blocks[idx] = block;
                self.blocks[idx].m_id = idx + index;
                m_idToBlock[idx + index] = &self.blocks[idx];
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