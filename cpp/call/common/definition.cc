#include "common/definition.h"

std::ostream& operator<<(std::ostream& os, MatrixAction en)
{
    switch(en)
    {
        case MatrixAction::CONTINUE_CURRENT_REJECT_NEXT:    os << "CONTINUE_CURRENT_REJECT_NEXT";   break;
        case MatrixAction::CONTINUE_CURRENT_HOLD_NEXT:      os << "CONTINUE_CURRENT_HOLD_NEXT";     break;
        case MatrixAction::TERMINATE_CURRENT_EXECUTE_NEXT:  os << "TERMINATE_CURRENT_EXECUTE_NEXT"; break;
        case MatrixAction::NODEF:                           os << "NODEF";                          break;
        default:                                            os.setstate(std::ios_base::failbit);
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, Champion en)
{
    switch(en)
    {
        case Champion::Garen:   os << "Garen";      break;
        case Champion::Teemo:   os << "Teemo";      break;
        case Champion::Caitlyn: os << "Caitlyn";    break;
        case Champion::Blitz:   os << "Blitz";      break;
        case Champion::Nami:    os << "Nami";       break;
        case Champion::Yasuo:   os << "Yasuo";      break;
        case Champion::Nunu:    os << "Nunu";       break;
        default:                os.setstate(std::ios_base::failbit);
    }

    return os;
}
