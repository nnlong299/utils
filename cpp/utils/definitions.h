namespace Definition
{
    enum class Color: uint32_t
    {
        Black   = 30,
        Red     = 31,
        Green   = 32,
        Yellow  = 33,
        Blue    = 34,
        Magenta = 35,
        Cyan    = 36,
        White   = 37,
    };

    constexpr static std::string_view ColorReset {"\033[0m"};
}