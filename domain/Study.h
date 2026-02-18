#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "StringUtils.h"
#include "Laboratory.h"
#include "Measurand.h"
#include "Sample.h"
#include "MeasurementResult.h"

class Study
{
public:
    Study(std::string studyId, std::string title = "")
        : studyId_(StringUtils::TrimCopy(studyId)),
          title_(std::move(title))
    {
        Validate();
    }

    // -------------------------
    // Basic getters / setters
    // -------------------------
    const std::string &GetStudyId() const noexcept { return studyId_; }
    const std::string &GetTitle() const noexcept { return title_; }
    const std::string &GetStartDateIso8601() const noexcept { return startDateIso8601_; }
    const std::string &GetEndDateIso8601() const noexcept { return endDateIso8601_; }

    void SetTitle(std::string title)
    {
        title_ = std::move(title);
    }

    void SetStartDateIso8601(std::string startDateIso8601)
    {
        startDateIso8601_ = std::move(startDateIso8601);
    }

    void SetEndDateIso8601(std::string endDateIso8601)
    {
        endDateIso8601_ = std::move(endDateIso8601);
    }

    // --------------------------------
    // Laboratory CRUD
    // --------------------------------
    void AddLaboratory(const Laboratory &laboratory)
    {
        const std::string laboratoryId = StringUtils::TrimCopy(laboratory.GetLaboratoryId());
        if (laboratoryId.empty())
        {
            throw std::invalid_argument("AddLaboratory: LaboratoryId must not be empty.");
        }

        if (FindLaboratoryIndexById(laboratoryId).has_value())
        {
            throw std::invalid_argument("AddLaboratory: Duplicate LaboratoryId.");
        }

        laboratories_.push_back(laboratory);
    }

    bool UpdateLaboratory(const std::string &laboratoryId, const Laboratory &newLaboratory)
    {
        const std::string trimmedId = StringUtils::TrimCopy(laboratoryId);
        const auto indexOpt = FindLaboratoryIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            return false;
        }

        const std::string newId = StringUtils::TrimCopy(newLaboratory.GetLaboratoryId());
        if (newId.empty())
        {
            throw std::invalid_argument("UpdateLaboratory: New LaboratoryId must not be empty.");
        }

        // If ID changes, ensure uniqueness
        if (newId != trimmedId && FindLaboratoryIndexById(newId).has_value())
        {
            throw std::invalid_argument("UpdateLaboratory: New LaboratoryId already exists.");
        }

        laboratories_[indexOpt.value()] = newLaboratory;

        // If ID changed, results that reference the old id become inconsistent.
        // For now: forbid changing ID when there are results for that laboratory.
        if (newId != trimmedId && HasResultsForLaboratory(trimmedId))
        {
            throw std::invalid_argument(
                "UpdateLaboratory: Cannot change LaboratoryId while results exist for it.");
        }

        return true;
    }

    bool RemoveLaboratoryById(const std::string &laboratoryId)
    {
        const std::string trimmedId = StringUtils::TrimCopy(laboratoryId);
        const auto indexOpt = FindLaboratoryIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            return false;
        }

        if (HasResultsForLaboratory(trimmedId))
        {
            throw std::invalid_argument("RemoveLaboratoryById: Laboratory has results.");
        }

        laboratories_.erase(laboratories_.begin() + indexOpt.value());
        return true;
    }

    const Laboratory &GetLaboratoryById(const std::string &laboratoryId) const
    {
        const std::string trimmedId = StringUtils::TrimCopy(laboratoryId);
        const auto indexOpt = FindLaboratoryIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            throw std::invalid_argument("GetLaboratoryById: LaboratoryId not found.");
        }

        return laboratories_[indexOpt.value()];
    }

    std::vector<Laboratory> GetLaboratories() const
    {
        return laboratories_;
    }

    // --------------------------------
    // Measurand CRUD
    // --------------------------------
    void AddMeasurand(const Measurand &measurand)
    {
        const std::string measurandId = StringUtils::TrimCopy(measurand.GetMeasurandId());
        if (measurandId.empty())
        {
            throw std::invalid_argument("AddMeasurand: MeasurandId must not be empty.");
        }

        if (FindMeasurandIndexById(measurandId).has_value())
        {
            throw std::invalid_argument("AddMeasurand: Duplicate MeasurandId.");
        }

        measurands_.push_back(measurand);
    }

    bool UpdateMeasurand(const std::string &measurandId, const Measurand &newMeasurand)
    {
        const std::string trimmedId = StringUtils::TrimCopy(measurandId);
        const auto indexOpt = FindMeasurandIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            return false;
        }

        const std::string newId = StringUtils::TrimCopy(newMeasurand.GetMeasurandId());
        if (newId.empty())
        {
            throw std::invalid_argument("UpdateMeasurand: New MeasurandId must not be empty.");
        }

        if (newId != trimmedId && FindMeasurandIndexById(newId).has_value())
        {
            throw std::invalid_argument("UpdateMeasurand: New MeasurandId already exists.");
        }

        // Forbid ID change if Samples reference the old ID
        if (newId != trimmedId && HasSamplesForMeasurand(trimmedId))
        {
            throw std::invalid_argument(
                "UpdateMeasurand: Cannot change MeasurandId while Samples reference it.");
        }

        measurands_[indexOpt.value()] = newMeasurand;
        return true;
    }

    bool RemoveMeasurandById(const std::string &measurandId)
    {
        const std::string trimmedId = StringUtils::TrimCopy(measurandId);
        const auto indexOpt = FindMeasurandIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            return false;
        }

        if (HasSamplesForMeasurand(trimmedId))
        {
            throw std::invalid_argument("RemoveMeasurandById: Measurand is used by Samples.");
        }

        measurands_.erase(measurands_.begin() + indexOpt.value());
        return true;
    }

    const Measurand &GetMeasurandById(const std::string &measurandId) const
    {
        const std::string trimmedId = StringUtils::TrimCopy(measurandId);
        const auto indexOpt = FindMeasurandIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            throw std::invalid_argument("GetMeasurandById: MeasurandId not found.");
        }

        return measurands_[indexOpt.value()];
    }

    std::vector<Measurand> GetMeasurands() const
    {
        return measurands_;
    }

    // --------------------------------
    // Sample CRUD
    // --------------------------------
    void AddSample(const Sample &sample)
    {
        const std::string sampleId = StringUtils::TrimCopy(sample.GetSampleId());
        if (sampleId.empty())
        {
            throw std::invalid_argument("AddSample: SampleId must not be empty.");
        }

        if (FindSampleIndexById(sampleId).has_value())
        {
            throw std::invalid_argument("AddSample: Duplicate SampleId.");
        }

        const std::string measurandId = StringUtils::TrimCopy(sample.GetMeasurandId());
        if (!FindMeasurandIndexById(measurandId).has_value())
        {
            throw std::invalid_argument("AddSample: MeasurandId not found.");
        }

        samples_.push_back(sample);
    }

    bool UpdateSample(const std::string &sampleId, const Sample &newSample)
    {
        const std::string trimmedId = StringUtils::TrimCopy(sampleId);
        const auto indexOpt = FindSampleIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            return false;
        }

        const std::string newId = StringUtils::TrimCopy(newSample.GetSampleId());
        if (newId.empty())
        {
            throw std::invalid_argument("UpdateSample: New SampleId must not be empty.");
        }

        if (newId != trimmedId && FindSampleIndexById(newId).has_value())
        {
            throw std::invalid_argument("UpdateSample: New SampleId already exists.");
        }

        const std::string measurandId = StringUtils::TrimCopy(newSample.GetMeasurandId());
        if (!FindMeasurandIndexById(measurandId).has_value())
        {
            throw std::invalid_argument("UpdateSample: MeasurandId not found.");
        }

        // Forbid ID change if results reference the old sample id
        if (newId != trimmedId && HasResultsForSample(trimmedId))
        {
            throw std::invalid_argument(
                "UpdateSample: Cannot change SampleId while results exist for it.");
        }

        samples_[indexOpt.value()] = newSample;
        return true;
    }

    bool RemoveSampleById(const std::string &sampleId)
    {
        const std::string trimmedId = StringUtils::TrimCopy(sampleId);
        const auto indexOpt = FindSampleIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            return false;
        }

        if (HasResultsForSample(trimmedId))
        {
            throw std::invalid_argument("RemoveSampleById: Sample has results.");
        }

        samples_.erase(samples_.begin() + indexOpt.value());
        return true;
    }

    const Sample &GetSampleById(const std::string &sampleId) const
    {
        const std::string trimmedId = StringUtils::TrimCopy(sampleId);
        const auto indexOpt = FindSampleIndexById(trimmedId);
        if (!indexOpt.has_value())
        {
            throw std::invalid_argument("GetSampleById: SampleId not found.");
        }

        return samples_[indexOpt.value()];
    }

    std::vector<Sample> GetSamples() const
    {
        return samples_;
    }

    // --------------------------------
    // MeasurementResult CRUD
    // --------------------------------
    void AddMeasurementResult(const MeasurementResult &result)
    {
        const std::string laboratoryId = StringUtils::TrimCopy(result.GetLaboratoryId());
        const std::string sampleId = StringUtils::TrimCopy(result.GetSampleId());
        const int replicateIndex = result.GetReplicateIndex();

        EnsureLaboratoryExists(laboratoryId);
        EnsureSampleExists(sampleId);

        if (FindResultIndexByKey(laboratoryId, sampleId, replicateIndex).has_value())
        {
            throw std::invalid_argument(
                "AddMeasurementResult: Duplicate (LaboratoryId, SampleId, ReplicateIndex).");
        }

        results_.push_back(result);
    }

    bool UpdateMeasurementResult(
        const std::string &laboratoryId,
        const std::string &sampleId,
        int replicateIndex,
        const MeasurementResult &newResult)
    {
        const std::string labId = StringUtils::TrimCopy(laboratoryId);
        const std::string sampId = StringUtils::TrimCopy(sampleId);

        const auto indexOpt = FindResultIndexByKey(labId, sampId, replicateIndex);
        if (!indexOpt.has_value())
        {
            return false;
        }

        // Key changes are forbidden in this operation to keep integrity simple.
        // You can remove + add if you truly need to change keys.
        if (StringUtils::TrimCopy(newResult.GetLaboratoryId()) != labId ||
            StringUtils::TrimCopy(newResult.GetSampleId()) != sampId ||
            newResult.GetReplicateIndex() != replicateIndex)
        {
            throw std::invalid_argument("UpdateMeasurementResult: Key fields cannot change.");
        }

        results_[indexOpt.value()] = newResult;
        return true;
    }

    bool RemoveMeasurementResult(
        const std::string &laboratoryId,
        const std::string &sampleId,
        int replicateIndex)
    {
        const std::string labId = StringUtils::TrimCopy(laboratoryId);
        const std::string sampId = StringUtils::TrimCopy(sampleId);

        const auto indexOpt = FindResultIndexByKey(labId, sampId, replicateIndex);
        if (!indexOpt.has_value())
        {
            return false;
        }

        results_.erase(results_.begin() + indexOpt.value());
        return true;
    }

    const MeasurementResult &GetMeasurementResult(
        const std::string &laboratoryId,
        const std::string &sampleId,
        int replicateIndex) const
    {
        const std::string labId = StringUtils::TrimCopy(laboratoryId);
        const std::string sampId = StringUtils::TrimCopy(sampleId);

        const auto indexOpt = FindResultIndexByKey(labId, sampId, replicateIndex);
        if (!indexOpt.has_value())
        {
            throw std::invalid_argument("GetMeasurementResult: Result key not found.");
        }

        return results_[indexOpt.value()];
    }

    std::vector<MeasurementResult> GetMeasurementResults() const
    {
        return results_;
    }

private:
    std::string studyId_;
    std::string title_;
    std::string startDateIso8601_;
    std::string endDateIso8601_;

    std::vector<Laboratory> laboratories_;
    std::vector<Measurand> measurands_;
    std::vector<Sample> samples_;
    std::vector<MeasurementResult> results_;

    void Validate() const
    {
        if (studyId_.empty())
        {
            throw std::invalid_argument("StudyId must not be empty.");
        }
    }

    // -------------------------
    // Find helpers (indexes)
    // -------------------------
    std::optional<std::size_t> FindLaboratoryIndexById(const std::string &laboratoryId) const
    {
        for (std::size_t index = 0; index < laboratories_.size(); ++index)
        {
            if (laboratories_[index].GetLaboratoryId() == laboratoryId)
            {
                return index;
            }
        }
        return std::nullopt;
    }

    std::optional<std::size_t> FindMeasurandIndexById(const std::string &measurandId) const
    {
        for (std::size_t index = 0; index < measurands_.size(); ++index)
        {
            if (measurands_[index].GetMeasurandId() == measurandId)
            {
                return index;
            }
        }
        return std::nullopt;
    }

    std::optional<std::size_t> FindSampleIndexById(const std::string &sampleId) const
    {
        for (std::size_t index = 0; index < samples_.size(); ++index)
        {
            if (samples_[index].GetSampleId() == sampleId)
            {
                return index;
            }
        }
        return std::nullopt;
    }

    std::optional<std::size_t> FindResultIndexByKey(
        const std::string &laboratoryId,
        const std::string &sampleId,
        int replicateIndex) const
    {
        for (std::size_t index = 0; index < results_.size(); ++index)
        {
            const auto &current = results_[index];
            if (current.GetLaboratoryId() == laboratoryId &&
                current.GetSampleId() == sampleId &&
                current.GetReplicateIndex() == replicateIndex)
            {
                return index;
            }
        }
        return std::nullopt;
    }

    // -------------------------
    // Integrity checks
    // -------------------------
    bool HasResultsForLaboratory(const std::string &laboratoryId) const
    {
        return std::any_of(results_.begin(), results_.end(),
                           [&](const MeasurementResult &r)
                           { return r.GetLaboratoryId() == laboratoryId; });
    }

    bool HasResultsForSample(const std::string &sampleId) const
    {
        return std::any_of(results_.begin(), results_.end(),
                           [&](const MeasurementResult &r)
                           { return r.GetSampleId() == sampleId; });
    }

    bool HasSamplesForMeasurand(const std::string &measurandId) const
    {
        return std::any_of(samples_.begin(), samples_.end(),
                           [&](const Sample &s)
                           { return s.GetMeasurandId() == measurandId; });
    }

    void EnsureLaboratoryExists(const std::string &laboratoryId) const
    {
        if (!FindLaboratoryIndexById(laboratoryId).has_value())
        {
            throw std::invalid_argument("EnsureLaboratoryExists: LaboratoryId not found.");
        }
    }

    void EnsureSampleExists(const std::string &sampleId) const
    {
        if (!FindSampleIndexById(sampleId).has_value())
        {
            throw std::invalid_argument("EnsureSampleExists: SampleId not found.");
        }
    }
};
