namespace cybrion
{
    template <typename B, typename... S>
    template <const_string name>
    B& TBlock<B, S...>::set(const auto& value)
    {
        u32 newId = getId() + block::state_holder<S...>::template set<name>(value);
        return (B&) Game::Get().getBlockRegistry().getBlock(newId);
    }
}