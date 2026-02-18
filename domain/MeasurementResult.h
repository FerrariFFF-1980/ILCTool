#pragma once

#include <string>
#include <stdexcept>
#include <cmath>
#include <utility>

#include "StringUtils.h"

class MeasurementResult
{
public:
    MeasurementResult(
        std::string laboratoryId,
        std::string sampleId,
        int replicateIndex,
        double value,
        std::string timestampIso8601 = "",
        std::string notes = "")
        : laboratoryId_(StringUtils::TrimCopy(laboratoryId)),
          sampleId_(StringUtils::TrimCopy(sampleId)),
          replicateIndex_(replicateIndex),
          value_(value),
          timestampIso8601_(std::move(timestampIso8601)),
          notes_(std::move(notes))
    {
        Validate();
    }

    // Getters
    const std::string &GetLaboratoryId() const noexcept { return laboratoryId_; }
    const std::string &GetSampleId() const noexcept { return sampleId_; }
    int GetReplicateIndex() const noexcept { return replicateIndex_; }
    double GetValue() const noexcept { return value_; }
    const std::string &GetTimestampIso8601() const noexcept { return timestampIso8601_; }
    const std::string &GetNotes() const noexcept { return notes_; }

    // Optional setters (minimal)
    void SetValue(double value)
    {
        value_ = value;
        Validate();
    }

    void SetNotes(std::string notes)
    {
        notes_ = std::move(notes);
    }

private:
    std::string laboratoryId_;
    std::string sampleId_;
    int replicateIndex_;
    double value_;
    std::string timestampIso8601_;
    std::string notes_;

    void Validate() const
    {
        if (laboratoryId_.empty())
        {
            throw std::invalid_argument("LaboratoryId must not be empty.");
        }

        if (sampleId_.empty())
        {
            throw std::invalid_argument("SampleId must not be empty.");
        }

        if (replicateIndex_ < 1)
        {
            throw std::invalid_argument("ReplicateIndex must be >= 1.");
        }

        if (!std::isfinite(value_))
        {
            throw std::invalid_argument("Value must be a finite number (not NaN/Inf).");
        }
    }
};
