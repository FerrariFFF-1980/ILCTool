#pragma once

#include <string>
#include <stdexcept>
#include <optional>

#include "StringUtils.h"

class Sample
{
public:
    Sample(
        std::string sampleId,
        std::string measurandId,
        std::optional<double> assignedValue = std::nullopt,
        std::optional<double> standardUncertainty = std::nullopt,
        std::string description = "")
        : sampleId_(StringUtils::TrimCopy(sampleId)),
          measurandId_(StringUtils::TrimCopy(measurandId)),
          assignedValue_(assignedValue),
          standardUncertainty_(standardUncertainty),
          description_(std::move(description))
    {
        Validate();
    }

    // Getters
    const std::string &GetSampleId() const noexcept { return sampleId_; }
    const std::string &GetMeasurandId() const noexcept { return measurandId_; }

    const std::optional<double> &GetAssignedValue() const noexcept { return assignedValue_; }
    const std::optional<double> &GetStandardUncertainty() const noexcept { return standardUncertainty_; }

    const std::string &GetDescription() const noexcept { return description_; }

    // Setters (minimal)
    void SetAssignedValue(std::optional<double> assignedValue)
    {
        assignedValue_ = assignedValue;
        Validate();
    }

    void SetStandardUncertainty(std::optional<double> standardUncertainty)
    {
        standardUncertainty_ = standardUncertainty;
        Validate();
    }

private:
    std::string sampleId_;
    std::string measurandId_;
    std::optional<double> assignedValue_;
    std::optional<double> standardUncertainty_;
    std::string description_;

    void Validate() const
    {
        if (sampleId_.empty())
        {
            throw std::invalid_argument("SampleId must not be empty.");
        }

        if (measurandId_.empty())
        {
            throw std::invalid_argument("MeasurandId must not be empty for a Sample.");
        }

        if (standardUncertainty_.has_value() && standardUncertainty_.value() < 0.0)
        {
            throw std::invalid_argument("StandardUncertainty must be >= 0.");
        }
    }
};
