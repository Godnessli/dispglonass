#ifndef BUILDREPORTTABLE_H
#define BUILDREPORTTABLE_H

#include "makeact.h"
#include <xlnt/xlnt.hpp>
#include <vector>
#include <string>

class BuildReportTable final : public MakeAct
{
public:
    BuildReportTable(const std::vector<std::vector<std::vector<std::string>>>&, const std::string[]);
};

#endif // BUILDREPORTTABLE_H
