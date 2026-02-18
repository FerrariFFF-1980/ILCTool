#pragma once

#include <string>
#include <stdexcept>

#include "StringUtils.h"

class Measurand
{
public:
    Measurand(
        std::string measurandId,
        std::string name,
        std::string unit,
        std::string description = "")
        : measurandId_(StringUtils::TrimCopy(measurandId)),
          name_(std::move(name)),
          unit_(std::move(unit)),
          description_(std::move(description))
    {
        Validate();
    }

    // Getters
    const std::string &GetMeasurandId() const noexcept { return measurandId_; }
    const std::string &GetName() const noexcept { return name_; }
    const std::string &GetUnit() const noexcept { return unit_; }
    const std::string &GetDescription() const noexcept { return description_; }

    // Optional setters (keep it minimal for now)
    void SetDescription(std::string description)
    {
        description_ = std::move(description);
    }

private:
    std::string measurandId_;
    std::string name_;
    std::string unit_;
    std::string description_;

    void Validate() const
    {
        if (measurandId_.empty())
        {
            throw std::invalid_argument("MeasurandId must not be empty.");
        }

        if (name_.empty())
        {
            throw std::invalid_argument("Measurand name must not be empty.");
        }

        if (unit_.empty())
        {
            throw std::invalid_argument("Measurand unit must not be empty.");
        }
    }
};
