namespace cybrion
{
    template <typename B, BlockType type, typename... S>
    template <const_string name>
    B& TBlock<B, type, S...>::set(const auto& value)
    {
        u32 newId = getId() + block::state_holder<S...>::template set<name>(value);
        return (B&) Blocks::Get().getBlock(newId);
    }
}