class BaseState
{
public:
    explicit BaseState(std::reference_wrapper<FSM> context) : _context(context) {}
    virtual ~BaseState() = default;
    virtual eState getState() = 0;

    // event handlers
    virtual std::unique_ptr<BaseState> process(CardPresented event) { return nullptr; }
    virtual std::unique_ptr<BaseState> process(TransactionDeclined event) { return nullptr; }
    virtual std::unique_ptr<BaseState> process(TransactionSuccess event) { return nullptr; }
    virtual std::unique_ptr<BaseState> process(PersonPassed event) { return nullptr; }
    virtual std::unique_ptr<BaseState> process(Timeout event) { return nullptr; }

protected:
    // https://www.learncpp.com/cpp-tutorial/arrays-of-references-via-stdreference_wrapper/
    std::reference_wrapper<FSM> _context;
};

int main()
{
    auto fsm = FSM();
    fsm.process(CardPresented())
        .process(TransactionSuccess(5, 100));
    // std::cout<<"test "<<test<<"\n";
    return 0;
}
