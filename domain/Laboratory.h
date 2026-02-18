#pragma once

#include <string>
#include <stdexcept>

#include "StringUtils.h"

class Laboratory
{
public:
    Laboratory(
        std::string laboratoryId,
        std::string laboratoryName = "",
        std::string organization = "",
        std::string location = "",
        std::string contactName = "",
        std::string contactEmail = "")
        : laboratoryId_(StringUtils::TrimCopy(laboratoryId)),
          laboratoryName_(std::move(laboratoryName)),
          organization_(std::move(organization)),
          location_(std::move(location)),
          contactName_(std::move(contactName)),
          contactEmail_(std::move(contactEmail))
    {
        Validate();
    }

    const std::string &GetLaboratoryId() const noexcept
    {
        return laboratoryId_;
    }

    const std::string &GetLaboratoryName() const noexcept
    {
        return laboratoryName_;
    }

    const std::string &GetOrganization() const noexcept
    {
        return organization_;
    }

    const std::string &GetLocation() const noexcept
    {
        return location_;
    }

    const std::string &GetContactName() const noexcept
    {
        return contactName_;
    }

    const std::string &GetContactEmail() const noexcept
    {
        return contactEmail_;
    }

    void SetLaboratoryName(std::string laboratoryName)
    {
        laboratoryName_ = std::move(laboratoryName);
    }

    void SetContactEmail(std::string contactEmail)
    {
        contactEmail_ = std::move(contactEmail);
        Validate();
    }

private:
    std::string laboratoryId_;
    std::string laboratoryName_;
    std::string organization_;
    std::string location_;
    std::string contactName_;
    std::string contactEmail_;

    void Validate() const
    {
        // ID must not be empty
        if (laboratoryId_.empty())
        {
            throw std::invalid_argument("LaboratoryId must not be empty.");
        }

        // If email is provided, it must contain '@'
        if (!contactEmail_.empty() && contactEmail_.find('@') == std::string::npos)
        {
            throw std::invalid_argument("ContactEmail is invalid (missing '@').");
        }
    }
};
