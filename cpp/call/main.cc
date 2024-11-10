#include "common/definition.h"

MatrixAction checkPrioMatrix(Champion current, Champion next) noexcept
{
    auto WHITE_CIRCLE  = MatrixAction::CONTINUE_CURRENT_REJECT_NEXT;
    auto DELTA         = MatrixAction::CONTINUE_CURRENT_HOLD_NEXT;
    auto BLACK_CIRCLE  = MatrixAction::TERMINATE_CURRENT_EXECUTE_NEXT;

    const std::map<Champion, std::array<MatrixAction, 7>> prioMatrix
    {
        //        next ->
        // current
        // |
        // v                Garen           Teemo           Caitlyn         Blitz           Nami            Yasuo           Nunu
        {Champion::Garen,   {DELTA,         DELTA,          DELTA,          BLACK_CIRCLE,   DELTA,          BLACK_CIRCLE,   BLACK_CIRCLE}},
        {Champion::Teemo,   {WHITE_CIRCLE,  WHITE_CIRCLE,   WHITE_CIRCLE,   BLACK_CIRCLE,   WHITE_CIRCLE,   BLACK_CIRCLE,   BLACK_CIRCLE}},
        {Champion::Caitlyn, {WHITE_CIRCLE,  DELTA,          WHITE_CIRCLE,   BLACK_CIRCLE,   DELTA,          BLACK_CIRCLE,   BLACK_CIRCLE}},
        {Champion::Blitz,   {WHITE_CIRCLE,  WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   BLACK_CIRCLE}},
        {Champion::Nami,    {WHITE_CIRCLE,  WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   BLACK_CIRCLE}},
        {Champion::Yasuo,   {WHITE_CIRCLE,  WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   BLACK_CIRCLE}},
        {Champion::Nunu,    {WHITE_CIRCLE,  WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE,   WHITE_CIRCLE}},
    };

    auto it = prioMatrix.find(current);
    if (it != prioMatrix.end())
    {
        try
        {
            return it->second.at(Utils::to_underlying(next));
        }
        catch (const std::out_of_range& ex)
        {
            std::cout << ex.what() << '\n';
        }
    }

    return MatrixAction::NODEF;
}


int main()
{
    auto test = checkPrioMatrix(Champion::Teemo, Champion::Caitlyn);
    std::cout<<"test "<<test<<"\n";
    return 0;
}
