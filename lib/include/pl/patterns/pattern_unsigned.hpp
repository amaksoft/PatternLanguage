#pragma once

#include <pl/patterns/pattern.hpp>

namespace pl::ptrn {

    class PatternUnsigned : public Pattern {
    public:
        PatternUnsigned(core::Evaluator *evaluator, u64 offset, size_t size)
            : Pattern(evaluator, offset, size) { }

        [[nodiscard]] std::unique_ptr<Pattern> clone() const override {
            return std::unique_ptr<Pattern>(new PatternUnsigned(*this));
        }

        [[nodiscard]] core::Token::Literal getValue() const override {
            u128 data = 0;
            this->getEvaluator()->readData(this->getOffset(), &data, this->getSize(), this->getSection());
            return transformValue(hlp::changeEndianess(data, this->getSize(), this->getEndian()));
        }

        [[nodiscard]] std::string getFormattedName() const override {
            return this->getTypeName();
        }

        [[nodiscard]] bool operator==(const Pattern &other) const override { return areCommonPropertiesEqual<decltype(*this)>(other); }

        void accept(PatternVisitor &v) override {
            v.visit(*this);
        }

        std::string getFormattedValue() override {
            auto data = this->getValue().toUnsigned();
            return this->formatDisplayValue(fmt::format("{:d} (0x{:0{}X})", data, data, this->getSize() * 2), this->getValue());
        }

        [[nodiscard]] std::string toString() const override {
            auto value = this->getValue();
            auto result = fmt::format("{:d}", value.toUnsigned());

            return this->formatDisplayValue(result, value);
        }
    };

}