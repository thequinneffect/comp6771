#include <memory>
#include <iostream>

class X {
  public:
    X(int data) : data_ {data} {};

    void ConstFunc() const {
        std::cout << this->data_ << "\n";
    }

    void NonConstFuncNoMutation() {
        std::cout << this->data_ << "\n";
    }

    void NonConstFuncMutation() {
        this->data_ = 7;
        std::cout << this->data_ << "\n";
    }

  private:
    int data_;
};



void func(std::unique_ptr<int> a, std::unique_ptr<int> b) {
    std::cout << *a << " " << *b << "\n";
}


int main(void)
{
    func(std::make_unique<int>(1), std::make_unique<int>(2));
    const X cx{2};
    cx.ConstFunc();
    X ncx{3};
    ncx.ConstFunc();
    ncx.NonConstFuncNoMutation();
    ncx.NonConstFuncMutation();
    cx.NonConstFuncNoMutation();
    cx.NonConstFuncMutation();

}