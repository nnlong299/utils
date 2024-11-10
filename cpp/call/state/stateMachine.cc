#include <typeinfo>
#include <memory>
#include "utils/utils.h"

class Context;

// reference: https://refactoring.guru/design-patterns/state/cpp/example
class State
{
protected:
    std::unique_ptr<Context> m_context;

public:
    virtual ~State() = default;

    void setContext(std::unique_ptr<Context> context)
    {
        m_context = std::move(context);
    }

    virtual void Handle1() = 0;
    virtual void Handle2() = 0;
};

/**
 * The Context defines the interface of interest to clients. It also maintains a
 * reference to an instance of a State subclass, which represents the current
 * state of the Context.
 */
class Context
{
private:
    std::unique_ptr<State> m_state;

public:
    Context(std::unique_ptr<State> state):
    m_state(nullptr)
    {
        TransitionTo(std::move(state));
    }
    ~Context() = default;

    /**
     * The Context allows changing the State object at runtime.
     */
    void TransitionTo(std::unique_ptr<State> state)
    {
        std::cout << "Context: Transition to " << typeid(*state).name() << ".\n";

        m_state = std::move(state);
        m_state->setContext(std::move(this));
    }

    void Request1()
    {
        m_state->Handle1();
    }

    void Request2()
    {
        m_state->Handle2();
    }
};

/**
 * Concrete States implement various behaviors, associated with a state of the
 * Context.
 */

class ConcreteStateA : public State
{
public:
    void Handle1() override;

    void Handle2() override
    {
        std::cout << "ConcreteStateA handles request2.\n";
    }
};

class ConcreteStateB : public State
{
public:
    void Handle1() override
    {
        std::cout << "ConcreteStateB handles request1.\n";
    }
    void Handle2() override
    {
        std::cout << "ConcreteStateB handles request2.\n";
        std::cout << "ConcreteStateB wants to change the state of the context.\n";
        this->context_->TransitionTo(new ConcreteStateA);
    }
};

void ConcreteStateA::Handle1()
{
    {
        std::cout << "ConcreteStateA handles request1.\n";
        std::cout << "ConcreteStateA wants to change the state of the context.\n";

        this->context_->TransitionTo(new ConcreteStateB);
    }
}

/**
 * The client code.
 */
void ClientCode()
{
    Context *context = new Context(new ConcreteStateA);
    context->Request1();
    context->Request2();
    delete context;
}