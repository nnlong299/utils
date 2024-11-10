#include <array>
#include <cstdint>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <ostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

enum class eState
{
    Locked,
    PaymentProcessing,
    PaymentFailed,
    PaymentSuccess,
    Unlocked
};

struct CardPresented
{
};

struct TransactionDeclined
{
};

struct TransactionSuccess
{
    TransactionSuccess(int fare_, int balance_):
    fare(fare_),
    balance(balance_)
    {
    }
    int fare;
    int balance;
};

struct PersonPassed
{
};

struct Timeout
{
};

class POSTerminal
{
public:
    explicit POSTerminal(std::string_view firstRow) : m_firstRow(firstRow)
    {
    }

    void setRows(std::string_view firstRow,
                 std::string_view secondRow = "",
                 std::string_view thirdRow = "")
    {
        m_firstRow = firstRow;
        m_secondRow = secondRow;
        m_thirdRow = thirdRow;
        std::cout << "Screen: " << m_firstRow << " " << m_secondRow << " " << m_thirdRow << "\n";
    }

    std::string_view getFirstRow() const { return m_firstRow; }
    std::string_view getSecondRow() const { return m_secondRow; }
    std::string_view getThirdRow() const { return m_thirdRow; }

private:
    std::string_view m_firstRow;
    std::string_view m_secondRow;
    std::string_view m_thirdRow;
};

class SwingDoor
{
public:
    enum class LEDPattern
    {
        Closed,
        Open
    };
    void open()
    {
        std::cout << "Door is open\n";
        m_status = LEDPattern::Open;
    }
    void close()
    {
        std::cout << "Door is closed\n";
        m_status = LEDPattern::Closed;
    }
    LEDPattern getStatus() const { return m_status; }

private:
    LEDPattern m_status{LEDPattern::Closed};
};

enum class LEDPattern: uint32_t
{
    RedCross,
    FlashRedCross,
    GreenArrow,
    OrangeCross
};

std::ostream& operator<<(std::ostream& os, LEDPattern en)
{
    switch(en)
    {
        case LEDPattern::RedCross:   os << "RedCross";      break;
        case LEDPattern::FlashRedCross:   os << "FlashRedCross";      break;
        case LEDPattern::GreenArrow: os << "GreenArrow";    break;
        case LEDPattern::OrangeCross:   os << "OrangeCross";      break;
        default:                os.setstate(std::ios_base::failbit);
    }
    return os;
}

class LEDController
{
public:

    void setStatus(LEDPattern status)
    {
        std::cout << "LED: "<<status<<"\n";
        m_status = status;
    };

    LEDPattern getStatus() const { return m_status; }

private:
    LEDPattern m_status{LEDPattern::RedCross};
};

class FSM
{
public:
    FSM()
    {
        m_door.close();
        m_led.setStatus(LEDPattern::RedCross);
        m_pos.setRows("Booting");

        std::cout << "System was boot OK with state Locked\n";
    }
    FSM &process(CardPresented event);
    FSM &process(TransactionDeclined event);
    FSM &process(TransactionSuccess event);
    FSM &process(PersonPassed event);
    FSM &process(Timeout event);
    eState getState() const { return m_state; };
    void transitionToPaymentProcessing();
    void transitionToLocked();
    void transitionToUnlocked();
    void transitionToPaymentSuccessful(int fare, int balance);

private:
    eState m_state{eState::Locked};
    SwingDoor m_door{};
    POSTerminal m_pos{"Touch Card"};
    LEDController m_led{};
};

void FSM::transitionToPaymentProcessing()
{
    m_state = eState::PaymentProcessing;
    m_pos.setRows("Processing");
    m_door.close();
    m_led.setStatus(LEDPattern::OrangeCross);
}

FSM &FSM::process(CardPresented event)
{
    switch (m_state)
    {
    case eState::Locked:
        transitionToPaymentProcessing();
        break;
    default:
        break;
    }
    return *this;
}

FSM &FSM::process(TransactionDeclined event)
{
    switch (m_state)
    {
    case eState::PaymentProcessing:
        m_state = eState::PaymentFailed;
        m_pos.setRows("Declined");
        m_door.close();
        m_led.setStatus(LEDPattern::FlashRedCross);
        break;
    default:
        break;
    }
    return *this;
}
FSM &FSM::process(TransactionSuccess event)
{
    switch (m_state)
    {
    case eState::PaymentProcessing:
        transitionToPaymentSuccessful(event.fare, event.balance);
        break;
    default:
        break;
    }
    return *this;
}
FSM &FSM::process(PersonPassed event)
{
    switch (m_state)
    {
    case eState::PaymentSuccess:
    case eState::Unlocked:
        transitionToLocked();
        break;
    default:
        break;
    }
    return *this;
}

void FSM::transitionToLocked()
{
    m_state = eState::Locked;
    m_pos.setRows("Touch Card");
    m_door.close();
    m_led.setStatus(LEDPattern::RedCross);
}

void FSM::transitionToPaymentSuccessful(int fare, int balance)
{
    m_state = eState::PaymentSuccess;
    std::string reason = "Fare: " + std::to_string(fare) + ", balance: " + std::to_string(balance);
    m_pos.setRows("Approved", reason);
    m_door.open();
    m_led.setStatus(LEDPattern::GreenArrow);
}

void FSM::transitionToUnlocked()
{
    m_state = eState::Unlocked;
    m_door.open();
    m_pos.setRows("Approved");
    m_led.setStatus(LEDPattern::GreenArrow);
}

FSM &FSM::process(Timeout event)
{
    switch (m_state)
    {
    case eState::PaymentProcessing:
        m_door.close();
        m_pos.setRows("Network error");
        m_led.setStatus(LEDPattern::FlashRedCross);
        m_state = eState::PaymentFailed;
        break;

    case eState::PaymentFailed:
        transitionToLocked();
        break;
    case eState::PaymentSuccess:
        transitionToUnlocked();
        break;
    default:
        break;
    }
    return *this;
}

int main()
{
    auto fsm = FSM();
    fsm.process(CardPresented())
    .process(TransactionSuccess(5, 100));
    // std::cout<<"test "<<test<<"\n";
    return 0;
}
