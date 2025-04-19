#include <iostream>
#include <memory>
class Filter {
public:
    virtual void apply() = 0;
};

class BlurFilter : public Filter {
public:
    void apply() override { std::cout << "Applying Blur\n"; }
};

class SharpenFilter : public Filter {
public:
    void apply() override { std::cout << "Applying Sharpen\n"; }
};

class ImageProcessor {
private:
    std::unique_ptr<Filter> filter;
public:
    void setFilter(std::unique_ptr<Filter> f) { filter = std::move(f); }
    void applyFilter() { if (filter) filter->apply(); }
};

int main() {
    ImageProcessor processor;
    processor.setFilter(std::make_unique<BlurFilter>());
    processor.applyFilter();
}
