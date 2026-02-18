#include <iostream>

#include "Laboratory.h"
#include "Measurand.h"
#include "Sample.h"
#include "MeasurementResult.h"
#include "Study.h"

int main()
{
    std::cout << "Hello from ILS\n";

    Laboratory lab("LAB_01", "Lab Central");
    Measurand measurand("GLUCOSE", "Glucose", "mg/dL");
    Sample sample("A", measurand.GetMeasurandId());
    MeasurementResult result(lab.GetLaboratoryId(), sample.GetSampleId(), 1, 123.4);

    std::cout << "Lab: " << result.GetLaboratoryId() << "\n";
    std::cout << "Sample: " << result.GetSampleId() << "\n";
    std::cout << "Replicate: " << result.GetReplicateIndex() << "\n";
    std::cout << "Value: " << result.GetValue() << "\n";

    return 0;
}
