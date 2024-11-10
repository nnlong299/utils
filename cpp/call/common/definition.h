#include "utils/utils.h"

enum class MatrixAction
{
    CONTINUE_CURRENT_REJECT_NEXT,  
    CONTINUE_CURRENT_HOLD_NEXT,
    TERMINATE_CURRENT_EXECUTE_NEXT,
    NODEF
};
std::ostream& operator<<(std::ostream& os, MatrixAction c);

// WARNING: the order of element is on purpose to access array by index
enum class Champion
{
    Garen,
    Teemo,
    Caitlyn,
    Blitz,
    Nami,
    Yasuo,
    Nunu,
};
std::ostream& operator<<(std::ostream& os, Champion c);
